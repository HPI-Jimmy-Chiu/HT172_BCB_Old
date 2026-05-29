//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop
//#include "cmydef.h"
#pragma package(smart_init)

//-------------------------------------------------------------------------
//for HT-172
//-------------------------------------------------------------------------
int CUSTOMER_CODE   =0;
int USE_SECS_GEM    =0;                                                         //Steven 20240619 : Add for SECS/GEM
int AUTO_TRAY_CNT   =20;
int MAGAZINE_CNT    =1;
int FIX_TRAY_CNT    =2;
int MAGAZINE_LAYER  =27;
int USE_PUSH_TRAY   =0;
const int eNoTray =0;
const int eHasTray=1;


const int CloseSiteIC = 5;
TMyLog myLog;                                           //Steven 20100629 : 紀錄修改項目
AnsiString asLogPath="D:\\HT-172_Log\\ChangeLog";    //Steven 20100907 : 換一下位置
AnsiString asUser="";

int iPickerNo=6;
int iSuckPicker=1;

bool flagBufferNoTray =false;
bool flagLoaderReady = false;
bool flagIndexReady = true;
bool flagIndex_In = false;
bool flagIndex_Test = true;
bool flagIndex_Out = true;
bool flagIndex_Vision =false;
bool flagLoaderEmptyReady = false;
bool flagNotificationNoTray = false;
bool bOnecycleTrayFeed=false;

bool flagTrayFeed=false;
bool flagOneCycleTrayEnd=false;
bool flagLoaderTrayEnd=false;
bool flagTrayEnd=false;

bool VisionBusy =false;
bool VisionGetResultFinish = false;
bool NeedTransportEmptyTray = false;                            //需要載入一盤空Tray
bool NeedTransportWorkTray = false;                             //需要載入一盤新Tray

AnsiString sVersion;
bool UseAutoBin[6];                                             //根據bin setup 去使用Auto1~6
int iErrorBinTray;
int iCCDFailBinTray;
bool bCloseSite =false;
bool bFirstRun =true;
AnsiString sWorkFile;
bool bClientConnect =false;
int iNowIndex ;
int ArmZDownSpeed;
bool bOCRmessage=false;
bool bFirstDevice =false;
bool bMPBAEnterflag=false;
//-------------------------------------------------------------------------
bool bFlagLoaderCleanoutFinish=false ;
bool bFlagEmpty1CleanoutFinish=false ;
bool bFlagEmpty2CleanoutFinish=false ;
bool bFlagAutoArmCleanoutFinish=false;
bool bFlagMagArmCleanoutFinish=false ;
bool bFlagTrackCleanoutFinish=false  ;
bool bFlagAutoBinCleanoutFinish=false;

bool bAlarmAfterPreAlarm=false;                                                 //Ifor 20170906 add 避免 PreAlarm -> Alarm -> SECS GEM Alarm 同時發生造成當機問題
bool bSECSGEM_NoteAlarm=false;                                                  //Ifor 20170616 (wei) add S10F3 後跳出Note Alarm旗標
bool bSECSGEMAlarm=false;                                                       //wei 20150817 S10F3 Alarm Reset按鍵
bool bStoreArmFin=false;                                                        //KenHsieh 20240514 : Tray Arm Tray Feed Finish
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// all type program can use
unsigned int MyLongMask[32]=
    {0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,0x00000080,
     0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,0x00008000,
     0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,0x00800000,
     0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000};

