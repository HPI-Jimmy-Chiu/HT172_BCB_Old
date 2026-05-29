//---------------------------------------------------------------------------
#ifndef uspeedH
#define uspeedH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TMySpeedPanel : public TComponent
{
    public:
        __fastcall TMySpeedPanel(TTrayMotor *Mot);
        __fastcall ~TMySpeedPanel();

        TPanel      *palMotorSpeed;
        TLabel      *labMotorSpeed;
        TEdit       *edtMotorSpeed;
        TScrollBar  *scbMotorSpeed;
        int         *iPersent;
        bool        bEnable;

        void __fastcall scbMotorSpeedChange(TObject *Sender);
        void __fastcall edtMotorSpeedClick(TObject *Sender);
        void SetSpeed(int Speed)
        {
            edtMotorSpeed->Text=AnsiString(Speed);
            scbMotorSpeed->Position=Speed;
        };

        void EnableFixSpeed(bool bEnabled, int iSpeed)
        {
            edtMotorSpeed->Enabled=!bEnabled;
            scbMotorSpeed->Enabled=!bEnabled;
            palMotorSpeed->Enabled=!bEnabled;

            if(bEnabled)
                SetSpeed(iSpeed);
        };
};
//---------------------------------------------------------------------------
class TfSpeed : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel10;
    TSpeedButton *spbExit;
    TSpeedButton *spbFullSpeed;
    TSpeedButton *spbAddSpeed;
    TSpeedButton *spbSubSpeed;
    TCheckBox *CheckBox1;
    TScrollBox *ScrollBox1;
    TPanel *palMotorSpeed;
    TLabel *labMotorSpeed;
    TEdit *edtMotorSpeed;
    TScrollBar *scbMotorSpeed;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall spbFullSpeedClick(TObject *Sender);
    void __fastcall spbExitClick(TObject *Sender);
    void __fastcall spbAddSpeedClick(TObject *Sender);
    void __fastcall spbSubSpeedClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:    // User declarations
    enum eSpeedType{eSubSpeed=0, eAddSpeed=1, eFullSpeed=2};

    void __fastcall SaveWorkFile();
    void __fastcall GroupSpeedAddSub(eSpeedType flag);
public:     // User declarations
    __fastcall TfSpeed(TComponent* Owner);
    void __fastcall AddSpeed( TScrollBar *Sender);
    void __fastcall SubSpeed( TScrollBar *Sender);
    void __fastcall OpenWorkFile();
    void SetFixedSpeed();
    bool fShow; // V2.1 START
    vector<TMySpeedPanel *> MySpeedPanel;
    void ResetAllMotorSpeed();
};
//---------------------------------------------------------------------------
extern PACKAGE TfSpeed *fSpeed;
//---------------------------------------------------------------------------
#endif
