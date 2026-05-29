
//==============================================================================
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "Password.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfPassword *fPassword;
//==============================================================================
__fastcall TfPassword::TfPassword(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;
}
//==============================================================================
void __fastcall TfPassword::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;
    Top =(1024-Height)/2;
    Edit1->Text="";
    Edit2->SetFocus();
    bShow=true;
}
//==============================================================================
void __fastcall TfPassword::SpeedButton1Click(TObject *Sender)                  // OK BUTTON //
{
    Close();
}
//==============================================================================
void __fastcall TfPassword::SpeedButton2Click(TObject *Sender)                  // CANCEL BUTTON //
{
    Edit1->Text="";
    Edit2->Text="";
    Close();
}
//==============================================================================
void __fastcall TfPassword::Edit2KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key==0x0d)                                                               // IMPORT ENTER //
        Edit1->SetFocus();
}
//==============================================================================
void __fastcall TfPassword::Edit1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key==0x0d)
        Close();
}
//==============================================================================
void __fastcall TfPassword::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//==============================================================================
void __fastcall TfPassword::ComboBox1Change(TObject *Sender)
{
    Edit2->Text=ComboBox1->Text;
}
//---------------------------------------------------------------------------

