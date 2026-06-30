#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop
//==============================================================================
#include "main.h"
#include "cmydef.h"
#include "cinitial.h"
#include "aseTest.h"
#include "TrayStepMotor.h"
#include "DecodeXML.h"
#include "FTPClient.h"
#include "automation.h"
#include "Auto172.h"


#pragma package(smart_init)
#pragma link "ALed"
#pragma link "HTray"
#pragma link "MyLed"
#pragma resource "*.dfm"
TfMain *fMain;
HAlarm *Alarm;
extern THGem *HGem;
AnsiString GetFileInfoItem(AnsiString FName, AnsiString InfoItem);
TStringList *PassList[4];   //Steven 20101109
AnsiString  UseName;        //Steven 20101109

//------------------------------------------------------------------------------
__fastcall TfMain::TfMain(TComponent* Owner)
    : TForm(Owner)
{
    MainVersion=VerInfo().GetSVNRev();
#ifdef __CODEGUARD__
    if(CUSTOMER_CODE==CC_HONPREC_QC)
        statMain->Panels->Items[emsVersion]->Text=MainVersion+".QC.CG";
    else
        statMain->Panels->Items[emsVersion]->Text=MainVersion+".CG";                            //20150119 wei add 版本
#else
    if(CUSTOMER_CODE==CC_HONPREC_QC)
        statMain->Panels->Items[emsVersion]->Text=MainVersion+".QC";
    else
        statMain->Panels->Items[emsVersion]->Text=MainVersion;                                  //20150119 wei add 版本
#endif
//----------------------------------
    for(int i=0; i<eTrayCount; i++)
    {
        if(i==eBinNotUse)
        {
            sTrayNameEKeeper[i].sprintf("NotUse");
            sTrayNameSpace[i].sprintf("Not Use");
            sTrayNameBin[i].sprintf("NotUse");
            sTrayNameCateData[i].sprintf("NotUse");                             //生產用的CateData
        }
        else if(i>=eAuto1 && i<=eAuto20)
        {
            sTrayNameEKeeper[i].sprintf("Auto%d", i);
            sTrayNameSpace[i].sprintf("Auto %d", i);
            sTrayNameBin[i].sprintf("Auto%02d", i);
            sTrayNameCateData[i].sprintf("Auto%02d", i);
        }
        else if(i==eFix1 || i==eFix2)
        {
            sTrayNameEKeeper[i].sprintf("Fix%d", i-eFix1+1);
            sTrayNameSpace[i].sprintf("Fix %d", i-eFix1+1);
            sTrayNameBin[i].sprintf("Fix%02d", i-eFix1+1);
            sTrayNameCateData[i].sprintf("Fix%02d", i-eFix1+1);
        }
        else if(i>=eMag1_01 && i<=eMag1_27)
        {
            if(MAGAZINE_CNT==1)
            {
                sTrayNameEKeeper[i].sprintf("Mag%d", i-eMag1_01+1);
                sTrayNameSpace[i].sprintf("Mag %d", i-eMag1_01+1);
                sTrayNameBin[i].sprintf("Mag2%02d", i-eMag1_01+1);              //故意變成Mag2, 設定bin的時候會跟Mag1相反
            }
            else
            {
                sTrayNameEKeeper[i].sprintf("Mag1%02d", i-eMag1_01+1);
                sTrayNameSpace[i].sprintf("Mag1 %02d", i-eMag1_01+1);
                sTrayNameBin[i].sprintf("Mag1%02d", i-eMag1_01+1);
            }
            sTrayNameCateData[i].sprintf("Mag1%02d", i-eMag1_01+1);
            sTrayNameSpace[i].sprintf("Mag1 %02d", i-eMag1_01+1);
        }
        else if(i>=eMag2_01 && i<=eMag2_27)
        {
            sTrayNameEKeeper[i].sprintf("Mag2%02d", i-eMag2_01+1);
            sTrayNameSpace[i].sprintf("Mag2 %02d", i-eMag2_01+1);
            if(MAGAZINE_CNT==1)
                sTrayNameBin[i].sprintf("Mag1%02d", i-eMag2_01+1);              //故意變成Mag1
            else
                sTrayNameBin[i].sprintf("Mag2%02d", i-eMag2_01+1);
            sTrayNameCateData[i].sprintf("Mag2%02d", i-eMag2_01+1);
        }
        else if(i>=eMag3_01 && i<=eMag3_27)
        {
            sTrayNameEKeeper[i].sprintf("Mag3%02d", i-eMag3_01+1);
            sTrayNameBin[i].sprintf("Mag3%02d", i-eMag3_01+1);
            sTrayNameSpace[i].sprintf("Mag3 %02d", i-eMag3_01+1);
            sTrayNameCateData[i].sprintf("Mag3%02d", i-eMag3_01+1);
        }
    }
    bNeedChange2OP=false;
    sr=new cStateRecord();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::FormCreate(TObject *Sender)
{
    Application->OnException = AppException;
    //Click Event,Auto Save log
    //==>
    BV=new TStringList;
    BL=new TList;
    sHeadList=new TStringList;
    //<==
    //Click Event,Auto Save log

    for(int i=0; i<4; i++)
    {
        HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].Off();
    }

    TPanel *tTMotPanel[]={palNotUse,
                          palAuto01, palAuto02, palAuto03, palAuto04, palAuto05, palAuto06, palAuto07, palAuto08, palAuto09, palAuto10,
                          palAuto11, palAuto12, palAuto13, palAuto14, palAuto15, palAuto16, palAuto17, palAuto18, palAuto19, palAuto20,
                          palMag1,   palMag3,
                          pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,
                          pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,
                          pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,   pnlMgz1,
                          pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,
                          pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,
                          pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,   pnlMgz2,
                          pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,
                          pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,
                          pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,   pnlMgz3,
                         };

    TPanel *tTCntPanel[]={palNotUseCnt,
                          palAuto01Cnt, palAuto02Cnt, palAuto03Cnt, palAuto04Cnt, palAuto05Cnt, palAuto06Cnt, palAuto07Cnt, palAuto08Cnt, palAuto09Cnt, palAuto10Cnt,
                          palAuto11Cnt, palAuto12Cnt, palAuto13Cnt, palAuto14Cnt, palAuto15Cnt, palAuto16Cnt, palAuto17Cnt, palAuto18Cnt, palAuto19Cnt, palAuto20Cnt,
                          palMag1TopCnt,palMag3TopCnt,
                          palMag1_01Cn, palMag1_02Cn, palMag1_03Cn, palMag1_04Cn, palMag1_05Cn, palMag1_06Cn, palMag1_07Cn, palMag1_08Cn, palMag1_09Cn, palMag1_10Cn,
                          palMag1_11Cn, palMag1_12Cn, palMag1_13Cn, palMag1_14Cn, palMag1_15Cn, palMag1_16Cn, palMag1_17Cn, palMag1_18Cn, palMag1_19Cn, palMag1_20Cn,
                          palMag1_21Cn, palMag1_22Cn, palMag1_23Cn, palMag1_24Cn, palMag1_25Cn, palMag1_26Cn, palMag1_27Cn,
                          palMag2_01Cn, palMag2_02Cn, palMag2_03Cn, palMag2_04Cn, palMag2_05Cn, palMag2_06Cn, palMag2_07Cn, palMag2_08Cn, palMag2_09Cn, palMag2_10Cn,
                          palMag2_11Cn, palMag2_12Cn, palMag2_13Cn, palMag2_14Cn, palMag2_15Cn, palMag2_16Cn, palMag2_17Cn, palMag2_18Cn, palMag2_19Cn, palMag2_20Cn,
                          palMag2_21Cn, palMag2_22Cn, palMag2_23Cn, palMag2_24Cn, palMag2_25Cn, palMag2_26Cn, palMag2_27Cn,
                          palMag3_01Cn, palMag3_02Cn, palMag3_03Cn, palMag3_04Cn, palMag3_05Cn, palMag3_06Cn, palMag3_07Cn, palMag3_08Cn, palMag3_09Cn, palMag3_10Cn,
                          palMag3_11Cn, palMag3_12Cn, palMag3_13Cn, palMag3_14Cn, palMag3_15Cn, palMag3_16Cn, palMag3_17Cn, palMag3_18Cn, palMag3_19Cn, palMag3_20Cn,
                          palMag3_21Cn, palMag3_22Cn, palMag3_23Cn, palMag3_24Cn, palMag3_25Cn, palMag3_26Cn, palMag3_27Cn,
                         };

    TPanel *tTLotCntPanel[]={palNotUseLotCnt,
                             palAuto01LotCnt,  palAuto02LotCnt,  palAuto03LotCnt,  palAuto04LotCnt,  palAuto05LotCnt,  palAuto06LotCnt,  palAuto07LotCnt,  palAuto08LotCnt,  palAuto09LotCnt,  palAuto10LotCnt,
                             palAuto11LotCnt,  palAuto12LotCnt,  palAuto13LotCnt,  palAuto14LotCnt,  palAuto15LotCnt,  palAuto16LotCnt,  palAuto17LotCnt,  palAuto18LotCnt,  palAuto19LotCnt,  palAuto20LotCnt,
                             palMag1TopLotCnt, palMag3TopLotCnt,
                             palMag1_01LotCnt, palMag1_02LotCnt, palMag1_03LotCnt, palMag1_04LotCnt, palMag1_05LotCnt, palMag1_06LotCnt, palMag1_07LotCnt, palMag1_08LotCnt, palMag1_09LotCnt, palMag1_10LotCnt,
                             palMag1_11LotCnt, palMag1_12LotCnt, palMag1_13LotCnt, palMag1_14LotCnt, palMag1_15LotCnt, palMag1_16LotCnt, palMag1_17LotCnt, palMag1_18LotCnt, palMag1_19LotCnt, palMag1_20LotCnt,
                             palMag1_21LotCnt, palMag1_22LotCnt, palMag1_23LotCnt, palMag1_24LotCnt, palMag1_25LotCnt, palMag1_26LotCnt, palMag1_27LotCnt,
                             palMag2_01LotCnt, palMag2_02LotCnt, palMag2_03LotCnt, palMag2_04LotCnt, palMag2_05LotCnt, palMag2_06LotCnt, palMag2_07LotCnt, palMag2_08LotCnt, palMag2_09LotCnt, palMag2_10LotCnt,
                             palMag2_11LotCnt, palMag2_12LotCnt, palMag2_13LotCnt, palMag2_14LotCnt, palMag2_15LotCnt, palMag2_16LotCnt, palMag2_17LotCnt, palMag2_18LotCnt, palMag2_19LotCnt, palMag2_20LotCnt,
                             palMag2_21LotCnt, palMag2_22LotCnt, palMag2_23LotCnt, palMag2_24LotCnt, palMag2_25LotCnt, palMag2_26LotCnt, palMag2_27LotCnt,
                             palMag3_01LotCnt, palMag3_02LotCnt, palMag3_03LotCnt, palMag3_04LotCnt, palMag3_05LotCnt, palMag3_06LotCnt, palMag3_07LotCnt, palMag3_08LotCnt, palMag3_09LotCnt, palMag3_10LotCnt,
                             palMag3_11LotCnt, palMag3_12LotCnt, palMag3_13LotCnt, palMag3_14LotCnt, palMag3_15LotCnt, palMag3_16LotCnt, palMag3_17LotCnt, palMag3_18LotCnt, palMag3_19LotCnt, palMag3_20LotCnt,
                             palMag3_21LotCnt, palMag3_22LotCnt, palMag3_23LotCnt, palMag3_24LotCnt, palMag3_25LotCnt, palMag3_26LotCnt, palMag3_27LotCnt,
                               };

    TPanel *tTBinPanel[]={palNotUseBin,
                          palAuto01Bin,  palAuto02Bin,  palAuto03Bin,   palAuto04Bin,  palAuto05Bin,  palAuto06Bin,  palAuto07Bin,  palAuto08Bin,  palAuto09Bin,  palAuto10Bin,
                          palAuto11Bin,  palAuto12Bin,  palAuto13Bin,   palAuto14Bin,  palAuto15Bin,  palAuto16Bin,  palAuto17Bin,  palAuto18Bin,  palAuto19Bin,  palAuto20Bin,
                          pnlMgz1TopBin, pnlMgz3TopBin,
                          palMag1_01Bin, palMag1_02Bin, palMag1_03Bin,  palMag1_04Bin, palMag1_05Bin, palMag1_06Bin, palMag1_07Bin, palMag1_08Bin, palMag1_09Bin, palMag1_10Bin,
                          palMag1_11Bin, palMag1_12Bin, palMag1_13Bin,  palMag1_14Bin, palMag1_15Bin, palMag1_16Bin, palMag1_17Bin, palMag1_18Bin, palMag1_19Bin, palMag1_20Bin,
                          palMag1_21Bin, palMag1_22Bin, palMag1_23Bin,  palMag1_24Bin, palMag1_25Bin, palMag1_26Bin, palMag1_27Bin,
                          palMag2_01Bin, palMag2_02Bin, palMag2_03Bin,  palMag2_04Bin, palMag2_05Bin, palMag2_06Bin, palMag2_07Bin, palMag2_08Bin, palMag2_09Bin, palMag2_10Bin,
                          palMag2_11Bin, palMag2_12Bin, palMag2_13Bin,  palMag2_14Bin, palMag2_15Bin, palMag2_16Bin, palMag2_17Bin, palMag2_18Bin, palMag2_19Bin, palMag2_20Bin,
                          palMag2_21Bin, palMag2_22Bin, palMag2_23Bin,  palMag2_24Bin, palMag2_25Bin, palMag2_26Bin, palMag2_27Bin,
                          palMag3_01Bin, palMag3_02Bin, palMag3_03Bin,  palMag3_04Bin, palMag3_05Bin, palMag3_06Bin, palMag3_07Bin, palMag3_08Bin, palMag3_09Bin, palMag3_10Bin,
                          palMag3_11Bin, palMag3_12Bin, palMag3_13Bin,  palMag3_14Bin, palMag3_15Bin, palMag3_16Bin, palMag3_17Bin, palMag3_18Bin, palMag3_19Bin, palMag3_20Bin,
                          palMag3_21Bin, palMag3_22Bin, palMag3_23Bin,  palMag3_24Bin, palMag3_25Bin, palMag3_26Bin, palMag3_27Bin
                         };

    TPanel *tTIDPanel[]={palNotUseID,
                         palAuto01ID, palAuto02ID, palAuto03ID, palAuto04ID, palAuto05ID, palAuto06ID, palAuto07ID, palAuto08ID, palAuto09ID, palAuto10ID,
                         palAuto11ID, palAuto12ID, palAuto13ID, palAuto14ID, palAuto15ID, palAuto16ID, palAuto17ID, palAuto18ID, palAuto19ID, palAuto20ID,
                         pnlMgz1TopID,pnlMgz3TopID,
                         palMgz1_01ID,palMgz1_02ID,palMgz1_03ID,palMgz1_04ID,palMgz1_05ID,palMgz1_06ID,palMgz1_07ID,palMgz1_08ID,palMgz1_09ID,palMgz1_10ID,
                         palMgz1_11ID,palMgz1_12ID,palMgz1_13ID,palMgz1_14ID,palMgz1_15ID,palMgz1_16ID,palMgz1_17ID,palMgz1_18ID,palMgz1_19ID,palMgz1_20ID,
                         palMgz1_21ID,palMgz1_22ID,palMgz1_23ID,palMgz1_24ID,palMgz1_25ID,palMgz1_26ID,palMgz1_27ID,
                         palMgz2_01ID,palMgz2_02ID,palMgz2_03ID,palMgz2_04ID,palMgz2_05ID,palMgz2_06ID,palMgz2_07ID,palMgz2_08ID,palMgz2_09ID,palMgz2_10ID,
                         palMgz2_11ID,palMgz2_12ID,palMgz2_13ID,palMgz2_14ID,palMgz2_15ID,palMgz2_16ID,palMgz2_17ID,palMgz2_18ID,palMgz2_19ID,palMgz2_20ID,
                         palMgz2_21ID,palMgz2_22ID,palMgz2_23ID,palMgz2_24ID,palMgz2_25ID,palMgz2_26ID,palMgz2_27ID,
                         palMgz3_01ID,palMgz3_02ID,palMgz3_03ID,palMgz3_04ID,palMgz3_05ID,palMgz3_06ID,palMgz3_07ID,palMgz3_08ID,palMgz3_09ID,palMgz3_10ID,
                         palMgz3_11ID,palMgz3_12ID,palMgz3_13ID,palMgz3_14ID,palMgz3_15ID,palMgz3_16ID,palMgz3_17ID,palMgz3_18ID,palMgz3_19ID,palMgz3_20ID,
                         palMgz3_21ID,palMgz3_22ID,palMgz3_23ID,palMgz3_24ID,palMgz3_25ID,palMgz3_26ID,palMgz3_27ID
                        };

    TTMyTray *mtTrayPanel[]={myNotUse,
                             mtAuto01,  mtAuto02,  mtAuto03,  mtAuto04,  mtAuto05,  mtAuto06,  mtAuto07,  mtAuto08,  mtAuto09,  mtAuto10,
                             mtAuto11,  mtAuto12,  mtAuto13,  mtAuto14,  mtAuto15,  mtAuto16,  mtAuto17,  mtAuto18,  mtAuto19,  mtAuto20,
                             mtMag1,    mtMag3,
                             mtMagBin101, mtMagBin102, mtMagBin103, mtMagBin104, mtMagBin105, mtMagBin106, mtMagBin107, mtMagBin108, mtMagBin109, mtMagBin110,
                             mtMagBin111, mtMagBin112, mtMagBin113, mtMagBin114, mtMagBin115, mtMagBin116, mtMagBin117, mtMagBin118, mtMagBin119, mtMagBin120,
                             mtMagBin121, mtMagBin122, mtMagBin123, mtMagBin124, mtMagBin125, mtMagBin126, mtMagBin127,
                             mtMagBin201, mtMagBin202, mtMagBin203, mtMagBin204, mtMagBin205, mtMagBin206, mtMagBin207, mtMagBin208, mtMagBin209, mtMagBin210,
                             mtMagBin211, mtMagBin212, mtMagBin213, mtMagBin214, mtMagBin215, mtMagBin216, mtMagBin217, mtMagBin218, mtMagBin219, mtMagBin220,
                             mtMagBin221, mtMagBin222, mtMagBin223, mtMagBin224, mtMagBin225, mtMagBin226, mtMagBin227,
                             mtMagBin301, mtMagBin302, mtMagBin303, mtMagBin304, mtMagBin305, mtMagBin306, mtMagBin307, mtMagBin308, mtMagBin309, mtMagBin310,
                             mtMagBin311, mtMagBin312, mtMagBin313, mtMagBin314, mtMagBin315, mtMagBin316, mtMagBin317, mtMagBin318, mtMagBin319, mtMagBin320,
                             mtMagBin321, mtMagBin322, mtMagBin323, mtMagBin324, mtMagBin325, mtMagBin326, mtMagBin327
                            };

    TPanel *tEmptyCnt[]={palNoUseEmptyCnt,
                         pnlAuto01EmptyCnt, pnlAuto02EmptyCnt, pnlAuto03EmptyCnt, pnlAuto04EmptyCnt, pnlAuto05EmptyCnt, pnlAuto06EmptyCnt, pnlAuto07EmptyCnt, pnlAuto08EmptyCnt, pnlAuto09EmptyCnt, pnlAuto10EmptyCnt,
                         pnlAuto11EmptyCnt, pnlAuto12EmptyCnt, pnlAuto13EmptyCnt, pnlAuto14EmptyCnt, pnlAuto15EmptyCnt, pnlAuto16EmptyCnt, pnlAuto17EmptyCnt, pnlAuto18EmptyCnt, pnlAuto19EmptyCnt, pnlAuto20EmptyCnt};

    TPanel *tICCnt[]={palNoUseICCnt,
                      pnlAuto01ICCnt, pnlAuto02ICCnt, pnlAuto03ICCnt, pnlAuto04ICCnt, pnlAuto05ICCnt, pnlAuto06ICCnt, pnlAuto07ICCnt, pnlAuto08ICCnt, pnlAuto09ICCnt, pnlAuto10ICCnt,
                      pnlAuto11ICCnt, pnlAuto12ICCnt, pnlAuto13ICCnt, pnlAuto14ICCnt, pnlAuto15ICCnt, pnlAuto16ICCnt, pnlAuto17ICCnt, pnlAuto18ICCnt, pnlAuto19ICCnt, pnlAuto20ICCnt};

    TPanel *tInputCnt[]={palNoUseIn,
                         palAuto01In, palAuto02In, palAuto03In, palAuto04In, palAuto05In, palAuto06In, palAuto07In, palAuto08In, palAuto09In, palAuto10In,
                         palAuto11In, palAuto12In, palAuto13In, palAuto14In, palAuto15In, palAuto16In, palAuto17In, palAuto18In, palAuto19In, palAuto20In};



    TPanel *tTrack1Info[]={palAutoInfo01, palAutoInfo02, palAutoInfo03, palAutoInfo04, palAutoInfo05};  //Sam 20250911 : 新增 AutoInfo 顯示轉向功能
    TPanel *tTrack2Info[]={palAutoInfo06, palAutoInfo07, palAutoInfo08, palAutoInfo09, palAutoInfo10};
    TPanel *tTrack3Info[]={palAutoInfo11, palAutoInfo12, palAutoInfo13, palAutoInfo14, palAutoInfo15};
    TPanel *tTrack4Info[]={palAutoInfo16, palAutoInfo17, palAutoInfo18, palAutoInfo19, palAutoInfo20};

    TGroupBox *tGrpTrack[]={grpTrack1, grpTrack2, grpTrack3, grpTrack4};


    for(int iCol=0; iCol<eTrackColTotal; iCol++)
    {
        palAutoInfo[0][iCol]= tTrack1Info[iCol];
        palAutoInfo[1][iCol]= tTrack2Info[iCol];
        palAutoInfo[2][iCol]= tTrack3Info[iCol];
        palAutoInfo[3][iCol]= tTrack4Info[iCol];
    }

    for(int i=0; i<eTrack4; i++)
    {
        GrpTrack[i]=tGrpTrack[i];
    }

    for(int i=eAuto1; i<=eAuto20; i++)
    {
        palEmptyCnt[i]  =tEmptyCnt[i];
        palLoadCnt[i]   =tICCnt[i];
        palICIn[i]      =tInputCnt[i];
    }

    for(int i=0; i<eTrayCount; i++)
    {
        tTPanelMot[i]       =tTMotPanel[i];
        tTPanelCnt[i]       =tTCntPanel[i];
        tTPanelLotCnt[i]    =tTLotCntPanel[i];                                  //Steven 20240627 : Lot的總入料數量
        tTPanelBin[i]       =tTBinPanel[i];
        tTPanelID[i]        =tTIDPanel[i];
        mtPanel[i]          =mtTrayPanel[i];

        tTPanelMot[i]->Tag          =i;
        tTPanelCnt[i]->Tag          =i;
        tTPanelBin[i]->Tag          =i;
        tTPanelID[i]->Tag           =i;
        tTPanelID[i]->OnMouseDown   =palAuto01IDMouseDown;
        mtPanel[i]->Tag             =i;

        if(MAGAZINE_CNT==3 &&
          (mtPanel[i]->Name=="mtMag1" ||
           mtPanel[i]->Name=="mtMag3"))                                         //Kenhsieh 20250820 : 修正Magazine 無法編輯Tray
        {
            mtPanel[i]->OnMouseDown =mtMag2MouseDown;
        }
        else
        {
            mtPanel[i]->OnMouseDown =mtAuto01MouseDown;
        }
    }

    if(FIX_TRAY_CNT==2)
    {
        lblMgzTop1->Caption="Fix 1 Bin :";
    }

    if(FIX_TRAY_CNT==1)
    {
        lblMgzTop3->Caption="Fix 1 Bin :";
    }
    else if(FIX_TRAY_CNT==2)
    {
        lblMgzTop3->Caption="Fix 2 Bin :";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::FormShow(TObject *Sender)
{
    GetTimeInfo();
    AnsiString S;
    fMain->Caption="HT-172";

    HSys.DecStopAllMotor();

    SetInitialWindowFrame();

    SetInitialData();

    InitialStringGridLockInfo();
    S="D:\\HT-172_Log\\";
    if(!DirectoryExists(S))
        mkdir(S.c_str());

    FormSysTools->SetLogsMemo( (TCustomMemo *)lstLog);
    FormSysTools->AddMyTimeStringShow((TObject *)fMain->statMain->Panels->Items[7], 0);

    ReadLastDataIni();
    Initial();

    fShow=true;
    LastClickButton=sbRecord;
    sbMotionViewClick(sbMotionView);

    UpdateWorkFileComboBox();
    LoadWorkData();
    MyThread=new TRunControl(false);
    InitialAllModule();                                                         //要移到上面才不會記憶體破壞
    InitialOK=true;
    HSys.BinDisCtrl->InitialOK=InitialOK;
    HSys.BinDisCtrlMag->InitialOK=InitialOK;                                    //JerryYang 20240601 : Add MGZ Bin Display

    pgcMain->ActivePage=tsMain;
    spbTrayStatus->Down=true;

//2009.08.08 Abel S
    fMaintenance->OpenWorkFile();                                               // ben 20110629 //
    fSetup->OpenWorkFile();
    fTeach->OpenWorkFile();
    fOffset->OpenWorkFile();
    fSpeed->OpenWorkFile();
    UpdateAllParameter();                                                       // ben 20110629 //
//2009.08.08 Abel E
    HSys.LastSet.iStartMode=0;                                                  // ben add initial mode 20110715 //
    sbInitial->Visible=true;
    sbContinue->Visible=false;
    HSys.LastSet.iStartMode=0;                                                  // ben add change initial 20111018 //
    sbRealIcon->Visible=true;
    sbDummyIcon->Visible=false;
    HSys.LastSet.iRealDummy=REALLY;                                             // ben add change real 20110715 //

    for(int i=0; i<pgcMonitor->PageCount; i++)
        pgcMonitor->Pages[i]->TabVisible=false;

    HSys.mapAlarmCodeList.clear();
    HSys.mapNameToAlarm.clear();
    FormSysTools->bInsertToDB=false;
    FormSysTools->CreateSystemAlarmCode();                                                // 90000 //   一定得在第一個執行
    FormSysTools->CreateSystemSensorAlarmCode();                                          // 30000 //
    FormSysTools->CreateCylinderAlarmCode();                                              // 40000 //
    FormSysTools->CreateMotorAlarmCode();                                                 // 50000 //
    FormSysTools->CreateSuckAlarmCode();                                                  // 60000 //

    InitialAllModule();
    flagTrayFeed=false;

    HSys.FuncA.Update();
    HSys.FuncB.Update();
    HSys.FuncC.Update();
    HSys.FuncN.Update();
    HSys.FuncT.Update();
    HSys.FuncS.Update();
    fComPort->RS232Init();
    SetWorkParameter();
    FSECS->GemInitial("HT-172", MainVersion);
    #ifdef DISABLE_SAFE_DOOR
        fMain->lbCheckSafeDoorDisable->Visible=true;
    #endif

    sgProductInfo->Cells[0][eLotStart ] ="Lot Start :";
    sgProductInfo->Cells[0][eLotEnd   ] ="Lot End :";
    sgProductInfo->Cells[0][eAlarmTime] ="Alarm Time :";
    sgProductInfo->Cells[0][ePauseTime] ="Pause Time :";
    sgProductInfo->Cells[0][eUPH      ] ="UPH :";
    sgProductInfo->Cells[2][eUPH      ] ="Unit / Hr";

    ShowBinCount();                                                             //Sam 201700907 (jou): 程式重啟顯示最後的分 Bin 數量狀態
    TabSheet9->TabVisible=false;

    #ifdef SOFT_SIMULATE
        HSys.LastSet.iRealDummy=DUMMY;
        sbRealIcon->Visible=false;
        sbDummyIcon->Visible=true;
        pnRealDummy->Font->Color=clBlack;
        RecordProcess("Change Dummy Mode");
        bMessageAlarm=false;

        btnRandom->Visible=true;    //Sam 20240613 : 新增 Random Tray Map 功能
        edRandom->Visible=true;
        chkLoadEmpty2Tray->Checked=true;
        chkLoadEmpty1Tray->Checked=true;
        chkLoadTray->Checked=true;
        edLotNo->Text="PH200L340P";
        btnAlarmTest->Visible=true;
    #endif

    palClearBtn->Visible=true;    

    if(CUSTOMER_CODE==CC_HONPREC_QC)
    {

        labTryRun->Visible=true;
    #ifdef SOFT_SIMULATE
        HSys.LastSet.iRealDummy=DUMMY;
    #else
        HSys.LastSet.iRealDummy=REALLY;     //JerryYang 20240603 :add
    #endif
        btnSetFullTray_Auto1->Visible=true;
        btnSetFullTray_Auto2->Visible=true;
        btnSetFullTray_Auto3->Visible=true;
        btnSetFullTray_Auto4->Visible=true;
        btnSetFullTray_Auto5->Visible=true;
        btnSetFullTray_Auto6->Visible=true;
        btnSetFullTray_Auto7->Visible=true;
        btnSetFullTray_Auto8->Visible=true;
        btnSetFullTray_Auto9->Visible=true;
        btnSetFullTray_Auto10->Visible=true;
        btnSetFullTray_Auto11->Visible=true;
        btnSetFullTray_Auto12->Visible=true;
        btnSetFullTray_Auto13->Visible=true;
        btnSetFullTray_Auto14->Visible=true;
        btnSetFullTray_Auto15->Visible=true;
        btnSetFullTray_Auto16->Visible=true;
        btnSetFullTray_Auto17->Visible=true;
        btnSetFullTray_Auto18->Visible=true;
        btnSetFullTray_Auto19->Visible=true;
        btnSetFullTray_Auto20->Visible=true;
        btnClearMgz1->Visible=true;
        btnClearMgz2->Visible=true;
        btnClearMgz3->Visible=true;
    }
    else
    {
        palClearBtn->Visible=false;
        labTryRun->Visible=false;
        btnSetFullTray_Auto1->Visible=false;
        btnSetFullTray_Auto2->Visible=false;
        btnSetFullTray_Auto3->Visible=false;
        btnSetFullTray_Auto4->Visible=false;
        btnSetFullTray_Auto5->Visible=false;
        btnSetFullTray_Auto6->Visible=false;
        btnSetFullTray_Auto7->Visible=false;
        btnSetFullTray_Auto8->Visible=false;
        btnSetFullTray_Auto9->Visible=false;
        btnSetFullTray_Auto10->Visible=false;
        btnSetFullTray_Auto11->Visible=false;
        btnSetFullTray_Auto12->Visible=false;
        btnSetFullTray_Auto13->Visible=false;
        btnSetFullTray_Auto14->Visible=false;
        btnSetFullTray_Auto15->Visible=false;
        btnSetFullTray_Auto16->Visible=false;
        btnSetFullTray_Auto17->Visible=false;
        btnSetFullTray_Auto18->Visible=false;
        btnSetFullTray_Auto19->Visible=false;
        btnSetFullTray_Auto20->Visible=false;
        btnClearMgz1->Visible=false;
        btnClearMgz2->Visible=false;
        btnClearMgz3->Visible=false;
    }

    ASESendMessage->initial();
    bSystemClose=false;

    SearchForm();

    fiosetview->InitialAll();
    apbLogs->Click();
    spbTrayStatus->Click();
    tsMgz1->TabVisible=(MAGAZINE_CNT>=2);
    tsMgz2->TabVisible=(MAGAZINE_CNT>=1);
    tsMgz3->TabVisible=(MAGAZINE_CNT>=3);
    pgcMgzCnt->ActivePage=tsMgz2;
    SetSimuScreenPara();                                                        //JerryYang 20240610 : 執行這個花了2秒,改到FormShow做一次就好
    ShowLotBinCount(true);

    if(HSys.MyGem!=NULL)
        HSys.MyGem->UpdateDataPath("D:\\HT172\\Data\\");          //Steven 20140902 : SECS GEM

    if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eJHTFormat)
    {
        labWaferLot->Visible=true;
        edWaferLot->Visible=true;
        labCusDevice->Visible=true;
        edCusDevice->Visible=true;
        labInsettion->Visible=true;
        edInsertion->Visible=true;
        labFlowID->Visible=true;
        edFlowID->Visible=true;
        labRunCard->Visible=false;
        edtRunCard->Visible=false;
    }
    else if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eWinstekFormat)
    {
        labWaferLot->Visible=false;
        edWaferLot->Visible=false;
        labCusDevice->Visible=false;
        edCusDevice->Visible=false;
        labInsettion->Visible=false;
        edInsertion->Visible=false;
        labFlowID->Visible=false;
        edFlowID->Visible=false;
        labRunCard->Visible=true;
        edtRunCard->Visible=true;
    }
    else if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->ePTIFormat)
    {
        labWaferLot->Visible=false;
        edWaferLot->Visible=false;
        labCusDevice->Visible=false;
        edCusDevice->Visible=false;
        labInsettion->Visible=false;
        edInsertion->Visible=false;
        labFlowID->Visible=false;
        edFlowID->Visible=false;
        labRunCard->Visible=false;
        edtRunCard->Visible=false;
    }

    if(DirectoryExists(sMNetLogPath)==false)
    {
        ForceDirectories(sMNetLogPath);
    }
    palClearBtn->Visible=true;                                                  //Steven 20240728 : 隱藏不需要的畫面
    FormSysTools->CreateNewJamErrorTable();

    if(CosFunction.bSaveJamCodeLog)
    {
        if(FormSysTools->tJamAlarm.LoadFromFile()==false)
        {
            AnsiString sErr=AnsiString().sprintf("Jam alarm table read failed. path=%s",FormSysTools->tJamAlarm.sJamTableFile());
            ShowMyMessage(sErr);
        }
    }
    labAutomation->Visible=(CUSTOMER_CODE==CC_PTI);
    cbEnableSimulation->Checked=false;
    btnTrayMap->Visible=(HSys.AccessLevel==3);
}
//------------------------------------------------------------------------------
void __fastcall TfMain::FormClose(TObject *Sender, TCloseAction &Action)        // main form close //
{
    AnsiString sFile="";
    ProgramFlag=true;
    HSys.DecStopAllMotor();                                                     // all motor stop //
    AllBreakLock();                                                             // lock all motor ben 20110817 //

    HSys.Sw.SwInterLock.OnOff(false);

    SoftStop=true;
    MyThread->Terminate();
    MyThread->WaitFor();

    for(int i=0; i<HSys.iTotalCylinder; i++)
    {
        if(HSys.CynPtr[i].Enable)
        {
            HSys.LastSet.bCynStatus[i]=HSys.CynPtr[i].Switch.OutValue;
        }
    }

    for(int i=0; i<HSys.iTotalSwitch; i++)
    {
        if(HSys.SwPtr[i].Enable)
        {
            HSys.LastSet.bSwStatus[i]=HSys.SwPtr[i].OutValue;
        }
    }

    SaveWorkData();                                                             // save lastest.ini //
    WriteLastDataIni();
    HSys.Sw.SwMotorRelay.Off();                                                 // close motor power ben 20110817 //

    sFile.sprintf("%s\\MNetLog_%04d-%02d-%02d_%02d%02d%02d.txt", sMNetLogPath, SystemYear, SystemMonth, SystemDate,SystemHour,SystemMin,SystemSec);

    if(fMain->memoIOStatus->Lines->Count>0)
    {
        fMain->memoIOStatus->Lines->SaveToFile(sFile);
        fMain->memoIOStatus->Clear();
    }

    if(MyPLCIOThread!=NULL)
    {
        MyPLCIOThread->Terminate();
        delete MyPLCIOThread;
        MyPLCIOThread=NULL;
    }
    delete MyThread;
    for(int i=0; i<4; i++)                                                      //Steven 20101109
        delete PassList[i];

    bSystemClose=true;
    delete sHeadList;    
    //Click Event,Auto Save log
    //==>
    DeletePtr();
    //<==
    //Click Event,Auto Save log
}
//------------------------------------------------------------------------------
void __fastcall TfMain::FormDestroy(TObject *Sender)
{
    delete Alarm;
    delete sr;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::AppException(TObject *Sender,Exception *E)              //Sam 20170803 (jou):add catch exception
{
    ShowRecordMessage(E->HelpContext + ":" + E->Message);
    return;
}
//------------------------------------------------------------------------------// NEED EDIT BY USER //
AnsiString GetFileInfoItem(AnsiString FName, AnsiString InfoItem)
{
    AnsiString Result = "";
    DWORD dwVerInfoSize=0;
    AnsiString s = FName;
    BYTE *bVerInfoBuf = NULL;

    dwVerInfoSize=GetFileVersionInfoSize(s.c_str(), &dwVerInfoSize);
    bVerInfoBuf=new BYTE[dwVerInfoSize];
    s = FName;

    if(GetFileVersionInfo(s.c_str(), 0, dwVerInfoSize, bVerInfoBuf))
    {
        LPDWORD lang;
        UINT len;
        LPSTR value;
        char buffer[MAX_PATH];
        char query[500];

        //get char-set
        VerQueryValue(bVerInfoBuf, "\\VarFileInfo\\Translation", (void**)&lang, &len);
        VerLanguageName(LOWORD(*lang), buffer, sizeof(buffer));

        //get info item
        sprintf(query, "\\StringFileInfo\\%04x%04x\\%s", LOWORD(*lang), HIWORD(*lang), InfoItem.c_str());

        len=0;
        if(VerQueryValue(bVerInfoBuf, query, (void**)&value, &len) || (len!=0))
        {
            CharToOem(value, value);
            Result = AnsiString(value);
        }
    }

    if(bVerInfoBuf)
    {
        delete [] bVerInfoBuf;
        bVerInfoBuf=0;
    }
    return Result;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::SetInitialWindowFrame()                                 // Main Form Set //
{
    Top=0;
    Left=0;
    Align=alClient;

    for(int i=0; i<pgcMonitor->PageCount; i++)
        pgcMonitor->Pages[i]->TabVisible=false;

    for(int i=0; i<pgcLog->PageCount; i++)
        pgcLog->Pages[i]->TabVisible=false;

    for(int i=0; i<pgcMain->PageCount; i++)
        pgcMain->Pages[i]->TabVisible=false;

    pnlMonitorMenu  ->BringToFront();
    pnlLogMenu      ->BringToFront();

    frm2DID->Initialization();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::InitialStringGridLockInfo()                             // set motor lock stringgrid //
{
    StringGridLockInfo->RowCount=HSys.iTotalMotor+1;
    StringGridLockInfo->ColWidths[0]=120;
    StringGridLockInfo->ColWidths[1]=50;
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        if(HSys.MotPtr[i]!=NULL)
            StringGridLockInfo->Cells[0][i+1]=HSys.MotPtr[i]->NumberAlias;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::ShowStringGridLockInfo()                                // show motor lock information //
{
    int ct;
    if(PageControl3->ActivePage!=TabSheet2)
        return;
    if(pgcMonitor->ActivePage!=TabOther)
        return;
    if(pgcMain->ActivePage!=tsMonitorView)
        return;

    StringGridLockInfo->RowCount=HSys.iTotalMotor+1;
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        if(HSys.MotPtr[i]==NULL)
            continue;
        ct=HSys.MotPtr[i]->GetLockCount();
        StringGridLockInfo->Cells[1][i+1]=ct;
        if((ct+2)>StringGridLockInfo->ColCount)
        {
            StringGridLockInfo->ColCount=ct+2;
            for(int j=2; j<StringGridLockInfo->ColCount; j++)
                StringGridLockInfo->ColWidths[j]=300;
        }
        for(int j=0; j<ct; j++)
            StringGridLockInfo->Cells[2+j][i+1]=HSys.MotPtr[i]->GetLockString(j);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::SetInitialData()                                        // set AccessLevel and data //
{
    Alarm = new HAlarm(this);
    Alarm->Clear();
    fShow=false;
    SetUserToOP();
    Now_Status_IsOneCycle=false;
    #ifdef SOFT_SIMULATE
        HSys.AccessLevel=3;
        cbbUserSelect->ItemIndex=3;
    #endif

    if(CUSTOMER_CODE==CC_HONPREC_QC)
    {
        HSys.AccessLevel=3;
        cbbUserSelect->ItemIndex=3;
    }

    asNowUserID=cbbUserSelect->Items->Strings[HSys.AccessLevel];                //Daver add

    for(int i=0; i<4; i++)                                                      //Steven 20101109
        PassList[i]= new TStringList;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::ScanKey()                                               // ben 20111026 //
{
    static int Key;
    Key=0;
    if(fNote->fShow || fiosetview->fShow || fHome->fShow)
        return;
    if(MyMessageBox->fShow)
        return;

    if((Key=ScanPannelKey())==-1)
        return;

    if(IsSafeLock())
    {
        return;
    }
    else if(Key==HSys.Sen.SnRKStart.Tag)
    {
        Start();    //JerryYang 20250106 : Run check
    }
    else if(Key==HSys.Sen.SnRKHome.Tag)
    {
        RecordProcess("Home pressed");
        HSys.Sys.SystemStart=true;
        EventReport(SECS_EVENT.PressHome);
        sbHomeClick(Owner);
    }
    else if(Key==HSys.Sen.SnRKPause.Tag)
    {
        RecordProcess("PAUSE pressed");
        SoftStop=true;
        HSys.Sys.SystemStart=false;
        EventReport(SECS_EVENT.PressPause);

    }
    else if(Key==HSys.Sen.SnRKOneCycle.Tag)
    {
        RecordProcess("ONE CYCLE pressed");                                   //KenHsieh 20250806 : 先Pass Oncycle 完成後再解
        EventReport(SECS_EVENT.PressOneCycle);
        sbOneCycleClick(Owner);
    }
    else if(Key==HSys.Sen.SnRKReset.Tag)
    {
        RecordProcess("RESET pressed");
        EventReport(SECS_EVENT.PressReset);
    }
    else if(Key==HSys.Sen.SnRKCleanOut.Tag)
    {
        #ifndef SOFT_SIMULATE                                                   //Clean Out 警視窗
        int ret=ShowMyMessageBox_YES_NO("確定要結批??");
        if(ret==TMyMessageBox::msgrtnYES)
        #endif
        {
            RecordProcess("CLEAN OUT pressed");
            EventReport(SECS_EVENT.PressCleanOut);
            sbCleanOutClick(Owner);
        }
        #ifndef SOFT_SIMULATE
        else
        {
            return;
        }
        #endif
    }
    else if(Key==HSys.Sen.SnRKTrayFeed.Tag)
    {
        RecordProcess("TRAY FEED pressed");
        EventReport(SECS_EVENT.PressTrayFeed);
        sbTrayFeedClick(Owner);
    }
    else if( Key==HSys.Sen.SnRKTrayEnd.Tag)
    {
        #ifndef SOFT_SIMULATE                                                   //Tray End 警視窗
        int ret=ShowMyMessageBox_YES_NO("確定將Tray盤退出??");
        if(ret==1)
        #endif
        {
            RecordProcess("TRAY End pressed");
            EventReport(SECS_EVENT.PressTrayEnd);
            sbTrayEndClick(Owner);
        }
        #ifndef SOFT_SIMULATE
        else
        {
            return;
        }
        #endif
    }
    else if(Key==HSys.Sen.SnMGZ1Step.Tag)                                       //KenHsieh 20240528 : 手動Magazine Tray Feed
    {
        if(HSys.Sys.RunMode!=Run_TrayFeed)
            return;

        if(MagazineModule1->TrayFeedFinish()==false)
        {
            RecordProcess("Magazine 1 Step pressed");
            if(MyMessageBox->fShow)
                MyMessageBox->Close();
            if(HSys.Sys.SystemStart==false)
            {
                SoftStart=true;
                HSys.Sys.SystemStart=true;
            }
        }
    }
    else if(Key==HSys.Sen.SnMGZ2Step.Tag)                                       //KenHsieh 20240528 : 手動Magazine Tray Feed
    {
        if(HSys.Sys.RunMode!=Run_TrayFeed)
            return;

        if(MagazineModule2->TrayFeedFinish()==false)
        {
            RecordProcess("Magazine 2 Step pressed");
            if(MyMessageBox->fShow)
                MyMessageBox->Close();
            if(HSys.Sys.SystemStart==false)
            {
                SoftStart=true;
                HSys.Sys.SystemStart=true;
            }
        }
    }
    else if(Key==HSys.Sen.SnMGZ3Step.Tag)                                       //KenHsieh 20240528 : 手動Magazine Tray Feed
    {
        if(HSys.Sys.RunMode!=Run_TrayFeed)
            return;

        if(MagazineModule3->TrayFeedFinish()==false)
        {
            RecordProcess("Magazine 3 Step pressed");
            if(MyMessageBox->fShow)
                MyMessageBox->Close();
            if(HSys.Sys.SystemStart==false)
            {
                SoftStart=true;
                HSys.Sys.SystemStart=true;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::sbOffsetClick(TObject *Sender)
{
    RecordProcess("Enter Offset");
    EventReport(SECS_EVENT.EnterOffset);
    fOffset->ShowModal();
    sbOffset->Down=false;
}
//---------------------------------------------------------------------------
typedef struct
{
    unsigned int Result0;
    unsigned int Result1;
    unsigned int Result2;
    unsigned int Result3;
    unsigned int Result4;
    unsigned int Result5;
    unsigned int Result6;
    unsigned int Result7;
    unsigned int Result8;
    unsigned int Result9;
    unsigned int Result10;
    unsigned int Result11;
    unsigned int Result12;
    unsigned int Result13;
    unsigned int Result14;
    unsigned int Result15;

    bool bError;

    char cReturn[100];
}TReceiveCopyMsg;
TReceiveCopyMsg *tReceiveCopyMsg;
//------------------------------------------------------------------------------
typedef struct
{
    unsigned int iTimeOutSecond;  // 0 : normal model
                                  // 1 : double contact
    unsigned int Site[16];
    //jou 981226 end
    bool bSimulate;
    bool b8080;
    bool bCloseGpib;
    bool bTimeOutProcess;
    int  GpibAddress;
    bool MachineISRun;
    bool IsTest;
    bool bGpibMode;

    int  iLotStatus;

    HWND HandlerHwnd;
    HWND GpibHwnd;
}TSendCopyMsg;
TSendCopyMsg tSendCopyMsg;
//---------------------------------------------------------------------------
void ClearAllYield()
{
}
//---------------------------------------------------------------------------
int iCheckChoromaTesterExist=0;
void __fastcall TfMain::sbMessageClick(TObject *Sender)
{
    RecordProcess("Enter Message");
    EventReport(SECS_EVENT.EnterMessage);
    fData->ShowModal();
    sbMessage->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::btnMainShowClick(TObject *Sender)
{
    pgcMain->ActivePage=tsMain;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::sbMonitorClick(TObject *Sender)
{
    pgcMain->ActivePage=tsMonitorView;
    sbMonitor->Down=false;
    LastClickButton->OnClick(this);
    LastClickButton->Down=true;
    MainFormChange();                                                           //20131120 wei   led 顯示
}
//---------------------------------------------------------------------------
void __fastcall TfMain::MainFormChange()                                        //20131120 wei   led 顯示
{

}
//---------------------------------------------------------------------------
void __fastcall TfMain::sbMotorViewClick(TObject *Sender)
{
    pgcMonitor->ActivePage=TabSheet7;
    LastClickButton=sbMotorView;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::sbMotionViewClick(TObject *Sender)
{
    pgcMonitor->ActivePage=TabSheet10;
    LastClickButton=sbMotionView;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::sbOtherClick(TObject *Sender)
{
    pgcMonitor->ActivePage=TabOther;
    LastClickButton=sbOther;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::sbProductClick(TObject *Sender)
{
    RecordProcess("Enter Product");
    EventReport(SECS_EVENT.EnterSetup);
    AnsiString S=cb_WorkFile->Text;
    fSetup->ShowModal();

    HSys.FuncA.Update();
    HSys.FuncB.Update();
    HSys.FuncC.Update();
    HSys.FuncN.Update();
    HSys.FuncT.Update();
    HSys.FuncS.Update();
    sbProduct->Down=false;
    UpdateWorkFileComboBox();
    dmTrayMotor->StartSetSpeed();                                               //Sam 20240525 : 速度資料更新後才能設定 Tray 速度。
    for(int i=0; i<cb_WorkFile->Items->Count; i++)
    {
        if(S==cb_WorkFile->Items->Strings[i])
        {
            cb_WorkFile->ItemIndex=i;
            break;
        }
    }
    fSetup->OpenWorkFile();
    ShowLotBinCount();                                                          //Sam 20240702 : 更新 BinCount 數值
}
//---------------------------------------------------------------------------
void __fastcall TfMain::UpdateWorkFileComboBox()
{
    AnsiString S;
    cb_WorkFile->Clear();
    for(int i=0; i<fSetup->flWorkFileList->Items->Count; i++)
    {
        S=fSetup->flWorkFileList->Items->Strings[i];
        S=S.SubString(1,S.Length()-4);
        cb_WorkFile->Items->Add(S);
    }
}
//==============================================================================
void __fastcall TfMain::cb_WorkFileChange(TObject *Sender)                      // change work file //
{
    if(cb_WorkFile->Text=="")
    {
        ShowMyMessage("Recipe name cannot be empty.");
        return;
    }
    AnsiString str="";
    fSetup->OpenWorkFile();                                                     // load setup data //
    fTeach->OpenWorkFile();                                                     // load teach data //
    fOffset->OpenWorkFile();                                                    // load offset data //
    fSpeed->OpenWorkFile();
    fMaintenance->OpenWorkFile();
    UpdateAllParameter();                                                       // ben add 20110803 update all //
    SaveWorkData();                                                             // save lastest.ini //
    sWorkFile = cb_WorkFile->Text;
    EventReport(SECS_EVENT.RecipeChange);                                       //Steven 20130723
    str.sprintf("Change Set Up File to %s", sWorkFile);

    if(CUSTOMER_CODE==CC_PTI)                                                   //Sam 20250905 : 切換工作檔時要 Initial
    {
        HSys.LastSet.iStartMode=0;                                              // change initial //
        LoadStartModePicture();
        fAllMotorHome=false;                                                    //啟泰要求要強制 HOME
    }

    RecordProcess(str);
}
//==============================================================================
void __fastcall TfMain::SaveWorkData()                                          // save lastest.ini //
{
    AnsiString S=HSys.CurrentDir+AnsiString("\\system\\lastset.ini");
    FormSysTools->OpenFormData(S);
    FormSysTools->SaveFormData(fMain, "lastset");
    FormSysTools->CloseFormData();
}
//==============================================================================
void __fastcall TfMain::LoadWorkData()                                          // load lastest.ini //
{
    AnsiString S=HSys.CurrentDir+AnsiString("\\system\\lastset.ini");
    FormSysTools->OpenFormData(S);
    FormSysTools->LoadFormData(fMain,"lastset");
    FormSysTools->CloseFormData();
    fMaintenance->OpenWorkFile();
    fSetup->OpenWorkFile();
    fSpeed->OpenWorkFile();                                                     // ben 20110701 //
    fComPort->OpenWorkFile();
    sWorkFile = cb_WorkFile->Text;

    EventReport(SECS_EVENT.RecipeChange);                                       //Steven 20130723
}
//==============================================================================
void __fastcall TfMain::sbLaguageClick(TObject *Sender)                         // change laguage //
{
    if(HSys.Sys.SystemStart)
        return;
    if(HSys.LastSet.iLanguageCountry==0)
        HSys.LastSet.iLanguageCountry=1;
    else
        HSys.LastSet.iLanguageCountry=0;
    Caption="HT-172";
    sbLaguage->Down=false;
}
//==============================================================================
void __fastcall TfMain::sbHomeClick(TObject *Sender)                            // do run home //
{
#ifndef SOFT_SIMULATE
    int ret=ShowMyMessageBox_YES_NO("確定要歸零??");
    if(ret==TMyMessageBox::msgrtnYES)
#endif
    {
        RecordProcess("HOME pressed");
        EventReport(SECS_EVENT.PressHome);
        ChangeRunMode(Run_Home);
        HSys.Sys.SystemStart=true;                                              //20140411 wei

        fAllMotorHome=false;
        SoftStart=true;
        bHomeByStart=false;
        tRunData.PauseTime=StrToTime("00:00:00");
        tRunData.AlarmTime=StrToTime("00:00:00");
    }
#ifndef SOFT_SIMULATE
    else
        return;
#endif
}
//==============================================================================
void __fastcall TfMain::sbStartClick(TObject *Sender)                           // do start //
{
    Start();        //JerryYang 20250106 : Run check
}
//==============================================================================
void __fastcall TfMain::sbPauseClick(TObject *Sender)                           // do pause //
{

    if(USE_SECS_GEM && HSys.FuncB.bN04_RunCheck && bPhysicalStart==true)        //JerryYang 20250106 : Run check
    {
        bPhysicalStart=false;
    }
    if(HSys.Sys.SystemStart==true)
    {
        RecordProcess("PAUSE pressed");
        EventReport(SECS_EVENT.PressPause);
        HSys.Sys.SystemStart=false;                                             //20140411 wei
    }
    SoftStop=true;
}
//==============================================================================
void __fastcall TfMain::sbTrayFeedClick(TObject *Sender)                        // do run tray feed //
{
    if(SoftStop==false)
        return;

    if(flagTrayFeed==false)
    {
        EventReport(SECS_EVENT.PressTrayFeed);
        RecordProcess("TRAY FEED pressed");
    }
    flagTrayFeed=true;
}
//==============================================================================
void __fastcall TfMain::sbTrayEndClick(TObject *Sender)                         // do run tray feed //
{
    if(flagTrayEnd==false && flagOneCycleTrayEnd==true)
    {
        EventReport(SECS_EVENT.PressTrayEnd);
        RecordProcess("TRAY End pressed");
        flagTrayEnd=true;
    }
}
//==============================================================================
void __fastcall TfMain::sbOneCycleClick(TObject *Sender)                        // do run one cycle //
{
    if(HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_CleanOut)
    {
        RecordProcess("ONE CYCLE pressed");
        EventReport(SECS_EVENT.PressOneCycle);
        ChangeRunMode(Run_OneCycle);
    }
}
//==============================================================================
void __fastcall TfMain::sbCleanOutClick(TObject *Sender)                        // do run clean out //
{
    if(HSys.Sys.RunMode==Run_Normal)
    {
        RecordProcess("CLEAN OUT pressed");
        EventReport(SECS_EVENT.PressCleanOut);
        ChangeRunMode(Run_CleanOut);
        bCleanOut=true;                                                         // ben add 20110803 //
    }
}
//==============================================================================
void __fastcall TfMain::sbRecordClick(TObject *Sender)                          // open record tab //
{
    pgcMonitor->ActivePage=TabRecord;
    LastClickButton=sbRecord;
}
//==============================================================================
void __fastcall TfMain::sbExitClick(TObject *Sender)                            // exit //
{
    if(CUSTOMER_CODE==CC_WINSTEK &&
       RunInfo.LotStart==true)
    {
        ShowMyMessage("Please Lot End before Exist.");
        return;
    }
    int ret=ShowMyMessageBox_YES_NO("Sure To Exit?");
    if(ret==TMyMessageBox::msgrtnNO)
    {
        return;
    }
    else
    {
        RecordProcess("Program Closed");
        EventReport(SECS_EVENT.PressExit);
        fMaintenance->SaveData();
        Close();
    }
}
//==============================================================================
void __fastcall TfMain::sbMaintanceClick(TObject *Sender)                       // open maintenance form //
{
    RecordProcess("Enter Maintance");
    EventReport(SECS_EVENT.EnterMaintenPage);
    fMaintenance->ShowModal();
    HSys.FuncA.Update();
    HSys.FuncB.Update();
    HSys.FuncC.Update();
    HSys.FuncN.Update();
    HSys.FuncT.Update();
    HSys.FuncS.Update();
    sbMaintance->Down=false;
    fMaintenance->OpenWorkFile();                                               // load maintenance form data //
    fTeach->OpenWorkFile();                                                     // load teach form data //
    fSpeed->OpenWorkFile();                                                     // load speed form data //
}
//==============================================================================
void __fastcall TfMain::sbSpeedClick(TObject *Sender)                           // open speed form //
{
    RecordProcess("Enter Speed");
    EventReport(SECS_EVENT.EnterSpeed);
    fSpeed->ShowModal();
    sbSpeed->Down=false;
}
//==============================================================================
void __fastcall TfMain::sbToolClick(TObject *Sender)                            // open tool form //
{
    RecordProcess("Enter Tools");
    EventReport(SECS_EVENT.EnterToolPage);
    sbTool->Down=false;
    FormSysTools->ShowModal();
}
//==============================================================================
void __fastcall TfMain::ShowMotorInfo()                                         // show motor information //
{
    static bool flag=false;
    TRect R;
    if(pgcMonitor->ActivePageIndex!=1)                                          // ben add if not open motor view not to update motor info //
        return;
    if(flag==false)
    {
        flag=true;
        sgMotorStatus->ColCount=iMotLedTotalCnt+4;
        sgMotorStatus->ColWidths[0]=120;
        sgMotorStatus->Cells[ 0][0]="Motor Name";
        sgMotorStatus->Cells[ 1][0]="Target";
        sgMotorStatus->Cells[ 2][0]="Position";
        sgMotorStatus->Cells[ 3][0]="Encoder";
        sgMotorStatus->Cells[ 4][0]="CW";
        sgMotorStatus->Cells[ 5][0]="HOME";
        sgMotorStatus->Cells[ 6][0]="CCW";
        sgMotorStatus->Cells[ 7][0]="Emg";
        sgMotorStatus->Cells[ 8][0]="Alarm";
        sgMotorStatus->Cells[ 9][0]="SoftCW";
        sgMotorStatus->Cells[10][0]="SoftCCW";
        sgMotorStatus->Cells[11][0]="ServoAlarm";
        sgMotorStatus->Cells[12][0]="InPos";
        sgMotorStatus->Cells[13][0]="Z Phase";
        sgMotorStatus->Cells[14][0]="ServoOn";
        sgMotorStatus->RowCount=HSys.iTotalMotor+2;
        for(int i=0; i<HSys.iTotalMotor; i++)
        {
            if( HSys.MotPtr[i]==NULL)
                continue;
            sgMotorStatus->Cells[0][i+2]=HSys.MotPtr[i]->NumberAlias;
        }
    }

    if(pgcMain->ActivePage!=tsMonitorView)
        return;
    if(pgcMonitor->ActivePage!=TabSheet7)
        return;

    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        //if(HSys.MotPtr[i]==NULL)
        if(HSys.MotPtr[i]==NULL || HSys.MotPtr[i]->GetEnable()==false)
            continue;

        if(HSys.Sys.SystemStart==false)
            HSys.MotPtr[i]->ReadPos();
        sgMotorStatus->Cells[1][i+2]=HSys.MotPtr[i]->TargetPosition;
        sgMotorStatus->Cells[2][i+2]=HSys.MotPtr[i]->Position;
        sgMotorStatus->Cells[3][i+2]=HSys.MotPtr[i]->EncoderPosition;
        HSys.MotPtr[i]->ScanMotorStatus();
        for(int j=0; j<iMotLedTotalCnt; j++)
        {
            R=sgMotorStatus->CellRect(4+j, i+2);
            R.Top++;
            R.Bottom-=1;
            R.Left++;
            R.Right-=2;

            if(HSys.MotPtr[i]->Led[j])
            {
                if(j==1)
                {
                    sgMotorStatus->Canvas->Brush->Color=clGreen;
                }
                else
                {
                    if(j==8)
                    {
                        if(HSys.Sys.SystemStart)
                            sgMotorStatus->Canvas->Brush->Color=clGreen;
                        else
                            sgMotorStatus->Canvas->Brush->Color=clRed;
                    }
                    else
                    {
                        sgMotorStatus->Canvas->Brush->Color=clRed;
                    }
                }
                sgMotorStatus->Canvas->FillRect(R);
            }
            else
            {
                sgMotorStatus->Canvas->Brush->Color=sgMotorStatus->Color;
                sgMotorStatus->Canvas->FillRect(R);
            }
        }
    }
}
//==============================================================================
const int iDoUpdateCount=30;
void TfMain::ShowTaskInfo()                                                     // show Task information //
{
    static bool flag=false;
    static int iNowCount=30;
    if(pgcMonitor->ActivePage!=TabRecord)                                       // ben add if not open motor view not to update motor info //
        return;
    if(flag==true)
    {
        return;
    }
    else
    {
        flag=true;
        iNowCount++;
        if(iNowCount>iDoUpdateCount)
        {
            //start
            lbTaskRecord->Items->BeginUpdate();  // 開始批次更新（避免閃爍）
            lbTaskRecord->Items->Clear();
            if (sr != NULL)  // 防呆：確認 sr 已 new
            {
                TStringList* tasks = sr->GetAllCurrentTasks(true);  // 取狀態列表（含時間）
                __try
                {
                    lbTaskRecord->Items->Assign(tasks);  // 直接 Assign 到 ListBox
                }
                __finally
                {
                    delete tasks;  // 記得釋放記憶體！
                }
            }
            lbTaskRecord->Items->EndUpdate();  // 結束批次更新
            iNowCount=0;
        }
    }
    flag=false;
}
//==============================================================================
void __fastcall TfMain::SetSimulateScreenStatus()                               // set Simulate Screen Status //
{
    static bool flag=false, oldstatus;
    bool status=(pgcMain->ActivePage==tsMonitorView && pgcMonitor->ActivePage==TabSheet10);
    if(flag==false)
    {
        oldstatus=status;
        flag=true;
        for(int i=0; i<HSys.iTotalMotor; i++)
        {
            if( HSys.MotPtr[i]==NULL)
                continue;
            HSys.MotPtr[i]->SetShowSimulateCompomentFlag(oldstatus);
        }
    }

    if(oldstatus!=status)
    {
        oldstatus= status;
        for(int i=0; i<HSys.iTotalMotor; i++)
        {
            if(HSys.MotPtr[i]==NULL)
                continue;
            HSys.MotPtr[i]->SetShowSimulateCompomentFlag(oldstatus);
        }
    }
}
//==============================================================================
void __fastcall TfMain::Timer1Timer(TObject *Sender)
{
    if(InitialOK==false)
        return;
    static bool flag=false;
    if(flag==true)
        return;
    flag=true;
    ScanKey();                                                                  // ben edit 20111026 //

    ShowStringGridLockInfo();                                                   // show motor lock information //
    ShowMotorInfo();                                                            // show motor information //
    ShowTaskInfo();
    SetSimulateScreenStatus();                                                  // set Simulate Screen Status //
    cbbUserSelect->Text=asNowUserID;                                                //Daver add
    //************************************************************************//
    char Estr[3][30]={{"No Tray"}, {"HasTray"}, {"Real"}};
    char Cstr[3][30]={{"虛擬"}, {"空盤"}, {"實際"}};
    if(HSys.LastSet.iRealDummy<DUMMY || HSys.LastSet.iRealDummy>REALLY)
        HSys.LastSet.iRealDummy=DUMMY;

    if(HSys.LastSet.iRealDummy==DUMMY)
    {
        sbRealIcon->Visible=false;
        sbDummyIcon->Visible=true;
        pnRealDummy->Font->Color=clBlack;
    }
    else if(HSys.LastSet.iRealDummy==REALLY)
    {
        sbRealIcon->Visible =true;
        sbDummyIcon->Visible=false;
        pnRealDummy->Font->Color=clRed;
    }
    else if(HSys.LastSet.iRealDummy==HAS_TRAY)
    {
        sbRealIcon->Visible=false;
        sbDummyIcon->Visible=true;
        pnRealDummy->Font->Color=clBlack;
    }
    if(HSys.LastSet.iLanguageCountry==0)
        pnRealDummy->Caption=Estr[HSys.LastSet.iRealDummy];
    else
        pnRealDummy->Caption=Cstr[HSys.LastSet.iRealDummy];
    //************************************************************************//
    char Estr1[2][30]={{"Initial"}, {"Continue"}};                              // edit run mode button show //
    char Cstr1[2][30]={{"初始化"}, {"繼續"}};                                   // ben 20111018 //
    if(HSys.LastSet.iStartMode<0 || HSys.LastSet.iStartMode>1)
        HSys.LastSet.iStartMode=0;
    if(HSys.LastSet.iStartMode==0)
    {
        sbInitial->Visible=true;
        sbContinue->Visible=false;
    }
    else
    {
        sbInitial->Visible=false;
        sbContinue->Visible=true;
    }
    if(HSys.LastSet.iLanguageCountry==0)
        pnStartMode->Caption=Estr1[HSys.LastSet.iStartMode];
    else
        pnStartMode->Caption=Cstr1[HSys.LastSet.iStartMode];
    //************************************************************************//
    sbLaguage       ->Enabled=(HSys.Sys.SystemStart==false);
    sbProduct       ->Enabled=(HSys.Sys.SystemStart==false);
    sbMaintance     ->Enabled=(HSys.Sys.SystemStart==false);
    sbTool          ->Enabled=(HSys.Sys.SystemStart==false);
    sbExit          ->Enabled=(HSys.Sys.SystemStart==false);
    if(fiosetview->fShow==false)
        HSys.Sw.SwInterLock.OnOff(HSys.Sys.SystemStart);

    //Sam 20240708 : 模擬 OneCycle 驗證
    #ifdef ONECYCLE_DEBUG
    static int iAutoOneCycleCnt=0;
    static int iOneCycleTestCnt=0;
    if(HSys.Sys.SystemStart)
    {
        iAutoOneCycleCnt++;
        if(iAutoOneCycleCnt>100 && HSys.Sys.RunMode==Run_Normal)
        {
            iAutoOneCycleCnt=0;
            iOneCycleTestCnt++;
            fMain->sbOneCycleClick(NULL);
        }
    }
    #endif
    flag=false;
}
//==============================================================================
void __fastcall TfMain::StoreHangupData()                                       // hangup //
{
    HDC hdc1;
    TImage *HangImage;
    AnsiString S, S1, S2;

    S=S=HSys.CurrentDir+AnsiString("\\hang_data\\");                            // BEN EDIT - S //
    if(!DirectoryExists(S))
    {
        if (!CreateDir(S))
        {
            S2="Cannot create "+S;
            throw Exception(S2);
//            return ;
        }
    }                                                                           // BEN EDIT - E //
    S1.printf("%04d_%02d_%02d  %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    S+=S1;
    mkdir(S.c_str());

    pgcMain->ActivePage=tsMonitorView;
    HangImage=new TImage(this);
    hdc1=GetDC(Handle);
    HangImage->Width=Width;
    HangImage->Height=Height;

    sbMotionViewClick(this);                                                    // MOTION VIEW //
    BitBlt(HangImage->Canvas->Handle, 0, 0, Width, Height, hdc1, 0, 0, SRCCOPY);
    S1=S+"\\simulate.bmp";
    HangImage->Picture->SaveToFile(S1);

    sbMotorViewClick(this);                                                     // MOTION VIEW //
    BitBlt(HangImage->Canvas->Handle, 0, 0, Width, Height, hdc1, 0, 0, SRCCOPY);
    S1=S+"\\MotorView.bmp";
    HangImage->Picture->SaveToFile(S1);

    sbRecordClick(this);
//    for(int i=0; i<PageControlTaskView->PageCount; i++)
//    {
//        PageControlTaskView->ActivePageIndex=i;
//        BitBlt(HangImage->Canvas->Handle, 0, 0, Width, Height, hdc1, 0, 0, SRCCOPY);
//        S1.printf("\\TaskView%03d.bmp",i);
//        S1=S+S1;
//        HangImage->Picture->SaveToFile(S1);
//    }
    ReleaseDC(Handle,hdc1);
    delete HangImage;
}
//==============================================================================
void __fastcall TfMain::sbStoreHangupClick(TObject *Sender)                     // button store hangup //
{
//    StoreHangupData();
    __try
    {
        sr->TriggerSnapshot("Manual");                                          //AI(HT172-Maintainer) 20260420 : full snapshot zip
    }
    __finally
    {
        sbStoreHangup->Down=false;
    }
}
//==============================================================================
void __fastcall TfMain::Timer2Timer(TObject *Sender)                            // ben add 20110712 //
{
    if(InitialOK==false)
        return;
    static bool flag=false;
    if(flag==true)
        return;
    flag=true;
    static int ct=0;
    if(ct++>8)
    {
        FlushFlag=!FlushFlag;                                                   // for system all flush 同步
        ct=0;
    }
    UpdateRecordScreen();
    flag=false;
}
//==============================================================================
void TfMain::LoadStartModePicture()
{
    char Estr[2][30]={{"Initial"}, {"Continue"}};
    char Cstr[2][30]={{"初始化"}, {"繼續"}};

    if(HSys.Sys.SystemStart && (HSys.Sys.RunMode==Run_Normal))
        return;

    if(HSys.LastSet.iStartMode==0)
    {
        sbInitial->Visible=false;
        sbContinue->Visible=true;
        RecordProcess("Change Continue Mode");
    }
    else
    {
        sbInitial->Visible=true;
        sbContinue->Visible=false;
        RecordProcess("Change Initial Start Mode");
    }
    if(HSys.LastSet.iLanguageCountry==0)
        pnStartMode->Caption=Estr[HSys.LastSet.iStartMode];
    else
        pnStartMode->Caption=Cstr[HSys.LastSet.iStartMode];
}
//==============================================================================
void __fastcall TfMain::pnStartModeClick(TObject *Sender)                       // ben add change start mode 20110715 //
{
    if(HSys.Sys.SystemStart && (HSys.Sys.RunMode==Run_Normal))
        return;

    if(HSys.LastSet.iStartMode==0)
    {
        HSys.LastSet.iStartMode=1;                                              // change continous //
    }
    else
    {
        HSys.LastSet.iStartMode=0;                                              // change initial //
    }

    LoadStartModePicture();
}
//==============================================================================
void TfMain::LoadRunModePicture()
{
    char Estr[3][30]={{"No Tray"}, {"HasTray"}, {"Real"}};
    char Cstr[3][30]={{"虛擬"}, {"空盤"}, {"實際"}};

    if(HSys.Sys.SystemStart)
        return;

    if(HSys.LastSet.iRealDummy==DUMMY)
    {
        sbRealIcon->Visible=false;
        sbDummyIcon->Visible=true;
        pnRealDummy->Font->Color=clBlack;
        RecordProcess("Change Dummy Mode1");
        bMessageAlarm=false;
    }
    else if(HSys.LastSet.iRealDummy==REALLY)
    {
        sbRealIcon->Visible=true;
        sbDummyIcon->Visible=false;
        pnRealDummy->Font->Color=clRed;
        RecordProcess("Change Real IC Mode");
    }
     else if(HSys.LastSet.iRealDummy==HAS_TRAY)
    {
        sbRealIcon->Visible=false;
        sbDummyIcon->Visible=true;
        pnRealDummy->Font->Color=clWhite;
        RecordProcess("Change Dummy Mode2");
        bMessageAlarm=false;
    }

    if(HSys.LastSet.iLanguageCountry==0)
        pnRealDummy->Caption=Estr[HSys.LastSet.iRealDummy];
    else
        pnRealDummy->Caption=Cstr[HSys.LastSet.iRealDummy];
}
//==============================================================================
void __fastcall TfMain::pnRealDummyClick(TObject *Sender)                       // ben add real dummy 20110715 //
{
    if(HSys.Sys.SystemStart)
        return;
    if(HSys.AccessLevel==0)
        return;
    HSys.LastSet.iRealDummy++;

    if(HSys.LastSet.iRealDummy>=3)
        HSys.LastSet.iRealDummy=0;

    LoadRunModePicture();
    UpdateAllParameter();
    EventReport(SECS_EVENT.RealDummy);                                          //Steven 20130723
}
//==============================================================================
void __fastcall TfMain::cbbUserSelectChange(TObject *Sender)                    //Daver add
{
    int iOldLevel=HSys.AccessLevel;
    AnsiString str1;
    TDateTime dtmp;
    unsigned short uh ,um ,us ,umm;

    int i, l;
    char dest[20],dest1[20],dest2[20];
    char str[256];
    AnsiString S, asPassword, asHonPrecPassword;
    static bool bEnter=false;
    static bool bError=false;

    if(cbbUserSelect->ItemIndex==0)
    {
        asNowUserID="Operator";
        asUser=asNowUserID;
        HSys.AccessLevel=0;
        cb_WorkFile->Enabled=false;
        return;
    }

    dtmp=Now();
    dtmp.DecodeTime(&uh, &um, &us, &umm);
    asHonPrecPassword=uh;

    AnsiString pwPath="C:\\winnt\\system32\\tech.com";
    if(FileExists(pwPath))                                                      //2012-01-03    Dell modify
    {
        fPassword->ComboBox1->Visible=true;                                     // switch to handler directory
        PassList[0]->Clear();                                                   //Name
        PassList[1]->Clear();                                                   //AccessLevel
        PassList[2]->Clear();                                                   //Pass
        PassList[3]->Clear();                                                   //Count

        PassList[3]->LoadFromFile(pwPath);                                      //2012-01-03    Dell modify
        fPassword->ComboBox1->Clear();                                          //Steven 20140530 : 使用密碼本時,用下拉選單選使用者
        for(i=0; i<PassList[3]->Count; i++)
        {
            strcpy(str, PassList[3]->Strings[i].c_str());
            SplitStrByDotSpaceOnly(str, dest, 20);
            PassList[0]->Add(dest);

            SplitStrByDotSpaceOnly(str, dest1, 20);
            PassList[1]->Add(dest1);

            SplitStrByDotSpaceOnly(str, dest2, 20);
            asPassword=dest2;
            PassList[2]->Add(asPassword);

            if(atoi(PassList[1]->Strings[i].c_str())==cbbUserSelect->ItemIndex)
            {
                fPassword->ComboBox1->Items->Add(dest);                         //Steven 20140530 : 使用密碼本時,用下拉選單選使用者
            }
        }

        if(PassList[0]->Count!=0)
        {
            asHonPrecPassword=uh+um;
            fPassword->Edit2->Text="";                                          // name
            fPassword->Edit1->Text="";                                          // password
            fPassword->ShowModal();
            for(i=0; i<PassList[0]->Count; i++)
            {
                l=atoi(PassList[1]->Strings[i].c_str());
                if(fPassword->Edit2->Text.UpperCase()==PassList[0]->Strings[i].UpperCase())
                {
                    if(l==3 && fPassword->Edit1->Text.UpperCase()==asHonPrecPassword.c_str())
                    {
                        bError=false;
                    }
                    else if((l==1 || l==2) && fPassword->Edit1->Text.UpperCase()==PassList[2]->Strings[i].UpperCase())
                    {
                        bError=false;
                    }
                    else
                    {
                        bError=true;
                    }

                    if(bError==false)
                    {
                        HSys.AccessLevel=l;
                        UseName=PassList[0]->Strings[i];
                        bEnter=false;

                        cbbUserSelect->Text=PassList[0]->Strings[i];

                        asNowUserID=cbbUserSelect->Text;
                        asUser=asNowUserID;
                        if(HSys.AccessLevel==0)
                        {
                            asNowUserLevel="Operator";
                        }
                        else if (HSys.AccessLevel==1)
                        {
                            asNowUserLevel="Engineer";
                        }
                        else if (HSys.AccessLevel==2)
                        {
                            asNowUserLevel="Supervisor";
                        }
                        else if (HSys.AccessLevel==3)
                        {
                            asNowUserLevel="HonPrec";
                        }

                        S=AnsiString("======== ")+asNowUserID+AnsiString(" login ========")+asNowUserLevel;
                        RecordProcess(S);
                        EventReport(SECS_EVENT.ChangeUser);
                        return;
                    }
                }
                else
                {
                    bError=true;
                }
            }
        }

        if(bError==true)
        {
            asNowUserID="Operator";
            asUser=asNowUserID;
            SetUserToOP();
            cb_WorkFile->Enabled=false;
            bEnter=false;
        }
    }
    else
    {
        fPassword->ComboBox1->Visible=false;

        asNowUserID=cbbUserSelect->Items->Strings[cbbUserSelect->ItemIndex];
        if((cbbUserSelect->ItemIndex==1)||(cbbUserSelect->ItemIndex==2))        //Engineer & Supervisor
        {
            if(cbbUserSelect->ItemIndex==1)
            {
                fPassword->Edit2->Text="Engineer";
            }
            else if(cbbUserSelect->ItemIndex==2)
            {
                fPassword->Edit2->Text="Supervisor";
            }
            else
            {
                fPassword->Edit2->Text="";
            }

            fPassword->Edit1->Text="";

            fPassword->ShowModal();
            if (FormSysTools->CheckPassword(cbbUserSelect->ItemIndex,
                                            fPassword->Edit2->Text,
                                            fPassword->Edit1->Text))
            {
                HSys.AccessLevel=cbbUserSelect->ItemIndex;
                asNowUserID=fPassword->Edit2->Text;
            }
            else
            {
                SetUserToOP();
            }
        }
        else if(cbbUserSelect->ItemIndex==3)                                    //HonPrec
        {
            fQwertyKey->ShowQwertyKey(fPassword->Edit1, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);

            if(fPassword->Edit1->Text==asHonPrecPassword.c_str())
            {
                HSys.AccessLevel=cbbUserSelect->ItemIndex;
                SetLotInTimeOutStart();
            }
            else
            {
                SetUserToOP();
            }
        }
        else
        {
            SetUserToOP();
        }

        if(HSys.AccessLevel==0)
        {
            asNowUserID="Operator";
        }
        else if (HSys.AccessLevel==3)
        {
            asNowUserID="HonPrec";
        }

        if(HSys.AccessLevel!=iOldLevel)
        {
            if((cbbUserSelect->ItemIndex==1)||(cbbUserSelect->ItemIndex==2))
            {
                asUser=fPassword->Edit2->Text;
                str1.sprintf("Change Level -- %s : %s", cbbUserSelect->Text, fPassword->Edit2->Text);
            }
            else
            {
                asUser=cbbUserSelect->Text;
                str1.sprintf("Change Level -- %s", cbbUserSelect->Text);
            }
            RecordProcess(str1);
        }
        btnTrayMap->Visible=(HSys.AccessLevel==3);
    }
    EventReport(SECS_EVENT.ChangeUser);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::sbClearCountClick(TObject *Sender)
{
    if(HSys.Sys.SystemStart==true)
        return;

    int ret;
    ret=ShowMyMessageBox_YES_NO("Clear Count? (確定要清空計數？)");
    if(ret==TMyMessageBox::msgrtnNO)
        return;

    ZeroMemory(tRunData.BinICCnt, sizeof(tRunData.BinICCnt));
    ZeroMemory(tRunData.TrayICCnt, sizeof(tRunData.TrayICCnt));
    tRunData.TotalIC=0;
    ShowBinCount();
    palloseCnt->Caption="0";
    EventReport(SECS_EVENT.ClearCount);                                         //Steven 20130723
}
//---------------------------------------------------------------------------
void TfMain::ShowProductInfo()
{
    static bool bRunning=false;
    if(bRunning==true)
        return;
    bRunning=true;
    //
    TDateTime dtmp;
    static TDateTime PauseStart ,Pause1 ,PauseCount, AlarmStart, Alarm, AlarmCount;

    sgProductInfo->Cells[1][eLotStart ] = FormatDateTime("hh:nn:ss", tRunData.StartTime);
    sgProductInfo->Cells[1][eLotEnd   ] = FormatDateTime("hh:nn:ss", tRunData.LotEndTime);
    sgProductInfo->Cells[1][eAlarmTime] = FormatDateTime("hh:nn:ss", tRunData.AlarmTime);
    sgProductInfo->Cells[1][ePauseTime] = FormatDateTime("hh:nn:ss", tRunData.PauseTime);

    if(HSys.Sys.SystemStart==true && bFirstRun==false)
    {
        PauseStart =Now();
        PauseCount = tRunData.PauseTime;
        if(tRunData.TotalIC>0)
        {
            tRunData.JamRate =  ((double)tRunData.JamCount/tRunData.TotalIC)*GetJamRateDenom() ;
            tRunData.UPH=GetCalculateUPH(Now());
            sgProductInfo->Cells[1][eUPH] = IntToStr(tRunData.UPH);
        }
    }
    else if(HSys.Sys.SystemStart==false && RunInfo.LotStart)
    {
        if(cMathTool().IsDoubleEqual(PauseStart,0.0))
        {
            PauseStart=Now();
        }
        Pause1=Now()-PauseStart;
        tRunData.PauseTime = Pause1 + PauseCount;
    }

    if(fNote->fShow==false && RunInfo.LotStart)
    {
        AlarmStart=Now();
        AlarmCount=tRunData.AlarmTime;
    }
    else if(fNote->fShow && RunInfo.LotStart)
    {
        Alarm= Now()-AlarmStart;
        tRunData.AlarmTime=Alarm+AlarmCount;
    }
    //
    bRunning=false;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::UpdateRecordScreen()                                    //Steven 20110827 :重新整理
{
    static Word OldMin=9999, OldSec=9999, P, n;
    static TDateTime StartTime1, Pause11, PauseTime1, PauseCount1, EndTime1, IntervalTime;
    if(SystemMin==9999 || SystemSec==9999)
        return; 

    if(OldMin==9999)
    {
        OldMin=SystemMin;
        OldSec=SystemSec;
    }

    if(OldMin==SystemMin && OldSec==SystemSec)
        return;

    P=OldMin*60+OldSec;           // last time
    n=SystemMin*60+SystemSec;

    if(n<P)
        n+=3600;

    P=n-P;

    if(P>3)
        P=1; // maybe user change system time

    OldMin=SystemMin;
    OldSec=SystemSec;

    //程式打開-------
    if(P>0)
    {
        HSys.LastSet.SystemTimeRecord[stPowerOn]+=P;       //Acc Power On
    }

    //機器有在跑------
    if(HSys.Sys.SystemStart==true)
    {
        HSys.LastSet.SystemTimeRecord[stStartTime]+=P;             //機器在動的時間

        if(fAllMotorHome==false)
        {
           HSys.LastSet.SystemTimeRecord[stHomeTime]+=P;          //歸零時間
        }
        else if(fSetup->fShow==true)
        {
            HSys.LastSet.SystemTimeRecord[stContactTest]+=P;       //contact test or auto height Time
        }
        else
        {
            HSys.LastSet.SystemTimeRecord[stProductTime]+=P;       //Acc Wrok Time
        }

        if(tRunData.JamCount!=0)
            HSys.LastSet.SystemTimeRecord[stMTBA]=((HSys.LastSet.SystemTimeRecord[stStartTime])/tRunData.JamCount);
        else
            HSys.LastSet.SystemTimeRecord[stMTBA]=HSys.LastSet.SystemTimeRecord[stStartTime];

    }

    //機器沒在跑------
    if(HSys.Sys.SystemStart==false && fNote->fShow==false)
    {
        HSys.LastSet.SystemTimeRecord[stPauseTime]+=P;     //Pause Time
    }

    if(fNote->fShow)
    {
//        bStartMTBA=false;
        HSys.LastSet.SystemTimeRecord[stJamTime]+=P;   //Jam Time
    }
}
//------------------------------------------------------------------------------
void TfMain::AddSuckMessage(AnsiString Msg)
{
    if(memoSuckStatus->Lines->Count>1024)
        memoSuckStatus->Clear();

    memoSuckStatus->Lines->Add(Msg);
}
//------------------------------------------------------------------------------
void __fastcall TfMain::sbTimeDataClick(TObject *Sender)
{
    pgcLog->ActivePage=tsTimeData;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::spbClearLoadCountClick(TObject *Sender)
{
    if(HSys.Sys.SystemStart==true)
        return;

    int ret;
    if(bOLPLoadMap)
    {

    }
    else
    {
        ret=ShowMyMessageBox_YES_NO("Clear Count? (確定要清空計數？)");
        if(ret==TMyMessageBox::msgrtnNO)
            return;
    }
    tRunData.Clear();
    ZeroMemory(HSys.LastSet.iJamCount, sizeof(HSys.LastSet.iJamCount));
    if(edLotNo->Text=="")
    {
        myXML.ResetData();
    }
    ShowBinCount();
    TrayArmPara->ClearTrayCount();
    EventReport(SECS_EVENT.ClearCount);                                         //Steven 20130723
    WriteLastDataIni();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnTimeEventClick(TObject *Sender)
{
    EventReport(SECS_EVENT.TimeEvent);
}
//------------------------------------------------------------------------------
void __fastcall TfMain::cb_WorkFileDropDown(TObject *Sender)
{
    if(HasICUnderMachineForCleanOut())
    {
        ShowMyMessage("機台上仍有IC資料,請先clean out");
        return;
    }
    LookForFile();    
}
//------------------------------------------------------------------------------
void __fastcall TfMain::Panel23MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btSaveTimeData->BevelInner=bvLowered;
    btSaveTimeData->BevelOuter=bvLowered;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::spbTrayStatusClick(TObject *Sender)
{
    pgcLog->ActivePage=tsTrayStatus;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::apbLogsClick(TObject *Sender)
{
    pgcLog->ActivePage=tsLogs;
}
//------------------------------------------------------------------------------
void TfMain::AddTimeData(int iRow, double Time)
{
    for(int i=14; i>1; i--)
    {
        fMain->sgTimeData->Cells[i][iRow]=fMain->sgTimeData->Cells[i-1][iRow];
    }
    fMain->sgTimeData->Cells[1][iRow]=AnsiString(Time);

    if(pgcLog->ActivePage==tsTimeData)
        fMain->sgTimeData->Refresh();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::spbStripPosClick(TObject *Sender)
{
    pgcMonitor->ActivePage=tsStripPos;
    LastClickButton=spbStripPos;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::tmrProductInfoTimer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    ShowProductInfo();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::sbActionProcessClick(TObject *Sender)
{
    if(HSys.Sys.SystemStart)
        return;
#ifndef SOFT_SIMULATE
    int ret=ShowMyMessageBox_YES_NO("確定進入確認動作業面??");
    if(ret==TMyMessageBox::msgrtnYES)
#endif
    {
        RecordProcess("Enter Action Check");
//        fAction->ShowModal();
        LastClickButton->Down=true;
        LastClickButton->OnClick(LastClickButton);                                                                                                                                                                                                          
    }
#ifndef SOFT_SIMULATE
    else
        return;
#endif
}
//------------------------------------------------------------------------------
void TfMain::EventLogCreate()                                                   //20151125 add
{
    AnsiString asLotNumber;                                                     //LotNumber: 批號
    AnsiString asReport="D:\\HT-172_Log\\ReportList";                           // 設定檔案放置地方

    AnsiString asStr[5];
    AnsiString sFileName;

    {
        asStr[0].sprintf("          %04d/%02d/%02d/%02d:%02d",SystemYear, SystemMonth, SystemDate, SystemHour,SystemMin);
        asStr[1].sprintf("Start Time:%s  End Time:%s", fMain->sgProductInfo->Cells[1][eLotStart], fMain->sgProductInfo->Cells[1][eLotEnd]);
        asStr[2].sprintf("Alarm Time:%s  Pause Time:%s ", fMain->sgProductInfo->Cells[1][eAlarmTime], fMain->sgProductInfo->Cells[1][ePauseTime]);
        asStr[3].sprintf("Lot No.:%s  UPH:%s",fMain->edLotNo->Text, fMain->sgProductInfo->Cells[1][eUPH]);
        asStr[4].sprintf("------------------------------------------------");

        for(int i=0; i<5; i++)
        {
            MemoEventLog->Lines->Add(asStr[i]);
        }

        sFileName.sprintf("%s\\%04d%02d\\", asReport, SystemYear, SystemMonth);
        if(DirectoryExists(sFileName)==false)
        {
            ForceDirectories(sFileName);
        }
        sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_Temp.csv", asReport, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        MemoEventLog->Lines->SaveToFile(sFileName);
        MemoEventLog->Clear();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::Button1Click(TObject *Sender)
{
    EventLogCreate();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::sbCCDLearningClick(TObject *Sender)
{
#ifndef SOFT_SIMULATE
    int ret=ShowMyMessageBox_YES_NO("是否確認Device??");
    if(ret==TMyMessageBox::msgrtnYES)
#endif
    {
        bTopCCDLearning     =true;
        bDownCCDLearning    =true;
        bAudioCCDLearning   =true;
    }
#ifndef SOFT_SIMULATE
    else
        return;
#endif
}
//==============================================================================
//Click Event,Auto Save log
//==>
//------------------------------------------------------------------------------
void __fastcall TfMain::SearchComponentParent(TWinControl *PCtrl,TMyProc *PP)   //搜尋物件上的Parent Path//
{

}
//------------------------------------------------------------------------------
void __fastcall TfMain::SetComponentClick(TWinControl *PCtrl)                   //搜尋物件上的Button和SpeedButton元件，加入message方式偵測//
{
    for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
    {
        TControl *P = PCtrl->Controls[iP];
        if (dynamic_cast <TPanel *>(P) != NULL          ||
            dynamic_cast <TPageControl *>(P) != NULL    ||
            dynamic_cast <TTabSheet *>(P) != NULL       ||
            dynamic_cast <TTabControl *>(P) != NULL     ||
            dynamic_cast <TForm *>(P) != NULL           ||
            dynamic_cast <THeader *>(P) != NULL         ||
            dynamic_cast <TPage *>(P) != NULL           ||
            dynamic_cast <TGroupBox*>(P) != NULL        ||
            dynamic_cast <TScrollBox*>(P) != NULL       ||
            dynamic_cast <TRadioGroup*>(P) != NULL  )
        {
            SetComponentClick((TWinControl *) P);    // 找該物件附屬的物件(遞迴)
        }
        TButton *PBButton               = dynamic_cast <TButton *>(P);
        TSpeedButton *PBSpeedButton     = dynamic_cast <TSpeedButton *>(P);
        TPanel *PBTPanel                = dynamic_cast <TPanel *>(P);
        if (PBButton != NULL)
        {
            TMyProc *PP;
            PP=new TMyProc;

            PP->asOwnName=PBButton->Name;                                       //考量Caption有可能一樣，故用Name記錄，建議取名有意義的名字//
            PP->asCaption=PBButton->Caption;
            PP->asOwner=PBButton->Owner->Name;
            PP->OldWinProc=PBButton->WindowProc;
            BV->AddObject("TButton",PBButton);
            BL->Add(PP);
        }
        else if (PBSpeedButton != NULL)
        {
            TMyProc *PP;
            PP=new TMyProc;

            PP->asOwnName=PBSpeedButton->Name;
            PP->asCaption=PBSpeedButton->Caption;
            PP->asOwner=PBSpeedButton->Owner->Name;
            PP->OldWinProc=PBSpeedButton->WindowProc;
            BV->AddObject("TSpeedButton",PBSpeedButton);
            BL->Add(PP);
        }
        else if (PBTPanel != NULL && PBTPanel->OnClick!=NULL)                   //部份Panel沒有Click事件，不做記錄//
        {
            TMyProc *PP;
            PP=new TMyProc;

            PP->asOwnName=PBTPanel->Name;
            PP->asCaption=PBTPanel->Caption;
            PP->asOwner=PBTPanel->Owner->Name;
            PP->OldWinProc=PBTPanel->WindowProc;
            BV->AddObject("TPanel",PBTPanel);
            BL->Add(PP);
        }
    }
}
//==============================================================================
void __fastcall TMyProc::MyMessage(TMessage & Message)                          //攔截訊息//
{
    switch (Message.Msg)
    {
        case WM_KEYDOWN:
        case WM_LBUTTONDOWN:
        {
        }
    }   OldWinProc(Message);
}
//------------------------------------------------------------------------------
void __fastcall TfMain::DeletePtr()                                             //刪除New出來的message//
{
    TMyProc *P;
    AnsiString S;
    TButton *TP;
    TSpeedButton *TS;
    TPanel *TL;

    for(int i=0; i<BV->Count; i++)
    {
        P=(TMyProc *)BL->Items[i];
        S=BV->Strings[i];
        if(S=="TButton")
        {
            TP=(TButton *)BV->Objects[i];
            TP->WindowProc=P->OldWinProc;
            delete P;
        }
        else if(S=="TSpeedButton")
        {
            TS=(TSpeedButton *)BV->Objects[i];
            TS->WindowProc=P->OldWinProc;
            delete P;
        }
        else if(S=="TPanel")
        {
            TL=(TPanel *)BV->Objects[i];
            TL->WindowProc=P->OldWinProc;
            delete P;
        }
    }
    delete BL;
    delete BV;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::SaveEventLog(char *Message)                             //儲存EventLog//
{

}
//------------------------------------------------------------------------------
void __fastcall TfMain::DeleteEventLog()                                        //刪除一年前的Event log//
{

}
//------------------------------------------------------------------------------
void __fastcall TfMain::SearchForm()
{
    for(int i=0; i<Application->ComponentCount; i++)
    {
        TComponent *P = Application->Components[i];
        if(dynamic_cast <TForm *>(P) != NULL )
            SetComponentClick((TWinControl *)Application->Components[i]);
    }
}
//------------------------------------------------------------------------------
//<==
//Click Event,Auto Save log
void __fastcall TfMain::btnTrayMapClick(TObject *Sender)
{
    pgcLog->ActivePage=tsMapTray;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnClearTrayClick(TObject *Sender)
{
    mtSortRecv->ClearCell();
}
//------------------------------------------------------------------------------
bool TfMain::ChoiceJobName(AnsiString Data)                                     //kevin 20130907
{
    bool bFindData=false;
    for(int i=0; i<=fMain->cbbUserSelect->Items->Count; i++)
    {
        if(fMain->cbbUserSelect->Items->Strings[i].UpperCase()==Data.UpperCase())
        {
            fMain->cbbUserSelect->Text=fMain->cbbUserSelect->Items->Strings[i];
            fMain->cbbUserSelectChange(fMain);

            HSys.LastSet.iStartMode=1;
            fMain->pnStartModeClick(fMain);
            bFindData=true;
        }
    }

    return bFindData;
}
//------------------------------------------------------------------------------
void TfMain::ShowLoadCarBinCount(AnsiString sStr)
{
    RecordProcess(sStr);
    if(MemoBinCount->Lines->Count>50)
        CleanBinCount();
    MemoBinCount->Lines->Add(sStr);
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnSetBinMatrixClick(TObject *Sender)
{
    LoaderModule->LoadClipIDMapping(edtTrayID->Text);
}
//------------------------------------------------------------------------------
void TfMain::ShowBinCount()
{
    bool bAutoBin[eTrayCount]={false};

    for(int iT=eBinNotUse; iT<eTrayCount; iT++)
    {
        bAutoBin[iT]=false;
    }

    MyBinToTrayStruct BtoT;
    for(int iT=eAuto1; iT<eTrayCount; iT++)
    {
        BtoT.ConvertAutoToBin(iT);
        tTPanelBin[iT]->Caption =BtoT.iBin;
        tTPanelCnt[iT]->Caption =tRunData.TrayICCnt[BtoT.iWhichAuto];
        bAutoBin[iT]=true;
    }

    if(MagazineModule1->MMagSortTopTray->Tray.iBin!=0)                          //Steven 20240627 : 修正Mag Top Tray顯示
    {
        BtoT.ConvertBinToAuto(MagazineModule1->MMagSortTopTray->Tray.iBin);
        pnlMgz1TopID ->Caption=MagazineModule1->MMagSortTopTray->Tray.ClipID;
        pnlMgz1TopBin->Caption=MagazineModule1->MMagSortTopTray->Tray.iBin;
        palMag1TopCnt->Caption=tRunData.TrayICCnt[BtoT.iWhichAuto];
    }

    if(MagazineModule2->MMagSortTopTray->Tray.iBin!=0)
    {
        BtoT.ConvertBinToAuto(MagazineModule2->MMagSortTopTray->Tray.iBin);
        pnlMgz2TopID ->Caption=MagazineModule2->MMagSortTopTray->Tray.ClipID;
        pnlMgz2TopBin->Caption=MagazineModule2->MMagSortTopTray->Tray.iBin;
        palMag2TopCnt->Caption=tRunData.TrayICCnt[BtoT.iWhichAuto];
    }

    if(MagazineModule3->MMagSortTopTray->Tray.iBin!=0)
    {
        BtoT.ConvertBinToAuto(MagazineModule3->MMagSortTopTray->Tray.iBin);
        pnlMgz3TopID ->Caption=MagazineModule3->MMagSortTopTray->Tray.ClipID;
        pnlMgz3TopBin->Caption=MagazineModule3->MMagSortTopTray->Tray.iBin;
        palMag3TopCnt->Caption=tRunData.TrayICCnt[BtoT.iWhichAuto];
    }

    for(int iT=eAuto1; iT<=eAuto20; iT++)
    {
        BtoT.ConvertAutoToBin(iT);
        if(BtoT.iBin==0)                                                        //Steven 20240707 : 修正顯示
            Auto->MAutoTray[iT]->Tray.iEmptyCount=0;
        palEmptyCnt[iT]->Caption=Auto->MAutoTray[iT]->Tray.iEmptyCount;
        palLoadCnt[iT]->Caption =Auto->MAutoTray[iT]->Tray.iCurrCount;
        if(SortArmPara->MotWorkLoader->fHasTray)
            palICIn[iT]->Caption    =SortArmPara->iInputIC_Loader[iT];
        else if(SortArmPara->MotLoader_Car->fHasTray)
            palICIn[iT]->Caption    =SortArmPara->iInputIC_LoadCar[iT];
        else
            palICIn[iT]->Caption    =0;
    }

    for(int iT=eAuto1; iT<eTrayCount; iT++)
    {
        if(bAutoBin[iT]==false)
            tTPanelBin[iT]->Caption="0";
    }

    palloadingCount->Caption  =tRunData.LoaderIC;
    palUnloadingCount->Caption=tRunData.TotalIC;
}
//------------------------------------------------------------------------------
void TfMain::ShowLotBinCount(bool bReset)                                       //Steven 20240627 : Lot的總入料數量
{
    MyBinToTrayStruct BtoT;
    if(bReset)
    {
        for(int iT=eAuto1; iT<eTrayCount; iT++)
        {
            SetLotCnt(iT,0);
        }
    }
    else
    {
        for(int iT=eAuto1; iT<eTrayCount; iT++)
        {
            BtoT.ConvertAutoToBin(iT);
            if(BtoT.iBin!=0)                                                    //Steven 20240707 : 修正顯示
                SetLotCnt(iT,myXML.iTotalBinCount[BtoT.iBin]);
            else
                SetLotCnt(iT,0);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::sbPaperSummaryClick(TObject *Sender)
{
    MyBinToTrayStruct BtoT;
    AnsiString asPath, asStr[eTrayCount], asSchedule, asStr1, asFileName="";

    if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eJHTFormat ||
       HSys.FuncT.iA01_InputDataFormat==LoaderModule->ePTIFormat)
    {
        asSchedule.sprintf("%s_%04d%02d%02d%02d%02d%02d", edLotNo->Text, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        asPath="D:\\BarcodeSorter\\Summary\\";
    }
    else if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eEKeeperFormat)
    {
        asSchedule=LoaderModule->eKeeperSchedule();
        asPath="D:\\BarcodeSorter\\Summary\\";
    }
    else
    {
        asSchedule=edLotNo->Text;
        asPath="D:\\BarcodeSorter\\Summary\\";
    }

    MyForceDirectories(asPath);                                                 //KenHsieh 20240522 : 無資料夾時需先建立
    asFileName.sprintf("%s.txt", asSchedule);
    asPath=asPath+asFileName;
    TStringList *SList;
    bool bHasFile=false;

    for(int iT=eAuto1; iT<eTrayCount; iT++)
    {
        if(HSys.TrayUsage[iT]==eNoTray)
            continue;

        BtoT.ConvertAutoToBin(iT);
        asStr[iT]="";
        if(BtoT.iBin!=0)
            asStr[iT]=asStr1.sprintf("%s:Bin%s=%s", sTrayNameEKeeper[iT], tTPanelBin[iT]->Caption, tTPanelCnt[iT]->Caption);
        else
            asStr[iT]=asStr1.sprintf("%s:Bin%s=NA;", sTrayNameEKeeper[iT], tTPanelBin[iT]->Caption, tTPanelCnt[iT]->Caption);
    }

    bHasFile=FileExists(asPath);
    if(bHasFile==false)
    {
        SList=new TStringList();                                                //Steven 20170518 (jou) : 換位置改善UPH
        SList->Clear();
        for(int iT=eAuto1; iT<eTrayCount; iT++)
        {
            if(HSys.TrayUsage[iT]==eNoTray)
                continue;

            if(asStr[iT]!="")
                SList->Add(asStr[iT]);                                          //Steven 20200416 : 整合Eventlog的title
        }
        SList->SaveToFile(asPath);
        SList->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete SList;                                                           //Steven 20170518 (jou) : 換位置改善UPH
    }
}
//------------------------------------------------------------------------------
AnsiString TfMain::ekeeperSummary()
{
    MyBinToTrayStruct BtoT;
    AnsiString Buffer2="", Buffer3="", asStr[eTrayCount], asStr1, Buffer1="";

    for(int iT=eAuto1; iT<eTrayCount; iT++)
    {
        if(HSys.TrayUsage[iT]==eNoTray)
            continue;

        BtoT.ConvertAutoToBin(iT);
        asStr[iT]="";
        if(BtoT.iBin!=0)
            asStr[iT]=asStr1.sprintf("%s:Bin%s=%s;", sTrayNameEKeeper[iT], tTPanelBin[iT]->Caption, tTPanelCnt[iT]->Caption);
        else
            asStr[iT]=asStr1.sprintf("%s:Bin%s=NA;", sTrayNameEKeeper[iT], tTPanelBin[iT]->Caption, tTPanelCnt[iT]->Caption);

        Buffer1+=asStr[iT];
    }
    return Buffer1;
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnSetFullTray_Auto1Click(TObject *Sender)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    MyBinToTrayStruct mBtoT;
    int iTag=Ptr->Tag;
    mBtoT.ConvertAutoToBin(iTag);
    if(mBtoT.iAutoRow>=eTrack1 && mBtoT.iAutoCol>=eTrackCol1)
        Auto->Mot_Auto_Work[mBtoT.iAutoRow][mBtoT.iAutoCol]->InitNewTray(HAS_IC);
}
//------------------------------------------------------------------------------
void __fastcall TfMain::lblLoadCurrID_1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HSys.Sys.SystemStart)
        return;

    if(HSys.AccessLevel<3)
        return;

    if(Button==mbRight && LoaderModule->MotWorkLoader->fHasTray)
    {
        TrayEditForm->EditTray(LoaderModule->MotWorkLoader->Tag, true);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::lblLoaderCarIDMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HSys.Sys.SystemStart)
        return;

    if(HSys.AccessLevel<3)
        return;

    if(Button==mbRight && LoaderModule->MotLoader_Car->fHasTray)
    {
        TrayEditForm->EditTray(LoaderModule->MotLoader_Car->Tag, true);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::lblMgzCurrID_1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HSys.Sys.SystemStart)
        return;

    if(HSys.AccessLevel<3)
        return;

    if(Button==mbRight && MagArmPara->MMagSortTray->fHasTray)
    {
        TrayEditForm->EditTray(MagArmPara->MMagSortTray->Tag, true);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::palAuto01IDMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HSys.Sys.SystemStart)
        return;

    TPanel *Ptr;
    Ptr=(TPanel *)Sender;

    if(Button==mbRight)
    {
        if(Auto->MAutoTray[Ptr->Tag]->fHasTray)
        {
            TrayEditForm->EditTray(Auto->MAutoTray[Ptr->Tag]->Tag, false);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::mtAuto01MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HSys.Sys.SystemStart)
        return;

    TTMyTray *Ptr;
    Ptr=(TTMyTray *)Sender;

    if(Button==mbRight)
    {
        if(Auto->MAutoTray[Ptr->Tag]->fHasTray)
        {
            TrayEditForm->EditTray(Auto->MAutoTray[Ptr->Tag]->Tag, false);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnClearMgz1Click(TObject *Sender)
{
    MagazineModule1->MMagSortTopTray->ClearTray();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnClearMgz2Click(TObject *Sender)
{
    MagazineModule2->MMagSortTopTray->ClearTray();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnClearMgz3Click(TObject *Sender)
{
    MagazineModule3->MMagSortTopTray->ClearTray();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::mtNowSortTrayMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HSys.Sys.SystemStart)
        return;

    if(Button==mbRight && LoaderModule->MotNowSortTray->fHasTray)               //JerryYang 20240601 : Add Edit Tray of MotNowSortTray
    {
        TrayEditForm->EditTray(LoaderModule->MotNowSortTray->Tag, true);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::mtMagNowSortTrayMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HSys.Sys.SystemStart)
        return;

    if(Button==mbRight && MagArmPara->MMagNowSortTray->fHasTray)                //JerryYang 20240601 : Add Edit Tray of MotNowSortTray
    {
        TrayEditForm->EditTray(MagArmPara->MMagNowSortTray->Tag, true);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMain::mtMag2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HSys.Sys.SystemStart)
        return;

    TPanel *Ptr;
    bool bHasTray=false;

    Ptr=(TPanel *)Sender;

    if(Button==mbRight)                                                         //JerryYang 20240601 : Add Edit Tray of MotNowSortTray
    {
        if(Ptr->Tag==21)                                                        //Kenhsieh 20250820 : 修正Magazine 無法編輯Tray
        {
            bHasTray=MagArmPara->MMagSortTopTray[0]->fHasTray;
            if(bHasTray)
                TrayEditForm->EditTray(MagArmPara->MMagSortTopTray[0]->Tag, true);
        }
        else if(Ptr->Tag==22)
        {
            bHasTray=MagArmPara->MMagSortTopTray[2]->fHasTray;
            if(bHasTray)
                TrayEditForm->EditTray(MagArmPara->MMagSortTopTray[2]->Tag, true);
        }
        else
        {
            bHasTray=MagArmPara->MMagSortTopTray[1]->fHasTray;
            if(bHasTray)
                TrayEditForm->EditTray(MagArmPara->MMagSortTopTray[1]->Tag, true);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::btnClearNowLoadClick(TObject *Sender)
{
    LoaderModule->MotNowSortTray->Tray.ClearData();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnClearNowMagClick(TObject *Sender)
{
    MagArmPara->MMagNowSortTray->Tray.ClearData();
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnLotStartClick(TObject *Sender)
{
    int iBin=0;
    MyBinToTrayStruct mLotBin;
    if(HSys.Sys.SystemStart==true)
    {
        return;
    }
    AnsiString sTemp="", sLoadMapFile="", sLog="", str="";

    spbClearLoadCountClick(btnClearCount);
    lblLotInfo->Caption="";

    if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eJHTFormat ||             //JerryYang 20240610 : ASECL download bin tray map
       HSys.FuncT.iA01_InputDataFormat==LoaderModule->ePTIFormat)
    {
        if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->ePTIFormat)           //Jimmychiu 20251216 : add check loader、empty1、empty2 has tray before start lot
        {
            bool bErr=false;
            AnsiString sErrMsg="";
            if(LoaderModule->Sn_CarHasTray[(int)eLoader1]->IsOn() ||
               LoaderModule->Sn_CCDPos[(int)eLoader1]->IsOn())
            {
                sErrMsg="Please Remove Loader car Tray!";
                bErr=true;
            }
            else if(LoaderModule->Sn_CarHasTray[(int)eEmpty1]->IsOn() ||
                    LoaderModule->Sn_CCDPos[(int)eEmpty1]->IsOn())
            {
                sErrMsg="Please Remove Empty1 car Tray!";
                bErr=true;
            }
            else if(LoaderModule->Sn_CarHasTray[(int)eEmpty2]->IsOn() ||
                    LoaderModule->Sn_CCDPos[(int)eEmpty2]->IsOn())
            {
                sErrMsg="Please Remove Empty2 car Tray!";
                bErr=true;
            }
            if(bErr==true)
            {
                ShowMyMessage(sErrMsg);
                return;
            }
        }
        if(edLotNo->Text=="")
        {
            if(bOLPLoadMap)
            {

            }
            else
            {
                ShowMyMessage("Please Enter LotID !");
                return;
            }
        }
        if(HSys.FuncN.iN02_JHTFormatLoadSelect==0)
        {
            LoaderModule->SummaryFileName=fFTPClient->Download_2DSortingList(edLotNo->Text);
        }
        else
        {
            sLoadMapFile="";
            fSetup->flLoadTrayMap->Mask="D:\\BarcodeSorter"+AnsiString("\\*.log");
            fSetup->flLoadTrayMap->Update();
            for(int i=0; i<fSetup->flLoadTrayMap->Items->Count; i++)
            {
                sTemp=fSetup->flLoadTrayMap->Items->Strings[i];
                if(sTemp.Pos(edLotNo->Text)!=0)
                {
                    if(sLoadMapFile=="")
                    {
                        sLoadMapFile=sTemp;
                    }
                    else
                    {
                        if(bOLPLoadMap)
                        {

                        }
                        else
                        {
                            sLog.sprintf("Duplicate LotID(%s) found", edLotNo->Text);
                            ShowMyMessage(sLog);
                        }
                        return;
                    }
                }
            }
            if(sLoadMapFile!="")
                LoaderModule->SummaryFileName=sLoadMapFile;
            else
            {
                if(bOLPLoadMap)
                {

                }
                else
                {
                    sLog.sprintf("LotID(%s) not found from D:\BarcodeSorter", edLotNo->Text);
                    ShowMyMessage(sLog);
                }
                return;
            }
        }
        if(DecodeTrayMapByJHTFormat(LoaderModule->SummaryFileName)==SUMMARY_SUCCESS)
        {
            Memo1->Lines->Clear();
            AnsiString str;

            for(IterTrayMapList=mapTrayMapList.begin(); IterTrayMapList!=mapTrayMapList.end(); IterTrayMapList++)
            {
                if(Memo1->Lines->Count>=50)   //AI(ht172-memo1cap) 20260617: cap tray-map memo to stop lastset.ini bloat (was up to ~1.26M lines/72MB). Keep first 50 lines.
                    break;
                Memo1->Lines->Add(IterTrayMapList->second.TRAY_ID);

                for(int R=0; R<30; R++)
                {
                    if(Memo1->Lines->Count>=50)   //AI(ht172-memo1cap) 20260617: keep first 50 lines only
                        break;
                    str="";
                    for(int C=0; C<20; C++)
                    {
                        str=str+AnsiString(IterTrayMapList->second.DevInfo[R][C].iBin)+AnsiString(" ");
                    }
                    Memo1->Lines->Add(str);
                }
                Memo1->Lines->Add("//-----------------------------------------\n");
            }
        }
        else
        {
             return;
        }
    }
    else if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eEKeeperFormat)
    {
        if(edLotNo->Text=="")
        {
            LoaderModule->SummaryFileName=LoaderModule->eKeeperSchedule();
        }

        if(edLotNo->Text=="")
        {
            ShowMyMessage("Can not got LotID from eKeeper!");
            return;
        }
    }

    RecordProcess("Lot start pressed.");
    bStartCallLot_StartExe=true;
    edLotNo->Enabled=false;
    edWaferLot->Enabled=false;
    edCusDevice->Enabled=false;
    edInsertion->Enabled=false;
    edFlowID->Enabled=false;
    edOperator->Enabled=false;

    btnLotStart->Enabled=false;

    RunInfo.LotStart=true;
    EventReport(SECS_EVENT.PressLotStart);

    if(HSys.FuncT.iT03_AutoBinSet)                                              //Sam 20240702 : Lot Start 自動更新 Bin Setting
    {
        if(CUSTOMER_CODE==CC_WINSTEK)
        {
            //
        }
        else
        {
            if(fSetup->AutoBinSet()==false)
            {
                bStartCallLot_StartExe=false;
                RunInfo.LotStart=false;
                edLotNo->Enabled=true;
                edWaferLot->Enabled=true;
                edCusDevice->Enabled=true;
                edInsertion->Enabled=true;
                edFlowID->Enabled=true;
                edOperator->Enabled=true;
                btnLotStart->Enabled=true;
                RecordProcess("Lot start aborted: AutoBinSet fail.");
                return;
            }
            fSetup->sbUpdateClick(NULL);
        }
        fSetup->OpenWorkFile();
        ShowLotBinCount();
    }

    for(int i=0; i<eTrayCount; i++)
    {
        iByLotTrayCnt[i]=0;
    }

    for(IterTrayMapList=mapTrayMapList.begin(); IterTrayMapList!=mapTrayMapList.end(); IterTrayMapList++)  //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞
    {
        for(int iR=0; iR<HSys.VMot.LoadTray_Car->Tray.YItem; iR++)
        {
            for(int iC=0; iC<HSys.VMot.LoadTray_Car->Tray.XItem; iC++)
            {
                iBin=IterTrayMapList->second.DevInfo[iR][iC].iBin;
                mLotBin.ConvertBinToAuto(iBin);
                if(mLotBin.iWhichAuto!=0)
                {
                    iByLotTrayCnt[mLotBin.iWhichAuto]++;

                }
            }
        }
    }

    int iCountToAuto=0;
    int iLotTotal=0;
    double dYield=0.0;
    for(int i=0; i<eTrayCount; i++)
    {
        if(iByLotTrayCnt[i]>0)
        {
            str.sprintf("%s: %d", sTrayNameBin[i], iByLotTrayCnt[i]);
            Memo1->Lines->Add(str);

            if(i<=eAuto20)
            {
                iCountToAuto+=iByLotTrayCnt[i];
            }
            iLotTotal+=iByLotTrayCnt[i];
        }
    }

    if(iLotTotal!=0)
        dYield=double(iCountToAuto)/double(iLotTotal);
    str.sprintf("%d = %0.2f", iCountToAuto, dYield);
    palloseCnt->Caption=str;                                                    //Steven 20240728
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnLotEndClick(TObject *Sender)
{
    if(HSys.Sys.SystemStart==true)
    {
        return;
    }

    if(HasICUnderMachine()==true)
    {
        ShowMyMessage("Must finish [Clean out]!!");
        return;
    }
    btnLotStart->Enabled=true;
    edLotNo->Enabled=true;
    edWaferLot->Enabled=true;
    edCusDevice->Enabled=true;
    edInsertion->Enabled=true;
    edFlowID->Enabled=true;
    edOperator->Enabled=true;
    sbPaperSummaryClick(sbPaperSummary);

    btnLotStart->Enabled=true;

    RunInfo.LotStart=false;
    SaveJHT_Log();

    RecordProcess("Lot End pressed.");
    edLotNo->Text="";
    edWaferLot->Text="";
    edCusDevice->Text="";
    edInsertion->Text="";
    edFlowID->Text="";
    edOperator->Text="";
    myXML.ResetData();

    EventReport(SECS_EVENT.PressLotEnd);

    if(FileExists(LoaderModule->sCheckDuplicateLotIDLoaderPath()))
        DeleteFile(LoaderModule->sCheckDuplicateLotIDLoaderPath());
    if(FileExists(LoaderModule->sCheckDuplicateLotIDEmpty1Path()))
        DeleteFile(LoaderModule->sCheckDuplicateLotIDEmpty1Path());
    if(FileExists(LoaderModule->sCheckDuplicateLotIDEmpty2Path()))
        DeleteFile(LoaderModule->sCheckDuplicateLotIDEmpty2Path());
}
//------------------------------------------------------------------------------
void __fastcall TfMain::edRandomMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 50);
}
//------------------------------------------------------------------------------
void __fastcall TfMain::btnRandomClick(TObject *Sender)                         //Sam 20240613 : 新增 Random Tray Map 功能
{
    AnsiString asSchedule="", sFullName="", sTrayID="", sTrayMap="",sRandom="";
    int iRandom=0;
    asSchedule=LoaderModule->eKeeperSchedule();
    sTrayID=edtTrayID->Text;
    sFullName.sprintf("D:\\BarcodeSorter\\Matrix\\%s.txt", asSchedule);
    iRandom=atoi(edRandom->Text.c_str());
    for(int iRow=0; iRow<LoaderModule->iYDivision; iRow++)
    {
        for(int iCol=0; iCol<LoaderModule->iXDivision; iCol++)
        {
            sRandom=random(iRandom)+1;
            if(iCol==LoaderModule->iXDivision-1)
                sTrayMap+=sRandom+";";
            else
                sTrayMap+=sRandom+",";
        }
    }
    WriteIniData(sFullName, "BinMatrix", sTrayID, sTrayMap);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::sbResetClick(TObject *Sender)
{
    RecordProcess("RESET pressed");
    EventReport(SECS_EVENT.PressReset);
}
//------------------------------------------------------------------------------
void __fastcall TfMain::SaveJHT_Log()
{
    MyBinToTrayStruct BtoT;
    AnsiString asPath, asStr[eTrayCount], asSchedule, asStr1, asFileName="", str="";


    tRunData.LotEndTime=Now();

    if(CUSTOMER_CODE==CC_WINSTEK)
    {
        if(fMain->edtRunCard->Text=="")
        {
            return;
        }
        else
        {
            asSchedule.sprintf("%s@HWBIN@%s", fMain->edtRunCard->Text, FormatDateTime("yyyymmdd_hhnnss", Now()));
        }
    }
    else
    {
        if(fMain->edLotNo->Text=="" ||
           fMain->edWaferLot->Text=="" ||
           fMain->edInsertion->Text=="" ||
           fMain->edFlowID->Text=="")
        {
            return;
        }
        else
        {
            asSchedule.sprintf("%s@%s@%s@99@HWBIN@%s", fMain->edLotNo->Text, fMain->edWaferLot->Text, fMain->edInsertion->Text, fMain->edFlowID->Text);
        }
    }



    asPath="D:\\BarcodeSorter\\Summary\\";

    ForceDirectories(asPath);                                                 //KenHsieh 20240522 : 無資料夾時需先建立
    asFileName.sprintf("%s.txt", asSchedule);
    asPath=asPath+asFileName;
    TStringList *SList;
    TStringList *SListBin;

    for(int iT=eAuto1; iT<=eAuto20; iT++)
    {
        if(HSys.TrayUsage[iT]==eNoTray)
            continue;

        BtoT.ConvertAutoToBin(iT);
        asStr[iT]="";
        if(BtoT.iBin!=0)
            asStr[iT]=asStr1.sprintf("_%02d:                 %d", BtoT.iBin, tRunData.TrayICCnt[BtoT.iWhichAuto]);
    }

    for(int iT=eMag2_01; iT<=eMag2_27; iT++)
    {
        BtoT.ConvertAutoToBin(iT);
        asStr[iT]="";
        if(BtoT.iBin!=0)
            asStr[iT]=asStr1.sprintf("_%02d:                 %d", BtoT.iBin, tRunData.TrayICCnt[BtoT.iWhichAuto]);
    }


    SList=new TStringList();                                                    //Steven 20170518 (jou) : 換位置改善UPH
    SList->Clear();

    SListBin=new TStringList();                                                 //Steven 20170518 (jou) : 換位置改善UPH
    SListBin->Clear();

    double dDay=0.0, dH=0.0, dN=0.0, ds=0.0;
    int iDay=0, iH=0, iN=0;
    dDay=double(tRunData.LotEndTime)-double(tRunData.StartTime);
    iDay=int(dDay);
    dH=(dDay-iDay)*24.0;
    iH=int(dH);
    dN=(dH-iH)*60.0;
    iN=int(dN);
    ds=(dN-iN)*60;

    SList->Add("Production record");
    SList->Add("--------------------------------------------------------------------------------------");
    str.sprintf("%s_%s",asHandlerID, asSerialNo);                             //Sam 20240717 : add
    SList->Add("Machine ID:          "+str);
    SList->Add("Run Mode:            Normal");
    str.sprintf("Device Set:          %s", fMain->cb_WorkFile->Text);
    SList->Add(str);
    str.sprintf("Lot Number/MO:       %s", fMain->edLotNo->Text);
    SList->Add(str);
    str.sprintf("Sub Lot Number:      %s", fMain->edWaferLot->Text);
    SList->Add(str);
    str.sprintf("Customer:            CUA");
    SList->Add(str);
    str.sprintf("Operator:            %s", fMain->edOperator->Text);
    SList->Add(str);
    str=       "Begin Time:          "+FormatDateTime("yyyy-mm-dd hh:nn:ss", tRunData.StartTime);
    SList->Add(str);
    str=       "End Time:            "+FormatDateTime("yyyy-mm-dd hh:nn:ss", tRunData.LotEndTime);
    SList->Add(str);
    str.sprintf("Duration:            %d:%02d:%02d:%02d", iDay, iH, iN, int(ds));
    SList->Add(str);
    str=       "UPH:                 "+IntToStr(tRunData.UPH);
    SList->Add(str);
    str=       "Jam Rate:            "+AnsiString().sprintf("%f/3000",tRunData.JamRate);
    SList->Add(str);
    SList->Add("--------------------------------------------------------------------------------------");
    str=       "Input Number:        "+IntToStr(tRunData.LoaderIC);
    SList->Add(str);
    str=       "Output Number:       "+IntToStr(tRunData.TotalIC);
    SList->Add(str);
    SList->Add("Bin Info:");


    for(int iT=eAuto1; iT<eTrayCount; iT++)
    {
        if(iT<eMag2_01 || iT>eMag2_27)
        {
            if(HSys.TrayUsage[iT]==eNoTray)
                continue;
        }

        if(asStr[iT]!="")
            SListBin->Add(asStr[iT]);                                          //Steven 20200416 : 整合Eventlog的title
    }
    SListBin->Sort();

    for(int i=0; i<SListBin->Count; i++)
    {
        SList->Add(SListBin->Strings[i]);
    }
    SList->Add("--------------------------------------------------------------------------------------");

    SList->SaveToFile(asPath);
    SList->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete SList;                                                           //Steven 20170518 (jou) : 換位置改善UPH

    SListBin->Clear();
    delete SListBin;

    if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eJHTFormat ||
       HSys.FuncT.iA01_InputDataFormat==LoaderModule->ePTIFormat)
    {
        fFTPClient->Upload_JHT_log_FTP(asSchedule);
    }

}
//---------------------------------------------------------------------------
void TfMain::CheckContinusStartIsReady()                                        //Sam 20240710 : 修正 OneCyce 異常
{
    if(HSys.LastSet.iStartMode==0)
    {
        if(HSys.FuncT.iT02_MagTraySource==1)
        {
            bInitialStartCheckMagLayer=true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::LookForFile()
{
    AnsiString Str=fMain->cb_WorkFile->Text;
    fMain->cb_WorkFile->Clear();
    fMain->UpdateWorkFileComboBox();
    fMain->cb_WorkFile->Text=Str;
}
//---------------------------------------------------------------------------
void TfMain::SetLotCnt(int iPos,int iCnt)
{
    tTPanelLotCnt[iPos]->Caption=IntToStr(iCnt);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Start()
{
    if(HSys.Sys.SystemStart==false)
    {
        if(edLotNo->Text=="")                                                   //Steven 20240625 : 沒有lot ID不能按start
        {
            ShowMyMessage("Please Enter LotID !");
            return;
        }
        CheckContinusStartIsReady();                                            //Sam 20240710 : 修正 StartMode 異常

        RecordProcess("START pressed");

        if(HasICUnderMachine())
            EventReport(SECS_EVENT.PressStartWithIC);
        else
            EventReport(SECS_EVENT.PressStartWithoutIC);

        if(USE_SECS_GEM && HSys.FuncB.bN04_RunCheck)
        {
            bPhysicalStart=true;
            Timer6->Enabled=true;
        }
        else
        {
            HSys.Sys.SystemStart=true;                                              //20140411 wei
            SoftStart=true;
        }

        if(fAllMotorHome==false)
        {
            bHomeByStart=true;
        }
        flagOneCycleTrayEnd=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Timer6Timer(TObject *Sender)
{
    static bool bTimerRunning=false;
    if(InitialOK==false || bTimerRunning==true)
        return;
    bTimerRunning=true;
    static int count=0;                                                         //JerryYang 20250106 : Run check
    if(USE_SECS_GEM && HSys.FuncB.bN04_RunCheck && bPhysicalStart==true)   
    {
        count++;
        if(count>fMaintenance->GetRunCheckTimeout())
        {
            count=0;
            bPhysicalStart=false;
            Timer6->Enabled=false;
            ShowMyMessage("Run Check timeout!");
        }
    }
    else
    {
        count=0;
        Timer6->Enabled=false;
    }
    bTimerRunning=false;
}
//---------------------------------------------------------------------------

void __fastcall TfMain::btnAlarmTestClick(TObject *Sender)
{
//    ShowSystemError(HSys.Sen.SnMGZ2TrayPositionF.Name, K_RETRY);
    CallLotProgram("Tray_Feed");
}
//---------------------------------------------------------------------------
void __fastcall TfMain::ShowOLPState(int iState)
{
    AnsiString Name="";
    Name="Automation ";

    switch(iState)
    {
        case 0:
            labAutomation->Caption=Name+"Off-Line";
            labAutomation->Font->Color=clTeal;
            break;
        case 1:
            labAutomation->Caption=Name+"On-Line";
            labAutomation->Font->Color=clBlue;
            break;
        case 2:
            labAutomation->Caption=Name+"Client Error";
            labAutomation->Font->Color=clMaroon;
            break;
    }
    labAutomation->Alignment=TAlignment(2);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::labAutomationDblClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)                          //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        fAutomation->edinputIP->Text    =HSys.FuncN.sN03_OLP_IP;
        fAutomation->edinputport->Text  =HSys.FuncN.sN03_OLP_Port;
    }

    if(HSys.AccessLevel==3)
        fAutomation->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfMain::btnTransposeClick(TObject *Sender)                      //Sam 20250911 : 新增 AutoInfo 顯示轉向功能
{
    if(GrpTrack[0]->Align==alTop)
    {
        for(int i=0; i<eTrack4; i++)
        {
            GrpTrack[i]->Align=alNone;
            GrpTrack[i]->Width=133;
            GrpTrack[i]->Left=133*i;
        }

        for(int iRow=0; iRow<eTrack4; iRow++)
        {
            for(int iCol=0; iCol<eTrackColTotal; iCol++)
            {
                palAutoInfo[iRow][iCol]->Align=alNone;
                palAutoInfo[iRow][iCol]->Height=150;
                palAutoInfo[iRow][iCol]->Top=150*iCol;
            }
        }

        for(int i=0; i<eTrack4; i++)
        {
            GrpTrack[i]->Align=alLeft;
        }
        for(int iRow=0; iRow<eTrack4; iRow++)
        {
            for(int iCol=0; iCol<eTrackColTotal; iCol++)
            {
                palAutoInfo[iRow][iCol]->Align=alTop;
                palAutoInfo[iRow][iCol]->Color=(TColor)0x00C2B8A6;
            }
        }
    }
    else
    {
        for(int i=eTrack4-1; i>=0; i--)
        {
            GrpTrack[i]->Align=alNone;
            GrpTrack[i]->Height=170;
            GrpTrack[i]->Top=170*(3-i);
        }

        for(int iRow=0; iRow<eTrack4; iRow++)
        {
            for(int iCol=0; iCol<eTrackColTotal; iCol++)
            {
                palAutoInfo[iRow][iCol]->Align=alNone;
                palAutoInfo[iRow][iCol]->Width=122;
                palAutoInfo[iRow][iCol]->Left=122*iCol;
            }
        }
        for(int i=eTrack4-1; i>=0; i--)
        {
            GrpTrack[i]->Align=alTop;
        }

        for(int iRow=0; iRow<eTrack4; iRow++)
        {
            for(int iCol=0; iCol<eTrackColTotal; iCol++)
            {
                 palAutoInfo[iRow][iCol]->Align=alLeft;
                 palAutoInfo[iRow][iCol]->Color=clBtnFace;
            }
        }
    }
    Panel9->Refresh();
}
//---------------------------------------------------------------------------
void TfMain::SetLotInTimeOutStart()
{
    tLotInTimeOutDelay.SetSecAndOn(HSys.FuncT.iA02_LoginTimeout);
    bNeedChange2OP=true;
}
//---------------------------------------------------------------------------
bool TfMain::GetLotInTimeOut()
{
    bool bret=false;
    if(HSys.FuncT.bA02_LoginTimeout)
    {
        bret=tLotInTimeOutDelay.Off();
    }
    return bret;
}
//---------------------------------------------------------------------------
void TfMain::SetUserToOP()
{
    cbbUserSelect->ItemIndex=0;
    HSys.AccessLevel=0;
    cbbUserSelectChange(this);
//    asNowUserID=cbbUserSelect->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::tmrDownCCDTriggerTimer(TObject *Sender)
{
    static bool bTimerRunning=false;
    if(InitialOK==false || bTimerRunning==true)
        return;
    bTimerRunning=true;
    //check Bin display
    if(HSys.FuncT.iT04_BinDisptype==1)
    {
        #ifndef SOFT_SIMULATE
        if(HSys.FuncT.iT05_BinDispComHardwareType==0)
        {
            int iTotal=HSys.BinDisCtrlTFT->GetUsedBinNumber();
            int iMapArea=0;
            if(iTotal>eBinDispTotal)
                iTotal=eBinDispTotal;
            for(int i=0;i<iTotal;i++)
            {
                if(HSys.BinDisCtrlTFT->GerErrNow(i))
                {
                    AnsiString sTempChi="";
                    sTempChi=AnsiString().sprintf("Auto %d",(i+1));
                    iMapArea=i+eAuto1;
                    if(fMaintenance->cbEnableArea[iMapArea]->Checked==false)
                    {
                        continue;
                    }
                    sTempChi=AnsiString().sprintf("%s 異常位置:%s,  請確認Bin顯示器的狀態!", HSys.BinDisCtrlTFT->GetRunStatus(), sTempChi);
                    //AI(ht172-binfix) 20260617: TFT bin-display comm error must NOT stop production (intermittent bad frames); removed ShowMyMessage (DecStopAllMotor+modal). Non-blocking: recover via ResetBinFlow+SerErrNow below.
                    HSys.BinDisCtrlTFT->ResetBinFlow();
                    HSys.BinDisCtrlTFT->SerErrNow(i,false);
                }
            }
            iTotal=HSys.BinDisCtrlMagTFT->GetUsedBinNumber();
            if(iTotal>eBinDispMagTotal)
                iTotal=eBinDispMagTotal;
            for(int i=0;i<iTotal;i++)
            {
                if(HSys.BinDisCtrlMagTFT->GerErrNow(i))
                {
                    AnsiString sTempChi="";
                    sTempChi=AnsiString().sprintf("Mag %d",(i+1));
                    sTempChi=AnsiString().sprintf("%s 異常位置:%s,  請確認Bin顯示器的狀態!", HSys.BinDisCtrlMagTFT->GetRunStatus(), sTempChi);
                    //AI(ht172-binfix) 20260617: TFT bin-display comm error must NOT stop production (intermittent bad frames); removed ShowMyMessage (DecStopAllMotor+modal). Non-blocking: recover via ResetBinFlow+SerErrNow below.
                    HSys.BinDisCtrlMagTFT->ResetBinFlow();
                    HSys.BinDisCtrlMagTFT->SerErrNow(i,false);
                }
            }
        }
        else
        {
            int iTotal=HSys.BinDisCtrlTFT->GetUsedBinNumber();
            int iMapArea=0;
            for(int i=0;i<iTotal;i++)
            {
                if(HSys.BinDisCtrlTFT->GerErrNow(i))
                {
                    AnsiString sTempChi="";
                    if(i>=0 && i<20)
                    {
                        sTempChi=AnsiString().sprintf("Auto %d",(i+1));
                        iMapArea=i+eAuto1;
                        if(fMaintenance->cbEnableArea[iMapArea]->Checked==false)
                        {
                            continue;
                        }
                    }
                    else
                    {
                        sTempChi=AnsiString().sprintf("Mag %d",(i+1-eBinDispTotal));
                    }
                    sTempChi=AnsiString().sprintf("%s 異常位置:%s,  請確認Bin顯示器的狀態!", HSys.BinDisCtrlTFT->GetRunStatus(), sTempChi);
                    //AI(ht172-binfix) 20260617: TFT bin-display comm error must NOT stop production (intermittent bad frames); removed ShowMyMessage (DecStopAllMotor+modal). Non-blocking: recover via ResetBinFlow+SerErrNow below.
                    HSys.BinDisCtrlTFT->ResetBinFlow();
                    HSys.BinDisCtrlTFT->SerErrNow(i,false);
                }
            }
        }
        #endif
    }
    //
    if(HSys.FuncT.bA02_LoginTimeout==true &&
       bNeedChange2OP==true &&
       GetLotInTimeOut()==true)
    {
        SetUserToOP();
    }
    //
    bTimerRunning=false;    
}
//---------------------------------------------------------------------------
void __fastcall TfMain::rgSimuItemsClick(TObject *Sender)
{
    int iSimuType=rgSimuItems->ItemIndex;
    if(iSimuType==eSimuOneAuto)
    {
        chbSimuSingleArea->Clear();
        for(int iArea=eAuto1; iArea<=eAuto20; iArea++)
        {
            chbSimuSingleArea->Items->Add(sTrayNameSpace[iArea]);
        }
        chbSimuSingleArea->ItemIndex=0;
        chbSimuSingleArea->Visible=true;
    }
    else if(iSimuType==eSimuOneMag)
    {
        chbSimuSingleArea->Clear();
        for(int iArea=eMag1_01; iArea<=eMag3_27; iArea++)
        {
            if(MAGAZINE_CNT==1)
            {
                if(iArea>=eMag2_01 &&
                   iArea<=eMag2_27)
                {
                    chbSimuSingleArea->Items->Add(sTrayNameSpace[iArea]);
                }
            }
            else if(MAGAZINE_CNT==2)
            {
                if(iArea>=eMag1_01 &&
                   iArea<=eMag2_27)
                {
                    chbSimuSingleArea->Items->Add(sTrayNameSpace[iArea]);
                }
            }
            else
            {
                chbSimuSingleArea->Items->Add(sTrayNameSpace[iArea]);
            }

        }
        chbSimuSingleArea->ItemIndex=0;
        chbSimuSingleArea->Visible=true;
    }
    else
    {
        chbSimuSingleArea->Visible=false;
    }
    btnLoadSimuData->Visible=(iSimuType>eSimuNone);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::btnLoadSimuDataClick(TObject *Sender)
{
    int iSimuType=rgSimuItems->ItemIndex;
    tSimuData.Clear();
    tSimuData.bRunSimulation=cbEnableSimulation->Checked;
    edLotNo->Text="Simulate Lot";
    if(tSimuData.bRunSimulation==false)
    {
        return;
    }
    if(iSimuType==eSimuOneAuto ||
       iSimuType==eSimuOneMag)
    {
        tSimuData.iMaxBin=1;
    }
    else if(iSimuType==eSimuAllAuto)
    {
        tSimuData.iMaxBin=(int)eAAN_AutoTotal;
    }
    else if(iSimuType==eSimuAllMag)
    {
        tSimuData.iMaxBin=(int)eMagTotal*MAGAZINE_CNT;
    }
    else if(iSimuType==eSimuAllAutoMag)
    {
        tSimuData.iMaxBin=(int)eAAN_AutoTotal+(int)eMagTotal*MAGAZINE_CNT;
    }
    //Set tray map
    DecodeTrayMapBySimulation();
    //Set bin
    if(iSimuType==eSimuOneAuto)
    {
        int iTrayOrder[eTrayCount]={eBinNotUse,
                                    eAuto6,    eAuto7,     eAuto11,    eAuto12,     eAuto16,
                                    eAuto17,   eAuto8,     eAuto13,    eAuto2,      eAuto14,
                                    eAuto9,    eAuto18,    eAuto3,     eAuto19,     eAuto15,
                                    eAuto10,   eAuto4,     eAuto20,    eAuto5,
                                
                                    eMag2_01, eMag2_02, eMag2_03, eMag2_04, eMag2_05, eMag2_06, eMag2_07, eMag2_08, eMag2_09,
                                    eMag2_10, eMag2_11, eMag2_12, eMag2_13, eMag2_14, eMag2_15, eMag2_16, eMag2_17, eMag2_18,
                                    eMag2_19, eMag2_20, eMag2_21, eMag2_22, eMag2_23, eMag2_24, eMag2_25, eMag2_26, eMag2_27,

                                    eFix1, eFix2,

                                    eMag1_01, eMag1_02, eMag1_03, eMag1_04, eMag1_05, eMag1_06, eMag1_07, eMag1_08, eMag1_09,
                                    eMag1_10, eMag1_11, eMag1_12, eMag1_13, eMag1_14, eMag1_15, eMag1_16, eMag1_17, eMag1_18,
                                    eMag1_19, eMag1_20, eMag1_21, eMag1_22, eMag1_23, eMag1_24, eMag1_25, eMag1_26, eMag1_27,

                                    eMag3_01, eMag3_02, eMag3_03, eMag3_04, eMag3_05, eMag3_06, eMag3_07, eMag3_08, eMag3_09,
                                    eMag3_10, eMag3_11, eMag3_12, eMag3_13, eMag3_14, eMag3_15, eMag3_16, eMag3_17, eMag3_18,
                                    eMag3_19,   eMag3_20,   eMag3_21,   eMag3_22,   eMag3_23,   eMag3_24,   eMag3_25,   eMag3_26,   eMag3_27,
                                    eAuto1};
        iTrayOrder[1]=GetAreaByName(chbSimuSingleArea->Text);
        AnsiString sBinSelectedList="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27";
        for(int iBin=1; iBin<=tSimuData.iMaxBin; iBin++)
        {
            if(iBin==1)
            {
                sBinSelectedList=IntToStr(iBin);
            }
            else
            {
                sBinSelectedList=sBinSelectedList+","+IntToStr(iBin);
            }
        }
        fSetup->ParseCommaSeparatedString(sBinSelectedList,fSetup->VecBins);
        fSetup->AutoBinSet(fSetup->VecBins,iTrayOrder);
        ShowLotBinCount();
    }
    else if(iSimuType==eSimuOneMag)
    {
        int iTrayOrder[eTrayCount]={eBinNotUse,
                                    eAuto6,    eAuto7,     eAuto11,    eAuto12,     eAuto16,
                                    eAuto17,   eAuto8,     eAuto13,    eAuto2,      eAuto14,
                                    eAuto9,    eAuto18,    eAuto3,     eAuto19,     eAuto15,
                                    eAuto10,   eAuto4,     eAuto20,    eAuto5,
                                
                                    eMag2_01, eMag2_02, eMag2_03, eMag2_04, eMag2_05, eMag2_06, eMag2_07, eMag2_08, eMag2_09,
                                    eMag2_10, eMag2_11, eMag2_12, eMag2_13, eMag2_14, eMag2_15, eMag2_16, eMag2_17, eMag2_18,
                                    eMag2_19, eMag2_20, eMag2_21, eMag2_22, eMag2_23, eMag2_24, eMag2_25, eMag2_26, eMag2_27,

                                    eFix1, eFix2,

                                    eMag1_01, eMag1_02, eMag1_03, eMag1_04, eMag1_05, eMag1_06, eMag1_07, eMag1_08, eMag1_09,
                                    eMag1_10, eMag1_11, eMag1_12, eMag1_13, eMag1_14, eMag1_15, eMag1_16, eMag1_17, eMag1_18,
                                    eMag1_19, eMag1_20, eMag1_21, eMag1_22, eMag1_23, eMag1_24, eMag1_25, eMag1_26, eMag1_27,

                                    eMag3_01, eMag3_02, eMag3_03, eMag3_04, eMag3_05, eMag3_06, eMag3_07, eMag3_08, eMag3_09,
                                    eMag3_10, eMag3_11, eMag3_12, eMag3_13, eMag3_14, eMag3_15, eMag3_16, eMag3_17, eMag3_18,
                                    eMag3_19,   eMag3_20,   eMag3_21,   eMag3_22,   eMag3_23,   eMag3_24,   eMag3_25,   eMag3_26,   eMag3_27,
                                    eAuto1};
        iTrayOrder[1]=GetAreaByName(chbSimuSingleArea->Text);
        AnsiString sBinSelectedList="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27";
        for(int iBin=1; iBin<=tSimuData.iMaxBin; iBin++)
        {
            if(iBin==1)
            {
                sBinSelectedList=IntToStr(iBin);
            }
            else
            {
                sBinSelectedList=sBinSelectedList+","+IntToStr(iBin);
            }
        }
        fSetup->ParseCommaSeparatedString(sBinSelectedList,fSetup->VecBins);
        fSetup->AutoBinSet(fSetup->VecBins,iTrayOrder);
        ShowLotBinCount();
    }
    else if(iSimuType==eSimuAllAuto)
    {
        int iTrayOrder[eTrayCount]={eBinNotUse,
                                    eAuto6,    eAuto7,     eAuto11,    eAuto12,     eAuto16,
                                    eAuto17,   eAuto8,     eAuto13,    eAuto2,      eAuto14,
                                    eAuto9,    eAuto18,    eAuto3,     eAuto19,     eAuto15,
                                    eAuto10,   eAuto4,     eAuto20,    eAuto5,      eAuto1,
                                
                                    eMag2_01, eMag2_02, eMag2_03, eMag2_04, eMag2_05, eMag2_06, eMag2_07, eMag2_08, eMag2_09,
                                    eMag2_10, eMag2_11, eMag2_12, eMag2_13, eMag2_14, eMag2_15, eMag2_16, eMag2_17, eMag2_18,
                                    eMag2_19, eMag2_20, eMag2_21, eMag2_22, eMag2_23, eMag2_24, eMag2_25, eMag2_26, eMag2_27,

                                    eFix1, eFix2,

                                    eMag1_01, eMag1_02, eMag1_03, eMag1_04, eMag1_05, eMag1_06, eMag1_07, eMag1_08, eMag1_09,
                                    eMag1_10, eMag1_11, eMag1_12, eMag1_13, eMag1_14, eMag1_15, eMag1_16, eMag1_17, eMag1_18,
                                    eMag1_19, eMag1_20, eMag1_21, eMag1_22, eMag1_23, eMag1_24, eMag1_25, eMag1_26, eMag1_27,

                                    eMag3_01, eMag3_02, eMag3_03, eMag3_04, eMag3_05, eMag3_06, eMag3_07, eMag3_08, eMag3_09,
                                    eMag3_10, eMag3_11, eMag3_12, eMag3_13, eMag3_14, eMag3_15, eMag3_16, eMag3_17, eMag3_18,
                                    eMag3_19,   eMag3_20,   eMag3_21,   eMag3_22,   eMag3_23,   eMag3_24,   eMag3_25,   eMag3_26,   eMag3_27
                                    };
        AnsiString sBinSelectedList="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27";
        for(int iBin=1; iBin<=tSimuData.iMaxBin; iBin++)
        {
            if(iBin==1)
            {
                sBinSelectedList=IntToStr(iBin);
            }
            else
            {
                sBinSelectedList=sBinSelectedList+","+IntToStr(iBin);
            }
        }
        fSetup->ParseCommaSeparatedString(sBinSelectedList,fSetup->VecBins);
        fSetup->AutoBinSet(fSetup->VecBins,iTrayOrder);
        ShowLotBinCount();
    }
    else if(iSimuType==eSimuAllMag)
    {
        int iTrayOrder[eTrayCount]={eBinNotUse,
                                    eMag2_01, eMag2_02, eMag2_03, eMag2_04, eMag2_05, eMag2_06, eMag2_07, eMag2_08, eMag2_09,
                                    eMag2_10, eMag2_11, eMag2_12, eMag2_13, eMag2_14, eMag2_15, eMag2_16, eMag2_17, eMag2_18,
                                    eMag2_19, eMag2_20, eMag2_21, eMag2_22, eMag2_23, eMag2_24, eMag2_25, eMag2_26, eMag2_27,

                                    eMag1_01, eMag1_02, eMag1_03, eMag1_04, eMag1_05, eMag1_06, eMag1_07, eMag1_08, eMag1_09,
                                    eMag1_10, eMag1_11, eMag1_12, eMag1_13, eMag1_14, eMag1_15, eMag1_16, eMag1_17, eMag1_18,
                                    eMag1_19, eMag1_20, eMag1_21, eMag1_22, eMag1_23, eMag1_24, eMag1_25, eMag1_26, eMag1_27,

                                    eMag3_01, eMag3_02, eMag3_03, eMag3_04, eMag3_05, eMag3_06, eMag3_07, eMag3_08, eMag3_09,
                                    eMag3_10, eMag3_11, eMag3_12, eMag3_13, eMag3_14, eMag3_15, eMag3_16, eMag3_17, eMag3_18,
                                    eMag3_19,   eMag3_20,   eMag3_21,   eMag3_22,   eMag3_23,   eMag3_24,   eMag3_25,   eMag3_26,   eMag3_27,

                                    eFix1, eFix2,

                                    eAuto6,    eAuto7,     eAuto11,    eAuto12,     eAuto16,
                                    eAuto17,   eAuto8,     eAuto13,    eAuto2,      eAuto14,
                                    eAuto9,    eAuto18,    eAuto3,     eAuto19,     eAuto15,
                                    eAuto10,   eAuto4,     eAuto20,    eAuto5,      eAuto1
                                    };
        AnsiString sBinSelectedList="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27";
        for(int iBin=1; iBin<=tSimuData.iMaxBin; iBin++)
        {
            if(iBin==1)
            {
                sBinSelectedList=IntToStr(iBin);
            }
            else
            {
                sBinSelectedList=sBinSelectedList+","+IntToStr(iBin);
            }
        }
        fSetup->ParseCommaSeparatedString(sBinSelectedList,fSetup->VecBins);
        fSetup->AutoBinSet(fSetup->VecBins,iTrayOrder);
        ShowLotBinCount();
    }
    else if(iSimuType==eSimuAllAutoMag)
    {
        int iTrayOrder[eTrayCount]={eBinNotUse,
                                    eAuto6,    eAuto7,     eAuto11,    eAuto12,     eAuto16,
                                    eAuto17,   eAuto8,     eAuto13,    eAuto2,      eAuto14,
                                    eAuto9,    eAuto18,    eAuto3,     eAuto19,     eAuto15,
                                    eAuto10,   eAuto4,     eAuto20,    eAuto5,      eAuto1,
                                
                                    eMag2_01, eMag2_02, eMag2_03, eMag2_04, eMag2_05, eMag2_06, eMag2_07, eMag2_08, eMag2_09,
                                    eMag2_10, eMag2_11, eMag2_12, eMag2_13, eMag2_14, eMag2_15, eMag2_16, eMag2_17, eMag2_18,
                                    eMag2_19, eMag2_20, eMag2_21, eMag2_22, eMag2_23, eMag2_24, eMag2_25, eMag2_26, eMag2_27,

                                    eMag1_01, eMag1_02, eMag1_03, eMag1_04, eMag1_05, eMag1_06, eMag1_07, eMag1_08, eMag1_09,
                                    eMag1_10, eMag1_11, eMag1_12, eMag1_13, eMag1_14, eMag1_15, eMag1_16, eMag1_17, eMag1_18,
                                    eMag1_19, eMag1_20, eMag1_21, eMag1_22, eMag1_23, eMag1_24, eMag1_25, eMag1_26, eMag1_27,

                                    eMag3_01, eMag3_02, eMag3_03, eMag3_04, eMag3_05, eMag3_06, eMag3_07, eMag3_08, eMag3_09,
                                    eMag3_10, eMag3_11, eMag3_12, eMag3_13, eMag3_14, eMag3_15, eMag3_16, eMag3_17, eMag3_18,
                                    eMag3_19,   eMag3_20,   eMag3_21,   eMag3_22,   eMag3_23,   eMag3_24,   eMag3_25,   eMag3_26,   eMag3_27,

                                    eFix1, eFix2,
                                   };
        AnsiString sBinSelectedList="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27";
        for(int iBin=1; iBin<=tSimuData.iMaxBin; iBin++)
        {
            if(iBin==1)
            {
                sBinSelectedList=IntToStr(iBin);
            }
            else
            {
                sBinSelectedList=sBinSelectedList+","+IntToStr(iBin);
            }
        }
        fSetup->ParseCommaSeparatedString(sBinSelectedList,fSetup->VecBins);
        fSetup->AutoBinSet(fSetup->VecBins,iTrayOrder);
        ShowLotBinCount();
    }
    tRunData.Clear();
}
//---------------------------------------------------------------------------
int TfMain::GetAreaByName(AnsiString sName)
{
    for(int iArea=eAuto1; iArea<=eTrayCount; iArea++)
    {
        if(sTrayNameSpace[iArea].Pos(sName)>0)
        {
            return iArea;
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::cbEnableSimulationClick(TObject *Sender)
{
    if(cbEnableSimulation->Checked)
    {
        rgSimuItems->Visible=true;
        btnLoadSimuData->Visible=true;
    }
    else
    {
        tSimuData.Clear();
        rgSimuItems->Visible=false;
        chbSimuSingleArea->Visible=false;
        btnLoadSimuData->Visible=false;
    }
}
//---------------------------------------------------------------------------

