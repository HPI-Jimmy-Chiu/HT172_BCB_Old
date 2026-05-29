//==============================================================================

#ifndef cmydefH
#define cmydefH
//==============================================================================
#include "myswitch.h"
#include "handlerlog.h"

//-------------------------------------------------------------------------
//for HT-172
//-------------------------------------------------------------------------
#define PNP_DONE         0 // the order is finished
#define PNP_DOING        1 // the order is doing

#define MAX_ARM_Row 4
#define MAX_ARM_Col 2

#define MAX_TRAY_Row 40
#define MAX_TRAY_Col 20
//-------------------------------------------------------------------------
#define Tester_Site_Number 8
#define Tester_SiteMap_Row 2
#define Su_iTrayCount 6
//#define Su_iBinCount 2
#define CCD_iBin 1

#define defPi                            3.14159
#define defAngleArgument                 0.01745
#define defImW                        1000
#define defImH                        1000
#define MapWidth                       500
#define MapHeigh                       500
#define MapNullIC                        0
#define MapHasIC                         1
// -----------------------------------------------------------------------------
#define defPickTgrPos1                    0                                     //EjectionTable X 由Syn M314 Card 1 Axis X 輸出
#define defPickTgrPos2                    1                                     //EjectionTable y 由Syn M314 Card 1 Axis Y 輸出
#define defPickCmpCh1                     0                                     //Trigger channel 1
#define defPickCmpCh2                     1                                     //Trigger channel 2
#define defPickCX                       320
#define defPickCY                       240
#define defTriggerSpace                 100
// -----------------------------------------------------------------------------
#define TrayStartX                      3545
#define TrayStartY                      3750

#define CarryTrayPitchX                 6500
#define CarryTrayPitchY                 6000

#define InTrayWidth                     5080
#define InTrayHeight                    5080

#define iControlPanelMode               1
// -----------------------------------------------------------------------------

#define Auto_Bin_Row  4
#define Auto_Bin_Col  5

#define AutoBin_Count 20

// -----------------------------------------------------------------------------
#define FourInchTray                    5500

//#define Pnp2SafePos                     18670                                   //PnpSafePos
//#define Pnp3SafePos                     -65046                                  //PnpSafePos
//#define Pnp2SafePos                     19077                                   //PnpSafePos  HT1501A+
//#define Pnp3SafePos                     -64000                                  //PnpSafePos
#define Wafer_radius                    11000                                   //Sam 20170930 : Wafer 可工作範圍為 200mm 測試先用150mm來驗證(=15000條)//Sam 20170607 Wafer 半徑 = Wafer 12吋 半徑 = 6*2.54 =  152.4 mm
                                        //11000 理論 110mm(11000) 100mm(10000)

#define MAXSUCK 20
#define MAX_SUCK 25
#define PCC_MAX_AXIS 4
#define MAX_CYLINDER 200
#define MAX_INARM_SUCK  4

#define SUCKER_1x4

#define Gali_MaxAxis 4

#define MAXHEAD 2
#define MAX_X_ITEM 20
#define MAX_Y_ITEM 40
#define SERVER_MOTOR_POWER_ON_DELAY 4

#define _ICFallDownProcess

#define KitPitchX     1000
#define KitPitchY     1000
#define Z1_Z2_Normal    0
#define Z1Up_Z2Down     1
#define Z1Down_Z2Up     2
#define IndexIsBack     3
#define QUAD_SITE_X_PITCH  2000
#define QUAD_SITE_Y_PITCH  2000
#define QUAD_SITE_X_PITCH_ForGLink  1250

#define SITE_X_PITCH_16  2500
#define SITE_Y_PITCH_16  2000

#define QUAD_SITE_Y_PITCH_9042  3000

#define KIT_PITCH     8000

#define INARM_Y_PITCH 6000
#define _5BitBit2ch     0
#define _5Binary2ch     1
#define _5BitBit4ch     2
#define _5Binary4ch     3
#define _10BitBit2Ch    4
#define _3Binary4ch     5
#define _5BitBit2chDutReset 6
#define _5Binary2chDutReset 7
#define _5Binary2chDualStart 8
#define _5BitBit2chDualStart 9
#define _5BitBinaryWithL_DutReset 10
#define _5BitBit1StartWithDut 11
#define _5BitBinary1StartWithDut 12
#define _5BitBitStart2AsDoubleControl 13
#define _5BitBinary1StartWithDutOff 14

