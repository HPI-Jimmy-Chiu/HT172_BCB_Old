/*---------------------------------------------------------------------------
    程式目的:Test hander 產品資料
    設計:lee
    設計日期:2000/03/01   ---   2000/06/01
  ---------------------------------------------------------------------------
*/
#ifndef prodH
#define prodH
//-------------------------------------------------------------------------
typedef struct {
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
typedef struct {
    int iLoad;
    int iUnLoad;
    int iStart;
    int Reserved[5];
}SM_TRAY_Y;
//-------------------------------------------------------------------------
typedef struct {
    int iPick;
    int iPlace;
    int Reserved[5];
}SM_PLATE;
//-------------------------------------------------------------------------
typedef struct {
    int iRight;
    int iLeft;
    int Reserved[5];
}SM_SHUTTLE;
//-------------------------------------------------------------------------
//==============================================================================
typedef struct
{
    int TrayZseparate                           ;//1
    int TrayZUnload                             ;

    int TrayLoaderLoadNew                       ;//3
    int TrayLoaderStart                         ;
    int TrayLoaderPitchOffset                   ;
    int TrayLoaderUnload                        ;

    int Arm1PickStart                           ;//7
    int Arm1PickPitchOffset                     ;
    int Arm1PlacetoHP1Start                     ;
    int Arm1PlacetoHP2Start                     ;
    int Arm1PlacePitchOffset                    ;

    int Arm1Z1PickHieght                        ;//12
    int Arm1Z1HP1PlaceHeight                    ;
    int Arm1Z1HP2PlaceHeight                    ;

    int Arm1Z2PickHieght                        ;//15
    int Arm1Z2HP1PlaceHeight                    ;
    int Arm1Z2HP2PlaceHeight                    ;

    int Arm1PickvarPitchOffset                  ;//18
    int Arm1PlacevarPitchOffset                 ;

    int HotPlate1LoadStart                      ;//20
    int HotPlate1LoadPitch                      ;
    int HotPlate1UnLoader                       ;

    int HotPlate2LoadStart                      ;//23
    int HotPlate2LoadPitch                      ;
    int HotPlate2UnLoader                       ;

    int Arm2PickFromHP1XStart                   ;//26
    int Arm2PickFromHP2XStart                   ;
    int Arm2PickfromHPXPitch                    ;
    int Arm2PlaceToSH1XStart                    ;
    int Arm2PlaceToSH2XStart                    ;

    int Arm2PickFromHP1YStart                   ;//31
    int Arm2PickFromHP2YStart                   ;
    int Arm2PickFromHPYPitch                    ;
    int Arm2PlaceToSH1YStart                    ;
    int Arm2PlaceToSH2YStart                    ;

    int Arm2Z1HP1PickHeight                     ;//36
    int Arm2Z1HP2PickHeight                     ;
    int Arm2Z1PlacetoSHT1Height                 ;
    int Arm2Z1PlacetoSHT2Height                 ;

    int Arm2Z2HP1PickHeight                     ;//40
    int Arm2Z2HP2PickHeight                     ;
    int Arm2Z2PlacetoSHT1Height                 ;
    int Arm2Z2PlacetoSHT2Height                 ;

    int Arm2PickvarPitchOffset                  ;//44
    int Arm2PlacevarPitchOffset                 ;

    int Shullte1LeftStop                        ;//46
    int Shullte1RightStop                       ;
    int Shullte2LeftStop                        ;
    int Shullte2RightStop                       ;

    int TestYFrontStop                          ;//50
    int TestYRearStop                           ;

    int OutArmPickFromSHT1X                     ;//52
    int OutArmPickFromSHT2X                     ;
    int OutArmPlacetoAuto1X                     ;
    int OutArmPlacetoAuto2X                     ;
    int OutArmPlacetoAuto3X                     ;
    int OutArmPlacetoFix1X                      ;
    int OutArmPlacetoFix2X                      ;
    int OutArmPlacetoFix3X                      ;

    int OutArmPickFromSHT1Y                     ;//60
    int OutArmPickFromSHT2Y                     ;
    int OutArmPlacetoAuto1Y                     ;
    int OutArmPlacetoAuto2Y                     ;
    int OutArmPlacetoAuto3Y                     ;
    int OutArmPlacetoFix1Y                      ;
    int OutArmPlacetoFix2Y                      ;
    int OutArmPlacetoFix3Y                      ;

    int OutArmZ1SHT1pickheight                  ;//68
    int OutArmZ1SHT2pickheight                  ;
    int OutArmZ1Autoplaceheight                 ;
    int OutArmZ1Fixplaceheight                  ;

    int OutArmZ2SHT1pickheight                  ;//72
    int OutArmZ2SHT2pickheight                  ;
    int OutArmZ2Autoplaceheight                 ;
    int OutArmZ2Fixplaceheight                  ;

    int OutArmPickvarPitchOffset                ;//76
    int OutArmPlacevarPitchOffset               ;

    int AutoTray1PlaceStart                     ;//78
    int AutoTray1Loadnew                        ;
    int AutoTray1unLoad                         ;

    int AutoTray2PlaceStart                     ;//81
    int AutoTray2Loadnew                        ;
    int AutoTray2unLoad                         ;

    int AutoTray3PlaceStart                     ;//84
    int AutoTray3Loadnew                        ;
    int AutoTray3unLoad                         ;

    int CatchtrayArmLoadStage                   ;//87
    int CatchtrayArmBuffer1                     ;
    int CatchtrayArmBuffer2                     ;
    int CatchtrayArmBuffer3                     ;
    int CatchtrayArmAuto1                       ;
    int CatchtrayArmAuto2                       ;
    int CatchtrayArmAuto3                       ;
    int TestZ1PickFromShuttle1                  ;
    int TestZ2PickFromShuttle2                  ;

    int OutArmPlacePitchXOffset;
    int OutArmPlacePitchYOffset;
}RUN_OFFSET;

extern RUN_OFFSET TempRunOffset;

typedef struct {
    char cFileName[256];
    char ProductInfo[10][256];  //產品說明
    int  Version;
    DATE_INFO LastEditDate;

    TRAY_DATA LoadForm;
    TRAY_DATA AutoForm[3];
    TRAY_DATA FixForm[3];
    TRAY_DATA HotPlateForm[2];
    TRAY_DATA BufferForm;
    bool bBufferInstall;

    bool fInArmSuckUse[4];
    bool fOutArmSuckUse[4];
    bool fSuckNeedAll;

    int  iShuttlePitch;


    bool fUseHotPlane;
    int HotMaxSpin;
    int HotMinSpin;
    int HotInitSpin;
    int BinMaxSpin;
    int BinMinSpin;
    int BinInitSpin;
    int iHotTime;
    bool fHotPlaneFull;
    int iHotShowLimit;
    int MiddlePush;
    int TesterDelay;
    int RetryCount;
    int BinType[13];

    int TrayZ_Up;
    int TrayZ_Wait;
    int TrayZ_Down;
    int TrayY_Load;
    int TrayY_Start;
    int XInArm1_Tray_Pick;
    int XInArm1_Plate1_Place;
    int XInArm1_Plate2_Place;
    int XInArm2_Plate1_Pick;
    int YInArm2_Plate1_Pick;
    int XInArm2_Plate2_Pick;
    int YInArm2_Plate2_Pick;

    int XInArm2_Shuttle1_Place;
    int YInArm2_Shuttle1_Place;
    int XInArm2_Shuttle2_Place;
    int YInArm2_Shuttle2_Place;
    SM_PLATE  Plate[2];



    int TrayY_UnLoad;
    int ZInArm1_Tray_Pick[2];
    int ZInArm1_Plate1_Place[2];
    int ZInArm1_Plate2_Place[2];
    int ZInArm1_Plate1_Safe[2];
    int ZInArm1_Plate2_Safe[2];
    int ZInArm2_Plate1_Pick[2];
    int ZInArm2_Plate2_Pick[2];
    int ZInArm2_Shuttle1_Place[2];
    int ZInArm2_Shuttle2_Place[2];
    int ZInArm2_Plate1_Safe[2];
    int ZInArm2_Shuttle_Safe[2];
    SM_SHUTTLE Shuttle[2];
    SM_TRAY_Y Bin[3];

    int TestY_Front;
    int TestY_Rear;
    int TestY_FrontCanDown; // 向後移動時 Z1 可以下移
    int TestY_RearCanDown;  // 向前移動時 Z2 可以下移


    int TestZ1_Pick;
    int TestZ1_Place;
    int TestZ1_Wait;
    int TestZ1_Test;
    int TestZ1_Safe;
    int TestZ1_Drop_Offset;

    int TestZ2_Pick;
    int TestZ2_Place;
    int TestZ2_Wait;
    int TestZ2_Test;
    int TestZ2_Safe;
    int TestZ2_Drop_Offset;

    int TestZ_Drop_Wait;
    int TypicalTotal;
    int SafeRangeTotal;


    int XOutArm_Shuttle1_Pick;
    int YOutArm_Shuttle1_Pick;
    int XOutArm_Shuttle2_Pick;
    int YOutArm_Shuttle2_Pick;
    int XOutArm_Auto_Place[3];
    int YOutArm_Auto_Place[3];
    int XOutArm_Fix_Place[3];
    int YOutArm_Fix_Place[3];
    int ZOutArm_Auto_Safe[2];
    int ZOutArm_Fix_Safe[2];
    int ZOutArm_Shuttle1_Pick[2];
    int ZOutArm_Shuttle2_Pick[2];

    int ZOutArm_Auto_Place[2];
    int ZOutArm_Fix_Place[2];
    int XOutArm_Buffer_Place;
    int YOutArm_Buffer_Place;
    int ZOutArm_Buffer_Pick;
    int ZOutArm_Buffer_Place;
    int iYAutoLoadTray[3];
    int iYAutoUnLoadTray[3];
    int iYAutoStart[3];
    int iXTrayLoad;
    int iXTrayColor1;
    int iXTrayColor2;
    int iXTrayColor3;
    int iXTrayAuto[3];
//  bool SuckerFullMove;
//  Prod.fSuckNeedAll
//  bool InArm1SuckDisable[2];
//  bool TestHeadDisable[2];
    bool ShuttleDisable[2];
    bool fColorTrayUse;

    bool bTesterFailAlarm;
    int  iTesterFailCount;
    int  iTesterMaxTime;
    int  iTesterDummyTime;
    int  iTesterStartDelayTime;
    int  iTesterInterface;

    int Arm1HomeWidth ;
    int Arm2HomeWidth ;
    int OutArmHomeWidth;

    int ZInArm2_Plate2_Safe[2];

} PROD_INFO_ST;
extern PROD_INFO_ST Prod;
extern PROD_INFO_ST TempProd;

typedef struct
{
    char LastOpenFilename[512];       //  上次開檔檔名
    int  Version;
    int  CommInterface;
    int  CommMachine;
    long SendCT[4];
    long HeadPass[8];
    long BinCT[4][8];
    long TestCT[4][8];
    int  SoftSpeed[30];
    char SystemDateRecord[8][64];
    long SystemAccSecond[4][8];
    int  MessageLight[8][3];
    int  MusicSelect[9];
    bool bMusicEnable;
    long lMaxAlarmCT;
    long lMaxMessageCT;
    long lMaxUserCT;
    int  iLanguageCountry;
    bool bShowCountType;
    int  iTemperature;
    int  iStartMode;
    int  iTester;
    int  iScanner;
    int  iRealDummy;
    bool bUT150Install[8];

    //2000/03/08
    int TrayLoaderSpeed;           // 0
    int InArm1Speed;               // 1
    int HotPlateSpeed;             // 2
    int InArm2Speed;               // 3
    int ShuttleSpeed;              // 4
    int IndexSpeed;                // 5
    int OutArmSpeed;               // 6
    int TrayUnLoaderSpeed;         // 7
    int CatchTraySpeed;            // 8

    int InArm1Retry;               // 9
    int InArm2Retry;               // 10
    int ShuttleRetry;              // 11
    int TesterRetry;               // 12
    int OutArmRetry;               // 13

    int TrayLoaderDelay;           // 14
    int InArm1Delay;               // 15
    int HotPlateDelay;             // 16
    int InArm2Delay;               // 17
    int ShuttleDelay;              // 18
    int IndexDelay;                // 19
    int OutArmDelay;               // 20
    int TrayUnLoaderDelay;         // 21
    int CatchTrayDelay;            // 22

    int SoakTime;                  // 23
    int JamSoakTime;               // 24
    int InitialWaitTime;           // 25
    int CollingTime;               // 26
    int TrayZSeparate_Offset;      // 27
    int TrayZUnload_Offset;        // 28
    int TrayLoaderStart_Offset;    // 29

    int Arm1PickFromTrayX_Offset;  // 30
    int Arm1PickFromTrayZ1_Offset; // 31
    int Arm1PickFromTrayZ2_Offset; // 32

    int Arm1PlaceToHotPlate1X_Offset;  // 33
    int Arm1PlaceToHotPlate1Z1_Offset; // 34
    int Arm1PlaceToHotPlate1Z2_Offset; // 35

    int Arm1PlaceToHotPlate2X_Offset;  // 36
    int Arm1PlaceToHotPlate2Z1_Offset; // 37
    int Arm1PlaceToHotPlate2Z2_Offset; // 38


    int HotPlate1Loader_Offset;        //39
    int HotPlate2Loader_Offset;        //40

    int HotPlate1UnLoader_Offset;      //41
    int HotPlate2UnLoader_Offset;      //42

    int Arm2PickFromHotPlate1X_Offset;  //43
    int Arm2PickFromHotPlate1Z1_Offset; //44
    int Arm2PickFromHotPlate1Z2_Offset; //45

    int Arm2PickFromHotPlate2X_Offset;  //46
    int Arm2PickFromHotPlate2Z1_Offset; //47
    int Arm2PickFromHotPlate2Z2_Offset; //48

    int Arm2PlaceToShuttle1X_Offset;    //49
    int Arm2PlaceToShuttle1Y_Offset;    //50
    int Arm2PlaceToShuttleZ1_Offset;    //51
    int Arm2PlaceToShuttleZ2_Offset;    //52
    int Arm2PlaceToShuttle2X_Offset;    //53
    int Arm2PlaceToShuttle2Y_Offset;    //54

    int OutArmPickFromShuttle1X_Offset; //55
    int OutArmPickFromShuttle1Y_Offset; //56
    int OutArmPickFromShuttle2X_Offset; //57
    int OutArmPickFromShuttle2Y_Offset; //58

    int OutArmPlaceToAuto1X_Offset;     //59
    int OutArmPlaceToAuto2X_Offset;     //60
    int OutArmPlaceToAuto3X_Offset;     //61

    int OutArmPlaceToFix1X_Offset;      //62
    int OutArmPlaceToFix2X_Offset;      //63
    int OutArmPlaceToFix3X_Offset;      //64

    int Auto1PlaceStart_Offset;         //65
    int Auto2PlaceStart_Offset;         //66
    int Auto3PlaceStart_Offset;         //67

    int Auto1UnLoader_Offset;           //68
    int Auto2UnLoader_Offset;           //69
    int Auto3UnLoader_Offset;           //70

    int Auto1Loader_Offset;             //71
    int Auto2Loader_Offset;             //72
    int Auto3Loader_Offset;             //73

    int LoopMovePos[30][2];
    int LoopSpeed[30][2];

    float Index1Torue[2];
    float Index1Preasure[2];
    float Index2Torue[2];
    float Index2Preasure[2];
    float Index1MaxTorue;
    float Index1MaxPreasure;
    float Index2MaxTorue;
    float Index2MaxPreasure;
    int   ContactSet[8];
    int   ContactCount[8];

    int SocketContact[2][4][11];     // [累計,目前][socket][Cat]

    int  AccessLevel[20];
    RUN_OFFSET RunOffset;
} LAST_GENERAL_SET;
extern LAST_GENERAL_SET LastSet;
//==============================================================================
typedef struct {
    char FileName[12][128];
}HANDLER_SETUP;
extern HANDLER_SETUP THS;
extern HANDLER_SETUP TempTHS;
//==============================================================================
typedef struct {
    int Arm1VacuumWait;
    int Arm2VacuumWait;
    int IndexVacuumWait;
    int OutVacuumWait;
    bool bVacuumWaitEnable;

    int Arm1DestroyWait;
    int Arm2DestroyWait;
    int IndexDestroyWait;
    int OutDestroyWait;
    bool bDestroyWaitEnable;


    bool bInArmHeadUse[4];
    bool bOutArmHeadUse[4];
    bool bArmFullMove;


    int  iShuttlePitch;

    int Arm1DropHeightOffset;
    int Arm2DropHeightOffset ;
    int IndexDropHeightOffset;
    int OutDropHeightOffset  ;


}SYSTEM_SETUP;
extern SYSTEM_SETUP SystemSetup;
extern SYSTEM_SETUP TempSystemSetup;
//==============================================================================
typedef struct {
    int Test1Contact;
    int Test2Contact;
    int Test1ReleaseOffset;
    int Test2ReleaseOffset;
    int TestReleaseWait;
    int Test1Drop;
    int Test2Drop;
    int TypicalPressure;
    int SafeRange;
    int TypicalTotal;
    int SafeRangeTotal;
    int XMoldDimension;
    int YMoldDimension;
    int XLeadDimension;
    int YLeadDimension;
    int XLeadNumber;
    int YLeadNumber;
}SYSTEM_DEVICE_FORM;
extern SYSTEM_DEVICE_FORM DeviceForm;
extern SYSTEM_DEVICE_FORM TempDeviceForm;
//==============================================================================
typedef struct
{
    int Type;
    int Degree;
    int Direction;
}TRAY_TYPE;
//==============================================================================
typedef struct
{
    int XPitch;
    int YPitch;
    int XStart;
    int YStart;
    int XDivision;
    int YDivision;
    int ZDepth;
    int Width;
    int Height;
    char Alias[30];
    bool bInstall;
}TRAY_TYPE_PARA;
//==============================================================================
typedef struct
{
    int  LodareType;
    bool LoaderToAuto[3];
    bool Color1ToAuto[3];
    bool Color2ToAuto[3];
    bool Color3ToAuto[3];
    TRAY_TYPE Fix[3];
    TRAY_TYPE Auto[3];
    TRAY_TYPE Loader;
    TRAY_TYPE Empty;
    TRAY_TYPE Color;
    TRAY_TYPE_PARA Type[3];
}SYSTEM_TRAY_FORM;
extern SYSTEM_TRAY_FORM TrayForm;
extern SYSTEM_TRAY_FORM TempTrayForm;
extern TRAY_TYPE_PARA TempHotPlateForm;
extern TRAY_TYPE_PARA HotPlateForm;
extern TRAY_TYPE_PARA TempBufferForm;
extern TRAY_TYPE_PARA BufferForm;

//==============================================================================
typedef struct
{
    int SoakTime;
    int JamSoakTime;
    int InitialWaitTime;
    int CoolingTime;
    int Mode;
    float Offset[8][4];
    int LowBase;
    int MidBase;
    int HighBase;
    int Default;
    int TempMode;
}SYSTEM_TEMPERATURE;
extern SYSTEM_TEMPERATURE Temperature;
extern SYSTEM_TEMPERATURE TempTemperature;
//==============================================================================
typedef struct
{
    int TestMode;
    bool bFailAlarm;
    int FailCount;
    int MaxTime;
    int DummyTime;
    int StartDelayTime;
    int Interface;
}SYSTEM_TEST_IF;
extern SYSTEM_TEST_IF TestIF;
extern SYSTEM_TEST_IF TempTestIF;
//==============================================================================
typedef struct
{
    bool bOnLine;
    int  Mode;
    int Rate;
    int ScanDelay;
    int IDCode;
    bool bFailAlarm;
    int FailCount;
    bool bScanCountOn;
    int ScanFailCount;
    int ScanTotalCount;
    bool ScanRateMode;
    int MG1;
    int MG2;
    int PassSet;
    int LimitTime;
}SYSTEM_SCANNER_IF;
extern SYSTEM_SCANNER_IF ScannerIF;
extern SYSTEM_SCANNER_IF TempScannerIF;
//==============================================================================
typedef struct
{
    int CatDtat[10];
    int Contach[10];
    int PassFail[10];
    int Scanner[10];
    int IfError;
}SYSTEM_BIN_SELECT;
extern SYSTEM_BIN_SELECT BinSelect;
extern SYSTEM_BIN_SELECT TempBinSelect;
//==============================================================================
typedef struct {
    DATE_INFO PowerOn;
    DATE_INFO Operate;
    DATE_INFO Down;
    int LoadCount;
    int ThroughPut;
    //int ContactCount[8];
    //int CatCount[10][4];
    bool bTestRowData;
    bool bTestPercent;
    int  ScannerMode;
    int  ScannerRate;
    bool bScannerRowData;
    bool bScannerPercent;
    int  ScannerReject;
    int  ScannerInvalid;
    int  ScannerRework;
    int  ScannerStatus;
    int  ScannerGood;
    int  ScannerTotal;
}DATA_SUB;
//==============================================================================
typedef struct
{
    DATA_SUB Cumulative;
    DATA_SUB Yield;
}DATA;
extern DATA Data;
//==============================================================================
typedef struct
{
    int RecordCT;
    char ID[30][30];
    char PassWord[30][30];
    int  Level[30];
}PASS_WORD;
extern PASS_WORD USER;
//==============================================================================
typedef struct
{
    int XStart;
    int YStart;
    int XWidth;
    int YHeight;
    int ZHeight;
}TECH_KIT_TRAY;
//==============================================================================
typedef struct
{
    int Load;
    int UnLoad;
}TECH_TRAY_LOADER;
//==============================================================================
typedef struct
{
    int XIndex;
    int YIndex;
    int XLeft;
    int YBottom;
}TECH_TRAY;
//==============================================================================
typedef struct
{
    int LoadStage;
    int Color1;
    int Color2;
    int Color3;
    int Auto1;
    int Auto2;
    int Auto3;

    int LoadStage_Left;
    int Color1_Left;
    int Color2_Left;
    int Color3_Left;
    int Auto1_Left;
    int Auto2_Left;
    int Auto3_Left;

}TECH_TRAY_ARM;
//==============================================================================
typedef struct
{
    int Pick1;     //(plate1,plate2)
    int Pick2;
    int Place1;    //(plate1,plate2) or (shuttle1,shuttle2) or (auto?,fix?);
    int Place2;
}TECH_ARM_Z;
//==============================================================================

typedef struct
{
    TECH_KIT_TRAY    TrayKit;
    TECH_KIT_TRAY    PlateKit;

    TECH_TRAY_LOADER TrayLoader;
    TECH_TRAY        LoadStage;
    TECH_TRAY        Arm1Plate1;
    TECH_TRAY        Arm1Plate2;

    TECH_TRAY        Arm2Plate1;
    int              Plate1ForPick;
    TECH_TRAY        Arm2Plate2;
    int              Plate2ForPick;

    int              Arm2XShuttle1;
    int              Arm2YShuttle1;
    int              Arm2XShuttle2;
    int              Arm2YShuttle2;

    int              Shuttle1Left;
    int              Shuttle1Right;
    int              Shuttle2Left;
    int              Shuttle2Right;
    int              Test1OnShuttle1;
    int              Test2OnShuttle2;
    int              Test1OnTestSocket;
    int              Test2OnTestSocket;

    int              OutArmXShuttle1;
    int              OutArmYShuttle1;
    int              OutArmXShuttle2;
    int              OutArmYShuttle2;

    TECH_TRAY        OutArmAuto1;
    int              Auto1ForPlace;
    TECH_TRAY        OutArmAuto2;
    int              Auto2ForPlace;
    TECH_TRAY        OutArmAuto3;
    int              Auto3ForPlace;

    TECH_TRAY        OutArmFix1;
    TECH_TRAY        OutArmFix2;
    TECH_TRAY        OutArmFix3;

    TECH_TRAY_ARM    TrayArm;

    int              LoadStageUnLoader;
    int              Auto1Loader;
    int              Auto2Loader;
    int              Auto3Loader;

    int              LoadStageUnLoader_Bottom;
    int              Auto1Loader_Bottom;
    int              Auto2Loader_Bottom;
    int              Auto3Loader_Bottom;

    int              Auto1UnLoader;
    int              Auto2UnLoader;
    int              Auto3UnLoader;

    int              LoadStageLoad;
    TECH_ARM_Z       Arm1Z1;
    TECH_ARM_Z       Arm1Z2;
    TECH_ARM_Z       Arm2Z1;
    TECH_ARM_Z       Arm2Z2;
    TECH_ARM_Z       TestZ1;
    TECH_ARM_Z       TestZ2;
    TECH_ARM_Z       OutArmZ1;
    TECH_ARM_Z       OutArmZ2;


    int              TestZ1TestHeight;
    int              TestZ2TestHeight;
    int              TestZ1TestWait;
    int              TestZ2TestWait;

    int              OutBufferX;
    int              OutBufferY;
    int              OutBufferZ1Pick;
    int              OutBufferZ2Pick;


    int               Arm1HomeWidth;
    int               Arm2HomeWidth;
    int               OutArmHomeWidth;

    int TestY_Front;
    int TestY_Rear;
    int TestY_FrontCanDown; // 向後移動時 Z1 可以下移
    int TestY_RearCanDown;  // 向前移動時 Z2 可以下移

    int Arm1Z1Plate1SafeHeight;
    int Arm1Z2Plate1SafeHeight;
    int Arm1Z1Plate2SafeHeight;
    int Arm1Z2Plate2SafeHeight;
    int Arm1XCanDown;

    int Arm2Z1Plate1SafeHeight;
    int Arm2Z2Plate1SafeHeight;
    int Arm2Z1ShuttleSafeHeight;
    int Arm2Z2ShuttleSafeHeight;
    int Arm2XCanDown;

    int OutArmZ1AutoSafeHeight;
    int OutArmZ2AutoSafeHeight;
    int OutArmZ1FixSafeHeight;
    int OutArmZ2FixSafeHeight;
    int OutArmXCanDown;
    int Arm2Z1Plate2SafeHeight;
    int Arm2Z2Plate2SafeHeight;
}TECH;
extern TECH Tech;
extern TECH TempTech;

typedef struct
{
    int iInArm1SuckPauseTime;
    int iInArm1DestroyPauseTime;
    int iInArm2SuckPauseTime;
    int iInArm2DestroyPauseTime;
    int iTestHeadSuckPauseTime;
    int iTestHeadDestroyPauseTime;
    int iOutArmSuckPauseTime;
    int iOutArmDestroyPauseTime;

    bool bInArm1SuckPauseEnable;
    bool bInArm1DestroyPauseEnable;
    bool bInArm2SuckPauseEnable;
    bool bInArm2DestroyPauseEnable;
    bool bTestHeadSuckPauseEnable;
    bool bTestHeadDestroyPauseEnable;
    bool bOutArmSuckPauseEnable;
    bool bOutArmDestroyPauseEnable;

    bool bInArm1SuckPreOn;
    bool bInArm1SuckPreOff;
    bool bInArm2SuckPreOn;
    bool bInArm2SuckPreOff;
    bool bTestHeadSuckPreOn;
    bool bTestHeadSuckPreOff;
    bool bOutArmSuckPreOn;
    bool bOutArmSuckPreOff;

    int  iInArm1SuckPreOnHeight;
    int  iInArm1SuckPreOffHeight;
    int  iInArm2SuckPreOnHeight;
    int  iInArm2SuckPreOffHeight;
    int  iTestHeadSuckPreOnHeight;
    int  iTestHeadSuckPreOffHeight;
    int  iOutArmSuckPreOnHeight;
    int  iOutArmSuckPreOffHeight;

}SPEC_SETUP;
extern SPEC_SETUP SpecSetup;

bool WriteLastDataFile();
bool ReadLastDataFile();
bool WriteData(char *cFName,char *ptr,int size);
bool ReadData(char *cFName,char *ptr,int size);
bool CheckFileExist(char *cFName);
bool WriteDataFile(char *cFName,PROD_INFO_ST *DataBuf);
bool ReadDataFile(char *cFName,PROD_INFO_ST *DataBuf);
void ClearLastSet();
void SavePassword();
void ReadPassword();
#endif


