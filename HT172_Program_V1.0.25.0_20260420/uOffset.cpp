//------------------------------------------------------------------------------
#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "uOffset.h"
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfOffset *fOffset;
int iOffsetItem;
//------------------------------------------------------------------------------
void __fastcall TfOffset::SetEditOnMouse( TWinControl *PCtrl )
{
    for (int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P = PCtrl->Controls[iP];
        if (dynamic_cast <TPanel *>(P) != NULL          ||
            dynamic_cast <TPageControl *>(P) != NULL    ||
            dynamic_cast <TTabSheet *>(P) != NULL       ||
            dynamic_cast <TTabControl *>(P) != NULL     ||
            dynamic_cast <TForm *>(P) != NULL           ||
            dynamic_cast <TPage *>(P) != NULL           ||
            dynamic_cast <TGroupBox*>(P) != NULL        )
        {
            SetEditOnMouse((TWinControl *) P);                                  // 找該物件附屬的物件(遞迴)
        }

        TEdit *PEdit=dynamic_cast <TEdit *>(P);
        TLabeledEdit *LEdit=dynamic_cast <TLabeledEdit *>(P);
        if(PEdit!=NULL)
        {
            PEdit->OnMouseDown=edMagazine2Z_TopPositionMouseDown;
        }
        else if(LEdit!=NULL)
        {
            LEdit->OnMouseDown=edMagazine2Z_TopPositionMouseDown;
        }
    }
}
//------------------------------------------------------------------------------
__fastcall TfOffset::TfOffset(TComponent* Owner)                                // need edit by user //
    : TForm(Owner)
{
    //**************************************************************************// 以下依使用者需求設定修改 //

    //**************************************************************************// 以上依使用者需求設定修改 //
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::FormCreate(TObject *Sender)
{
    for(int i=0; i<PageOffset->PageCount; i++)
        PageOffset->Pages[i]->TabVisible=false;

    SetEditOnMouse(fOffset);
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::OpenWorkFile()                                        // Load Offset Parameter //
{
    AnsiString S;
    S=HSys.CurrentDir+AnsiString("\\data\\")+fMain->cb_WorkFile->Text+AnsiString(".ofs");
    if( CheckFileExist(S.c_str()) )
    {
        FormSysTools->OpenFormData(S);
        for(int iP=0; iP<PageOffset->ControlCount; iP++)
        {
            TControl *P=PageOffset->Controls[iP];

            TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);

            if(TabPtr!=NULL)
            {
                FormSysTools->LoadFormData(TabPtr, TabPtr->Name);
            }
        }
        FormSysTools->CloseFormData();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::SaveWorkFile()                                        // Save Offset Parameter //
{
    AnsiString S;
    S=HSys.CurrentDir+AnsiString("\\data\\")+fMain->cb_WorkFile->Text+AnsiString(".ofs");
    FormSysTools->OpenFormData(S);
    for(int iP=0; iP<PageOffset->ControlCount; iP++)
    {
        TControl *P=PageOffset->Controls[iP];

        TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);

        if(TabPtr!=NULL)
        {
            FormSysTools->SaveFormData(TabPtr, TabPtr->Name);
        }
    }
    FormSysTools->CloseFormData();
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::sbOffsetUpdateClick(TObject *Sender)                  // save //
{
    SaveWorkFile();                                                             // save data to ini //
    UpdateAllParameter();                                                       // ben add 20110803 update all //
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::sbOffsetExitClick(TObject *Sender)                    // exit //
{
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::sbOffsetChange0Click(TObject *Sender)                 //切換PageControl1頁面
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    PageOffset->ActivePageIndex=Ptr->Tag-1;
    palOffsetCaption->Caption=Ptr->Caption;
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::SetToTech(TEdit *TechPtr,TEdit *OffsetPtr)            // 條 //
{
    int Target,Source;
    Source=atoi(OffsetPtr->Text.c_str());
    Target=atoi(TechPtr->Text.c_str());

    Target+=Source;
    Source=0;
    OffsetPtr->Text=Source;
    TechPtr->Text=Target;
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::SetTouMTech( TEdit *TechPtr,TEdit *OffsetPtr)         // um //
{
    int Target,Source;
    Source=Get0_001MMType(OffsetPtr->Text.c_str());
    Target=Get0_001MMType(TechPtr->Text.c_str());
    Target+=Source;
    Source=0;
    OffsetPtr->Text=ConvertTouMType(Source);
    TechPtr->Text=ConvertTouMType(Target);
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::sbOffsetReAligmentClick(TObject *Sender)              // set to tech //
{            
    int ret=ShowMyMessageBox_YES_NO("Sure to write ?");
    if(ret==TMyMessageBox::msgrtnNO)
        return;
    //**************************************************************************// 以下依使用者表單設定修改 //
//    SetToTech( fTeach->edTeach000 ,edOffset000);
    //**************************************************************************// 以上依使用者表單設定修改 //

    AnsiString S=HSys.CurrentDir+AnsiString("\\system\\tech.ini");                         // save tech.ini //
    fTeach->SaveWorkFile(S);
    SaveWorkFile();                                                             // save *.ofs //
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::FormShow(TObject *Sender)
{
    fShow=true;
    Left=(1920-Width)/2;
    Top =0;
    sbOffsetChange0->Click();
    OpenWorkFile();

    if(HSys.Mot.PushStoreArmX1->GetEnable() ||
       HSys.Mot.PushStoreArmX2->GetEnable() ||
       HSys.Mot.PushStoreArmX3->GetEnable() ||
       HSys.Mot.PushStoreArmX4->GetEnable())                                    //KenHsieh 20240830 : 推Tray 改為馬達不給調整Tray Arm offset
    {
        pnlStoreArmOfs->Visible=false;
        pnlPushStoreArmOfs->Visible=true;
    }
    else
    {
        pnlStoreArmOfs->Visible=true;
        pnlPushStoreArmOfs->Visible=false;
    }

    if(MAGAZINE_CNT==1)                                                         //Jimmychiu 20260303 : 僅開啟有用的畫面
    {
        pnlMag1Offset->Visible=false;
        pnlMag2Offset->Visible=true;
        pnlMag3Offset->Visible=false;
    }
    else if(MAGAZINE_CNT==2)
    {
        pnlMag1Offset->Visible=true;
        pnlMag2Offset->Visible=true;
        pnlMag3Offset->Visible=false;
    }
    else
    {
        pnlMag1Offset->Visible=true;
        pnlMag2Offset->Visible=true;
        pnlMag3Offset->Visible=true;
    }
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::FormClose(TObject *Sender, TCloseAction &Action)
{
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfOffset::edMagazine2Z_TopPositionMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, -500, 500);
}
//------------------------------------------------------------------------------