#define WRITE_MEMO
#define iHT9082InArmPitch 10100

#define DUAL_SITE 0
#define QUAL_SITE 1

#define TTL_MODE  0
#define GPIB_MODE 1
#define RS232_MODE 2

#define OFF_LINE  0
#define ON_LINE   1

#define MotionnetIO_L112  0
#define MotionnetIO_MN200 1

#define MotionCard_SYN      0
#define MotionCard_Contec   1
#define  MAX_MOTOR_TEST_COUNT 50                                                //推Tray 機構改為馬達 40->50
#define TEST_MAX_BIN 999

enum eTrayName                  //使用的Harware Bin數量與名稱
    {
        eBinNotUse      = 0,     //Bin未使用的位置
        eAuto1          = 1,
        eAuto2          = 2,
        eAuto3          = 3,
        eAuto4          = 4,
        eAuto5          = 5,
        eAuto6          = 6,
        eAuto7          = 7,
        eAuto8          = 8,
        eAuto9          = 9,
        eAuto10         =10,
        eAuto11         =11,
        eAuto12         =12,
        eAuto13         =13,
        eAuto14         =14,
        eAuto15         =15,
        eAuto16         =16,
        eAuto17         =17,
        eAuto18         =18,
        eAuto19         =19,
        eAuto20         =20,
        eFix1           =21,
        eFix2           =22,
        eMag1_01        =23,
        eMag1_02        =24,
        eMag1_03        =25,
        eMag1_04        =26,
        eMag1_05        =27,
        eMag1_06        =28,
        eMag1_07        =29,
        eMag1_08        =30,
        eMag1_09        =31,
        eMag1_10        =32,
        eMag1_11        =33,
        eMag1_12        =34,
        eMag1_13        =35,
        eMag1_14        =36,
        eMag1_15        =37,
        eMag1_16        =38,
        eMag1_17        =39,
        eMag1_18        =40,
        eMag1_19        =41,
        eMag1_20        =42,
        eMag1_21        =43,
        eMag1_22        =44,
        eMag1_23        =45,
        eMag1_24        =46,
        eMag1_25        =47,
        eMag1_26        =48,
        eMag1_27        =49,
        eMag2_01        =50,
        eMag2_02        =51,
        eMag2_03        =52,
        eMag2_04        =53,
        eMag2_05        =54,
        eMag2_06        =55,
        eMag2_07        =56,
        eMag2_08        =57,
        eMag2_09        =58,
        eMag2_10        =59,
        eMag2_11        =60,
        eMag2_12        =61,
        eMag2_13        =62,
        eMag2_14        =63,
        eMag2_15        =64,
        eMag2_16        =65,
        eMag2_17        =66,
        eMag2_18        =67,
        eMag2_19        =68,
        eMag2_20        =69,
        eMag2_21        =70,
        eMag2_22        =71,
        eMag2_23        =72,
        eMag2_24        =73,
        eMag2_25        =74,
        eMag2_26        =75,
        eMag2_27        =76,
        eMag3_01        =77,
        eMag3_02        =78,
        eMag3_03        =79,
        eMag3_04        =80,
        eMag3_05        =81,
        eMag3_06        =82,
        eMag3_07        =83,
        eMag3_08        =84,
        eMag3_09        =85,
        eMag3_10        =86,
        eMag3_11        =87,
        eMag3_12        =88,
        eMag3_13        =89,
        eMag3_14        =90,
        eMag3_15        =91,
        eMag3_16        =92,
        eMag3_17        =93,
        eMag3_18        =94,
        eMag3_19        =95,
        eMag3_20        =96,
        eMag3_21        =97,
        eMag3_22        =98,
        eMag3_23        =99,
        eMag3_24        =100,
        eMag3_25        =101,
        eMag3_26        =102,
        eMag3_27        =103,
        eTrayCount
    };

enum eTrackRowName
    {
        eTrackNotUse    =0,
        eTrack1         =1,
        eTrack2         =2,
        eTrack3         =3,
        eTrack4         =4,
        eTrackMgz1      =5,
        eFixRow1        =6,
        eFixRow2        =7,
        eTrackMgz2      =8,
        eTrackMgz3      =9,
        eTrackCount
    };
