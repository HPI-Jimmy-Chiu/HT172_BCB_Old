//---------------------------------------------------------------------------
#ifndef aMagArmH
#define aMagArmH
//---------------------------------------------------------------------------

void UpdateMagArmParameter();
void InitMagArm();

class MyMagArmParameter
{
    private:
        int Tag;
        int iPlaceToTopMagTask;
        int iPickFromMagLoaderTask;
        int iTakeTrayToMagTask;
        int iCatchEmptyTrayTask;
        int iPickAndPlace2TopMagTask;
        int iBaseSuckX;
        int iBaseSuckY;
        int iActSuckC;
        int iActSuckR;
        int iTopFullTrayTask;
        int iTrayIC[eTrayCount];
        int iSearchAuto;
    public:
        __fastcall MyMagArmParameter();

        //Position
        int iPos_MagLoader_X;
        int iPos_MagLoader_Y;
        int iPos_MagLoader_Z[MAX_ARM_Row][MAX_ARM_Col];
        int iPos_MagTop_X[eMGZTotal];
        int iPos_MagTop_Y[eMGZTotal];
        int iPos_MagTop_Z[eMGZTotal][MAX_ARM_Row][MAX_ARM_Col];
        int iPos_Unload_P;

        int iPos_Y120Pitch1;
        int iPos_Y40Pitch1;
        int iPos_Y120Pitch2;
        int iPos_Y40Pitch2;
        int iPos_X60Pitch;
        int iPos_X15Pitch;

        int iPos_MagPickTray_X;
        int iPos_MagPickTray_Y;
        int iPos_MagPlaceTray_X[eMGZTotal];
        int iPos_MagPlaceTray_Y[eMGZTotal];

        int iAutoX;
        int iAutoY;
        int iStepCol;
        int iStepRow;
        int iColPitch;
        int iRowPitch;
        int iUseSuckNo;

        //Parameters
        int iTrayXDivision;
        int iTrayYDivision;
        int iTrayXPitch;
        int iTrayYPitch;
        int iTrayXStart;
        int iTrayYStart;
        int iKitPitchX;
        int iWhichAuto;
        int iWhichAutoX;
        int iWhichAutoY;
        bool bCheckMagArmDestroyActive[MAX_ARM_Row][MAX_ARM_Col];
        bool bCheckMagArmDestroyActiveFinish[MAX_ARM_Row][MAX_ARM_Col];
        TQPF_Timer CheckMagArmDestroyActiveDelay[MAX_ARM_Row][MAX_ARM_Col];
        bool bMagArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col];
        bool bMagArmCheckDestroyACTBuf[MAX_ARM_Row][MAX_ARM_Col];
        int iTask[MAX_ARM_Row][MAX_ARM_Col];
        TQPF_Timer OutArmIdle;
        bool bMagArmSuckActive[MAX_ARM_Row][MAX_ARM_Col];

//        int iOutArmYPosition;
        int iRetry;
        bool bXMoveFinish;
        bool bYMoveFinish;
        TQPF_Timer OutArmErrortimeDelay;

        bool flag;
        bool bDuplicateErr10000;
        bool bOverTray;                                                         //jou 2012-09-04 修正極限無法到達時,回填Null_IC會造成找不到地方放,會放在 x=-1,y=-1的位置上
        bool bOutArmManualStepPress;


        int iMaxSpaceOnePlace;

        TMyKitSuck *MagArmSuck;
        TTrayMotor *MMagArmX;
        TTrayMotor *MMagArmY;
        TTrayMotor *MMagArmPitchX;
        TTrayMotor *MMagArmPitchY1;
        TTrayMotor *MMagArmPitchY2;
        TTrayMotor *MMagArmZ[MAX_ARM_Row][MAX_ARM_Col];

        TTrayMotor *MMagSortTray;
        TTrayMotor *MMagNowSortTray;
        TTrayMotor *MMagSortTopTray[3];

        TTrayMotor *MagTransferLD;

        TMySensor *SnMGZ2TopHasTray;

        TMyCylinder *C_MGZ_TopFixTray;
        TMyCylinder *C_MagArm_UpDown;
        TMyCylinder *C_CatchTray;
        bool MoveMagArmXYToPickNoZDown();   //Sam 20240727 : Magazine 換 Tray 時XY提前移動

        bool bZNeedSuck[MAX_ARM_Row][MAX_ARM_Col];
        int  iZPos[MAX_ARM_Row][MAX_ARM_Col];
        bool MagArmContinuousMove(int X, int Y, int VariX, int VariY1, int VariY2, bool ZNeedDown, bool bPick); //Sam 20240607 : Suck 預吸吹功能
        bool MoveMagArmToAutoSafe();
        bool MoveMagArmXYToPick();
        bool MoveMagArmXYToPlace(int iWhichMag);
        bool DoPickICFromMagLoader(int iWhichMag, bool bReset=false);
        bool DoPlaceICToMagTop(int iWhichMag, bool bReset=false);
        bool DoMagArmPlaceToTop(int iWhichAuto, int iWhichMag);
        bool DoCatchEmptyTray(int iWhichMag, bool bReset=false);
        bool DoPnPToMagTop(int iWhichMag, bool bReset=false);
        bool DoTopFullTray(int iWhichMag, bool bReset=false);

        int  GetMagArmPitchY1(int w);
        int  GetMagArmPitchY2(int w);
        int  GetMagArmPitchX(int w);

        bool DoMoveMagArmXYToPick(int iRowPitch, int iColPitch, bool ReadMove);
        bool DoMoveMagArmXYToPlace(int iRowPitch, int iColPitch, bool ReadMove, int iWhichMag);

        bool MagArmPitchMove(int VariX, int VariY1, int VariY2, bool bInit=false);    //Steven for HT1032

        int AutoCalculateOutArmClosePitch();
        int AutoCalculateOutArmClosePitchCol(int &iStep);
        int AutoCalculateOutArmClosePitchRow(int &iStep); 

        int iMagArmXPosition;
        int iMagArmYPosition;
        bool IsOutArmToAutoSafe();

