//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "MyKitSuck.h"
#pragma package(smart_init)

//==============================================================================
//
//==============================================================================
_fastcall TMySucker::TMySucker()
{
    Enable=false;
    Status=false;
    Task=1;
    iMyBravoSuckTask=1;
    OnAlarmCode=0;
    OffAlarmCode=0;
    OnAlarmTime=100;
    OffAlarmTime=100;
    OnDelayTime=0;
    OffDelayTime=0;

    Delay.Clear();
    Error=false;
    VacuumOnTime=120;          // 75    100    110
    VacuumOffTime=70;          // 35     50      60
                               // 3400  3300
    SuckerName="";
    for(int i=0; i<20; i++)
    {
        VacuumOnTimeBuffer[i]=-1;
        VacuumOffTimeBuffer[i]-1;
    }
    RealTimeRefreshVacuumOnOffTime=false;//Eliot 2008_04_17
    iSuckType=eSuckTypePtoV;
    bSuckOk=true;
};
//------------------------------------------------------------------------------
void TMySucker::PushOnTime()
{
    int ct=0, diff;
    DWORD iSum=0, tsum=0;
    for(int i=0; i<19; i++)
        VacuumOnTimeBuffer[i]=VacuumOnTimeBuffer[i+1];
    VacuumOnTimeBuffer[19]=VacuumOnTime;
    for(int i=0; i<20; i++)
    {
        if(int(VacuumOnTimeBuffer[i])!=-1)
        {
            ct++;
            iSum+=VacuumOnTimeBuffer[i];
        }
    }
    if(ct)
        iSum/=ct;
    else
        return;
    ct=0;
    for(int i=0; i<20; i++)
    {
        diff=abs(iSum-VacuumOnTimeBuffer[i]);
        if(diff<100)
        {
            tsum+=VacuumOnTimeBuffer[i];
            ct++;
        }
    }
    if(ct)
        VacuumOnTime=tsum/ct;
}
//------------------------------------------------------------------------------
void TMySucker::PushOffTime()
{
    int ct=0, diff;
    DWORD iSum=0,tsum=0;
    for(int i=0; i<19; i++)
        VacuumOffTimeBuffer[i]=VacuumOffTimeBuffer[i+1];
    VacuumOffTimeBuffer[19]=VacuumOffTime;
    for(int i=0; i<20; i++)
    {
        if(int(VacuumOffTimeBuffer[i])!=-1)
        {
            ct++;
            iSum+=VacuumOffTimeBuffer[i];
        }
    }
    if(ct)
        iSum/=ct;
    else
        return;
    ct=0;
    for(int i=0; i<20; i++)
    {
        diff=abs(iSum-VacuumOffTimeBuffer[i]);
        if(diff<100)
        {
            tsum+=VacuumOffTimeBuffer[i];
            ct++;
        }
    }
    if(ct)
        VacuumOffTime=tsum/ct;
}
//------------------------------------------------------------------------------
void TMySucker::Reset()
{
    iMyBravoSuckTask=1;
    Task=1;
    Delay.Clear();
    Error=false;
}
//------------------------------------------------------------------------------
bool TMySucker::GetOnBit()
{
    return OnSw.OutValue;
}
//------------------------------------------------------------------------------
bool TMySucker::GetOffBit()
{
    return OffSw.OutValue;
}
//------------------------------------------------------------------------------
bool TMySucker::GetStatus()
{
    #ifdef SOFT_SIMULATE
        return true;
    #else
        if(HSys.LastSet.iRealDummy==DUMMY)
            return true;
        else
            return Sensor.IsOn();
    #endif
}
//------------------------------------------------------------------------------
void TMySucker::OnSuck()                                                        // no delay,no alarm
{
    if(OffSw.Status())
    {
        AnsiString Str;
        Str.sprintf("%s turn suck on while destroy is on.", SuckerName);
        fMain->AddSuckMessage(Str);
    }
    OnSw.On();
}
//------------------------------------------------------------------------------
void TMySucker::OffSuck()                                                       // no delay,no alarm
{
    OnSw.Off();
}
//------------------------------------------------------------------------------
void TMySucker::OnDestroy()                                                     // no delay,no alarm
{
    if(OnSw.Status())
    {
        AnsiString Str;
        Str.sprintf("%s turn destroy on while suck is on.", SuckerName);
        fMain->AddSuckMessage(Str);
    }
    OffSw.On();
}
//------------------------------------------------------------------------------
void TMySucker::OffDestroy()                                                    // no delay,no alarm
{
    OffSw.Off();
}
//------------------------------------------------------------------------------
void TMySucker::On()                                                            // no delay,no alarm
{
    OffDestroy();                                                               // 關閉破壞
    OnSuck();                                                                   // 打開真空
}
//------------------------------------------------------------------------------
void TMySucker::Off()                                                           // no delay,no alarm
{
    OffSuck();                                                                  // 關閉真空
    OnDestroy();                                                                // 打開破壞
}
//------------------------------------------------------------------------------
void TMySucker::Normal()
{
    OffSuck();
    OffDestroy();
}
//------------------------------------------------------------------------------
bool TMySucker::Suck()
{
    if(Error==true)
    {
        bSuckOk=true;
        Task=1;
        return false;
    }

    if(HSys.LastSet.iRealDummy!=REALLY)
    {
        switch(Task)
        {
            case 1:
                bSuckOk=false;
                Delay.SetMS(OnDelayTime);
                Delay.On();
                On();
                Task=2;
            case 2:
                if(Delay.Off())
                {
                    bSuckOk=true;
                    Normal();
                    Task=1;
                    return true;
                }
                break;
        }
        return false;
    }

    if(Task!=200)
    {
        bSuckOk=false;
        On();
    }

    if(Task==1 || Task==2)
    {
        if(Enable) //  has install onsensor
        {
            if(OnDelayTime==0)
            {
                if(GetStatus()) // ok
                {
                    Task=100;      // do on delay
                }
                else
                {
                    Task=1;
                    OffSuck();
                    Error=true;
                    bSuckOk=true;
                    return false;
                }
            }
            else
            {
                StartTick=GetTickCount();
                if(GetStatus())     // already on
                {
                    Task=100;
                }
                else    //異常!
                {
                    Delay.Clear();
                    Delay.SetMS(OnAlarmTime);
                    Delay.On();
                    Task=50;
                }
            }
        }
        else
        {
            Task=100;      //need delay
        }
    }

    if(Task==50)    //異常!
    {
        if(GetStatus())
        {
            Task=100;      // do on delay
        }
        else
        {
            if(Delay.Off())
            {
                OffSuck();
                Task=200;       //異常!
                Delay.Clear();
                Delay.SetMS(10);
                Delay.On();
                //Error=true;
                return false;
            }
            else
            {
                return false;
            }
        }
    }

    if(Task>=100)
    {
        switch(Task)
        {
            case 100:       //正常吸取
                EndTick=GetTickCount();
                VacuumOnTime=EndTick-StartTick;
                if(RealTimeRefreshVacuumOnOffTime)
                {
                    PushOnTime();
                }
                if(OnDelayTime==0)
                {
                    Task=1;
                    Error=false;
                    bSuckOk=true;
                    return true;
                }
                else        //吸取Delay
                {
                    Delay.Clear();
                    Delay.SetMS(OnDelayTime);
                    Delay.On();
                    Task=101;
                }
            case 101:
                if(Delay.Off())
                {
                    Error=false;
                    Task=1;
                    bSuckOk=true;

//                    if(iSuckType==eSuckTypeVtoV)
//                    {
//                        Normal();
//                    }
                    return true;
                }
                return false;
            case 200:       //異常!
                if(Delay.Off())
                {
                    if(iSuckType==eSuckTypeVtoV)
                    {
                        Normal();
                    }
                    Error=true;
                    Task=1;
                    bSuckOk=true;
                    return false;
                }
                return false;
        }
    }
    Error=false;
    Task=1;
    bSuckOk=true;
    return true;
}
//------------------------------------------------------------------------------
bool TMySucker::Destroy()
{
    AnsiString Str;
    bool InRet;
    //不能直接使用 On() 因為會直接改變 Status
    //Error=false;
    if(Error==true)
    {
        Task=1;
        return false;
    }
    if(HSys.LastSet.iRealDummy==DUMMY)
    {
        switch(Task)
        {
            case 1:
                Delay.Clear();
                if( VacuumOffTime>1000 )       // 2007_04_24 lee start
                    VacuumOffTime=50;
                Delay.SetMS(VacuumOffTime);
                Delay.On();
                Off();
                Task=2;
            case 2:
                if( Delay.Off() )
                {
                    Delay.Clear();
                    Delay.SetMS(OffDelayTime);
                    Delay.On();
                    Task=50;
                }
                break;
            case 50:
                if( Delay.Off() )
                {
                    Normal();
                    Task=1;
                    return true;
                }
                break;
        }
        return false;
    }
    Off();
    if(Task==1 ||Task==2)
    {
        if(Enable) //  has install onsensor
        {
            if(OffAlarmTime==0)
            {
                InRet=GetStatus();
                if(InRet==false)
                {      // ok
                    Task=100;      // do on delay
                }
                else
                {
                    Task=1;
                    OffDestroy();

                    Str.sprintf("%s is destroy error (1).", SuckerName);
                    fMain->AddSuckMessage(Str);

                    Error=true;
                    return false;
                }
            }
            else
            {
                StartTick=GetTickCount();
                if(GetStatus()==false)     // already off
                {
                    Task=100;
                }
                else
                {
                    Delay.Clear();
                    Delay.SetMS(OffAlarmTime);
                    Delay.On();
                    Task=50;
                }
            }
        }
        else
        {
            Task=100;      //need delay
        }
    }
    if(Task==50)
    {
        InRet=GetStatus();
        if(InRet==false)
        {   // ok
            Task=100;      // do on delay
        }
        else
        {
            if(Delay.Off())
            {
                Task=1;      //
                OffDestroy();
                Task=200;      //
                Delay.Clear();
                Delay.SetMS(10);
                Delay.On();
                return false;
            }
            else
                return false;
        }
    }
    if(Task>=100)
    {
        switch(Task)
        {
            case 100:
                EndTick=GetTickCount();
                VacuumOffTime=EndTick-StartTick;
                if(RealTimeRefreshVacuumOnOffTime)
                {
                    PushOffTime();
                }
                if(OffDelayTime==0)
                {
                    Task=1;
                    OffDestroy();
                    Error=false;
                    return true;
                }
                else
                {
                    Delay.Clear();
                    Delay.SetMS(OffDelayTime);
                    Delay.On();
                    Task=101;
                }
            case 101:
                if(Delay.Off())
                {
                    Task=1;
                    OffDestroy();
                    Error=false;
                    return true;
                }
                return false;
            case 200:
                if(Delay.Off())
                {
                    Str.sprintf("%s is destroy error (2).", SuckerName);
                    fMain->AddSuckMessage(Str);
                    Error=true;
                    Task=1;
                    return false;
                }
                break;

        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMySucker::Destroy(bool bDelayOff)
{
    bool InRet;
    //不能直接使用 On() 因為會直接改變 Status
    //Error=false;
    if(Error==true)
    {
        Task=1;
        return false;
    }
    if(HSys.LastSet.iRealDummy==DUMMY)
    {
        switch(Task)
        {
            case 1:
                Delay.Clear();
                if(VacuumOffTime>1000)       // 2007_04_24 lee start
                    VacuumOffTime=50;
                Delay.SetMS(VacuumOffTime);
                Delay.On();
                Off();
                Task=2;
            case 2:
                if(Delay.Off())
                {
                    Delay.Clear();
                    Delay.SetMS(OffDelayTime);
                    Delay.On();
                    Task=50;
                }
                break;
            case 50:
                if(Delay.Off())
                {
                    Task=1;
                    Normal();
                    return true;
                }
                break;
        }
        return false;
    }
    Off();
    if(Task==1 ||Task==2)
    {
        if(Enable) //  has install onsensor
        {
            if(OffAlarmTime==0)
            {
                InRet=GetStatus();
                if(InRet==false)
                {      // ok
                    Task=100;      // do on delay
                }
                else
                {
                    Task=1;
                    OffDestroy();
                    Error=true;
                    return false;
                }
            }
            else
            {
                StartTick=GetTickCount();
                if(GetStatus()==false)     // already off
                {
                    Task=100;
                }
                else
                {
                    Delay.Clear();
                    Delay.SetMS(OffAlarmTime);
                    Delay.On();
                    Task=50;
                }
            }
        }
        else
        {
            Task=100;      //need delay
        }
    }
    if(Task==50)
    {
        InRet=GetStatus();
        if(InRet==false)
        {   // ok
            Task=100;      // do on delay
        }
        else
        {
            if(Delay.Off())
            {
                Task=1;      //
                OffDestroy();
                Task=200;      //
                Delay.Clear();
                Delay.SetMS(10);
                Delay.On();
                return false;
            }
            else
                return false;
        }
    }
    if(Task>=100)
    {
        switch(Task)
        {
            case 100:
                EndTick=GetTickCount();
                VacuumOffTime=EndTick-StartTick;
                if(RealTimeRefreshVacuumOnOffTime)
                {
                    PushOffTime();
                }
                if(OffDelayTime==0)
                {
                    Task=1;
                    if(bDelayOff==false)
                        OffDestroy();     //2012-10-24    Dell 由自己下off()關閉真空
                    Error=false;
                    return true;
                }
                else
                {
                    Delay.Clear();
                    Delay.SetMS(OffDelayTime);
                    Delay.On();
                    Task=101;
                }
            case 101:
                if(Delay.Off())
                {
                    Task=1;
                    if(bDelayOff==false)
                        OffDestroy();     //2012-10-24    Dell 由自己下off()關閉真空
                    Error=false;
                    return true;
                }
                return false;
            case 200:
                if(Delay.Off())
                {
                    Error=true;
                    Task=1;
                    return false;
                }
                break;

        }
    }
    return false;
}
//------------------------------------------------------------------------------
void  TMySucker::ResetSuckTask()
{
    if(Task!=1)
        Task=2;
}
//------------------------------------------------------------------------------
void TMySucker::CheckIsFallDown()
{
    if(Enable==false || Task>2)
        return;
    if(Sensor.IsOn()==false) //if( Sensor.Status()==false)   V1.6
        Normal();
}
//---------------------------------------------------------------------------
bool TMySucker::HasRealIC()
{
    return !NoRealIC();
}
//---------------------------------------------------------------------------
bool TMySucker::NoRealIC()
{
    if(Item==NULL_IC          ||
       Item==HAS_NULL_IC      ||
       Item==NULL_IC_TEST     ||
       Item==HAS_NULL_IC_TEST)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
// 2009/09/03 Lee
// Version 2.0
//---------------------------------------------------------------------------
__fastcall TMyKitSuck::TMyKitSuck()
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            Suck[i][j].Item=NULL_IC;
            Suck[i][j].pLed=NULL;
        }
    }
}
//---------------------------------------------------------------------------
__fastcall TMyKitSuck::~TMyKitSuck()
{
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::SetMyLed(int iR, int iC, TMyLed *ledPtr)
{
    Suck[iR][iC].pLed=ledPtr;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::SetItemData(int iR, int iC, int Data)
{
    Suck[iR][iC].Item=Data;
    Suck[iR][iC].DeviceInfo.Clear();
    if(Suck[iR][iC].pLed!=NULL && Suck[iR][iC].pLed->Visible)
    {
        if(Data!=NULL_IC)
        {
            Suck[iR][iC].pLed->TrueColor=GetDeviceColor(Data);
            Suck[iR][iC].pLed->Value=true;
        }
        else
        {
            Suck[iR][iC].pLed->Value=false;
        }
    }
}
//---------------------------------------------------------------------------
void TMyKitSuck::SetItemData(int iR, int iC, int Data, TDeviceInfo DInfo)
{
    SetItemData(iR, iC, Data);
    Suck[iR][iC].DeviceInfo.CopyFrom(DInfo);
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::SetItemAmount(int iRow, int iCol)
{
    if(iRow>MAX_SUCKER_ROW ||
       iCol>MAX_SUCKER_COL)
    {
        ShowMessage("Suck amount over system setup");
        MaxItemR=MAX_SUCKER_ROW;
        MaxItemC=MAX_SUCKER_COL;
    }
    else
    {
        MaxItemR=iRow;
        MaxItemC=iCol;
    }
//    iTotalSubSucker=MaxItem;
};
//---------------------------------------------------------------------------
void TMyKitSuck::initMyKitSuck(AnsiString sName, AnsiString sFlushPanel, int iRow, int iCol)
{
    Name=sName.Trim();
    FlushPanelName=sFlushPanel.Trim();
    SetItemAmount(iRow, iCol);
    MaxItem =MaxItemR*MaxItemC;
    AnsiString Str;

    for(int iR=0; iR<MaxItemR; iR++)
    {
        for(int iC=0; iC<MaxItemC; iC++)
        {
            Suck[iR][iC].Tag=iR*10+iC;
            Str.sprintf("%c%c", 'A'+iR, 'a'+iC);
            Suck[iR][iC].Alias=Str;
            Suck[iR][iC].iMyRow=iR;
            Suck[iR][iC].iMyCol=iC;
            Suck[iR][iC].SensorName=Name+Suck[iR][iC].Alias;
        }
    }

    AlarmName[ePickUpErr   ]=Name;
    AlarmName[eVacSenOffErr]=Name+"_Off";
    AlarmName[eDestoryErr  ]=Name+"_Destory";
    AlarmName[eDropErr     ]=Name+"_Drop";
    AlarmName[eInitOffErr  ]=Name+"_InitOff";
    AlarmName[eInitOnErr   ]=Name+"_InitOn";
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
bool TMyKitSuck::NoIC()
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            if(Suck[i][j].Item!=NULL_IC && Suck[i][j].Item!=NULL_IC_TEST)
                return false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
bool TMyKitSuck::HasIC()
{
    return !NoIC();
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
bool TMyKitSuck::HasType(int p)
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            if(Suck[i][j].Item==p)
                return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
int TMyKitSuck::GetCountOfDeiveType(int data)
{
    int sum=0;
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            if(Suck[i][j].Item==data)
                sum++;
        }
    }
    return sum;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
bool TMyKitSuck::AllIs_HAS_NULL_IC()
{
    return GetCountOfDeiveType(HAS_NULL_IC)==MaxItem;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
bool TMyKitSuck::HasRealIC()
{
    if( GetCountOfDeiveType(NULL_IC)==0          &&
        GetCountOfDeiveType(HAS_NULL_IC)==0      &&
        GetCountOfDeiveType(NULL_IC_TEST)==0     &&
        GetCountOfDeiveType(HAS_NULL_IC_TEST)==0 )
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::ClearSingle(int iR, int iC)
{
    SetItemData(iR, iC, NULL_IC);
    Suck[iR][iC].DeviceInfo.Clear();
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::ClearAll()
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            ClearSingle(i, j);
        }
    }
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::SetAll(int Type)
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            SetItemData(i, j, Type);
        }
    }
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
bool TMyKitSuck::HasTestedDevice()
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            if(Suck[i][j].Item>=TEST_PASS)
                return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
bool TMyKitSuck::HasDeviceNotTested()
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            if(Suck[i][j].Item<TEST_PASS)
                return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::MoveSingleItem(TMyKitSuck &Source, int iR, int iC)
{
    CopySingleItem(Source,iR,iC);
    Source.SetItemData(iR, iC, NULL_IC);
}
//---------------------------------------------------------------------------
void TMyKitSuck::MoveSingleItem(TTrayMotor *MotPtr, int iR, int iC, int X, int Y)
{
    SetItemData(iR, iC, MotPtr->Tray.Data[X][Y], MotPtr->Tray.DeviceInfo[X][Y]);
    MotPtr->SetTraySingleData(X, Y, NULL_IC);
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::CopySingleItem(TMyKitSuck &Source, int iR, int iC)
{
    SetItemData(iR, iC, Source.Suck[iR][iC].Item);
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
//void TMyKitSuck::MoveAllItem(class TMyKitSuck &Source)
//{
//    int Min;
//    if(Source.MaxItem>=MaxItem)
//        Min=MaxItem;
//    else
//        Min=Source.MaxItem;
//    for(i=0;i<Min;i++)
//        MoveSingleItem(Source,i);
//}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
//void TMyKitSuck::CopyFrom(class TMyKitSuck &Source)
//{
//    int Min, i;
//    if(Source.MaxItem >= MaxItem )     Min=MaxItem;
//    else                                Min=Source.MaxItem;
//    for(i=0; i<Min; i++)
//        CopySingleItem(Source,i);
//}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::MoveSuckData(TMyKitSuck &Source, int iR, int iC)
{
    MoveSingleItem(Source, iR, iC);
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
//void TMyKitSuck::MoveSuckDataDiff(class TMyKitSuck &Source, int iR, int iC, int tag)
//{
//    SetItemData(tag, Source.Suck[i].Item);
//    Source.SetItemData(i, NULL_IC);
//    Suck[tag].DeviceInfo.MoveFrom(Source.Suck[i].DeviceInfo);
//}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
bool TMyKitSuck::AllDeviceTested()
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            if(Suck[i][j].Item<TEST_PASS)
                return false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
void TMyKitSuck::ResetAll()
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            Suck[i][j].ResetSuckTask();
        }
    }
    ClearAllError();                                                            //JerryYang 20240623 : pick up error可以SKIP
}
//---------------------------------------------------------------------------
void  TMyKitSuck::ClearAllError()
{
    for(int i=0; i<MaxItemR; i++)
    {
        for(int j=0; j<MaxItemC; j++)
        {
            Suck[i][j].Error=false;
        }
    }
}
//---------------------------------------------------------------------------
// 2009.09.03 Lee
// Version 2.0
//---------------------------------------------------------------------------
// if sensor Off  ,off vacuum ,flag not chage
// if        On   ,if has not Real IC then off vacuum,flag=true
//                 else do not change
//---------------------------------------------------------------------------
void TMyKitSuck::CheckVaccumIsIniaialON(int iR, int iC, bool &flag)
{
    if(Suck[iR][iC].GetStatus())
    {
        if(Suck[iR][iC].Item==NULL_IC         ||
           Suck[iR][iC].Item==HAS_NULL_IC     ||
           Suck[iR][iC].Item==NULL_IC_TEST    ||
           Suck[iR][iC].Item==HAS_NULL_IC_TEST)
        {
            Suck[iR][iC].Normal();
            flag=true;
        }
    }
    else
    {
        Suck[iR][iC].Normal();
    }
}
//---------------------------------------------------------------------------
AnsiString TMyKitSuck::GetErrorPart(int AlarmStatus)
{
    AnsiString ErrorPart="";

    if(AlarmStatus==Vaccum_FallDown)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                if(Suck[i][j].Enable &&
                   Suck[i][j].HasRealIC() &&
                   Suck[i][j].GetStatus()==false)
                {
                    ErrorPart+=Suck[i][j].Alias+AnsiString(",");
                }
            }
        }
    }
    else if(AlarmStatus==Vaccum_On)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                if(Suck[i][j].Enable &&
                   Suck[i][j].Error)
                {
                    ErrorPart+=Suck[i][j].Alias+AnsiString(",");
                }
            }
        }
    }

    ErrorPart=ErrorPart.SubString(1, ErrorPart.Length()-1);
    return ErrorPart;
}
//---------------------------------------------------------------------------
void TMyKitSuck::SkipError(int AlarmStatus, int iICType)
{
    if(AlarmStatus==Vaccum_FallDown)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                if(Suck[i][j].Enable && Suck[i][j].HasRealIC() && Suck[i][j].GetStatus()==false)
                {
                    Suck[i][j].Reset();
                    if(iICType!=EMPTY_IC)
                    {
                        SetItemData(i, j, iICType);
                    }
                    else
                    {
                        SetItemData(i, j, NULL_IC);
                    }
                }
            }
        }
    }
    else if(AlarmStatus==Vaccum_On)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                if(Suck[i][j].Enable && Suck[i][j].Error)
                {
                    Suck[i][j].Reset();
                    if(iICType!=EMPTY_IC)
                    {
                        SetItemData(i, j, iICType);
                    }
                    else
                    {
                        SetItemData(i, j, NULL_IC);
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TMyKitSuck::SkipError(int AlarmStatus, TMyKitSuck &Source, int iICType)    //將有錯誤的位置改為傳入的Kit的狀態
{
    if(AlarmStatus==Vaccum_FallDown)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                if(Suck[i][j].Enable &&
                   Suck[i][j].HasRealIC() &&
                   Suck[i][j].GetStatus()==false)
                {
                    Suck[i][j].Reset();
                    Source.Suck[i][j].Reset();
                    if(iICType!=EMPTY_IC)
                    {
                        SetItemData(i, j, iICType);
                        Source.SetItemData(i, j, NULL_IC);
                    }
                    else
                    {
                        MoveSingleItem(Source, i, j);
                    }
                }
            }
        }
    }
    else if(AlarmStatus==Vaccum_On)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                if(Suck[i][j].Enable &&
                   Suck[i][j].Error)
                {
                    Suck[i][j].Reset();
                    Source.Suck[i][j].Reset();
                    if(iICType!=EMPTY_IC)
                    {
                        SetItemData(i, j, iICType);
                        Source.SetItemData(i, j, NULL_IC);
                    }
                    else
                    {
                        MoveSingleItem(Source, i, j);
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
bool TMyKitSuck::CheckHasError(int AlarmStatus,int &iSuckCol,int &iSuckRow)     //確認吸嘴有沒有出錯
{
    #ifndef SOFT_SIMULATE
    if(AlarmStatus==Vaccum_FallDown)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                if(Suck[i][j].Enable &&
                   Suck[i][j].HasRealIC() &&
                   Suck[i][j].GetStatus()==false)
                {
                    iSuckRow=i;
                    iSuckCol=j;
                    return true;
                }
            }
        }
    }
    else if(AlarmStatus==Vaccum_On)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                if(Suck[i][j].Enable &&
                   Suck[i][j].Item==NULL_IC &&
                   Suck[i][j].Error)
                {
                    iSuckRow=i;
                    iSuckCol=j;
                    return true;
                }
            }
        }
    }
    #endif
    return false;
}
//------------------------------------------------------------------------------
bool TMyKitSuck::CheckHasError(int AlarmStatus, TMyKitSuck &Source)             //確認吸嘴有沒有出錯, 並將無錯誤的進行資料交換
{
    bool bHasErr=false;
    if(AlarmStatus==Vaccum_On)
    {
        for(int i=0; i<MaxItemR; i++)
        {
            for(int j=0; j<MaxItemC; j++)
            {
                #ifdef SOFT_SIMULATE
                    if(Suck[i][j].Item==NULL_IC && Suck[i][j].Error==false)
                    {
                        MoveSingleItem(Source, i, j);
                    }
                #else
                    if(Suck[i][j].Enable && Suck[i][j].Item==NULL_IC && Suck[i][j].Error==false)
                    {
                        MoveSingleItem(Source, i, j);
                    }

                    if(Suck[i][j].Enable && Suck[i][j].Item==NULL_IC && Suck[i][j].Error)
                    {
                        bHasErr=true;
                    }
                #endif
            }
        }
    }

    return bHasErr;
}
//------------------------------------------------------------------------------
