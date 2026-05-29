//---------------------------------------------------------------------------
#ifndef languageH
#define languageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfLan : public TForm
{
__published:    // IDE-managed Components
    TTable *Table1;
    TSpeedButton *SpeedButton1;
private:    // User declarations
public:     // User declarations
    __fastcall TfLan(TComponent* Owner);
    bool __fastcall WriteToCompomentDatabase(AnsiString S,AnsiString S1);
    void __fastcall SaveBinaryTreeCompoment(TWinControl *PCtrl);
    void __fastcall LoadBinaryTreeCompoment(TWinControl *PCtrl);
    void __fastcall LoadLanguageFromDataBase();
    bool __fastcall ReadFromList(AnsiString Name,char *str,char *str2);
    void __fastcall ShowCompomentName(TWinControl *PCtrl);
    void __fastcall BeginChange(int Type);
    void __fastcall EndChange();
    void __fastcall ChangeLanguage(TForm *P);
    void __fastcall BeginUpdate();
    void __fastcall EndUpdate();
    void __fastcall UpdateLanguageToDataBase(TForm *P);
    AnsiString LanguageFormName;
    int LanguageType;
    bool ShowHint;

};
//---------------------------------------------------------------------------
extern PACKAGE TfLan *fLan;
//---------------------------------------------------------------------------
#endif
