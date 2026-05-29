//---------------------------------------------------------------------------


#include "IncludeAllHeader.h"       //Dell 盢.h参,硉build
#pragma hdrstop
#include "cprod.h"
#pragma package(smart_init)
//============================================================================//
//   ノ跑计跋                                                               //
//============================================================================//
SYSTEM_BIN_SELECT BinSelect[2];
PASS_WORD USER;
TRAY_DATA TrayDef;
RUN_INFO RunInfo;
RUN_INFO RunInfo2;
TFunction tFunction;
TMotionnetIO tMotionnetIO;  //20140330 WEI
HT172_COUSTOMER_FUNCTION CosFunction;
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
TRunData tRunData;
TLatchCycleTime lctLoader;
//==============================================================================
bool WriteData(char *cFName,char *ptr,int size)
{
    FILE *Fp = fopen(cFName,"wb");
    if (Fp != NULL)
    {
        fwrite(ptr,size,1,Fp);
        fclose(Fp);
        return true;
    }
    else
        ShowRecordMessage("file save error ");
    return false;
}
//==============================================================================
bool ReadData(char *cFName,char *ptr,int size)
{
    FILE *Fp = fopen(cFName,"rb");
    if (Fp != NULL)
    {
        fread(ptr,size,1,Fp);
        fclose(Fp);
        return true;
    }
    else
        ShowRecordMessage("file read error ");
    return false;
}
//==============================================================================
bool ReadLastDataFile()
{
    return true;
}
//==============================================================================
bool WriteLastDataFile()
{
    bool flag=true;
    return flag;
}
//==============================================================================
void ClearLastSet()                                                             // ben disable 20110713 //
{
}
//==============================================================================
void SavePassword()                                                             // save password //
{
    FILE *Fp = fopen((HSys.CurrentDir+"\\system\\login.dat").c_str(),"wb");
    if (Fp != NULL)
    {
        fwrite((char *)&USER.RecordCT,1,sizeof(PASS_WORD),Fp);
        fclose(Fp);
    }
    else
    {
        ShowRecordMessage("file save error ");
    }
}
//==============================================================================
void ReadPassword()                                                             // read password //
{
    FILE *Fp = fopen((HSys.CurrentDir+"\\system\\login.dat").c_str(),"rb");
    if (Fp != NULL)
    {
        fread((char *)&USER.RecordCT,1,sizeof(PASS_WORD),Fp);
        fclose(Fp);
    }
    else
    {
        memset(&USER.ID[0][0],0,sizeof(PASS_WORD));
    }
}
//==============================================================================
bool CheckFileExist(char *cFName)
{
    if (access(cFName,0)==0)
    {
        int fhandle;
        if((fhandle=_rtl_open(cFName, O_RDONLY))==-1)
            return false;
        _rtl_close(fhandle);
        return true;
    }
    return false;
}

