#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop
#include "aMagazine.h"

//------------------------------------------------------------------------------
#pragma package(smart_init)
TMagazineModule *MagazineModule1=new TMagazineModule(eMagazine1);
TMagazineModule *MagazineModule2=new TMagazineModule(eMagazine2);
TMagazineModule *MagazineModule3=new TMagazineModule(eMagazine3);
TMagazineModule *MagazineModule[eMGZTotal]={MagazineModule1, MagazineModule2, MagazineModule3};
//------------------------------------------------------------------------------
void InitialMag()
{
    //MGZ 1 --------------------------------------------------------------------
    MagazineModule1->iMotMGZ_X           =HSys.Mot.Magazine1X->Tag;
    MagazineModule1->iMotMGZ_Z           =HSys.Mot.Magazine1Z->Tag;
    MagazineModule1->MMagSortTopTray     =HSys.VMot.MMag1SortTopTray;
    MagazineModule1->MGZTrayArm          =HSys.VMot.MagazineTrayArm[eMagazine1];

    MagazineModule1->MMagNowSortTray     =HSys.VMot.Mag_NowSortTray;
    MagazineModule1->MMagSortTray        =HSys.VMot.Mag_LD_Work_Y;

    MagazineModule1->Sn_MGZArmHasTray    =&HSys.Sen.SnMGZ1ArmHasTray;
    MagazineModule1->Sn_DetectTray       =&HSys.Sen.SnMGZ1DetectTray;
    MagazineModule1->Sn_MGZLayerHasTray  =&HSys.Sen.SnMGZ1LayerHasTray;
    MagazineModule1->Sn_SidePushUp       =&HSys.Sen.SnMGZ1SidePushUp;
    MagazineModule1->Sn_MGZTrayFloating  =&HSys.Sen.SnMGZ1TrayFloating;
    MagazineModule1->Sn_MGZTopHasTray    =&HSys.Sen.SnMGZ1TopHasTray;
    MagazineModule1->Sn_TrayPositionR    =&HSys.Sen.SnMGZ1TrayPositionR;
    MagazineModule1->Sn_TrayPositionF    =&HSys.Sen.SnMGZ1TrayPositionF;
    MagazineModule1->Sn_TrayDirection    =&HSys.Sen.SnMGZ1TrayDirection;
    MagazineModule1->Sn_CatchCyOff       =&HSys.Sen.SnMGZ1CatchCyOff;
    MagazineModule1->Sn_MGZStep          =&HSys.Sen.SnMGZ1Step;

    MagazineModule1->Sw_MGZStep          =&HSys.Sw.Sw_MGZ1Step;

    MagazineModule1->C_MGZ_CatchTray     =&HSys.Cyn.C_MGZ1_CatchTray;
    MagazineModule1->C_MGZ_EdgeFixer     =&HSys.Cyn.C_MGZ1_EdgeFixer;
    MagazineModule1->C_MGZ_SidePush_Up   =&HSys.Cyn.C_MGZ1_SidePush_Up;
    MagazineModule1->C_MGZ_SidePush      =&HSys.Cyn.C_MGZ1_SidePush;
    MagazineModule1->C_MGZ_LayerTrayPush =&HSys.Cyn.C_MGZ1_LayerTrayPush;
    MagazineModule1->C_MGZ_TrayPush      =&HSys.Cyn.C_MGZ1_TrayPush;
    MagazineModule1->C_MGZ_TopFixTray    =&HSys.Cyn.C_MGZ1_TopFixTray;

    MagazineModule1->C_Mag_CCD_Stop      =&HSys.Cyn.C_Empty2_CCDStop;
    MagazineModule1->SnMag_CCDPos        =&HSys.Sen.SnEmpty2CCDPosition;
    MagazineModule1->C_Empty2_EmptyStop  =&HSys.Cyn.C_Empty2_EmptyStop;
    MagazineModule1->SnEmpty2HasEmptyTray=&HSys.Sen.SnEmpty2HasEmptyTray;
    MagazineModule1->SwACEmpty2CW        =&HSys.Sw.SwACEmpty2CW;

    MagazineModule1->MotEmpty2_Tray      =HSys.VMot.MagEmptyTray;

    MagazineModule1->MMagArmX            =HSys.Mot.SortArm2X;
    MagazineModule1->MMagArmY            =HSys.Mot.SortArm2Y;

    MagazineModule1->MagTrayArmSuck      =&HSys.Suck.MagTrayArmSuck;
    MagazineModule1->bMgzStepLight       =false;

    MagazineModule1->MGZLayer[eMag01]    =HSys.VMot.tmAutoAreaAuto[eMag1_01];
    MagazineModule1->MGZLayer[eMag02]    =HSys.VMot.tmAutoAreaAuto[eMag1_02];
    MagazineModule1->MGZLayer[eMag03]    =HSys.VMot.tmAutoAreaAuto[eMag1_03];
    MagazineModule1->MGZLayer[eMag04]    =HSys.VMot.tmAutoAreaAuto[eMag1_04];
    MagazineModule1->MGZLayer[eMag05]    =HSys.VMot.tmAutoAreaAuto[eMag1_05];
    MagazineModule1->MGZLayer[eMag06]    =HSys.VMot.tmAutoAreaAuto[eMag1_06];
    MagazineModule1->MGZLayer[eMag07]    =HSys.VMot.tmAutoAreaAuto[eMag1_07];
    MagazineModule1->MGZLayer[eMag08]    =HSys.VMot.tmAutoAreaAuto[eMag1_08];
    MagazineModule1->MGZLayer[eMag09]    =HSys.VMot.tmAutoAreaAuto[eMag1_09];
    MagazineModule1->MGZLayer[eMag10]    =HSys.VMot.tmAutoAreaAuto[eMag1_10];
    MagazineModule1->MGZLayer[eMag11]    =HSys.VMot.tmAutoAreaAuto[eMag1_11];
    MagazineModule1->MGZLayer[eMag12]    =HSys.VMot.tmAutoAreaAuto[eMag1_12];
    MagazineModule1->MGZLayer[eMag13]    =HSys.VMot.tmAutoAreaAuto[eMag1_13];
    MagazineModule1->MGZLayer[eMag14]    =HSys.VMot.tmAutoAreaAuto[eMag1_14];
    MagazineModule1->MGZLayer[eMag15]    =HSys.VMot.tmAutoAreaAuto[eMag1_15];
    MagazineModule1->MGZLayer[eMag16]    =HSys.VMot.tmAutoAreaAuto[eMag1_16];
    MagazineModule1->MGZLayer[eMag17]    =HSys.VMot.tmAutoAreaAuto[eMag1_17];
    MagazineModule1->MGZLayer[eMag18]    =HSys.VMot.tmAutoAreaAuto[eMag1_18];
    MagazineModule1->MGZLayer[eMag19]    =HSys.VMot.tmAutoAreaAuto[eMag1_19];
    MagazineModule1->MGZLayer[eMag20]    =HSys.VMot.tmAutoAreaAuto[eMag1_20];
    MagazineModule1->MGZLayer[eMag21]    =HSys.VMot.tmAutoAreaAuto[eMag1_21];
    MagazineModule1->MGZLayer[eMag22]    =HSys.VMot.tmAutoAreaAuto[eMag1_22];
    MagazineModule1->MGZLayer[eMag23]    =HSys.VMot.tmAutoAreaAuto[eMag1_23];
    MagazineModule1->MGZLayer[eMag24]    =HSys.VMot.tmAutoAreaAuto[eMag1_24];
    MagazineModule1->MGZLayer[eMag25]    =HSys.VMot.tmAutoAreaAuto[eMag1_25];
    MagazineModule1->MGZLayer[eMag26]    =HSys.VMot.tmAutoAreaAuto[eMag1_26];
    MagazineModule1->MGZLayer[eMag27]    =HSys.VMot.tmAutoAreaAuto[eMag1_27];

    MagazineModule1->SetEnable(Enable_MAG1);

    //MGZ 2 --------------------------------------------------------------------
    MagazineModule2->iMotMGZ_X           =HSys.Mot.Magazine2X->Tag;
    MagazineModule2->iMotMGZ_Z           =HSys.Mot.Magazine2Z->Tag;
    MagazineModule2->MMagSortTopTray     =HSys.VMot.MMag2SortTopTray;
    MagazineModule2->MGZTrayArm          =HSys.VMot.MagazineTrayArm[eMagazine2];

    MagazineModule2->MMagNowSortTray     =HSys.VMot.Mag_NowSortTray;
    MagazineModule2->MMagSortTray        =HSys.VMot.Mag_LD_Work_Y;

    MagazineModule2->Sn_MGZArmHasTray    =&HSys.Sen.SnMGZ2ArmHasTray;
    MagazineModule2->Sn_DetectTray       =&HSys.Sen.SnMGZ2DetectTray;
    MagazineModule2->Sn_MGZLayerHasTray  =&HSys.Sen.SnMGZ2LayerHasTray;
    MagazineModule2->Sn_SidePushUp       =&HSys.Sen.SnMGZ2SidePushUp;
    MagazineModule2->Sn_MGZTrayFloating  =&HSys.Sen.SnMGZ2TrayFloating;
    MagazineModule2->Sn_MGZTopHasTray    =&HSys.Sen.SnMGZ2TopHasTray;
    MagazineModule2->Sn_TrayPositionR    =&HSys.Sen.SnMGZ2TrayPositionR;
    MagazineModule2->Sn_TrayPositionF    =&HSys.Sen.SnMGZ2TrayPositionF;
    MagazineModule2->Sn_TrayDirection    =&HSys.Sen.SnMGZ2TrayDirection;
    MagazineModule2->Sn_CatchCyOff       =&HSys.Sen.SnMGZ2CatchCyOff;
    MagazineModule2->Sn_MGZStep          =&HSys.Sen.SnMGZ2Step;

    MagazineModule2->Sw_MGZStep          =&HSys.Sw.Sw_MGZ2Step;

    MagazineModule2->C_MGZ_CatchTray     =&HSys.Cyn.C_MGZ2_CatchTray;
    MagazineModule2->C_MGZ_EdgeFixer     =&HSys.Cyn.C_MGZ2_EdgeFixer;
    MagazineModule2->C_MGZ_SidePush_Up   =&HSys.Cyn.C_MGZ2_SidePush_Up;
    MagazineModule2->C_MGZ_SidePush      =&HSys.Cyn.C_MGZ2_SidePush;
    MagazineModule2->C_MGZ_LayerTrayPush =&HSys.Cyn.C_MGZ2_LayerTrayPush;
    MagazineModule2->C_MGZ_TrayPush      =&HSys.Cyn.C_MGZ2_TrayPush;
    MagazineModule2->C_MGZ_TopFixTray    =&HSys.Cyn.C_MGZ2_TopFixTray;

    MagazineModule2->C_Mag_CCD_Stop      =&HSys.Cyn.C_Empty2_CCDStop;
    MagazineModule2->SnMag_CCDPos        =&HSys.Sen.SnEmpty2CCDPosition;
    MagazineModule2->C_Empty2_EmptyStop  =&HSys.Cyn.C_Empty2_EmptyStop;
    MagazineModule2->SnEmpty2HasEmptyTray=&HSys.Sen.SnEmpty2HasEmptyTray;
    MagazineModule2->SwACEmpty2CW        =&HSys.Sw.SwACEmpty2CW;

    MagazineModule2->MotEmpty2_Tray      =HSys.VMot.MagEmptyTray;

    MagazineModule2->MMagArmX            =HSys.Mot.SortArm2X;
    MagazineModule2->MMagArmY            =HSys.Mot.SortArm2Y;

    MagazineModule2->MagTrayArmSuck      =&HSys.Suck.MagTrayArmSuck;
    MagazineModule2->bMgzStepLight       =false;

    MagazineModule2->MGZLayer[eMag01]    =HSys.VMot.tmAutoAreaAuto[eMag2_01];
    MagazineModule2->MGZLayer[eMag02]    =HSys.VMot.tmAutoAreaAuto[eMag2_02];
    MagazineModule2->MGZLayer[eMag03]    =HSys.VMot.tmAutoAreaAuto[eMag2_03];
    MagazineModule2->MGZLayer[eMag04]    =HSys.VMot.tmAutoAreaAuto[eMag2_04];
    MagazineModule2->MGZLayer[eMag05]    =HSys.VMot.tmAutoAreaAuto[eMag2_05];
    MagazineModule2->MGZLayer[eMag06]    =HSys.VMot.tmAutoAreaAuto[eMag2_06];
    MagazineModule2->MGZLayer[eMag07]    =HSys.VMot.tmAutoAreaAuto[eMag2_07];
    MagazineModule2->MGZLayer[eMag08]    =HSys.VMot.tmAutoAreaAuto[eMag2_08];
    MagazineModule2->MGZLayer[eMag09]    =HSys.VMot.tmAutoAreaAuto[eMag2_09];
    MagazineModule2->MGZLayer[eMag10]    =HSys.VMot.tmAutoAreaAuto[eMag2_10];
    MagazineModule2->MGZLayer[eMag11]    =HSys.VMot.tmAutoAreaAuto[eMag2_11];
    MagazineModule2->MGZLayer[eMag12]    =HSys.VMot.tmAutoAreaAuto[eMag2_12];
    MagazineModule2->MGZLayer[eMag13]    =HSys.VMot.tmAutoAreaAuto[eMag2_13];
    MagazineModule2->MGZLayer[eMag14]    =HSys.VMot.tmAutoAreaAuto[eMag2_14];
    MagazineModule2->MGZLayer[eMag15]    =HSys.VMot.tmAutoAreaAuto[eMag2_15];
    MagazineModule2->MGZLayer[eMag16]    =HSys.VMot.tmAutoAreaAuto[eMag2_16];
    MagazineModule2->MGZLayer[eMag17]    =HSys.VMot.tmAutoAreaAuto[eMag2_17];
    MagazineModule2->MGZLayer[eMag18]    =HSys.VMot.tmAutoAreaAuto[eMag2_18];
    MagazineModule2->MGZLayer[eMag19]    =HSys.VMot.tmAutoAreaAuto[eMag2_19];
    MagazineModule2->MGZLayer[eMag20]    =HSys.VMot.tmAutoAreaAuto[eMag2_20];
    MagazineModule2->MGZLayer[eMag21]    =HSys.VMot.tmAutoAreaAuto[eMag2_21];
    MagazineModule2->MGZLayer[eMag22]    =HSys.VMot.tmAutoAreaAuto[eMag2_22];
    MagazineModule2->MGZLayer[eMag23]    =HSys.VMot.tmAutoAreaAuto[eMag2_23];
    MagazineModule2->MGZLayer[eMag24]    =HSys.VMot.tmAutoAreaAuto[eMag2_24];
    MagazineModule2->MGZLayer[eMag25]    =HSys.VMot.tmAutoAreaAuto[eMag2_25];
    MagazineModule2->MGZLayer[eMag26]    =HSys.VMot.tmAutoAreaAuto[eMag2_26];
    MagazineModule2->MGZLayer[eMag27]    =HSys.VMot.tmAutoAreaAuto[eMag2_27];

    MagazineModule2->SetEnable(Enable_MAG2);

    //MGZ 3 --------------------------------------------------------------------
    MagazineModule3->iMotMGZ_X           =HSys.Mot.Magazine3X->Tag;
    MagazineModule3->iMotMGZ_Z           =HSys.Mot.Magazine3Z->Tag;
    MagazineModule3->MMagSortTopTray     =HSys.VMot.MMag3SortTopTray;
    MagazineModule3->MGZTrayArm          =HSys.VMot.MagazineTrayArm[eMagazine3];

    MagazineModule3->MMagNowSortTray     =HSys.VMot.Mag_NowSortTray;
    MagazineModule3->MMagSortTray        =HSys.VMot.Mag_LD_Work_Y;

    MagazineModule3->Sn_MGZArmHasTray    =&HSys.Sen.SnMGZ3ArmHasTray;
    MagazineModule3->Sn_DetectTray       =&HSys.Sen.SnMGZ3DetectTray;
    MagazineModule3->Sn_MGZLayerHasTray  =&HSys.Sen.SnMGZ3LayerHasTray;
    MagazineModule3->Sn_SidePushUp       =&HSys.Sen.SnMGZ3SidePushUp;
    MagazineModule3->Sn_MGZTrayFloating  =&HSys.Sen.SnMGZ3TrayFloating;
    MagazineModule3->Sn_MGZTopHasTray    =&HSys.Sen.SnMGZ3TopHasTray;
    MagazineModule3->Sn_TrayPositionR    =&HSys.Sen.SnMGZ3TrayPositionR;
    MagazineModule3->Sn_TrayPositionF    =&HSys.Sen.SnMGZ3TrayPositionF;
    MagazineModule3->Sn_TrayDirection    =&HSys.Sen.SnMGZ3TrayDirection;
    MagazineModule3->Sn_CatchCyOff       =&HSys.Sen.SnMGZ3CatchCyOff;
    MagazineModule3->Sn_MGZStep          =&HSys.Sen.SnMGZ3Step;

    MagazineModule3->Sw_MGZStep          =&HSys.Sw.Sw_MGZ3Step;

    MagazineModule3->C_MGZ_CatchTray     =&HSys.Cyn.C_MGZ3_CatchTray;
    MagazineModule3->C_MGZ_EdgeFixer     =&HSys.Cyn.C_MGZ3_EdgeFixer;
    MagazineModule3->C_MGZ_SidePush_Up   =&HSys.Cyn.C_MGZ3_SidePush_Up;
    MagazineModule3->C_MGZ_SidePush      =&HSys.Cyn.C_MGZ3_SidePush;
    MagazineModule3->C_MGZ_LayerTrayPush =&HSys.Cyn.C_MGZ3_LayerTrayPush;
    MagazineModule3->C_MGZ_TrayPush      =&HSys.Cyn.C_MGZ3_TrayPush;
    MagazineModule3->C_MGZ_TopFixTray    =&HSys.Cyn.C_MGZ3_TopFixTray;

    MagazineModule3->C_Mag_CCD_Stop      =&HSys.Cyn.C_Empty2_CCDStop;
    MagazineModule3->SnMag_CCDPos        =&HSys.Sen.SnEmpty2CCDPosition;
    MagazineModule3->C_Empty2_EmptyStop  =&HSys.Cyn.C_Empty2_EmptyStop;
    MagazineModule3->SnEmpty2HasEmptyTray=&HSys.Sen.SnEmpty2HasEmptyTray;
    MagazineModule3->SwACEmpty2CW        =&HSys.Sw.SwACEmpty2CW;

    MagazineModule3->MotEmpty2_Tray      =HSys.VMot.MagEmptyTray;

    MagazineModule3->MMagArmX            =HSys.Mot.SortArm2X;
    MagazineModule3->MMagArmY            =HSys.Mot.SortArm2Y;

    MagazineModule3->MagTrayArmSuck      =&HSys.Suck.MagTrayArmSuck;
    MagazineModule3->bMgzStepLight       =false;

    MagazineModule3->MGZLayer[eMag01]    =HSys.VMot.tmAutoAreaAuto[eMag3_01];
    MagazineModule3->MGZLayer[eMag02]    =HSys.VMot.tmAutoAreaAuto[eMag3_02];
    MagazineModule3->MGZLayer[eMag03]    =HSys.VMot.tmAutoAreaAuto[eMag3_03];
    MagazineModule3->MGZLayer[eMag04]    =HSys.VMot.tmAutoAreaAuto[eMag3_04];
    MagazineModule3->MGZLayer[eMag05]    =HSys.VMot.tmAutoAreaAuto[eMag3_05];
    MagazineModule3->MGZLayer[eMag06]    =HSys.VMot.tmAutoAreaAuto[eMag3_06];
    MagazineModule3->MGZLayer[eMag07]    =HSys.VMot.tmAutoAreaAuto[eMag3_07];
    MagazineModule3->MGZLayer[eMag08]    =HSys.VMot.tmAutoAreaAuto[eMag3_08];
    MagazineModule3->MGZLayer[eMag09]    =HSys.VMot.tmAutoAreaAuto[eMag3_09];
    MagazineModule3->MGZLayer[eMag10]    =HSys.VMot.tmAutoAreaAuto[eMag3_10];
    MagazineModule3->MGZLayer[eMag11]    =HSys.VMot.tmAutoAreaAuto[eMag3_11];
    MagazineModule3->MGZLayer[eMag12]    =HSys.VMot.tmAutoAreaAuto[eMag3_12];
    MagazineModule3->MGZLayer[eMag13]    =HSys.VMot.tmAutoAreaAuto[eMag3_13];
    MagazineModule3->MGZLayer[eMag14]    =HSys.VMot.tmAutoAreaAuto[eMag3_14];
    MagazineModule3->MGZLayer[eMag15]    =HSys.VMot.tmAutoAreaAuto[eMag3_15];
    MagazineModule3->MGZLayer[eMag16]    =HSys.VMot.tmAutoAreaAuto[eMag3_16];
    MagazineModule3->MGZLayer[eMag17]    =HSys.VMot.tmAutoAreaAuto[eMag3_17];
    MagazineModule3->MGZLayer[eMag18]    =HSys.VMot.tmAutoAreaAuto[eMag3_18];
    MagazineModule3->MGZLayer[eMag19]    =HSys.VMot.tmAutoAreaAuto[eMag3_19];
    MagazineModule3->MGZLayer[eMag20]    =HSys.VMot.tmAutoAreaAuto[eMag3_20];
    MagazineModule3->MGZLayer[eMag21]    =HSys.VMot.tmAutoAreaAuto[eMag3_21];
    MagazineModule3->MGZLayer[eMag22]    =HSys.VMot.tmAutoAreaAuto[eMag3_22];
    MagazineModule3->MGZLayer[eMag23]    =HSys.VMot.tmAutoAreaAuto[eMag3_23];
    MagazineModule3->MGZLayer[eMag24]    =HSys.VMot.tmAutoAreaAuto[eMag3_24];
    MagazineModule3->MGZLayer[eMag25]    =HSys.VMot.tmAutoAreaAuto[eMag3_25];
    MagazineModule3->MGZLayer[eMag26]    =HSys.VMot.tmAutoAreaAuto[eMag3_26];
    MagazineModule3->MGZLayer[eMag27]    =HSys.VMot.tmAutoAreaAuto[eMag3_27];

    MagazineModule3->SetEnable(Enable_MAG3);
}
//------------------------------------------------------------------------------
void UpdateMagazineParameter()
{
    //eMagazine1
    MagazineModule1->Z_TopPos               =atoi(fTeach->edMagazine1Z_TopPosition->Text.c_str())   +atoi(fOffset->edMagazine1Z_TopPosition->Text.c_str());
    MagazineModule1->X_PutTrayPos           =atoi(fTeach->edMagazine1X_PutTray->Text.c_str())       +atoi(fOffset->edMagazine1X_PutTray->Text.c_str());
    MagazineModule1->Z_LayerPos             =atoi(fTeach->edMagazine1Z_LayerPosition->Text.c_str()) +atoi(fOffset->edMagazine1Z_LayerPosition->Text.c_str());
    MagazineModule1->X_TakeTrayPos          =atoi(fTeach->edMagazine1X_TakeTray->Text.c_str())      +atoi(fOffset->edMagazine1X_TakeTray->Text.c_str());
    MagazineModule1->X_PushTrayPos          =atoi(fTeach->edMagazine1X_PushTray->Text.c_str())      +atoi(fOffset->edMagazine1X_PushTray->Text.c_str());
    MagazineModule1->iTrayXDivision         =atoi(fSetup->edXDivision->Text.c_str());
    MagazineModule1->iTrayYDivision         =atoi(fSetup->edYDivision->Text.c_str());
    MagazineModule1->MMagSortTopTray->SetTrayXYItem(LoaderModule->iXDivision, LoaderModule->iYDivision);

    //eMagazine2
    MagazineModule2->Z_TopPos               =atoi(fTeach->edMagazine2Z_TopPosition->Text.c_str())   +atoi(fOffset->edMagazine2Z_TopPosition->Text.c_str());
    MagazineModule2->X_PutTrayPos           =atoi(fTeach->edMagazine2X_PutTray->Text.c_str())       +atoi(fOffset->edMagazine2X_PutTray->Text.c_str());
    MagazineModule2->Z_LayerPos             =atoi(fTeach->edMagazine2Z_LayerPosition->Text.c_str()) +atoi(fOffset->edMagazine2Z_LayerPosition->Text.c_str());
    MagazineModule2->X_TakeTrayPos          =atoi(fTeach->edMagazine2X_TakeTray->Text.c_str())      +atoi(fOffset->edMagazine2X_TakeTray->Text.c_str());
    MagazineModule2->X_PushTrayPos          =atoi(fTeach->edMagazine2X_PushTray->Text.c_str())      +atoi(fOffset->edMagazine2X_PushTray->Text.c_str());
    MagazineModule2->iTrayXDivision         =atoi(fSetup->edXDivision->Text.c_str());
    MagazineModule2->iTrayYDivision         =atoi(fSetup->edYDivision->Text.c_str());

    MagazineModule2->MMagSortTopTray->SetTrayXYItem(LoaderModule->iXDivision, LoaderModule->iYDivision);

    //eMagazine3
    MagazineModule3->Z_TopPos               =atoi(fTeach->edMagazine3Z_TopPosition->Text.c_str())   +atoi(fOffset->edMagazine3Z_TopPosition->Text.c_str());
    MagazineModule3->X_PutTrayPos           =atoi(fTeach->edMagazine3X_PutTray->Text.c_str())       +atoi(fOffset->edMagazine3X_PutTray->Text.c_str());
    MagazineModule3->Z_LayerPos             =atoi(fTeach->edMagazine3Z_LayerPosition->Text.c_str()) +atoi(fOffset->edMagazine3Z_LayerPosition->Text.c_str());
    MagazineModule3->X_TakeTrayPos          =atoi(fTeach->edMagazine3X_TakeTray->Text.c_str())      +atoi(fOffset->edMagazine3X_TakeTray->Text.c_str());
    MagazineModule3->X_PushTrayPos          =atoi(fTeach->edMagazine3X_PushTray->Text.c_str())      +atoi(fOffset->edMagazine3X_PushTray->Text.c_str());
    MagazineModule3->iTrayXDivision         =atoi(fSetup->edXDivision->Text.c_str());
    MagazineModule3->iTrayYDivision         =atoi(fSetup->edYDivision->Text.c_str());
    MagazineModule3->MMagSortTopTray->SetTrayXYItem(LoaderModule->iXDivision, LoaderModule->iYDivision);

    //
    MagazineModule1->iLayerBase=eMag1_01;
    MagazineModule2->iLayerBase=eMag2_01;
    MagazineModule3->iLayerBase=eMag3_01;

    MagazineModule1->iLayerRow=eTrackMgz1;
    MagazineModule2->iLayerRow=eTrackMgz2;
    MagazineModule3->iLayerRow=eTrackMgz3;
}
//------------------------------------------------------------------------------
__fastcall TMagazineModule::TMagazineModule(int iTag)
{
    Tag         =iTag;
    InitialFlag();
}
//------------------------------------------------------------------------------
void TMagazineModule::InitialFlag()
{
    iRetry      =0;
    iNowTopBin  =0;
    bCynActOk1  =false;
    bCynActOk2  =false;
    mTopTray.ConvertBinToAuto(0);

    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        iBinDisp[i]=-1;
    }
    iMagArmStatus=0;
    bDoManualMagFinish=false;
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoTrayFromEmpty(int iBin, bool bReset)
{
    int &Task=iTrayFromEmptyTask;
    bool bResult=false, bflag=false;
    MyBinToTrayStruct mBinToTray_Mag;
    AnsiString str="";
    int iX=0;
    if(bReset)
    {
        bZMoveFinish=false;
        bXMoveFinish=false;
        Task=1;
        return bResult;
    }
    switch(Task)
    {
        case 1:
            if(Sn_MGZTopHasTray->IsOn()==false)
            {
                C_MGZ_TopFixTray->Reset();
                C_MGZ_SidePush->Reset();
                bCynActOk1=false;
                bCynActOk2=false;
                Task=100;
            }
            else
            {
                str.sprintf("Magazin%d Top has tray, Please check.", Tag+1);    //Jimmychiu 20260223 : fixed for sensor on without tray and hangup
                ShowMyMessage(str);
                //Task=9900;
            }
            break;
        case 100:
            if(bCynActOk1==false)
                bCynActOk1=C_MGZ_TopFixTray->Pop();
            if(bCynActOk2==false)
                bCynActOk2=C_MGZ_SidePush->Pop();

            if(bCynActOk1 && bCynActOk2)
            {
                iMagArmStatus=eHS_MA_NeedEmptyTray;
                Task=200;
            }
            break;
        case 200:
            if(MagArmPara->iMagStatus==(int)MagArmPara->eHS_MAG_EmptyTrayReady1+Tag)
            {
                if(Sn_MGZTopHasTray->IsOff()==false)
                {
                    iMagArmStatus=eHS_MA_Idle;
                    bZMoveFinish=false;
                    Task=300;
                }
                else
                {
                    ShowSystemError(Sn_MGZTopHasTray->Name, K_RETRY);           //("Tray is not on Magazine top position, please check");
                }
            }
            break;
        case 300:
            if(bCheckMGZZCanMve())
            {
                bZMoveFinish=HSys.MotPtr[iMotMGZ_Z]->MotorMove(Z_TopPos);       //JerryYang 20240530 : 修正MAG Z軸沒移動到最上層就夾
                if(bZMoveFinish)
                {
                    bZMoveFinish=false;

                    if(HSys.FuncT.iT07_MGZDirection==0)
                    {
                        C_MGZ_SidePush_Up->Reset();
                        Task=400;
                    }
                    else if(HSys.FuncT.iT07_MGZDirection==1)
                    {
                        bCynActOk1=false;
                        bCynActOk2=false;
                        C_MGZ_TrayPush->Reset();
                        C_MGZ_EdgeFixer->Reset();
                        Task=1000;
                    }
                }
            }
            else                                                                //KenHsieh 20250812 : 新增異常提示，避免Hangup
            {
                ShowSystemError(Sn_TrayPositionF->Name, K_RETRY);
            }
            break;
        case 400:
            if(C_MGZ_SidePush_Up->Push() || HSys.LastSet.iRealDummy==DUMMY)
            {
                C_MGZ_SidePush->Reset();
                C_MGZ_TopFixTray->Reset();
                bCynActOk1=false;
                bCynActOk2=false;
                Task=500;
            }
            break;
        case 500:
            if(bCynActOk1==false)
                bCynActOk1=C_MGZ_TopFixTray->Push();
            if(bCynActOk2==false)
                bCynActOk2=C_MGZ_SidePush->Push();

            if((bCynActOk1 && bCynActOk2) || HSys.LastSet.iRealDummy==DUMMY)
            {
                C_MGZ_SidePush->Reset();
                C_MGZ_SidePush_Up->Reset();
                bCynActOk1=false;
                bCynActOk2=false;
                Task=600;
            }
            break;
        case 600:
            if(bCynActOk1==false)
                bCynActOk1=C_MGZ_SidePush_Up->Pop();
            if(bCynActOk2==false)
                bCynActOk2=C_MGZ_SidePush->Pop();

            if((bCynActOk1 && bCynActOk2) || HSys.LastSet.iRealDummy==DUMMY)
            {
                if(Sn_MGZTopHasTray->IsOff()==false)
                {
                    if(HSys.FuncT.iT07_MGZDirection==0)
                        Task=2000;
                    else if(HSys.FuncT.iT07_MGZDirection==1)
                        Task=1000;
                }
                else
                {
                    ShowSystemError(Sn_MGZTopHasTray->Name, K_RETRY);           //("Tray is not on Magazine top position, please check");
                }
            }
            break;
        case 1000:
            iX=HSys.MotPtr[iMotMGZ_X]->ReadEncoderPos();
            if(iX==X_TakeTrayPos)
            {
                if(Sn_DetectTray->IsOn()==false)
                {
                    Task=1005;
                }
                else
                {
                    str.sprintf("Magazin%d Top has tray, Please check or take the tray to the Mag elevator area.", Tag+1);
                    ShowMyMessage(str);
                }
            }
            else
            {
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PutTrayPos);

                if(bXMoveFinish)
                {
                    bXMoveFinish=false;
                    Task=1010;
                }
            }
            break;
        case 1005:
            C_MGZ_CatchTray->Off();
            CatchOffDelay.SetMSAndOn(300);
            Task=1006;
            break;
        case 1006:
            if(CatchOffDelay.Off())
            {
                DoTakeTrayFromTop(true);
                Task=1030;
            }
            break;
        case 1010:
            C_MGZ_CatchTray->Off();
            CatchOffDelay.SetMSAndOn(500);
            Task=1015;
            break;
        case 1015:
            if(CatchOffDelay.Off())
            {
                if(Sn_CatchCyOff->IsOff()==false)
                {
                    Task=1020;
                }
                else
                {
                    str.sprintf("Please check Magazin%d C_MGZ_CatchTray is open.", Tag+1);
                    ShowMyMessage(str);
                }
            }
            break;
        case 1020:
            if(Sn_MGZArmHasTray->IsOn()==false)
            {
                DoTakeTrayFromTop(true);
                Task=1030;
            }
            else
            {
                str.sprintf("Magazin%d Sn_MGZArmHasTray is on and has tray, please check or remove unknow tray.", Tag+1);
                ShowMyMessage(str);
            }
            break;
        case 1030:
             bflag=DoTakeTrayFromTop(false, false);
             if(bflag)
                Task=1035;
             break;
        case 1035:
            if(Sn_MGZArmHasTray->IsOff()==false)
                Task=2000;
            else
                ShowSystemError(Sn_MGZArmHasTray->Name, K_RETRY);
             break;
        case 2000:
            MMagSortTopTray->MoveTrayFrom(MotEmpty2_Tray);
            MMagSortTopTray->Tray.iBin=iBin;
            iNowTopBin=iBin;                                                    //JerryYang 20240601 : Add MGZ Bin Display
            SetMagTopBin(iNowTopBin);                                           //JerryYang 20240601 : Add MGZ Bin Display

            if(Tag==eMagazine1)
                Task=2010;
            else
                Task=9900;
            break;
        case 2010:
            for(int i=0; i<MMagSortTopTray->Tray.XItem; i++)
            {
                for(int j=MMagSortTopTray->Tray.YItem/2; j<MMagSortTopTray->Tray.YItem; j++)
                {
                    MMagSortTopTray->SetTraySingleData(i, j, HAS_NULL_IC);
                }
            }
            Task=9900;
            break;
        case 9900:
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
void TMagazineModule::Find_TrayLayer(TTrayMotor *Mot)
{
    for(int i=0; i<iTrayXDivision; i++)
    {
        for(int j=0; j<iTrayYDivision; j++)
        {
            if(Mot->Tray.Data[i][j]!=NULL_IC)
            {
                mBinToTray.ConvertBinToAuto(Mot->Tray.DeviceInfo[i][j].iBin);
                return;
            }
        }
    }
}
//------------------------------------------------------------------------------
bool TMagazineModule::Find_LayerHasTray(int iTray)
{
    if(Auto->MAutoTray[iTray]->fHasTray==false)
        return false;
    else
        return true;
}
//------------------------------------------------------------------------------
bool TMagazineModule::CheckAllMagHasTray()
{
    MyBinToTrayStruct BtoT;
    bool bHasTray=false;
    int iT=0;
    for(int i=eMag01; i<eMagTotal; i++)
    {
        iT=i+iLayerBase;
        BtoT.ConvertAutoToBin(iT);
        if(bHasTray==false && BtoT.iBin!=0)
            bHasTray=Find_LayerHasTray(iT);
    }

    if(bHasTray==false)
        bHasTray=MMagSortTopTray->fHasTray;

    return bHasTray;
}
//------------------------------------------------------------------------------
void TMagazineModule::DoMagazine(int &Task)
{
    bool bflag=false;
    AnsiString Str="";

    switch(Task)
    {
        case 1:                                                                 //初始化
            DoTrayFromTopToLayer(true);
            DoTrayFromLayerToTop(0, true);
            DoTakeTrayFromTop(true);
            DoPutTrayTopToLayer(true);
            DoTakeTrayFromLayer(0, true);
            DoPutTrayLayerToTop(true);
            bDoManualMag=false;

            if(HSys.Sys.RunMode==Run_OneCycle &&
               MGZTrayArm->fHasTray==false)                                     //Steven 20240707 : Fixed for one cycle
            {
                bSortMagOneCycleFinish[Tag]=true;
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed)
            {
                bDoManualMagFinish=false;
                Task=6000;
            }
            else
            {
                if(HSys.LastSet.iStartMode==0)
                {
                    if(HSys.FuncT.iT02_MagTraySource==1)
                    {
                        Task=50;
                        DoInitialEmptyTrayToMag(true);
                        bIsInitLoadEmptyTrayToMag=true;
                    }
                    else
                    {
                        Task=100;
                    }
                }
                else if(HSys.FuncT.iT02_MagTraySource==1 &&
                        CheckAllMagHasTray()==false)                            //JerryYang 20240623 : pick up error可以SKIP
                {
                    Task=50;
                    DoInitialEmptyTrayToMag(true);
                    bIsInitLoadEmptyTrayToMag=true;
                }
                else
                {
                    Task=100;
                }
            }
            break;
        case 50:
            if(DoInitialEmptyTrayToMag())                                       //JerryYang 20240623 : Initial Magazine自動補Tray  //Initial自動補空tray到magazine
            {
                bIsInitLoadEmptyTrayToMag=false;
                Task=100;
            }
            break;
        case 100:                                                               //wait
            if(HSys.Sys.RunMode==Run_OneCycle &&
               MGZTrayArm->fHasTray==false)                                     //Steven 20240707 : Fixed for one cycle
            {
                Task=1;
                break;
            }
            else if(MagArmPara->iMagStatus==(int)MagArmPara->eHS_MAG_FullTrayAlarm1+Tag)
            {
                C_MGZ_TopFixTray->Reset();
                Task=200;
                break;
            }
            else if(MMagSortTopTray->fHasTray &&
                    MMagSortTopTray->Tray.iBin!=mTopTray.iBin)
            {
                mTopTray.ConvertBinToAuto(MMagSortTopTray->Tray.iBin);
            }
            else if(MMagSortTopTray->fHasTray==false && mTopTray.iBin!=0)
            {
                mTopTray.ConvertBinToAuto(0);
            }

            if(MagArmPara->mNowBin.iBin>0 &&
               WhichMag(MagArmPara->mNowBin.iBin)==Tag)
            {
                if(MMagSortTopTray->fHasTray)
                {
                    if(MagArmPara->mNowBin.iBin==mTopTray.iBin)
                    {
                        //wait
                    }
                    else
                    {
                        DoTrayFromTopToLayer(true);
                        Task=1000;                                              //需要換Tray
                    }
                }
                else
                {
                    if(MagArmPara->mNowBin.iWhichAuto<iLayerBase ||
                       MagArmPara->mNowBin.iWhichAuto>=iLayerBase+(int)eMagTotal)
                    {
                        ShowMyMessage("TMagazineModule DoMagazine 程式邏輯異常");
                    }
                    else if(Find_LayerHasTray(MagArmPara->mNowBin.iWhichAuto))  //下面有Tray要優先從下面補
                    {
                        DoTrayFromLayerToTop(0, true);
                        Task=2000;
                    }
                    else
                    {
                        DoTrayFromEmpty(MagArmPara->mNowBin.iBin, true);        //下面沒Tray就從外面補
                        Task=500;
                    }
                }
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed)
            {
                bDoManualMagFinish=false;
                Task=6000;
            }
            break;
        case 200:
            bCynActOk1=C_MGZ_TopFixTray->Pop();
            if(bCynActOk1)
            {
                if(HSys.FuncT.iT07_MGZDirection==0)
                {
                    Task=250;
                }
                else if(HSys.FuncT.iT07_MGZDirection==1)
                {
                    DoPutTrayLayerToTop(true);
                    Task=210;
                }
            }
            break;
        case 210:
            if(DoPutTrayLayerToTop(false, true))
            {
                Task=250;
            }
            break;
        case 250:
            if(HSys.FuncT.iT07_MGZDirection==1)
                bCynActOk1=C_MGZ_TopFixTray->Pop();

            Str.sprintf("Mag %d, Bin %d", Tag+1, mTopTray.iBin);
            ShowSystemError(Sn_MGZTopHasTray->OnAlarmCode, K_RETRY, 0, Str);
            Task=300;
            break;
        case 300:
            if(Sn_MGZTopHasTray->IsOn()==false)
            {
                mTopTray.ConvertBinToAuto(0);
                MMagSortTopTray->ClearTray();
                Task=450;
            }
            else
            {
                C_MGZ_TopFixTray->Reset();
                if(HSys.FuncT.iT07_MGZDirection==0)
                    Task=200;
                else
                    Task=250;
            }
            break;
        case 450:
            iMagArmStatus=eHS_MA_FullTrayAlarmFinish;
            mTopTray.ConvertBinToAuto(0);
            MMagSortTopTray->ClearTray();
            Task=460;
            break;
        case 460:
            if(MagArmPara->iMagStatus==MagArmPara->eHS_MAG_Idle)
            {
                iMagArmStatus=eHS_MA_Idle;
                Task=100;
            }
            break;
        case 500:
            bflag=DoTrayFromEmpty(MagArmPara->mNowBin.iBin);
            if(bflag)
                Task=100;
            break;
        case 1000:                                                              //從Top到Layer
            bflag=DoTrayFromTopToLayer();
            if(bflag)
                Task=100;
             break;
        case 2000:                                                              //從Layer到Top
            bflag=DoTrayFromLayerToTop(MagArmPara->mNowBin.iBin);
            if(bflag)
                Task=100;
            break;
        case 6000:
            #ifdef SOFT_SIMULATE
                Task=7000;
            #else
            if(MagArmPara->iMagStatus==(int)MagArmPara->eHS_MAG_FullTrayAlarm1+Tag ||
               MagArmPara->iMagStatus==(int)MagArmPara->eHS_MAG_SafePos)
            {
                C_MGZ_TopFixTray->Reset();
                if(Sn_MGZArmHasTray->IsOff() &&
                   CheckAllMagHasTray()==false)
                {
                    bDoManualMagFinish=true;
                    Task=9999;
                }
                else if(HSys.FuncT.iT07_MGZDirection==0)
                {
                    Task=7000;
                }
                else if(HSys.FuncT.iT07_MGZDirection==1)
                {
                    DoPutTrayLayerToTop(true);
                    Task=6100;
                }
            }
            #endif
            break;
        case 6100:
            if(DoPutTrayLayerToTop(false, true))
            {
                Task=7000;
            }
            break;
        case 7000:
            bCynActOk1=C_MGZ_TopFixTray->Pop();
            if(bCynActOk1)
            {
                bDoManualMagFinish=false;                                       //KenHsieh 20240528 : 手動Magazine Tray Feed
                DoMGZManualTakeTray(true);                                      //KenHsieh 20240528 : 手動Magazine Tray Feed
                Task=7100;
            }
            break;
        case 7100:                                                              //KenHsieh 20240528 : 手動Magazine Tray Feed
            if(DoMGZManualTakeTray())
            {
                bDoManualMagFinish=true;
                Task=9999;
            }
            break;
        case 9999:                                                              //KenHsieh 20240706 : 狀態改變且Trayfeed完成再回1，避免重複進入
            if((HSys.Sys.RunMode!=Run_TrayFeed &&
                bDoManualMagFinish==false)     ||
                HSys.Sys.RunMode==Run_Normal)
            {
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoTrayFromTopToLayer(bool bReset)
{
    int &Task=iTrayFromTopToLayerTask;
    bool bResult=false;
    bool bflag;

    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:                                                                 //initial
            #ifndef SOFT_SIMULATE                                               //Sam 20240613 : 修正 Magazine 模擬
            for(int i=0; i<8; i++)                                              //JerryYang 20240603 : add
            {
                if(HSys.MotPtr[HSys.Mot.SortArm2ZA->Tag+i]->Led[iHomeLed]==false)
                {
                    return false;
                }
            }
            #endif

            if(MMagSortTopTray->Tray.FullIC()==false)
            {
                for(int iC=0; iC<MagArmPara->MagArmSuck->MaxItemC; iC++)        //JerryYang 20240603 : add
                {
                    for(int iR=0; iR<MagArmPara->MagArmSuck->MaxItemR; iR++)
                    {
                        if(MagArmPara->MagArmSuck->Suck[iR][iC].Item!=NULL_IC ||
                           MagArmPara->MagArmSuck->Suck[iR][iC].GetStatus())
                        {
                            if(MagArmPara->MagArmSuck->Suck[iR][iC].DeviceInfo.iBin==MMagSortTopTray->Tray.iBin)
                            {
                                return false;
                            }
                        }
                    }
                }
            }

            if(MagArmPara->iMagStatus==(int)MagArmPara->eHS_MAG_PlaceICTo1+Tag)
            {
                return false;
            }

            DoTakeTrayFromTop(true);
            Task=100;
        case 100:
            if(HSys.FuncT.iT07_MGZDirection==0)
            {
                bflag=DoTakeTrayFromTop();
                if(bflag)
                {
                    DoPutTrayTopToLayer(true);
                    Task=200;
                }
            }
            else if(HSys.FuncT.iT07_MGZDirection==1)
            {
                DoPutTrayTopToLayer(true);
                Task=150;
            }
            break;
        case 150:
            MGZTrayArm->MoveTrayFrom(MMagSortTopTray);                          //這裡TRAY盤已經在MAG ARM上面
            mTopTray.ConvertBinToAuto(0);
            SetMagTopBin(0);                                                    //JerryYang 20240601 : Add MGZ Bin Display
            Task=200;
            break;
        case 200:
            if(DoPutTrayTopToLayer())
            {
                Task=9900;
            }
            break;
        case 9900:  //Finish
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoTakeTrayFromTop(bool bReset, bool bDataTrans)
{
    int &Task=iTakeTrayFromTopTask;
    bool bResult=false;
    bool bSoft_Simulate=false;
    #ifdef SOFT_SIMULATE
        bSoft_Simulate=true;
    #endif

    if(bReset)
    {
        bXMoveFinish=false;
        bZMoveFinish=false;
        bCynActOk1  =false;
        bCynActOk2  =false;
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:                                                                 //Initial
            C_MGZ_TopFixTray->Off();
            bXMoveFinish=false;
            Task=10;
        case 10:
            #ifdef SOFT_SIMULATE                                                //Sam 20240613 : 修正 Magazine 模擬
                bCynActOk1=true;
            #else
                if(HSys.FuncT.iT07_MGZDirection==0)
                    bCynActOk1=C_MGZ_TopFixTray->IsOff();
                else
                    bCynActOk1=true;
            #endif

            if(bCynActOk1)
            {
                bCynActOk1=false;
                bZMoveFinish=false;
                bXMoveFinish=false;
                Task=100;
            }
            else
            {
                break;
            }
        case 100:
            if(bZMoveFinish==false)
                bZMoveFinish=HSys.MotPtr[iMotMGZ_Z]->MotorMove(Z_TopPos);
            if(bXMoveFinish==false)
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_TakeTrayPos);

            if(bZMoveFinish && bXMoveFinish)
            {
                bZMoveFinish=false;
                bXMoveFinish=false;
                C_MGZ_LayerTrayPush->Reset();
                Task=300;
            }
            else
            {
                break;
            }
        case 300:
            if(C_MGZ_LayerTrayPush->Push())
                Task=400;
            else
                break;
        case 400:
            C_MGZ_CatchTray->On();
            bXMoveFinish=false;
            if(HSys.FuncS.bS01_MagX)                                            //Sam 20240619 : 速度獨立設定
            {
                bMagazineXFixSpeed=true;
                HSys.MotPtr[iMotMGZ_X]->SetPersentSpeed(HSys.FuncS.iS01_MagX,false);
            }
            CatchOnDelay.SetMSAndOn(100);                                       //Sam 20240619 : 優化 Magazine 動作
            Task=500;
            break;
        case 500:
            if(CatchOnDelay.Off())
            {
                C_MGZ_LayerTrayPush->Off();
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PutTrayPos);

                if(bXMoveFinish)
                {
                    bXMoveFinish=false;
                    bMagazineXFixSpeed=false;                                   //Sam 20240619 : 速度獨立設定
                    HSys.MotPtr[iMotMGZ_X]->SetPersentSpeed(HSys.MotPtr[iMotMGZ_X]->GetPersentSpeed());
                    Task=600;
                }
            }
            break;
        case 600:
            C_MGZ_CatchTray->Off();
            CatchOffDelay.SetMSAndOn(100);                                      //Sam 20240619 : 優化 Magazine 動作
            Task=700;
            break;
        case 700:
            if(CatchOffDelay.Off())
            {
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PutTrayPos);

                if(bXMoveFinish && (Sn_CatchCyOff->IsOn() || bSoft_Simulate))
                {
                    bXMoveFinish=false;
                    bCynActOk1=false;
                    bCynActOk2=false;
                    C_MGZ_TrayPush->Reset();
                    C_MGZ_EdgeFixer->Reset();
                    Task=800;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        case 800:
            if(Sn_DetectTray->IsOn()==false)
            {
                if(bCynActOk1==false)
                    bCynActOk1=C_MGZ_EdgeFixer->Push();
            }
            else
            {
                AnsiString Str=AnsiString().sprintf("Magazin%d Sn_DetectTray is on and has tray, please check or take the tray to the Mag Tray Arm area.", Tag+1);
                ShowMyMessage(Str);
            }

            if(bCynActOk1)// && bCynActOk2)
            {
                bCynActOk1=false;
                bCynActOk2=false;
                Task=810;                                                       //KenHsieh 20250806 : 改為先側勾再後推，避免因Tray 問題騎到檔塊上
            }
            break;
        case 810:                                                               //KenHsieh 20250806 : 改為先側勾再後推，避免因Tray 問題騎到檔塊上
            if(bCynActOk2==false)
                bCynActOk2=C_MGZ_TrayPush->Push();

            if(bCynActOk2)
            {
                bCynActOk2=false;
                if(bDataTrans)
                {
                    Task=9900;
                }
                else
                {
                    bResult=true;
                    break;
                }
            }
            else
            {
                break;
            }
        case 9900:
            MGZTrayArm->MoveTrayFrom(MMagSortTopTray);                          //這裡TRAY盤已經在MAG ARM上面
            mTopTray.ConvertBinToAuto(0);
            SetMagTopBin(0);                                                    //JerryYang 20240601 : Add MGZ Bin Display
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoPutTrayTopToLayer(bool bReset)
{
    int &Task=iPutTrayTopToLayerTask;
    bool bResult=false;
    AnsiString str="";

    if(bReset)
    {
        bXMoveFinish=false;
        bZMoveFinish=false;
        bCynActOk1  =false;
        bCynActOk2  =false;
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:  //Check
            if(Sn_MGZArmHasTray->IsOff()==false)
            {
                mTopToLayer.ConvertBinToAuto(MGZTrayArm->Tray.iBin);
                if(mTopToLayer.iBin<=0 ||
                   mTopToLayer.iAutoRow!=iLayerRow)
                {
                    ShowMyMessage("問題2");
                    break;
                }
                else if(Find_LayerHasTray(mTopToLayer.iWhichAuto)==true)        //JerryYang 20240530 : 增加保護
                {
                    ShowMyMessage("Search layer error.");
                    break;

                }
                Task=100;
            }
            else
            {
                ShowSystemError(Sn_MGZArmHasTray->Name, K_RETRY);               //例外
                break;
            }
        case 100:
            bZMoveFinish=HSys.MotPtr[iMotMGZ_Z]->MotorMove(GetLayerPosition(mTopToLayer.iWhichAuto-iLayerBase));
            if(bZMoveFinish)
            {
                bZMoveFinish=false;
                C_MGZ_EdgeFixer->Reset();
                C_MGZ_TrayPush->Reset();
                #ifndef SOFT_SIMULATE                                           //Sam 20240613 : 修正 Magazine 模擬
                if(Sn_MGZLayerHasTray->IsOff()==false)                          //JerryYang 20240602 : add protect
                {
                    str.sprintf("Magazine layer%d has tray !!", mTopToLayer.iWhichAuto-iLayerBase+1);
                    ShowMyMessage(str);
                    break;
                }
                #endif
                Task=200;
            }
            else
            {
                break;
            }
        case 200:
            if(bCynActOk1==false)
                bCynActOk1=C_MGZ_EdgeFixer->Pop();
            if(bCynActOk2==false)
                bCynActOk2=C_MGZ_TrayPush->Pop();

            if(bCynActOk1 && bCynActOk2)
            {
                if(HSys.FuncS.bS01_MagX)                                        //Sam 20240619 : 速度獨立設定
                {
                    bMagazineXFixSpeed=true;
                    HSys.MotPtr[iMotMGZ_X]->SetPersentSpeed(HSys.FuncS.iS01_MagX, false);
                }
               bCynActOk1=false;
               bCynActOk2=false;
               Task=300;
            }
            else
            {
                break;
            }
        case 300:
            bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PutTrayPos);
            if(bXMoveFinish)
            {
                bXMoveFinish=false;
                bMagazineXFixSpeed=false;                                       //Sam 20240619 : 速度獨立設定
                HSys.MotPtr[iMotMGZ_X]->SetPersentSpeed(HSys.MotPtr[iMotMGZ_X]->GetPersentSpeed());
                Task=400;
            }
            else
            {
                break;
            }
        case 400:
//            C_MGZ_CatchTray->On();                                            //夾Tray前推會跳盤
            C_MGZ_CatchTray->Off();
            CatchOnDelay.SetMSAndOn(100);                                       //Sam 20240619 : 優化 Magazine 動作
            Task=500;
            break;
        case 500:
            if(CatchOnDelay.Off())
            {
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PushTrayPos); 
                if(bXMoveFinish)
                {
                    bXMoveFinish=false;
                    Task=700;
                }
            }
            break;
        case 700:
            bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_TakeTrayPos);
            if(bXMoveFinish)
            {
                bXMoveFinish=false;
                C_MGZ_SidePush_Up->Reset();
                Task=800;
            }
            else
            {
                break;
            }
        case 800:
            if(C_MGZ_SidePush_Up->Push() || HSys.LastSet.iRealDummy==DUMMY)
            {
                bCynActOk1=false;
                C_MGZ_SidePush->Reset();
                Task=900;
            }
            else
            {
                break;
            }
        case 900:
            bCynActOk1=C_MGZ_SidePush->Push();
            if(bCynActOk1==true)
            {
                if(Sn_TrayDirection->IsOn()==false)                             //Sam 20240613 : 修正 Magazine 模擬
                {
                    C_MGZ_SidePush->Reset();
                    SidePushDelay.SetMSAndOn(1000);
                    Task=1000;
                }
                else
                {
                    ShowSystemError(Sn_TrayDirection->Name, K_RETRY);           //例外;
                }
            }
            break;
        case 1000:
            if(SidePushDelay.Off())
            {
                if(C_MGZ_SidePush->Pop())
                {
                    C_MGZ_SidePush_Up->Reset();
                    Task=1010;
                }
            }
            break;
        case 1010:
            if(C_MGZ_SidePush_Up->Pop())
            {
                if(Sn_DetectTray->IsOn()==true)
                {
                    AnsiString Str=AnsiString().sprintf("Magazin%d Sn_DetectTray is on and has tray, Please check or take the tray to the Layer.", Tag+1);
                    ShowMyMessage(Str);
                    break;
                }
                else if(Sn_TrayPositionF->IsOn()==false &&                      //Sam 20250703 : 修正模擬
                        Sn_TrayPositionR->IsOn()==false)
                {
                    Task=9900;
                }
                else
                {
                    ShowSystemError(Sn_TrayPositionF->Name, K_RETRY);           //例外;
                    break;
                }
            }
            else
            {
                break;
            }
        case 9900:                                                              //Finish
            Auto->MAutoTray[mTopToLayer.iWhichAuto]->MoveTrayFrom(MGZTrayArm);
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoTrayFromLayerToTop(int iBin, bool bReset)
{
    int &Task=iTrayFromLayerToTopTask;
    bool bResult=false;

    if(bReset)
    {
        Task=1;
        bXMoveFinish=false;
        bZMoveFinish=false;
        return bResult;
    }

    switch(Task)
    {
        case 1:                                                                 //initial
            mBinToTray.ConvertBinToAuto(iBin);
            if(MMagNowSortTray->HasIC()==false &&
               MMagNowSortTray->fHasTray &&
               Find_LayerHasTray(mBinToTray.iWhichAuto)==false)                 //JerryYang 20240606 : fix Layer沒tray還跑去取
            {
                return true;
            }
            if(mBinToTray.iWhichAuto>eBinNotUse &&
               mBinToTray.iWhichAuto<eTrayCount)
            {
                if(Find_LayerHasTray(mBinToTray.iWhichAuto)==false)             //判斷此層有無Tray
                {
                    bXMoveFinish=false;
                    bZMoveFinish=false;
                    Task=10;
                }
                else
                {
                    DoTakeTrayFromLayer(0, true);
                    DoPutTrayLayerToTop(true);
                    Task=100;
                    break;
                }
            }
            else
            {
                DoTakeTrayFromLayer(0, true);
                DoPutTrayLayerToTop(true);
                Task=100;
                break;
            }
        case 10:
            if(bXMoveFinish==false)
            {
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_TakeTrayPos);
            }
            if(bZMoveFinish==false)
            {
                bZMoveFinish=HSys.MotPtr[iMotMGZ_Z]->MotorMove(Z_TopPos);
            }
            if(bXMoveFinish && bZMoveFinish)
            {
                bXMoveFinish=false;
                bZMoveFinish=false;
                bResult=true;
                Task=1;
            }
            break;
        case 100:
            if(DoTakeTrayFromLayer(iBin))                                       //從下層取出Tray
                Task=200;
            break;
        case 200:
            if(DoPutTrayLayerToTop())                                           //從下面移動一盤到頂層
                Task=9900;
            break;
        case 9900:                                                              //Finish
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoTakeTrayFromLayer(int iBin, bool bReset)                //從下層取出Tray
{
    int &Task=iTakeTrayFromLayerTask;
    bool bResult=false;

    if(bReset)
    {
        bXMoveFinish=false;
        bZMoveFinish=false;
        bCynActOk1  =false;
        bCynActOk2  =false;
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            mBinToTray.ConvertBinToAuto(iBin);
            if(mBinToTray.iWhichAuto>eBinNotUse &&
               mBinToTray.iWhichAuto<eTrayCount)
            {
                bXMoveFinish=false;
                Task=5;
            }
            else
            {
                ShowMyMessage("have problem");
                break;
            }
            break;
        case 5:
            bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_TakeTrayPos);
            if(bXMoveFinish)
            {
                bXMoveFinish=false;
                Task=10;
            }
            else
            {
                break;
            }
        case 10:
//            if(Sn_MGZArmHasTray->IsOff()==false)      //Sam 20240619 : 因為要加快 Magazine 換 Tray 速度，iMotMGZ_X 待機位置都改在 X_TakeTrayPos 位置上，這個偵測會有問題先 Mark
//            {
//                ShowMyMessage("Please take off the tray from MAG Arm!");
//                break;
//            }
//            else
//            {
                Task=20;
//            }
            //break;
        case 20:
            if(bCheckMGZZCanMve())
            {
                bXMoveFinish=false;
                bZMoveFinish=false;
                Task=100;
            }
            else
            {
                ShowSystemError(Sn_TrayPositionF->Name, K_RETRY);               //KenHsieh 20250812 : 新增異常提示，避免Hangup
                break;
            }
        case 100:
            if(bZMoveFinish==false)
                bZMoveFinish=HSys.MotPtr[iMotMGZ_Z]->MotorMove(GetLayerPosition(mBinToTray.iWhichAuto-iLayerBase));
            if(bXMoveFinish==false)
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_TakeTrayPos);

            if(bZMoveFinish && bXMoveFinish)
            {
                bXMoveFinish=false;
                bZMoveFinish=false;
                C_MGZ_LayerTrayPush->Reset();
                Task=200;
            }
            else
            {
                break;
            }
        case 200:
            if(C_MGZ_LayerTrayPush->Push())
                Task=300;
            else
                break;
        case 300:
            if(HSys.FuncS.bS01_MagX)                                            //Sam 20240619 : 速度獨立設定
            {
                bMagazineXFixSpeed=true;
                HSys.MotPtr[iMotMGZ_X]->SetPersentSpeed(HSys.FuncS.iS01_MagX, false);
            }
            C_MGZ_CatchTray->On();
            CatchOnDelay.SetMSAndOn(100);
            Task=400;
        case 400:
            if(CatchOnDelay.Off())
            {
                C_MGZ_LayerTrayPush->Off();
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PutTrayPos);

                if(bXMoveFinish)
                {
                    bMagazineXFixSpeed=false;                                   //Sam 20240619 : 速度獨立設定
                    HSys.MotPtr[iMotMGZ_X]->SetPersentSpeed(HSys.MotPtr[iMotMGZ_X]->GetPersentSpeed());
                    bXMoveFinish=false;
                    Task=500;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        case 500:
            C_MGZ_CatchTray->Off();
            CatchOffDelay.SetMSAndOn(100);
            Task=600;
        case 600:
            if(CatchOffDelay.Off())
            {
                bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PutTrayPos);
                if(bXMoveFinish && Sn_CatchCyOff->IsOff()==false)               //Sam 20240613 : 修正 Magazine 模擬
                {
                    bXMoveFinish=false;
                    bCynActOk1=false;
                    bCynActOk2=false;
                    C_MGZ_TrayPush->Reset();
                    C_MGZ_EdgeFixer->Reset();
                    Task=700;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        case 700:
            if(Sn_DetectTray->IsOn()==false)
            {
                if(bCynActOk1==false)
                    bCynActOk1=C_MGZ_EdgeFixer->Push();
            }
            else
            {
                AnsiString Str=AnsiString().sprintf("Magazin%d Sn_DetectTray is on and has tray, Please check or take the tray to the Mag elevator area.", Tag+1);
                ShowMyMessage(Str);
            }

            if(bCynActOk1)
            {
               bCynActOk1=false;
               bCynActOk2=false;
               Task=710;                                                        //KenHsieh 20250806 : 改為先側勾再後推，避免因Tray 問題騎到檔塊上
            }
            break;
         case 710:                                                              //KenHsieh 20250806 : 改為先側勾再後推，避免因Tray 問題騎到檔塊上
            if(bCynActOk2==false)
                bCynActOk2=C_MGZ_TrayPush->Push();

            if(bCynActOk2)
            {
                bCynActOk2=false;
                Task=9900;
            }
            else
            {
                break;
            }
        case 9900:                                                              //Finfish
            MGZTrayArm->MoveTrayFrom(Auto->MAutoTray[mBinToTray.iWhichAuto]);
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoPutTrayLayerToTop(bool bReset, bool bFullTray)          //從下面移動一盤到頂層
{
    int &Task=iPutTrayLayerToTopTask;
    bool bResult=false;

    if(bReset)
    {
        bXMoveFinish=false;
        bZMoveFinish=false;
        bCynActOk1  =false;
        bCynActOk2  =false;
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(Sn_MGZArmHasTray->IsOff()==false)                                //Sam 20240613 : 修正 Magazine 模擬
            {
                bCynActOk1=false;
                C_MGZ_TopFixTray->Reset();
                Task=100;
            }
            else
            {
                ShowMyMessage(AnsiString().sprintf("Magazin%d Top has no tray, Please check.", Tag+1));   //Jimmychiu 20260223 : fixed for sensor on without tray and hangup
            }
            break;
        case 100:
            if(bCheckMGZZCanMve())
            {
                bZMoveFinish=HSys.MotPtr[iMotMGZ_Z]->MotorMove(Z_TopPos);
                if(bZMoveFinish)
                {
                    bZMoveFinish=false;
                    bCynActOk1=false;
                    bCynActOk2=false;
                    if(HSys.FuncT.iT07_MGZDirection==0 ||
                      (HSys.FuncT.iT07_MGZDirection==1 &&
                      (bFullTray || HSys.Sys.RunMode==Run_TrayFeed)))
                    {
                        C_MGZ_EdgeFixer->Reset();
                        C_MGZ_TrayPush->Reset();
                        Task=200;
                    }
                    else if(HSys.FuncT.iT07_MGZDirection==1)
                    {
                        Task=9900;
                    }
                }
                else
                {
                    break;
                }
            }
            else                                                                //KenHsieh 20250812 : 新增異常提示，避免Hangup
            {
                ShowSystemError(Sn_TrayPositionF->Name, K_RETRY);
            }
            break;
        case 200:
            if(bCynActOk1==false)
                bCynActOk1=C_MGZ_EdgeFixer->Pop();
            if(bCynActOk2==false)
                bCynActOk2=C_MGZ_TrayPush->Pop();

            if(bCynActOk1 && bCynActOk2)
            {
                bCynActOk1=false;
                bCynActOk2=false;
                Task=300;
            }
            else
            {
                break;
            }
        case 300:
            bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PutTrayPos);
            if(bXMoveFinish)
            {
                bXMoveFinish=false;
                Task=400;
            }
            else
            {
                break;
            }
        case 400:
            if(HSys.FuncS.bS01_MagX)                                            //Sam 20240619 : 速度獨立設定
            {
                bMagazineXFixSpeed=true;
                HSys.MotPtr[iMotMGZ_X]->SetPersentSpeed(HSys.FuncS.iS01_MagX, false);
            }
            C_MGZ_CatchTray->Off();
            Task=500;
        case 500:
            bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_PushTrayPos);
            if(bXMoveFinish)
            {
                bXMoveFinish=false;
                bMagazineXFixSpeed=false;                                       //Sam 20240619 : 速度獨立設定
                HSys.MotPtr[iMotMGZ_X]->SetPersentSpeed(HSys.MotPtr[iMotMGZ_X]->GetPersentSpeed());
                Task=700;
            }
            else
            {
                break;
            }
        case 700:
            bXMoveFinish=HSys.MotPtr[iMotMGZ_X]->MotorMove(X_TakeTrayPos);
            if(bXMoveFinish)
            {
                bXMoveFinish=false;
                C_MGZ_SidePush_Up->Reset();
                Task=800;
            }
            else
            {
                break;
            }
        case 800:
            if(C_MGZ_SidePush_Up->Push() || HSys.LastSet.iRealDummy==DUMMY)
            {
                bCynActOk1=false;
                C_MGZ_SidePush->Reset();
                Task=900;
            }
            else
            {
                break;
            }
        case 900:
            bCynActOk1=C_MGZ_SidePush->Push();

            if(bCynActOk1==true)
            {
                if(Sn_TrayDirection->IsOn()==false)                             //Sam 20240613 : 修正 Magazine 模擬
                {
                    C_MGZ_SidePush->Reset();
                    C_MGZ_TopFixTray->Reset();
                    bCynActOk1=false;
                    Task=910;
                }
                else    //例外
                {
                    ShowSystemError(Sn_TrayDirection->Name, K_RETRY);
                    break;
                }
            }
            else
            {
                break;
            }
        case 910:
            bCynActOk1=C_MGZ_TopFixTray->Push();

            if(bCynActOk1)
            {
                bCynActOk1=false;
                C_MGZ_SidePush->Reset();
                SidePushDelay.SetMSAndOn(100);
                Task=1000;
            }
            break;
        case 1000:
            if(SidePushDelay.Off())
            {
                bCynActOk1=C_MGZ_SidePush->Pop();
                if(bCynActOk1==true)
                {
                    C_MGZ_SidePush_Up->Reset();
                    Task=1010;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        case 1010:
            bCynActOk1=C_MGZ_SidePush_Up->Pop();
            if(bCynActOk1==true)
            {
                if(Sn_TrayPositionF->IsOn()==false &&                           //Sam 20250703 : 修正模擬
                   Sn_TrayPositionR->IsOn()==false)
                {
                    if(HSys.FuncT.iT07_MGZDirection==1 && bFullTray)
                    {
                        bResult=true;
                        break;
                    }
                    else
                    {
                        Task=9900;
                    }
                }
                else if(Sn_TrayPositionF->IsOn())
                {
                    ShowSystemError(Sn_TrayPositionF->Name, K_RETRY);           //例外;
                    break;
                }
                else
                {
                    ShowSystemError(Sn_TrayPositionR->Name, K_RETRY);           //例外;
                    break;
                }
            }
            else
            {
                break;
            }
        case 9900:                                                              //Finish
            MMagSortTopTray->MoveTrayFrom(MGZTrayArm);
            mTopTray.ConvertBinToAuto(MMagSortTopTray->Tray.iBin);
            SetMagTopBin(MMagSortTopTray->Tray.iBin);
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TMagazineModule::bCheckMGZZCanMve()
{
    #ifdef SOFT_SIMULATE                                                        //Sam 20240613 : 修正 Magazine 模擬
        return true;
    #else
    bool ret=false;
    if(Sn_TrayPositionR->IsOff() &&
       Sn_TrayPositionF->IsOff() &&
      (Sn_TrayDirection->Enable==false ||
       Sn_TrayDirection->IsOff()))
    {
        ret=true;
    }
    return ret;
    #endif
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoMGZManualTakeTray(bool bReset)
{
    AnsiString Str;
    int iT=eBinNotUse;
    int &Task=iManualTakeTrayTask;
    bool bResult=false, bLayerHasTray=false;

    if(bReset)
    {
        bDoManualMag=true;
        iManualTakeTrayTask=1;
        mTemp.ConvertBinToAuto(0);
        bCynActOk1=false;
        return bResult;
    }

    switch(Task)
    {
        case 1:
             if(MMagSortTopTray->fHasTray)
            {
                if(Sn_MGZTopHasTray->IsOff()==false)         //判斷Top有Tray
                    Task=500;
                else
                    ShowMyMessage("Mag Top有Tray資料 Sensor未偵測，請確認Tray 位置");
            }
            else
            {
                if(Sn_MGZTopHasTray->IsOff()==false)                            //KenHsieh 20240615 : 有Tray 要移除
                    Task=500;
                else
                    Task=100;
            }
            break;
        case 100:
            for(int i=eMag01; i<eMagTotal; i++)
            {
                iT=i+iLayerBase;
                if(MGZLayer[i]->fHasTray)
                {
                    mTemp.ConvertAutoToBin(iT);
                    bLayerHasTray=true;
                    break;
                }
            }

            if(bLayerHasTray)
            {
                DoTrayFromLayerToTop(mTemp.iBin, true);
                Task=200;
            }
            else
            {
                Task=9900;
            }
            break;
        case 200:
            if(DoTrayFromLayerToTop(mTemp.iBin, false))
                Task=500;
            break;
        case 500:
            bCynActOk1=false;
            C_MGZ_TopFixTray->Reset();
            Task=510;
            break;
        case 510:
            bCynActOk1=C_MGZ_TopFixTray->Pop();

            if(bCynActOk1)
            {
                bCynActOk1=false;
                Task=600;
            }
            break;
        case 600:
            bMgzStepLight=true;
            SetMagTopBin(MMagSortTopTray->Tray.iBin);
            #ifndef SOFT_SIMULATE                                               //Sam 20250703 : 修正模擬
            Str.sprintf("Please take out the tray from Magazine %d top. Bin : %d, IC Count : %d", Tag+1, MMagSortTopTray->Tray.iBin, MMagSortTopTray->Tray.HowManyIC());    //Sam 20250910 : Magazine Tray Feed add ic count info
            ShowMyMessage(Str);                                                 //JerryYang 20240608 : add bin顯示
            #endif
            Task=700;
            break;
        case 700:
            if(Sn_MGZTopHasTray->IsOn()==false)                                 //Sam 20250703 : 修正模擬
            {
                if(CUSTOMER_CODE==CC_WINSTEK)
                {
                    MMagSortTopTray->SaveWinstekLog();
                }
                MMagSortTopTray->ClearTray();
                MMagSortTopTray->Tray.iBin=0;
                Sw_MGZStep->Off();
                SetMagTopBin(0);
                bMgzStepLight=false;
                Task=100;
            }
            else
            {
                Task=600;
            }
            break;
        case 9900:
            bDoManualMag=false;
            bMgzStepLight=false;
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
void TMagazineModule::ManualStepBtnCanLight()                                   //JerryYang 20240608 : 整合Magazine手動取盤
{
    if(bEnable==false)
        return;

    if(bMgzStepLight && Sn_MGZTopHasTray->IsOff())
        Sw_MGZStep->On();
    else
        Sw_MGZStep->Off();
}
//------------------------------------------------------------------------------
void TMagazineModule::SetMagTopBin(int iNum)                                    //JerryYang 20240601 : Add MGZ Bin Display
{
    #ifdef SOFT_SIMULATE
    return;
    #else
    if(iNum<0)
        iNum=123;
    iBinDisp[0]=iNum;
    AnsiString sLog="";
    sLog.sprintf("Tag=%d, Bin=%d, T04=%d, T05=%d", Tag, iNum, HSys.FuncT.iT04_BinDisptype, HSys.FuncT.iT05_BinDispComHardwareType);
    if(HSys.FuncT.iT04_BinDisptype==0)
    {
        if(HSys.BinDisCtrlMag!=NULL)
            HSys.BinDisCtrlMag->AddBinDisplayLog("MagTop", sLog+", Ctrl=NormalMag");
        HSys.BinDisCtrlMag->WriteTargetBin(Tag, iBinDisp, 1);
        HSys.BinDisCtrlMag->bFirstInit=true;
        HSys.BinDisCtrlMag->ProcessStopStart(true);
    }
    else if(HSys.FuncT.iT04_BinDisptype==1)
    {
        if(HSys.FuncT.iT05_BinDispComHardwareType==0)
        {
            if(HSys.BinDisCtrlMagTFT!=NULL)
            {
                HSys.BinDisCtrlMagTFT->AddBinDisplayLog("MagTop", sLog+", Ctrl=MagTFT, Index="+IntToStr(Tag));
                HSys.BinDisCtrlMagTFT->RequestSetBinTFTNumber(Tag, iNum, 2);
            }
        }
        else
        {
            if(HSys.BinDisCtrlTFT!=NULL)
            {
                int iDisplayIndex=eBinDispTotal+Tag;
                HSys.BinDisCtrlTFT->AddBinDisplayLog("MagTop", sLog+", Ctrl=MainTFT, Index="+IntToStr(iDisplayIndex));
                HSys.BinDisCtrlTFT->RequestSetBinTFTNumber(iDisplayIndex, iNum, 2);
            }
        }
    }
    #endif
}
//------------------------------------------------------------------------------
int TMagazineModule::GetLayerPosition(int iLayer)
{
    int iLayPos=0;
    if(0<=iLayer && iLayer<(int)eMagTotal)
    {
        iLayPos=2000*iLayer;
        iLayPos=Z_LayerPos-iLayPos;
    }
    return iLayPos;
}
//------------------------------------------------------------------------------
bool TMagazineModule::TrayFeedFinish()
{
    return (bEnable==true && bDoManualMagFinish==true) || (bEnable==false);
}
//------------------------------------------------------------------------------
void TMagazineModule::SetEnable(bool _b)
{
    bEnable=_b;
}
//------------------------------------------------------------------------------
int TMagazineModule::WhichMag(int iBin)
{
    MyBinToTrayStruct mTemp;
    mTemp.ConvertBinToAuto(iBin);
    if(mTemp.iAutoRow==eTrackMgz1 &&
       (mTemp.iWhichAuto>=eMag1_01 && mTemp.iWhichAuto<=eMag1_27))
    {
        return eMagazine1;
    }
    else if(mTemp.iAutoRow==eTrackMgz2 &&
            (mTemp.iWhichAuto>=eMag2_01 && mTemp.iWhichAuto<=eMag2_27))
    {
        return eMagazine2;
    }
    else if(mTemp.iAutoRow==eTrackMgz3 &&
            (mTemp.iWhichAuto>=eMag3_01 && mTemp.iWhichAuto<=eMag3_27))
    {
        return eMagazine3;
    }
    else
    {
        return -1;
    }
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoInitialEmptyTrayToMag(bool bReset)                      //JerryYang 20240623 : Initial Magazine自動補Tray
{
    int &Task=iInitialEmptyTrayToMagTask, iT;
    bool bResult=false;

    static MyBinToTrayStruct mTemp;
    bool bNeedTray=false;
    bool bflag=false;

    if(bReset)
    {
        iInitialEmptyTrayToMagTask=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            bNeedTray=false;
            for(int i=eMag01; i<eMagTotal; i++)
            {
                iT=i+iLayerBase;
                mTemp.ConvertAutoToBin(iT);
                if(bNeedTray==false && mTemp.iBin!=0)
                {
                    if(Find_LayerHasTray(i)==false)
                    {
                        bNeedTray=true;
                    }
                }
                if(bNeedTray==true)
                {
                    break;
                }
            }

            if(bNeedTray==true)
            {
                DoTrayFromEmpty(mTemp.iBin, true);
                Task=100;
                break;
            }
            else
            {
                bResult=true;
            }
            break;

        case 100:
            bflag=DoTrayFromEmpty(mTemp.iBin);
            if(bflag)
            {
                Task=200;
                DoTrayFromTopToLayer(true);
            }
            break;
        case 200:                                                               //從Top到Layer
            bflag=DoTrayFromTopToLayer();
            if(bflag)
                Task=1;
             break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TMagazineModule::DoPutTrayManualToTop(bool bReset)                         //JerryYang 20240623 : Magazine手動補Tray
{
    int &Task=iPutTrayLayerToTopTask;
    bool bResult=false;

    if(bReset)
    {
        bXMoveFinish=false;
        bZMoveFinish=false;
        Task=1;
        return bResult;
    }

    if(HSys.FuncT.iT02_MagTraySource==0)
    {
        if(Sn_MGZTopHasTray->IsOn()==false)
        {
            bResult=true;
        }
        return bResult;
    }

    switch(Task)
    {
        case 1:
            C_MGZ_SidePush_Up->Reset();
            Task=800;
            break;
        case 800:
            if(C_MGZ_SidePush_Up->Push() || HSys.LastSet.iRealDummy==DUMMY)
            {
                C_MGZ_SidePush->Reset();
                Task=900;
            }
            else
            {
                break;
            }
        case 900:
            if(C_MGZ_SidePush->Push())
            {
                if(Sn_TrayDirection->Enable==false ||
                   Sn_TrayDirection->IsOff())
                {
                    C_MGZ_SidePush->Reset();
                    C_MGZ_TopFixTray->Reset();
                    Task=910;
                }
                else                                                            //例外
                {
                    ShowSystemError(Sn_TrayDirection->Name, K_RETRY);
                    break;
                }
            }
            else
            {
                break;
            }
        case 910:
            bCynActOk1=C_MGZ_TopFixTray->Push();

            if(bCynActOk1==true)
            {
                C_MGZ_SidePush->Reset();
                SidePushDelay.SetMSAndOn(100);
                Task=1000;
            }
            break;
        case 1000:
            if(SidePushDelay.Off())
            {
                if(C_MGZ_SidePush->Pop())
                {
                    C_MGZ_SidePush_Up->Reset();
                    Task=1010;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        case 1010:
            if(C_MGZ_SidePush_Up->Pop())
            {
                if(Sn_TrayPositionF->IsOn()==false &&
                   Sn_TrayPositionR->IsOn()==false)
                {
                    Task=9900;
                }
                else if(Sn_TrayPositionF->IsOn())
                {
                    ShowSystemError(Sn_TrayPositionF->Name, K_RETRY);           //例外;
                    break;
                }
                else
                {
                    ShowSystemError(Sn_TrayPositionR->Name, K_RETRY);           //例外;
                    break;
                }
            }
            else
            {
                break;
            }
        case 9900:                                                              //Finish
            iMagArmStatus=eHS_MA_FullTrayAlarmFinish;
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------

