//---------------------------------------------------------------------------
#ifndef maintenanceH
#define maintenanceH
//---------------------------------------------------------------------------
#include "ALed.hpp"
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfMaintenance : public TForm
{
__published:    // IDE-managed Components
    TPopupMenu *PopupMenu1;
    TMenuItem *Delete1;
    TColorDialog *ColorDialog1;
    TPanel *Panel3;
    TSpeedButton *spbPassword;
    TSpeedButton *spbSoftSimu;
    TSpeedButton *spbTrayDef;
    TSpeedButton *spbExit;
    TSpeedButton *spbAlignment;
    TSpeedButton *spbMotorTest;
    TSpeedButton *spbFunctionDef;
    TSpeedButton *spbComPort;
    TSpeedButton *sbSecsGem;
    TSpeedButton *spbTowerLight;
    TPageControl *pcMaintenance;
    TTabSheet *TabTower;
    TPanel *Panel2;
    TPanel *Panel12;
    TALed *RGB50;
    TALed *RGB40;
    TALed *RGB30;
    TALed *RGB20;
    TALed *RGB10;
    TALed *RGB00;
    TALed *RGB01;
    TALed *RGB11;
    TALed *RGB21;
    TALed *RGB31;
    TALed *RGB41;
    TALed *RGB51;
    TALed *RGB52;
    TALed *RGB42;
    TALed *RGB32;
    TALed *RGB22;
    TALed *RGB12;
    TALed *RGB02;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label6;
    TLabel *Label7;
    TBevel *Bevel1;
    TBevel *Bevel2;
    TBevel *Bevel3;
    TBevel *Bevel4;
    TLabel *Label43;
    TLabel *Label17;
    TPanel *Panel13;
    TPanel *Panel14;
    TPanel *Panel15;
    TPanel *Panel16;
    TPanel *Panel17;
    TPanel *Panel18;
    TRadioGroup *RadioGroup2;
    TRadioGroup *RadioGroup3;
    TRadioGroup *RadioGroup4;
    TRadioGroup *RadioGroup5;
    TRadioGroup *RadioGroup6;
    TRadioGroup *RadioGroup7;
    TPanel *Panel19;
    TSpeedButton *sbMusic1;
    TSpeedButton *sbMusic2;
    TSpeedButton *sbMusic3;
    TSpeedButton *sbMusic4;
    TTabSheet *TabPassword;
    TPanel *Panel1;
    TSpeedButton *SpeedButton21;
    TSpeedButton *sbDeletePassword;
    TPanel *Panel20;
    TLabel *Label18;
    TSpeedButton *SpeedButton22;
    TSpeedButton *SpeedButton23;
    TLabel *Label19;
    TRadioButton *RadioButton1;
    TRadioButton *RadioButton2;
    TMaskEdit *MaskEdit1;
    TEdit *Edit14;
    TCheckListBox *CheckListBox1;
    TPanel *Panel6;
    TTabSheet *TabSoft;
    TPanel *Panel7;
    TPageControl *pgcSoftSimu;
    TTabSheet *TabSheet13;
    TStringGrid *StringGrid2;
    TTabSheet *TabFunctionDefine;
    TPanel *Panel28;
    TPageControl *pgcFunction;
    TTabSheet *tsA00;
    TTabSheet *TabTrayDB;
    TTabSheet *tsN00;
    TGroupBox *gbN01;
    TSpeedButton *spbIoMonitor;
    TPanel *pnlTray;
    TSpeedButton *btnAddTray;
    TSpeedButton *btnDeleteTray;
    TSpeedButton *btnModifyTray;
    TSpeedButton *sbUpdateTray;
    TSpeedButton *sbtReloadTray;
    TPanel *palTrayDef;
    TStringGrid *strngrdTray;
    TEdit *edtTemp;
    TTabSheet *tsT00;
    TLabeledEdit *edN01_MachineID;
    TLabeledEdit *edN01_FactoryID;
    TLabeledEdit *edSerialNo;
    TGroupBox *gbN02;
    TLabel *labPath;
    TLabel *lab;
    TLabel *labPWD;
    TLabel *Label9;
    TEdit *ed_N02_Host;
    TEdit *edN02_ID;
    TEdit *ed_N02_PWD;
    TEdit *ed_N02_Path;
    TEdit *ed_N02_Path_Upload;
    TTabSheet *tsS00;
    TPanel *pnlT001;
    TGroupBox *gbT01UseVib;
    TCheckBox *cbT01_AutoRow1Vib2;
    TCheckBox *cbT01_AutoRow2Vib2;
    TCheckBox *cbT01_AutoRow2Vib1;
    TCheckBox *cbT01_AutoRow1Vib1;
    TCheckBox *cbT01_AutoRow3Vib2;
    TCheckBox *cbT01_AutoRow4Vib2;
    TCheckBox *cbT01_AutoRow4Vib1;
    TCheckBox *cbT01_AutoRow3Vib1;
    TRadioGroup *rgT02_MagTraySource;
    TPanel *pnlT002;
    TRadioGroup *rgT04_BinDisptype;
    TPanel *pnlS001;
    TGroupBox *gbS01UseSpeed;
    TCheckBox *cbS01_MagX;
    TEdit *edS01_MagX;
    TCheckBox *cbS01_SortArmTrayZ;
    TEdit *edS01_SortArmTrayZ;
    TGroupBox *grpS02;
    TCheckBox *chkS02;
    TGroupBox *grpS03;
    TCheckBox *chkS03_MagArmXYPreMove;
    TCheckBox *chkS03_ShortDistanceMove;
    TGroupBox *gbN03;
    TCheckBox *cbN03;
    TEdit *ed_N03_Path_Upload;
    TLabel *Label10;
    TLabel *labN03;
    TRadioGroup *rgT03;
    TRadioGroup *rgT03_AutoPickMode;
    TPanel *Panel4;
    TGroupBox *GroupBox1;
    TCheckBox *cbN04;
    TGroupBox *GroupBox2;
    TEdit *edRunCheckTimeout;
    TPanel *Panel5;
    TRadioGroup *rgA01_InputDataFormat;
    TGroupBox *gbT05_FillTrayAtCleanOut;
    TCheckBox *cbT05_FillTrayAtCleanOut;
    TRadioGroup *rgT06_AutoRowType;
    TRadioGroup *rgT08_Empty1PushTray;
    TGroupBox *GroupBox3;
    TLabeledEdit *edN03_IP;
    TLabeledEdit *edN03_Port;
    TCheckBox *cbN03_Log;
    TLabeledEdit *edN03_LotStartPatch;
    TLabeledEdit *edN03_LotEndPatch;
    TRadioGroup *rgN02;
    TRadioGroup *rgT05_ComportHardware;
    TRadioGroup *rgT07_MGZDirection;
    TRadioGroup *rgT09_TrayIDReadPos;
    TTabSheet *tsP00;
    TGroupBox *gbEnableArea;
    TGroupBox *gbAreaSetting;
    TButton *btnP_PassAll;
    TPanel *Panel8;
    TCheckBox *cbA02;
    TEdit *edA02;
    TPanel *Panel9;
    TCheckBox *cbA03; //JerryYang 20250621 : Auto跟Magazine使用不同COM Port
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall spbMotorTestClick(TObject *Sender);
    void __fastcall edA18_DelayTimeChange(TObject *Sender);
    void __fastcall spbExitClick(TObject *Sender);
    void __fastcall spbTowerLightClick(TObject *Sender);
    void __fastcall spbIoMonitorClick(TObject *Sender);
    void __fastcall spbAlignmentClick(TObject *Sender);
    void __fastcall CheckListBox1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton21Click(TObject *Sender);
    void __fastcall SpeedButton22Click(TObject *Sender);
    void __fastcall SpeedButton23Click(TObject *Sender);
        void __fastcall sbDeletePasswordClick(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall RGB00Click(TObject *Sender);
        void __fastcall sbMusic1Click(TObject *Sender);
        void __fastcall DataSource3DataChange(TObject *Sender,
          TField *Field);
        void __fastcall FormCreate(TObject *Sender);
    void __fastcall sbSecsGemClick(TObject *Sender);
    void __fastcall edA01_CCDDown_IPMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edA01_CCDDown_PortMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edA04MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edA08_SetPersentSpeedMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Panel18MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Panel17MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall spbComPortClick(TObject *Sender);
    void __fastcall strngrdTraySelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall strngrdTrayDblClick(TObject *Sender);
    void __fastcall btnModifyTrayClick(TObject *Sender);
    void __fastcall btnAddTrayClick(TObject *Sender);
    void __fastcall btnDeleteTrayClick(TObject *Sender);
    void __fastcall sbUpdateTrayClick(TObject *Sender);
    void __fastcall sbtReloadTrayClick(TObject *Sender);
    void __fastcall edS01_MagXMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnP_PassAllClick(TObject *Sender);
    void __fastcall edA02MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:    // User declarations
    void __fastcall ShowMotorSimulateSpeed();
    int iTmpSelectLevel;
public:     // User declarations
    __fastcall TfMaintenance(TComponent* Owner);
    void __fastcall ShowMotorSelect(int Index,int Attr);
    bool fShow;

    bool fFunctionProcess;
    int CatchFrom;
    int CatchTo;
    int CheckPtrItem;
    int RowPoint;
//    void DisableVisibleAllTabSheet(TTabSheet *Sender);
    TSpeedButton *LastClickButton;
    void __fastcall SaveWorkFile(AnsiString S);
    void __fastcall OpenWorkFile();
    //--- Daver Add - s //
    void __fastcall ShowPassword();
    void __fastcall AppendPassword(AnsiString asID, AnsiString asPassword, int iLevel);
    void __fastcall DeletePassword();
    //--- Daver Add - e //
    void BackUpOutputData();                                                    // ben add 20110803 //
    void RestoreOutputData();                                                   // ben add 20110803 //
    void SaveData();
    int iSelTrayRow;
    int iSelTrayCol;
    int GetRunCheckTimeout();
    bool EnableUnloaderTrayDataFilePath();
    AnsiString GetUnloaderTrayDataFilePath();
    TCheckBox *cbEnableArea[eTrayCount];                                        //Jimmychiu 20251021 : enable pass area
    void InitEnableArea();
    void InitAreaPass();
    void CloseAreaPass(TCheckBox *cbarea);
    void CtrlAllAreaPass(bool bOn);
};
//---------------------------------------------------------------------------
extern PACKAGE TfMaintenance *fMaintenance;
//---------------------------------------------------------------------------
#endif
