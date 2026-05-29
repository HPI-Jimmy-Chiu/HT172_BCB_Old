//------------------------------------------------------------------------------
#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

//------------------------------------------------------------------------------
#include "LevelSetup.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfLevelSetup *fLevelSetup;
//==============================================================================
__fastcall TfLevelSetup::TfLevelSetup(TComponent* Owner)
    : TForm(Owner)
{
}
//==============================================================================
void __fastcall TfLevelSetup::SetLevel1Click(TObject *Sender)
{
    AnsiString S;
    TComponent *P;

    P=SetLevelPopupMenu->PopupComponent;
    PCtrl=(TControl *)P;

    BaseName=P->Name;
    while(1)
    {
        if( P->HasParent()==false )
        {
            FormName=P->Name;
            break;
        }
        else
        {
            P=P->GetParentComponent();
        }
    }
    ReadComponentFont(PCtrl);
    ShowModal();
}
//============================================================================== //20100120 terry start//
// 讀取原件字型、大小、顏色
//==============================================================================
void __fastcall TfLevelSetup::ReadComponentFont(TControl *TempCtrl)
{
    TPanel *PanelPtr              = dynamic_cast <TPanel *>   (TempCtrl);
    TEdit *EditPtr                = dynamic_cast <TEdit *>(TempCtrl);
    TLabeledEdit *LEditPtr        = dynamic_cast <TLabeledEdit *>(TempCtrl);
    TLabel *LabelPtr              = dynamic_cast <TLabel *>(TempCtrl);
    TSpeedButton *SpeedBtnPtr     = dynamic_cast <TSpeedButton *>(TempCtrl);
    TRadioGroup *RadGup           = dynamic_cast <TRadioGroup *>(TempCtrl);
    TForm *FrmPtr                 = dynamic_cast <TForm *>    (TempCtrl);
    TTabSheet *ShtPtr             = dynamic_cast <TTabSheet *>(TempCtrl);
    TGroupBox *PGroupBox          = dynamic_cast <TGroupBox *>(TempCtrl);
    TComboBox *ComboBoxPtr        = dynamic_cast <TComboBox *>(TempCtrl);
    TRadioButton *RadioButtonPtr  = dynamic_cast <TRadioButton *>(TempCtrl);
    TCheckBox *CheckPtr           = dynamic_cast <TCheckBox *>(TempCtrl);
    TButton *ButtonPtr            = dynamic_cast <TButton *>(TempCtrl);

    if(PanelPtr != NULL)
    {
        asFontName=PanelPtr->Font->Name;
        iFontSize=PanelPtr->Font->Size;
        iFontColor=PanelPtr->Font->Color;
    }
    else if(EditPtr!=NULL)
    {
        asFontName=EditPtr->Font->Name;
        iFontSize=EditPtr->Font->Size;
        iFontColor=EditPtr->Font->Color;
    }
    else if(LEditPtr!=NULL)
    {
        asFontName=LEditPtr->Font->Name;
        iFontSize=LEditPtr->Font->Size;
        iFontColor=LEditPtr->Font->Color;
    }
    else if(LabelPtr != NULL)
    {
        asFontName=LabelPtr->Font->Name;
        iFontSize=LabelPtr->Font->Size;
        iFontColor=LabelPtr->Font->Color;
    }
    else if(SpeedBtnPtr != NULL)
    {
        asFontName=SpeedBtnPtr->Font->Name;
        iFontSize=SpeedBtnPtr->Font->Size;
        iFontColor=SpeedBtnPtr->Font->Color;
    }
    else if(RadGup != NULL)
    {
        asFontName=RadGup->Font->Name;
        iFontSize=RadGup->Font->Size;
        iFontColor=RadGup->Font->Color;
    }
    else if(FrmPtr != NULL)
    {
        asFontName=FrmPtr->Font->Name;
        iFontSize=FrmPtr->Font->Size;
        iFontColor=FrmPtr->Font->Color;
    }
    else if(ShtPtr != NULL)
    {
        asFontName=ShtPtr->Font->Name;
        iFontSize=ShtPtr->Font->Size;
        iFontColor=ShtPtr->Font->Color;
    }
    else if(PGroupBox != NULL)
    {
        asFontName=PGroupBox->Font->Name;
        iFontSize=PGroupBox->Font->Size;
        iFontColor=PGroupBox->Font->Color;
    }
    else if(ComboBoxPtr != NULL)
    {
        asFontName=ComboBoxPtr->Font->Name;
        iFontSize=ComboBoxPtr->Font->Size;
        iFontColor=ComboBoxPtr->Font->Color;
    }
    else if(RadioButtonPtr != NULL)
    {
        asFontName=RadioButtonPtr->Font->Name;
        iFontSize=RadioButtonPtr->Font->Size;
        iFontColor=RadioButtonPtr->Font->Color;
    }
    else if(CheckPtr != NULL)
    {
        asFontName=CheckPtr->Font->Name;
        iFontSize=CheckPtr->Font->Size;
        iFontColor=CheckPtr->Font->Color;
    }
    else if(ButtonPtr != NULL)
    {
        asFontName=ButtonPtr->Font->Name;
        iFontSize=ButtonPtr->Font->Size;
        iFontColor=ButtonPtr->Font->Color;
    }
    else
    {
        asFontName="9999";
        iFontSize=9999;
        iFontColor=9999;
    }

    if(iFontColor==-2147483640)                                                 //-2147483640 is default value//
        iFontColor=0;
}
//==============================================================================  //20100120 terry end//
void __fastcall TfLevelSetup::WriteIni(char *group, char *cName,int Value)
{
    TIniFile *INIFile = new TIniFile(HSys.CurrentDir+"\\system\\levelsetup.ini");
    INIFile->WriteInteger(group, cName, Value);
    delete INIFile;
}
//==============================================================================
void __fastcall TfLevelSetup::WriteIni(char *group, char *cName,AnsiString Value)
{
    TIniFile *INIFile = new TIniFile(HSys.CurrentDir+"\\system\\levelsetup.ini");
    INIFile->WriteString(group, cName, Value);
    delete INIFile;
}
//==============================================================================
int __fastcall TfLevelSetup:: ReadIni(char *group,char *cName,int Value)
{
    int ret;
    TIniFile *INIFile = new TIniFile(HSys.CurrentDir+"\\system\\levelsetup.ini");
    ret=INIFile->ReadInteger(group,cName,Value);
    delete INIFile;
    return ret;
}
//==============================================================================
AnsiString  __fastcall TfLevelSetup:: ReadIni(char *group,char *cName,AnsiString Value)
{
    AnsiString ret;
    TIniFile *INIFile = new TIniFile(HSys.CurrentDir+"\\system\\levelsetup.ini");
    ret=INIFile->ReadString(group,cName,Value);
    delete INIFile;
    return ret;
}
//==============================================================================
void __fastcall TfLevelSetup::FormShow(TObject *Sender)
{
    int ret;
    AnsiString S,S1,S2;
    Left=(1280-Width)/2;
//    Top =(1024-Height)/2;
    Top =0;
    S=BaseName;

    S1=S+"_Enable";
    ret=ReadIni(FormName.c_str(),S1.c_str(),9999);
    if( ret==9999 || ret<0 || ret>=5 )
        RadioGroup1->ItemIndex=-1;
    else
        RadioGroup1->ItemIndex=ret;

    S1=S+"_Visible";
    ret=ReadIni(FormName.c_str(),S1.c_str(),9999);
    if( ret==9999 || ret<0 || ret>=5 )
        RadioGroup2->ItemIndex=-1;
    else
        RadioGroup2->ItemIndex=ret;

    S1=S+"_EnableAtRun";
    ret=ReadIni(FormName.c_str(),S1.c_str(),9999);
    if( ret==9999 || ret<0 || ret>=3 )
        RadioGroup3->ItemIndex=-1;
    else
        RadioGroup3->ItemIndex=ret;

    S1=S+"_VisibleAtRun";
    ret=ReadIni(FormName.c_str(),S1.c_str(),9999);
    if( ret==9999 || ret<0 || ret>=3 )
        RadioGroup4->ItemIndex=-1;
    else
        RadioGroup4->ItemIndex=ret;

    S1=S+"_EnableAtHot";
    ret=ReadIni(FormName.c_str(),S1.c_str(),9999);
    if( ret==9999 || ret<0 || ret>=3 )
        RadioGroup5->ItemIndex=-1;
    else
        RadioGroup5->ItemIndex=ret;

    S1=S+"_VisibleAtHot";
    ret=ReadIni(FormName.c_str(),S1.c_str(),9999);
    if( ret==9999 || ret<0 || ret>=3 )
        RadioGroup6->ItemIndex=-1;
    else
        RadioGroup6->ItemIndex=ret;

    char str[256];
    S1=S+"_English";
    PCtrl->GetTextBuf(str,64);
    S2=ReadIni(FormName.c_str(),S1.c_str(),str);
    Edit1->Text=S2;

    S1=S+"_Chinese";
    S2=ReadIni(FormName.c_str(),S1.c_str(),str);
    Edit2->Text=S2;

    S1=S+"_Font";                                                               //20100120 terry start//
    S2=ReadIni(FormName.c_str(),S1.c_str(),asFontName);
    edFont->Text=S2;

    S1=S+"_Size";
    S2=ReadIni(FormName.c_str(),S1.c_str(),iFontSize);
    edSize->Text=S2;

    S1=S+"_Color";
    S2=ReadIni(FormName.c_str(),S1.c_str(),iFontColor);
    edColor->Text=S2;                                                           //20100120 terry end//
}
//==============================================================================//20100120 terry start//
void __fastcall TfLevelSetup::edFontClick(TObject *Sender)
{
    if(asFontName!="9999")
        FontDialog1->Font->Name=asFontName;
    if(iFontSize!=9999)
        FontDialog1->Font->Size=iFontSize;
    if(iFontColor!=9999)
        FontDialog1->Font->Color=TColor(iFontColor);                            //V1.2
//        FontDialog1->Font->Color=iFontColor;                                  //V1.1
    if(FontDialog1->Execute())
    {
        edFont->Text=FontDialog1->Font->Name;
        edSize->Text=FontDialog1->Font->Size;
        edColor->Text=FontDialog1->Font->Color;
    }
}
//==============================================================================//20100120 terry end//
void __fastcall TfLevelSetup::BitBtn1Click(TObject *Sender)
{
    AnsiString S,S1;
    S=BaseName;
    S1=S+"_Enable";
    WriteIni(FormName.c_str(),S1.c_str(),RadioGroup1->ItemIndex);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,AnsiString(RadioGroup1->ItemIndex));   // map ben add 20110728 //

    S1=S+"_Visible";
    WriteIni(FormName.c_str(),S1.c_str(),RadioGroup2->ItemIndex);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,AnsiString(RadioGroup2->ItemIndex));   // map ben add 20110728 //

    S1=S+"_EnableAtRun";
    WriteIni(FormName.c_str(),S1.c_str(),RadioGroup3->ItemIndex);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,AnsiString(RadioGroup3->ItemIndex));   // map ben add 20110728 //

    S1=S+"_VisibleAtRun";
    WriteIni(FormName.c_str(),S1.c_str(),RadioGroup4->ItemIndex);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,AnsiString(RadioGroup4->ItemIndex));   // map ben add 20110728 //

    S1=S+"_EnableAtHot";
    WriteIni(FormName.c_str(),S1.c_str(),RadioGroup5->ItemIndex);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,AnsiString(RadioGroup5->ItemIndex));   // map ben add 20110728 //

    S1=S+"_VisibleAtHot";
    WriteIni(FormName.c_str(),S1.c_str(),RadioGroup6->ItemIndex);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,AnsiString(RadioGroup6->ItemIndex));   // map ben add 20110728 //

    S1=S+"_English";
    WriteIni(FormName.c_str(),S1.c_str(),Edit1->Text);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,Edit1->Text);                          // map ben add 20110728 //

    S1=S+"_Chinese";
    WriteIni(FormName.c_str(),S1.c_str(),Edit2->Text);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,Edit2->Text);                          // map ben add 20110728 //

    S1=S+"_Font";                                                               //20100120 terry start//
    WriteIni(FormName.c_str(),S1.c_str(),edFont->Text);
    FormSysTools->UpdateLevelSetupMap(FormName,S1,edFont->Text);                         // map ben add 20110728 //

    S1=S+"_Size";
    WriteIni(FormName.c_str(),S1.c_str(), atoi(edSize->Text.c_str()));
    FormSysTools->UpdateLevelSetupMap(FormName,S1,edSize->Text);                         // map ben add 20110728 //

    S1=S+"_Color";
    WriteIni(FormName.c_str(),S1.c_str(), atoi(edColor->Text.c_str()));                //20100120 terry end//
    FormSysTools->UpdateLevelSetupMap(FormName,S1,edColor->Text);                        // map ben add 20110728 //
}
//==============================================================================
void __fastcall TfLevelSetup::BitBtn2Click(TObject *Sender)
{
    Close();
}
//==============================================================================
void __fastcall TfLevelSetup::BitBtn3Click(TObject *Sender)
{
    BitBtn1Click(this);
    Close();
}
//==============================================================================

