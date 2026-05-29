//---------------------------------------------------------------------------
#ifndef noteH
#define noteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "butPa1.h"
#include <Buttons.hpp>
#include <Graphics.hpp>
#include "ALed.hpp"
#include "HTray.h"
#include "MyKitSuck.h"
#include "htray.h"
#include <ComCtrls.hpp>
#include <jpeg.hpp>
#include "MyLed.h"

//---------------------------------------------------------------------------
class MyNoteStruct
{
    public:     // User declarations
    TStringList *SystemErrorCode;
    TStringList *SysFlushPanelName;

    ~MyNoteStruct()
    {
        Clear();
        delete SystemErrorCode;
        delete SysFlushPanelName;
    };

    void AddSysErr(AnsiString ErrorCode, AnsiString FlushPanel)
    {
        SystemErrorCode->Add(ErrorCode);
        SysFlushPanelName->Add(FlushPanel);
    };
    void Clear()
    {
        SystemErrorCode->Clear();
        SysFlushPanelName->Clear();
    }

};
//---------------------------------------------------------------------------
class TfNote : public TForm
{
__published:    // IDE-managed Components
    TTimer *Timer1;
    TALed *NullLed;
    TLabel *Label1;
    TMemo *Memo1;
    TPanel *Panel1;
    TBtnPanel *BtnSkip;
    TBtnPanel *BtnRetry;
    TBtnPanel *BtnTrayFeed;
    TBtnPanel *BtnTrayEnd;
    TBtnPanel *BtnCleanOut;
    TBtnPanel *BtnStart;
    TBtnPanel *BtnPause;
    TBtnPanel *BtnOffBuzzer;
    TTMyTray *NullTray;
    TPanel *Panel7;
    TEdit *edtAlarmMsg;
    TEdit *edtAlarmCode;
    TLabel *Label2;
    TPanel *PanelMain6;
    TPanel *palSaveDoor1;
    TPanel *palSaveDoor2;
    TPanel *palSaveDoor3;
    TPanel *palSaveDoor4;
    TPanel *palSaveDoor5;
    TPanel *palSaveDoor6;
    TPanel *palSaveDoor7;
    TPanel *palSaveDoor8;
    TPanel *palSaveDoor9;
    TPanel *palSaveDoor10;
    TPanel *palEmg4;
    TPanel *palEmg1;
    TPanel *palEmg2;
    TPanel *palEmg3;
    TPanel *palVisionErr;
    TLabel *labCCDStep;
    TPanel *Panel6;
    TPanel *Panel11;
    TPanel *Panel3;
    TPanel *Panel14;
    TPanel *Panel20;
    TPanel *pnlAuto1PathR;
    TPanel *palWorkEmpty1;
    TTMyTray *mtWorkEmpty1;
    TPanel *palEmpty1Car;
    TTMyTray *mtEmptyCar;
    TPanel *pnlAuto1PathL;
    TPanel *Panel2;
    TPanel *palWorkLoader;
    TTMyTray *mtWorkLoader;
    TPanel *palLoadCar;
    TTMyTray *mtLoader;
    TPanel *Panel5;
    TPanel *palAuto11;
    TTMyTray *mtAuto11;
    TPanel *palAuto12;
    TTMyTray *mtAuto12;
    TPanel *palAuto13;
    TTMyTray *mtAuto13;
    TPanel *palAuto14;
    TTMyTray *mtAuto14;
    TPanel *palAuto15;
    TTMyTray *mtAuto15;
    TPanel *palAuto7;
    TTMyTray *mtAuto7;
    TPanel *palAuto8;
    TTMyTray *mtAuto8;
    TPanel *palAuto9;
    TTMyTray *mtAuto9;
    TPanel *palAuto10;
    TTMyTray *mtAuto10;
    TPanel *palAuto2;
    TTMyTray *mtAuto2;
    TPanel *palAuto3;
    TTMyTray *mtAuto3;
    TPanel *palAuto4;
    TTMyTray *mtAuto4;
    TPanel *palAuto5;
    TTMyTray *mtAuto5;
    TPanel *palAuto6;
    TTMyTray *mtAuto6;
    TPanel *palAuto1;
    TTMyTray *mtAuto1;
    TPanel *palLoadCarID;
    TPanel *Panel29;
    TPanel *Panel31;
    TPanel *palEmpty2Car;
    TTMyTray *mtMagEmptyCar;
    TPanel *Panel34;
    TPanel *palMagLD;
    TTMyTray *mtMagLD;
    TPanel *Panel35;
    TLabel *labAuto20;
    TLabel *labAuto19;
    TLabel *labAuto18;
    TLabel *labAuto17;
    TLabel *labAuto16;
    TPanel *palAutoStore20;
    TPanel *palAutoStore19;
    TPanel *palAutoStore18;
    TPanel *palAutoStore17;
    TPanel *palAutoStore16;
    TPanel *Panel36;
    TLabel *labAuto15;
    TLabel *labAuto14;
    TLabel *labAuto13;
    TLabel *labAuto12;
    TLabel *labAuto11;
    TPanel *palAutoStore15;
    TPanel *palAutoStore14;
    TPanel *palAutoStore13;
    TPanel *palAutoStore12;
    TPanel *palAutoStore11;
    TPanel *Panel37;
    TLabel *labAuto10;
    TLabel *labAuto09;
    TLabel *labAuto08;
    TLabel *labAuto07;
    TLabel *labAuto06;
    TPanel *palAutoStore10;
    TPanel *palAutoStore09;
    TPanel *palAutoStore08;
    TPanel *palAutoStore07;
    TPanel *palAutoStore06;
    TPanel *Panel39;
    TLabel *labAuto05;
    TLabel *labAuto04;
    TLabel *labAuto03;
    TLabel *labAuto02;
    TLabel *labAuto01;
    TPanel *palAutoStore05;
    TPanel *palAutoStore04;
    TPanel *palAutoStore03;
    TPanel *palAutoStore02;
    TPanel *palAutoStore01;
    TPanel *Panel67;
    TPanel *palMag3;
    TTMyTray *mtMag3;
    TPanel *palMag1;
    TTMyTray *mtMag1;
    TPanel *palMag2;
    TTMyTray *mtMag2;
    TPanel *palTrayArm;
    TPanel *pnlSortArm1_X;
    TPanel *pnlSortArm1_Y;
    TPanel *palSortArm1;
    TMyLed *ledSortArm1ZA;
    TMyLed *ledSortArm1ZB;
    TMyLed *ledSortArm1ZC;
    TMyLed *ledSortArm1ZD;
    TMyLed *ledSortArm1ZE;
    TMyLed *ledSortArm1ZF;
    TMyLed *ledSortArm1ZG;
    TMyLed *ledSortArm1ZH;
    TPanel *pnlSortArm2_Y;
    TPanel *pnlSortArm2_X;
    TPanel *palSortArm2;
    TMyLed *ledSortArm2ZA;
    TMyLed *ledSortArm2ZC;
    TMyLed *ledSortArm2ZE;
    TMyLed *ledSortArm2ZG;
    TMyLed *ledSortArm2ZB;
    TMyLed *ledSortArm2ZD;
    TMyLed *ledSortArm2ZF;
    TMyLed *ledSortArm2ZH;
    TPanel *palWorkEmpty2;
    TTMyTray *mtWorkEmpty2;
    TPanel *palWorkEmpty3;
    TTMyTray *mtWorkEmpty3;
    TPanel *palWorkEmpty4;
    TTMyTray *mtWorkEmpty4;
    TPanel *Panel30;
    TPanel *palMagEmptyTray;
    TTMyTray *mtMagEmptyTray;
    TPanel *palMagEmptyID;
    TPanel *Panel71;
    TPanel *Panel72;
    TMyLed *MyLed5;
    TPanel *palShiftStage;
    TTMyTray *mtShiftStage;
    TPanel *palTrack1TrayArm;
    TTMyTray *mtTrack1TrayArm;
    TPanel *palTrack2TrayArm;
    TTMyTray *mtTrack2TrayArm;
    TPanel *palTrack3TrayArm;
    TTMyTray *mtTrack3TrayArm;
    TPanel *palMagTrayArm;
    TPanel *palTrack4TrayArm;
    TTMyTray *mtTrack4TrayArm;
    TPanel *palAuto20;
    TTMyTray *mtAuto20;
    TPanel *palAuto19;
    TTMyTray *mtAuto19;
    TPanel *palAuto18;
    TTMyTray *mtAuto18;
    TPanel *palAuto17;
    TTMyTray *mtAuto17;
    TPanel *palAuto16;
    TTMyTray *mtAuto16;
    TPanel *palWorkEmpty1ID;
    TPanel *palSysErr;
    TBtnPanel *BtnHome;
    TPanel *palNoticeEmptyTray;
    TLabel *Label12;
    TLabel *Label13;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BtnStartClick(TObject *Sender);
    void __fastcall BtnPauseClick(TObject *Sender);
        void __fastcall BtnOffBuzzerClick(TObject *Sender);
        void __fastcall BtnSkipClick(TObject *Sender);
private:    // User declarations
    bool bHasChangeNewTray;
public:     // User declarations
    __fastcall TfNote(TComponent* Owner);
    int Code;
    int ReturnCode;
    bool fShow;
    int KeyCode;
    bool Select[6];
    void __fastcall FlushLabel();
    void __fastcall ScanKey();
    int iBackOldMemo2Y;
    int iBackMemo2Height;
    bool fMemoPos;
    TTMyTray *trayPtr;
    AnsiString sObjName;

