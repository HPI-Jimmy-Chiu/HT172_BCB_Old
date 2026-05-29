#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "auto172.h"

#include "myQwertyKeyBoard.h"
#include "automation.h"
#include "cMydef.h"
#include "cProd.h"
#include "common.h"
#include "main.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfAutomation *fAutomation;
//Steven 20100830 : For OLP
static bool bStandard=true; //Steven 20110208
bool bRestConnect=false;    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
//int iFTPCanUse=10;
AnsiString TCPstr="";
AnsiString sDLFileName="";
AnsiString ReceiveString="";
AnsiString TCPstr2="";
AnsiString ReceiveString2="";
AnsiString STX;
AnsiString SOH;
AnsiString ETX;
AnsiString stx = "[STX]";
AnsiString soh = "[SOH]";
AnsiString etx = "[ETX]";
AnsiString HandlerType;
AnsiString aDataPath;
bool WRITE=false;
bool READ=true;
//Steven 20100830 End

//ChungHung 20101119 Start
const int iMaxEvent = 10;
struct sResult
{
    bool def;
    AnsiString CommandString;
    AnsiString NumString;
    sResult()
    {
        def=false;
        CommandString="";
        NumString="";
    };
}MyEventResult[iMaxEvent];
//---------------------------------------------------------------------------
__fastcall TfAutomation::TfAutomation(TComponent* Owner)
    : TForm(Owner)
{
    try
    {
        brecordmsgLock=false;
        STX.sprintf("%c",0x02);
        SOH.sprintf("%c",0x01);
        ETX.sprintf("%c",0x03);
        OLPClientConnected=false;
        if(CUSTOMER_CODE==CC_ASE_CL)
        {
            bStandard=true;     //Steven 20110208
            OLPServer->Open();
            gbAutomation->Visible=true;
        }
        else if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)                 //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            OLPServer->Port=6671;                                               //Sam 20210812 : PTI 強制寫死6671
            #ifdef SOFT_SIMULATE
            OLPServer->Port=5001;
            #endif
            bStandard=false;
            OLPServer->Open();
            gbAutomation->Visible=true;
        }
        else
        {
            bStandard=true;
            OLPServer->Close();
            gbAutomation->Visible=false;
        }

        if(CUSTOMER_CODE==CC_PTI)                                               //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            OLPClient->Address=edinputIP->Text;
            OLPClient->Port=atoi(edinputport->Text.c_str());
            #ifdef SOFT_SIMULATE
            OLPClient->Address="127.0.0.1";
            OLPClient->Port=5002;
            #endif
            OLPClient->Open();
        }
        else
        {
            OLPClient->Close();
        }

        if(CUSTOMER_CODE==CC_PTI)      //Steven 20141229 : OLP功能
            tmrOLP->Enabled=true;
        else
            tmrOLP->Enabled=false;

        //Steven 20100830 Start: For OLP
        CommandBuffer=new TList(); // 2009.11.13 , Joye
        CommandBuffer->Clear();
        CommandClientReplyBuffer=new TStringList(); //Sam 20200813 : 增加 Log debug
        CommandClientReplyBuffer->Clear();

        GetTimeInfo();
        HandlerType=GetHandlerType();
        //aDataPath=GetDataPath();

        bReceive=false;
        bReceive2=false;
        iSocketHandle=0;    //Sam 20200727 : 修正連線問題

        bReceiveHasData = false;    // 2011.09.05 , Joye , OLP
        bOneCycle=false;
        bCleanOut=false;

        TestMode=0;
        for(int i=0; i<256; i++)
        {
            sATKSendMessage[i]='\0';
            sATKSendTemp[i]='\0';
            sATKSendMap[i]='\0';
            sATKSendTempAndMap[i]='\0';
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAutomation::TfAutomation");
    }
    iClientReConnect=0;         //Sam 20240226 : Client 重連次數
}
//---------------------------------------------------------------------------
bool TfAutomation::GetEventNum(int &num,AnsiString cmdstr,AnsiString numstr)
{
    for(int i=0; i<iMaxEvent; i++)
    {
        if(MyEventResult[i].CommandString=="")
        {
            num=i;
            SetEventResult(i,false,cmdstr,numstr);
            return true;
        }
    }
    num=-1;
    return false;
}
//---------------------------------------------------------------------------
bool TfAutomation::SetEventResult(int num,bool def,AnsiString cmdstr,AnsiString numstr)
{
    if(num>=0 && num<iMaxEvent)
    {
        MyEventResult[num].def=def;
        MyEventResult[num].CommandString=cmdstr;
        MyEventResult[num].NumString=numstr;

        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfAutomation::GetEventResult(int num)
{
    bool result=false;
    if(num>=0 && num<iMaxEvent)
    {
        result=MyEventResult[num].def;
        //str=MyEventResult[num].GetString;
        return result;
    }
    return result;
}
//---------------------------------------------------------------------------
void TfAutomation::CompareEvent(AnsiString str)
{
    for(int i=0; i<iMaxEvent; i++)
    {
        if(MyEventResult[i].CommandString!="")
        {
            if(str.Pos(MyEventResult[i].CommandString)!=0 &&
               str.Pos(MyEventResult[i].NumString)!=0 || MyEventResult[i].NumString=="")
            {
                SetEventResult(i, true, "", "");
                return;
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfAutomation::ClearEvent(int num)
{
    SetEventResult(num, false, "", "");
    return;
}
//ChungHung 20101119 End
//---------------------------------------------------------------------------
AnsiString SplitDataBySoh(AnsiString &Buffer)
{
    int pos;
    AnsiString Data;
    pos=Buffer.Pos(SOH);
    if(pos==0)
        pos=Buffer.Pos(ETX);

    Data=Buffer.SubString(1, pos-1);
    Buffer.Delete(1, pos);
    return Data;
}
//---------------------------------------------------------------------------
AnsiString GetCRC(AnsiString &Buffer)
{
    AnsiString Data;
    Data=Buffer.SubString(1, 5);
    Buffer.Delete(1, 1);
    return Data;
}
//---------------------------------------------------------------------------
AnsiString __fastcall AddBlock(AnsiString S)
{
    AnsiString S1;
    S1="["+S+"]";
    return S1;
}
//---------------------------------------------------------------------------
bool IsStartWord(AnsiString &Buffer, AnsiString w)
{
    if(Buffer.Pos(w)==1)
    {
        Buffer.Delete(1, 1);
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
AnsiString crc16(char *ip_str, int n)
{
    unsigned  crctmp=65535, ch;
    char str[256];
    AnsiString S;
    for(int i=0; i<n; i++)
    {
        ch=ip_str[i];
        crctmp=crctmp^ch;
        for(int j=0; j<8; j++)
        {
            if((crctmp & 1)==1)
            {
                crctmp=crctmp/2;
                crctmp=crctmp^40961;
            }
            else
            {
                crctmp=crctmp/2;
            }
        }
    }
    sprintf(str, "%5d", crctmp);
    S=str;
    return S;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfAutomation::GetTimeInfo()
{
    static TDateTime dtPresent;
    AnsiString TimeString;
    dtPresent= Now();
    DecodeDate(dtPresent, aSystemYear, aSystemMonth, aSystemDate);
    DecodeTime(dtPresent, aSystemHour, aSystemMin, aSystemSec, aSystemMSec);
    TimeString.sprintf("%04d%02d%02d%02d%02d%02d", aSystemYear, aSystemMonth, aSystemDate, aSystemHour, aSystemMin, aSystemSec);
    return TimeString;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPServerClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString sLog="";
    int iConnectCount=(int)(OLPServer->Socket->ActiveConnections);              //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server

    if(CUSTOMER_CODE==CC_PTI)//Sam 20200826 : PTI 允許多重連線
    {
        OnLine->Enabled=true;
        ShowOLPState(1);   // 2009.12.02 , Joye
        if(iConnectCount>1)
        {
            sLog.sprintf("OLP server is about to reset due to too many connections. iConnectCount=%d", iConnectCount);
            RecordProcess(sLog);
        }
    }
    else
    {
        if(iConnectCount==1)
        {
            OnLine->Enabled=true;
            ShowOLPState(1);   // 2009.12.02 , Joye
        }
        else
        {
            sLog.sprintf("OLP server is about to reset due to too many connections. iConnectCount=%d", iConnectCount);
            RecordProcess(sLog);
            bRestConnect=true;
            ShowOLPState(0);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPServerClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    OnLine->Enabled=false;
    ShowOLPState(0);   // 2009.12.02 , Joye
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPServerClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ShowMSG("OLPServerClient Connect fail : "+GetErrorMsg(Sender, ErrorCode));
    if(ErrorCode==10053)   // 網路線拔除
    {
        ShowOLPState(2);   // 2009.12.02 , Joye
    }
    else if(ErrorCode==10061) //連線失敗
    {
        ShowOLPState(0);   // 2009.12.02 , Joye
    }
    else
    {
        ShowOLPState(0);
    }
    ErrorCode=0;
    OnLine->Enabled=false;
    OLPServer->Close();
    OLPServer->Open();
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPServerClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int StartPos=0,EndPos=0;
    if (ReceiveString.Length() > 1024)                                         //jimmychiu 20251009 : avoid msg too long to dead
    {
        ShowMSG(AnsiString().sprintf("OLPServerClientRead too long to delete, %s",ReceiveString));
        ReceiveString="";
    }
    //接收字串
    ReceiveString+=Socket->ReceiveText();   //Sam 20200727 : 修正連線問題
    iSocketHandle=Socket->SocketHandle ;
    //尋找STX
    StartPos=ReceiveString.Pos(STX);
    //尋找ETX
    EndPos=ReceiveString.Pos(ETX);

    if(bReceive==false && StartPos>0 && EndPos>0 && StartPos<EndPos)
    {
        TCPstr=ReceiveString.SubString(StartPos,EndPos);
        ReceiveString.Delete(1,EndPos);
        bReceive=true;
    }
    else
    {
        ReceiveString="";
    }

    if(bReceive)
    {
        ProcessBuffer(TCPstr,iSocketHandle); //Sam 20200813 : 增加 Log debug //被動回傳
        bReceive=false;

        // 2011.09.05 , Joye , OLP ------------------------
        if( ReceiveString.Length() > 0)
        {
            bReceiveHasData = true;
        }
        else
            bReceiveHasData = false;
        // ------------------------------------------------
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::CommandProcess(AnsiString CMD, AnsiString V_TOTAL, AnsiString *Data, bool Standard,int iHandle) //Sam 20200813 : 增加 Log debug
{
    AnsiString HEAD, head, SendString, S2, cmd, v_total;

    cmd=AddBlock(CMD);
    v_total=AddBlock(V_TOTAL);

    HEAD=STX+CMD+SOH+V_TOTAL;
    head=stx+cmd+soh+v_total;

    for(int i=0; i<atoi(V_TOTAL.c_str()); i++)
    {
        HEAD=HEAD+SOH+Data[i];
        head=head+soh+AddBlock(Data[i]);
    }
    HEAD+=ETX;
    head+=etx;

    SendString=HEAD;
    S2=head;
    if(Standard==true)
    {
        bReceive=false;
        SendClient(SendString,S2,iHandle);  //Sam 20200813 : 增加 Log debug
    }
    else
    {
        bReceive=false;
        SendServer(SendString,S2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::tmrOLPTimer(TObject *Sender) //主動回傳
{
    static bool bRun=false;
    if(bRun)
        return;
    bRun=true;
    //
    static int TimeCount=0;
    static int bfirst=true;
    AnsiString S="";
    int iConnectCount=0;
    static bool bRecordState=false;
    AnsiString sLog="";
    if(CUSTOMER_CODE!=CC_PTI || InitialOK==false)
    {
        bRun=false;
        return;
    }

    if(bfirst)
    {
        fAutomation->edinputIP->Text    =HSys.FuncN.sN03_OLP_IP;
        if(HSys.FuncN.sN03_OLP_IP=="")
            HSys.FuncN.sN03_OLP_IP="6670";
        fAutomation->edinputport->Text  =HSys.FuncN.sN03_OLP_Port;
        bfirst=false;
    }
    if(SendInvalid==false)
    {
        SendInvalid=true;
        TimeCount=0;
    }
    if(SendInvalid==true && TimeCount>=10000 && OnLine2->Enabled==true)
    {
        SendOLPClient(" "+ETX);
        SendInvalid=false;
        TimeCount=0;
    }
    else if(OnLine2->Enabled==false)
    {
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)  //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            if(iClientReConnect<10) //Sam 20240226 : Client 重連次數
            {
                iClientReConnect++;
                OLPClient->Address=edinputIP->Text;
                OLPClient->Port=atoi(edinputport->Text.c_str());
                #ifdef SOFT_SIMULATE
                OLPClient->Address="127.0.0.1";
                OLPClient->Port=5002;
                #endif
                OLPClient->Open();
            }
        }
    }
    else
    {
        TimeCount++;
    }
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
    //==>
    try
    {
        if(bRestConnect==true && bReceive==false)  //Sam 20200727 : 修正連線問題
        {
            OLPServer->Close();
            OLPServer->Open();
            OnLine->Enabled=false;
            bRestConnect=false;
            bRecordState=false;
            RecordProcess("OLP server reset !!!");
            bRun=false;
            return;
        }
        else if(bRestConnect==true && bReceive==true && bRecordState==false)//Sam 20200813 : 增加 Log debug
        {
            bRecordState=true;
            RecordProcess("OLP server will reset connect,But client is receiving data!!!");
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAutomation::tmrOLPTimer");
    }
    //<==
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server

    if(CheckInitialOK()==false)
    {
        bRun=false;
        return;
    }
    AnsiString Data[32], TimeString="";     //Sam 20191126 : 20>32
    AnsiString MainStatus=GetMainStatus();
    MyProcessBuffer *ProcessPtr;
    static int Task=1;
    static MyProcessBuffer Process;
    static AnsiString SysStatus="";
    int DutCount;

    if(SysStatus=="")
        SysStatus=MainStatus;

    if(MainStatus!=SysStatus)
    {
        SendReportRequest("0001", bStandard);
        SysStatus=MainStatus;
    }

    switch(Task)
    {
        case 1:
            if(CommandBuffer->Count!=0)
            {
                if((bStandard==true && OnLine->Enabled==true) || (bStandard==false && OnLine2->Enabled==true))
                {
                    ProcessPtr=(MyProcessBuffer *)CommandBuffer->Items[0];
                    Process.Command     =ProcessPtr->Command;
                    Process.SubCommand  =ProcessPtr->SubCommand;
                    Process.AlarmID     =ProcessPtr->AlarmID;
                    Process.AlarmMessage=ProcessPtr->AlarmMessage;
                    Process.TimeString  =ProcessPtr->TimeString;
                    Process.ErrType     =ProcessPtr->ErrType;
                    CommandBuffer->Delete(0);
                    Task=100;
                }
                else
                {
                    WriteTCPDataToTextFile();                    //2008/08/05 lee
                }
            }
            break;
        case 100:
            if(Process.Command=="ALARM_REQUEST")
            {
                Data[0]=GetTimeInfo();
                Data[1]=Process.AlarmID;
                Data[2]="1";        //Steven 20110802 : Count固定為1
                Data[3]=Process.AlarmMessage;
                CommandProcess(Process.Command, 4, Data, bStandard);
            }
            else if(Process.Command=="MESSAGE_REQUEST")
            {
                Data[0]=GetTimeInfo();
                Data[1]=Process.AlarmMessage;
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if(Process.Command=="PRODUCTION_REQUEST")
            {
                Data[0]=GetTimeInfo();
                Data[1]=Process.AlarmMessage;
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if(Process.Command=="PRODUCTION_REPLY")
            {
                Data[0]=Process.AlarmMessage;     //Brian,20181130
                Data[1]="0";                      //Brian,20181130
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if(Process.Command=="TEST_RESULT_REQUEST")
            {
                //DutCount=GetTestResult(Data);
                CommandProcess("TEST_RESULT_REQUEST", DutCount, Data, bStandard);
            }
            Task=1;
            break;
    }

    // 2011.09.05 , Joye , OLP ------------------------>>
    if(bReceiveHasData==true)
    {
        //尋找STX
        int StartPos=ReceiveString.Pos(STX);
        //尋找ETX
        int EndPos=ReceiveString.Pos(ETX);

        if(StartPos!=0 && EndPos!=0 && StartPos<EndPos)
        {
            AnsiString sSubTCPstr = ReceiveString.SubString(StartPos, EndPos);
            ReceiveString.Delete(1,EndPos);

            ProcessBuffer(sSubTCPstr,iSocketHandle); //Sam 20200813 : 增加 Log debug //被動回傳

            if(ReceiveString.Length()>0)
            {
                bReceiveHasData=true;
            }
            else
            {
                bReceiveHasData=false;
            }
        }
    }
    // 2011.09.05 , Joye , OLP ------------------------<<

    if(CommandClientReplyBuffer->Count>=1)                                      //Sam 20200813 : send client data error add retry send
    {
        iConnectCount=(int)(OLPServer->Socket->ActiveConnections);
        if(OnLine->Enabled==true && iConnectCount>=1)
        {
            if(iConnectCount>1)
            {
                sLog.sprintf("OLP connected warnning!!! iConnectCount=%d", iConnectCount);
                RecordProcess(sLog);
            }
            sLog.sprintf("OLP retry reply!!! Send socket handle=%d", OLPServer->Socket->Connections[0]->Handle);
            RecordProcess(sLog);
            S=CommandClientReplyBuffer->Strings[0];
            OLPServer->Socket->Connections[0]->SendBuf(S.c_str(), S.Length());
            ShowCharHex(S);
            CommandClientReplyBuffer->Delete(0);
        }
    }
    bRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btnConnectClick(TObject *Sender)
{
    if(edinputIP->Text!=0)
        OLPClient->Address=edinputIP->Text;
    if(edinputport->Text!=0)
        OLPClient->Port=edinputport->Text.ToInt();

    try
    {
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)  //Sam 20240226 : Client 重連次數
        {
            iClientReConnect=0;
        }
        btnConnect->Enabled=false;
        OLPServer->Open();

    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAutomation::btnConnectClick");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btnDisconnectClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
    {
        iClientReConnect=0;
    }
    OLPServer->Close();
    OnLine->Enabled=false;

    ShowOLPState(0);   // 2009.12.02 , Joye
    btnConnect->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btEventReportClick(TObject *Sender)
{
    AnsiString S;
    S=GetMainStatus().UpperCase();
    if(S=="PAUSE" || S=="HALT")
    {
    }
    else
    {
        S="RUN";
    }
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)//Sam 20190429 : Add CC_PTI_NEWWORK
    {
        SendReportRequest(cbbOLPCommand->Text,0);
    }
    else
    {
        SendReportRequest(cbbOLPCommand->Text);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btAlarmReportClick(TObject *Sender)
{
    DoCommandBuffer("ALARM_REQUEST", "", "InArm:Device pick-up error on Hot Plate", 1, "JAM0109");
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::btTestResultClick(TObject *Sender)
{
    DoCommandBuffer("TEST_RESULT_REQUEST");
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::MemoDblClick(TObject *Sender)
{
    SaveRecord();
    Memo->Clear();
    MemoChar->Clear();
    MemoCode->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::ShowCharHex(AnsiString S)
{
    if(chkViewComm->Checked==false)
        return;

    AnsiString S1="", S2="";
    char s[12];
    char Target[10240];
    int len, i;
    strncpy(Target, S.c_str(), sizeof(Target));
    len=strlen(Target);
    for(i=0; i<len; i++)
    {
        if(Target[i]==0x01 || Target[i]==0x02 || Target[i]==0x03)
        {
            if(Target[i]==0x02)
                s[0]='<';
            if(Target[i]==0x01)
                s[0]='=';
            if(Target[i]==0x03)
                s[0]='>';

            s[1]=' ';
            s[2]=0;
        }
        else
        {
            s[0]=Target[i];
            s[1]=' ';
            s[2]=0;
        }
        S1+=s;

        sprintf(s, "%02X ", Target[i]);
        S2+=s;
    }
    MemoChar->Lines->Add(S1);
    MemoChar->Lines->Add("");
    MemoCode->Lines->Add(S2);
    MemoCode->Lines->Add("");

    if(MemoChar->Lines->Count>1000)
    {
        MemoChar->Clear();
    }

    if(MemoCode->Lines->Count>1000)
    {
        MemoCode->Clear();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::ShowRecord(bool bRead , AnsiString S,int iHandle) //Sam 20200813 : 增加 Log debug
{
    if(HSys.FuncN.bN03_OLP_Log && chkViewComm->Checked==false)
        return;

    AnsiString S2;
    char str[256];
    if(bRead)
        sprintf(str, "%s [Receive] [%06d]:", GetTimeInfo(),iHandle);            //Sam 20200813 : 增加 Log debug
    else
        sprintf(str, "%s [Send   ] [%06d]:", GetTimeInfo(),iHandle);            //Sam 20200813 : 增加 Log debug
    S2=AnsiString(str)+S;
    ShowMSG(S2);
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SaveRecord()  //Steven 20141229 : OLP的Log要存檔
{
    AnsiString FileName;

    if(CUSTOMER_CODE==CC_PTI)   //Sam 20200812 : PTI 暫時強制打開
    {
        HSys.FuncN.bN03_OLP_Log=true;
    }

    if(HSys.FuncN.bN03_OLP_Log)
    {
        FileName.sprintf("%s\\%04d_%02d", asOLPLogPath, SystemYear, SystemMonth);
        MyForceDirectories(FileName);
        FileName.sprintf("%s\\%04d_%02d\\%04d-%02d-%02d %02d-%02d-%02d.log", asOLPLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        Memo->Lines->SaveToFile(FileName);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SendClient(AnsiString S, AnsiString S2,int iHandle)
{
    int i;
    int iConnectCount=(int)(OLPServer->Socket->ActiveConnections);
    int iConnect=-1;

    AnsiString sLog="";
    if(OnLine->Enabled==true && iConnectCount>=1) //Sam 20200727 : 如果連接數大於1以上，以  SocketHandle 來確保回傳
    {
        iConnect=-1;
        for(i=0; i<iConnectCount; i++)    //找尋對應 iConnect Client Handle
        {
            if(iHandle!=iSocketHandle)
            {
                //iHandle != iSocketHandle 時做紀錄 Debug 用
                //NewRecordProcess("","OLP iHandle not match iSocketHandle","iHandle="+IntToStr(iHandle)+",iSocketHandle="+IntToStr(iSocketHandle));
                sLog.sprintf("OLP iHandle not match iSocketHandle. iHandle=%d iSocketHandle=%d", iHandle, iSocketHandle);
                RecordProcess(sLog);
            }

            //基本上 iHandle &  iSocketHandle 是一樣的東西 Debug 中先這樣弄
            if(OLPServer->Socket->Connections[i]->SocketHandle==iHandle)            //從資料流丟過來的 Handle
            {
                iConnect=i;
                break;
            }
            else if(OLPServer->Socket->Connections[i]->SocketHandle==iSocketHandle) //接收到 Client 時紀錄 Handle
            {
                iConnect=i;
                break;
            }
        }

        if(iConnect==-1)
        {
            //如果真的找不到對應 Handle 就丟 iConnect=0
            sLog.sprintf("OLP Not Find SocketHandle. iConnectCount=%d", iConnectCount);
            RecordProcess(sLog);
            iConnect=0;
        }

        if(iConnect!=-1)
        {
            OLPServer->Socket->Connections[iConnect]->SendBuf(S.c_str(), S.Length());
            ShowCharHex(S);
            ShowRecord(WRITE, S2,OLPServer->Socket->Connections[iConnect]->SocketHandle);
        }
        else
        {
            if(bStandard==false)
                ShowMyMessage("Not Find SocketHandle.");
        }
    }
    else
    {
        ShowRecord(WRITE, S2,iHandle);
        CommandClientReplyBuffer->Add(S);
        sLog.sprintf("OLP Wait client connect, iHandle=%d, iSocketHandle=%d, iConnectCount=%d", iHandle, iSocketHandle, iConnectCount);
        RecordProcess(sLog);  //Sam 20200826 : Add Log
        if(bStandard==false)
            ShowMyMessage("Wait client connect.");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SendServer(AnsiString S, AnsiString S2)
{
    int iHandle=0;
    if(OnLine2->Enabled==true)//是否連線中
    {
        iHandle=(int)OLPClient->Socket->Handle;
        OLPClient->Socket->SendBuf(S.c_str(), S.Length());
        ShowCharHex(S);
        ShowRecord(WRITE, S2, iHandle);      //Sam 20200813 : 增加 Log debug
    }
    else
    {
        if(HSys.FuncN.bN03_OLP_Log==false && chkViewComm->Checked==false)    //Sam 20240226 : Client 重連次數
            return;

        char str[256];
        sprintf(str, "%s [Send Error] [%06d]:", GetTimeInfo(),0);    //Sam 20200813 : 增加 Log debug
        S2=AnsiString(str)+S2;
        ShowMSG(S2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::WriteTCPDataToTextFile()    //把離線的相關資料寫到Log檔
{
    AnsiString str="", datestr="";
    FILE *file;
    MyProcessBuffer *ProcessPtr;
    MyProcessBuffer *Process=new MyProcessBuffer();
    GetTimeInfo();

    ProcessPtr=(MyProcessBuffer *)CommandBuffer->Items[0];
    if(CommandBuffer->Count>0)
    {
        ProcessPtr=(MyProcessBuffer *)CommandBuffer->Items[0];
        Process->Command=ProcessPtr->Command;
        if(Process->Command=="")
            Process->Command="NULL";

        Process->SubCommand=ProcessPtr->SubCommand;
        if(Process->SubCommand=="")
            Process->SubCommand="NULL";

        Process->AlarmID=ProcessPtr->AlarmID;
        if(Process->AlarmID=="")
            Process->AlarmID="NULL";

        Process->AlarmMessage=ProcessPtr->AlarmMessage;
        if(Process->AlarmMessage=="")
            Process->AlarmMessage="NULL";

        Process->TimeString=ProcessPtr->TimeString;
        if(Process->TimeString=="")
            Process->TimeString="NULL";

        Process->ErrType=ProcessPtr->ErrType;
        str.sprintf("%s#%s#%s#%s#%s#%d\n", Process->Command.c_str(), Process->SubCommand.c_str(), Process->AlarmID.c_str(), Process->AlarmMessage.c_str(), Process->TimeString.c_str(), Process->ErrType);

        datestr.sprintf("d:\\%s_Log\\TCP_Data\\", HandlerType);
        MyForceDirectories(datestr);
        GetTimeInfo();                                                          //Steven 20101105
        datestr.sprintf("d:\\%s_Log\\TCP_Data\\%04d%02d%02d.tcp", HandlerType, aSystemYear, aSystemMonth, aSystemDate);
        file=fopen(datestr.c_str(), "a+");
        if(file!=NULL)
        {
            fputs(str.c_str(), file);
            fclose(file);
        }
        CommandBuffer->Clear();
    }
    delete Process;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::DoCommandBuffer(AnsiString Command, AnsiString SubCommand, AnsiString AlarmMessage, int ErrType, AnsiString AlarmID)
{
    P1=new MyProcessBuffer();      //Steven 20120102 : 改為全域變數
    P1->Command=Command;
    P1->SubCommand=SubCommand;
    P1->AlarmMessage=AlarmMessage;
    P1->ErrType=ErrType;
    P1->AlarmID=AlarmID;
    P1->TimeString=GetTimeInfo();
    CommandBuffer->Add(P1);
}
//---------------------------------------------------------------------------
extern bool SoftStart;
void __fastcall TfAutomation::ProcessBuffer(AnsiString Buffer, int iHandle)  //Sam 20200813 : 增加 Log debug
{
    bool bClient=true;

    if(Buffer==NULL)    //如果Buffer是Null的話，就要跳開，不然會出現異常。
        return;

    AnsiString Command, V_Total, ShowString;
    AnsiString Data[AUTOMATION_MAX_DATA];        //Sam 20200415 : fix 32 Site
    int v_total;

    ShowCharHex(Buffer);

    if(IsStartWord(Buffer, STX))
    {
        ShowString=stx;
        Command=SplitDataBySoh(Buffer);
        ShowString=ShowString+AddBlock(Command);
    }
    else
    {
        Command="";
        return;
    }

    V_Total=SplitDataBySoh(Buffer);
    ShowString=ShowString+soh+AddBlock(V_Total);
    v_total=atoi(V_Total.c_str());

    for(int i=0; i<v_total; i++)
    {
        Data[i]=SplitDataBySoh(Buffer);
        ShowString=ShowString+soh+AddBlock(Data[i]);
    }
    ShowString=ShowString+etx;
    ShowRecord(READ, ShowString, iHandle);  //Sam 20200813 : 增加 Log debug

    if(Command.Pos("INQUIRE")!=0)
    {
        if(Command=="REPORT_INQUIRE")
        {
            Data[0]=0;
            CommandProcess("REPORT_GRANT", 1, Data, bClient, iHandle);  //Sam 20200813 : 增加 Log debug
            MySleep(100);           //Steven 20110902 : 動作太快可能只會做其中一個,所以要Delay一下
            SendReportRequest(Data[2]);

        }
        else
        {
            TCPstr = "";
            int iInquire=0;
            AnsiString CMD=Command;
            CMD.Delete(CMD.Length()-6, 7);
            CMD+="GRANT";

            if(Command=="Version_INQUIRE")
            {
                Data[0]=GetSoftwareVersion();
                iInquire=2;
            }
            else if(Command=="SetUpFileName_INQUIRE")
            {
                Data[0]=GetSetUpFileName();
                iInquire=2;
            }
            else if(Command=="TesterInfo_INQUIRE")
            {
                iInquire=GetTesterInfo(Data)+1;
            }
            else if(Command=="LotTotal_INQUIRE")
            {
                iInquire=GetLotTotal(Data)+1;                                   //Sam 20250506 : 待補
            }
            else if(Command=="Category_INQUIRE")
            {
                iInquire=GetCategory(Data)+1;                                   //Sam 20250506 : 待補
            }
            else if(Command=="LotStartProgram_INQUIRE")
            {
                Data[0]=GetLotStartProgram();
                iInquire=2;
            }
            else if(Command=="LotEndProgram_INQUIRE")
            {
                Data[0]=GetLotEndProgram();
                iInquire=2;
            }
            else if(Command=="GetSortBin_INQUIRE")
            {
                AnsiString sLotID="", sTrayID="", sTrayX="", sTrayY="";
                sLotID=Data[0].c_str();
                sTrayID=Data[1].c_str();
                sTrayX=Data[2].c_str();
                sTrayY=Data[3].c_str();
                Data[0]=GetSortBin(sLotID, sTrayID, sTrayX, sTrayY);            //Sam 20250506 : 待補
                iInquire=2;
            }
            else if(Command=="GetIP_INQUIRE")
            {
                iInquire=GetIP(Data)+1;
            }
            else if(Command=="GetPort_INQUIRE")
            {
                iInquire=GetPort(Data)+1;
            }
            else if(Command=="GetCusCode_INQUIRE")
            {
                iInquire=GetCusCode(Data)+1;
            }
            else if(Command=="JamCount_INQUIRE")
            {
                Data[0]=GetJamCount();
                iInquire=2;
            }
            else if(Command=="UPH_INQUIRE")
            {
                Data[0]=GetUPH();
                iInquire=2;
            }
            else if(Command=="BinSetMode_INQUIRE")
            {
                Data[0]=GetUPH();
                iInquire=2;
            }
            else if(Command=="LotTotalQuantity_INQUIRE")
            {
                iInquire=GetLotTotalQuantity(Data)+1;
            }
            if(iInquire!=1)
                Data[iInquire-1]="0";
            CommandProcess(CMD, iInquire, Data, bClient, iHandle);
        }
    }
    else if(Command=="ON_LINE_REQUEST")  // 2008/05/23
    {
        DoOnLineReply(Data);
        CommandProcess("ON_LINE_REPLY", 5, Data, bClient, iHandle);//for sback
    }
    else if(Command=="INITIATE_REQUEST")  //2008/05/23
    {
        SYSTEMTIME SysTime;
        SysTime.wYear   = atoi(Data[0].SubString(1,  4).c_str());
        SysTime.wMonth  = atoi(Data[0].SubString(5,  2).c_str());
        SysTime.wDay    = atoi(Data[0].SubString(7,  2).c_str());
        SysTime.wHour   = atoi(Data[0].SubString(9,  2).c_str());
        SysTime.wMinute = atoi(Data[0].SubString(11, 2).c_str());
        SysTime.wSecond = atoi(Data[0].SubString(13, 2).c_str());
        SysTime.wMilliseconds=0;
        SetLocalTime(&SysTime);

        Data[0]=0;

        if(CUSTOMER_CODE==CC_PTI)   //2016.03.24 , Brian   Add CC_PTI_NEWWORK
        {
            //fMain->sbHomeClick(fMain);
        }
        CommandProcess("INITIATE_REPLY", 1, Data, bClient, iHandle);
        //Sam 20190429 : Add CC_PTI_NEWWORK
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
        {
            OLPServer->Close();
            OnLine->Enabled=false;

            ShowOLPState(0);
            btnConnect->Enabled=true;

            if(edinputIP->Text!=0)
                OLPClient->Address=edinputIP->Text;
            if(edinputport->Text != 0)
                OLPClient->Port=edinputport->Text.ToInt();
            try
            {
                btnConnect->Enabled=false;
                OLPServer->Open();
                OLPServer->Active=true;
                ShowOLPState(1);//Hsiong 2015.04.16 Add CC_MTI_NEWWORK
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfAutomation::ProcessBuffer");
            }
        }
    }
    else if(Command=="Pause_REQUEST")
    {
        MyDBIProcess("Message","Automation Command Pause succeed!!");
        fMain->sbPauseClick(fMain);

        Data[0]=0;
        CommandProcess("Pause_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="Start_REQUEST")
    {
        if(fMain->palMainStatus->Caption=="HALT" && HSys.Sys.SystemStart==false) //Sam 20240125 : 只能在閒置時啟動
        {
            MyDBIProcess("Message","Automation Command Start succeed!!");
            fMain->sbStartClick(fMain);

            if(bStartCallLot_StartExe)
            {
                bStartCallLot_StartExe=false;
                CallLotProgram("Start");
            }
        }
        else
        {
            MyDBIProcess("Message","Automation Command Start fail!!");
        }
        Data[0]=0;
        CommandProcess("Start_REPLY",1,Data, bClient, iHandle);
    }
    else if(Command=="ClearCounter_REQUEST")
    {
        DoClearReportRequest();
        Data[0]=0;
        CommandProcess("Clear_Report_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="Category_REQUEST")
    {
        RecordProcess("Category_REQUEST");                                      //Sam 20230803 : 客戶設定 Bin 要特別紀錄
        RecordProcess(ShowString);                                              //Sam 20230803 : 客戶設定 Bin 要特別紀錄
        Data[0]=SetCategory(Data);
        CommandProcess("Category_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="SetUpFileName_REQUEST")
    {
        Data[0]=SetSetUpFileName(Data);
        CommandProcess("SetupFileName_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="TesterInfo_REQUEST")
    {
        Data[0]=SetTesterInfo(Data);
        CommandProcess("TesterInfo_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="LotStartProgram_REQUEST")
    {
         Data[0]=SetLotStartProgram(Data);
        CommandProcess("LotStartProgram_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="LotEndProgram_REQUEST")
    {
         Data[0]=SetLotEndProgram(Data);
        CommandProcess("LotEndProgram_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="CallLotStartProgram_REQUEST")
    {
        Data[0]=CallLotProgram("Start");
        CommandProcess("CallLotStartProgram_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="CallLotEndProgram_REQUEST")
    {
        Data[0]=CallLotProgram("End");
        CommandProcess("CallLotEndProgram_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="LoadTrayMap_REQUEST")
    {
        Data[0]=LoadTrayMap(Data);
        CommandProcess("LoadTrayMap_REPLY", 1, Data, bClient, iHandle);
    }
    else if(Command=="BinSetMode_REQUEST")
    {
        Data[0]=SetBinSetMode(Data);
        CommandProcess("BinSetMode_REPLY", 1, Data, bClient, iHandle);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::ProcessBuffer1()
{
    AnsiString Command, V_Total, Temp, ShowString, filename, filelength;
    AnsiString Data[4];
    int pos=0;

    if(IsStartWord(TCPstr, STX))
    {
        ShowCharHex(TCPstr);
        ShowString=stx;
        pos=TCPstr.Pos('\x01');//取得分隔符號位置
        Command=TCPstr.SubString(1, pos-1);//取得Command
        TCPstr.Delete(1, pos);//刪除以擷取字串
        ShowString=ShowString+AddBlock(Command)+soh;
    }
    else
    {
        Command="";
        return;
    }
    TCPstr="";
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SendReportRequest(AnsiString ReportID,bool Standard)
{
    int iTotal, i;
    AnsiString CMD="REPORT_REQUEST",SV_TOTAL;
    AnsiString V_TOTAL=2;
    AnsiString HEAD,head,SendString,S2,cmd,v_total,sv_total;//,ReportID;
    AnsiString Data[100];
    AnsiString R_TIME,r_time,S;

    R_TIME=GetTimeInfo();
    r_time=AddBlock(R_TIME);

    cmd=AddBlock(CMD);
    v_total=AddBlock(V_TOTAL);

    HEAD=STX+CMD+SOH+V_TOTAL+SOH+R_TIME+SOH+ReportID+SOH;
    head=stx+cmd+soh+v_total+soh+r_time+soh+AddBlock(ReportID)+soh;

    if(ReportID=="0001")
    {
        iTotal=1;
        S=GetMainStatus();
        Data[0]=S;
    }
    else if(ReportID=="0002")   //MTBF與MUBF
    {
//        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
//        {
//            iTotal=7;
//        }
//        else
//        {
//            iTotal=4;
//        }
//        GetProductivity(Data);
    }
    else if(ReportID=="0003")   //載入的IC數
    {
//        iTotal=1;
//        Data[0]=GetLoadCount();
    }
    else if(ReportID=="0004")   //每個Tray分幾顆IC
    {
//        iTotal=9;
//        GetSortingCount(Data);
    }
    else if(ReportID=="0005")
    {
//        iTotal=GetSocketCount(Data);
    }
    else if(ReportID=="0006")
    {
//        iTotal=GetHeadCount(Data);
    }
    else if(ReportID=="0007")
    {
//        iTotal=2;
//        Data[0]="100";
//        Data[1]="101";
    }

    SV_TOTAL=AnsiString(iTotal);
    sv_total=AddBlock(SV_TOTAL);
    HEAD=HEAD+SV_TOTAL;
    head=head+sv_total;

    for(i=0; i<iTotal; i++)
    {
        HEAD=HEAD+SOH+Data[i];
        head=head+soh+AddBlock(Data[i]);
    }

    SendString=HEAD+ETX;
    S2=head+etx;

    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        ShowOLPState(1);
    }

    if(Standard)
        SendClient(SendString, S2, iSocketHandle);                                        //2007_0522
    else
        SendServer(SendString, S2);
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    if (ReceiveString2.Length() > 1000)
    {
        ShowMSG(AnsiString().sprintf("OLPServerClientRead too long to delete, %s",ReceiveString2));
        ReceiveString2="";
    }
    //讀取訊息
    int StartPos=0,EndPos=0;
    //接收字串
    ReceiveString2+=OLPClient->Socket->ReceiveText();
    while(true)
    {
        //尋找STX
        StartPos=ReceiveString2.Pos(STX);
        //尋找ETX
        EndPos=ReceiveString2.Pos(ETX);

        if(bReceive2==false && StartPos>0 && EndPos>0  && StartPos<EndPos)
        {
            TCPstr2=ReceiveString2.SubString(StartPos,EndPos);
            ReceiveString2.Delete(1,EndPos);
            bReceive2=true;
        }
        else
        {
            ReceiveString2="";
        }
        if(bReceive2)
        {
            CompareEvent(TCPstr2);
            bReceive2=false;
        }
        else
        {
            break;
        }
    }

    SendInvalid=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    OnLine2->Enabled=true;
    iClientReConnect=0; //Sam 20240226 : Client 重連次數
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        ShowOLPState(1);
    }
    OLPClientConnected=true;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    OnLine2->Enabled=false;
    iClientReConnect=0; //Sam 20240226 : Client 重連次數
    OLPClientConnected=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ShowMSG("OLPClient Connect fail : "+GetErrorMsg(Sender, ErrorCode));
    if(ErrorCode==10053)   // 網路線拔除
    {
        if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            ShowOLPState(2);
        }
    }
    else if(ErrorCode==10061) //連線失敗
    {
    }
    else
    {
    }

    ErrorCode=0;
    OnLine2->Enabled=false;
    if(Socket!=NULL)
    {
        Socket->Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::OLPClientConnecting(TObject *Sender,
      TCustomWinSocket *Socket)
{
    OnLine2->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::FormDestroy(TObject *Sender)
{
    try
    {
        SaveRecord();  //Steven 20141229 : OLP的Log要存檔
        OLPClient->Close();
        OLPServer->Close();
        tmrOLP->Enabled=false;
        if(CommandBuffer->Count ==0)
            delete CommandBuffer;

        CommandClientReplyBuffer->Clear();
        delete CommandClientReplyBuffer;

    //    delete Pbuffer;   //Steven 20120102 : 改為全域變數
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAutomation::FormDestroy");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::ShowCharHexATK(bool bRead, char *Message, int Length)     //Steven 20120330 : ATK Site Map Monitorning
{
    AnsiString S1="", S2="";

    for(int i=0; i<Length; i++)
    {
        S2=AnsiString(IntToHex(Message[i], 1));
        if(S2.Length()==1)
        {
            S2="0"+S2;
        }
        S1+="<0x"+S2+">";
    }

    if(bRead)
    {
        S2.sprintf("%s [Receive] : %s", GetTimeInfo(), S1);
    }
    else
        S2.sprintf("%s [Send   ] : %s", GetTimeInfo(), S1);
    ShowMSG(S2);
}
//---------------------------------------------------------------------------
//ChungHung 20120413 add
bool charcmp(char* buf1,char* buf2,int size)
{
    for(int i=0; i<size; i++)
    {
        if(buf1[i] != buf2[i])
            return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::NMUDP1DataReceived(TComponent *Sender,
      int NumberBytes, AnsiString FromIP, int Port)
{
    if(NumberBytes<=0) //ChungHung 20120410 add NumberBytes有可能出現負值
        return;

    char *buff=new char[NumberBytes+1];
    int i;
    AnsiString ReadMessage;
    NMUDP1->ReadBuffer(buff, 4, i);
    buff[NumberBytes]='\0';
    ShowCharHexATK(READ, buff, NumberBytes);
    ReadMessage="FromIP:" + FromIP + ":" + "Port:" + Port + ":" +"Bytes:" + NumberBytes;
    ShowMSG(ReadMessage);
    //delete buff;  //與法錯誤會照成亂刪記憶體
    delete[] buff;  //ChungHung 20131231 alter
    buff=NULL;  //kevin 20180517
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::SckSendAlarmCode(AnsiString Text)  //ChungHung 20150518 add for SCK send Alarm Code
{
    SendOLPClient(Text);
    AnsiString S2;
    S2.sprintf("%s : %s", GetTimeInfo(), Text);
    ShowMSG(S2);
}
//---------------------------------------------------------------------------
int TfAutomation::GetEventStrResult(int num)    //Brian,20181025
{
    AnsiString str1,str2;
    if(num>=0 && num<iMaxEvent)
    {
        str1=MyEventResult[num].CommandString;
        str2=MyEventResult[num].NumString;
        if(str1=="PRODUCTION_REPLY" && str2=="0003")
        {
            return 1;
        }
        if(str1=="PRODUCTION_REQUEST" && str2=="0007")
        {
            return 2;
        }
        if(str1=="TEST_RESULT_REPLY" && str2=="")
        {
            return 3;
        }
    }
    return 4;
}
//---------------------------------------------------------------------------
void __fastcall TfAutomation::edinputportMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void TfAutomation::SendOLPClient(AnsiString Text)
{
    if(OLPClientConnected==true)
    {
        OLPClient->Socket->SendText(Text);
    }
}
//---------------------------------------------------------------------------
void TfAutomation::ShowMSG(AnsiString asStr)
{
    if(brecordmsgLock==false)
    {
        brecordmsgLock=true;
        if(Memo->Lines->Count>500)
        {
            SaveRecord();
            Memo->Lines->Clear();
        }
        Memo->Lines->Add(asStr);
        brecordmsgLock=false;
    }
    Memo->Lines->Add("");
}
//---------------------------------------------------------------------------