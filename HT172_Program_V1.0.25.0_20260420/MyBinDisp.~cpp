#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "MyBinDisp.h"
#include "TextProcess.h"
#define Bin_MAX_RETRY 2
#define Bin_MAX_WAIT  3
#define Bin_STX       2
#define Bin_ETX       3
#define Bin_CR        13
#define Bin_LF        10
//---------------------------------------------------------------------
#pragma link "SPComm"
#pragma resource "*.dfm"
TDataModule3 *DataModule3;
//---------------------------------------------------------------------
__fastcall TDataModule3::TDataModule3(TComponent* AOwner)
    : TDataModule(AOwner)
{
};
//------------------------------------------------------------------------------
// 建構子
//------------------------------------------------------------------------------
TMyBinDispCtrl::TMyBinDispCtrl()
{
    for(int i=0; i<Bin_MAX_NUM; i++)
    {
        bHasUnitArray[i]=false;
        bSetBin[i]=false;
        if(CosFunction.bUseBinAutoConvert)
        {
        }
        else
        {
            for(int j=0; j<TEST_MAX_BIN; j++)                                       //kevin 20140326
                iSetBin[i][j]=-1;
        }
        iSetColor[i]=1;
        Alias[i]="";
        bSliding[i]=true;
        iVersion[i]=0;
        iBinNow[i]=0;                                                           // 顯示器目前的Bin
        iColorNow[i]=1;                                                         // 顯示器目前的的Color
        bHasError[i]=false;                                                     // 顯示器是否有出錯
    }

    slBinDispLog=new TMyStringList("D:\\HT-172_Log\\BinDisplayLog",             //Sam 20240629 : Log 資料夾修改 //Steven 20210910 : Bin顯示器的log
                                   "BinDisplayLog",
                                   "Date, Time, Action, Message");
    iSetColor[0]=3;     //橘色
    iSetColor[1]=3;
    iSetColor[2]=3;

    bStopProcess=false;
    bStartSetColor=false;
    bStartSetBin=false;
    bHasUnit=false;
    BinDispRecv=false;
    ComPort=4;
    ComParity=None;

    iDelaySec=5;
    InitialOK=false;

    iTotalInstalledUnit=-1;
    iBinDispCtrlTask=1;
    bFirstInit=true;                                                            //Steven 20110407
    iRusStatus=0;
    iUsedBinNumber=0;
    iFuncNum=0;
    SwTFTBinRelay=&HSys.Sw.SwTFTBinRelay;
    bFirst=true;
    iBinTask=1;
    bTimerRun=false;
    bTFTWaitEcho=false;
    bTFTDirectPending=false;
    bTFTDirectWaiting=false;
    iTFTExpectedAddr=-1;
    iTFTExpectedFuncHigh=-1;
    iTFTExpectedFuncLow=-1;
    iTFTExpectedDataHigh=-1;
    iTFTExpectedDataLow=-1;
    iTFTDirectIndex=-1;
    iTFTDirectBin=0;
    iTFTDirectRetry=0;
    iTFTDirectMaxRetry=0;
    asTFTExpectedAction="";
    ZeroMemory(iErrCount, sizeof(iErrCount));
    ZeroMemory(iCount, sizeof(iCount));
};
//------------------------------------------------------------------------------
// 解構子
//------------------------------------------------------------------------------
TMyBinDispCtrl::~TMyBinDispCtrl()
{
    try
    {
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMyBinDispCtrl");
    }
}
//------------------------------------------------------------------------------
const unsigned char T_HEX2ASCII[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const unsigned char T_ASXII2HEX[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
unsigned char TMyBinDispCtrl::T_HEX2ASCII_Mac(unsigned char hex2ascii) {return(T_HEX2ASCII[(hex2ascii)&0x0f]);}
unsigned char TMyBinDispCtrl::T_ASXII2HEX_Mac(unsigned char ascii2hex)
{
    if(ascii2hex-'0'>22 || ascii2hex-'0'<0)
        return 0;
    return(T_ASXII2HEX[ascii2hex-'0']);
}
void  TMyBinDispCtrl::SetComParity(TParity Parity)  {ComParity=Parity;}                 // 設定顯示器群組是利用那一個Parity
bool  TMyBinDispCtrl::UnitHasInstall(int Index)     {return bHasUnitArray[Index];}      // 確認該顯示器使是否有啟用
void  TMyBinDispCtrl::CloseUnit(int Index)          {bHasUnitArray[Index]=false;}       // 關閉該顯示器
void  TMyBinDispCtrl::OpenUnit(int Index)           {bHasUnitArray[Index]=true;}        // 開啟該顯示器
void  TMyBinDispCtrl::SetDelayTime(int Sec)         {iDelaySec=Sec;}                    // 設定多久輪替一次
int   TMyBinDispCtrl::GetDelayTime()                {return iDelaySec;}                 // 設定多久輪替一次
int   TMyBinDispCtrl::GetTotalInstalledUnit()       {return iTotalInstalledUnit+1;}     // 取得總共安裝了幾個顯示器
int   TMyBinDispCtrl::GetColorNow(int Index)        {return iColorNow[Index];}          // 顯示器目前的顏色
int   TMyBinDispCtrl::GetBinNow(int Index)          {return iBinNow[Index];}            // 顯示器目前的字元
bool  TMyBinDispCtrl::GerErrNow(int Index)          {return bHasError[Index];}          // 顯示器是否有出錯
void  TMyBinDispCtrl::SerErrNow(int Index,bool bErr){bHasError[Index]=bErr;}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::GetRunStatus()                                       // 回傳目前在執行的動作
{
    AnsiString Message="";
    switch(iRusStatus)
    {
        case 0: Message="Initialing...";    break;
        case 1: Message="Get status...";    break;
        case 2: Message="Color Setting.";   break;
        case 3: Message="Bin Setting.";     break;
        case 4: Message="Display Error!!";  break;
        case 5: Message="Bin Running.";     break;
    }
    return Message;
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::SetComPort(AnsiString port)                                // 設定顯示器群組是利用那一個COM PORT
{
    ComPort=port;
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::ProcessStopStart(bool Value)
{
    bStopProcess=Value;
    if(bFirstInit==true)
    {
        iBinDispCtrlTask=1;
        bStartSetColor=true;                                                    //重設顏色
        bStartSetBin=true;                                                      //重設Bin
        bFirstInit=false;
    }
    else
    {
        if(bStopProcess==true)                                                  //Steven 20110216
            iBinDispCtrlTask=50;
    }
}
//---------------------------------------------------------------------------
unsigned char TMyBinDispCtrl::A_Create_LCR(unsigned char *Sptr, unsigned char length)
{
    unsigned char Btmp,Btmp1;
    Btmp1=0;
    do
    {
        Btmp =T_ASXII2HEX_Mac(*Sptr);                                           //CONV 0-9 A-F
        Sptr++;
        Btmp =(Btmp<<4)|T_ASXII2HEX_Mac(*Sptr);                                 //CONV 0-9 A-F
        Btmp1+=Btmp;                                                            //MATH LRC
        Sptr++;
        --length;
        if(length==0)                                                           //ChungHung 20121030 修正會造成無窮迴圈 執行緒會死掉
            break;
    }
    while(--length);                                                            //check EVEN
    return ((~Btmp1)+1);                                                        //LRC CODE
}
//------------------------------------------------------------------------------
// COM PORT 接收資料function
//------------------------------------------------------------------------------
void __fastcall TMyBinDispCtrl::CommBinReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    if(BufferLength>=1024)                                                      //Steven 20141110 : Avoid memory overwrite
        return;
    ZeroMemory(BinDispCom2Buffer, sizeof(BinDispCom2Buffer));
    strncpy(BinDispCom2Buffer, (char*)Buffer, BufferLength);
    BinDispCom2Buffer[BufferLength]='\x0';

    sReadBuffer.sprintf("%s", AnsiString(BinDispCom2Buffer));
    if(sReadBuffer=="")                                                         //kevin 20170710 (wei) add bin display
        return;

    AnsiString asHex=Chararr2Hexstring(BinDispCom2Buffer,BufferLength);
    LogBinDisplay("Recv", asHex, true);

    if(bTFTWaitEcho)
    {
        AnsiString asReason="";
        if(CheckTFTAck(BinDispCom2Buffer, BufferLength, asReason))
        {
            BinDispRecv=true;
            LogBinDisplay("TFTAckOK", asReason, true);
            if(bTFTDirectPending && bTFTDirectWaiting)
            {
                AnsiString asMsg="";
                asMsg.sprintf("Index=%d, Bin=%d, Retry=%d", iTFTDirectIndex, iTFTDirectBin, iTFTDirectRetry);
                LogBinDisplay("TFTNumberDone", asMsg, true);
                bTFTDirectPending=false;
                bTFTDirectWaiting=false;
            }
            bTFTWaitEcho=false;
        }
        else
        {
            BinDispRecv=false;
            bTFTWaitEcho=false;
            if(bTFTDirectWaiting)
                bTFTDirectWaiting=false;
            LogBinDisplay("TFTAckNG", asReason+", Hex="+asHex, true);
        }
        return;
    }

    BinDispRecv=true;
};
//---------------------------------------------------------------------------
bool TMyBinDispCtrl::GetCOMPortStatus(AnsiString Com)
{
    HANDLE h=INVALID_HANDLE_VALUE;
    AnsiString CN="\\\\.\\"+Com;
    h=::CreateFile(CN.c_str(),
        GENERIC_READ|GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        0);
    CloseHandle(h);
    if(h==INVALID_HANDLE_VALUE)
        return false;
    return true;
};
//------------------------------------------------------------------------------
// 設定address=Index 是有存在的
//------------------------------------------------------------------------------
void TMyBinDispCtrl::InstalledUnit(int Index)
{
    bHasUnitArray[Index]=true;
    bHasUnit=true;
    bSetBin[Index]=true;
    if(iTotalInstalledUnit<Index)
        iTotalInstalledUnit=Index;
};
//------------------------------------------------------------------------------
// 顯示器控制中心
//------------------------------------------------------------------------------
void TMyBinDispCtrl::Spin()
{
    AnsiString Str;
    if(InitialOK==false)                                                    //jou 2010-05-19 start : 未initital完成,不能執行
        return;
    if(bStopProcess==false)
        return;
    if(bTimerRun)
        return;

    bTimerRun=true;
    int &Task=iBinDispCtrlTask;
    AnsiString CN;

    switch(Task)
    {
        case 1:
            iRusStatus=0;
            if(ComPort.Pos("COM")==0)
            {
                bTimerRun=false;
                return;
            }
            bStartSetBin=true;                                              //重設Bin
            for(int i=0; i<iUsedBinNumber; i++)
            {
                bHasError[i]=false;
            }
            #ifndef SOFT_SIMULATE
            bStartSetColor=true;                                            //重設顏色
            if(GetCOMPortStatus(ComPort))
            {
                StartComport(CommBin,ComPort);
                Task=50;
                iStartGetStatusTask=1;
            }
            else
            {
                StopComport(CommBin,ComPort);
            }
            #else
                bStartSetColor=false;
                Task=50;
                iStartGetStatusTask=1;
            #endif
            break;
        case 50:
            if(DoStartGetStatus())
            {
                Task=100;
            }
            break;
        case 100:
            if(bHasUnit==false)
                break;
            #ifndef SOFT_SIMULATE
            if(GetCOMPortStatus(ComPort))                                   //假設RS232斷線
            {
                iRusStatus=0;
                CommBin->StopComm();

                for(int i=0; i<iUsedBinNumber; i++)
                {
                    iBinNow[i]=0;                                           // 顯示器目前的Bin
                    iColorNow[i]=1;                                         // 顯示器目前的的Color
                }

                bStartSetColor=true;                                        //重設顏色
                bStartSetBin=true;                                          //重設Bin
                Task=1;
                break;
            }
            #endif
            if(bStartSetBin)
            {
                iStartSetBinTask=1;
                Task=300;
            }
            else if(bStartSetBin==false)
            {
                iStartSetBinTask=100;
                Task=300;
            }
            break;
        case 200:
            if(DoStartSetColor())
            {
                bStartSetColor=false;
                Task=100;
            }
            break;
        case 300:
            if(DoStartSetBin())
            {
                Task=100;
            }
            break;
    }
    bTimerRun=false;
}
//------------------------------------------------------------------------------
void TMyBinDispCtrl::WriteTargetBin(int Index, int *bin, int color)
{
    //jou 2011-08-15 start : Addr破壞記憶體
    if(Index>iUsedBinNumber)
        return;
    if(Index<0)
        return;
    //jou 2011-08-15 end

    //bin = -1      : 不顯示，所以填入123
    //bin = 0~99    : 顯示數字0~99
    //bin = 100~125 : 顯示字母A~Z

    // bin輸入值對照表
    //  A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z
    //  100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125

    iSetColor[Index]=color;
    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        iSetBin[Index][i]=bin[i];
        bStartSetBin=true;
        bStartSetColor=true;
        bSetBin[Index]=true;
    }
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::WriteTargetBin(int color)
{
    bStartSetBin=true;
    bStartSetColor=true;
    for(int i=0; i<Bin_MAX_NUM; i++)
    {
        iSetColor[i]=color;
        bSetBin[i]=true;
    }

}
//---------------------------------------------------------------------------
//AI(general) 20260520 : separate COM Magazine TFT uses initial/connection polling only
void TMyBinDispCtrl::SpinTFTInitialOnly()
{
    if(InitialOK==false)
        return;
    if(bTimerRun)
        return;

    bTimerRun=true;
    int &Task=iBinDispCtrlTask;
    switch(Task)
    {
        case 1:
            iRusStatus=0;
            if(ComPort.Pos("COM")==0)
            {
                bTimerRun=false;
                return;
            }
            for(int i=0; i<iUsedBinNumber; i++)
            {
                bHasError[i]=false;
            }
            #ifndef SOFT_SIMULATE
            if(GetCOMPortStatus(ComPort))
            {
                StartComport(CommBin,ComPort);
                Task=50;
                iStartGetStatusTask=1;
            }
            else
            {
                StopComport(CommBin,ComPort);
            }
            #else
            Task=50;
            iStartGetStatusTask=1;
            #endif
            break;
        case 50:
            if(DoInitialStatusTFT())
            {
                Task=100;
            }
            break;
        case 100:
            if(bHasUnit==false)
                break;
            #ifndef SOFT_SIMULATE
            if(GetCOMPortStatus(ComPort))
            {
                iRusStatus=0;
                CommBin->StopComm();
                for(int i=0; i<iUsedBinNumber; i++)
                {
                    iBinNow[i]=0;
                    iColorNow[i]=1;
                }
                Task=1;
                break;
            }
            #endif
            ProcessTFTDirectRetry();
            break;
    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::SpinTFT()
{
    AnsiString Str="";
    if(InitialOK==false)                                                    //jou 2010-05-19 start : 未initital完成,不能執行
        return;
    if(bTimerRun)
        return;

    bTimerRun=true;
    int &Task=iBinDispCtrlTask;
    switch(Task)
    {
        case 1:
            iRusStatus=0; 
            if(ComPort.Pos("COM")==0)
            {
                bTimerRun=false;
                return;
            }
            bStartSetBin=true;                                              //重設Bin
            for(int i=0; i<iUsedBinNumber; i++)
            {
                bHasError[i]=false;
            }
            #ifndef SOFT_SIMULATE
            if(GetCOMPortStatus(ComPort))
            {
                StartComport(CommBin,ComPort);
                Task=50;
                iStartGetStatusTask=1;
            }
            else
            {
                StopComport(CommBin,ComPort);
            }
            #else
            Task=50;
            iStartGetStatusTask=1;
            #endif
            break;
        case 50:
            if(DoInitialStatusTFT())
            {
                Task=100;
            }
            break;
        case 100:
            if(bHasUnit==false)
                break;
            #ifndef SOFT_SIMULATE
            if(GetCOMPortStatus(ComPort))                                   //假設RS232斷線
            {
                iRusStatus=0;
                CommBin->StopComm();
                for(int i=0; i<iUsedBinNumber; i++)
                {
                    iBinNow[i]=0;                                           // 顯示器目前的Bin
                    iColorNow[i]=1;                                         // 顯示器目前的的Color
                }
                bStartSetBin=true;                                          //重設Bin
                Task=1;
                break;
            }
            #endif
            ProcessTFTDirectRetry();
            if(bTFTDirectPending)
                break;
            if(bStartSetBin)
            {
                iStartSetBinTask=1;
                Task=300;
            }
            else if(bStartSetBin==false)
            {
                iStartSetBinTask=100;
                Task=300;
            }
            break;
        case 300:
            if(DoStartSetBinTFT())
            {
                Task=100;
            }
            break;
    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
int iAddArray[30]= {
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
                    0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 
                    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E
};
//---------------------------------------------------------------------------
//AI(general) 20260520 : TFT command log, ACK validation, and direct-write retry support
void TMyBinDispCtrl::LogBinDisplay(AnsiString asAction, AnsiString asMessage, bool bMemo)
{
    AnsiString asLine=asAction+", "+asMessage;
    if(slBinDispLog!=NULL)
        slBinDispLog->AddTextWithDateTime(asLine);
    if(bMemo && fComPort!=NULL && fComPort->cbBinCheckLog!=NULL && fComPort->cbBinCheckLog->Checked)
        fComPort->MemoAddString(fComPort->memoBinCom, "", asLine);
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::AddBinDisplayLog(AnsiString asAction, AnsiString asMessage)
{
    LogBinDisplay(asAction, asMessage, false);
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::FlushBinDisplayLog()
{
    if(slBinDispLog!=NULL)
        slBinDispLog->MySaveToFile();
}
//---------------------------------------------------------------------------
unsigned char TMyBinDispCtrl::CalcTFTLRC(char* cstr, int iStart, int iEnd)
{
    unsigned char cSum=0;
    for(int i=iStart; i<=iEnd; i++)
        cSum+=(unsigned char)cstr[i];
    return (unsigned char)((~cSum)+1);
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::GetTFTErrorText(unsigned char cCode)
{
    switch(cCode)
    {
        case 0x43: return "Error Data Range(CC)";
        case 0x44: return "Error Data Item(DD)";
        case 0x41: return "Error Func Code(AA)";
        case 0x42: return "Error End(BB)";
        case 0x45: return "Error Addr(EE)";
        case 0x46: return "Error LRC(FF)";
    }
    return "";
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::PrepareTFTAck(int index, int iFuncHigh, int iFuncLow, int iDataHigh, int iDataLow, AnsiString asAction)
{
    if(index>=0 && index<30)
        iTFTExpectedAddr=iAddArray[index];
    else
        iTFTExpectedAddr=-1;
    iTFTExpectedFuncHigh=iFuncHigh;
    iTFTExpectedFuncLow=iFuncLow;
    iTFTExpectedDataHigh=iDataHigh;
    iTFTExpectedDataLow=iDataLow;
    asTFTExpectedAction=asAction;
    bTFTWaitEcho=true;
}
//---------------------------------------------------------------------------
bool TMyBinDispCtrl::CheckTFTAck(char* cstr, int iNum, AnsiString &asReason)
{
    int iFuncPos=4;
    int iDataPos=6;
    int iDataStart=8;
    int iDataEnd=16;
    int iLRCPos=17;
    int iCRPos=18;
    int iLFPos=19;

    if(iNum>=20 && (unsigned char)cstr[2]==0x00)
    {
        iFuncPos=4;
        iDataPos=6;
        iDataStart=8;
        iDataEnd=16;
        iLRCPos=17;
        iCRPos=18;
        iLFPos=19;
    }
    else if(iNum>=19)
    {
        iFuncPos=3;
        iDataPos=5;
        iDataStart=7;
        iDataEnd=15;
        iLRCPos=16;
        iCRPos=17;
        iLFPos=18;
    }
    else
    {
        asReason.sprintf("LenTooShort=%d, Action=%s", iNum, asTFTExpectedAction.c_str());
        return false;
    }

    if((unsigned char)cstr[0]!=0x3A)
    {
        asReason.sprintf("HeaderNG=%02X, Action=%s", (unsigned char)cstr[0], asTFTExpectedAction.c_str());
        return false;
    }
    if((unsigned char)cstr[iCRPos]!=0x0D || (unsigned char)cstr[iLFPos]!=0x0A)
    {
        asReason.sprintf("EndNG=%02X%02X, Action=%s", (unsigned char)cstr[iCRPos], (unsigned char)cstr[iLFPos], asTFTExpectedAction.c_str());
        return false;
    }
    unsigned char cLRC=CalcTFTLRC(cstr, 1, iDataEnd);
    if(cLRC!=(unsigned char)cstr[iLRCPos])
    {
        asReason.sprintf("LRCNG Calc=%02X Recv=%02X, Action=%s", cLRC, (unsigned char)cstr[iLRCPos], asTFTExpectedAction.c_str());
        return false;
    }
    for(int i=iDataStart; i<iDataEnd; i++)
    {
        if((unsigned char)cstr[i]==(unsigned char)cstr[i+1])
        {
            AnsiString asError=GetTFTErrorText((unsigned char)cstr[i]);
            if(asError!="")
            {
                asReason=asError+", Action="+asTFTExpectedAction;
                return false;
            }
        }
    }
    if(iTFTExpectedAddr>=0 && (unsigned char)cstr[1]!=(unsigned char)iTFTExpectedAddr)
    {
        asReason.sprintf("AddrNG Exp=%02X Recv=%02X, Action=%s", iTFTExpectedAddr, (unsigned char)cstr[1], asTFTExpectedAction.c_str());
        return false;
    }
    if((unsigned char)cstr[iFuncPos]!=(unsigned char)iTFTExpectedFuncHigh ||
       (unsigned char)cstr[iFuncPos+1]!=(unsigned char)iTFTExpectedFuncLow)
    {
        asReason.sprintf("FuncNG Exp=%02X%02X Recv=%02X%02X, Action=%s", iTFTExpectedFuncHigh, iTFTExpectedFuncLow, (unsigned char)cstr[iFuncPos], (unsigned char)cstr[iFuncPos+1], asTFTExpectedAction.c_str());
        return false;
    }
    bool bDataItemOK=((unsigned char)cstr[iDataPos]==(unsigned char)iTFTExpectedDataHigh &&
                      (unsigned char)cstr[iDataPos+1]==(unsigned char)iTFTExpectedDataLow);
    if(bDataItemOK==false && iTFTExpectedFuncHigh==0x08 && iTFTExpectedFuncLow==0x00 &&
       iTFTExpectedDataHigh==0x30 && iTFTExpectedDataLow==0x30 &&
       (unsigned char)cstr[iDataPos]==0x30 && (unsigned char)cstr[iDataPos+1]==0x31)
    {
        bDataItemOK=true;
    }
    if(bDataItemOK==false)
    {
        asReason.sprintf("DataItemNG Exp=%02X%02X Recv=%02X%02X, Action=%s", iTFTExpectedDataHigh, iTFTExpectedDataLow, (unsigned char)cstr[iDataPos], (unsigned char)cstr[iDataPos+1], asTFTExpectedAction.c_str());
        return false;
    }
    asReason.sprintf("Addr=%02X, Func=%02X%02X, DataItem=%02X%02X, Action=%s", (unsigned char)cstr[1], (unsigned char)cstr[iFuncPos], (unsigned char)cstr[iFuncPos+1], (unsigned char)cstr[iDataPos], (unsigned char)cstr[iDataPos+1], asTFTExpectedAction.c_str());
    return true;
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::SendTFTCommand(AnsiString asAction, int index, int iFuncHigh, int iFuncLow, int iDataHigh, int iDataLow, AnsiString asCommand, int iBin)
{
    AnsiString asHex=Chararr2Hexstring((char*)asCommand.c_str(), asCommand.Length());
    AnsiString asMsg="";
    if(CommBin==NULL)
    {
        asMsg.sprintf("Action=%s, Index=%d, Bin=%d, NoComm, Hex=%s", asAction.c_str(), index, iBin, asHex.c_str());
        LogBinDisplay("SendNG", asMsg, true);
        return;
    }
    PrepareTFTAck(index, iFuncHigh, iFuncLow, iDataHigh, iDataLow, asAction);
    asMsg.sprintf("Action=%s, COM=%s, Index=%d, Addr=%02X, Bin=%d, Hex=%s", asAction.c_str(), ComPort.c_str(), index, (index>=0 && index<30)?iAddArray[index]:0, iBin, asHex.c_str());
    LogBinDisplay("Send", asMsg, true);
    CommBin->WriteCommData(asCommand.c_str(), asCommand.Length());
}
//---------------------------------------------------------------------------
bool TMyBinDispCtrl::RequestSetBinTFTNumber(int index, int ibin, int iMaxRetry)
{
    if(index<0 || index>=iUsedBinNumber || index>=Bin_MAX_NUM)
    {
        AnsiString asMsg="";
        asMsg.sprintf("Index=%d, Bin=%d, Used=%d", index, ibin, iUsedBinNumber);
        LogBinDisplay("TFTNumberReject", asMsg, true);
        return false;
    }
    if(iMaxRetry<0)
        iMaxRetry=0;
    if(iMaxRetry>5)
        iMaxRetry=5;
    bTFTDirectPending=true;
    bTFTDirectWaiting=false;
    iTFTDirectIndex=index;
    iTFTDirectBin=ibin;
    iTFTDirectRetry=0;
    iTFTDirectMaxRetry=iMaxRetry;
    AnsiString asMsg="";
    asMsg.sprintf("Index=%d, Bin=%d, MaxRetry=%d", index, ibin, iMaxRetry);
    LogBinDisplay("TFTNumberRequest", asMsg, true);
    ProcessTFTDirectRetry();
    return true;
}
//---------------------------------------------------------------------------
void TMyBinDispCtrl::ProcessTFTDirectRetry()
{
    if(bTFTDirectPending==false)
        return;
    if(bTFTDirectWaiting)
    {
        if(TFTDirectRetryDelay.Off()==false)
            return;
        bTFTDirectWaiting=false;
        bTFTWaitEcho=false;
        if(iTFTDirectRetry>=iTFTDirectMaxRetry)
        {
            AnsiString asMsg="";
            asMsg.sprintf("Index=%d, Bin=%d, Retry=%d", iTFTDirectIndex, iTFTDirectBin, iTFTDirectRetry);
            LogBinDisplay("TFTNumberFail", asMsg, true);
            if(iTFTDirectIndex>=0 && iTFTDirectIndex<Bin_MAX_NUM)
                bHasError[iTFTDirectIndex]=true;
            bTFTDirectPending=false;
            return;
        }
        iTFTDirectRetry++;
        AnsiString asRetry="";
        asRetry.sprintf("Index=%d, Bin=%d, Retry=%d", iTFTDirectIndex, iTFTDirectBin, iTFTDirectRetry);
        LogBinDisplay("TFTNumberRetry", asRetry, true);
    }
    else if(bTFTWaitEcho)
    {
        LogBinDisplay("TFTNumberQueue", asTFTExpectedAction, true);
        return;
    }
    SetBin_TFT_number(iTFTDirectIndex, iTFTDirectBin, true);
    if(bTFTWaitEcho)
    {
        bTFTDirectWaiting=true;
        TFTDirectRetryDelay.SetSecAndOn(1);
    }
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::ReadVersion_TFT(int index)
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x08, 0x00},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x30,0x30},
        iNumberOfData[9]={0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;
    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

    SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBinFont_TFT_number(int index)       //Ian 20241105 : TFT 設定數字字形
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x02},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x01},
        iNumberOfData[9]={0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    iNumberOfData[4]=0x22;      //Green
    iNumberOfData[5]=0x8b;
    iNumberOfData[6]=0x22;

    iNumberOfData[8]=0xff;      //透明度
    iNumberOfData[7]=0x04;      //字體06
    iNumberOfData[0]=0x08;      //置中
    iNumberOfData[1]=0x15;

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

    SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBinFont_TFT_bin(int index)       //Ian 20241105 : TFT 設定Bin字形
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x02},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x02},
        iNumberOfData[9]={0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    iNumberOfData[4]=0xff;
    iNumberOfData[5]=0x80;
    iNumberOfData[6]=0x00;

    iNumberOfData[8]=0xff;  //透明度    Ian 0131
    iNumberOfData[7]=0x01;  //字體1

    iNumberOfData[0]=0x09;    //置中
    iNumberOfData[1]=0x01;

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBinFont_TFT_EA(int index)                               //Ian 20241105 : TFT 設定EA字形
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x02},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x03},
        iNumberOfData[9]={0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    iNumberOfData[4]=0xff;
    iNumberOfData[5]=0xff;
    iNumberOfData[6]=0xf0;

    iNumberOfData[8]=0xff;  //透明度    Ian 0131
    iNumberOfData[7]=0x01;  //字體1

    iNumberOfData[0]=0x60;    //置中
    iNumberOfData[1]=0x58;

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBinFont_TFT_Count(int index)                            //Ian 20241105 : TFT 設定數量字形
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x02},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x04},
        iNumberOfData[9]={0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    iNumberOfData[4]=0xff;
    iNumberOfData[5]=0xff;
    iNumberOfData[6]=0xf0;

    iNumberOfData[8]=0xff;  //透明度    Ian 0131
    iNumberOfData[7]=0x01;  //字體1

    iNumberOfData[0]=0x01;    //置中
    iNumberOfData[1]=0x58;

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBackGround_TFT(int index)
{
    int iHeader=0x3a,
        iByteCount[2]={0x00, 0x0d},
        iFunctionCode[2]={0x00, 0x01},
        iDataItem[2]={0x00,0x02},
        iNumberOfData[9]={0x30,0x30,0x00,0x59,0xaa,0x30,0x69,0X69,0x69},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::WriteScreenNoBackGround_TFT(int index)
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x04},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x01},
        iNumberOfData[9]={0x30,0x30,0x30,0x30,0x30,0x00,0x00,0x07,0x00},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBin_TFT_number(int index, int *bin)                            //Ian 20241105 : TFT 設定數量
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x03},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x01},
        iNumberOfData[9]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
        iDelimiter[2]={0x0d,0x0a};
    char cSetLimit[8]="";
    AnsiString sCmmd="";
    unsigned char Btmp1;

    sprintf(cSetLimit,"%03d",bin[0]);
    if(bin[0]==-1)
    {
        iNumberOfData[6]=0x2D;
        iNumberOfData[7]=0x2D;
        iNumberOfData[8]=0x2D;
    }
    else
    {
        iNumberOfData[6]=cSetLimit[0];
        iNumberOfData[7]=cSetLimit[1];
        iNumberOfData[8]=cSetLimit[2];
    }

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBin_TFT_number(int index, int ibin, bool bsend)                            //Ian 20241105 : TFT 設定數量
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x03},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x01},
        iNumberOfData[9]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
        iDelimiter[2]={0x0d,0x0a};
    char cSetLimit[8]="";
    AnsiString sCmmd="";
    unsigned char Btmp1;

    sprintf(cSetLimit,"%03d",ibin);
    if(ibin==-1)
    {
        iNumberOfData[6]=0x2D;
        iNumberOfData[7]=0x2D;
        iNumberOfData[8]=0x2D;
    }
    else
    {
        iNumberOfData[6]=cSetLimit[0];
        iNumberOfData[7]=cSetLimit[1];
        iNumberOfData[8]=cSetLimit[2];
    }

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow || bsend)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBin_TFT_bin(int index)
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x03},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x02},
        iNumberOfData[9]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    iNumberOfData[0]=0x42;  //B
    iNumberOfData[1]=0x69;  //I
    iNumberOfData[2]=0x6e;  //N

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBin_TFT_EA(int index)
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x03},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x03},
        iNumberOfData[9]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    iNumberOfData[0]=0x45; //E
    iNumberOfData[1]=0x41; //A

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::SetBin_TFT_Count(int index)
{
    int iHeader=0x3a,
        iFunctionCode[2]={0x00, 0x03},
        iByteCount[2]={0x00, 0x0d},
        iDataItem[2]={0x00,0x04},
        iNumberOfData[9]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
        iDelimiter[2]={0x0d,0x0a};
    AnsiString sCmmd="";
    unsigned char Btmp1;

    iNumberOfData[0]=0x45; //E
    iNumberOfData[1]=0x41; //A

    Btmp1=iAddArray[index]+iByteCount[0]+iByteCount[1]+iFunctionCode[0]+iFunctionCode[1]+iDataItem[0]+iDataItem[1]+
          iNumberOfData[0]+iNumberOfData[1]+iNumberOfData[2]+iNumberOfData[3]+iNumberOfData[4]+
          iNumberOfData[5]+iNumberOfData[6]+iNumberOfData[7]+iNumberOfData[8];
    Btmp1=~Btmp1;
    Btmp1+=1;

    sCmmd=AnsiString().sprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            iHeader,iAddArray[index],iByteCount[0], iByteCount[1],iFunctionCode[0],
            iFunctionCode[1],iDataItem[0],iDataItem[1], iNumberOfData[0], iNumberOfData[1],
            iNumberOfData[2], iNumberOfData[3], iNumberOfData[4], iNumberOfData[5], iNumberOfData[6],
            iNumberOfData[7], iNumberOfData[8], Btmp1, iDelimiter[0], iDelimiter[1]);