//------------------------------------------------------------------------------
        bool bYMoveOK;
        bool bZMoveOK;

        void InitialFlag();
        void InitialSuckFlag();
        void DoMagArm(int &Task);

        int iMoveToSafeTask;

        void CheckBin(int &ct);

        void InitMagArmXYMoveTask();
        int  MagArmXYMove(int iXComPos, int iYComPos);

        int MagArmDeviceDropCheck();                                            //0:OK ,1:In 異常 ,2:Out  異常  //Steven 20130808 :掉料檢查
        void CheckMagArmDestroyActive();

        int iMagTray[eTrayCount];
        int  Find_SortBinCount_Mag(int *iAutoBin, int bPreLoad=0);
        bool Find_SortBin_Mag(int iBin);
        int  Find_SortBin_Mag_Random(int &iWhichMag,bool bFindNexMag);
        int  Find_SortBin_Mag_TopReady(int &iWhichMag);                        //AI(HT172-Maintainer) 20260415 : cross-mag Top-ready priority

        bool CheckHasSpaceToPlace(int iWhichMag, int &iCount);
        bool CheckZNeedSuck(int iUseSuckNo);
        bool CheckZNeedPlace(int iUseSuckNo);

        int iMagArmMapTrayPos[MAX_ARM_Row][MAX_ARM_Col];
        void GetMapTrayPosFromEncorder(int idata, int &iRow, int &iCol);
        int GetMapTrayPosFromRowCol(int iRow, int iCol);
        bool CheckCanAction(int iWitchMGZ);
        void AddLoadingCount(int iSuckR, int iSuclC, int iAutoX, int iAutoY);   //Steven 20161123 : 將Loader吸取的數量計算統一成Function
        bool SetWorkBin(int iBin);
        bool GetWhichMag(int iBin,int &iMag);
        bool MagTopTrayHasMapBin(int &iWhichMag);
        MyBinToTrayStruct mBinToTray;
        bool bOneByOnePick;
        void Find_MagArm_Single(TTrayMotor *Motor, int &iX, int &iY);
        int Find_MagArm_PlaceMaxUseCountOnTime(TTrayMotor *Motor, int iStepRow, int iStepCol, bool bPick=false, bool bZDown=true);  //Sam 20240727 : Magazine 換 Tray 時XY提前移動
        void AdjustOutArmClosePitchCondition(bool &bCanPick2ICAtOnceTime, int &iStep);
        MyBinToTrayStruct mNowBin;

        //Sam 20240607 : Suck 預吸吹功能
        //==>
        bool bPreSuckOn;
        int iPreSuckOn;
        bool bPreSuckOff;
        int iPreSuckOff;
        bool bPreZDown;
        int iPreZDown;
        //<==
        //Sam 20240607 : Suck 預吸吹功能
        void MagArmSuckReset();

        double dDeviceConfirm;  //Sam20240627 : 修正回黏問題
        bool IsCheckMagArmDestroyActiveFinish(int iX, int iY);                  //Sam20240627 : 修正回黏問題
        bool __fastcall CheckMagArmSameBin();                                   //JerryYang 20240817 : 檢查ARM上的BIN必須相同
        bool __fastcall ManualTakeOutMagArmDevice(bool bHome=false);            //JerryYang 20240817 : 檢查ARM上的BIN必須相同
        bool __fastcall CheckMagArmBin(int iBin);                               //JerryYang 20240817 : 檢查ARM上的BIN必須相同
        bool bNeedPlaceFirst;                                                   //KenHsieh(JerryYang) 20240826 : ARM上已經有IC就先放料, 不要重新search
        //status
        int iEmptyMagStatus;

        enum eHS_EmptyMag
        {
            eHS_EM_Idle=0,
            eHS_EM_NeedTray,
            eHS_EM_WorkTrayOut,
        };

        int iMagStatus;

        enum eHS_Mag
        {
            eHS_MAG_Idle=0,
            eHS_MAG_IsReady,
            eHS_MAG_EmptyTrayReady1,
            eHS_MAG_EmptyTrayReady2,
            eHS_MAG_EmptyTrayReady3,
            eHS_MAG_FullTrayAlarm1,
            eHS_MAG_FullTrayAlarm2,
            eHS_MAG_FullTrayAlarm3,
            eHS_MAG_PlaceICTo1,
            eHS_MAG_PlaceICTo2,
            eHS_MAG_PlaceICTo3,
            eHS_MAG_SafePos,
        };
};
extern MyMagArmParameter *MagArmPara;
#endif
