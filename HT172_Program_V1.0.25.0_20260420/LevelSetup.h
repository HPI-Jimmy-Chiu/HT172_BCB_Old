//---------------------------------------------------------------------------

#ifndef LevelSetupH
#define LevelSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TfLevelSetup : public TForm
{
__published:    // IDE-managed Components
    TRadioGroup *RadioGroup1;
    TRadioGroup *RadioGroup2;
    TRadioGroup *RadioGroup3;
    TPopupMenu *SetLevelPopupMenu;
    TMenuItem *SetLevel1;
    TRadioGroup *RadioGroup4;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Edit1;
    TEdit *Edit2;
    TRadioGroup *RadioGroup5;
    TRadioGroup *RadioGroup6;
    TGroupBox *GroupBox2;
    TLabel *Label3;
    TLabel *Label4;
    TFontDialog *FontDialog1;
    TEdit *edFont;
    TEdit *edSize;
    TEdit *edColor;
    TLabel *Label5;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
    void __fastcall SetLevel1Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall edFontClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
private:    // User declarations
    AnsiString BaseName;
    AnsiString FormName;
    AnsiString asFontName;
    int iFontSize;
    int iFontColor;
    //TComponent *PCtrl;
    TControl *PCtrl;
    void __fastcall WriteIni(char *group, char *cName,int Value);
    void __fastcall WriteIni(char *group, char *cName,AnsiString Value);
    void __fastcall ReadComponentFont(TControl *TempCtrl);                      //20090120 terry //

public:     // User declarations
    __fastcall TfLevelSetup(TComponent* Owner);
    int __fastcall  ReadIni(char *group,char *cName,int Value)  ;
    AnsiString __fastcall ReadIni(char *group,char *cName,AnsiString Value);
};
//---------------------------------------------------------------------------
extern PACKAGE TfLevelSetup *fLevelSetup;
//---------------------------------------------------------------------------
#endif
