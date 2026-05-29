//---------------------------------------------------------------------------
#ifndef databaseH
#define databaseH
//---------------------------------------------------------------------------
#include "SPComm.hpp"
#include <ActnList.hpp>
#include <Classes.hpp>
#include <ExtCtrls.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <comctrls.hpp>
                              
#include "cprod.h"
#include "MyMotor.h"
//#include "myio_ISA.h"
#include "mycylin.h"
#include "mysensor.h"
#include "myswitch.h"
#include "MyKitSuck.h"
#include "uHGemClass.h"
#include "MyBinDisp.h"
#include "cmydef.h"

enum eIOType{eMotionNet=0,
             eISABase  =1,
             ePCI1735U =2,
             ePCI1203  =3,
             e_PLCbase  =4
             };

typedef struct TIODATA
{
    int Tag;
    AnsiString _CommaText;
    AnsiString Type;
    AnsiString Alias;
    AnsiString IOPos;
    AnsiString sIP;
    int iLane;
    int iModuleType;
    int iIP;
    int iPort;
    int iBit;
    int iInType;
    int iISABase;
    int iEnable;
    int iOnAlarmTime;
    int iOffAlarmTime;
    int iOnDelayTime;
    int iOffDelayTime;
    bool bDelete;

    TIODATA(AnsiString Str=AnsiString(""));
}TIODATA;
//---------------------------------------------------------------------------
typedef struct TIOTABLENO
{
    int eioType;
    int eioAlias;
    int eioLane;
    int eioModuleType;
    int eioIP;
    int eioPort;
    int eioBit;
    int eioInType;
    int eioISABase;
    int eioEnable;
    int eioOnAlarmTime;
    int eioOffAlarmTime;
    int eioOnDelayTime;
    int eioOffDelayTime;
    int eioNote;
    int eioTotal;
    int SetIOTableNo(AnsiString Str);

    TIOTABLENO();
}TIOTABLENO;
//---------------------------------------------------------------------------
typedef struct TMOTDATA
{
    AnsiString _CommaText;
    AnsiString No;
    AnsiString Alias;
    int iEnable;
    int iBoardID;
    int iPort;
    double dGearRatio;
    int iSoftLimitN;
    int iSoftLimitP;
    AnsiString CardModel;
    int MotorKind;
    AnsiString FlushPanel;
    AnsiString HomeOrder;

    int iDirection;
    int iHomeDirectior;
    double dAcc;
    double dDec;
    int iInitSpeed;
    int iHomeHighSpeed;
    int iHomeLowSpeed;
    int iJogHighSpeed;
    int iJogLowSpeed;
    int iRange;
    int iRate;
    int iServoAlarmOn;
    int i1P2P;
    int iSensorType;
    int iEncodeType;
    int iLimitLogic;
    int iIn1Logic;
    int iSimulateSpeed;

    TMOTDATA(AnsiString Str=AnsiString(""));
}TMOTDATA;
//---------------------------------------------------------------------------
typedef struct TMOTNO
{
    int emotNo;
    int emotAlias;
    int emotDirection;
    int emotGearRatio;
    int emotHomeDirectior;
    int emotHomeHighSpeed;
    int emotHomeLowSpeed;
    int emotInitSpeed;
    int emotJogHighSpeed;
    int emotJogLowSpeed;
    int emotRate;
    int emotSoftLimitN;
    int emotSoftLimitP;
    int emotEnable;
    int emotServoAlarmOn;
    int emotRange;
    int emot1P2P;
    int emotSensorType;
    int emotSimulateSpeed;
    int emotCardModel;
    int emotBoardID;
    int emotPort;
    int emotAcc;
    int emotDec;
    int emotMotorKind;
    int emoFlushPanel;
    int emoHomeOrder;
    int emotLimitLogic;
    int emotIn1Logic;
    int emotTotal;

    TMOTNO();
    int SetMOTTableNo(AnsiString Str);
}TMOTNO;
//---------------------------------------------------------------------------
class TDataModule1 : public TDataModule
{
__published:    // IDE-managed Components
    TActionList *UserActionList;
    TAction *SpecificSetupForMotorParameter;
    TAction *InitialCylinderName;
    TAction *InitialMotorName;
    TAction *InitialSensorName;
    TAction *InitialSwitchName;
    TAction *SpecificSetupForSwitchParameter;
    TAction *SpecificSetupForSuckerParameter;
    TAction *SpecificSetupForCylinderParameter;
    TAction *SpecificSetupForSensorParameter;
    TAction *InitialSuckerName;
    TAction *Initial_IO_Setup;
    TActionList *UserMotion;
    TTimer *Timer1;
    TAction *SortArm1;
    TAction *Loader;
    TAction *SortArm2;
    TAction *ActionMag2;
    TAction *ActionStoreArm;
    TAction *ActionEmpty1;
    TAction *ActionEmpty1_Track;
    TAction *ActionEmpty2;
    TAction *ActionMag3;
    TAction *ActionMag1;
    TAction *ActionTrack1;
    TAction *ActionTrack2;
    TAction *ActionTrack3;
    TAction *ActionTrack4;
    void __fastcall SpecificSetupForMotorParameterExecute(TObject *Sender);
    void __fastcall InitialCylinderNameExecute(TObject *Sender);
    void __fastcall InitialMotorNameExecute(TObject *Sender);
    void __fastcall InitialSwitchNameExecute(TObject *Sender);
    void __fastcall InitialSensorNameExecute(TObject *Sender);
    void __fastcall SpecificSetupForSwitchParameterExecute(
          TObject *Sender);
    void __fastcall SpecificSetupForSuckerParameterExecute(
          TObject *Sender);
    void __fastcall SpecificSetupForCylinderParameterExecute(
          TObject *Sender);
    void __fastcall SpecificSetupForSensorParameterExecute(
          TObject *Sender);
    void __fastcall Initial_IO_SetupExecute(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall InitialSuckerNameExecute(TObject *Sender);
    void __fastcall SortArm1Execute(TObject *Sender);
    void __fastcall LoaderExecute(TObject *Sender);
    void __fastcall ActionEmpty1Execute(TObject *Sender);
    void __fastcall ActionEmpty1_TrackExecute(TObject *Sender);
    void __fastcall ActionEmpty2Execute(TObject *Sender);
    void __fastcall ActionStoreArmExecute(TObject *Sender);
    void __fastcall ActionMag2Execute(TObject *Sender);
    void __fastcall SortArm2Execute(TObject *Sender);
    void __fastcall ActionTrack1Execute(TObject *Sender);
    void __fastcall ActionTrack2Execute(TObject *Sender);
    void __fastcall ActionTrack3Execute(TObject *Sender);
    void __fastcall ActionTrack4Execute(TObject *Sender);
    void __fastcall ActionMag1Execute(TObject *Sender);
    void __fastcall ActionMag3Execute(TObject *Sender);

private:    // User declarations
public:     // User declarations
    __fastcall TDataModule1(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule1 *DataModule1;
//---------------------------------------------------------------------------
//==============================================================================
//  真實馬達集合 _
//  使用者依不同機器修改 _
//==============================================================================
typedef struct MOTOR_MODULAR_STRUCT                                             // need edit by user //
{
    TTrayMotor *SortArm1X       ;
    TTrayMotor *SortArm1Y       ;
    TTrayMotor *SortArm1ZA      ;
    TTrayMotor *SortArm1ZB      ;
    TTrayMotor *SortArm1ZC      ;
    TTrayMotor *SortArm1ZD      ;
    TTrayMotor *SortArm1ZE      ;
    TTrayMotor *SortArm1ZF      ;
    TTrayMotor *SortArm1ZG      ;
    TTrayMotor *SortArm1ZH      ;
    TTrayMotor *SortArm1PitchX  ;
    TTrayMotor *SortArm1PitchY1 ;
    TTrayMotor *SortArm1PitchY2 ;
    TTrayMotor *LoaderCCDArmX   ;
    TTrayMotor *StoreArmX       ;
    TTrayMotor *StoreArmY       ;
    TTrayMotor *StoreArmZ       ;
    TTrayMotor *TrayShiftStageX ;
    TTrayMotor *SortArm2X       ;
    TTrayMotor *SortArm2Y       ;
    TTrayMotor *SortArm2ZA      ;
    TTrayMotor *SortArm2ZB      ;
    TTrayMotor *SortArm2ZC      ;
    TTrayMotor *SortArm2ZD      ;
    TTrayMotor *SortArm2ZE      ;
    TTrayMotor *SortArm2ZF      ;
    TTrayMotor *SortArm2ZG      ;
    TTrayMotor *SortArm2ZH      ;
    TTrayMotor *SortArm2PitchX  ;
    TTrayMotor *SortArm2PitchY1 ;
    TTrayMotor *SortArm2PitchY2 ;
    TTrayMotor *Magazine1X      ;
    TTrayMotor *Magazine1Z      ;
    TTrayMotor *Magazine2X      ;
    TTrayMotor *Magazine2Z      ;
    TTrayMotor *Magazine3X      ;
    TTrayMotor *Magazine3Z      ;
    TTrayMotor *PushStoreArmX1  ;                                               //推Tray 機構改為馬達
    TTrayMotor *PushStoreArmX2  ;                                               //推Tray 機構改為馬達
    TTrayMotor *PushStoreArmX3  ;                                               //推Tray 機構改為馬達
    TTrayMotor *PushStoreArmX4  ;                                               //推Tray 機構改為馬達

    TTrayMotor *AutoTrolly1Z    ;
    TTrayMotor *AutoTrolly2Z    ;
    TTrayMotor *AutoTrolly3Z    ;
    TTrayMotor *AutoTrolly4Z    ;
    TTrayMotor *AutoTrolly1X    ;
    TTrayMotor *AutoTrolly2X    ;
    TTrayMotor *AutoTrolly3X    ;
    TTrayMotor *AutoTrolly4X    ;
}MOTOR_MODULAR;
//==============================================================================
//  虛擬馬達集合 _
//  使用者依不同機器修改 _
//==============================================================================
typedef struct VIRTUAL_MOTOR_MODULAR_STRUCT                                     // need edit by user //
{
    TTrayMotor *tmAutoAreaAuto[eTrayCount];
    TTrayMotor *LoadTray_Car    ;

//    TTrayMotor *Loader_Up       ;
    TTrayMotor *MotWorkLoader   ;
    TTrayMotor *NowSortTray     ;

    TTrayMotor *Mag_LD_Car      ;
    TTrayMotor *Mag_LD_Work_Y   ;
    TTrayMotor *Mag_NowSortTray ;
    TTrayMotor *MMag1SortTopTray ;
    TTrayMotor *MMag2SortTopTray ;
    TTrayMotor *MMag3SortTopTray ;

    TTrayMotor *MagEmptyTray    ;

    TTrayMotor *Empty_1         ;
    TTrayMotor *Empty_2         ;
    TTrayMotor *Track_1         ;
    TTrayMotor *Track_2         ;
    TTrayMotor *Track_3         ;
    TTrayMotor *Track_4         ;

    TTrayMotor *Track_1_TrayArm ;
    TTrayMotor *Track_2_TrayArm ;
    TTrayMotor *Track_3_TrayArm ;
    TTrayMotor *Track_4_TrayArm ;

    TTrayMotor *Transfer_LoaderY;
    TTrayMotor *MagazineTrayArm[eMGZTotal];
    TTrayMotor *TrayArmTray;

}VIRTUAL_MOTOR_MODULAR;
//==============================================================================
//  真實汽缸集合 _
//  使用者依不同機器修改 _
//==============================================================================
typedef struct CYLINDER_MODULAR_STRUCR                                          // need edit by user //
{
    TMyCylinder C_LoaderEdgePush      ;
    TMyCylinder C_TrayY_Fixer         ;
    TMyCylinder C_Load_Middle         ;
    TMyCylinder C_Load_Up             ;
    TMyCylinder C_TrayZ_Selector      ;
    TMyCylinder C_Loader_Stop         ;
    TMyCylinder C_Loader_CCDStop      ;
    TMyCylinder C_Empty1_Middle       ;
    TMyCylinder C_Empty1_Up           ;
    TMyCylinder C_Empty1Z_Selector    ;
    TMyCylinder C_Empty1_Stop1        ;
    TMyCylinder C_Empty1_Stop2        ;
    TMyCylinder C_Empty1_Stop3        ;
    TMyCylinder C_Empty1_Stop4        ;
    TMyCylinder C_Empty1_PushTray1    ;
    TMyCylinder C_Empty1_PushTray2    ;
    TMyCylinder C_Empty1_PushTray3    ;
    TMyCylinder C_Empty1_PushTray4    ;
    TMyCylinder C_Empty1_CCDStop      ;
    TMyCylinder C_Empty2_Middle       ;
    TMyCylinder C_Empty2_Up           ;
    TMyCylinder C_Empty2Z_Selector    ;
    TMyCylinder C_Empty2EdgePush      ;
    TMyCylinder C_Empty2_Fixer        ;
    TMyCylinder C_Empty2_Stop         ;
    TMyCylinder C_Empty2_EmptyStop    ;
    TMyCylinder C_Empty2_CCDStop      ;
    TMyCylinder C_ShiftStageEdgePush  ;
    TMyCylinder C_Auto1_InclinePush   ;
    TMyCylinder C_Auto1_AntiDrop      ;
    TMyCylinder C_Auto2_InclinePush   ;
    TMyCylinder C_Auto2_AntiDrop      ;
    TMyCylinder C_Auto3_InclinePush   ;
    TMyCylinder C_Auto3_AntiDrop      ;
    TMyCylinder C_Auto4_InclinePush   ;
    TMyCylinder C_Auto4_AntiDrop      ;
    TMyCylinder C_Auto5_InclinePush   ;
    TMyCylinder C_Auto5_AntiDrop      ;
    TMyCylinder C_Auto6_InclinePush   ;
    TMyCylinder C_Auto6_AntiDrop      ;
    TMyCylinder C_Auto7_InclinePush   ;
    TMyCylinder C_Auto7_AntiDrop      ;
    TMyCylinder C_Auto8_InclinePush   ;
    TMyCylinder C_Auto8_AntiDrop      ;
    TMyCylinder C_Auto9_InclinePush   ;
    TMyCylinder C_Auto9_AntiDrop      ;
    TMyCylinder C_Auto10_InclinePush  ;
    TMyCylinder C_Auto10_AntiDrop     ;
    TMyCylinder C_Auto11_InclinePush  ;
    TMyCylinder C_Auto11_AntiDrop     ;
    TMyCylinder C_Auto12_InclinePush  ;
    TMyCylinder C_Auto12_AntiDrop     ;
    TMyCylinder C_Auto13_InclinePush  ;
    TMyCylinder C_Auto13_AntiDrop     ;
    TMyCylinder C_Auto14_InclinePush  ;
    TMyCylinder C_Auto14_AntiDrop     ;
    TMyCylinder C_Auto15_InclinePush  ;
    TMyCylinder C_Auto15_AntiDrop     ;
    TMyCylinder C_Auto16_InclinePush  ;
    TMyCylinder C_Auto16_AntiDrop     ;
    TMyCylinder C_Auto17_InclinePush  ;
    TMyCylinder C_Auto17_AntiDrop     ;
    TMyCylinder C_Auto18_InclinePush  ;
    TMyCylinder C_Auto18_AntiDrop     ;
    TMyCylinder C_Auto19_InclinePush  ;
    TMyCylinder C_Auto19_AntiDrop     ;
    TMyCylinder C_Auto20_InclinePush  ;
    TMyCylinder C_Auto20_AntiDrop     ;
    TMyCylinder C_Auto1_Stop          ;
    TMyCylinder C_Auto2_Stop          ;
    TMyCylinder C_Auto3_Stop          ;
    TMyCylinder C_Auto4_Stop          ;
    TMyCylinder C_Auto5_Stop          ;
    TMyCylinder C_Auto6_Stop          ;
    TMyCylinder C_Auto7_Stop          ;
    TMyCylinder C_Auto8_Stop          ;
    TMyCylinder C_Auto9_Stop          ;
    TMyCylinder C_Auto10_Stop         ;
    TMyCylinder C_Auto11_Stop         ;
    TMyCylinder C_Auto12_Stop         ;
    TMyCylinder C_Auto13_Stop         ;
    TMyCylinder C_Auto14_Stop         ;
    TMyCylinder C_Auto15_Stop         ;
    TMyCylinder C_Auto16_Stop         ;
    TMyCylinder C_Auto17_Stop         ;
    TMyCylinder C_Auto18_Stop         ;
    TMyCylinder C_Auto19_Stop         ;
    TMyCylinder C_Auto20_Stop         ;
    TMyCylinder C_Auto1_Zup           ;
    TMyCylinder C_Auto2_Zup           ;
    TMyCylinder C_Auto3_Zup           ;
    TMyCylinder C_Auto4_Zup           ;
    TMyCylinder C_Auto5_Zup           ;
    TMyCylinder C_Auto6_Zup           ;
    TMyCylinder C_Auto7_Zup           ;
    TMyCylinder C_Auto8_Zup           ;
    TMyCylinder C_Auto9_Zup           ;
    TMyCylinder C_Auto10_Zup          ;
    TMyCylinder C_Auto11_Zup          ;
    TMyCylinder C_Auto12_Zup          ;
    TMyCylinder C_Auto13_Zup          ;
    TMyCylinder C_Auto14_Zup          ;
    TMyCylinder C_Auto15_Zup          ;
    TMyCylinder C_Auto16_Zup          ;
    TMyCylinder C_Auto17_Zup          ;
    TMyCylinder C_Auto18_Zup          ;
    TMyCylinder C_Auto19_Zup          ;
    TMyCylinder C_Auto20_Zup          ;
    TMyCylinder C_AutoRow1_Stop       ;
    TMyCylinder C_AutoRow2_Stop       ;
    TMyCylinder C_AutoRow3_Stop       ;
    TMyCylinder C_AutoRow4_Stop       ;
    TMyCylinder C_AutoRow1_PushTray   ;
    TMyCylinder C_AutoRow2_PushTray   ;
    TMyCylinder C_AutoRow3_PushTray   ;
    TMyCylinder C_AutoRow4_PushTray   ;
    TMyCylinder C_StoreArm_EdgeFixer  ;
    TMyCylinder C_MGZ1_CatchTray      ;
    TMyCylinder C_MGZ1_EdgeFixer      ;
    TMyCylinder C_MGZ1_SidePush_Up    ;
    TMyCylinder C_MGZ1_SidePush       ;
    TMyCylinder C_MGZ1_LayerTrayPush  ;
    TMyCylinder C_MGZ1_TrayPush       ;
    TMyCylinder C_MGZ1_TopFixTray     ;
    TMyCylinder C_MGZ2_CatchTray      ;
    TMyCylinder C_MGZ2_EdgeFixer      ;
    TMyCylinder C_MGZ2_SidePush_Up    ;
    TMyCylinder C_MGZ2_SidePush       ;
    TMyCylinder C_MGZ2_LayerTrayPush  ;
    TMyCylinder C_MGZ2_TrayPush       ;
    TMyCylinder C_MGZ2_TopFixTray     ;
    TMyCylinder C_MGZ3_CatchTray      ;
    TMyCylinder C_MGZ3_EdgeFixer      ;
    TMyCylinder C_MGZ3_SidePush_Up    ;
    TMyCylinder C_MGZ3_SidePush       ;
    TMyCylinder C_MGZ3_LayerTrayPush  ;
    TMyCylinder C_MGZ3_TrayPush       ;
    TMyCylinder C_MGZ3_TopFixTray     ;
    TMyCylinder C_SortArm2_UpDown     ;
    TMyCylinder C_CatchTray_Fix       ;

    TMyCylinder C_AutoTrolly1_Fixer   ;
    TMyCylinder C_AutoTrolly2_Fixer   ;
    TMyCylinder C_AutoTrolly3_Fixer   ;
    TMyCylinder C_AutoTrolly4_Fixer   ;
    TMyCylinder C_AutoTrolly1_Stop    ;
    TMyCylinder C_AutoTrolly2_Stop    ;
    TMyCylinder C_AutoTrolly3_Stop    ;
    TMyCylinder C_AutoTrolly4_Stop    ;

}CYLINDER_MODULAR;
//==============================================================================
//  真實Sensor集合 _
//  使用者依不同機器修改 _
//==============================================================================
typedef struct SENSOR_MODULAR_STRUCR                                            // need edit by user //
{
    TMySensor SnFKPowerOff           ;
    TMySensor SnFKPowerOn            ;
    TMySensor SnFKReset              ;
    TMySensor SnFKPause              ;
    TMySensor SnFKHome               ;
    TMySensor SnFKStart              ;
    TMySensor SnFKOneCycle           ;
    TMySensor SnFKRetry              ;
    TMySensor SnFKSkip               ;
    TMySensor SnFKCleanOut           ;
    TMySensor SnFKTrayFeed           ;
    TMySensor SnFKTrayEnd            ;
    TMySensor SnFKAlarmReset         ;
    TMySensor SnFKCoverOpen          ;
    TMySensor SnRKPowerOff           ;
    TMySensor SnRKPowerOn            ;
    TMySensor SnRKReset              ;
    TMySensor SnRKPause              ;
    TMySensor SnRKHome               ;
    TMySensor SnRKStart              ;
    TMySensor SnRKOneCycle           ;
    TMySensor SnRKRetry              ;
    TMySensor SnRKSkip               ;
    TMySensor SnRKCleanOut           ;
    TMySensor SnRKTrayFeed           ;
    TMySensor SnRKTrayEnd            ;
    TMySensor SnRKAlarmReset         ;
    TMySensor SnRKCoverOpen          ;
    TMySensor SnRKManualStep         ;
    TMySensor SnRKManualTStart       ;
    TMySensor SnLoaderIsFull         ;
    TMySensor SnLoaderTrayHasTray    ;
    TMySensor SnLoaderCarHasTray     ;
    TMySensor SnLoaderCCDPos         ;
    TMySensor SnLoaderFixCyPush      ;
    TMySensor SnLoaderEdgePush       ;
    TMySensor SnLoaderPreDete        ;
    TMySensor SnLoaderSureTray       ;
    TMySensor SnEmpty1IsFull         ;
    TMySensor SnEmpty1SelectHasTray  ;
    TMySensor SnEmpty1CarHasTray     ;
    TMySensor SnEmpty1CCDPosition    ;
    TMySensor SnEmpty1HasTray1       ;
    TMySensor SnEmpty1FCyPushTray1   ;
    TMySensor SnEmpty1RCyPushTray1   ;
    TMySensor SnEmpty1HasTray2       ;
    TMySensor SnEmpty1FCyPushTray2   ;
    TMySensor SnEmpty1RCyPushTray2   ;
    TMySensor SnEmpty1HasTray3       ;
    TMySensor SnEmpty1FCyPushTray3   ;
    TMySensor SnEmpty1RCyPushTray3   ;
    TMySensor SnEmpty1HasTray4       ;
    TMySensor SnEmpty1FCyPushTray4   ;
    TMySensor SnEmpty1RCyPushTray4   ;
    TMySensor SnEmpty2IsFull         ;
    TMySensor SnEmpty2SelectHasTray  ;
    TMySensor SnEmpty2CarHasTray     ;
    TMySensor SnEmpty2CCDPosition    ;
    TMySensor SnEmpty2HasTray        ;
    TMySensor SnEmpty2CCWDete        ;
    TMySensor SnEmpty2FixCyPush      ;
    TMySensor SnEmpty2EdgePush       ;
    TMySensor SnEmpty2HasEmptyTray   ;
    TMySensor SnShiftStageHasTray    ;
    TMySensor SnShiftStageCWPreDete  ;
    TMySensor SnShiftStageEdgePush   ;
    TMySensor SnAuto1HasTray         ;
    TMySensor SnAuto2HasTray         ;
    TMySensor SnAuto3HasTray         ;
    TMySensor SnAuto4HasTray         ;
    TMySensor SnAuto5HasTray         ;
    TMySensor SnAuto6HasTray         ;
    TMySensor SnAuto7HasTray         ;
    TMySensor SnAuto8HasTray         ;
    TMySensor SnAuto9HasTray         ;
    TMySensor SnAuto10HasTray        ;
    TMySensor SnAuto11HasTray        ;
    TMySensor SnAuto12HasTray        ;
    TMySensor SnAuto13HasTray        ;
    TMySensor SnAuto14HasTray        ;
    TMySensor SnAuto15HasTray        ;
    TMySensor SnAuto16HasTray        ;
    TMySensor SnAuto17HasTray        ;
    TMySensor SnAuto18HasTray        ;
    TMySensor SnAuto19HasTray        ;
    TMySensor SnAuto20HasTray        ;
    TMySensor SnAuto1CyAntiDrop1     ;
    TMySensor SnAuto1CyAntiDrop2     ;
    TMySensor SnAuto1CyAntiDrop3     ;
    TMySensor SnAuto2CyAntiDrop1     ;
    TMySensor SnAuto2CyAntiDrop2     ;
    TMySensor SnAuto2CyAntiDrop3     ;
    TMySensor SnAuto3CyAntiDrop1     ;
    TMySensor SnAuto3CyAntiDrop2     ;
    TMySensor SnAuto3CyAntiDrop3     ;
    TMySensor SnAuto4CyAntiDrop1     ;
    TMySensor SnAuto4CyAntiDrop2     ;
    TMySensor SnAuto4CyAntiDrop3     ;
    TMySensor SnAuto5CyAntiDrop1     ;
    TMySensor SnAuto5CyAntiDrop2     ;
    TMySensor SnAuto5CyAntiDrop3     ;
    TMySensor SnAuto6CyAntiDrop1     ;
    TMySensor SnAuto6CyAntiDrop2     ;
    TMySensor SnAuto6CyAntiDrop3     ;
    TMySensor SnAuto7CyAntiDrop1     ;
    TMySensor SnAuto7CyAntiDrop2     ;
    TMySensor SnAuto7CyAntiDrop3     ;
    TMySensor SnAuto8CyAntiDrop1     ;
    TMySensor SnAuto8CyAntiDrop2     ;
    TMySensor SnAuto8CyAntiDrop3     ;
    TMySensor SnAuto9CyAntiDrop1     ;
    TMySensor SnAuto9CyAntiDrop2     ;
    TMySensor SnAuto9CyAntiDrop3     ;
    TMySensor SnAuto10CyAntiDrop1    ;
    TMySensor SnAuto10CyAntiDrop2    ;
    TMySensor SnAuto10CyAntiDrop3    ;
    TMySensor SnAuto11CyAntiDrop1    ;
    TMySensor SnAuto11CyAntiDrop2    ;
    TMySensor SnAuto11CyAntiDrop3    ;
    TMySensor SnAuto12CyAntiDrop1    ;
    TMySensor SnAuto12CyAntiDrop2    ;
    TMySensor SnAuto12CyAntiDrop3    ;
    TMySensor SnAuto13CyAntiDrop1    ;
    TMySensor SnAuto13CyAntiDrop2    ;
    TMySensor SnAuto13CyAntiDrop3    ;
    TMySensor SnAuto14CyAntiDrop1    ;
    TMySensor SnAuto14CyAntiDrop2    ;
    TMySensor SnAuto14CyAntiDrop3    ;
    TMySensor SnAuto15CyAntiDrop1    ;
    TMySensor SnAuto15CyAntiDrop2    ;
    TMySensor SnAuto15CyAntiDrop3    ;
    TMySensor SnAuto16CyAntiDrop1    ;
    TMySensor SnAuto16CyAntiDrop2    ;
    TMySensor SnAuto16CyAntiDrop3    ;
    TMySensor SnAuto17CyAntiDrop1    ;
    TMySensor SnAuto17CyAntiDrop2    ;
    TMySensor SnAuto17CyAntiDrop3    ;
    TMySensor SnAuto18CyAntiDrop1    ;
    TMySensor SnAuto18CyAntiDrop2    ;
    TMySensor SnAuto18CyAntiDrop3    ;
    TMySensor SnAuto19CyAntiDrop1    ;
    TMySensor SnAuto19CyAntiDrop2    ;
    TMySensor SnAuto19CyAntiDrop3    ;
    TMySensor SnAuto20CyAntiDrop1    ;
    TMySensor SnAuto20CyAntiDrop2    ;
    TMySensor SnAuto20CyAntiDrop3    ;
    TMySensor SnAuto1CarHasTray      ;
    TMySensor SnAuto2CarHasTray      ;
    TMySensor SnAuto3CarHasTray      ;
    TMySensor SnAuto4CarHasTray      ;
    TMySensor SnAuto5CarHasTray      ;
    TMySensor SnAuto6CarHasTray      ;
    TMySensor SnAuto7CarHasTray      ;
    TMySensor SnAuto8CarHasTray      ;
    TMySensor SnAuto9CarHasTray      ;
    TMySensor SnAuto10CarHasTray     ;
    TMySensor SnAuto11CarHasTray     ;
    TMySensor SnAuto12CarHasTray     ;
    TMySensor SnAuto13CarHasTray     ;
    TMySensor SnAuto14CarHasTray     ;
    TMySensor SnAuto15CarHasTray     ;
    TMySensor SnAuto16CarHasTray     ;
    TMySensor SnAuto17CarHasTray     ;
    TMySensor SnAuto18CarHasTray     ;
    TMySensor SnAuto19CarHasTray     ;
    TMySensor SnAuto20CarHasTray     ;
    TMySensor SnAutoRow1HasTray      ;
    TMySensor SnAutoRow2HasTray      ;
    TMySensor SnAutoRow3HasTray      ;
    TMySensor SnAutoRow4HasTray      ;
    TMySensor SnAutoRow1SlowDown     ;
    TMySensor SnAutoRow2SlowDown     ;
    TMySensor SnAutoRow3SlowDown     ;
    TMySensor SnAutoRow4SlowDown     ;
    TMySensor SnStoreArmTakeTray     ;
    TMySensor SnStoreArmHasTray      ;
    TMySensor SnStoreArmTakePos      ;
    TMySensor SnStoreArmPutPos1      ;
    TMySensor SnStoreArmPutPos2      ;
    TMySensor SnStore1Full           ;
    TMySensor SnStore1Door           ;
    TMySensor SnStore2Full           ;
    TMySensor SnStore2Door           ;
    TMySensor SnStore3Full           ;
    TMySensor SnStore3Door           ;
    TMySensor SnStore4Full           ;
    TMySensor SnStore4Door           ;
    TMySensor SnStore5Full           ;
    TMySensor SnStore5Door           ;
    TMySensor SnStore6Full           ;
    TMySensor SnStore6Door           ;
    TMySensor SnStore7Full           ;
    TMySensor SnStore7Door           ;
    TMySensor SnStore8Full           ;
    TMySensor SnStore8Door           ;
    TMySensor SnStore9Full           ;
    TMySensor SnStore9Door           ;
    TMySensor SnStore10Full          ;
    TMySensor SnStore10Door          ;
    TMySensor SnStore11Full          ;
    TMySensor SnStore11Door          ;
    TMySensor SnStore12Full          ;
    TMySensor SnStore12Door          ;
    TMySensor SnStore13Full          ;
    TMySensor SnStore13Door          ;
    TMySensor SnStore14Full          ;
    TMySensor SnStore14Door          ;
    TMySensor SnStore15Full          ;
    TMySensor SnStore15Door          ;
    TMySensor SnStore16Full          ;
    TMySensor SnStore16Door          ;
    TMySensor SnStore17Full          ;
    TMySensor SnStore17Door          ;
    TMySensor SnStore18Full          ;
    TMySensor SnStore18Door          ;
    TMySensor SnStore19Full          ;
    TMySensor SnStore19Door          ;
    TMySensor SnStore20Full          ;
    TMySensor SnStore20Door          ;
    TMySensor SnMGZ1ArmHasTray       ;
    TMySensor SnMGZ1DetectTray       ;
    TMySensor SnMGZ1LayerHasTray     ;
    TMySensor SnMGZ1SidePushUp       ;
    TMySensor SnMGZ1TrayFloating     ;
    TMySensor SnMGZ1TopHasTray       ;
    TMySensor SnMGZ1TrayPositionR    ;
    TMySensor SnMGZ1TrayPositionF    ;
    TMySensor SnMGZ1TrayDirection    ;
    TMySensor SnMGZ1CatchCyOff       ;
    TMySensor SnMGZ1Step             ;
    TMySensor SnMGZ1ArmXTakeTray     ;
    TMySensor SnMGZ2ArmHasTray       ;
    TMySensor SnMGZ2DetectTray       ;
    TMySensor SnMGZ2LayerHasTray     ;
    TMySensor SnMGZ2SidePushUp       ;
    TMySensor SnMGZ2TrayFloating     ;
    TMySensor SnMGZ2TopHasTray       ;
    TMySensor SnMGZ2TrayPositionR    ;
    TMySensor SnMGZ2TrayPositionF    ;
    TMySensor SnMGZ2TrayDirection    ;
    TMySensor SnMGZ2CatchCyOff       ;
    TMySensor SnMGZ2Step             ;
    TMySensor SnMGZ2ArmXTakeTray     ;
    TMySensor SnMGZ3ArmHasTray       ;
    TMySensor SnMGZ3DetectTray       ;
    TMySensor SnMGZ3LayerHasTray     ;
    TMySensor SnMGZ3SidePushUp       ;
    TMySensor SnMGZ3TrayFloating     ;
    TMySensor SnMGZ3TopHasTray       ;
    TMySensor SnMGZ3TrayPositionR    ;
    TMySensor SnMGZ3TrayPositionF    ;
    TMySensor SnMGZ3TrayDirection    ;
    TMySensor SnMGZ3CatchCyOff       ;
    TMySensor SnMGZ3Step             ;
    TMySensor SnMGZ3ArmXTakeTray     ;
    TMySensor SnMotorPower           ;
    TMySensor SnAirIsEnough          ;
    TMySensor SnRKSafeLock           ;
    TMySensor SnIonFan_01            ;
    TMySensor SnIonFan_02            ;
    TMySensor SnIonFan_03            ;
    TMySensor SnIonFan_04            ;
    TMySensor SnIonFan_05            ;
    TMySensor SnIonFan_06            ;
    TMySensor SnIonFan_07            ;
    TMySensor SnIonFan_08            ;
    TMySensor SnSafeDoor_01          ;
    TMySensor SnSafeDoor_02          ;
    TMySensor SnSafeDoor_03          ;
    TMySensor SnSafeDoor_04          ;
    TMySensor SnSafeDoor_05          ;
    TMySensor SnSafeDoor_06          ;
    TMySensor SnSafeDoor_07          ;
    TMySensor SnSafeDoor_08          ;
    TMySensor SnSafeDoor_09          ;
    TMySensor SnSafeDoor_10          ;
    TMySensor SnSafeDoor_11          ;
    TMySensor SnSafeDoor_12          ;
    TMySensor SnEMG01                ;
    TMySensor SnEMG02                ;
    TMySensor SnEMG03                ;
    TMySensor SnEMG04                ;
    TMySensor SnPLCSafer             ;
    TMySensor SnSortArm1XInterfere   ;
    TMySensor SnSortArm1YInterfere   ;
    TMySensor SnSortArm2XInterfere   ;
    TMySensor SnSortArm2YInterfere   ;
    TMySensor SnManual_StoreArm_Z    ;
    TMySensor SnTrackTrayDetect1     ;
    TMySensor SnTrackTrayDetect2     ;
    TMySensor SnTrackTrayDetect3     ;
    TMySensor SnTrackTrayDetect4     ;
    TMySensor SnAutoRow1ULDCheck     ;
    TMySensor SnAutoRow2ULDCheck     ;
    TMySensor SnAutoRow3ULDCheck     ;
    TMySensor SnAutoRow4ULDCheck     ;
    TMySensor SnAutoRow1PushCheck    ;                                          //推Tray 機構改為馬達
    TMySensor SnAutoRow2PushCheck    ;                                          //推Tray 機構改為馬達
    TMySensor SnAutoRow3PushCheck    ;                                          //推Tray 機構改為馬達
    TMySensor SnAutoRow4PushCheck    ;                                          //推Tray 機構改為馬達
    TMySensor SnAutoRow1CyPushTray   ;                                          //推Tray 機構改為馬達
    TMySensor SnAutoRow2CyPushTray   ;                                          //推Tray 機構改為馬達
    TMySensor SnAutoRow3CyPushTray   ;                                          //推Tray 機構改為馬達
    TMySensor SnAutoRow4CyPushTray   ;                                          //推Tray 機構改為馬達

    TMySensor SnAutoTrolly1HasTray   ;
    TMySensor SnAutoTrolly2HasTray   ;
    TMySensor SnAutoTrolly3HasTray   ;
    TMySensor SnAutoTrolly4HasTray   ;
    TMySensor SnLoaderCarDete        ;
    TMySensor SnStore1HasTray        ;
    TMySensor SnStore2HasTray        ;
    TMySensor SnStore3HasTray        ;
    TMySensor SnStore4HasTray        ;
    TMySensor SnStore5HasTray        ;
    TMySensor SnStore6HasTray        ;
    TMySensor SnStore7HasTray        ;
    TMySensor SnStore8HasTray        ;
    TMySensor SnStore9HasTray        ;
    TMySensor SnStore10HasTray       ;
    TMySensor SnStore11HasTray       ;
    TMySensor SnStore12HasTray       ;
    TMySensor SnStore13HasTray       ;
    TMySensor SnStore14HasTray       ;
    TMySensor SnStore15HasTray       ;
    TMySensor SnStore16HasTray       ;
    TMySensor SnStore17HasTray       ;
    TMySensor SnStore18HasTray       ;
    TMySensor SnStore19HasTray       ;
    TMySensor SnStore20HasTray       ;

}SENSOR_MODULAR;
//==============================================================================
//  真實輸出點集合 _
//  使用者依不同機器修改 _
//==============================================================================
typedef struct SWITCH_MODULAR_STRUCR                                            // need edit by user //
{
    TMySwitch SwACLoaderCCW        ;
    TMySwitch SwACLoaderCW         ;
    TMySwitch SwLoaderCWDete       ;
    TMySwitch SwACEmpty1CCW        ;
    TMySwitch SwACEmpty1CW         ;
    TMySwitch SwACEmpty2CCW        ;
    TMySwitch SwACEmpty2CW         ;
    TMySwitch SwEmpty2CCWDete      ;
    TMySwitch SwACAutoRow1CW       ;
    TMySwitch SwACAutoRow1CCW      ;
    TMySwitch SwACAutoRow2CW       ;
    TMySwitch SwACAutoRow2CCW      ;
    TMySwitch SwACAutoRow3CW       ;
    TMySwitch SwACAutoRow3CCW      ;
    TMySwitch SwACAutoRow4CW       ;
    TMySwitch SwACAutoRow4CCW      ;
    TMySwitch SwACShiftStageCCW    ;
    TMySwitch SwACShiftStageCW     ;
    TMySwitch Sw_MGZ1Step          ;
    TMySwitch Sw_MGZ2Step          ;
    TMySwitch Sw_MGZ3Step          ;
    TMySwitch SwLight              ;
    TMySwitch SwMotorRelay         ;
    TMySwitch SwBreak_StoreArm_Z   ;
    TMySwitch SwManual_StoreArm_Z  ;
    TMySwitch SwBreak_MGZ1Arm_Z    ;
    TMySwitch SwBreak_MGZ2Arm_Z    ;
    TMySwitch SwBreak_MGZ3Arm_Z    ;
    TMySwitch SwTowerRed           ;
    TMySwitch SwTowerYellow        ;
    TMySwitch SwTowerGreen         ;
    TMySwitch SwMusic1             ;
    TMySwitch SwMusic2             ;
    TMySwitch SwMusic3             ;
    TMySwitch SwMusic4             ;
    TMySwitch SwInterLock          ;
    TMySwitch SwFKPowerOff         ;
    TMySwitch SwFKPowerOn          ;
    TMySwitch SwFKReset            ;
    TMySwitch SwFKPause            ;
    TMySwitch SwFKHome             ;
    TMySwitch SwFKStart            ;
    TMySwitch SwFKOneCycle         ;
    TMySwitch SwFKRetry            ;
    TMySwitch SwFKSkip             ;
    TMySwitch SwFKCleanOut         ;
    TMySwitch SwFKTrayFeed         ;
    TMySwitch SwFKTrayEnd          ;
    TMySwitch SwFKAlarmReset       ;
    TMySwitch SwFKCoverOpen        ;
    TMySwitch SwRKPowerOff         ;
    TMySwitch SwRKPowerOn          ;
    TMySwitch SwRKReset            ;
    TMySwitch SwRKPause            ;
    TMySwitch SwRKHome             ;
    TMySwitch SwRKStart            ;
    TMySwitch SwRKOneCycle         ;
    TMySwitch SwRKRetry            ;
    TMySwitch SwRKSkip             ;
    TMySwitch SwRKCleanOut         ;
    TMySwitch SwRKTrayFeed         ;
    TMySwitch SwRKTrayEnd          ;
    TMySwitch SwRKAlarmReset       ;
    TMySwitch SwRKCoverOpen        ;
    TMySwitch SwRKManualStep       ;
    TMySwitch SwRKManualTStart     ;
    TMySwitch SwAutoRow1Vib1       ;
    TMySwitch SwAutoRow1Vib2       ;
    TMySwitch SwAutoRow2Vib1       ;
    TMySwitch SwAutoRow2Vib2       ;
    TMySwitch SwAutoRow3Vib1       ;
    TMySwitch SwAutoRow3Vib2       ;
    TMySwitch SwAutoRow4Vib1       ;
    TMySwitch SwAutoRow4Vib2       ;
    TMySwitch SwAutoRow1Dete       ;
    TMySwitch SwAutoRow2Dete       ;
    TMySwitch SwAutoRow3Dete       ;
    TMySwitch SwAutoRow4Dete       ;
    TMySwitch SwTFTBinRelay        ;
}SWITCH_MODULAR;
//==============================================================================
//  真實真空產生器集合 _
//  使用者依不同機器修改 _
//==============================================================================// need edit by user //
typedef struct SUCKER_MODULAR_STRUCR
{
    TMyKitSuck AutoArmSuck;
    TMyKitSuck MagArmSuck;
    TMyKitSuck TrayArmSuck;
    TMyKitSuck MagTrayArmSuck;
}SUCKER_MODULAR;
//==============================================================================
typedef struct MyShowStruct
{
    TObject *palPtr;
    long    *lPtr;
    long    lBackupValue;
    int    *iPtr;
    int     iBackupValue;
    int     dataType;  // 0:int ,1:long

}MyShowList;
//==============================================================================
typedef struct MyTimeStringShowStruct
{
    TObject *palPtr;
    int     dataType;  // 0: hh:mm:ss

}MyTimeStringShowList;
//==============================================================================
enum RunModeEnum                        //enum 型別
{
    Run_Normal      =0,
    Run_Home        =1,
    Run_OneCycle    =2,
    Run_CleanOut    =3,
    Run_TrayFeed    =4,
    Run_Scan        =5,
    Run_Check       =6
};
//==============================================================================
typedef struct MyNormalParameter
{
    bool SystemStart;
    bool bDummyRunSumulateHardware;
    RunModeEnum RunMode;
    bool bNeedHome;
    bool bHeaterReady;

}NormalParameter;
//==============================================================================
typedef struct MyFunctionA
{
    //------------------------------Setup---------------------------------------
    int iA01_LoaderTraySpeed;
    int iA01_LoaderTray2ndSpeed;

    int iA01_Empty1TraySpeed;
    int iA01_Empty1Tray2ndSpeed;

    int iA01_Empty2TraySpeed;
    int iA01_Empty2Tray2ndSpeed;

    int iA01_TransferTraySpeed;
    int iA01_TransferTray2ndSpeed;

    int iA01_Track1TraySpeed;
    int iA01_Track1Tray2ndSpeed;

    int iA01_Track2TraySpeed;
    int iA01_Track2Tray2ndSpeed;

    int iA01_Track3TraySpeed;
    int iA01_Track3Tray2ndSpeed;

    int iA01_Track4TraySpeed;
    int iA01_Track4Tray2ndSpeed;
    void Update();
}FunctionA;
//==============================================================================
typedef struct MyFunctionB
{
    bool bN03_UploadTrayData;
    bool bN04_RunCheck;         //JerryYang 20250106 : Run check
    void Update();
}FunctionB;
//==============================================================================
typedef struct MyFunctionC
{
    void Update();
}FunctionC;
//==============================================================================
typedef struct MyFunctionN
{
    int iN02_JHTFormatLoadSelect;
    bool bN03_OLP_Log;
    AnsiString sN03_OLP_IP;
    AnsiString sN03_OLP_Port;
    AnsiString sN03_LotStartPatch;
    AnsiString sN03_LotEndPatch;
    void Update();
}FunctionN;
//==============================================================================
typedef struct MyFunctionT
{
    bool bT01_AutoRow1Vib1;
    bool bT01_AutoRow1Vib2;
    bool bT01_AutoRow2Vib1;
    bool bT01_AutoRow2Vib2;
    bool bT01_AutoRow3Vib1;
    bool bT01_AutoRow3Vib2;
    bool bT01_AutoRow4Vib1;
    bool bT01_AutoRow4Vib2;
    int  iT02_MagTraySource;
    int  iT03_AutoBinSet;
    int  iT04_BinDisptype;
    int  iT05_BinDispComHardwareType;
    int  rgT03_AutoPickMode;
    int  iA01_InputDataFormat;
    int  iT06_AutoRowType;                                                      //KenHsieh 20250613 : add T06 Auto Row Type
    int  iT07_MGZDirection;                                                     //KenHsieh 20250614 : add 判斷Magazine direction
    int  iT08_Empty1PushTray;
    int  iT09_TrayIDReadPos;
    
    bool bA02_LoginTimeout;
    int  iA02_LoginTimeout;
    bool bA03_ChkTrollyHasTray;
    void Update();
}FunctionT;
//==============================================================================
typedef struct MyFunctionS  //Sam 20240619 : 速度獨立設定
{
    bool bS01_MagX;
    int  iS01_MagX;
    bool bS01_SortArmTrayZ;
    int  iS01_SortArmTrayZ;
    bool bS02_SearchShortestMode;
    bool bS03_MagArmXYPreMove;                                                  //Sam 20240727 : Magazine 換 Tray 時XY提前移動
    bool bS03_ShortDistanceMove;                                                //Sam 20240727 : 新增短距離加減速減半

    void Update();
}FunctionS;
//==============================================================================
class MyAlarmCodeStruct
{
    public:     // User declarations
        __fastcall MyAlarmCodeStruct();
        __fastcall MyAlarmCodeStruct(AnsiString _AlarmCode, int _AlarmType, AnsiString _E_ErrMessage, AnsiString _C_ErrMessage, AnsiString _E_Description, AnsiString _C_Description, AnsiString _FlushPanelName="");
        __fastcall ~MyAlarmCodeStruct();

        AnsiString AlarmCode;
        int        AlarmType;
        AnsiString E_ErrMessage;
        AnsiString C_ErrMessage;
        AnsiString E_Description;
        AnsiString C_Description;
        AnsiString FlushPanelName;

        AnsiString CommaText();
};
//==============================================================================
class MyBinToTrayStruct
{
    public:     // User declarations
        __fastcall MyBinToTrayStruct();
        __fastcall MyBinToTrayStruct(int _Bin);
        void ResetData();
        void ConvertBinToAuto(int _Bin);
        void ConvertAutoToBin(int _Auto, bool bReset=true);
        void ConvertRowColToBin(int _iRow, int _iCol);
        int iBin;
        int iWhichAuto;
        int iAutoRow;                                                           //這個是從no use開始, 所以auto 1 是 [1][0]
        int iAutoCol;
};
//==============================================================================
class cBinAutoConvert
{
    private:
        map<int, int>mapBinToAuto;
        map<int, int>mapAutoToBin;
        int iTotalSize;
    public:     // User declarations
        cBinAutoConvert();
        ~cBinAutoConvert();
        void Clear();
        void AddBinAuto(int iBin,int iAuto);
        int GetBinbyAuto(int iAuto);
        void SetBinbyAuto(int iSetBin,int iByAuto);
        int GetAutobyBin(int iBin);
        void SetAutobyBin(int iSetAuto,int iByBin);
        int GetMapCount();
        AnsiString SaveToJson();
        void LoadFromJson(AnsiString sFilePath);
        AnsiString sBinDataFileName(){return "BinDatas.jason";}
};
extern cBinAutoConvert cBinAutoTrans;
//==============================================================================
typedef struct MyLanguageStruct
{
    TForm *FormPtr;
    bool bShowHint;
}Language;
//==============================================================================
typedef struct MyLevelSetupStruct
{
    TForm *FormPtr;
}LevelSetup;
//==============================================================================
// 電控原件及共用參數集合
//==============================================================================
class SYSTEM_MODULAR
{
    private:
        // 用來更改畫面保留之結構,
    protected:
        void LoadCylinderParameterFromDataBase();
        void LoadSensorParameterFromDataBase();
        void LoadSwitchParameterFromDataBase();
        void LoadSuckerParameterFromDataBase();
        void InitialHardwareNameAndLoadDatabaseMotor();
        void InitialHardwareNameAndLoadDatabase();
        void InstallColorBinDisplay();
        void __fastcall ChoiceTempController();
    public:

        SYSTEM_MODULAR();
        ~SYSTEM_MODULAR();

        void LoadIoData();
        void LoadMotData();
        TIOTABLENO IoNo;
        TMOTNO     MotNo;
        vector <TIODATA *>IOTable;
        map<AnsiString, AnsiString>mapIOTable;
        map<AnsiString, AnsiString>::iterator mapIOTableIter;
        vector <TMOTDATA *>MotTable;
        map<AnsiString, AnsiString>mapMotTable;
        map<AnsiString, AnsiString>::iterator mapMotTableIter;
        AnsiString MotTablePath;
        AnsiString IoTablePath;
        AnsiString TrayTablePath;
        AnsiString AlarmTablePath;
        AnsiString AlarmMapPath;

        int iTotalMotor;
        int iTotalVMotor;
        int iTotalCylinder;
        int iTotalSensor;
        int iTotalSwitch;
        int iTotalSucker;
//        int iTotalSubSucker;
        AnsiString CurrentDir;
        int AccessLevel;
        bool bUseRemoteIO;
        NormalParameter Sys;
        FunctionA       FuncA;
        FunctionB       FuncB;
        FunctionC       FuncC;  //讓位功能
        FunctionN       FuncN;
        FunctionT       FuncT;  //讓位功能
        FunctionS       FuncS;  //Sam 20240619 : 速度獨立設定

        LAST_GENERAL_SET LastSet;
        LAST_GENERAL_SET LastSet_Backup;

        MOTOR_MODULAR           Mot;      // can not change ,add,delete order with next line
        VIRTUAL_MOTOR_MODULAR   VMot;     //
        CYLINDER_MODULAR        Cyn;
        SENSOR_MODULAR          Sen;
        SWITCH_MODULAR          Sw;
        SUCKER_MODULAR          Suck;
        TTrayMotor  **MotPtr;
        TTrayMotor  **VMotPtr;
        TMyCylinder *CynPtr;
        TMySensor   *SenPtr;
        TMySwitch   *SwPtr;
        TMyKitSuck  *SuckPtr;

        void InitialHardwareNameAndLoadDatabaseIO();
        void LoadMotorParameterFromDataBase(int Index=-1, bool bInitial=true);
        void LoadSingleMotorParameterFromDataBase(int Index, bool bInitial=true);
        void InitialVMotorParameter();

        bool         bHasTimeString;
        TStatusPanel *TimeString;
        void __fastcall Initial();
        void AddLanguageForm(TForm *FormPtr, bool bShowHint);
        void StopAllMotor();
        void DecStopAllMotor();

        HTGem *MyGem;
        TMyBinDispCtrl *BinDisCtrl;
        TMyBinDispCtrl *BinDisCtrlMag;
        TMyBinDispCtrl *BinDisCtrlTFT;
        TMyBinDispCtrl *BinDisCtrlMagTFT;

        map<AnsiString, MyAlarmCodeStruct>mapAlarmCodeList;
        map<AnsiString, MyAlarmCodeStruct>::iterator IterAlarmCodeList;
        map<AnsiString, AnsiString>mapNameToAlarm;
        map<AnsiString, AnsiString>::iterator IterNameToAlarm;

        int TrayUsage[eTrayCount];
};
extern SYSTEM_MODULAR HSys;
//---------------------------------------------------------------------------
typedef struct
{
    bool bRunSimulation;
    int iMaxBin;
    AnsiString sClipID;

    void Clear()
    {
        sClipID="Simulate";
        iMaxBin=0;
        bRunSimulation=false;
    }
}TSimulationData;
extern TSimulationData tSimuData;
//---------------------------------------------------------------------------
#endif
