//---------------------------------------------------------------------------

#ifndef aLoaderH
#define aLoaderH
//---------------------------------------------------------------------------
extern void InitialLoader();
extern void InitialAuto();
extern void UpdateLoaderParameter();
class MainChannel;
class TAutoModule
{
   public:
      __fastcall TAutoModule();
        TTrayMotor *Mot_Auto_Work[eTrackCount][eTrackColTotal];
        TTrayMotor *MAutoTray[eTrayCount];
        TTrayMotor *MTrackToStore[eTrackCount];
        int TrackHasTray();
};
extern TAutoModule *Auto;
class MainChannel
{
    private:
    public:
        MainChannel();
        ~MainChannel();
        //status
        int iStackStatus;
        enum eHS_StackArea
        {
            eHS_StackArea_Idle=0,
            eHS_StackArea_NeedTray,
            eHS_StackArea_ToStack,
        };
        int iCCDStatus;
        enum eHS_CCD
        {
            eHS_CCD_Idle=0,
            eHS_CCD_NeedPhoto,
        };
        int iTransLDStatus;
        enum eHS_TransforLD
        {
            eHS_TransLD_Idle=0,
            eHS_TransLD_NeedOutTray,
        };
        int iTransMGZStatus;
        enum eHS_TransforMAG
        {
            eHS_TransMGZ_Busy=0,
            eHS_TransMGZ_Idle,
        };
};
class LoaderChannel : public MainChannel
{
    private:
    public:
        LoaderChannel();
        ~LoaderChannel();
};
class Empty1Channel : public MainChannel
{
    private:
    public:
        Empty1Channel();
        ~Empty1Channel();
};
class Empty2Channel : public MainChannel
{
    private:
    public:
        Empty2Channel();
        ~Empty2Channel();
        TMySensor   *Sn_CarHasTray         ;
        TMyCylinder *C_Separatory          ;
        TMyCylinder *C_Middle              ;
        TMyCylinder *C_Up                  ;
        HTimer TrayToCarDelay;
        HTimer CynDelay;
};
class TransforChannel
{
    private:
        int iDoAllActionTask;
        int iDoGetLoaderTrayTask;
        int iDoOutTrayToMagTask;
        MainChannel *mainch;
        MainChannel *magch;
        HTimer CynDelay;
    public:
        TransforChannel();
        ~TransforChannel();
        void SetMainCh(MainChannel *mch){mainch=mch;}
        void SetMAGCh(MainChannel *mch){magch=mch;}
        TMyCylinder *C_ShiftStageEdgePush                ;
        TMySwitch *SwTransferCCW           ;
        TMySwitch *SwTransferCW            ;
        TTrayMotor *MotTransferX           ;
        TTrayMotor *MotTransferX_Data      ;
        TMySensor *SnShiftStageEdgePush    ;
        TMySensor *SnShiftStageCWPreDete   ;
        TMySensor *SnShiftStageHasTray     ;
        int iTransferX_LD;
        int iTransferX_Empty;
        //action
        bool DoAllAction(bool bReset);
        bool DoGetLoaderTray(bool bReset);
        bool DoOutTrayToMag(bool bReset);
        //status
        int iLoaderStatus;
        enum eHS_Loader
        {
            eHS_LD_Idle=0,
            eHS_LD_Ready,
            eHS_LD_Busy,
        };
        int iMGZStatus;
        enum eHS_MGZ
        {
            eHS_MGZ_Idle=0,
            eHS_MGZ_Ready,
            eHS_MGZ_Busy,
        };

};
class TLoaderModule
{
   private:
        int iCynUpTask[eLoaderTotal];
        int iCynMidTask[eLoaderTotal];
        int iCynLowTask[eLoaderTotal];

        TQPF_Timer CynUpDelay[eLoaderTotal];
        TQPF_Timer CynMidDelay[eLoaderTotal];
        TQPF_Timer CynLowDelay[eLoaderTotal];

        bool AutoCylinderUp(bool bReset, int iAuto);
        bool AutoCylinderMiddle(bool bReset, int iAuto);
        bool AutoCylinderLower(bool bReset, int iAuto);