//==============================================================================
bool CheckFileCanAccess(char *cFName)
{
    int fhandle;
    if((fhandle=_rtl_open(cFName, O_RDWR))==-1)
        return false;
    _rtl_close(fhandle);
    return true;
}
//==============================================================================
void ReadLastDataIni()
{
    AnsiString OpenIniFile=HSys.CurrentDir+AnsiString("\\system\\lastdata.ini");
    TIniFile *INIFile = new TIniFile(OpenIniFile);
    AnsiString SS="", Str="";

    HSys.LastSet.iLanguageCountry   =INIFile->ReadInteger("System",     "iLanguageCountry", 0);
    HSys.LastSet.iStartMode         =INIFile->ReadInteger("System",     "iStartMode",       0);
    HSys.LastSet.iTransferPos       =INIFile->ReadInteger("Motor",      "iTransferPos",     0);
    HSys.LastSet.iShuttlePos        =INIFile->ReadInteger("Motor",      "iShuttlePos",      0);
    HSys.LastSet.iAutoPos           =INIFile->ReadInteger("Motor",      "iAutoPos",         0);
    HSys.LastSet.iAutoPos           =INIFile->ReadInteger("Motor",      "iAutoPos",         0);
    HSys.LastSet.iMaxContactCount   =INIFile->ReadInteger("Contact",    "iMaxContactCount", 0);
    tRunData.StartTime              =INIFile->ReadFloat("RunData",   "StartTime",    0.0);
    tRunData.LotEndTime             =INIFile->ReadFloat("RunData",   "LotEndTime",   0.0);
    tRunData.PauseTime              =INIFile->ReadFloat("RunData",   "PauseTime",    0.0);
    tRunData.AlarmTime              =INIFile->ReadFloat("RunData",   "AlarmTime",    0.0);
    //Category_CT
    TStringList *sList      =new TStringList();
    sList->Clear();
    sList->CommaText=INIFile->ReadString("RunData",    "Category_CT", "");
    for(int i=0; i<sList->Count; i++)                                           //Sam 20170907 (jou):  Category Count 魁タ
    {
        if(i<TEST_MAX_BIN)
        {
            tRunData.BinICCnt[i]=atoi(sList->Strings[i].c_str());
        }
    }
    sList->Clear();
    delete sList;
    //
    for(int i=eAuto1; i<eTrayCount; i++)
    {
        if(HSys.TrayUsage[i]==eNoTray)
        {
            tRunData.TrayICCnt[i]   =0;
        }
        else
        {
            SS=fMain->sTrayNameBin[i]+IntToStr(i)+AnsiString("_CT");
            tRunData.TrayICCnt[i]   =INIFile->ReadInteger("RunDataOfTray",   SS,     0);
        }
    }

    tRunData.LoaderIC               =INIFile->ReadInteger("RunData",    "LoaderIC",     0);
    tRunData.TotalIC                =INIFile->ReadInteger("RunData",    "TotalIC",      0);
    tRunData.iPauseTime             =INIFile->ReadInteger("RunData",    "iPauseTime",   0);
    tRunData.JamCount               =INIFile->ReadInteger("RunData",    "JamCount",     0);

    HSys.LastSet.SystemTimeRecordName[stStartTime]      ="Start Time";
    HSys.LastSet.SystemTimeRecordName[stPauseTime]      ="Pause Time";
    HSys.LastSet.SystemTimeRecordName[stPowerOn]        ="Power On Time";
    HSys.LastSet.SystemTimeRecordName[stProductTime]    ="Product Time";
    HSys.LastSet.SystemTimeRecordName[stJamTime]        ="Jam Time";
    HSys.LastSet.SystemTimeRecordName[stContactTest]    ="Contact Test Time";
    HSys.LastSet.SystemTimeRecordName[stHomeTime]       ="Home Time";
    HSys.LastSet.SystemTimeRecordName[stMTBA]           ="MTBA Time";

    for(int i=0; i<stTotalCnt; i++)
    {
        HSys.LastSet.SystemTimeRecord[i]=INIFile->ReadInteger("System Time Data", HSys.LastSet.SystemTimeRecordName[i], 0);
    }

    for(int i=0; i<eNozzleCount; i++)                                           //Sam 20170907 : 糤糒溃Ω计
    {
        SS="NozzleForce"+IntToStr(i)+"_CT" ;
        tRunData.NozzleForceCT[i]=INIFile->ReadInteger("RunData",   SS,     0);
    }

    tRunData.iEjectionPinCT=INIFile->ReadInteger("RunData",   "EjectionPinCT",     0);
    for(int i=0; i<HSys.iTotalCylinder; i++)
    {
        SS="Cylinder"+IntToStr(i);
        if(INIFile->ReadBool("Cylinder", SS, 0))
            HSys.LastSet.bCynStatus[i]=true;
        else
            HSys.LastSet.bCynStatus[i]=false;
    }
    for(int i=0; i<HSys.iTotalSwitch; i++)
    {
        SS="Switch"+IntToStr(i);
        HSys.LastSet.bSwStatus[i]=INIFile->ReadBool("Switch", SS, 0);
    }

    delete INIFile;
}
//==============================================================================
void WriteLastDataIni()
{
    AnsiString OpenIniFile=HSys.CurrentDir+AnsiString("\\system\\lastdata.ini");
    TIniFile *INIFile = new TIniFile(OpenIniFile);
    AnsiString SS="", Str="";

    INIFile->WriteInteger("System", "iLanguageCountry", HSys.LastSet.iLanguageCountry);
    INIFile->WriteInteger("System", "iStartMode",       HSys.LastSet.iStartMode);
    INIFile->WriteInteger("Motor",  "iTransferPos",     HSys.LastSet.iTransferPos);
    INIFile->WriteInteger("Motor",  "iShuttlePos",      HSys.LastSet.iShuttlePos);
    INIFile->WriteInteger("Motor",  "iAutoPos",         HSys.LastSet.iAutoPos);

    INIFile->WriteInteger("Contact", "iMaxContactCount", HSys.LastSet.iMaxContactCount);

    for(int i=0; i<stTotalCnt; i++)
    {
        INIFile->WriteInteger("System Time Data", HSys.LastSet.SystemTimeRecordName[i], HSys.LastSet.SystemTimeRecord[i]);
    }

    for(int i=0; i<HSys.iTotalCylinder; i++)
    {
        if(HSys.CynPtr[i].Enable)
        {
            SS="Cylinder"+IntToStr(i);
            INIFile->WriteBool("Cylinder", SS, HSys.LastSet.bCynStatus[i]);
        }
    }
    for(int i=0; i<HSys.iTotalSwitch; i++)
    {
        if(HSys.SwPtr[i].Enable)
        {
            SS="Switch"+IntToStr(i);
            INIFile->WriteBool("Switch", SS, HSys.LastSet.bSwStatus[i]);
        }
    }

    INIFile->WriteFloat("RunData",   "StartTime",    (double)tRunData.StartTime);
    INIFile->WriteFloat("RunData",   "LotEndTime",   (double)tRunData.LotEndTime);
    INIFile->WriteFloat("RunData",   "PauseTime",    (double)tRunData.PauseTime);
    INIFile->WriteFloat("RunData",   "AlarmTime",    (double)tRunData.AlarmTime);

    //Category_CT
    TStringList *sList      =new TStringList();
    sList->Clear();
    for(int i=0; i<TEST_MAX_BIN; i++)                                           //Sam 20170907 (jou):  Category Count 魁タ
    {
        sList->Add(AnsiString().sprintf("%d",tRunData.BinICCnt[i]));
    }
    Str=sList->CommaText;
    INIFile->WriteString( "RunData",    "Category_CT",     Str);
    sList->Clear();
    delete sList;
    //
    for(int i=eAuto1; i<eTrayCount; i++)
    {
        if(HSys.TrayUsage[i]==eNoTray)
        {
            tRunData.TrayICCnt[i]=0;
        }
        else
        {
            SS=fMain->sTrayNameBin[i]+IntToStr(i)+AnsiString("_CT");
            INIFile->WriteInteger("RunDataOfTray",   SS,    tRunData.TrayICCnt[i]);
        }
    }

    INIFile->WriteInteger("RunData",    "LoaderIC",     tRunData.LoaderIC);
    INIFile->WriteInteger("RunData",    "TotalIC",      tRunData.TotalIC);
    INIFile->WriteInteger("RunData",    "iPauseTime",   tRunData.iPauseTime);
    INIFile->WriteInteger("RunData",    "JamCount",     tRunData.JamCount);

    for(int i=0; i<eNozzleCount; i++)                                           //Sam 20170907 : 糤糒溃Ω计
    {
        SS="NozzleForce"+IntToStr(i)+"_CT" ;
        INIFile->WriteInteger("RunData",    SS,  tRunData.NozzleForceCT[i]);
    }
    INIFile->WriteInteger("RunData",    "EjectionPinCT",  tRunData.iEjectionPinCT);
    delete INIFile;
}
//==============================================================================
void UpdateAllParameter()                                                       // ben add 20110803 穝┮Τ笆把计 //
{
    //**************************************************************************//a*.cppず穝把计
    UpdateHomeParameter();
    UpdateLoaderParameter();
    UpdateSortArmParameter();
    UpdateTrayArmParameter();
    UpdateMagArmParameter();
    frm2DID->Update2DParameter();
    UpdateMagazineParameter();
    //**************************************************************************//a*.cppず穝把计
    if(CosFunction.bUseBinAutoConvert)
    {
        HSys.BinDisCtrl->WriteTargetBin(1);
    }
    else
    {
        int iBin[eBinDispTotal][TEST_MAX_BIN];
        int iCount[eBinDispTotal], iBDispAddr=0;
        for(int i=0; i<eBinDispTotal; i++)
        {
            iCount[i]=0;
            for(int iB=0; iB<TEST_MAX_BIN; iB++)
                iBin[i][iB]=-1;
        }
        MyBinToTrayStruct BtoT;
        for(int iB=1; iB<TEST_MAX_BIN; iB++)
        {
            BtoT.ConvertBinToAuto(iB);
            iBDispAddr=BtoT.iWhichAuto-eAuto1;
            if(iBDispAddr>=eBinNotUse &&
               iBDispAddr<(eBinDispTotal))
            {
                iBin[iBDispAddr][iCount[iBDispAddr]]=iB;
                iCount[iBDispAddr]++;
            }
        }
        for(int i=0; i<eBinDispTotal; i++)
            HSys.BinDisCtrl->WriteTargetBin(i, iBin[i], 1);
    }

    if(HSys.FuncT.iT04_BinDisptype==0)
    {
        HSys.BinDisCtrl->bFirstInit=true;
        HSys.BinDisCtrl->ProcessStopStart(true);
    }
    else
    {
        HSys.BinDisCtrlTFT->bFirstInit=true;
        HSys.BinDisCtrlTFT->ProcessStopStart(true);
    }
}
//==============================================================================
int TLatchCycleTime::LatchCycleTime(bool Start)
{
    int itmp =0;

    if (Start)
    {
        iCount =GetTickCount();
    }
    else
    {
        itmp = GetTickCount() - iCount;
    }

    return itmp;
}
//==============================================================================
void CustomerFunctionSelect()
{
    InitialCosFunction(); 
}
//------------------------------------------------------------------------------
int GetJamRateDenom()
{
    if(CUSTOMER_CODE==CC_WINSTEK)
    {
        tRunData.JamRateDenom=3000;
    }
    else
    {
        tRunData.JamRateDenom=10000;
    }
    return tRunData.JamRateDenom;
}
//------------------------------------------------------------------------------
AnsiString GetTotalQuantityAutoPercent()
{
    double Numerator=(double)tRunData.iAutoQuantity;
    double Denominator=(double)tRunData.GetTotalQuantity();
    double dPercent=cMathTool().Division(Numerator,Denominator)*100.0;
    AnsiString sret=AnsiString().sprintf("%2.1f", dPercent);;
    return sret;
}
//------------------------------------------------------------------------------
AnsiString GetTotalQuantityMagPercent()
{
    double Numerator=(double)tRunData.iMagQuantity;
    double Denominator=(double)tRunData.GetTotalQuantity();
    double dPercent=cMathTool().Division(Numerator,Denominator)*100.0;
    AnsiString sret=AnsiString().sprintf("%2.1f", dPercent);;
    return sret;
}
//------------------------------------------------------------------------------
