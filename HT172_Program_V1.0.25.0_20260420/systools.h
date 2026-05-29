//---------------------------------------------------------------------------

#ifndef systoolsH
#define systoolsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <vcl/inifiles.hpp>
#include <Sysutils.hpp>
#include <map>
#include "mysensor.h"
using namespace std;
//---------------------------------------------------------------------------
typedef struct RecordAlarmMessageStruct
{
    AnsiString Date;
    AnsiString Time;
    AnsiString AlarmCode;
    AnsiString Message;
    AnsiString ErrorPart;
    AnsiString Recovery;
    int PauseTime;
    AnsiString Duplicate;
    void AddRecord(AnsiString Code, AnsiString Mess)
    {
        Date.sprintf("%04d\\%02d\\%02d", SystemYear, SystemMonth, SystemDate);
        Time.sprintf("%02d:%02d:%02d", SystemHour, SystemMin, SystemSec);
        AlarmCode=Code;
        Message=Mess;
        Recovery=" ";
        PauseTime=0;
        Duplicate=" ";
    };
    void AddRecord(AnsiString UnitName, AnsiString Mess, AnsiString _ErrorPart)
    {
        Date.sprintf("%04d-%02d-%02d", SystemYear, SystemMonth, SystemDate);
        Time.sprintf("%02d:%02d:%02d", SystemHour, SystemMin, SystemSec);
        AlarmCode=UnitName;
        Message=Mess;
        Recovery=" ";
        PauseTime=0;
        Duplicate=" ";
        ErrorPart=_ErrorPart;
    };
    void SetProcessData(AnsiString KCode, int iPauseTime, int iDuplicate)
    {
        if(KCode!="")
            Recovery=KCode;

        PauseTime=iPauseTime;

        if(iDuplicate!=0)
            Duplicate="Yes";
        else
            Duplicate=" ";
    };

    AnsiString CSVString()
    {
        TStringList *List=new TStringList();                                    //Steven 20240728 : event log改成逗號分隔
        AnsiString Str;
        List->Add(Date);
        List->Add(Time);
        List->Add(Recovery);
        List->Add(PauseTime);
        List->Add(Duplicate);
        List->Add(AlarmCode);
        List->Add(Message);
        List->Add(ErrorPart);
        Str=List->CommaText;
        List->Clear();
        delete List;
        return Str;
    };

}MyRecordAlarmMessage;
//---------------------------------------------------------------------------
typedef struct MyLevelSetupPopMenuBackupStruct
{
    TControl *PCtrl;
    TPopupMenu *PopupMenuPtr;
}LevelSetupPopMenuBackup;
//---------------------------------------------------------------------------
class TAlarm                                                                    //Stevenhong 20251231 : 力成要把JAM獨立額外生成
{
public: 
    int AlarmCode;
    AnsiString Category;
    int AlarmType;
    AnsiString E_ErrMessage;
    AnsiString C_ErrMessage;
    AnsiString E_Description;
    AnsiString C_Description;
    TAlarm() : AlarmCode(0), AlarmType(0) {}
};
//---------------------------------------------------------------------------
class TAlarmManager                                                             //Stevenhong 20251231 : 力成要把JAM獨立額外生成
{
private:
    std::vector<TAlarm> FAlarmList;
    void ParseCSVLine(AnsiString Line, TAlarm &AlarmObj);
public:
    TAlarmManager(); // 建構子
    ~TAlarmManager(); // 解構子
    bool LoadFromFile(AnsiString FileName);
    bool LoadFromFile();
    int GetCount();
    void Clear();
    TAlarm* FindByCode(int Code);
    bool IsJamAlarm(int Code);
    AnsiString sJamTableFile(){return "D:\\HT172\\system\\JamAlarmList.csv";}
};
//---------------------------------------------------------------------------
class TFormSysTools : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TPopupMenu *PopupMenu1;
    TTimer *Timer1;
    TSpeedButton *SpeedButton3;
    TGroupBox *GroupBox1;
    TMemo *Memo1;
    TButton *Button1;
    TComboBox *ComboBox1;
    TButton *Button4;
    TMemo *Memo2;

    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall SpeedButton3Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations

    // 設定原件可視不可視,中英文等等變數與函式
    bool   bSetUserAccessLevel;
    TList  *LevelSetupList;
    TList  *LevelSetupPopupMenuBackupList;
    void BackupPopupMenu(TControl *PWinCtrl,TPopupMenu *PCtrl);
    void SetAndBackPopMenu(TWinControl *PCtrl,TPopupMenu *PopCtrl);
    void AddSetLevelPopMenu(TForm *FormPtr);
    template <class TPanel2> void MyTemp(LevelSetupPopMenuBackup *P,TPanel2 *P2,TControl *PCtrl);
    void SetLevel(AnsiString FormName,AnsiString BaseName,TControl *P);
    AnsiString TaskString;
    TIniFile *INIFile ;
    TCustomMemo *Logs;
    void RefreshMyShow();
    TList *ShowList;
    TList *ShowTimeStringList;
    TList *NoNeedHomeCheckList;
    void RefreshMyTimeString();
    void RefreshNoNeedHomeCheckList();
    void __fastcall SetComponentFont(AnsiString FormName,AnsiString BaseName,TControl *TempCtrl); //20090120 terry//
    void __fastcall InsertAlarmCodeToDB(eAlarmType AlarmType, AnsiString sCode, AnsiString sEngMessage, AnsiString sChiMessage, AnsiString PanelName, AnsiString sEngDescription, AnsiString sChiDescription);
    void __fastcall InsertProcessCodeToDB(AnsiString sMessage);
    int iProcessCodeIndex;