        int  iLoadNewTrayToLoaderCarTask;
        int  iLoadNewTrayToEmpty1CarTask;
        int  iLoadNewTrayToEmpty2CarTask;
        int  iLoadEmpty2MagTask;
        bool DoLoadNewTrayToLoaderCar(bool bReset=false);                       //TrayCar
        bool DoLoadNewTrayToEmpty1Car(bool bReset=false);
        bool DoLoadNewTrayToEmpty2Car(bool bReset=false);
        bool DoLoadEmpty2Mag(bool bReset=false);

        TQPF_Timer TrayToCarDelay[eLoaderTotal];
        //TQPF_Timer TrayToWorkDelay;
        TQPF_Timer TrayInPosDelay;
        TQPF_Timer LoaderCylinderOffTimeOut;                                    //Sam 20240717 : 穝糤 Alarm Code
        TQPF_Timer LoaderMoveTimeOut;                                           //Sam 20240730 : 穝糤 Alarm Code
        TQPF_Timer CatchTrayDelay;
        bool LoaderCylinderPreOn(bool bReset, int iAuto);                       //Sam 20240810 : Loader Cylinder PreOn
        TQPF_Timer LoaderPreOnDelay[eLoaderTotal];
        int iLoaderPreOnCount[eLoaderTotal];
        int iLoaderPreOnTask[eLoaderTotal];

        int  iEmptyTrayToTrackAreaTask;
        bool bLoadingEmptyTrayToTrack;
        bool DoLoadEmptyTrayToTrackArea(bool bReset=false);                     //Empty Car絃瓂笵跋办
        TQPF_Timer tEmpty1ToTrackDelay[eTrackCount];
        TQPF_Timer tTrackToStoreDelay[eTrackCount][eTrackColTotal];
        
        int  iEmptyTrayToStoreAreaTask;
        int  iEmpTrayToStoreTask[eTrackCount];
        int  iNeedOutEmptyTrack;
        bool DoEmptyTrayToStoreArea(bool bReset=false);                         //Empty tray to store area
        bool DoEmptyTrayToStoreArea(bool bReset,int iRowTrack);
        bool DoEmptyTrayToStoreAreaByServoMot(bool bReset,int iRowTrack);
        bool GetEmpty1HasTray(int iAuto=-1);
        int  iLoaderTrayToWorkAreaTask;
        int  iWorkAreaToTransferTask;
        bool DoLoaderTrayToWorkArea(bool bReset=false);                         //秈跋办
        bool DoLoaderTrayToTansfer(bool bReset=false);                          //瞒秨跋办

        int  iTransferToMagLDTask;
        bool DoTransferToMagLD(bool bReset=false);                              //眖Transfer癶Mag Work

        int  iTransferToMagUnloaderTask;
        bool DoTransferToMagUnloader(bool bReset=false);                        //眖Transfer癶Empty 2

        int  iWorkAreaToMagUnloaderTask;
        bool DoWorkAreaToMagUnloader(bool bNeedGoOut, bool bReset=false);       //眖Loader钡癶Empty 2

        int  iUnLoadTrayTask[eTrackCount];
        bool DoUnLoadTrayUp(eTrackCount, bool Reset=false);                     //どUnloader 

        bool bCheckSystemPause;
        bool LoadHasNoTrayNeedShowAlarm();
        bool NeedLoadNewTrayToCar();
        AnsiString sLoaderError;
        int iStatus;
        AnsiString sTemp;
        void DownStopAtEmpty1(int iFrom,int iTo);
   public:
        TLoaderModule();
        ~TLoaderModule();
        void InitialFlag();
        AnsiString TrayName[eLoaderTotal]       ;

        TMyCylinder *C_Separatory[eLoaderTotal] ;
        TMyCylinder *C_Middle[eLoaderTotal]     ;
        TMyCylinder *C_Up[eLoaderTotal]         ;
        TMyCylinder *C_EdgePush[eLoaderTotal]   ;
        TMyCylinder *C_Fixer[eLoaderTotal]      ;
        TMyCylinder *C_Stop[eLoaderTotal]       ;
        TMyCylinder *C_CCD_Stop[eLoaderTotal]   ;
        TMyCylinder *C_Empty2_EmptyStop;

        TMySensor *Sn_CarHasTray[eLoaderTotal]  ;
        TMySensor *Sn_CCDPos[eLoaderTotal]      ;
        TMySensor *SnSelectHasTray[eLoaderTotal];
        TMySensor *Sn_CarIsFull[eLoaderTotal]  ;

