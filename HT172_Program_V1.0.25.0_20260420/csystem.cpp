
//---------------------------------------------------------------------------

#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "csystem.h"

#include "main.h"
#include "DecodeXML.h"
#include "aseTest.h"
#include "Auto172.h"
#pragma package(smart_init)
double lastc=0;
int iTesterActionTag =0;
enum {emLotStart =2 ,emLotEnd =10};

//==============================================================================
HTimer BreakDelay;
int IsEMGPressed()                                                             // check emg //
{   // need edit by user //
    int iEmg=0;
#ifndef SOFT_SIMULATE
    bool bPLCSafe=false;
    if(Enable_PLCSafety_IO)
    {
        if(bPLCIOEffect==false || bIOPowered==false)
            bPLCSafe=true;
    }
    static bool bServoOff=false;
    for(int i=0; i<4; i++)
    {
        if((HSys.SenPtr[HSys.Sen.SnEMG01.Tag+i].Enable && HSys.SenPtr[HSys.Sen.SnEMG01.Tag+i].IsOff()) || bPLCSafe)
            iEmg=HSys.SenPtr[HSys.Sen.SnEMG01.Tag+i].Tag;
    }

    if(iEmg!=0 || bPLCSafe)
    {
        bHasPushEMG=true;
        HSys.StopAllMotor();
        for(int i=0; i<HSys.iTotalMotor; i++)                                     // server on off //
        {
            if(HSys.MotPtr[i]->ReadServoAlarmOn()==true)
                HSys.MotPtr[i]->ServoOnOff(Server_Off);
        }
        bServoOff=true;
        fAllMotorHome=false;
    }
    else
    {
        if(bServoOff)
        {
            bServoOff=false;
            for(int i=0; i<HSys.iTotalMotor; i++)                                     // server on off //
            {
                if(HSys.MotPtr[i]->ReadServoAlarmOn()==true)
                    HSys.MotPtr[i]->ServoOnOff(Server_On);
            }
        }
    }

#endif
    return iEmg;
}
//==============================================================================
bool IsSafeLock()
{
    #ifdef SOFT_SIMULATE
        return false;
    #else
    bool ret=false;
    if(HSys.Sen.SnRKSafeLock.IsOff())
    {
        SoftStop=true;
        HSys.DecStopAllMotor();
        ret=true;
    }
    return ret;
    #endif
}
//==============================================================================
bool IsSystemPowerOff()                                                         // check motor power //
{
    if(HSys.Sen.SnMotorPower.IsOff() && MotorPowerOnDelay<=0)
        return true;
    return false;
}
//==============================================================================
// 此為系統檢查安全門情形
//==============================================================================
int IsSafeDoorOpen()                                                           // check safe door //
{
    int iSafeDoor=0;

    #ifndef SOFT_SIMULATE
    int iSafeDoorCount=10;

    for(int i=0; i<iSafeDoorCount; i++)
    {
        if(HSys.SenPtr[HSys.Sen.SnSafeDoor_01.Tag+i].Enable && HSys.SenPtr[HSys.Sen.SnSafeDoor_01.Tag+i].IsOff())
            iSafeDoor=HSys.SenPtr[HSys.Sen.SnSafeDoor_01.Tag+i].Tag;
    }
    #endif

    return iSafeDoor;
}
//******************************************************************************
//
//  注意!! Handler IonFan 相關, 不可任意修改!!
//
//******************************************************************************
int IsIonFanAlarm()                                                             // check IonFan //
{
    return 0;
#ifndef SOFT_SIMULATE
    AnsiString S, sErr="";
    static bool bIonFanStatus[MAX_IONFAN]={false};      //Ion Fan   上一次狀態
    bool bFan=false;                                    //IO 目前狀態
    bool bFanAlarm=false;                               //發生Ion Fan   Alarm
    //Sam 20230301 :　USE_PULSE_TYPE 改為用 Timer 方式
    //==>
    static bool bRealAlarm[MAX_IONFAN]={false};
    static TQPF_Timer tIonFanPulse[MAX_IONFAN];
    static TQPF_Timer tIonPowerPulse[MAX_IONFAN];
    static bool bFirstTime=false;
    static int iLastTime=GetTickCount(), iNowTickTime=0;

    int iIonTimer=5000;  //Sam 20230327 : Home 時強制將 ION Timeout 設定 20s
    if(fAllMotorHome)
        iIonTimer=5000;
    else
        iIonTimer=20000;


    if(bFirstTime==false)
    {
        bFirstTime=true;
        for(int i=0; i<MAX_IONFAN; i++)
        {
            bIonFanStatus[i]=false;
            bRealAlarm[i]=false;            //Sam 20230327 : Alarm flag reset
            tIonFanPulse[i].SetMSAndOn(iIonTimer);
        }
    }
    //避免 Lag 誤報警
    iNowTickTime=GetTickCount();
    if((iNowTickTime-iLastTime)>1500)   //Sam 20230327 : 將時間 Lay Reset 固定為1.5s
    {
        bFirstTime=false;
        iLastTime=iNowTickTime;
        return 0;
    }
    else
    {
        iLastTime=iNowTickTime;
    }
    for(int i=0; i<MAX_IONFAN; i++)
    {
        if(HSys.SenPtr[HSys.Sen.SnIonFan_01.Tag+i].Enable)
        {
            bFan=HSys.SenPtr[HSys.Sen.SnIonFan_01.Tag+i].IsOn();
            if(bIonFanStatus[i]!=bFan)  //ION 呼吸燈變化就重新計數
            {
                bIonFanStatus[i]=bFan;
                bRealAlarm[i]=false;
                tIonFanPulse[i].SetMSAndOn(iIonTimer);
            }
            else
            {
                if(tIonFanPulse[i].Off())
                {
                    bRealAlarm[i]=true;
                }
            }
        }
    }
    //<==
    //Sam 20230301 :　USE_PULSE_TYPE 改為用 Timer 方式
    if(HSys.Sys.SystemStart &&
       HSys.LastSet.iRealDummy==REALLY || CUSTOMER_CODE==CC_HONPREC_QC)   //Steven 20180209 : Dummy Run不檢查離子風扇
    {

        bFanAlarm=false;    //Sam 20230301 :　USE_PULSE_TYPE 改為用 Timer 方式
        for(int i=0; i<MAX_IONFAN; i++)
        {
            if(bRealAlarm[i])   //Sam 20230301 :　USE_PULSE_TYPE 改為用 Timer 方式
            {
                S.sprintf("Ion Fan Alarm: %d", i+1);                      //Steven 20150204 : 重編ESD Alarm到20
                ShowMyMessage(S);     //Steven 20221116 : Ion Alarm顯示對應位置
                bFanAlarm=true;
            }
            //==>
            //Ifor 20190412 : add KEYENCE Ion PULSE Type
        }
        if(bFanAlarm)   //Sam 20230301 :　USE_PULSE_TYPE 改為用 Timer 方式
        {
            return 1;
        }
    }
    return 0;
#endif
}
//==============================================================================
bool CountMotorPowerDelay()                                                     // 計數MOTOR POWER DELAY //
{
    #ifdef SOFT_SIMULATE
        MotorPowerOnDelay=0;
        return true;
    #else
        static int OldSecond;
        if(MotorPowerOnDelay && HSys.Sen.SnMotorPower.IsOff()==false && IsEMGPressed()==0)
        {
            if(OldSecond!=SystemSec)
            {
                OldSecond=SystemSec;
                MotorPowerOnDelay--;
            }
            if(MotorPowerOnDelay<=0)
            {
                if(AllBreakFree()==false)
                {
                    MotorPowerOnDelay=1;
                }
            }
        }
        if(MotorPowerOnDelay)
            return false;
        return true;
    #endif
}
//==============================================================================
void AllBreakLock()                                                             // break lock //    // need edit by user //
{
    HSys.DecStopAllMotor();
    HSys.Sw.SwBreak_StoreArm_Z.Off();
    HSys.Sw.SwBreak_MGZ1Arm_Z.Off();
    HSys.Sw.SwBreak_MGZ2Arm_Z.Off();
    HSys.Sw.SwBreak_MGZ3Arm_Z.Off();
}
//==============================================================================
TQPF_Timer IniBreakDelay;
bool AllBreakFree()                                                             // break free //    // need edit by user //
{
    bool bResult=true;
    HSys.Mot.StoreArmZ->ServoOnOff(Server_On);
    HSys.Mot.Magazine2Z->ServoOnOff(Server_On);
    HSys.Sw.SwBreak_StoreArm_Z.On();
    HSys.Sw.SwBreak_MGZ1Arm_Z.On();
    HSys.Sw.SwBreak_MGZ2Arm_Z.On();
    HSys.Sw.SwBreak_MGZ3Arm_Z.On();
    return bResult;
}
//------------------------------------------------------------------------------
//==============================================================================
// 此為系統檢查馬達若發生 Alarm 信號就將該軸馬達設成未歸零模式
//==============================================================================
void ScanAllMotorStatus()
{
    for (int i=0;i<HSys.iTotalMotor;i++)
    {
        HSys.MotPtr[i]->ScanMotorStatus();
        if(HSys.MotPtr[i]->Led[iAlarmLed] && HSys.MotPtr[i]->GetEnable() &&
           HSys.MotPtr[i]->ReadServoAlarmOn() && bMotorHomePowerOn)             // check alarm led
        {
            if(HSys.Sys.SystemStart==true)
            {
                if(HSys.MotPtr[i]->Led[iCwLed] || HSys.MotPtr[i]->Led[iCcwLed])
                    return;
                if(HSys.Sys.RunMode==Run_Home && fHome->bCheckMotorPower==true)
                {
                    if(fHome->iHomeStep>1000)   //避免歸零時,關電按暫停,然後再開始就會Alarm
                    {
                        int iRef=HSys.MotPtr[i]->GetErrorIndex();
                        if(iRef==9)
                            iRef=6;
                        AnsiString S=AnsiString().printf("%d%03d%1d",(int)eMotorAlarm, i, iRef);        // motor error  10000 Start //// ben edit 20110719 //
                        ShowMotorError(S,__FUNC__);                                      // ben edit 20110719 //
                        fAllMotorHome=false;
                        HSys.Sys.SystemStart=false;
                    }
                }
            }
            else
            {
                if(fAllMotorHome==true)
                {
                    AnsiString sMotName=HSys.MotPtr[i]->NumberAlias;
                    ShowMyMessage(AnsiString().printf("Motor %s Home flag reset.Func=%s AlarmLed=%d ,ReadServoAlarmOn=%d",sMotName,__FUNC__,HSys.MotPtr[i]->Led[iAlarmLed], HSys.MotPtr[i]->ReadServoAlarmOn()));
                }
                HSys.MotPtr[i]->bHomeFlag=false;
                fAllMotorHome=false;
                HSys.Sys.SystemStart=false;
                return;
            }
        }
    }
}
//==============================================================================
// 檢查當畫面不在 IOSETVIEW 或暫停時面板 [Power On] [Power Off]情形
//==============================================================================
void CheckMotorPowerShutDown()
{
    static int DelayMotNo=0;                                                    //V1.9
    static bool MOTPower=false,flag=true;
    bool bOn=false,bOff=false;
    static bool bKeyPowerOffPressed=false; //Steven 20100225 : 防止Relay一直跳!

    if(flag && DelayMotNo>=100)
    {
        AllBreakLock();
        HSys.Sw.SwMotorRelay.On();                                              // ben disable 20111018 //
        for(int i=0;i<HSys.iTotalMotor;i++)                                     // server on off //
        {
            if(HSys.MotPtr[i]->ReadServoAlarmOn()==true)
                HSys.MotPtr[i]->ServoOnOff(Server_On);
        }
        #ifdef SOFT_SIMULATE
            MotorPowerOnDelay=0;
        #else
            MotorPowerOnDelay=SERVER_MOTOR_POWER_ON_DELAY;
        #endif
        bMotorPowerState=true;
        flag=false;
        MOTPower=true;
    }
    if(flag && DelayMotNo<100)
        DelayMotNo++;


    bOn=HSys.Sen.SnRKPowerOn.Status();
    bOff=HSys.Sen.SnRKPowerOff.Status();

    if((bOff && bOn==false))
    {
        if(bKeyPowerOffPressed)
        {
            AllBreakLock() ;
            HSys.Sw.SwMotorRelay.Off();
            bMotorPowerState=false;
        }
    }
    else if(bMotorPowerState==false && bOn)
    {
        bMotorPowerState=true;
        HSys.Sw.SwMotorRelay.On();
        AllBreakLock() ;
        MotorPowerOnDelay=SERVER_MOTOR_POWER_ON_DELAY;

    }
    if(bMotorPowerState)
    {
        bLampPowerOn=true;
        bLampPowerOff=false;
        bKeyPowerOffPressed=true;
    }
    else
    {
        fAllMotorHome=false;
        bLampPowerOn=FlushFlag;
        bLampPowerOff=true;
        bKeyPowerOffPressed=false;
    }
}
//==============================================================================
void ProcessSuckDuringPause()
{
    if(InitialOK==false)
        return;
    if(HSys.Sys.SystemStart==false)
    {
        for(int i=0; i<HSys.iTotalSucker; i++)
        {
            for(int iR=0; iR<HSys.SuckPtr[i].MaxItemR; iR++)
            {
                for(int iC=0; iC<HSys.SuckPtr[i].MaxItemC; iC++)
                {
                    if(HSys.SuckPtr[i].Suck[iR][iC].iSuckType==eSuckTypeVtoV)
                    {
                        if(HSys.SuckPtr[i].Suck[iR][iC].bSuckOk==false)
                        {
                            HSys.SuckPtr[i].Suck[iR][iC].Suck();
                        }
                    }
                }
            }
        }
    }
}
//==============================================================================
void ScanSystemSenser()                                                         // need edit by user //
{
    int iSafeDoor   =IsSafeDoorOpen();
    int iEmg        =IsEMGPressed();
    int iIonFan     =IsIonFanAlarm();

    if(MotorPowerOnDelay==0)
    {
        if(iEmg>0)                                                    // emg //
        {
            HSys.StopAllMotor();
            AllBreakLock();

            if(HSys.Sys.SystemStart==true)
                ShowSystemError(HSys.SenPtr[iEmg].Name, K_RETRY);
            HSys.Sys.SystemStart =false;
            fAllMotorHome=false;
            MotorPowerOnDelay=SERVER_MOTOR_POWER_ON_DELAY;
        }
        else if(HSys.Sen.SnMotorPower.IsOff()==true && fHome->fShow==false)     // 馬達斷電
        {
            HSys.DecStopAllMotor();
            AllBreakLock();

            if(HSys.Sys.SystemStart==true)
                ShowSystemError(HSys.Sen.SnMotorPower.Name, K_RETRY);
            HSys.Sys.SystemStart=false;
            fAllMotorHome=false;
            MotorPowerOnDelay=SERVER_MOTOR_POWER_ON_DELAY;
        }
    }

    if(HSys.Sys.SystemStart)
    {                                                                           // edit ben no use //
        #ifdef DISABLE_SAFE_DOOR
            iSafeDoor=0;
        #endif
        if(iSafeDoor>0)                                                         //安全門
        {
            HSys.StopAllMotor();
            ShowSystemError(HSys.SenPtr[iSafeDoor].Name , K_RETRY);
            HSys.Sys.SystemStart=false;
        }
        else if(iEmg>0)                                                              // emg //
        {
            HSys.StopAllMotor();
            AllBreakLock();
            ShowSystemError(HSys.SenPtr[iEmg].Name, K_RETRY);
            fAllMotorHome=false;
            HSys.Sys.SystemStart =false;
        }
        else if(iIonFan>0)
        {
            HSys.DecStopAllMotor();
            ShowSystemError(HSys.SenPtr[iIonFan].Name, K_RETRY);                //JerryYang 20240615 : 要補ALARM CODE
            HSys.Sys.SystemStart =false;
        }
        else if(HSys.Sen.SnAirIsEnough.IsOff())                                   // air //
        {
            HSys.DecStopAllMotor();
            ShowSystemError(HSys.Sen.SnAirIsEnough.Name, K_RETRY);
            HSys.Sys.SystemStart=false;
        }
        else if(MotorPowerOnDelay)
        {
            HSys.Sys.SystemStart=false;
        }
    }
}
//==============================================================================
void DoSystem()                                                                 // system check //
{
    static bool bFirstEnter=true;
    ProcessAlarm();
    ScanAllMotorStatus();
    ScanSystemSenser();
    DoSystemMessage();
    ProcessSuckDuringPause();
    bFirstEnter=false;
    if(HSys.Sys.SystemStart==false)                                             //V1.9
         CheckMotorPowerShutDown();
    if(CountMotorPowerDelay()==false)
        HSys.Sys.SystemStart=false;

    DoSystemMessage();
    RecordSafeDoorStates();                                                     //JerryYang 20240807 : Record when door opened
}
//==============================================================================
// 檢查是否有 motor 尚未歸零
//==============================================================================
bool CheckMotorHome()                                                           // check all motor is home //
{
    #ifdef SOFT_SIMULATE
    return true;
    #else
    int i;
    for(i=0; i<HSys.iTotalMotor; i++)
    {
        if(HSys.MotPtr[i]->bHomeFlag==false)
        {
            fAllMotorHome=false;
            return false;
        }
    }
    return true;
    #endif
}
//==============================================================================
// user define
//==============================================================================
void DoTemptureControl()                                                        // 控制溫控器 //
{

}
//==============================================================================
void AddNoNeedHomeSensorList()
{

}
//==============================================================================
void ProcessStartMode()
{
    if(SoftStart==true)                                                         //啟動檢查
    {
        SetWorkParameter();
        fHome->iHomeStep=1;
        ClearAllAlarm();                                                        // ben add //
        SoftStart=false;
        SoftStop=false;
        if(HSys.Sys.RunMode!=Run_Home && fSetup->fShow==false)                  // ben add //
        {
            if(fAllMotorHome)
            {
                if(CheckMotorHome()==false)
                {
                    ShowMyMessage("Must home again");
                    return;
                }
            }
        }

        if(CUSTOMER_CODE==CC_ASE_CL && HSys.LastSet.iRealDummy==REALLY && RunInfo.LotStart==false)
        {
            ShowMyMessage("Please Press Lot Start!!");
            return;
        }
        SetMotorSpeed();                                                        // ben add -s //
        bLampSkip=false;
        bLampRetry=false;
        bLampReset=false;
        bLampTrayEnd=false;
        HSys.Sys.SystemStart=true;
        bDelayCheckLevelSetup=true;                                             // ben add delay levelsetup update //
    }
    else if(SoftStop==true)                                                     //暫停檢查
    {
        HSys.DecStopAllMotor();
        SoftStop=false;
        HSys.Sys.SystemStart=false;
        bDelayCheckLevelSetup=true;                                             // ben add delay levelsetup update //
    }
}
//==============================================================================
void ProcessRunStatus(bool bProgramStart)                                       // 檢查生產狀態 //
{
    int        iMode=fMain->HALT;
    static int iOldMode=-1;
    //
    static char str[][20]={{"HALT"},            //0
                           {"PAUSE"},           //1
                           {"HOMEING"},         //2
                           {"RUNNING"},         //3
                           {"HAETING"},         //4
                           {"LOCK"},            //5
                           {"EMG"},             //6
                           {"Motor Off"} ,      //7
                           {"One Cycle"} ,      //8
                           {"Clean Out"} ,      //9
                           {"Tray Feed"},       //10
                           {"RUN CHECK"}};      //11  //JerryYang 20250106 : Run check

    static char strChi[][20]={{"停機"},{"暫停"},{"歸零中"},{"運轉中"},{"加熱中"},{"鎖住"},{"緊急停止"},{"馬達斷電"} ,{"單循環"} ,{"結批"}, {"退盤"}, {"檢查中"}};
    static int iCountDown=7;
    static int OldSecond;
    if(bProgramStart==false  && iCountDown!=0)   //20140312 wei
    {
        //iMode=0;
        if(OldSecond!=SystemSec)
        {
            OldSecond=SystemSec;
            iCountDown--;
            fMain->palMainStatus->Font->Color=clRed;
            fMain->palMainStatus->Caption=(AnsiString)iCountDown;
        }
    }
    else
    {
        if(HSys.Sys.SystemStart)
        {
            if(fAllMotorHome==false)
            {
                iMode=fMain->HOMING;                                                            // home //
                fMain->palMainStatus->Font->Color=clGreen;
            }
            else if(HSys.Sys.RunMode==Run_OneCycle)
            {
                iMode=fMain->ONE_CYCLE;
                fMain->palMainStatus->Font->Color=clYellow;
                bLampOneCycle=true;
            }
            else if(HSys.Sys.RunMode==Run_CleanOut)
            {
                iMode=fMain->CLEAN_OUT;
                fMain->palMainStatus->Font->Color=clYellow;
                bLampCleanOut=true;
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed)
            {
                iMode=fMain->TRAY_FEED;
                fMain->palMainStatus->Font->Color=clYellow;
                bLampTrayFeed=true;
            }
            else
            {
                iMode=fMain->RUNNING;                                                        // run //
                fMain->palMainStatus->Font->Color=clGreen;
                bLampCleanOut=false;
                bLampOneCycle=false;
                bLampTrayFeed=false;
            }
        }
        else if(IsSafeLock()==true)
        {
            iMode=fMain->LOCK;                                                            // emg //
            fMain->palMainStatus->Font->Color=clRed;
        }
        else if(IsEMGPressed()>0)                                               // ben add 20111018 //
        {
            iMode=fMain->EMG;                                                            // emg //
            fMain->palMainStatus->Font->Color=clRed;
        }
        else if(HSys.Sen.SnMotorPower.IsOff()==true && fHome->fShow==false)
        {
            iMode=fMain->MOTOR_OFF;                                                            // Motor Off //
            fMain->palMainStatus->Font->Color=clRed;
        }
        else if(USE_SECS_GEM && HSys.FuncB.bN04_RunCheck && bPhysicalStart==true)   //JerryYang 20250106 : Run check
        {
            iMode=fMain->RUN_CHECK;
            fMain->palMainStatus->Font->Color=clBlue;
        }        
        else if(HasICUnderFrontMachine()==false)
        {
            iMode=fMain->HALT;
            fMain->palMainStatus->Font->Color=clRed;
            if(bCalculatePauseTime==false)                                      //Sam 20240526 : 新增每盤的UPH
            {
                tUPH_PauseStartTime=Now();
                bCalculatePauseTime=true;
            }
        }
        else
        {
            iMode=fMain->PAUSE;                                                            // pause //
            fMain->palMainStatus->Font->Color=clRed;
            if(bCalculatePauseTime==false)                                      //Sam 20240526 : 新增每盤的UPH
            {
                tUPH_PauseStartTime=Now();
                bCalculatePauseTime=true;
            }
        }

        if(HSys.LastSet.iLanguageCountry==0)
        {
            fMain->palMainStatus->Caption=str[iMode];                           // show status in main //
            fMain->palMainStatus_En->Caption=str[iMode];                        //20140124 wei 修改sces_gam 中文問題
        }
        else
        {
            fMain->palMainStatus->Caption=strChi[iMode];
            fMain->palMainStatus_En->Caption=str[iMode];                        //20140124 wei 修改sces_gam 中文問題
        }

        if(iOldMode!=iMode)
        {
            EventReport(SECS_EVENT.HandlerStatus);                              //Steven 20130723
            iOldMode=iMode;
        }
    }
    RunInfo.iSystemStatus=iMode;
}
//------------------------------------------------------------------------------
void InitialAllTask()                                                           // initial all process task //
{                                                                               // need edit by user //
    int iActCount=DataModule1->UserMotion->ActionCount;
    for(int i=0; i<iActCount; i++)
    {
        DataModule1->UserMotion->Actions[i]->Tag=1;
    }
}
//------------------------------------------------------------------------------
void DoAllProcess()                                                             // use usermotion do all process //
{
    for(int i=0; i<DataModule1->UserMotion->ActionCount; i++)
    {
        if(HSys.Sys.SystemStart==false)
        {
            HSys.DecStopAllMotor();
            return;
        }
        DataModule1->UserMotion->Actions[i]->Execute();
    }
    TrayArmPara->NewDoAutoTrayEdgeCylinderLoop();
}
//------------------------------------------------------------------------------
bool DoInitialProgramStart()                                                    // first start need initial //
{
    static int Task=1;
    static HTimer Delay;
    #ifdef SOFT_SIMULATE
    Task=200;
    #endif
    switch(Task)
    {
        case 1:
            HSys.DecStopAllMotor();
            AllBreakLock();
            Task++;
            break;
        case 2:
            HSys.Sw.SwMotorRelay.Off();                                         // ben disable 20111018 //
            Delay.SetMS(4000);
            Delay.On();
            Task++;
            break;
        case 3:
            if(Delay.Off())
            {
                HSys.Sw.SwMotorRelay.On();                                      // ben disable 20111018 //
                Delay.SetMS(2000);
                Delay.On();
                Task=100;
            }
            break;
        case 100:
            if(Delay.Off())
            {
                if(AllBreakFree())                                              // ben 20110705 //
                    Task=200;
            }
            break;
        case 200:
            DataModule1->Timer1->Enabled=true;
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool AllMagTrayFeedFinish()
{
    bool bret=false;
    if(CosFunction.bUseMagzineArea)
    {
        bret=MagazineModule1->TrayFeedFinish() && MagazineModule2->TrayFeedFinish() && MagazineModule3->TrayFeedFinish();
    }
    else
    {
        bret=true;
    }
    return bret;
}
//---------------------------------------------------------------------------
bool AllMagArmTrayFeedFinish()
{
    bool bret=false;
    if(CosFunction.bUseMagzineArea)
    {
        bret=(MagArmPara->iMagStatus==MagArmPara->eHS_MAG_SafePos);
    }
    else
    {
        bret=true;
    }
    return bret;
}
//---------------------------------------------------------------------------
bool AllMagOnecycleFinish()
{
    bool bret=false;
    if(CosFunction.bUseMagzineArea)
    {
        bret=MagazineModule1->TrayFeedFinish() && MagazineModule2->TrayFeedFinish() && MagazineModule3->TrayFeedFinish();
    }
    else
    {
        bret=true;
    }
    return bret;
}
//---------------------------------------------------------------------------
bool AllMagArmOnecycleFinish()
{
    bool bret=false;
    if(CosFunction.bUseMagzineArea)
    {
        bret=(MagArmPara->iMagStatus==MagArmPara->eHS_MAG_SafePos);
    }
    else
    {
        bret=true;
    }
    return bret;
}
//---------------------------------------------------------------------------
void ProcessMotion()                                                            // do all process motion //
{
    int ret;
    AnsiString Str,Str1;
    TDateTime dtmp;                                                             //Sam 20170829 : 修正 UPH 異常
    if(HSys.Sys.SystemStart)
    {
        if(bCalculatePauseTime)
        {
            tUPH_PauseTime+=Now()-tUPH_PauseStartTime;
            bCalculatePauseTime=false;
        }
        CheckMotorHome();                                                       // check all motor is home //
        if(fAllMotorHome==false)
        {
            bSortArmNeedHome=false;
            bSortArmYNeedHome=false;
            bMagArmNeedHome=false;
            ChangeRunMode(Run_Home);                                            //20120102 Daver add
            if(ProcessMotorHome(0))                                             // do home //
            {
                InitialAllTask();
                InitialAllModule();                                             // initial all process task //
                fAllMotorHome=true;
                RunState=LED_Pause;
                ChangeRunMode(Run_Normal);                                      //20120102 Daver add
                SetMotorSpeed();
                if(bHomeByStart==true)
                {
                    bHomeByStart=false;
                }
                else
                {
                    SoftStop=true;
                }
            }
        }
        else if(bSortArmNeedHome)                                               //JerryYang 20240610 : Z auto home
        {
            if(fHome->SortArm1ZHome(false, bSortArmNeedHome, bSortArmYNeedHome))    //JerryYang 20240704 : Pick up error Y軸一起回HOME
            {
                bSortArmNeedHome=false;
                bSortArmYNeedHome=false;
            }
        }
        else if(bMagArmNeedHome)                                                //Sam 20240618 : MagArm Z auto home
        {
            if(fHome->SortArm2ZHome(false, bMagArmNeedHome))
            {
                bMagArmNeedHome=false;
            }
        }
        else
        {
            DoAllProcess();
            if(bFirstRun)
            {
                fMain->ShowBinCount();                                          //Sam 20170907 (jou): 資料清空之後 UI 更新
                tRunData.StartTime=Now();

                tRunData.LotEndTime=StrToTime("00:00:00");                      //Sam 20170825 (jou): 清除結批時間
                tRunData.PauseTime=StrToTime("00:00:00");
                tRunData.AlarmTime=StrToTime("00:00:00");
                tRunData.iPauseTime=0;

                RunInfo.LotStart=true;
                bFirstRun=false;
                bMPBAEnterflag=true;
                bFirstDevice=true;
            }

            if(HSys.Sys.RunMode==Run_CleanOut)
            {
                if(CheckCleanOutFinish())
                {
                    bCleanOut=false;                                            //KenHsieh 20250820 : 補上flag，在cleanout下執行onecycle完成後，可自動變為cleanout
                    tRunData.LotEndTime=Now();
                    tRunData.UPH=GetCalculateUPH(tRunData.LotEndTime);
                    fMain->sgProductInfo->Cells[1][eUPH] = IntToStr(tRunData.UPH);
                    fMain->sgProductInfo->Cells[1][eLotEnd]= FormatDateTime("hh:nn:ss" , tRunData.LotEndTime);
                    //UPH log
                    AnsiString sDir=AnsiString().sprintf("D:\\HT-172_Log\\UPHLog\\%04d_%02d", SystemYear, SystemMonth);
                    AnsiString sFileName=AnsiString().sprintf("%s_%04d_%02d_%2d.csv", asHandlerID, SystemYear, SystemMonth, SystemDate);
                    AnsiString sDefHead="Data Time, Lot No, UPH";
                    Str.sprintf("%02d:%02d:%02d.%03d, %s, %s", SystemHour, SystemMin, SystemSec, SystemMSec, fMain->edLotNo->Text, fMain->sgProductInfo->Cells[1][eUPH]);
                    FormSysTools->SaveLogRpt(sDir,sFileName,sDefHead,Str);

                    Str.sprintf("End of Lot: %s, Load: %d, UPH: %s", fMain->edLotNo->Text, tRunData.TotalIC, fMain->sgProductInfo->Cells[1][eUPH]);
                    RecordProcess(Str);
                    Str.sprintf("==============================================================");
                    RecordProcess(Str);
                    InitialAllTask();
                    bFlagLoaderCleanoutFinish   =false;
                    bFlagEmpty1CleanoutFinish   =false;
                    bFlagEmpty2CleanoutFinish   =false;
                    bFlagAutoArmCleanoutFinish  =false;
                    bFlagMagArmCleanoutFinish   =false;
                    bFlagTrackCleanoutFinish    =false;
                    bFlagAutoBinCleanoutFinish  =false;

                    ChangeRunMode(Run_Normal);
                    EventReport(SECS_EVENT.CleanOutOK);                         //Steven 20130723
                    ret=ShowSystemError(HSys.Sen.SnFKCleanOut.Name, K_SKIP|K_TRAY_FEED,0);
                    if(ret==K_TRAY_FEED)
                    {
                        RecordProcess("TRAY FEED pressed");
                        bOnecycleTrayFeed=true;
                        ChangeRunMode(Run_TrayFeed);
                        fMain->sbTrayFeed->Click();
                    }
                    else
                    {
                        RecordProcess("SKIP pressed");
                    }
                    if(CUSTOMER_CODE==CC_PTI)                                   //Sam 20250602 : Tray Feed 自動 LotEnd 並執行 PTI lot_End.exe
                    {
                        CallLotProgram("End");                                  //Jimmychiu 20260120 : 原命令於Tray feed finish後送出，Joe要求放在Clean out finish送出優化產能
                    }
                }
            }
            else if(HSys.Sys.RunMode==Run_OneCycle)
            {
                if(CheckOneCycleFinish())
                {
                    flagOneCycleTrayEnd=true;
                    bSortArm1OneCycleFinish    =false;                          //Steven 20240707 : Fixed for one cycle
                    bSortArm2OneCycleFinish    =false;
                    bSortStoreArmOneCycleFinish=false;
                    bSortMagOneCycleFinish[eMagazine1]=false;
                    bSortMagOneCycleFinish[eMagazine2]=false;
                    bSortMagOneCycleFinish[eMagazine3]=false;

                    ChangeRunMode(Run_Normal);
                    EventReport(SECS_EVENT.OneCycleOK);                         //Steven 20130723
                    #ifndef ONECYCLE_DEBUG
                    ShowSystemError(HSys.Sen.SnFKOneCycle.Name, K_RETRY, 0);
                    #endif

                    if(bCleanOut==true)                                         //KenHsieh 20250820 : 補上flag，在cleanout下執行onecycle完成後，可自動變為cleanout
                        ChangeRunMode(Run_CleanOut);
                }
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed)
            {
                if(CheckAllTrayFeedFinish()==true)                               //KenHsieh 20240528 : 手動Magazine Tray Feed
                {
                    flagTrayFeed=false;
                    ChangeRunMode(Run_Normal);
                    bOnecycleTrayFeed=false;
                    bEmpty1TrayFeedFinish=false;

                    EventReport(SECS_EVENT.TrayFeedOK);
                    ShowSystemError(HSys.Sen.SnFKTrayFeed.Name, K_RETRY);

                    if((HSys.FuncT.iA01_InputDataFormat==LoaderModule->eEKeeperFormat) &&
                        iHasSkipTrayDataNeedHold==0)
                        ASESendMessage->SendTrayFeed();
                    else
                        fMain->sbPaperSummary->Click();

                    bFirstRun=true;
                    RunInfo.LotStart=false;
                    fMain->pnStartMode->Enabled=true;
                    //Initial status
                    MagArmPara->InitialFlag();
                    MagazineModule1->InitialFlag();
                    MagazineModule2->InitialFlag();
                    MagazineModule3->InitialFlag();
                    LoaderModule->InitialFlag();
                    TrayArmPara->bTrayArmTrayFeedFinish=false;
                    if(CUSTOMER_CODE==CC_PTI)                                   //Sam 20250602 : Tray Feed 自動 LotEnd 並執行 PTI lot_End.exe
                    {
                        fMain->btnLotEndClick(fMain);
                        CallLotProgram("Tray_Feed");                            //Jimmychiu 20260120 : 原命令於Tray feed finish後送出，Joe要求放在Clean out finish送出優化產能
                    }
                }
            }
            else if(flagTrayEnd)
            {
                flagTrayEnd=false;
                ShowSystemError(HSys.Sen.SnFKTrayEnd.Name, K_RETRY);
            }
        }
    }
    else
    {
        bPauseMagMotor=true;
        bPauseSortMotor=true;

        if(HSys.Sys.RunMode==Run_TrayFeed)                                      //KenHsieh 20240528 : 手動Magazine Tray Feed
        {
            if(MagArmPara->iMagStatus==MagArmPara->eHS_MAG_SafePos)
            {
                if(MagazineModule1->TrayFeedFinish()==false)
                {
                    MagazineModule1->ManualStepBtnCanLight();
                }

                if(MagazineModule2->TrayFeedFinish()==false)
                {
                    MagazineModule2->ManualStepBtnCanLight();
                }

                if(MagazineModule3->TrayFeedFinish()==false)
                {
                    MagazineModule3->ManualStepBtnCanLight();
                }
            }
        }
    }
    if(bDelayCheckLevelSetup==true)                                             // ben add delay levelsetup update //
        bDelayCheckLevelSetup=false;
}
//---------------------------------------------------------------------------
void MainProc()                                                                 // 主執行緒 //
{
    static bool bProgramStart=false;
    if(fiosetview->fShow)
        return;
    ProcessStartMode();                                                         // 啟動或暫停檢查 //
    DoSystem();                                                                 // 即時系統檢查 //
    ProcessRunStatus(bProgramStart);                                            // 檢查生產狀態 //

    if(bProgramStart==false)                                                   // first start need initial //
    {
        bProgramStart=DoInitialProgramStart();
    }
    else
    {
        ProcessMotion();
    }
}
//------------------------------------------------------------------------------
void InitialAllModule()
{
    InitialAuto();
    InitialLoader();
    InitSortArm();
    InitTrayArm();
    InitMagArm();
    InitialMag();
}
//---------------------------------------------------------------------------
bool CheckCleanOutFinish()
{
    bool bResult =true;
    bResult &= bFlagLoaderCleanoutFinish  ;
    bResult &= bFlagAutoArmCleanoutFinish ;
    bResult &= bFlagMagArmCleanoutFinish  ;
    bResult &= bFlagAutoBinCleanoutFinish ;
    bResult &= bFlagTrackCleanoutFinish   ;

    return bResult;
}
//---------------------------------------------------------------------------
bool CheckOneCycleFinish()
{
    bool bResult=true;
    bResult &= bSortArm1OneCycleFinish    ;                                     //Steven 20240707 : Fixed for one cycle
    bResult &= bSortArm2OneCycleFinish    ;
    bResult &= bSortStoreArmOneCycleFinish;
    if(MAGAZINE_CNT>1)
    {
        bResult &= bSortMagOneCycleFinish[eMagazine1];
    }
    bResult &= bSortMagOneCycleFinish[eMagazine2];
    if(MAGAZINE_CNT==3)
    {
        bResult &= bSortMagOneCycleFinish[eMagazine3];
    }

    bResult &= bTrackOneCycleFinish[eTrack1];
    bResult &= bTrackOneCycleFinish[eTrack2];
    bResult &= bTrackOneCycleFinish[eTrack3];
    bResult &= bTrackOneCycleFinish[eTrack4];    

    return bResult;
}
//---------------------------------------------------------------------------
void CCDLoadJobs()
{
}
//---------------------------------------------------------------------------
bool HasICUnderMachine()
{
    for(int i=0; i<HSys.iTotalVMotor; i++)
    {
        if(HSys.VMotPtr[i]->HasIC())
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool HasICUnderFrontMachine()
{
    return false;
}
//---------------------------------------------------------------------------
bool HasICUnderMachineForCleanOut()
{
    bool bret=false;
    for(int i=eAuto1; i<=eAuto20; i++)
    {
        if(HSys.VMotPtr[i]->HasIC())
        {
            return true;
        }
    }
    bret|=HSys.VMot.LoadTray_Car->HasIC();
    bret|=HSys.VMot.MotWorkLoader->HasIC();
    bret|=HSys.VMot.Mag_LD_Car->HasIC();
    bret|=HSys.VMot.Mag_LD_Work_Y->HasIC();
    bret|=HSys.VMot.Track_1_TrayArm->HasIC();
    bret|=HSys.VMot.Track_2_TrayArm->HasIC();
    bret|=HSys.VMot.Track_3_TrayArm->HasIC();
    bret|=HSys.VMot.Track_4_TrayArm->HasIC();
    bret|=HSys.VMot.Transfer_LoaderY->HasIC();
    bret|=HSys.VMot.TrayArmTray->HasIC();
    return bret;
}
//---------------------------------------------------------------------------
void ChangeRunMode(RunModeEnum RunMode)
{
    HSys.Sys.RunMode=RunMode;
}
//---------------------------------------------------------------------------
void RecordSafeDoorStates()                                                     //JerryYang 20240807 : Record when door opened
{
    AnsiString Str;
    static bool bClear=false;                                                   //Steven 20211229 : 只做一次就好
    int iSafeDoorCount=10;
    static bool bSafeDoorOpen[10];        //JerryYang 20230704 : 整合安全門15->MAX_SAFE_DOOR_CNT

    bool bPLCSafe=false;
    if(Enable_PLCSafety_IO)
    {
        if(bPLCIOEffect==false || bIOPowered==false)
            bPLCSafe=true;
    }

    if(HSys.Sys.SystemStart)
    {
        if(bClear==false)       //Steven 20211229 : 只做一次就好
        {
            ZeroMemory(bSafeDoorOpen, sizeof(bSafeDoorOpen));
            bClear=true;
        }
        return;
    }
    bClear=false;               //Steven 20211229 : 只做一次就好

    if(bPLCSafe==false)
    {
        for(int i=0; i<iSafeDoorCount; i++)                                      //JerryYang 20230704 : 整合安全門15->MAX_SAFE_DOOR_CNT
        {
            if(HSys.SenPtr[HSys.Sen.SnSafeDoor_01.Tag+i].Enable && HSys.SenPtr[HSys.Sen.SnSafeDoor_01.Tag+i].IsOff()==true && bSafeDoorOpen[i]==false) //open Safe Door
            {
                Str.sprintf("Safe Door %d is Opened", i+1);
                RecordProcess(Str);
                bSafeDoorOpen[i]=true;
            }
        }
    }
}
//------------------------------------------------------------------------------
bool CheckEmpty1TrayFeedFinish()
{
    return bEmpty1TrayFeedFinish;
}
//------------------------------------------------------------------------------
bool CheckAllTrayFeedFinish(bool reset)
{
    bool bResult=true;
    bResult&=AllMagArmTrayFeedFinish();
    bResult&=AllMagTrayFeedFinish();
    bResult&=CheckEmpty1TrayFeedFinish();
    bResult&=TrayArmPara->bTrayArmTrayFeedFinish;
    return bResult;
}
//------------------------------------------------------------------------------
int GetCalculateUPH(TDateTime tEndTime)
{
//    static bool brec=false;
    int iret=0;
    if(tRunData.TotalIC>0)
    {
        double dDay = double(tEndTime) - double(tRunData.StartTime) - double(tRunData.PauseTime);     //Sam 20240620 : 修正 UPH 跨日問題。
        double dHour=dDay*24.0;
        if(dHour>0)
        {
            iret= tRunData.TotalIC/dHour;
//            if(brec)
//            {
//                AnsiString sDir=AnsiString().sprintf("D:\\HT-172_Log\\TestLog\\%04d_%02d", SystemYear, SystemMonth);
//                AnsiString sFileName=AnsiString().sprintf("%s_%04d_%02d_%2d.csv", asHandlerID, SystemYear, SystemMonth, SystemDate);
//                FormSysTools->SaveLogRpt(sDir,sFileName,AnsiString().sprintf("%d,%f,%f,%f,%f",tRunData.TotalIC,dHour,tEndTime,tRunData.StartTime,tRunData.PauseTime));
//            }
        }
    }
    return iret;
}
//---------------------------------------------------------------------------
