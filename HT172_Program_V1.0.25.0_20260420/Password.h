//---------------------------------------------------------------------------
#ifndef PasswordH
#define PasswordH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfPassword : public TForm
{
__published:    // IDE-managed Components
    TEdit *Edit1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Edit2;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TComboBox *ComboBox1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall Edit2KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ComboBox1Change(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfPassword(TComponent* Owner);
    bool bShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfPassword *fPassword;
//---------------------------------------------------------------------------
#endif
