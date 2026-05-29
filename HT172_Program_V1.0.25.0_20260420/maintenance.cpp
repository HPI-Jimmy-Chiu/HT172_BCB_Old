
//------------------------------------------------------------------------------
#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "maintenance.h"
#pragma package(smart_init)
#pragma link "ALed"
#pragma resource "*.dfm"

#define MaxIOBack 512
TfMaintenance *fMaintenance;
//------------------------------------------------------------------------------
int ipbTempCount,iMaxLevelItem,iMaxShapeBin;
bool bMusicOn[4]={false,false,false,false};
bool bChangeMotorDatabase=false;
bool bChangeIODatabase=false;
bool bRead34970AData=false;
bool bBackSwitchPortData[MaxIOBack];                                                  // ben add 20110803 output backup //
bool bBackCylinderPortData[MaxIOBack];                                                // ben add 20110803 output backup //
bool bBackSuckPortData[4][MAX_SUCKER_ROW][MAX_SUCKER_COL][2];                   //V1.6

bool bFlag[2]={false,false};
//------------------------------------------------------------------------------
__fastcall TfMaintenance::TfMaintenance(TComponent* Owner)
    : TForm(Owner)
{
//    pcMaintenance->Top=-30;                                                      // 隱藏標頭 //
    InitEnableArea();
    InitAreaPass();
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::FormCreate(TObject *Sender)                      // ben add 20110801 //
{
    Left=(1280-Width)/2;
    Top =(1024-Height)/2;
    for(int i=0; i<pcMaintenance->PageCount; i++)
        pcMaintenance->Pages[i]->TabVisible=false;
    spbTrayDef->Click();
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;
    Top =0;
    fShow=true;
    fFunctionProcess=false;
    Height=1015;
    ShowMotorSimulateSpeed();
    spbTowerLight->OnClick(spbTowerLight);
    spbTowerLight->Down=true;

    sbSecsGem->Visible=(USE_SECS_GEM>0);                                        //Steven 20240619 : Add for SECS/GEM
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
    AnsiString S;
    HSys.DecStopAllMotor();

    S=fMain->cb_WorkFile->Text;                                                 // ben edit 20110714 //
    S=S.SubString(1, S.LastDelimiter("."));//Eliot 2008_02_13
    S=S+"TES";

    SetMotorSpeed();
    SavePassword();
    S=HSys.CurrentDir+AnsiString("\\system\\maintance.ini");                    // save form data //
    SaveWorkFile(S);

    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::ShowMotorSimulateSpeed()                         // 顯示馬達模擬速度 //
{
    StringGrid2->Cells[0][0]="No.";
    StringGrid2->Cells[1][0]="Soft Speed";

    for(int i=0;i<HSys.iTotalMotor;i++)
    {
        if(HSys.MotPtr[i]==NULL)
            continue;
        StringGrid2->Cells[0][i+1]=HSys.MotPtr[i]->Alias;
        StringGrid2->Cells[1][i+1]=HSys.MotPtr[i]->SimulateSpeed;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::spbMotorTestClick(TObject *Sender)              // open motor test form //
{
    if(HSys.Sys.SystemStart)
        return;
    AnsiString Str;
    int ret;

    Str.sprintf("確定是否要進入Motor Teach");
    ret=ShowMyMessageBox_YES_NO(Str);

    if(ret==TMyMessageBox::msgrtnYES)
    {
        RecordProcess("Enter Motor Test");
        EventReport(SECS_EVENT.EnterMotorTest);
        fMotorTest->ShowModal();
        fAllMotorHome=false;
        LastClickButton->Down=true;
        LastClickButton->OnClick(LastClickButton);
        fSpeed->ResetAllMotorSpeed();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::edA18_DelayTimeChange(TObject *Sender)
{
//    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2);
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::spbExitClick(TObject *Sender)
{
    LastClickButton->Down=true;
    LastClickButton->OnClick(LastClickButton);
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::spbTowerLightClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    pcMaintenance->ActivePageIndex=Ptr->Tag-1;

    LastClickButton=(TSpeedButton *)Sender;
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::spbIoMonitorClick(TObject *Sender)               // ben edit 20110803 backup and restore output //
{
    if(HSys.Sys.SystemStart)
        return;
    RecordProcess("Enter IO");
    EventReport(SECS_EVENT.EnterIOPage);
    BackUpOutputData();
    fiosetview->ShowModal();
    LevelRecordProcess();
    int ret;
    if(fiosetview->bOutDataChange)
    {
        if(fiosetview->bNoRestoreIO==true)
        {
            ret=ShowMyMessageBox_YES_NO("IO already change,want to restore?");
            if(ret==TMyMessageBox::msgrtnYES)
            {
                RecordProcess("Restore IO");
                RestoreOutputData();
            }
            else
            {
                RecordProcess("No Restore IO");
            }
        }
        else
        {
            if(HasICUnderMachine()==true)
            {
                ShowMyMessage("IO already change, will restore IO!!");
                RestoreOutputData();
            }
            else
            {
                ShowMyMessageBox_YES_NO("IO already change,want to restore?");
                if(ret==TMyMessageBox::msgrtnYES)
                {
                    RecordProcess("Restore IO");
                    RestoreOutputData();
                }
                else
                {
                    RecordProcess("No Restore IO");
                }
            }
        }
    }
    LastClickButton->Down=true;
    LastClickButton->OnClick(LastClickButton);
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::spbAlignmentClick(TObject *Sender)              // ben edit 20110803 backup and restore output //
{
    if(HSys.Sys.SystemStart)
        return;
    AnsiString Str=AnsiString().sprintf("確定是否要進入Teach");
    int ret=ShowMyMessageBox_YES_NO(Str);

    if(ret==TMyMessageBox::msgrtnYES)
    {
        RecordProcess("Enter Teach");
        EventReport(SECS_EVENT.EnterTeach);
        BackUpOutputData();
        fTeach->ShowModal();

        LastClickButton->Down=true;
        LastClickButton->OnClick(LastClickButton);
        fSpeed->ResetAllMotorSpeed();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::CheckListBox1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    CheckListBox1->ItemIndex=-1;
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::SpeedButton21Click(TObject *Sender)
{
    iTmpSelectLevel =1;     //預設Engineer
    RadioButton2->Checked=true;
    sbDeletePassword->Visible=false;
    SpeedButton21->Visible=false;
    Panel20->Visible=true;
    MaskEdit1->Text="";
    Edit14->Text="";
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::SpeedButton22Click(TObject *Sender)
{
    if((Edit14->Text!="")&&(MaskEdit1->Text!=""))
        AppendPassword(Edit14->Text,MaskEdit1->Text,iTmpSelectLevel);
    ShowPassword();
    sbDeletePassword->Visible=true;
    SpeedButton21->Visible=true;
    Panel20->Visible=false;
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::SpeedButton23Click(TObject *Sender)
{
    sbDeletePassword->Visible=true;
    SpeedButton21->Visible=true;
    Panel20->Visible=false;
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::SaveWorkFile(AnsiString S)                       // Save Maintenance Parameter //
{
    FormSysTools->OpenFormData(S);
    for(int iP=0; iP<pcMaintenance->ControlCount; iP++)
    {
        TControl *P=pcMaintenance->Controls[iP];
        TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);
        if(TabPtr!=NULL)
        {
            FormSysTools->SaveFormData(TabPtr, TabPtr->Name, true);
        }
    }
    FormSysTools->CloseFormData();
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::OpenWorkFile()                                   // Load Maintenance Parameter //
{
    AnsiString S;
    S=HSys.CurrentDir+AnsiString("\\system\\maintance.ini");
    if( CheckFileExist(S.c_str()))
    {
        FormSysTools->OpenFormData(S);
        for(int iP=0; iP<pcMaintenance->ControlCount; iP++)
        {
            TControl *P=pcMaintenance->Controls[iP];
            TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);
            if(TabPtr!=NULL)
            {
                FormSysTools->LoadFormData(TabPtr, TabPtr->Name, true);
            }
        }
        FormSysTools->CloseFormData();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::ShowPassword()                                   // Daver add - s //
{
    AnsiString asUserData,asUserLevel;
    CheckListBox1->Items->Clear();
    for (int i=0; i<USER.RecordCT; i++)
    {
        switch (USER.Level[i])
        {
            case 0 : asUserLevel="Operator";   break;
            case 1 : asUserLevel="Engineer";   break;
            case 2 : asUserLevel="Supervisor"; break;
            case 3 : asUserLevel="HonPrec";    break;
            default : asUserLevel="Operator";
        }
        asUserData.sprintf("[%02d]  %s   %s    ",i+1,asUserLevel,USER.ID[i]);
        CheckListBox1->Items->Add(asUserData);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::AppendPassword(AnsiString asID, AnsiString asPassword, int iLevel)
{
    AnsiString asTmpStr;
    bool bFindexists=false;
    int iFindNo;
    for (int i=0; i<USER.RecordCT; i++)
    {
        asTmpStr.sprintf("%s",USER.ID[i]);
        if (asID==asTmpStr)
        {
            iFindNo=i;
            bFindexists=true;
            break;
        }
    }
    if (bFindexists)
    {
        strcpy(USER.ID[iFindNo],asID.c_str() );
        strcpy(USER.PassWord[iFindNo],asPassword.c_str() );
        USER.Level[iFindNo]=iLevel;
    }
    else;
    {
        USER.RecordCT++;
        if (USER.RecordCT>=30)
        {
            USER.RecordCT--;
            return;
        }
        strcpy(USER.ID[USER.RecordCT-1],asID.c_str() );
        strcpy(USER.PassWord[USER.RecordCT-1],asPassword.c_str() );
        USER.Level[USER.RecordCT-1]=iLevel;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::DeletePassword()
{
    PASS_WORD TmpUser;
    TmpUser.RecordCT=0;
    for (int i=0; i<USER.RecordCT; i++)
    {
        if (!CheckListBox1->Checked[i])
        {
            TmpUser.RecordCT++;
            strcpy(TmpUser.ID[TmpUser.RecordCT-1],       USER.ID[i]);
            strcpy(TmpUser.PassWord[TmpUser.RecordCT-1],USER.PassWord[i]);
            TmpUser.Level[TmpUser.RecordCT-1]=USER.Level[i];
        }
    }
    memcpy(&USER,&TmpUser,sizeof(PASS_WORD));
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::sbDeletePasswordClick(TObject *Sender)
{
    DeletePassword();
    ShowPassword();
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::RadioButton2Click(TObject *Sender)
{
    TRadioButton *Ptr = (TRadioButton *)Sender;
    iTmpSelectLevel=Ptr->Tag-1;
}
//------------------------------------------------------------------------------// Daver add - e //
void __fastcall TfMaintenance::RGB00Click(TObject *Sender)                      // ben add 20110712 //
{                                                                               // 三色燈狀態 //
    TALed *Ptr;
    Ptr=(TALed *) Sender;
    int i=Ptr->Tag/3;
    int j=Ptr->Tag%3;
    if(i>7 || i<0 || j>2 || j<0)
        return;

    if( Ptr->Value==true && Ptr->Blink==false )                                 // 恆亮 //
    {
        Ptr->Value=true;
        Ptr->Blink=true;
    }
    else if( Ptr->Value==true && Ptr->Blink==true )                             // 閃爍 //
    {
        Ptr->Value=false;
        Ptr->Blink=false;
    }
    else                                                                        // 恆滅 //
    {
        Ptr->Value=true;
        Ptr->Blink=false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::sbMusic1Click(TObject *Sender)                   // 音樂測試 //
{                                                                               // ben add 20110713 //
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    if( bMusicOn[Ptr->Tag-1] )
    {
        CloseBuzzerOff();
        bMusicOn[Ptr->Tag-1]=false;
    }
    else
    {
        CloseBuzzerOff();
        for(int i=0; i<4; i++)
            bMusicOn[i]=false;
        HSys.SwPtr[HSys.Sw.SwMusic1.Tag+(Ptr->Tag-1)].On();
        bMusicOn[Ptr->Tag-1]=true;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::DataSource3DataChange(TObject *Sender,
      TField *Field)
{
    if( bChangeMotorDatabase )
    {
        HSys.LoadMotorParameterFromDataBase();
        ShowMotorSimulateSpeed();
    }
    if( bChangeIODatabase )
        HSys.InitialHardwareNameAndLoadDatabaseIO();
}
//------------------------------------------------------------------------------
void TfMaintenance::BackUpOutputData()                                          //V1.6
{
    for(int i=0;i<HSys.iTotalSwitch;i++)
    {
        if(i<MaxIOBack)
            bBackSwitchPortData[i]=HSys.SwPtr[i].OutValue;
    }

    for(int i=0; i<HSys.iTotalSucker; i++)
    {
        for(int iR=0; iR<HSys.SuckPtr[i].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[i].MaxItemC; iC++)
            {
                bBackSuckPortData[i][iR][iC][0]=HSys.SuckPtr[i].Suck[iR][iC].OnSw.OutValue;
                bBackSuckPortData[i][iR][iC][1]=HSys.SuckPtr[i].Suck[iR][iC].OffSw.OutValue;
            }
        }
    }

    for(int i=0; i<HSys.iTotalCylinder; i++)
    {
        if(i<MaxIOBack)
            bBackCylinderPortData[i]=HSys.CynPtr[i].Switch.OutValue;
    }
}
//------------------------------------------------------------------------------
void TfMaintenance::RestoreOutputData()                                         //V1.6
{
    for(int i=0; i<HSys.iTotalSwitch; i++)
    {
        if(i<MaxIOBack)
            HSys.SwPtr[i].OnOff(bBackSwitchPortData[i]);
    }

    for(int i=0; i<HSys.iTotalSucker; i++)
    {
        for(int iR=0; iR<HSys.SuckPtr[i].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[i].MaxItemC; iC++)
            {
                HSys.SuckPtr[i].Suck[iR][iC].OnSw.OnOff(bBackSuckPortData[i][iR][iC][0]);
                HSys.SuckPtr[i].Suck[iR][iC].OffSw.OnOff(bBackSuckPortData[i][iR][iC][1]);
            }
        }
    }

    for(int i=0; i<HSys.iTotalCylinder; i++)
    {
        if(i<MaxIOBack)
            HSys.CynPtr[i].Switch.OnOff(bBackCylinderPortData[i]);
    }
}
//------------------------------------------------------------------------------
void TfMaintenance::SaveData()
{
    AnsiString S=HSys.CurrentDir+AnsiString("\\system\\maintance.ini");                    // save form data //
    SaveWorkFile(S);
}
//---------------------------------------------------------------------------
int TfMaintenance::GetRunCheckTimeout()
{
    int iret=0;
    iret=atoi((edRunCheckTimeout->Text).c_str());
    if(iret>3000)
    {
        iret=3000;
        edRunCheckTimeout->Text="3000";
    }
    return iret;
}
//---------------------------------------------------------------------------
bool TfMaintenance::EnableUnloaderTrayDataFilePath()
{
    bool bret=0;
    bret=cbN03->Checked;
    return bret;
}
//---------------------------------------------------------------------------
AnsiString TfMaintenance::GetUnloaderTrayDataFilePath()
{
    AnsiString sret=0;
    sret=ed_N03_Path_Upload->Text;
    return sret;
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::sbSecsGemClick(TObject *Sender)
{
    if(HSys.Sys.SystemStart)
        return;
    RecordProcess("Enter SECSGEM");
    EventReport(SECS_EVENT.EnterSECSPage);
    FSECS->Show();

    LastClickButton->Down=true;
    LastClickButton->OnClick(LastClickButton);
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::edA01_CCDDown_IPMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_NO_SYMBOL);
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::edA01_CCDDown_PortMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1000, 9999);
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::edA04MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 0.0, 5.0);
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::edA08_SetPersentSpeedMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1, 100);
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::Panel18MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight)
    {
        if(bFlag[0]==false)
        {
            bFlag[0]=true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::Panel17MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight)
    {
        if(bFlag[0]==true)
        {
//            palMotorTest->Visible=false;
        }
        else
        {
//            palMotorTest->Visible=true;
            bFlag[0]=false;
            bFlag[1]=false;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::spbComPortClick(TObject *Sender)
{
    fComPort->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::strngrdTraySelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    iSelTrayRow=ARow;
    iSelTrayCol=ACol;
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::strngrdTrayDblClick(TObject *Sender)
{
    btnModifyTray->Click();
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::btnModifyTrayClick(TObject *Sender)
{
    if(iSelTrayRow>0 && iSelTrayCol>=0)
    {
        edtTemp->Text=strngrdTray->Cells[iSelTrayCol][iSelTrayRow];
        if(strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Package Type"))!=0 ||
           strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Group"))!=0     ||
           strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Memo"))!=0)
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_NO_SYMBOL);
        }
        else if(strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Columns (X)"))!=0 ||
                strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Rows (Y)"))!=0     ||
                strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("BlockNumberX"))!=0 ||
                strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("BlockNumberY"))!=0)
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_INTEGER, 0, true, 0, 1000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_DOUBLE, 2, true, 0.00, 1000.00);
        }
        strngrdTray->Cells[iSelTrayCol][iSelTrayRow]=edtTemp->Text;
    }
    btnModifyTray->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::btnAddTrayClick(TObject *Sender)
{
    int iRow=strngrdTray->RowCount;
    strngrdTray->RowCount=iRow+1;
    for(int j=0; j<strngrdTray->ColCount; j++)
    {
        strngrdTray->Cells[j][iRow]="";
    }
    btnAddTray->Down=false;
    strngrdTray->Row=strngrdTray->RowCount-1;
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::btnDeleteTrayClick(TObject *Sender)
{
    if(iSelTrayRow<=0)
        return;
    Tag=atoi(strngrdTray->Cells[0][iSelTrayRow].c_str());
    for(int i=iSelTrayRow; i<strngrdTray->RowCount-1; i++)
    {
        for(int j=0; j<strngrdTray->ColCount; j++)
        {
            strngrdTray->Cells[j][i]=strngrdTray->Cells[j][i+1];
        }
    }
    strngrdTray->RowCount=strngrdTray->RowCount-1;
    btnDeleteTray->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::sbUpdateTrayClick(TObject *Sender)
{
    TStringList *sList=new TStringList();
    TStringList *SL=new TStringList();

    for(int i=0; i<strngrdTray->RowCount; i++)
    {
        SL->Clear();
        for(int j=0; j<strngrdTray->ColCount; j++)
        {
            SL->Add(strngrdTray->Cells[j][i].Trim());
        }
        sList->Add(SL->CommaText);
    }

    sList->SaveToFile(HSys.TrayTablePath);
    sList->Clear();
    SL->Clear();
    delete sList;
    delete SL;

    sbtReloadTray->Click();
    sbUpdateTray->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::sbtReloadTrayClick(TObject *Sender)
{
    TStringList *List=new TStringList();
    AnsiString S1, S2;
    int iPos=0;

    strngrdTray->ColCount=16;
    strngrdTray->Font->Size=10;
    strngrdTray->DefaultColWidth=80;
    strngrdTray->ColWidths[0]=200;

    for(int i=0; i<strngrdTray->RowCount; i++)
    {
        for(int j=0; j<16; j++)
            strngrdTray->Cells[j][i]="";
    }

    List->LoadFromFile(HSys.TrayTablePath);
    strngrdTray->RowCount=List->Count;

    for(int i=0; i<List->Count; i++)
    {
        int j=0;
        S1=List->Strings[i];
        S1=StringReplace(S1, "\"", "", TReplaceFlags()<<rfReplaceAll);
        do
        {
            iPos=S1.AnsiPos(",");
            if(iPos>0)
            {
                S2=S1.SubString(0, iPos-1);
                S1=S1.SubString(iPos+1, S1.Length());
                strngrdTray->Cells[j][i]=S2;
                j++;
            }
        }while(iPos>0);
    }
    if(strngrdTray->RowCount>1)
        strngrdTray->FixedRows=1;
    strngrdTray->FixedCols=0;
    List->Clear();
    delete List;
    sbtReloadTray->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMaintenance::edS01_MagXMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 100);     //Sam 20240619 : 速度獨立設定
}
//---------------------------------------------------------------------------
void TfMaintenance::InitEnableArea()
{
    AnsiString str="";
    int iWidthSingle=140;
    int iHeightSingle=30;
    int numOfCol=gbEnableArea->Width/iWidthSingle;
    cbEnableArea[eBinNotUse]          =new TCheckBox(this);
    for(int iArea=1; iArea<eTrayCount; iArea++)
    {
        str.sprintf("cbEnableArea%02d", iArea);
        cbEnableArea[iArea]          =new TCheckBox(this);
        cbEnableArea[iArea]->Name    =str;
        cbEnableArea[iArea]->Parent  =gbEnableArea;
        cbEnableArea[iArea]->Left    =iWidthSingle*((iArea-1)%numOfCol)+20;
        cbEnableArea[iArea]->Top     =iHeightSingle*((iArea-1)/numOfCol)+20;
        cbEnableArea[iArea]->Width   =iWidthSingle;
        cbEnableArea[iArea]->Height  =iHeightSingle;
        cbEnableArea[iArea]->Caption ="Enable "+fMain->sTrayNameSpace[iArea];
        cbEnableArea[iArea]->Tag     =iArea;
        cbEnableArea[iArea]->Checked =true;
    }
}
//------------------------------------------------------------------------------
void TfMaintenance::InitAreaPass()
{
    for(int iArea=1; iArea<eTrayCount; iArea++)
    {
        if(iArea==eFix1 ||
           iArea==eFix2)
        {
            CloseAreaPass(cbEnableArea[iArea]);
        }
        else if(iArea>=eMag1_01 && iArea<=eMag1_27)
        {
            if(MAGAZINE_CNT==1)
            {
                CloseAreaPass(cbEnableArea[iArea]);
            }
            else
            {
                //Open
            }
        }
        else if(iArea>=eMag2_01 && iArea<=eMag2_27)
        {
        }
        else if(iArea>=eMag3_01 && iArea<=eMag3_27)
        {
            if(MAGAZINE_CNT==1)
            {
                CloseAreaPass(cbEnableArea[iArea]);
            }
            else
            {
                //Open
            }
        }
    }
}
//------------------------------------------------------------------------------
void TfMaintenance::CloseAreaPass(TCheckBox *cbarea)
{
    cbarea->Enabled=false;
    cbarea->Checked=false;
}
//------------------------------------------------------------------------------
void TfMaintenance::CtrlAllAreaPass(bool bOn)
{
    for(int iArea=1; iArea<eTrayCount; iArea++)
    {
        if(iArea==eFix1 ||
           iArea==eFix2)
        {
            continue;
        }
        else if(iArea>=eMag1_01 && iArea<=eMag1_27)
        {
            if(MAGAZINE_CNT==1)
            {
                continue;
            }
            else
            {
                //Open
            }
        }
        else if(iArea>=eMag2_01 && iArea<=eMag2_27)
        {
        }
        else if(iArea>=eMag3_01 && iArea<=eMag3_27)
        {
            if(MAGAZINE_CNT==1)
            {
                continue;
            }
            else
            {
                //Open
            }
        }
        cbEnableArea[iArea]->Checked=bOn;
    } 
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::btnP_PassAllClick(TObject *Sender)
{
    CtrlAllAreaPass(true);
}
//------------------------------------------------------------------------------
void __fastcall TfMaintenance::edA02MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 10, true, 10, 300);     //Sam 20240619 : 速度獨立設定
}
//------------------------------------------------------------------------------

