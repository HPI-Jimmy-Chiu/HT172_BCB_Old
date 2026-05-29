#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "ComPort.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma link "SPComm"
#pragma link "ALed"
#pragma link "BtnPanelLane"
#pragma link "MyLed"
#pragma resource "*.dfm"
#include "uPadInterface.h"
#include "TextProcess.h"
#include "TrayStepMotor.h"
TfComPort *fComPort;
//------------------------------------------------------------------------------
__fastcall TfComPort::TfComPort(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::FormDestroy(TObject *Sender)
{
    StopAllCom();
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::FormShow(TObject *Sender)
{
    OpenWorkFile();
    bShow=true;
    if(HSys.FuncT.iT04_BinDisptype==0)
    {
        tsNormal->TabVisible=true;
        tsTFT->TabVisible=false;
    }
    else if(HSys.FuncT.iT04_BinDisptype==1)
    {
        tsNormal->TabVisible=false;
        tsTFT->TabVisible=true;
    }
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::SaveWorkFile()
{
    AnsiString S=HSys.CurrentDir+AnsiString("\\system\\ComPort.ini");
    FormSysTools->OpenFormData(S);
    for(int iP=0; iP<pgComPort->ControlCount; iP++)
    {
        TControl *P=pgComPort->Controls[iP];

        TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);

        if(TabPtr!=NULL)
        {
            FormSysTools->SaveFormData(TabPtr, TabPtr->Name);
        }
    }

    FormSysTools->CloseFormData();
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::OpenWorkFile()
{
    AnsiString S=HSys.CurrentDir+AnsiString("\\system\\ComPort.ini");
    if(CheckFileExist(S.c_str()))
    {
        FormSysTools->OpenFormData(S);
        for(int iP=0; iP<pgComPort->ControlCount; iP++)
        {
            TControl *P=pgComPort->Controls[iP];

            TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);

            if(TabPtr!=NULL)
            {
                FormSysTools->LoadFormData(TabPtr, TabPtr->Name);
            }
        }
        FormSysTools->CloseFormData();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::sbUpdateClick(TObject *Sender)
{
    SaveWorkFile();
    sbUpdate->Down=false;
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::sbExitClick(TObject *Sender)
{
    sbExit->Down=false;
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::RS232Init()
{                  
    AnsiString Str;
    #ifndef SOFT_SIMULATE
    int iErr=0;
    StopAllCom();
    bool flag=false;
    bPadInterfaceAndTrayStepCommPort=(cbPadComm->Text==cbStepTrayComm->Text)?true:false;


    if(bPadInterfaceAndTrayStepCommPort==false)
    {
        flag=GetCOMPortStatus(cbPadComm->Text);
        if(flag==false)
        {
            iErr = GetLastError();
            ShowMyMessage(" Control Panel  : "+cbPadComm->Text+" port error , Error Code = "+iErr);
        }
        else
        {
            fPadInterface->PadComm=PadComm;
            fPadInterface->PadComPort=cbPadComm->Text;
            PadComm->CommName="\\\\.\\"+fPadInterface->PadComPort;
            PadComm->ReadIntervalTimeout=1;                                         //Sam 20220617 : 加快控制面板通訊
            try
            {
                PadComm->Parity=None;
                PadComm->BaudRate=115200;
                PadComm->ByteSize=_8;
                PadComm->ParityCheck=false;
                PadComm->StopBits=_1;
                PadComm->StopComm();
                PadComm->StartComm();                                               //僅能啟動一次
                fPadInterface->RecordCommunication("[Connect]", " OK");
                fPadInterface->bRs232Ok=true;
            }
            catch(...)
            {
                fPadInterface->RecordCommunication("[Connect]", " FAIL");
                ShowMyMessage("Control Panel : "+cbPadComm->Text+" port error");
                fPadInterface->bRs232Ok=false;
            }
        }
    }



    flag=GetCOMPortStatus(cbStepTrayComm->Text);
    if(flag==false)
    {
        iErr = GetLastError();
        ShowMyMessage(" Control Panel  : "+cbStepTrayComm->Text+" port error , Error Code = "+iErr);
    }
    else
    {
        dmTrayMotor->comTrayStepMotor=StepTrayComm;
        StepTrayComm->CommName="\\\\.\\"+cbStepTrayComm->Text;
        try
        {
            StepTrayComm->Parity=None;
            StepTrayComm->BaudRate=115200;
            StepTrayComm->ByteSize=_8;
            StepTrayComm->ParityCheck=false;
            StepTrayComm->StopBits=_1;
            StepTrayComm->StopComm();
            StepTrayComm->StartComm();                                          //僅能啟動一次
            if(bPadInterfaceAndTrayStepCommPort)
                fPadInterface->bRs232Ok=true;
        }
        catch(...)
        {
            ShowMyMessage("Control Panel : "+cbStepTrayComm->Text+" port error");
            if(bPadInterfaceAndTrayStepCommPort)
                fPadInterface->bRs232Ok=false;
        }
    }

    if(dmTrayMotor!=NULL)
        dmTrayMotor->mmoTrayStepMotor=memoStepMot;

    #endif
    if(HSys.FuncT.iT04_BinDisptype==0)
    {
        HSys.BinDisCtrl->SetComPort(cbBinComm->Text);
        HSys.BinDisCtrl->SetDelayTime(atoi(labedtNumDelay->Text.c_str()));
        HSys.BinDisCtrl->InitialOK=true;
        HSys.BinDisCtrl->CommBin=commBinDisStore;

        HSys.BinDisCtrlMag->SetComPort(cbMagBinComm->Text);                     //JerryYang 20240601 : Add MGZ Bin Display
        HSys.BinDisCtrlMag->SetDelayTime(atoi(labedtNumDelay->Text.c_str()));
        HSys.BinDisCtrlMag->CommBin=commBinDisMag;
    }
    else if(HSys.FuncT.iT04_BinDisptype==1)
    {
        HSys.BinDisCtrlTFT->SetComPort(cbBinComm->Text);
        HSys.BinDisCtrlTFT->SetDelayTime(atoi(labedtNumDelay->Text.c_str()));
        if(HSys.FuncT.iT05_BinDispComHardwareType==0)
            HSys.BinDisCtrlTFT->SetUsedBinNumber(eBinDispTotal);
        else
            HSys.BinDisCtrlTFT->SetUsedBinNumber(eBinDispTotal+eBinDispMagTotal);
        HSys.BinDisCtrlTFT->InitialOK=true;
        HSys.BinDisCtrlTFT->CommBin=commBinDisStore;
        HSys.BinDisCtrlTFT->CommBin->OnReceiveData=HSys.BinDisCtrlTFT->CommBinReceiveData;
        HSys.BinDisCtrlTFT->CommBin->Parity=HSys.BinDisCtrlTFT->ComParity;

        if(HSys.FuncT.iT05_BinDispComHardwareType==0)                           //JerryYang 20250621 : Auto跟Magazine使用不同COM Port
        {
            #ifndef SOFT_SIMULATE
            HSys.BinDisCtrlMagTFT->SetComPort(cbMagBinComm->Text);
            HSys.BinDisCtrlMagTFT->SetDelayTime(atoi(labedtNumDelay->Text.c_str()));
            HSys.BinDisCtrlMagTFT->CommBin=commBinDisMag;
            HSys.BinDisCtrlMagTFT->InitialOK=true;
            HSys.BinDisCtrlMagTFT->CommBin->OnReceiveData=HSys.BinDisCtrlMagTFT->CommBinReceiveData;
            HSys.BinDisCtrlMagTFT->CommBin->Parity=HSys.BinDisCtrlMagTFT->ComParity;
            #endif
        }
    }
}
//------------------------------------------------------------------------------
void TfComPort::MemoAddString(TMemo *memo, AnsiString asTitle, AnsiString Str)
{
    if(memo->Lines->Count>4096)
        memo->Clear();

    AnsiString str;

    if(asTitle!="")
        str.sprintf("%02d:%02d:%02d %s: %s", SystemHour, SystemMin, SystemSec, asTitle, Str);
    else
        str.sprintf("%02d:%02d:%02d %s", SystemHour, SystemMin, SystemSec, Str);

    memo->Lines->Add(str);
}
//------------------------------------------------------------------------------
void TfComPort::StopAllCom()
{
    PadComm->StopComm();                                                        //KenHsieh 20220207 : Add PadInterface
    StepTrayComm->StopComm();                                                   //KenHsieh 20220207 : Add PadInterface
//    commBinDisStore->StopComm();
//    commBinDisMag->StopComm();
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::spbResetComClick(TObject *Sender)
{
    StopAllCom();
    RS232Init();
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
extern AnsiString sStepMotorAxis[esMotTotal];
extern int iSetpMotorVer[esMotTotal];
void __fastcall TfComPort::PadCommReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString sAxis="", sFunc="", sSubSunc="", Msg="";
    int iAxis=0;
    AnsiString sReciveData=(char *)Buffer;
    AnsiString sUnit=sReciveData.SubString(1, 3);                               //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    if(sUnit=="t07")                                                            //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    {
        sAxis=sReciveData.SubString(4, 1);
        sFunc=sReciveData.SubString(6, 2);
        sSubSunc=sReciveData.SubString(11, 3);
        iAxis=atoi(sAxis.c_str());                                              //Steven 20200706 : 確保數字沒有超出陣列

        if(iAxis>=0 && iAxis<esMotTotal)
        {
            if(sFunc=="07")
            {
                Msg.sprintf("RECV, %s, Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="15")
            {
                if(sSubSunc=="200")
                    Msg.sprintf("RECV, %s, ACC", sStepMotorAxis[atoi(sAxis.c_str())]);
                else if(sSubSunc=="400")
                    Msg.sprintf("RECV, %s, DEC", sStepMotorAxis[atoi(sAxis.c_str())]);
                else
                    Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="17")
            {
                if(sSubSunc=="602")
                    Msg.sprintf("RECV, %s, CW_Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
                else if(sSubSunc=="803")
                    Msg.sprintf("RECV, %s, CCW_Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
                else
                    Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="01")                                                //Ifor 20200620 add: 讀取Tray Step Motor版本
            {
                Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
                int iAxisAdd=atoi(sAxis.c_str());
                AnsiString asVersion="";
                AnsiString asData="";
                int iNumber=0;
                int iHight=0;
                int iLow=0;
                for(int i=0; i<7; i++)
                {
                    iNumber=atoi(sReciveData.SubString(8+i*2, 2).c_str());
                    iHight=iNumber/10;
                    iLow=iNumber%10;
                    asData=MyDeCodeASCII((iHight*16)+iLow);
                    asVersion=asVersion+asData;
                }
                iSetpMotorVer[iAxisAdd]=atoi(asVersion.SubString(asVersion.AnsiPos("I")+1, 3).c_str());
            }
            else
            {
                Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
        }
        else
        {
            Msg.sprintf("RECV, Data format error!");
        }
        dmTrayMotor->AddTrayStepMotorLog(Msg, sReciveData);
        bReceiveData=true;
    }
    else if(sUnit=="t05")                                                       //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    {
        fPadInterface->CommReceiveLength->Add(BufferLength);
        fPadInterface->CommReceiveList->Add(sReciveData);                       //Sam 20220617 asCommand >> sReciveData
        MemoAddString(memoPanelCom, "[Recv]", sReciveData);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::sbPanelSend_ComClick(TObject *Sender)
{
    fPadInterface->SendCommand(edPanelSend_Com->Text);
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::RecordCommunication(AnsiString aTitle, AnsiString Command)  //JerryYang 20230814 : add震動馬達通訊調速版本
{

}
//--------------------------------------------------------------------------
void TfComPort::Spin()
{
    #ifndef SOFT_SIMULATE 
    fPadInterface->Main232();
    dmTrayMotor->DoTrayStepMotor();
    if(bShow==false)
    {
        if(HSys.FuncT.iT04_BinDisptype==0)
        {
            HSys.BinDisCtrl->Spin();
            HSys.BinDisCtrlMag->Spin();
        }
        else
        {
            HSys.BinDisCtrlTFT->SpinTFT();
            if(HSys.FuncT.iT05_BinDispComHardwareType==0)
                HSys.BinDisCtrlMagTFT->SpinTFTInitialOnly();
        }
    }
    #endif
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::btnStartClick(TObject *Sender)                       //20240430 Bin test
{
    HSys.BinDisCtrl->bFirstInit=true;
    HSys.BinDisCtrl->ProcessStopStart(true);
    HSys.BinDisCtrlMag->bFirstInit=true;
    HSys.BinDisCtrlMag->ProcessStopStart(true);
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::btnBinSetTestClick(TObject *Sender)
{
    if(CosFunction.bUseBinAutoConvert)
    {
        int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
        int iBin=atoi(lbledtBin->Text.c_str());
        if(HSys.FuncT.iT04_BinDisptype==1)
        {
            HSys.BinDisCtrlTFT->SetBin_TFT_number(iUnit, iBin);
        }
    }
    else
    {
        int iUnit=0, iBin[eBinDispTotal][TEST_MAX_BIN]={-1};
        iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
        for(int i=0; i<eBinDispTotal; i++)
        {
            for(int iB=0; iB<TEST_MAX_BIN; iB++)
            {
                iBin[i][iB]=-1;
            }
        }
        iBin[0][0]=atoi(lbledtBin->Text.c_str());
        if(HSys.FuncT.iT04_BinDisptype==0)
        {
            HSys.BinDisCtrl->WriteTargetBin(iUnit, iBin[0], 1);
            HSys.BinDisCtrlMag->WriteTargetBin(iUnit, iBin[0], 1);
        }
        else if(HSys.FuncT.iT04_BinDisptype==1)
        {
            HSys.BinDisCtrlTFT->SetBin_TFT_number(iUnit, iBin[0]);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::StepTrayCommReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString sAxis="", sFunc="", sSubSunc="", Msg="";
    int iAxis=0;
    AnsiString sReciveData=(char *)Buffer;
    AnsiString sUnit=sReciveData.SubString(1, 3);                               //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    if(sUnit=="t07")                                                            //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    {
        sAxis=sReciveData.SubString(4, 1);
        sFunc=sReciveData.SubString(6, 2);
        sSubSunc=sReciveData.SubString(11, 3);
        iAxis=atoi(sAxis.c_str());                                              //Steven 20200706 : 確保數字沒有超出陣列

        if(iAxis>=0 && iAxis<esMotTotal)
        {
            if(sFunc=="07")
            {
                Msg.sprintf("RECV, %s, Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="15")
            {
                if(sSubSunc=="200")
                    Msg.sprintf("RECV, %s, ACC", sStepMotorAxis[atoi(sAxis.c_str())]);
                else if(sSubSunc=="400")
                    Msg.sprintf("RECV, %s, DEC", sStepMotorAxis[atoi(sAxis.c_str())]);
                else
                    Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="17")
            {
                if(sSubSunc=="602")
                    Msg.sprintf("RECV, %s, CW_Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
                else if(sSubSunc=="803")
                    Msg.sprintf("RECV, %s, CCW_Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
                else
                    Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="01")                                                //Ifor 20200620 add: 讀取Tray Step Motor版本
            {
                Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
                int iAxisAdd=atoi(sAxis.c_str());
                AnsiString asVersion="";
                AnsiString asData="";
                int iNumber=0;
                int iHight=0;
                int iLow=0;
                for(int i=0; i<7; i++)
                {
                    iNumber=atoi(sReciveData.SubString(8+i*2, 2).c_str());
                    iHight=iNumber/10;
                    iLow=iNumber%10;
                    asData=MyDeCodeASCII((iHight*16)+iLow);
                    asVersion=asVersion+asData;
                }
                iSetpMotorVer[iAxisAdd]=atoi(asVersion.SubString(asVersion.AnsiPos("I")+1, 3).c_str());
            }
            else
            {
                Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
        }
        else
        {
            Msg.sprintf("RECV, Data format error!");
        }
        dmTrayMotor->AddTrayStepMotorLog(Msg, sReciveData);
        dmTrayMotor->bReceiveData=true;
    }
    else if(sUnit=="t05")                                                       //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    {
        fPadInterface->CommReceiveLength->Add(BufferLength);
        fPadInterface->CommReceiveList->Add(sReciveData);                       //Sam 20220617 asCommand >> sReciveData
        MemoAddString(memoPanelCom, "[Recv]", sReciveData);
        fPadInterface->RecordCommunication("[Recv]", sReciveData);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::btnStepTrayMotSendClick(TObject *Sender)
{
    AnsiString str=Edit2->Text;
    dmTrayMotor->SendCommand(str);
    dmTrayMotor->AddTrayStepMotorLog(__FUNC__,"[Manual Send]"+str);
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::btnReadVersionClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->ReadVersion_TFT(iUnit);
}
//------------------------------------------------------------------------------
void __fastcall TfComPort::btnSetBinFontNumClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->SetBinFont_TFT_number(iUnit);
}
//--------------------------------------------------------------------------- 
void __fastcall TfComPort::btnSetBinFontBinClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->SetBinFont_TFT_bin(iUnit);
}
//---------------------------------------------------------------------------
void __fastcall TfComPort::btnSetBinFontEAClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->SetBinFont_TFT_EA(iUnit);
}
//---------------------------------------------------------------------------
void __fastcall TfComPort::btnTFTSetCountClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->SetBinFont_TFT_Count(iUnit);
}
//---------------------------------------------------------------------------
void __fastcall TfComPort::btnTFTSetBackGroundClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->SetBackGround_TFT(iUnit);
}
//--------------------------------------------------------------------------- 
void __fastcall TfComPort::btnSetBin_TFT_binClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->SetBin_TFT_bin(iUnit);
}
//---------------------------------------------------------------------------
void __fastcall TfComPort::btnWriteScreenNoBackGround_TFTClick(
      TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->WriteScreenNoBackGround_TFT(iUnit);
}
//---------------------------------------------------------------------------
void __fastcall TfComPort::btnSetBin_TFT_EAClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->SetBin_TFT_EA(iUnit);
}
//---------------------------------------------------------------------------
void __fastcall TfComPort::btnSetBin_TFT_CountClick(TObject *Sender)
{
    int iUnit=atoi(lbledtBinUnit->Text.c_str())-1;
    HSys.BinDisCtrlTFT->SetBin_TFT_Count(iUnit);
}
//---------------------------------------------------------------------------
void __fastcall TfComPort::btnDipComStartClick(TObject *Sender)
{
    HSys.BinDisCtrlTFT->CommBin=commBinDisStore;
    HSys.BinDisCtrlTFT->StartComport(HSys.BinDisCtrlTFT->CommBin,cbBinComm->Text);
}
//--------------------------------------------------------------------------- 
void __fastcall TfComPort::btnDipComStopClick(TObject *Sender)
{
    HSys.BinDisCtrlTFT->CommBin=commBinDisStore;
    HSys.BinDisCtrlTFT->StopComport(HSys.BinDisCtrlTFT->CommBin,cbBinComm->Text);
}
//---------------------------------------------------------------------------

