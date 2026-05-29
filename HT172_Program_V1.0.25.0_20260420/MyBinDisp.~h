//----------------------------------------------------------------------------
#ifndef MyBinDispH
#define MyBinDispH
//----------------------------------------------------------------------------

#include <Classes.hpp>
#include "SPComm.hpp"
#include "MyStringList.h"
#include <ExtCtrls.hpp>
#define Bin_MAX_NUM 23
//----------------------------------------------------------------------------
class TDataModule3 : public TDataModule
{
__published:
    TComm *BinDisp;
private:
public:
    virtual __fastcall TDataModule3(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TDataModule3 *DataModule3;
//----------------------------------------------------------------------------
class TMyBinDispCtrl
{
private :
    AnsiString  sTotalCommand;
    TMySwitch *SwTFTBinRelay;
    bool bFirst;
    int iBinTask;
    bool bTimerRun;
    AnsiString Chararr2Hexstring(char* cstr,int iNum);
    void LogBinDisplay(AnsiString asAction, AnsiString asMessage, bool bMemo);
    void PrepareTFTAck(int index, int iFuncHigh, int iFuncLow, int iDataHigh, int iDataLow, AnsiString asAction);
    bool CheckTFTAck(char* cstr, int iNum, AnsiString &asReason);
    unsigned char CalcTFTLRC(char* cstr, int iStart, int iEnd);
    AnsiString GetTFTErrorText(unsigned char cCode);
    void SendTFTCommand(AnsiString asAction, int index, int iFuncHigh, int iFuncLow, int iDataHigh, int iDataLow, AnsiString asCommand, int iBin);
    void ProcessTFTDirectRetry();
    bool bTFTWaitEcho;
    bool bTFTDirectPending;
    bool bTFTDirectWaiting;
    int iTFTExpectedAddr;
    int iTFTExpectedFuncHigh;
    int iTFTExpectedFuncLow;
    int iTFTExpectedDataHigh;
    int iTFTExpectedDataLow;
    int iTFTDirectIndex;
    int iTFTDirectBin;
    int iTFTDirectRetry;
    int iTFTDirectMaxRetry;
    AnsiString asTFTExpectedAction;
    TQPF_Timer TFTDirectRetryDelay;
protected:  // User declarations
    int    Addr;
    int    iFuncNum;
    TQPF_Timer BinDisDelay;
    TQPF_Timer BinDisRecDelay;
    void __fastcall Timer1Timer(TObject *Sender);

    bool  bHasUnitArray[Bin_MAX_NUM];                   // 個別的顯示器是否有安裝
    bool  bHasUnit;                                     // 如果都沒有裝顯示器就會=false
    bool  bSliding[Bin_MAX_NUM];                        // 判斷該位置是否需要跳號
    bool  bStopProcess;                                 // 是否要停止Timer
    bool  bSetBin[Bin_MAX_NUM];                         // 那些顯示器要開始設定Bin
    int   iSetBin[Bin_MAX_NUM][TEST_MAX_BIN];           // 顯示器要設定的Bin            //Steven 20140402 : Fixed 記憶體破壞
    bool  bSetColor[Bin_MAX_NUM];                       // 那些顯示器要開始設定Bin
    bool  bGetStatus[Bin_MAX_NUM];
    int   iSetColor[Bin_MAX_NUM];                       // 顯示器要設定的Color
    bool  bStartSetBin;                                 // 開始進行設定顯示器Bin
    bool  bStartSetColor;                               // 開始進行設定顯示器Color
    int   iDelaySec;                                    // Bin輪替的時間
    int   iVersion[Bin_MAX_NUM];                        // 顯示器的版本
    int   iBinNow[Bin_MAX_NUM];                         // 顯示器目前的Bin
    int   iColorNow[Bin_MAX_NUM];                       // 顯示器目前的的Color
    bool  bHasError[Bin_MAX_NUM];                       // 顯示器是否有出錯
    int   iRusStatus;                                   // 顯示目前在執行的動作

    char SendBuffer[1024];                              // 欲對顯示器通訊之資料
    bool BinDispRecv;                                   // 有從顯示器通訊之 Comm Port收到一筆資料
    char BinDispCom2Buffer[1024];                              // 有從顯示器通訊之 Comm Port收到一筆資料之內容
    AnsiString  ComPort;                                // 顯示器通訊之 Comm Port

    unsigned char Com2ReceiveByte[1024];
    int Receivelen;

    int iStartSetBinTask;
    int iStartSetColorTask;
    int iStartGetStatusTask;
    int iBinDispCtrlTask;
    int iTotalInstalledUnit;

    //虛擬函式，需要在子類別實做
    virtual void WriteBin  (int Addr, int Command, short value)=0;  // 對address=Index顯示器送出欲設定之Bin
    virtual void WriteColor(int Addr, short value)=0;               // 對address=Index顯示器送出欲設定之顏色
    virtual void ReadVersion(int Addr)=0;                           // 讀取顯示器的狀態
    virtual bool DoStartSetBin()    {return false;};                // 開始Bin的顯示
    virtual bool DoStartSetColor()  {return false;};                // 開始設定顯示器顏色
    virtual bool DoStartGetStatus() {return false;};                // 確認顯示器的狀態

    //Steven 20100517 Start: 為了不要#include "cpublic.h"
    bool GetCOMPortStatus(AnsiString Com);
    unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii);
    unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex);
    unsigned char A_Create_LCR(unsigned char *Sptr, unsigned char length);
    int iErrCount[Bin_MAX_NUM];
    int iCount[Bin_MAX_NUM];
    int iUsedBinNumber;
    //Steven 20100517 End
public:
    TMyBinDispCtrl();
    ~TMyBinDispCtrl();
    AnsiString Alias[Bin_MAX_NUM];
    TComm  *CommBin;
    void ProcessStopStart(bool Value)  ;// 啟動或停止跳號
    void SetComPort(AnsiString port)   ;// 設定顯示器群組是利用那一個COM PORT
    void SetComParity(TParity Parity)  ;// 設定顯示器群組是利用那一個Parity
    bool UnitHasInstall(int Index)     ;// 確認該顯示器使是否有啟用
    void CloseUnit(int Index)          ;// 關閉該顯示器
    void OpenUnit(int Index)           ;// 開啟該顯示器
    void SetDelayTime(int Sec)         ;// 設定多久輪替一次
    int  GetDelayTime()                ;// 取得輪替的時間
    int  GetTotalInstalledUnit()       ;// 取得總共安裝了幾個顯示器
    int  GetColorNow(int Index)        ;// 顯示器目前的顏色
    int  GetBinNow(int Index)          ;// 顯示器目前的字元
    bool GerErrNow(int Index)          ;// 顯示器是否有出錯
    void SerErrNow(int Index,bool bErr)          ;// 顯示器是否有出錯
    AnsiString GetRunStatus()          ;// 回傳目前在執行的動作
    AnsiString GetComPort(){return ComPort;}
    void __fastcall CommBinReceiveData(TObject *Sender, Pointer Buffer, WORD BufferLength);
    TParity ComParity;                                  // 顯示器通訊之 Parity
    void InstalledUnit(int Index);                         // 設定address=Index 的顯示器是有存在的
    void WriteTargetBin(int Index, int *bin, int color);   // 將address=Index顯示器設定文字與顏色
    void WriteTargetBin(int ibin);
    AnsiString ReadVersion_TFT(int index);
    AnsiString SetBinFont_TFT_number(int index);
    AnsiString SetBinFont_TFT_bin(int index);
    AnsiString SetBinFont_TFT_EA(int index);
    AnsiString SetBinFont_TFT_Count(int index);
    AnsiString SetBackGround_TFT(int index);
    AnsiString WriteScreenNoBackGround_TFT(int index);
    AnsiString SetBin_TFT_number(int index, int *bin);
    AnsiString SetBin_TFT_number(int index, int ibin ,bool bsend=false);
    AnsiString SetBin_TFT_bin(int index);
    AnsiString SetBin_TFT_EA(int index);
    AnsiString SetBin_TFT_Count(int index);
    bool InitialOK;                                        //判斷主控端是否已經準備完成
    bool bFirstInit;
    TMyStringList *slBinDispLog;            //Steven 20210910 : Bin顯示器的log
    AnsiString sReadBuffer;                 //kevin 20170711 (Steven) fix define
    void Spin();
    void SpinTFT();
    void SpinTFTInitialOnly();
    bool StartComport(TComm *Comm,AnsiString port);
    bool StopComport(TComm *Comm,AnsiString port);
    bool DoInitialStatusTFT();
    bool DoStartSetBinTFT();
    void SetUsedBinNumber(int iNum);
    int GetUsedBinNumber(){return iUsedBinNumber;}
    void ResetBinFlow(){iBinDispCtrlTask=1;}
    void AddBinDisplayLog(AnsiString asAction, AnsiString asMessage);
    void FlushBinDisplayLog();
    bool RequestSetBinTFTNumber(int index, int ibin, int iMaxRetry);
};
//----------------------------------------------------------------------------
class TMyBinDispHT9046:public TMyBinDispCtrl
{
    private:
    protected:
        virtual void WriteBin(int Addr, int Command, short Value);
        virtual void WriteColor(int Addr, short Value);
        virtual void ReadVersion(int Addr);
        virtual bool DoStartSetBin();
        virtual bool DoStartSetColor();
        virtual bool DoStartGetStatus();
};
#endif