enum eAutoAreaName
    {
        eAAN_Auto01        = 0,
        eAAN_Auto02        = 1,
        eAAN_Auto03        = 2,
        eAAN_Auto04        = 3,
        eAAN_Auto05        = 4,
        eAAN_Auto06        = 5,
        eAAN_Auto07        = 6,
        eAAN_Auto08        = 7,
        eAAN_Auto09        = 8,
        eAAN_Auto10        = 9,
        eAAN_Auto11        =10,
        eAAN_Auto12        =11,
        eAAN_Auto13        =12,
        eAAN_Auto14        =13,
        eAAN_Auto15        =14,
        eAAN_Auto16        =15,
        eAAN_Auto17        =16,
        eAAN_Auto18        =17,
        eAAN_Auto19        =18,
        eAAN_Auto20        =19,
        eAAN_AutoTotal
    };

enum eMagColName
    {
        eMag01        = 0,
        eMag02        = 1,
        eMag03        = 2,
        eMag04        = 3,
        eMag05        = 4,
        eMag06        = 5,
        eMag07        = 6,
        eMag08        = 7,
        eMag09        = 8,
        eMag10        = 9,
        eMag11        =10,
        eMag12        =11,
        eMag13        =12,
        eMag14        =13,
        eMag15        =14,
        eMag16        =15,
        eMag17        =16,
        eMag18        =17,
        eMag19        =18,
        eMag20        =19,
        eMag21        =20,
        eMag22        =21,
        eMag23        =22,
        eMag24        =23,
        eMag25        =24,
        eMag26        =25,
        eMag27        =26,
        eMagTotal
    };

enum eMagazine
{
    eMagazine1=0,
    eMagazine2,
    eMagazine3,
    eMGZTotal
};

enum eTrackColName
    {
        eTrackCol1      =0,
        eTrackCol2      =1,
        eTrackCol3      =2,
        eTrackCol4      =3,
        eTrackCol5      =4,
        eTrackColTotal
    };

enum eNozzleForceCT{ePnPX       =0,                                             //Sam 20170907 : 增加吸嘴壓的次數
                    ePnP1X      =1,
                    eNozzleCount
                   };

extern int CUSTOMER_CODE;
extern int USE_SECS_GEM;                                                        //Steven 20240619 : Add for SECS/GEM
extern int USE_PUSH_TRAY;
extern int AUTO_TRAY_CNT;
extern int FIX_TRAY_CNT;
extern int MAGAZINE_CNT;
extern int MAGAZINE_LAYER;

extern const int eNoTray ;
extern const int eHasTray;

//Steven 20100629 Start: 紀錄修改項目
extern TMyLog myLog;
extern AnsiString asLogPath;
extern AnsiString asUser;

//Steven 20100629 End

extern int iPickerNo;
extern int iSuckPicker;

extern const int CloseSiteIC;                       //定義CloseSite=5

extern bool flagBufferNoTray;                       //通知Loader Buffer沒有Tray;
extern bool flagLoaderReady;                        //Loader 通知其它Module Working tray己準備完成
extern bool flagIndexReady;                         //Index 通知其它Module 己準備完成
extern bool flagIndex_In;                           //InArm Module 通知其它Index 己入料完成
extern bool flagIndex_Test;                         //Tester Module通知其它Index 己測試完成
extern bool flagIndex_Out;                          //OutArm Module 通知其它Index 己取料完成
extern bool flagIndex_Vision;                       //Vision Module 通知其它Index 己取料完成
extern bool flagLoaderEmptyReady;                   //Loader 通知其它Module Empty tray己準備完成
extern bool flagNotificationNoTray;                 //Unload 通知 OutAlarm 有Auto 沒Tray
extern bool NeedTransportEmptyTray;                 //需要載入一盤空Tray
extern bool NeedTransportWorkTray;                  //需要載入一盤新Tray
extern bool bOnecycleTrayFeed;

extern bool bSendWACCDSendStart;

