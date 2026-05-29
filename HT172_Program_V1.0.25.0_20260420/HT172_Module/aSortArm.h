//---------------------------------------------------------------------------
#ifndef aSortArmH
#define aSortArmH
//---------------------------------------------------------------------------

#include "cmydef.h"
void UpdateSortArmParameter();
void InitSortArm();

class MySortArmParameter
{
    private:
        int Tag;
        int iPlaceToAutoTask;
        int iPlaceToFixTask;
        int iBaseSuckX;
        int iBaseSuckY;
        int iActSuckR;
        int iActSuckC;
        int iTrayIC[eTrayCount];
        int iSearchAuto;
    public:
        __fastcall MySortArmParameter();

        int iTrayXDivision;
        int iTrayYDivision;
        int iTrayXPitch;
        int iTrayYPitch;
        int iTrayXStart;
        int iTrayYStart;

        //Position
        int iPos_Loader_X;
        int iPos_Loader_Y;
        int iPos_Loader_Z[MAX_ARM_Row][MAX_ARM_Col];
        int iPos_Unload_X[eTrackCount][eMagTotal];
        int iPos_Unload_Y[eTrackCount][eMagTotal];
        int iPos_Unload_Z[eTrackCount][eMagTotal][MAX_ARM_Row][MAX_ARM_Col];
        int iPos_Unload_P;
        int iULPitchOffset[3][eTrackCount][eMagTotal];

        int iPos_Y120Pitch1;
        int iPos_Y40Pitch1;
        int iPos_Y120Pitch2;
        int iPos_Y40Pitch2;
        int iPos_X60Pitch;
        int iPos_X15Pitch;

        //Parameters
        int iKitPitchX;
        int iWhichAuto;
        int iWhichAutoX;
        int iWhichAutoY;
        bool bCheckSortArmDestroyActive[MAX_ARM_Row][MAX_ARM_Col];
        bool bCheckSortArmDestroyActiveFinish[MAX_ARM_Row][MAX_ARM_Col];
        TQPF_Timer CheckSortArmDestroyActiveDelay[MAX_ARM_Row][MAX_ARM_Col];
        bool bSortArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col];
        bool bSortArmCheckDestroyACTBuf[MAX_ARM_Row][MAX_ARM_Col];
        int iTask[MAX_ARM_Row][MAX_ARM_Col];
        TQPF_Timer OutArmIdle;
        bool bSortArmSuckActive[MAX_ARM_Row][MAX_ARM_Col];
        bool bSortArm1UseSuck[MAX_ARM_Row][MAX_ARM_Col];
        bool GetSortArm1NeedSuck(int iSuckR, int iSuckC);
        //
        int iRetry;
        bool bXMoveFinish;
        TQPF_Timer OutArmErrortimeDelay;
        //HTimer
        bool flag;
        bool bDuplicateErr10000;
        bool bOutArmManualStepPress;

        int iAutoX;
        int iAutoY;
        int iStepCol;
        int iStepRow;
        int iColPitch;
        int iRowPitch;
        int iUseSuckNo;

        int iMaxSpaceOnePlace;                                                  //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞
        //TMyKitSuck *OutArmSuck;
        TMyKitSuck *SortArmSuck;
        TTrayMotor *MSortArmX;
        TTrayMotor *MSortArmY;
        TTrayMotor *MSortArmPitchX;
        TTrayMotor *MSortArmPitchY1;
        TTrayMotor *MSortArmPitchY2;
        TTrayMotor *MSortArmZ[MAX_ARM_Row][MAX_ARM_Col];

        int iInputIC_Loader[eTrayCount];
        int iInputIC_LoadCar[eTrayCount];
        int iInputIC_ByLot[eTrayCount];

        TTrayMotor *MotLoader_Car;
        TTrayMotor *MotWorkLoader;
        TTrayMotor *MotNowSortTray;

        TMySensor *SnAutoTrayDetect[4];

        //function
//        bool MoveOutArmToAutoUpSafe();
        bool MoveSortArmToAutoSafe();
        bool MoveSortArmXYToPick();
        bool MoveSortArmXYToPlace();

        bool bZNeedSuck[MAX_ARM_Row][MAX_ARM_Col];
        int  iZPos[MAX_ARM_Row][MAX_ARM_Col];
        bool SortArmContinuousMove(int X, int Y, int VariX, int VariY1, int VariY2, bool ZNeedDown, bool bPick);
        bool SortArmPitchMove(int VariX, int VariY1, int VariY2, bool bInit=false);    //Steven for HT1032

        bool DoPlaceToAuto(bool bReset=false);
        bool DoPickFromLoader(bool bReset=false);
        bool DoSortArmPlaceToAuto(int iToWhichAuto);  //Steven 20110216

        int  GetSortArmPitchY1(int w);
        int  GetSortArmPitchY2(int w);
        int  GetSortArmPitchX(int w);

        int AutoCalculateSortArmClosePitch();
        int AutoCalculateSortArmClosePitchCol(int &iStep);
        int AutoCalculateSortArmClosePitchRow(int &iStep);

        int Find_SortArm_PlaceMaxUseCountOnTime(TTrayMotor *Motor, int iStepRow, int iStepCol, bool bPick=false);
        int Find_SortArm_SearchShortedPosForPick(TTrayMotor *Motor, int iStepRow, int iStepCol);
        bool DoMoveSortArmXYToPick(int iRowPitch,int iColPitch,bool ReadMove);
        bool DoMoveSortArmXYToPlace(int iRowPitch,int iColPitch,bool ReadMove);

