#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "cpublic.h"

#include "cCCDCtrl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm2DID *frm2DID;

//---------------------------------------------------------------------------
AnsiString asBarCodeCommLogPath     ="D:\\HT-172_Log\\BarCodeComm";             //Steven 20160421 : Add 2D CCD Log
//---------------------------------------------------------------------------
enum CCD_PHOTO_STEP
{
    STEP_PHOTO_INIT = 0,
    STEP_PHOTO_LIGHT_ON,
    STEP_PHOTO_MOVE_MIN,
    STEP_PHOTO_MOVE_MAX,
    STEP_PHOTO_DELAY,
    STEP_PHOTO_WAIT,
    STEP_PHOTO_FINISH,
    STEP_PHOTO_END = -1
};
//---------------------------------------------------------------------------
const FSM_STEP CCDPhotoSteps[] =
{
    { STEP_PHOTO_INIT,      STEP_PHOTO_LIGHT_ON, 0,   1,      STEP_PHOTO_FINISH },
    { STEP_PHOTO_LIGHT_ON,  STEP_PHOTO_MOVE_MIN, 0,   0,      STEP_PHOTO_FINISH },
    { STEP_PHOTO_MOVE_MIN,  STEP_PHOTO_MOVE_MAX, 0,   0,      STEP_PHOTO_FINISH },
    { STEP_PHOTO_MOVE_MAX,  STEP_PHOTO_WAIT,     0,   0,      STEP_PHOTO_FINISH },
    { STEP_PHOTO_WAIT,      STEP_PHOTO_FINISH,   0,   0,      STEP_PHOTO_FINISH },
    { STEP_PHOTO_FINISH,    -1,                  0,   0,      STEP_PHOTO_FINISH },
    { -1,                   -1,                  0,   0,      -1 }
};
//---------------------------------------------------------------------------
FSM_RESULT TMyCCDIDPanel::DoExecCCDStep(int iAct)
{
    switch(iAct)
    {
        case (int)STEP_PHOTO_INIT:
            return FSM_PhotoInit();
        case (int)STEP_PHOTO_LIGHT_ON:
            return FSM_LightOn();
        case (int)STEP_PHOTO_MOVE_MIN:
            return FSM_MoveToMin();
        case (int)STEP_PHOTO_MOVE_MAX:
            return FSM_MoveToMax();
        case (int)STEP_PHOTO_WAIT:
            return FSM_WaitPhoto();
        case (int)STEP_PHOTO_FINISH:
            return FSM_PhotoFinish();
        default:
            return FSM_ERROR;
    }
}
//---------------------------------------------------------------------------
__fastcall Tfrm2DID::Tfrm2DID(TComponent* Owner)
    : TForm(Owner)
{
    MyCCD[eLoader1]=new TMyCCDIDPanel(this, "Loader", eLoader1);
    MyCCD[eEmpty1]=new TMyCCDIDPanel(this, "Empty1", eEmpty1);
    MyCCD[eEmpty2]=new TMyCCDIDPanel(this, "Empty2", eEmpty2);
}
//---------------------------------------------------------------------------
void Tfrm2DID::AlignGUI(bool bShowOnSetup)
{
    if(bShowOnSetup && fSetup!=NULL)
    {
        MyCCD[eLoader1]->gbClipID->Parent=fSetup->tsCCD;
        MyCCD[eEmpty1]->gbClipID->Parent=fSetup->tsCCD;
        MyCCD[eEmpty2]->gbClipID->Parent=fSetup->tsCCD;
    }
    else
    {
        if(frm2DID!=NULL && scrlbxCCD!=NULL)
        {
            MyCCD[eLoader1]->gbClipID->Parent=scrlbxCCD;
            MyCCD[eEmpty1]->gbClipID->Parent=scrlbxCCD;
            MyCCD[eEmpty2]->gbClipID->Parent=scrlbxCCD;
        }
    }
}
//---------------------------------------------------------------------------
void Tfrm2DID::Initialization()
{
    AlignGUI(true);

    MyCCD[eLoader1]->SetLimit(10,7200);                                         //JerryYang 20240905 : 1400 -> 100
    MyCCD[eLoader1]->SetCCDArm(HSys.Mot.LoaderCCDArmX);
}
//---------------------------------------------------------------------------
void __fastcall Tfrm2DID::FormDestroy(TObject *Sender)
{
    for(int i=0; i<eLoaderTotal; i++)
    {
        MyCCD[i]->TimerCCDConnect->Enabled=false;
        MyCCD[i]->cSocketClipID->Close();
        delete MyCCD[i];
    }
}
//---------------------------------------------------------------------------
__fastcall TMyCCDIDPanel::TMyCCDIDPanel(TComponent* Owner, AnsiString _name, int _Tag) : TComponent(Owner)
{
    AnsiString Str;
    bTimerRun=false;
    iConnectCount=0;
    bEnableCCD=false;
    iMyTag=_Tag;
    sName =_name;

    Str.sprintf("gbClipID_%s", sName);
    gbClipID=new TGroupBox(frm2DID->scrlbxCCD);
    gbClipID->Name=Str;
    gbClipID->Caption=sName;
    gbClipID->Parent=frm2DID->scrlbxCCD;
    gbClipID->Tag=iMyTag;
    gbClipID->Top=250*_Tag+1;
    gbClipID->Height=245;
    gbClipID->Align=alTop;
    gbClipID->Font->Name="Arial";
    gbClipID->Font->Charset=DEFAULT_CHARSET;
    gbClipID->Font->Size=14;

    labAddr=new TLabel(gbClipID);
    Str.sprintf("labAddr_%s", sName);
    labAddr->Name=Str;
    labAddr->Parent=gbClipID;
    labAddr->Height=18;
    labAddr->Left  =9;
    labAddr->Top   =22;
    labAddr->Width =59;
    labAddr->Font->Name="Arial";
    labAddr->Font->Charset=DEFAULT_CHARSET;
    labAddr->Font->Size=12;
    labAddr->Caption="Address";

    labPort=new TLabel(gbClipID);
    Str.sprintf("labPort_%s", sName);
    labPort->Name=Str;
    labPort->Parent=gbClipID;
    labPort->Height=18;
    labPort->Left  =143;
    labPort->Top   =22;
    labPort->Width =29;
    labPort->Font->Name="Arial";
    labPort->Font->Charset=DEFAULT_CHARSET;
    labPort->Font->Size=12;
    labPort->Caption="Port";

    labMax=new TLabel(gbClipID);
    Str.sprintf("labMax_%s", sName);
    labMax->Name=Str;
    labMax->Parent=gbClipID;
    labMax->Height=18;
    labMax->Left  =260;
    labMax->Top   =46;
    labMax->Width =100;
    labMax->Font->Name="Arial";
    labMax->Font->Charset=DEFAULT_CHARSET;
    labMax->Font->Size=12;
    labMax->Caption="Max Length";
    labMax->Visible=false;                                                      //以讀取之值去搜尋ID，故應是用不到

    labMin=new TLabel(gbClipID);
    Str.sprintf("labMin_%s", sName);
    labMin->Name=Str;
    labMin->Parent=gbClipID;
    labMin->Height=18;
    labMin->Left  =400;
    labMin->Top   =46;
    labMin->Width =100;
    labMin->Font->Name="Arial";
    labMin->Font->Charset=DEFAULT_CHARSET;
    labMin->Font->Size=12;
    labMin->Caption="Min Length";
    labMin->Visible=false;                                                      //以讀取之值去搜尋ID，故應是用不到

    labErr=new TLabel(gbClipID);
    Str.sprintf("labErr_%s", sName);
    labErr->Name=Str;
    labErr->Parent=gbClipID;
    labErr->Height=18;
    labErr->Left  =540;
    labErr->Top   =46;
    labErr->Width =100;
    labErr->Font->Name="Arial";
    labErr->Font->Charset=DEFAULT_CHARSET;
    labErr->Font->Size=12;
    labErr->Caption="Error Code";
    labErr->Visible=false;                                                      //以讀取之值去搜尋ID，故應是用不到

    edAddr=new TEdit(gbClipID);
    Str.sprintf("edAddr_%s", sName);
    edAddr->Name=Str;
    edAddr->Parent=gbClipID;
    edAddr->Height=26;
    edAddr->Left  =7;
    edAddr->Top   =46;
    edAddr->Width =130;
    edAddr->Font->Name="Arial";
    edAddr->Font->Charset=DEFAULT_CHARSET;
    edAddr->Font->Size=12;
    edAddr->Text="127.000.000.100";
    edAddr->Tag=5;                    //Sam 20240729 : 修正2D的資料輸入
    edAddr->OnClick=edAddrClick;

    edPort=new TEdit(gbClipID);
    Str.sprintf("edPort_%s", sName);
    edPort->Name=Str;
    edPort->Parent=gbClipID;
    edPort->Height=26;
    edPort->Left  =142;
    edPort->Top   =46;
    edPort->Width =91;
    edPort->Font->Name="Arial";
    edPort->Font->Charset=DEFAULT_CHARSET;
    edPort->Font->Size=12;
    edPort->Text=5000+_Tag;
    edPort->Tag=6;
    edPort->OnClick=edPortClick;

    edCmd=new TEdit(gbClipID);
    Str.sprintf("edCmd_%s", sName);
    edCmd->Name=Str;
    edCmd->Parent=gbClipID;
    edCmd->Height=26;
    edCmd->Left  =131;
    edCmd->Top   =78;
    edCmd->Width =102;
    edCmd->Font->Name="Arial";
    edCmd->Font->Charset=DEFAULT_CHARSET;
    edCmd->Font->Size=12;
    edCmd->Text="";
    edCmd->Tag=7;
    edCmd->OnClick=edErr2DClick;

    edMax2DLen=new TEdit(gbClipID);
    Str.sprintf("edMaxIDLen_%s", sName);
    edMax2DLen->Name=Str;
    edMax2DLen->Parent=gbClipID;
    edMax2DLen->Height=26;
    edMax2DLen->Left  =260;
    edMax2DLen->Top   =70;
    edMax2DLen->Width =100;
    edMax2DLen->Font->Name="Arial";
    edMax2DLen->Font->Charset=DEFAULT_CHARSET;
    edMax2DLen->Font->Size=12;
    edMax2DLen->Text="10";
    edMax2DLen->OnClick=edMax2DClick;
    edMax2DLen->Visible=false;                                                  //以讀取之值去搜尋ID，故應是用不到

    edMin2DLen=new TEdit(gbClipID);
    Str.sprintf("edMinIDLen_%s", sName);
    edMin2DLen->Name=Str;
    edMin2DLen->Parent=gbClipID;
    edMin2DLen->Height=26;
    edMin2DLen->Left  =400;
    edMin2DLen->Top   =70;
    edMin2DLen->Width =100;
    edMin2DLen->Font->Name="Arial";
    edMin2DLen->Font->Charset=DEFAULT_CHARSET;
    edMin2DLen->Font->Size=12;
    edMin2DLen->Text="5";
    edMin2DLen->OnClick=edMin2DClick;
    edMin2DLen->Visible=false;                                                  //以讀取之值去搜尋ID，故應是用不到

    edErr2DStr=new TEdit(gbClipID);
    Str.sprintf("edErrIDStr_%s", sName);
    edErr2DStr->Name=Str;
    edErr2DStr->Parent=gbClipID;
    edErr2DStr->Height=26;
    edErr2DStr->Left  =540;
    edErr2DStr->Top   =70;
    edErr2DStr->Width =100;
    edErr2DStr->Font->Name="Arial";
    edErr2DStr->Font->Charset=DEFAULT_CHARSET;
    edErr2DStr->Font->Size=12;
    edErr2DStr->Text="ERROR";
    edErr2DStr->OnClick=edErr2DClick;
    edErr2DStr->Visible=false;                                                  //以讀取之值去搜尋ID，故應是用不到

    cbEnable=new TCheckBox(gbClipID);
    Str.sprintf("cbEnable_%s", sName);
    cbEnable->Name=Str;
    cbEnable->Parent=gbClipID;
    cbEnable->Height=17;
    cbEnable->Left  =260;
    cbEnable->Top   =20;
    cbEnable->Width =400;
    cbEnable->Font->Name="Arial";
    cbEnable->Font->Charset=DEFAULT_CHARSET;
    cbEnable->Font->Color=clRed;
    cbEnable->Font->Size=14;
    cbEnable->Checked=false;
    Str.sprintf("Enable Clip ID of %s", sName);
    cbEnable->Caption=Str;

    btConnect=new TButton(gbClipID);
    Str.sprintf("btConnect_%s", sName);
    btConnect->Name=Str;
    btConnect->Parent=gbClipID;
    btConnect->Height=30;
    btConnect->Left  =6;
    btConnect->Top   =76;
    btConnect->Width =120;
    btConnect->Font->Name="Arial";
    btConnect->Font->Charset=DEFAULT_CHARSET;
    btConnect->Font->Size=12;
    btConnect->Font->Color=clGreen;
    btConnect->Caption="Connect";
    btConnect->OnClick=BtConnectClick;

    btDisConnect=new TButton(gbClipID);
    Str.sprintf("btDisConnect_%s", sName);
    btDisConnect->Name=Str;
    btDisConnect->Parent=gbClipID;
    btDisConnect->Height=30;
    btDisConnect->Left  =6;
    btDisConnect->Top   =112;
    btDisConnect->Width =120;
    btDisConnect->Font->Name="Arial";
    btDisConnect->Font->Charset=DEFAULT_CHARSET;
    btDisConnect->Font->Size=12;
    btDisConnect->Font->Color=TColor(0x00404080);
    btDisConnect->Caption="Disconnect";
    btDisConnect->OnClick=BtDisconnectClick;

    btSend=new TButton(gbClipID);
    Str.sprintf("btSend_%s", sName);
    btSend->Name=Str;
    btSend->Parent=gbClipID;
    btSend->Height=30;
    btSend->Left  =132;
    btSend->Top   =112;
    btSend->Width =120;
    btSend->Font->Name="Arial";
    btSend->Font->Charset=DEFAULT_CHARSET;
    btSend->Font->Size=12;
    btSend->Font->Color=clWindowText;
    btSend->Caption="Send";
    btSend->OnClick=BtTriggerClick;

    meLog=new TMemo(gbClipID);
    Str.sprintf("meLog_%s", sName);
    meLog->Name=Str;
    meLog->Parent=gbClipID;
    meLog->Color=TColor(0x00DFD9CC);
    meLog->Height=90;
    meLog->Font->Name="Arial";
    meLog->Font->Charset=DEFAULT_CHARSET;
    meLog->Font->Size=10;
    meLog->Align =alBottom;
    meLog->Lines->Clear();
    meLog->ScrollBars=ssVertical;

    cSocketClipID=new TClientSocket(gbClipID);
    Str.sprintf("cSocketClipID_%s", sName);
    cSocketClipID->Name=Str;
    cSocketClipID->OnConnect=ClientSocket_Connect;
    cSocketClipID->OnDisconnect=ClientSocket_Disconnect;
    cSocketClipID->OnError=ClientSocket_Error;
    cSocketClipID->OnRead=ClientSocket_Read;

    TimerCCDConnect=new TTimer(gbClipID);
    Str.sprintf("TimerCCDConnect_%s", sName);
    TimerCCDConnect->Name=Str;
    TimerCCDConnect->Enabled=false;
    TimerCCDConnect->Interval=100;
    TimerCCDConnect->OnTimer=TimerCCDConnectTimer;

    //FSM
    bCanMove=false;
    sClipID="";
    iMin=iMax=0;
}
//---------------------------------------------------------------------------
__fastcall TMyCCDIDPanel::~TMyCCDIDPanel()
{
    cSocketClipID->Active=false;
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::ClientSocket_Connect(TObject *Sender, TCustomWinSocket *Socket)
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;

    if(InitialOK==false || bEnableCCD==false)
    {
        Ptr->Active=false;
        return;
    }
    SendCCDCommand("[Connect OK]", "");
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::ClientSocket_Disconnect(TObject *Sender, TCustomWinSocket *Socket)
{
    SendCCDCommand("[DisConnect OK]", "");
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::ClientSocket_Error(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    if(InitialOK==false)
    {
        return;
    }

    AnsiString S;
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    S=GetSocketErrorMsg(ErrorCode);
    ErrorCode =0;
    ErrorEvent=TErrorEvent(NULL);
    try
    {                            
        SendCCDCommand(S, "");
        Ptr->Close();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfBarCode::ClientSocket_Shuttle1_AError", S);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::ClientSocket_Read(TObject *Sender, TCustomWinSocket *Socket)
{
    if(InitialOK==false)
        return;

    AnsiString EthernetBuffer;
    AnsiString Log;
    int iBufferLenght=0;
    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0) //13.09.30.01   klutter
    {
        EthernetBuffer=Socket->ReceiveText();
        EthernetBuffer=StringReplace(EthernetBuffer, "\n", "", TReplaceFlags()<<rfReplaceAll);
        EthernetBuffer=StringReplace(EthernetBuffer, "\r", "", TReplaceFlags()<<rfReplaceAll); //Steven 20250510 : 不可以有尾巴

        sClipID=EthernetBuffer;
        Log.sprintf("Recv, , %s", EthernetBuffer);
        AddCCDCommunicationLog(Log);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::BtConnectClick(TObject *Sender)
{
    cSocketClipID->Address=edAddr->Text;
    cSocketClipID->Port=atoi(edPort->Text.c_str());
    TimerCCDConnect->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::BtDisconnectClick(TObject *Sender)
{
    cSocketClipID->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::BtTriggerClick(TObject *Sender)
{
    SendCCDCommand(" ", edCmd->Text);
}
//---------------------------------------------------------------------------
void TMyCCDIDPanel::AddCCDCommunicationLog(AnsiString Str)
{
    if(meLog==NULL || InitialOK==false)
        return;

    AnsiString Log, Path;
    GetTimeInfo();
    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d.%03d, %s, %s, %s, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, sName, edAddr->Text, edPort->Text, Str);
    if(meLog->Lines->Count>1000)
        meLog->Clear();
    meLog->Lines->Add(Log);

    Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeCommLogPath, SystemYear, SystemMonth, SystemDate);
    if(!DirectoryExists(Path))
        ForceDirectories(Path);

    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d_%s.txt", asBarCodeCommLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour, sName);
    WriteDataToFile(Path.c_str() , Log.c_str());
}
//---------------------------------------------------------------------------
void TMyCCDIDPanel::SendCCDCommand(AnsiString Str1, AnsiString Str2)
{
    if(meLog==NULL || InitialOK==false)
        return;

    AnsiString Log;
    if(Str2=="")
        Log.sprintf("Action, %s", Str1);
    else
        Log.sprintf("Send, %s, %s", Str1, Str2);

    if(Str2=="")  //JerryYang 20240802 : modify
    {

    }
    else
    {
        if(cSocketClipID->Active)
        {
            cSocketClipID->Socket->SendText(Str2+"\r\n");
        }
    }
    AddCCDCommunicationLog(Log);
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::TimerCCDConnectTimer(TObject *Sender)
{
    if(InitialOK==false || bTimerRun==true) //Steven 20160512 : 2D Timer加上保護
    {
        iConnectCount=95;
        return;
    }
    bTimerRun=true;
    AnsiString Str;

    if(cSocketClipID->Active                &&  //相同的IP跟Port就不要重新連線
       cSocketClipID->Address==edAddr->Text &&
       cSocketClipID->Port==edPort->Text)
    {
    }
    else
    {
        if(cSocketClipID->Active)
        {
            cSocketClipID->Close();
            SendCCDCommand("ClientSocket Close", "");
            iConnectCount=0;
        }
    }

    if(cSocketClipID->Active==false && bEnableCCD==true)
    {
        iConnectCount++;
        if(iConnectCount>100)
        {
            cSocketClipID->Address=edAddr->Text;
            cSocketClipID->Port=atoi(edPort->Text.c_str());
            SendCCDCommand("ClientSocket connecting..", "");
            try
            {
                cSocketClipID->Open();
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfBarCode::TimerDownCCDConnectTimer", iMyTag);
            }
            SendCCDCommand("ClientSocket Open", "");
            iConnectCount=0;
        }
    }

    bTimerRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::edPortClick(TObject *Sender)
{
     fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 99999);
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::edAddrClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NUM_PAD);
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::edErr2DClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::edMax2DClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 999, atoi(edMin2DLen->Text.c_str())+1);
}
//---------------------------------------------------------------------------
void __fastcall TMyCCDIDPanel::edMin2DClick(TObject *Sender)
{
    if(atoi(edMax2DLen->Text.c_str())<2)
        edMax2DLen->Text=10;
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, atoi(edMax2DLen->Text.c_str())-1);
}
//---------------------------------------------------------------------------
void Tfrm2DID::Update2DParameter()
{
    for(int i=0; i<eLoaderTotal; i++)
    {
        MyCCD[i]->bEnableCCD  =MyCCD[i]->cbEnable->Checked;
        if(MyCCD[i]->bEnableCCD)
            MyCCD[i]->btConnect->Click();
        else
            MyCCD[i]->btDisConnect->Click();
    }
}
//---------------------------------------------------------------------------
void Tfrm2DID::StartCCDAuto(int iArea)
{
    MyCCD[iArea]->FSM_PhotoInit();
    MyCCD[iArea]->m_FMSRunner.SetStepTable(CCDPhotoSteps);
    MyCCD[iArea]->m_FMSRunner.SetExecutor(CCDExecTrampoline, &MyCCD[iArea]);
    MyCCD[iArea]->m_FMSRunner.Start(STEP_PHOTO_INIT);
}
//---------------------------------------------------------------------------
bool Tfrm2DID::UpdateCCDAuto(int iArea)
{
    FSM_RESULT rc = MyCCD[iArea]->Update();
    if (rc == FSM_DONE)
    {
        // 這裡可以觸發下一個 FSM
        return true;
    }
    else if (rc == FSM_ERROR || rc == FSM_ABORT)
    {
        // 記錄錯誤 / 停機
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
AnsiString Tfrm2DID::GetBarcode(int iArea)
{
    return MyCCD[iArea]->sClipID;
}
//---------------------------------------------------------------------------
FSM_RESULT TMyCCDIDPanel::Update()
{
    if(bEnableCCD==false)
    {
        return FSM_DONE;
    }
    const FSM_STEP* step = m_FMSRunner.FindStep(m_FMSRunner.GetCurrentStep());
    if (!step)
    {
        return FSM_ABORT;
    }
    FSM_RESULT r = DoExecCCDStep(m_FMSRunner.GetCurrentStep());
    // ===== Step still running =====
    if (r == FSM_RUNNING)
    {
        return FSM_RUNNING;
    }
    // ===== Step done =====
    else if (r == FSM_DONE)
    {
        if (step->nextStep < 0)
        {
            return FSM_DONE;
        }
        m_FMSRunner.SetCurrentStep(step->nextStep);
        return FSM_RUNNING;
    }
    // ===== Error / Abort =====
    if (step->abortStep >= 0)
    {
        m_FMSRunner.SetCurrentStep(step->abortStep);
        return FSM_RUNNING;
    }
    return FSM_ABORT;
}
//---------------------------------------------------------------------------
FSM_RESULT TMyCCDIDPanel::FSM_PhotoInit()
{
    // Clear socket buffer to avoid stale data from previous scan
    if(cSocketClipID->Active && cSocketClipID->Socket->Connected)
    {
        try
        {
            while(cSocketClipID->Socket->ReceiveLength() > 0)
                cSocketClipID->Socket->ReceiveText();
        }
        catch(...){}
    }
    sClipID="";
    bDoWaitPhoto=false;
    return FSM_DONE;
}
//---------------------------------------------------------------------------
FSM_RESULT TMyCCDIDPanel::FSM_LightOn()
{
    if (bEnableCCD)
        SendCCDCommand("", "LON");
    return FSM_DONE;
}
//---------------------------------------------------------------------------
FSM_RESULT TMyCCDIDPanel::FSM_MoveToMin()
{
    if (bCanMove==false)
        return FSM_DONE;
    if (sClipID != "")
        return FSM_DONE;
    if (CCDArmX->MotorMove(iMin))
        return FSM_DONE;
    return FSM_RUNNING;
}
//---------------------------------------------------------------------------
FSM_RESULT TMyCCDIDPanel::FSM_MoveToMax()
{
    if (bCanMove==false)
        return FSM_DONE;
    if (sClipID != "")
        return FSM_DONE;
    if (CCDArmX->MotorMove(iMax))
        return FSM_DONE;

    return FSM_RUNNING;
}
//---------------------------------------------------------------------------
FSM_RESULT TMyCCDIDPanel::FSM_WaitPhoto()
{
    if(bDoWaitPhoto==false)
    {
        bDoWaitPhoto=true;
        WaitPhotoIdle.SetSecAndOn(10);
    }
    else if(bDoWaitPhoto==true &&
            WaitPhotoIdle.Off())
    {
        return FSM_DONE;
    }
    if (sClipID != "")
        return FSM_DONE;
    return FSM_RUNNING;
}
//---------------------------------------------------------------------------
FSM_RESULT TMyCCDIDPanel::FSM_PhotoFinish()
{
    SendCCDCommand("", "LOFF");
    return FSM_DONE;
}
//---------------------------------------------------------------------------

