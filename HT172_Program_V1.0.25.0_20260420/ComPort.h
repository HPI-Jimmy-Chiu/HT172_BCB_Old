//---------------------------------------------------------------------------

#ifndef ComPortH
#define ComPortH
//---------------------------------------------------------------------------
#include "SPComm.hpp"
#include "MyComm.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include "ALed.hpp"
#include "BtnPanelLane.h"
#include "MyLed.h"
#include <vector>

using namespace std;
//---------------------------------------------------------------------------
class MotorTorque
{
    private:
        int     iErrorCount;
        bool    bNeedReadSetTorque;
        bool    bNeedWriteSetTorque;
        bool    bReadingCurrent;
        int     iReadCurrTorqueTask;
        int     iReadSetTorqueTask;
        int     iWriteSetTorqueTask;
        int     iMotToqrueTask;
        int     ptreot;
        int     ptrenq;

        bool DoWritePanasonicA5SetTorque(bool Reset=false);
        bool DoReadPanasonicA5SetTorque (bool Reset=false);
        bool DoReadPanasonicA5CurrTorque(bool Reset=false);
        void __fastcall TorqueSend(unsigned char *str, int len, AnsiString Task);
    public:
        MotorTorque(int iTag, TTrayMotor *Mot, TScrollBox *SB);
//        ~MotorTorque();
        TComm       *MotorComm;
        TPanel      *palMotorCom;
        TPanel      *palMotorSetting;
        TLabel      *labMotorCom;
        TLabel      *labMotorComPort;
        TLabel      *labTorque;
        TMemo       *memoMotorCom;
        TButton     *btnWriteSet;
        TButton     *btnReadSet;
        TButton     *btnReadCurrent;
        TEdit       *edSetTorque;
        TComboBox   *rgMotorComPort;
        bool    bNeedReadCurrTorque;    //Steven 20130905

        int     Tag;
        int     iMotTag;
        unsigned int  iSettingTorque;
        double  dCurrentTorque;
        double  dReadingTorque;

        TQPF_Timer MotTorqueDelay;

        bool    bEnable;

        bool    bReceiveData;
        unsigned char cSendStr;
        unsigned char cRecvStr;

        AnsiString asMotorName;

        bool    bReadCurrTorqueOK;
        bool    bReadSetTorqueOK;
        bool    bWriteSetTorqueOK;

        void WritePanasonicA5Parameter(unsigned char Command, unsigned Data);
        void ReadPanasonicA5Parameter(unsigned char Command);
        void __fastcall MotorComReceiveData(TObject *Sender, Pointer Buffer, WORD BufferLength);
        void __fastcall btnWriteSetOnClick(TObject *Sender);
        void __fastcall btnReadSetOnClick(TObject *Sender);
        void __fastcall btnReadCurrentOnClick(TObject *Sender);
        void __fastcall DoReadTorque(int iMotorIndex, bool Reset=false);
        void AddMessage(AnsiString Message);
        void ResetMotTorqueFlag();
};

//---------------------------------------------------------------------------
class TfComPort : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pgComPort;
    TPanel *Panel1;
    TSpeedButton *sbExit;
    TSpeedButton *sbUpdate;
    TSpeedButton *spbResetCom;
    TTabSheet *tsPanel;
    TPanel *palPanelCom;
    TLabel *labPanelCom;
    TPanel *palPanelSetting;
    TLabel *Label19;
    TComboBox *cbPadComm;
    TMemo *memoPanelCom;
    TButton *sbPanelSend_Com;
    TEdit *edPanelSend_Com;
    TComm *PadComm;
    TTabSheet *tsBin;
    TPanel *palBinCom;
    TLabel *labBinCom;
    TPanel *palBinSetting;
    TLabel *Label15;
    TComboBox *cbBinComm;
    TMemo *memoBinCom;
    TLabeledEdit *labedtNumDelay;
    TCheckBox *cbBinCheckLog;
    TPanel *palBinTest;
    TTabSheet *tsStepTray;
    TPanel *Panel13;
    TLabel *Label14;
    TPanel *Panel14;
    TLabel *Label20;
    TComboBox *cbStepTrayComm;
    TButton *btnStepTrayMotSend;
    TEdit *Edit2;
    TMemo *memoStepMot;
    TComm *StepTrayComm;
    TLabel *Label21;
    TComboBox *cbMagBinComm;
    TPageControl *PageControl1;
    TTabSheet *tsNormal;
    TTabSheet *tsTFT;
    TLabeledEdit *lbledtBinUnit;
    TLabeledEdit *lbledtBin;
    TPanel *plLeftNormal;
    TGroupBox *GroupBox1;
    TButton *btnStart;
    TButton *btnBinSetTest;
    TPanel *Panel2;
    TGroupBox *GroupBox2;
    TButton *btnReadVersion;
    TButton *btnSetBinFontNum;
    TButton *btnSetBinFontBin;
    TButton *btnSetBinFontEA;
    TButton *btnTFTSetCount;
    TButton *btnTFTSetBackGround;
    TButton *btnSetBin_TFT_bin;
    TButton *btnWriteScreenNoBackGround_TFT;
    TPanel *Panel3;
    TGroupBox *GroupBox3;
    TButton *btnSetBin_TFT_Count;
    TButton *btnSetBin_TFT_EA;
    TButton *Button3;
    TButton *Button4;
    TButton *Button5;
    TButton *Button6;
    TButton *Button7;
    TButton *Button8;
    TButton *btnDipComStart;
    TButton *btnDipComStop;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall sbExitClick(TObject *Sender);
    void __fastcall spbResetComClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall PadCommReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall sbPanelSend_ComClick(TObject *Sender);
    void __fastcall btnStartClick(TObject *Sender);
    void __fastcall btnBinSetTestClick(TObject *Sender);
    void __fastcall StepTrayCommReceiveData(TObject *Sender,
          Pointer Buffer, WORD BufferLength);
    void __fastcall btnStepTrayMotSendClick(TObject *Sender);
    void __fastcall btnReadVersionClick(TObject *Sender);
    void __fastcall btnSetBinFontNumClick(TObject *Sender);
    void __fastcall btnSetBinFontBinClick(TObject *Sender);
    void __fastcall btnSetBinFontEAClick(TObject *Sender);
    void __fastcall btnTFTSetCountClick(TObject *Sender);
    void __fastcall btnTFTSetBackGroundClick(TObject *Sender);
    void __fastcall btnSetBin_TFT_binClick(TObject *Sender);
    void __fastcall btnWriteScreenNoBackGround_TFTClick(TObject *Sender);
    void __fastcall btnSetBin_TFT_EAClick(TObject *Sender);
    void __fastcall btnSetBin_TFT_CountClick(TObject *Sender);
    void __fastcall btnDipComStartClick(TObject *Sender);
    void __fastcall btnDipComStopClick(TObject *Sender);
private:    // User declarations

public:     // User declarations
    __fastcall TfComPort(TComponent* Owner);
    TMyComm *commBinDisStore;   // Phase1: Bin display (TFT) - self-built TMyComm
    TMyComm *commBinDisMag;     // Phase1: MGZ Bin display (TFT)
    void __fastcall SaveWorkFile();
    void __fastcall OpenWorkFile();

    void __fastcall RS232Init();
    void StopAllCom();
    void __fastcall RecordCommunication(AnsiString aTitle, AnsiString Command);
    void Spin();
    bool bReceiveData;
    void MemoAddString(TMemo *memo, AnsiString asTitle, AnsiString Str);
    bool bShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfComPort *fComPort;
//---------------------------------------------------------------------------
#endif
