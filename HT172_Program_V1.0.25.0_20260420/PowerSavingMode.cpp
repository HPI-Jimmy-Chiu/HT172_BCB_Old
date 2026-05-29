#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "PowerSavingMode.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

class TPowerSaving *tPowerSaving;
const int MaxMinute= 200;     //Alarm 設定最大值
//---------------------------------------------------------------------------
__fastcall TPowerSaving::TPowerSaving()
{
    ScanTmr =new TTimer(fMain);

    ScanTmr->OnTimer= OnScanTmr;
    ScanTmr->Interval = 1000;
    ScanTmr->Enabled = false;

    flagStartTmr =false;
    bRestart =true;

    MtrModule =new TMtrModule;
    PumpModule =new TPumpModule;

    tModule = new TModule;

    SetFunction(true);
//    flagUseFunc=false;
}
//---------------------------------------------------------------------------
__fastcall TPowerSaving::~TPowerSaving()
{
    delete MtrModule;
    delete PumpModule;
    delete tModule;
    delete ScanTmr;
}
//---------------------------------------------------------------------------
//主流程
void __fastcall TPowerSaving::OnScanTmr(TObject *Sender)
{
    static int Task =0;

    if(HSys.Sys.SystemStart)
    {
        bRestart=true;
        return;
    }

    if(bRestart)
    {
        bRestart =false;
        Task =0;
        return ;
    }
    ScanTmr->Enabled =false;
    switch(Task)
    {
        case 0:
            if(CheckSystemRun())
            {
                flagStartTmr=false;
                Task=100;
            }
            break;
        case 100:
            flagStartTmr=true;
            StartTmr =Time();
            PumpModule->Doing(true);
            Task=200;
            break;
        case 200:
            if(flagUseFunc)
            {
                if (tModule->TimeOut(StartTmr))
                {
                    Task=300;
                }
            }
            break;
        case 300:
            int iTmp;
            iTmp =0;

            if(HSys.FuncA.iA08_PSMMode==0)
            {
                Task=400;
            }
            else if(HSys.FuncA.iA08_PSMMode==1)
            {
                //jou 2012-03-06 不能用ShowErrorMessage,因為retry->Start,機台就跑起來了
                iTmp=ShowMyMessageBox_YES_NO("Enter power saving mode");   //Enter power saving mode
                if(iTmp==1)
                {
                    Task=400;
                }
                else
                {
                    Restart();
                }
            }
            break;
        case 400:
            if(MtrModule->Enabled)
            {   //Use Motor Module
                if(MtrModule->TimeOut(StartTmr))
                {
                    MtrModule->Doing(false);
                    MtrModule->Enabled =false;
                }
            }

            if(PumpModule->Enabled)
            {  //Use Hot Module
                if(PumpModule->TimeOut(StartTmr))
                {
                    if(PumpModule->Doing(false))
                    {
                        PumpModule->Enabled =false;
                    }
                }
            }

            if((MtrModule->Enabled==false) && (PumpModule->Enabled==false))
                Task=500;
            break;
        case 500:
            break;
        default:
            Task =0;
            break;
    }
    ScanTmr->Enabled=true;
}
//---------------------------------------------------------------------------
bool TPowerSaving::CheckSystemRun()
{
    if((HSys.Sys.SystemStart==false) && (HasICUnderMachine()==false))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//單位:分鐘
void TPowerSaving::SetAlarmTime(TDateTime &Module ,int Minute)
{
    Word whh ,wmm ,wss ,wms;

    if (Minute == 0)
        Minute =1;
    else if(Minute > MaxMinute)
        Minute = MaxMinute;

    whh = Minute/60;
    wmm = Minute%60;
    wss = wms =0;

    Module = EncodeTime(whh ,wmm ,wss ,wms);
}
//---------------------------------------------------------------------------
void TPowerSaving::SetFunction(bool OnOff)
{
    int tmp=0;
    flagUseFunc=(HSys.FuncA.bA08_PSMEnableMotor || HSys.FuncA.bA08_PSMEnablePump);

    if(HSys.FuncA.iA08_PSMMotorTime<=HSys.FuncA.iA08_PSMPumpTime)
        tmp=HSys.FuncA.iA08_PSMMotorTime;
    else
        tmp=HSys.FuncA.iA08_PSMPumpTime;

    if(flagUseFunc)
    {
        tModule->Enabled =true;
        SetAlarmTime(tModule->AlarmTmr, tmp);

        ScanTmr->Enabled=true;
    }
    else
    {
        ScanTmr->Enabled=false;
    }

    if(HSys.FuncA.bA08_PSMEnableMotor)
    {
        SetAlarmTime(MtrModule->AlarmTmr, HSys.FuncA.iA08_PSMMotorTime);
    }

    if(HSys.FuncA.bA08_PSMEnablePump)
    {
        SetAlarmTime(PumpModule->AlarmTmr, HSys.FuncA.iA08_PSMPumpTime);
    }
    MtrModule->Enabled  =HSys.FuncA.bA08_PSMEnableMotor;
    PumpModule->Enabled =HSys.FuncA.bA08_PSMEnablePump;
}
//---------------------------------------------------------------------------
void TPowerSaving::Restart()
{
    bRestart =true;
    HSys.Sw.SwVacuumPump1.Off();         //Off的時候是關掉幫浦
    HSys.Sw.SwVacuumPump2.Off();         //OFF OPEN
    HSys.Sw.SwVacuumPump3.Off();
    HSys.Sw.SwVacuumPump4.Off();

    SetFunction(true);
}
//---------------------------------------------------------------------------
__fastcall TModule::TModule()
{
    Enabled =true;
}
//---------------------------------------------------------------------------
__fastcall TModule::~TModule()
{

}
//---------------------------------------------------------------------------
bool TModule::TimeOut(TDateTime StartTmr)
{
    TDateTime nowT = Time();
    TDateTime OverDayPM = StrToTime("pm 11:59:59");
    TDateTime OverDayAM = StrToTime("am 12:00:00");
    bool bResult =false;

    if(nowT > StartTmr)
    {
        bResult = ((nowT - StartTmr) > AlarmTmr);
    }
    else
    {
        bResult = ((nowT - OverDayAM) + (OverDayPM - StartTmr)) > AlarmTmr;
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TModule::Doing(bool Reset)
{
   return true;
}
//---------------------------------------------------------------------------
bool TMtrModule::Doing(bool Reset)
{
    AllBreakLock();
    HSys.Sw.SwMotorRelay.Off();
    bMotorPowerState=false;
    fAllMotorHome=false;    //Steven 20110211
    return true;
}
//---------------------------------------------------------------------------
bool TPumpModule::Doing(bool Reset)
{
    static int Task =0;
    static int CountTmr =0;
    int Now =0;
    int Waiting =10000;
    bool bResult =false;

    if(Reset)
    {
        Task =0;
        return bResult;
    }
    switch(Task)
    {
        case 0:
            HSys.Sw.SwVacuumPump1.On();         //On的時候是關掉幫浦
            HSys.Sw.SwVacuumPump2.On();
            HSys.Sw.SwVacuumPump3.On();
            HSys.Sw.SwVacuumPump4.On();
            CountTmr =GetTickCount();
            Task++;
            break;
        case 1:
            Now = GetTickCount() - CountTmr;
            if(Now > Waiting)
            {
                HSys.Sw.SwVacuumPump1.On();
                HSys.Sw.SwVacuumPump2.On();
                HSys.Sw.SwVacuumPump3.On();
                HSys.Sw.SwVacuumPump4.On();
                bResult =true;
            }
            break;
    }
    return bResult;
}
//---------------------------------------------------------------------------