extern bool flagVision;                             //InArm --> Vision 通知
extern bool flagOutArmCatchBusy;                    //當Catch動作執行中台車不動
extern bool flagIndexHome;                          //通知Index 回Home修正Pos誤差
extern bool flagWorkEmpty;                          //Working Area 退出的空盤,要收到空Tray區
extern bool flagWorkEmpty_1;                        // 回Home時就有Tray無法判斷方向,所以要收到空Tray區
extern bool flagTrayFeed;
extern bool flagLoaderTrayEnd;                      //直接退Tray
extern bool flagTrayEnd;                            //直接退Tray
extern bool flagOneCycleTrayEnd;                    //直接退Tray


extern AnsiString sVersion;                         //軟體版本
extern bool VisionBusy;                             //Vision檢測中
extern bool VisionGetResultFinish;                  //Vision分析完成
extern bool UseAutoBin[6];                          //Auto Bin 有無使用
extern int iErrorBinTray;                           //Error Bin 是那一Auto Tray
extern int iCCDFailBinTray;                         //CCD Fail　是那一Auto Tray
extern bool bCloseSite;                             //有Close Site = true
extern bool bFirstRun;                              //更換一批新的產品
extern AnsiString sWorkFile;                        //生產檔名
extern bool bClientConnect;                         //與CCD 是否連線
extern int iNowIndex ;                              //Index 站目前所在位置
extern int ArmZDownSpeed;
extern bool bOCRmessage;
extern bool bFirstDevice;
extern bool bMPBAEnterflag;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

extern unsigned int MyLongMask[32];
extern byte MyBitMask[8];


extern const int GaliPosOffSet;
extern const int MotErrPos;

extern const int CY_PUSH;
extern const int CY_POP;

extern const int EMPTY_IC;
extern const int NULL_IC;
extern const int HAS_IC;
//extern const int HOTED;
extern const int HAS_PASS_IC;
extern const int HAS_NULL_IC;
//extern const int HAS_COVER;
//extern const int HAS_MOLD_IC;
extern const int HAS_RING_HAS_IC;
extern const int HAS_RING_NO_IC;
extern const int HAS_FAIL_IC;

extern const int START_TEST       ;
extern const int TEST_PASS        ;
extern const int TEST_FAIL        ;
extern const int NULL_IC_TEST     ;
extern const int HAS_NULL_IC_TEST ;
extern const int TEST_FAIL1       ;
extern const int TEST_FAIL2       ;
extern const int TEST_FAIL3       ;

enum eSystemErrType
{
    SysErr_SystemPower                  =0,
    SysErr_MotorPower                   ,
    SysErr_AirNotEnough                 ,
    SysErr_VacuumNotEnough              ,
    SysErr_SafeDoor                     ,
    SysErr_EMG                          ,
    SysErr_Vaccum                       ,
    SysErr_Interface                    ,
    SysErr_InterfaceSetErr              ,
    SysErr_ContinuousFail               ,
//    SysMessage_OneCycleFinish           ,
//    SysErr_CleanOutFinish               ,
    SysErr_IonFanAlarm                  ,
    SysErr_IonFanLevelAlarm             ,
    SysErr_IonBarrierAlarm              ,
    SysErr_IonBarrierLevelAlarm         ,
    SysErr_IonBarrierConditionAlarm     ,
    SysErr_OverMaxFailCount             ,
    SysErr_OverArmSiteMaxFailCount
};

extern const int TRAY_MISS;
extern const int TRAY_MORE;
extern const int TRAY_FULL;
extern const int TRAY_EMPTY;
extern const int TRAY_TIME_OUT;

