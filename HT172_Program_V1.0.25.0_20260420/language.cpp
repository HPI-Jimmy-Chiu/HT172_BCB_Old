//---------------------------------------------------------------------------
#include <vcl.h>
#include <comctrls.hpp>
#pragma hdrstop

#include "language.h"
#include "cprod.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfLan *fLan;
TStringList *TempList[6]; // for chinese and english use
TStringList *STempList[6]; // for chinese and english use

//---------------------------------------------------------------------------
__fastcall TfLan::TfLan(TComponent* Owner)
    : TForm(Owner)
{
    LanguageType=0;
    ShowHint=false;
    Table1->DatabaseName=ALIAS;
}
//---------------------------------------------------------------------------
bool __fastcall TfLan::WriteToCompomentDatabase(AnsiString S,AnsiString S1)
{
    int i,j;
    for(i=0;i<TempList[0]->Count;i++)
    {
        if( LanguageFormName==TempList[0]->Strings[i] && TempList[1]->Strings[i]==S)
        {
            TempList[2]->Strings[i]=S1;
            for(j=0;j<6;j++)
            {
                STempList[j]->Add( TempList[j]->Strings[i] );
                TempList[j]->Delete(i);
            }
            return false;
        }
    }
    STempList[0]->Add(LanguageFormName);
    STempList[1]->Add(S);
    STempList[2]->Add(S1);
    STempList[3]->Add("");
    STempList[4]->Add("");
    STempList[5]->Add("");
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfLan::SaveBinaryTreeCompoment(TWinControl *PCtrl)
{
    AnsiString S="",S1="";
    bool flag;

    // 先將主物件的資料寫入 ......................
    TPanel    *PanelPtr  = dynamic_cast <TPanel *>   (PCtrl);
    TTabSheet *ShtPtr    = dynamic_cast <TTabSheet *>(PCtrl);
    TForm     *FrmPtr    = dynamic_cast <TForm *>    (PCtrl);
    TGroupBox *PGroupBox = dynamic_cast <TGroupBox *>(PCtrl);
    TRadioGroup *RadGup  = dynamic_cast <TRadioGroup *>(PCtrl);
    if (PanelPtr != NULL)
        WriteToCompomentDatabase(PanelPtr->Name,PanelPtr->Caption);
    else if (ShtPtr != NULL)
        WriteToCompomentDatabase(ShtPtr->Name,ShtPtr->Caption);
    else if (FrmPtr != NULL)
        WriteToCompomentDatabase(FrmPtr->Name,FrmPtr->Caption);
    else if (PGroupBox  != NULL)
        WriteToCompomentDatabase(PGroupBox->Name,PGroupBox->Caption);
    else if (RadGup  != NULL)
        WriteToCompomentDatabase(RadGup->Name,RadGup->Caption);

    // 再將附屬物件的資料寫入 .....................
    for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
    {
        TControl *P = PCtrl->Controls[iP];
        if (dynamic_cast <TPanel *>(P) != NULL          ||     // 有子節點
            dynamic_cast <TPageControl *>(P) != NULL    ||
            dynamic_cast <TTabSheet *>(P) != NULL       ||
            dynamic_cast <TTabControl *>(P) != NULL     ||
            dynamic_cast <TForm *>(P) != NULL           ||
            dynamic_cast <THeader *>(P) != NULL         ||
            dynamic_cast <TPage *>(P) != NULL           ||
            dynamic_cast <TGroupBox*>(P) != NULL        ||
            dynamic_cast <TScrollBox*>(P) != NULL       ||
            dynamic_cast <TRadioGroup*>(P) != NULL
            )
        {
            SaveBinaryTreeCompoment((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
        }
        TLabel *PLabel               = dynamic_cast <TLabel *>(P);
        TSpeedButton    *PSpeedButton = dynamic_cast <TSpeedButton *>(P);
        TButton         *PButton      = dynamic_cast <TButton *>(P);
        TCheckBox       *PCheckBox    = dynamic_cast <TCheckBox *>(P);
        TRadioButton    *PRadioButton = dynamic_cast <TRadioButton *>(P);
        if (PLabel != NULL)                 // 如果是TLabel型態
            WriteToCompomentDatabase(PLabel->Name,PLabel->Caption);
        else if (PSpeedButton != NULL)           // 如果是TSpeedButton型態
            WriteToCompomentDatabase(PSpeedButton->Name,PSpeedButton->Caption);
        else if (PButton != NULL)                // 如果是TButton型態
            WriteToCompomentDatabase(PButton->Name,PButton->Caption);
        else if (PCheckBox != NULL)              // 如果是TCheckBox型態
            WriteToCompomentDatabase(PCheckBox->Name,PCheckBox->Caption);
        else if (PRadioButton != NULL)           // 如果是TRadioButton型態
        {
            S=PRadioButton->Name;
            if (RadGup  != NULL)            // 其父屬為 TRadioButton
            {
                S=RadGup->Name;
                S=S+AnsiString("_SUB_")+AnsiString(iP);
            }
            WriteToCompomentDatabase(S,PRadioButton->Caption);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLan::LoadBinaryTreeCompoment(TWinControl *PCtrl)
{
    AnsiString S="",S1="";
//    static ct=0;
    char str[256]="",str2[256]="";
    bool flag;
    // 先將主物件的資料寫入 ......................
    TPanel    *PanelPtr  = dynamic_cast <TPanel *>   (PCtrl);
    TTabSheet *ShtPtr    = dynamic_cast <TTabSheet *>(PCtrl);
    TForm     *FrmPtr    = dynamic_cast <TForm *>    (PCtrl);
    TGroupBox *PGroupBox = dynamic_cast <TGroupBox *>(PCtrl);
//    TScrollBox *ScrBox   = dynamic_cast <TScrollBox *>(PCtrl);
    TRadioGroup *RadGup  = dynamic_cast <TRadioGroup *>(PCtrl);
    if (PanelPtr != NULL){
        if( ReadFromList(PanelPtr->Name,str,str2) )
            PanelPtr->Caption=str;
    }
    else if (ShtPtr != NULL)
    {
        if( ReadFromList(ShtPtr->Name,str,str2) )
            ShtPtr->Caption=str;
    }
    else if (FrmPtr != NULL)
    {
        if( ReadFromList(FrmPtr->Name,str,str2) )
            FrmPtr->Caption=str;
    }
    else if (PGroupBox  != NULL)
    {
        if( ReadFromList(PGroupBox->Name,str,str2) )
            PGroupBox->Caption=str;
    }
    else if (RadGup  != NULL)
    {
        if( ReadFromList(RadGup->Name,str,str2) )
            RadGup->Caption=str;
    }
    // 再將附屬物件的資料寫入 .....................
    for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
    {
        TControl *P = PCtrl->Controls[iP];
        if (dynamic_cast <TPanel *>(P) != NULL          ||
            dynamic_cast <TPageControl *>(P) != NULL    ||
            dynamic_cast <TTabSheet *>(P) != NULL       ||
            dynamic_cast <TTabControl *>(P) != NULL     ||
            dynamic_cast <TForm *>(P) != NULL           ||
            dynamic_cast <THeader *>(P) != NULL         ||
            dynamic_cast <TPage *>(P) != NULL           ||
            dynamic_cast <TGroupBox*>(P) != NULL        ||
            dynamic_cast <TScrollBox*>(P) != NULL       ||
            dynamic_cast <TRadioGroup*>(P) != NULL  )
        {
            LoadBinaryTreeCompoment((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
        }
        TLabel *PLabel               = dynamic_cast <TLabel *>(P);
        TSpeedButton    *PSpeedButton = dynamic_cast <TSpeedButton *>(P);
        TButton         *PButton      = dynamic_cast <TButton *>(P);
        TCheckBox       *PCheckBox    = dynamic_cast <TCheckBox *>(P);
        TRadioButton    *PRadioButton = dynamic_cast <TRadioButton *>(P);
        if (PLabel != NULL)                 // 如果是TLabel型態
        {
            if( ReadFromList(PLabel->Name,str,str2) )
                PLabel->Caption=str;


            if(ShowHint)
            {
                PLabel->Hint=str2;
                if( strlen(str2) ) //has Hint string
                    PLabel->ShowHint=true;
                else
                    PLabel->ShowHint=false;
            }
        }
        else if (PSpeedButton != NULL)           // 如果是TSpeedButton型態
        {
            if( ReadFromList(PSpeedButton->Name,str,str2) )
                PSpeedButton->Caption=str;
        }
        else if (PButton != NULL)                // 如果是TButton型態
        {
            if( ReadFromList(PButton->Name,str,str2) )
                PButton->Caption=str;
        }
        else if (PCheckBox != NULL)              // 如果是TCheckBox型態
        {
            if( ReadFromList(PCheckBox->Name,str,str2) )
                PCheckBox->Caption=str;
        }
        else if (PRadioButton != NULL)           // 如果是TRadioButton型態
        {
            S=PRadioButton->Name;
            if (RadGup  != NULL)            // 其父屬為 TRadioButton
            {
                S=RadGup->Name;
                S=S+AnsiString("_SUB_")+AnsiString(iP);
            }
            if( ReadFromList(S,str,str2) )
                PRadioButton->Caption=str;
        }
    }
}
//----------------------------------------------------------------
void __fastcall TfLan::ShowCompomentName(TWinControl *PCtrl)
{
    AnsiString S="",S1="";
//    static ct=0;
    char str[256],str2[256];
    bool flag;
    TPanel    *PanelPtr  = dynamic_cast <TPanel *>   (PCtrl);
    TTabSheet *ShtPtr    = dynamic_cast <TTabSheet *>(PCtrl);
    TForm     *FrmPtr    = dynamic_cast <TForm *>    (PCtrl);
    TGroupBox *PGroupBox = dynamic_cast <TGroupBox *>(PCtrl);
//    TScrollBox *ScrBox   = dynamic_cast <TScrollBox *>(PCtrl);
    TRadioGroup *RadGup  = dynamic_cast <TRadioGroup *>(PCtrl);
    if (PanelPtr != NULL)
    {
        PanelPtr->Caption=PanelPtr->Name;
        PanelPtr->Font->Name="MS Sans Serif";
        PanelPtr->Font->Size=9;
    }
    else if (ShtPtr != NULL)
    {
        ShtPtr->Caption=ShtPtr->Name;
        ShtPtr->Font->Size=9;
        ShtPtr->Font->Name="MS Sans Serif";
    }
    else if (FrmPtr != NULL)
    {
        FrmPtr->Caption=FrmPtr->Name;
        FrmPtr->Font->Size=9;
        FrmPtr->Font->Name="MS Sans Serif";
    }
    else if (PGroupBox  != NULL)
    {
        PGroupBox->Caption=PGroupBox->Name;
        PGroupBox->Font->Size=9;
        PGroupBox->Font->Name="MS Sans Serif";
    }
    else if (RadGup  != NULL)
    {
        RadGup->Caption=RadGup->Name;
        RadGup->Font->Size=9;
        RadGup->Font->Name="MS Sans Serif";
    }
    for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
    {
        TControl *P = PCtrl->Controls[iP];
        if (dynamic_cast <TPanel *>(P) != NULL          ||
            dynamic_cast <TPageControl *>(P) != NULL    ||
            dynamic_cast <TTabSheet *>(P) != NULL       ||
            dynamic_cast <TTabControl *>(P) != NULL     ||
            dynamic_cast <TForm *>(P) != NULL           ||
            dynamic_cast <THeader *>(P) != NULL         ||
            dynamic_cast <TPage *>(P) != NULL           ||
            dynamic_cast <TGroupBox*>(P) != NULL        ||
            dynamic_cast <TScrollBox*>(P) != NULL       ||
            dynamic_cast <TRadioGroup*>(P) != NULL  )
        {
            ShowCompomentName((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
        }
        TLabel *PLabel               = dynamic_cast <TLabel *>(P);
        TSpeedButton    *PSpeedButton = dynamic_cast <TSpeedButton *>(P);
        TButton         *PButton      = dynamic_cast <TButton *>(P);
        TCheckBox       *PCheckBox    = dynamic_cast <TCheckBox *>(P);
        TRadioButton    *PRadioButton = dynamic_cast <TRadioButton *>(P);
//        TCustomEdit     *PCustomEdit  = dynamic_cast <TCustomEdit *>(P);
        TComboBox       *PComboBox    = dynamic_cast <TComboBox *>(P);
        if (PLabel != NULL)                 // 如果是TLabel型態
        {
            PLabel->Caption=PLabel->Name;
            PLabel->Font->Size=9;
            PLabel->Font->Name="MS Sans Serif";
        }
        else if (PSpeedButton != NULL)           // 如果是TSpeedButton型態
        {
            PSpeedButton->Caption=PSpeedButton->Name;
            PSpeedButton->Font->Size=9;
            PSpeedButton->Font->Name="MS Sans Serif";
        }
        else if (PButton != NULL)                // 如果是TButton型態
        {
            PButton->Caption=PButton->Name;
            PButton->Font->Size=9;
            PButton->Font->Name="MS Sans Serif";
        }
        else if (PCheckBox != NULL)              // 如果是TCheckBox型態
        {
            PCheckBox->Caption=PCheckBox->Name;
            PCheckBox->Font->Size=9;
            PCheckBox->Font->Name="MS Sans Serif";
        }
        else if (PRadioButton != NULL)           // 如果是TRadioButton型態
        {
            S=PRadioButton->Name;
            if (RadGup  != NULL)            // 其父屬為 TRadioButton
            {
                S=RadGup->Name;
                S=S+AnsiString("_SUB_")+AnsiString(iP);
            }
            PRadioButton->Caption=9;
            PRadioButton->Font->Size=8;
            PRadioButton->Font->Name="MS Sans Serif";
        }
        //else if (PCustomEdit != NULL)            // 如果是TCustomEdit型態
        //{
        //    PCustomEdit->Text=PCustomEdit->Name;
        //}
        else if (PComboBox!=NULL)
        {
            PComboBox->Text=PComboBox->Name;
            PComboBox->Font->Name="MS Sans Serif";
        }
    }
}
bool IsMainFormNotChange(AnsiString S)
{
    //#define MaxMainFormNotChangeItem 63
    int MaxMainFormNotChangeItem;
    int i;
    char str[][20]={
    {"Panel42"},        {"Edit39"},         {"Edit40"},         {"Panel43"},      {"lbSite0"},
    {"lbSite1"},        {"lbSite2"},        {"lbSite3"},        {"lbSite4"},      {"lbSite5"},
    {"lbSite6"},        {"lbSite7"},        {"lbSite00"},       {"lbSite11"},     {"lbSite22"},
    {"lbSite33"},       {"lbSite44"},       {"lbSite55"},       {"lbSite66"},     {"lbSite77"},
    {"Panel46"},        {"Panel44"},        {"Panel6"},         {"edBinResult0"}, {"edBinResult1"},
    {"edBinResult2"},   {"edBinResult3"},   {"edBinResult4"},   {"edBinResult5"}, {"edBinResult6"},
    {"edBinResult7"},   {"grpResult0"},     {"grpResult1"},     {"grpResult2"},   {"grpResult3"},
    {"grpResult4"},     {"grpResult5"},     {"grpResult6"},     {"grpResult7"},   {"edBinAuto1"},
    {"edBinAuto2"},     {"edBinAuto3"},     {"edBinFix1"},      {"edBinFix2"},    {"edBinFix3"},
    {"edBinFix4"},      {"edBinFix5"},      {"edBinFix6"},      {"edScanerPass"}, {"edScanerInvalid"},
    {"edScanerFailure"},{"edScanerRework"}, {"Panel50"},        {"Panel52"},      {"palOutputAuto1"},
    {"palOutputAuto2"}, {"palOutputAuto3"}, {"palOutputFix1"},  {"palOutputFix2"},{"palOutputFix3"},
    {"spbFan"},         {"Label5"}  ,       {"SpeedButton7"} ,  {"panTempName0"} ,{"panTempName1"} ,
    {"panTempName2"} ,  {"panTempName3"} ,  {"panTempName4"} ,  {"panTempName5"} ,{"panTempName6"} ,
    {"panTempName7"} ,  {"panTempName8"} ,  {"panTempName9"},
    {"lbSLT00"},        {"lbSLT01"},        {"lbSLT02"},        {"lbSLT03"},      {"lbSLT04"},
    {"lbSLT05"},        {"lbSLT06"},        {"lbSLT07"},        {"lbSLT08"},      {"lbSLT09"},
    {"edSLT00"},        {"edSLT01"},        {"edSLT02"},        {"edSLT03"},      {"edSLT04"},
    {"edSLT05"},        {"edSLT06"},        {"edSLT07"},        {"edSLT08"},      {"edSLT09"}




    };
    MaxMainFormNotChangeItem=sizeof(str)/20;
    for(i=0;i<MaxMainFormNotChangeItem;i++)
        if( S==AnsiString(str[i]) )
            return true;
    return false;
}

void __fastcall TfLan::LoadLanguageFromDataBase()
{
    int i;
    TTable *T;
    T=Table1;
    T->First();
    while(!T->Eof)
    {
        if( T->Fields->Fields[0]->AsString=="fMain" && IsMainFormNotChange( T->Fields->Fields[1]->AsString ) )
        {

        }
        else
        {
            for(i=0;i<6;i++)
                TempList[i]->Add(T->Fields->Fields[i]->AsString);
        }
        T->Next();
    }
}
//----------------------------------------------------------------
bool __fastcall TfLan::ReadFromList(AnsiString Name,char *str,char *str2)
{
    int i,j;
    if(Name=="")
    {
        strcpy(str2,"");
        return false;
    }
    for(i=0;i<TempList[0]->Count;i++)
    {
        if( LanguageFormName == TempList[0]->Strings[i] &&// which form
            Name             == TempList[1]->Strings[i] )        // compoment name
        {
            if(LanguageType)
            {
                strcpy(str ,TempList[3]->Strings[i].c_str() );
                strcpy(str2,TempList[5]->Strings[i].c_str() );
            }
            else
            {
                strcpy(str ,TempList[2]->Strings[i].c_str() );
                strcpy(str2,TempList[4]->Strings[i].c_str() );
            }
            for(j=0;j<6;j++)
                TempList[j]->Delete(i);
            if( strlen(str) )    return true;
            else                 return false;
        }
    }
    return false;
}
//----------------------------------------------------------------
void __fastcall TfLan::BeginChange(int Type)
{
    int i;
    LanguageType=Type;
    for(i=0;i<6;i++)
        TempList[i]= new TStringList;
    Table1->Active=true;
    LoadLanguageFromDataBase();
    Table1->Active=false;
}
//----------------------------------------------------------------
void __fastcall TfLan::EndChange()
{
    int i;
    for(i=0;i<6;i++)
        delete TempList[i];
}
//----------------------------------------------------------------

void __fastcall TfLan::ChangeLanguage(TForm *P)
{
    LanguageFormName=P->Name;
    LoadBinaryTreeCompoment(P);
}
//----------------------------------------------------------------
void __fastcall TfLan::BeginUpdate()
{
    int i;
    for(i=0;i<6;i++)
    {
        TempList[i]= new TStringList;
        STempList[i]= new TStringList;
    }
    Table1->Active=true;
    LoadLanguageFromDataBase();
    Table1->Active=false;
}
void __fastcall TfLan::EndUpdate()
{
    int i,j;
    Table1->Active=true;
    Table1->First();
    while(!Table1->Eof) // delete all data on database
    {
        Table1->Edit();
        Table1->Delete();
    }
    for(i=0;i<STempList[0]->Count;i++)
    {
        Table1->Last();
        Table1->Append();
        for(j=0;j<6;j++)
            Table1->Fields->Fields[j]->AsString=STempList[j]->Strings[i];
    }
    for(i=0;i<6;i++)
    {
        delete TempList[i];
        delete STempList[i];
    }
    Table1->Post();
    Table1->Active=false;
}

void __fastcall TfLan::UpdateLanguageToDataBase(TForm *P)
{
    LanguageFormName=P->Name;
    SaveBinaryTreeCompoment(P);
}



