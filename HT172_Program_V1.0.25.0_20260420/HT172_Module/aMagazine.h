//---------------------------------------------------------------------------

#ifndef aMagazineH
#define aMagazineH
//---------------------------------------------------------------------------

void UpdateMagazineParameter();
void InitialMag();

class TMagazineModule
{
    private:
        int iTrayFromTopToLayerTask;
        int iTrayFromLayerToTopTask;
        int iTakeTrayFromTopTask;
        int iPutTrayTopToLayerTask;
        int iTakeTrayFromLayerTask;
        int iPutTrayLayerToTopTask;
        int iManualTakeTrayTask;
        int iTrayFromEmptyTask;
        int iCheckMagLayerHasTrayTask;
        int iBinDisp[TEST_MAX_BIN];
        int iInitialEmptyTrayToMagTask;
        int iNowTopBin;
        bool bEnable;
    public:
        __fastcall TMagazineModule(int iTag);
        void InitialFlag();
        int Tag;
        int iMotMGZ_X;
        int iMotMGZ_Z;

        int X_PutTrayPos;
        int X_TakeTrayPos;
        int X_PushTrayPos;
        int Z_TopPos;
        int Z_LayerPos;
        TTrayMotor *MGZLayer[eMagTotal];
        int iLayerBase;
        int iLayerRow;
        int iTrayXDivision;
        int iTrayYDivision;

        TTrayMotor *MMagSortTopTray;
        TTrayMotor *MGZTrayArm;
        TTrayMotor *MMagNowSortTray;
        TTrayMotor *MMagArmX;
        TTrayMotor *MMagArmY;
        TTrayMotor *MotEmpty2_Tray;
        TTrayMotor *MMagSortTray;

        TMySensor *Sn_MGZArmHasTray;                                            //確認Arm 是否有Tray
        TMySensor *Sn_DetectTray;                                               //檢查Tray盤是否正在搬運中
        TMySensor *Sn_MGZLayerHasTray;                                          //檢查每一層是否有Tray
        TMySensor *Sn_SidePushUp;                                               //C_MGZ_SidePush_Up 上升檢查
        TMySensor *Sn_MGZTrayFloating;                                          //檢查是否IC置偏
        TMySensor *Sn_MGZTopHasTray;                                            //檢查MGZ Top是否有Tray
        TMySensor *Sn_TrayPositionR;                                            //檢查MGZ 每一層是否有Tray 跑出來
        TMySensor *Sn_TrayPositionF;                                            //檢查MGZ 每一層是否有Tray 跑出來
        TMySensor *Sn_TrayDirection;                                            //檢查MGZ 每一層方向是否正確
        TMySensor *Sn_CatchCyOff;                                               //C_MGZ_CatchTray 退出檢查
        TMySensor *Sn_MGZStep;                                                  //MGZ按鈕_Lamp
        TMySensor *SnEmpty2HasEmptyTray;

        TMySensor *SnMag_CCDPos;

        TMySwitch *Sw_MGZStep;                                                  //MGZ按鈕
        TMySwitch *SwACEmpty2CW;

        TMyCylinder *C_MGZ_CatchTray;
        TMyCylinder *C_MGZ_EdgeFixer;
        TMyCylinder *C_MGZ_SidePush_Up;
        TMyCylinder *C_MGZ_SidePush;
        TMyCylinder *C_MGZ_LayerTrayPush;
        TMyCylinder *C_MGZ_TrayPush;
        TMyCylinder *C_MGZ_TopFixTray;

        TMyCylinder *C_Empty2_EmptyStop;
        TMyCylinder *C_Mag_CCD_Stop;

        TMyKitSuck *MagTrayArmSuck;

        void DoMagazine(int &Task);
        bool DoTrayFromTopToLayer(bool bReset=false);
        bool DoTrayFromLayerToTop(int iBin, bool bReset=false);
        bool DoTakeTrayFromTop(bool bReset=false, bool bDataTrans=true);
        bool DoPutTrayTopToLayer(bool bReset=false);
        bool DoTakeTrayFromLayer(int iBin, bool bReset=false);
        bool DoPutTrayLayerToTop(bool bReset=false, bool bFullTray=false);
        bool DoPutTrayManualToTop(bool bReset=false);

        bool bCheckMGZZCanMve();
        bool CheckAllMagHasTray();

        bool DoMGZManualTakeTray(bool bReset=false);
        bool DoInitialEmptyTrayToMag(bool bReset=false);
        bool DoTrayFromEmpty(int iBin, bool bReset=false);
        bool Find_LayerHasTray(int iTray);
        void Find_TrayLayer(TTrayMotor *Mot);
        int  GetLayerPosition(int iLayer);

        TQPF_Timer CatchOnDelay;
        TQPF_Timer CatchOffDelay;
        TQPF_Timer SidePushDelay;
        TQPF_Timer LoadTrayDelay;
        TQPF_Timer CatchTrayDelay;

        bool bDoManualMag;
        bool bMgzStepLight;
        void ManualStepBtnCanLight();

        MyBinToTrayStruct mBinToTray;
        MyBinToTrayStruct mTopToLayer;
        MyBinToTrayStruct mTopTray;
        void SetMagTopBin(int iNum);
        bool bDoManualMagFinish;
        bool bCynActOk1;
        bool bCynActOk2;
        bool bXMoveFinish;
        bool bYMoveFinish;
        bool bZMoveFinish;
        bool bMagazineXFixSpeed;
        int iRetry;
        bool TrayFeedFinish();
        void SetEnable(bool _b);
        int WhichMag(int iBin);
        //status
        int iMagArmStatus;
        MyBinToTrayStruct mTemp;    //Sam 20250602 : 修正 DoMGZManualTakeTray 三座 Magazine 資料混亂
        enum eHS_MagArm
        {
            eHS_MA_Idle=0,
            eHS_MA_NeedEmptyTray,
            eHS_MA_FullTrayAlarmFinish,
        };
};
extern TMagazineModule *MagazineModule[eMGZTotal];
extern TMagazineModule *MagazineModule1;
extern TMagazineModule *MagazineModule2;
extern TMagazineModule *MagazineModule3;
//---------------------------------------------------------------------------
#endif