extern const int Vaccum_On;
extern const int Vaccum_Off;
extern const int Vaccum_FallDown;
extern const int Vaccum_Initial_On;
extern const int Vaccum_Initial_Off;
extern const int TRAY_SUCK_ERROR;
extern const int TRAY_PLACE_ERROR;
// -----------------------------------------------------------------------------
extern const int HAS_TRAY   ;
extern const int REALLY     ;
extern const int DUMMY      ;
// -----------------------------------------------------------------------------
extern const int TYPE_A;
extern const int TYPE_B;
// -----------------------------------------------------------------------------
extern bool InitialOK;
//extern bool SystemStart;
extern bool fAllMotorHome;
extern bool SoftStart;
extern bool SoftStop;
extern Word SystemHour,SystemMin,SystemSec,SystemMSec;
extern Word SystemYear,SystemMonth,SystemDate;
extern Word SystemYearYesterday,SystemMonthYesterday,SystemDateYesterday;
//extern char CurrentDir[256];
// -----------------------------------------------------------------------------
extern char Com2Buffer[1024];
extern bool Com2ReceiveOK;
extern bool OneCycleArm1;
extern bool OneCycleArm2;
extern bool OneCycleShutte1;
extern bool OneCycleShutte2;
extern bool OneCycleOutArm;
extern bool CleanOutArm1;
extern bool CleanOutArm2;
extern bool CleanOutShutte1;
extern bool CleanOutShutte2;
extern bool CleanOutOutArm;
extern bool CleanOutTrayArm;
extern int  iOneCycle;
extern int  iCleanOut;
extern int  iTrayFeed;
extern int  iHome;
extern int  iReset;
extern int  iAlarmReset;
extern int  iCatchTrayControlManual;
extern int  OutArmPlaceToManual;
extern int  iHotLineChange;
extern int SearchLine;
extern bool fGetHotWaitTime;
extern bool fPlate1Change;
extern bool fPlate2Change;
extern bool fTestSoucketSearch[2];
extern const int  K_RETRY    ;
extern const int  K_SKIP     ;
extern const int  K_CLEAN_OUT;
extern const int  K_TRAY_FEED;
extern const int  K_TRAY_END ;
extern const int  K_HOME     ;

extern const int  N_INTEGER   ; //只有整數
extern const int  N_DOUBLE    ; //只有浮點數
extern const int  N_NO_SYMBOL ; //沒有特殊符號
extern const int  N_PASSWORD  ; //密碼文
extern const int  N_NO_SPACE  ; //無空白鍵
extern const int  N_UPPERCASE ; //大寫優先
extern const int  N_NO_NUM_PAD; //不需要數字鍵
extern const int  N_NUM_PAD   ; //只需要數字鍵
extern const int  N_PORT      ; //通訊埠
extern const int  N_IP_ADDR   ; //IP位置

extern bool ManualTrayChange[3];
extern bool CheckSystemPower;
extern DWORD Cycle1,Cycle1_B,Cycle2,Cycle2_B;
//===================================================================
extern bool fHeaterOK;
extern int  iHeaterWait;
extern int  iHeaterCooling;
extern bool FlushFlag;
extern int RunState;
extern bool bLampPowerOff  ;
extern bool bLampPowerOn   ;
extern bool bLampReset     ;
extern bool bLampPause     ;
extern bool bLampHome      ;
extern bool bLampStart     ;
extern bool bLampOneCycle  ;
extern bool bLampRetry     ;
extern bool bLampSkip      ;
extern bool bLampCleanOut  ;
extern bool bLampTrayFeed  ;
extern bool bLampTrayEnd   ;
extern bool bLampAlarmReset;
extern bool bLampCoverOpenFR;
extern bool bLampCoverOpenFL;
extern bool bLampCoverOpenRR;
extern bool bLampCoverOpenRL;
extern bool bLampManualSetp;
extern bool bLampManualStart;
extern bool bLampFKCoverOpen;
extern bool bLampRKCoverOpen;
extern bool bMoveInArm1ZToPlate1Safe;
extern bool bMoveInArm1ZToPlate2Safe;
extern bool bMoveInArm2ZToPlate1Safe;
extern bool bMoveOutArm1ToFixSafe;
extern bool bFrontPadActive;
extern int iResetTask;
extern int iTrayFeedTask;
extern bool fPlate1Load;
extern bool fPlate1UnLoad;
extern bool fPlate2Load;
extern bool fPlate2UnLoad;
extern int  iCatchTrayCatchColor1;
extern int  MotorPowerOnDelay;
extern bool bMotorPowerState;
extern bool bSystemNGHappan;
extern bool bAlarmBuzzer;
extern int iMostNearHeaterOKSecond;
extern bool bPlate1PlaceNextRow;
extern bool bPlate2PlaceNextRow;
extern bool bButtonManualStep;
extern bool First_Test;
extern bool Led_7_Step;
extern int  i_7_Step_Led;
extern bool Now_Status_IsOneCycle;
extern int iX_Plate1_PlacePos;
extern int iY_Plate1_PlacePos;
extern int iX_Plate1_PickPos;
extern int iY_Plate1_PickPos;
extern bool bAll_Ready_Finish_Pick_HotPlate;
extern int iPickFromPlate;
extern bool PickHotPlate;
extern int iReadTorueCT;
extern int TestZ1_Pos;
extern int TestZ2_Pos;
extern bool bTray_IsEmpty;
extern int IndexStatus;
extern bool MotAlarmFlag;
extern bool Soft_Ready;
extern bool bNeedArmZHome;
extern bool ProgramFlag;
extern bool bQuardSiteMode;
extern int Z1Safe;
extern int XShiftF;
extern int Z1Up;
extern int Z2DownSafe;
extern int Z2Down;
extern int Z2Safe;
extern int XShiftR;
extern int Z1DownSafe;
extern int Z2Up;
extern int Z1Down;
extern int XShiftFTMY2;
extern int XShiftMTRY1;
extern bool bInArmZNeedHome[];
extern bool bOutArmZNeedHome[];
extern int iInArmPitch;
extern int iOutArmPitch;
extern bool bCanNotDisableOneCycle;
extern bool bGetMotTableData;
extern int iReceiveEmptyTray;
extern bool fEmptyCanSupplyNewTray;
extern int iReceiveColorTray;
extern bool fColorCanSupplyNewTray;
extern int MachineStatus;
extern const int ISInitMode;
extern const int ISRunMode;
extern bool Arm1PreSuckIc;
extern bool Arm2PreSuckIc;
extern long FirstCount;
extern int iGali_VsSpeed;
extern int iGali_SpSpeed;
extern bool bTTL_StartDigital[4];





