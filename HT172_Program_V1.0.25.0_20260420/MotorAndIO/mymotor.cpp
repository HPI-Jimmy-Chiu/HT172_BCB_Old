//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "MyMotor.h"
#include "maintenance.h"
#include "FileInfo.h"

#include "DecodeXML.h"
#pragma package(smart_init)
#define RESET_TIMES 5000

//---------------------------------------------------------------------------
__fastcall TMyMotor::TMyMotor()
{
    speed=1000;
    iPersentSpeed=1;
    LockList=new TList;
    LockList->Clear();
    SimuCtrlList=new TList;
    SimuCtrlList->Clear();
    MoveCheckCallBack=NULL;
    bShowSimulateCompoment=false;
    bLinearStatus=false;
    HomeOrder=new TStringList;

    TargetPosition=0;
    EncoderPosition=0;
    Position=0;
}
//---------------------------------------------------------------------------
TMyMotor::~TMyMotor()
{
//    delete Motor;
    delete HomeOrder;
}
//---------------------------------------------------------------------------
void TMyMotor::SetHomeOrder(AnsiString OrderString)                    //Steven : 歸零的順序
{
    if(OrderString!="")
    {
        int iFlagPos;
        while(OrderString.AnsiPos("|")!=0)
        {
            iFlagPos=OrderString.AnsiPos("|");
            OrderString.Delete(iFlagPos, 1);
            OrderString.Insert(",", iFlagPos);
        }
        HomeOrder->CommaText=OrderString;
    }
}
//==============================================================================
// simulate compoment position
//==============================================================================
int TMyMotor::GetSpeed()
{
    if(Motor->Enable)
        return Motor->ReadSpeed();
    else
        return speed;
}
//==============================================================================
void TMyMotor::SetSpeed(unsigned int p)
{
//    Sleep(500);
    if(Motor->Enable)
        Motor->SetSpeed(p);
    else
        speed=p;
}
//==============================================================================
// get 1..100 percent speed
//==============================================================================
int TMyMotor::GetPersentSpeed()
{
    return iPersentSpeed;
}
//==============================================================================
int TMyMotor::GetInitSpeed()
{
    return Motor->ReadInitSpeed();
}
//==============================================================================
int TMyMotor::GetHomeHighSpeed()
{
    return Motor->HomeHighSpeed;
}
//==============================================================================
int TMyMotor::GetHomeLowSpeed()
{
    return Motor->HomeLowSpeed;
}
//==============================================================================
double TMyMotor::GetAcc()
{
    return Motor->ReadAcc();
}
//==============================================================================
double TMyMotor::GetDec()
{
    return Motor->ReadDec();
}
//==============================================================================
// get error type of motor
//==============================================================================
int TMyMotor::GetErrorIndex()
{
    //2012-06-11    Dell    Fix
    if( Led[iAlarmLed] && Led[iServoalarmLed] && Led[iInposLed])                // ALARM & SERVOALARM & INP //
        return 0;
    else if(Led[iAlarmLed] && Led[iServoalarmLed])                              // ALARM & SERVOALARM //
        return 1;
    else if(Led[iCwLed] && Led[iAlarmLed])                                      // ALARM & CW //
        return 2;
    else if(Led[iCcwLed] && Led[iAlarmLed])                                     // ALARM & CCW //
        return 3;
    else if(Led[iAlarmLed] && Led[iSoftcwLed])                                  // ALARM & SOFTCW //
        return 4;
    else if(Led[iAlarmLed] && Led[iSoftccwLed])                                 // ALARM & SOFTCCW //
        return 5;
    else if(Led[iAlarmLed])                                                     // ALARM & SOFTCCW //
        return 6;
    else if(Led[iServoalarmLed])                                                // ALARM & SOFTCCW //
        return 8;
    else return 7;
}
//==============================================================================
// all function about safe lock by other motor
//==============================================================================
void TMyMotor::Lock(AnsiString MotorAlias, AnsiString FunctionName, int Task)
{
    int i=-1;
    MyLockList *p;
    for(i=0;i<LockList->Count;i++)
    {
        p=(MyLockList *)LockList->Items[i];
        if(p->MotorAlias==MotorAlias &&
           p->LockFunc==FunctionName)
            return;
    }
    p=new MyLockList;
    p->MotorAlias=MotorAlias;
    p->LockFunc=FunctionName;
    p->LockTask=Task;
    LockList->Add((MyLockList *)p);
}
//==============================================================================
void TMyMotor::UnLock(AnsiString MotorAlias,AnsiString FunctionName)
{
    int i=-1;
    MyLockList *p;
    for(i=0;i<LockList->Count;i++)
    {
        p=(MyLockList *)LockList->Items[i];
        if( p->MotorAlias==MotorAlias &&
            p->LockFunc==FunctionName )
        {
            delete p;
            LockList->Delete(i);
            return;
        }
    }
}
//==============================================================================
void TMyMotor::ClearLock()
{
    int i=-1;
    MyLockList *p;
    for(i=0;i<LockList->Count;i++)
    {
        p=(MyLockList *)LockList->Items[i];
        delete p;
    }
    LockList->Clear();
}
//==============================================================================
int TMyMotor::GetLockCount()
{
    return LockList->Count;
}
//==============================================================================
AnsiString TMyMotor::GetLockString(int Index)
{
    MyLockList *p;
    AnsiString S;
    if( Index<LockList->Count && Index>=0)
    {
        p=(MyLockList *)LockList->Items[Index];
        S=p->MotorAlias+AnsiString("  ")+p->LockFunc+AnsiString("  ")+p->LockTask;
    }
    return S;
}
//==============================================================================
void TMyMotor::InitHomeTask_forSingleAxis()
{
    iHomeTask=1;
    bHomeFinish=false;                                                          //JerryYang 20240610 : Z auto home
}
//==============================================================================
void TMyMotor::InitHomeTask()
{
    iHomeTask=1;
    bHomeFlag=false;
}
//==============================================================================
bool TMyMotor::Home(AnsiString &sErr)
{
    if(Motor->GetMotorKind()==eVoiceCoilMotor)
    {
        return VoiceCoilMotorHome();
    }

    int ret,iRef;//,i;
    //char str[256];
    AnsiString S;
    int &Task=iHomeTask;
    switch(Task)
    {
        case 1:
            if(Motor->Enable==false)
            {
                Motor->SetPosition(0);
                Motor->SetCommand(0);
                bHomeFlag=true;
                bQuickHome=true;
                return true;
            }
            bQuickHome=false;
            InitHomeControlTask();
            Motor->SetSpeed(Motor->HomeHighSpeed);
            sErr="";
            Task=100;
            break;
        case 100:
            ret=HomeControl(0);
            if(Led[iHomeLed]==true)
                bQuickHome=true;
            if(ret==2)
            {
                Stop();
                Motor->SetSpeed(Motor->JogLowSpeed);
                iRef=GetErrorIndex();
                if(iRef==9)
                    iRef=7;
                S.printf("%d%03d%1d",(int)eMotorAlarm,Tag,iRef);                                  // motor error  10000 Start //
                ShowMotorError(S,__FUNC__);
                sErr=S;
               return false;
            }
            else if(ret==3)
            {
                Motor->SetSpeed(Motor->JogLowSpeed);
                iRef=GetErrorIndex();
                if(iRef==9)
                    iRef=6;
                S.printf("%d%03d%1d",(int)eMotorAlarm,Tag,iRef);                                  // motor error  10000 Start //
                ShowMotorError(S,__FUNC__);
                sErr=S;
               return false;
            }
            else if(ret==1)
            {
                ResetTime.Clear();
                ResetTime.SetMS(1000);
                ResetTime.On();
                Task=200;
            }
            break;
        case 200:
            if(ResetTime.Off())
            {
                Motor->SetPosition(0);
                Motor->SetCommand(0);
                ClearLock();
                Motor->SetSpeed(Motor->JogLowSpeed);
                bHomeFlag=true;
                return true;
            }
            break;
    }
    return false;
}
//===========================================================================
bool TMyMotor::VoiceCoilMotorHome()
{
    int &Task=iHomeTask;
    switch(Task)
    {
        case 1:
            Motor->SetCommand(0);
            Sleep(100);
            Motor->SetPosition(0);
            Motor->MotOutputOn(3);
            HomeDelay.SetSecAndOn(0.3);
            Task=10;
            break;
        case 10:
            if(HomeDelay.Off())
            {
                Motor->MotOutputOff(3);
                Task=100;
            }
            break;
        case 100:
            ScanMotorStatus();
            if(Motor->Enable==false || Led[iHomeLed]==true)
            {
                Task=1000;
                LastHomePos=Motor->ReadRealPos();
                #ifdef SOFT_SIMULATE
                    HomeDelay.SetSecAndOn(0.2);
                #else
                    HomeDelay.SetSecAndOn(2);
                #endif
            }
            break;
        case 1000:
            if(HomeDelay.Off())
            {
                Motor->MotOutputOn(3);
                HomeDelay.SetSecAndOn(0.3);
                Task=1100;
            }
            break;
        case 1100:
            if(HomeDelay.Off())
            {
                Motor->MotOutputOff(3);
                Task=1200;
            }
            break;
        case 1200:
            ScanMotorStatus();
            if(Motor->Enable==false || Led[iHomeLed]==true)
            {
                Task=2000;
                #ifdef SOFT_SIMULATE
                    HomeDelay.SetSecAndOn(0.3);
                #else
                    HomeDelay.SetSecAndOn(3);
                #endif
            }
            break;
        case 2000:
            if(HomeDelay.Off())
            {
                Motor->SetCommand(0);
                Sleep(100);
                Motor->SetPosition(0);
                Motor->SetSpeed(OldSpeed);
                Motor->SoftLimitEnable(false);
                bHomeFlag=true;
                Task=1;
                return true;
            }
    }
    return false;
}
//===========================================================================
void TMyMotor::InitHomeControlTask()
{
    iHomeControlTask=1;
    ResetTime.Clear();
    ResetTime.Set(RESET_TIMES);
    ResetTime.On();
}
//==============================================================================
int TMyMotor::HomeControl(bool Flag)
{
    int ret;
    int &Task=iHomeControlTask;
    if( GetAlarm() )
    {
         ScanMotorStatus();
         ret=GetErrorIndex();
         if( ret<2 || ret>5 )
         {
             Task=1;
             return 3;
         }
    }
    switch(Task)
    {
        case 1:
            iHomeRetryCT=3;
            Task=100;
            break;
        case 100:
            Motor->SetHomeobjectTask(1);
            ResetTime.Clear();
            ResetTime.Set(RESET_TIMES);
            ResetTime.On();
            Task=200;
            break;
        case 200:
            if( HomeObject() )
            {
                htWaitHomeSensorOnDelay.Clear();
                htWaitHomeSensorOnDelay.SetMS(5000);
                htWaitHomeSensorOnDelay.On();
                Task=300;
            }
            break;
        case 300:
            if( Motor->HomeFlag() )
            {
                if(fMotorTest->fShow==true ||
                   fTeach->fShow==true)
                {
                    return 1;
                }
                else
                {
                    if( iHomeRetryCT>=3 )
                    {
                        iHomeRetryCT--;
                        Task=100;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else if( htWaitHomeSensorOnDelay.Off() )
            {
                if( iHomeRetryCT>0 )
                {
                    iHomeRetryCT--;
                    Task=100;
                }
                else
                {
                    return 2;
                }
            }
            break;
    }
    if (ResetTime.Off())
        return 2;
    return 0;
}
//==============================================================================
int TMyMotor::MotorMove(int p, int PreDonePos, bool JogP)
{
//範例
// 0    --> 5000,  要在 4000 return true : ( 5000,  1000, true)
// 5000 --> 0,     要在 1000 return true : (    0,  1000, false)
// 0    --> -5000, 要在-4000 return true : (-5000, -1000, false)
//-5000 --> 0,     要在-1000 return true : (    0, -1000, true)

    MotorMoveSub(p,true);
    if(JogP==true && Position>=p-PreDonePos)
        return true;
    else if(JogP==false && Position<=p+PreDonePos)
        return true;
    else
        return false;
}
//==============================================================================
bool TMyMotor::MotorMove(int p)
{
    bool bCheck=MotorMoveSub(p,true);
    return bCheck;
}
//==============================================================================
bool TMyMotor::MotorMoveSKLED(int p)
{
    return MotorMoveSub(p,false);
}
//==============================================================================
bool TMyMotor::MotorMoveSub(int p, bool bCheckLed)
{
    bool ret;
    if(LockList->Count)
    {
        Stop();
        if(p==ReadPos())
            return true;
        else
            return false;
    }

    if(MoveCheckCallBack!=NULL)
    {
        if(MoveCheckCallBack()==false)
            return false;
    }

    if(CheckSoftLimit(p)==false)
    {
        return false;
    }

    if(Motor->Enable)
    {
        if(fTeach->CheckCanMove(Tag)==false)                                    //KenHsieh 20240904 : add 馬達安全檢查
            return false;

        ret=MotorMovePosition(Position, GetSpeed(), p);
        if(ret==true)
        {
            if(bCheckLed)
            {
                ScanMotorStatus();
                if(Led[iEmgLed])
                {
                    ret=false;
                }

                if(Led[iInposLed])
                {
                    ret=false;
                }
            }
        }
    }
    else
    {
        ret=SimulateMotorMovePosition(Position, speed, p);
    }
    UpdateSimulateCompomentPosition();
    return ret;
}
//==============================================================================
// check target position over or below soft limit
//==============================================================================
bool TMyMotor::CheckSoftLimit(int Tar,bool bAlarm)
{
    AnsiString S;
    bErrorMove=false;
    if(Tar>=Motor->SoftLimitP)
    {
        if(bAlarm)
            ShowSystemError(AlarmName[eMotSoftPErr], K_RETRY);
        bErrorMove=true;
        return false;
    }
    if(Tar<=Motor->SoftLimitN)
    {
        if(bAlarm)
            ShowSystemError(AlarmName[eMotSoftNErr], K_RETRY);
        bErrorMove=true;
        return false;
    }
    return true;
}
//==============================================================================
// move reference
//==============================================================================
bool TMyMotor::MotorMovePosition(int &Position, int iSpeed, int Tar)
{
    int ret;
    AnsiString S;
    TargetPosition=Tar;
    int iGap=2;                                                                 //Sam 20230621 : Gap容許誤差改為1>2 //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條

    Position=ReadPos();
    ScanMotorStatus();
    #ifndef USE_CompareCommandPos
    if(Tar==Position)
    #else
    if(CompareCommandPos(Tar, iGap)==1)  //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
    #endif
    {
        if(Led[iEmgLed]==false)
            return true;
    }
    if((Led[iAlarmLed] && GetMotorKind()!=eStepServo) || Led[iServoalarmLed])
    {
        ret=GetErrorIndex();
        S.printf("%d%03d%1d",(int)eMotorAlarm,Tag,ret);                                           // motor error  10000 Start //
        ShowMotorError(S,__FUNC__);
        bHomeFlag=false;
        return false;
    }

    if(HSys.FuncS.bS03_ShortDistanceMove &&                                     //Sam 20240727 : 新增短距離加減速減半
      (Alias=="SortArm1X" ||
       Alias=="SortArm2X"))
    {
        if(MoveToPosShortDistance(Tar))
        {
            Position=Tar;
            ScanMotorStatus();
            return !Led[iEmgLed];
        }
    }
    else
    {
        if(MoveTo(Tar))
        {
            Position=Tar;
            ScanMotorStatus();
            return !Led[iEmgLed];
        }
    }
    return false;
}
//==============================================================================
bool TMyMotor::SimulateMotorMovePosition(int &Position,int iSpeed,int Tar)
{
    TargetPosition=Tar;
    if(Position==Tar)
    {
        return true;
    }
    else
    {
        if(Position>Tar)
        {
            Position-=speed;
            if(Position<=Tar)
            {
                Position=Tar;
                return true;
            }
        }
        else
        {
            Position+=speed;
            if(Position>=Tar)
            {
                Position=Tar;
                return true;
            }
        }
    }
    return false;
}
//==============================================================================
// enable or disable simulate compoment refresh position
//==============================================================================
void TMyMotor::SetShowSimulateCompomentFlag(bool flag)
{
    bShowSimulateCompoment=flag;
}

//==============================================================================
// set which vcl compoment to simulate motion of motor
//==============================================================================
//void TMyMotor::SetSimulateCompoment(TObject *PCtrl,int StartPos,int EndPos,int Left,int Top,int Right,int Bottom)
//{
//    int i;
//    MyMotorSimulateList *TempP,*P,*p3;
//
//    TWinControl *PTempWinCtrl;
//    PTempWinCtrl=dynamic_cast<TWinControl *> (PCtrl);    // new code ,old is TWinctontrol,
//    if( PTempWinCtrl!=NULL)
//    {
//        PTempWinCtrl->DoubleBuffered=true;
//    }
//
//    TControl *PWinCtrl;
//    PWinCtrl=dynamic_cast<TControl *> (PCtrl);    // new code ,old is TWinctontrol,
//    if( PWinCtrl!=NULL )                          // Left,Top are Properties of TControl
//    {
//        for(i=0;i<SimuCtrlList->Count;i++)
//        {
//            TempP=(MyMotorSimulateList *)SimuCtrlList->Items[i];
//            if(TempP->PWinCtrl==PWinCtrl)
//            {
//                TempP->FactStart=StartPos;
//                TempP->FactEnd=EndPos;
//                if( Left==Right )                                               // move up down //
//                {
//                    TempP->RefStart=Top;
//                    TempP->RefEnd=Bottom;
//                    TempP->bUpDownMove=true;
//                }
//                else                                                            // move left right //
//                {
//                    TempP->RefStart=Left;
//                    TempP->RefEnd=Right;
//                    TempP->bUpDownMove=false;
//                }
//                TempP->Scale=GetScale(TempP->RefStart,TempP->RefEnd,TempP->FactStart,TempP->FactEnd);
//                return;
//            }
//        }
//        P=new MyMotorSimulateList;
//        P->PWinCtrl=PWinCtrl;
//        P->FactStart=StartPos;
//        P->FactEnd=EndPos;
//        if( Left==Right )
//        {
//            P->RefStart=Top;
//            P->RefEnd=Bottom;
//            P->bUpDownMove=true;
//        }
//        else
//        {
//            P->RefStart=Left;
//            P->RefEnd=Right;
//            P->bUpDownMove=false;
//        }
//        P->Scale=GetScale(P->RefStart,P->RefEnd,P->FactStart,P->FactEnd);
//        SimuCtrlList->Add( (MyMotorSimulateList *)P );
//    }
//}
//==============================================================================
void TMyMotor::SetSimulateCompoment(TObject *PCtrl, TAnchorKind Alignment, int StartPos, int EndPos, int simuStartPos, int simuEndPos)
{
    int i;
    MyMotorSimulateList *TempP,*P;//,*p3;

    TWinControl *PTempWinCtrl;
    PTempWinCtrl=dynamic_cast<TWinControl *> (PCtrl);    // new code ,old is TWinctontrol,
    if( PTempWinCtrl!=NULL)
    {
        PTempWinCtrl->DoubleBuffered=true;
    }

    TControl *PWinCtrl;
    PWinCtrl=dynamic_cast<TControl *> (PCtrl);    // new code ,old is TWinctontrol,
    if( PWinCtrl!=NULL )                          // Left,Top are Properties of TControl
    {
        for(i=0;i<SimuCtrlList->Count;i++)
        {
            TempP=(MyMotorSimulateList *)SimuCtrlList->Items[i];
            if(TempP->PWinCtrl==PWinCtrl)
            {
                TempP->FactStart=StartPos;
                TempP->FactEnd=EndPos;
                if(Alignment==akLeft || Alignment==akRight)                                               // move up down //
                {
                    TempP->RefStart=simuStartPos;
                    TempP->RefEnd=simuEndPos;
                    TempP->bUpDownMove=true;
                }
                else                                                            // move left right //
                {
                    TempP->RefStart=simuStartPos;
                    TempP->RefEnd=simuEndPos;
                    TempP->bUpDownMove=false;
                }
                TempP->Scale=GetScale(TempP->RefStart,TempP->RefEnd,TempP->FactStart,TempP->FactEnd);
                return;
            }
        }
        P=new MyMotorSimulateList;
        P->PWinCtrl=PWinCtrl;
        P->FactStart=StartPos;
        P->FactEnd=EndPos;
        if(Alignment==akLeft || Alignment==akRight)
        {
            P->RefStart=simuStartPos;
            P->RefEnd=simuEndPos;
            P->bUpDownMove=true;
        }
        else
        {
            P->RefStart=simuStartPos;
            P->RefEnd=simuEndPos;
            P->bUpDownMove=false;
        }
        P->Scale=GetScale(P->RefStart,P->RefEnd,P->FactStart,P->FactEnd);
        SimuCtrlList->Add( (MyMotorSimulateList *)P );
    }
}
//==============================================================================
void TMyMotor::UpdateSimulateCompomentPosition()
{
    int i,ScreenPos;
    if( bShowSimulateCompoment==false)
        return;
    MyMotorSimulateList *P;
    for(i=0;i<SimuCtrlList->Count;i++)
    {
        P=(MyMotorSimulateList *)SimuCtrlList->Items[i];
        ScreenPos= (int)(P->Scale*(Position-P->FactStart))+P->RefStart;
        if( P->bUpDownMove )
            P->PWinCtrl->Top=ScreenPos;
        else
            P->PWinCtrl->Left=ScreenPos;
    }
}
//==============================================================================
bool TMyMotor::JogP()
{
    return Motor->JogP();
}
//==============================================================================
bool TMyMotor::JogN()
{
    return Motor->JogN();
}
//==============================================================================
void TMyMotor::Stop()
{
    bLinearStatus=false;
    return Motor->Stop();
}
//==============================================================================
void TMyMotor::DecStop()
{
    bLinearStatus=false;
    return Motor->DecStop();
}
//==============================================================================
int TMyMotor::ReadPos()
{
    if(Motor!=NULL &&
       Motor->Enable)
    {
        Position=Motor->ReadPos();
        EncoderPosition=Motor->ReadEncoderPos();
    }
    else
    {
        EncoderPosition=Position;
    }
    UpdateSimulateCompomentPosition();
    return Position;
}
//------------------------------------------------------------------------------
int TMyMotor::ReadEncoderPos()
{
    if(Motor!=NULL &&
       Motor->Enable)
    {
        Position=Motor->ReadPos();
        EncoderPosition=Motor->ReadEncoderPos();
    }
    else
    {
        EncoderPosition=Position;
    }
    return EncoderPosition;
}
//==============================================================================
void TMyMotor::ScanMotorStatus()
{
    if(Motor!=NULL &&
       Motor->Enable)
        Motor->ScanMotorStatus(Led);
}
//==============================================================================
bool TMyMotor::MoveTo(int Tar)
{
    return Motor->MoveTo(Tar);
}
//==============================================================================
bool TMyMotor::MoveToPosShortDistance(int Tar)                                  //Sam 20240727 : 新增短距離加減速減半
{
    return Motor->MoveToPosShortDistance(Tar);
}
//==============================================================================
bool TMyMotor::HomeObject(void)
{
    return Motor->HomeObject();
}
//==============================================================================
bool TMyMotor::GetAlarm(void)
{
    return Motor->GetAlarm();
}
//==============================================================================
void TMyMotor::SetPersentSpeed(int persent, bool bSave)
{
    if( persent>100)
        persent=100;
    else if(persent<0)
        persent=1;
    int s;

    if(bSave)
        iPersentSpeed=persent;
    if(Motor->Enable)
    {
        s=Motor->JogHighSpeed*persent/100;
        SetSpeed(s);
    }
    else
    {
        speed=SimulateSpeed*persent/100;
    }
}
//==============================================================================
void TMyMotor::SetServoAlarmOn(bool Value)
{
    Motor->SetServoAlarmOn(Value);
}
//==============================================================================
void TMyMotor::SetInitSpeed(unsigned int x)
{
    Motor->SetInitSpeed(x);
}
//==============================================================================
bool TMyMotor::ReadServoAlarmOn()
{
    return Motor->ReadServoAlarmOn();
}
//==============================================================================
void TMyMotor::SetRange(unsigned int a)
{
    if(a<=0)
        return;
    Motor->SetRange(a);
}
//==============================================================================
int TMyMotor::GetRange()
{
    return Motor->ReadRange();
}
//==============================================================================
void TMyMotor::SetRate(unsigned int a)
{
    Motor->SetRate(a);
}
//==============================================================================
int TMyMotor::GetRate()
{
    return Motor->ReadRate();
}
//==============================================================================
void TMyMotor::SetAcc(double a)
{
    Motor->SetAcc(a);
}
//==============================================================================
void TMyMotor::SetDec(double a)
{
    Motor->SetDec(a);
}
//==============================================================================
void TMyMotor::SetDirection(bool Value)
{
    Motor->Direction=Value;
}
//==============================================================================
void TMyMotor::SetHomeDirection(bool Value)
{
    Motor->HomeDirection=Value;
}
//==============================================================================
void TMyMotor::SetGearRatio(double a)
{
    if(a==0)
    {
        Motor->GearRatio=1;
        return;
    }
    Motor->GearRatio=a;
}
//==============================================================================
void TMyMotor::SetHomeHighSpeed(unsigned int a)
{
    Motor->HomeHighSpeed=a;
}
//==============================================================================

void TMyMotor::SetHomeLowSpeed(unsigned int a)
{
    Motor->HomeLowSpeed=a;
}
//==============================================================================
void TMyMotor::SetJogHighSpeed(unsigned int a)
{
    Motor->JogHighSpeed=a;
}
//==============================================================================
void TMyMotor::SetJogLowSpeed(unsigned int a)
{
    Motor->JogLowSpeed=a;
}
//==============================================================================
void TMyMotor::SetSoftLimitN(int a)
{
    Motor->SoftLimitN=a;
}
//==============================================================================
void TMyMotor::SetSoftLimitP(int a)
{
    Motor->SoftLimitP=a;
}
//==============================================================================
void TMyMotor::SetMotorType(bool Value)
{
    Motor->MotorType=Value;
}
//==============================================================================
void TMyMotor::SetSensorType(bool Value)
{
    Motor->bSensorType=Value;
}
//==============================================================================
void TMyMotor::SetEnable(bool Value)
{
    Motor->Enable=Value;
}
//==============================================================================
bool TMyMotor::GetEnable()
{
    if(Motor==NULL)
        return false;
    return Motor->Enable;
}
//==============================================================================
void TMyMotor::InitMotor(int IoAddress)
{
    Motor->InitMotor(IoAddress);
}
//==============================================================================
int  TMyMotor::GetAddress()
{
    return Motor->Address;
}
//==============================================================================
int  TMyMotor::GetJogHighSpeed()
{
    return Motor->JogHighSpeed;
}
//==============================================================================
int  TMyMotor::GetJogLowSpeed()
{
    return Motor->JogLowSpeed;
}
//==============================================================================
void TMyMotor::SetMotorKind(eMotorKind Kind)
{
    Motor->SetMotorKind(Kind);
}
//==============================================================================
eMotorKind TMyMotor::GetMotorKind()
{
    return Motor->GetMotorKind();
}
//==============================================================================
void TMyMotor::SetMotionCardType(eMotionCardType Type)
{
    Motor->SetMotionCardType(Type);
}
//==============================================================================
eMotionCardType TMyMotor::GetMotionCardType()
{
    return Motor->GetMotionCardType();
}
//==============================================================================
int  TMyMotor::GetLastHomePos()
{
    return Motor->LastHomePos;
}
//==============================================================================
double TMyMotor::GetGearRatio()
{
    return Motor->GearRatio;
}
//==============================================================================
int  TMyMotor::GetSoftLimitP()
{
    return Motor->SoftLimitP;
}
//==============================================================================
int  TMyMotor::GetSoftLimitN()
{
    return Motor->SoftLimitN;
}
//==============================================================================
void TMyMotor::InitialMotorObject(int addr)
{
//    #ifdef SOFT_SIMULATE      //Steven : 暫時馬克掉,記得改回來
//        Motor=new HTMotor();
//    #else
        if(CardModel=="SMC")
            Motor=new TMySMCMotor(addr);
        else if(CardModel=="MN200")
            Motor=new TMyMN200Motor(addr);
        else
            Motor=new HTMotor();
//    #endif
}
//==============================================================================
//void TMyMotor::ServoOnOff(bool bStatus)
//{
//    Motor->ServoOnOff(bStatus);
//}
//==============================================================================
void TMyMotor::MotInputStatus(bool *bInputPort)                                 //讀DI訊號//
{
    Motor->MotInputStatus(bInputPort);
}
//==============================================================================
void TMyMotor::MotOutputOn(int iOutPort)
{
    Motor->MotOutputOn(iOutPort);
}
//==============================================================================
void TMyMotor::MotOutputOff(int iOutPort)
{
    Motor->MotOutputOn(iOutPort);
}
//==============================================================================
bool TMyMotor::ResetPos(int p)
{
    return Motor->ResetPos(p);
}
//==============================================================================
void TMyMotor::SoftLimitEnable(bool bFlag)
{
    Motor->SoftLimitEnable(bFlag);
}
//==============================================================================
bool TMyMotor::GetDirection()
{
    return Motor->Direction;
}
//==============================================================================
bool TMyMotor::LinearAxisMoveTo(TMyMotor* LineMotPtr[8], long lPos[8])
{
    int iLinePortID[8];
    if(bLinearStatus==false)
    {
        for(int i=0; i<8; i++)
        {
            if(LineMotPtr[i]!=NULL && Motor->iPortID==LineMotPtr[i]->Motor->iPortID)
                break;
            else if(i==7)
                return false;
        }

        for(int i=0; i<8; i++)
        {
            if(LineMotPtr[i]!=NULL)
            {
                if(Motor->iBoardID==LineMotPtr[i]->Motor->iBoardID)
                {
                    iLinePortID[i]=LineMotPtr[i]->Motor->iPortID;
                    lPos[i]=lPos[i]-LineMotPtr[i]->ReadPos();
                    if(lPos[i]>0)
                        lPos[i]=(lPos[i]/LineMotPtr[i]->GetGearRatio())+0.5;
                    else if(lPos[i]<0)
                        lPos[i]=(lPos[i]/LineMotPtr[i]->GetGearRatio())-0.5;
                    else
                        lPos[i]=0;
                    if(LineMotPtr[i]->GetDirection())
                        lPos[i]=-lPos[i];
                    else
                        lPos[i]=lPos[i];
                }
                else
                {
                    return false;
                }
            }
            else
            {
                iLinePortID[i]=-1;
            }
        }
        Motor->LinearAxisMoveTo(iLinePortID,lPos,bLinearStatus);
        bLinearStatus=true;
    }
    else
    {
        if(Motor->LinearAxisMoveTo(iLinePortID,lPos,bLinearStatus))
        {
            bLinearStatus=false;
            return true;
        }
    }
    return false;

}
//==============================================================================
// all function about tray under motor
//==============================================================================
__fastcall TTrayMotor::TTrayMotor()
{
    fPanel=false;
    fHTary=false;
    fHasTray=false;
    fSubHTary=false;
    fPanelID=false;
    Tray.SetTrayXYItem(0, 0);                                                   //Steven 20240707 : 修正顯示
    ClearTray();
}
TTrayMotor::~TTrayMotor()
{
}
//==============================================================================
bool TTrayMotor::HasRing()
{
    if(fHasTray==false)
        return false;
    if(Tray.HasRing())
        return true;
    return false;
}
//==============================================================================
bool TTrayMotor::HasIC()
{
    if(fHasTray==false)
        return false;
    if(Tray.HasIC())
        return true;
    return false;
}
//==============================================================================
bool TTrayMotor::HasRealIC()
{
    if(fHasTray==false)
        return false;
    if(Tray.HasRealIC())
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool TTrayMotor::RowYFullIC(int Y)
{
    if(fHasTray==false)
        return false;
    return Tray.RowYFullIC(Y);
}
//------------------------------------------------------------------------------
void TTrayMotor::SetPTrayData(int x, int y, int iBin)
{
    if(iBin>0)
    {
        if(fHTary)
        {
            pHTray->SetCellNumber(x, y, iBin);
        }

        if(fSubHTary)
        {
            pSubHTray->SetCellNumber(x, y, iBin);
        }
    }
    else
    {
        if(fHTary)
        {
            pHTray->SetCellNumber(x, y, "");
        }

        if(fSubHTary)
        {
            pSubHTray->SetCellNumber(x, y, "");
        }
    }
}
//------------------------------------------------------------------------------
void  TTrayMotor::SetTraySingleData(int x, int y, int data)
{
    Tray.DeviceInfo[x][y].Clear();
    Tray.SetData(x, y, data);
    if(fHTary)
    {
        if(data>=10)
        {
            if(data==TEST_PASS)
                pHTray->SetCellColorIndex(x, y, 3);
            else if(data==TEST_FAIL1)
               pHTray->SetCellColorIndex(x, y, 4);
        }
        else
        {
            pHTray->SetCellColorIndex(x, y, data);
        }
    }

    if(fSubHTary)
    {
        if(data>=10)
        {
            if(data==TEST_PASS)
                pSubHTray->SetCellColorIndex(x, y, 3);
            else if(data==TEST_FAIL1)
               pSubHTray->SetCellColorIndex(x, y, 4);
        }
        else
        {
            pSubHTray->SetCellColorIndex(x, y, data);
        }
    }
}
//------------------------------------------------------------------------------
void TTrayMotor::SetTraySingleData(int x, int y, int data, int iBin)
{
    SetTraySingleData(x, y, data);
    Tray.DeviceInfo[x][y].iBin=iBin;
    SetPTrayData(x, y, iBin);
}
//------------------------------------------------------------------------------
void TTrayMotor::SetTraySingleData(int x, int y, int data, TDeviceInfo DInfo)
{
    SetTraySingleData(x, y, data);
    Tray.DeviceInfo[x][y].CopyFrom(DInfo);
    int iBin=Tray.DeviceInfo[x][y].iBin;
    SetPTrayData(x, y, iBin);
}
//------------------------------------------------------------------------------
void TTrayMotor::Refresh()
{
    if(fHTary)
    {
        if(pHTray->XItem!=Tray.XItem)                                           //Steven 20240707 : 修正顯示
            pHTray->XItem=Tray.XItem;

        if(pHTray->YItem!=Tray.YItem)
            pHTray->YItem=Tray.YItem;

        for(int x=0; x<Tray.XItem; x++)
            for(int y=0; y<Tray.YItem; y++)
                pHTray->SetCellColorIndex(x, y, Tray.Data[x][y]);
    }

    if(fSubHTary)
    {
        if(pSubHTray->XItem!=Tray.XItem)                                        //Steven 20240707 : 修正顯示
            pSubHTray->XItem=Tray.XItem;

        if(pSubHTray->YItem!=Tray.YItem)
            pSubHTray->YItem=Tray.YItem;

        for(int x=0; x<Tray.XItem; x++)
            for(int y=0; y<Tray.YItem; y++)
                pSubHTray->SetCellColorIndex(x, y, Tray.Data[x][y]);
    }
}
//==============================================================================
void TTrayMotor::InitNewTray(int data)
{
    fHasTray=true;
    Tray.SetData(data);
    if(fHTary)
    {
        pHTray->XItem=Tray.XItem;
        pHTray->YItem=Tray.YItem;
    }

    if(fSubHTary)
    {
        pSubHTray->XItem=Tray.XItem;
        pSubHTray->YItem=Tray.YItem;
    }

    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=0; j<Tray.YItem; j++)
        {
            if(fHTary)
            {
                if(data>=10)
                {
                    if(data==TEST_PASS)
                    {
                        pHTray->SetCellColorIndex(i, j, 3);
                    }
                    else if(data==TEST_FAIL1)
                    {
                        pHTray->SetCellColorIndex(i, j, 4);
                    }
                }
                else
                {
                    pHTray->SetCellColorIndex(i, j, data);
                }
            }

            if(fSubHTary)
            {
                if(data>=10)
                {
                    if(data==TEST_PASS)
                        pSubHTray->SetCellColorIndex(i, j, 3);
                    else if(data==TEST_FAIL1)
                        pSubHTray->SetCellColorIndex(i, j, 4);
                }
                else
                {
                    pSubHTray->SetCellColorIndex(i, j, data);
                }
            }

            Tray.DeviceInfo[i][j].Clear();
        }
    }
}
//==============================================================================
void TTrayMotor::InitEmptyTray()
{
    Tray.ClearData();
    fHasTray=true;
    if(fHTary)
    {
        pHTray->XItem=Tray.XItem;
        pHTray->YItem=Tray.YItem;
    }

    if(fSubHTary)
    {
        pSubHTray->XItem=Tray.XItem;
        pSubHTray->YItem=Tray.YItem;
    }

    for(int i=0; i<Tray.XItem; i++)
    {
        for(int j=0; j<Tray.YItem; j++)
        {
            if(fHTary)
                pHTray->SetCellColorIndex(i, j, NULL_IC);
            if(fSubHTary)
                pSubHTray->SetCellColorIndex(i, j, NULL_IC);
            Tray.DeviceInfo[i][j].Clear();
        }
    }
}
//==============================================================================
void  TTrayMotor::SetIDPanel(TPanel *ptr)
{
    fPanelID    =true;
    pPalTrayID  =ptr;
}
//==============================================================================
void  TTrayMotor::SetTrayPanel(TPanel *ptr)
{
    fPanel=true;
    pPanel=ptr;
}
//==============================================================================
void TTrayMotor::SetHTrayPanel(TTMyTray *ptr)                                   // tray color need edit by user //
{
    fHTary=true;
    pHTray=ptr;
    pHTray->SetColorMap(NULL_IC,            clWhite);
    pHTray->SetColorMap(HAS_IC,             clGreen);
    pHTray->SetColorMap(HAS_NULL_IC,        clYellow);
    pHTray->SetColorMap(HAS_PASS_IC,        clLime);
    pHTray->SetColorMap(HAS_RING_HAS_IC,    clGreen);
    pHTray->SetColorMap(HAS_RING_NO_IC,     clYellow);
    pHTray->SetColorMap(HAS_FAIL_IC,        clRed);
    pHTray->SetColorMap(7 , clBlue);
    pHTray->SetColorMap(8 , (TColor)0x00FF80FF);
    pHTray->SetColorMap(9 , (TColor)0x00FF0080);
    pHTray->SetColorMap(10, clBtnFace);
}
//==============================================================================
void TTrayMotor::SetSubHTrayPanel(TTMyTray *ptr)                                // tray color need edit by user //
{
    fSubHTary=true;
    pSubHTray=ptr;
    pSubHTray->SetColorMap(NULL_IC,            clWhite);
    pSubHTray->SetColorMap(HAS_IC,             clGreen);
    pSubHTray->SetColorMap(HAS_NULL_IC,        clYellow);
    pSubHTray->SetColorMap(HAS_PASS_IC,        clLime);
    pSubHTray->SetColorMap(HAS_RING_HAS_IC,    clGreen);
    pSubHTray->SetColorMap(HAS_RING_NO_IC,     clYellow);
    pSubHTray->SetColorMap(HAS_FAIL_IC,        clRed);
    pSubHTray->SetColorMap(7 , clBlue);
    pSubHTray->SetColorMap(8 , (TColor)0x00FF80FF);
    pSubHTray->SetColorMap(9 , (TColor)0x00FF0080);
    pSubHTray->SetColorMap(10, clBtnFace);
}
//==============================================================================
void TTrayMotor::SetTray(int data, bool bWithCover)
{
    fHasTray=true;
    bHasCover=bWithCover;
    if(fHTary)
    {
        fHasTray=true;
        InitNewTray(data);
    }
}
//==============================================================================
void TTrayMotor::SetTrayVisible(bool bVisible)
{
    if(fHTary)
        pHTray->Visible=bVisible;

    if(fSubHTary)
        pSubHTray->Visible=bVisible;

    if(fPanel)
        pPanel->Visible=bVisible;
}
//==============================================================================
void TTrayMotor::SetTrayXYItem(int iXItem, int iYItem)
{
    Tray.SetTrayXYItem(iXItem, iYItem);
    Refresh();
}
//==============================================================================
void TTrayMotor::SetHTrayParameter(int iWidth, int iHeight, int iTop, int iLeft)
{
    if(fHTary)
    {
        pHTray->Width=iWidth;
        pHTray->Height=iHeight;
        pHTray->Top=iTop;
        pHTray->Left=iLeft;
    }
}
//==============================================================================
void TTrayMotor::SetSubHTrayParameter(int iWidth, int iHeight, int iTop, int iLeft)
{
    if(fSubHTary)
    {
        pSubHTray->Width=iWidth;
        pSubHTray->Height=iHeight;
        pSubHTray->Top=iTop;
        pSubHTray->Left=iLeft;
    }
}
//==============================================================================
void TTrayMotor::SetTrayID(AnsiString ID)
{
    Tray.ClipID=ID;
    if(fPanelID)
    {
        pPalTrayID->Caption=ID;
    }
}
//==============================================================================
void TTrayMotor::ClearTray()
{
    fHasTray=false;
    Tray.ClearData();
    if(fHTary)
    {
        pHTray->XItem=0;
        pHTray->YItem=0;
        pHTray->SetCellColorIndex(0, 0, NULL_IC);
    }

    if(fSubHTary)
    {
        pSubHTray->XItem=0;
        pSubHTray->YItem=0;
        pSubHTray->SetCellColorIndex(0, 0, NULL_IC);
    }
    SetTrayID("");
    SetTrayVisible(false);
}
//==============================================================================
bool TTrayMotor::FullIC()
{
    if(fHasTray==false)
        return false;
    return Tray.FullIC();
}
//==============================================================================
bool TTrayMotor::FullThisIC(int data)
{
    if(fHasTray==false)
        return false;
    for(int x=0; x<Tray.XItem; x++)
    {
        for(int y=0; y<Tray.YItem; y++)
        {
            if(Tray.Data[x][y]!=data)
            {
                return false;
            }
        }
    }
    return true;
}
//==============================================================================
bool TTrayMotor::HasThisIC(int data)
{
    if(fHasTray==false)
        return false;
    for(int x=0; x<Tray.XItem; x++)
    {
        for(int y=0; y<Tray.YItem; y++)
        {
            if(Tray.Data[x][y]==data)
            {
                return true;
            }
        }
    }
    return false;
}
//==============================================================================
void  TTrayMotor::SetTrayInfo(int iRow, int iCol)
{
      MyBinToTrayStruct mBtoT;
      mBtoT.ConvertRowColToBin(iRow, iCol);

      Tray.iTrackCol=iCol;
      Tray.iTrackRow=iRow;
      Tray.iBin     =mBtoT.iBin;

      Tray.sStartTime=FormatDateTime("yyyymmddhhnnss", Now());
}
//==============================================================================
void TTrayMotor::CopyTrayFrom(int Index)
{
    CopyTrayFrom(HSys.VMotPtr[Index]);
}
//==============================================================================
void TTrayMotor::MoveTrayFrom(int Index)
{
    MoveTrayFrom(HSys.VMotPtr[Index]);
}
//==============================================================================
void TTrayMotor::CopyTrayFrom(TTrayMotor *MotPtr)                               //V1.1
{
    InitNewTray(NULL_IC);
    fHasTray=true;
    Tray.CopyFromMyTray(MotPtr->Tray);
    SetTrayID(MotPtr->Tray.ClipID);
    Tray.CaculateCount();
    for(int x=0; x<Tray.XItem; x++)
    {
        for(int y=0; y<Tray.YItem; y++)
        {
            SetTraySingleData(x, y, MotPtr->Tray.Data[x][y], MotPtr->Tray.DeviceInfo[x][y]);
        }
    }
    Tray.CaculateCount();
}
//==============================================================================
void TTrayMotor::MoveTrayFrom(TTrayMotor *MotPtr)                               //V1.1
{
    CopyTrayFrom(MotPtr);
    MotPtr->ClearTray();
    SetTrayVisible(true);
}
//==============================================================================
void TMyMotor::SetEncoderType(int a)
{
    Motor->EncoderType=a;
}
//------------------------------------------------------------------------------
void TMyMotor::SetEncoderToCommand()
{
    ReadPos();
    Motor->SetCommand(EncoderPosition);
}
//------------------------------------------------------------------------------
void TMyMotor::ServoOnOff(bool IsOn)
{
    if(Motor->GetMotionCardType()==eSMC || Motor->GetMotionCardType()==eMN200)
    {
        if(Motor->Enable)
        {
            Motor->SetServoOn(IsOn);
            if(Motor->ServoAlarmOn)
            {
                if(IsOn)
                {
                    ::Sleep(200);                                               //Steven 20110810 : ServonOn後要Delay一下
                    ServoOnResetPos();                                          //Steven 20110709 : 伺服馬達Servo On之後,要重置CommandPos
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void TMyMotor::ServoOnResetPos()                                                //Steven 20110628 : Servo On後,重設Command Pos
{
    if(Motor->GetMotionCardType()==eSMC || Motor->GetMotionCardType()==eMN200)
    {
        if(Motor->Enable==false)
            return;
        int p=Motor->ReadEnCoderRealPos();
        if(Motor->Direction)
            p=-p;
        Motor->ResetPos(p);
    }
}
//------------------------------------------------------------------------------
void TMyMotor::EnableTrigger(int iFalg, int iMode, long lValue)
{
    Motor->EnableTrigger(iFalg, iMode, lValue);
}
//------------------------------------------------------------------------------
void TMyMotor::ManualTestTrigger(bool bOn)
{
    Motor->ManualTestTrigger(bOn);
}
//------------------------------------------------------------------------------
void TMyMotor::SetLimitLogic(bool logic)
{
    Motor->bLimitLogic=logic;
}
//------------------------------------------------------------------------------
void TMyMotor::SetIn1Logic(bool logic)
{
    Motor->bIn1Logic=logic;
}
//------------------------------------------------------------------------------
void TMyMotor::ClearPosition(int cmd)                                           //清空Command & Encoder Pos
{
    Motor->SetPosition(cmd);
    Motor->SetCommand(cmd);
}
//------------------------------------------------------------------------------
int TMyMotor::CompareCommandPos(int iPos, int iGap)                             //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
{
    int iCommandPos=ReadPos();
    if(Motor->GearRatio<=1)   //完全相等    //Ifor 20221117 0 => 1
        iGap=1;

    if(iPos-iGap<=iCommandPos && iCommandPos<=iPos+iGap)                        //Sam 20230621 : 改 CompareCommandPos 判斷式
        return 1;
    else
        return -1;
}
//==============================================================================
bool TTrayMotor::Find1stIC(int &iRow,int &iCol)
{
    for(int x=0; x<Tray.XItem; x++)
    {
        for(int y=0; y<Tray.YItem; y++)
        {
            if(Tray.Data[x][y]==HAS_IC && Tray.DeviceInfo[x][y].iBin>0)
            {
                iRow=y;
                iCol=x;
                return true;
            }
        }
    }
    return false;
}
//==============================================================================
void TTrayMotor::SaveWinstekLog()
{
    if(CUSTOMER_CODE!=CC_WINSTEK)
        return;

    AnsiString Str="", Str2="", PathName2="", sEndTime="", asPath="", strPath="";
    bool bResult=false;
    sEndTime=FormatDateTime("yyyymmddhhnnss", Now());
    TStringList *List=new TStringList();
    List->Clear();
    IterTrayMapList=mapTrayMapList.end();
    int iICRow=0,iICCol=0;
    if(Find1stIC(iICRow,iICCol))
    {
        IterTrayMapList=mapTrayMapList.find(Tray.DeviceInfo[iICCol][iICRow].InputClipID);
    }
    if(IterTrayMapList!=mapTrayMapList.end())
    {
        TTrayMap trayMap=IterTrayMapList->second;
        Str.sprintf("#DEVICE_NAME %s", trayMap.DEVICE_NAME);
        List->Add(Str);
        Str.sprintf("#TEST_PROGRAM %s", trayMap.TEST_PROGRAM);
        List->Add(Str);
        Str.sprintf("#TESTER_ID %s", trayMap.TESTER_ID);
        List->Add(Str);
        Str.sprintf("#HANDLER_ID %s", trayMap.HANDLER_ID);
        List->Add(Str);
        Str.sprintf("#LOADBOARD_ID %s", trayMap.LOADBOARD_ID);
        List->Add(Str);
        Str.sprintf("#LOT_ID %s", trayMap.LOT_ID);
        List->Add(Str);
        Str.sprintf("#RUN_CARD_ID %s", trayMap.RUN_CARD_ID);
        List->Add(Str);
        Str.sprintf("#STAGE %s", trayMap.STAGE);
        List->Add(Str);
//        Str.sprintf("#SUB_STAGE %s", trayMap.SUB_STAGE);
//        List->Add(Str);
        Str.sprintf("#ATTRIBUTE %s", trayMap.ATTRIBUTE);
        List->Add(Str);
        Str.sprintf("#TRAY_ID %s", Tray.ClipID);
        List->Add(Str);
        Str.sprintf("#START_TIME %s", trayMap.START_TIME);
        List->Add(Str);
        Str.sprintf("#END_TIME %s", trayMap.END_TIME);
        List->Add(Str);
    }
    for(int x=0; x<Tray.XItem; x++)
    {
        for(int y=0; y<Tray.YItem; y++)
        {
            if(Tray.Data[x][y]==HAS_IC && Tray.DeviceInfo[x][y].iBin>0)
            {

                Str=AnsiString().sprintf("%d   %d   %d   %s   %s   %s   %s   \"%s\"",
                                        x+1,
                                        y+1,
                                        Tray.DeviceInfo[x][y].iBin,
                                        Tray.DeviceInfo[x][y].Arm,
                                        Tray.DeviceInfo[x][y].SiteNO,
                                        Tray.DeviceInfo[x][y].Indext_Time,
                                        Tray.DeviceInfo[x][y].Test_Time,
                                        Tray.DeviceInfo[x][y].Device2DID);
                List->Add(Str);
            }
        }
    }
    PathName2.sprintf("D:\\HT-172_Log\\TrayMapOutput\\%04d\\%02d\\", SystemYear, SystemMonth);                    //Steven 20230215 : 存檔路徑加上年月
    MyForceDirectories(PathName2);
    Str.sprintf("%s_%s_SO_T1_PASS_BIN%d_%s_%s.txt", fMain->edLotNo->Text, fMain->edtRunCard->Text, Tray.iBin, Tray.ClipID, sEndTime);
    AnsiString sTarget=AnsiString().sprintf("%s%s",PathName2,Str);
    List->SaveToFile(sTarget);
    List->Clear();
    delete List;
    asPath=PathName2+Str;
    if(HSys.FuncB.bN03_UploadTrayData)
    {
        FileInfo().EnsureDirectoriesExist(asTrayPathUpload);
        if(DirectoryExists(asTrayPathUpload))
        {
            if(FileExists(asPath))
            {
                strPath=FileInfo().PathCombin(asTrayPathUpload,fMain->edtRunCard->Text);
                FileInfo().EnsureDirectoriesExist(strPath);
                strPath=FileInfo().PathCombin(strPath,Str);
                bResult=CopyFile(asPath.c_str(), strPath.c_str(), false);
                if(bResult==false)
                {
                    ShowMyMessage(AnsiString().sprintf("Uploaded Unload tray log error\r\n Copy file failed.\r\n %s",strPath));
                }
                else
                {
                    RecordProcess(AnsiString().sprintf("Uploaded Unload tray log successfully. %s",strPath));   //Steven 20190722 : add TSV log
                }
            }
        }
        else
        {
            ShowMyMessage(AnsiString().sprintf("Uploaded Unload tray log error\r\nPlease check the path of N-2.\r\n %s",asTrayPathUpload));
        }
    }
}
//==============================================================================
bool TMyMotor::CheckArmPosInRange(int iNowPos,int iMin,int iMax)
{
    int iTemp;
    if(iMin>iMax)
    {
        iTemp=iMax;
        iMax=iMin;
        iMin=iTemp;
    }

    if(iMin<=iNowPos &&                                                         //JerryYang 20230309 : 修正index arm移動保護
       iNowPos<=iMax)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//==============================================================================
bool TMyMotor::CheckArmPosArrival(int iNowPos,int iDestination,int iTolerance)
{
    if(CheckArmPosInRange(iNowPos, iDestination-iTolerance, iDestination+iTolerance))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//==============================================================================
void TTrayMotor::GetNewNullTray()
{
    SetTrayVisible(true);
    SetTray(NULL_IC);
}
//==============================================================================