        //Loader----------------------------------------------------------------
        TTrayMotor *MotLoader_Car               ;
        TTrayMotor *MotWorkLoader               ;
        TTrayMotor *MotNowSortTray              ;                               //KenHsieh 20240514 : 干Loader戈锣传睲NowTray

        TMySensor *SnLDPreDete                  ;
        TMySensor *SnLDSureTray                 ;
        TMySensor *SnLD_FixCyPush               ;                               //KenHsieh 20240528 : 干═玂臔
        TMySensor *SnLD_EdgePush                ;                               //KenHsieh 20240528 : 干═玂臔

        TMySwitch *SwLoaderCCW                  ;
        TMySwitch *SwLoaderCW                   ;
        TMySwitch *SwLoaderCWDete               ;

        //Empty 1---------------------------------------------------------------
        TTrayMotor *MotEmpty1_Car                                   ;

        TMySwitch *SwEmptyCCW                                       ;
        TMySwitch *SwEmptyCW                                        ;

        //Empty 1瓂笵-----------------------------------------------------------
        TTrayMotor *MotTrack_Work[eTrackCount]                      ;
        TMySwitch  *SwEmptyRowCW[eTrackCount]                       ;
        TMySwitch  *SwEmptyRowCCW[eTrackCount]                      ;
        TMySwitch  *SwAutoRowDete[eTrackCount]                      ;

        TMyCylinder *C_Auto_Stop[eTrackCount][eTrackColTotal]       ;
        TMyCylinder *C_Empty_Fixer[eTrackCount]                     ;
        TMyCylinder *C_Empty_PushTray[eTrackCount]                  ;
        TMyCylinder *C_Empty_Stop[eTrackCount]                      ;
        TMyCylinder *C_Auto_AntiDrop[eTrackCount][eTrackColTotal]   ;
        TMyCylinder *C_Auto_InclinePush[eTrackCount][eTrackColTotal];
        TMyCylinder *C_Auto_Zup[eTrackCount][eTrackColTotal]        ;
        TMyCylinder *C_AutoRow_PushTray[eTrackCount]                ;
        TMyCylinder *C_AutoRow_Stop[eTrackCount]                    ;
        TMyCylinder *C_Mag_CCD_Stop;

        TMySensor *SnAutoCyAntiDrop1[eTrackCount][eTrackColTotal]   ;
        TMySensor *SnAutoCyAntiDrop2[eTrackCount][eTrackColTotal]   ;
        TMySensor *SnAutoCyAntiDrop3[eTrackCount][eTrackColTotal]   ;

        TMySensor *SnEmptyHasTray[eTrackCount]                      ;
        TMySensor *SnEmptyFCyPushTray[eTrackCount]                  ;
        TMySensor *SnEmptyRCyPushTray[eTrackCount]                  ;
        TMySensor *SnAutoRowHasTray[eTrackCount]                    ;
        TMySensor *Sen_Auto_HasTray[eTrackCount][eTrackColTotal]    ;
        TMySensor *Sen_Auto_CarHasTray[eTrackCount][eTrackColTotal] ;

        //Empty 2---------------------------------------------------------------
        TTrayMotor *MotEmpty2_Car               ;
        TTrayMotor *MotEmpty2_Tray              ;
        TTrayMotor *MotMagLD_Work               ;

        TMySensor *SnMagPreDete                 ;
        TMySensor *SnMagSureTray                ;
        TMySensor *SnEmpty2HasEmptyTray         ;
        TMySensor *SnMag_EdgePush             ;                                 //KenHsieh 20240528 : 干═玂臔
        TMySensor *SnMag_FixCyPush            ;                                 //KenHsieh 20240528 : 干═玂臔

        TMySwitch *SwMagCCW                     ;
        TMySwitch *SwMagCW                      ;
        TMySwitch *SwEmpty2CCWDete              ;

        //Transfer--------------------------------------------------------------
        TTrayMotor *MotTransferX                ;
        TTrayMotor *MotTransferX_Data           ;

        TMySensor *SnShiftStageEdgePush         ;
        TMySensor *SnShiftStageCWPreDete        ;
        TMySensor *SnShiftStageHasTray          ;

        TMySwitch *SwTransferCCW                ;
        TMySwitch *SwTransferCW                 ;

        TMyCylinder *C_ShiftStageEdgePush       ;

        void DoLoader(int &Task);
        void DoEmpty1(int &Task) ;
        void DoSupportAuto(int &Task);
        void DoEmpty2(int &Task);