//---------
        bool bYMoveOK;
        bool bZMoveOK;

        void InitialFlag();
        void InitialSuckFlag();
        void DoSortArm1(int &Task);

        int iPickFromLoaderTask;

        int iMoveToSafeTask;

        void CheckBin(int &ct);
        void InitSortArmXYMoveTask();
        int  SortArmXYMove(int iXComPos, int iYComPos, bool bPick);

        int OutArmDeviceDropCheck();         //0:OK ,1:In 異常 ,2:Out  異常  //Steven 20130808 :掉料檢查
        void CheckSortArmDestroyActive();
        void RecordTrayCount();

        int Find_SortBinCount_Auto(int *iAutoBin, bool bPreLoad=false);
        void Find_SortBin_Auto();

        bool CheckHasSpaceToPlace(int &iCount);
        bool CheckZNeedSuck(int iUseSuckNo);
        bool CheckZNeedPlace(int iUseSuckNo);

        int iSortArmMapTrayPos[MAX_ARM_Row][MAX_ARM_Col];
        void GetMapTrayPosFromEncorder(int idata, int &iRow, int &iCol);
        int GetMapTrayPosFromRowCol(int iRow, int iCol);
        bool FindHasAutoSortBin();
        void AddLoadingCount(int iSuckR, int iSuckC, int iAutoX, int iAutoY);   //Steven 20161123 : 將Loader吸取的數量計算統一成Function
        int iSafePosX;
        int iSafePosY;

        MyBinToTrayStruct mBinToTray;
        bool bOneByOnePick;
        void __fastcall CalculateUPH();
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
        //status
        int iStatus;
        enum eHS_SortArmStatus
        {
            eHS_Idle=0,
            eHS_Busy=1,
            eHS_NeedToTransfer=2,
        };

        bool DoMoveZUp(bool bReset=false);
        bool DoMoveZDownToTester(int Pos, bool bReset=false);
        void SortArmSuckReset();

        double dDestroyCheckTime;                                   //Sam20240627 : 修正回黏問題
        bool IsCheckSortArmDestroyActiveFinish(int iX, int iY);     //Sam20240627 : 修正回黏問題
        bool bCheckAutoTrayDetectSensor(int iWhichAuto);
        bool CheckAllAutoTrayDetectSensor(); //JerryYang 20240718 : real time 檢查AUTO TRAY
        bool __fastcall CheckSortArmSameBin();  //JerryYang 20240817 : 檢查ARM上的BIN必須相同
        bool __fastcall ManualTakeOutSortArmDevice(bool bHome=false);  //JerryYang 20240817 : 檢查ARM上的BIN必須相同
        bool __fastcall CheckSortArmBin(int iBin);
        typedef struct                                                                  //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
        {
            uPoint2D pArmXYPos;
            uPoint2D pTrayRowCol;
            int iArmXV1;
            int iArmYV1;
            int iArmYV2;
            int iWhichAuto;
            bool bZActive[MAX_ARM_Row][MAX_ARM_Col];
            int iZPosition[MAX_ARM_Row][MAX_ARM_Col];
            void Clear()
            {
                pArmXYPos.Clear();
                pTrayRowCol.Clear();
                iArmXV1=0;
                iArmYV1=0;
                iArmYV2=0;
                iWhichAuto=0;
                ZeroMemory(bZActive   , sizeof(bZActive));
                ZeroMemory(iZPosition , sizeof(iZPosition));
            }
        }ArmAndSuckInfo;
        typedef struct                                                                  //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
        {
            ArmAndSuckInfo ssTarget;
            ArmAndSuckInfo ssSource;
            uPoint2D pArmSuckActive;
            void Clear()
            {
                ssTarget.Clear();
                ssSource.Clear();
                pArmSuckActive.Clear();
            }
        }MyFillTheTrayAfterOutArmPlace;
        MyFillTheTrayAfterOutArmPlace mFillTray;                                 //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
        bool IsEnableFillTheTrayAfterOutArmPlace();                                     //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
        bool IsRunWhichAtuoFillTray(int iWhichAuto);                                    //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
        bool HasGapsInTheTray(int iWhichAuto,uPoint2D &pTargetPos,uPoint2D &pSourcePos);
        void GetWhichAutoStart(int iWhichAuto,uPoint2D &pStartPos);
        int  GetWhichAutoPickZ(int iWhichAuto,int irow,int icol);
        int  GetWhichAutoPlaceZ(int iWhichAuto,int irow,int icol);
        bool IsOutArmPosOutLimit(uPoint2D pArmPos);
        bool MoveOutArmXYAndSuck(ArmAndSuckInfo ssTarget,uPoint2D pActiveSuck);
        bool MoveOutArmXYAndSuck(ArmAndSuckInfo asInfo);
        bool IsOutArmArrival(uPoint2D pTargetPos);
        bool DoOutArmFillTheTrayAfterPlaceAllIC(bool bIsFirst,int iWhichAuto);
        bool RecTrayICStatus(TTrayMotor *myMotor,int irow,int icol,bool &bHasMidEmpty,uPoint2D &pTargetPos,uPoint2D &pSourcePos);
        bool DetermineFeasibilityOfSuck(ArmAndSuckInfo &ssp,uPoint2D pArmSuckActive);
        bool DoInArmAutoCalSuckZ(bool bReset);
        TQPF_Timer SortArmDelay;
        int iDoSortArmAutoCalSuckZTask;
        int iDoSuckZAllHomeTask;
        bool DoSuckZAllHome(bool bReset);
        bool bSortArmZHome[MAX_SUCKER_ROW][MAX_SUCKER_COL];
        int iInArmZVacuumbufferPos[MAX_SUCKER_ROW][MAX_SUCKER_COL];            //Z軸吸取高度
        int iInArmZHeightDiff[MAX_SUCKER_ROW][MAX_SUCKER_COL];
        int GetNextAutoArea(int inowAuto);
};
extern MySortArmParameter *SortArmPara;

#endif