//    if(fComPort->bShow)
    {
        SendTFTCommand("TFTCommand", index, iFunctionCode[0], iFunctionCode[1], iDataItem[0], iDataItem[1], sCmmd, -999);
    }
    return sCmmd;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Addr: 0~F
//Comm:
//0 Read Status     : 2,X,0,3,0,0,8,0,0,0,0,1   XX,XX   \R\N
//1 Write Num       : 2,X,0,6,0,0,8,0,0,0,X,X   XX,XX   \R\N    0~99
//2 Write Eng       : 2,X,0,6,0,0,8,1,0,0,X,X   XX,XX   \R\N    A~Z = 0~26
//3 R LIGHT         : 2,X,0,6,0,0,8,2,0,0,0,1   XX,XX   \R\N
//4 G LIGHT         : 2,X,0,6,0,0,8,2,0,0,0,2   XX,XX   \R\N
//5 R+G LIGHT       : 2,X,0,6,0,0,8,2,0,0,0,3   XX,XX   \R\N
//------------------------------------------------------------------------------
void TMyBinDispHT9046::WriteBin(int Addr, int Command, short Value)
{
    unsigned char Btmp1;
    AnsiString Str;
    sprintf(SendBuffer, ":%02d06008%d00%02d00%c%c", Addr+1, Command, Value, Bin_CR, Bin_LF);

    Btmp1=A_Create_LCR(&SendBuffer[1], 12);
    SendBuffer[13]=T_HEX2ASCII_Mac(Btmp1>>4);   //Set return number by byte
    SendBuffer[14]=T_HEX2ASCII_Mac(Btmp1);
    CommBin->WriteCommData(SendBuffer, strlen(SendBuffer));

     Str.sprintf("Send, :%02d06008%d00%02d00%s%s, WriteBin", Addr+1, Command, Value, MyDeCodeASCII(SendBuffer[13]), MyDeCodeASCII(SendBuffer[14]));

    if(fComPort->cbBinCheckLog->Checked)
    {
        slBinDispLog->AddTextWithDateTime(Str);
        fComPort->MemoAddString(fComPort->memoBinCom, "", Str);
    }
};
//------------------------------------------------------------------------------
void TMyBinDispHT9046::WriteColor(int Addr, short Value)
{
    unsigned char Btmp1;
    AnsiString Str;
    if(Addr>=10)
        sprintf(SendBuffer, ":%02X06008200%02d00%c%c", Addr+38, Value, Bin_CR, Bin_LF);
    else
        sprintf(SendBuffer, ":%02X06008200%02d00%c%c", Addr+32, Value, Bin_CR, Bin_LF);
    Btmp1=A_Create_LCR(&SendBuffer[1], 12);
    SendBuffer[13]=T_HEX2ASCII_Mac(Btmp1>>4);   //Set return number by byte
    SendBuffer[14]=T_HEX2ASCII_Mac(Btmp1);
    CommBin->WriteCommData(SendBuffer, strlen(SendBuffer));

    if(Addr>=10)
        Str.sprintf("Send, :%02X06008200%02d%s%s, WriteColor", Addr+38, Value, MyDeCodeASCII(SendBuffer[13]), MyDeCodeASCII(SendBuffer[14]));
    else
        Str.sprintf("Send, :%02X06008200%02d%s%s, WriteColor", Addr+32, Value, MyDeCodeASCII(SendBuffer[13]), MyDeCodeASCII(SendBuffer[14]));

    if(fComPort->cbBinCheckLog->Checked)
    {
        slBinDispLog->AddTextWithDateTime(Str);
        fComPort->MemoAddString(fComPort->memoBinCom, "", Str);
    }
};
//------------------------------------------------------------------------------
void TMyBinDispHT9046::ReadVersion(int Addr)
{
    unsigned char Btmp1;
    AnsiString Str;
    sprintf(SendBuffer, ":%02d030080000100%c%c", Addr+1, Bin_CR, Bin_LF);

    Btmp1=A_Create_LCR(&SendBuffer[1], 12);
    SendBuffer[13]=T_HEX2ASCII_Mac(Btmp1>>4);   //Set return number by byte
    SendBuffer[14]=T_HEX2ASCII_Mac(Btmp1);
    CommBin->WriteCommData(SendBuffer, strlen(SendBuffer));
    Str.sprintf("Send, :%02d0300800001%s%s, ReadVersion", Addr+1, MyDeCodeASCII(SendBuffer[13]), MyDeCodeASCII(SendBuffer[14]));

    if(fComPort->cbBinCheckLog->Checked)
    {
        slBinDispLog->AddTextWithDateTime(Str);
        fComPort->MemoAddString(fComPort->memoBinCom, "", Str);
    }
}
//------------------------------------------------------------------------------
bool TMyBinDispHT9046::DoStartSetBin()
{
    AnsiString sCheckWord="";
    int &Task=iStartSetBinTask;
    int tag=0;

    if(bStartSetBin==true)
    {
        iRusStatus=3;
    }
    else
    {
        iRusStatus=5;
    }

    switch(Task)
    {
        case 1:         //初始化，只要重設定Bin，就要初始化一次
            Addr=0;
            Task=100;
            for(int i=0; i<iUsedBinNumber; i++)
            {
                bSliding[i]=true;
                bSetBin[i]=true;
                iCount[i]=0;
            }
            break;
        case 100:       //開始設定
            //Steven 20140404 Start : Fixed 256Bin
            while(1)
            {
                if(Addr>=iUsedBinNumber)  //第一次設定完成，準備開始輪播
                {
                    Addr=0;
                    if(bStartSetBin)
                    {
                        Task=1000;
                        bStartSetBin=false;
                    }
                    break;
                }

                if(bSliding[Addr]==true && bSetBin[Addr]==true && bHasUnitArray[Addr])
                {
                    //Steven 20140404 Start: Fixed 256Bin
                    if(iSetBin[Addr][iCount[Addr]]==-1)
                    {
                        iCount[Addr]=0;
                    }
                    else if(iTestBinCount>=16 && Addr>=3 && iSetBin[Addr][iCount[Addr]]>=100 && iSetBin[Addr][iCount[Addr]]!=104) //256 bin 超過顯示bin
                    {
                        if(Addr==BinSelect[tag].IfError+3)
                            iCount[Addr]++;
                        else
                            iCount[Addr]=0;
                    }

                    if(iSetBin[Addr][0]==-1)                    //沒有設定，所以寫X
                    {
                        WriteBin(Addr, 1, 123-100);
                    }
                    else if(iSetBin[Addr][iCount[Addr]]<100)    //數字
                    {
                        WriteBin(Addr, 0, iSetBin[Addr][iCount[Addr]]);
                    }  //ChungHung 20140419 修正顯示問題
                    else if(Addr<=3 || iSetBin[Addr][iCount[Addr]]==104)
                    {
                        WriteBin(Addr, 1, iSetBin[Addr][iCount[Addr]]-100);
                    }
                    else
                    {
                        Addr++;
                        break;
                    }

                    Task=200;
                    BinDispRecv=false;
                    BinDisDelay.SetSecAndOn(2);
                    break;
                }
                Addr++;
            }
            //Steven 20140404 End: Fixed 256Bin
            break;
        case 200:   //接收回傳資料
            if(BinDispRecv)
            {
                if(Addr>=iUsedBinNumber)
                {
                    Task=100;
                    break;
                }
                if(iVersion[Addr]==1)  //舊模組
                {
                    sCheckWord.sprintf(":%02d06020010", Addr+1);
                }
                else if(iVersion[Addr]==2) //新模組
                {
                    if(iSetBin[Addr][0]==-1)                    //沒有設定，所以寫X
                        sCheckWord.sprintf(":%02d060201%02d", Addr+1, 123-100);
                    else if(iSetBin[Addr][iCount[Addr]]<100)    //數字
                        sCheckWord.sprintf(":%02d060200%02d", Addr+1, iSetBin[Addr][iCount[Addr]]);
                    else                                        //字母
                        sCheckWord.sprintf(":%02d060201%02d", Addr+1, iSetBin[Addr][iCount[Addr]]-100);
                }

                if(sReadBuffer.Pos(sCheckWord)==1)
                {
                    iBinNow[Addr]=iSetBin[Addr][iCount[Addr]];
                    bHasError[Addr]=false;                  // 顯示器是否有出錯
                    iErrCount[Addr]=0;

                    iCount[Addr]++;
                    if(iSetBin[Addr][iCount[Addr]]==-1 ||   //如果是0 表示已經到最後一個Bin，所以從頭開始Show
                       iCount[Addr]>=iTestBinCount)         //Steven 20140404 : eBinDispTotal --> iTestBinCount
                    {
                        iCount[Addr]=0;
                    }
                    if(iSetBin[Addr][0]==-1 || iSetBin[Addr][1]==-1)    //如果是X 表示該位置沒有設Bin，不需要輪撥
                    {
                        bSliding[Addr]=false;
                    }
                    Addr++; //Steven 20211230 : 修正Bin顯示器異常恢復後不會輪播的問題
                    if(Addr>=iUsedBinNumber)           //kevin 20230518 add over   iErrCount[Addr]   :12
                    {
                        Task=100;
                        break;
                    }
                }
                else
                {
                    iErrCount[Addr]++;
                }
                //Steven 20110326
                Task=100;
            }
            else if(BinDisDelay.Off())
            {
                iErrCount[Addr]++;
                Task=100;
            }

            if(Addr>=iUsedBinNumber)  //JerryYang 20230515 : 避免超出陣列
            {
                Addr=0;
            }
            if(iErrCount[Addr]>5)
            {
                iRusStatus=4;
                Task=100;
                bHasError[Addr]=true;       // 顯示器是否有出錯
            }
            break;
        case 1000:
            BinDisDelay.SetSecAndOn(iDelaySec);
            Task=1100;
        case 1100:
            if(BinDisDelay.Off())
            {
                for(int i=0; i<iUsedBinNumber ; i++)
                    if(bHasUnitArray[i])
                        bSetBin[i]=true;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyBinDispHT9046::DoStartSetColor()
{
    AnsiString sCheckWord="";
    int &Task=iStartSetColorTask;
    iRusStatus=2;
    switch(Task)
    {
        case 1:
            Addr=0;
            Task=100;
            for(int i=0; i<iUsedBinNumber; i++)
                bSetColor[i]=true;
            break;
        case 100:
            while(1)
            {
                if(Addr>=iUsedBinNumber)
                    return true;

                if(bSetColor[Addr]==true && bHasUnitArray[Addr])
                {
                    WriteColor(Addr, iSetColor[Addr]);   //設定顏色
                    Task=200;
                    BinDispRecv=false;
                    BinDisDelay.SetSecAndOn(2);
                    break;
                }
                Addr++;
            }
            break;
        case 200:
            if(BinDispRecv)
            {
                if(iVersion[Addr]==1)  //舊模組
                {
                    if(Addr>=10)
                        sCheckWord.sprintf(":%02X06020010", Addr+38);
                    else
                        sCheckWord.sprintf(":%02X06020010", Addr+32);
                }
                else if(iVersion[Addr]==2) //新模組
                {
                    if(Addr>=10)
                        sCheckWord.sprintf(":%02X060202%02d", Addr+38, iSetColor[Addr]);
                    else
                        sCheckWord.sprintf(":%02X060202%02d", Addr+32, iSetColor[Addr]);
                }

                if(sReadBuffer.Pos(sCheckWord)==1)
                {
                    iColorNow[Addr]=iSetColor[Addr];    //紀錄現在的顏色
                    bHasError[Addr]=false;              //模組沒有出錯
                    iErrCount[Addr]=0;
                    bSetColor[Addr]=false;
                }
                else
                {
                    iErrCount[Addr]++;
                }
                //Steven 20110326 End
                Task=100;
            }
            else if(BinDisDelay.Off())
            {
                iErrCount[Addr]++;
                Task=100;
            }

            if(iErrCount[Addr]>5)
            {
                iRusStatus=4;
                bHasError[Addr]=true;
                iErrCount[Addr]=0;
                CommBin->StopComm();
                iBinDispCtrlTask=1;
                bStartSetColor=true;        //重設顏色
                bStartSetBin=true;          //重設Bin
                //Addr++;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyBinDispHT9046::DoStartGetStatus()
{
    AnsiString sCheckWord="";
    int &Task=iStartGetStatusTask;
    iRusStatus=1;

    switch(Task)
    {
        case 1:
            Addr=0;
            Task=100;
            for(int i=0; i<iUsedBinNumber; i++)
            {
                bGetStatus[i]=true;
                bHasUnitArray[Addr]=true;
            }
            break;
        case 100:
            while(1)
            {
                if(Addr>=iUsedBinNumber)
                    return true;

                if(bGetStatus[Addr]==true)
                {
                    ReadVersion(Addr);   //設定取得版本
                    Task=200;
                    BinDispRecv=false;
                    BinDisDelay.SetSecAndOn(2);
                    break;
                }
                Addr++;
            }
            break;
        case 200:
            if(BinDispRecv)
            {
                sCheckWord.sprintf(":%02d03020001", Addr+1);
                if(sReadBuffer.Pos(sCheckWord)==1)  //舊模組
                {
                    iVersion[Addr]=1;
                }
                else
                {
                    sCheckWord.sprintf(":%02d03020002", Addr+1);

                    if(sReadBuffer.Pos(sCheckWord)==1)  //新模組
                    {
                        iVersion[Addr]=2;
                    }
                    else
                    {
                        iVersion[Addr]=0;
                    }
                }
                if(iVersion[Addr]!=0)
                {
                    bHasUnitArray[Addr]=true;
                    bGetStatus[Addr]=false;
                    iErrCount[Addr]=0;
                }
                else
                {
                    iErrCount[Addr]++;
                }
                Task=100;
            }
            else if(BinDisDelay.Off())
            {
                iErrCount[Addr]++;
                Task=100;
            }

            if(iErrCount[Addr]>5)
            {
                iRusStatus=4;
                iErrCount[Addr]=0;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyBinDispCtrl::StartComport(TComm *Comm,AnsiString port)
{
    bool bret=false;
    AnsiString CN="", Str="";
    try
    {
        if(Comm==NULL)
        {
            SetComPort(port);
            SetDelayTime(100);
            InitialOK=true;
        }
        Comm->Parity=ComParity;
        Comm->OnReceiveData=CommBinReceiveData;
        CN="\\\\.\\"+port;
        Comm->CommName=CN;
        Comm->StartComm();
        Str.sprintf("BinDisp, Start Comm OK., %s", Comm->CommName);
        if(fComPort->cbBinCheckLog->Checked)
            fComPort->MemoAddString(fComPort->memoBinCom, "", Str);
        slBinDispLog->AddTextWithDateTime(Str);
        bret=true;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TMyBinDispCtrl::Timer1Timer");
        ShowMyMessage("Error open com port");
        Str.sprintf("BinDisp, Start Comm NG!, %s", Comm->CommName);
        if(fComPort->cbBinCheckLog->Checked)
            fComPort->MemoAddString(fComPort->memoBinCom, "", Str);
        slBinDispLog->AddTextWithDateTime(Str);
    }
    return bret;
}
//---------------------------------------------------------------------------
bool TMyBinDispCtrl::StopComport(TComm *Comm,AnsiString port)
{
    bool bret=false;
    AnsiString CN="", Str="";
    try
    {
        if(Comm==NULL)
        {
            SetComPort(port);
            SetDelayTime(100);
            InitialOK=true;
        }
        Comm->Parity=ComParity;
        Comm->OnReceiveData=CommBinReceiveData;
        CN="\\\\.\\"+port;
        Comm->CommName=CN;
        Comm->StopComm();
        Str.sprintf("BinDisp, Stop Comm OK., %s", Comm->CommName);
        if(fComPort->cbBinCheckLog->Checked)
            fComPort->MemoAddString(fComPort->memoBinCom, "", Str);
        slBinDispLog->AddTextWithDateTime(Str);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TMyBinDispCtrl::Timer1Timer");
        ShowMyMessage("Error close com port");
        Str.sprintf("BinDisp, Stop Comm NG!, %s", Comm->CommName);
        if(fComPort->cbBinCheckLog->Checked)
            fComPort->MemoAddString(fComPort->memoBinCom, "", Str);
        slBinDispLog->AddTextWithDateTime(Str);
    }
    return bret;
}
//---------------------------------------------------------------------------
typedef AnsiString (__closure *SetTFTBinInitalFunc)(int);
#define funcTotal 4
bool TMyBinDispCtrl::DoInitialStatusTFT()
{
    int &Task=iBinTask;
    switch(Task)
    {
        case 1:
            Addr=0;
            iFuncNum=0;
            Task=50;
            sTotalCommand="";
            break;
        case 50:
            if(Addr<iUsedBinNumber)
            {
                if(Addr>=20 && Addr<=22) //magzine
                {
                    if(bFirst)
                    {
                        Task=100;
                    }
                    else
                    {
                        Addr++;  //pass 軟體重開只初始化一次
                        iFuncNum=0;
                    }
                }
                else//auto
                {
                    Task=100;
                }
            }
            else
            {
                Task=9999;
            }
            break;
        case 100:
            BinDispRecv=false;
            if(iFuncNum==0)
            {
                Task=200;
            }
            else if(iFuncNum==1)
            {
                Task=300;
            }
            else if(iFuncNum==2)
            {
                Task=400;
            }
            else if(iFuncNum==3)
            {
                Task=500;
            }
            else
            {
                Addr++;
                iFuncNum=0;
                Task=50;
            }
            break;
        case 200:
            SetBinFont_TFT_number(Addr);
            BinDisDelay.SetSecAndOn(2);
            Task=1000;
            break;
        case 300:
            SetBinFont_TFT_bin(Addr);
            BinDisDelay.SetSecAndOn(2);
            Task=1000;
            break;
        case 400:
            WriteScreenNoBackGround_TFT(Addr);
            BinDisDelay.SetSecAndOn(2);
            Task=1000;
            break;
        case 500:
            SetBin_TFT_bin(Addr);
            BinDisDelay.SetSecAndOn(2);
            Task=1000;
            break;
        case 1000:
            if(BinDispRecv)
            {
                iFuncNum++;
                Task=100;
            }
            else if(BinDisDelay.Off())
            {
                iErrCount[Addr]++;
                Task=100;
            }
            if(iErrCount[Addr]>5)
            {
                iRusStatus=0;
                bHasError[Addr]=true;
                iErrCount[Addr]=0;
                Task=9999;
            }
            break;
        case 9999:
            Task=1;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyBinDispCtrl::DoStartSetBinTFT()
{
    AnsiString sCheckWord="";
    int &Task=iStartSetBinTask;
    int iBin[Bin_MAX_NUM];
    int iCount[Bin_MAX_NUM];
    int iDisplayIndex=0;
    for(int i=0; i<Bin_MAX_NUM; i++)
    {
        iBin[i]=0;
        iCount[i]=0;
    }
    MyBinToTrayStruct BtoT;
    for(int iAuto=0; iAuto<eAuto20+1; iAuto++)
    {
        BtoT.ConvertAutoToBin(iAuto);
        iBin[iAuto]=BtoT.iBin;
    }
    switch(Task)
    {
        case 1:         //初始化，只要重設定Bin，就要初始化一次
            Addr=eAuto1;
            Task=100;
            for(int i=0; i<eBinDispTotal; i++)
            {
                bSetBin[i]=true;
                iCount[i]=0;
            }
            sTotalCommand="";
            break;
        case 100:       //開始設定 
            if(Addr>=eAuto20+1)
            {
                if(bStartSetBin)
                {
                    Addr=eAuto1;
                    bStartSetBin=false;
                    sTotalCommand="";
                    Task=1000;
                }
                break;
            }
            iDisplayIndex=Addr-eAuto1;
            BinDispRecv=false;
            SetBin_TFT_number(iDisplayIndex, iBin[Addr]);
            Task=200;
            BinDisDelay.SetSecAndOn(3);
            break;
        case 200:   //接收回傳資料
            iDisplayIndex=Addr-eAuto1;
            if(iDisplayIndex<0 || iDisplayIndex>=eBinDispTotal)
            {
                Addr=0;
                Task=100;
                break;
            }
            if(BinDispRecv)
            {
                bHasError[iDisplayIndex]=false;
                iErrCount[iDisplayIndex]=0;
                Addr++;
                Task=100;
                break;
            }
            else if(BinDisDelay.Off())
            {
                iErrCount[iDisplayIndex]++;
                Task=100;
            }

            if(Addr>=eAuto20+1)                                            //JerryYang 20230515 : 避免超出陣列
            {
                Addr=0;
            }
            if(iErrCount[iDisplayIndex]>5)
            {
                iRusStatus=4;
                Task=100;
                bHasError[iDisplayIndex]=true;                                           // 顯示器是否有出錯
            }
            break;
        case 1000:
            BinDisDelay.SetSecAndOn(iDelaySec);
            Task=1100;
        case 1100:
            if(BinDisDelay.Off())
            {
                for(int i=0; i<eBinDispTotal ; i++)
                    if(bHasUnitArray[i])
                        bSetBin[i]=true;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void TMyBinDispCtrl::SetUsedBinNumber(int iNum)
{
    if(iNum>0 && iNum<Bin_MAX_NUM)
    {
        iUsedBinNumber=iNum;
    }
    else
    {
        iUsedBinNumber=Bin_MAX_NUM;
    }
}
//------------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::Chararr2Hexstring(char* cstr, int iNum)
{
    AnsiString tempStr="";
    for(int i=0; i<iNum; i++)
        tempStr+=" "+AnsiString().sprintf("%02X",(byte)cstr[i]);
    return tempStr;
}
//------------------------------------------------------------------------------
