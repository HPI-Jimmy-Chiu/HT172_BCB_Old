//------------------------------------------------------------------------------
#ifndef aTrayArmH
#define aTrayArmH
//------------------------------------------------------------------------------
#include "cmydef.h"
void UpdateTrayArmParameter();
void InitTrayArm();

class MyTrayArmParameter
{
    private:
        int Tag;
        int iLoaderTrayToStorageTask;
        bool DoLoaderTrayToStorage(bool bReset=false);

    public:
        __fastcall MyTrayArmParameter();

        //Position
        int iPos_Storage_X[eTrackCount][eTrackColTotal];
        int iPos_Storage_Y[eTrackCount][eTrackColTotal];
        int iPos_Storage_Z[eTrackCount][eTrackColTotal];

        int iPos_Track_X[eTrackCount];
        int iPos_Track_Y[eTrackCount];
        int iPos_Track_Z[eTrackCount];

        int iPush_Track_X[eTrackCount];                                         //推Tray 機構改為馬達

        int iPos_Storage_Z_Up;

        int iPos_Y120Pitch;
        int iPos_Y30Pitch;
        int iPos_X75Pitch;
        int iPos_X25Pitch;

        //Parameters
        HTimer TrayArmDelay;
        HTimer tTrayToStoreTimerOut;    //Sam 20240618 : auto to store track timeout
        TTrayMotor *MStoreArmX        ;
        TTrayMotor *MStoreArmY        ;
        TTrayMotor *MStoreArmZ        ;

        TTrayMotor *MPushStoreArmX[eTrackCount] ;                               //推Tray 機構改為馬達
        TTrayMotor *MtTrayArmTray;
        TTrayMotor *MCarrierStoreX[eTrackCount]     ;
        TTrayMotor *MCarrierStoreZ[eTrackCount]     ;
        TMyCylinder *C_AutoTrolly_Stop[eTrackCount] ;
        int iPush_AutoTrolly_Track_X[eTrackCount]   ;
        int iPush_AutoTrolly_ZDown[eTrackCount];

        TMyCylinder *C_Track_Stop[eTrackCount]  ;
        TMySwitch   *Track_CW[eTrackCount]      ;
        TMySwitch   *Track_CCW[eTrackCount]     ;
        TMyCylinder *C_Track_PushTray[eTrackCount];
        TMySwitch   *Track_Dete[eTrackCount]    ;
        TMySwitch   *AutoRowVib1[eTrackCount]   ;
        TMySwitch   *AutoRowVib2[eTrackCount]   ;

        TMySensor *SnStoreArmTakeTray ;
        TMySensor *SnStoreArmHasTray  ;

        TMySensor *SnStoreArmPutPos1  ;
        TMySensor *SnStoreArmPutPos2  ;

        TMySensor *Sen_Store_Full[eTrackCount][eTrackColTotal];
        TMySensor *Sen_Store_Door[eTrackCount][eTrackColTotal];
        TMySensor *Sen_AutoRowULDCheck[eTrackCount];
        TMySensor *Sen_PushTrayCheck[eTrackCount];                              //推Tray 機構改為馬達
        TMySensor *Sen_TrackCyPushTray[eTrackCount];                            //推Tray 機構改為馬達

        TPanel    *Auto_Store[eTrackCount][eTrackColTotal];

        TMyCylinder *C_StoreArm_EdgeFixer;
        TMyKitSuck  *TrayArmSuck       ;

        void DoStoreArm(int &Task);

        TQPF_Timer NewLoopEdgeDelay;
        TQPF_Timer NewLoopEdgeOnDelay;
        TQPF_Timer PushTrayCyDelay[eTrackCount];                                //推Tray 機構改為馬達
        TQPF_Timer PushTrayMotDelay[eTrackCount];                               //推Tray 機構改為馬達
        TQPF_Timer PushTrayHomeDelay;                                           //KenHsieh 20240830 : add delay判斷推Tray 馬達SnHome
        void NewDoAutoTrayEdgeCylinderLoop();
        void ClearTrayCount();

        MyBinToTrayStruct mBinToTray;
        bool InitialPushTray(bool bReset, int iNowTrack);                       //推Tray 機構改為馬達
        bool TrayArmToTrackNo(int iTrackNum,AnsiString sFunc,int iTask);
        enum TrayArmXPos
        {
            taxp_Standby=0,
            taxp_TrackReceive=1,
            taxp_Storage=2,
        };
        bool DoMoveTrayArmX(int iPos,int iTrackNum=-1,int iAutoTray=-1);
        bool DoMoveTrayArmToStorageStandby(int iTrackNum,int iAutoTray);
        bool DoMoveTrayArmZInStorage(bool bUp,int iTrackNum,int iAutoTray);
        void ShowTrayArmMessage(AnsiString sMsg,AnsiString sFunc,int iTask);
        int GetStorageX(int iTrackNum,int iAutoTray);
        int GetStorageY(int iTrackNum,int iAutoTray);
        int GetStorageZ(int iTrackNum,int iAutoTray);
        bool IsTrayAutoInRange(int iTrackNum,int iAutoTray);
        int iNowAutoTray;
        int iNowTrack;
        int iTrayCount;
        bool bTrayArmTrayFeedFinish;
};
extern MyTrayArmParameter *TrayArmPara;
#endif
