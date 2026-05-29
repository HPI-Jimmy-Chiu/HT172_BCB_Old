//---------------------------------------------------------------------------
#ifndef BarcodeH
#define BarcodeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "FSMRunner.h"
#include "MyMotor.h"
//---------------------------------------------------------------------------
class PACKAGE TMyCCDIDPanel : public TComponent
{
private:    // User declarations
    bool bTimerRun;
    int  iConnectCount;

    //FSM 
public:
    __fastcall TMyCCDIDPanel(TComponent* Owner, AnsiString _name, int _Tag);
    __fastcall ~TMyCCDIDPanel();
    TTimer *TimerCCDConnect;
    void __fastcall TimerDownCCDConnectTimer(TObject *Sender);
    TGroupBox *gbClipID;
    TLabel *labAddr;
    TLabel *labPort;
    TLabel *labMax;
    TLabel *labMin;
    TLabel *labErr;
    TEdit *edAddr;
    TEdit *edPort;
    TEdit *edCmd;
    TEdit *edMax2DLen;
    TEdit *edMin2DLen;
    TEdit *edErr2DStr;
    TButton *btConnect;
    TButton *btDisConnect;
    TButton *btSend;
    TCheckBox *cbEnable;
    TMemo *meLog;
    TClientSocket *cSocketClipID;
    int iMyTag;
    AnsiString sName;
    bool bEnableCCD;

    void __fastcall edPortClick(TObject *Sender);
    void __fastcall edAddrClick(TObject *Sender);
    void __fastcall edErr2DClick(TObject *Sender);
    void __fastcall edMax2DClick(TObject *Sender);
    void __fastcall edMin2DClick(TObject *Sender);
    void __fastcall ClientSocket_Connect(TObject *Sender, TCustomWinSocket *Socket);
    void __fastcall ClientSocket_Disconnect(TObject *Sender, TCustomWinSocket *Socket);
    void __fastcall ClientSocket_Error(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
    void __fastcall ClientSocket_Read(TObject *Sender, TCustomWinSocket *Socket);
    void __fastcall BtConnectClick(TObject *Sender);
    void __fastcall BtDisconnectClick(TObject *Sender);
    void __fastcall BtTriggerClick(TObject *Sender);
    void SendCCDCommand(AnsiString Str1, AnsiString Str2);
    void AddCCDCommunicationLog(AnsiString Str);
    void __fastcall TimerCCDConnectTimer(TObject *Sender);

    //FSM
    FSM_RESULT FSM_PhotoInit();
    FSM_RESULT FSM_LightOn();
    FSM_RESULT FSM_MoveToMin();
    FSM_RESULT FSM_MoveToMax();
    FSM_RESULT FSM_WaitPhoto();
    FSM_RESULT FSM_PhotoFinish();
    FSM_RESULT DoExecCCDStep(int iAct);
    FSM_RESULT Update();
    FSMRunner m_FMSRunner;
    AnsiString sClipID;
    void SetCCDArm(TTrayMotor *ccdarmX){CCDArmX=ccdarmX;bCanMove=true;}
    void SetLimit(int imin,int imax)
    {
        iMin=imin;
        iMax=imax;
    }
    bool bCanMove;
    TTrayMotor *CCDArmX;
    int iMin;
    int iMax;
    bool bDoWaitPhoto;
    TQPF_Timer WaitPhotoIdle;
};
//---------------------------------------------------------------------------
class Tfrm2DID : public TForm
{
__published:    // IDE-managed Components
    TScrollBox *scrlbxCCD;
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall Tfrm2DID(TComponent* Owner);
    TMyCCDIDPanel *MyCCD[eLoaderTotal];
    void Update2DParameter();
    void AlignGUI(bool bShowOnSetup);
    void Initialization();
    void StartCCDAuto_Loader();
    void UpdateAuto_Loader();
    void StartCCDAuto(int iArea);
    bool UpdateCCDAuto(int iArea);
    AnsiString GetBarcode(int iArea);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm2DID *frm2DID;
//---------------------------------------------------------------------------

#endif

