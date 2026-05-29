//------------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop
//------------------------------------------------------------------------------

#pragma package(smart_init)
AnsiString asTravelingLogPath       ="D:\\HT-172_Log\\Production_Log";
//------------------------------------------------------------------------------
TDeviceInfo::TDeviceInfo()
{
    Clear();
}
//------------------------------------------------------------------------------
void TDeviceInfo::CopyFrom(TDeviceInfo &Source)
{
    iBin        =Source.iBin;
    CodeReader  =Source.CodeReader;
    iAuto       =Source.iAuto;
    ClipID      =Source.ClipID;
    InputClipID =Source.InputClipID;
    OutputClipID=Source.OutputClipID;
    ErrorCode   =Source.ErrorCode;

    iInputR     =Source.iInputR;
    iInputC     =Source.iInputC;
    iOutputR    =Source.iOutputR;
    iOutputC    =Source.iOutputC;

    iSuckArm    =Source.iSuckArm;
    iSuckR      =Source.iSuckR;
    iSuckC      =Source.iSuckC;

    Arm         =Source.Arm;
    SiteNO      =Source.SiteNO;
    Indext_Time =Source.Indext_Time;
    Test_Time   =Source.Test_Time;
    OrgStr      =Source.OrgStr;
    Picker      =Source.Picker;
    DeviceName  =Source.DeviceName;
    LotID       =Source.LotID;
    SoftBin     =Source.SoftBin;
    TimeFlag    =Source.TimeFlag;

    TEST_PROGRAM=Source.TEST_PROGRAM;
    TESTER_ID   =Source.TESTER_ID   ;
    HANDLER_ID  =Source.HANDLER_ID  ;
    LOADBOARD_ID=Source.LOADBOARD_ID;
    RUN_CARD_ID =Source.RUN_CARD_ID ;
    STAGE       =Source.STAGE       ;
    SUB_STAGE   =Source.SUB_STAGE   ;
    ATTRIBUTE   =Source.ATTRIBUTE   ;
    START_TIME  =Source.START_TIME  ;
    END_TIME    =Source.END_TIME    ;
    Device2DID  =Source.Device2DID  ;

    sStartTime  =Source.sStartTime;
    sLoadTime   =Source.sLoadTime;
    sWhichAuto  =Source.sWhichAuto;
    sUnloaderTime   =Source.sUnloaderTime;
    sErrorCode  =Source.sErrorCode;

};
//------------------------------------------------------------------------------
void TDeviceInfo::Clear()
{
    CodeReader  ="";
    iBin        =0;
    iAuto       =0;
    CodeReader  ="";
    ClipID      ="";
    InputClipID ="";
    OutputClipID="";
    ErrorCode   ="";

    iInputR     =-1;
    iInputC     =-1;
    iOutputR    =-1;
    iOutputC    =-1;

    iSuckArm    =-1;
    iSuckR      =-1;
    iSuckC      =-1;

    Arm         ="";
    SiteNO      ="";
    Indext_Time ="";
    Test_Time   ="";
    OrgStr      ="";
    Picker      ="";
    DeviceName  ="";
    LotID       ="";
    SoftBin     ="";
    TimeFlag    ="";

    TEST_PROGRAM="";
    TESTER_ID   ="";
    HANDLER_ID  ="";
    LOADBOARD_ID="";
    RUN_CARD_ID ="";
    STAGE       ="";
    SUB_STAGE   ="";
    ATTRIBUTE   ="";
    START_TIME  ="";
    END_TIME    ="";
    Device2DID    ="";

    sStartTime   ="";
    sLoadTime   ="";
    sWhichAuto ="";
    sUnloaderTime ="";
    sErrorCode ="";
};
//------------------------------------------------------------------------------
void TDeviceInfo::MoveFrom(TDeviceInfo &Source)
{
    CopyFrom(Source);
    Source.Clear();
};
//------------------------------------------------------------------------------
void TDeviceInfo::AddInputInfo(AnsiString _ClipID, int iTrayR, int iTrayC, int _iSuckArm, int _iSuckR, int _iSuckC)
{
    sStartTime  =FormatDateTime("yyyy-mm-dd_hhnnss", tRunData.StartTime);
    iInputR     =iTrayR;
    iInputC     =iTrayC;
    InputClipID =_ClipID;

    iSuckArm    =_iSuckArm;
    iSuckR      =_iSuckR;
    iSuckC      =_iSuckC;

    sLoadTime=GetNowTime();
}
//------------------------------------------------------------------------------
void TDeviceInfo::AddOutputInfo(int _iWhichAuto,AnsiString _ClipID, int iTrayR, int iTrayC)
{
    sWhichAuto   =fMain->sTrayNameBin[_iWhichAuto];
    iOutputR     =iTrayR;
    iOutputC     =iTrayC;
    OutputClipID =_ClipID;
    sUnloaderTime=GetNowTime();
}
//------------------------------------------------------------------------------
void TDeviceInfo::AddSourceDataByJHTFormat(AnsiString Str)
{
    TStringList *List=new TStringList();
    int iPos;
    AnsiString SubStr=Str, Str2;
    AnsiString subStrBin, subStrRow, subStrCol, subStrID;

    OrgStr          =Str;

    iPos            =SubStr.AnsiPos("$");                                       //Date Time
    TimeFlag        =SubStr.SubString(0, iPos-1);

    SubStr          =SubStr.SubString(iPos+1, SubStr.Length());
    iPos            =SubStr.AnsiPos("$");                                       //Device Name
    DeviceName      =SubStr.SubString(0, iPos-1);

    SubStr          =SubStr.SubString(iPos+1, SubStr.Length());
    iPos            =SubStr.AnsiPos("$");                                       //Lot ID
    LotID           =SubStr.SubString(0, iPos-1);

    Str2=StringReplace(Str.SubString(Str.AnsiPos("$ Bin"), Str.Length()), " $ ", ",", TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, "Bin: ",       "HBin,",    TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, "Picker#",     "Picker,",  TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, "TrayID: (",   "TrayID,",  TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, "Row(Y):",     "RowY,",    TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, "Col(X):",     "ColX,",    TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, "DUT#",        "DUT,",     TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, "SoftBin:",    "SBin,",    TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, "(",           ",",        TReplaceFlags()<<rfReplaceAll);
    Str2=StringReplace(Str2, ")",           ",",        TReplaceFlags()<<rfReplaceAll);

    List->CommaText=Str2;
    for(int j=0; j<List->Count; j++)
    {
        if(List->Strings[j].AnsiPos("HBin")!=0)
        {
            subStrBin=List->Strings[j+1];
        }
        else if(List->Strings[j].AnsiPos("RowY")!=0)
        {
            subStrRow=List->Strings[j+1];
        }
        else if(List->Strings[j].AnsiPos("ColX")!=0)
        {
            subStrCol=List->Strings[j+1];
        }
        else if(List->Strings[j].AnsiPos("TrayID")!=0)
        {
            ClipID=List->Strings[j+1];
        }
        else if(List->Strings[j].AnsiPos("Picker")!=0)
        {
            Picker=List->Strings[j+1];
        }
        else if(List->Strings[j].AnsiPos("SBin")!=0)
        {
            SoftBin=List->Strings[j+1];
        }
    }

    iBin    =atoi(subStrBin.c_str());
    iInputR =atoi(subStrRow.c_str());
    iInputC =atoi(subStrCol.c_str());
    List->Clear();
    delete List;
}
//------------------------------------------------------------------------------
void TDeviceInfo::AddSourceDataByWinstekFormat(AnsiString Str)
{
    TStringList *List=new TStringList();
    AnsiString SubStr=Str, Str2;
    AnsiString subStrBin, subStrRow, subStrCol, subStrID;

    List->Clear();
    OrgStr=Str;
    Str=StringReplace(Str, "    ", ",", TReplaceFlags()<<rfReplaceAll);
    Str=StringReplace(Str, "   " , ",", TReplaceFlags()<<rfReplaceAll);
    Str=StringReplace(Str, "  "  , ",", TReplaceFlags()<<rfReplaceAll);
    Str=StringReplace(Str, " "   , ",", TReplaceFlags()<<rfReplaceAll);
    List->CommaText=Str;

    if(List->Count>=7)
    {
        iInputC     =atoi(List->Strings[0].c_str())-1;                          //Steven 20240603 : IC座標從1開始, 所以要-1
        iInputR     =atoi(List->Strings[1].c_str())-1;
        iBin        =atoi(List->Strings[2].c_str());
        Arm         =List->Strings[3];
        SiteNO      =List->Strings[4];
        Indext_Time =List->Strings[5];
        Test_Time   =List->Strings[6];
        if(List->Count>=8)                                                      //Jimmychiu 20240106 : Add Device2DID in device info
            Device2DID=List->Strings[7];
    }
    List->Clear();
    delete List;
}
//------------------------------------------------------------------------------
void TDeviceInfo::AddSourceDataByHonPrecCSVFormat(int iTrayR, int iTrayC, int _iBin)
{
    iBin    =_iBin;
    iInputC =iTrayC;
    iInputR =iTrayR;
}
//------------------------------------------------------------------------------
void TDeviceInfo::SetErrorCode(AnsiString sErr)
{
    sErrorCode=sErr;
}
//------------------------------------------------------------------------------
AnsiString TDeviceInfo::GetDataFilePath()
{
    AnsiString asTravelingLogPath       ="D:\\HT-172_Log\\Production_Log";
    AnsiString asStr="", asPath="", asFileName="";
    AnsiString sYYYYMM=FormatDateTime("yyyymm", tRunData.StartTime);            //Sam 20250903 : 修正跨月份存檔問題
    asPath.sprintf("%s\\%s\\", asTravelingLogPath, sYYYYMM);
    MyForceDirectories(asPath);
    if(CUSTOMER_CODE==CC_PTI)
        asFileName.sprintf("%s_%s.csv", fMain->edLotNo->Text, FormatDateTime("yyyy-mm-dd_hhnnss", tRunData.StartTime));
    else
        asFileName.sprintf("%s_%04d%02d%02d.csv", "HT_172", SystemYear, SystemMonth, SystemDate);     //Steven 20170123 (Jou) : 修改檔案命名格式
    asPath+=asFileName;
    return asPath;
}
//------------------------------------------------------------------------------
void TDeviceInfo::SaveRecord()
{
    AnsiString asPath=GetDataFilePath();
    bool bHasFile=FileExists(asPath);
    FILE* file=fopen(asPath.c_str(),"a");
    if(file!=NULL)
    {
        if(bHasFile==false)
        {
            fputs(GetTitleList().c_str(), file);
        }
        fputs(GetDatasList().c_str(), file);
        fclose(file);
    }
    else
    {
        asPath+=".bak";
        bHasFile=FileExists(asPath);
        file=fopen(asPath.c_str(),"a");
        if(file!=NULL)
        {
            if(bHasFile==false)
            {
                fputs(GetTitleList().c_str(), file);
            }
            fputs(GetDatasList().c_str(), file);
            fclose(file);
        }
        else
        {
            RecordProcess(AnsiString().sprintf("IC Record Save Fail,%s,Path=%s",__FUNC__,asPath));
        }
    }
}
//------------------------------------------------------------------------------
AnsiString TDeviceInfo::GetTitleList()
{
    AnsiString sDataList[eDataTotal];
    sDataList[eStartTime]="Start Time";
    sDataList[eLoadX]="Load_X";
    sDataList[eLoadY]="Load_Y";
    sDataList[eLoadTime]="Load_Time";
    sDataList[eLoadTrayID]="Tray_ID";
    sDataList[eWhichArm]="Which Arm";
    sDataList[eSuckX]="Suck_X";
    sDataList[eSuckY]="Suck_Y";
    sDataList[eWhichAuto]="Which Auto";
    sDataList[eBin]="Bin";
    sDataList[eOutTrayID]="Output tray";
    sDataList[eUnloadX]="Unload_X";
    sDataList[eUnloadY]="Unload_Y";
    sDataList[eUnloadTime]="Unload_Time";
    sDataList[eErrorCode]="Error log";
    AnsiString strBack="";
    for(int i=0;i<eDataTotal;i++)
    {
        if(i!=0)
            strBack+=",";
        strBack+=sDataList[i];
    }
    strBack+="\n";
    return strBack;
}
//------------------------------------------------------------------------------
AnsiString TDeviceInfo::GetDatasList()
{
    AnsiString sDataList[eDataTotal];
    sDataList[eStartTime]=sStartTime;
    sDataList[eLoadX]=iInputC;
    sDataList[eLoadY]=iInputR;
    sDataList[eLoadTime]=sLoadTime;
    sDataList[eLoadTrayID]=InputClipID;
    sDataList[eWhichArm]=iSuckArm;
    sDataList[eSuckX]=iSuckC;
    sDataList[eSuckY]=iSuckR;
    sDataList[eWhichAuto]=sWhichAuto;
    sDataList[eBin]=SoftBin;
    sDataList[eOutTrayID]=OutputClipID;
    sDataList[eUnloadX]=iOutputC;
    sDataList[eUnloadY]=iOutputR;
    sDataList[eUnloadTime]=sUnloaderTime;
    sDataList[eErrorCode]=sErrorCode;
    AnsiString strBack="";
    for(int i=0;i<eDataTotal;i++)
    {
        if(i!=0)
            strBack+=",";
        strBack+=sDataList[i];
    }
    strBack+="\n";
    return strBack;
}
//------------------------------------------------------------------------------
