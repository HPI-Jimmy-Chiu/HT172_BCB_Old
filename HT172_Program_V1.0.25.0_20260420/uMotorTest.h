//---------------------------------------------------------------------------
#ifndef uMotorTestH
#define uMotorTestH
//---------------------------------------------------------------------------
#include "ALed.hpp"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

class TfMotorTest : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet6;
    TLabel *Label42;
    TPanel *Panel20;
    TLabel *Label38;
    TScrollBar *ScrollBar1;
    TEdit *edActiveMotorSpeed;
    TTimer *Timer1;
    TTabSheet *TabSheet1;
    TPanel *palMotorSelect;
    TLabel *Label8;
    TLabel *Label23;
    TPanel *palActiveMotor;
    TLabel *Label24;
    TEdit *edActiveMotorPosition;
    TLabel *Label25;
    TEdit *edActiveMotorEncoder;
    TGroupBox *select;
    TRadioButton *radSingle;
    TRadioButton *radMulti;
    TSpeedButton *spbLoop;
    TComboBox *coboWaitTime;
    TLabel *Label27;
    TLabel *lbTimeOfOneTrack;
    TLabel *Label28;
    TLabel *lbAverageLoopTime;
    TLabel *Label1;
    TLabel *lbTotalLoopCount;
    TPanel *Panel4;
    TBitBtn *spbCopyMotorParameter;
    TComboBox *coboSourceMotorIndex;
    TPopupMenu *PopupMenuSoftLimit1;
    TMenuItem *settoSoftLimitP1;
    TMenuItem *settoSoftLimitP2;
    TPopupMenu *PopupMenuSoftLimit2;
    TMenuItem *MenuItem1;
    TMenuItem *MenuItem2;
    TPopupMenu *PopupMenuSpeedSetup;
    TMenuItem *SettoMaxSpeed1;
    TMenuItem *SettoMinSpeed1;
    TMenuItem *SettoMaxHomeSpeed1;
    TMenuItem *SettoMinHomeSpeed1;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TPopupMenu *PopupMenuMotorPositionClear;
    TMenuItem *Clear1;
    TMenuItem *ClearAll1;
    TTabSheet *TabSheet2;
    TStringGrid *MotorInformationGrid;
    TLabel *Label2;
    TLabel *Label6;
    TLabel *Label12;
    TLabel *lbServoMotorCount;
    TLabel *lbStepMotorCount;
    TLabel *lbMotionCardCount;
    TMemo *Memo1;
    TLabel *Label16;
    TLabel *lbReservedMotor;
    TSpeedButton *spbJogP;
    TSpeedButton *spbJogN;
    TSpeedButton *spbMovePositive;
    TSpeedButton *spbMoveNegtive;
    TComboBox *coboMoveInterval;
    TPanel *Panel2;
    TSpeedButton *spbSetPos1;
    TSpeedButton *spbSetPos2;
    TSpeedButton *spbHome;
    TSpeedButton *spbGotoSoftPositive;
    TSpeedButton *spbGotoSoftNegitive;
    TSpeedButton *spbGo;
    TSpeedButton *spbStop;
    TEdit *Edit1;
    TPanel *Panel3;
    TPanel *Panel5;
    TTabSheet *TabSheet3;
    TSpeedButton *SpeedButton7;
    TSpeedButton *SpeedButton8;
    TSpeedButton *SpeedButton9;
    TSpeedButton *SpeedButton10;
    TLabel *Label114;
    TLabel *Label115;
    TLabel *Label116;
    TLabel *Label117;
    TLabel *Label118;
    TLabel *Label119;
    TLabel *Label120;
    TLabel *Label121;
    TLabel *Label122;
    TALed *ledCW;
    TALed *ledHome;
    TALed *ledCCW;
    TALed *ledEmg;
    TALed *ledAlarm;
    TALed *ledSoftCW;
    TALed *ledSoftCCW;
    TALed *ledServoAlarm;
    TALed *ledInPos;
    TPanel *Panel6;
    TSpeedButton *SpeedButton11;
    TSpeedButton *btnReadFromFile;
    TPanel *Panel1;
    TSpeedButton *btnMotorPower;
    TSpeedButton *SpeedButton4;
    TSpeedButton *spbTerminalProgram;
    TLabel *Label13;
    TLabel *Label14;
    TSpeedButton *SpeedButton13;
    TSpeedButton *spbServoOn;
    TLabel *Label15;
    TLabel *Label17;
    TALed *ledZPhase;
    TALed *ledServoOn;
    TStringGrid *strngrdMotorData;
    TSpeedButton *btnAddMotor;
    TSpeedButton *btnModify;
    TSpeedButton *sbtReload;
    TSpeedButton *sbUpdate;
    TLabel *lblMotorName;
    TSpeedButton *btnDeleteMotor;
    TEdit *Edit2;
    TStringGrid *strngrdMotor;
    TSpeedButton *btnSaveToFile;
    TEdit *Edit3;
    TCheckBox *CheckBox1;
    TEdit *Edit4;
    TLabel *Label3;
    void __fastcall lM00Click(TObject *Sender);
    void __fastcall lpA00Click(TObject *Sender);
    void __fastcall ScrollBar1Scroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
    void __fastcall spbJogNMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall spbJogNMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall spbJogPMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall spbJogPMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall spbSetPos1Click(TObject *Sender);
    void __fastcall spbSetPos2Click(TObject *Sender);
    void __fastcall spbGotoSoftPositiveClick(TObject *Sender);
    void __fastcall spbGotoSoftNegitiveClick(TObject *Sender);
    void __fastcall spbHomeClick(TObject *Sender);
    void __fastcall spbMoveNegtiveClick(TObject *Sender);
    void __fastcall spbMovePositiveClick(TObject *Sender);
    void __fastcall spbLoopClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall SpeedButton4Click(TObject *Sender);
    void __fastcall spbGoClick(TObject *Sender);
    void __fastcall btnMotorPowerClick(TObject *Sender);
    void __fastcall spbStopClick(TObject *Sender);
    void __fastcall Clear1Click(TObject *Sender);
    void __fastcall ClearAll1Click(TObject *Sender);
    void __fastcall spbCopyMotorParameterClick(TObject *Sender);
    void __fastcall edActiveMotorSpeedChange(TObject *Sender);
    void __fastcall SpeedButton13Click(TObject *Sender);
    void __fastcall spbServoOnClick(TObject *Sender);
    void __fastcall strngrdMotorDataSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall btnAddMotorClick(TObject *Sender);
    void __fastcall btnDeleteMotorClick(TObject *Sender);
    void __fastcall btnModifyClick(TObject *Sender);
    void __fastcall sbtReloadClick(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall strngrdMotorDataDragDrop(TObject *Sender,
          TObject *Source, int X, int Y);
    void __fastcall strngrdMotorDataDblClick(TObject *Sender);
    void __fastcall strngrdMotorDataDragOver(TObject *Sender,
          TObject *Source, int X, int Y, TDragState State, bool &Accept);
    void __fastcall strngrdMotorDataMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall strngrdMotorSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall btnSaveToFileClick(TObject *Sender);
    void __fastcall btnSaveToFileMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    TCheckBox *AllMotLoopMove[MAX_MOTOR_TEST_COUNT];
    TEdit *AllMotMove1[MAX_MOTOR_TEST_COUNT];
    TEdit *AllMotMove2[MAX_MOTOR_TEST_COUNT];
    bool AllMotMoveFlag[MAX_MOTOR_TEST_COUNT];

    int iSelectRow;
    int iSelectCol;
    int iDragCol;
    int iDragRow;

    int SelMotSpeed;
    bool bStartReadMotorReg;
    bool bStartWriteMotorReg;
    void __fastcall DoReadWriteMotorReg();
    void __fastcall SetCanCommunctionMotorMode(char *MotorName);
    bool bMotorParameterChange;
public:     // User declarations
    __fastcall TfMotorTest(TComponent* Owner);
    int ActiveIndex;
    TLabel *MNameLab[MAX_MOTOR_TEST_COUNT];
    TEdit  *MSpeed[MAX_MOTOR_TEST_COUNT];
    TEdit  *MPos1[MAX_MOTOR_TEST_COUNT];
    TEdit  *MPos2[MAX_MOTOR_TEST_COUNT];
    void __fastcall DoLoopMoveSingle();
    void __fastcall DoLoopMoveMulti();
    void __fastcall DoLoopMove();
    int LoopTask;
    bool fShow;
    void __fastcall UpdateMotorLed();
    void __fastcall ShowMotorSelect(int Index,int Attr);
    bool bSingleHome;
    int  iSingleHomeIndex;
    DWORD dwStartTickCount;
    DWORD dwNowTickCount;
    DWORD dwLoopCount;
    bool __fastcall IsMotorCanRun(bool);
    bool __fastcall DoGaliHome(int ActiveIndex);
    void __fastcall OpenWorkFile();
    void __fastcall SaveWorkFile();
    int  __fastcall FindMotorParameter(AnsiString MotorName);
    int  __fastcall FindMotorParameter(int Index);
    void __fastcall ShowMotorInformation();
    void __fastcall CheckNeedReloadMotorParameter();
//    void __fastcall CheckNeedReloadTechMotorParameter(int index,int speed);

    void __fastcall UpdateMotorParameter();                                     //Steven 20240502 : fixed for motor
    bool bGetDistance;
};
//---------------------------------------------------------------------------
extern PACKAGE TfMotorTest *fMotorTest;
//---------------------------------------------------------------------------
#endif
