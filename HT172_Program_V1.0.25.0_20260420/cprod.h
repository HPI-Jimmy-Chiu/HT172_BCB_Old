#ifndef cprodH
#define cprodH

//-------------------------------------------------------------------------

#include "cmydef.h"
#include "CosFunction.h"
//-------------------------------------------------------------------------
struct TRunData
{
    TDateTime StartTime;
    TDateTime LotEndTime;
    TDateTime AlarmTime;
    TDateTime HomeTime;
    TDateTime PauseTime;
    int BinICCnt[TEST_MAX_BIN];
    int TrayICCnt[eTrayCount];
    int LoaderIC;
    int TotalIC;
    int iPauseTime;
    int JamCount;
    int UPH;
    int NozzleForceCT[eNozzleCount];
    double CycleTime[eTimeDataTotal];
    int iEjectionPinCT; //Sam 20190620 : 頂針次數
    double JamRate;
    int JamRateDenom;
    int iTotalQuantity;
    int iAutoQuantity;
    int iMagQuantity;
    TRunData()
    {
        Clear();
    }
    void Clear()
    {
        StartTime=Now();
        LotEndTime=StrToTime("00:00:00");
        PauseTime=StrToTime("00:00:00");
        AlarmTime=StrToTime("00:00:00");
        LoaderIC        =0;
        TotalIC         =0;
        iPauseTime      =0;
        JamCount        =0;
        UPH             =0;
        ZeroMemory(CycleTime,       sizeof(CycleTime));
        ZeroMemory(BinICCnt,        sizeof(BinICCnt));
        ZeroMemory(TrayICCnt,       sizeof(TrayICCnt));
        ZeroMemory(NozzleForceCT,   sizeof(NozzleForceCT));
        iEjectionPinCT=0;
        JamRate=0.0;
        JamRateDenom=10000;
        iTotalQuantity=0;
        iAutoQuantity=0;
        iMagQuantity=0;
    }
    void SetAutoQuantity(int iValue){iAutoQuantity=iValue;}
    void SetMagQuantity(int iValue){iMagQuantity=iValue;}
    int GetTotalQuantity(){return iAutoQuantity+iMagQuantity;}
};
extern TRunData tRunData;
extern int GetJamRateDenom();
//
class TLatchCycleTime
{
private:
    int iCount;
public:
    int LatchCycleTime(bool Start =false);
};
extern TLatchCycleTime lctLoader;

struct TFunction
{
    bool UseCCD;
    bool RejectCCDfail;
    bool UseHitCylinder;
    int  HitRetry;
    bool UsePreAlignment;
};
extern TFunction tFunction;