extern const int LED_Running;
extern const int LED_ErrJam;
extern const int LED_Pause;
extern const int LED_Message;
extern const int LED_Heating;
extern const int LED_Homeing;
extern int iTestZAxisSpeed;
extern bool bQualSiteQuickMode;
extern bool bInstallRotate;
extern AnsiString sSuperVisorString;

typedef struct SuckPortDataClass
{
    int OnPort;
    int OnBit;
    int OnType;
    int OffPort;
    int OffBit;
    int OffType;
    int SensorPort;
    int SensorBit;
    int SensorType;
}SuckPortData;
typedef struct RESERVE_EMPTY_POINT
{ //Eliot 2008_05_19
    bool bHasData;
    int iXPoint;
    int iYPoint;
}RESERVE_EMPTY_POINT;
extern bool EP_Install;
extern int iNowMaxFailBinCountPerOneLot[15];
extern int iIndexArmMaxFailCountBySite[2][8];


extern bool bOffBuzzer;                                                         // ben 20110706 - s //
extern bool bMessageTimer1Check;
extern bool bNoteTimer1Check;
extern bool bCleanOut;
extern bool FixTubeChange[6];
// --------------------------------------------------------------------------
extern const int Tube_FULL;
extern const int Tube_EMPTY;
extern const int Tube_MISS;
extern const int Tube_MORE;
extern const int Tube_TIME_OUT;
//---------------------------------------------------------------------------
extern bool bDelayCheckLevelSetup;                                              // delay update levelsetup //
extern bool bLoadLevelSetupToMap;                                               // map //
//---------------------------------------------------------------------------
extern const bool Server_Off;                                                    // server on off //
extern const bool Server_On;                                                     // server on off //
//---------------------------------------------------------------------------
extern bool bHot1Ready;
extern bool bHot2Ready;
extern bool bMessageAlarm;
extern bool bTesterOnLine;
extern bool bDoChamberTap;
extern bool bNeedChangeFix1;
extern bool bNeedChangeFix2;
extern bool bNeedChangeFix3;
extern bool bNeedChangeFix4;
extern bool bNeedChangeFix5;
extern bool bNeedChangeFix6;
extern bool bChangeFix1OK;
extern bool bChangeFix2OK;
extern bool bChangeFix3OK;
extern bool bChangeFix4OK;
extern bool bChangeFix5OK;
extern bool bChangeFix6OK;

extern AnsiString asNowUserID;
extern AnsiString asNowUserLevel;
extern bool bMotorHomePowerOn;
extern bool bHomeByStart;
extern bool bHasPushEMG;
//extern bool bOpenFinish; //kevin 20130409  開蓋完成

extern bool UseCleanOut;           //20131120 wei 除了真的沒卡匣之外其餘時間不能執行CleanOut