byte MyBitMask[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

const int CY_PUSH=1;
const int CY_POP=2;

const int EMPTY_IC=-1;
const int NULL_IC=0;
const int HAS_IC=1;
const int HAS_NULL_IC=2;
const int HAS_PASS_IC=3;
//const int HOTED=4;
//const int HAS_COVER=4;
//const int HAS_MOLD_IC=5;
const int HAS_RING_HAS_IC=4;
const int HAS_RING_NO_IC=5;
const int HAS_FAIL_IC=6;

const int START_TEST       =1000;
const int TEST_PASS        =1001;
const int TEST_FAIL        =1002;
const int NULL_IC_TEST     =1002;
const int HAS_NULL_IC_TEST =1002;
const int TEST_FAIL1       =1003;
const int TEST_FAIL2       =1004;
const int TEST_FAIL3       =1005;

//---------------------------------------------------------------------------
const int GaliPosOffSet=10000;  //for new motor parameter
const int MotErrPos=6;
// --------------------------------------------------------------------------
const int Vaccum_On=0;
const int Vaccum_Off=1;
const int Vaccum_FallDown=2;
const int Vaccum_Initial_On=3;
const int Vaccum_Initial_Off=4;
// --------------------------------------------------------------------------
const int TRAY_FULL=0;
const int TRAY_EMPTY=1;
const int TRAY_MISS=2;
const int TRAY_MORE=3;
const int TRAY_TIME_OUT=4;
const int TRAY_SUCK_ERROR=5;
const int TRAY_PLACE_ERROR=6;
//---------------------------------------------------------------------------
const int REALLY     =2;
const int HAS_TRAY   =1;
const int DUMMY      =0;
// --------------------------------------------------------------------------
const int TYPE_A=1;
const int TYPE_B=0;
// --------------------------------------------------------------------------

const int SEND       =3000;
const int RECEIVE    =3001;
// --------------------------------------------------------------------------
bool InitialOK=false;
//bool SystemStart=false;
bool fAllMotorHome=false;
bool bOnlyVoiceMotorHome=false;
bool SoftStart=false;
bool SoftStop=false;
Word SystemHour=9999,SystemMin=9999,SystemSec=9999,SystemMSec=9999;
Word SystemYear=9999,SystemMonth=9999,SystemDate;
Word SystemYearYesterday=9999,SystemMonthYesterday=9999,SystemDateYesterday=9999;
//char CurrentDir[256];
// --------------------------------------------------------------------------
char Com2Buffer[1024];
bool Com2ReceiveOK=false;
bool OneCycleArm1=false;
bool OneCycleArm2=false;
bool OneCycleShutte1=false;
bool OneCycleShutte2=false;
bool OneCycleOutArm=false;
bool CleanOutArm1=false;
bool CleanOutArm2=false;
bool CleanOutShutte1=false;
bool CleanOutShutte2=false;
bool CleanOutOutArm=false;
bool CleanOutTrayArm=false;
int  iOneCycle=0;
int  iCleanOut=0;
int  iTrayFeed=0;
int  iHome=1;
int  iReset=0;
int  iAlarmReset=0;
int  iCatchTrayControlManual=0;
int  OutArmPlaceToManual=0;
int  iHotLineChange=0;
int SearchLine=0;
bool fGetHotWaitTime=false;
bool fPlate1Change=false;
bool fPlate2Change=false;
bool fTestSoucketSearch[2]={false,false};
const int  K_RETRY    =0x0001;
const int  K_SKIP     =0x0002;
const int  K_CLEAN_OUT=0x0004;
const int  K_TRAY_FEED=0x0008;
const int  K_TRAY_END =0x0010;
const int  K_HOME     =0x0040;

const int  N_INTEGER   =0x0001; //只有整數
const int  N_DOUBLE    =0x0002; //只有浮點數
const int  N_NO_SYMBOL =0x0004; //沒有特殊符號
const int  N_PASSWORD  =0x0008; //密碼文
const int  N_NO_SPACE  =0x0010; //無空白鍵
const int  N_UPPERCASE =0x0020; //大寫優先
const int  N_NO_NUM_PAD=0x0040; //不需要數字鍵
const int  N_NUM_PAD   =0x0080; //只需要數字鍵
const int  N_PORT      =0x0100; //通訊埠
const int  N_IP_ADDR   =0x0200; //IP位置

bool ManualTrayChange[3]={false,false,false};
bool CheckSystemPower=true;
DWORD Cycle1=9999,Cycle1_B=9999,Cycle2=9999,Cycle2_B=9999;

bool fHeaterOK=false;
int  iHeaterWait=0;
int  iHeaterCooling=0;
bool FlushFlag;
int RunState=-1;


bool bLampPowerOff  =false;
bool bLampPowerOn   =false;
bool bLampReset     =false;
bool bLampPause     =false;
bool bLampHome      =false;
bool bLampStart     =false;
bool bLampOneCycle  =false;
bool bLampRetry     =false;
bool bLampSkip      =false;
bool bLampCleanOut  =false;
bool bLampTrayFeed  =false;
bool bLampTrayEnd   =false;
bool bLampAlarmReset=false;
bool bLampCoverOpenFR=false;
bool bLampCoverOpenFL=false;
bool bLampCoverOpenRR=false;
bool bLampCoverOpenRL=false;
bool bLampManualSetp=false;
bool bLampManualStart=false;

bool bLampFKCoverOpen=false;
bool bLampRKCoverOpen=false;

bool bMoveInArm1ZToPlate1Safe=false;
bool bMoveInArm1ZToPlate2Safe=false;
bool bMoveInArm2ZToPlate1Safe=false;
bool bMoveOutArm1ToFixSafe=false;

bool bFrontPadActive=false;

int iResetTask=1;
int iTrayFeedTask=1;

bool fPlate1Load=false;
bool fPlate1UnLoad=false;
bool fPlate2Load=false;
bool fPlate2UnLoad=false;
int  iCatchTrayCatchColor1=0 ;

int  MotorPowerOnDelay=10;
bool bMotorPowerState=false;    // true:motor power is on
bool bSystemNGHappan=false;
bool bAlarmBuzzer=false;
int iMostNearHeaterOKSecond=0;


bool bPlate1PlaceNextRow=false;
bool bPlate2PlaceNextRow=false;
bool bButtonManualStep=false;
bool First_Test=true;
bool Led_7_Step=false;
int  i_7_Step_Led=1;
bool Now_Status_IsOneCycle=false;
int iX_Plate1_PlacePos=0;
int iY_Plate1_PlacePos=0;
int iX_Plate1_PickPos=0;
int iY_Plate1_PickPos=0;
bool bAll_Ready_Finish_Pick_HotPlate=true;
int iPickFromPlate=0;
bool PickHotPlate=false;
int iReadTorueCT;
int TestZ1_Pos=0;
int TestZ2_Pos=0;
bool bTray_IsEmpty=false;
int IndexStatus=0;
bool MotAlarmFlag=false;
bool Soft_Ready=false;
bool bNeedArmZHome=false;
bool ProgramFlag=false;

bool bQuardSiteMode=false;
int Z1Safe=0;;
int XShiftF=0;
int Z1Up=0;
int Z2DownSafe=0;
int Z2Down=0;

int Z2Safe=0;
int XShiftR=0;
int Z1DownSafe=0;
int Z2Up=0;
int Z1Down=0;
int XShiftFTMY2=0;
int XShiftMTRY1=0;
bool bInArmZNeedHome[]={false,false,false,false};
bool bOutArmZNeedHome[]={false,false,false,false};
int iInArmPitch=3900;
int iOutArmPitch=3900;
bool bCanNotDisableOneCycle=false;
bool bGetMotTableData=false;
int iReceiveEmptyTray=0;
int iReceiveColorTray=0;
bool fEmptyCanSupplyNewTray=true;
bool fColorCanSupplyNewTray=true;
//TMySwitch *TTL_StartData[4]={&HSys.Sw.SwStart0,&HSys.Sw.SwStart1,&HSys.Sw.SwStart2,&HSys.Sw.SwStart3};
//TMySwitch *TTL_ClearData[4]={&HSys.Sw.SwClear0,&HSys.Sw.SwClear1,&HSys.Sw.SwClear2,&HSys.Sw.SwClear3};
int MachineStatus=0;
const int ISInitMode=0;
const int ISRunMode=1;
bool Arm1PreSuckIc=false;
bool Arm2PreSuckIc=false;
long FirstCount=0;
int iGali_VsSpeed=0;
int iGali_SpSpeed=0;
bool bTTL_StartDigital[4]={false,false,false,false};


const int LED_Running=0;
const int LED_ErrJam=1;
const int LED_Pause=2;
const int LED_Message=3;
const int LED_Heating=4;
const int LED_Homeing=5;
int iTestZAxisSpeed;

bool bQualSiteQuickMode=false;


bool bInstallRotate=true;

AnsiString sSuperVisorString="16943420";

int iNowMaxFailBinCountPerOneLot[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int iIndexArmMaxFailCountBySite[2][8];

bool bOffBuzzer=false;                                                          // ben 20110706 - s //
bool bMessageTimer1Check=false;
bool bNoteTimer1Check=false;
bool bCleanOut=false;
bool FixTubeChange[6]={false,false,false,false,false,false};
// --------------------------------------------------------------------------
const int Tube_FULL=0;
const int Tube_EMPTY=1;
const int Tube_MISS=2;
const int Tube_MORE=3;
const int Tube_TIME_OUT=4;
//---------------------------------------------------------------------------
bool bDelayCheckLevelSetup=false;
bool bLoadLevelSetupToMap=true;
//---------------------------------------------------------------------------
const bool Server_Off=false;                                                         // server on off //
const bool Server_On=true;                                                          // server on off //
//---------------------------------------------------------------------------
bool bHot1Ready=false;
bool bHot2Ready=false;
bool bMessageAlarm=false;
bool bTesterOnLine=false;

AnsiString asNowUserID;
AnsiString asNowUserLevel;
bool bMotorHomePowerOn=false;
bool bHomeByStart=false;
bool EP_Install;
bool bHasPushEMG=false;


bool UseCleanOut=false;     //20131120 wei 除了真的沒卡匣之外其餘時間不能執行CleanOut

//Motion_Card_Mode==============================================================================
int MOTION_CARD_TYPE        =0;                     //Brian 20121015     //20140331 wei
int IO_CARD_TYPE            =0;                     //Brian 20121015     //20140331 wei

char RunStatus;
//int  InLoaderCount=0;
//int  OutLoaderCount=0;
//bool bOpenTeach=false;

int iContactCount=0;
int iLoaderCount[2]={0};

//bool bFlagMgzOnecycleFinish     =false;
//bool bFlagEJOnecycleFinish      =false;
//bool bFlagPnpOnecycleFinish     =false;
//bool bFlagPnp1OnecycleFinish    =false;
//bool bFlagEmpty1OnecycleFinish  =false;
//bool bFlagEmpty2OnecycleFinish  =false;
//bool bFlagTube1OnecycleFinish   =false;
//bool bFlagTube2OnecycleFinish   =false;

bool bSortArm1OneCycleFinish    =false;                                         //Steven 20240707 : Fixed for one cycle
bool bSortArm2OneCycleFinish    =false;
bool bSortStoreArmOneCycleFinish=false;
bool bSortMagOneCycleFinish[eMGZTotal]={false, false, false};
bool bTrackOneCycleFinish[eTrackCount]={false};

bool bFlagMgzCleanoutFinish     =false;
bool bFlagEJCleanoutFinish      =false;
bool bFlagPnpCleanoutFinish     =false;
bool bFlagPnp1CleanoutFinish     =false;                                        //Sam 20170802 : 第二支 Arm Cleanout 旗標
bool bFlagAuto1CleanoutFinish   =false;
bool bFlagAuto2CleanoutFinish   =false;
bool bFlagTube1CleanoutFinish   =false;
bool bFlagTube2CleanoutFinish   =false;
bool bFlagCatchYCleanoutFinish  =false;

bool bEmpty1TrayFeedFinish  =false;

int iBinSortCount=0;
bool bHT1501Aflag               =false;
bool bTopCCDLearning            =false;
bool bDownCCDLearning           =false;
bool bAudioCCDLearning          =false;
//bool bPnpZToSafePos             =false;                                       //Sam 20171024 : 精簡掉
bool bMagNeedAutoLoadTray      =false;  //Sam 20240710 : 修正 OneCyce 異常
bool bInitialStartCheckMagLayer=false;  //Sam 20240710 : 修正 OneCyce 異常


//int  iBlockXMovePitch           =0;
//int  iBlockYMovePitch           =0;

int  MachineTypeChoice          =0;
AnsiString asModel             ="HT172";
AnsiString asSerialNo          ="";
AnsiString asFactory           ="T6";
AnsiString asHandlerID         ="PPTX";

AnsiString asUserID          ="";
AnsiString asPassword        ="";
AnsiString asHost            ="";
AnsiString asFTPPath         ="";
AnsiString asFTPPathUpload   ="";

AnsiString asTrayPathUpload  ="";


int  Pnp2SafePos                =0;
int  Pnp3SafePos                =0;


bool bFlagAuto1TrayFeedFinish   =false;                                         //Frank 20170828 (jou): 修正 Tray Feed Hang up 問題
bool bFlagAuto2TrayFeedFinish   =false;                                         //Frank 20170828 (jou): 修正 Tray Feed Hang up 問題

const int  HT1501               =0;                                             //Sam 20171019 (jou): 增加機型 Const
const int  HT1501A              =1;
const int  HT1501APlus          =2;

//int  iSortTrayIndex[eTrayCount];

int iSortArmXYMoveTask          =0;
int iMagArmXYMoveTask           =0;
int iTrayArmXYMoveTask          =0;
bool bSystemClose               =false;
bool bNeedLoadTrayToTransfer    =false;
bool bNeedPickTrayToMagTop      =false;
bool bNeedLoadTrayToMagEmpty    =false;
bool bIsInitLoadEmptyTrayToMag  =false;

bool bAseStart                  =false;                                         //kevin 20150427
bool bAseHome                   =false;                                         //kevin 20150427
bool bAsePause                  =false;                                         //kevin 20150427
bool bAseReset                  =false;                                         //kevin 20150427
bool bAseCleanOut               =false;                                         //kevin 20150427
bool bAseSKIP                   =false;                                         //kevin 20150427
bool bAseRetry                  =false;                                         //kevin 20150427
bool bAseOneCycle               =false;                                         //kevin 20150427
bool bAseTrayEnd                =false;                                         //kevin 20150427
bool bAseTrayFeed               =false;                                         //kevin 20150427
bool bTSTART                    =false;                                         //kevin 20160303
bool bSTEP                      =false;                                         //kevin 20160303

int  iAseHome                   =0;                                             //kevin 20150925
int  iAseCleanOut               =0;                                             //kevin 20150925
int  iAseOneCycle               =0;                                             //kevin 20150925
int  iAseAlarmReset             =0;                                             //kevin 20150925
int  iAseTrayEnd                =0;                                             //kevin 20150925
int  iAseTrayFeed               =0;                                             //kevin 20150925

bool bWaiteKeepRespond          =false;                                         //kevin 20231215 等待eKeep 回應
AnsiString ASE_ReturnState      ="";                                            //kevin 20150925  回傳目前機台狀況

AnsiString sLoaderID            ="";
AnsiString sEmpty1ID            ="";
AnsiString sEmpty2ID            ="";

bool bPauseSortMotor            =false;
bool bPauseMagMotor             =false;
bool Enable_PLCSafety_IO        =false;    //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
int iPLCSafetyVer               =0;        //joe 20220406.01 debug safe plc
int  iTestBinCount              =47;
int  IN_OUT_ARM_Y_PITCH_MIN              =6000;
int  IN_OUT_ARM_Y_PITCH_MAX              =12000;
int  IN_OUT_ARM_X_PITCH_MIN              =3000;
int  IN_OUT_ARM_X_PITCH_MAX              =6000;
int iHasSkipTrayDataNeedHold    =0;
bool bCalculatePauseTime=false; //Sam 20240526 : 新增每盤的UPH
int iUPH_PickCnt=0;
TDateTime tUPH_StartTime=0;
TDateTime tUPH_PauseStartTime=0;
TDateTime tUPH_PauseTime=0;
TDateTime tUPH_EndTime=0;
int  iUPH_LoaderCount=0;
bool bHandlerPause=false;
int iHandlerStartCount=0;

bool bSortArmTrayZFixSpeed=false;   //Sam 20240619 : 速度獨立設定
bool bSortArmNeedHome=false;
bool bSortArmYNeedHome=false;   //JerryYang 20240704 : Pick up error Y軸一起回HOME
bool bMagArmNeedHome=false;
int iByLotTrayCnt[eTrayCount]={0};    //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞
bool Enable_MAG1=false;
bool Enable_MAG2=true;
bool Enable_MAG3=false;
AnsiString sMNetLogPath="D:\\HT-172_Log\\MNetLog";

AnsiString asNetDisk="D:\\SortMap";

bool bSortArm2UseSuck[MAX_ARM_Row][MAX_ARM_Col]={{true, true},
                                                 {true, true},
                                                 {true, true},
                                                 {true, true}};
bool bPhysicalStart;                             //JerryYang 20250106 : Run check
bool bOLPLoadMap=false;
bool bStartCallLot_StartExe=false;
bool bPadInterfaceAndTrayStepCommPort=false;
AnsiString sLotAuto1Info="";                //Sam 20250724 : 新增 AutoSetBin By Lot Auto Info
AnsiString sLotAuto2Info="";
AnsiString sLotAuto3Info="";


