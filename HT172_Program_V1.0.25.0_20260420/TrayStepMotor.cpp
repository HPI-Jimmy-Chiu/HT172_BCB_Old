#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "TrayStepMotor.h"

#include "mymessbox.h"
#include "cmydef.h"
#include "uPadInterface.h"                                                      //KenHsieh 20211222 : Pad與步進馬達為同一Comport
#include "main.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma resource "*.dfm"
TdmTrayMotor *dmTrayMotor;

double dStepMotorDelay=2;                                                       //讀取的Delay間隔
double dVibrateMotorDelay=3;                                                    //讀取的Delay間隔
AnsiString sStepMotorAxis[esMotTotal]={"AutoRow2", "AutoRow1", "ShiftStage", "Empty2", "Empty1",
                                       "Load", "AutoRow3", "AutoRow4"};         //Sam 20240525 : 修正軌道設定順序
AnsiString sVibrationMotorAxis[2]={"VibrationMotor1", "VibrationMotor2"};       //指定軌道的名稱
int iSetpMotorVer[esMotTotal];                                                  //軌道驅動器的韌體版本

extern int  CUSTOMER_CODE;
extern bool InitialOK;
extern Word SystemHour, SystemMin, SystemSec, SystemMSec;
extern Word SystemYear, SystemMonth, SystemDate;
extern Word SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday;
//------------------------------------------------------------------------------
__fastcall TdmTrayMotor::TdmTrayMotor(TComponent* Owner)
    : TDataModule(Owner)
{
    bNeedSetStepMotorSpeed=true;
    iSetStepMotorTask=1;
    iVibrationMotorTask=1;
    brecordmsgLock=false;
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::DataModuleDestroy(TObject *Sender)
{
    dmTrayMotor->comTrayStepMotor->StopComm();
    mmoTrayStepMotor=NULL;
}
//------------------------------------------------------------------------------
void TdmTrayMotor::StartSetSpeed()
{
    bNeedSetStepMotorSpeed=true;
}
//------------------------------------------------------------------------------
void TdmTrayMotor::DoTrayStepMotor()                                            //KenHsieh 20211222 : Pad與步進馬達為同一Comport
{
    #ifdef SOFT_SIMULTE
        return;
    #else
    static int iAddr=0;
    int &Task=iSetStepMotorTask;
    int iSpeed=30;

    static int iErrCount=0;                                                     //JerryYang 20240111 : 新增retry

    if(InitialOK==false)                                                        //JerryYang 20230814 : add震動馬達通訊調速版本
        return;

    bRunTimer=true;
    if(bNeedSetStepMotorSpeed)                                                  //JerryYang 20230814 : add震動馬達通訊調速版本
    {
        iAddr=0;
        Task=1;
        bReceiveData=false;
        bNeedSetStepMotorSpeed=false;
    }

    switch(Task)
    {
        case 1:                                                                 //Ifor 20200620 add: 讀取Tray Step Motor版本
            if(iAddr>=esMotTotal)                                               //Ifor 20220916 : 加上保護避免超出陣列大小      //JerryYang 20230915 : add
                iAddr=0;
            CMD_CheckStepMotorVer(iAddr);
            Task=100;
            break;
        case 100:
            if(bReceiveData)
            {
                Task=1000;
            }
            else if(SetStepMotorDelay.Off())
            {
                Task=6000;                                                      //例外處理
            }
            break;
        case 1000:
            if(iAddr==esMotLoader)
                iSpeed=HSys.FuncA.iA01_LoaderTraySpeed;
            else if(iAddr==esMotEmpty1)
                iSpeed=HSys.FuncA.iA01_Empty1TraySpeed;
            else if(iAddr==esMotEmpty2)
                iSpeed=HSys.FuncA.iA01_Empty2TraySpeed;
            else if(iAddr==esMotShift )
                iSpeed=HSys.FuncA.iA01_TransferTraySpeed;
            else if(iAddr==esMotTrack1)
                iSpeed=HSys.FuncA.iA01_Track1TraySpeed;
            else if(iAddr==esMotTrack2)
                iSpeed=HSys.FuncA.iA01_Track2TraySpeed;
            else if(iAddr==esMotTrack3)
                iSpeed=HSys.FuncA.iA01_Track3TraySpeed;
            else if(iAddr==esMotTrack4)
                iSpeed=HSys.FuncA.iA01_Track4TraySpeed;

            CMD_SetStepMotorSpeed(iAddr, iSpeed);
            Task=1100;
            break;
        case 1100:
            if(bReceiveData)
            {
                Task=2000;
            }
            else if(SetStepMotorDelay.Off())
            {
                Task=2000;
                //例外處理
            }
            break;
        case 2000:
            CMD_SetStepMotorSecondSpeedAcc(iAddr);
            Task=2100;
            break;
        case 2100:
            if(bReceiveData)
            {
                Task=3000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=3000;                                                  //例外處理
                }
            }
            break;
        case 3000:
            CMD_SetStepMotorSecondSpeedDec(iAddr);
            Task=3100;
        case 3100:
            if(bReceiveData)
            {
                Task=4000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=4000;                                                  //例外處理
                }
                break;
            }
        case 4000:
            if(iAddr==esMotLoader)
                iSpeed=HSys.FuncA.iA01_LoaderTray2ndSpeed;
            else if(iAddr==esMotEmpty1)
                iSpeed=HSys.FuncA.iA01_Empty1Tray2ndSpeed;
            else if(iAddr==esMotEmpty2)
                iSpeed=HSys.FuncA.iA01_Empty2Tray2ndSpeed;
            else if(iAddr==esMotShift )
                iSpeed=HSys.FuncA.iA01_TransferTray2ndSpeed;
            else if(iAddr==esMotTrack1)
                iSpeed=HSys.FuncA.iA01_Track1Tray2ndSpeed;
            else if(iAddr==esMotTrack2)
                iSpeed=HSys.FuncA.iA01_Track2Tray2ndSpeed;
            else if(iAddr==esMotTrack3)
                iSpeed=HSys.FuncA.iA01_Track3Tray2ndSpeed;
            else if(iAddr==esMotTrack4)
                iSpeed=HSys.FuncA.iA01_Track4Tray2ndSpeed;
            CMD_SetStepMotorSecondSpeedCW(iAddr, iSpeed);
            Task=4100;
        case 4100:
            if(bReceiveData)
            {
                Task=5000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=5000;                                                  //例外處理
                }
                break;
            }
        case 5000:
            if(iAddr==esMotLoader)
                iSpeed=HSys.FuncA.iA01_LoaderTray2ndSpeed;
            else if(iAddr==esMotEmpty1)
                iSpeed=HSys.FuncA.iA01_Empty1Tray2ndSpeed;
            else if(iAddr==esMotEmpty2)
                iSpeed=HSys.FuncA.iA01_Empty2Tray2ndSpeed;
            else if(iAddr==esMotShift )
                iSpeed=HSys.FuncA.iA01_TransferTray2ndSpeed;
            else if(iAddr==esMotTrack1)
                iSpeed=HSys.FuncA.iA01_Track1Tray2ndSpeed;
            else if(iAddr==esMotTrack2)
                iSpeed=HSys.FuncA.iA01_Track2Tray2ndSpeed;
            else if(iAddr==esMotTrack3)
                iSpeed=HSys.FuncA.iA01_Track3Tray2ndSpeed;
            else if(iAddr==esMotTrack4)
                iSpeed=HSys.FuncA.iA01_Track4Tray2ndSpeed;
            CMD_SetStepMotorSecondSpeedCCW(iAddr, iSpeed);
            Task=5100;
        case 5100:
            if(bReceiveData)
            {
                Task=6000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=6000;                                                  //例外處理
                }
                break;
            }
        case 6000:
            iAddr++;

            if(iAddr>=esMotTotal)
            {
                iAddr=0;
                Task=6001;
            }
            else
            {
                Task=1;
            }
            break;
        case 6001:
            break;
    }
        bRunTimer=false;
    #endif
}
//------------------------------------------------------------------------------
void TdmTrayMotor::AddTrayStepMotorLog(AnsiString sFunction, AnsiString str)    //Steven 20200529 : Loader入Tray改步進
{
    AnsiString sReciveData;
    sReciveData.sprintf("%02d:%02d:%02d.%03d, %s, %s", SystemHour, SystemMin, SystemSec, SystemMSec, sFunction, str);
    ShowMSG(sReciveData);
    //StepMot log
    AnsiString sDir=AnsiString().sprintf("D:\\HT-172_Log\\StepMotLog\\%04d_%02d", SystemYear, SystemMonth);
    AnsiString sFileName=AnsiString().sprintf("%s_%04d_%02d_%2d.csv", asHandlerID, SystemYear, SystemMonth, SystemDate);
    AnsiString sDefHead="DataTime, Function, Active, Command, parameter";
    FormSysTools->SaveLogRpt(sDir,sFileName,sDefHead,sReciveData);
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_CheckStepMotorVer(int Axis)                   //ASK Ver
{
    AnsiString StepMotorCMD, Msg;
    StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%02x%02x\r", 0x70+Axis, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, Version", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD);
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSpeed(int Axis, int SpeedPerc)
{
    int iSpeedPerc=0;

    AnsiString StepMotorCMD, Msg;

    iSpeedPerc=SpeedPerc*140;                                                   //Steven 20200630 : 奉Jeremy指示, 先讓機台穩穩跑, 所以減慢速度

    StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, iSpeedPerc);
    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, Speed", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD);
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSecondSpeedCW(int Axis, int SpeedPerc)   //使用Sensor 3做CW減速
{
    AnsiString StepMotorCMD_CW, Msg;
    int iSpeedPerc=SpeedPerc*30;

    StepMotorCMD_CW.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x16, 0x01, 0x05, 0x02, 0x00, 0x00, iSpeedPerc);
    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD_CW.c_str(), strlen(StepMotorCMD_CW.c_str()));
    Msg.sprintf("SEND, %s, CW_Speed", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD_CW);
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSecondSpeedCCW(int Axis, int SpeedPerc)  //使用Sensor 4做CCW減速
{
    AnsiString StepMotorCMD_CCW, Msg;
    int iSpeedPerc=SpeedPerc*30;

    StepMotorCMD_CCW.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x16, 0x01, 0x07, 0x03, 0x00, 0x00, iSpeedPerc);
    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD_CCW.c_str(), strlen(StepMotorCMD_CCW.c_str()));
    Msg.sprintf("SEND, %s, CCW_Speed", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD_CCW);
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSecondSpeedAcc(int Axis)
{
    AnsiString StepMotorCMD, Msg;
    AnsiString asMotorAcc="0";

    if(iSetpMotorVer[Axis]<230)                                                 //Ifor 20200701 : 修改For 新舊版本韌體使用不同的速度
    {
        StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x14, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00);  //Steven 20200616 : Tray Step馬達加減速改成0, 避免暴衝
    }
    else
    {
        StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x14, 0x01, 0x01, 0x00, 0x00, 0x00, 0xC8);
        asMotorAcc="200";
    }

    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, ACC", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD);
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSecondSpeedDec(int Axis)
{
    AnsiString StepMotorCMD, Msg;
    AnsiString asMotorDec="0";
    if(iSetpMotorVer[Axis]<230)                                                 //Ifor 20200701 : 修改For 新舊版本韌體使用不同的速度
    {
        StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x14, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00);  //Steven 20200616 : Tray Step馬達加減速改成0, 避免暴衝
    }
    else
    {
        StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x14, 0x01, 0x03, 0x00, 0x00, 0x00, 0x64);
        asMotorDec="100";
    }

    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, DEC", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD);
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetVibrationMotorSpeed(int Axis)              //JerryYang 20230814 : add震動馬達通訊調速版本
{
    AnsiString sVibrationMotorCMD, Msg;
    int iHP1=100, iHP2=100, iSHT1=100, iSHT2=100, iAUTO1=100, iAUTO2=100, iAUTO3=100;

    if(Axis==0)
        sVibrationMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%02x%02x\r", 0x80+Axis, 0x04, iHP1, iHP2, iSHT1, iSHT2,0x00,0x00,0x00);
    else
        sVibrationMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%02x%02x\r", 0x80+Axis, 0x04, iAUTO1, iAUTO2, iAUTO3, 0x00, 0x00, 0x00, 0x00);

    bReceiveVibrateMotData=false;
    SetVibrateMotorDelay.SetSecAndOn(dVibrateMotorDelay);
    comTrayStepMotor->WriteCommData(sVibrationMotorCMD.c_str(), strlen(sVibrationMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, DEC", sVibrationMotorAxis[Axis]);
}
//------------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_CheckVibrationMotVer(int Axis)                //ASK Ver
{
    AnsiString StepMotorCMD, Msg;

    StepMotorCMD.sprintf("t%03x80000000000000000\r", 0x80+Axis);

    bReceiveVibrateMotData=false;
    SetVibrateMotorDelay.SetSecAndOn(dVibrateMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, Version", sVibrationMotorAxis[Axis]);
}
//------------------------------------------------------------------------------
void TdmTrayMotor::SendCommand(AnsiString str)
{
    str.sprintf("%s\r", str);
    comTrayStepMotor->WriteCommData(str.c_str(), strlen(str.c_str()));
}
//------------------------------------------------------------------------------
void TdmTrayMotor::ShowMSG(AnsiString asStr)
{
    if(brecordmsgLock==false)
    {
        brecordmsgLock=true;
        if(mmoTrayStepMotor->Lines->Count>1024)
            mmoTrayStepMotor->Clear();
        mmoTrayStepMotor->Lines->Add(asStr);
        brecordmsgLock=false;
    }
}
//---------------------------------------------------------------------------