//Motion Mode----------------------------------------------------------------------
extern int MOTION_CARD_TYPE;                    //Brian 20121015
extern int IO_CARD_TYPE;                        //Brian 20121015

extern char RunStatus;
extern int  InLoaderCount;
extern int  OutLoaderCount;
//extern bool bOpenTeach;

extern int iContactCount;
extern int iLoaderCount[2];

extern bool bSortArm1OneCycleFinish    ;                                        //Steven 20240707 : Fixed for one cycle
extern bool bSortArm2OneCycleFinish    ;
extern bool bSortStoreArmOneCycleFinish;
extern bool bSortMagOneCycleFinish[eMGZTotal]  ;
extern bool bTrackOneCycleFinish[eTrackCount];

extern bool bFlagMgzCleanoutFinish     ;
extern bool bFlagEJCleanoutFinish      ;
extern bool bFlagPnpCleanoutFinish     ;
extern bool bFlagPnp1CleanoutFinish    ;
extern bool bFlagAuto1CleanoutFinish   ;
extern bool bFlagAuto2CleanoutFinish   ;
extern bool bFlagTube1CleanoutFinish   ;
extern bool bFlagTube2CleanoutFinish   ;
extern bool bFlagCatchYCleanoutFinish  ;

extern bool bEmpty1TrayFeedFinish;

extern int iBinSortCount;
extern bool bHT1501Aflag               ;
extern bool bTopCCDLearning            ;
extern bool bDownCCDLearning           ;
extern bool bAudioCCDLearning          ;
extern bool bMagNeedAutoLoadTray       ;    //Sam 20240710 : 修正 OneCyce 異常
extern bool bInitialStartCheckMagLayer ;    //Sam 20240710 : 修正 OneCyce 異常
//extern bool bPnpZToSafePos             ;                                      //Sam 20171024 : 精簡掉

//extern int iBlockXMovePitch            ;
//extern int iBlockYMovePitch            ;

extern int  MachineTypeChoice;                                                  //0:HT1501 1:HT1501A 2:HT1501A_Plus
extern AnsiString asModel;                                                     //Sam 20170802 : 增加機型 HT1501、HT1501A、HT1501A+
extern AnsiString asSerialNo;                                                  //Sam 20170802 : 增加機號 FML746、FML999、HML823、HML824
extern AnsiString asFactory;                                                   //Sam 20170802 : 增加廠號 T5、T6
extern AnsiString asHandlerID;                                                 //Sam 20170720 : 增加 HandlerID

extern int  Pnp2SafePos;                                                        //HT1501A 和 HT1501A+ 兩支 Arm 的安全位置使用，原本是 #define Pnp2SafePos 改為用 ini 檔變數讀取
extern int  Pnp3SafePos;
//extern const int iChecZ2EnablePos;                                            //Sam 20170802 : 改機構用不到了  //Sam 20170710 (jou) : 小於此數值才需要檢查 Z2 位置是否被卡住
extern bool bFlagAuto1TrayFeedFinish   ;                                        //Frank 20170828 : 修正 Tray Feed Hang up 問題
extern bool bFlagAuto2TrayFeedFinish   ;                                        //Frank 20170828 : 修正 Tray Feed Hang up 問題
extern const int  HT1501                            ;                                        //Sam 20171019 : 增加機型 Const
extern const int  HT1501A                           ;
extern const int  HT1501APlus                       ;

//extern int iSortTrayIndex[eTrayCount];
extern int iSortArmXYMoveTask;
extern int iMagArmXYMoveTask;
extern int iTrayArmXYMoveTask;
extern bool bSystemClose;
extern bool bNeedLoadTrayToTransfer;
extern bool bNeedPickTrayToMagTop;
extern bool bNeedLoadTrayToMagEmpty;

extern bool bAseStart;                                                          //kevin 20150427
extern bool bAseHome;                                                           //kevin 20150427
extern bool bAsePause;                                                          //kevin 20150427
extern bool bAseReset;                                                          //kevin 20150427
extern bool bAseCleanOut;                                                       //kevin 20150427
extern bool bAseSKIP;                                                           //kevin 20150427
extern bool bAseSKIP;                                                           //kevin 20150427
extern bool bAseRetry;                                                          //kevin 20150427
extern bool bAseOneCycle;                                                       //kevin 20150427
extern bool bAseTrayEnd;                                                        //kevin 20150427
extern bool bAseTrayFeed;                                                       //kevin 20150427
extern bool bTSTART;                                                            //kevin 20160303
extern bool bSTEP;                                                              //kevin 20160303