        bool bSupportingTray;
        bool bSupportingEmpty1TrayToCar;                                        //Empty 1タ絃
        bool bTrackUnloadingTray[eTrackCount];                                  //Auto 瓂笵タ癶絃
        bool bAutoUnloadingTray[eTrackCount][eTrackColTotal];                   //Auto タ癶絃
        bool DoTrackAreaToStoreTrayArm(int &Task, int iTrack);                  //癶跋办

        double dLoadArrival;
        double dLoadLock;
        double dLoadMidd;
        double dLoadDown;

        double dUnloadArrival;
        double dUnloadDown;

        int iXStart;
        int iXPitch;
        int iYStart;
        int iYPitch;
        int iXDivision;
        int iYDivision;
        int HitCount;
        bool ArmMotionDone;
        bool bLoadEnable;

        AnsiString SummaryFileName;
        TQPF_Timer Empty2TrayDelay;
        TQPF_Timer Empty1MoveTimeOut;
        TQPF_Timer Empty2MoveTimeOut;
        TQPF_Timer LoadTrayDelay;

        int  CheckAutoHasTray();
        bool CheckAllAutoNoTray();
        bool CheckEmpty2NeedTray();
        bool CheckEmpty1TrackNeedTray(int iTrack);
        bool CheckFullTray(int &X, int &Y);

        int iXBlockStart;
        int iYBlockStart;
        int iXBlockPitch;
        int iYBlockPitch;
        int iXBlockItem;
        int iYBlockItem;

        int iTransferX_LD;
        int iTransferX_Empty;

        bool CheckAutoNeedTray(int &iX, int &iY);
        int LoadClipIDMapping(AnsiString asID);
        int CheckDuplicateLotID(AnsiString sFile,AnsiString asID);              //KenHsieh 20250806 : э狡絏厨牡(bool -> int)

        AnsiString eKeeperSchedule();
        //status
        enum eHS_SortArm
        {
            eHS_Busy=0,
            eHS_WorkAreaReady,
        };
        AnsiString GetStatusStr();
        void SetStatus(int is,AnsiString sFunc);
        int GetStatus();
        bool LoadAreaWorkReady(){return GetStatus()==(int)eHS_WorkAreaReady;}
        //status
        int iHS_StackAreaStatus;
        int iHS_StackAreaStatusEmpty2;

//        LoaderChannel* LoaderCh;
//        Empty1Channel* Empty1Ch;
//        Empty2Channel* Empty2Ch;

        TTrayMotor *MCarrierStoreX[eTrackCount];
        TTrayMotor *MCarrierStoreZ[eTrackCount];

        TMyCylinder *C_AutoTrolly_Stop[eTrackCount]  ;
        TMyCylinder *C_AutoTrolly_Fix[eTrackCount]   ;

        TMySensor *SnAutoTrollyHasTray[eTrackCount]  ;

        int iPos_Auto_X[eTrackCount][eTrackColTotal];
        int iPos_Auto_Z[eTrackCount][eTrackColTotal];
        int iPos_Auto_ZDown[eTrackCount][eTrackColTotal];
        int iPos_AutoXPush[eTrackCount];

        TransforChannel* TransCh;
        int SetTrayMapByMapList(AnsiString asID);
        bool bCheckStatusReady(int iTrack);

        enum eMapFormat
        {
            eEKeeperFormat  =0,
            eJHTFormat      =1,
            eWinstekFormat  =2,
            ePTIFormat      =3
        };
        //status
        int iMagArmStatus;
        enum eHS_MagArm
        {
            eHS_MA_Idle=0,
            eHS_MA_EmptyTrayReady,
            eHS_MA_WorkTrayOutReady,
        };
        void InitialLoader();
        bool bTransforMoving();
        int  iAutoZUpDelay;
        bool CheckClipIDCode(AnsiString sClipID);
        AnsiString sCheckDuplicateLotIDLoaderPath(){return AnsiString("D:\\BarcodeSorter\\DuplicateClipID_Loader.txt");}
        AnsiString sCheckDuplicateLotIDEmpty1Path(){return AnsiString("D:\\BarcodeSorter\\DuplicateClipID_Empty1.txt");}
        AnsiString sCheckDuplicateLotIDEmpty2Path(){return AnsiString("D:\\BarcodeSorter\\DuplicateClipID_Empty2.txt");}
};
extern TLoaderModule *LoaderModule;
#endif