    void __fastcall ProcessErrMessage(AnsiString EC,AnsiString Str,int Type);
//    void __fastcall GetEventID();
//    void __fastcall UpdateAlarmPassTime();
//    bool __fastcall CheckCodeIsExist(AnsiString Str);
    void __fastcall Start();
    void __fastcall UpdateButtonStatus(TObject *Sender);
    void __fastcall LevelProcessErrMessage();

    void GetFlushPanel(TWinControl *PCtrl, AnsiString PanelName);
    MyNoteStruct *SystemError;
    TColor FlushPanelColor;

    void ChangePalPos(TPanel *Panel, int Height, int Left, int Top, int Width, bool Visible=true);
};
//---------------------------------------------------------------------------
extern PACKAGE TfNote *fNote;
//---------------------------------------------------------------------------
int  ShowSystemError(AnsiString Name, int KCode,int iDuplicate=0, AnsiString Message="");
int  ShowSuckError(TMyKitSuck &Ptr, int CodeType, int KCode, AnsiString errPart, int iDuplicate=0);
void ShowMotorError(AnsiString Code,AnsiString sFunc);
//void ShowCylinderError(int Code);
void __fastcall MyDBIProcess(AnsiString asTable, AnsiString S, AnsiString subMess=" ");
void RecordProcess(AnsiString S);
void LevelRecordProcess();
//extern AnsiString   RecordLastHappenTimeString,    RecordLastHappenErrorCode;
extern DWORD        RecordHappenTime;
AnsiString GetRefrenceCode(AnsiString S);
#endif