extern int  iAseHome;                                                           //kevin 20150925
extern int  iAseCleanOut;                                                       //kevin 20150925
extern int  iAseOneCycle;                                                       //kevin 20150925
extern int  iAseAlarmReset;                                                     //kevin 20150925
extern int  iAseTrayEnd;                                                        //kevin 20150925
extern int  iAseTrayFeed;                                                       //kevin 20150925

extern bool bWaiteKeepRespond;                                                  //kevin 20231215 等待eKeep 回應
extern AnsiString ASE_ReturnState;                                              //kevin 20150925  回傳目前機台狀況

extern AnsiString sLoaderID;
extern AnsiString sEmpty1ID;
extern AnsiString sEmpty2ID;

extern bool bPauseSortMotor;
extern bool bPauseMagMotor;
extern bool Enable_PLCSafety_IO;                                                //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
extern int iPLCSafetyVer;                                                       //joe 20220406.01 debug safe plc

extern bool bFlagLoaderCleanoutFinish ;
extern bool bFlagEmpty1CleanoutFinish ;
extern bool bFlagEmpty2CleanoutFinish ;
extern bool bFlagAutoArmCleanoutFinish;
extern bool bFlagMagArmCleanoutFinish ;
extern bool bFlagTrackCleanoutFinish  ;
extern bool bFlagAutoBinCleanoutFinish;

extern bool bAlarmAfterPreAlarm;                                                //Ifor 20170906 add 避免 PreAlarm -> Alarm -> SECS GEM Alarm 同時發生造成當機問題
extern bool bSECSGEM_NoteAlarm;                                                 //Ifor 20170616 (wei) add S10F3 後跳出Note Alarm旗標
extern bool bSECSGEMAlarm;                                                      //wei 20150817 S10F3 Alarm Reset按鍵

extern bool bStoreArmFin;                                                       //KenHsieh 20240514 : Tray Arm Tray Feed Finish

extern int iTestBinCount;
extern int IN_OUT_ARM_Y_PITCH_MIN;
extern int IN_OUT_ARM_Y_PITCH_MAX;
extern int IN_OUT_ARM_X_PITCH_MIN;
extern int IN_OUT_ARM_X_PITCH_MAX;

extern int iHasSkipTrayDataNeedHold;
extern bool bCalculatePauseTime;    //Sam 20240526 : 新增每盤的UPH
extern int iUPH_PickCnt;
extern TDateTime tUPH_StartTime;
extern TDateTime tUPH_PauseStartTime;
extern TDateTime tUPH_PauseTime;
extern TDateTime tUPH_EndTime;
extern int iUPH_LoaderCount;
extern bool bHandlerPause;
extern int iHandlerStartCount;

extern bool bSortArmTrayZFixSpeed;  //Sam 20240619 : 速度獨立設定
extern bool bSortArmNeedHome;
extern bool bSortArmYNeedHome;
extern bool bMagArmNeedHome;
extern AnsiString asUserID          ;
extern AnsiString asPassword        ;
extern AnsiString asHost            ;
extern AnsiString asFTPPath         ;
extern AnsiString asFTPPathUpload   ;
extern AnsiString asTrayPathUpload  ;
extern bool bIsInitLoadEmptyTrayToMag;
extern int iByLotTrayCnt[eTrayCount];   //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞
extern bool Enable_MAG1;
extern bool Enable_MAG2;
extern bool Enable_MAG3;

extern AnsiString asNetDisk;
extern AnsiString sMNetLogPath;
extern bool bSortArm2UseSuck[MAX_ARM_Row][MAX_ARM_Col]  ;
extern bool bPhysicalStart;             //JerryYang 20250106 : Run check
extern bool bOLPLoadMap;
extern bool bStartCallLot_StartExe;
extern bool bPadInterfaceAndTrayStepCommPort;
extern AnsiString sLotAuto1Info;        //Sam 20250724 : 新增 AutoSetBin By Lot Auto Info
extern AnsiString sLotAuto2Info;
extern AnsiString sLotAuto3Info;
#endif