public:     // User declarations
    __fastcall TFormSysTools(TComponent* Owner);
    // 設定原件可視不可視,中英文等等變數與函式
    void InitialSetLevelPopMenuList();
    void RestoreSetLevelPopMenu();
    void __fastcall SetSubPlaneAsNoFrame(TWinControl *PCtrl);                   //20111014 terry//
    void GetAndSetLevel(AnsiString FormName,TWinControl *PCtrl);
    void __fastcall OpenFormData(AnsiString Filename);
    void __fastcall CloseFormData();
    void __fastcall CheckLevelSetup();
    void __fastcall UpdateLevelSetupMap(AnsiString FormName,AnsiString TypeName,AnsiString Value);
    void __fastcall LoadFormData(TWinControl *PCtrl,AnsiString GruppName, bool bLoadLed=false);
    void __fastcall SaveFormData(TWinControl *PCtrl,AnsiString GruppName, bool bLoadLed=false);
    void TerminalProgram();
    void WriteLogs(AnsiString S);
    void SetLogsMemo(TCustomMemo *P);

        // 用來更改畫面,
    void AddMyShow(long *iRefValue,TObject *RefPalnelPtr);
    void AddMyShow(int *iRefValue,TObject *RefPalnelPtr);
    void AddMyTimeStringShow(TObject *T,int DataType);
    void HookKeyboard();
    void UnHookKeyboard();
    void __fastcall CreateNewJamErrorTable();
    void __fastcall CreateMotorAlarmCode        ();
    void __fastcall CreateCylinderAlarmCode     ();
    void __fastcall CreateSystemSensorAlarmCode ();
    void __fastcall CreateSuckAlarmCode         ();
    void __fastcall CreateSystemAlarmCode       ();
    void __fastcall RecordAlarmMessage( MyRecordAlarmMessage R);
    void AddNoNeedHomeCheckList(TMySensor *RefSensor);
    bool CheckPassword(int iInLevel, AnsiString asInUserName, AnsiString asInPassword); //Daver add
    bool bInsertToDB;
    TAlarmManager tJamAlarm;
//    void SaveLogRpt(AnsiString sLogPath,AnsiString sLogName, AnsiString sMsg);
    void SaveLogRpt(AnsiString sLogPath,AnsiString sLogName,AnsiString sDefHead, AnsiString sMsg);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSysTools *FormSysTools;

//---------------------------------------------------------------------------
#endif
