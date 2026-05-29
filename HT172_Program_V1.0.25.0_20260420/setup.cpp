#include "IncludeAllHeader.h"
#pragma hdrstop


#include "setup.h"
#include "TrayStepMotor.h"
#include "DecodeXML.h"
#include "FileInfo.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "butPa1"
#pragma link "HTray"
#pragma link "htray"
#pragma resource "*.dfm"
TfSetup *fSetup;
enum eBinSettingItems
{
    eBinNo              =0,                                                 //Bin名稱的位置
    eBinSetting         =1,                                                 //Bin設定的起始點
    eBinSetTotal        =(eBinSetting+eTrayCount)
};

const int iRowHeight=19;
const int iColWidth =30;
bool bEditVisible=false;

const int eItemName   =0;                                                       //名稱的位置
enum eTrayNameFunc                                                              //使用的功能
    {
        eItemPass   =0,                                                         //Tray為Pass或Fail
        eItemTotal
    };

enum eTrayColorMap
{   eCLWhite        =0,
    eCLGreen        =1,
    eCLRed          =2,
    eCLYellow       =3,
    eCLGray         =4,
    eCLSilver       =5,
    eCLBtnFace      =6,
    eCLOlive        =7,
    eCLSkyBlue      =8,                                                     //Steven 20240626 : 增加bin設定的可閱讀性
};
//------------------------------------------------------------------------------
__fastcall TfSetup::TfSetup(TComponent* Owner)
     : TForm(Owner)
{
    sListBin   =new TStringList();
    sListTray  =new TStringList();
    AnsiString str;
    pcSetup->Top=-25;
    flWorkFileList->Visible=false;
    flTemp->Visible=false;
    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        str.sprintf("cbEnableBin%02d", i);
        cbEnableBin[i]          =new TCheckBox(this);
        cbEnableBin[i]->Name    =str;
        cbEnableBin[i]->Parent  =grpSettinBin;
        cbEnableBin[i]->Left    =24;
        cbEnableBin[i]->Top     =i*20;
        cbEnableBin[i]->Width   =120;
        cbEnableBin[i]->Caption ="Enable Bin "+AnsiString(i);
    }
    cbEnableBin[0]->Visible=false;
    sBinSetting="";
    InitialBinAreaSG();
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::FormCreate(TObject *Sender)                            // load all workfile.ini //
{
    flWorkFileList->Mask=HSys.CurrentDir+AnsiString("\\data\\*.ini");

    for(int i=0; i<pcSetup->PageCount; i++)
        pcSetup->Pages[i]->TabVisible=false;

    SetEditOnMouse(this);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::FormShow(TObject *Sender)
{
    fShow=true;
    iCurrX=0;                                                                   //Steven 20240626 : 增加bin設定的可閱讀性
    iCurrY=0;
    iOldX=0;
    iOldY=0;

    OpenWorkFile();
    UpdateWorkFileCheckListBox();                                               //20110728 Daver add
    //--------------------------------------------------------------------------
    TabSheet1->TabVisible=false;

    tsAutoBinSet->TabVisible=(fMaintenance->rgT03->ItemIndex==1);               //Steven 20240627 : 自動設定bin tray
    if(fMaintenance->rgT03->ItemIndex==1)
    {
        pgcBinSet->ActivePage=tsAutoBinSet;
        pgcBinSet->ActivePageIndex=1;
    }

    TMyTray1->XItem=atoi(edXDivision->Text.c_str());
    TMyTray1->YItem=atoi(edYDivision->Text.c_str());
    AnsiString asMap[4][2]={{"Aa", "Ba"},
                            {"Ab", "Bb"},
                            {"Ac", "Bc"},
                            {"Ad", "Bd"}};

    if(rgUseSuck->ItemIndex==0)
    {
        for(int iR=0; iR<SortArmPara->SortArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<SortArmPara->SortArmSuck->MaxItemC; iC++)
            {
                mtSortArm1OnOff->SetCellNumber(iC, iR, asMap[iR][iC]);
                mtSortArm1OnOff->SetCellColorIndex(iC, iR, eCLGreen);
            }
        }
        for(int iR=0; iR<MagArmPara->MagArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<MagArmPara->MagArmSuck->MaxItemC; iC++)
            {
                mtSortArm2OnOff->SetCellNumber(iC, iR, asMap[iR][iC]);
                mtSortArm2OnOff->SetCellColorIndex(iC, iR, eCLGreen);
            }
        }
        mtSortArm1OnOff->Enabled=false;
        mtSortArm2OnOff->Enabled=false;
    }
    else
    {
        for(int iR=0; iR<SortArmPara->SortArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<SortArmPara->SortArmSuck->MaxItemC; iC++)
            {
                mtSortArm1OnOff->SetCellNumber(iC, iR, asMap[iR][iC]);
                if(SortArmPara->bSortArm1UseSuck[iR][iC]==true)
                    mtSortArm1OnOff->SetCellColorIndex(iC, iR, eCLGreen);
                else
                    mtSortArm1OnOff->SetCellColorIndex(iC, iR, eCLWhite);
            }
        }

        for(int iR=0; iR<MagArmPara->MagArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<MagArmPara->MagArmSuck->MaxItemC; iC++)
            {
                mtSortArm2OnOff->SetCellNumber(iC, iR, asMap[iR][iC]);
                if(bSortArm2UseSuck[iR][iC]==true)
                    mtSortArm2OnOff->SetCellColorIndex(iC, iR, eCLGreen);
                else
                    mtSortArm2OnOff->SetCellColorIndex(iC, iR, eCLWhite);
            }
        }
    }
    sbTrayForm->OnClick(sbTrayForm);
    sbTrayForm->Down=true;
    pgcBinSet->Enabled=!HasICUnderMachine();                                    //Sam 20240729 : CleanOut 才能設定 Bin

    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629 : 擺在最下面,不要動
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::SetEditOnMouse(TWinControl *PCtrl)
{
    for (int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P = PCtrl->Controls[iP];
        if(dynamic_cast <TPanel *>(P) != NULL          ||
           dynamic_cast <TPageControl *>(P) != NULL    ||
           dynamic_cast <TTabSheet *>(P) != NULL       ||
           dynamic_cast <TTabControl *>(P) != NULL     ||
           dynamic_cast <TForm *>(P) != NULL           ||
           dynamic_cast <TPage *>(P) != NULL           ||
           dynamic_cast <TGroupBox*>(P) != NULL        )
        {
            SetEditOnMouse((TWinControl *) P);                                  // 找該物件附屬的物件(遞迴)
        }

        TEdit        *PEdit                 = dynamic_cast <TEdit           *>(P);
        TLabeledEdit *LEdit                 = dynamic_cast <TLabeledEdit    *>(P);
        TTabSheet    *PTabSheet             = dynamic_cast <TTabSheet       *>(P);    //KenHsieh 20240612 : CCD頁面不使用此事件
        if(PTabSheet!=NULL && PTabSheet->Name!="tsCCD")
        {
            //pass
        }
        else if(PEdit!=NULL)
        {
            PEdit->OnMouseDown=edXStartMouseDown;
        }
        else if(LEdit!=NULL)
        {
            LEdit->OnMouseDown=edXStartMouseDown;
        }
    }
}
//==============================================================================
void __fastcall TfSetup::OpenWorkFile()                                         // Load Setup Parameter //
{
    AnsiString S, pos;

    flWorkFileList->Update();
    S=HSys.CurrentDir+AnsiString("\\data\\")+fMain->cb_WorkFile->Text+AnsiString(".ini");
    if(CheckFileExist(S.c_str()))
    {
        FormSysTools->OpenFormData(S);
        for(int iP=0; iP<pcSetup->ControlCount; iP++)
        {
            TControl *P=pcSetup->Controls[iP];

            TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);

            if(TabPtr!=NULL)
            {
                if(TabPtr->Name!="TabWorkFileManegement" ||
                   TabPtr->Name!="TabSheet1")
                    FormSysTools->LoadFormData(TabPtr, TabPtr->Name);
            }
        }
        FormSysTools->CloseFormData();
    }
    if(CosFunction.bUseBinAutoConvert)
    {
//        AnsiString Str=CheckAndReadIniData(S, AnsiString("tsBinSetting"), AnsiString("cBinAutoTrans"), AnsiString(""));
        cBinAutoTrans.LoadFromJson(GetSetUpBinDataFileName());
        cBinArea.cBinAutoTrans_to_BackTray();
    }
    frm2DID->Update2DParameter();
}
//==============================================================================
void __fastcall TfSetup::sbBuildClick(TObject *Sender)                          // change tabsheet //
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    pcSetup->ActivePageIndex=Ptr->Tag-1;
}
//==============================================================================
void __fastcall TfSetup::SaveWorkFile(AnsiString S)                             // Save Setup Parameter //
{
    FormSysTools->OpenFormData(S);
    for(int iP=0; iP<pcSetup->ControlCount; iP++)
    {
        TControl *P=pcSetup->Controls[iP];

        TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);

        if(TabPtr!=NULL)
        {
            if(TabPtr->Name!="TabWorkFileManegement" ||
               TabPtr->Name!="TabSheet1")
                FormSysTools->SaveFormData(TabPtr, TabPtr->Name);
        }
    }
    FormSysTools->CloseFormData();
    //
    cBinArea.BackTray_to_cBinAutoTrans();
    AnsiString Str=cBinAutoTrans.SaveToJson();     ;
