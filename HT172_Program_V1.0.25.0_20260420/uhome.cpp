//==============================================================================
#include "IncludeAllHeader.h"
#pragma hdrstop

#include "uhome.h"

#include "aSortArm.h"
#include "aMagArm.h"

#pragma package(smart_init)
#pragma link "ALed"
#pragma link "butPa1"
#pragma resource "*.dfm"
//==============================================================================
TfHome *fHome;
int iFramWidth;
int iFramHeight;
int iFramTop;
int iFramLeft;
int iHomeFinishTransferPos;
int iHomeFinishShuttlePos;
int iHomeFinishAutoPos;
HTimer ResetOKDeleyTime;
HTimer THomeCheckTimeOut;
TQPF_Timer PushTrayTimeOut;
//==============================================================================
int iRowCoun=14;

__fastcall TfHome::TfHome(TComponent* Owner)
     : TForm(Owner)
{
    fLedInitial=false;
    iHomeStep=1;
    fShow=false;
    fAbort=false;
    iFramWidth=Width;
    iFramHeight=Height;
    //iFramTop=(1024-Height)/2;;
    //iFramLeft=(1280-Width)/2;;
    int LedPitch =150;
    int EditPitch =175;

    int i;//, x, y;
    for(i=0; i<HSys.iTotalMotor; i++)
    {
        if(HSys.MotPtr[i]==NULL)
            continue;
        LabelMotorName[i]=new TLabel(this);
        LabelMotorName[i]->Parent=Panel1;
        LabelMotorName[i]->Left=5+300*(i/iRowCoun);
        LabelMotorName[i]->Top =8+ 30*(i%iRowCoun);
        LabelMotorName[i]->Caption=HSys.MotPtr[i]->NumberAlias;
        LabelMotorName[i]->Font->Size=10;

        LedPtr[i]=new TALed(this);
        LedPtr[i]->Parent=Panel1;
        LedPtr[i]->Left=LedPitch+300*(i/iRowCoun);
        LedPtr[i]->Top =8+ 30*(i%iRowCoun);
        LedPtr[i]->LEDStyle=LEDSqLarge;
        LedPtr[i]->Blink=false;
        LedPtr[i]->FalseColor=clSilver;
        LedPtr[i]->TrueColor=clLime;

        EditMotorPos[i]=new TEdit(this);
        EditMotorPos[i]->Parent=Panel1;
        EditMotorPos[i]->Left=EditPitch+300*(i/iRowCoun);
        EditMotorPos[i]->Top =8+ 30*(i%iRowCoun);
        EditMotorPos[i]->Font->Size=10;
    }
    bCheckMotorPower=false;
}
//---------------------------------------------------------------------------
void UpdateHomeParameter()
{
//    iHomeFinishTransferPos=Get0_01MMType(fTeach->edTeach000->Text.c_str())+Get0_01MMType(fOffset->edOffset000->Text.c_str());
//    iHomeFinishShuttlePos=Get0_01MMType(fTeach->edTeach005->Text.c_str())+Get0_01MMType(fOffset->edOffset005->Text.c_str());
//    iHomeFinishAutoPos=Get0_01MMType(fTeach->edTeach018->Text.c_str())+Get0_01MMType(fOffset->edOffset018->Text.c_str());
}
//==============================================================================
void TfHome::SetWidth()                                                         // set form size //
{
    Width=iFramWidth;
    Height=iFramHeight;
    BorderStyle=bsSingle;
    //Top=iFramTop;
    //Left=iFramLeft;
}
//==============================================================================
void TfHome::ShowLed(int index, eHomeLedColor attr)  // led status //
{
    if(attr==eHomeUnuse)
    {
        LedPtr[index]->Value=false;
    }
    else
    {
        if(attr==eHomeOk)
            LedPtr[index]->TrueColor=clLime;
        else if(attr==eHomeError)
            LedPtr[index]->TrueColor=clRed;
        else if(attr==eHomeBusy)
            LedPtr[index]->TrueColor=clYellow;
        LedPtr[index]->Value=true;
    }
}
//==============================================================================
void TfHome::ResetAllMotorLed()                                                 // no use //
{
    int i;
    for(i=0;i<HSys.iTotalMotor;i++)
        ShowLed(i, fHome->eHomeUnuse);
}
//==============================================================================
void TfHome::ShowMotorHomePos(int i)                                            // show motor now pos //
{
    if(HSys.MotPtr[i]->GetEnable())
        EditMotorPos[i]->Text=HSys.MotPtr[i]->ReadPos();
    else
        EditMotorPos[i]->Text=0;
}
//==============================================================================
void SetHomeStart(TTrayMotor *Motor)                                            // show motor homeing //
{
    AnsiString Str;
    Motor->InitHomeTask();
    fHome->ShowLed(Motor->Tag, fHome->eHomeBusy);
    Str.sprintf("%s homeing ....", Motor->NumberAlias.c_str());
    fHome->lstHomeMsg->Items->Insert(0, Str.c_str());
    Motor->bHomeFinish=false;
}
//==============================================================================
void SetHomeFinish(TTrayMotor *Motor)                                                       // show motor home finish //
{
    AnsiString Str;
    fHome->ShowLed(Motor->Tag, fHome->eHomeOk);
    Str.sprintf("%s home finish.", Motor->NumberAlias.c_str());
    fHome->lstHomeMsg->Items->Insert(0, Str.c_str());
    Motor->bHomeFinish=true;
}
//==============================================================================
void SetCylinderFinishSerial(int i)                                             // show cylinder pop finish //
{
    AnsiString str;
    str.sprintf("[%d] %s Pop finish....",i+1, HSys.CynPtr[i].CylinderName);
    fHome->lstHomeMsg->Items->Insert(0, str.c_str());
}
//==============================================================================
void ProcessMotorHomeSub(TMyMotor *m)
{
    if( m->bHomeFlag==false)
    {
        AnsiString sErr="";
        m->Home(sErr);
        if( m->bHomeFlag==true)
            fHome->ShowLed(m->Tag, fHome->eHomeOk);
    }
    fHome->ShowMotorHomePos(m->Tag);                                            // show motor now pos //
}
//==============================================================================
bool TfHome::TrayArmHome(bool bReset)
{
    bool bOK=true;
//    bool bCheckflag=true;

    AnsiString str;

    static bool bFlag[11];
    static int Task=1;

    if(bReset)
    {
        Task=1;
        return false;
    }

    switch(Task)
    {
        case 1:
            SetHomeStart(HSys.Mot.StoreArmX);
            Task=1000;
            break;
        case 1000:
            if(HSys.Mot.StoreArmX->GetEnable()==false)
            {
                Task=1100;
            }
            else
            {
                HSys.Mot.StoreArmX->ScanMotorStatus();
                if((HSys.Sen.SnStoreArmPutPos1.IsOn() && HSys.Sen.SnStoreArmPutPos2.IsOn()) ||
                    HSys.Mot.StoreArmX->Led[iHomeLed]==true)
                {
                    Task=1100;
                }
                else
                {
                    SpeedButton1Click(this);
                    ShowMyMessage("請手動將StoreArmX 移至安全點位");
                }
            }
            break;
        case 1100:
            if(HSys.Mot.StoreArmX->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.StoreArmX->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.StoreArmX);
                    Task=2000;
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.StoreArmX->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            break;
        case 2000:
            SetHomeStart(HSys.Mot.StoreArmY);
            SetHomeStart(HSys.Mot.StoreArmZ);
            Task=2100;
            break;
        case 2100:
            bOK=true;
            if(HSys.Mot.StoreArmY->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.StoreArmY->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.StoreArmY);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.StoreArmY->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(HSys.Mot.StoreArmZ->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.StoreArmZ->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.StoreArmZ);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.StoreArmZ->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(bOK)
            {
                bFlag[0]=false;
                bFlag[1]=false;
                HSys.Mot.StoreArmZ->SetPersentSpeed(60);
                Task=2200;
            }
            break;
        case 2200:
            if(bFlag[0]==false)
                bFlag[0]=HSys.Mot.StoreArmZ->MotorMove(TrayArmPara->iPos_Track_Z[eTrack1]);

            if(bFlag[1]==false)
                bFlag[1]=HSys.Mot.StoreArmY->MotorMove(TrayArmPara->iPos_Track_Y[eTrack1]);

            if(bFlag[0] && bFlag[1])
            {
                bFlag[0]=false;
                bFlag[1]=false;
                return true;
            }
            break;
    }
    return false;
}
//==============================================================================
bool TfHome::SortArm1ZHome(bool bReset, bool bRestHomeFlag, bool bYHome)
{
    bool bOK=true;
    AnsiString str="";
    static int Task=1;
    if(bReset)
    {
        Task=1;
        return false;
    }
    switch(Task)
    {
        case 1:
            for(int i=HSys.Mot.SortArm1ZA->Tag; i<=HSys.Mot.SortArm1ZH->Tag ; i++)
            {
                if(bRestHomeFlag)                                               //JerryYang 20240610 : Z auto home
                {
                    HSys.MotPtr[i]->InitHomeTask_forSingleAxis();
                }
                else
                {
                    SetHomeStart(HSys.MotPtr[i]);
                }
            }
            Task=1100;
            break;
        case 1100:
            bOK=true;
            for(int i=HSys.Mot.SortArm1ZA->Tag; i<=HSys.Mot.SortArm1ZH->Tag ; i++)
            {
                if(HSys.MotPtr[i]->bHomeFinish==false)
                {
                    AnsiString sErr="";
                    if(HSys.MotPtr[i]->Home(sErr))
                    {
                        SetHomeFinish(HSys.MotPtr[i]);
                    }
                    else
                    {
                        fHome->ShowLed(HSys.MotPtr[i]->Tag,   fHome->eHomeBusy);
                        bOK=false;
                    }
                }
            }

            if(bOK)
            {
                Task=1200;
            }
            break;
        case 1200:
            if(bRestHomeFlag)                                                   //JerryYang 20240610 : Z auto home
            {
                HSys.MotPtr[HSys.Mot.SortArm1PitchX->Tag]->InitHomeTask_forSingleAxis();
                HSys.MotPtr[HSys.Mot.SortArm1PitchY1->Tag]->InitHomeTask_forSingleAxis();
                HSys.MotPtr[HSys.Mot.SortArm1PitchY2->Tag]->InitHomeTask_forSingleAxis();
                if(bYHome)  //JerryYang 20240704 : Pick up error Y軸一起回HOME
                {
                    HSys.MotPtr[HSys.Mot.SortArm1Y->Tag]->InitHomeTask_forSingleAxis();
                }
            }
            else
            {
                SetHomeStart(HSys.Mot.SortArm1PitchX);
                SetHomeStart(HSys.Mot.SortArm1PitchY1);
                SetHomeStart(HSys.Mot.SortArm1PitchY2);
            }

            Task=1300;
            break;
        case 1300:
            bOK=true;
            if(HSys.Mot.SortArm1PitchX->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.SortArm1PitchX->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.SortArm1PitchX);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.SortArm1PitchX->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(HSys.Mot.SortArm1PitchY1->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.SortArm1PitchY1->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.SortArm1PitchY1);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.SortArm1PitchY1->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(HSys.Mot.SortArm1PitchY2->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.SortArm1PitchY2->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.SortArm1PitchY2);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.SortArm1PitchY2->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(bYHome && HSys.Mot.SortArm1Y->bHomeFinish==false)    //JerryYang 20240704 : Pick up error Y軸一起回HOME
            {
                AnsiString sErr="";
                if(HSys.Mot.SortArm1Y->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.SortArm1Y);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.SortArm1Y->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(bOK)
            {
                Task=1400;
            }
            break;
        case 1400:
            if(bRestHomeFlag)                                                   //Sam 20240622 : 修正 Auto Home 後的速度恢復
                SetMotorSpeed(true);
            return true;

    }
    return false;
}
//==============================================================================
bool TfHome::SortArm2ZHome(bool bReset, bool bRestHomeFlag)                     //Sam 20240618 : MagArm Z auto home
{
    bool bOK=true;
    AnsiString str="";
    static int Task=1;
    if(bReset)
    {
        Task=1;
        return false;
    }

    switch(Task)
    {
        case 1:
            for(int i=HSys.Mot.SortArm2ZA->Tag; i<=HSys.Mot.SortArm2ZH->Tag ; i++)
            {
                if(bRestHomeFlag)                                               //Sam 20240618 : MagArm Z auto home
                {
                    HSys.MotPtr[i]->InitHomeTask_forSingleAxis();
                }
                else
                {
                    SetHomeStart(HSys.MotPtr[i]);
                }
            }
            Task=1100;
            break;
        case 1100:
            bOK=true;
            for(int i=HSys.Mot.SortArm2ZA->Tag; i<=HSys.Mot.SortArm2ZH->Tag ; i++)
            {
                if(HSys.MotPtr[i]->bHomeFinish==false)
                {
                    AnsiString sErr="";
                    if(HSys.MotPtr[i]->Home(sErr))
                    {
                        SetHomeFinish(HSys.MotPtr[i]);
                    }
                    else
                    {
                        fHome->ShowLed(HSys.MotPtr[i]->Tag,   fHome->eHomeBusy);
                        bOK=false;
                    }
                }
            }

            if(bOK)
            {
                Task=1200;
            }
            break;
        case 1200:
            if(bRestHomeFlag)   //Sam 20240618 : MagArm Z auto home
            {
                HSys.MotPtr[HSys.Mot.SortArm2PitchX->Tag]->InitHomeTask_forSingleAxis();
                HSys.MotPtr[HSys.Mot.SortArm2PitchY1->Tag]->InitHomeTask_forSingleAxis();
                HSys.MotPtr[HSys.Mot.SortArm2PitchY2->Tag]->InitHomeTask_forSingleAxis();
            }
            else
            {
                SetHomeStart(HSys.Mot.SortArm2PitchX);
                SetHomeStart(HSys.Mot.SortArm2PitchY1);
                SetHomeStart(HSys.Mot.SortArm2PitchY2);
            }
            Task=1300;
            break;
        case 1300:
            bOK=true;
            if(HSys.Mot.SortArm2PitchX->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.SortArm2PitchX->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.SortArm2PitchX);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.SortArm2PitchX->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(HSys.Mot.SortArm2PitchY1->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.SortArm2PitchY1->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.SortArm2PitchY1);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.SortArm2PitchY1->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(HSys.Mot.SortArm2PitchY2->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.Mot.SortArm2PitchY2->Home(sErr))
                {
                    SetHomeFinish(HSys.Mot.SortArm2PitchY2);
                }
                else
                {
                    fHome->ShowLed(HSys.Mot.SortArm2PitchY2->Tag,   fHome->eHomeBusy);
                    bOK=false;
                }
            }
            if(bOK)
            {
                Task=1400;
            }
            break;
        case 1400:
            if(bRestHomeFlag)                                                   //Sam 20240622 : 修正 Auto Home 後的速度恢復
                SetMotorSpeed(true);
            return true;

    }
    return false;
}

