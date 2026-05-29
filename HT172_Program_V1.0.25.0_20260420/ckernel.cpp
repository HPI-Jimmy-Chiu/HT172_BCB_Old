//==============================================================================
// 最後檢查日:2006/04/18
// 檢查人    :lee
// 目的      :9016 & 9040T
//==============================================================================
//---------------------------------------------------------------------------

//==============================================================================
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "ckernel.h"
#include "MyPLC_IO_Modbus.h"
#pragma package(smart_init)
//==============================================================================

//============================================================================//
//   共用變數區                                                               //
//============================================================================//
#define     ALM_MOTOR_MOVE  55555
extern int AccelateTask;
//==============================================================================
void CheckManualTrayChange()                                                    // need edit by user //
{                                                                               // ben 20110630 //
    /*
    int i;
    for(i=0;i<3;i++)
    {
        if( HSys.MotPtr[HSys.VMot.MManualTray1.Tag+i]->fHasTray && HSys.SenPtr[HSys.Sen.SnFixedTray1Detect.Tag+i].IsOff()==true)
            ManualTrayChange[i]=true;
    }
    */
}
//==============================================================================
bool CheckThermo()                                                              // check溫度 //
{
    /*
    #ifdef SOFT_SIMULATE
        return true;
    #else
        int i;
        if( HSys.LastSet.iTemperature )
            return true;
        if( iHeaterCooling!=0)
            return false;
        for(i=0;i<HSys.LastSet.iMaxUT150;i++)
        {
            if( i==0 || i==1)
                continue;
            //if( HSys.LastSet.bUT150Install[i] && HSys.TempCtrl->ReadCurrentTemp(i) > HSys.LastSet.iAmbientDefine )
            //    return false;
        }
        return true;
    #endif
    */
    return true;
}
//==============================================================================
void StopAllDestroy()                                                           // all suck destroy stop //
{
    /*                                                                          // ben 20110630 need edit by user //
    int i;
    for(i=0;i<2;i++)
    {
        HSys.Suck.InArm.Suck[i].OffDestroy();
        HSys.Suck.OutArm.Suck[i].OffDestroy();
        HSys.Suck.FTest.Suck[i].OffDestroy();
        HSys.Suck.BTest.Suck[i].OffDestroy();
    }
    HSys.Suck.CatchTray.Suck[0].OffDestroy();
    */
}
//==============================================================================
// 處理三色燈
//==============================================================================
void ShowRunLed()                                                               // ben edit 20110706 //
{
    if(InitialOK==false)
        return;

    static bool OldFlushFlag;
    int is,i;
    if(fNote->fShow)
    {
        RunState=LED_ErrJam;
    }
    else if((bOffBuzzer==false && MyMessageBox->fShow) || bMessageAlarm)
    {
        RunState=LED_Message;
    }
    else if(fHome->fShow)
    {
        RunState=LED_Homeing;
    }
    else if(HSys.Sys.SystemStart)
    {
        if(HSys.LastSet.iTemperature && fHeaterOK==false)
            RunState=LED_Heating;
        else
            RunState=LED_Running;
        bLampStart=true;                                                        // ben add 20110803 //
        bLampPause=false;                                                       // ben add 20110803 //
    }
    else
    {
        RunState=LED_Pause;
        bLampStart=false;                                                       // ben add 20110803 //
        bLampPause=true;                                                        // ben add 20110803 //
    }

    // 處理音樂
    static TRadioGroup *RadioGroupPtr[6]=                                       // ben edit 20110713 - s //
    {
        fMaintenance->RadioGroup2,
        fMaintenance->RadioGroup3,
        fMaintenance->RadioGroup4,
        fMaintenance->RadioGroup5,
        fMaintenance->RadioGroup6,
        fMaintenance->RadioGroup7
    };

    if((fMaintenance->fShow==false) || (fMaintenance->fShow && fNote->fShow) || (fMaintenance->fShow && MyMessageBox->fShow))
    {
        is=RadioGroupPtr[RunState]->ItemIndex;                                      // ben edit 20110713 - e //
        if(is)
        {
            is--;
            for(i=0;i<4;i++)
            {
                if(is==i)
                {
                    if(RunState==LED_ErrJam && bAlarmBuzzer==false)
                        HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].Off();
                    else
                        HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].On();
                }
                else
                {
                    HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].Off();
                }
            }
        }
        else
        {
            for(i=0;i<4;i++)
                HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].Off();
        }
    }

    if(OldFlushFlag==FlushFlag)
        return;
    OldFlushFlag=FlushFlag;

    static TALed *LedPtr[6][3]=                                                 // ben add - s 20110706 //
    {
        {fMaintenance->RGB00,fMaintenance->RGB01,fMaintenance->RGB02},
        {fMaintenance->RGB10,fMaintenance->RGB11,fMaintenance->RGB12},
        {fMaintenance->RGB20,fMaintenance->RGB21,fMaintenance->RGB22},
        {fMaintenance->RGB30,fMaintenance->RGB31,fMaintenance->RGB32},
        {fMaintenance->RGB40,fMaintenance->RGB41,fMaintenance->RGB42},
        {fMaintenance->RGB50,fMaintenance->RGB51,fMaintenance->RGB52}
    };                    // fMaintenance->RGB00->Value
    if(LedPtr[RunState][0]->Value ==1 && LedPtr[RunState][0]->Blink ==0)
        fMain->ledGreen->Value=true;
    else if(LedPtr[RunState][0]->Value ==1 && LedPtr[RunState][0]->Blink ==1)
        fMain->ledGreen->Value=FlushFlag;
    else
        fMain->ledGreen->Value=false;

    if(LedPtr[RunState][1]->Value ==1 && LedPtr[RunState][1]->Blink ==0)
        fMain->ledYellow->Value=true;
    else if(LedPtr[RunState][1]->Value ==1 && LedPtr[RunState][1]->Blink ==1)
        fMain->ledYellow->Value=FlushFlag;
    else
        fMain->ledYellow->Value=false;

    if(LedPtr[RunState][2]->Value ==1 && LedPtr[RunState][2]->Blink ==0)
        fMain->ledRed->Value=true;
    else if(LedPtr[RunState][2]->Value ==1 && LedPtr[RunState][2]->Blink ==1)
        fMain->ledRed->Value=FlushFlag;
    else
        fMain->ledRed->Value=false;

    fMain->ledGreen1->Value =fMain->ledGreen->Value;
    fMain->ledYellow1->Value=fMain->ledYellow->Value;
    fMain->ledRed1->Value   =fMain->ledRed->Value;

    HSys.Sw.SwTowerRed.OnOff(fMain->ledRed->Value);
    HSys.Sw.SwTowerYellow.OnOff(fMain->ledYellow->Value);
    HSys.Sw.SwTowerGreen.OnOff(fMain->ledGreen->Value);                         // ben add - e 20110706 //
    //ben 20230913 add
    //==>
    if(Enable_PLCSafety_IO==true)
    {
        fMain->pnlSafePLC->Visible=true;
        fMain->ledSafePLC->Value=bSafePLCThread;
    }
    else
    {
        fMain->pnlSafePLC->Visible=false;
    }
    //<==
    //ben 20230913 add
}
//==============================================================================
extern int OutArmTask;
//==============================================================================
void DoPanelLamp()                                                              // need edit by user //
{                                                                               // ben edit 20110706 //
    if(bMotorPowerState && MotorPowerOnDelay==0)
    {
        HSys.Sw.SwRKPause.OnOff(bLampPause);
        HSys.Sw.SwRKStart.OnOff(bLampStart);
        HSys.Sw.SwRKRetry.OnOff(bLampRetry);
        HSys.Sw.SwRKSkip.OnOff(bLampSkip);
        HSys.Sw.SwRKTrayEnd.OnOff(bLampTrayEnd);
        HSys.Sw.SwRKTrayFeed.OnOff(bLampTrayFeed);
        HSys.Sw.SwRKCleanOut.OnOff(bLampCleanOut);
        HSys.Sw.SwRKOneCycle.OnOff(bLampOneCycle);
        HSys.Sw.SwRKPowerOn.OnOff(bLampPowerOn);                                    //Frank 20150624
        HSys.Sw.SwRKPowerOff.OnOff(bLampPowerOff);
    }
    else
    {
        HSys.Sw.SwRKPause.Off();
        HSys.Sw.SwRKStart.Off();
        HSys.Sw.SwRKRetry.Off();
        HSys.Sw.SwRKSkip.Off();
        HSys.Sw.SwRKTrayEnd.Off();
        HSys.Sw.SwRKTrayFeed.Off();
        HSys.Sw.SwRKCleanOut.Off();
        HSys.Sw.SwRKOneCycle.Off();
    }
}
//==============================================================================
void DoSystemMessage()
{
    ShowRunLed();
    DoPanelLamp();
}
//==============================================================================
bool ScanRetryButton()
{
    if(HSys.Sen.SnRKRetry.IsOn())
        return true;
    else
        return false;
}
//==============================================================================
bool ScanSkipButton()
{
    if(HSys.Sen.SnRKSkip.IsOn())
        return true;
    else
        return false;
}
//==============================================================================
bool ScanAlarmButton()
{
    if(HSys.Sen.SnRKAlarmReset.IsOn())
        return true;
    else
        return false;
}
//==============================================================================
int ScanPannelKey()                                                             // need edit by user //
{                                                                               // ben edit 20110706 //
    int i;
    static bool flag=true;
    static bool bK[64];
    int ret, iret1=-1;//, p;

    if(flag)
    {
        for(i=0; i<64; i++)
            bK[i]=false;
        flag=false;
    }
    ret=-1;

    if(HSys.Sen.SnRKPause.IsOn())
        ret=HSys.Sen.SnRKPause.Tag;
    if(HSys.Sen.SnFKPause.IsOn())
        ret=HSys.Sen.SnFKPause.Tag;

    if(HSys.Sen.SnRKAlarmReset.IsOn())
        ret=HSys.Sen.SnRKAlarmReset.Tag;

    if(HSys.Sen.SnRKStart.IsOn())
        ret=HSys.Sen.SnRKStart.Tag;
    else if(HSys.Sen.SnRKHome.IsOn())
        ret=HSys.Sen.SnRKHome.Tag;
    else if(HSys.Sen.SnRKOneCycle.IsOn())
        ret=HSys.Sen.SnRKOneCycle.Tag;
    else if(HSys.Sen.SnRKRetry.IsOn())
        ret=HSys.Sen.SnRKRetry.Tag;
    else if(HSys.Sen.SnRKSkip.IsOn())
        ret=HSys.Sen.SnRKSkip.Tag;
    else if(HSys.Sen.SnRKCleanOut.IsOn())
        ret=HSys.Sen.SnRKCleanOut.Tag;
    else if(HSys.Sen.SnRKTrayFeed.IsOn())
        ret=HSys.Sen.SnRKTrayFeed.Tag;
    else if(HSys.Sen.SnRKTrayEnd.IsOn())
        ret=HSys.Sen.SnRKTrayEnd.Tag;
    else if(HSys.Sen.SnMGZ1Step.IsOn())
        iret1=HSys.Sen.SnMGZ1Step.Tag;
    else if(HSys.Sen.SnMGZ2Step.IsOn())
        iret1=HSys.Sen.SnMGZ2Step.Tag;
    else if(HSys.Sen.SnMGZ3Step.IsOn())
        iret1=HSys.Sen.SnMGZ3Step.Tag;

    if(ret!=-1)
    {
        if(bK[ret]==false)
        {
            bK[ret]=true;
            return ret;
        }
        else
            ret=-1;
    }

    for(i=0; i<64; i++)
    {
        if(bK[i])
        {
            if(HSys.SenPtr[i].IsOn()==false)
            {
                bK[i]=false;
            }
        }
    }

    if(iret1!=-1)
    {
        return iret1;
    }

    return ret;
}
//==============================================================================
//int GetMotorAlarmCode(TComponent *Comp)                                         // no use //
//{                                                                               // ben edit 20110706 //
//    int i;
//    for(i=0;i<HSys.iTotalMotor;i++)
//    {
//        if(HSys.MotPtr[i]==Comp)                                               // is Motor Error
//        {
//            HSys.MotPtr[i]->ScanMotorStatus();
//            return 5000+i*10+HSys.MotPtr[i]->GetErrorIndex();
//        }
//    }
//    return 0;
//}
//==============================================================================
AnsiString GetMotorAlarmCodeString(TComponent *Comp)                            // motor error  10000 Start //
{                                                                               // ben add 20110719 //
    AnsiString S;
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
       // if(HSys.MotPtr[i]==Comp)                                               // is Motor Error
         if(HSys.MotPtr[i] ==Comp && HSys.MotPtr[i]->GetEnable())
        {
            HSys.MotPtr[i]->ScanMotorStatus();
            S.printf("%d%03d%1d", (int)eMotorAlarm, i, HSys.MotPtr[i]->GetErrorIndex());
            return S;
        }
    }
    return "0";
}
//==============================================================================
// only process Motor and Cylinder error
//==============================================================================
void ProcessAlarm()                                                             //氣缸 Error is 自動加 10000
{
    TComponent *Comp;
    int iCode=0;
    AnsiString sCode;
    while(PopUpAlarm(&Comp, iCode))
    {
        SoftStop=false;
        SoftStart=false;
        if(iCode==ALM_MOTOR_MOVE)
        {
            sCode=GetMotorAlarmCodeString(Comp);
            fAllMotorHome=false;                                                //發生馬達錯誤
            ShowMotorError(sCode,__FUNC__);                                              // motor error  10000 Start //// ben edit 20110719 //
        }
        else
        {
            ShowSystemError(AnsiString(iCode), K_RETRY);
        }
    }
    ClearAllAlarm();
}
//==============================================================================
void count_seconds(double *seconds)
{
    LARGE_INTEGER  time1, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&time1);
    *seconds=double(time1.QuadPart)/double(freq.QuadPart);
}
//==============================================================================
