//---------------------------------------------------------------------------

#ifndef uHGemFormH
#define uHGemFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

#define GEM_DefineEventReport     4                                             //pig 2012.12.27 SECS_GEM
#define GEM_RPTIDCount            100                                           //pig 2013.04.26 SECS_GEM
#define GEM_Recipe_Constant       14                                            //pig 2012.12.27 SECS_GEM

typedef struct                                                                  //pig 2012.12.26 SECS_GEM
{
    float fOutPassYield;
    float fOutFailYield;
    float fInCCDFailYield;
    float fOutCCDFailYield;
    bool bEvenLink[100][100][1000];                         //CEID,RPTID,VID
    AnsiString MachineStatus;
    int MachineLight;
    int MachineUPH;
    int SafeDoor[9];
    int EMG[4];
    int LOCK;
    int MachineAir;
    int MachineMotorPower;
    int MachineIonFan[5];
    int MachineIonFanLevel[5];

}SECS_GEM;

class TfHGem : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TGroupBox *GroupBox8;
    TPanel *SECSConnectionState;
    TGroupBox *GroupBox9;
    TSpeedButton *BtnEnableComm;
    TSpeedButton *BtnDisableComm;
    TPanel *GEMCommunicatingState;
    TRadioGroup *GemEnableOrDisable;
    TGroupBox *GroupBox10;
    TSpeedButton *GemBtnOnlineRequest;
    TSpeedButton *GemBtnOfflineRequest;
    TSpeedButton *GemBtnOnlineRemote;
    TSpeedButton *GemBtnOnlineLocal;
    TPanel *GemPanelControlState;
    TPanel *Panel2;
    TSpeedButton *GemBtnSendTerminalMessage;
    TSpeedButton *GemSBSetup;
    TSpeedButton *GemRefreshLocalFile;
    TSpeedButton *GemUpLoadLocalFileName;
    TSpeedButton *GemDisSelectAllLocalFile;
    TSpeedButton *GemSelectAllLocalFile;
    TSpeedButton *GemDeleteRemoteFileName;
    TSpeedButton *GemDownLoadRemoteFileName;
    TSpeedButton *GemListRemoteFileName;
    TSpeedButton *GemSelectAllRemoteFile;
    TSpeedButton *GemDisSelectAllRemoteFile;
    TSpeedButton *GemProcessProgramLoadInquire;
    TSpeedButton *GemFormattedProcessProgramSend;
    TSpeedButton *GemFormattedProcessProgramRequest;
    TMemo *GemCommMemo;
    TListBox *GemTerminalWindow;
    TEdit *GemTerminalSendEdit;
    TCheckListBox *GemLocalFileLixtBox;
    TCheckListBox *GemRemoteReceipeList;
    TLabel *labVacuum;
private:    // User declarations
public:     // User declarations
    __fastcall TfHGem(TComponent* Owner);
    void SaveSECSDefineReportData(int RPTID,int VID,int VIDSum);
    void DoSendSECSVIDForDefineReportCEID(int CEID);                                   //S6F19
    bool ReadSECSCEIDEnableOrDisable(int iCEID);
    void SaveSECSCEIDEnableOrDisable(int iCEID,bool bEnable);
    void SECS_VIDForDefineReportRPTIDEnable(int iCEID,int iRPTID,bool bEnable);
    void SECS_RPTIDForDefineReportCEIDEnable(int iCEID,bool bEnable);                //回傳已LINK CEID 的RPTID
    void DeleteSECSDefineReportCEIDLinkData( int CEID , int iMode );                //0:刪除單一個CEID 1:刪除全部CEID
    void DeleteSECSDefineReportLinkData( int CEID, int iMode );                      //0:刪除單一個CEID 1:刪除全部CEID
    bool GetSECSDefineReportCEIDRepeat(int CEID);                                    //判斷是否有已定義的CEID For S2F35
    int  ReadSECSDefineReportCEIDLinkData(int index);
    void ReadSECSDefineReportLinkData();
    void SaveSECSDefineReportCEIDLinkData(int CEID);
    void SaveSECSDefineReportLinkData(int CEID,int RPTID,int RPTIDSum);              //S2F35
    void ShowRecipeName(AnsiString aMane);
    void DeleteSECSDefineReportRPTData( int RPTID , int iMode );                     //0:刪除單一個RPTID 1:刪除全部RPTID
    void DeleteSECSDefineReportData( int RPTID, int iMode );                         //0:刪除單一個RPTID 1:刪除全部RPTID
    void InitialSECSDefineReport();         //Machine Define event report            //pig 2013.04.29 SECS_GEM
    void SendSECSVIDForDefineReportRPTID(int RPTID);                                 //S6F19
    bool GetSECSDefineReportRPTIDRepeat(int RPTID);                                  //判斷是否有已定義的RPTID For S2F33
    void SECS_MachineActionEventReportName(int iCEID);                               //回傳機台動作訊息名稱
    int  ReadSECSDefineReportRPTData(int index);
    void ReadSECSDefineReportData();
    void SaveSECSDefineReportRPTData(int RPTID);

    int WriteRecipeData(char *Path,char *Name,char *Data);                          //pig 2013.01.18 SECS_GEM
    void __fastcall SetHostNewValueforSECS(AnsiString ID,int NewValue);             //pig 2013.01.15 SECS_GEM
    void GemInitial();
    void GemAlarmCodeList();
    void UpDataSECSMachineStatus();
    void GemInitialDefault();
    int  MySFCode();


    bool bReportSECS_GEM_Message;                                                   //pig 2012.10.03 SECS_GEM
    //char GEM_MachineName[64];                                                     //pig 2012.10.03 SECS_GEM
    //char GEM_MachineVer[64];                                                      //pig 2012.10.03 SECS_GEM
    AnsiString GEM_MachineName;                                                     //pig 2012.10.03 SECS_GEM
    AnsiString GEM_MachineVer;                                                      //pig 2012.10.03 SECS_GEM
    AnsiString GEM_OriginalWF;

    vector < int > SECS_GEM_VID;                                                    //pig 2013.04.25 SECS_GEM
    map < int , vector < int > > SECS_GEM_RPTID;                                    //pig 2013.04.25 SECS_GEM

    vector < int > SECS_GEM_RPTID_KEY;                                              //pig 2013.05.03 SECS_GEM
    map < int , vector < int > > SECS_GEM_CEID;                                     //pig 2013.05.03 SECS_GEM

    bool SECS_GEM_PPSIGNALTOWER_CONTROL_flag;
    int  iSECS_GEM_PPSIGNALTOWER_CONTROL_RED;
    int  iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN;
    int  iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW;
    int  iSECS_GEM_PPSIGNALTOWER_Status[6][3];
    int  iSECS_GEM_Speed[7];
};
//---------------------------------------------------------------------------
extern PACKAGE TfHGem *fHGem;
//---------------------------------------------------------------------------
#endif
