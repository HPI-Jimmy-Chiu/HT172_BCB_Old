#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "uteach.h"
#pragma package(smart_init)
#pragma link "ALed"
#pragma resource "*.dfm"
TfTeach *fTeach;
int SelMotSpeed=0;
int TECH_MAX_ITEM=0;
//---------------------------------------------------------------------------
TECH_PARA::TECH_PARA(TTrayMotor *Mot, TGroupBox *GroupBox, bool Visible)
{
    MotorSelect     =Mot->Tag;
    gbPanel=GroupBox;
    gbPanel     ->Visible       =Visible;
    bbtGo=false;
    bfunButton=false;
    for(int iP=0; iP<gbPanel->ControlCount; iP++)
    {
        TControl *P=gbPanel->Controls[iP];

        TSpeedButton *BtnPtr=dynamic_cast <TSpeedButton *>(P);
        TEdit        *EdPtr =dynamic_cast <TEdit        *>(P);

        if(BtnPtr!=NULL)
        {
            if(BtnPtr->Caption.AnsiPos("GO")==1)
            {
                btGo=BtnPtr;
                btGo        ->Visible       =Visible;
                btGo        ->OnClick       =fTeach->btTeachGo000Click;
                bbtGo=true;
            }
            else
            {
                funButton=BtnPtr;
                funButton   ->Visible       =Visible;
                funButton   ->OnClick       =fTeach->sbTeach000Click;
                bfunButton=true;
            }
        }
        else if(EdPtr!=NULL)
        {
            SetEdit=EdPtr;
            SetEdit->Visible       =Visible;
            SetEdit->OnMouseDown   =fTeach->edTrayShiftStageX_LoaderMouseDown;
        }
    }
}
//---------------------------------------------------------------------------
__fastcall TfTeach::TfTeach(TComponent* Owner)
    : TForm(Owner)
{
    ActiveMotorIndex=-1;
    fShow=false;
    bInArmAllUp=false;
    bMgzArmAllUp=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::FormCreate(TObject *Sender)
{
    for(int i=0; i<PageTeach->PageCount; i++)
        PageTeach->Pages[i]->TabVisible=false;

    SetTechPara(PageTeach);

    TECH_MAX_ITEM=TechPara.size();
    for(int i=0; i<TECH_MAX_ITEM; i++)
    {
        if(TechPara[i]->bfunButton)
        {
            TechPara[i]->funButton->GroupIndex=1+i;
            TechPara[i]->funButton->AllowAllUp=true;
            TechPara[i]->funButton->Tag=i;
        }
        if(TechPara[i]->bbtGo)
        {
            TechPara[i]->btGo->GroupIndex=1+i;
            TechPara[i]->btGo->AllowAllUp=true;
            TechPara[i]->btGo->Tag=i;
        }
    }
}
//---------------------------------------------------------------------------
void TfTeach::SetTechPara(TWinControl *PCtrl)
{
    AnsiString S="",S1="";
    TPageControl *PagePtr   =dynamic_cast <TPageControl   *>(PCtrl);
    TPanel       *PalPtr    =dynamic_cast <TPanel         *>(PCtrl);
    TTabSheet    *TabPtr    =dynamic_cast <TTabSheet      *>(PCtrl);
    TGroupBox    *BoxPtr    =dynamic_cast <TGroupBox      *>(PCtrl);
    if(PagePtr!=NULL || TabPtr!=NULL || PalPtr!=NULL)
    {
        for(int iP=0; iP<PCtrl->ControlCount; iP++)
        {
            TControl *P=PCtrl->Controls[iP];
            if(dynamic_cast <TWinControl *>(P)!=NULL)
            {
                SetTechPara((TWinControl *) P);
            }
        }
    }
    else if(BoxPtr!=NULL)
    {
        for(int i=0; i<HSys.iTotalMotor; i++)
        {
            if(BoxPtr->Name.AnsiPos(HSys.MotPtr[i]->Alias)==3)
            {
                TechPara.push_back(new TECH_PARA(HSys.MotPtr[i], BoxPtr));
                break;
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTeach::FormShow(TObject *Sender)                              // set teach form data //
{                                                                               // need edit by user //
    fShow=true;
    Left =30;
    Top =0;
    //**************************************************************************// 以下依使用者表單設定修改 //
    for(int i=0; i<PageTeach->PageCount; i++)
        PageTeach->Pages[i]->TabVisible=false;
    sbTeachChange0->Click();
    bStartHome=false;
    for(int i=0;i<HSys.iTotalMotor;i++)
    {
        if(HSys.MotPtr[i]!=NULL)
            HSys.MotPtr[i]->ClearLock();
    }
    bOutDataChange=false;
    fAllMotorHome=false;

    if(HSys.FuncT.iT06_AutoRowType==1)
        palAutoRow->Visible=true;
    else
        palAutoRow->Visible=false;
    //
    chbLayerMag1->Clear();
    for(int iArea=eMag1_01; iArea<=eMag1_27; iArea++)
    {
        chbLayerMag1->Items->Add(fMain->sTrayNameSpace[iArea]);
    }
    chbLayerMag1->ItemIndex=0;
    chbLayerMag2->Clear();
    for(int iArea=eMag2_01; iArea<=eMag2_27; iArea++)
    {
        chbLayerMag2->Items->Add(fMain->sTrayNameSpace[iArea]);
    }
    chbLayerMag2->ItemIndex=0;
    chbLayerMag3->Clear();
    for(int iArea=eMag3_01; iArea<=eMag3_27; iArea++)
    {
        chbLayerMag3->Items->Add(fMain->sTrayNameSpace[iArea]);
    }
    chbLayerMag3->ItemIndex=0;
    if(MAGAZINE_CNT==1)
    {
        tsMag1->TabVisible=false;
        tsMag2->TabVisible=true;
        tsMag3->TabVisible=false;
    }
    else if(MAGAZINE_CNT==2)
    {
        tsMag1->TabVisible=true;
        tsMag2->TabVisible=true;
        tsMag3->TabVisible=false;
    }
    else
    {
        tsMag1->TabVisible=true;
        tsMag2->TabVisible=true;
        tsMag3->TabVisible=true;
    }
    //
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfTeach::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    for(int i=0;i<HSys.iTotalMotor;i++)                                         // server on off //
    {
        if(HSys.MotPtr[i]->ReadServoAlarmOn()==true && HSys.MotPtr[i]->Led[iServoOn]==false)
            HSys.MotPtr[i]->ServoOnOff(Server_On);
    }
    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfTeach::spbSetNowPositionClick(TObject *Sender)                // set now position //
{
    EditPtr->Text=edNowPosition->Text;
}
//==============================================================================
void __fastcall TfTeach::spbSetNowEncoderClick(TObject *Sender)                 // set now encoder //
{
    EditPtr->Text=edNowPosition->Text;
}
//==============================================================================
void __fastcall TfTeach::ReleaseOtherMotionButton(TSpeedButton *P)              // release other button //
{
    P->Down=true;
    if(P!=sbMove)
        sbMove->Down=false;
    if(P!=sbMoveP)
        sbMoveP->Down=false;
    if(P!=sbMoveN)
        sbMoveN->Down=false;
    if(P!=sbHome)
        sbHome->Down=false;
    if(P!=sbJogN)                                                         // ben 20110704 add - s //
        sbJogN->Down=false;
    if(P!=sbJogP)
        sbJogP->Down=false;                                               // ben 20110704 add - e //
    HSys.DecStopAllMotor();
    SleepEx(100,false);
}
//==============================================================================
void __fastcall TfTeach::OpenWorkFile()                                         // Load Teach Parameter //
{
    AnsiString S=HSys.CurrentDir+AnsiString("\\system\\tech.ini");
    if(CheckFileExist(S.c_str()) )
    {
        FormSysTools->OpenFormData(S);
        //**********************************************************************// 以下依使用者表單設定 //
        for(int iP=0; iP<PageTeach->ControlCount; iP++)
        {
            TControl *P=PageTeach->Controls[iP];

            TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);

            if(TabPtr!=NULL)
            {
                FormSysTools->LoadFormData(TabPtr, TabPtr->Name);
            }
        }
        //**********************************************************************// 以上依使用者表單設定 //
        FormSysTools->CloseFormData();
    }
}
//==============================================================================
void __fastcall TfTeach::SaveWorkFile(AnsiString S)                             // Save Teach Parameter //
{
    FormSysTools->OpenFormData(S);
    //**************************************************************************// 以下依使用者表單設定修改 //
    for(int iP=0; iP<PageTeach->ControlCount; iP++)
    {
        TControl *P=PageTeach->Controls[iP];

        TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);

        if(TabPtr!=NULL)
        {
            FormSysTools->SaveFormData(TabPtr, TabPtr->Name);
        }
    }
    //**************************************************************************// 以上依使用者表單設定修改 //
    FormSysTools->CloseFormData();
}
//==============================================================================
bool  __fastcall TfTeach::CheckCanMove(int iMot)                                // check motor can move or not //
{                                                                               // need edit by user //
    int iEmg=IsEMGPressed();
    if(iEmg>0)
    {
        ShowSystemError(HSys.SenPtr[iEmg].Name, K_RETRY);
        return false;
    }
    ScanAllMotorStatus();

    if(fTeach->fShow==false)                                                    //KenHsieh 20240904 : add 馬達安全檢查
    {
        if(iMot!=-1)
            ActiveMotorIndex=iMot;
        else
            return false;
    }

    if(ActiveMotorIndex==HSys.Mot.Magazine1Z->Tag)
    {
        if(HSys.Cyn.C_MGZ1_LayerTrayPush.IsOn() || HSys.Cyn.C_MGZ1_LayerTrayPush.GetOutBit()==true)
        {
            ShowMyMessage("C_MGZ1_LayerTrayPush must off");
            return false;
        }
        if(HSys.Sen.SnMGZ1TrayPositionR.IsOn() || HSys.Sen.SnMGZ1TrayPositionF.IsOn() || HSys.Sen.SnMGZ1TrayDirection.IsOn())
        {
            ShowMyMessage("Mag 1 Bottom sensor is On");
            return false;
        }
    }
    else if(ActiveMotorIndex==HSys.Mot.Magazine2Z->Tag)
    {
        if(HSys.Cyn.C_MGZ2_LayerTrayPush.IsOn()|| HSys.Cyn.C_MGZ2_LayerTrayPush.GetOutBit()==true)
        {
            ShowMyMessage("C_MGZ2_LayerTrayPush must off");
            return false;
        }
        if(HSys.Sen.SnMGZ2TrayPositionR.IsOn() || HSys.Sen.SnMGZ2TrayPositionF.IsOn() || HSys.Sen.SnMGZ2TrayDirection.IsOn())
        {
            ShowMyMessage("Mag 2 Bottom sensor is On");
            return false;
        }
    }
    else if(ActiveMotorIndex==HSys.Mot.Magazine3Z->Tag)
    {
        if(HSys.Cyn.C_MGZ3_LayerTrayPush.IsOn() || HSys.Cyn.C_MGZ3_LayerTrayPush.GetOutBit()==true)
        {
            ShowMyMessage("C_MGZ3_LayerTrayPush must off");
            return false;
        }
        if(HSys.Sen.SnMGZ3TrayPositionR.IsOn() || HSys.Sen.SnMGZ3TrayPositionF.IsOn() || HSys.Sen.SnMGZ3TrayDirection.IsOn())
        {
            ShowMyMessage("Mag 3 Bottom sensor is On");
            return false;
        }
    }
    else if(ActiveMotorIndex==HSys.Mot.SortArm1X->Tag || ActiveMotorIndex==HSys.Mot.SortArm1Y->Tag)
    {
        for(int i=0; i<8; i++)
        {
            if(HSys.MotPtr[HSys.Mot.SortArm1ZA->Tag+i]->Led[iHomeLed]==false)
            {
                ShowMyMessage("Sort arm 1 z must at home position!");
                return false;
            }
        }
    }
    else if(ActiveMotorIndex==HSys.Mot.SortArm2X->Tag || ActiveMotorIndex==HSys.Mot.SortArm2Y->Tag)
    {
        for(int i=0; i<8; i++)
        {
            if(HSys.MotPtr[HSys.Mot.SortArm2ZA->Tag+i]->Led[iHomeLed]==false)
            {
                ShowMyMessage("Sort arm 2 z must at home position!");
                return false;
            }
        }
        if(HSys.Cyn.C_SortArm2_UpDown.IsOn() || HSys.Cyn.C_SortArm2_UpDown.GetOutBit()==true)
        {
            ShowMyMessage("C_SortArm2_UpDown need Up when MagArm Move!");
            return false;
        }
    }
    else if(ActiveMotorIndex==HSys.Mot.Magazine1X->Tag)
    {
        if(HSys.Cyn.C_MGZ1_SidePush_Up.IsOff()==false)
        {
            ShowMyMessage("C_MGZ1_SidePush_Up need down when Magazine1X move!");
            return false;
        }
        if(HSys.Cyn.C_MGZ1_TrayPush.IsOn()==true)
        {
            ShowMyMessage("C_MGZ1_TrayPush need pop when Magazine1X move!");
            return false;
        }
        if(HSys.Cyn.C_MGZ1_EdgeFixer.IsOff()==false)
        {
            ShowMyMessage("C_MGZ1_EdgeFixer need pop when Magazine1X move!");
            return false;
        }
    }
    else if(ActiveMotorIndex==HSys.Mot.Magazine2X->Tag)
    {
        if(HSys.Cyn.C_MGZ2_SidePush_Up.IsOff()==false)
        {
            ShowMyMessage("C_MGZ1_SidePush_Up need down when Magazine2X move!");
            return false;
        }
        if(HSys.Cyn.C_MGZ2_TrayPush.IsOn()==true)
        {
            ShowMyMessage("C_MGZ2_TrayPush need pop when Magazine2X move!");
            return false;
        }
        if(HSys.Cyn.C_MGZ2_EdgeFixer.IsOff()==false)
        {
            ShowMyMessage("C_MGZ2_EdgeFixer need pop when Magazine2X move!");
            return false;
        }
    }
    else if(ActiveMotorIndex==HSys.Mot.Magazine3X->Tag)
    {
        if(HSys.Cyn.C_MGZ3_SidePush_Up.IsOff()==false)
        {
            ShowMyMessage("C_MGZ1_SidePush_Up need down when Magazine3X move!");
            return false;
        }
        if(HSys.Cyn.C_MGZ3_TrayPush.IsOn()==true)
        {
            ShowMyMessage("C_MGZ3_TrayPush need pop when Magazine3X move!");
            return false;
        }
        if(HSys.Cyn.C_MGZ3_EdgeFixer.IsOff()==false)
        {
            ShowMyMessage("C_MGZ3_EdgeFixer need pop when Magazine3X move!");
            return false;
        }
    }
    return true;
}
//==============================================================================
void __fastcall TfTeach::sbJogPMouseDown(TObject *Sender,                 // Jog P //
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CheckCanMove()==false || ActiveMotorIndex==-1 )
        return;
    ReleaseOtherMotionButton(sbJogP);
    SelMotSpeed=atoi(edTeachSpeed->Text.c_str());
    HSys.MotPtr[ActiveMotorIndex]->SetSpeed(SelMotSpeed);
    HSys.MotPtr[ActiveMotorIndex]->JogP();
}
//==============================================================================
void __fastcall TfTeach::sbJogNMouseDown(TObject *Sender,                 // Jog N //
      TMouseButton Button, TShiftState Shedift, int X, int Y)
{
    if(CheckCanMove()==false || ActiveMotorIndex==-1 )
        return;
    ReleaseOtherMotionButton(sbJogN);
    SelMotSpeed=atoi(edTeachSpeed->Text.c_str());
    HSys.MotPtr[ActiveMotorIndex]->SetSpeed(SelMotSpeed);
    HSys.MotPtr[ActiveMotorIndex]->JogN();
}
//==============================================================================
void __fastcall TfTeach::sbJogPMouseUp(TObject *Sender,                   // stop jog P or N //
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveMotorIndex==-1)
        return;
    HSys.MotPtr[ActiveMotorIndex]->DecStop();
}
//==============================================================================
void __fastcall TfTeach::sbTeach000Click(TObject *Sender)                       // teach button select //
{
    int Tag,i;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;

    ActiveMotorIndex=TechPara[Tag]->MotorSelect;
    EditPtr=TechPara[Tag]->SetEdit;
    UpdateMotorTeachMonitor();
    for(i=0; i<TECH_MAX_ITEM; i++)
    {
        if(i==Tag)
            TechPara[i]->funButton->Down=true;
        else
            TechPara[i]->funButton->Down=false;
    }
    sbMoveP->Down=false;
    sbMoveN->Down=false;
    sbHome->Down=false;
    sbMove->Down=false;
}
//==============================================================================
void __fastcall TfTeach::UpdateMotorTeachMonitor()                              // show motor data //
{
    scbTeachSpeed->Min=1;
    scbTeachSpeed->Max=HSys.MotPtr[ActiveMotorIndex]->GetJogHighSpeed();                //2011.05.03    klutter
    scbTeachSpeed->Min=HSys.MotPtr[ActiveMotorIndex]->GetJogLowSpeed();              // ben edit 20110817 //
    int Speed=scbTeachSpeed->Position;
    HSys.MotPtr[ActiveMotorIndex]->SetSpeed(Speed);
    if(Speed>HSys.MotPtr[ActiveMotorIndex]->GetJogHighSpeed())
        Speed=HSys.MotPtr[ActiveMotorIndex]->GetJogHighSpeed();
    if(Speed<HSys.MotPtr[ActiveMotorIndex]->GetJogLowSpeed())
        Speed=HSys.MotPtr[ActiveMotorIndex]->GetJogLowSpeed();
    edTeachSpeed->Text=HSys.MotPtr[ActiveMotorIndex]->GetSpeed();
    palMotorName->Caption=HSys.MotPtr[ActiveMotorIndex]->NumberAlias;
}
//==============================================================================
void __fastcall TfTeach::scbTeachSpeedChange(TObject *Sender)                   // change speed scroll bar //
{
    if(ActiveMotorIndex==-1)
        return;
    int Speed=scbTeachSpeed->Position;
    HSys.MotPtr[ActiveMotorIndex]->SetSpeed(Speed);
    edTeachSpeed->Text=HSys.MotPtr[ActiveMotorIndex]->GetSpeed();
}
//==============================================================================
void __fastcall TfTeach::ScanNowMotorStatus()                                   // show motor status led //
{
    static TALed *pLed[iMotLedTotalCnt]={ledCW, ledHome, ledCCW, ledEmg, ledAlarm, ledSoftCW, ledSoftCCW, ledServoAlarm, ledInPos, ledZPhase, ledServoOn};
    if(ActiveMotorIndex==-1)
    {
        for(int i=0; i<iMotLedTotalCnt; i++)
            pLed[i]->Value=false;
        return;
    }
    HSys.MotPtr[ActiveMotorIndex]->ScanMotorStatus();
    HSys.MotPtr[ActiveMotorIndex]->ReadPos();                                   //20110817 Daver add
    edNowPosition->Text = HSys.MotPtr[ActiveMotorIndex]->Position;// ben edit 20110802 //
    edNowEncoder->Text  = HSys.MotPtr[ActiveMotorIndex]->EncoderPosition; // ben edit 20110802 //

    for(int i=0; i<iMotLedTotalCnt; i++)
        pLed[i]->Value=HSys.MotPtr[ActiveMotorIndex]->Led[i];
}
//==============================================================================
void __fastcall TfTeach::Timer1Timer(TObject *Sender)                           // timer 1 //
{
    if( fShow==false )
        return;
    static bool bTimerRunning=false;
    if(InitialOK==false || bTimerRunning==true)
        return;
    bTimerRunning=true;
    //
    ScanNowMotorStatus();
    ScanLed(fTeach);

    if(fNote->fShow ||
       MyMessageBox->fShow)                                       // ben edit 20110817 //
    {
        sbHome->Down=false;
        bStartHome=false;
        sbMoveP->Down=false;
        sbMoveN->Down=false;
    }
    if(DoZHome()==false || DoMgzArmZHome()==false)
    {
        bTimerRunning=false;
        return;
    }
    if(sbHome->Down && bStartHome)
    {
        AnsiString sErr="";
        if(HSys.MotPtr[ActiveMotorIndex]->Home(sErr))
        {
            edHomeDistance->Text= -(int)(HSys.MotPtr[ActiveMotorIndex]->GetGearRatio()*HSys.MotPtr[ActiveMotorIndex]->GetLastHomePos()); // ben add 20110802 //
            sbHome->Down=false;
            bStartHome=false;
        }
    }
    if(sbMove->Down ||
       sbMoveP->Down ||
       sbMoveN->Down)
    {
        if(HSys.MotPtr[ActiveMotorIndex]->CheckSoftLimit(iMovePos)==false)
        {
            sbMove->Down=false;
            sbMoveP->Down=false;
            sbMoveN->Down=false;
        }
        else if(HSys.MotPtr[ActiveMotorIndex]->MotorMove(iMovePos))                  // BEN ADD 20111102 //
        {
            sbMove->Down=false;
            sbMoveP->Down=false;
            sbMoveN->Down=false;
        }
    }
    bTimerRunning=false;
}
//==============================================================================
void __fastcall TfTeach::btTeachGo000Click(TObject *Sender)                     // go button select //
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    int Tag,iNewPos;
    Tag=Ptr->Tag;

    ActiveMotorIndex=TechPara[Tag]->MotorSelect;
    if(CheckCanMove()==false)
        return;
    TechPara[Tag]->funButton->Down=false;

    Ptr->Down=false;
    if( HSys.MotPtr[ActiveMotorIndex]->GetEnable()==false)
        return;
    if( HSys.MotPtr[ActiveMotorIndex]->bHomeFlag==false )
    {
        ShowRecordMessage("Motor not home ,can not move");
        return;
    }
    //************************************************************************//
    //                             define by user                             //
    //************************************************************************//
    EditPtr=TechPara[Tag]->SetEdit;
    HSys.MotPtr[ActiveMotorIndex]->SetSpeed(scbTeachSpeed->Position);
    UpdateMotorTeachMonitor();
    iNewPos=atoi(EditPtr->Text.c_str());

    if(iNewPos>HSys.MotPtr[ActiveMotorIndex]->GetSoftLimitP())
    {
        ShowRecordMessage("Over max soft limit,abort process !");
        return;
    }
    if(iNewPos<HSys.MotPtr[ActiveMotorIndex]->GetSoftLimitN())
    {
        ShowRecordMessage("Bellow min soft limit,abort process !");
        return;
    }
    HSys.MotPtr[ActiveMotorIndex]->SetSpeed(scbTeachSpeed->Position);
    HSys.MotPtr[ActiveMotorIndex]->MotorMove(iNewPos);

}
//==============================================================================
void __fastcall TfTeach::sbMovePClick(TObject *Sender)                    // move P //
{
    if(CheckCanMove()==false || ActiveMotorIndex==-1)
        return;
    ReleaseOtherMotionButton(sbMoveP);
    int P1=atoi(edNowPosition->Text.c_str());                              // ben edit 20110802 - s //
    int P2=atoi(cbMoveDistance->Text.c_str());
    iMovePos=P1+P2;
    HSys.MotPtr[ActiveMotorIndex]->MotorMove(iMovePos);                         // ben edit 20110802 - e //
    if(HSys.MotPtr[ActiveMotorIndex]->bErrorMove)
        sbMoveP->Down=false;
}
//==============================================================================
void __fastcall TfTeach::sbMoveNClick(TObject *Sender)                    // move N //
{
    if(CheckCanMove()==false || ActiveMotorIndex==-1 )
        return;
    ReleaseOtherMotionButton(sbMoveN);
    int P1=atoi(edNowPosition->Text.c_str());                              // ben edit 20110802 - s //
    int P2=atoi(cbMoveDistance->Text.c_str());
    iMovePos=P1-P2;
    HSys.MotPtr[ActiveMotorIndex]->MotorMove(iMovePos);                         // ben edit 20110802 - e //
    if(HSys.MotPtr[ActiveMotorIndex]->bErrorMove)
        sbMoveN->Down=false;
}
//==============================================================================
void __fastcall TfTeach::sbHomeClick(TObject *Sender)                    // button home //
{
    bStartHome=false;
    if(CheckCanMove()==false)
    {
        sbHome->Down=false;
        return;
    }
    if(ActiveMotorIndex==-1)
    {
        sbHome->Down=false;
        return;
    }
    HSys.DecStopAllMotor();
    sbMove->Down=false;
    sbMoveP->Down=false;
    sbMoveN->Down=false;
    if(sbHome->Down)
    {
        HSys.MotPtr[ActiveMotorIndex]->InitHomeTask();
        fHome->iHomeStep=1;
        bStartHome=true;
    }
}
//==============================================================================
void __fastcall TfTeach::sbMoveClick(TObject *Sender)                     // button move //
{
    if(CheckCanMove()==false || ActiveMotorIndex==-1)
        return;
    ReleaseOtherMotionButton(sbMove);
    iMovePos=atoi(edMoveTo->Text.c_str());                                // ben edit 20110802 //
    HSys.MotPtr[ActiveMotorIndex]->MotorMove(iMovePos);
    if(HSys.MotPtr[ActiveMotorIndex]->bErrorMove)
        sbMove->Down=false;
    edMoveTo->Text=iMovePos;                                      // ben edit 20110802 //
}
//==============================================================================
void __fastcall TfTeach::sbSetClick(TObject *Sender)                     // button set to //
{
    EditPtr->Text=edHomeDistance->Text;
}
//==============================================================================
void __fastcall TfTeach::sbMotorToolClick(TObject *Sender)                    // buttom motortool //
{
    fMotorTest->ShowModal();
    fAllMotorHome=false;
    sbMotorTool->Down=false;
}
//==============================================================================
void __fastcall TfTeach::sbIOFormClick(TObject *Sender)                     // button io form /
{
    fiosetview->ShowModal();
    sbIOForm->Down=false;
}
//==============================================================================
void __fastcall TfTeach::sbStopClick(TObject *Sender)                     // button stop //
{
    sbMove->Down=false;
    sbMoveP->Down=false;
    sbMoveN->Down=false;
    sbHome->Down=false;
    HSys.DecStopAllMotor();
    SleepEx(100,false);
}
//==============================================================================
void __fastcall TfTeach::edTeachSpeedChange(TObject *Sender)                    // speed edit change //
{
    if(ActiveMotorIndex==-1)
        return;
    int Speed=atoi(edTeachSpeed->Text.c_str());
    if(Speed>scbTeachSpeed->Max)
        Speed=scbTeachSpeed->Max;
    if(Speed<scbTeachSpeed->Min)
        Speed=scbTeachSpeed->Min;
    scbTeachSpeed->Position=Speed;
    HSys.MotPtr[ActiveMotorIndex]->SetSpeed(Speed);
    edTeachSpeed->Text=HSys.MotPtr[ActiveMotorIndex]->GetSpeed();
}
//==============================================================================
void __fastcall TfTeach::sbTeachExitClick(TObject *Sender)                      // button exit //
{
    Close();
}
//==============================================================================
void __fastcall TfTeach::sbTeachSaveClick(TObject *Sender)                      // button save //
{
    AnsiString S=HSys.CurrentDir+AnsiString("\\system\\tech.ini");
    SaveWorkFile(S);
    UpdateAllParameter();                                                       // ben add 20110803 update all //
    fAllMotorHome=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::sbTeachChange0Click(TObject *Sender)                   // button change tabsheet //
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    PageTeach->ActivePageIndex=Ptr->Tag-1;
    palTitle->Caption=Ptr->Caption;
}
//==============================================================================
void __fastcall TfTeach::edMoveToClick(TObject *Sender)                            // ben add 20110802 //
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::FormDestroy(TObject *Sender)
{
    for(vector<TECH_PARA *>::iterator iter=TechPara.begin(); iter!=TechPara.end(); ++iter)
    {
        delete *iter;
    }
    TechPara.clear();
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::edTrayShiftStageX_LoaderMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::spbServoOnClick(TObject *Sender)
{
    spbServoOn->Down=false;
    if(ActiveMotorIndex==-1)
        return;
    HSys.MotPtr[ActiveMotorIndex]->ServoOnOff(!HSys.MotPtr[ActiveMotorIndex]->Led[iServoOn]);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSetAllSortArm1ZClick(TObject *Sender)
{
    int iZ[8];
    TEdit *teSortArm1[]={edSortArm1ZA_Offset, edSortArm1ZB_Offset, edSortArm1ZC_Offset, edSortArm1ZD_Offset,
                         edSortArm1ZE_Offset, edSortArm1ZF_Offset, edSortArm1ZG_Offset, edSortArm1ZH_Offset};

    for(int i=0; i<8; i++)
        teSortArm1[i]->Text=HSys.MotPtr[HSys.Mot.SortArm1ZA->Tag+i]->ReadPos();

    int iStandZ=atoi(edSortArm1ZD_Offset->Text.c_str());

    for(int i=0; i<8; i++)
    {
        iZ[i]=atoi(teSortArm1[i]->Text.c_str())-iStandZ;
        teSortArm1[i]->Text=iZ[i];
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSetAllSortArm2ZClick(TObject *Sender)
{
    int iZ[8];
    TEdit *teSortArm2[]={edSortArm2ZA_Offset, edSortArm2ZB_Offset, edSortArm2ZC_Offset, edSortArm2ZD_Offset,
                         edSortArm2ZE_Offset, edSortArm2ZF_Offset, edSortArm2ZG_Offset, edSortArm2ZH_Offset};

    for(int i=0; i<8; i++)
        teSortArm2[i]->Text=HSys.MotPtr[HSys.Mot.SortArm2ZA->Tag+i]->ReadPos();

    int iStandZ=atoi(edSortArm2ZD_Offset->Text.c_str());

    for(int i=0; i<8; i++)
    {
        iZ[i]=atoi(teSortArm2[i]->Text.c_str())-iStandZ;
        teSortArm2[i]->Text=iZ[i];
    }
}
//---------------------------------------------------------------------------
bool TfTeach::DoZHome()
{
    if(bInArmAllUp==false)
        return true;
    bool bSortArmWait=false;
    AnsiString sErr="";
    for(int iR=0; iR<MAX_ARM_Row; iR++)
    {
        for(int iC=0; iC<MAX_ARM_Col; iC++)
        {
            if(bInArmZHome[iR][iC]==false)
            {
                bInArmZHome[iR][iC]=SortArmPara->MSortArmZ[iR][iC]->Home(sErr);
                bSortArmWait=true;
            }
        }
    }
    if(bSortArmWait)
        btnInZAllUp->Caption="Homeing...";
    else
        btnInZAllUp->Caption="Sort1 Z All Up";

    if(bSortArmWait==false)
    {
        bInArmAllUp=false;
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------
bool TfTeach::DoMgzArmZHome()
{
    if(bMgzArmAllUp==false)
        return true;
    bool bSortArm2Wait=false;
    AnsiString sErr="";
    for(int iR=0; iR<MAX_ARM_Row; iR++)
    {
        for(int iC=0; iC<MAX_ARM_Col; iC++)
        {
            if(bMgzArmZHome[iR][iC]==false)
            {
                bMgzArmZHome[iR][iC]=MagArmPara->MMagArmZ[iR][iC]->Home(sErr);
                bSortArm2Wait=true;
            }
        }
    }
    if(bSortArm2Wait)
        btnMgzZAllUp->Caption="Homeing...";
    else
        btnMgzZAllUp->Caption="Sort2 Z All Up";

    if(bSortArm2Wait==false)
    {
        bMgzArmAllUp=false;
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnInZAllUpClick(TObject *Sender)
{
    bInArmAllUp=false;
    for(int iR=0; iR<MAX_ARM_Row; iR++)
    {
        for(int iC=0; iC<MAX_ARM_Col; iC++)
        {
            SortArmPara->MSortArmZ[iR][iC]->InitHomeTask();
            bInArmZHome[iR][iC]=false;
        }
    }
    bInArmAllUp=true;
    bStartHome=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnMgzZAllUpClick(TObject *Sender)
{
    bMgzArmAllUp=false;
    for(int iR=0; iR<MAX_ARM_Row; iR++)
    {
        for(int iC=0; iC<MAX_ARM_Col; iC++)
        {
            MagArmPara->MMagArmZ[iR][iC]->InitHomeTask();
            bMgzArmZHome[iR][iC]=false;
        }
    }
    bMgzArmAllUp=true;
    bStartHome=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnLayerMag1GoClick(TObject *Sender)
{
    TButton *Ptr=(TButton *)Sender;
    TComboBox *chbLayer;
    TMagazineModule *Mag;
    AnsiString Name=Ptr->Name;
    if(Name.Pos("btnLayerMag1Go")>0)
    {
        ActiveMotorIndex=HSys.Mot.Magazine1Z->Tag;
        chbLayer=chbLayerMag1;
        Mag=MagazineModule1;
    }
    else if(Name.Pos("btnLayerMag2Go")>0)
    {
        ActiveMotorIndex=HSys.Mot.Magazine2Z->Tag;
        chbLayer=chbLayerMag2;
        Mag=MagazineModule2;
    }
    else if(Name.Pos("btnLayerMag3Go")>0)
    {
        ActiveMotorIndex=HSys.Mot.Magazine3Z->Tag;
        chbLayer=chbLayerMag3;
        Mag=MagazineModule3;
    }
    else
    {
        return;
    }
    if(CheckCanMove()==false)
        return;
    if( HSys.MotPtr[ActiveMotorIndex]->bHomeFlag==false )
    {
        ShowRecordMessage("Motor not home ,can not move");
        return;
    }
    //************************************************************************//
    //                             define by user                             //
    //************************************************************************//
    UpdateMotorTeachMonitor();
    int iLayerIndex=chbLayer->ItemIndex;
    int iTargetPos=Mag->GetLayerPosition(iLayerIndex);
    if(iTargetPos==0)
    {
        ShowRecordMessage("Magzine Z target pos is zero. Error");
        return;
    }
    if(HSys.MotPtr[ActiveMotorIndex]->CheckSoftLimit(iTargetPos)==false)
    {
    }
    else if(HSys.MotPtr[ActiveMotorIndex]->MotorMove(iTargetPos))                  // BEN ADD 20111102 //
    {
    }
}
//---------------------------------------------------------------------------