typedef struct        //20140330 WEI
{
    int IO_Dev_Num[4];
    bool bInitialScan;

    void TMotionnetIO()
    {
        bInitialScan =false;
    }
}TMotionnetIO;
extern TMotionnetIO tMotionnetIO;       //2013-01-15    Dell 開機的時候要先Scan Motionnet IO 數量
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
typedef struct
{
    int iXItem;
    int iYItem;
    int iXPitch;
    int iYPitch;
    int iXEdge;
    int iYEdge;
    int iRotate;
    int iMethod;
} TRAY_DATA ;
extern TRAY_DATA TrayDef;
//-------------------------------------------------------------------------
typedef struct {
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    int Reserved[5];  //  保留
}DATE_INFO;
//-------------------------------------------------------------------------
typedef struct
{
    int iRight;
    int iLeft;
    int iRightForIndexZ2;
    int Reserved[4];
}SM_SHUTTLE;
//------------------------------------------------------------------------
typedef struct
{
    int Baud_Rate;
    int Bit_Length;
    int Stop_Bit;
    int Parity;
}RS232_DATA;
//-------------------------------------------------------------------------
typedef struct
{
    int TrayZseparate                   ;
    int EmptyZseparate                  ;
    int ColorZseparate                  ;
    int InArmPickStartX                 ;
    int InArmPickStartY                 ;
    int InArmZ2PickHieght               ;

    int InArmPlaceToSH2XStart           ;
    int InArmPlaceToSH2YStart           ;
    int InArmShuttle2_Release           ;

    int InArmPlaceToSH1XStart           ;
    int InArmPlaceToSH1YStart           ;
    int InArmShuttle1_Release           ;
    int InArmVarPitchClose              ;
    int InArmVarPitchOpen               ;

    int InArmPickFromHP1XStart          ;
    int InArmPickFromHP1YStart          ;
    int InArmZ2HP1PickHeight            ;
    int HotPlate1_Release               ;
    int InArmPickFromHP2XStart          ;
    int InArmPickFromHP2YStart          ;

    int TestZ2PickFromShuttle2          ;
    int TestZ2PlaceToShuttle2           ;
    int Test_Z2_Position_Offset         ;
    int Shullte2RightStop               ;
    int TestZ1PickFromShuttle1          ;
    int TestZ1PlaceToShuttle1           ;
    int Test_Z1_Position_Offset         ;
    int Shullte1RightStop               ;

    int OutArmPickFromSHT2X             ;
    int OutArmPickFromSHT2Y             ;
    int OutArmZ2SHT2PickHeight          ;
    int OutArmPickFromSHT1X             ;
    int OutArmPickFromSHT1Y             ;
    int OutArmZ2SHT1PickHeight          ;
    int OutArmVarPitchClose             ;
    int OutArmVarPitchOpen              ;

    int OutArmPlacetoFix1X              ;
    int OutArmPlacetoFix1Y              ;
    int OutArmFix1_Release              ;
    int OutArmPlacetoFix2X              ;
    int OutArmPlacetoFix2Y              ;
    int OutArmFix2_Release              ;
    int OutArmPlacetoFix3X              ;
    int OutArmPlacetoFix3Y              ;
    int OutArmFix3_Release              ;
    int OutArmPlacetoAuto1X             ;
    int OutArmPlacetoAuto1Y             ;
    int OutArmAuto1_Release             ;
    int OutArmPlacetoAuto2X             ;
    int OutArmPlacetoAuto2Y             ;
    int OutArmAuto2_Release             ;
    int OutArmPlacetoAuto3X             ;
    int OutArmPlacetoAuto3Y             ;
    int OutArmAuto3_Release             ;

    int CatchtrayArmLoadStage           ;
    int CatchtrayArmEmpty               ;
    int CatchtrayArmColor               ;
    int CatchtrayArmAuto1               ;
    int CatchtrayArmAuto2               ;
    int CatchtrayArmAuto3               ;

    int iOutArmXInspect                 ;
    int iOutArmYInspect                 ;
    int OutArmZ1Inspect                 ;
    int Auto1Zseparate                  ;
    int OutArmInspectWaitTime           ;
    int OutArmAuto1_Pick                ;

    int Shullte1RightHalfStop           ;
    int Shullte2RightHalfStop           ;

    int OutArmZ2Inspect                 ;
    int OutArmZ3Inspect                 ;
    int OutArmZ4Inspect                 ;

    int OutArmPlaceX2Offset             ;
    int OutArmPlaceY2Offset             ;

    int OutArmPlacetoFix1X_Scan         ;
    int OutArmPlacetoFix1Y_Scan         ;
    int OutArmFix1_Release_Scan         ;

    int OutArmPlacetoFix2X_Scan         ;
    int OutArmPlacetoFix2Y_Scan         ;
    int OutArmFix2_Release_Scan         ;

    int OutArmPlacetoFix3X_Scan         ;
    int OutArmPlacetoFix3Y_Scan         ;
    int OutArmFix3_Release_Scan         ;

    int OutArmPlacetoAuto1X_Scan        ;
    int OutArmPlacetoAuto1Y_Scan        ;
    int OutArmAuto1_Release_Scan        ;

    int OutArmPlacetoAuto2X_Scan        ;
    int OutArmPlacetoAuto2Y_Scan        ;
    int OutArmAuto2_Release_Scan        ;

    int OutArmPlacetoAuto3X_Scan        ;
    int OutArmPlacetoAuto3Y_Scan        ;
    int OutArmAuto3_Release_Scan        ;
    //HT9040------------------------------------------
    int Z1PlaceToSH2                    ;
    int Z2PickFrontSH1                  ;

    int iTempMode                       ;
    int SoakTime                        ;
    int Default                         ;
    int iTemperature                    ;

    int OutArmZ2SHT1PlaceHeight         ;
    int InArmPickFromLeftHP1XStart      ;
    int InArmPickFromLeftHP1YStart      ;
    int InArmPickFromLeftHP2XStart      ;
    int InArmPickFromLeftHP2YStart      ;
    int iXInArmRotate[4]                ;
    int iYInArmRotate[4]                ;
    int iZInArmRotatePIC[4]             ;
    int iZInArmRotatePLA[4]             ;
    int iXOutArmRotate[4]               ;
    int iYOutArmRotate[4]               ;
    int iZOutArmRotatePIC[4]            ;
    int iZOutArmRotatePLA[4]            ;
    int iXInArmXPlaceSH1_2OffSet        ;
    int iXInArmXPlaceSH2_2OffSet        ;
    int iXOutArmXPlaceSH1_2OffSet       ;
    int iXOutArmXPlaceSH2_2OffSet       ;

    int Shullte1RightStopForZ2          ;

    int iYOutArmXPlaceSH1_2OffSet       ;
    int iYOutArmXPlaceSH2_2OffSet       ;

    int OutArmPlaceX_Z1_Offset          ;
    int OutArmPlaceY_Z1_Offset          ;
    int OutArmPlaceX_Z3_Offset          ;
    int OutArmPlaceY_Z3_Offset          ;

    int XOutArm_Auto1_Place_ForZ3Z4     ;
    int XOutArm_Auto2_Place_ForZ3Z4     ;
    int XOutArm_Auto3_Place_ForZ3Z4     ;
    int XOutArm_Fix1_Place_ForZ3Z4      ;
    int XOutArm_Fix2_Place_ForZ3Z4      ;
    int XOutArm_Fix3_Place_ForZ3Z4      ;

    int YOutArm_Auto1_Place_ForZ3Z4     ;
    int YOutArm_Auto2_Place_ForZ3Z4     ;
    int YOutArm_Auto3_Place_ForZ3Z4     ;
    int YOutArm_Fix1_Place_ForZ3Z4      ;
    int YOutArm_Fix2_Place_ForZ3Z4      ;
    int YOutArm_Fix3_Place_ForZ3Z4      ;

    int XInArm_TubeLoadKit_Pick         ;
    int YInArm_TubeLoadKit_Pick         ;

    int XOutArm_TubeUnLoadKit_Place     ;
    int YOutArm_TubeUnLoadKit_Place     ;

    int ZInArm_TubeLoadKit_Pick         ;
    int ZOutArm_TubeUnLoadKit_Place     ;
    int ZInArm_ShuttlePlace             ;

    int iTubeSelectTubeBufferGet        ;
    int iTubeSelectTrackPut[2]          ;

    // for In arm pick independ
    int InArmZ1PickHieght               ;
    int InArmZ3PickHieght               ;
    int InArmZ4PickHieght               ;

    int XOutArm_Auto1_Place_ForZ2       ;
    int XOutArm_Auto2_Place_ForZ2       ;
    int XOutArm_Auto3_Place_ForZ2       ;
    int XOutArm_Fix1_Place_ForZ2        ;
    int XOutArm_Fix2_Place_ForZ2        ;
    int XOutArm_Fix3_Place_ForZ2        ;

    int YOutArm_Auto1_Place_ForZ2       ;
    int YOutArm_Auto2_Place_ForZ2       ;
    int YOutArm_Auto3_Place_ForZ2       ;
    int YOutArm_Fix1_Place_ForZ2        ;
    int YOutArm_Fix2_Place_ForZ2        ;
    int YOutArm_Fix3_Place_ForZ2        ;

    int ZOutArm_Auto1_Place_ForZ2       ;
    int ZOutArm_Auto2_Place_ForZ2       ;
    int ZOutArm_Auto3_Place_ForZ2       ;
    int ZOutArm_Fix1_Place_ForZ2        ;
    int ZOutArm_Fix2_Place_ForZ2        ;
    int ZOutArm_Fix3_Place_ForZ2        ;

    int XOutArm_Auto1_Place_ForZ3       ;
    int XOutArm_Auto2_Place_ForZ3       ;
    int XOutArm_Auto3_Place_ForZ3       ;
    int XOutArm_Fix1_Place_ForZ3        ;
    int XOutArm_Fix2_Place_ForZ3        ;
    int XOutArm_Fix3_Place_ForZ3        ;

    int YOutArm_Auto1_Place_ForZ3       ;
    int YOutArm_Auto2_Place_ForZ3       ;
    int YOutArm_Auto3_Place_ForZ3       ;
    int YOutArm_Fix1_Place_ForZ3        ;
    int YOutArm_Fix2_Place_ForZ3        ;
    int YOutArm_Fix3_Place_ForZ3        ;

    int ZOutArm_Auto1_Place_ForZ3       ;
    int ZOutArm_Auto2_Place_ForZ3       ;
    int ZOutArm_Auto3_Place_ForZ3       ;
    int ZOutArm_Fix1_Place_ForZ3        ;
    int ZOutArm_Fix2_Place_ForZ3        ;
    int ZOutArm_Fix3_Place_ForZ3        ;

    int XOutArm_Auto1_Place_ForZ4       ;
    int XOutArm_Auto2_Place_ForZ4       ;
    int XOutArm_Auto3_Place_ForZ4       ;
    int XOutArm_Fix1_Place_ForZ4        ;
    int XOutArm_Fix2_Place_ForZ4        ;
    int XOutArm_Fix3_Place_ForZ4        ;

    int YOutArm_Auto1_Place_ForZ4       ;
    int YOutArm_Auto2_Place_ForZ4       ;
    int YOutArm_Auto3_Place_ForZ4       ;
    int YOutArm_Fix1_Place_ForZ4        ;
    int YOutArm_Fix2_Place_ForZ4        ;
    int YOutArm_Fix3_Place_ForZ4        ;

    int ZOutArm_Auto1_Place_ForZ4       ;
    int ZOutArm_Auto2_Place_ForZ4       ;
    int ZOutArm_Auto3_Place_ForZ4       ;
    int ZOutArm_Fix1_Place_ForZ4        ;
    int ZOutArm_Fix2_Place_ForZ4        ;
    int ZOutArm_Fix3_Place_ForZ4        ;

    int iInArmW50Pitch_Offset           ;
    int iInArmW80Pitch_Offset           ;
    int iInArmW50Pitch2_Offset          ;
    int iInArmW80Pitch2_Offset          ;

    int iInArmW50Pitch_Offset_RotateOn  ;
    int iInArmW80Pitch_Offset_RotateOn  ;
    int iInArmW50Pitch2_Offset_RotateOn ;
    int iInArmW80Pitch2_Offset_RotateOn ;

    int iOutArmW50Pitch_Offset          ;
    int iOutArmW80Pitch_Offset          ;
    int iOutArmW50Pitch2_Offset         ;
    int iOutArmW80Pitch2_Offset         ;

    int iOutArmW50Pitch_Offset_RotateOn ;
    int iOutArmW80Pitch_Offset_RotateOn ;
    int iOutArmW50Pitch2_Offset_RotateOn;
    int iOutArmW80Pitch2_Offset_RotateOn;

    int XInArm_Loader_Pick_ForZ1        ;
    int XInArm_Loader_Pick_ForZ2        ;
    int XInArm_Loader_Pick_ForZ3        ;
    int XInArm_Loader_Pick_ForZ4        ;

    int YInArm_Loader_Pick_ForZ1        ;
    int YInArm_Loader_Pick_ForZ2        ;
    int YInArm_Loader_Pick_ForZ3        ;
    int YInArm_Loader_Pick_ForZ4        ;

    int ZInArm_Loader_Pick_ForZ1        ;
    int ZInArm_Loader_Pick_ForZ2        ;
    int ZInArm_Loader_Pick_ForZ3        ;
    int ZInArm_Loader_Pick_ForZ4        ;

    int iInArmPitchSingleOffSet         ;
    int iInArmPitch2SingleOffSet        ;
    int iOutArmPitchSingleOffSet        ;
    int iOutArmPitch2SingleOffSet       ;

    int iShuttle1SingleSiteXOffSet      ;//Eliot 2007_11_12
    int iShuttle2SingleSiteXOffSet      ;//Eliot 2007_11_12

}RUN_OFFSET;