//==============================================================================
bool TfHome::TrollyZHome(bool bReset)
{
    bool bOK=true;
    AnsiString str;

//    static bool bFlag[11];
    static int Task=1;

    if(bReset)
    {
        Task=1;
        return false;
    }

    switch(Task)
    {
        case 1:
            for(int i=HSys.Mot.AutoTrolly1Z->Tag; i<=HSys.Mot.AutoTrolly4Z->Tag ; i++)
            {
                SetHomeStart(HSys.MotPtr[i]);
            }
            Task=1100;
            break;
        case 1100:
            bOK=true;
            for(int i=HSys.Mot.AutoTrolly1Z->Tag; i<=HSys.Mot.AutoTrolly4Z->Tag ; i++)
            {
                if(HSys.MotPtr[i]->bHomeFinish==false)
                {
                    AnsiString sErr="";
                    if(HSys.MotPtr[i]->Home(sErr))
                    {
                        SetHomeFinish(HSys.MotPtr[i]);
                    }
                    else
                    {
                        fHome->ShowLed(HSys.MotPtr[i]->Tag,   fHome->eHomeBusy);
                        bOK=false;
                    }
                }
            }

            if(bOK)
            {
                Task=1200;
            }
            break;
        case 1200:
            return true;
    }
    return false;
}
//==============================================================================
bool TfHome::PushStoreArmHome(bool bReset)
{
    bool bOK=true, bCyFlag[4];
    AnsiString str;
    static int Task=1;

    if(bReset)
    {
        for(int i=0; i<4; i++)
            bCyFlag[i]=false;
        Task=1;
        return false;
    }

    switch(Task)
    {
        case 1:
            for(int i=HSys.Mot.PushStoreArmX1->Tag; i<=HSys.Mot.PushStoreArmX4->Tag; i++)
                SetHomeStart(HSys.MotPtr[i]);
            Task=100;
            break;
        case 100:
            for(int i=HSys.Mot.PushStoreArmX1->Tag; i<=HSys.Mot.PushStoreArmX4->Tag; i++)
            {
                if(HSys.MotPtr[i]->GetEnable()==false)
                {
                    bOK=true;
                }
                else
                {
                    if(HSys.MotPtr[i]->bHomeFinish==false)
                    {
                        AnsiString sErr="";
                        if(HSys.MotPtr[i]->Home(sErr))
                        {
                            SetHomeFinish(HSys.MotPtr[i]);
                        }
                        else
                        {
                            fHome->ShowLed(HSys.MotPtr[i]->Tag, fHome->eHomeBusy);
                            bOK=false;
                        }
                    }
                }
            }

            if(bOK)
            {
                PushTrayTimeOut.SetSecAndOn(3);
                Task=200;
            }
            break;
        case 200:
            bCyFlag[0]=(HSys.Cyn.C_AutoRow1_PushTray.Pop() && HSys.Sen.SnAutoRow1CyPushTray.IsOff());
            bCyFlag[1]=(HSys.Cyn.C_AutoRow2_PushTray.Pop() && HSys.Sen.SnAutoRow2CyPushTray.IsOff());
            bCyFlag[2]=(HSys.Cyn.C_AutoRow3_PushTray.Pop() && HSys.Sen.SnAutoRow3CyPushTray.IsOff());
            bCyFlag[3]=(HSys.Cyn.C_AutoRow4_PushTray.Pop() && HSys.Sen.SnAutoRow4CyPushTray.IsOff());

            #ifdef SOFT_SIMULATE
            Task=990;
            #else
            if(bCyFlag[0] && bCyFlag[1] && bCyFlag[2] && bCyFlag[3])
            {
                Task=990;
            }
            else if(PushTrayTimeOut.Off())
            {
                if(bCyFlag[0]==false)
                    ShowMyMessage("Please check C_AutoRow1_PushTray status");
                else if(bCyFlag[1]==false)
                    ShowMyMessage("Please check C_AutoRow2_PushTray status");
                else if(bCyFlag[2]==false)
                    ShowMyMessage("Please check C_AutoRow3_PushTray status");
                else if(bCyFlag[3]==false)
                    ShowMyMessage("Please check C_AutoRow4_PushTray status");
            }
            #endif
            break;
        case 990:
            return true;
    }
    return false;
}
//==============================================================================
bool TfHome::MagazineX_Home(bool bReset, int iWhichMGZ)
{
    AnsiString str;
    static int Task[eMGZTotal]={1, 1, 1};
    static bool bflag[7]={false, false, false, false, false, false, false};
    TMyCylinder *HomeC_MGZ_LayerTrayPush[eMGZTotal]={&HSys.Cyn.C_MGZ1_LayerTrayPush, &HSys.Cyn.C_MGZ2_LayerTrayPush, &HSys.Cyn.C_MGZ3_LayerTrayPush};         //大側推
    TMyCylinder *HomeC_MGZ_SidePush_Up  [eMGZTotal]={&HSys.Cyn.C_MGZ1_SidePush_Up,   &HSys.Cyn.C_MGZ2_SidePush_Up,   &HSys.Cyn.C_MGZ2_SidePush_Up};           //Side up
    TMyCylinder *HomeC_MGZ_SidePush     [eMGZTotal]={&HSys.Cyn.C_MGZ1_SidePush,      &HSys.Cyn.C_MGZ2_SidePush,      &HSys.Cyn.C_MGZ2_SidePush};              //Side
    TMyCylinder *HomeC_MGZ_EdgeFixer    [eMGZTotal]={&HSys.Cyn.C_MGZ1_EdgeFixer,     &HSys.Cyn.C_MGZ2_EdgeFixer,     &HSys.Cyn.C_MGZ2_EdgeFixer};             //電梯夾TRAY左右
    TMyCylinder *HomeC_MGZ_TrayPush     [eMGZTotal]={&HSys.Cyn.C_MGZ1_TrayPush,      &HSys.Cyn.C_MGZ2_TrayPush,      &HSys.Cyn.C_MGZ2_TrayPush};              //電梯TRAY前後推
    TMyCylinder *HomeC_MGZ_TopFixTray   [eMGZTotal]={&HSys.Cyn.C_MGZ1_TopFixTray,    &HSys.Cyn.C_MGZ2_TopFixTray,    &HSys.Cyn.C_MGZ2_TopFixTray};            //Top前後夾TRAY
    TMyCylinder *HomeC_MGZ_CatchTray    [eMGZTotal]={&HSys.Cyn.C_MGZ1_CatchTray,     &HSys.Cyn.C_MGZ2_CatchTray,     &HSys.Cyn.C_MGZ2_CatchTray};             //電梯X軸夾爪
    TTrayMotor  *Home_MotMGZ_X          [eMGZTotal]={HSys.Mot.Magazine1X, HSys.Mot.Magazine2X, HSys.Mot.Magazine3X};

    if(bReset)
    {
        Task[iWhichMGZ]=1;
        return false;
    }

    switch(Task[iWhichMGZ])
    {
        case 1:
            HomeC_MGZ_LayerTrayPush[iWhichMGZ]->Reset();
            HomeC_MGZ_SidePush_Up  [iWhichMGZ]->Reset();
            HomeC_MGZ_SidePush     [iWhichMGZ]->Reset();
            HomeC_MGZ_EdgeFixer    [iWhichMGZ]->Reset();
            HomeC_MGZ_TrayPush     [iWhichMGZ]->Reset();
            HomeC_MGZ_TopFixTray   [iWhichMGZ]->Reset();
            HomeC_MGZ_CatchTray    [iWhichMGZ]->Off();

            bflag[0]=false;
            bflag[1]=false;
            bflag[2]=false;
            bflag[3]=false;
            bflag[4]=false;
            bflag[5]=false;
            bflag[6]=false;

            Task[iWhichMGZ]=100;
            break;
         case 100:
//            bflag[0]=HSys.Cyn.C_MGZ2_LayerTrayPush.Pop();                       //Sam 20240525 : 增加 Magazine 汽缸的保護
//            bflag[1]=HSys.Sen.SnMGZ2DetectTray.IsOff();                         //Sam 20240525 : 增加保護避免換 Tray 到一半 Home
            #ifdef SOFT_SIMULATE
                Task[iWhichMGZ]=200;
            #endif
            if(bflag[0]==false)
            {
                bflag[0]=HomeC_MGZ_LayerTrayPush[iWhichMGZ]->Pop();   //大側推
            }
            if(bflag[1]==false)
            {
                bflag[1]=HomeC_MGZ_SidePush_Up  [iWhichMGZ]->Pop();   //Side up
            }
            if(bflag[2]==false)
            {
                bflag[2]=HomeC_MGZ_SidePush     [iWhichMGZ]->Pop();       //Side
            }
            if(bflag[3]==false)
            {
                bflag[3]=HomeC_MGZ_EdgeFixer    [iWhichMGZ]->Pop();      //電梯夾TRAY左右
            }
            if(bflag[4]==false)
            {
                bflag[4]=HomeC_MGZ_TrayPush     [iWhichMGZ]->Pop();       //電梯TRAY前後推
            }
            if(bflag[5]==false)
            {
                bflag[5]=HomeC_MGZ_CatchTray    [iWhichMGZ]->Pop();       //電梯TRAY前後推
            }

            if(bflag[0]==true && bflag[1]==true && bflag[2]==true && bflag[3]==true && bflag[4]==true && bflag[5]==true)
            {
                Task[iWhichMGZ]=200;
            }
            break;
        case 200:

            SetHomeStart(Home_MotMGZ_X[iWhichMGZ]);
            Task[iWhichMGZ]=300;

            break;
        case 300:
            if(Home_MotMGZ_X[iWhichMGZ]->GetEnable()==false)
            {
                Task[iWhichMGZ]=1000;
            }
            else
            {
                Task[iWhichMGZ]=1000;
            }
            break;
        case 1000:
            if(Home_MotMGZ_X[iWhichMGZ]->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(Home_MotMGZ_X[iWhichMGZ]->Home(sErr))
                {
                    SetHomeFinish(Home_MotMGZ_X[iWhichMGZ]);
                    Task[iWhichMGZ]=2000;
                }
                else
                {
                    fHome->ShowLed(Home_MotMGZ_X[iWhichMGZ]->Tag,   fHome->eHomeBusy);
                }
            }
            break;
        case 2000:
            return true;
    }
    return false;
}
//==============================================================================
bool TfHome::SingleMotorHome(bool bReset, int iMotor)                           //KenHsieh 20240830 : add 運作中單軸回Home
{
    bool bOK=true;
    int &iTask=iSingleMotHomeTask[iMotor];
    AnsiString str="";

    if(bReset)
    {
        iTask=1;
        if(HSys.MotPtr[iMotor]->Alias=="AutoTrolly1Z" ||                        //JerryYang 20250717 : add log
           HSys.MotPtr[iMotor]->Alias=="AutoTrolly2Z" ||
           HSys.MotPtr[iMotor]->Alias=="AutoTrolly3Z" ||
           HSys.MotPtr[iMotor]->Alias=="AutoTrolly4Z")
        {
            str.sprintf("Reset_SingleMotorHome_%s", HSys.MotPtr[iMotor]->Alias);
            RecordProcess(str);
        }
        return false;
    }

    switch(iTask)
    {
        case 1:
            if(iMotor<0 || iMotor>=HSys.iTotalMotor)
            {
                iTask=1;
                return true;
            }

            if(HSys.MotPtr[iMotor]->GetEnable()==false)
            {
                iTask=1;
                return true;
            }

            HSys.MotPtr[iMotor]->InitHomeTask_forSingleAxis();
            iTask=1000;
            break;
        case 1000:
            bOK=true;
            if(HSys.MotPtr[iMotor]->bHomeFinish==false)
            {
                AnsiString sErr="";
                if(HSys.MotPtr[iMotor]->Home(sErr))
                    SetHomeFinish(HSys.MotPtr[iMotor]);
                else
                    bOK=false;
            }

            if(bOK)
                iTask=9900;
            break;
        case 9900:
            HSys.MotPtr[iMotor]->SetPersentSpeed(HSys.MotPtr[iMotor]->GetPersentSpeed());
            return true;
    }
    return false;
}
//==============================================================================
bool ProcessMotorHome(bool Flag2)                                               // do home //
{
    static int iPowerCount=0;                                                   //馬達斷送電計時
    static bool bflag[10]={false};
    static Word OldSystemSec;

    int &Task=fHome->iHomeStep;
    bool bOK=true;
    bool bResult=false;
    bool bHomeOrder;
    int ret=0, iZpos[eMGZTotal];

    if(HSys.Sys.SystemStart==false)                                             //Press pause 20111124 Daver add
    {
        HSys.DecStopAllMotor();
        HSys.Sys.bNeedHome=true;
        fAllMotorHome=false;
        SoftStop=true;
        fHome->Close();
        Task=1;
        return false;
    }

    switch(Task)
    {
        case 1:
            for(int i=0; i<HSys.iTotalMotor; i++)                               //關閉的軸的直接完成 Home
            {
                if(HSys.MotPtr[i]->GetEnable()==false)
                {
                    HSys.MotPtr[i]->bHomeFlag=true;
                    fHome->ShowLed(i, fHome->eHomeOk);                                        // clLime //
                }
                else
                {
                    fHome->ShowLed(i, fHome->eHomeUnuse);                                        // null //
                }
            }

            for(int i=0; i<HSys.iTotalCylinder; i++)
            {
                if(HSys.CynPtr[i].Enable==true)
                    HSys.CynPtr[i].bInitialOk=false;
                else
                    HSys.CynPtr[i].bInitialOk=true;
            }

            #ifndef SOFT_SIMULATE   //Sam 20240515 : 修正 Dummy Run
            if(CosFunction.bRemoveICofSuckerWhenHome==true)
            {
                if(SortArmPara->ManualTakeOutSortArmDevice(true)==false)
                {
                    return false;
                }
                if(MagArmPara->ManualTakeOutMagArmDevice(true)==false)
                {
                    return false;
                }
            }

            if(LoaderModule->MotTrack_Work[eTrack4]->fHasTray ||
               fMain->palWorkEmpty4->Visible)                                   //KenHsieh 20240521 : 移除後需清資料
            {
                int iret=ShowMyMessageBox_YES_NO("請移除Empty Track 4 Tray, 並按下YES移除資料");

                if(iret==TMyMessageBox::msgrtnYES)
                {
                    LoaderModule->MotTrack_Work[eTrack4]->ClearTray();
                }
                return false;
            }
            else if(HSys.Sen.SnEmpty1HasTray4.IsOff()==false)
            {
                ShowMyMessage("請移除Empty Track 4 Tray");
                return false;
            }

            if(LoaderModule->MotTrack_Work[eTrack3]->fHasTray ||
               fMain->palWorkEmpty3->Visible)                                   //KenHsieh 20240521 : 移除後需清資料
            {
                int iret=ShowMyMessageBox_YES_NO("請移除Empty Track 3 Tray, 並按下YES移除資料");

                if(iret==TMyMessageBox::msgrtnYES)
                {
                    LoaderModule->MotTrack_Work[eTrack3]->ClearTray();
                }
                return false;
            }
            else if(HSys.Sen.SnEmpty1HasTray3.IsOff()==false)
            {
                ShowMyMessage("請移除Empty Track 3 Tray");
                return false;
            }

            if(LoaderModule->MotTrack_Work[eTrack2]->fHasTray ||
               fMain->palWorkEmpty2->Visible)                                   //KenHsieh 20240521 : 移除後需清資料
            {
                int iret=ShowMyMessageBox_YES_NO("請移除Empty Track 2 Tray, 並按下YES移除資料");

                if(iret==TMyMessageBox::msgrtnYES)
                {
                    LoaderModule->MotTrack_Work[eTrack2]->ClearTray();
                }
                return false;
            }
            else if(HSys.Sen.SnEmpty1HasTray2.IsOff()==false)
            {
                ShowMyMessage("請移除Empty Track 2 Tray");
                return false;
            }

             if(LoaderModule->MotTrack_Work[eTrack1]->fHasTray ||
               fMain->palWorkEmpty1->Visible)                                   //KenHsieh 20240521 : 移除後需清資料
            {
                int iret=ShowMyMessageBox_YES_NO("請移除Empty Track 1 Tray, 並按下YES移除資料");

                if(iret==TMyMessageBox::msgrtnYES)
                {
                    LoaderModule->MotTrack_Work[eTrack1]->ClearTray();
                }
                return false;
            }
            else if(HSys.Sen.SnEmpty1HasTray1.IsOff()==false)
            {
                ShowMyMessage("請移除Empty Track 1 Tray");
                return false;
            }

            if(HSys.Sen.SnMGZ2TrayPositionR.IsOff()==false)
            {
                ShowMyMessage("Magazine2安全sensor is on!! 請確認SnMGZ2TrayPositionR");
                return false;
            }

            if(HSys.Sen.SnMGZ2TrayPositionF.IsOff()==false)
            {
                ShowMyMessage("Magazine2安全sensor is on!! 請確認SnMGZ2TrayPositionF");
                return false;
            }

            if(HSys.Sen.SnMGZ1TrayPositionR.Enable==true &&
               HSys.Sen.SnMGZ1TrayPositionR.IsOff()==false)
            {
                ShowMyMessage("Magazine1安全sensor is on!! 請確認SnMGZ2TrayPositionR");
                return false;
            }

            if(HSys.Sen.SnMGZ1TrayPositionF.Enable==true &&
               HSys.Sen.SnMGZ1TrayPositionF.IsOff()==false)
            {
                ShowMyMessage("Magazine1安全sensor is on!! 請確認SnMGZ2TrayPositionF");
                return false;
            }

            if(HSys.Sen.SnMGZ3TrayPositionR.Enable==true &&
               HSys.Sen.SnMGZ3TrayPositionR.IsOff()==false)
            {
                ShowMyMessage("Magazine3安全sensor is on!! 請確認SnMGZ2TrayPositionR");
                return false;
            }

            if(HSys.Sen.SnMGZ3TrayPositionF.Enable==true &&
               HSys.Sen.SnMGZ3TrayPositionF.IsOff()==false)
            {
                ShowMyMessage("Magazine3安全sensor is on!! 請確認SnMGZ2TrayPositionF");
                return false;
            }

            #endif

            fHome->Show();
            fHome->lstHomeMsg->Clear();
            fHome->lstHomeMsg->Items->Insert(0, "Starting home procedure....");

            AllBreakLock();

            Task=1000;
            break;
        case 1000:
            HSys.Sw.SwMotorRelay.Off();                                           //馬達斷電
            fHome->bCheckMotorPower=false;                                      //繼電器的電 24V 旗標
            bMotorHomePowerOn=false;                                            //馬達 Server On/Off 旗標
            fHome->lstHomeMsg->Items->Insert(0, "Power off....");

            iPowerCount=3;                                                     //斷電 15 秒
            OldSystemSec=-1;
            #ifdef SOFT_SIMULATE
                ResetOKDeleyTime.SetSecAndOn(0.1);
            #else
                ResetOKDeleyTime.SetSecAndOn(iPowerCount);
            #endif
            Task=2000;
            break;
        case 2000:                                                              //馬達送電
            if(ResetOKDeleyTime.Off())                                          //斷電計數結束
            {
                HSys.Sw.SwMotorRelay.On();                                      //馬達送電
                fHome->bCheckMotorPower=true;
                fHome->lstHomeMsg->Items->Insert(0, "Power on....");

                iPowerCount=3;                                                  //送電 3 秒
                OldSystemSec=-1;
                #ifdef SOFT_SIMULATE
                    ResetOKDeleyTime.SetSecAndOn(0.1);
                #else
                    ResetOKDeleyTime.SetSecAndOn(iPowerCount);
                #endif
                Task=2500;
            }
            else                                                                //Power Down Count
            {
                if(iPowerCount>=0)
                {
                    if(OldSystemSec!=SystemSec)
                    {
                        fHome->lstHomeMsg->Items->Insert(0, "Power on count down : "+AnsiString(iPowerCount));
                        iPowerCount--;
                        OldSystemSec=SystemSec;
                    }
                }
            }
            break;
        case 2500:
            if(ResetOKDeleyTime.Off())                                          //送電計數結束
            {
                THomeCheckTimeOut.SetSecAndOn(3);
                HSys.Cyn.C_MGZ2_LayerTrayPush.Reset();
                bflag[0]=false;
                bflag[1]=false;
                Task=2600;
            }
            break;
        case 2600:
            bflag[0]=HSys.Cyn.C_MGZ2_LayerTrayPush.Pop();                       //Sam 20240525 : 增加 Magazine 汽缸的保護
            bflag[1]=HSys.Sen.SnMGZ2DetectTray.IsOff();                         //Sam 20240525 : 增加保護避免換 Tray 到一半 Home

            #ifdef SOFT_SIMULATE
            Task=3000;
            #endif
            if(bflag[0] && bflag[1])
            {
                bflag[0]=false;                                                 //推Tray 機構改為馬達
                bflag[1]=false;
                bflag[2]=false;
                bflag[3]=false;
                Task=3000;
            }
            else if(THomeCheckTimeOut.Off())
            {
                if(bflag[1]==false)
                {
                     ShowMyMessage("SnMGZ2DetectTray is on!! 請取下Magazine 電梯上的TRAY盤");
                     return false;
                }
            }
            break;
        case 3000:                                                              //馬達 server On
            AllBreakFree();
            for(int i=0; i<HSys.iTotalMotor; i++)                           // server on off //
            {
                if(HSys.MotPtr[i]->ReadServoAlarmOn()==true)
                    HSys.MotPtr[i]->ServoOnOff(Server_On);
                else  //jou? ScanMotorStatus
                    HSys.MotPtr[i]->Led[iAlarmLed]=false;                   //Sam 20171101 : 清除線馬 Alarm 訊號
            }
            bMotorHomePowerOn=true;

            fHome->lstHomeMsg->Items->Insert(0, "Start All Z Axis、Store Push Tray Module homing....");  //KenHsieh 20240829 : 推Tray 馬達回Home修改

            bflag[0]=false;
            bflag[1]=false;
            bflag[2]=false;
            bflag[3]=false;
            bflag[4]=false;
            bflag[5]=false;

            fHome->SortArm1ZHome(true);
            fHome->SortArm2ZHome(true);
            if(HSys.FuncT.iT06_AutoRowType==0)
                fHome->PushStoreArmHome(true);                                      //KenHsieh 20240829 : 推Tray 馬達回Home修改
            else
                fHome->TrollyZHome(true);

            fHome->MagazineX_Home(true, eMagazine1);
            fHome->MagazineX_Home(true, eMagazine2);
            fHome->MagazineX_Home(true, eMagazine3);
            Task=4000;
            break;
        case 4000:                                                              //All Z Axis home， Z Axis 要先 Home 避免硬體干涉
            if(bflag[0]==false)
                bflag[0]=fHome->SortArm1ZHome(false);
            if(bflag[1]==false)
                bflag[1]=fHome->SortArm2ZHome(false);
            if(bflag[2]==false)
            {
                if(HSys.FuncT.iT06_AutoRowType==1)
                {
                    bflag[2]=fHome->TrollyZHome(false);
                }
                else if(USE_PUSH_TRAY==ePushByConveyor)
                {
                    bflag[2]=true;
                }
                else
                {
                    bflag[2]=fHome->PushStoreArmHome(false);                        //KenHsieh 20240829 : 推Tray 馬達回Home修改
                }
            }
            if(bflag[3]==false)
                bflag[3]=fHome->MagazineX_Home(false, eMagazine2);

            if(MAGAZINE_CNT==3)
            {
                if(bflag[4]==false)
                    bflag[4]=fHome->MagazineX_Home(false, eMagazine1);

                if(bflag[5]==false)
                    bflag[5]=fHome->MagazineX_Home(false, eMagazine3);
            }
            else
            {
                bflag[4]=true;
                bflag[5]=true;
            }
            if(bflag[0] && bflag[1] && bflag[2] && bflag[3])
            {
                bflag[0]=false;
                fHome->TrayArmHome(true);
                fHome->lstHomeMsg->Items->Insert(0, "Start Tray Arm homeing....");
                Task=4100;
            }
            break;
        case 4100:                                                              //KenHsieh 20240829 : 推Tray 馬達回Home修改
            if(bflag[0]==false)
                bflag[0]=fHome->TrayArmHome(false);
            if(bflag[0])
                Task=5000;
            break;
        case 5000:
            if(MagazineModule[eMagazine2]->CheckAllMagHasTray()==true || MagazineModule[eMagazine2]->MGZTrayArm->fHasTray)       //JerryYang 20240827 : magazine有TRAY要全部拿掉
            {
                if(HSys.Sen.SnMGZ2TopHasTray.IsOff()==false)     //JerryYang 20240827
                {
                    ShowMyMessage("SnMGZ2TopHasTray is on!! 請取下Magazine top上的TRAY盤");
                    return false;
                }

                if(HSys.Sen.SnMGZ2DetectTray.IsOff()==false)
                {
                    ShowMyMessage("SnMGZ2DetectTray is on!! 請取下Magazine 電梯上的TRAY盤");
                    return false;
                }

                ret=ShowMyMessageBox_YES_NO("請確認是否已將Magazine top及電梯的tray都取下??");
                if(ret==TMyMessageBox::msgrtnYES)
                {
                    MagazineModule[eMagazine2]->mTopTray.ConvertBinToAuto(0);       //Magazine Top clear data
                    MagArmPara->MMagSortTopTray[eMagazine2]->ClearTray();
                    MagazineModule[eMagazine2]->MGZTrayArm->ClearTray();            //Magazine 電梯 clear data

                    HSys.Sys.SystemStart=true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                #ifndef SOFT_SIMULATE
                if(HSys.Sen.SnMGZ2TopHasTray.IsOff()==false)     //JerryYang 20240827
                {
                    ShowMyMessage("SnMGZ2TopHasTray is on!! 請取下Magazine top上的TRAY盤");
                    return false;
                }

                if(HSys.Sen.SnMGZ2DetectTray.IsOff()==false)
                {
                    ShowMyMessage("SnMGZ2DetectTray is on!! 請取下Magazine 電梯上的TRAY盤");
                    return false;
                }
                #endif
            }

            if(MAGAZINE_CNT==3)
                Task=5100;
            else
                Task=6000;
            break;
        case 5100:
            if(MagazineModule[eMagazine1]->CheckAllMagHasTray()==true || MagazineModule[eMagazine1]->MGZTrayArm->fHasTray)       //JerryYang 20240827 : magazine有TRAY要全部拿掉
            {
                if(HSys.Sen.SnMGZ1TopHasTray.IsOff()==false)     //JerryYang 20240827
                {
                    ShowMyMessage("SnMGZ1TopHasTray is on!! 請取下Magazine top上的TRAY盤");
                    return false;
                }
                if(HSys.Sen.SnMGZ1DetectTray.IsOff()==false)
                {
                    ShowMyMessage("SnMGZ1DetectTray is on!! 請取下Magazine 電梯上的TRAY盤");
                    return false;
                }

                ret=ShowMyMessageBox_YES_NO("請確認是否已將Magazine1 top及電梯的tray都取下??");
                if(ret==TMyMessageBox::msgrtnYES)
                {
                    MagazineModule[eMagazine1]->mTopTray.ConvertBinToAuto(0);       //Magazine Top clear data
                    MagArmPara->MMagSortTopTray[eMagazine1]->ClearTray();
                    MagazineModule[eMagazine1]->MGZTrayArm->ClearTray();            //Magazine 電梯 clear data
                    HSys.Sys.SystemStart=true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                #ifndef SOFT_SIMULATE
                if(HSys.Sen.SnMGZ1TopHasTray.IsOff()==false)     //JerryYang 20240827
                {
                    ShowMyMessage("SnMGZ1TopHasTray is on!! 請取下Magazine top上的TRAY盤");
                    return false;
                }

                if(HSys.Sen.SnMGZ1DetectTray.IsOff()==false)
                {
                    ShowMyMessage("SnMGZ1DetectTray is on!! 請取下Magazine 電梯上的TRAY盤");
                    return false;
                }
                #endif
            }
            Task=5150;
            break;
        case 5150:
            if(MagazineModule[eMagazine3]->CheckAllMagHasTray()==true || MagazineModule[eMagazine3]->MGZTrayArm->fHasTray)       //JerryYang 20240827 : magazine有TRAY要全部拿掉
            {
                if(HSys.Sen.SnMGZ3TopHasTray.IsOff()==false)     //JerryYang 20240827
                {
                    ShowMyMessage("SnMGZ3TopHasTray is on!! 請取下Magazine top上的TRAY盤");
                    return false;
                }
                if(HSys.Sen.SnMGZ3DetectTray.IsOff()==false)
                {
                    ShowMyMessage("SnMGZ3DetectTray is on!! 請取下Magazine 電梯上的TRAY盤");
                    return false;
                }

                ret=ShowMyMessageBox_YES_NO("請確認是否已將Magazine top及電梯的tray都取下??");
                if(ret==TMyMessageBox::msgrtnYES)
                {
                    MagazineModule[eMagazine3]->mTopTray.ConvertBinToAuto(0);       //Magazine Top clear data
                    MagArmPara->MMagSortTopTray[eMagazine3]->ClearTray();
                    MagArmPara->MMagSortTopTray[eMagazine3]->Tray.iBin=0;

                    MagazineModule[eMagazine3]->MGZTrayArm->ClearTray();            //Magazine 電梯 clear data
                    MagazineModule[eMagazine3]->MGZTrayArm->Tray.iBin=0;
                    MagazineModule[eMagazine3]->MGZTrayArm->SetTrayVisible(false);

                    HSys.Sys.SystemStart=true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                #ifndef SOFT_SIMULATE
                if(HSys.Sen.SnMGZ3TopHasTray.IsOff()==false)     //JerryYang 20240827
                {
                    ShowMyMessage("SnMGZ3TopHasTray is on!! 請取下Magazine top上的TRAY盤");
                    return false;
                }

                if(HSys.Sen.SnMGZ3DetectTray.IsOff()==false)
                {
                    ShowMyMessage("SnMGZ3DetectTray is on!! 請取下Magazine 電梯上的TRAY盤");
                    return false;
                }
                #endif
            }
            Task=6000;
            break;
        case 6000:                                                              //Other Cylinder normal
            SetHomeStart(HSys.Mot.TrayShiftStageX);
            SetHomeStart(HSys.Mot.SortArm1X);
            SetHomeStart(HSys.Mot.SortArm1Y);
            SetHomeStart(HSys.Mot.SortArm2X);
            SetHomeStart(HSys.Mot.SortArm2Y);
            SetHomeStart(HSys.Mot.LoaderCCDArmX);
            SetHomeStart(HSys.Mot.Magazine1X);
            SetHomeStart(HSys.Mot.Magazine1Z);
            SetHomeStart(HSys.Mot.Magazine2X);
            SetHomeStart(HSys.Mot.Magazine2Z);
            SetHomeStart(HSys.Mot.Magazine3X);
            SetHomeStart(HSys.Mot.Magazine3Z);
            SetHomeStart(HSys.Mot.AutoTrolly1Z);
            SetHomeStart(HSys.Mot.AutoTrolly2Z);
            SetHomeStart(HSys.Mot.AutoTrolly3Z);
            SetHomeStart(HSys.Mot.AutoTrolly4Z);
            SetHomeStart(HSys.Mot.AutoTrolly1X);
            SetHomeStart(HSys.Mot.AutoTrolly2X);
            SetHomeStart(HSys.Mot.AutoTrolly3X);
            SetHomeStart(HSys.Mot.AutoTrolly4X);

            if(USE_PUSH_TRAY==ePushByMotor)
            {
                SetHomeStart(HSys.Mot.PushStoreArmX1);
                SetHomeStart(HSys.Mot.PushStoreArmX2);
                SetHomeStart(HSys.Mot.PushStoreArmX3);
                SetHomeStart(HSys.Mot.PushStoreArmX4);
            }
            fHome->lstHomeMsg->Items->Insert(0, "Start SortArm/MagArm X、Y Go Out home....");
            Task=7000;
            break;
        case 7000:                                                              //檔片問題 Wafer X Y 需先 JOG 離開原點
            bOK=true;

            #ifndef SOFT_SIMULATE
            if(HSys.Sen.SnMGZ2TrayPositionR.IsOff()==false)
            {
                ShowMyMessage("Magazine安全sensor is on!! 請確認SnMGZ2TrayPositionR");
                return false;
            }

            if(HSys.Sen.SnMGZ2TrayPositionF.IsOff()==false)
            {
                ShowMyMessage("Magazine安全sensor is on!! 請確認SnMGZ2TrayPositionF");
                return false;
            }

            if(HSys.Sen.SnMGZ1TrayPositionR.Enable==true &&
               HSys.Sen.SnMGZ1TrayPositionR.IsOff()==false)
            {
                ShowMyMessage("Magazine安全sensor is on!! 請確認SnMGZ1TrayPositionR");
                return false;
            }

            if(HSys.Sen.SnMGZ1TrayPositionF.Enable==true &&
               HSys.Sen.SnMGZ1TrayPositionF.IsOff()==false)
            {
                ShowMyMessage("Magazine安全sensor is on!! 請確認SnMGZ1TrayPositionF");
                return false;
            }

            if(HSys.Sen.SnMGZ3TrayPositionR.Enable==true &&
               HSys.Sen.SnMGZ3TrayPositionR.IsOff()==false)
            {
                ShowMyMessage("Magazine安全sensor is on!! 請確認SnMGZ3TrayPositionR");
                return false;
            }

            if(HSys.Sen.SnMGZ3TrayPositionF.Enable==true &&
               HSys.Sen.SnMGZ3TrayPositionF.IsOff()==false)
            {
                ShowMyMessage("Magazine安全sensor is on!! 請確認SnMGZ3TrayPositionF");
                return false;
            }
            #endif

            for(int i=0; i<HSys.iTotalMotor; i++)
            {
                if(HSys.MotPtr[i]->bHomeFinish==false)
                {
                    bHomeOrder=true;

                    if(HSys.MotPtr[i]->HomeOrder->Count==0)
                    {
                        bHomeOrder=true;
                    }
                    else
                    {
                        for(int j=0; j<HSys.MotPtr[i]->HomeOrder->Count; j++)
                        {
                            for(int k=0; k<HSys.iTotalMotor; k++)
                            {
                                if(HSys.MotPtr[i]->HomeOrder->Strings[j]==HSys.MotPtr[k]->Alias)
                                {
                                    if(HSys.MotPtr[k]->bHomeFinish==false)
                                    {
                                        bHomeOrder=false;
                                    }
                                }
                            }
                        }
                    }
                    AnsiString sErr="";
                    if(bHomeOrder && HSys.MotPtr[i]->Home(sErr))
                    {
                        SetHomeFinish(HSys.MotPtr[i]);
                    }
                    else
                    {
                        fHome->ShowLed(HSys.MotPtr[i]->Tag,   fHome->eHomeBusy);
                        bOK=false;
                    }
                }
            }
            if(bOK)
            {
                bflag[0]=false;
                bflag[1]=false;
                bflag[2]=false;
                bflag[3]=false;
                bflag[4]=false;
                bflag[5]=false;
                bflag[6]=false;
                bflag[7]=false;
                bflag[8]=false;

                if(MAGAZINE_CNT==3)
                {
                    HSys.Mot.Magazine1Z->SetPersentSpeed(60);
                    HSys.Mot.Magazine3Z->SetPersentSpeed(60);
                }
                HSys.Mot.Magazine2Z->SetPersentSpeed(60);                        //Sam 20240525 : Magazine 2Z Home 偷跑
                HSys.Mot.SortArm1X->SetPersentSpeed(20);
                HSys.Mot.SortArm1Y->SetPersentSpeed(20);
                HSys.Mot.SortArm2X->SetPersentSpeed(20);
                HSys.Mot.SortArm2Y->SetPersentSpeed(20);

                for(int i=0; i<eMGZTotal; i++)
                    iZpos[i]=0;

                Task=10000;
            }
            break;
        case 10000:
            if(MagazineModule[eMagazine2]->CheckAllMagHasTray()==true || MagazineModule[eMagazine2]->MGZTrayArm->fHasTray)
            {
                iZpos[eMagazine2]=MagazineModule[eMagazine2]->GetLayerPosition(eMag27);
            }
            else
            {
                iZpos[eMagazine2]=MagazineModule[eMagazine2]->Z_TopPos;
            }

            if(MAGAZINE_CNT==3)
            {
                if(MagazineModule[eMagazine1]->CheckAllMagHasTray()==true ||
                   MagazineModule[eMagazine1]->MGZTrayArm->fHasTray)
                {
                    iZpos[eMagazine1]=MagazineModule[eMagazine1]->GetLayerPosition(eMag27);
                }
                else
                {
                    iZpos[eMagazine1]=MagazineModule[eMagazine1]->Z_TopPos;
                }

                if(MagazineModule[eMagazine3]->CheckAllMagHasTray()==true ||
                   MagazineModule[eMagazine3]->MGZTrayArm->fHasTray)
                {
                    iZpos[eMagazine3]=MagazineModule[eMagazine3]->GetLayerPosition(eMag27);
                }
                else
                {
                    iZpos[eMagazine3]=MagazineModule[eMagazine3]->Z_TopPos;
                }
            }

            if(bflag[0]==false)
                bflag[0]=HSys.Mot.SortArm1X->MotorMove(SortArmPara->iPos_Loader_X);
            if(bflag[1]==false)
                bflag[1]=HSys.Mot.SortArm1Y->MotorMove(SortArmPara->iPos_Loader_Y);
            if(bflag[2]==false)
                bflag[2]=HSys.Mot.SortArm2X->MotorMove(MagArmPara->iPos_MagLoader_X);
            if(bflag[3]==false)
                bflag[3]=HSys.Mot.SortArm2Y->MotorMove(MagArmPara->iPos_MagLoader_Y);
            if(bflag[4]==false)
                bflag[4]=HSys.Mot.TrayShiftStageX->MotorMove(LoaderModule->iTransferX_LD);
            if(bflag[5]==false)
                bflag[5]=HSys.Mot.LoaderCCDArmX->MotorMove(4700);               //KenHsieh 20240515 : 5000 -> 4700
            if(bflag[6]==false)
                bflag[6]=HSys.Mot.Magazine2Z->MotorMove(iZpos[eMagazine2]);

            if(MAGAZINE_CNT==3)
            {
                if(bflag[7]==false)
                    bflag[7]=HSys.Mot.Magazine1Z->MotorMove(iZpos[eMagazine1]);
                if(bflag[8]==false)
                    bflag[8]=HSys.Mot.Magazine3Z->MotorMove(iZpos[eMagazine3]);
            }
            else
            {
                bflag[7]=true;
                bflag[8]=true;
            }
            if(bflag[0] &&
                bflag[1]&&
                bflag[2]&&
                bflag[3]&&
                bflag[4]&&
                bflag[5]&&
                bflag[6]&&
                bflag[7]&&
                bflag[8])
            {
                bflag[0]=false;
                bflag[1]=false;
                bflag[2]=false;
                bflag[3]=false;
                bflag[4]=false;
                bflag[5]=false;
                bflag[6]=false;
                Task=11000;
            }
            break;
        case 11000:                                                             //WR_Fix 插，然後 Wafer_Z Home
            if(MagazineModule[eMagazine2]->CheckAllMagHasTray()==true || MagazineModule[eMagazine2]->MGZTrayArm->fHasTray)
            {
                ret=ShowMyMessageBox_YES_NO("請確認是否已將Magazine top及電梯的tray都取下??");
                if(ret==TMyMessageBox::msgrtnYES)
                {
                    for(int i=eMag2_01; i<eMag2_27; i++)
                    {
                        Auto->MAutoTray[i]->ClearTray();
                        Auto->MAutoTray[i]->Tray.iBin=0;
                    }
                    bMagNeedAutoLoadTray=true;
                    bInitialStartCheckMagLayer=true;

                    HSys.Sys.SystemStart=true;
                    if(MAGAZINE_CNT==3)
                        Task=11100;
                    else
                        Task=13000;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if(MAGAZINE_CNT==3)
                    Task=11100;
                else
                    Task=13000;
            }

            break;
        case 11100:                                                             //WR_Fix 插，然後 Wafer_Z Home
            if(MagazineModule[eMagazine1]->CheckAllMagHasTray()==true || MagazineModule[eMagazine1]->MGZTrayArm->fHasTray)
            {
                ret=ShowMyMessageBox_YES_NO("請確認是否已將Magazine1 top及電梯的tray都取下??");
                if(ret==TMyMessageBox::msgrtnYES)
                {
                    for(int i=eMag1_01; i<eMag1_27; i++)
                    {
                        Auto->MAutoTray[i]->ClearTray();
                        Auto->MAutoTray[i]->Tray.iBin=0;
                    }
                    bMagNeedAutoLoadTray=true;
                    bInitialStartCheckMagLayer=true;

                    HSys.Sys.SystemStart=true;
                    Task=11150;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                Task=11150;
            }

            break;
        case 11150:                                                             //WR_Fix 插，然後 Wafer_Z Home
            if(MagazineModule[eMagazine3]->CheckAllMagHasTray()==true || MagazineModule[eMagazine3]->MGZTrayArm->fHasTray)
            {
                ret=ShowMyMessageBox_YES_NO("請確認是否已將Magazine top及電梯的tray都取下??");
                if(ret==TMyMessageBox::msgrtnYES)
                {
                    for(int i=eMag3_01; i<eMag3_27; i++)
                    {
                        Auto->MAutoTray[i]->ClearTray();
                        Auto->MAutoTray[i]->Tray.iBin=0;
                    }
                    bMagNeedAutoLoadTray=true;
                    bInitialStartCheckMagLayer=true;

                    HSys.Sys.SystemStart=true;
                    Task=13000;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                Task=13000;
            }
            break;
        case 13000:
            fSpeed->ResetAllMotorSpeed();
            SetMotorSpeed(true);
            fHome->Close();
            bResult = true;
            break;
    }
    return bResult;
}
//==============================================================================
bool ProcessOnlyVoiceMotorHome(bool Flag2)                                               // only voice motor home //
{
    static int iPowerCount=0;                                                   //馬達斷送電計時
    static bool bflag[2];
    static Word OldSystemSec;

    int &Task=fHome->iVoiceHomeStep;
    bool bResult=false;

    if(HSys.Sys.SystemStart==false)                                             //Press pause 20111124 Daver add
    {
        HSys.DecStopAllMotor();
        fHome->Close();
        Task=1;
        return false;
    }

    switch(Task)
    {
        case 1:
            fHome->Show();
            fHome->lstHomeMsg->Clear();
            fHome->lstHomeMsg->Items->Insert(0, "Starting Only Voice Motor procedure....");
            Task=1000;
            break;

        case 1000:
//            HSys.Sw.VoicePower.On();                                             //Voice Motor 斷電
            fHome->lstHomeMsg->Items->Insert(0, "Voice Motor Power off....");
            iPowerCount=15;                                                     //斷電 15 秒
            OldSystemSec=-1;
            #ifdef SOFT_SIMULATE
                ResetOKDeleyTime.SetSecAndOn(0.1);
            #else
                ResetOKDeleyTime.SetSecAndOn(iPowerCount);
            #endif
            Task=2000;
            break;
        case 2000:
            if(ResetOKDeleyTime.Off())                                          //斷電計數結束
            {
//                HSys.Sw.VoicePower.Off();                                        //馬達送電
                fHome->lstHomeMsg->Items->Insert(0, "Voice Power on....");

                iPowerCount=3;                                                  //送電 3 秒
                OldSystemSec=-1;
                #ifdef SOFT_SIMULATE
                    ResetOKDeleyTime.SetSecAndOn(0.1);
                #else
                    ResetOKDeleyTime.SetSecAndOn(iPowerCount);
                #endif
                Task=3000;
            }
            else                                                                //Power Down Count
            {
                if(iPowerCount>=0)
                {
                    if(OldSystemSec!=SystemSec)
                    {
                        fHome->lstHomeMsg->Items->Insert(0, "Voice Motor Power on count down : "+AnsiString(iPowerCount));
                        iPowerCount--;
                        OldSystemSec=SystemSec;
                    }
                }
            }
            break;
        case 3000:                                                              //馬達 server On
            if(ResetOKDeleyTime.Off())                                          //送電計數結束
            {
                bflag[0]=false;
                bflag[1]=false;
                Task=4000;
            }
            break;
        case 4000:
            fSpeed->ResetAllMotorSpeed();
            SetMotorSpeed(true);
            fHome->Close();
            bResult = true;
            break;
    }
    return bResult;
}

//==============================================================================
void __fastcall TfHome::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;
//    Top =(1024-Height)/2;
    Top =0;
    fShow=true;
    fAbort=false;
    SetWidth();
}
//==============================================================================
void __fastcall TfHome::SpeedButton1Click(TObject *Sender)
{
    HSys.StopAllMotor();                                                        // BEN EDIT 20110822 //
    HSys.Sys.bNeedHome=true;
    fAllMotorHome=false;
    SoftStop=true;                                                              //V1.9
    Close();
}
//==============================================================================
void __fastcall TfHome::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
}
//==============================================================================
void __fastcall TfHome::ScanKey()
{
    int Key;//,i;
    Key=ScanPannelKey();
    /*                                                                          // ben 20110630 -s //
    if(Key==HSys.Sen.SnPause.Tag)
    {
        SpeedButton1Click(this);
        Close();
    }
    */
    if(Key==HSys.Sen.SnFKPause.Tag || Key==HSys.Sen.SnRKPause.Tag)
    {
        SpeedButton1Click(this);
        Close();
    }                                                                           // ben 20110630 -e //
}
//==============================================================================
void __fastcall TfHome::Timer1Timer(TObject *Sender)
{
    if(fShow==false)
        return;
    ScanKey();

    for (int i=0;i<HSys.iTotalMotor;i++)                                        // Daver add
    {
        fHome->ShowMotorHomePos(i);
    }
}
//==============================================================================

