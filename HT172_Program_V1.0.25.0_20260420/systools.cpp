//------------------------------------------------------------------------------
#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

//------------------------------------------------------------------------------
#include "systools.h"
#include "database.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSysTools *FormSysTools;
std::map <AnsiString, AnsiString>mapLevelSetup;                                 //Map structure for LevelSetup ben add //
//==============================================================================
static int iOldAccessLevel=-9999;
static bool bOldSystemStart, iOldTemperature, iOldLanguageCountry;
HHOOK OldHook=NULL;
//==============================================================================
// 鍵盤攔截之call back function
//==============================================================================
long CALLBACK KeyProc(int code, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *pkbhs;
    if(code<0)
    {
        return CallNextHookEx(OldHook, code, wParam, lParam);
    }

    pkbhs=(KBDLLHOOKSTRUCT*) lParam;

    if(pkbhs->vkCode==91 || pkbhs->vkCode==VK_RWIN)                             //攔劫win
    {
        return 1;
    }
    if(pkbhs->vkCode==VK_ESCAPE && GetAsyncKeyState(VK_CONTROL)<0)
    {
        return 1;                                                               //CTRL+ESC
    }
    if(pkbhs->vkCode==VK_TAB && pkbhs->flags&LLKHF_ALTDOWN)
    {
        return 1;                                                               //ALT+TAB
    }
    if(pkbhs->vkCode==115 && pkbhs->flags & LLKHF_ALTDOWN)
    {
        return 1;                                                               //ALT+F4
    }
    if(pkbhs->vkCode==VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
    {
        return 1;                                                               //ALT+ESC
    }
    return CallNextHookEx(OldHook, code, wParam, lParam);
}
//==============================================================================
// 開始鍵盤攔截
//==============================================================================
void TFormSysTools::HookKeyboard()
{
    OldHook=SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyProc, HInstance, 0);
}
//==============================================================================
// 取消鍵盤攔截
//==============================================================================
void TFormSysTools::UnHookKeyboard()
{
    if(OldHook!=NULL)
    {
        UnhookWindowsHookEx(OldHook);
        OldHook=NULL;
    }
}
//==============================================================================
__fastcall TFormSysTools::TFormSysTools(TComponent* Owner)
    : TForm(Owner)
{
    bSetUserAccessLevel=false;
    LevelSetupList=new TList;
    LevelSetupList->Clear();
    LevelSetupPopupMenuBackupList=new TList;
    LevelSetupPopupMenuBackupList->Clear();
    Logs=NULL;
    ShowList=new TList;
    ShowList->Clear();
    ShowTimeStringList=new TList;
    ShowTimeStringList->Clear();
    NoNeedHomeCheckList=new TList;
    NoNeedHomeCheckList->Clear();
    AddNoNeedHomeSensorList();
}
//------------------------------------------------------------------------------
void __fastcall TFormSysTools::FormDestroy(TObject *Sender)
{
    delete LevelSetupPopupMenuBackupList;
    delete LevelSetupList;
    delete ShowList;
    delete ShowTimeStringList;
    delete NoNeedHomeCheckList;
}
//==============================================================================
// 設定原件可視不可視,中英文等等變數與函式
// 除非要新增控制項,否則不需修改
//------------------------------------------------------------------------------
void TFormSysTools::BackupPopupMenu(TControl *PWinCtrl,TPopupMenu *PCtrl)
{
    LevelSetupPopMenuBackup *P;
    P=new LevelSetupPopMenuBackup;
    P->PCtrl=PWinCtrl;
    P->PopupMenuPtr=PCtrl;
    LevelSetupPopupMenuBackupList->Add((LevelSetupPopMenuBackup*)P);
}
//==============================================================================
void TFormSysTools::SetAndBackPopMenu(TWinControl *PCtrl, TPopupMenu *PopCtrl)
{
    AnsiString S="", S1="";

    TPanel    * PanelPtr    = dynamic_cast <TPanel      *>(PCtrl);
    TTabSheet   *ShtPtr     = dynamic_cast <TTabSheet   *>(PCtrl);
    TForm       *FrmPtr     = dynamic_cast <TForm       *>(PCtrl);
    TGroupBox   *PGroupBox  = dynamic_cast <TGroupBox   *>(PCtrl);
//    TScrollBox  *ScrBox     = dynamic_cast <TScrollBox  *>(PCtrl);
    TRadioGroup *RadGup     = dynamic_cast <TRadioGroup *>(PCtrl);

    if(PanelPtr!=NULL)
    {
        BackupPopupMenu(PanelPtr, PanelPtr->PopupMenu);
        PanelPtr->PopupMenu=PopCtrl;
    }
    else if(ShtPtr!=NULL)
    {
        BackupPopupMenu(ShtPtr, ShtPtr->PopupMenu);
        ShtPtr->PopupMenu=PopCtrl;
    }
    else if(FrmPtr!=NULL)
    {
        BackupPopupMenu(FrmPtr, FrmPtr->PopupMenu);
        FrmPtr->PopupMenu=PopCtrl;
    }
    else if(PGroupBox!=NULL)
    {
        BackupPopupMenu(PGroupBox, PGroupBox->PopupMenu);
        PGroupBox->PopupMenu=PopCtrl;
    }
    else if(RadGup!=NULL)
    {
        BackupPopupMenu(RadGup, RadGup->PopupMenu);
        RadGup->PopupMenu=PopCtrl;
    }

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            SetAndBackPopMenu((TWinControl *) P, PopCtrl);
        }

        TLabel          *PLabel       = dynamic_cast <TLabel        *>(P);
        TSpeedButton    *PSpeedButton = dynamic_cast <TSpeedButton  *>(P);
        TButton         *PButton      = dynamic_cast <TButton       *>(P);
        TCheckBox       *PCheckBox    = dynamic_cast <TCheckBox     *>(P);
        TRadioButton    *PRadioButton = dynamic_cast <TRadioButton  *>(P);
        TComboBox       *PTComboBox   = dynamic_cast <TComboBox     *>(P);
        TEdit           *PTEdit       = dynamic_cast <TEdit         *>(P);
        TLabeledEdit    *LTEdit       = dynamic_cast <TLabeledEdit  *>(P);

        if(PLabel!=NULL)
        {
            BackupPopupMenu(PLabel, PLabel->PopupMenu);
            PLabel->PopupMenu=PopCtrl;
        }
        else if(PSpeedButton!=NULL)
        {
            BackupPopupMenu(PSpeedButton, PSpeedButton->PopupMenu);
            PSpeedButton->PopupMenu=PopCtrl;
        }
        else if(PButton!=NULL)
        {
            BackupPopupMenu(PButton, PButton->PopupMenu);
            PButton->PopupMenu=PopCtrl;
        }
        else if(PCheckBox!=NULL)
        {
            BackupPopupMenu(PCheckBox, PCheckBox->PopupMenu);
            PCheckBox->PopupMenu=PopCtrl;
        }
        else if(PRadioButton!=NULL)
        {
            BackupPopupMenu(PRadioButton, PRadioButton->PopupMenu);
            PRadioButton->PopupMenu=PopCtrl;
        }
        else if(PTComboBox!=NULL)
        {
            BackupPopupMenu(PTComboBox, PTComboBox->PopupMenu);
            PTComboBox->PopupMenu=PopCtrl;
        }
        else if(PTEdit!=NULL)
        {
            BackupPopupMenu(PTEdit, PTEdit->PopupMenu);
            PTEdit->PopupMenu=PopCtrl;
        }
        else if(LTEdit!=NULL)
        {
            BackupPopupMenu(LTEdit, LTEdit->PopupMenu);
            LTEdit->PopupMenu=PopCtrl;
        }
    }
}
//==============================================================================
void TFormSysTools::AddSetLevelPopMenu(TForm *FormPtr)
{
    LevelSetup *P;
    LevelSetup *P1;
    P=new LevelSetup;
    P->FormPtr=FormPtr;
    for(int i=0; i<LevelSetupList->Count; i++)
    {
        P1=(MyLevelSetupStruct *)LevelSetupList->Items[i];
        if(P1->FormPtr==P->FormPtr)
        {
            return;
        }
    }
    LevelSetupList->Add((Language*)P);
    SetAndBackPopMenu(FormPtr, fLevelSetup->SetLevelPopupMenu);
}
//==============================================================================
void TFormSysTools::InitialSetLevelPopMenuList()
{
    TForm *P;
    TComponent *PCtrl;
    AnsiString S;
    LevelSetupList->Clear();
    LevelSetupPopupMenuBackupList->Clear();

    for(int i=0; i<Application->ComponentCount; i++)
    {
        PCtrl=Application->Components[i];
        P=dynamic_cast <TForm *>(PCtrl);
        if(P!=NULL && P->Name!="fLevelSetup")
            AddSetLevelPopMenu(P);
    }
}
//==============================================================================
// 回復原原件之PopupMenu
//==============================================================================
template <class TPanel2> void TFormSysTools::MyTemp(LevelSetupPopMenuBackup *P, TPanel2 *P2, TControl *PCtrl)
{
    if(dynamic_cast <TPanel2 *>(PCtrl)!=NULL)
    {
        P2=dynamic_cast <TPanel2 *>(PCtrl);
        if(P->PopupMenuPtr==NULL)
        {
            P2->PopupMenu=NULL;                                                 //V1.1
        }
        else
        {
            P2->PopupMenu=P->PopupMenuPtr;
        }
    }
}
//==============================================================================
void TFormSysTools::RestoreSetLevelPopMenu()
{
    LevelSetupPopMenuBackup *P;
    for(int i=0; i<LevelSetupPopupMenuBackupList->Count; i++)
    {
        P=(LevelSetupPopMenuBackup *)LevelSetupPopupMenuBackupList->Items[i];
        TPanel          *TempClass1;
        TTabSheet       *TempClass2;
        TRadioGroup     *TempClass3;
        TLabel          *TempClass4;
        TSpeedButton    *TempClass5;
        TButton         *TempClass6;
        TCheckBox       *TempClass7;
        TRadioButton    *TempClass8;
        TGroupBox       *TempClass9;
        TScrollBox      *TempClass10;
        TComboBox       *TempClass11;

        //TComboBox       *TempClass12;                                         //V1.0
        TForm          *TempClass12;                                            //V1.1

        TEdit           *TempClass13;

        MyTemp(P, TempClass1, P->PCtrl);
        MyTemp(P, TempClass2, P->PCtrl);
        MyTemp(P, TempClass3, P->PCtrl);
        MyTemp(P, TempClass4, P->PCtrl);
        MyTemp(P, TempClass5, P->PCtrl);
        MyTemp(P, TempClass6, P->PCtrl);
        MyTemp(P, TempClass7, P->PCtrl);
        MyTemp(P, TempClass8, P->PCtrl);
        MyTemp(P, TempClass9, P->PCtrl);
        MyTemp(P, TempClass10, P->PCtrl);
        MyTemp(P, TempClass11, P->PCtrl);
        MyTemp(P, TempClass12, P->PCtrl);
        MyTemp(P, TempClass13, P->PCtrl);
    }
}
//============================================================================== //20100120 terry start//
// 設定原件字型、大小、顏色
//==============================================================================
void __fastcall TFormSysTools::SetComponentFont(AnsiString FormName,AnsiString BaseName,TControl *TempCtrl)
{
    AnsiString asFontName, S1;
    int iFontSize, iFontColor;
    std::map<AnsiString,AnsiString >::iterator mapIter;                         // map ben add 20110727 //

    S1=BaseName+"_Font";
    if(bLoadLevelSetupToMap)                                                    // map ben add 20110727 -s //
    {
        asFontName=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), "9999");
        mapLevelSetup[FormName+"_"+S1]=asFontName;
    }
    else
    {
        mapIter=mapLevelSetup.find(FormName+"_"+S1);
        if(mapIter->second!="")
            asFontName=mapIter->second;
        else
            asFontName="9999";
    }
    if(asFontName=="9999")
        return;

    S1=BaseName+"_Size";
    if(bLoadLevelSetupToMap)
    {
        iFontSize=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), 9999);
        mapLevelSetup[FormName+"_"+S1]=AnsiString(iFontSize);
    }
    else
    {
        mapIter=mapLevelSetup.find(FormName+"_"+S1);
        if(mapIter->second!="")
            iFontSize=StrToInt(mapIter->second);
        else
            iFontSize=9999;
    }
    if(iFontSize==9999)
        return;
    S1=BaseName+"_Color";
    if(bLoadLevelSetupToMap)
    {
        iFontColor=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), 9999);
        mapLevelSetup[FormName+"_"+S1]=AnsiString(iFontColor);
    }
    else
    {
        mapIter=mapLevelSetup.find(FormName+"_"+S1);
        if(mapIter->second!="")
            iFontColor=StrToInt(mapIter->second);
        else
            iFontColor=9999;
    }                                                                           // map ben add 20110727 -e //
    if(iFontColor==9999)
        return;

    TPanel *PanelPtr              = dynamic_cast <TPanel        *>(TempCtrl);
    TEdit *EditPtr                = dynamic_cast <TEdit         *>(TempCtrl);
    TLabeledEdit *LEditPtr        = dynamic_cast <TLabeledEdit  *>(TempCtrl);
    TLabel *LabelPtr              = dynamic_cast <TLabel        *>(TempCtrl);
    TSpeedButton *SpeedBtnPtr     = dynamic_cast <TSpeedButton  *>(TempCtrl);
    TRadioGroup *RadGup           = dynamic_cast <TRadioGroup   *>(TempCtrl);
    TForm *FrmPtr                 = dynamic_cast <TForm         *>(TempCtrl);
    TTabSheet *ShtPtr             = dynamic_cast <TTabSheet     *>(TempCtrl);
    TGroupBox *PGroupBox          = dynamic_cast <TGroupBox     *>(TempCtrl);
    TComboBox *ComboBoxPtr        = dynamic_cast <TComboBox     *>(TempCtrl);
    TRadioButton *RadioButtonPtr  = dynamic_cast <TRadioButton  *>(TempCtrl);
    TCheckBox *CheckPtr           = dynamic_cast <TCheckBox     *>(TempCtrl);
    TButton *ButtonPtr            = dynamic_cast <TButton       *>(TempCtrl);

    if(PanelPtr!=NULL)
    {
        PanelPtr->Font->Name=asFontName;
        PanelPtr->Font->Size=iFontSize;
        PanelPtr->Font->Color=TColor(iFontColor);                               //V1.2
    }
    else if(EditPtr!=NULL)
    {
        EditPtr->Font->Name=asFontName;
        EditPtr->Font->Size=iFontSize;
        EditPtr->Font->Color=TColor(iFontColor);                                //V1.2
    }
    else if(LEditPtr!=NULL)
    {
        LEditPtr->Font->Name=asFontName;
        LEditPtr->Font->Size=iFontSize;
        LEditPtr->Font->Color=TColor(iFontColor);                               //V1.2
    }
    else if(LabelPtr!=NULL)
    {
        LabelPtr->Font->Name=asFontName;
        LabelPtr->Font->Size=iFontSize;
        LabelPtr->Font->Color=TColor(iFontColor);                               //V1.2
    }
    else if(SpeedBtnPtr!=NULL)
    {
        SpeedBtnPtr->Font->Name=asFontName;
        SpeedBtnPtr->Font->Size=iFontSize;
        SpeedBtnPtr->Font->Color=TColor(iFontColor);                            //V1.2
    }
    else if(RadGup!=NULL)
    {
        RadGup->Font->Name=asFontName;
        RadGup->Font->Size=iFontSize;
        RadGup->Font->Color=TColor(iFontColor);                                 //V1.2
    }
    else if(FrmPtr!=NULL)
    {
        FrmPtr->Font->Name=asFontName;
        FrmPtr->Font->Size=iFontSize;
        FrmPtr->Font->Color=TColor(iFontColor);                                 //V1.2
    }
    else if(ShtPtr!=NULL)
    {
        ShtPtr->Font->Name=asFontName;
        ShtPtr->Font->Size=iFontSize;
        ShtPtr->Font->Color=TColor(iFontColor);                                 //V1.2
    }
    else if(PGroupBox!=NULL)
    {
        PGroupBox->Font->Name=asFontName;
        PGroupBox->Font->Size=iFontSize;
        PGroupBox->Font->Color=TColor(iFontColor);                              //V1.2
    }
    else if(ComboBoxPtr!=NULL)
    {
        ComboBoxPtr->Font->Name=asFontName;
        ComboBoxPtr->Font->Size=iFontSize;
        ComboBoxPtr->Font->Color=TColor(iFontColor);                            //V1.2
    }
    else if(RadioButtonPtr!=NULL)
    {
        RadioButtonPtr->Font->Name=asFontName;
        RadioButtonPtr->Font->Size=iFontSize;
        RadioButtonPtr->Font->Color=TColor(iFontColor);                         //V1.2
    }
    else if(CheckPtr!=NULL)
    {
        CheckPtr->Font->Name=asFontName;
        CheckPtr->Font->Size=iFontSize;
        CheckPtr->Font->Color=TColor(iFontColor);                               //V1.2
    }
    else if(ButtonPtr!=NULL)
    {
        ButtonPtr->Font->Name=asFontName;
        ButtonPtr->Font->Size=iFontSize;
        ButtonPtr->Font->Color=TColor(iFontColor);                              //V1.2
    }
}                                                                               //20100120 terry end//
//==============================================================================
// 將第二層 Panel 之框線去除
//==============================================================================
void __fastcall TFormSysTools::SetSubPlaneAsNoFrame(TWinControl *PCtrl)         //20111014 terry//
{
    TPanel *P;
    for(int i=0; i<PCtrl->ControlCount; i++)                                    //第一層
    {
        P=dynamic_cast <TPanel *>(PCtrl->Controls[i]);
        if(P!=NULL)                                                             //找出所有Group第二層內的Panel
        {
            P->BevelOuter=bvNone;
            P->BevelInner=bvNone;
        }
    }
}
//==============================================================================
// 設定原件可視不可視,中英文等等變數與函式
//==============================================================================
void TFormSysTools::SetLevel(AnsiString FormName, AnsiString BaseName, TControl *P)
{
    int ret1, ret2, ret3, ret4, ret5, ret6;
    bool flag1=true, flag2=true, flag3=true, flag4=true, flag5=true, flag6=true;
    AnsiString S, S1, S2, S3;
    std::map<AnsiString, AnsiString >::iterator mapIter;                        // map ben add 20110727 //
    S=BaseName;

    if(bLoadLevelSetupToMap)                                                    // map ben add 20110727 -s //
    {
        S1=S+"_English";
        S2=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), "NULL_STRING");
        mapLevelSetup[FormName+"_"+S1]=S2;
        if(S2!="NULL_STRING" && S2!="")
        {
            P->SetTextBuf(S2.c_str());
            SetComponentFont(FormName,BaseName,P);                              //20100120 terry//
        }

        S1=S+"_Chinese";
        S2=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), "NULL_STRING");
        mapLevelSetup[FormName+"_"+S1]=S2;
        if(S2!="NULL_STRING" && S2!="")
        {
            P->SetTextBuf(S2.c_str());
            SetComponentFont(FormName, BaseName, P);                            //20100120 terry//
        }
    }                                                                           // map ben add 20110727 -e //

    if(HSys.LastSet.iLanguageCountry==0)
    {
        S1=S+"_English";
    }
    else
    {
        S1=S+"_Chinese";
    }

    mapIter=mapLevelSetup.find(FormName+"_"+S1);                                // map ben add 20110727 -s //
    if(mapIter->second!="")
        S2=mapIter->second;
    else
        S2="NULL_STRING";

    if(S2!="NULL_STRING" && S2!="")
    {
        S3=P->ClassName();
        if(S3=="TLabel")
        {
            TLabel *pTempLabel=dynamic_cast<TLabel *>(P);
            pTempLabel->Caption=S2;
        }
        else if(S3=="TButton")
        {
            TButton *pTempLabel=dynamic_cast<TButton *>(P);
            pTempLabel->Caption=S2;
        }
        P->SetTextBuf(S2.c_str());
        SetComponentFont(FormName,BaseName,P);                                  //20100120 terry//
    }

    S1=S+"_Enable";
    if(bLoadLevelSetupToMap)
    {
        ret1=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), 9999);          // enable //
        S3=FormName+"_"+S1;
        mapLevelSetup[S3]=AnsiString(ret1);
    }
    else
    {
        S3=FormName+"_"+S1;
        mapIter=mapLevelSetup.find(S3);
        if(mapIter->second!="")
            ret1=StrToInt(mapIter->second);
        else
            ret1==9999;
    }
    if(ret1==9999)
        return;
    S1=S+"_Visible";
    if(bLoadLevelSetupToMap)
    {
        ret2=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), 9999);          // Visible //
        mapLevelSetup[FormName+"_"+S1]=AnsiString(ret2);
    }
    else
    {
        mapIter=mapLevelSetup.find(FormName+"_"+S1);
        if(mapIter->second!="")
            ret2=StrToInt(mapIter->second);
        else
            ret2==9999;
    }
    if(ret2==9999)
        return;
    S1=S+"_EnableAtRun";
    if(bLoadLevelSetupToMap)
    {
        ret3=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), 9999);          // enable //
        mapLevelSetup[FormName+"_"+S1]=AnsiString(ret3);
    }
    else
    {
        mapIter=mapLevelSetup.find(FormName+"_"+S1);
        if(mapIter->second!="")
            ret3=StrToInt(mapIter->second);
        else
            ret3==9999;
    }
    if(ret3==9999)
        return;
    S1=S+"_VisibleAtRun";
    if( bLoadLevelSetupToMap )
    {
        ret4=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), 9999);          // Visible //
        mapLevelSetup[FormName+"_"+S1]=AnsiString(ret4);
    }
    else
    {
        mapIter=mapLevelSetup.find(FormName+"_"+S1);
        if(mapIter->second!="")
            ret4=StrToInt(mapIter->second);
        else
            ret4=9999;
    }
    if(ret4==9999)
        return;
    S1=S+"_EnableAtHot";
    if(bLoadLevelSetupToMap)
    {
        ret5=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), 9999);          // enable //
        mapLevelSetup[FormName+"_"+S1]=AnsiString(ret5);
    }
    else
    {
        mapIter=mapLevelSetup.find(FormName+"_"+S1);
        if(mapIter->second!="")
            ret5=StrToInt(mapIter->second);
        else
            ret5=9999;
    }
    if(ret5==9999)
        return;
    S1=S+"_VisibleAtHot";
    if(bLoadLevelSetupToMap)
    {
        ret6=fLevelSetup->ReadIni(FormName.c_str(), S1.c_str(), 9999);          // Visible //
        mapLevelSetup[FormName+"_"+S1]=AnsiString(ret6);
    }
    else
    {
        mapIter=mapLevelSetup.find(FormName+"_"+S1);
        if(mapIter->second!="")
            ret6=StrToInt(mapIter->second);
        else
            ret6=9999;
    }
    if(ret6==9999)                                                              // map ben add 20110727 -e //
        return;

    if(ret1!=-1)
    {
        if(HSys.AccessLevel<ret1)
            flag1=false;
    }

    if(ret2!=-1)
    {
        if(HSys.AccessLevel<ret2)
            flag2=false;
    }

    if(ret3==0 && HSys.Sys.SystemStart==true)
    {
        flag3=false;
    }
    else if( ret3==1 && HSys.Sys.SystemStart==false)
    {
        flag3=false;
    }

    if(ret4==0 && HSys.Sys.SystemStart==true)
    {
        flag4=false;
    }
    else if(ret4==1 && HSys.Sys.SystemStart==false)
    {
        flag4=false;
    }

    if(ret5==0 && HSys.LastSet.iTemperature==1)
    {
        flag5=false;
    }
    else if(ret5==1 && HSys.LastSet.iTemperature==0)
    {
        flag5=false;
    }

    if(ret6==0 && HSys.LastSet.iTemperature==1)
    {
        flag6=false;
    }
    else if(ret6==1 && HSys.LastSet.iTemperature==0)
    {
        flag6=false;
    }

    P->Enabled=flag1 & flag3 & flag5;
    P->Visible=flag2 & flag4 & flag6;
}
//==============================================================================
void TFormSysTools::GetAndSetLevel(AnsiString FormName, TWinControl *PCtrl)
{
    SetLevel(FormName,PCtrl->Name,(TControl *)PCtrl);
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P = PCtrl->Controls[iP];

        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            GetAndSetLevel(FormName, (TWinControl *) P);
        }
        else
        {
            SetLevel(FormName, P->Name, (TControl *)P);
        }
    }
}
//==============================================================================
// 可以將 TWinControl 內所有子物件予以存檔
// 存檔步驟 1.OpenFormData   2.SaveFormData  3.CloseFormData()
// 載檔步驟 1.OpenFormData   2.LoadFormData  3.CloseFormData()
//==============================================================================
void __fastcall TFormSysTools::OpenFormData(AnsiString Filename)
{
    INIFile = new TIniFile(Filename);
}
//==============================================================================
void __fastcall TFormSysTools::CloseFormData()
{
    delete INIFile;
}
//==============================================================================
void __fastcall TFormSysTools::SaveFormData(TWinControl *PCtrl, AnsiString GroupName, bool bLoadLed)
{
    AnsiString S="", S1="", S2="";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            TRadioGroup *PRadioGroup=dynamic_cast <TRadioGroup *>(P);
            if(PRadioGroup!=NULL && PRadioGroup->Name!="")
            {
                INIFile->WriteString(GroupName, PRadioGroup->Name, AnsiString(PRadioGroup->ItemIndex));
            }

            SaveFormData((TWinControl *) P, GroupName, bLoadLed);               // 找該物件附屬的物件(遞迴)
        }
        TCustomEdit *PEdit         = dynamic_cast <TCustomEdit *>(P);
        TCheckBox *PCheckBox       = dynamic_cast <TCheckBox *>(P);
        TRadioButton *PRadioButton = dynamic_cast <TRadioButton *>(P);
        TComboBox *PComboBox       = dynamic_cast <TComboBox *>(P);
        TScrollBar *PScrollBar     = dynamic_cast <TScrollBar *>(P);
        TImage *PImage             = dynamic_cast <TImage *>(P);
        TALed *PALed               = dynamic_cast <TALed *>(P);                 // ben add 20110712 //

        if(PEdit!=NULL && PEdit->Name!="")                                      // 如果是TLabel型態
        {
            S=PEdit->Name;
            if(PEdit->Text.Length()==0)
                PEdit->Text=" ";

            if(GroupName=="tsCCD" && (PEdit->Name=="meLog_Loader" || PEdit->Name=="meLog_Empty1" || PEdit->Name=="meLog_Empty2"))  //JerryYang 20240802 : modify
            {

            }
            else
            {
                INIFile->WriteString(GroupName, PEdit->Name, PEdit->Text);
            }
        }
        if(PCheckBox!=NULL && PCheckBox->Name!="")                              // 如果是TLabel型態
        {
            if(PCheckBox->Checked)
                INIFile->WriteString(GroupName, PCheckBox->Name, "1");
            else
                INIFile->WriteString(GroupName, PCheckBox->Name, "0");
        }
        if(PRadioButton!=NULL && PRadioButton->Name!="")                        // 如果是TLabel型態
        {
            if(PRadioButton->Checked)
                INIFile->WriteString(GroupName, PRadioButton->Name, "1");
            else
                INIFile->WriteString(GroupName, PRadioButton->Name, "0");
        }
        if(PComboBox!=NULL && PComboBox->Name!="")
        {
            INIFile->WriteString(GroupName, PComboBox->Name, AnsiString(PComboBox->Text));
        }
        if(PScrollBar!=NULL && PScrollBar->Name!="")
        {
            INIFile->WriteString(GroupName, PScrollBar->Name, AnsiString(PScrollBar->Position));
        }
        if(PImage!=NULL && PImage->Name!="")
        {
            INIFile->WriteString(GroupName, PImage->Name, AnsiString(PImage->Tag));
        }

        if(PALed!=NULL && PALed->Name!="")                                      // ben add 20110712 //
        {
            S1=PALed->Name+"_Blink";                                            // 閃爍 //
            if(PALed->Blink)
                INIFile->WriteString(GroupName, S1.c_str(), "1");
            else
                INIFile->WriteString(GroupName, S1.c_str(), "0");

            if(bLoadLed)
            {
                S2=PALed->Name+"_Value";                                        // 亮滅 //
                if(PALed->Value)
                    INIFile->WriteString(GroupName, S2.c_str(), "1");
                else
                    INIFile->WriteString(GroupName, S2.c_str(), "0");
            }
        }
    }
}
//==============================================================================
void __fastcall TFormSysTools::LoadFormData(TWinControl *PCtrl, AnsiString GroupName, bool bLoadLed)
{
    AnsiString S="", S1="", S2="";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P = PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            TRadioGroup *PRadioGroup = dynamic_cast <TRadioGroup *>(P);
            if(PRadioGroup!=NULL && PRadioGroup->Name!="")
            {

                S=INIFile->ReadString(GroupName, PRadioGroup->Name, "0");
                PRadioGroup->ItemIndex=atoi(S.c_str());

            }
            LoadFormData((TWinControl *) P, GroupName, bLoadLed);               // 找該物件附屬的物件(遞迴)
        }

        TCustomEdit *PEdit          = dynamic_cast <TCustomEdit *>(P);
        TCheckBox *PCheckBox        = dynamic_cast <TCheckBox *>(P);
        TRadioButton *PRadioButton  = dynamic_cast <TRadioButton *>(P);
        TComboBox *PComboBox        = dynamic_cast <TComboBox *>(P);
        TScrollBar *PScrollBar      = dynamic_cast <TScrollBar *>(P);
        TImage *PImage              = dynamic_cast <TImage *>(P);
        TALed *PALed                = dynamic_cast <TALed *>(P);                // ben add 20110712 //

        if(PEdit!=NULL && PEdit->Name!="")
        {
            PEdit->Text=INIFile->ReadString(GroupName, PEdit->Name , PEdit->Text);
        }

        if(PCheckBox!=NULL && PCheckBox->Name!=NULL)
        {
            S=INIFile->ReadString(GroupName, PCheckBox->Name, "0");
            if(S=="1")
                PCheckBox->Checked=true;
            else
                PCheckBox->Checked=false;
        }

        if(PRadioButton!=NULL && PRadioButton->Name!="")
        {
            S=INIFile->ReadString(GroupName, PRadioButton->Name, "0");
            if(S=="1")
                PRadioButton->Checked=true;
            else
                PRadioButton->Checked=false;
        }

        if(PComboBox!=NULL && PComboBox->Name!="")
        {
            S=INIFile->ReadString(GroupName, PComboBox->Name, "0");
            PComboBox->ItemIndex=PComboBox->Items->IndexOf(S);
        }
        if(PScrollBar!=NULL && PScrollBar->Name!="")
        {
            S=INIFile->ReadString(GroupName, PScrollBar->Name, "0");
            PScrollBar->Position=atoi(S.c_str());
        }

        if(PImage!=NULL && PImage->Name!="")
        {
            S=INIFile->ReadString(GroupName, PImage->Name, "0");
            PImage->Tag=atoi(S.c_str());
        }

        if(PALed!=NULL && PALed->Name!="")                                      // ben add 20110712 //
        {
            S1=PALed->Name+"_Blink";                                            // 閃爍 //
            S=INIFile->ReadString(GroupName, S1.c_str(), "0");
            if(S=="1")
                PALed->Blink=true;
            else
                PALed->Blink=false;

            if(bLoadLed)
            {
                S2=PALed->Name+"_Value";                                        // 亮滅 //
                S=INIFile->ReadString(GroupName, S2.c_str(), "0");
                if(S=="1")
                    PALed->Value=true;
                else
                    PALed->Value=false;
            }
        }
    }
}
//==============================================================================
void TFormSysTools::TerminalProgram()                                           // stop thread and close program //
{
    MyThread->Suspend();
    Application->Terminate();
}
//==============================================================================
void TFormSysTools::WriteLogs(AnsiString S)
{
    if(Logs!=NULL)
        Logs->Lines->Add(S);
};
//==============================================================================
void TFormSysTools::SetLogsMemo(TCustomMemo *P)
{
    Logs=P;
}
//==============================================================================
void TFormSysTools::AddMyShow(long *lRefValue, TObject *RefPalnelPtr)
{
    TPanel *pTempPanel;
    MyShowList *P;
    MyShowList *P2;

    P=new MyShowStruct;
    P->palPtr=RefPalnelPtr;
    P->lPtr=lRefValue;
    P->lBackupValue=*lRefValue;
    P->dataType=1;

    pTempPanel=dynamic_cast<TPanel *>(RefPalnelPtr);
    if(pTempPanel!=NULL)
    {
        pTempPanel->Caption=*lRefValue;
    }
    for(int i=0; i<ShowList->Count; i++)
    {
        P2=(MyShowStruct *)ShowList->Items[i];
        if(P2->palPtr==P->palPtr)
        {
            ShowList->Delete(i);
            break;
        }
    }
    ShowList->Add((MyShowStruct*)P);
}
//==============================================================================
void TFormSysTools::AddMyShow(int *iRefValue, TObject *RefPalnelPtr)
{
    TPanel *pTempPanel;
    MyShowList *P;
    MyShowList *P2;

    P=new MyShowStruct;
    P->palPtr=RefPalnelPtr;
    P->iPtr=iRefValue;
    P->iBackupValue=*iRefValue;
    P->dataType=0;

    pTempPanel=dynamic_cast<TPanel *>(RefPalnelPtr);
    if( pTempPanel!=NULL)
    {
        pTempPanel->Caption=*iRefValue;
    }
    for(int i=0; i<ShowList->Count; i++)
    {
        P2=(MyShowStruct *)ShowList->Items[i];
        if(P2->palPtr==P->palPtr)
        {
            ShowList->Delete(i);
            break;
        }
    }
    ShowList->Add((MyShowStruct*)P);
}
//==============================================================================
void TFormSysTools::RefreshMyShow()
{
    MyShowStruct *P;
    TPanel *pTempPanel;
    if(ShowList->Count==0)
        return;
    for(int i=0; i<ShowList->Count; i++)
    {
        P=(MyShowStruct *)ShowList->Items[i];
        if(P->dataType==1)
        {
            if(*P->lPtr!=P->lBackupValue)
            {
                P->lBackupValue=*P->lPtr;
                pTempPanel=dynamic_cast<TPanel *>(P->palPtr);
                if(pTempPanel!=NULL)
                    pTempPanel->Caption=*P->lPtr;
            }
        }
        else if(P->dataType==0)
        {
            if(*P->iPtr!=P->iBackupValue)
            {
                P->iBackupValue=*P->iPtr;
                pTempPanel=dynamic_cast<TPanel *>(P->palPtr);
                if(pTempPanel!=NULL)
                    pTempPanel->Caption=*P->iPtr;
            }
        }
    }
}
//==============================================================================
void TFormSysTools::AddMyTimeStringShow(TObject *RefPalnelPtr, int DataType)
{
    MyTimeStringShowList *P2;
    MyTimeStringShowList *P;
    for(int i=0; i<ShowTimeStringList->Count; i++)
    {
        P2=(MyTimeStringShowList *)ShowTimeStringList->Items[i];
        if(P2->palPtr==RefPalnelPtr)
        {
            ShowTimeStringList->Delete(i);
            break;
        }
    }

    P=new MyTimeStringShowList;
    P->palPtr=RefPalnelPtr;
    if(DataType<0 || DataType>1)
        DataType=0;
    P->dataType=DataType;
    ShowTimeStringList->Add((MyTimeStringShowList*)P);
};
//==============================================================================
void TFormSysTools::RefreshMyTimeString()                                       // ben edit 20110718 //
{
    char str[3][256];
    static DWORD OldSystemSec=-1;

    sprintf(str[0],"%04d-%02d-%02d  %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    if(SystemHour>=13)
        sprintf(str[1],"%04d-%02d-%02d  %02d:%02d:%02d PM", SystemYear, SystemMonth, SystemDate, SystemHour-12, SystemMin, SystemSec);
    else
        sprintf(str[1],"%04d-%02d-%02d  %02d:%02d:%02d AM", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    int i;
    MyTimeStringShowList *P;

    TStatusPanel  *PTempStatusPanel;
    TControl *pTempControl;

    if(ShowTimeStringList->Count==0)
        return;
    if(OldSystemSec!=(DWORD)SystemSec)
        OldSystemSec=SystemSec;
    else
        return;

    for(i=0;i<ShowTimeStringList->Count;i++)
    {
        P=(MyTimeStringShowList *)ShowTimeStringList->Items[i];

        PTempStatusPanel=dynamic_cast<TStatusPanel *>(P->palPtr);
        if( PTempStatusPanel!=NULL)
            PTempStatusPanel->Text=str[P->dataType];

        pTempControl=dynamic_cast<TControl *>(P->palPtr);
        if( pTempControl!=NULL)
            pTempControl->SetTextBuf( str[P->dataType] );
    }
}
//==============================================================================
void __fastcall TFormSysTools::CheckLevelSetup()
{
    TComponent *PCtrl;
    TForm *P;
    if(iOldAccessLevel!=HSys.AccessLevel || HSys.Sys.SystemStart!= bOldSystemStart || iOldLanguageCountry!=HSys.LastSet.iLanguageCountry ||
       iOldTemperature!=HSys.LastSet.iTemperature )
    {
        for(int i=0; i<Application->ComponentCount; i++)
        {
            PCtrl=Application->Components[i];
            P=dynamic_cast <TForm *>(PCtrl);
            if(P!=NULL && P->Name!="fLevelSetup")
            {
                FormSysTools->GetAndSetLevel(P->Name,(TWinControl *)PCtrl);
            }
        }
        if(bLoadLevelSetupToMap)                                                // map ben add 20110727 //
            bLoadLevelSetupToMap=false;                                         // map ben add 20110727 //
        bOldSystemStart=HSys.Sys.SystemStart;
        iOldAccessLevel=HSys.AccessLevel;
        iOldLanguageCountry=HSys.LastSet.iLanguageCountry;
        iOldTemperature=HSys.LastSet.iTemperature;
    }
}
//==============================================================================
// 本Form 內 User Application
//==============================================================================
void __fastcall TFormSysTools::SpeedButton1Click(TObject *Sender)
{
    if( OldHook!=NULL )
    {
        UnHookKeyboard();
        SpeedButton1->Caption="Disble system access";
    }
    else
    {
        HookKeyboard();
        SpeedButton1->Caption="Enable system access";
    }
}
//------------------------------------------------------------------------------
void __fastcall TFormSysTools::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;
    Top =100;
    if(OldHook!=NULL)
        SpeedButton1->Caption="Enable system access";
    else
        SpeedButton1->Caption="Disble system access";
}
//------------------------------------------------------------------------------
void __fastcall TFormSysTools::SpeedButton2Click(TObject *Sender)
{
    if(bSetUserAccessLevel==false)
    {
        InitialSetLevelPopMenuList();
        bSetUserAccessLevel=true;
        SpeedButton2->Caption="Stop set user access level";
    }
    else
    {
        RestoreSetLevelPopMenu();
        bSetUserAccessLevel=false;
        SpeedButton2->Caption="Begin set user access level";
    }
}
//------------------------------------------------------------------------------
void __fastcall TFormSysTools::Timer1Timer(TObject *Sender)
{
    static bool bSystemInitialOk=false;

    GetTimeInfo();                                                              // get now time info //
    RefreshMyShow();                                                            // refresh showlist //
    RefreshMyTimeString();                                                      // refresh time info //
    if(bDelayCheckLevelSetup==false)                                            // ben add delay levelsetup update //
        CheckLevelSetup();                                                      // check level setup //

    if(bSystemInitialOk==false)
    {
        RecordProcess("Program Start with version "+fMain->statMain->Panels->Items[0]->Text);
        bSystemInitialOk=true;
    }
}
//------------------------------------------------------------------------------
void __fastcall TFormSysTools::CreateNewJamErrorTable()
{
    try
    {
        TStringList *StrList=new TStringList();
        StrList->Add("AlarmCode,AlarmType,E_ErrMessage,C_ErrMessage,E_Description,C_Description");

        for(HSys.IterAlarmCodeList=HSys.mapAlarmCodeList.begin(); HSys.IterAlarmCodeList!=HSys.mapAlarmCodeList.end(); HSys.IterAlarmCodeList++)
        {
            StrList->Add(HSys.IterAlarmCodeList->second.CommaText());
        }
        StrList->SaveToFile(HSys.AlarmTablePath);
        StrList->Clear();

        for(HSys.IterNameToAlarm=HSys.mapNameToAlarm.begin(); HSys.IterNameToAlarm!=HSys.mapNameToAlarm.end(); HSys.IterNameToAlarm++)
        {
            StrList->Add(HSys.IterNameToAlarm->first+AnsiString("=")+HSys.IterNameToAlarm->second);
        }
        StrList->SaveToFile(HSys.AlarmMapPath);
        StrList->Clear();
        delete StrList;
    }
    catch(...)
    {
      //  ShowRecordMessage("JamError.db 建立失敗");
    }
}
//==============================================================================
void __fastcall TFormSysTools::CreateMotorAlarmCode()                           // code No. 10010 start //
{
    char MotorErrorEngString[eMotErrTotal][256]={
    {"--Motor Power Off Error"},
    {"--Motor Out Of Torque Error"},
    {"--Motor CW sensor ON Error"},
    {"--Motor CCW sensor ON Error"},
    {"--Motor Soft P position Error"},
    {"--Motor Soft N positionError"},
    {"--Motor position Error,Home and restart"},
    {"--Motor Undefine Error"},
    {"--Motor Target will Out Of Limit Position Error"}};

    char MotorErrorChiString[eMotErrTotal][256]={
    {"--馬達斷電異常"},
    {"--馬達跳脫異常"},
    {"--馬達觸碰正極限異常"},
    {"--馬達觸碰負極限異常"},
    {"--馬達觸碰軟體正極限異常"},
    {"--馬達觸碰軟體負極限異常"},
    {"--馬達位置異常,重新歸零"},
    {"--馬達不明錯誤"},
    {"--馬達要移動的位置超出極限位置!"}};

    char MotorErrorEngProcess[eMotErrTotal][512]={
    {"[1] Cechk power  \\r\\n[2] check wire       \\r\\n[3] Check machine"},
    {"[1] Cechk power  \\r\\n[2] check wire       \\r\\n[3] Check machine"},
    {"[1] Cechk wire   \\r\\n[2] check sensor     \\r\\n[3] Check machine"},
    {"[1] Cechk wire   \\r\\n[2] check sensor     \\r\\n[3] Check machine"},
    {"[1] Cechk wire   \\r\\n[2] check position   \\r\\n[3] Check machine"},
    {"[1] Cechk wire   \\r\\n[2] check position   \\r\\n[3] Check machine"},
    {"[1] Cechk wire   \\r\\n[2] check position   \\r\\n[3] Check machine"},
    {"[1] Cechk power  \\r\\n[2] check wire       \\r\\n[3] Check machine"},
    {""}};

    char MotorErrorChiProcess[eMotErrTotal][512]={
    {"[1] 檢查電源  \\r\\n[2] 檢查線路   \\r\\n[3] 檢查機台"},
    {"[1] 檢查電源  \\r\\n[2] 檢查線路   \\r\\n[3] 檢查機台"},
    {"[1] 檢查線路  \\r\\n[2] 檢查感應器 \\r\\n[3] 檢查機台"},
    {"[1] 檢查線路  \\r\\n[2] 檢查感應器 \\r\\n[3] 檢查機台"},
    {"[1] 檢查線路  \\r\\n[2] 檢查點位   \\r\\n[3] 檢查機台"},
    {"[1] 檢查線路  \\r\\n[2] 檢查點位   \\r\\n[3] 檢查機台"},
    {"[1] 檢查線路  \\r\\n[2] 檢查點位   \\r\\n[3] 檢查機台"},
    {"[1] 檢查電源  \\r\\n[2] 檢查線路   \\r\\n[3] 檢查機台"},
    {""}};

    AnsiString AlarmCode, SEng, SChi, SDecE, SDecC;
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        for(int j=0; j<eMotErrTotal; j++)
        {
            AlarmCode.sprintf("%d%03d%1d", eMotorAlarm, i, j);
            SEng.sprintf("\"[M%02d - %s] Motor %s\"", i+1, HSys.MotPtr[i]->Alias, MotorErrorEngString[j]);
            SChi.sprintf("\"[M%02d - %s] 馬達 %s\"",  i+1, HSys.MotPtr[i]->Alias, MotorErrorChiString[j]);
            SDecE.sprintf("\"%s\"", MotorErrorEngProcess[j]);
            SDecC.sprintf("\"%s\"", MotorErrorChiProcess[j]);

            MyAlarmCodeStruct AlarmCodeList=MyAlarmCodeStruct(AlarmCode, int(eMotorAlarm), SEng, SChi, SDecE, SDecC);
            HSys.mapAlarmCodeList[AlarmCode]=AlarmCodeList;
            HSys.mapNameToAlarm[AlarmCode]=AlarmCode;
            HSys.mapNameToAlarm[HSys.MotPtr[i]->AlarmName[j]]=AlarmCode;
        }
    }
}
//==============================================================================
void __fastcall TFormSysTools::CreateCylinderAlarmCode()                        // code No. 20000 start //
{
    AnsiString AlarmCode, SEng, SChi, SDecE, SDecC;
    char MotorErrorEngProcess[512]={"[1] check sensor \\r\\n[2] check wire \\r\\n[3] check air or air tube"};
    char MotorErrorChiProcess[512]={"[1] 檢查感應器   \\r\\n[2] 檢查線路   \\r\\n[3] 檢查空氣或氣管"};
    SDecE.sprintf("\"%s\"", MotorErrorEngProcess);
    SDecC.sprintf("\"%s\"", MotorErrorChiProcess);

    for(int i=0; i<HSys.iTotalCylinder; i++)
    {
        for(int j=0; j<eCynErrTotal; j++)
        {
            AlarmCode.printf("%d%03d%1d", eCynAlarm, i, j);                     // code No. 20000 start //
            if(j==eOffNotOnErr)
            {
                SEng.printf("\"The cylinder [%s] can not off error\"", HSys.CynPtr[i].OffSensor.Name.c_str());
                SChi.printf("\"汽缸 [%s] OFF時，Off感測器無法感測到動作完成錯誤\"", HSys.CynPtr[i].CylinderName.c_str());
            }
            else if(j==eOffNotOffErr)
            {
                SEng.printf("\"The cylinder [%s] can not on error\"", HSys.CynPtr[i].OffSensor.Name.c_str());
                SChi.printf("\"汽缸 [%s] On時，Off感測器感測機構未動作完成錯誤\"", HSys.CynPtr[i].CylinderName.c_str());
            }
            else if(j==eOffIsOnErr)
            {
                SEng.printf("\"The cylinder [%s] off sensor is on error\"", HSys.CynPtr[i].OffSensor.Name.c_str());
                SChi.printf("\"汽缸 [%s] ON時，Off感測器感測到機構動作異常錯誤\"", HSys.CynPtr[i].CylinderName.c_str());
            }
            else if(j==eOnNotOnErr)
            {
                SEng.printf("\"The cylinder [%s] can not on error\"", HSys.CynPtr[i].OnSensor.Name.c_str());
                SChi.printf("\"汽缸 [%s] ON時，On感測器無法感測到動作完成錯誤\"", HSys.CynPtr[i].CylinderName.c_str());
            }
            else if(j==eOnNotOffErr)
            {
                SEng.printf("\"The cylinder [%s] can not off error\"", HSys.CynPtr[i].OnSensor.Name.c_str());
                SChi.printf("\"汽缸 [%s] OFF時，On感測器感測機構未動作完成錯誤\"", HSys.CynPtr[i].CylinderName.c_str());
            }
            else if(j==eOnIsOnErr)
            {
                SEng.printf("\"The cylinder [%s] on sensor is on error\"", HSys.CynPtr[i].OnSensor.Name.c_str());
                SChi.printf("\"汽缸 [%s] ON時，On感測器感測到機構動作異常錯誤\"", HSys.CynPtr[i].CylinderName.c_str());
            }

            MyAlarmCodeStruct AlarmCodeList=MyAlarmCodeStruct(AlarmCode, int(eCynAlarm), SEng, SChi, SDecE, SDecC, HSys.CynPtr[i].FlushPanelName);
            HSys.mapAlarmCodeList[AlarmCode]=AlarmCodeList;
            HSys.mapNameToAlarm[AlarmCode]  =AlarmCode;
            HSys.mapNameToAlarm[HSys.CynPtr[i].ErrorName[j]]=AlarmCode;
            ComboBox1->Items->Add(AlarmCode);
//            fNote->SystemError->AddSysErr(AlarmCode+"="+AlarmCode, HSys.CynPtr[i].FlushPanelName);
        }
    }
}
//==============================================================================
void __fastcall TFormSysTools::InsertAlarmCodeToDB(eAlarmType AlarmType, AnsiString AlarmCode, AnsiString SEng, AnsiString SChi, AnsiString PanelName, AnsiString SDecE, AnsiString SDecC)                      //   增加例外處理用的錯誤代碼
{
    SEng.sprintf("\"%s\"", SEng);
    SChi.sprintf("\"%s\"", SChi);
    SDecE.sprintf("\"%s\"", SDecE);
    SDecC.sprintf("\"%s\"", SDecC);

    MyAlarmCodeStruct AlarmCodeList=MyAlarmCodeStruct(AlarmCode, int(AlarmType), SEng, SChi, SDecE, SDecC, PanelName);
    HSys.mapAlarmCodeList[AlarmCode]=AlarmCodeList;
    HSys.mapNameToAlarm[AlarmCode]=AlarmCode;

    ComboBox1->Items->Add(AlarmCode);
//    fNote->SystemError->AddSysErr(AlarmCode+"="+AlarmCode, PanelName);
}
//==============================================================================
void __fastcall TFormSysTools::InsertProcessCodeToDB(AnsiString sMessage)
{
    AnsiString AlarmCode, SEng, SChi, SDecE, SDecC;
    AlarmCode.sprintf("%X%04d", int(eRecordProcess), iProcessCodeIndex);
    SEng.sprintf("\"%s\"", sMessage);
    SChi.sprintf("\"\"");
    SDecE.sprintf("\"\"");
    SDecC.sprintf("\"\"");

    MyAlarmCodeStruct AlarmCodeList=MyAlarmCodeStruct(AlarmCode, int(eRecordProcess), SEng, SChi, SDecE, SDecC);
    HSys.mapAlarmCodeList[AlarmCode]=AlarmCodeList;

    HSys.mapNameToAlarm[sMessage]=AlarmCode;

    ComboBox1->Items->Add(AlarmCode);
//    fNote->SystemError->AddSysErr(sMessage+"="+AlarmCode, "palSysErr");
    iProcessCodeIndex++;
}
//==============================================================================
void __fastcall TFormSysTools::CreateSystemAlarmCode()                          //   增加例外處理用的錯誤代碼
{
    //因為下面是自動編碼,所以盡量不要從中間插入!!
    iProcessCodeIndex=0;
    InsertProcessCodeToDB("Program Start"                 );
    InsertProcessCodeToDB("Program Closed"                );
    InsertProcessCodeToDB("START pressed"                 );
    InsertProcessCodeToDB("PAUSE pressed"                 );
    InsertProcessCodeToDB("HOME pressed"                  );
    InsertProcessCodeToDB("RESET pressed"                 );
    InsertProcessCodeToDB("CLEAN OUT pressed"             );
    InsertProcessCodeToDB("ONE CYCLE pressed"             );
    InsertProcessCodeToDB("TRAY FEED pressed"             );
    InsertProcessCodeToDB("ALARM RESET pressed"           );
    InsertProcessCodeToDB("SKIP pressed"                  );
    InsertProcessCodeToDB("RETRY pressed"                 );
    InsertProcessCodeToDB("TRAY END pressed"              );
    InsertProcessCodeToDB("Enter Offset"                  );
    InsertProcessCodeToDB("Enter Tools"                   );
    InsertProcessCodeToDB("Enter Maintance"               );
    InsertProcessCodeToDB("Enter Message"                 );
    InsertProcessCodeToDB("Enter Speed"                   );
    InsertProcessCodeToDB("Enter IO"                      );
    InsertProcessCodeToDB("Enter Product"                 );
    InsertProcessCodeToDB("Enter Motor Test"              );
    InsertProcessCodeToDB("Enter Teach"                   );
    InsertProcessCodeToDB("Enter COM Port Setting"        );
    InsertProcessCodeToDB("Enter Mold Process"            );
    InsertProcessCodeToDB("Power switch pressed (On)!"    );
    InsertProcessCodeToDB("Power switch pressed (Off)!"   );
    InsertProcessCodeToDB("Restore IO"                    );
    InsertProcessCodeToDB("No Restore IO"                 );
    InsertProcessCodeToDB("Change Continue Mode"          );
    InsertProcessCodeToDB("Change Initial Start Mode"     );
    InsertProcessCodeToDB("Change Real IC Mode"           );
    InsertProcessCodeToDB("Change Dummy Mode"             );
    InsertProcessCodeToDB("Change Level"                  );
    InsertProcessCodeToDB("CCD Vision On."                );
    InsertProcessCodeToDB("CCD Vision Off."               );
    InsertProcessCodeToDB("CCD Start Repeat."             );

    //自動編號結束-----

//eOther------
    InsertAlarmCodeToDB(eOther, "80001",   "Message Error -- Read loader tray ID error." ,
                                           "Message Error -- 讀取loader Tray ID異常!",
                                           "palLoadCarID",
                                           "[1] Check loader tray ID and key document",
                                           "[1] 請確認loader tray ID的讀取狀態");

    InsertAlarmCodeToDB(eOther, "80002",   "Message Error -- Can not find file of tray map." ,
                                           "Message Error -- 沒有tray map檔案!",
                                           "palLoadCarID",
                                           "[1] Check file of tray map",
                                           "[1] 請確認tray map檔案的狀態");

    InsertAlarmCodeToDB(eOther, "80003",   "Message Error -- There does not has data inside the file of tray map." ,
                                           "Message Error -- Tray map檔案中沒有對應的資料!",
                                           "palLoadCarID",
                                           "[1] Check file of tray map",
                                           "[1] 請確認tray map檔案的狀態");

    InsertAlarmCodeToDB(eOther, "80004",   "Message Error -- The file of tray map is empty." ,
                                           "Message Error -- Tray map檔案是空的!",
                                           "palLoadCarID",
                                           "[1] Check file of tray map",
                                           "[1] 請確認tray map檔案的狀態");
    InsertAlarmCodeToDB(eOther, "80005",   "Message Error -- The Loader tray clip id is Duplicate." ,
                                           "Message Error -- Tray Loader tray clip id 是重複的!",
                                           "palLoadCarID",
                                           "[1] Check file of load tray clip id",
                                           "[1] 請確認 load tray clip id 的資料");

    InsertAlarmCodeToDB(eOther, "80006",   "Message Error -- sort bin error." , //Sam 20250903 : 增加分Bin異常保護
                                           "Message Error -- 分Bin錯誤!",
                                           "palSysErr",
                                           "[1] Please call the engineer",
                                           "[1] 請工程師確認問題");
    InsertAlarmCodeToDB(eOther, "80007",   "Message Error -- Read loader tray ID code error." ,         //Sam 20250905 : 新增ClipID編碼檢查
                                           "Message Error -- 讀取loader Tray ID編碼錯誤!",
                                           "palLoadCarID",
                                           "[1] Check loader tray ID and key document",
                                           "[1] 請確認loader tray ID的編碼");


    InsertAlarmCodeToDB(eOther, "80011",   "Message Error -- Read empty tray ID error." ,
                                           "Message Error -- 讀取Empty Tray ID異常!",
                                           "palWorkEmpty1ID",
                                           "[1] Check empty tray ID.",
                                           "[1] 請確認empty tray ID的讀取狀態");

    InsertAlarmCodeToDB(eOther, "80012",   "Message Error -- Read empty tray ID code error." ,          //Sam 20250905 : 新增ClipID編碼檢查
                                           "Message Error -- 讀取Empty Tray ID編碼錯誤!",
                                           "palWorkEmpty1ID",
                                           "[1] Check empty tray ID.",
                                           "[1] 請確認empty tray ID的編碼");
    InsertAlarmCodeToDB(eOther, "80013",   "Message Error -- CCD returned abnormal response." ,          //JimmyChiu 20260416 : CCD abnormal response
                                           "Message Error -- CCD returned abnormal response!",
                                           "palWorkEmpty1ID",
                                           "[1] Check CCD connection and barcode quality.",
                                           "[1] Check CCD connection and barcode quality.");



    InsertAlarmCodeToDB(eOther, "80021",   "Message Error -- Read magazine empty tray ID error." ,
                                           "Message Error -- 讀取magazine empty Tray ID異常!",
                                           "palMagEmptyID",
                                           "[1] Check magazine empty tray ID",
                                           "[1] 請確認magazine empty tray ID的讀取狀態");

    InsertAlarmCodeToDB(eOther, "80022",   "Message Error -- Read empty tray ID code error." ,          //Sam 20250905 : 新增ClipID編碼檢查
                                           "Message Error -- 讀取Empty Tray ID 編碼錯誤!",
                                           "palMagEmptyID",
                                           "[1] Check empty tray ID.",
                                           "[1] 請確認empty tray ID的編碼");



    InsertAlarmCodeToDB(eOther, "80100",   "Message Error -- SortTrayArm tray is lose" ,            //Sam 20240709 : 新增 Alarm Code
                                           "Message Error -- SortTrayArm tray 遺失!",
                                           "palTrayArm",
                                           "[1] Check SortTrayArm tray is exist",
                                           "[1] 請確認SortTrayArm tray 是否存在");

    InsertAlarmCodeToDB(eOther, "80101",   "Message Error -- SortTrayArm has unknown tray" ,
                                           "Message Error -- SortTrayArm 未知tray!",
                                           "palTrayArm",
                                           "[1] Please remove the tray",
                                           "[1] 請移除Tray");

    InsertAlarmCodeToDB(eOther, "80102",   "Message Error -- Loader cylinder off time out" ,             //Sam 20240717 : 新增 Alarm Code
                                           "Message Error -- Loader 汽缸off逾時!",
                                           "palWorkLoader",
                                           "[1] Please check loader cylinder off status",
                                           "[1] 請檢查Loader汽缸狀態");

    InsertAlarmCodeToDB(eOther, "80103",   "Message Error -- Loader tray move to tansfer time out" ,             //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Loader tray 輸送逾時!",
                                           "palShiftStage",
                                           "[1] Please check loader tray status",
                                           "[1] 請檢查Loader tray 狀態");

    InsertAlarmCodeToDB(eOther, "80104",   "Message Error -- Loader tray move to CCDStop time out" ,            //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Loader tray 輸送逾時!",
                                           "palLoadCarID",
                                           "[1] Please check loader tray status",
                                           "[1] 請檢查Loader tray 狀態");

    InsertAlarmCodeToDB(eOther, "80105",   "Message Error -- Tansfer tray move to Magazine Loader time out" ,   //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Tansfer tray 輸送逾時!",
                                           "palMagLD",
                                           "[1] Please check Empty2 tray status",
                                           "[1] 請檢查 Empty2 tray 狀態");

    InsertAlarmCodeToDB(eOther, "80106",   "Message Error -- Tansfer tray move to Unloader time out" ,          //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Tansfer tray 輸送逾時!",
                                           "palShiftStage",
                                           "[1] Please check Empty2 tray status",
                                           "[1] 請檢查 Empty2 tray 狀態");

    InsertAlarmCodeToDB(eOther, "80107",   "Message Error -- Magazine Loader tray move to Unloader time out" ,        //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Magazine Loader tray 輸送逾時!",
                                           "palMagLD",
                                           "[1] Please check Empty2 tray status",
                                           "[1] 請檢查 Empty2 tray 狀態");

    InsertAlarmCodeToDB(eOther, "80108",   "Message Error -- Empty2 empty tray move to CCDStop time out" ,        //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Empty2 empty tray 輸送逾時!",
                                           "palMagEmptyID",
                                           "[1] Please check Empty2 tray status",
                                           "[1] 請檢查 Empty2 tray 狀態");
    InsertAlarmCodeToDB(eOther, "80109",   "Message Error -- Empty2 empty tray move to EmptyStop time out" ,        //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Empty2 empty tray 輸送逾時!",
                                           "palMagEmptyTray",
                                           "[1] Please check Empty2 tray status",
                                           "[1] 請檢查 Empty2 tray 狀態");
    InsertAlarmCodeToDB(eOther, "80110",   "Message Error -- Empty1 empty tray move to CCDStop time out" ,        //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Empty1 empty tray 輸送逾時!",
                                           "palWorkEmpty1ID",
                                           "[1] Please check Empty1 tray status",
                                           "[1] 請檢查 Empty1 tray 狀態");                                           
    InsertAlarmCodeToDB(eOther, "80110",   "Message Error -- Empty1 empty tray move to CCDStop time out" ,        //Sam 20240730 : 新增 Alarm Code
                                           "Message Error -- Empty1 empty tray 輸送逾時!",
                                           "palWorkEmpty1ID",
                                           "[1] Please check Empty1 tray status",
                                           "[1] 請檢查 Empty1 tray 狀態");
}
//---------------------------------------------------------------------------   // need edit by user //
void __fastcall TFormSysTools::CreateSystemSensorAlarmCode()                    // system error code //
{
    AnsiString AlarmCode[2], SEng[2], SChi[2], SDecE[2], SDecC[2];
    AnsiString sEngMessage[2], sChiMessage[2], sFlushPanelName;
    AnsiString sEngDescription[2], sChiDescription[2];
    int iIsOn=0, iIsOff=1;
    bool bFlag[2]={false, false};

    if(bInsertToDB==true)
    {
//        Table1->TableName = HSys.CurrentDir+"\\system\\AlarmIndex";
//        Table1->Open();
//        Table1->Edit();
    }

    for(int i=0; i<HSys.iTotalSensor; i++)
    {
        sFlushPanelName="palSysErr";
        bFlag[iIsOn]=false;
        bFlag[iIsOff]=false;
        if(HSys.SenPtr[i].Tag==HSys.Sen.SnMotorPower.Tag)
        {
            sEngMessage[iIsOff]="System Error -- Motor Power Error";
            sChiMessage[iIsOff]="系統錯誤 -- 馬達電源錯誤";
            sEngDescription[iIsOff].sprintf("[1] check Power \\r\\n[2] check sensor or wire");
            sChiDescription[iIsOff].sprintf("[1] 檢查電源 \\r\\n[2] 檢查感應器或線路");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAirIsEnough.Tag)
        {
            sEngMessage[iIsOff]="System Error -- Air Not Enough Error";
            sChiMessage[iIsOff]="系統錯誤 -- 空壓不足錯誤";
            sEngDescription[iIsOff].sprintf("[1] check Air \\r\\n[2] check sensor or wire");
            sChiDescription[iIsOff].sprintf("[1] 檢查空壓 \\r\\n[2] 檢查感應器或線路");
            bFlag[iIsOff]=true;
        }
//        else if(HSys.SenPtr[i].Tag==HSys.Sen.VacuumPump2.Tag)                   //Frank 20151223
//        {
//            sEngMessage[iIsOff]="System Error -- Vacuum Air Not Enough Error";
//            sChiMessage[iIsOff]="系統錯誤 -- 負壓幫浦空壓不足錯誤";
//            sEngDescription[iIsOff].sprintf("[1] check vacuum pump \\r\\n[2] check sensor or wire");
//            sChiDescription[iIsOff].sprintf("[1] 檢查負壓幫浦 \\r\\n[2] 檢查感應器或線路");
//            bFlag[iIsOff]=true;
//        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEMG01.Tag)
        {
            sEngMessage[iIsOff]="System Error -- EMG01 Error";
            sChiMessage[iIsOff]="系統錯誤 -- 緊急停止錯誤";
            sEngDescription[iIsOff].sprintf("[1] check EMG button \\r\\n[2] check wire");
            sChiDescription[iIsOff].sprintf("[1] 檢查緊急停止按鈕 \\r\\n[2] 檢查線路");
            sFlushPanelName.sprintf("palEmg1");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEMG02.Tag)
        {
            sEngMessage[iIsOff]="System Error -- EMG02 Error";
            sChiMessage[iIsOff]="系統錯誤 -- 緊急停止錯誤";
            sEngDescription[iIsOff].sprintf("[1] check EMG button \\r\\n[2] check wire");
            sChiDescription[iIsOff].sprintf("[1] 檢查緊急停止按鈕 \\r\\n[2] 檢查線路");
            sFlushPanelName.sprintf("palEmg2");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEMG03.Tag)
        {
            sEngMessage[iIsOff]="System Error -- EMG03 Error";
            sChiMessage[iIsOff]="系統錯誤 -- 緊急停止錯誤";
            sEngDescription[iIsOff].sprintf("[1] check EMG button \\r\\n[2] check wire");
            sChiDescription[iIsOff].sprintf("[1] 檢查緊急停止按鈕 \\r\\n[2] 檢查線路");
            sFlushPanelName.sprintf("palEmg3");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEMG04.Tag)
        {
            sEngMessage[iIsOff]="System Error -- EMG04 Error";
            sChiMessage[iIsOff]="系統錯誤 -- 緊急停止錯誤";
            sEngDescription[iIsOff].sprintf("[1] check EMG button \\r\\n[2] check wire");
            sChiDescription[iIsOff].sprintf("[1] 檢查緊急停止按鈕 \\r\\n[2] 檢查線路");
            sFlushPanelName.sprintf("palEmg4");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag>=HSys.Sen.SnSafeDoor_01.Tag && HSys.SenPtr[i].Tag<=HSys.Sen.SnSafeDoor_10.Tag)
        {
            sEngMessage[iIsOff].sprintf("System Error -- Safe Door %d Error", i-HSys.Sen.SnSafeDoor_01.Tag+1);
            sChiMessage[iIsOff].sprintf("系統錯誤 -- 安全門 %d 開啟錯誤", i-HSys.Sen.SnSafeDoor_01.Tag+1);
            sEngDescription[iIsOff].sprintf("[1] check safe Door \\r\\n[2] check wire");
            sChiDescription[iIsOff].sprintf("[1] 檢查安全門 \\r\\n[2] 檢查線路");
            sFlushPanelName.sprintf("palSaveDoor%d", i-HSys.Sen.SnSafeDoor_01.Tag+1);
            bFlag[iIsOff]=true;
        }
//        else if(HSys.SenPtr[i].Tag==HSys.Sen.IonFan_Balance.Tag)
//        {
//            sEngMessage[iIsOff].sprintf("System Error -- Ion Fan Error");
//            sChiMessage[iIsOff].sprintf("系統錯誤 -- 離子風扇異常");
//            sEngDescription[iIsOff].sprintf("[1] check ion fan \\r\\n[2] check wire");
//            sChiDescription[iIsOff].sprintf("[1] 檢查離子風扇 \\r\\n[2] 檢查線路");
//            bFlag[iIsOff]=true;
//        }
//        else if(HSys.SenPtr[i].Tag==HSys.Sen.IonFan_Power.Tag)
//        {
//            sEngMessage[iIsOff].sprintf("System Error -- Ion Fan is Off");
//            sChiMessage[iIsOff].sprintf("系統錯誤 -- 離子風扇電源異常");
//            sEngDescription[iIsOff].sprintf("[1] check ion fan \\r\\n[2] check wire");
//            sChiDescription[iIsOff].sprintf("[1] 檢查離子風扇 \\r\\n[2] 檢查線路");
//            bFlag[iIsOff]=true;
//        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnFKCleanOut.Tag)
        {
            sEngMessage[iIsOff].sprintf("System Message -- Clean Out Finish");
            sChiMessage[iIsOff].sprintf("系統訊息 -- 結批完成");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf(" ");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnFKOneCycle.Tag)
        {
            sEngMessage[iIsOff].sprintf("System Message -- One Cycle Finish");
            sChiMessage[iIsOff].sprintf("系統訊息 -- 單循環完成");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf(" ");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnFKTrayFeed.Tag)
        {
            sEngMessage[iIsOff].sprintf("System Message -- Tray Feed Finish");
            sChiMessage[iIsOff].sprintf("系統訊息 -- 退盤完成");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf(" ");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnFKTrayEnd.Tag)
        {
            sEngMessage[iIsOff].sprintf("System Message -- Tray End Finish");
            sChiMessage[iIsOff].sprintf("系統訊息 -- 退盤完成");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf(" ");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnLoaderTrayHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- No Tray on Loader Car!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Loader 卡夾上沒有Tray!");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf("[1] 請確認 Loader 上是否有Tray，請放入新的Tray盤或是Clean Out結束工作");
            sFlushPanelName.sprintf("palLoadCar");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnLoaderCarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray Missing Under Loader Car!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Loader 卡夾下遺失Tray!");
            sEngDescription[iIsOff].sprintf("[1] 請確認 Loader 軌道上是否有Tray，請放入新的Tray盤");
            sChiDescription[iIsOff].sprintf("[1] 請確認 Loader 軌道上是否有Tray，請放入新的Tray盤");
            sFlushPanelName.sprintf("palLoadCar");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Loader has tray error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Loader 上有Tray!");
            sEngDescription[iIsOn].sprintf("[1] There has tray at Loader, pleaseremove it manually.");
            sChiDescription[iIsOn].sprintf("[1] Loader 軌道上有Tray，請手動移除Tray盤");
            sFlushPanelName.sprintf("palLoadCar");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnLoaderSureTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Loader worker does not has tray error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Loader吸料區沒有Tray!");
            sEngDescription[iIsOff].sprintf("[1] 請確認 Loader 吸料區上是否有Tray，請放入新的Tray盤");
            sChiDescription[iIsOff].sprintf("[1] 請確認 Loader 吸料區上是否有Tray，請放入新的Tray盤");
            sFlushPanelName.sprintf("palLoadCar");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty1SelectHasTray.Tag)         //Steven 20240602 : Add SnEmpty1SelectHasTray.OffAlarmCode
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Empty 1 does not has tray error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Empty 1 沒有Tray!");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf("[1] 請確認 Empty 1 上是否有Tray，請放入新的Tray盤");
            sFlushPanelName.sprintf("palEmpty1Car");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty2SelectHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Empty 2 does not has tray error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Empty 2 沒有Tray!");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf("[1] 請確認 Empty 2 上是否有Tray，請放入新的Tray盤");
            sFlushPanelName.sprintf("palEmpty2Car");           //Sam 20240727 : 修正 Alarm 顯示
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty1CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Empty 1 does not has tray error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Empty 1 沒有Tray!");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf("[1] 請確認 Empty 1 軌道上是否有Tray，請放入新的Tray盤");
            sFlushPanelName.sprintf("palEmpty1Car");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Empty 1 has tray error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Empty 1 上有Tray!");
            sEngDescription[iIsOn].sprintf("[1] There has tray at Empty 1, pleaseremove it manually.");
            sChiDescription[iIsOn].sprintf("[1] Empty 1 軌道上有Tray，請手動移除Tray盤");
            sFlushPanelName.sprintf("palEmpty1Car");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty2CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Magazine Empty does not has tray error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Magazine Empty 沒有Tray!");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf("[1] 請確認 Magazine Empty 軌道上是否有Tray，請放入新的Tray盤");
            sFlushPanelName.sprintf("palEmpty2Car");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Magazine Empty has tray error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Magazine Empty 上有Tray!");
            sEngDescription[iIsOn].sprintf("[1] There has tray at Magazine Empty, pleaseremove it manually.");
            sChiDescription[iIsOn].sprintf("[1] Magazine Empty軌道上有Tray，請手動移除Tray盤");
            sFlushPanelName.sprintf("palEmpty2Car");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAutoRow1HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Track 1 to Tray Arm Area Time Out!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- 軌道1的Tray進入時間超時!");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf("[1] 請確認軌道1上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palWorkEmpty1");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAutoRow2HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Track 2 to Tray Arm Area Time Out!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- 軌道2的Tray進入時間超時!");
            sEngDescription[iIsOff].sprintf(" ");
            sChiDescription[iIsOff].sprintf("[1] 請確認軌道2上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palWorkEmpty2");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAutoRow3HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Track 3 to Tray Arm Area Time Out!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- 軌道3的Tray進入時間超時!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認軌道3上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palWorkEmpty3");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAutoRow4HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Track 4 to Tray Arm Area Time Out!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- 軌道4的Tray進入時間超時!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認軌道4上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palWorkEmpty4");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty1HasTray1.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Track 1 error!!");   //Sam 20240729 : 修正
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- 軌道1的入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認軌道1上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palWorkEmpty1");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty1HasTray2.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Track 2 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- 軌道2的入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認軌道2上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palWorkEmpty2");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty1HasTray3.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Track 3 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- 軌道3的入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認軌道3上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palWorkEmpty3");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty1HasTray4.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error --Load tray to  Track 4 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- 軌道4的入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認軌道4上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palWorkEmpty4");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto1HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 1 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 1入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto1HasTray and C_Auto1_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 1上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto1");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 1 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 1退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto1_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 1 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto1");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto2HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 2 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 2入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto2HasTray and C_Auto2_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 2上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto2");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 2 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 2退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto2_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 2 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto2");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto3HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 3 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 3入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto3HasTray and C_Auto3_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 3上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto3");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 3 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 3退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto3_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 3 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto3");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto4HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 4 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 4入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto4HasTray and C_Auto4_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 4上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto4");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 4 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 4退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto4_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 4 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto4");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto5HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 5 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 5入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto5HasTray and C_Auto5_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 5上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto5");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 5 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 5退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto5_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 5 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto5");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto6HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 6 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 6入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto6HasTray and C_Auto6_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 6上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto6");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 6 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 6退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto6_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 6 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto6");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto7HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 7 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 7入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto7HasTray and C_Auto7_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 7上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto7");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 7 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 7退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto7_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 7 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto7");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto8HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 8 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 8入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto8HasTray and C_Auto8_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 8上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto8");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 8 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 8退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto8_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 8 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto8");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto9HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 9 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 9入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto9HasTray and C_Auto9_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto9");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 9 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 9退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto9_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 19 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto9");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto10HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 10 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 10入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto10HasTray and C_Auto10_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 10上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto10");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 10 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 10退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto10_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 10 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto10");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto11HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 11 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 11入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto11HasTray and C_Auto11_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 11上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto11");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 11 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 11退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto11_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 11 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto11");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto12HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 12 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 12入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto12HasTray and C_Auto12_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 12上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto12");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 12 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 12退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto12_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 12 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto12");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto13HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 13 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 13入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto13HasTray and C_Auto13_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 13上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto13");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 13 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 13退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto13_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 13 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto13");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto14HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 14 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 14入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto14HasTray and C_Auto14_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 14 上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto14");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 14 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 14退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto14_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 14 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto14");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto15HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 15 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 15入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto15HasTray and C_Auto15_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 15上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto15");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 15 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 15退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto15_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 15 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto15");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto16HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 16 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 16入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto16HasTray and C_Auto16_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 16上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto16");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 16 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 16退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto16_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 16 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto16");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto17HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 17 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 17入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto17HasTray and C_Auto17_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 17上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto17");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 17 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 17退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto17_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 17 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto17");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto18HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 18 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 18入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto18HasTray and C_Auto18_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 18上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto18");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 18 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 18退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto18_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 18 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto18");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto19HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 19 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 19入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto19HasTray and C_Auto19_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 19上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto19");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 19 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 19退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto19_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 19 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto19");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto20HasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 20 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 20入Tray時異常!");
            sEngDescription[iIsOff].sprintf("[1] Please check SnAuto20HasTray and C_Auto20_AntiDrop");
            sChiDescription[iIsOff].sprintf("[1] 請確認Auto 20上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto20");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Message Error -- Unload tray from Auto 20 error!!");
            sChiMessage[iIsOn].sprintf("入 Tray 錯誤 -- Auto 20退Tray時異常!");
            sEngDescription[iIsOn].sprintf("[1] Please check the C_Auto20_InclinePush.");
            sChiDescription[iIsOn].sprintf("[1] 請確認Auto 20 Z氣缸動作是否正常，或將Tray盤放好.");
            sFlushPanelName.sprintf("palAuto20");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto1CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 1 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 1入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 1上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto1");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto2CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 2 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 2入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 2上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto2");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto3CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 3 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 3入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 3上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto3");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto4CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 4 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 4入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 4上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto4");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto5CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 5 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 5入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 5上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto5");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto6CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 6 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 6入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 6上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto6");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto7CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 7 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 7入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 7上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto7");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto8CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 8 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 8入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 8上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto8");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto9CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 9 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 9入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 9上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto9");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto10CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 10 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 10入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 10上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto10");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto11CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 11 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 11入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 11上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto11");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto12CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 12 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 12入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 12上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto12");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto13CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 13 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 13入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 13上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto13");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto14CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 14 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 14入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 14上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto14");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto15CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 15 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 15入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 15上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto15");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto16CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 16 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 16入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 16上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto16");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto17CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 17 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 17入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 17上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto17");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto18CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 18 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 18入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 18上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto18");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto19CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 19 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 19入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 19上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto19");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto20CarHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Load tray to Auto 20 error!!");
            sChiMessage[iIsOff].sprintf("入 Tray 錯誤 -- Auto 20入Tray時異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( "[1] 請確認Auto 20上是否有Tray，或將Tray盤放好");
            sFlushPanelName.sprintf("palAuto20");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ1SidePushUp.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Magazine 1 C_SidePush_Up position error, it's status need up!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 1的側推動作異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag1");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ2SidePushUp.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Magazine 2 C_SidePush_Up position error, it's status need up!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 2的側推動作異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag2");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ3SidePushUp.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Magazine 3 C_SidePush_Up position error, it's status need up!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 3的側推動作異常!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag3");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ1TopHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray is not on Magazine 1 top position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 1 Top的位置沒有Tray!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag1");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Magazine -- Tray on Magazine 1 top is fulled with IC!!");
            sChiMessage[iIsOn].sprintf("Magazine -- Magazine 1 Top的位置已滿盤!");
            sEngDescription[iIsOn].sprintf("[1] Please remove the tray at Magazine 1 top");
            sChiDescription[iIsOn].sprintf("[1] 請將Magazine 1 top的Tray取下");
            sFlushPanelName.sprintf("palMag1");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ2TopHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray is not on Magazine 2 top position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 2 Top的位置沒有Tray!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag2");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Magazine -- Tray on Magazine 2 top is fulled with IC!!");
            sChiMessage[iIsOn].sprintf("Magazine -- Magazine 2 Top的位置已滿盤!");
            sEngDescription[iIsOn].sprintf("[1] Please remove the tray at Magazine 2 top");
            sChiDescription[iIsOn].sprintf("[1] 請將Magazine 2 top的Tray取下");
            sFlushPanelName.sprintf("palMag2");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ3TopHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray is not on Magazine 3 top position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 3 Top的位置沒有Tray!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag3");
            bFlag[iIsOff]=true;

            sEngMessage[iIsOn].sprintf("Magazine -- Tray on Magazine 3 top is fulled with IC!!");
            sChiMessage[iIsOn].sprintf("Magazine -- Magazine 3 top的位置已滿盤!");
            sEngDescription[iIsOn].sprintf("[1] Please remove the tray at Magazine 3 top");
            sChiDescription[iIsOn].sprintf("[1] 請將Magazine 3 top的Tray取下");
            sFlushPanelName.sprintf("palMag3");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ1ArmHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray is not on Magazine Arm 1 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine Arm 1 的位置沒有Tray!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag1");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ2ArmHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray is not on Magazine Arm 2 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine Arm 2 的位置沒有Tray!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag2");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ3ArmHasTray.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray is not on Magazine Arm 3 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine Arm 3 的位置沒有Tray!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag3");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ1TrayDirection.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray direction is wrong on Magazine 1 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 1 上的Tray方向錯誤!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag1");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ2TrayDirection.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray direction is wrong on Magazine 2 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 2 上的Tray方向錯誤!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag2");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ3TrayDirection.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray direction is wrong on Magazine 3 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 3 上的Tray方向錯誤!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag3");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ1TrayPositionF.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray positionF is wrong on Magazine 1 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 1 上的Tray位置錯誤!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag1");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ2TrayPositionF.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray positionF is wrong on Magazine 2 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 2 上的Tray位置錯誤!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag2");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnMGZ3TrayPositionF.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message Error -- Tray positionF is wrong on Magazine 3 position, please check!!");
            sChiMessage[iIsOff].sprintf("Magazine錯誤 -- Magazine 3 上的Tray位置錯誤!");
            sEngDescription[iIsOff].sprintf( " ");
            sChiDescription[iIsOff].sprintf( " ");
            sFlushPanelName.sprintf("palMag3");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore1Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 1 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 1 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore01");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore2Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 2 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 2 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore02");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore3Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 3 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 3 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore03");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore4Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 4 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 4 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore04");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore5Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 5 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 5 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore05");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore6Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 6 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 6 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore06");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore7Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 7 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 7 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore07");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore8Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 8 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 8 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore08");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore9Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 9 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 9 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore09");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore10Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 10 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 10 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore10");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore11Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 11 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 11 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore11");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore12Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 12 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 12 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore12");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore13Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 13 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 13 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore13");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore14Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 14 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 14 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore14");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore15Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 15 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 15 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore15");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore16Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 16 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 16 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore16");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore17Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 17 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 17 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore17");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore18Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 18 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 18 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore18");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore19Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 19 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 19 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore19");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore20Full.Tag)
        {
            sEngMessage[iIsOn].sprintf("Message -- Store 20 is fulled with tray. Please remove the tray!!");
            sChiMessage[iIsOn].sprintf("訊息 -- 料倉 20 已滿請將Tray移出!");
            sEngDescription[iIsOn].sprintf( " ");
            sChiDescription[iIsOn].sprintf( " ");
            sFlushPanelName.sprintf("palAutoStore20");
            bFlag[iIsOn]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore1Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 1 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 1 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 1.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 1 的門");
            sFlushPanelName.sprintf("palAutoStore01");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore2Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 2 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 2 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 2.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 2 的門");
            sFlushPanelName.sprintf("palAutoStore02");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore3Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 3 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 3 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 3.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 3 的門");
            sFlushPanelName.sprintf("palAutoStore03");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore4Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 4 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 4 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 4.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 4 的門");
            sFlushPanelName.sprintf("palAutoStore04");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore5Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 5 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 5 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 5.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 5 的門");
            sFlushPanelName.sprintf("palAutoStore05");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore6Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 6 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 6 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 6.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 6 的門");
            sFlushPanelName.sprintf("palAutoStore06");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore7Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 7 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 7 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 7.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 7 的門");
            sFlushPanelName.sprintf("palAutoStore07");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore8Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 8 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 8 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 8.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 8 的門");
            sFlushPanelName.sprintf("palAutoStore08");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore9Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 9 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 9 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 9.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 9 的門");
            sFlushPanelName.sprintf("palAutoStore09");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore10Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 10 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 10 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 10.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 10 的門");
            sFlushPanelName.sprintf("palAutoStore10");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore11Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 11 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 11 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 11.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 11 的門");
            sFlushPanelName.sprintf("palAutoStore11");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore12Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 12 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 12 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 12.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 12 的門");
            sFlushPanelName.sprintf("palAutoStore12");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore13Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 13 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 13 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 13.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 13 的門");
            sFlushPanelName.sprintf("palAutoStore13");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore14Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 14 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 14 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 14.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 14 的門");
            sFlushPanelName.sprintf("palAutoStore14");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore15Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 15 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 15 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 15.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 15 的門");
            sFlushPanelName.sprintf("palAutoStore15");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore15Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 15 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 15 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 15.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 15 的門");
            sFlushPanelName.sprintf("palAutoStore15");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore16Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 16 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 16 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 16.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 16 的門");
            sFlushPanelName.sprintf("palAutoStore16");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore17Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 17 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 17 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 17.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 17 的門");
            sFlushPanelName.sprintf("palAutoStore17");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore18Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 18 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 18 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 18.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 18 的門");
            sFlushPanelName.sprintf("palAutoStore18");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore19Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 19 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 19 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 19.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 19 的門");
            sFlushPanelName.sprintf("palAutoStore19");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStore20Door.Tag)
        {
            sEngMessage[iIsOff].sprintf("Message -- Store 20 door is opened! Please close it");
            sChiMessage[iIsOff].sprintf("訊息 -- 料倉 20 的門未關閉, 請關閉!");
            sEngDescription[iIsOff].sprintf( "Please close the door of store 20.");
            sChiDescription[iIsOff].sprintf( "請關上料倉 20 的門");
            sFlushPanelName.sprintf("palAutoStore20");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnStoreArmHasTray.Tag) //Sam 20240709 : 新增 Alarm Code
        {
            sEngMessage[iIsOff].sprintf("Message Error -- No Tray on Tray Arm!!");
            sChiMessage[iIsOff].sprintf("訊息 -- 沒有Tray 在Tray arm!!");
            sEngDescription[iIsOff].sprintf("[1] Track to Tray Arm Area Time Out, please check tray arm has tray.\\r\\n[2] Please check track/tray arm has tray, and put tray in place.");
            sChiDescription[iIsOff].sprintf("[1] 軌道的Tray進入時間超時，請確認軌道上是否有Tray\\r\\n[2] 請確認track/tray arm 是否有Tray，並將其放好");
            sFlushPanelName.sprintf("palTrayArm");
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnLoaderCCDPos.Tag) //Sam 20240709 : 新增 Alarm Code
        {
            AnsiString sArea="Loader";
            sEngMessage[iIsOff].sprintf("Message Error -- %s CCD Area No Tray Alarm!",sArea);
            sChiMessage[iIsOff].sprintf("%s CCD Tray 錯誤 -- %s 掃秒CCD區的Tray遺失!",sArea,sArea);
            sEngDescription[iIsOff].sprintf("Please check %s CCD Area tray.",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s CCD區是否有Tray，或將Tray盤放好",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty1SelectHasTray.Tag) //Sam 20240709 : 新增 Alarm Code
        {
            AnsiString sArea="Empty1";
            sEngMessage[iIsOff].sprintf("Message Error -- %s CCD Area No Tray Alarm!",sArea);
            sChiMessage[iIsOff].sprintf("%s CCD Tray 錯誤 -- %s 掃秒CCD區的Tray遺失!",sArea,sArea);
            sEngDescription[iIsOff].sprintf("Please check %s CCD Area tray.",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s CCD區是否有Tray，或將Tray盤放好",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnEmpty2SelectHasTray.Tag) //Sam 20240709 : 新增 Alarm Code
        {
            AnsiString sArea="Empty2";
            sEngMessage[iIsOff].sprintf("Message Error -- %s CCD Area No Tray Alarm!",sArea);
            sChiMessage[iIsOff].sprintf("%s CCD Tray 錯誤 -- %s 掃秒CCD區的Tray遺失!",sArea,sArea);
            sEngDescription[iIsOff].sprintf("Please check %s CCD Area tray.",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s CCD區是否有Tray，或將Tray盤放好",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAutoTrolly1HasTray.Tag)          //JerryYang 20250716 : add
        {
            AnsiString sArea="Trolly1";
            sEngMessage[iIsOff].sprintf("Message Error -- %s tray is missing Alarm!",sArea);
            sChiMessage[iIsOff].sprintf("%s tray盤遺失!",sArea);
            sEngDescription[iIsOff].sprintf("Please makesure %s has tray.",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s 上必須有tray，或將Tray盤放好",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAutoTrolly2HasTray.Tag)          //JerryYang 20250716 : add
        {
            AnsiString sArea="Trolly2";
            sEngMessage[iIsOff].sprintf("Message Error -- %s tray is missing Alarm!",sArea);
            sChiMessage[iIsOff].sprintf("%s tray盤遺失!",sArea);
            sEngDescription[iIsOff].sprintf("Please makesure %s has tray.",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s 上必須有tray，或將Tray盤放好",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAutoTrolly3HasTray.Tag)          //JerryYang 20250716 : add
        {
            AnsiString sArea="Trolly3";
            sEngMessage[iIsOff].sprintf("Message Error -- %s tray is missing Alarm!",sArea);
            sChiMessage[iIsOff].sprintf("%s tray盤遺失!",sArea);
            sEngDescription[iIsOff].sprintf("Please makesure %s has tray.",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s 上必須有tray，或將Tray盤放好",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAutoTrolly4HasTray.Tag)          //JerryYang 20250716 : add
        {
            AnsiString sArea="Trolly4";
            sEngMessage[iIsOff].sprintf("Message Error -- %s tray is missing Alarm!",sArea);
            sChiMessage[iIsOff].sprintf("%s tray盤遺失!",sArea);
            sEngDescription[iIsOff].sprintf("Please makesure %s has tray.",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s 上必須有tray，或將Tray盤放好",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }        
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto1CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto1";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto2CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto2";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto3CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto3";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto4CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto4";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto5CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto5";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto6CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto6";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto7CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto7";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto8CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto8";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }                        
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto9CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto9";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto10CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto10";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto11CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto11";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto12CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto12";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto13CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto13";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto14CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto14";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto15CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto15";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto16CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto16";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto17CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto17";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto18CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto18";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }                        
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto19CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto19";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }
        else if(HSys.SenPtr[i].Tag==HSys.Sen.SnAuto20CyAntiDrop1.Tag)            //JerryYang 20250716 : add
        {
            AnsiString sArea="Auto20";
            sEngMessage[iIsOff].sprintf("Message Error -- %s Anti drop sensor!",sArea);
            sChiMessage[iIsOff].sprintf("%s 夾爪sensor異常!",sArea);
            sEngDescription[iIsOff].sprintf("Please check %s  Anti drop sensor!",sArea);
            sChiDescription[iIsOff].sprintf("[1] 請確認%s區Anti drop sensor!",sArea);
            sFlushPanelName.sprintf("%s",sArea);
            bFlag[iIsOff]=true;
        }




        if(bFlag[iIsOff])
        {
            AlarmCode[iIsOff].sprintf("%d%03d%1d", eSystemMess, i, 0);          //30000

            SEng[iIsOff].sprintf("\"%s\"", sEngMessage[iIsOff]);
            SChi[iIsOff].sprintf("\"%s\"", sChiMessage[iIsOff]);
            SDecE[iIsOff].sprintf("\"%s\"", sEngDescription[iIsOff]);
            SDecC[iIsOff].sprintf("\"%s\"", sChiDescription[iIsOff]);

            MyAlarmCodeStruct AlarmCodeList=MyAlarmCodeStruct(AlarmCode[iIsOff], int(eSystemMess), SEng[iIsOff], SChi[iIsOff], SDecE[iIsOff], SDecC[iIsOff], sFlushPanelName);
            HSys.mapAlarmCodeList[AlarmCode[iIsOff]]=AlarmCodeList;
            HSys.SenPtr[i].OffAlarmCode=AlarmCode[iIsOff];

            HSys.mapNameToAlarm[HSys.SenPtr[i].Name]=AlarmCode[iIsOff];
            HSys.mapNameToAlarm[HSys.SenPtr[i].OffAlarmCode]=AlarmCode[iIsOff];
//            fNote->SystemError->AddSysErr(HSys.SenPtr[i].Name+"="+AlarmCode, sFlushPanelName);
            ComboBox1->Items->Add(HSys.SenPtr[i].OffAlarmCode);
        }

        if(bFlag[iIsOn])
        {
            AlarmCode[iIsOn].sprintf("%d%03d%1d", eSystemMess, i, 1);           //30000

            SEng[iIsOn].sprintf("\"%s\"", sEngMessage[iIsOn]);
            SChi[iIsOn].sprintf("\"%s\"", sChiMessage[iIsOn]);
            SDecE[iIsOn].sprintf("\"%s\"", sEngDescription[iIsOn]);
            SDecC[iIsOn].sprintf("\"%s\"", sChiDescription[iIsOn]);

            MyAlarmCodeStruct AlarmCodeList=MyAlarmCodeStruct(AlarmCode[iIsOn], int(eSystemMess), SEng[iIsOn], SChi[iIsOn], SDecE[iIsOn], SDecC[iIsOn], sFlushPanelName);
            HSys.mapAlarmCodeList[AlarmCode[iIsOn]]=AlarmCodeList;
            HSys.SenPtr[i].OnAlarmCode=AlarmCode[iIsOn];

            if(bFlag[iIsOff]==false)                                            //Name只能代表其中一種含意
                HSys.mapNameToAlarm[HSys.SenPtr[i].Name]=AlarmCode[iIsOn];
            HSys.mapNameToAlarm[HSys.SenPtr[i].OnAlarmCode]=AlarmCode[iIsOn];
//            fNote->SystemError->AddSysErr(HSys.SenPtr[i].Name+"="+AlarmCode, sFlushPanelName);
            ComboBox1->Items->Add(HSys.SenPtr[i].OnAlarmCode);
        }
    }

//    if(bInsertToDB==true)
//        Table1->Close();                                                      //20111228 Daver add
}
//---------------------------------------------------------------------------   // need edit by user //
void __fastcall TFormSysTools::CreateSuckAlarmCode()                            // code No. 60000 start //
{
    AnsiString AlarmCode, SEng, SChi, SDecE, SDecC;
    char SuckErrorEngString[eSuckErrTotal][256]={
    {"--Pick Up Error"},
    {"--Destory Error"},
    {"--Vacuum Sensor Off Error"},
    {"--Device Be Droped Error"},
    {"--Initial Sensor Off Error"},
    {"--Initial Sensor On Error"}};

    char SuckErrorChiString[eSuckErrTotal][256]={
    {"--吸取異常"},
    {"--放料異常"},
    {"--真空感測器關閉異常"},
    {"--掉落異常"},
    {"--初始化感應器關閉異常"},
    {"--初始化感應器開啟異常"}};

    char SuckErrorEngProcess[eSuckErrTotal][512]={
    {"[1] Sensor isn't detect, Adjust sensor detection value\\r\\n[2] Confirm the function of the solenoid valve, Check wire and pipeline\\r\\n[3] Suck up-down too fast, Adjust speed\\r\\n[4] No vacuum(Pressure too weak), Confirm the function of device\\r\\n[5] Device didn't put away, Please put away it and select RETRY to continue or remove it and select SKIP to continue\\r\\n[6] Pick time too short, Advice Engineers set longer pick time"},
    {"[1] Sensor always on, Adjust sensor\\r\\n[2] Blow too weak, Confirm the blow\\r\\n[3] Confirm the function of the solenoid valve, Check wire and pipeline"},
    {"[1] Sensor always on, Adjust sensor\\r\\n[2] Blow too weak, Confirm the blow\\r\\n[3] Confirm the function of the solenoid valve, Check wire and pipeline"},
    {"[1] Check whether or hinder\\r\\n[2] Vacuum sensor isn't normal, Adjust vacuum sensor and check wire\\r\\n[3] Confirm the vacuum sensor"},
    {"[1] Sensor initialization always on, Adjust sensor\\r\\n[2] Vacuum sensor isn't normal, Adjust vacuum sensor and check wire\\r\\n [3]Confirm the function of the solenoid valve, Check wire and pipeline"},
    {"[1] Sensor initialization always off, Adjust sensor\\r\\n[2] Vacuum sensor isn't normal, Adjust vacuum sensor and check wire\\r\\n [3]Confirm the function of the solenoid valve, Check wire and pipeline"}};

    char SuckErrorChiProcess[eSuckErrTotal][512]={
    {"[1] 感測器未偵測,調整感測器偵測值\\r\\n[2] 確認電磁閥之功能,檢查線路及管路\\r\\n[3] 吸嘴上下速度太快,調整速度\\r\\n[4] 無真空(壓力太弱),確認產生器之功能\\r\\n[5] 產品未放好,請放好產品後按RETRY繼續或移除產品後按SKIP繼續\\r\\n[6] 吸取時間太短,通知工程師加長吸取時間"},
    {"[1] 感測器一直為開啟狀態,調整感測器\\r\\n [2] 吹氣量太弱,確認吹氣量\\r\\n [3] 確認電磁閥之功能,檢查線路及管路"},
    {"[1] 感測器一直為開啟狀態,調整感測器\\r\\n [2] 吹氣量太弱,確認吹氣量\\r\\n [3] 確認電磁閥之功能,檢查線路及管路"},
    {"[1] 檢查是否阻礙\\r\\n[2] 真空感測器不正常,調整真空感測器及檢查線路\\r\\n[3] 確認真空感測器"},
    {"[1] 初始化時發生感測器一直為開啟狀態,調整感測器\\r\\n[2] 真空感測器不正常,調整真空感測器及檢查線路\\r\\n[3] 確認電磁閥之功能,檢查線路及管路"},
    {"[1] 初始化時發生感測器一直為關閉狀態,調整感測器\\r\\n[2] 真空感測器不正常,調整真空感測器及檢查線路\\r\\n[3] 確認電磁閥之功能,檢查線路及管路"}};

    for(int i=0; i<HSys.iTotalSucker; i++)                                      // kitsuck //
    {
        for(int j=0; j<eSuckErrTotal; j++)
        {
            AlarmCode.printf("%d%03d%1d", eSuckAlarm, i, j);                            // code No. 60000 start i=kitsuck j=type//
            SEng.printf("\"%s%s\"", HSys.SuckPtr[i].Name, SuckErrorEngString[j]);
            SChi.printf("\"%s%s\"", HSys.SuckPtr[i].Name, SuckErrorChiString[j]);
            SDecE.sprintf("\"%s\"", SuckErrorEngProcess[j]);
            SDecC.sprintf("\"%s\"", SuckErrorChiProcess[j]);

            MyAlarmCodeStruct AlarmCodeList=MyAlarmCodeStruct(AlarmCode, int(eSuckAlarm), SEng, SChi, SDecE, SDecC, HSys.SuckPtr[i].FlushPanelName);
            HSys.mapAlarmCodeList[AlarmCode]=AlarmCodeList;
            HSys.mapNameToAlarm[AlarmCode]=AlarmCode;
            HSys.mapNameToAlarm[HSys.SuckPtr[i].AlarmName[j]]=AlarmCode;
            ComboBox1->Items->Add(AlarmCode);
//            fNote->SystemError->AddSysErr(AlarmCode+"="+AlarmCode, HSys.SuckPtr[i].FlushPanelName);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::SpeedButton3Click(TObject *Sender)               // auto create alarm  message //
{
    bInsertToDB=true;
    HSys.mapAlarmCodeList.clear();
    HSys.mapNameToAlarm.clear();

//    fNote->SystemError->SystemErrorCode->Clear();
    CreateSystemAlarmCode();                                                    // 80000 //    一定得在第一個執行
    CreateSystemSensorAlarmCode();                                              // 30000 //
    CreateCylinderAlarmCode();                                                  // 40000 //
    CreateMotorAlarmCode();                                                     // 50000 //
    CreateSuckAlarmCode();                                                      // 60000 //

    CreateNewJamErrorTable();
    ShowMessage("Create Alarm Message Finish!");
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::RecordAlarmMessage(MyRecordAlarmMessage R)       // 錯誤訊息存到log檔
{
    GetTimeInfo();
    //Event log
    AnsiString sDir=AnsiString().sprintf("D:\\HT-172_Log\\EventLog\\%04d_%02d", SystemYear, SystemMonth);
    AnsiString sFileName=AnsiString().sprintf("%s_%04d_%02d_%2d.csv", asHandlerID, SystemYear, SystemMonth, SystemDate);
    AnsiString sDefHead="Data, Time, Recovery, PauseTime, Duplicate, AlarmCode, Message, ErrorPart";
    SaveLogRpt(sDir,sFileName,sDefHead,R.CSVString());

    //Jam log
    if(CosFunction.bSaveJamCodeLog &&
       tJamAlarm.IsJamAlarm(R.AlarmCode.ToIntDef(0)))
    {
        sDir=AnsiString().sprintf("D:\\HT-172_Log\\JAMLog\\%04d_%02d", SystemYear, SystemMonth);
        sFileName=AnsiString().sprintf("%s_%04d_%02d_%2d.csv", asHandlerID, SystemYear, SystemMonth, SystemDate);
        SaveLogRpt(sDir,sFileName,sDefHead,R.CSVString());
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::UpdateLevelSetupMap(AnsiString FormName, AnsiString TypeName, AnsiString Value)
{
    mapLevelSetup[FormName+"_"+TypeName]=Value;
}
//---------------------------------------------------------------------------
bool TFormSysTools::CheckPassword(int iInLevel, AnsiString asInUserName, AnsiString asInPassword) //Daver add
{
    AnsiString asTmpUN,asTmpPW;
    for(int i=0; i<USER.RecordCT; i++)
    {
        if(iInLevel==USER.Level[i])
        {
            asTmpUN=asTmpUN.sprintf("%s", USER.ID[i]);
            asTmpPW=asTmpPW.sprintf("%s", USER.PassWord[i]);
            if((asInUserName==asTmpUN)&&(asInPassword==asTmpPW))
                return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void TFormSysTools::AddNoNeedHomeCheckList(TMySensor *RefSensor)
{
}
//---------------------------------------------------------------------------
void TFormSysTools::RefreshNoNeedHomeCheckList()
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::Button1Click(TObject *Sender)
{
    Memo1->Clear();
    for(HSys.IterNameToAlarm=HSys.mapNameToAlarm.begin(); HSys.IterNameToAlarm!=HSys.mapNameToAlarm.end(); HSys.IterNameToAlarm++)
    {
        Memo1->Lines->Add(HSys.IterNameToAlarm->first+AnsiString(" = ")+HSys.IterNameToAlarm->second);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::FormCreate(TObject *Sender)
{
    #ifdef SOFT_SIMULATE
        GroupBox1->Visible=true;
    #else
        GroupBox1->Visible=false;
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::Button4Click(TObject *Sender)
{
    ShowSystemError(HSys.Sen.SnSafeDoor_01.Name, K_RETRY);
    ShowSystemError("80101", K_RETRY);
    ShowSystemError(HSys.Cyn.C_Load_Up.ErrorName[eOnNotOnErr], K_RETRY);
}
//---------------------------------------------------------------------------
TAlarmManager::TAlarmManager() { FAlarmList.clear(); }                          //Stevenhong 20251231 : 力成要把JAM獨立額外生成
//---------------------------------------------------------------------------
TAlarmManager::~TAlarmManager() { FAlarmList.clear(); }                         //Stevenhong 20251231 : 力成要把JAM獨立額外生成
//---------------------------------------------------------------------------
void TAlarmManager::Clear() { FAlarmList.clear(); }                             //Stevenhong 20251231 : 力成要把JAM獨立額外生成
//---------------------------------------------------------------------------
int TAlarmManager::GetCount() { return FAlarmList.size(); }                     //Stevenhong 20251231 : 力成要把JAM獨立額外生成
//---------------------------------------------------------------------------
TAlarm* TAlarmManager::FindByCode(int Code)
{
    for (unsigned int i = 0; i < FAlarmList.size(); i++)
    {
        if (FAlarmList[i].AlarmCode == Code)
            return &FAlarmList[i];
    }
    return NULL;
}
//---------------------------------------------------------------------------
bool TAlarmManager::IsJamAlarm(int Code)
{
    TAlarm* p = FindByCode(Code);
    if(p==NULL) return false;
    if(p->Category.Pos("JAM")>0)
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TAlarmManager::ParseCSVLine(AnsiString Line, TAlarm &AlarmObj)
{
    std::vector<AnsiString> Fields;
    AnsiString currentField = "";
    bool inQuotes = false;
    for (int i = 1; i <= Line.Length(); i++)
    {
        char c = Line[i];
        if (c == '\"')
        {
            inQuotes = !inQuotes; // 切換引號狀態
        }
        else if (c == ',' && !inQuotes)
        {
            Fields.push_back(currentField.Trim());                              // 遇到逗號且不在引號內 -> 切割欄位
            currentField = "";
        }
        else
        {
            currentField += c;
        }
    }
    Fields.push_back(currentField.Trim());                                      // 加入最後一個欄位
    if (Fields.size() >= 7)                                                     // 填入結構 (依據你的 CSV 順序: Code, Category, Type, E_Msg, C_Msg, E_Desc, C_Desc)
    {
        AlarmObj.AlarmCode     = Fields[0].ToIntDef(0);
        AlarmObj.Category      = Fields[1];
        AlarmObj.AlarmType     = Fields[2].ToIntDef(0);
        AlarmObj.E_ErrMessage  = Fields[3];
        AlarmObj.C_ErrMessage  = Fields[4];
        AlarmObj.E_Description = Fields[5];
        AlarmObj.C_Description = Fields[6];
    }
}
//---------------------------------------------------------------------------
bool TAlarmManager::LoadFromFile(AnsiString FileName)
{
    if (!FileExists(FileName)) return false;
    Clear();
    TStringList *sl = new TStringList();
    try
    {
        sl->LoadFromFile(FileName);
        for (int i = 1; i < sl->Count; i++)                                     // 從 i=1 開始 (跳過標題行)
        {
            AnsiString line = sl->Strings[i].Trim();
            if (line.IsEmpty()) continue;
            TAlarm tempAlarm;
            ParseCSVLine(line, tempAlarm);                                      // 解析
            FAlarmList.push_back(tempAlarm);                                    // 加入清單
        }
    }
    __finally
    {
        delete sl;
    }
    return true;
}
//---------------------------------------------------------------------------
bool TAlarmManager::LoadFromFile()
{
    return LoadFromFile(sJamTableFile());
}
//---------------------------------------------------------------------------
void TFormSysTools::SaveLogRpt(AnsiString sLogPath,AnsiString sLogName,AnsiString sDefHead,AnsiString sMsg)
{
    FileInfo().EnsureDirectoriesExist(sLogPath);
    try
    {
        AnsiString sLogPathName=FileInfo().PathCombin(sLogPath,sLogName);
        if(FileExists(sLogPathName))
        {
        }
        else
        {
            WriteDataToFile(sLogPathName , sDefHead);
        }
        WriteDataToFile(sLogPathName , sMsg);
    }
    catch(...)
    {
        RecordProcess("Exception RecordAlarmMessage");
    }
}
//---------------------------------------------------------------------------
