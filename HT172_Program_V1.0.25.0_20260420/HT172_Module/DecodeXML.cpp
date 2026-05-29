//------------------------------------------------------------------------------
#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "DecodeXML.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfXML *fXML;

#include <oxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>

const int SUMMARY_SUCCESS               =    1;
const int SUMMARY_FILE_NOT_EXIST        =   -4;
const int SUMMARY_DUPLICATE_TRAY_ID     =   -5;
const int TRAY_MAP_FILE_IS_NULL         =  -11;
const int TRAY_ID_IS_NULL               =  -12;
const int TRAY_ID_NOT_EXIST             =  -13;
const int TRAY_ID_DUPLICATE             =  -14;
//------------------------------------------------------------------------------
TXMLSummary myXML;
//------------------------------------------------------------------------------
map<AnsiString, TTrayMap>mapTrayMapList;
map<AnsiString, TTrayMap>::iterator IterTrayMapList;
//------------------------------------------------------------------------------
__fastcall TfXML::TfXML(TComponent* Owner)
    : TForm(Owner)
{
}
//------------------------------------------------------------------------------
TBinInfo::TBinInfo(AnsiString pass, int iCnt)
{
    iCount      =iCnt;
    sPassFail   =pass;
    bPass       =(pass=="P")?true:false;
}
//------------------------------------------------------------------------------
TXMLSummary::TXMLSummary()
{
    ResetData();
}
//------------------------------------------------------------------------------
TXMLSummary::TXMLSummary(AnsiString FileName)
{
    ConvertXMLToStrusture(FileName);
}
//------------------------------------------------------------------------------
TXMLSummary::~TXMLSummary()
{
    TRAY_MAP_LIST.clear();
    HBINSUM.clear();
    SBINSUM.clear();
}
//------------------------------------------------------------------------------
void TXMLSummary::ResetData()
{
    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        iTotalBinCount[i]=0;
    }

    TRAY_MAP_LIST.clear();
    HBINSUM.clear();
    SBINSUM.clear();

    if(fMain!=NULL)
        fMain->ShowLotBinCount(true);
}
//------------------------------------------------------------------------------
int TXMLSummary::ConvertXMLToStrusture(AnsiString FileName)
{
    ResetData();
    if(FileExists(FileName)==false)
        return SUMMARY_FILE_NOT_EXIST;

    AnsiString sTrayID, sFileName, sHBin, sSBin, str3, str4, str;
    TStringList *List=new TStringList;
    fXML->XMLDoc->LoadFromFile(FileName);
    _di_IXMLNodeList nodelst;                                                   //nodelst結點列表指向<enable>,<ladder>…這//一層結點
    _di_IXMLNode RNode=fXML->XMLDoc->DocumentElement;                           //Get access violations on this line
    _di_IXMLNode RNodeHead;
    _di_IXMLNode RNodeFTData;

    if(RNode!=NULL)
    {
        int iIndex=-1;
        iIndex=RNode->ChildNodes->IndexOf("HEADER");
        if(iIndex>=0)
        {
            nodelst=RNode->ChildNodes;
            RNodeHead=nodelst->operator [](iIndex);                             //取結點列表的第二個結點也就是<HEADER>

            iIndex=RNodeHead->ChildNodes->IndexOf("CUST_CODE");

            fMain->sHeadList->Clear();            
            for(int i=0; i<RNodeHead->ChildNodes->Count; i++)
            {
                str3=RNodeHead->ChildNodes->Nodes[i]->GetNodeName();
                str4=RNodeHead->ChildNodes->Nodes[i]->Text;
                str.sprintf("#%s %s", str3, str4);
                fMain->sHeadList->Add(str);
            }
        }

        iIndex=RNode->ChildNodes->IndexOf("TRAY_MAP_LIST");
        TRAY_MAP_LIST.clear();
        HBINSUM.clear();
        SBINSUM.clear();
        if(iIndex>=0)
        {
            nodelst     =RNode->ChildNodes;
            RNodeHead   =nodelst->operator [](iIndex);                          //取結點列表的第二個結點也就是<TRAY_MAP_LIST>
            nodelst     =RNodeHead->ChildNodes;
            for(int i=0; i<nodelst->Count; i++)
            {
                sFileName   =RNodeHead->ChildNodes->Nodes[i]->Text;                 //取得檔案名稱
                sTrayID     =RNodeHead->ChildNodes->Nodes[i]->GetAttribute("ID");   //取得Tray ID
                IterTRAY_MAP_LIST=TRAY_MAP_LIST.find(sTrayID);
                if(IterTRAY_MAP_LIST==TRAY_MAP_LIST.end())
                {
                    TRAY_MAP_LIST[sTrayID]=sFileName;
                }
                else
                {
                    List->Clear();
                    delete List;
                    return SUMMARY_DUPLICATE_TRAY_ID;
                }
            }
        }

        iIndex=RNode->ChildNodes->IndexOf("FTDATA");
        if(iIndex>=0)
        {
            nodelst     =RNode->ChildNodes;
            RNodeFTData =nodelst->operator [](iIndex);                          //取結點列表的第二個結點也就是<TRAY_MAP_LIST>
            iIndex      =RNodeFTData->ChildNodes->IndexOf("HBINSUM");
            if(iIndex>=0)
            {
                nodelst     =RNodeFTData->ChildNodes;
                RNodeHead   =nodelst->operator [](iIndex);                      //取結點列表的第二個結點也就是<HBINSUM>
                nodelst     =RNodeHead->ChildNodes;
                for(int i=0; i<nodelst->Count; i++)
                {
                    sHBin   =RNodeHead->ChildNodes->Nodes[i]->Text;             //取得檔案名稱
                    List->Clear();
                    List->CommaText=StringReplace(sHBin, "|", ",", TReplaceFlags()<<rfReplaceAll);
                    if(List->Count==3)
                        HBINSUM[atoi(List->Strings[0].c_str())]=TBinInfo(List->Strings[1], atoi(List->Strings[2].c_str()));
                }
            }

            iIndex=RNodeFTData->ChildNodes->IndexOf("SBINSUM");
            if(iIndex>=0)
            {
                nodelst     =RNodeFTData->ChildNodes;
                RNodeHead   =nodelst->operator [](iIndex);                      //取結點列表的第二個結點也就是<SBINSUM>
                nodelst     =RNodeHead->ChildNodes;
                for(int i=0; i<nodelst->Count; i++)
                {
                    sSBin   =RNodeHead->ChildNodes->Nodes[i]->Text;             //取得檔案名稱
                    List->Clear();
                    List->CommaText=StringReplace(sSBin, "|", ",", TReplaceFlags()<<rfReplaceAll);
                    if(List->Count==3)
                        SBINSUM[atoi(List->Strings[0].c_str())]=TBinInfo(List->Strings[1], atoi(List->Strings[2].c_str()));
                }
            }
        }
    }

    List->Clear();
    delete List;
    return SUMMARY_SUCCESS;
}
//------------------------------------------------------------------------------
TTrayMap::TTrayMap(AnsiString ID)
{
    InitialData();
    TRAY_ID=ID;
}
//------------------------------------------------------------------------------
void TTrayMap::InitialData()
{
    TRAY_ID     ="";
    iDevMaxRow  =MAX_TRAY_Row;
    iDevMaxCol  =MAX_TRAY_Col;

    for(int i=0; i<iDevMaxRow; i++)
    {
        for(int j=0; j<iDevMaxCol; j++)
        {
            DevInfo[i][j].Clear();
        }
    }
    iDevRealRow=0;
    iDevRealCol=0;
}
//------------------------------------------------------------------------------
TTrayMap::~TTrayMap()
{

}
//------------------------------------------------------------------------------
int DecodeTrayMapByJHTFormat(AnsiString sFileName)
{
    bool bStartSort=false;
    TDeviceInfo DevInfoByLot;
    TDeviceInfo DevInfo;

    AnsiString Str, sFullName;

    myXML.ResetData();
    sFullName.sprintf("D:\\BarcodeSorter\\%s", sFileName);                      //JerryYang 20240610 : ASECL download bin tray map
    mapTrayMapList.clear();
    sLotAuto1Info="";                                                           //Sam 20250724 : 新增 AutoSetBin By Lot Auto Info
    sLotAuto2Info="";
    sLotAuto3Info="";
    if(FileExists(sFullName)==false)
    {
        return SUMMARY_FILE_NOT_EXIST;
    }

    TStringList *List=new TStringList();
    List->LoadFromFile(sFullName);

    for(int i=0; i<List->Count; i++)
    {
        Str=List->Strings[i];

        if(bStartSort==true)
        {
            DevInfo.Clear();
            DevInfo.CopyFrom(DevInfoByLot);
            DevInfo.AddSourceDataByJHTFormat(Str);

            IterTrayMapList=mapTrayMapList.find(DevInfo.ClipID);
            if(IterTrayMapList==mapTrayMapList.end())
            {
                mapTrayMapList[DevInfo.ClipID]=TTrayMap(DevInfo.ClipID);
            }

            IterTrayMapList=mapTrayMapList.find(DevInfo.ClipID);
            IterTrayMapList->second.HANDLER_ID=DevInfoByLot.HANDLER_ID;
            IterTrayMapList->second.LOT_ID    =DevInfoByLot.LotID;
            IterTrayMapList->second.DevInfo[DevInfo.iInputR][DevInfo.iInputC].CopyFrom(DevInfo);
            myXML.iTotalBinCount[DevInfo.iBin]++;
            DevInfo.Clear();
        }
        else if(Str.AnsiPos("Lot_id")!=0)
        {
            DevInfoByLot.LotID=Str.SubString(7, Str.Length());
            DevInfoByLot.LotID=DevInfoByLot.LotID.Trim();
        }
        else if(Str.AnsiPos("Handler ID")!=0)
        {
            DevInfoByLot.HANDLER_ID=Str.SubString(12, Str.Length());
            DevInfoByLot.HANDLER_ID=DevInfoByLot.HANDLER_ID.Trim();
        }
        else if(Str.AnsiPos("Auto1-")!=0)                                       //Sam 20250724 : 新增 AutoSetBin By Lot Auto Info
        {
            sLotAuto1Info=Str.SubString(7, Str.Length());
            sLotAuto1Info=sLotAuto1Info.Trim();
            sLotAuto1Info=StringReplace(sLotAuto1Info, ".", ",", TReplaceFlags()<<rfReplaceAll);
        }
        else if(Str.AnsiPos("Auto2-")!=0)
        {
            sLotAuto2Info=Str.SubString(7, Str.Length());
            sLotAuto2Info=sLotAuto2Info.Trim();
            sLotAuto2Info=StringReplace(sLotAuto2Info, ".", ",", TReplaceFlags()<<rfReplaceAll);
        }
        else if(Str.AnsiPos("Auto3-")!=0)
        {
            sLotAuto3Info=Str.SubString(7, Str.Length());
            sLotAuto3Info=sLotAuto3Info.Trim();
            sLotAuto3Info=StringReplace(sLotAuto3Info, ".", ",", TReplaceFlags()<<rfReplaceAll);
        }
        else if(Str=="JHT_LOG_START")                                           //這個用來卡掉前面的無效資訊
        {
            bStartSort=true;
        }
    }
    fMain->ShowLotBinCount();
    List->Clear();
    delete List;
    return SUMMARY_SUCCESS;
}
//------------------------------------------------------------------------------
void SplitString(const AnsiString &input, TStringList *list, char delimiter)
{
    if(list==NULL)
    {
        return;
    }
    int start=1;
    int pos=input.Pos(AnsiString(delimiter));
    while(pos>0)
    {
        list->Add(input.SubString(start, pos - start));
        start=pos+1;
        AnsiString subStr=input.SubString(start, input.Length() - start + 1);
        pos=subStr.Pos(AnsiString(delimiter));
        if(pos > 0)
        {
            pos=start+pos-1;
        }
    }
    if(start<=input.Length())
    {
        list->Add(input.SubString(start, input.Length() - start + 1));
    }
}
//------------------------------------------------------------------------------
AnsiString GetTrayMapTrayID_Winstek(AnsiString sFileName)
{
    AnsiString sret="";
    TStringList *list=new TStringList;
    SplitString(sFileName, list,'_');
    if(list->Count>5)
    {
        sret=list->Strings[5];
    }
    delete list;
    return sret;
}
//------------------------------------------------------------------------------
AnsiString GetTrayMapData_Winstek(AnsiString sLineStr)
{
    AnsiString sret="";
    sLineStr=sLineStr.Trim();
    int spacePos=sLineStr.Pos(" ");
    if(spacePos>0)
    {
        sret=sLineStr.SubString(spacePos+1,sLineStr.Length()-spacePos);
        sret=sret.Trim();
    }
    return sret;
}
//------------------------------------------------------------------------------
int DecodeTrayMapByWinstekFormat(AnsiString sFileName)
{
    TDeviceInfo DevInfo;
    AnsiString sFullName, Str, sFile, str2;
    TStringList *List=new TStringList();
    sFullName.sprintf("D:\\BarcodeSorter\\%s.xml", sFileName);
    mapTrayMapList.clear();
    myXML.TRAY_MAP_LIST.clear();
    myXML.ConvertXMLToStrusture(sFullName);
    AnsiString sTraymapFilename="",sTRAY_ID="";
    for(myXML.IterTRAY_MAP_LIST=myXML.TRAY_MAP_LIST.begin(); myXML.IterTRAY_MAP_LIST!=myXML.TRAY_MAP_LIST.end(); myXML.IterTRAY_MAP_LIST++)
    {
        sTraymapFilename=myXML.IterTRAY_MAP_LIST->second;
        sFile=FileInfo().PathCombin(asNetDisk,sTraymapFilename);
        str2=FileInfo().PathCombin("D:\\BarcodeSorter\\Matrix\\",sTraymapFilename);
        #ifndef SOFT_SIMULATE                                                       //Sam 20240515 : 修正 Dummy Run
        if(FileExists(sFile))
        {
            try
            {
                CopyFile(sFile.c_str(), str2.c_str(), false);
            }
            catch(...)
            {}
        }
        else
        {
            str2.sprintf("File is not exist. %s", sFile);
            ShowMyMessage(str2);
        }
        #endif
        sFullName=FileInfo().PathCombin("D:\\BarcodeSorter\\Matrix\\",sTraymapFilename);
        if(FileExists(sFullName))
        {
            sTRAY_ID=GetTrayMapTrayID_Winstek(sTraymapFilename);
            IterTrayMapList=mapTrayMapList.find(sTRAY_ID);
            if(IterTrayMapList==mapTrayMapList.end())
            {
                mapTrayMapList[sTRAY_ID]=TTrayMap(sTRAY_ID);
                IterTrayMapList=mapTrayMapList.find(sTRAY_ID);
            }
            List->Clear();
            List->LoadFromFile(sFullName);
            for(int i=0; i<List->Count; i++)
            {
                Str=List->Strings[i];
                if(Str=="")
                {
                    continue;
                }
                else if(Str.AnsiPos("DEVICE_NAME")>0)
                {
                    IterTrayMapList->second.DEVICE_NAME=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("TEST_PROGRAM")>0)
                {
                    IterTrayMapList->second.TEST_PROGRAM=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("TESTER_ID")>0)
                {
                    IterTrayMapList->second.TESTER_ID=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("HANDLER_ID")>0)
                {
                    IterTrayMapList->second.HANDLER_ID=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("LOADBOARD_ID")>0)
                {
                    IterTrayMapList->second.LOADBOARD_ID=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("LOT_ID")>0)
                {
                    IterTrayMapList->second.LOT_ID=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("RUN_CARD_ID")>0)
                {
                    IterTrayMapList->second.RUN_CARD_ID=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("STAGE")>0)
                {
                    IterTrayMapList->second.STAGE=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("SUB_STAGE")>0)
                {
                    IterTrayMapList->second.SUB_STAGE=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("ATTRIBUTE")>0)
                {
                    IterTrayMapList->second.ATTRIBUTE=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("TRAY_ID")>0)
                {
                    IterTrayMapList->second.TRAY_ID=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("START_TIME")>0)
                {
                    IterTrayMapList->second.START_TIME=GetTrayMapData_Winstek(Str);
                }
                else if(Str.AnsiPos("END_TIME")>0)
                {
                    IterTrayMapList->second.END_TIME=GetTrayMapData_Winstek(Str);
                }
                else
                {
                    DevInfo.Clear();
                    DevInfo.AddSourceDataByWinstekFormat(Str);
                    if(DevInfo.iInputR<MAX_TRAY_Row &&
                       DevInfo.iInputR>=0 &&
                       DevInfo.iInputC<MAX_TRAY_Col &&
                       DevInfo.iInputC>=0)
                    {
                        IterTrayMapList->second.DevInfo[DevInfo.iInputR][DevInfo.iInputC].CopyFrom(DevInfo);
                        myXML.iTotalBinCount[DevInfo.iBin]++;
                    }
                }
            }
        }
        else
        {
            Str.sprintf("Can not find the file %s", sFullName);
            ShowMyMessage(Str);
        }
    }
    fMain->ShowLotBinCount();
    List->Clear();
    delete List;
    return SUMMARY_SUCCESS;
}
//------------------------------------------------------------------------------
int GetRandomBin()
{
    int i=random(1000)+1, iBin=1;

    if(i>=1 && i<=160)    //16%
    {
        iBin=1;
    }
    else if(i>=160 && i<=300)
    {
        iBin=2;
    }
    else if(i>=300 && i<=390)
    {
        iBin=3;
    }
    else if(i>=390 && i<=460)
    {
        iBin=4;
    }
    else if(i>=460 && i<=510)
    {
        iBin=5;
    }
    else if(i>=500 && i<=550)
    {
        iBin=6;
    }
    else if(i>=550 && i<=590)
    {
        iBin=7;
    }
    else if(i>=590 && i<=630)
    {
        iBin=8;
    }
    else if(i>=630 && i<=670)
    {
        iBin=9;
    }
    else if(i>=670 && i<=700)
    {
        iBin=10;
    }
    else if(i>=700 && i<=730)
    {
        iBin=11;
    }
    else if(i>=730 && i<=750)
    {
        iBin=12;
    }
    else if(i>=750 && i<=770)
    {
        iBin=13;
    }
    else if(i>=770 && i<=790)
    {
        iBin=14;
    }
    else if(i>=790 && i<=810)
    {
        iBin=15;
    }
    else if(i>=810 && i<=830)
    {
        iBin=16;
    }
    else if(i>=830 && i<=845)
    {
        iBin=17;
    }
    else if(i>=845 && i<=860)
    {
        iBin=18;
    }
    else if(i>=860 && i<=875)
    {
        iBin=19;
    }
    else if(i>=875 && i<=911)
    {
        iBin=20;
    }
    else if(i>=911 && i<=921)
    {
        iBin=21;
    }
    else if(i>=921 && i<=930)
    {
        iBin=22;
    }
    else if(i>=930 && i<=936)
    {
        iBin=23;
    }
    else if(i>=936 && i<=942)
    {
        iBin=24;
    }
    else if(i>=942 && i<=950)
    {
        iBin=25;
    }
    else if(i>=950 && i<=958)
    {
        iBin=26;
    }
    else if(i>=958 && i<=962)
    {
        iBin=27;
    }
    else if(i>=962 && i<=965)
    {
        iBin=28;
    }
    else if(i>=965 && i<=970)
    {
        iBin=29;
    }
    else if(i>=970 && i<=975)
    {
        iBin=30;
    }
    else if(i>=975 && i<=977)
    {
        iBin=31;
    }
    else if(i>=977 && i<=980)
    {
        iBin=32;
    }
    else if(i>=980 && i<=983)
    {
        iBin=33;
    }
    else if(i>=983 && i<=986)
    {
        iBin=34;
    }
    else if(i>=986 && i<=988)
    {
        iBin=35;
    }
    else if(i>=988 && i<=990)
    {
        iBin=36;
    }
    else if(i>=990 && i<=994)
    {
        iBin=37;
    }
    else if(i>=994 && i<=996)
    {
        iBin=38;
    }
    else if(i>=996 && i<=998)
    {
        iBin=39;
    }
    else if(i>=998 && i<=999)
    {
        iBin=40;
    }
    else if(i>=999 && i<=1000)
    {
        iBin=1;
    }else
    {
        iBin=1;
    }
    return iBin;
}
//------------------------------------------------------------------------------
int DecodeTrayMapByEKeeperFormat(AnsiString SummaryFileName)
{
    bool bStartSort=false;
    TDeviceInfo DevInfo;
    TDeviceInfo DevInfoByRow;
    AnsiString sFullName, Str, sID;
    int iPos;
    myXML.ResetData();
    sFullName.sprintf("D:\\BarcodeSorter\\Matrix\\%s.txt", SummaryFileName);
    mapTrayMapList.clear();

    if(FileExists(sFullName)==false)
    {
        return SUMMARY_FILE_NOT_EXIST;
    }

    TStringList *List       =new TStringList();
    TStringList *ListByTray =new TStringList();
    TStringList *ListByRow  =new TStringList();
    List->LoadFromFile(sFullName);

    for(int i=0; i<List->Count; i++)
    {
        Str=List->Strings[i];
        if(Str=="[BinMatrix]")
        {
            bStartSort=true;
        }
        else if(bStartSort)
        {
            iPos=Str.AnsiPos("=");
            if(iPos!=0)
            {
                DevInfoByRow.ClipID=Str.SubString(0, iPos-1);
                IterTrayMapList=mapTrayMapList.find(DevInfoByRow.ClipID);
                if(IterTrayMapList==mapTrayMapList.end())
                {
                    mapTrayMapList[DevInfoByRow.ClipID]=TTrayMap(DevInfoByRow.ClipID);
                }
                IterTrayMapList=mapTrayMapList.find(DevInfoByRow.ClipID);

                Str=Str.SubString(iPos+1, Str.Length());
                ListByTray->Text=StringReplace(Str, ";", "\n", TReplaceFlags()<<rfReplaceAll);

                for(int iR=0; iR<ListByTray->Count; iR++)
                {
                    ListByRow->CommaText=ListByTray->Strings[iR];
                    for(int iC=0; iC<ListByRow->Count; iC++)
                    {
                        DevInfo.Clear();
                        DevInfo.CopyFrom(DevInfoByRow);
//                        DevInfo.AddSourceDataByHonPrecCSVFormat(iR, iC, atoi(ListByRow->Strings[iC].c_str()));
                        #ifdef FORCE_RANDOM
                        DevInfo.AddSourceDataByHonPrecCSVFormat(iR, iC, GetRandomBin());
                        #else
                        DevInfo.AddSourceDataByHonPrecCSVFormat(iR, iC, atoi(ListByRow->Strings[iC].c_str()));
                        #endif
                        IterTrayMapList->second.iDevRealCol=iC+1;
                        IterTrayMapList->second.iDevRealRow=iR+1;
                        IterTrayMapList->second.DevInfo[DevInfo.iInputR][DevInfo.iInputC].CopyFrom(DevInfo);
                        myXML.iTotalBinCount[DevInfo.iBin]++;
                    }
                }
            }
        }
    }
    fMain->ShowLotBinCount();
    List->Clear();
    ListByTray->Clear();
    ListByRow->Clear();
    delete List;
    delete ListByTray;
    delete ListByRow;
    return SUMMARY_SUCCESS;
}
//------------------------------------------------------------------------------
int GetNextBinNum(int iNowBin)
{
    if(iNowBin>=tSimuData.iMaxBin)
    {
        return 1;
    }
    else
    {
        return iNowBin+1;
    }
}
//------------------------------------------------------------------------------
int DecodeTrayMapBySimulation()
{
    TDeviceInfo DevInfo;
    myXML.ResetData();
    mapTrayMapList.clear();
    mapTrayMapList[tSimuData.sClipID]=TTrayMap(tSimuData.sClipID);
    IterTrayMapList=mapTrayMapList.find(tSimuData.sClipID);
    int iNowBin=1;
    for(int iR=0; iR<LoaderModule->iYDivision; iR++)
    {
        for(int iC=0; iC<LoaderModule->iXDivision; iC++)
        {
            DevInfo.Clear();
            DevInfo.AddSourceDataByHonPrecCSVFormat(iR, iC, iNowBin);
            IterTrayMapList->second.iDevRealCol=iC+1;
            IterTrayMapList->second.iDevRealRow=iR+1;
            IterTrayMapList->second.DevInfo[DevInfo.iInputR][DevInfo.iInputC].CopyFrom(DevInfo);
            myXML.iTotalBinCount[DevInfo.iBin]++;
            iNowBin=GetNextBinNum(iNowBin);
        }
    }
    fMain->ShowLotBinCount();
    return SUMMARY_SUCCESS;
}
//------------------------------------------------------------------------------