//    WriteIniData(S, "tsBinSetting", "cBinAutoTrans", Str);
    WriteDataToFile(GetSetUpBinDataFileName(), Str, true);
}
//==============================================================================
void __fastcall TfSetup::sbUpdateClick(TObject *Sender)                         // update button //
{
    AnsiString S=GetSetUpFileName();
    SaveWorkFile(S);
    OpenWorkFile();                                                             //Sam 20240702 : 修正 SetBin 顯示器沒有同步更新顯示
    UpdateAllParameter();
    sbUpdate->Down=false;
}
//==============================================================================
void __fastcall TfSetup::sbExitClick(TObject *Sender)                           // exit button //
{
    sbExit->Down=false;
    Close();
}
//==============================================================================
void __fastcall TfSetup::sbSaveAsClick(TObject *Sender)                         // save button //
{
    AnsiString asSF="",asDF="";
    SaveDialog->DefaultExt="ini";
    SaveDialog->InitialDir=HSys.CurrentDir+AnsiString("\\data");
    SaveDialog->Filter="*.ini|*.ini";
    if( SaveDialog->Execute() )
    {
        AnsiString sfullPath="",spath="",sfilename="",sextension="";
        sfullPath=SaveDialog->FileName;
        FileInfo().SplitPath(sfullPath,spath,sfilename,sextension);

        //ini
        asDF=HSys.CurrentDir+AnsiString("\\data\\")+sfilename+AnsiString(".ini");
        asSF=HSys.CurrentDir+AnsiString("\\data\\")+fMain->cb_WorkFile->Text+AnsiString(".ini");
        CopyFile(asSF.c_str(), asDF.c_str(), false) ;
        //ofs
        asDF=HSys.CurrentDir+AnsiString("\\data\\")+sfilename+AnsiString(".ofs");
        asSF=HSys.CurrentDir+AnsiString("\\data\\")+fMain->cb_WorkFile->Text+AnsiString(".ofs");
        if(FileExists(asSF)==false)
        {
            fOffset->SaveWorkFile();
        }
        CopyFile(asSF.c_str(), asDF.c_str(), false) ;
    }
    sbSaveAs->Down=false;
    UpdateWorkFileCheckListBox();
}
//==============================================================================
void __fastcall TfSetup::UpdateWorkFileCheckListBox()                           // update workfile list //
{
    AnsiString S;
    flWorkFileList->Mask=HSys.CurrentDir+AnsiString("\\data\\*.ini");           // 強制Refresh,不然不會update
    flWorkFileList->Update();                                                   // Daver add
    clWorkFile->Items->Clear();
    for(int i=0; i<flWorkFileList->Items->Count; i++)
    {
        S=flWorkFileList->Items->Strings[i];
        S=S.SubString(1,S.Length()-4);
        if(S!=fMain->cb_WorkFile->Items->Strings[fMain->cb_WorkFile->ItemIndex] )
        {
            clWorkFile->Items->Add(S);
        }
    }
}
//==============================================================================
void __fastcall TfSetup::spbExportClick(TObject *Sender)                        //Daver Add
{
    AnsiString asSF,asDF;
    for(int i=0; i<clWorkFile->Items->Count;i++)
    {
        if(clWorkFile->Checked[i])
        {
            asSF=HSys.CurrentDir+AnsiString("\\data\\")+clWorkFile->Items->Strings[i]+AnsiString(".ini");
            asDF=DirectoryListBox1->Directory+"\\"+clWorkFile->Items->Strings[i]+AnsiString(".ini");
            if (FileExists(asSF))
                CopyFile(asSF.c_str(), asDF.c_str(), false) ;
            asSF=HSys.CurrentDir+AnsiString("\\data\\")+clWorkFile->Items->Strings[i]+AnsiString(".ofs");
            asDF=DirectoryListBox1->Directory+"\\"+clWorkFile->Items->Strings[i]+AnsiString(".ofs");
            if (FileExists(asSF))
                CopyFile(asSF.c_str(), asDF.c_str(), false) ;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::spbDeleteSetupFileClick(TObject *Sender)               //Daver Add
{
    AnsiString asSF;
    for (int i=0; i<clWorkFile->Items->Count;i++)
    {
        if (clWorkFile->Checked[i])
        {
            asSF=HSys.CurrentDir+AnsiString("\\data\\")+clWorkFile->Items->Strings[i]+AnsiString(".ini");
            if (FileExists(asSF))
                DeleteFile(asSF);
            asSF=HSys.CurrentDir+AnsiString("\\data\\")+clWorkFile->Items->Strings[i]+AnsiString(".ofs");
            if (FileExists(asSF))
                DeleteFile(asSF);
        }
    }
    UpdateWorkFileCheckListBox();
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::spbImportClick(TObject *Sender)                        //Daver Add
{
    AnsiString asSF,asDF,asTmp;
    flImport->Mask=DirectoryListBox1->Directory+AnsiString("\\*.ini");
    flImport->Update();
    for (int i=0; i<flImport->Items->Count; i++)
    {
        asSF=DirectoryListBox1->Directory+"\\"+flImport->Items->Strings[i];
        asDF=HSys.CurrentDir+AnsiString("\\data\\")+flImport->Items->Strings[i];
        CopyFile(asSF.c_str(), asDF.c_str(), false) ;
        asTmp=flImport->Items->Strings[i];
        asTmp=asTmp.Delete(asTmp.Length()-3,4);
        asSF=DirectoryListBox1->Directory+"\\"+asTmp+".ofs";
        asDF=HSys.CurrentDir+AnsiString("\\data\\")+asTmp+".ofs";
        if (FileExists(asSF))
            CopyFile(asSF.c_str(), asDF.c_str(), false) ;
    }
    UpdateWorkFileCheckListBox();
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::SpPickCPClick(TObject *Sender)
{
    AnsiString asGetName;
    bool       bFind=true;
    int        iGetPos;

    OpenPickMap->Filter="*.*|*.*";

    if(OpenPickMap->Execute())
    {
        asGetName=OpenPickMap->FileName;
        if((asGetName.SubString(OpenPickMap->FileName.Length()-2, 3)=="TXT")||
           (asGetName.SubString(OpenPickMap->FileName.Length()-2, 3)=="txt"))
        {
            while (bFind)
            {
                iGetPos=asGetName.Pos("\\");
                if (iGetPos!=0)
                {
                    asGetName=asGetName.SubString(iGetPos+1, asGetName.Length()-iGetPos);
                }
                else
                {
                    bFind=false;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::FormDestroy(TObject *Sender)
{
    sListBin->Clear();
    sListTray->Clear();
    delete sListBin;
    delete sListTray;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::BinChange()                                            // check change pos //
{
    int iSX, iEX;
    if(iStartX>iEndX)
    {
        iSX=iEndX;
        iEX=iStartX;
    }
    else
    {
        iSX=iStartX;
        iEX=iEndX;
    }
    iStartX=iSX;
    iEndX=iEX;
}
//------------------------------------------------------------------------------
void TfSetup::ClearBinSet(int tag, bool bSetToFile)                             //Steven 20240627 : 自動設定bin tray
{
    AnsiString Str="";
    if(CosFunction.bUseBinAutoConvert)
    {
        cBinAutoTrans.Clear();
    }
}
//------------------------------------------------------------------------------
bool TfSetup::AutoBinSet()                                                      //Steven 20240627 : 自動設定bin tray
{
    TStringList *sList      =new TStringList();
    sList->Clear();
    int iBin=0;
    AnsiString sAutoInfo="",sLog="";

    VecBins.clear();
    if(HSys.FuncT.iT03_AutoBinSet==1)   //依據 Setup 畫面選擇的 Bin
    {
        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            if(cbEnableBin[i]->Checked)
            {
                VecBins.push_back(i);
            }
        }   
    }                                                                           
    else if(HSys.FuncT.iT03_AutoBinSet==2 ||                                    //Sam 20250724 : 新增 AutoSetBin By Lot Auto Info
            HSys.FuncT.iT03_AutoBinSet==3 ||
            HSys.FuncT.iT03_AutoBinSet==4)
    {
        if(HSys.FuncT.iT03_AutoBinSet==2)
            sAutoInfo=sLotAuto1Info;
        else if(HSys.FuncT.iT03_AutoBinSet==3)
            sAutoInfo=sLotAuto2Info;
        else
            sAutoInfo=sLotAuto3Info;
        sLog.sprintf("AutoSetBin By Auto%d %s", HSys.FuncT.iT03_AutoBinSet-1, sAutoInfo);
        RecordProcess(sLog);
        sList->CommaText=sAutoInfo;
        for(int i=0; i<sList->Count; i++)
        {
            iBin=atoi(sList->Strings[i].c_str());
            if(iBin>0)
                VecBins.push_back(iBin);
        }
        if(VecBins.size()==0)
        {
            int iActiveBinCount=0;
            int iLotTotalCount=0;
            for(int i=0; i<TEST_MAX_BIN; i++)
            {
                if(myXML.iTotalBinCount[i]>0)
                {
                    iActiveBinCount++;
                    iLotTotalCount+=myXML.iTotalBinCount[i];
                }
            }
            sLog.sprintf("AutoSetBin Fail: Auto%d bin list empty. Lot=%s ActiveBin=%d LotTotal=%d Auto1=%s Auto2=%s Auto3=%s", HSys.FuncT.iT03_AutoBinSet-1, fMain->edLotNo->Text.c_str(), iActiveBinCount, iLotTotalCount, sLotAuto1Info.c_str(), sLotAuto2Info.c_str(), sLotAuto3Info.c_str());
            RecordProcess(sLog);
            ShowMyMessage("AutoSetBin fail: Auto bin list empty. Please check lot map Auto1/2/3.");
            sList->Clear();
            delete sList;
            return false;
        }
    }
    else
    {
        sList->Clear();
        delete sList;
        ShowMyMessage("AutoBinSet error!!");
        return false;
    }
    AutoBinSet(VecBins);
    sList->Clear();
    delete sList;
    return true;
}
//------------------------------------------------------------------------------
void TfSetup::AutoBinSet(const std::vector<int> &vbins)                                               //Steven 20240627 : 自動設定bin tray
{
    std::vector<int> lsTrayOrder=GetTrayOrderList();
    TStringList *sList      =new TStringList();
    AnsiString Str="", sLog="";
    int iBin=0,iBinCnt=0, iAutoSum=0, iMagSum=0;
    unsigned int iTray=0;
    sList->Clear();
    sListBin->Clear();
    sListTray->Clear();
    cBinAutoTrans.Clear();
    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        sListTray->Add(fMain->sTrayNameBin[eBinNotUse]);
        if(myXML.iTotalBinCount[i]>0)
        {
            for(unsigned int j=0; j<vbins.size(); j++)
            {
                if(i==vbins[j])
                {
                    Str.sprintf("%08d,%02d", myXML.iTotalBinCount[i], i);
                    sListBin->Add(Str);
                    break;
                }
            }
        }
    }
    sListBin->Sort();                                                           //排序完是由小到大
    sLog.sprintf("AutoBinSet By Lot:%s", fMain->edLotNo->Text);
    RecordProcess(sLog);
    AnsiString sAreaName="";
    int iArea=0;
    for(int i=sListBin->Count-1; i>=0; i--)
    {
        if(iTray<lsTrayOrder.size())
        {
            sList->Clear();
            sList->CommaText=sListBin->Strings[i];
            iBin=atoi(sList->Strings[1].c_str());
            iBinCnt=atoi(sList->Strings[0].c_str());
            //
            iArea=lsTrayOrder[iTray];
            sAreaName=fMain->sTrayNameBin[iArea];
            sListTray->Strings[iBin]=sAreaName;
            sList->Add(sAreaName);
            sListBin->Strings[i]=sList->CommaText;
            cBinAutoTrans.AddBinAuto(iBin, iArea);
            sLog.sprintf("Set bin%02d to %s (%d)", cBinAutoTrans.GetBinbyAuto(iArea), sAreaName, iBinCnt);
            if(iArea>=eAuto1 && iArea<=eAuto20)
                iAutoSum+=iBinCnt;
            else
                iMagSum+=iBinCnt;
            iTray++;
            
            RecordProcess(sLog);
        }
        else
        {       	
        	ShowMyMessage("Autosetbin setting range error!!");	
			sList->Clear();
    		delete sList;
    		return;		
        }
    }
    if(iAutoSum+iMagSum>0)
    {
        tRunData.SetAutoQuantity(iAutoSum);
        tRunData.SetMagQuantity(iMagSum);
        sLog.sprintf("Auto total %d。%s", iAutoSum, GetTotalQuantityAutoPercent());
        RecordProcess(sLog+"%");
        sLog.sprintf("Mag total %d。%s",  iMagSum,  GetTotalQuantityMagPercent());
        RecordProcess(sLog+"%");
        Str.sprintf("Lot Quantity：%d [%s][%s]", tRunData.GetTotalQuantity(), GetTotalQuantityAutoPercent(), GetTotalQuantityMagPercent());
        fMain->lblLotInfo->Caption=Str;     //Sam 20250723 : 新增顯示 Lot 總數量到畫面上
    }
    cBinArea.cBinAutoTrans_to_BackTray();
    sbUpdate->Click();
    fMain->ShowBinCount();
    DisPlayBinTrayMap();

    sList->Clear();
    delete sList;
}
//------------------------------------------------------------------------------
void TfSetup::AutoBinSet(const std::vector<int> &vbins,int iTrayOrder[eTrayCount])                                               //Steven 20240627 : 自動設定bin tray
{
    if(CUSTOMER_CODE==CC_WINSTEK &&
       CosFunction.bUseMagzineArea==false)
    {
        iTrayOrder[20]=eAuto1;
    }
    TStringList *sList      =new TStringList();
    AnsiString Str="", sLog="";
    int iBin=0,iAuto=0, iTray=1, iBinCnt=0, iAutoSum=0, iMagSum=0;
    sList->Clear();
    sListBin->Clear();
    sListTray->Clear();
    cBinAutoTrans.Clear();
    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        sListTray->Add(fMain->sTrayNameBin[eBinNotUse]);
        if(myXML.iTotalBinCount[i]>0)
        {
            for(unsigned int j=0; j<vbins.size(); j++)
            {
                if(i==vbins[j])
                {
                    Str.sprintf("%08d,%02d", myXML.iTotalBinCount[i], i);
                    sListBin->Add(Str);
                    break;
                }
            }
        }
    }
    sListBin->Sort();                                                           //排序完是由小到大
    sLog.sprintf("AutoBinSet By Lot:%s", fMain->edLotNo->Text);
    RecordProcess(sLog);
    for(int i=sListBin->Count-1; i>=0; i--)
    {
        sList->Clear();
        sList->CommaText=sListBin->Strings[i];
        iBin=atoi(sList->Strings[1].c_str());
        iBinCnt=atoi(sList->Strings[0].c_str());
        sListTray->Strings[iBin]=fMain->sTrayNameBin[iTrayOrder[iTray]];
        sList->Add(fMain->sTrayNameBin[iTrayOrder[iTray]]);
        sListBin->Strings[i]=sList->CommaText;
        cBinAutoTrans.AddBinAuto(iBin,iTrayOrder[iTray]);
        iAuto=cBinAutoTrans.GetAutobyBin(iBin);
        sLog.sprintf("Set bin%02d to %s (%d)", cBinAutoTrans.GetBinbyAuto(iTrayOrder[iTray]), fMain->sTrayNameBin[iAuto], iBinCnt);
        if(iTrayOrder[iTray]>=eAuto1 && iTrayOrder[iTray]<=eAuto20)
            iAutoSum+=iBinCnt;
        else
            iMagSum+=iBinCnt;
        iTray++;
        RecordProcess(sLog);
    }
    if(iAutoSum+iMagSum>0)
    {
        tRunData.SetAutoQuantity(iAutoSum);
        tRunData.SetMagQuantity(iMagSum);
        sLog.sprintf("Auto total %d。%s", iAutoSum, GetTotalQuantityAutoPercent());
        RecordProcess(sLog+"%");
        sLog.sprintf("Mag total %d。%s",  iMagSum,  GetTotalQuantityMagPercent());
        RecordProcess(sLog+"%");
        Str.sprintf("Lot Quantity：%d [%s][%s]", tRunData.GetTotalQuantity(), GetTotalQuantityAutoPercent(), GetTotalQuantityMagPercent());
        fMain->lblLotInfo->Caption=Str;     //Sam 20250723 : 新增顯示 Lot 總數量到畫面上
    }
    cBinArea.cBinAutoTrans_to_BackTray();
    sbUpdate->Click();
    fMain->ShowBinCount();
    DisPlayBinTrayMap();

    sList->Clear();
    delete sList;
}
//------------------------------------------------------------------------------
bool TfSetup::ParseCommaSeparatedString(const AnsiString &sBinSelectedList, std::vector<int> &result)
{
    result.clear();
    if (sBinSelectedList.IsEmpty())
        return false;
    AnsiString s = sBinSelectedList;
    TStrings *list = new TStringList();
    try
    {
        list->CommaText = s;
        if (list->Count == 0)
        {
            delete list;
            return false;
        }
        for (int i = 0; i < list->Count; i++)
        {
            try
            {
                result.push_back(list->Strings[i].ToInt());
            }
            catch (EConvertError &e)
            {
                result.clear();
                delete list;
                return false;
            }
        }
        delete list;
        return true;
    }
    catch (...)
    {
        // 處理任何其他異常
        result.clear();
        delete list;
        return false;
    }
}
//------------------------------------------------------------------------------
void TfSetup::AutoBinSetAutoAreaOnly(AnsiString sBinSelectedList)               //Ian : 自動設定bin tray在Auto區only
{
    ParseCommaSeparatedString(sBinSelectedList,VecBins);
    AutoBinSet(VecBins);
    sbUpdate->Click();
    fMain->ShowLotBinCount();
}
//------------------------------------------------------------------------------
void TfSetup::DisPlayBinTrayMap()
{
    bool bFind;
    int iTray=1;
    TStringList *sList      =new TStringList();
    sgBinSet->Cells[0][0]="Bin";
    sgBinSet->Cells[1][0]="Tray";
    sgBinSet->Cells[2][0]="Count";

    if(sListTray->Count==0)                                                     //Sam 20240702 : 避免沒有資料時的溢位
    {
        sListTray->Clear();
        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            sListTray->Add(fMain->sTrayNameBin[eBinNotUse]);
        }
    }

    if(rgSoter->ItemIndex==0)
    {
        for(int i=1; i<sgBinSet->RowCount; i++)
        {
            for(int j=0; j<sgBinSet->ColCount; j++)
            {
                sgBinSet->Cells[j][i]="";
                sgBinSet->Cells[j][i]="";
                sgBinSet->Cells[j][i]="";
            }
        }
        sgBinSet->RowCount=TEST_MAX_BIN;

        for(int i=1; i<TEST_MAX_BIN; i++)
        {
            sgBinSet->Cells[0][i]=i;
            sgBinSet->Cells[1][i]=sListTray->Strings[i];
            sgBinSet->Cells[2][i]=myXML.iTotalBinCount[i];
        }
    }
    else if(rgSoter->ItemIndex==1)
    {
        for(int i=1; i<sgBinSet->RowCount; i++)
        {
            for(int j=0; j<sgBinSet->ColCount; j++)
            {
                sgBinSet->Cells[j][i]="";
                sgBinSet->Cells[j][i]="";
                sgBinSet->Cells[j][i]="";
            }
        }
        sgBinSet->RowCount=eTrayCount;

        for(int i=1; i<eTrayCount; i++)
        {
            sgBinSet->Cells[1][i]=fMain->sTrayNameBin[i];
        }

        for(int j=1; j<TEST_MAX_BIN; j++)
        {
            bFind=false;
            for(int i=1; i<eTrayCount; i++)
            {
                if(bFind==false)
                {
                    if(fMain->sTrayNameBin[i]==sListTray->Strings[j])
                    {
                        sgBinSet->Cells[0][i]=j;
                        sgBinSet->Cells[2][i]=myXML.iTotalBinCount[j];
                        bFind=true;
                    }
                }
            }
        }
    }
    else
    {
        for(int i=1; i<sgBinSet->RowCount; i++)
        {
            for(int j=0; j<sgBinSet->ColCount; j++)
            {
                sgBinSet->Cells[j][i]="";
                sgBinSet->Cells[j][i]="";
                sgBinSet->Cells[j][i]="";
            }
        }
        sgBinSet->RowCount=sListBin->Count+1;
        iTray=1;
        for(int i=sListBin->Count-1; i>=0; i--)
        {
            sList->Clear();
            sList->CommaText=sListBin->Strings[i];
            sgBinSet->Cells[0][iTray]=sList->Strings[1];
            sgBinSet->Cells[1][iTray]=sList->Strings[2];
            sgBinSet->Cells[2][iTray]=atoi(sList->Strings[0].c_str());
            iTray++;
        }
    }
    sList->Clear();
    delete sList;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::edReleaseOffsetMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, -5.00, 5.00);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::edXStartMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TEdit *Ptr;
    Ptr=(TEdit *)Sender;

    if(Ptr->Tag==1)
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 0.0, true, 0.0, 500.0);
    else if(Ptr->Tag==2)
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 100);
    else if(Ptr->Tag==3)                                                        //Sam 20240607 : Suck 預吸吹功能
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 0.0, true, 0.0, 20.0);
    else if(Ptr->Tag==4)                                                        //Sam20240627 : 修正回黏問題
         fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 0.0, true, 0.2, 1.0);
    else if(Ptr->Tag==5)
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NUM_PAD);                  //Sam 20240729 : 修正2D的資料輸入
    else if(Ptr->Tag==6)
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 99999);
    else if(Ptr->Tag==7)
         fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, -500, 500);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btnClearBinClick(TObject *Sender)
{
    int tag=0;
    ClearBinSet(tag);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btnDoAutoBinSetClick(TObject *Sender)
{
    AutoBinSet();                                                               //Steven 20240627 : 自動設定bin tray
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btnSelectAllBinClick(TObject *Sender)
{
    cbEnableBin[0]->Checked=false;
    for(int i=1; i<TEST_MAX_BIN; i++)
    {
        cbEnableBin[i]->Checked=true;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btnUnSelectAllBinClick(TObject *Sender)
{
    for(int i=0; i<TEST_MAX_BIN; i++)
        cbEnableBin[i]->Checked=false;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btnSelect1To20Click(TObject *Sender)
{
    cbEnableBin[0]->Checked=false;
    for(int i=1; i<=20; i++)
        cbEnableBin[i]->Checked=true;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btnSelect21To41Click(TObject *Sender)
{
    cbEnableBin[0]->Checked=false;
    for(int i=21; i<=41; i++)
        cbEnableBin[i]->Checked=true;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::rgSoterClick(TObject *Sender)
{
    DisPlayBinTrayMap();
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::rgUseSuckClick(TObject *Sender)
{
    if(rgUseSuck->ItemIndex==1)
    {
        mtSortArm1OnOff->Enabled=true;
        mtSortArm2OnOff->Enabled=true;
    }
    else
    {
        for(int iR=0; iR<SortArmPara->SortArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<SortArmPara->SortArmSuck->MaxItemC; iC++)
            {
                mtSortArm1OnOff->SetCellColorIndex(iC, iR, eCLGreen);
            }
        }
        for(int iR=0; iR<MagArmPara->MagArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<MagArmPara->MagArmSuck->MaxItemC; iC++)
            {
                mtSortArm2OnOff->SetCellColorIndex(iC, iR, eCLGreen);
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::mtSortArm2OnOffMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    mtSortArm2OnOff->ConvertIndexCells(X, Y);

    if(bSortArm2UseSuck[Y][X]==true)
    {
        bSortArm2UseSuck[Y][X]=false;
        mtSortArm2OnOff->SetCellColorIndex(X, Y, eCLWhite);
    }
    else
    {
        bSortArm2UseSuck[Y][X]=true;
        mtSortArm2OnOff->SetCellColorIndex(X, Y, eCLGreen);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::mtSortArm1OnOffMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    mtSortArm1OnOff->ConvertIndexCells(X, Y);

    if(SortArmPara->bSortArm1UseSuck[Y][X]==true)                                            //Steven 20240728 : = --> ==
    {
        SortArmPara->bSortArm1UseSuck[Y][X]=false;
        mtSortArm1OnOff->SetCellColorIndex(X, Y, eCLWhite);
    }
    else
    {
        SortArmPara->bSortArm1UseSuck[Y][X]=true;
        mtSortArm1OnOff->SetCellColorIndex(X, Y, eCLGreen);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btnSelect1To50Click(TObject *Sender)
{
    cbEnableBin[0]->Checked=false;	//Sam 20250718 : 新增快速設定
    for(int i=1; i<=50; i++)
        cbEnableBin[i]->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TfSetup::btnSelect51To98Click(TObject *Sender)
{
    cbEnableBin[0]->Checked=false;	//Sam 20250718 : 新增快速設定
    for(int i=51; i<=98; i++)
        cbEnableBin[i]->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TfSetup::btnSelect99To150Click(TObject *Sender)
{
    cbEnableBin[0]->Checked=false;	//Sam 20250718 : 新增快速設定
    for(int i=99; i<=150; i++)
        cbEnableBin[i]->Checked=true;
}
//---------------------------------------------------------------------------
void TfSetup::SetBinByArea(int iBin,int iArea)
{
    int tag=0;
    for(int ibin=0; ibin<TEST_MAX_BIN; ibin++)
    {
        if(MyBinPanel[tag]->BackTray[ibin][iArea]==1)
        {
            MyBinPanel[tag]->BackTray[ibin][iArea]=0;
            MyBinPanel[tag]->BackTray[ibin][0]=1;
        }
    }
    MyBinPanel[tag]->BackTray[iBin][iArea]=1;
}
//------------------------------------------------------------------------------
std::vector<int> TfSetup::GetTrayOrderList()
{
    //排序的原則是以 Loader Teach 點位 Auto Teach 點位的直線距離來排序。 Auto1 放到最後
    int iTrayOrderMagCnt3[eTrayCount]=
                                       {eBinNotUse,
                                        eAuto6,    eAuto7,     eAuto11,    eAuto12,     eAuto16,
                                        eAuto17,   eAuto8,     eAuto13,    eAuto2,      eAuto14,
                                        eAuto9,    eAuto18,    eAuto3,     eAuto19,     eAuto15,
                                        eAuto10,   eAuto4,     eAuto20,    eAuto5,
                                        eMag2_01, eMag2_02, eMag2_03, eMag2_04, eMag2_05, eMag2_06, eMag2_07, eMag2_08, eMag2_09,
                                        eMag2_10, eMag2_11, eMag2_12, eMag2_13, eMag2_14, eMag2_15, eMag2_16, eMag2_17, eMag2_18,
                                        eMag2_19, eMag2_20, eMag2_21, eMag2_22, eMag2_23, eMag2_24, eMag2_25, eMag2_26, eMag2_27,
                                        eMag3_01, eMag3_02, eMag3_03, eMag3_04, eMag3_05, eMag3_06, eMag3_07, eMag3_08, eMag3_09,
                                        eMag3_10, eMag3_11, eMag3_12, eMag3_13, eMag3_14, eMag3_15, eMag3_16, eMag3_17, eMag3_18,
                                        eMag3_19, eMag3_20, eMag3_21, eMag3_22, eMag3_23, eMag3_24, eMag3_25, eMag3_26, eMag3_27,
                                        eAuto1,
                                        eMag1_01, eMag1_02, eMag1_03, eMag1_04, eMag1_05, eMag1_06, eMag1_07, eMag1_08, eMag1_09,
                                        eMag1_10, eMag1_11, eMag1_12, eMag1_13, eMag1_14, eMag1_15, eMag1_16, eMag1_17, eMag1_18,
                                        eMag1_19, eMag1_20, eMag1_21, eMag1_22, eMag1_23, eMag1_24, eMag1_25, eMag1_26, eMag1_27,
                                        eFix1,    eFix2};
    std::vector<int> list;
    int iMapArea=0;
    for(int iarea=1; iarea<eTrayCount; iarea++)
    {
        iMapArea=iTrayOrderMagCnt3[iarea];
        if(fMaintenance->cbEnableArea[iMapArea]->Checked==true)
            list.push_back(iMapArea);
    }
    return list;
}
//------------------------------------------------------------------------------
AnsiString TfSetup::GetSetUpFileName()
{
    AnsiString S=FileInfo().PathCombin(HSys.CurrentDir,AnsiString("data"));
    S=FileInfo().PathCombin(S,fMain->cb_WorkFile->Text+AnsiString(".ini"));
    return S;
}
//------------------------------------------------------------------------------
AnsiString TfSetup::GetSetUpBinDataFileName()
{
    AnsiString S=FileInfo().PathCombin(HSys.CurrentDir,AnsiString("data"));
    S=FileInfo().PathCombin(S,cBinAutoTrans.sBinDataFileName());
    return S;
}
//------------------------------------------------------------------------------
cBinAreaSettingGUI::~cBinAreaSettingGUI()
{
    if(sgBinArea)
        delete sgBinArea;
    sgBinArea=NULL;
}
//------------------------------------------------------------------------------
void cBinAreaSettingGUI::InitialSGBinSetting(TStringGrid* sg)
{
    int iCol=sg->ColCount;
    int iTitleWidth=80;
//    int iAvgColW=(int)(sg->Width/(iCol-1))-1;
    int iAvgColW=40;
    sg->ColWidths[0]=iTitleWidth;
    for(int i=1;i<iCol;i++)
    {
        sg->ColWidths[i]=iAvgColW;
    }
    sg->OnDrawCell=sgBinAreaDrawCell;
    sg->OnMouseDown=sgBinAreaMouseDown;
    sg->OnMouseUp=sgBinAreaMouseUp;
    sg->OnMouseMove=sgBinAreaMouseMove;
}
//------------------------------------------------------------------------------
void cBinAreaSettingGUI::InitialSGBinSettingTitle(TStringGrid* sg,int ibinS,int ibinR)
{
    //col title
    iBinStart=ibinS;
    iBinRange=ibinR;
    int iGetBinRange=ibinR+eBinSetting;
    sg->ColCount=iGetBinRange;
    for(int iBin=0;iBin<iBinRange;iBin++)
    {
        sg->Cells[iBin+eBinSetting][0]=IntToStr(iBinStart+iBin);
    }
    //row title
    int itraycount=(int)eTrayCount;
    sg->RowCount=itraycount+1;
    for(int i=eBinNotUse;i<itraycount;i++)
    {
        sg->Cells[0][i+1]=fMain->sTrayNameSpace[i];
    }
    //cell default value
    for(int irow=0;irow<itraycount;irow++)
    {
        for(int icol=0;icol<iBinRange;icol++)
        {
            sg->Cells[icol+1][irow+1]=IntToStr(icol+iBinStart);
        }
    }
    //
    iHoverCol=0;
    iHoverRow=0;
}
//------------------------------------------------------------------------------
void cBinAreaGUIConvert::BackTray_to_cBinAutoTrans()
{
    cBinAutoTrans.Clear();
    for(int iBin=0; iBin<TEST_MAX_BIN; iBin++)
    {
        for(int iArea=eBinNotUse; iArea<eTrayCount; iArea++)
        {
            if(iArea>eBinNotUse &&
               cGUIData.GetBackTray(iBin,iArea)==true)
            {
                cBinAutoTrans.AddBinAuto(iBin, iArea);
            }
        }
    }
}
//------------------------------------------------------------------------------
void cBinAreaGUIConvert::cBinAutoTrans_to_BackTray()
{
    cGUIData.Clear();
    int iArea=0;
    for(int iBin=0; iBin<TEST_MAX_BIN; iBin++)
    {
        iArea=cBinAutoTrans.GetAutobyBin(iBin);
        cGUIData.SetBackTray(iBin, iArea, true);
    }
}
//------------------------------------------------------------------------------
void cBinAreaGUIConvert::AddTStringGrid(TStringGrid *sg, int iStart, int iRange)
{
    cBinAreaSettingGUI *cbGUI=new cBinAreaSettingGUI(cGUIData);
    cbGUI->SetSGBinSetting(sg,iStart,iRange);
    vBinAreaGUI.push_back(cbGUI);
}
//------------------------------------------------------------------------------
void __fastcall cBinAreaSettingGUI::sgBinAreaDrawCell(TObject *Sender, int ACol, int ARow,
        const TRect &Rect, TGridDrawState State)
{
    TStringGrid *grid = static_cast<TStringGrid*>(Sender);
    bool isHoveredCell = (ACol == iHoverCol && ARow == iHoverRow);
    bool isHoveredRowTitle = (ACol == 0 && ARow == iHoverRow);
    bool isHoveredColTitle = (ACol == iHoverCol && ARow == 0);
    bool isTitleCell = (ARow == 0 || ACol == 0);
    bool isSetting=false;
    bool bRangeSelected=false;
    int iBin=(ACol-eBinSetting)+iBinStart;
    int iArea=(ARow-eBinSetting);
    if(iBin>=0 &&
       iArea>=0)
    {
        isSetting=refBinAreaGUIData.GetBackTray(iBin,iArea);
        if (bBinMouseDown &&
            iAreaSelectedStart==0)
        {
            cMathTool().EnsureMinMax(iBinSelectedStart,iBinSelectedEnd);
            if(iBin>=iBinSelectedStart &&
               iBin<=iBinSelectedEnd &&
               iArea==0)
            {
                bRangeSelected=true;
            }
            else
            {
                bRangeSelected=false;
            }
        }
    }
    if(bRangeSelected)
    {
        grid->Canvas->Brush->Color = clYellow;
    }
    else if (isSetting &&
            !isTitleCell)
    {
        if(ARow == eBinSetting)
        {
            grid->Canvas->Brush->Color = clGray;
        }
        else
        {
            grid->Canvas->Brush->Color = clGreen;
        }
    }
    else if(isHoveredCell)
    {
        grid->Canvas->Brush->Color = (TColor)RGB(220, 240, 255); // 淺藍色
    }
    else if (isHoveredRowTitle ||
             isHoveredColTitle)
    {
        grid->Canvas->Brush->Color = clSkyBlue; // 標題高亮色
        grid->Canvas->Font->Color = clBlack;
    }
    else if (ARow == 0 ||
             ACol == 0)
    {
        grid->Canvas->Brush->Color = clSilver; // 一般標題色
    }
    else
    {
        grid->Canvas->Brush->Color = clWhite; // 一般儲存格
    }

    // 字體顏色設定
    if(isTitleCell)
    {
        grid->Canvas->Font->Color = clNavy;
    }
    else
    {
        grid->Canvas->Font->Color = clSilver; // 非標題儲存格預設字體顏色
    }
    grid->Canvas->FillRect(Rect);
    grid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 2, grid->Cells[ACol][ARow]);
}
//------------------------------------------------------------------------------
void __fastcall cBinAreaSettingGUI::sgBinAreaMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TStringGrid *grid=static_cast<TStringGrid*>(Sender);
    if(grid==NULL)
    {
        return;
    }
    int iCol=0,iRow=0;
    grid->MouseToCell(X, Y, iCol, iRow);
    if(iCol<=0)
    {
        return;
    }
    //上下兩段不可以顛倒!!-------
    if(iRow<eBinNotUse ||
       iCol>=TEST_MAX_BIN)                                                      //Sam 20171127 : 增加 BinTray Warning Count
    {
        return;
    }
    bBinMouseDown=true;
    iBinSelectedStart=iCol-eBinSetting+iBinStart;
    iAreaSelectedStart=iRow-eBinSetting;
    iBinSelectedEnd=iCol-eBinSetting+iBinStart;
    iAreaSelectedEnd=iRow-eBinSetting;
}
//------------------------------------------------------------------------------
void __fastcall cBinAreaSettingGUI::sgBinAreaMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TStringGrid *grid=static_cast<TStringGrid*>(Sender);
    if(grid==NULL)
    {
        return;
    }
    int iCol=0,iRow=0;
    grid->MouseToCell(X, Y, iCol, iRow);
    int iBin=(iCol-eBinSetting)+iBinStart;
    int iArea=iRow-eBinSetting;
    if(iBin>=0 &&
       iArea>=0)
    {
        if(iAreaSelectedStart==eBinNotUse)
        {
            cMathTool().EnsureMinMax(iBinSelectedStart,iBinSelectedEnd);
            for(int iBin=iBinSelectedStart; iBin<=iBinSelectedEnd; iBin++)
            {
                for(int iArea=eBinNotUse; iArea<eTrayCount; iArea++)
                {
                    if(iArea==eBinNotUse)
                    {
                        refBinAreaGUIData.SetBackTray(iBin, eBinNotUse, true);
                    }
                    else
                    {
                        refBinAreaGUIData.SetBackTray(iBin, iArea, false);
                    }
                }
            }
        }
        else
        {
            for(int iArea=eBinNotUse; iArea<eTrayCount; iArea++)
            {
                if(iArea==iAreaSelectedStart)
                {
                    refBinAreaGUIData.SetBackTray(iBin, iArea, !refBinAreaGUIData.GetBackTray(iBin,iArea));
                }
                else
                {
                    refBinAreaGUIData.SetBackTray(iBin, iArea, false);
                }
            }
        }
    }
    bBinMouseDown=false;
}
//------------------------------------------------------------------------------
void __fastcall cBinAreaSettingGUI::sgBinAreaMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    TStringGrid *grid=static_cast<TStringGrid*>(Sender);
    if(grid==NULL)
    {
        return;
    }
    int iCol=0,iRow=0;
    grid->MouseToCell(X, Y, iCol, iRow);
    if(iCol>=eBinSetting)
    {
        iHoverCol=iCol;
        iHoverRow=iRow;
        grid->Invalidate();
    }
    if(bBinMouseDown)
    {
        iBinSelectedEnd=iCol-eBinSetting+iBinStart;
        iAreaSelectedEnd=iRow-eBinSetting;
    }
}
//---------------------------------------------------------------------------
void cBinAreaSettingGUI::SetSGBinSetting(TStringGrid* sg,int ibinS,int ibinR)
{
    sgBinArea=sg;                
    InitialSGBinSettingTitle(sgBinArea,ibinS,ibinR);
    InitialSGBinSetting(sgBinArea);
}
//---------------------------------------------------------------------------
bool cBinAreaSettingGUI::IsValidGrid(TStringGrid* sg)
{
    bool bret=false;
    bret=(sg!=NULL);
    bret&=(sg->RowCount > 0);
    bret&=(sg->ColCount > 0);
    return bret;
}
//---------------------------------------------------------------------------
AnsiString cBinAreaSettingGUI::GetCellValue(TStringGrid* sg,int row, int col)
{
    if (!IsValidGrid(sg) ||
        row < 0 ||
        row >= sg->RowCount ||
        col < 0 ||
        col >= sg->ColCount)
    {
        return "";  // 預設空字串
    }
    return sg->Cells[col][row];
}
//---------------------------------------------------------------------------
void cBinAreaSettingGUI::SetCellValue(TStringGrid* sg,int row, int col, AnsiString value)
{
    if (!IsValidGrid(sg) ||
        row < 0 ||
        row >= sg->RowCount ||
        col < 0 ||
        col >= sg->ColCount)
    {
        return;  // 忽略無效存取
    }
    sg->Cells[col][row] = value;
}
//---------------------------------------------------------------------------
void cBinAreaGUIConvert::GetStartRange(int iorder, int imaxbinnum, int ibinrange, int &iStart, int &iRange)
{
    if (ibinrange <= 0 || imaxbinnum <= 0)
    {
        iStart = 0;
        iRange = 0;
        return;
    }
    // 計算開始位置 (1-based)
    iStart = iorder * ibinrange + 1;
    if (iStart > imaxbinnum)
    {
        // 已經超出最大值，不存在範圍
        iStart = 0;
        iRange = 0;
        return;
    }
    // 計算本區塊實際數量 (可能不足 binrange)
    int iEnd = iStart + ibinrange - 1;
    if (iEnd > imaxbinnum)
    {
        iEnd = imaxbinnum;
    }
    iRange = iEnd - iStart + 1;
    if(iRange!=ibinrange)
        iRange-=1;                                                              //Max bin=260 只能使用259bin，因記憶體0捨棄
}
//---------------------------------------------------------------------------
void TfSetup::InitialBinAreaSG()
{
    int iBinRange=40;
    int iTotalGroup=cMathTool().Ceil(TEST_MAX_BIN,iBinRange);
    int iStart=0,iRange=0;
    //
    int existingPageCount = PageControl1->PageCount;
    if (existingPageCount >= iTotalGroup)
        return;

    TTabSheet *newPage;
    TStringGrid *grid;
    for(int iorder=0;iorder<iTotalGroup;iorder++)
    {
        cBinAreaGUIConvert().GetStartRange(iorder,TEST_MAX_BIN,iBinRange,iStart,iRange);
        if(iorder<existingPageCount)
        {
            newPage=PageControl1->Pages[iorder];
            newPage->Caption = AnsiString().sprintf("Bin%d_%d", iStart, iStart + iRange - 1);   // 更新 Caption（避免可能因 TEST_MAX_BIN 變動造成資訊過期）
            grid = dynamic_cast<TStringGrid*>(
                newPage->FindComponent("sgBinOrder" + IntToStr(iorder + 1))
            );
            if (!grid)
            {
                // 找不到就補創建，避免例外
                grid = new TStringGrid(newPage);
                grid->Parent = newPage;
                grid->Align = alClient;
                grid->Name = "sgBinOrder" + IntToStr(iorder + 1);
            }
        }
        else
        {
            newPage = new TTabSheet(this);
            newPage->PageControl = PageControl1;
            newPage->Width = PageControl1->Width-16;
            newPage->Name = AnsiString().sprintf("tsBin%d_%d",iStart,iStart+iRange-1);
            newPage->Caption = AnsiString().sprintf("Bin%d_%d",iStart,iStart+iRange-1);
            grid = new TStringGrid(newPage);
            grid->Parent = newPage;
            grid->Align = alClient;
            grid->Name = "sgBinOrder" + IntToStr(iorder+1);
        }
        cBinArea.AddTStringGrid(grid,iStart,iRange);
    }
}
//---------------------------------------------------------------------------
bool cBinAreaGUIData::GetBackTray(int bin, int tray) const
{
    if (bin >= 0 &&
        bin < TEST_MAX_BIN &&
        tray >= 0 &&
        tray < eTrayCount)
    {
        return bBackTray[bin][tray];
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void cBinAreaGUIData::SetBackTray(int bin, int tray, bool value)
{
    if (bin >= 0 &&
        bin < TEST_MAX_BIN &&
        tray >= 0 &&
        tray < eTrayCount)
    {
        bBackTray[bin][tray] = value;
    }
    else
    {
        value=0;
    }
}
//---------------------------------------------------------------------------
