#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "uspeed.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSpeed *fSpeed;
//---------------------------------------------------------------------------
//AI(general) 20260521 : machine-level speed file, independent from recipe/work file
static AnsiString GetMachineSpeedFileName()
{
    return HSys.CurrentDir+AnsiString("\\system\\machine_speed.ini");
}
//---------------------------------------------------------------------------
static AnsiString GetRecipeSpeedFileName()
{
    return HSys.CurrentDir+AnsiString("\\data\\")+fMain->cb_WorkFile->Text+AnsiString(".ini");
}
//---------------------------------------------------------------------------
__fastcall TMySpeedPanel::TMySpeedPanel(TTrayMotor *Mot) : TComponent(Owner)    //Steven 20121127 : 改用動態排列速度面板
{
    AnsiString str;
    palMotorSpeed   =new TPanel(this);
    labMotorSpeed   =new TLabel(this);
    edtMotorSpeed   =new TEdit(this);
    scbMotorSpeed   =new TScrollBar(this);

    iPersent=&Mot->iPersentSpeed;

    str.sprintf("palMotorSpeed_%s", Mot->Alias.c_str());
    palMotorSpeed->Name     =str;
    palMotorSpeed->Parent   =fSpeed->ScrollBox1;
    palMotorSpeed->Caption  ="";
    palMotorSpeed->Color    =TColor(0x00C2B8A6);
    palMotorSpeed->Height   =43;
    palMotorSpeed->Left     =16;
    palMotorSpeed->Width    =573;
    palMotorSpeed->Tag      =Mot->Tag;

    str.sprintf("labMotorSpeed_%s", Mot->Alias.c_str());
    labMotorSpeed->Name     =str;
    labMotorSpeed->Parent   =palMotorSpeed;
    labMotorSpeed->Height   =27;
    labMotorSpeed->Left     =15;
    labMotorSpeed->Top      =9;
    labMotorSpeed->Width    =146;
    labMotorSpeed->Font->Size=14;
    labMotorSpeed->Caption  =Mot->Alias;
    labMotorSpeed->Tag      =Mot->Tag;

    str.sprintf("edtMotorSpeed_%s", Mot->Alias.c_str());
    edtMotorSpeed->Name     =str;
    edtMotorSpeed->Text     =1;
    edtMotorSpeed->Parent   =palMotorSpeed;
    edtMotorSpeed->Height   =24;
    edtMotorSpeed->Left     =225;
    edtMotorSpeed->Top      =9;
    edtMotorSpeed->Width    =65;
    edtMotorSpeed->OnClick  =edtMotorSpeedClick;
    edtMotorSpeed->Tag      =Mot->Tag;

    str.sprintf("scbMotorSpeed_%s", Mot->Alias.c_str());
    scbMotorSpeed->Name     =str;
    scbMotorSpeed->Position =1;
    scbMotorSpeed->Parent   =palMotorSpeed;
    scbMotorSpeed->Height   =23;
    scbMotorSpeed->Left     =295;
    scbMotorSpeed->Top      =10;
    scbMotorSpeed->Width    =256;
    scbMotorSpeed->Max      =100;
    scbMotorSpeed->Min      =1;
    scbMotorSpeed->OnChange =scbMotorSpeedChange;
    scbMotorSpeed->Tag      =Mot->Tag;

    bEnable=Mot->GetEnable();
}
//---------------------------------------------------------------------------
__fastcall TMySpeedPanel::~TMySpeedPanel()                                      //Steven 20121127 : 改用動態排列速度面板
{
    iPersent=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TMySpeedPanel::scbMotorSpeedChange(TObject *Sender)             //Steven 20121127 : 改用動態排列速度面板
{
    edtMotorSpeed->Text=AnsiString(scbMotorSpeed->Position);
    *iPersent=scbMotorSpeed->Position;
    SetMotorSpeed();
}
//---------------------------------------------------------------------------
void __fastcall TMySpeedPanel::edtMotorSpeedClick(TObject *Sender)              //Steven 20121127 : 改用動態排列速度面板
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1, 100);
    scbMotorSpeed->Position=atoi(edtMotorSpeed->Text.c_str());
    *iPersent=scbMotorSpeed->Position;
    SetMotorSpeed();
}
//---------------------------------------------------------------------------
__fastcall TfSpeed::TfSpeed(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::FormCreate(TObject *Sender)
{
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        MySpeedPanel.push_back(new TMySpeedPanel(HSys.MotPtr[i]));
    }

    int iMaxMotorItem=MySpeedPanel.size();
    //決定哪些不可以調整並隱藏

    #ifndef SOFT_SIMULATE
    for(int i=0; i<iMaxMotorItem; i++)
    {
        if(MySpeedPanel[i]->bEnable)
        {
            MySpeedPanel[i]->palMotorSpeed->Visible=true;
        }
        else
        {
             MySpeedPanel[i]->palMotorSpeed->Visible=false;
        }
    }
    #endif

    //重新排列位置 Start
    int iPitch=52, iStart=2;
    for(int i=0; i<iMaxMotorItem; i++)      //Steven 20121127 : 改用動態排列速度面板
    {
        #ifndef SOFT_SIMULATE
        if(MySpeedPanel[i]->bEnable && MySpeedPanel[i]->palMotorSpeed->Visible)
        #else
        if(MySpeedPanel[i]->palMotorSpeed->Visible)
        #endif
        {
            MySpeedPanel[i]->palMotorSpeed->Top=iStart;
            iStart+=iPitch;
        }
    }
    //重新排列位置 End
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::FormDestroy(TObject *Sender)
{
    for(vector<TMySpeedPanel *>::iterator iter=MySpeedPanel.begin(); iter!=MySpeedPanel.end(); ++iter)
    {
        delete *iter;
    }
    MySpeedPanel.clear();
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::FormShow(TObject *Sender)
{
    fShow=true;
    Left=(1280-Width)/2;                                                        // 1024->1280 ben //
    Top =20;
    FormSysTools->SetSubPlaneAsNoFrame((TWinControl *)ScrollBox1);                  //20111014 terry//

    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629 : 擺在最下面,不要動
}
//---------------------------------------------------------------------------
void TfSpeed::SetFixedSpeed()
{

}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::OpenWorkFile()                                         // Load Speed Parameter //
{
    AnsiString S=GetMachineSpeedFileName();
    bool bNeedCreateMachineSpeedFile=false;

    if(CheckFileExist(S.c_str())==false)
    {
        S=GetRecipeSpeedFileName();
        bNeedCreateMachineSpeedFile=true;
    }

    if(CheckFileExist(S.c_str()))
    {
        FormSysTools->OpenFormData(S);
        FormSysTools->LoadFormData(ScrollBox1, "Speed");
        FormSysTools->CloseFormData();

        if(bNeedCreateMachineSpeedFile)
        {
            SaveWorkFile();
        }
    }

    SetFixedSpeed();
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::SaveWorkFile()                                         // Save Speed Parameter //
{
    SetFixedSpeed();

    AnsiString sDir=HSys.CurrentDir+AnsiString("\\system");
    if(DirectoryExists(sDir)==false)
    {
        ForceDirectories(sDir);
    }

    AnsiString S=GetMachineSpeedFileName();
    FormSysTools->OpenFormData(S);
    FormSysTools->SaveFormData(ScrollBox1, "Speed");
    FormSysTools->CloseFormData();
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false; // V2.1 START

    SaveWorkFile();

    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629 : 擺在最下面,不要動
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbAddSpeedClick(TObject *Sender)
{
    GroupSpeedAddSub(eAddSpeed);
    SetMotorSpeed();
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbSubSpeedClick(TObject *Sender)
{
    GroupSpeedAddSub(eSubSpeed);
    SetMotorSpeed();
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::spbFullSpeedClick(TObject *Sender)
{
    GroupSpeedAddSub(eFullSpeed);
    SetMotorSpeed();
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::AddSpeed(TScrollBar *Sender)
{
    Sender->Position+=10;
    if(Sender->Position>100)
        Sender->Position=100;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::SubSpeed( TScrollBar *Sender)
{
    if(Sender->Position>20)
        Sender->Position-=10;
    else if(Sender->Position>10)
        Sender->Position-=5;
    else if(Sender->Position>0)
        Sender->Position-=1;
}
//---------------------------------------------------------------------------
void __fastcall TfSpeed::GroupSpeedAddSub(eSpeedType flag)
{
    int i, j;
    TScrollBox  *PCtrl=ScrollBox1;
    TWinControl *P1;
    TControl *P, *P2;
    for(i=0; i<PCtrl->ControlCount; i++)  //第一層
    {
        P=PCtrl->Controls[i];
        if(dynamic_cast <TPanel *>(P)!=NULL) //找出所有Group第二層內的Panel
        {
            P1=(TWinControl *)P;
            for(j=0; j<P1->ControlCount; j++)    //找出所有Group第二層Panel內的ScrollBar
            {
                P2=P1->Controls[j];
                TScrollBar *PScrollBar=dynamic_cast <TScrollBar *>(P2);
                if(PScrollBar!=NULL && PScrollBar->Name!="")
                {
                    //Disable的就不用動
                    if(dynamic_cast <TPanel *>(P)->Enabled==false)
                    {
//                        int QQ=0;
                    }
                    //不顯示的就設定為100 %
                    else if(dynamic_cast <TPanel *>(P)->Visible==false)
                    {
                        PScrollBar->Position=100;
                        AddSpeed(PScrollBar);
                    }
                    else
                    {
                        if(flag==eAddSpeed)
                            AddSpeed(PScrollBar);
                        else if(flag==eFullSpeed)
                        {
                            PScrollBar->Position=100;
                            AddSpeed(PScrollBar);
                        }
                        else
                            SubSpeed(PScrollBar);
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfSpeed::ResetAllMotorSpeed()
{
    OpenWorkFile();
    for(unsigned int i=0; i<MySpeedPanel.size(); i++)
    {
        *MySpeedPanel[i]->iPersent=MySpeedPanel[i]->scbMotorSpeed->Position;
    }
    SetMotorSpeed();
}
//---------------------------------------------------------------------------


