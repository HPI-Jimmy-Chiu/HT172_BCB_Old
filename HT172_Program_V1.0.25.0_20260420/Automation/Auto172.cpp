//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "Auto172.h"
#include "cmydef.h"
#include "main.h"
#include "DecodeXML.h"
#include "automation.h"
#include "setup.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
bool CheckSystemStart()
{
    return HSys.Sys.SystemStart;
}
//---------------------------------------------------------------------------
AnsiString GetHandlerType()
{
    return fData->labModel->Caption;
}
//---------------------------------------------------------------------------
int ShowOLPState(int iState)
{
    fMain->ShowOLPState(iState);
    return 0;
}
//---------------------------------------------------------------------------
bool CheckInitialOK()
{
    return InitialOK;
}
//---------------------------------------------------------------------------
AnsiString GetMainStatus()
{
    AnsiString S="";

    if(fMain->palMainStatus->Caption!="")                                       //KenHsieh 20250812 : 新增保護，避免記憶體溢位
        S=fMain->palMainStatus->Caption.UpperCase();
    else
        S="---";

    if(S=="RUNNING" ||
       S=="PAUSE" ||
       S=="HALT"  ||
       S=="LOCK")
    {
        if(S=="RUNNING")
        {
            S="RUN";
        }
        return S;
    }
    else
    {
        return "RUN";
    }
}
//---------------------------------------------------------------------------
AnsiString GetSoftwareVersion()
{
    return fData->labVersion->Caption;
}
//---------------------------------------------------------------------------
AnsiString GetSetUpFileName()
{
    return fMain->cb_WorkFile->Text;
}
//---------------------------------------------------------------------------
AnsiString GetJamCount()
{
    return IntToStr(HSys.LastSet.iJamCount[0]);
}
//---------------------------------------------------------------------------
AnsiString GetUPH()
{
    return IntToStr(tRunData.UPH);
}
//---------------------------------------------------------------------------
AnsiString GetBinSetMode()
{
    return IntToStr(HSys.FuncT.iT03_AutoBinSet);
}
//---------------------------------------------------------------------------
int GetIP(AnsiString *Data)
{
    Data[0]=HSys.FuncN.sN03_OLP_IP;
    return 1;
}
//---------------------------------------------------------------------------
int GetPort(AnsiString *Data)
{
    Data[0]=HSys.FuncN.sN03_OLP_Port;
    return 1;
}
//---------------------------------------------------------------------------
int GetCusCode(AnsiString *Data)
{
    Data[0]=IntToStr(CUSTOMER_CODE);
    return 1;
}
//---------------------------------------------------------------------------
int GetTesterInfo(AnsiString *Data)
{
    Data[0]=IntToStr(HSys.LastSet.iRealDummy);                                  //0:DUMMY, 1:HAS_TRAY, 2:REALLY
    Data[1]=IntToStr(HSys.LastSet.iStartMode);                                  //0:Initial, 1:Continue
    Data[2]=fMain->edLotNo->Text;
    Data[3]=fMain->edOperator->Text;                                            //Sam 20250724 : 新增 OperatorID 
    return 4;
}
//---------------------------------------------------------------------------
int SetTesterInfo(AnsiString *Data)
{
    if(HasICUnderMachine())
        return 1;

    if(CheckSystemStart())
        return 2;

    HSys.LastSet.iRealDummy=atoi(Data[0].c_str());
    fMain->LoadRunModePicture();
    UpdateAllParameter();
    EventReport(SECS_EVENT.RealDummy);
    HSys.LastSet.iStartMode=atoi(Data[1].c_str());
    fMain->LoadStartModePicture();
    fMain->edLotNo->Text=Data[2].c_str();
    fMain->edOperator->Text=Data[3].c_str();                                    //Sam 20250724 : 新增 OperatorID 
    return 0;
}
//---------------------------------------------------------------------------
int GetLotTotal(AnsiString *Data)
{
    Data[0]=fMain->palloadingCount->Caption;
    Data[1]=GetJamCount();
    Data[2]=fMain->palUnloadingCount->Caption;

    int iMaxBin = TEST_MAX_BIN;
    if(iMaxBin + 3 > AUTOMATION_MAX_DATA)
        iMaxBin = AUTOMATION_MAX_DATA - 3;

    for(int i=0; i<iMaxBin; i++)
    {
       Data[i+3]= tRunData.BinICCnt[i];
    }
    return 3+iMaxBin;
}
//---------------------------------------------------------------------------
int GetLotTotalQuantity(AnsiString *Data)
{
    Data[0]=IntToStr(tRunData.GetTotalQuantity());
    Data[1]=GetTotalQuantityAutoPercent();
    Data[2]=GetTotalQuantityMagPercent();
    return 3;
}
//---------------------------------------------------------------------------
int GetLotEndTime(AnsiString *Data)
{
    Data[0]=IntToStr(tRunData.LotEndTime);
    return 1;
}
//---------------------------------------------------------------------------
void DoOnLineReply(AnsiString *Data)
{
    Data[0]=fData->labModel->Caption;
    Data[1]=fData->labMachineId->Caption;
    Data[2]=fData->labVersion->Caption;   //軟體版本
    Data[3]="Ready";
    Data[4]="0";
}
//---------------------------------------------------------------------------
void DoClearReportRequest()
{
    tRunData.Clear();
    if(fMain->edLotNo->Text=="")
    {
        myXML.ResetData();
    }

    fMain->ShowBinCount();
    TrayArmPara->ClearTrayCount();
    EventReport(SECS_EVENT.ClearCount);
    WriteLastDataIni();
}
//---------------------------------------------------------------------------
int SetSetUpFileName(AnsiString *Data)
{
    AnsiString str="";

    if(HasICUnderMachine())
        return 1;

    if(CheckSystemStart())
        return 2;
    AnsiString sSetRecipe=Data[0];
    AnsiString sNowRecipe=fMain->cb_WorkFile->Text;
    if(sSetRecipe==sNowRecipe)
    {
        return 0;
    }
    int index = fMain->cb_WorkFile->Items->IndexOf(Data[0]);
    if(index!=-1)
    {
        fMain->cb_WorkFile->Text=sSetRecipe;
        fMain->cb_WorkFile->OnChange(fMain->cb_WorkFile);
        return 0;
    }
    return 3;
}
//---------------------------------------------------------------------------
int CheckCategoryName(AnsiString sName)
{
    for(int i=0; i<eTrayCount; i++)
    {
        if(sName==fMain->sTrayNameBin[i])
            return i;           //設定的文字正確
    }
    return -1;
}
//---------------------------------------------------------------------------
int SetCategory(AnsiString *Data)
{
    if(HasICUnderMachine())
        return 1;

    if(CheckSystemStart())
        return 2;

    int iRet=0, iCate=0;
    int SetCateCnt[TEST_MAX_BIN];

    TStringList *sList;
    sList  =new TStringList();
    sList->Clear();
    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        sList->Add(fMain->sTrayNameBin[eBinNotUse]);
    }

    ZeroMemory(SetCateCnt, sizeof(SetCateCnt));
    int iMaxCate = TEST_MAX_BIN;
    if(iMaxCate > AUTOMATION_MAX_DATA)
        iMaxCate = AUTOMATION_MAX_DATA;

    for(int i=0; i<iMaxCate; i++)
    {
        iCate=CheckCategoryName(Data[i]);
        if(iCate>0 )                                                            //Sam 20250603 : NotUsed not checked
        {
            sList->Strings[i]=Data[i];
            SetCateCnt[iCate]++;
            if(SetCateCnt[iCate]>1)
            {
                iRet=3;                                                         //錯誤設定將不同的Bin設定到同一個位置上
                break;
            }
        }
        else if(iCate==-1 && Data[i]!="")
        {
            iRet=4;                                                             //Data 字串比對錯誤
            break;
        }
    }

    if(iRet==0)                                                                 //設定沒問題再寫入
    {
        fSetup->DisPlayBinTrayMap();

        fSetup->sbUpdateClick(NULL);
        fSetup->OpenWorkFile();
        fMain->ShowLotBinCount();
    }
    sList->Clear();
    delete sList;

    return iRet;
}
//---------------------------------------------------------------------------
int GetCategory(AnsiString *Data)
{
    int iCount=0;
    TStringList *sList;
    sList  =new TStringList();
    sList->Clear();
//    sList->CommaText=MyBinPanel[0]->edBinSetting->Text;
    if(sList->Count>AUTOMATION_MAX_DATA-1)
        iCount=AUTOMATION_MAX_DATA-1;
    else
        iCount=sList->Count;

    for(int i=0; i<iCount; i++)
    {
       Data[i]=sList->Strings[i];
    }

    sList->Clear();
    delete sList;

    return iCount;
}
//---------------------------------------------------------------------------
AnsiString GetLotStartProgram()
{
    return HSys.FuncN.sN03_LotStartPatch;
}
//---------------------------------------------------------------------------
AnsiString GetLotEndProgram()
{
    return HSys.FuncN.sN03_LotEndPatch;
}
//---------------------------------------------------------------------------
int SetLotStartProgram(AnsiString *Data)
{
    AnsiString S="";
    S=HSys.CurrentDir+AnsiString("\\system\\maintance.ini");
    WriteIniData(S,"TabFunctionDefine","edN03_LotStartPatch",Data[0]);
    fMaintenance->edN03_LotStartPatch->Text=Data[0];
    HSys.FuncN.sN03_LotStartPatch=Data[0];
    return 0;
}
//---------------------------------------------------------------------------
int SetLotEndProgram(AnsiString *Data)
{
    AnsiString S="";
    S=HSys.CurrentDir+AnsiString("\\system\\maintance.ini");
    WriteIniData(S,"TabFunctionDefine","edN03_LotEndPatch",Data[0]);
    fMaintenance->edN03_LotEndPatch->Text=Data[0];
    HSys.FuncN.sN03_LotEndPatch=Data[0];
    return 0;
}
//---------------------------------------------------------------------------
int SetBinSetMode(AnsiString *Data)
{
    AnsiString S="";
    S=HSys.CurrentDir+AnsiString("\\system\\maintance.ini");
    WriteIniData(S,"TabFunctionDefine","rgT03",Data[0]);
    fMaintenance->rgT03->ItemIndex=atoi(Data[0].c_str());
    HSys.FuncT.iT03_AutoBinSet=atoi(Data[0].c_str());
    return 0;
}
//---------------------------------------------------------------------------
int GetSortBin(AnsiString sLotID, AnsiString sTrayID, AnsiString sTrayX, AnsiString sTrayY)
{
    return -1;
}
//---------------------------------------------------------------------------
int LoadTrayMap(AnsiString *Data)
{
    if(HasICUnderMachine())
        return 1;

    if(CheckSystemStart())
        return 2;

    if(RunInfo.LotStart)
        return 3;

    fMain->edLotNo->Text=Data[0];
    bOLPLoadMap=true;
    fMain->btnLotStartClick(fMain);

    bOLPLoadMap=false;
    if(RunInfo.LotStart==false)
        return 5;

    return 0;
}
//---------------------------------------------------------------------------
STARTUPINFO si;
PROCESS_INFORMATION pi;
int CallLotProgram(AnsiString cmd)
{
    if(HasICUnderMachineForCleanOut())                                          //Jimmychiu 20260120 : 原命令於Tray feed finish後送出，Joe要求放在Clean out finish送出優化產能
        return 1;

    if(CheckSystemStart())
        return 2;

    AnsiString asFilePath="";
    if(cmd=="Start")
        asFilePath=HSys.FuncN.sN03_LotStartPatch;
    else
        asFilePath=HSys.FuncN.sN03_LotEndPatch;

    if(FileExists(asFilePath))
    {
        AnsiString sPara="";
        if(cmd.Pos("Tray_Feed")>0)
        {
            sPara="1";
        }
        DoRunExecutFile(asFilePath.c_str(), sPara);
    }
    else
    {
        return 3;
    }
    return 0;
}
//---------------------------------------------------------------------------