extern RUN_OFFSET Offset;
extern RUN_OFFSET Offset2;
//-------------------------------------------------------------------------
//------------------------------------------------------------------------- 
typedef struct
{
    int  iLanguageCountry;
    int  iTemperature;
    int  iStartMode;
    int  iRealDummy;
    int SoakTime;                      // 23
    int JamSoakTime;                   // 24
    int InitialWaitTime;               // 25
    int CollingTime;                   // 26
    bool bPasswordCanInputByMouse;
    int  iJamCount[3];
    int  iErrorCount[10];                                                       // ben 20110721 //
    int  iTransferPos;
    int  iShuttlePos;
    int  iAutoPos;
    bool bCynStatus[200];
    bool bSwStatus[100];
    int iMaxContactCount;
    long SystemTimeRecord[stTotalCnt];
    AnsiString SystemTimeRecordName[stTotalCnt];

//    int iTestRunMode;

} LAST_GENERAL_SET;
//-------------------------------------------------------------------------
typedef struct
{
    int     iCategData          [TEST_MAX_BIN];
    int     IfError;
    bool    bStackDefFailCate   [eTrayCount];

    bool    bCategoryFail           [eTrayCount];                               //Sam 20171127 : 增加 BinTray Warning Count
    int     iCategoryFailCountLimit [eTrayCount];                               //Sam 20171127 : 增加 BinTray Warning Count
    int     iOpenBin;   //jou 2011-03-24 start : Auto Site Mapping
}SYSTEM_BIN_SELECT;
extern SYSTEM_BIN_SELECT BinSelect[2];
//-------------------------------------------------------------------------
typedef struct
{
    int RecordCT;
    char ID[30][30];
    char PassWord[30][30];
    int  Level[30];
}PASS_WORD;
extern PASS_WORD USER;
//-------------------------------------------------------------------------
typedef struct
{
    bool iTemperture;
    int  iSoakTime;
    bool OnOffLine;
    bool RealDummy;
    int  iLoad;
    int  iTotal;
    int  iAuto[3];
    int  iFix[3];
    char szRunStatus[24];
    int  iUPH;
    int  iTestCT[8];
    int  iHeadPass[8];
    int  iYieldChart[8][25]; // min  unit
    int  iYieldHour[25];  // min  unit
    int  iYieldMin[25];   // min  unit
    int  iUpdateInterval; // 幾分鐘更新一次
    int  iUpdateCount;
    int  iSystemStatus;   // {0:"HALT"},{1:"PAUSE"},{2:"HOMEING"},{3:"RUNNING"},{4:"HAETING"},{5:"LOCK"},{6:"EMG"},{7:"Motor Off"}
    bool bSiteEnable[8];
    int  iYieldChartByCount[8][25]; // min  unit
    int  iYieldCount[25];  // min  unit
    bool LotStart;


}RUN_INFO;
extern RUN_INFO RunInfo;
extern RUN_INFO RunInfo2;
//-------------------------------------------------------------------------

bool WriteLastDataFile();
bool ReadLastDataFile();
bool WriteData(char *cFName,char *ptr,int size);
bool ReadData(char *cFName,char *ptr,int size);
bool CheckFileExist(char *cFName);
void ClearLastSet();
void SavePassword();
void ReadPassword();
void CheckLastData();
bool CheckFileCanAccess(char *cFName);
void ReadLastDataIni();                                                         //讀取關閉前最後設定參數
void WriteLastDataIni();
void UpdateAllParameter();                                                      // ben add 20110803 //
void CustomerFunctionSelect();                                                  //客戶功能選擇區
AnsiString GetTotalQuantityAutoPercent();
AnsiString GetTotalQuantityMagPercent();
#endif

