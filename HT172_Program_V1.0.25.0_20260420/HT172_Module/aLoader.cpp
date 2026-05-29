#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "aLoader.h"
#include "DecodeXML.h"
#include "uhome.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
TLoaderModule *LoaderModule=new TLoaderModule();
TAutoModule   *Auto=new TAutoModule;
//------------------------------------------------------------------------------
__fastcall TAutoModule::TAutoModule()
{

}
//------------------------------------------------------------------------------
void InitialAuto()                                                              //初始化Robot所需要的參數
{
    LoaderModule->MCarrierStoreX[eTrack1]   =HSys.Mot.AutoTrolly1X              ;
    LoaderModule->MCarrierStoreX[eTrack2]   =HSys.Mot.AutoTrolly2X              ;
    LoaderModule->MCarrierStoreX[eTrack3]   =HSys.Mot.AutoTrolly3X              ;
    LoaderModule->MCarrierStoreX[eTrack4]   =HSys.Mot.AutoTrolly4X              ;
    LoaderModule->MCarrierStoreZ[eTrack1]   =HSys.Mot.AutoTrolly1Z              ;
    LoaderModule->MCarrierStoreZ[eTrack2]   =HSys.Mot.AutoTrolly2Z              ;
    LoaderModule->MCarrierStoreZ[eTrack3]   =HSys.Mot.AutoTrolly3Z              ;
    LoaderModule->MCarrierStoreZ[eTrack4]   =HSys.Mot.AutoTrolly4Z              ;

    Auto->Mot_Auto_Work[eTrack1][eTrackCol1]=HSys.VMot.tmAutoAreaAuto[ 1]       ;
    Auto->Mot_Auto_Work[eTrack1][eTrackCol2]=HSys.VMot.tmAutoAreaAuto[ 2]       ;
    Auto->Mot_Auto_Work[eTrack1][eTrackCol3]=HSys.VMot.tmAutoAreaAuto[ 3]       ;
    Auto->Mot_Auto_Work[eTrack1][eTrackCol4]=HSys.VMot.tmAutoAreaAuto[ 4]       ;
    Auto->Mot_Auto_Work[eTrack1][eTrackCol5]=HSys.VMot.tmAutoAreaAuto[ 5]       ;
    Auto->Mot_Auto_Work[eTrack2][eTrackCol1]=HSys.VMot.tmAutoAreaAuto[ 6]       ;
    Auto->Mot_Auto_Work[eTrack2][eTrackCol2]=HSys.VMot.tmAutoAreaAuto[ 7]       ;
    Auto->Mot_Auto_Work[eTrack2][eTrackCol3]=HSys.VMot.tmAutoAreaAuto[ 8]       ;
    Auto->Mot_Auto_Work[eTrack2][eTrackCol4]=HSys.VMot.tmAutoAreaAuto[ 9]       ;
    Auto->Mot_Auto_Work[eTrack2][eTrackCol5]=HSys.VMot.tmAutoAreaAuto[10]       ;
    Auto->Mot_Auto_Work[eTrack3][eTrackCol1]=HSys.VMot.tmAutoAreaAuto[11]       ;
    Auto->Mot_Auto_Work[eTrack3][eTrackCol2]=HSys.VMot.tmAutoAreaAuto[12]       ;
    Auto->Mot_Auto_Work[eTrack3][eTrackCol3]=HSys.VMot.tmAutoAreaAuto[13]       ;
    Auto->Mot_Auto_Work[eTrack3][eTrackCol4]=HSys.VMot.tmAutoAreaAuto[14]       ;
    Auto->Mot_Auto_Work[eTrack3][eTrackCol5]=HSys.VMot.tmAutoAreaAuto[15]       ;
    Auto->Mot_Auto_Work[eTrack4][eTrackCol1]=HSys.VMot.tmAutoAreaAuto[16]       ;
    Auto->Mot_Auto_Work[eTrack4][eTrackCol2]=HSys.VMot.tmAutoAreaAuto[17]       ;
    Auto->Mot_Auto_Work[eTrack4][eTrackCol3]=HSys.VMot.tmAutoAreaAuto[18]       ;
    Auto->Mot_Auto_Work[eTrack4][eTrackCol4]=HSys.VMot.tmAutoAreaAuto[19]       ;
    Auto->Mot_Auto_Work[eTrack4][eTrackCol5]=HSys.VMot.tmAutoAreaAuto[20]       ;

    Auto->MTrackToStore[eTrack1]        =HSys.VMot.Track_1_TrayArm              ;
    Auto->MTrackToStore[eTrack2]        =HSys.VMot.Track_2_TrayArm              ;
    Auto->MTrackToStore[eTrack3]        =HSys.VMot.Track_3_TrayArm              ;
    Auto->MTrackToStore[eTrack4]        =HSys.VMot.Track_4_TrayArm              ;

    for(int i=eBinNotUse; i<eTrayCount; i++)
        Auto->MAutoTray[i]              =HSys.VMot.tmAutoAreaAuto[i]            ;
}
//------------------------------------------------------------------------------
void InitialLoader()                                                            //初始化Robot所需要的參數
{
    LoaderModule->C_Separatory[eLoader1]    =&HSys.Cyn.C_TrayZ_Selector         ;
    LoaderModule->C_Separatory[eEmpty1 ]    =&HSys.Cyn.C_Empty1Z_Selector       ;
    LoaderModule->C_Separatory[eEmpty2 ]    =&HSys.Cyn.C_Empty2Z_Selector       ;

    LoaderModule->C_Middle[eLoader1]        =&HSys.Cyn.C_Load_Middle            ;
    LoaderModule->C_Middle[eEmpty1 ]        =&HSys.Cyn.C_Empty1_Middle          ;
    LoaderModule->C_Middle[eEmpty2 ]        =&HSys.Cyn.C_Empty2_Middle          ;

    LoaderModule->C_Up[eLoader1]            =&HSys.Cyn.C_Load_Up                ;
    LoaderModule->C_Up[eEmpty1 ]            =&HSys.Cyn.C_Empty1_Up              ;
    LoaderModule->C_Up[eEmpty2 ]            =&HSys.Cyn.C_Empty2_Up              ;

    LoaderModule->C_EdgePush[eLoader1]      =&HSys.Cyn.C_LoaderEdgePush         ;
    LoaderModule->C_EdgePush[eEmpty2]       =&HSys.Cyn.C_Empty2EdgePush         ;

    LoaderModule->C_Fixer[eLoader1]         =&HSys.Cyn.C_TrayY_Fixer            ;
    LoaderModule->C_Fixer[eEmpty2]          =&HSys.Cyn.C_Empty2_Fixer           ;

    LoaderModule->C_Stop[eLoader1]          =&HSys.Cyn.C_Loader_Stop            ;
    LoaderModule->C_Stop[eEmpty1 ]          =&HSys.Cyn.C_Empty1_Stop1           ;
    LoaderModule->C_Stop[eEmpty2 ]          =&HSys.Cyn.C_Empty2_Stop            ;

    LoaderModule->TrayName[eLoader1]        ="Loader";
    LoaderModule->TrayName[eEmpty1 ]        ="Empty1";
    LoaderModule->TrayName[eEmpty2 ]        ="Empty2";

    LoaderModule->Sn_CCDPos[eLoader1]       =&HSys.Sen.SnLoaderCCDPos           ;
    LoaderModule->Sn_CCDPos[eEmpty1 ]       =&HSys.Sen.SnEmpty1CCDPosition      ;
    LoaderModule->Sn_CCDPos[eEmpty2 ]       =&HSys.Sen.SnEmpty2CCDPosition      ;

    LoaderModule->C_CCD_Stop[eLoader1]      =&HSys.Cyn.C_Loader_CCDStop         ;
    LoaderModule->C_CCD_Stop[eEmpty1 ]      =&HSys.Cyn.C_Empty1_CCDStop         ;
    LoaderModule->C_CCD_Stop[eEmpty2 ]      =&HSys.Cyn.C_Empty2_CCDStop         ;

    LoaderModule->SnSelectHasTray[eLoader1] =&HSys.Sen.SnLoaderTrayHasTray      ;
    LoaderModule->SnSelectHasTray[eEmpty1 ] =&HSys.Sen.SnEmpty1SelectHasTray    ;
    LoaderModule->SnSelectHasTray[eEmpty2 ] =&HSys.Sen.SnEmpty2SelectHasTray    ;

    LoaderModule->Sn_CarHasTray[eLoader1]   =&HSys.Sen.SnLoaderCarHasTray       ;
    LoaderModule->Sn_CarHasTray[eEmpty1 ]   =&HSys.Sen.SnEmpty1CarHasTray       ;
    LoaderModule->Sn_CarHasTray[eEmpty2 ]   =&HSys.Sen.SnEmpty2CarHasTray       ;

    LoaderModule->Sn_CarIsFull[eLoader1] =&HSys.Sen.SnLoaderIsFull      ;
    LoaderModule->Sn_CarIsFull[eEmpty1 ] =&HSys.Sen.SnEmpty1IsFull    ;
    LoaderModule->Sn_CarIsFull[eEmpty2 ] =&HSys.Sen.SnEmpty2IsFull    ;

    LoaderModule->SnLDPreDete               =&HSys.Sen.SnLoaderPreDete          ;
    LoaderModule->SnLDSureTray              =&HSys.Sen.SnLoaderSureTray         ;
    LoaderModule->SnLD_FixCyPush            =&HSys.Sen.SnLoaderFixCyPush        ;     //KenHsieh 20240528 : 補上汽缸到位保護
    LoaderModule->SnLD_EdgePush             =&HSys.Sen.SnLoaderEdgePush         ;     //KenHsieh 20240528 : 補上汽缸到位保護

    LoaderModule->SnAutoTrollyHasTray[eTrack1] =&HSys.Sen.SnAutoTrolly1HasTray  ;
    LoaderModule->SnAutoTrollyHasTray[eTrack2] =&HSys.Sen.SnAutoTrolly2HasTray  ;
    LoaderModule->SnAutoTrollyHasTray[eTrack3] =&HSys.Sen.SnAutoTrolly3HasTray  ;
    LoaderModule->SnAutoTrollyHasTray[eTrack4] =&HSys.Sen.SnAutoTrolly4HasTray  ;

    LoaderModule->C_Auto_Stop[eTrack1][eTrackCol1]=&HSys.Cyn.C_Auto1_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack1][eTrackCol2]=&HSys.Cyn.C_Auto2_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack1][eTrackCol3]=&HSys.Cyn.C_Auto3_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack1][eTrackCol4]=&HSys.Cyn.C_Auto4_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack1][eTrackCol5]=&HSys.Cyn.C_Auto5_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack2][eTrackCol1]=&HSys.Cyn.C_Auto6_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack2][eTrackCol2]=&HSys.Cyn.C_Auto7_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack2][eTrackCol3]=&HSys.Cyn.C_Auto8_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack2][eTrackCol4]=&HSys.Cyn.C_Auto9_Stop       ;
    LoaderModule->C_Auto_Stop[eTrack2][eTrackCol5]=&HSys.Cyn.C_Auto10_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack3][eTrackCol1]=&HSys.Cyn.C_Auto11_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack3][eTrackCol2]=&HSys.Cyn.C_Auto12_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack3][eTrackCol3]=&HSys.Cyn.C_Auto13_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack3][eTrackCol4]=&HSys.Cyn.C_Auto14_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack3][eTrackCol5]=&HSys.Cyn.C_Auto15_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack4][eTrackCol1]=&HSys.Cyn.C_Auto16_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack4][eTrackCol2]=&HSys.Cyn.C_Auto17_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack4][eTrackCol3]=&HSys.Cyn.C_Auto18_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack4][eTrackCol4]=&HSys.Cyn.C_Auto19_Stop      ;
    LoaderModule->C_Auto_Stop[eTrack4][eTrackCol5]=&HSys.Cyn.C_Auto20_Stop      ;

    LoaderModule->SnEmptyHasTray[eTrack1]       =&HSys.Sen.SnEmpty1HasTray1     ;
    LoaderModule->SnEmptyHasTray[eTrack2]       =&HSys.Sen.SnEmpty1HasTray2     ;
    LoaderModule->SnEmptyHasTray[eTrack3]       =&HSys.Sen.SnEmpty1HasTray3     ;
    LoaderModule->SnEmptyHasTray[eTrack4]       =&HSys.Sen.SnEmpty1HasTray4     ;

    LoaderModule->C_Empty_PushTray[eTrack1]     =&HSys.Cyn.C_Empty1_PushTray1   ;
    LoaderModule->C_Empty_PushTray[eTrack2]     =&HSys.Cyn.C_Empty1_PushTray2   ;
    LoaderModule->C_Empty_PushTray[eTrack3]     =&HSys.Cyn.C_Empty1_PushTray3   ;
    LoaderModule->C_Empty_PushTray[eTrack4]     =&HSys.Cyn.C_Empty1_PushTray4   ;

    LoaderModule->C_Empty_Stop[eTrack1]         =&HSys.Cyn.C_Empty1_Stop1       ;
    LoaderModule->C_Empty_Stop[eTrack2]         =&HSys.Cyn.C_Empty1_Stop2       ;
    LoaderModule->C_Empty_Stop[eTrack3]         =&HSys.Cyn.C_Empty1_Stop3       ;
    LoaderModule->C_Empty_Stop[eTrack4]         =&HSys.Cyn.C_Empty1_Stop4       ;

    LoaderModule->C_AutoTrolly_Stop[eTrack1]    =&HSys.Cyn.C_AutoTrolly1_Stop   ;
    LoaderModule->C_AutoTrolly_Stop[eTrack2]    =&HSys.Cyn.C_AutoTrolly2_Stop   ;
    LoaderModule->C_AutoTrolly_Stop[eTrack3]    =&HSys.Cyn.C_AutoTrolly3_Stop   ;
    LoaderModule->C_AutoTrolly_Stop[eTrack4]    =&HSys.Cyn.C_AutoTrolly4_Stop   ;

    LoaderModule->C_AutoTrolly_Fix[eTrack1]     =&HSys.Cyn.C_AutoTrolly1_Fixer  ;
    LoaderModule->C_AutoTrolly_Fix[eTrack2]     =&HSys.Cyn.C_AutoTrolly2_Fixer  ;
    LoaderModule->C_AutoTrolly_Fix[eTrack3]     =&HSys.Cyn.C_AutoTrolly3_Fixer  ;
    LoaderModule->C_AutoTrolly_Fix[eTrack4]     =&HSys.Cyn.C_AutoTrolly4_Fixer  ;

    LoaderModule->Sen_Auto_HasTray[eTrack1][eTrackCol1]=&HSys.Sen.SnAuto1HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack1][eTrackCol2]=&HSys.Sen.SnAuto2HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack1][eTrackCol3]=&HSys.Sen.SnAuto3HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack1][eTrackCol4]=&HSys.Sen.SnAuto4HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack1][eTrackCol5]=&HSys.Sen.SnAuto5HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack2][eTrackCol1]=&HSys.Sen.SnAuto6HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack2][eTrackCol2]=&HSys.Sen.SnAuto7HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack2][eTrackCol3]=&HSys.Sen.SnAuto8HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack2][eTrackCol4]=&HSys.Sen.SnAuto9HasTray       ;
    LoaderModule->Sen_Auto_HasTray[eTrack2][eTrackCol5]=&HSys.Sen.SnAuto10HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack3][eTrackCol1]=&HSys.Sen.SnAuto11HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack3][eTrackCol2]=&HSys.Sen.SnAuto12HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack3][eTrackCol3]=&HSys.Sen.SnAuto13HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack3][eTrackCol4]=&HSys.Sen.SnAuto14HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack3][eTrackCol5]=&HSys.Sen.SnAuto15HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack4][eTrackCol1]=&HSys.Sen.SnAuto16HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack4][eTrackCol2]=&HSys.Sen.SnAuto17HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack4][eTrackCol3]=&HSys.Sen.SnAuto18HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack4][eTrackCol4]=&HSys.Sen.SnAuto19HasTray      ;
    LoaderModule->Sen_Auto_HasTray[eTrack4][eTrackCol5]=&HSys.Sen.SnAuto20HasTray      ;

    LoaderModule->Sen_Auto_CarHasTray[eTrack1][eTrackCol1]=&HSys.Sen.SnAuto1CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack1][eTrackCol2]=&HSys.Sen.SnAuto2CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack1][eTrackCol3]=&HSys.Sen.SnAuto3CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack1][eTrackCol4]=&HSys.Sen.SnAuto4CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack1][eTrackCol5]=&HSys.Sen.SnAuto5CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack2][eTrackCol1]=&HSys.Sen.SnAuto6CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack2][eTrackCol2]=&HSys.Sen.SnAuto7CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack2][eTrackCol3]=&HSys.Sen.SnAuto8CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack2][eTrackCol4]=&HSys.Sen.SnAuto9CarHasTray    ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack2][eTrackCol5]=&HSys.Sen.SnAuto10CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack3][eTrackCol1]=&HSys.Sen.SnAuto11CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack3][eTrackCol2]=&HSys.Sen.SnAuto12CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack3][eTrackCol3]=&HSys.Sen.SnAuto13CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack3][eTrackCol4]=&HSys.Sen.SnAuto14CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack3][eTrackCol5]=&HSys.Sen.SnAuto15CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack4][eTrackCol1]=&HSys.Sen.SnAuto16CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack4][eTrackCol2]=&HSys.Sen.SnAuto17CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack4][eTrackCol3]=&HSys.Sen.SnAuto18CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack4][eTrackCol4]=&HSys.Sen.SnAuto19CarHasTray   ;
    LoaderModule->Sen_Auto_CarHasTray[eTrack4][eTrackCol5]=&HSys.Sen.SnAuto20CarHasTray   ;

    LoaderModule->C_Auto_Zup[eTrack1][eTrackCol1]       =&HSys.Cyn.C_Auto1_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack1][eTrackCol2]       =&HSys.Cyn.C_Auto2_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack1][eTrackCol3]       =&HSys.Cyn.C_Auto3_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack1][eTrackCol4]       =&HSys.Cyn.C_Auto4_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack1][eTrackCol5]       =&HSys.Cyn.C_Auto5_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack2][eTrackCol1]       =&HSys.Cyn.C_Auto6_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack2][eTrackCol2]       =&HSys.Cyn.C_Auto7_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack2][eTrackCol3]       =&HSys.Cyn.C_Auto8_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack2][eTrackCol4]       =&HSys.Cyn.C_Auto9_Zup          ;
    LoaderModule->C_Auto_Zup[eTrack2][eTrackCol5]       =&HSys.Cyn.C_Auto10_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack3][eTrackCol1]       =&HSys.Cyn.C_Auto11_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack3][eTrackCol2]       =&HSys.Cyn.C_Auto12_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack3][eTrackCol3]       =&HSys.Cyn.C_Auto13_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack3][eTrackCol4]       =&HSys.Cyn.C_Auto14_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack3][eTrackCol5]       =&HSys.Cyn.C_Auto15_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack4][eTrackCol1]       =&HSys.Cyn.C_Auto16_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack4][eTrackCol2]       =&HSys.Cyn.C_Auto17_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack4][eTrackCol3]       =&HSys.Cyn.C_Auto18_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack4][eTrackCol4]       =&HSys.Cyn.C_Auto19_Zup         ;
    LoaderModule->C_Auto_Zup[eTrack4][eTrackCol5]       =&HSys.Cyn.C_Auto20_Zup         ;

    LoaderModule->C_Auto_AntiDrop[eTrack1][eTrackCol1]  =&HSys.Cyn.C_Auto1_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack1][eTrackCol2]  =&HSys.Cyn.C_Auto2_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack1][eTrackCol3]  =&HSys.Cyn.C_Auto3_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack1][eTrackCol4]  =&HSys.Cyn.C_Auto4_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack1][eTrackCol5]  =&HSys.Cyn.C_Auto5_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack2][eTrackCol1]  =&HSys.Cyn.C_Auto6_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack2][eTrackCol2]  =&HSys.Cyn.C_Auto7_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack2][eTrackCol3]  =&HSys.Cyn.C_Auto8_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack2][eTrackCol4]  =&HSys.Cyn.C_Auto9_AntiDrop     ;
    LoaderModule->C_Auto_AntiDrop[eTrack2][eTrackCol5]  =&HSys.Cyn.C_Auto10_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack3][eTrackCol1]  =&HSys.Cyn.C_Auto11_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack3][eTrackCol2]  =&HSys.Cyn.C_Auto12_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack3][eTrackCol3]  =&HSys.Cyn.C_Auto13_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack3][eTrackCol4]  =&HSys.Cyn.C_Auto14_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack3][eTrackCol5]  =&HSys.Cyn.C_Auto15_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack4][eTrackCol1]  =&HSys.Cyn.C_Auto16_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack4][eTrackCol2]  =&HSys.Cyn.C_Auto17_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack4][eTrackCol3]  =&HSys.Cyn.C_Auto18_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack4][eTrackCol4]  =&HSys.Cyn.C_Auto19_AntiDrop    ;
    LoaderModule->C_Auto_AntiDrop[eTrack4][eTrackCol5]  =&HSys.Cyn.C_Auto20_AntiDrop    ;

    LoaderModule->C_Auto_InclinePush[eTrack1][eTrackCol1]=&HSys.Cyn.C_Auto1_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack1][eTrackCol2]=&HSys.Cyn.C_Auto2_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack1][eTrackCol3]=&HSys.Cyn.C_Auto3_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack1][eTrackCol4]=&HSys.Cyn.C_Auto4_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack1][eTrackCol5]=&HSys.Cyn.C_Auto5_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack2][eTrackCol1]=&HSys.Cyn.C_Auto6_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack2][eTrackCol2]=&HSys.Cyn.C_Auto7_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack2][eTrackCol3]=&HSys.Cyn.C_Auto8_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack2][eTrackCol4]=&HSys.Cyn.C_Auto9_InclinePush  ;
    LoaderModule->C_Auto_InclinePush[eTrack2][eTrackCol5]=&HSys.Cyn.C_Auto10_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack3][eTrackCol1]=&HSys.Cyn.C_Auto11_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack3][eTrackCol2]=&HSys.Cyn.C_Auto12_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack3][eTrackCol3]=&HSys.Cyn.C_Auto13_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack3][eTrackCol4]=&HSys.Cyn.C_Auto14_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack3][eTrackCol5]=&HSys.Cyn.C_Auto15_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack4][eTrackCol1]=&HSys.Cyn.C_Auto16_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack4][eTrackCol2]=&HSys.Cyn.C_Auto17_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack4][eTrackCol3]=&HSys.Cyn.C_Auto18_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack4][eTrackCol4]=&HSys.Cyn.C_Auto19_InclinePush ;
    LoaderModule->C_Auto_InclinePush[eTrack4][eTrackCol5]=&HSys.Cyn.C_Auto20_InclinePush ;

    LoaderModule->SnAutoCyAntiDrop1[eTrack1][eTrackCol1]=&HSys.Sen.SnAuto1CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack1][eTrackCol2]=&HSys.Sen.SnAuto2CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack1][eTrackCol3]=&HSys.Sen.SnAuto3CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack1][eTrackCol4]=&HSys.Sen.SnAuto4CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack1][eTrackCol5]=&HSys.Sen.SnAuto5CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack2][eTrackCol1]=&HSys.Sen.SnAuto6CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack2][eTrackCol2]=&HSys.Sen.SnAuto7CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack2][eTrackCol3]=&HSys.Sen.SnAuto8CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack2][eTrackCol4]=&HSys.Sen.SnAuto9CyAntiDrop1  ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack2][eTrackCol5]=&HSys.Sen.SnAuto10CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack3][eTrackCol1]=&HSys.Sen.SnAuto11CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack3][eTrackCol2]=&HSys.Sen.SnAuto12CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack3][eTrackCol3]=&HSys.Sen.SnAuto13CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack3][eTrackCol4]=&HSys.Sen.SnAuto14CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack3][eTrackCol5]=&HSys.Sen.SnAuto15CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack4][eTrackCol1]=&HSys.Sen.SnAuto16CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack4][eTrackCol2]=&HSys.Sen.SnAuto17CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack4][eTrackCol3]=&HSys.Sen.SnAuto18CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack4][eTrackCol4]=&HSys.Sen.SnAuto19CyAntiDrop1 ;
    LoaderModule->SnAutoCyAntiDrop1[eTrack4][eTrackCol5]=&HSys.Sen.SnAuto20CyAntiDrop1 ;

    LoaderModule->SnAutoCyAntiDrop2[eTrack1][eTrackCol1]=&HSys.Sen.SnAuto1CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack1][eTrackCol2]=&HSys.Sen.SnAuto2CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack1][eTrackCol3]=&HSys.Sen.SnAuto3CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack1][eTrackCol4]=&HSys.Sen.SnAuto4CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack1][eTrackCol5]=&HSys.Sen.SnAuto5CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack2][eTrackCol1]=&HSys.Sen.SnAuto6CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack2][eTrackCol2]=&HSys.Sen.SnAuto7CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack2][eTrackCol3]=&HSys.Sen.SnAuto8CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack2][eTrackCol4]=&HSys.Sen.SnAuto9CyAntiDrop2  ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack2][eTrackCol5]=&HSys.Sen.SnAuto10CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack3][eTrackCol1]=&HSys.Sen.SnAuto11CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack3][eTrackCol2]=&HSys.Sen.SnAuto12CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack3][eTrackCol3]=&HSys.Sen.SnAuto13CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack3][eTrackCol4]=&HSys.Sen.SnAuto14CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack3][eTrackCol5]=&HSys.Sen.SnAuto15CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack4][eTrackCol1]=&HSys.Sen.SnAuto16CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack4][eTrackCol2]=&HSys.Sen.SnAuto17CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack4][eTrackCol3]=&HSys.Sen.SnAuto18CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack4][eTrackCol4]=&HSys.Sen.SnAuto19CyAntiDrop2 ;
    LoaderModule->SnAutoCyAntiDrop2[eTrack4][eTrackCol5]=&HSys.Sen.SnAuto20CyAntiDrop2 ;

    LoaderModule->SnAutoCyAntiDrop3[eTrack1][eTrackCol1]=&HSys.Sen.SnAuto1CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack1][eTrackCol2]=&HSys.Sen.SnAuto2CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack1][eTrackCol3]=&HSys.Sen.SnAuto3CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack1][eTrackCol4]=&HSys.Sen.SnAuto4CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack1][eTrackCol5]=&HSys.Sen.SnAuto5CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack2][eTrackCol1]=&HSys.Sen.SnAuto6CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack2][eTrackCol2]=&HSys.Sen.SnAuto7CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack2][eTrackCol3]=&HSys.Sen.SnAuto8CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack2][eTrackCol4]=&HSys.Sen.SnAuto9CyAntiDrop3  ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack2][eTrackCol5]=&HSys.Sen.SnAuto10CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack3][eTrackCol1]=&HSys.Sen.SnAuto11CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack3][eTrackCol2]=&HSys.Sen.SnAuto12CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack3][eTrackCol3]=&HSys.Sen.SnAuto13CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack3][eTrackCol4]=&HSys.Sen.SnAuto14CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack3][eTrackCol5]=&HSys.Sen.SnAuto15CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack4][eTrackCol1]=&HSys.Sen.SnAuto16CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack4][eTrackCol2]=&HSys.Sen.SnAuto17CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack4][eTrackCol3]=&HSys.Sen.SnAuto18CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack4][eTrackCol4]=&HSys.Sen.SnAuto19CyAntiDrop3 ;
    LoaderModule->SnAutoCyAntiDrop3[eTrack4][eTrackCol5]=&HSys.Sen.SnAuto20CyAntiDrop3 ;




    LoaderModule->SnEmptyFCyPushTray[eTrack1]   =&HSys.Sen.SnEmpty1FCyPushTray1 ;
    LoaderModule->SnEmptyFCyPushTray[eTrack2]   =&HSys.Sen.SnEmpty1FCyPushTray2 ;
    LoaderModule->SnEmptyFCyPushTray[eTrack3]   =&HSys.Sen.SnEmpty1FCyPushTray3 ;
    LoaderModule->SnEmptyFCyPushTray[eTrack4]   =&HSys.Sen.SnEmpty1FCyPushTray4 ;

    LoaderModule->SnEmptyRCyPushTray[eTrack1]   =&HSys.Sen.SnEmpty1RCyPushTray1 ;
    LoaderModule->SnEmptyRCyPushTray[eTrack2]   =&HSys.Sen.SnEmpty1RCyPushTray2 ;
    LoaderModule->SnEmptyRCyPushTray[eTrack3]   =&HSys.Sen.SnEmpty1RCyPushTray3 ;
    LoaderModule->SnEmptyRCyPushTray[eTrack4]   =&HSys.Sen.SnEmpty1RCyPushTray4 ;

    LoaderModule->SwEmptyRowCW [eTrack1]        =&HSys.Sw.SwACAutoRow1CW        ;
    LoaderModule->SwEmptyRowCW [eTrack2]        =&HSys.Sw.SwACAutoRow2CW        ;
    LoaderModule->SwEmptyRowCW [eTrack3]        =&HSys.Sw.SwACAutoRow3CW        ;
    LoaderModule->SwEmptyRowCW [eTrack4]        =&HSys.Sw.SwACAutoRow4CW        ;

    LoaderModule->SwEmptyRowCCW[eTrack1]        =&HSys.Sw.SwACAutoRow1CCW       ;
    LoaderModule->SwEmptyRowCCW[eTrack2]        =&HSys.Sw.SwACAutoRow2CCW       ;
    LoaderModule->SwEmptyRowCCW[eTrack3]        =&HSys.Sw.SwACAutoRow3CCW       ;
    LoaderModule->SwEmptyRowCCW[eTrack4]        =&HSys.Sw.SwACAutoRow4CCW       ;

    LoaderModule->MotTrack_Work[eTrack1]        =HSys.VMot.Track_1              ;
    LoaderModule->MotTrack_Work[eTrack2]        =HSys.VMot.Track_2              ;
    LoaderModule->MotTrack_Work[eTrack3]        =HSys.VMot.Track_3              ;
    LoaderModule->MotTrack_Work[eTrack4]        =HSys.VMot.Track_4              ;

    LoaderModule->SnAutoRowHasTray[eTrack1]     =&HSys.Sen.SnAutoRow1HasTray    ;
    LoaderModule->SnAutoRowHasTray[eTrack2]     =&HSys.Sen.SnAutoRow2HasTray    ;
    LoaderModule->SnAutoRowHasTray[eTrack3]     =&HSys.Sen.SnAutoRow3HasTray    ;
    LoaderModule->SnAutoRowHasTray[eTrack4]     =&HSys.Sen.SnAutoRow4HasTray    ;

    LoaderModule->C_AutoRow_Stop[eTrack1]       =&HSys.Cyn.C_AutoRow1_Stop      ;
    LoaderModule->C_AutoRow_Stop[eTrack2]       =&HSys.Cyn.C_AutoRow2_Stop      ;
    LoaderModule->C_AutoRow_Stop[eTrack3]       =&HSys.Cyn.C_AutoRow3_Stop      ;
    LoaderModule->C_AutoRow_Stop[eTrack4]       =&HSys.Cyn.C_AutoRow4_Stop      ;

    LoaderModule->SwAutoRowDete[eTrack1]        =&HSys.Sw.SwAutoRow1Dete        ;
    LoaderModule->SwAutoRowDete[eTrack2]        =&HSys.Sw.SwAutoRow2Dete        ;
    LoaderModule->SwAutoRowDete[eTrack3]        =&HSys.Sw.SwAutoRow3Dete        ;
    LoaderModule->SwAutoRowDete[eTrack4]        =&HSys.Sw.SwAutoRow4Dete        ;

    LoaderModule->C_AutoRow_PushTray[eTrack1]   =&HSys.Cyn.C_AutoRow1_PushTray  ;
    LoaderModule->C_AutoRow_PushTray[eTrack2]   =&HSys.Cyn.C_AutoRow2_PushTray  ;
    LoaderModule->C_AutoRow_PushTray[eTrack3]   =&HSys.Cyn.C_AutoRow3_PushTray  ;
    LoaderModule->C_AutoRow_PushTray[eTrack4]   =&HSys.Cyn.C_AutoRow4_PushTray  ;

    LoaderModule->SwLoaderCCW             =&HSys.Sw.SwACLoaderCCW         ;
    LoaderModule->SwLoaderCW              =&HSys.Sw.SwACLoaderCW          ;
    LoaderModule->SwLoaderCWDete          =&HSys.Sw.SwLoaderCWDete        ;

    LoaderModule->MotLoader_Car           =HSys.VMot.LoadTray_Car           ;
    LoaderModule->MotWorkLoader           =HSys.VMot.MotWorkLoader          ;
    LoaderModule->MotNowSortTray          =HSys.VMot.NowSortTray            ;   //KenHsieh 20240514 : 補上Loader資料轉換時，也清NowTray

    LoaderModule->MotEmpty1_Car           =HSys.VMot.Empty_1                ;

    LoaderModule->SwEmptyCCW              =&HSys.Sw.SwACEmpty1CCW           ;
    LoaderModule->SwEmptyCW               =&HSys.Sw.SwACEmpty1CW            ;

    LoaderModule->MotMagLD_Work           =HSys.VMot.Mag_LD_Work_Y          ;
    LoaderModule->MotEmpty2_Car           =HSys.VMot.Mag_LD_Car             ;
    LoaderModule->MotEmpty2_Tray          =HSys.VMot.MagEmptyTray           ;

    LoaderModule->SnMagPreDete            =&HSys.Sen.SnEmpty2CCWDete     ;
    LoaderModule->SnMagSureTray           =&HSys.Sen.SnEmpty2HasTray     ;
    LoaderModule->SnMag_EdgePush          =&HSys.Sen.SnEmpty2EdgePush    ;      //KenHsieh 20240528 : 補上汽缸到位保護
    LoaderModule->SnMag_FixCyPush         =&HSys.Sen.SnEmpty2FixCyPush   ;      //KenHsieh 20240528 : 補上汽缸到位保護

    LoaderModule->SwMagCCW                =&HSys.Sw.SwACEmpty2CCW           ;
    LoaderModule->SwMagCW                 =&HSys.Sw.SwACEmpty2CW            ;
    LoaderModule->SwEmpty2CCWDete         =&HSys.Sw.SwEmpty2CCWDete         ;

    LoaderModule->MotTransferX            =HSys.Mot.TrayShiftStageX       ;
    LoaderModule->MotTransferX_Data       =HSys.VMot.Transfer_LoaderY     ;

    LoaderModule->SnShiftStageEdgePush    =&HSys.Sen.SnShiftStageEdgePush ;
    LoaderModule->SnShiftStageCWPreDete   =&HSys.Sen.SnShiftStageCWPreDete;
    LoaderModule->SnShiftStageHasTray     =&HSys.Sen.SnShiftStageHasTray  ;

    LoaderModule->C_ShiftStageEdgePush    =&HSys.Cyn.C_ShiftStageEdgePush ;

    LoaderModule->SwTransferCCW           =&HSys.Sw.SwACShiftStageCCW     ;
    LoaderModule->SwTransferCW            =&HSys.Sw.SwACShiftStageCW      ;
    LoaderModule->C_Empty2_EmptyStop      =&HSys.Cyn.C_Empty2_EmptyStop   ;

    LoaderModule->C_Mag_CCD_Stop          =&HSys.Cyn.C_Empty2_CCDStop;
    LoaderModule->SnEmpty2HasEmptyTray    =&HSys.Sen.SnEmpty2HasEmptyTray;
}
//------------------------------------------------------------------------------
void UpdateLoaderParameter()
{
    LoaderModule->dLoadArrival    =atof(fSetup->edLoadTrayArrival->Text.c_str());
    LoaderModule->dLoadLock       =atof(fSetup->edLoadLock->Text.c_str());
    LoaderModule->dLoadMidd       =atof(fSetup->edLoadMiddle->Text.c_str());
    LoaderModule->dLoadDown       =atof(fSetup->edLoadDown->Text.c_str());
    LoaderModule->dUnloadArrival  =atof(fSetup->edUnloadArrival->Text.c_str());
    LoaderModule->dUnloadDown     =atof(fSetup->edUnloadUp->Text.c_str());

    LoaderModule->iXStart         =atof(fSetup->edXStart->Text.c_str())*100.0;
    LoaderModule->iXPitch         =atof(fSetup->edXPitch->Text.c_str())*100.0;
    LoaderModule->iYStart         =atof(fSetup->edYStart->Text.c_str())*100.0;
    LoaderModule->iYPitch         =atof(fSetup->edYPitch->Text.c_str())*100.0;
    LoaderModule->iXDivision      =atoi(fSetup->edXDivision->Text.c_str());
    LoaderModule->iYDivision      =atoi(fSetup->edYDivision->Text.c_str());

    LoaderModule->iTransferX_LD    =atoi(fTeach->edTrayShiftStageX_Loader->Text.c_str());
    LoaderModule->iTransferX_Empty =atoi(fTeach->edTrayShiftStageX_Empty->Text.c_str());

    LoaderModule->MotWorkLoader->SetTrayXYItem(LoaderModule->iXDivision, LoaderModule->iYDivision);
    LoaderModule->MotMagLD_Work->SetTrayXYItem(LoaderModule->iXDivision, LoaderModule->iYDivision);

    TEdit *AutoX[4]={fTeach->edAutoTrolly1X_Auto1, fTeach->edAutoTrolly2X_Auto6, fTeach->edAutoTrolly3X_Auto11, fTeach->edAutoTrolly4X_Auto16};
    TEdit *AutoZ[4]={fTeach->edAutoTrolly1Z_Auto1, fTeach->edAutoTrolly2Z_Auto6, fTeach->edAutoTrolly3Z_Auto11, fTeach->edAutoTrolly4Z_Auto16};
    TEdit *AutoZDown[4]={fTeach->edAutoTrolly1Z_Down, fTeach->edAutoTrolly2Z_Down, fTeach->edAutoTrolly3Z_Down, fTeach->edAutoTrolly4Z_Down};
    TEdit *AutoX_Push[4]={fTeach->edAutoTrolly1X_Track1, fTeach->edAutoTrolly2X_Track2, fTeach->edAutoTrolly3X_Track3, fTeach->edAutoTrolly4X_Track4};

    for(int iR=eTrack1; iR<=eTrack4; iR++)
    {
        LoaderModule->iPos_Auto_X[iR][0]    =atoi(AutoX[iR-1]->Text.c_str());
        LoaderModule->iPos_Auto_Z[iR][0]    =atoi(AutoZ[iR-1]->Text.c_str());
        LoaderModule->iPos_Auto_ZDown[iR][0]=atoi(AutoZDown[iR-1]->Text.c_str());
        LoaderModule->iPos_AutoXPush[iR]    =atoi(AutoX_Push[iR-1]->Text.c_str());

        for(int iC=eTrackCol2; iC<eTrackColTotal; iC++)
        {
            LoaderModule->iPos_Auto_X[iR][iC]       =LoaderModule->iPos_Auto_X[iR][0]+((180.0*100.0)*iC);  //Auto pitch 180mm
            LoaderModule->iPos_Auto_Z[iR][iC]       =LoaderModule->iPos_Auto_Z[iR][0];
            LoaderModule->iPos_Auto_ZDown[iR][iC]   =LoaderModule->iPos_Auto_ZDown[iR][0];
        }
    }
    LoaderModule->iAutoZUpDelay                     =atoi(fSetup->edAutoZUpDelay->Text.c_str());
    for(int i=eTrack1; i<=eTrack4; i++)
    {
        for(int j=eTrackCol1; j<eTrackColTotal; j++)
        {
            if(Auto->Mot_Auto_Work[i][j]!=NULL)
            {
                Auto->Mot_Auto_Work[i][j]->SetTrayXYItem(LoaderModule->iXDivision, LoaderModule->iYDivision);
            }
        }
    }

    if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eEKeeperFormat)
    {
        LoaderModule->SummaryFileName=LoaderModule->eKeeperSchedule();
    }
    else if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eJHTFormat ||
            HSys.FuncT.iA01_InputDataFormat==LoaderModule->ePTIFormat)
    {
        LoaderModule->SummaryFileName="LOTID123@Insertion123@FlowID123";
    }
    else if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eWinstekFormat)
    {
        LoaderModule->SummaryFileName=LoaderModule->eKeeperSchedule();
    }
}
//----------------------------------------------------------------------------
TLoaderModule::TLoaderModule()
{
    sLoaderError="";
    iStatus=eHS_Busy;

    iHS_StackAreaStatus=0;
    iHS_StackAreaStatusEmpty2=0;
    InitialFlag();
}
//----------------------------------------------------------------------------
TLoaderModule::~TLoaderModule()
{
}
//----------------------------------------------------------------------------
void TLoaderModule::InitialFlag()
{
    iMagArmStatus=0;
    bSupportingEmpty1TrayToCar=false;
    bSupportingTray=false;
    iAutoZUpDelay=1;
}
//---------------------------------------------------------------------------
bool TLoaderModule::LoadHasNoTrayNeedShowAlarm()
{
    bool bResult=false;

    return bResult;
}
//----------------------------------------------------------------------------
void TLoaderModule::DoLoader(int &Task)
{
    bool bFlag;
    int iLoader=eLoader1;
    int ret;
    AnsiString asErrMsgStr;
    switch(Task)
    {
        case 1:
            if(HSys.Sys.RunMode==Run_OneCycle)
            {

            }
            else if(MotWorkLoader->fHasTray &&                                  //工作區要退盤
                    MotTransferX_Data->fHasTray==false &&
                    bNeedLoadTrayToTransfer==true)
            {
                Task=500;
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed)                             //Tray Feed就是不再進盤
            {
                ;
            }
            else if(MotLoader_Car->fHasTray==true &&                            //台車上有盤
                    MotWorkLoader->fHasTray==false)                             //工作區沒盤
            {
                Task=300;                                                       //進入工作區域
            }
            #ifndef SOFT_SIMULATE
            else if(((HSys.LastSet.iRealDummy==DUMMY &&
                      fMain->chkLoadTray->Checked==false) ||
                     SnSelectHasTray[iLoader]->IsOff()==false) &&               //Load上有盤
                    MotLoader_Car->fHasTray==false)                             //台車上沒盤
            {
                fMain->ledLoad->Value=true;
                Task=100;                                                       //新的一盤
            }
            #else
            else if(MotLoader_Car->fHasTray==false &&
                    HSys.LastSet.iRealDummy==DUMMY &&
                    fMain->chkLoadTray->Checked)
            {
                fMain->ledLoad->Value=true;
                Task=100;                                                       //新的一盤
            }
            #endif
            else if(((HSys.LastSet.iRealDummy==DUMMY &&
                      fMain->chkLoadTray->Checked==false) ||
                     SnSelectHasTray[iLoader]->IsOff()==true) &&
                    MotLoader_Car->fHasTray==false  &&
                    MotWorkLoader->fHasTray==false &&
                    bNeedLoadTrayToTransfer==false)
            {
                if(HSys.Sys.RunMode==Run_CleanOut)
                {
                    bFlagLoaderCleanoutFinish=true;
                }
                else
                {
                    ret=ShowSystemError(SnSelectHasTray[iLoader]->OffAlarmCode, K_RETRY|K_CLEAN_OUT);
                    if(ret==K_CLEAN_OUT)
                    {
                        ChangeRunMode(Run_CleanOut);
                        bCleanOut=true;                                         //KenHsieh 20250820 : 補上flag，在cleanout下執行onecycle完成後，可自動變為cleanout
                    }
                }
            }
            break;
        case 100:
            DoLoadNewTrayToLoaderCar(true);
            Task=200;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 200:
            bFlag=DoLoadNewTrayToLoaderCar();
            if(bFlag)
            {
                Task=1;
            }
            break;
        case 300:
            DoLoaderTrayToWorkArea(true);
            Task=400;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 400:
            bFlag=DoLoaderTrayToWorkArea();
            if(bFlag)
            {
                ArmMotionDone=true;
                Task=1;
            }
            break;
        case 500:
            DoLoaderTrayToTansfer(true);
            Task=600;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 600:
            bFlag=DoLoaderTrayToTansfer();
            if(bFlag)
            {
                bNeedLoadTrayToTransfer=false;
                ArmMotionDone=true;
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
TQPF_Timer tLoaderDelay;
bool TLoaderModule::DoLoaderTrayToTansfer(bool bReset)
{
    AnsiString Str;
    bool bResult=false;
    static bool bXMove=false, bCyCheck[3]={false, false, false};
    int &Task=iWorkAreaToTransferTask;
    int iLoader=eLoader1;
    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(MotWorkLoader->fHasTray && MotTransferX_Data->fHasTray==false && bNeedLoadTrayToTransfer==true)
            {
                SetStatus(eHS_Busy, __FUNC__);
                C_ShiftStageEdgePush->Reset();
                Task=100;
            }
            break;
        case 100:
            if(C_ShiftStageEdgePush->Pop())
            {
                C_Stop[iLoader]->Reset();
                C_EdgePush[iLoader]->Reset();
                C_Fixer[iLoader]->Reset();
                bXMove=false;
                Task=200;
            }
            break;
        case 200:
            if(bXMove==false)
                bXMove=MotTransferX->MotorMove(iTransferX_LD);

            if(bXMove==true)
            {
                bCyCheck[0]=false;
                bCyCheck[1]=false;
                bCyCheck[2]=false;
                Task=300;
                LoaderCylinderOffTimeOut.SetSecAndOn(5.0);      //Sam 20240717 : 新增 Alarm Code
            }
            break;
        case 300:
            if(bCyCheck[0]==false)
                bCyCheck[0]=C_Stop[iLoader]->Pop();
            if(bCyCheck[1]==false)
                bCyCheck[1]=(C_EdgePush[iLoader]->Pop());
            if(bCyCheck[2]==false)
                bCyCheck[2]=C_Fixer[iLoader]->Pop();

            if(bCyCheck[0] && bCyCheck[1] && bCyCheck[2])
            {
                Task=400;
            }
            else if(LoaderCylinderOffTimeOut.Off())     //Sam 20240717 : 新增 Alarm Code
            {
                ShowSystemError("80102", K_RETRY);
                LoaderCylinderOffTimeOut.SetSecAndOn(5.0);
                Task=200;
            }
            break;
        case 400:
            SwTransferCW->On();
            SwLoaderCWDete->Off();
            SwLoaderCW->On();
            LoaderMoveTimeOut.SetSecAndOn(10.0);     //Sam 20240730 : 新增 Alarm Code
            Task=401;
//            break;
        case 401:
            if(SwLoaderCW->Status()==false || SwTransferCW->Status()==false)     //Sam 20240730 : 新增 Alarm Code
                LoaderMoveTimeOut.SetSecAndOn(10.0);

            SwTransferCW->On();
            SwLoaderCW->On();
            if((SnShiftStageCWPreDete->IsOff()==false &&
                SnShiftStageHasTray->IsOff()==false) ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                Task=500;
                tLoaderDelay.SetSecAndOn(1);                                    //JerryYang 20240607 : delay一下, 避免Tray還沒到位
            }
            else if(LoaderMoveTimeOut.Off())                                    //Sam 20240730 : 新增 Alarm Code
            {
                ShowSystemError("80103", K_RETRY);
                Task=400;
            }
            break;
        case 500:
            SwTransferCW->On();
            SwLoaderCW->On();
            if(tLoaderDelay.Off())                                              //JerryYang 20240607 : delay一下, 避免Tray還沒到位
            {
                MotTransferX_Data->MoveTrayFrom(MotWorkLoader);                 //Steven 20240520 : CopyTrayFrom --> MoveTrayFrom
                MotNowSortTray->ClearTray();                                    //KenHsieh 20240618 : 補上Magazine Now
                SwTransferCW->Off();
                SwLoaderCW->Off();
                bResult=true;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
void TLoaderModule::DoSupportAuto(int &Task)
{
    bool bFlag;
    static int iX=0, iY=0;
    AnsiString asErrMsgStr;
    switch(Task)
    {
        case 1:
            if(HSys.Sys.RunMode==Run_TrayFeed)
            {
                if(bEmpty1TrayFeedFinish==true)
                {
                }
                else
                {
                    if(CosFunction.bClearEmpty1TrayAtTrayFeed)
                    {
                        if(GetEmpty1HasTray())
                        {
                            DoEmptyTrayToStoreArea(true);
                            Task=400;
                        }
                        else
                        {
                            bEmpty1TrayFeedFinish=true;
                        }
                    }
                    else
                    {
                        bEmpty1TrayFeedFinish=true;
                    }
                }
            }
            else if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal)
            {
                if(bFlagAutoArmCleanoutFinish==true)
                {
                    bSupportingTray=false;                                          //Empty 1正在入盤
                    if(CheckAutoHasTray()!=eBinNotUse)
                        Task=300;
                    else
                        bFlagAutoBinCleanoutFinish=true;
                }
                else if(CheckFullTray(iX, iY))                                      //Sam 20240527 : 滿盤優先做出料動作
                {
                    bSupportingTray=false;                                          //Empty 1正在入盤
                    Task=300;
                }
                else if(CheckFullTray(iX, iY)==false)
                {
                    if((HSys.FuncT.iT09_TrayIDReadPos==0 && MotEmpty1_Car->fHasTray==true) ||
                       (HSys.FuncT.iT09_TrayIDReadPos==1 && MotTrack_Work[eTrack1]->fHasTray==true))
                    {
                        Task=100;
                    }
                }
            }
            break;
        case 100:
            DoLoadEmptyTrayToTrackArea(true);
            Task=200;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 200:
            if(bSupportingEmpty1TrayToCar==true)
            {
                break;//wait
            }
            bSupportingTray=true;                                               //Empty 1正在入盤
            if((HSys.FuncT.iT09_TrayIDReadPos==0 && MotEmpty1_Car->fHasTray==false) ||
               (HSys.FuncT.iT09_TrayIDReadPos==1 && MotTrack_Work[eTrack1]->fHasTray==false))
            {
                bSupportingTray=false;                                          //Empty 1正在入盤
                ArmMotionDone=true;
                Task=1;
            }
            bFlag=DoLoadEmptyTrayToTrackArea();
            if(bFlag)
            {
                bSupportingTray=false;                                          //Empty 1正在入盤
                ArmMotionDone=true;
                Task=1;
            }
            break;
        case 300:
            bFlag=(bTrackUnloadingTray[eTrack1]==false &&
                   bTrackUnloadingTray[eTrack2]==false &&
                   bTrackUnloadingTray[eTrack3]==false &&
                   bTrackUnloadingTray[eTrack4]==false &&
                   Auto->TrackHasTray()==eTrackNotUse);
            if(bFlag)
            {
                ArmMotionDone=true;
                Task=1;
            }
            break;
        case 400:
            if(DoEmptyTrayToStoreArea(false))
            {
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
void TLoaderModule::DoEmpty1(int &Task)
{
    bool bFlag;
    AnsiString asErrMsgStr;
    switch(Task)
    {
        case 1:
            if(HSys.Sys.RunMode==Run_TrayFeed)
            {
            }
            else if((HSys.Sys.RunMode!=Run_CleanOut || bFlagAutoArmCleanoutFinish==false) && flagTrayEnd==false)
            {
                if(HSys.FuncT.iT09_TrayIDReadPos==0)
                {
                    if(MotEmpty1_Car->fHasTray==false)
                    {
                        Task=100;
                    }
                }
                else
                {
                    if(MotTrack_Work[eTrack1]->fHasTray==false)
                    {
                        Task=100;
                    }
                }
            }
            break;
        case 100:
            if(bSupportingTray==true)
            {
                break;//wait
            }
            bSupportingEmpty1TrayToCar=true;
            DoLoadNewTrayToEmpty1Car(true);
            Task=200;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 200:
            bFlag=DoLoadNewTrayToEmpty1Car();
            if(bFlag)
            {
                bSupportingEmpty1TrayToCar=false;
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
void TLoaderModule::DoEmpty2(int &Task)
{
    bool bFlag;
    AnsiString asErrMsgStr;
    switch(Task)
    {
        case 1:
            if(MagArmPara->iEmptyMagStatus==MagArmPara->eHS_EM_NeedTray)
            {
                if(MotEmpty2_Car->fHasTray==false)
                {
                    Task=500;
                }
                else
                {
                    DoLoadEmpty2Mag(true);
                    Task=2000;
                }
            }
            else if(MagArmPara->iEmptyMagStatus==MagArmPara->eHS_EM_WorkTrayOut)  //退盤
            {
                Task=700;
            }
                                                        //載台                               //Sort 區
            else if(MotTransferX_Data->fHasTray==true && MotTransferX_Data->HasIC()==false && MotMagLD_Work->fHasTray==false && bIsInitLoadEmptyTrayToMag==false)  //JerryYang 20240623 : Initial Magazine自動補Tray
            {
                Task=100;
            }
            else if(MotTransferX_Data->fHasTray==true && MotTransferX_Data->HasIC()==true && MotMagLD_Work->fHasTray==false && bIsInitLoadEmptyTrayToMag==false)  //JerryYang 20240623 : Initial Magazine自動補Tray
            {
                Task=300;
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed)
            {
                if(MotMagLD_Work->fHasTray==false && MotEmpty2_Car->fHasTray==true)
                    Task=1000;
            }
            else if(MotEmpty2_Car->fHasTray==false)
            {
                if(CheckEmpty2NeedTray())
                {
                    Task=500;
                }
            }
            break;
        case 100:
            DoTransferToMagUnloader(true);
            Task=200;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 200:
            bFlag=DoTransferToMagUnloader();
            if(bFlag)
            {
                Task=1;
                if(CheckEmpty2NeedTray()==false)
                {
                    Task=1000;
                }
            }
            break;
        case 300:
            DoTransferToMagLD(true);
            Task=400;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 400:
            bFlag=DoTransferToMagLD();
            if(bFlag)
            {
                Task=1;
            }
            break;
        case 500:
            DoLoadNewTrayToEmpty2Car(true);
            Task=600;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 600:
            bFlag=DoLoadNewTrayToEmpty2Car();
            if(bFlag)
            {
                Task=1;
            }
            break;
        case 700:
            DoWorkAreaToMagUnloader(true, true);
            Task=800;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 800:
            bFlag=DoWorkAreaToMagUnloader(true);
            if(bFlag)
            {
                iMagArmStatus=eHS_MA_WorkTrayOutReady;
                if(MagArmPara->iEmptyMagStatus==MagArmPara->eHS_EM_Idle)
                {
                    iMagArmStatus=eHS_MA_Idle;
                    if(MotEmpty2_Tray->fHasTray==false && MotMagLD_Work->fHasTray==false)
                    {
                        Task=1;
                    }
                    else
                    {
                        Task=1000;
                    }
                }
            }
            break;
        case 1000:
            DoWorkAreaToMagUnloader(false, true);
            Task=1100;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 1100:
            bFlag=DoWorkAreaToMagUnloader(false);
            if(bFlag)
            {
                Task=1;
            }
            break;
        case 2000:
            if(DoLoadEmpty2Mag(false))
            {
                iMagArmStatus=eHS_MA_EmptyTrayReady;
                if(MagArmPara->iEmptyMagStatus==MagArmPara->eHS_EM_Idle)
                {
                    iMagArmStatus=eHS_MA_Idle;
                    Task=1;
                }
            }
            break;
    }
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoLoadNewTrayToLoaderCar(bool bReset)
{
    bool bFlag;
    AnsiString Str;
    AnsiString StrMsg;
    bool bResult=false;
    int &Task=iLoadNewTrayToLoaderCarTask;
    static int iRetry=0, iFlag=0;
    int ret=0;
    int iLoader=eLoader1;

    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(HSys.LastSet.iRealDummy==DUMMY)
                fMain->ledLoad->Value=fMain->chkLoadTray->Checked;
            else
                fMain->ledLoad->Value=SnSelectHasTray[iLoader]->IsOn();

            if(HSys.LastSet.iRealDummy==DUMMY)
            {
                if(fMain->chkLoadTray->Checked)
                {
                    AutoCylinderUp(true, iLoader);
                    Task=50;
                }
                else if(HSys.Sys.RunMode!=Run_CleanOut)
                {
                    ret=ShowSystemError(SnSelectHasTray[iLoader]->OffAlarmCode, K_RETRY|K_CLEAN_OUT);
                    if(ret==K_CLEAN_OUT)
                    {
                        Task=9999;
                        ChangeRunMode(Run_CleanOut);
                        bCleanOut=true;                                         //KenHsieh 20250820 : 補上flag，在cleanout下執行onecycle完成後，可自動變為cleanout
                    }
                }
                else
                {
                    Task=9999;    //JerryYang 20240626 : fix hang up, clean out Loader no tray
                }
            }
            else if(Sn_CarHasTray[iLoader]->IsOff()==true)
            {
                if(SnSelectHasTray[iLoader]->IsOn()==true)
                {
                    AutoCylinderUp(true, iLoader);
                    Task=50;
                }
                else
                {
                    if(HSys.Sys.RunMode!=Run_CleanOut)
                    {
                        ret=ShowSystemError(SnSelectHasTray[iLoader]->OffAlarmCode, K_RETRY|K_CLEAN_OUT);
                        if(ret==K_CLEAN_OUT)
                        {
                            Task=9999;
                            ChangeRunMode(Run_CleanOut);
                            bCleanOut=true;                                     //KenHsieh 20250820 : 補上flag，在cleanout下執行onecycle完成後，可自動變為cleanout
                        }
                    }
                    else
                    {
                        Task=9999;                                              //JerryYang 20240626 : fix hang up, clean out Loader no tray
                    }
                }
            }
            else if(C_Middle[iLoader]->IsOn())                                  //汽缸在上面
            {
                if(Sn_CarHasTray[iLoader]->IsOff())
                {
                    C_Up[iLoader]->Off();
                    C_Middle[iLoader]->Off();
                    break;
                }
            }
            else if(Sn_CarHasTray[iLoader]->IsOn() || Sn_CCDPos[iLoader]->IsOn())
            {
                ShowSystemError(Sn_CarHasTray[iLoader]->OnAlarmCode, K_RETRY);
            }
            break;
        case 50:                                                                //jimmychiu 20251009 : 入料前確認Loader滿盤Sensor
            if(Sn_CarIsFull[eLoader1]->IsOn())
            {
                ShowMyMessage("The trays of Loader are Full!");
            }
            else
            {
                Task=60;
            }
            break;
        case 60:
            bFlag=AutoCylinderUp(false, iLoader);
            if(bFlag)
            {
                C_Separatory[iLoader]->Reset();
                TrayToCarDelay[iLoader].SetSecAndOn(1);                         //Steven 20130904 : 加入Time Out 避免Hang Up
                Task=100;
            }
            break;
        case 100:
            if(TrayToCarDelay[iLoader].Off())
            {
                C_Separatory[iLoader]->Push();
                if(C_Separatory[iLoader]->IsOff()==false)
                    Task=200;
                AutoCylinderMiddle(true, iLoader);
            }
            break;
        case 200:
            bFlag=AutoCylinderMiddle(false, iLoader);
            if(bFlag)
            {
                C_Separatory[iLoader]->Reset();
                Task=300;
            }
            break;
        case 300:
            if(C_Separatory[iLoader]->Pop())
            {
                Task=400;
            }
            TrayToCarDelay[iLoader].SetSecAndOn(3);
            break;
        case 400:
            if(TrayToCarDelay[iLoader].Off())
            {
                AutoCylinderLower(true, iLoader);
                Task=410;
            }
            break;
        case 410:
            bFlag=AutoCylinderLower(false, iLoader);
            if(bFlag)
            {
                TrayToCarDelay[iLoader].SetSecAndOn(1);
                C_CCD_Stop[iLoader]->Reset();
                Task=420;
            }
            break;
        case 420:
            if(Sn_CarHasTray[iLoader]->IsOn() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                fMain->ledLoad->Value=false;
                MotLoader_Car->SetTrayXYItem(iXDivision, iYDivision);
                MotLoader_Car->GetNewNullTray();

                C_CCD_Stop[iLoader]->On();
                C_CCD_Stop[iLoader]->Reset();
                Task=450;
            }
            else if(TrayToCarDelay[iLoader].Off())
            {
                ret=ShowSystemError(SnSelectHasTray[iLoader]->OffAlarmCode, K_RETRY|K_SKIP|K_CLEAN_OUT);
                if(ret==K_SKIP)
                {
                    Task=9999;
                }
                else if(ret==K_CLEAN_OUT)
                {
                    Task=9999;
                    ChangeRunMode(Run_CleanOut);
                    bCleanOut=true;                                             //KenHsieh 20250820 : 補上flag，在cleanout下執行onecycle完成後，可自動變為cleanout
                }
            }
            break;
        case 450:                                                               //Read Tray ID
            if(C_CCD_Stop[iLoader]->Push())
            {
                SwLoaderCWDete->Off();                                          //JerryYang 20240530 : add
                SwLoaderCW->On();
                sLoaderID="";
                iRetry=0;
                LoaderMoveTimeOut.SetSecAndOn(5.0);                             //Sam 20240730 : 新增 Alarm Code
                Task=500;
            }
            break;
        case 500:
            SwLoaderCW->On();
            if(HSys.FuncT.iT09_TrayIDReadPos==1)
                LoaderMoveTimeOut.SetSecAndOn(10.0);
            else
                LoaderMoveTimeOut.SetSecAndOn(3.0);
            Task=510;
            break;
        case 510:
            if(SwLoaderCW->Status()==false)                                     //KenHsieh 20250821 : 暫停後須重新計數
            {
                if(HSys.FuncT.iT09_TrayIDReadPos==1)
                    LoaderMoveTimeOut.SetSecAndOn(10.0);
                else
                    LoaderMoveTimeOut.SetSecAndOn(3.0);
            }
            SwLoaderCW->On();
            if(Sn_CCDPos[iLoader]->IsOn() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                if(frm2DID->MyCCD[iLoader]->bEnableCCD==false)
                {
                    Task=610;                                                   //JerryYang 20240607 : modify
                }
                else
                {
                    Task=600;
                }
            }
            else if(LoaderMoveTimeOut.Off())                                    //Sam 20240730 : 新增 Alarm Code
            {
                ret=ShowSystemError(Sn_CCDPos[iLoader]->OffAlarmCode, K_RETRY|K_SKIP);
                LoaderMoveTimeOut.SetSecAndOn(3.0);
                if(ret==K_SKIP)
                {
                    iFlag=0;
                    MotLoader_Car->ClearTray();
                    Task=1;
                }
            }
            break;
        case 600:
            sLoaderID="";
            frm2DID->StartCCDAuto(iLoader);
            Task=605; 
            break;
        case 605:
            if(frm2DID->GetBarcode(iLoader)!="")
            {
                sLoaderID=frm2DID->GetBarcode(iLoader);
                Task=610;
            }
            else if(frm2DID->UpdateCCDAuto(iLoader))
            {
                if(frm2DID->MyCCD[iLoader]->bEnableCCD==true)                   //JerryYang 20240607 : modify
                {
                    sLoaderID=frm2DID->GetBarcode(iLoader);
                }
                Task=610;
            }
            break;
        case 610:
            if(tSimuData.bRunSimulation)
            {
                sLoaderID=tSimuData.sClipID;
            }
            else if(frm2DID->MyCCD[iLoader]->bEnableCCD==false || HSys.LastSet.iRealDummy==DUMMY)                      //Sam 20240515 : 修正 Dummy Run        //JerryYang 20250716 : has tray要可以讀ID
            {
                sLoaderID=fMain->edtTrayID->Text;
            }
            else if(fMain->chkLoadMatrix->Checked)
            {
                sLoaderID=fMain->edtTrayID->Text;
            }

            if(sLoaderID!="")
            {
                if(CheckClipIDCode(sLoaderID)==false)	                        //Sam 20250905 : 新增ClipID編碼檢查
                {
                    StrMsg.sprintf("ClipID=%s", sLoaderID);
                    ShowSystemError("80007", K_RETRY, 0, StrMsg);
                    iRetry=0;
                    Task=500;
                    break;
                }

                iFlag=LoadClipIDMapping(sLoaderID);
                EventReport(SECS_EVENT.LoaderCar_ReadTrayID);                   //JerryYang 20240905 : add
                if(iFlag==SUMMARY_SUCCESS)
                {
                    #ifndef SOFT_SIMULATE
                    iFlag=CheckDuplicateLotID(sCheckDuplicateLotIDLoaderPath(),sLoaderID);                       //KenHsieh 20250806 : 修改重複碼報警
                    #endif

                    if(frm2DID->MyCCD[iLoader]->bEnableCCD==false ||
                       iFlag==SUMMARY_SUCCESS)                                  //JerryYang 20250804 : 沒開CCD就不用檢查重複碼
                    {
                        iFlag=0;
                        Task=1;
                        bResult=true;
                    }
                    else
                    {
                        Task=1000;
                    }
                }
                else
                {
                    iRetry++;
                    if(iRetry>3)
                    {
                        Task=1000;
                    }
                    else
                    {
                        Task=500;
                    }
                }
            }
            else if(TrayToCarDelay[iLoader].Off())
            {
                iRetry++;
                if(iRetry>3)
                {
                    Task=1000;
                    iFlag=TRAY_ID_IS_NULL;
                }
                else
                {
                    Task=500;
                }
            }
            SwLoaderCW->Off();
            break;
        case 1000:
            if(iFlag==SUMMARY_FILE_NOT_EXIST)
            {
                StrMsg="SUMMARY_FILE_NOT_EXIST";
                sLoaderError="80002";                                           //("沒檔案，有問題啊");
            }
            else if(iFlag==TRAY_MAP_FILE_IS_NULL)
            {
                StrMsg="TRAY_MAP_FILE_IS_NULL";
                sLoaderError="80002";                                           //("有檔案沒資料，有問題啊");
            }
            else if(iFlag==TRAY_ID_NOT_EXIST)
            {
                StrMsg="TRAY_ID_NOT_EXIST";
                sLoaderError="80004";                                           //("資料是空的，有問題啊");
            }
            else if(iFlag==TRAY_ID_IS_NULL)
            {
                StrMsg="TRAY_ID_IS_NULL";
                sLoaderError="80001";                                           //("Loader CCD 讀取異常");
            }
            else if(iFlag==TRAY_ID_DUPLICATE)                                   //Sam 20250728 : Loader 讀取到重複 Tray ID
            {
                StrMsg="TRAY_ID_DUPLICATE";
                sLoaderError="80005";                                           //("重複的Tray ID");
            }
            iFlag=0;
            ret=ShowSystemError(sLoaderError, K_RETRY|K_SKIP, 0, StrMsg + " : " +sLoaderID);    //Sam 20240629 : 新增 Alarm 時顯示 Tray ID
            if(ret==K_RETRY)
            {
                iRetry=0;
                Task=500;
            }
            else                                                                //K_SKIP
            {
                iFlag=0;
                MotLoader_Car->ClearTray();
                Task=1010;
            }
            break;
        case 1010:
            if(Sn_CCDPos[iLoader]->IsOn())                                      //JerryYang 20240625 : TRAY盤要拿掉
            {
                ShowMyMessage("Please remove the tray at Loader CCD Position");
            }
            else
            {
                Task=1;
            }
            break;
        case 9999:                                                              //Idle Arm
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoLoadNewTrayToEmpty1Car(bool bReset)
{
    bool bFlag;
    int ret;
    AnsiString Str;
    bool bResult=false;
    int &Task=iLoadNewTrayToEmpty1CarTask;
    int iEmpty1=eEmpty1;

    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(HSys.LastSet.iRealDummy==DUMMY)
                fMain->ledEmpty1->Value=fMain->chkLoadEmpty1Tray->Checked;
            else
                fMain->ledEmpty1->Value=SnSelectHasTray[iEmpty1]->IsOn();
            if(HSys.LastSet.iRealDummy==DUMMY)
            {
                if(fMain->chkLoadEmpty1Tray->Checked)
                {
                    AutoCylinderUp(true, iEmpty1);
                    Task=50;
                }
            }
            else if(Sn_CarHasTray[iEmpty1]->IsOn() ||
                    Sn_CCDPos[iEmpty1]->IsOn())
            {
                if(HSys.FuncT.iT09_TrayIDReadPos==0)                            //KenHsieh 20250812 : add T09 Tray ID Func. 判斷，避免Hangup
                {
                    MotEmpty1_Car->GetNewNullTray();
                    Task=1;
                    return true;
                }
                else
                {
                    Task=420;
                }
            }
            else if(Sn_CarHasTray[iEmpty1]->IsOff()==true)                      //Car沒Tray, 要降一盤
            {
                if(SnSelectHasTray[iEmpty1]->IsOn()==true)                      //Empty上有Tray
                {
                    AutoCylinderUp(true, iEmpty1);
                    Task=50;
                }
                else                                                            //Empty上沒Tray, Alarm提醒補Tray
                {
                    ShowSystemError(SnSelectHasTray[iEmpty1]->OffAlarmCode, K_RETRY);           //這裡沒有ALARM CODE      EMPTY1
                }
            }
            else if(C_Middle[iEmpty1]->IsOff()==false && Sn_CarHasTray[iEmpty1]->IsOff())                                 //汽缸在上面
            {
                C_Up[iEmpty1]->Off();
                C_Middle[iEmpty1]->Off();
                break;
            }
            break;
        case 50:
            if(Sn_CarIsFull[iEmpty1]->IsOn())
            {
                ShowMyMessage("The trays of Empty1 are Full!");
            }
            else
            {
                Task=60;
            }
            break;
        case 60:
            bFlag=AutoCylinderUp(false, iEmpty1);
            if(bFlag)
            {
                C_Separatory[iEmpty1]->Reset();
                Task=100;
            }
            break;
        case 100:
            C_Separatory[iEmpty1]->Push();

            if(C_Separatory[iEmpty1]->IsOff()==false)
                Task=200;

            AutoCylinderMiddle(true, iEmpty1);
            break;
        case 200:
            bFlag=AutoCylinderMiddle(false, iEmpty1);
            if(bFlag)
            {
                C_Separatory[iEmpty1]->Reset();
                Task=300;
            }
            break;
        case 300:
            if(C_Separatory[iEmpty1]->Pop())
            {
                Task=400;
            }
            TrayToCarDelay[iEmpty1].SetSecAndOn(3);
            break;
        case 400:
            if(TrayToCarDelay[iEmpty1].Off())
            {
                AutoCylinderLower(true, iEmpty1);
                Task=410;
            }
            break;
        case 410:
            bFlag=AutoCylinderLower(false, iEmpty1);
            if(bFlag)
            {
                TrayToCarDelay[iEmpty1].SetSecAndOn(5);
                C_Empty_Stop[eTrack1]->Reset();
                Task=420;
            }
            break;
        case 420:
            if(Sn_CarHasTray[iEmpty1]->IsOn() ||
               Sn_CCDPos[iEmpty1]->IsOn()     ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                if(HSys.FuncT.iT09_TrayIDReadPos==0)
                {
                    fMain->ledEmpty1->Value=false;
                    MotEmpty1_Car->GetNewNullTray();
                    Task=1;
                    bResult=true;
                }
                else
                {
                    if(C_Empty_Stop[eTrack1]->Push())
                    {
                        TrayToCarDelay[iEmpty1].SetSecAndOn(10);
                        SwEmptyCW->On();
                        Task=500;
                    }
                }
            }
            else if(TrayToCarDelay[iEmpty1].Off())
            {
                ret=ShowSystemError(Sn_CarHasTray[iEmpty1]->OffAlarmCode, K_RETRY|K_SKIP);
                if(ret==K_SKIP)
                {
                    Task=1;
                }
            }
            break;
        case 500:
            if(SwEmptyCW->Status()==false)                                      //KenHsieh 20250821 : 暫停後須重新計數
                TrayToCarDelay[iEmpty1].SetSecAndOn(10);
            SwEmptyCW->On();
            if(SnEmptyHasTray[eTrack1]->IsOff()==false)
            {
                SwEmptyCW->Off();
                fMain->ledEmpty1->Value=false;
                MotTrack_Work[eTrack1]->GetNewNullTray();
                Task=1;
                bResult=true;
            }
            else if(TrayToCarDelay[iEmpty1].Off())                              //KenHsieh 20250812 : 新增異常提示，避免Hangup
            {
                ShowSystemError(SnEmptyHasTray[eTrack1]->OffAlarmCode, K_RETRY);
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoLoadNewTrayToEmpty2Car(bool bReset)
{
    bool bFlag;
    AnsiString Str;
    bool bResult=false;
    int &Task=iLoadNewTrayToEmpty2CarTask;
    int iEmpty2=eEmpty2;

    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(HSys.LastSet.iRealDummy==DUMMY)
                fMain->ledEmpty2->Value=fMain->chkLoadEmpty2Tray->Checked;
            else
                fMain->ledEmpty2->Value=SnSelectHasTray[iEmpty2]->IsOn();
            if(HSys.LastSet.iRealDummy==DUMMY)
            {
                if(fMain->chkLoadEmpty2Tray->Checked)
                {
                    AutoCylinderUp(true, iEmpty2);
                    Task=50;
                }
            }
            else if(Sn_CarHasTray[iEmpty2]->IsOn() || Sn_CCDPos[iEmpty2]->IsOn())
            {
                MotEmpty2_Car->GetNewNullTray();
                Task=1;
                return true;
            }
            else if(Sn_CarHasTray[iEmpty2]->IsOff()==true)
            {
                if(SnSelectHasTray[iEmpty2]->IsOn()==true)
                {
                    AutoCylinderUp(true, iEmpty2);
                    Task=50;
                }
                else
                {
                    ShowSystemError(SnSelectHasTray[iEmpty2]->OffAlarmCode, K_RETRY);
                }
            }
            else if(C_Middle[iEmpty2]->IsOff()==false && Sn_CarHasTray[iEmpty2]->IsOn()==false)                                 //汽缸在上面
            {
                C_Up[iEmpty2]->Off();
                C_Middle[iEmpty2]->Off();
                break;
            }
            break;
        case 50:
            bFlag=AutoCylinderUp(false, iEmpty2);
            if(bFlag)
            {
                C_Separatory[iEmpty2]->Reset();
                Task=100;
            }
            break;
        case 100:
            C_Separatory[iEmpty2]->Push();

            if(C_Separatory[iEmpty2]->IsOff()==false)
                Task=200;

            AutoCylinderMiddle(true, iEmpty2);
            break;
        case 200:
            bFlag=AutoCylinderMiddle(false, iEmpty2);
            if(bFlag)
            {
                C_Separatory[iEmpty2]->Reset();
                Task=300;
            }
            break;
        case 300:
            if(C_Separatory[iEmpty2]->Pop())
            {
                Task=400;
                TrayToCarDelay[iEmpty2].SetSecAndOn(3);
            }
            break;
        case 400:
            if(TrayToCarDelay[iEmpty2].Off())
            {
                AutoCylinderLower(true, iEmpty2);
                Task=410;
            }
            break;
        case 410:
            bFlag=AutoCylinderLower(false, iEmpty2);
            if(bFlag)
            {
                TrayToCarDelay[iEmpty2].SetSecAndOn(1);
                Task=420;
            }
            break;
        case 420:
            if(Sn_CarHasTray[iEmpty2]->IsOn() ||
               (HSys.LastSet.iRealDummy==DUMMY && fMain->chkLoadEmpty2Tray->Checked==true))
            {
                fMain->ledEmpty2->Value=false;
                MotEmpty2_Car->GetNewNullTray();

                if(HSys.LastSet.iRealDummy==DUMMY && fMain->chkLoadEmpty2Tray->Checked==true)
                {
                    Str.sprintf("Empty2_%02d%02d%02d", SystemHour, SystemMin, SystemSec);
                    MotEmpty2_Car->SetTrayID(Str);
                }
                Task=1;
                bResult=true;
            }
            else if(TrayToCarDelay[iEmpty2].Off())
            {
                int ret;
                ShowSystemError(Sn_CarHasTray[iEmpty2]->OffAlarmCode, K_RETRY|K_SKIP);
                if(ret==K_SKIP)
                {
                    Task=1;
                }
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoTransferToMagLD(bool bReset)
{
    AnsiString Str;
    bool bResult=false;
    int &Task=iTransferToMagLDTask;
    static bool bXMove=false;
    int iEmpty2=eEmpty2;
    static bool bCyCheck[2]={false, false};
    bool bsimu=(HSys.LastSet.iRealDummy==DUMMY);
    if(bReset)
    {
        bCyCheck[0]=false;
        bCyCheck[1]=false;
        Task=1;
        return bResult;
    }
    if(bNeedPickTrayToMagTop==true)
        return false;

    switch(Task)
    {
        case 1:
            if(MotTransferX_Data->fHasTray)
            {
                bXMove=false;
                Task=100;
            }
            break;
        case 100:
            if(bXMove==false)
                bXMove=MotTransferX->MotorMove(iTransferX_Empty);
            if(bXMove)
            {
                C_ShiftStageEdgePush->Reset();
                Task=200;
            }
            break;
        case 200:
            if(MotMagLD_Work->fHasTray==false)
            {
                if(C_ShiftStageEdgePush->Pop())
                {
                    C_Stop[iEmpty2]->Reset();
                    LoaderCylinderPreOn(true, eEmpty2);                         //Sam 20240810 : Loader Cylinder PreOn
                    Task=210;
                }
            }
            break;
        case 210:
            if(LoaderCylinderPreOn(false, eEmpty2))
                Task=300;
            break;
        case 300:
            if(C_Stop[iEmpty2]->Push())
            {
                SwEmpty2CCWDete->On();
                SwMagCCW->On();
                SwTransferCCW->On();    //JerryYang 20240724 : add timeout
                Empty2MoveTimeOut.SetSecAndOn(20);
                Task=400;
            }
            break;
        case 400:
            if(SwMagCCW->Status()==false || SwTransferCCW->Status()==false)     //Sam 20240730 : 新增 Alarm Code
                Empty2MoveTimeOut.SetSecAndOn(20);

            SwMagCCW->On();
            SwTransferCCW->On();
            if((SnMagPreDete->IsOff()==false &&
               SnMagSureTray->IsOff()==false) || bsimu)
            {
                C_Fixer[iEmpty2]->Reset();
                C_EdgePush[iEmpty2]->Reset();
                Empty2TrayDelay.SetSecAndOn(dUnloadArrival);
                Task=500;
            }
            else if(Empty2MoveTimeOut.Off())   //Sam 20240730 : 新增 Alarm Code //JerryYang 20240724 : add timeout
            {
                ShowSystemError("80105", K_RETRY);
                Task=300;
            }
            break;
        case 500:
            SwTransferCCW->On();
            SwMagCCW->On();
            if(Empty2TrayDelay.Off())
            {
                SwMagCCW->Off();
                SwEmpty2CCWDete->Off();
                SwTransferCCW->Off();
                bCyCheck[0]=false;
                bCyCheck[1]=false;
                Task=550;
            }
            break;
        case 550:
            if(bCyCheck[0]==false)
                bCyCheck[0]=(C_EdgePush[iEmpty2]->Push());
            if(bCyCheck[1]==false)
                bCyCheck[1]=(C_Fixer[iEmpty2]->Push());

            if(bCyCheck[0] && bCyCheck[1])
            {
                C_EdgePush[iEmpty2]->Reset();
                C_Fixer[iEmpty2]->Reset();
                bCyCheck[0]=false;
                bCyCheck[1]=false;
                Task=600;
            }
            break;
        case 600:
            MotMagLD_Work->MoveTrayFrom(MotTransferX_Data);
            bXMove=false;           //Sam 20240619 : Shift Transfer 動作優化
            Task=700;
            break;
        case 700:                   //Sam 20240619 : Shift Transfer 動作優化
            if(bXMove==false)
                bXMove=MotTransferX->MotorMove(iTransferX_LD);
            if(bXMove)
                bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoWorkAreaToMagUnloader(bool bNeedGoOut, bool bReset)
{
    bool bFlag;
    AnsiString Str;
    bool bResult=false;
    int &Task=iWorkAreaToMagUnloaderTask;
    static bool bCyflag[5]={false, false, false, false, false};
    int iEmpty2=eEmpty2;
    double dBackTrayDelay=40.0;
    bool bsimu=(HSys.LastSet.iRealDummy==DUMMY);

    if(bNeedPickTrayToMagTop==true)
    {
        ShowMyMessage("Magazine prcoess error!");
        return bResult;
    }

    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(Sn_CarIsFull[iEmpty2]->IsOn()==true)
            {
                ShowMyMessage("The trays of Empty2 are Full!");
                break;
            }
            Task=100;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 100:
            C_Stop[iEmpty2]->Reset();
            C_Fixer[iEmpty2]->Reset();
            C_EdgePush[iEmpty2]->Reset();
            C_CCD_Stop[iEmpty2]->Reset();

            for(int i=0; i<5; i++)
                bCyflag[i]=false;
            Task=200;
            if(bsimu)
                break;
        case 200:
            if(bCyflag[1]==false)
                bCyflag[1]=C_Stop[iEmpty2]->Pop();
            if(bCyflag[2]==false)
                bCyflag[2]=C_Fixer[iEmpty2]->Pop();
            if(bCyflag[3]==false)
                bCyflag[3]=(C_EdgePush[iEmpty2]->Pop());
            if(bCyflag[4]==false)
                bCyflag[4]=C_CCD_Stop[iEmpty2]->Pop();

            if(bCyflag[1] && bCyflag[2] && bCyflag[3] && bCyflag[4])
            {
                Task=300;
            }
            break;
        case 300:                                                               //要先判斷有無Tray 在Car上
            if(Sn_CarHasTray[iEmpty2]->IsOff()==false ||
               MotEmpty2_Car->fHasTray==true)
            {
                AutoCylinderMiddle(true, iEmpty2);
                Task=400;
            }
            else
            {
                Task=500;
            }
            break;
        case 400:
            bFlag=AutoCylinderMiddle(false, iEmpty2);
            if(bFlag)
            {
                C_Separatory[iEmpty2]->Reset();
                Task=410;
            }
            break;
        case 410:
            if(C_Separatory[iEmpty2]->Push())
            {
                Task=420;
                TrayToCarDelay[iEmpty2].SetSecAndOn(3);
            }
            break;
        case 420:
            if(TrayToCarDelay[iEmpty2].Off())
            {
                AutoCylinderUp(true, iEmpty2);
                Task=430;
            }
            break;
        case 430:
            bFlag=AutoCylinderUp(false, iEmpty2);
            if(bFlag)
            {
                C_Separatory[iEmpty2]->Reset();
                Task=440;
            }
            break;
        case 440:
            if(C_Separatory[iEmpty2]->Pop())
            {
                Task=450;
                AutoCylinderLower(true, iEmpty2);
            }
            break;
        case 450:
            bFlag=AutoCylinderLower(false, iEmpty2);
            if(bFlag)
            {
                C_Separatory[iEmpty2]->Reset();
                MotEmpty2_Car->ClearTray();
                if(bNeedGoOut)
                {
                    Task=500;
                }
                else
                    bResult=true;
            }
            break;
        case 500:
            SwMagCCW->On();
            Empty2MoveTimeOut.SetSecAndOn(dBackTrayDelay);        //Sam 20240730 : 新增 Alarm Code
            Task=550;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 550:
            if(SwMagCCW->Status()==false)       //Sam 20240730 : 新增 Alarm Code
                Empty2MoveTimeOut.SetSecAndOn(dBackTrayDelay);
            SwMagCCW->On();
            if(Sn_CarHasTray[iEmpty2]->IsOff()==false ||
               bsimu)
            {
                Task=600;
            }
            else if(Empty2MoveTimeOut.Off())    //Sam 20240730 : 新增 Alarm Code
            {
                ShowSystemError("80107", K_RETRY);
                Task=500;
            }
            break;
        case 600:
            TrayToCarDelay[iEmpty2].SetSecAndOn(6.0);
            Empty2MoveTimeOut.SetSecAndOn(6.0);
            Task=601;
//            break;
        case 601:
            if(SwMagCCW->Status()==false)       //Sam 20240730 : 新增 Alarm Code
                Empty2MoveTimeOut.SetSecAndOn(6.0);
            SwMagCCW->On();
            if((Sn_CarHasTray[iEmpty2]->IsOff()==false &&
               TrayToCarDelay[iEmpty2].Off()) ||
               bsimu)
            {
                SwMagCCW->Off();
                SwTransferCCW->Off();
                MotEmpty2_Car->MoveTrayFrom(MotMagLD_Work);                     //HSys.VMot.Transfer_LoaderY
                MagArmPara->MMagNowSortTray->ClearTray();                       //KenHsieh 20240618 : 補上Magazine Now
                if(HSys.FuncT.iT02_MagTraySource==1)    //JerryYang 20240626 : Color的tray盤要退到上面
                {
                    Task=700;
                    AutoCylinderMiddle(true, iEmpty2);  //JerryYang 20240713 : 初始化
                }
                else
                {
                    bResult=true;
                }
            }
            else if(Empty2MoveTimeOut.Off())    //Sam 20240730 : 新增 Alarm Code
            {
                ShowSystemError("80107", K_RETRY);
                Task=600;
            }
            break;
        case 700:
            bFlag=AutoCylinderMiddle(false, iEmpty2);
            if(bFlag)
            {
                C_Separatory[iEmpty2]->Reset();
                Task=800;
            }
            break;
        case 800:
            if(C_Separatory[iEmpty2]->Push())
            {
                Task=850;
                TrayToCarDelay[iEmpty2].SetSecAndOn(3);
            }
            break;
        case 850:
            if(TrayToCarDelay[iEmpty2].Off())
            {
                AutoCylinderUp(true, iEmpty2);
                Task=900;
            }
            break;
        case 900:
            bFlag=AutoCylinderUp(false, iEmpty2);
            if(bFlag)
            {
                TrayToCarDelay[iEmpty2].SetSecAndOn(1);
                C_Separatory[iEmpty2]->Reset();
                Task=950;
            }
            break;
        case 950:
            if(C_Separatory[iEmpty2]->Pop())
            {
                Task=1000;
                AutoCylinderLower(true, iEmpty2);
            }
            TrayToCarDelay[iEmpty2].SetSecAndOn(3);
            break;
        case 1000:
            bFlag=AutoCylinderLower(false, iEmpty2);
            if(bFlag)
            {
                MotEmpty2_Car->ClearTray();
                bResult=true;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoTransferToMagUnloader(bool bReset)
{
    bool bFlag;
    AnsiString Str;
    bool bResult=false;
    int &Task=iTransferToMagUnloaderTask;
    static bool bCyflag[5]={false, false, false, false, false};
    static bool bXMove=false;
    int iEmpty2=eEmpty2;
    double dBackTrayDelay=40.0;
    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(MotTransferX_Data->fHasTray)
            {
                bXMove=false;
                Task=100;
            }
            break;
        case 100:
            if(bXMove==false)
                bXMove=MotTransferX->MotorMove(iTransferX_Empty);
            if(bXMove)
            {
                C_ShiftStageEdgePush->Reset();
                C_Stop[iEmpty2]->Reset();
                C_Fixer[iEmpty2]->Reset();
                C_EdgePush[iEmpty2]->Reset();
                C_CCD_Stop[iEmpty2]->Reset();

                for(int i=0; i<5; i++)
                    bCyflag[i]=false;
                Task=200;
            }
            break;
        case 200:
            if(MotMagLD_Work->fHasTray==false)
            {
                if(bCyflag[0]==false)
                    bCyflag[0]=C_ShiftStageEdgePush->Pop();
                if(bCyflag[1]==false)
                    bCyflag[1]=C_Stop[iEmpty2]->Pop();
                if(bCyflag[2]==false)
                    bCyflag[2]=C_Fixer[iEmpty2]->Pop();
                if(bCyflag[3]==false)
                    bCyflag[3]=(C_EdgePush[iEmpty2]->Pop());
                if(bCyflag[4]==false)
                    bCyflag[4]=C_CCD_Stop[iEmpty2]->Pop();

                if(bCyflag[0] && bCyflag[1] && bCyflag[2] && bCyflag[3] && bCyflag[4])
                {
                    Task=300;
                }
            }
            break;
        case 300:                                                               //要先判斷有無Tray 在Car上
            if(Sn_CarIsFull[iEmpty2]->IsOn()==true)
            {
                ShowMyMessage("The trays of Empty2 are Full!");
                break;
            }
            else if(Sn_CarHasTray[iEmpty2]->IsOn() ||
               MotEmpty2_Car->fHasTray==true)
            {
                AutoCylinderMiddle(true, iEmpty2);
                Task=400;
            }
            else
            {
                Task=500;
            }
            break;
        case 400:
            bFlag=AutoCylinderMiddle(false, iEmpty2);
            if(bFlag)
            {
                C_Separatory[iEmpty2]->Reset();
                Task=410;
            }
            break;
        case 410:
            if(C_Separatory[iEmpty2]->Push())
            {
                Task=420;
            }
            TrayToCarDelay[iEmpty2].SetSecAndOn(3);
            break;
        case 420:
            if(TrayToCarDelay[iEmpty2].Off())
            {
                AutoCylinderUp(true, iEmpty2);
                Task=430;
            }
            break;
        case 430:
            bFlag=AutoCylinderUp(false, iEmpty2);
            if(bFlag)
            {
                TrayToCarDelay[iEmpty2].SetSecAndOn(1);
                C_Separatory[iEmpty2]->Reset();
                Task=440;
            }
            break;
        case 440:
            if(C_Separatory[iEmpty2]->Pop())
            {
                Task=450;
                AutoCylinderLower(true, iEmpty2);
            }
            TrayToCarDelay[iEmpty2].SetSecAndOn(6);
            break;
        case 450:
            bFlag=AutoCylinderLower(false, iEmpty2);
            if(bFlag)
            {
                TrayToCarDelay[iEmpty2].SetSecAndOn(6);
                C_Separatory[iEmpty2]->Reset();
                MotEmpty2_Car->ClearTray();
                Task=500;
            }
            break;
        case 500:
            SwMagCCW->On();
            SwTransferCCW->On();
            Empty2MoveTimeOut.SetSecAndOn(dBackTrayDelay);  //Sam 20240730 : 新增 Alarm Code
            Task=600;
            break;
        case 600:
            if(SwMagCCW->Status()==false || SwTransferCCW->Status()==false) //Sam 20240730 : 新增 Alarm Code
                Empty2MoveTimeOut.SetSecAndOn(dBackTrayDelay);
            SwMagCCW->On();
            SwTransferCCW->On();
            if((Sn_CarHasTray[iEmpty2]->IsOn() && TrayToCarDelay[iEmpty2].Off()) ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                TrayToCarDelay[iEmpty2].SetSecAndOn(6);
                Task=610;
            }
            else if(Empty2MoveTimeOut.Off())     //Sam 20240730 : 新增 Alarm Code
            {
                ShowSystemError("80106", K_RETRY);
                Task=500;
            }
            break;
        case 610:
            if((Sn_CarHasTray[iEmpty2]->IsOn() && TrayToCarDelay[iEmpty2].Off()) ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                MotEmpty2_Car->MoveTrayFrom(MotTransferX_Data);                 //Steven 20240520 : CopyTrayFrom --> MoveTrayFrom
                SwMagCCW->Off();
                SwTransferCCW->Off();
                bXMove=false;
                Task=700;
            }
            break;
        case 700:
            if(bXMove==false)
                bXMove=MotTransferX->MotorMove(iTransferX_LD);
            if(bXMove)
                bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::CheckFullTray(int &X, int &Y)
{
    bool bResult=false;

    for(int i=eTrack1; i<=eTrack4; i++)
    {
        for(int j=eTrackCol1; j<eTrackColTotal; j++)
        {
            if(bResult==false &&
               Auto->Mot_Auto_Work[i][j]->fHasTray==true &&
               Auto->Mot_Auto_Work[i][j]->FullIC())
            {
                X=i;
                Y=j;

                return true;
            }
        }
    }

    return bResult;
}
//------------------------------------------------------------------------------
int TLoaderModule::CheckAutoHasTray()
{
    MyBinToTrayStruct mBtoT;
    mBtoT.ResetData();
    for(int i=eTrack1; i<=eTrack4; i++)
    {
        for(int j=eTrackCol1; j<eTrackColTotal; j++)
        {
            if(Sen_Auto_HasTray[i][j]->IsOn() &&
               Auto->Mot_Auto_Work[i][j]->fHasTray==true)
            {
                mBtoT.ConvertRowColToBin(i, j);
                return mBtoT.iWhichAuto;
            }
        }
    }

    return mBtoT.iWhichAuto;
}
//------------------------------------------------------------------------------
bool TLoaderModule::CheckAllAutoNoTray()
{
    bool bRet=true;
    for(int i=eTrack1; i<=eTrack4; i++)
    {
        for(int j=eTrackCol1; j<eTrackColTotal; j++)
        {
            if(Auto->Mot_Auto_Work[i][j]->fHasTray)
                bRet=false;
        }
    }
    return bRet;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoLoaderTrayToWorkArea(bool bReset)                         //進入工作區域
{
    int &Task=iLoaderTrayToWorkAreaTask;
    static bool bCynActOk1=false, bCynActOk2=false;
    bool bResult=false;
    AnsiString Str;
    int iLoader=eLoader1;
    int ret=0;

    if(bReset)
    {
        bCynActOk1=false;
        bCynActOk2=false;
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            LoaderCylinderPreOn(true, iLoader);                                 //Sam 20240810 : Loader Cylinder PreOn
            Task=10;
        case 10:
            if(LoaderCylinderPreOn(false, iLoader))
                Task=50;
            break;
        case 50:
            C_Stop[iLoader]->Reset();
            C_Fixer[iLoader]->Reset();
            C_EdgePush[iLoader]->Reset();
            C_CCD_Stop[iLoader]->Reset();
            SwLoaderCWDete->On();                                               //JerryYang 20240530 : 移到上面
            Task=100;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 100:                                                               //Read Tray ID
            C_CCD_Stop[iLoader]->Off();
            C_Stop[iLoader]->On();
            C_EdgePush[iLoader]->Off();
            C_Fixer[iLoader]->Off();
            Task=200;

            LoaderMoveTimeOut.SetSecAndOn(10);                                  //JerryYang 20250716 : fix hang up

            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 200:
            if((C_Stop[iLoader]->IsOn()            &&
                C_Fixer[iLoader]->IsOff()          &&
                C_CCD_Stop[iLoader]->IsOn()==false &&
                SnLD_EdgePush->IsOff())            ||                           //KenHsieh 20240528 : 補上汽缸到位保護
                HSys.LastSet.iRealDummy==DUMMY)
            {
                SwLoaderCW->On();
                LoaderMoveTimeOut.SetSecAndOn(10);                              //Sam 20240730 : 新增 Alarm Code
                Task=600;
            }
            else if(LoaderMoveTimeOut.Off())                                    //JerryYang 20250716 : fix hang up
            {
                if(C_Stop[iLoader]->IsOn()==false)
                {
                    ShowSystemError(HSys.Cyn.C_Loader_Stop.ErrorName[eOnNotOnErr], K_RETRY);
                }
                if(C_Fixer[iLoader]->IsOff()==false)
                {
                    ShowSystemError(HSys.Cyn.C_TrayY_Fixer.ErrorName[eOffNotOffErr], K_RETRY);
                }
                if(C_CCD_Stop[iLoader]->IsOn())
                {
                    ShowSystemError(HSys.Cyn.C_Loader_CCDStop.ErrorName[eOffNotOffErr], K_RETRY);
                }
                if(SnLD_EdgePush->IsOff()==false)
                {
                    ShowSystemError(HSys.Cyn.C_LoaderEdgePush.ErrorName[eOffNotOffErr], K_RETRY);
                }
                Task=100;
            }            
            break;
        case 600:                                                               //確認Stop 氣缸打開
            if(SwLoaderCW->Status()==false)                                     //Sam 20240527 : 修正 Loader 進料時被按暫停 TimeOut 問題。
                LoaderMoveTimeOut.SetSecAndOn(10);                              //Sam 20240730 : 新增 Alarm Code

            SwLoaderCWDete->On();
            SwLoaderCW->On();

            if(HSys.LastSet.iRealDummy==DUMMY ||
               (SnLDSureTray->IsOn() && SnLDPreDete->IsOn()))
            {
                TrayInPosDelay.SetSecAndOn(dLoadArrival);
                Task=610;
            }
            else if(LoaderMoveTimeOut.Off())                                    //Sam 20240730 : 新增 Alarm Code
            {
                ret=ShowSystemError(SnLDSureTray->Name, K_RETRY|K_SKIP);        //Sam 20170726 : 再次判斷有無 Tray 在軌道上

                if(ret==K_SKIP)
                {
                    MotLoader_Car->ClearTray();
                    SwLoaderCWDete->Off();
                    SwLoaderCW->Off();
                    bResult=true;
                }
                else
                {
                    Task=100;
                }
            }
            break;
        case 610:
            if(TrayInPosDelay.Off())
            {
                SwLoaderCWDete->Off();
                SwLoaderCW->Off();
                Task=700;
            }
            break;
        case 700:                                                               //勾住Tray
            if(HSys.LastSet.iRealDummy==DUMMY ||
               (SnLDSureTray->IsOn() && SnLDPreDete->IsOn()))
            {
                bCynActOk1=false;
                bCynActOk2=false;
                C_Fixer[iLoader]->Reset();
                C_EdgePush[iLoader]->Reset();
                Task=710;
            }
            else                                                                //Sam 20170713 (jou) : 勾住 Tray 時可能會造成 Tray 滑動，使 Sn_CarHasTray[0] Sensor 偵測不到 Tray 造成 Hang Up 增加報警
            {
                ShowSystemError(SnLDSureTray->Name, K_RETRY);
                Task=610;
            }
            break;
        case 710:                                                               //KenHsieh 20240528 : 補上汽缸到位保護
             if(bCynActOk1==false)
                bCynActOk1=(C_Fixer[iLoader]->Push() && SnLD_FixCyPush->IsOff()==false);
             if(bCynActOk2==false)
                bCynActOk2=(C_EdgePush[iLoader]->Push() && SnLD_EdgePush->IsOff()==false);

             if(bCynActOk1 && bCynActOk2)
             {
                Task=800;
                TrayInPosDelay.SetSecAndOn(dLoadArrival);
             }
             break;
        case 800:
            if(TrayInPosDelay.Off())
            {
                iUPH_PickCnt=0;
                MotWorkLoader->MoveTrayFrom(MotLoader_Car);                         //Steven 20240520 : 改用MoveTrayFrom
                for(int i=0; i<eTrayCount; i++)
                {
                    SortArmPara->iInputIC_Loader[i] =SortArmPara->iInputIC_LoadCar[i];
                    SortArmPara->iInputIC_LoadCar[i]=0;
                }
                SetStatus(eHS_WorkAreaReady, __FUNC__);
                Task++;                                                             //確保不要一直進來, 不然會重置Tray狀態
                bResult=true;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoTrackAreaToStoreTrayArm(int &Task, int iTrack)            //退出工作區域
{
    bool bResult=false;
    static bool bCyFlag[eTrackCount]={false};
    static int iNowAutoNeedTray[eTrackCount]={0};

    int iCol=iNowAutoNeedTray[iTrack];
    AnsiString Str="";

    static bool bXMove[eTrackCount]={false};
    static bool bZMove[eTrackCount]={false};
    static bool bCylin[eTrackCount]={false};

    switch(Task)
    {
        case 1:
            for(int j=eTrackCol1; j<eTrackColTotal; j++)
            {
                if(Auto->Mot_Auto_Work[iTrack][j]->fHasTray==true)
                {
                    Task=50;
                }
            }

            if(bSupportingTray==true)                                           //Empty 1正在入盤
                Task=1;

            if(Task!=50)
            {
                if(HSys.Sys.RunMode==Run_OneCycle)
                {
                    bTrackOneCycleFinish[iTrack]=true;
                }
                bTrackUnloadingTray[iTrack]=false;
                break;
            }
        case 50:
            if(bSupportingTray==true)                                           //Sam 20240527 : 增加保護。//Empty 1正在入盤
                Task=1;
            iCol=100;
            bCyFlag[iTrack]=false;
            for(int iC=eTrackCol1; iC<eTrackColTotal; iC++)
            {
                if((Auto->Mot_Auto_Work[iTrack][iC]->FullIC() ||
                   (bFlagAutoArmCleanoutFinish==true &&
                    Auto->Mot_Auto_Work[iTrack][iC]->fHasTray==true)) &&
                   iCol==100)
                {
                    C_Auto_Stop[iTrack][iC]->Reset();
                    C_Auto_Zup[iTrack][iC]->Reset();
                    C_Auto_InclinePush[iTrack][iC]->Reset();

                    iCol=iC;
                    iNowAutoNeedTray[iTrack]=iC;
                }
            }

            if(iCol==100)
            {
                bTrackUnloadingTray[iTrack]=false;
                for(int iC=eTrackCol1; iC<eTrackColTotal; iC++)
                {
                    bAutoUnloadingTray[iTrack][iC]=false;
                }
                if(HSys.Sys.RunMode==Run_OneCycle)
                {
                    bTrackOneCycleFinish[iTrack]=true;
                }                

                bResult=true;
            }
            else
            {
                bTrackOneCycleFinish[iTrack]=false;                             //JerryYang 20240829 : 滿TRAY退完再跳ONE CYCLE FINISH            
                bTrackUnloadingTray[iTrack]=true;
                bAutoUnloadingTray[iTrack][iCol]=true;
                Task=100;
            }
            break;
        case 100:
            if(Auto->MTrackToStore[iTrack]->fHasTray==false)
            {
                C_Auto_InclinePush[iTrack][iCol]->On();                         //KenHsieh 20240515 : 斜推先放

                if(HSys.FuncT.iT06_AutoRowType==1)
                {
                    tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(10);
                    bXMove[iTrack]=false;
                    bZMove[iTrack]=false;
                    Task=1000;
                }
                else if(USE_PUSH_TRAY==ePushByMotor)
                {
                    TrayArmPara->InitialPushTray(true, iTrack);                     //推Tray 機構改為馬達
                    Task=110;
                }
                else //ePushByConveyor
                {
                    Task=200;
                }
            }
            break;
        case 110:                                                               //推Tray 機構改為馬達
            if(TrayArmPara->InitialPushTray(false, iTrack))
            {
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(10);
                Task=200;
            }
            break;
        case 200:
            if(C_Auto_InclinePush[iTrack][iCol]->IsOn()==false || USE_PUSH_TRAY==ePushByConveyor)
            {
                C_Auto_Stop[iTrack][iCol]->Off();
                C_Auto_Zup[iTrack][iCol]->On();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(0.8);      //JerryYang 20240607 : Auto區Up汽缸先ON/OFF各0.5秒洩氣減少暴衝
                Task=205;
            }
            else if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                MyBinToTrayStruct BtoT;
                BtoT.ConvertRowColToBin(iTrack, iCol);
                ShowMyMessage(AnsiString().sprintf("C_Auto_InclinePush Auto %d Off Timeout!", BtoT.iWhichAuto));   //KenHsieh 20240710 : iTrack 不用減一
                Task=100;
            }
            break;
        case 205:
            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                C_Auto_Zup[iTrack][iCol]->Off();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(0.5);
                Task=206;
            }
            break;
        case 206:
            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                C_Auto_Zup[iTrack][iCol]->On();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(5);
                Task=210;
            }
            break;
        case 210:
            if(C_Auto_Zup[iTrack][iCol]->IsOn() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                C_Auto_AntiDrop[iTrack][iCol]->Reset();
                C_Auto_InclinePush[iTrack][iCol]->Reset();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(1.5);
                Task=215;
            }
            else if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                ShowSystemError(C_Auto_Zup[iTrack][iCol]->ErrorName[eOnNotOnErr], K_RETRY);     //"Auto Zup 汽缸上升異常"
            }
            break;
        case 215:
            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                C_Auto_AntiDrop[iTrack][iCol]->On();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(1);
                Task=300;
            }
            else
            {
                break;
            }
        case 300:
            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                C_Auto_Zup[iTrack][iCol]->Reset();
                C_Auto_Zup[iTrack][iCol]->Off();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(10);
                Task=400;
            }
            else
            {
                break;
            }
        case 400:
            if(C_Auto_Zup[iTrack][iCol]->IsOff() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                if(Sen_Auto_HasTray[iTrack][iCol]->IsOn()==false ||
                   HSys.LastSet.iRealDummy==DUMMY)
                {
                    C_AutoRow_Stop[iTrack]->Reset();
                    tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(1);
                    Task=500;
                }
                else if(tTrackToStoreDelay[iTrack][iCol].Off())                 //Time Out
                {
                    ShowSystemError(Sen_Auto_HasTray[iTrack][iCol]->OnAlarmCode, K_RETRY);
                    Task=300;
                }
            }
            else if(tTrackToStoreDelay[iTrack][iCol].Off())                     //Time Out
            {
                ShowSystemError(C_Auto_Zup[iTrack][iCol]->ErrorName[eOffNotOffErr], K_RETRY);
                Task=300;
            }
            break;
        case 500:
            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                if(C_AutoRow_Stop[iTrack]->Push())
                {
                    SwAutoRowDete[iTrack]->On();
                    SwEmptyRowCW[iTrack]->On();
                    tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(30);           //KenHsieh 20240515 : 改為到位後開始計數
                    Task=600;
                }
            }
            break;
        case 600:
            if(SwEmptyRowCW[iTrack]->Status()==false)
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(30);
            SwAutoRowDete[iTrack]->On();
            SwEmptyRowCW[iTrack]->On();
            if(SnAutoRowHasTray[iTrack]->IsOff()==false ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(1.5);              //KenHsieh 20240830 : 0.5 -> 1.5 延長滾動時間
                Task=610;
            }
            else if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                ShowSystemError(SnAutoRowHasTray[iTrack]->Name, K_RETRY);       //"Track%d to Tray Arm Area Time Out"
                Task=500;
            }
            break;
        case 610:
            SwAutoRowDete[iTrack]->On();
            SwEmptyRowCW[iTrack]->On();
            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                SwEmptyRowCW[iTrack]->Off();
                SwAutoRowDete[iTrack]->Off();

                if(CUSTOMER_CODE==CC_WINSTEK)
                    Auto->Mot_Auto_Work[iTrack][iCol]->SaveWinstekLog();

                Auto->MTrackToStore[iTrack]->Tray.iTrackCol=iCol;
                Auto->MTrackToStore[iTrack]->Tray.iTrackRow=iTrack;

                Auto->MTrackToStore[iTrack]->MoveTrayFrom(Auto->Mot_Auto_Work[iTrack][iCol]);
                bAutoUnloadingTray[iTrack][iCol]=false;

                Task=1;
            }
            break;
        case 1000:
            if(bZMove[iTrack]==false)
                bZMove[iTrack]=MCarrierStoreZ[iTrack]->MotorMove(iPos_Auto_ZDown[iTrack][iCol]);

            if(bZMove[iTrack]==true && bXMove[iTrack]==false)
            {
                MCarrierStoreZ[iTrack]->ScanMotorStatus();
                if(MCarrierStoreZ[iTrack]->Led[iHomeLed]==false &&
                   HSys.LastSet.iRealDummy!=DUMMY)
                {
                    bZMove[iTrack]=false;
                    fHome->SingleMotorHome(true, MCarrierStoreZ[iTrack]->Tag);
                    Task=1005;
                    break;
                }
                else
                {
                    bXMove[iTrack]=MCarrierStoreX[iTrack]->MotorMove(iPos_Auto_X[iTrack][iCol]);
                }
            }

            if(bXMove[iTrack] && bZMove[iTrack])
            {
                C_Auto_InclinePush[iTrack][iCol]->On();
                bXMove[iTrack]=false;
                bZMove[iTrack]=false;
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(10);
                Task=1010;
            }
            break;
        case 1005:
            if(fHome->SingleMotorHome(false, MCarrierStoreZ[iTrack]->Tag))
            {
                bZMove[iTrack]=false;
                bXMove[iTrack]=false;
                Task=1000;
            }
            break;
        case 1010:
            if(C_Auto_InclinePush[iTrack][iCol]->IsOn()==false)
            {
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(0.8);      //JerryYang 20240607 : Auto區Up汽缸先ON/OFF各0.5秒洩氣減少暴衝
                Task=1020;
            }
            else if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                MyBinToTrayStruct BtoT;
                BtoT.ConvertRowColToBin(iTrack, iCol);
                ShowMyMessage(AnsiString().sprintf("C_Auto_InclinePush Auto %d Off Timeout!", BtoT.iWhichAuto));   //KenHsieh 20240710 : iTrack 不用減一
                Task=100;
            }
            break;
        case 1020:
            if(bZMove[iTrack]==false)
                bZMove[iTrack]=MCarrierStoreZ[iTrack]->MotorMove(iPos_Auto_Z[iTrack][iCol]);

            if(bZMove[iTrack])
            {
                bZMove[iTrack]=false;
                C_Auto_InclinePush[iTrack][iCol]->Reset();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(iAutoZUpDelay);
                Task=1030;
            }
            break;
        case 1030:
            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                C_Auto_AntiDrop[iTrack][iCol]->Reset();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(1);
                Task=1035;                                                      //JerryYang 20250716 : add
            }
            else
            {
                break;
            }
        case 1035:
            if(C_Auto_AntiDrop[iTrack][iCol]->Push())
            {
                Task=1036;
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(5);
            }        
            break;
        case 1036:
            if(C_Auto_AntiDrop[iTrack][iCol]->IsOn()==false &&
               SnAutoCyAntiDrop1[iTrack][iCol]->IsOn()==false &&
               SnAutoCyAntiDrop2[iTrack][iCol]->IsOn()==false &&
               SnAutoCyAntiDrop3[iTrack][iCol]->IsOn()==false)
            {
                Task=1040;
                break;
            }

            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                ShowSystemError(C_Auto_AntiDrop[iTrack][iCol]->ErrorName[eOnNotOnErr], K_RETRY);
            }
            break;
        case 1040:
            bZMove[iTrack]=MCarrierStoreZ[iTrack]->MotorMove(-50);
            if(bZMove[iTrack])
            {
                #ifndef SOFT_SIMULATE                                       //Sam 20250703 : 修正模擬
                MCarrierStoreZ[iTrack]->ScanMotorStatus();
                if(MCarrierStoreZ[iTrack]->Led[iHomeLed]==true)
                {
                    bZMove[iTrack]=false;
                    tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(10);
                    Task=1050;
                }
                else
                {
                    Str.sprintf("Please check AutoTrolly %d Z axis position, home sensor is not ON.", iTrack);
                    ShowMyMessage(Str);
                    fHome->SingleMotorHome(true, MCarrierStoreZ[iTrack]->Tag);
                    Task=1045;                        
                }
                #else
                    bZMove[iTrack]=false;
                    tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(10);
                    Task=1050;
                #endif
            }
            break;
        case 1045:
            if(fHome->SingleMotorHome(false, MCarrierStoreZ[iTrack]->Tag))
            {
                bZMove[iTrack]=false;
                bXMove[iTrack]=false;
                Task=1040;
            }
            break;

        case 1050:
            if(SnAutoTrollyHasTray[iTrack]->IsOff()==false)                     //JerryYang 20250717 : add
            {
                Task=1055;
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(10);
            }
            else if(tTrackToStoreDelay[iTrack][iCol].Off())                     //Time Out
            {
                ShowSystemError(SnAutoTrollyHasTray[iTrack]->Name, K_RETRY); //"Auto Tray Load Tray異常"
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(10);            
            }            
            break;
        case 1055:
            if(Sen_Auto_HasTray[iTrack][iCol]->IsOn()==false ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                C_AutoTrolly_Fix[iTrack]->Reset();
                tTrackToStoreDelay[iTrack][iCol].SetSecAndOn(1);
                Task=1060;
            }
            else if(tTrackToStoreDelay[iTrack][iCol].Off())                     //Time Out
            {
                ShowSystemError(Sen_Auto_HasTray[iTrack][iCol]->OnAlarmCode, K_RETRY);
                Task=1040;
            }
            break;
        case 1060:
            if(tTrackToStoreDelay[iTrack][iCol].Off())
            {
                if(C_AutoTrolly_Fix[iTrack]->Push())
                {
                    C_AutoTrolly_Fix[iTrack]->Reset();
                    bXMove[iTrack]=false;
                    bCylin[iTrack]=false;
                    Task=1070;
                }
            }
            break;
        case 1070:                                                              //變更台車速度，避免Tray 偏移
            if(bXMove[iTrack]==false)
            {
                bXMove[iTrack]=MCarrierStoreX[iTrack]->MotorMove(iPos_AutoXPush[iTrack]-5000);
            }
            if(bXMove[iTrack]==true)
            {
                MCarrierStoreX[iTrack]->SetPersentSpeed(5, false);
                bCylin[iTrack]=false;
                bXMove[iTrack]=false;
                Task=1071;
            }
            break;
        case 1071:
            if(bXMove[iTrack]==false)
                bXMove[iTrack]=MCarrierStoreX[iTrack]->MotorMove(iPos_AutoXPush[iTrack]);
            if(bXMove[iTrack]==true && bCylin[iTrack]==false)
                bCylin[iTrack]=C_AutoTrolly_Fix[iTrack]->Pop();
            if(bXMove[iTrack] && bCylin[iTrack])
            {
                MCarrierStoreX[iTrack]->SetPersentSpeed(MCarrierStoreX[iTrack]->GetPersentSpeed());
                C_AutoTrolly_Stop[iTrack]->Reset();
                bCylin[iTrack]=false;
                bXMove[iTrack]=false;
                Task=1080;
            }
            break;
        case 1080:
            if(bCylin[iTrack]==false)
                bCylin[iTrack]=C_AutoTrolly_Stop[iTrack]->Pop();

            if(bCylin[iTrack])
            {
                Auto->Mot_Auto_Work[iTrack][iCol]->SaveWinstekLog();
                Auto->MTrackToStore[iTrack]->MoveTrayFrom(Auto->Mot_Auto_Work[iTrack][iCol]);
                Auto->MTrackToStore[iTrack]->Tray.iTrackCol=iCol;
                Auto->MTrackToStore[iTrack]->Tray.iTrackRow=iTrack;
                bAutoUnloadingTray[iTrack][iCol]=false;
                bXMove[iTrack]=false;
                bCylin[iTrack]=false;
                Task=1;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::CheckEmpty2NeedTray()
{
    bool bNeedTray=false;
    MyBinToTrayStruct mBtoT;
    for(int iT=eMag2_01; iT<=eMag2_27;  iT++)
    {
        mBtoT.ConvertAutoToBin(iT);
        if(mBtoT.iBin!=0)
        {
            bNeedTray=true;
        }
    }

    return bNeedTray;
}
//------------------------------------------------------------------------------
bool TLoaderModule::CheckEmpty1TrackNeedTray(int iTrack)
{
    bool bNeedTray=false;
    MyBinToTrayStruct mBtoT;
    for(int iC=eTrackCol1; iC<eTrackColTotal; iC++)
    {
        mBtoT.ConvertRowColToBin(iTrack, iC);
        if(mBtoT.iBin!=0)
        {
            bNeedTray=true;
        }
    }

    return bNeedTray;
}
//------------------------------------------------------------------------------
bool TLoaderModule::CheckAutoNeedTray(int &iX, int &iY)
{
    int iTrayIC[eTrayCount]={0};
    bool bNeedTray=false;
    MyBinToTrayStruct mBtoT;

    if(MotWorkLoader->fHasTray==true)
    {
        SortArmPara->Find_SortBinCount_Auto(&iTrayIC[0]);

        for(int iT=eAuto1; iT<=eAuto20;  iT++)
        {
            if(bNeedTray==false &&
               iTrayIC[iT]>0)
            {
                mBtoT.ConvertAutoToBin(iT);
                if(mBtoT.iBin!=0 &&
                   Auto->Mot_Auto_Work[mBtoT.iAutoRow][mBtoT.iAutoCol]->fHasTray==false)
                {
                    iX=mBtoT.iAutoRow;
                    iY=mBtoT.iAutoCol;
                    bNeedTray=true;
                }
            }
        }
    }

    if(bNeedTray==false &&
       MotLoader_Car->fHasTray &&
       MotLoader_Car->Tray.ClipID!="")
    {
        SortArmPara->Find_SortBinCount_Auto(&iTrayIC[0], true);
        for(int iT=eAuto1; iT<=eAuto20; iT++)
        {
            if(bNeedTray==false &&
               iTrayIC[iT]>0)
            {
                mBtoT.ConvertAutoToBin(iT);
                if(mBtoT.iBin!=0 &&
                   Auto->Mot_Auto_Work[mBtoT.iAutoRow][mBtoT.iAutoCol]->fHasTray==false)
                {
                    iX=mBtoT.iAutoRow;
                    iY=mBtoT.iAutoCol;
                    bNeedTray=true;
                }
            }
        }
    }

    if(bNeedTray==false)
    {
        for(int iT=eAuto20; iT>=eAuto1;  iT--)
        {
            mBtoT.ConvertAutoToBin(iT);
            if(mBtoT.iBin!=0 &&
               Auto->Mot_Auto_Work[mBtoT.iAutoRow][mBtoT.iAutoCol]->fHasTray==false)
            {
                iX=mBtoT.iAutoRow;
                iY=mBtoT.iAutoCol;
                return true;
            }
        }
    }
    return bNeedTray;
}
//------------------------------------------------------------------------------
bool TLoaderModule::bCheckStatusReady(int iTrack)
{
    if(HSys.FuncT.iT08_Empty1PushTray==0)
    {
        if((SnEmptyFCyPushTray[iTrack]->IsOn() &&
            SnEmptyRCyPushTray[iTrack]->IsOn()) ||
            HSys.LastSet.iRealDummy==DUMMY)
            return true;
    }
    else
    {
        if(iTrack==eTrack4 &&
          (SnEmptyFCyPushTray[iTrack]->IsOn() &&
           SnEmptyRCyPushTray[iTrack]->IsOn()))
            return true;
        else if(HSys.LastSet.iRealDummy==DUMMY)
            return true;
        else if(C_Empty_PushTray[iTrack]->Pop())
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoLoadEmptyTrayToTrackArea(bool bReset)                     //進入工作區域
{
    int &Task=iEmptyTrayToTrackAreaTask;
    bool bResult=false;
    static bool bNeedEmptyTray[eTrackCount]={false};
    static int iNowEmptyTrack=0;
    static int iNowAutoNeedTray=0;
    static int iNowHasTray=eTrackNotUse;
    static int iCWRetry=0;
    AnsiString asID;
    AnsiString asStr;
    int ret;
    static int iRetry=0;
    int iEmpty1=eEmpty1;
    MyBinToTrayStruct BtoT;

    static bool bXMove=false;
    static bool bZMove=false;
    static bool bCylin=false;

    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            for(int i=eTrack1; i<=eTrack4; i++)
            {
                bNeedEmptyTray[i]=false;
            }
            iNowEmptyTrack=eTrackNotUse;
            Task=100;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 100:
            if(CheckAutoNeedTray(iNowEmptyTrack, iNowAutoNeedTray))
            {
                Task=300;
                if(iNowEmptyTrack!=eTrackNotUse && MotTrack_Work[iNowEmptyTrack]->fHasTray==false)
                {
                    bNeedEmptyTray[iNowEmptyTrack]=true;
                    C_Empty_Stop[iNowEmptyTrack]->Reset();
                    Task=200;
                }
                break;
            }
            else
            {
                Task=100;
            }

            for(int i=eTrack4; i>eTrackNotUse; i--)
            {
                if(MotTrack_Work[i]->fHasTray==false)
                {
                    if(CheckEmpty1TrackNeedTray(i)==true)
                    {
                        iNowEmptyTrack=i;
                        bNeedEmptyTray[i]=true;
                        C_Empty_Stop[i]->Reset();
                        if(HSys.FuncT.iT08_Empty1PushTray==1)
                            C_Empty_PushTray[iNowEmptyTrack]->Reset();
                        Task=200;
                    }
                }
            }

            if(Task==100)
                bResult=true;
            break;
        case 200:
            if(bCheckStatusReady(iNowEmptyTrack))
            {
                C_Empty_Stop[iNowEmptyTrack]->On();

                iNowHasTray=eTrackNotUse;
                if(iNowEmptyTrack>=1)
                {
                    for(int i=iNowEmptyTrack-1; i>eTrackNotUse; i--)
                    {
                        if(bNeedEmptyTray[i]==true)
                        {
                            C_Empty_Stop[i]->Off();
                        }
                        else if(iNowHasTray==eTrackNotUse)
                        {
                            if(MotTrack_Work[i]->fHasTray==true)
                            {
                                iNowHasTray=i;
                                DownStopAtEmpty1(iNowHasTray,iNowEmptyTrack);   //Jimmychiu 20240920 : fixed for empty tray not arrival taget
                                if(HSys.FuncT.iT09_TrayIDReadPos==0)
                                {
                                    C_CCD_Stop[iEmpty1]->On();  //Sam 20240526 : Empty軌道上有 Tray 時 要把 C_CCD_Stop 打起來避免偷跑
                                }
                            }
                        }
                    }

                    if(iNowHasTray==eTrackNotUse)
                    {
                        C_CCD_Stop[iEmpty1]->Off();
                        for(int i=iNowEmptyTrack-1; i>eTrackNotUse; i--)
                        {
                            C_Empty_Stop[i]->Off();
                        }
                    }

                    if(HSys.FuncT.iT09_TrayIDReadPos==1)
                    {
                        C_Empty_Stop[eTrack1]->On();
                    }
                }
                else
                {
                    if(HSys.FuncT.iT09_TrayIDReadPos==0)
                    {
                        C_CCD_Stop[iEmpty1]->On();  //Sam 20240526 : Empty軌道上有 Tray 時 要把 C_CCD_Stop 打起來避免偷跑
                    }
                }
                Task=500;
                if(iNowHasTray==eTrackNotUse)
                {
                    if((HSys.FuncT.iT09_TrayIDReadPos==0 && MotEmpty1_Car->fHasTray==false) ||
                       (HSys.FuncT.iT09_TrayIDReadPos==1 && MotTrack_Work[eTrack1]->fHasTray==false))
                    {
                        bResult=true;
                    }
                }
            }
            break;
        case 300:
            if(iNowEmptyTrack==1 && MotTrack_Work[iNowEmptyTrack]->Tray.ClipID=="" && HSys.FuncT.iT09_TrayIDReadPos==1 && frm2DID->MyCCD[iEmpty1]->bEnableCCD)
            {
                Task=500;
                break;
            }

            if(HSys.FuncT.iT06_AutoRowType==0)
            {
                C_Auto_Stop[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                if(Auto->MTrackToStore[iNowEmptyTrack]->fHasTray==false)            //KenHsieh 20240829 : 避免Track 尚有滿Tray 軌道就開始做動
                    Task=2000;
            }
            else
            {
                if(Auto->MTrackToStore[iNowEmptyTrack]->fHasTray==false)
                {
                    C_AutoTrolly_Stop[iNowEmptyTrack]->Reset();
                    C_AutoTrolly_Fix[iNowEmptyTrack]->Reset();
                    bXMove=false;
                    bZMove=false;
                    Task=3000;
                }
            }
            break;
        case 500:
            if(frm2DID->MyCCD[iEmpty1]->bEnableCCD==true)
            {
                if(HSys.FuncT.iT09_TrayIDReadPos==1)
                    C_Empty_Stop[eTrack1]->Reset();
                else
                    C_CCD_Stop[iEmpty1]->Reset();
                Task=550;
                bCylin=false;
            }
            else
            {
                if(HSys.LastSet.iRealDummy!=REALLY)
                {
                    asID.sprintf("Tray%02d_%02d%02d%02d", iNowEmptyTrack, SystemHour, SystemMin, SystemSec);
                    MotEmpty1_Car->SetTrayID(asID);
                }
                Task=1000;
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(25);
            }
            break;
        case 550:
            if(bCylin==false)
            {
                if(HSys.FuncT.iT09_TrayIDReadPos==1)
                    bCylin=C_Empty_Stop[eTrack1]->Push();
                else
                    bCylin=C_CCD_Stop[iEmpty1]->Push();
            }                                                                   //Read Tray ID

            if(bCylin)
            {
                SwEmptyCW->On();
                Empty1MoveTimeOut.SetSecAndOn(3.0);                             //Sam 20240730 : 新增 Alarm Code
                bCylin=false;
                iRetry=0;
                Task=560;
            }
            break;
        case 560:
            if(SwEmptyCW->Status()==false)                                      //Sam 20240730 : 新增 Alarm Code
                Empty1MoveTimeOut.SetSecAndOn(5.0);
            SwEmptyCW->On();
            if((HSys.FuncT.iT09_TrayIDReadPos==0 && Sn_CCDPos[iEmpty1]->IsOff()==false)||
               (HSys.FuncT.iT09_TrayIDReadPos==1 && SnEmptyHasTray[eTrack1]->IsOff()==false))
            {
                Task=565;
            }
            else if(Empty1MoveTimeOut.Off())                                    //Sam 20240730 : 新增 Alarm Code
            {
                SwEmptyCW->Off();
                ShowSystemError("80110", K_RETRY);
                Empty1MoveTimeOut.SetSecAndOn(5.0);
            }
            break;
        case 565:
            sEmpty1ID="";
            frm2DID->StartCCDAuto(iEmpty1);
            Task=567;
//            break;
        case 567:
            SwEmptyCW->On();
            if(frm2DID->GetBarcode(iEmpty1)!="")
            {
                sEmpty1ID=frm2DID->GetBarcode(iEmpty1);
                Task=570;
            }
            else if(frm2DID->UpdateCCDAuto(iEmpty1))
            {
                if(frm2DID->MyCCD[iEmpty1]->bEnableCCD==true)                   //JerryYang 20240607 : modify
                {
                    sEmpty1ID=frm2DID->GetBarcode(iEmpty1);
                }
                Task=570;
            }
            break;
        case 570:
            SwEmptyCW->On();
            if(sEmpty1ID!="")
            {
                //JimmyChiu 20260416 : CCD abnormal response check (ERROR / non-numeric / unexpected string)
                if(sEmpty1ID.Pos("ERROR")>0 ||
                   sEmpty1ID.Pos("FAIL")>0  ||
                   sEmpty1ID.Pos("NG")>0    ||
                   sEmpty1ID.Pos("TIMEOUT")>0)
                {
                    asStr.sprintf("CCD abnormal response ClipID=%s", sEmpty1ID);
                    ret=ShowSystemError("80013", K_RETRY|K_SKIP, 0, asStr);
                    if(ret==K_RETRY)
                    {
                        iRetry=0;
                        Task=500;
                    }
                    else  //K_SKIP
                    {
                        if(HSys.FuncT.iT09_TrayIDReadPos==1)
                            MotTrack_Work[eTrack1]->ClearTray();
                        else
                            MotEmpty1_Car->ClearTray();
                        Task=575;
                    }
                    break;
                }
                else if(CheckClipIDCode(sEmpty1ID)==false)             	        //Sam 20250905 : 新增ClipID編碼檢查
                {
                    asStr.sprintf("ClipID=%s", sEmpty1ID);
                    ret=ShowSystemError("80012", K_RETRY|K_SKIP, 0, asStr);     //JimmyChiu 20260416 : add K_SKIP
                    if(ret==K_RETRY)
                    {
                        iRetry=0;
                        Task=500;
                    }
                    else  //K_SKIP
                    {
                        if(HSys.FuncT.iT09_TrayIDReadPos==1)
                            MotTrack_Work[eTrack1]->ClearTray();
                        else
                            MotEmpty1_Car->ClearTray();
                        Task=575;
                    }
                    break;
                }
                if(HSys.FuncT.iT09_TrayIDReadPos==1)
                {
                    MotTrack_Work[eTrack1]->SetTrayID(sEmpty1ID);
                    C_Empty_Stop[eTrack1]->Reset();
                }
                else
                {
                    MotEmpty1_Car->SetTrayID(sEmpty1ID);
                }
                Task=1000;
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(25);            //JerryYang 20240716 : add timeout
            }
            else if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                iRetry++;
                if(iRetry>3)
                {
                    ShowSystemError("80011", K_RETRY);
                    iRetry=0;
                    Task=500;
                }
                else
                {
                    Task=560;
                }
            }
            break;
        case 575:   //JimmyChiu 20260416 : After skip, confirm tray removed from CCD position
            if(HSys.FuncT.iT09_TrayIDReadPos==1)
            {
                if(SnEmptyHasTray[eTrack1]->IsOn())
                    ShowMyMessage("Please remove the tray at Empty1 CCD Position");
                else
                    Task=1;
            }
            else
            {
                if(Sn_CCDPos[iEmpty1]->IsOn())
                    ShowMyMessage("Please remove the tray at Empty1 CCD Position");
                else
                    Task=1;
            }
            break;
        case 1000:
            if(SwEmptyCW->Status()==false)
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(25);
            SwEmptyCW->On();
            if(HSys.FuncT.iT09_TrayIDReadPos==0)
            {
                if(iNowHasTray>0)                                                   //Sam 20240526 : Empty軌道上有 Tray 時 要把 C_CCD_Stop 打起來避免偷跑
                    C_CCD_Stop[iEmpty1]->On();
                else
                    C_CCD_Stop[iEmpty1]->Off();
            }
            else if(HSys.FuncT.iT09_TrayIDReadPos==1)
            {
                if(iNowEmptyTrack!=eTrack1)
                {
                    if(iNowHasTray>1)                                               //Sam 20240526 : Empty軌道上有 Tray 時 要把 C_CCD_Stop 打起來避免偷跑
                        C_Empty_Stop[eTrack1]->On();
                    else
                        C_Empty_Stop[eTrack1]->Off();
                }
                else
                {
                    Task=100;
                    break;
                }
            }

            if(iNowEmptyTrack==eTrack4 && bTransforMoving())
            {
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(25);
                break;
            }
            else if(SnEmptyHasTray[iNowEmptyTrack]->IsOn() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(5);
                Task=1100;
            }
            else if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())                  //JerryYang 20240716 : add timeout
            {                                                                   //Tray盤未到位
                ShowSystemError(SnEmptyHasTray[iNowEmptyTrack]->Name, K_RETRY); //"Empty1 Load Tray異常"
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(25);
            }
            break;
        case 1100:
            if(SwEmptyCW->Status()==false)
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(5);
            SwEmptyCW->On();
            if(SnEmptyHasTray[iNowEmptyTrack]->IsOn() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                Task=1200;
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetMSAndOn(500);            //JerryYang 20240527 : 增加Empty tray到位後delay,避免跳tray
            }
            else if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {                                                                   //Tray盤未到位
                ShowSystemError(SnEmptyHasTray[iNowEmptyTrack]->Name, K_RETRY); //"Empty1 Load Tray異常"
                Task=1000;
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(25);            //JerryYang 20240716 : add timeout
            }
            break;
        case 1200:
            SwEmptyCW->On();
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off()==false)
                break;
            SwEmptyCW->Off();
            bNeedEmptyTray[iNowEmptyTrack]=false;

            if(iNowHasTray==eTrackNotUse)
            {
                MotTrack_Work[iNowEmptyTrack]->MoveTrayFrom(MotEmpty1_Car);
            }
            else
            {
                MotTrack_Work[iNowEmptyTrack]->MoveTrayFrom(MotTrack_Work[iNowHasTray]);
            }
            Task=100;
            break;
        case 2000:
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(C_Auto_Stop[iNowEmptyTrack][iNowAutoNeedTray]->Push())
            {
                C_Empty_PushTray[iNowEmptyTrack]->Reset();
                Task=2010;
            }
            break;
        case 2010:
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(Auto->MTrackToStore[iNowEmptyTrack]->fHasTray==false)
            {
                SwEmptyRowCW[iNowEmptyTrack]->On();
                Task=2100;
            }
            break;
        case 2100:
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(C_Empty_PushTray[iNowEmptyTrack]->Push())
            {
                C_Auto_AntiDrop[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(8);
                Task=2200;
            }
            break;
        case 2200:
            if(SwEmptyRowCW[iNowEmptyTrack]->Status()==false)
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(8);
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(Sen_Auto_CarHasTray[iNowEmptyTrack][iNowAutoNeedTray]->IsOff()==false ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                iCWRetry=0;
                #ifndef SOFT_SIMULATE
                if(Sen_Auto_HasTray[iNowEmptyTrack][iNowAutoNeedTray]->IsOff()==false)      //JerryYang 20240615 : 新增保護避免Top已經有tray盤又打開夾爪
                {
                    BtoT.ConvertRowColToBin(iNowEmptyTrack, iNowAutoNeedTray);
                    asStr.sprintf("Please take off the tray of %d", BtoT.iWhichAuto);
                    ShowMyMessage(asStr);
                    break;
                }
                #endif
                C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->On();
                C_Auto_AntiDrop[iNowEmptyTrack][iNowAutoNeedTray]->On();
                C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetMSAndOn(1500);
                Task=2300;
            }
            else if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                if(iCWRetry>3)
                {
                    ShowSystemError(Sen_Auto_CarHasTray[iNowEmptyTrack][iNowAutoNeedTray]->Name, K_RETRY); //"Auto Tray Load Tray異常"
                    Task=2100;
                }
                else
                {
                    iCWRetry++;
                    SwEmptyRowCW[iNowEmptyTrack]->Off();
                    SwEmptyRowCCW[iNowEmptyTrack]->On();
                    Task=2250;
                    tEmpty1ToTrackDelay[iNowEmptyTrack].SetMSAndOn(1500);
                }
            }
            break;
        case 2250:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                SwEmptyRowCW[iNowEmptyTrack]->On();
                SwEmptyRowCCW[iNowEmptyTrack]->Off();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(8);
                Task=2200;
            }
            break;
        case 2300:
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                SwEmptyRowCW[iNowEmptyTrack]->Off();
                C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->On();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(0.5);           //JerryYang 20240607 : Auto區Up汽缸先ON/OFF各0.5秒洩氣減少暴衝
                Task=2305;
            }
            break;
        case 2305:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->Off();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(0.5);
                Task=2306;
            }
            break;
        case 2306:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->On();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(10);
                Task=2310;
            }
            break;
        case 2310:
            if(C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->IsOn() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                C_Empty_PushTray[iNowEmptyTrack]->Reset();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(1);
                Task=2330;                                                      //Delay Time
            }
            else if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                ShowSystemError(C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->ErrorName[eOnNotOnErr], K_RETRY);
                Task=2320;
            }
            break;
        case 2320:
            C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
            tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(1);
            Task=2300;
            break;
        case 2330:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                C_Empty_PushTray[iNowEmptyTrack]->Off();
                Task=2400;
            }
            break;
        case 2400:
            C_Auto_AntiDrop[iNowEmptyTrack][iNowAutoNeedTray]->Off();
            C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
            tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(1);
            Task=2410;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 2410:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->Off();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(10);
                Task=2500;
            }
            break;
        case 2500:
            C_Auto_Stop[iNowEmptyTrack][iNowAutoNeedTray]->Off();
            if(C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->IsOff() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                if(Sen_Auto_HasTray[iNowEmptyTrack][iNowAutoNeedTray]->IsOff()==false ||
                   HSys.LastSet.iRealDummy==DUMMY)
                {
                    C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->Off();
                    Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->MoveTrayFrom(MotTrack_Work[iNowEmptyTrack]);
                    Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->SetTrayInfo(iNowEmptyTrack, iNowAutoNeedTray);
                    if(HSys.FuncT.rgT03_AutoPickMode==0)
                    {
                        if(iNowEmptyTrack==eTrack1 && iNowAutoNeedTray==eTrackCol1) //JerryYang 20240527 : AUTO 1 設定半盤
                        {
                            for(int i=0; i<Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->Tray.XItem; i++)
                            {
                                for(int j=Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->Tray.YItem/2;
                                j<Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->Tray.YItem; j++)
                                {
                                    Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->SetTraySingleData(i, j, HAS_NULL_IC);
                                }
                            }
                        }
                    }
                    Task=100;
                }
                else
                {
                    ShowSystemError(Sen_Auto_HasTray[iNowEmptyTrack][iNowAutoNeedTray]->OffAlarmCode, K_RETRY);
                    Task=2400;
                }
            }
            else if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                ShowSystemError(C_Auto_Zup[iNowEmptyTrack][iNowAutoNeedTray]->ErrorName[eOffNotOffErr], K_RETRY);
                Task=2400;
            }
            break;
        case 3000:
            if(bZMove==false)
                bZMove=MCarrierStoreZ[iNowEmptyTrack]->MotorMove(iPos_Auto_ZDown[iNowEmptyTrack][iNowAutoNeedTray]);

            if(bZMove==true && bXMove==false)
            {
                #ifndef SOFT_SIMULATE
                MCarrierStoreZ[iNowEmptyTrack]->ScanMotorStatus();
                if(MCarrierStoreZ[iNowEmptyTrack]->Led[iHomeLed]==false)
                {
                    bZMove=false;
                    fHome->SingleMotorHome(true, MCarrierStoreZ[iNowEmptyTrack]->Tag);
                    Task=3005;
                    break;
                }
                else
                {
                    bXMove=MCarrierStoreX[iNowEmptyTrack]->MotorMove(iPos_Auto_X[iNowEmptyTrack][eTrackCol1]);   //接盤位置
                }
                #else
                    bXMove=MCarrierStoreX[iNowEmptyTrack]->MotorMove(iPos_Auto_X[iNowEmptyTrack][eTrackCol1]);   //接盤位置
                #endif
            }

            if(bXMove && bZMove && C_AutoTrolly_Stop[iNowEmptyTrack]->Push())
            {
                bXMove=false;
                bZMove=false;
                SwEmptyRowCW[iNowEmptyTrack]->On();
                C_Empty_PushTray[iNowEmptyTrack]->Reset();
                C_AutoTrolly_Fix[iNowEmptyTrack]->Reset();
                Task=3010;
            }
            break;
        case 3005:
            if(fHome->SingleMotorHome(false, MCarrierStoreZ[iNowEmptyTrack]->Tag))
            {
                bZMove=false;
                bXMove=false;
                Task=1000;
            }
            break;
        case 3010:
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(Auto->MTrackToStore[iNowEmptyTrack]->fHasTray==false)
            {
                if(C_AutoTrolly_Fix[iNowEmptyTrack]->Pop())
                {
                    SwEmptyRowCW[iNowEmptyTrack]->On();

                    if(iNowEmptyTrack==eTrack4)                                 //Empty4 stop設置在靠近尾端，進去Auto Row 後須將其設置下降
                    {
                        C_Empty_Stop[eTrack4]->Reset();
                        Task=3015;
                    }
                    else
                    {
                        Task=3100;
                    }
                }
            }
            break;
        case 3015:
            if(C_Empty_Stop[eTrack4]->Pop())
            {
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(0.5);
                Task=3020;
            }
//            C_Empty_Stop[eTrack4]->Off();
            break;
        case 3020:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
                Task=3100;
            break;
        case 3100:
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(C_Empty_PushTray[iNowEmptyTrack]->Push())
            {
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(8);
                Task=3200;
            }
            break;
        case 3200:
            if(SwEmptyRowCW[iNowEmptyTrack]->Status()==false)
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(8);
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(SnAutoTrollyHasTray[iNowEmptyTrack]->IsOff()==false)
            {
                iCWRetry=0;
                C_Empty_PushTray[iNowEmptyTrack]->Off();                        //Sam 20250704 : Fix C_Empty_PushTray 沒有收回來
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetMSAndOn(1500);
                C_AutoTrolly_Fix[iNowEmptyTrack]->Reset();
                Task=3300;
            }
            else if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                if(iCWRetry>3)
                {
                    ShowSystemError(SnAutoTrollyHasTray[iNowEmptyTrack]->Name, K_RETRY); //"Auto Tray Load Tray異常"
                    Task=3100;         //JerryYang 20250710 : fix
                }
                else
                {
                    iCWRetry++;
                    SwEmptyRowCW[iNowEmptyTrack]->Off();
                    SwEmptyRowCCW[iNowEmptyTrack]->On();
                    Task=3250;
                    tEmpty1ToTrackDelay[iNowEmptyTrack].SetMSAndOn(1500);
                }
            }
            break;
        case 3250:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                SwEmptyRowCW[iNowEmptyTrack]->On();
                SwEmptyRowCCW[iNowEmptyTrack]->Off();
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(8);
                Task=3200;
            }
            break;
        case 3300:
            SwEmptyRowCW[iNowEmptyTrack]->On();
            if(C_AutoTrolly_Fix[iNowEmptyTrack]->Push())
            {
                SwEmptyRowCW[iNowEmptyTrack]->Off();
                C_Auto_AntiDrop[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                bXMove=false;
                Task=3400;
            }
            break;
        case 3400:
            if(bXMove==false)
                bXMove=MCarrierStoreX[iNowEmptyTrack]->MotorMove(iPos_Auto_X[iNowEmptyTrack][iNowAutoNeedTray]);
            if(bXMove)
            {
                C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->On();
                C_Auto_AntiDrop[iNowEmptyTrack][iNowAutoNeedTray]->On();
                C_AutoTrolly_Fix[iNowEmptyTrack]->Reset();
                bCylin=false;
                bZMove=false;
                bXMove=false;
                if(HSys.FuncT.bA03_ChkTrollyHasTray &&
                   SnAutoTrollyHasTray[iNowEmptyTrack]->IsOff()==true)
                {
                    AnsiString Str=AnsiString().sprintf("Please check AutoTrolly %d X axis position, Has tray sensor is not ON.", iNowEmptyTrack);
                    ShowMyMessage(Str);
                }
                else
                {
                    Task=3500;
                }
            }
            break;
        case 3500:
            if(bCylin==false)
                bCylin=C_AutoTrolly_Fix[iNowEmptyTrack]->Pop();
            if(bCylin==true && bZMove==false)
                bZMove=MCarrierStoreZ[iNowEmptyTrack]->MotorMove(iPos_Auto_Z[iNowEmptyTrack][iNowAutoNeedTray]);

            if(bZMove)
            {
                bCylin=false;
                bZMove=false;
                tTrackToStoreDelay[iNowEmptyTrack][iNowAutoNeedTray].SetSecAndOn(iAutoZUpDelay);
                Task=3600;
            }
            break;
        case 3600:
            if(tTrackToStoreDelay[iNowEmptyTrack][iNowAutoNeedTray].Off())
            {
                C_Auto_AntiDrop[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                Task=3650;
            }
            break;
        case 3650:
            if(C_Auto_AntiDrop[iNowEmptyTrack][iNowAutoNeedTray]->Pop())
            {
                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(1);
                Task=3700;
            }
            break;
        case 3700:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                if(bZMove==false)
                    bZMove=MCarrierStoreZ[iNowEmptyTrack]->MotorMove(iPos_Auto_ZDown[iNowEmptyTrack][iNowAutoNeedTray]);

                if(bZMove)
                {
                    MCarrierStoreZ[iNowEmptyTrack]->ScanMotorStatus();
                    #ifndef SOFT_SIMULATE
                    if(MCarrierStoreZ[iNowEmptyTrack]->Led[iHomeLed]==false)
                    {
                        bZMove=false;
                        fHome->SingleMotorHome(true, MCarrierStoreZ[iNowEmptyTrack]->Tag);
                        Task=3705;
                        break;
                    }
                    else
                    {
                        bZMove=false;
                        C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                        Task=3710;
                    }
                    #else
                        bZMove=false;
                        C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->Reset();
                        Task=3710;
                    #endif
                }
            }
            break;
        case 3705:
            if(fHome->SingleMotorHome(false, MCarrierStoreZ[iNowEmptyTrack]->Tag))
            {
                bZMove=false;
                Task=3700;
            }
            break;
        case 3710:                                                              //斜推需先固定Tray，HasTray Sensor 才能判斷
            C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->Off();
            tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(1);
            Task=3715;
            break;
        case 3715:
            if(tEmpty1ToTrackDelay[iNowEmptyTrack].Off())
            {
                if(iNowEmptyTrack==eTrack4)                                     //Empty4 stop設置在靠近尾端，進去Auto Row 後須將其設置下降
                    C_Empty_Stop[eTrack4]->Reset();
                Task=3800;
            }
            break;
        case 3800:
            if(Sen_Auto_HasTray[iNowEmptyTrack][iNowAutoNeedTray]->IsOff()==false ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                if(iNowEmptyTrack==eTrack4)                                     //Empty4 stop設置在靠近尾端，進去Auto Row 後須將其設置下降
                    C_Empty_Stop[eTrack4]->On();
                C_Auto_InclinePush[iNowEmptyTrack][iNowAutoNeedTray]->Off();
                Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->MoveTrayFrom(MotTrack_Work[iNowEmptyTrack]);
                Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->SetTrayInfo(iNowEmptyTrack, iNowAutoNeedTray);
                if(HSys.FuncT.rgT03_AutoPickMode==0)
                {
                    if(iNowEmptyTrack==eTrack1 && iNowAutoNeedTray==eTrackCol1) //JerryYang 20240527 : AUTO 1 設定半盤
                    {
                        for(int i=0; i<Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->Tray.XItem; i++)
                        {
                            for(int j=Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->Tray.YItem/2;
                            j<Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->Tray.YItem; j++)
                            {
                                Auto->Mot_Auto_Work[iNowEmptyTrack][iNowAutoNeedTray]->SetTraySingleData(i, j, HAS_NULL_IC);
                            }
                        }
                    }
                }
                Task=100;
            }
            else
            {
                ShowSystemError(Sen_Auto_HasTray[iNowEmptyTrack][iNowAutoNeedTray]->OffAlarmCode, K_RETRY);
                Task=3600;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TLoaderModule::DoEmptyTrayToStoreArea(bool bReset)                         //Empty tray to store area
{
    int &Task=iEmptyTrayToStoreAreaTask;
    bool bResult=false,bflag=false;
    if(bReset)
    {
        Task=1;
        return bResult;
    }
    switch(Task)
    {
        case 1:
            bflag=false;
            for(int i=eTrack1; i<=eTrack4; i++)
            {
                if(MotTrack_Work[i]->fHasTray==true)
                {
                    iNeedOutEmptyTrack=i;
                    if(HSys.FuncT.iT06_AutoRowType==0)
                    {
                        DoEmptyTrayToStoreArea(true,iNeedOutEmptyTrack);
                    }
                    else
                    {
                        DoEmptyTrayToStoreAreaByServoMot(true,iNeedOutEmptyTrack);
                    }
                    bflag=true;
                    break;
                }
            }
            if(bflag)
            {
                Task=100;
            }
            else
            {
                Task=9900;
            }
            break;
        case 100:
            if(HSys.FuncT.iT06_AutoRowType==0)
            {
                bflag=DoEmptyTrayToStoreArea(false,iNeedOutEmptyTrack);
            }
            else
            {
                bflag=DoEmptyTrayToStoreAreaByServoMot(false,iNeedOutEmptyTrack);
            }
            if(bflag)
            {
                Task=1;
            }
            break;
        case 9900:
            bResult=true;
            break;
    }
    return bResult;

}
//------------------------------------------------------------------------------
bool TLoaderModule::DoEmptyTrayToStoreArea(bool bReset, int iRowTrack)
{
    int &Task=iEmpTrayToStoreTask[iRowTrack];
    bool bResult=false;
    if(bReset)
    {
        Task=1;
        return bResult;
    }
    switch(Task)
    {
        case 1:
            SwEmptyRowCCW[iRowTrack]->Off();
            SwEmptyRowCW[iRowTrack]->On();
            C_AutoRow_Stop[iRowTrack]->Reset();
            Task=100;
            break;
        case 100:
            SwEmptyRowCW[iRowTrack]->On();
            if(C_AutoRow_Stop[iRowTrack]->Push())
            {
                C_Empty_PushTray[iRowTrack]->Reset();
                Task=200;
            }
            break;
        case 200:
            SwEmptyRowCW[iRowTrack]->On();
            if(C_Empty_PushTray[iRowTrack]->Push())
            {
                C_Empty_PushTray[iRowTrack]->Off();
                tEmpty1ToTrackDelay[iRowTrack].SetSecAndOn(15);
                Task=300;
            }
            break;
        case 300:
            SwEmptyRowCW[iRowTrack]->On();
            if(SnAutoRowHasTray[iRowTrack]->IsOff()==false ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                tEmpty1ToTrackDelay[iRowTrack].SetSecAndOn(1.5);              //KenHsieh 20240830 : 0.5 -> 1.5 延長滾動時間
                Task=500;
            }
            else if(tEmpty1ToTrackDelay[iRowTrack].Off())
            {
//                ShowSystemError(SnAutoRowHasTray[iTrack]->Name, K_RETRY);       //"Track%d to Tray Arm Area Time Out"
                SwEmptyRowCW[iRowTrack]->Off();
                SwEmptyRowCCW[iRowTrack]->On();
                tEmpty1ToTrackDelay[iRowTrack].SetSecAndOn(2);
                Task=400;
            }
            break;
        case 400:
            SwEmptyRowCCW[iRowTrack]->On();
            if(tEmpty1ToTrackDelay[iRowTrack].Off())
            {
                SwEmptyRowCCW[iRowTrack]->Off();
                SwEmptyRowCW[iRowTrack]->On();
                tEmpty1ToTrackDelay[iRowTrack].SetSecAndOn(600);
                Task=500;
            }
            break;
        case 500:
            SwEmptyRowCW[iRowTrack]->On();
            if(SnAutoRowHasTray[iRowTrack]->IsOff()==false ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                tEmpty1ToTrackDelay[iRowTrack].SetSecAndOn(1.5);                //KenHsieh 20240830 : 0.5 -> 1.5 延長滾動時間
                Task=600;
            }
            else if(tEmpty1ToTrackDelay[iRowTrack].Off())
            {
                ShowSystemError(SnAutoRowHasTray[iRowTrack]->Name, K_RETRY);    //"Track%d to Tray Arm Area Time Out"
                Task=400;
            }
            break;
        case 600:
            SwAutoRowDete[iRowTrack]->On();
            SwEmptyRowCW[iRowTrack]->On();
            if(tEmpty1ToTrackDelay[iRowTrack].Off())
            {
                SwEmptyRowCW[iRowTrack]->Off();
                SwAutoRowDete[iRowTrack]->Off();
                Auto->MTrackToStore[iRowTrack]->MoveTrayFrom(MotTrack_Work[iRowTrack]);
                Auto->MTrackToStore[iRowTrack]->Tray.iTrackCol=eTrackCol1;
                Auto->MTrackToStore[iRowTrack]->Tray.iTrackRow=iRowTrack;
                Task=9900;
            }
            break;
        case 9900:
            bResult=true;
            break;
    }
    return bResult;

}
//------------------------------------------------------------------------------
bool TLoaderModule::DoEmptyTrayToStoreAreaByServoMot(bool bReset, int iRowTrack)
{
    int &Task=iEmpTrayToStoreTask[iRowTrack];
    bool bResult=false;
    if(bReset)
    {
        Task=1;
        return bResult;
    }
    switch(Task)
    {
        case 1:
            if(MCarrierStoreZ[iRowTrack]->MotorMove(iPos_Auto_ZDown[iRowTrack][eTrackCol1]))
            {
                if(iRowTrack==eTrack4)
                {
                    C_Empty_Stop[eTrack4]->Off();
                }
                Task=100;
            }
            break;
        case 100:
            if(MCarrierStoreX[iRowTrack]->MotorMove(iPos_Auto_X[iRowTrack][eTrackCol1]))
            {
                C_AutoTrolly_Stop[iRowTrack]->Reset();
                Task=200;
            }
            break;
        case 200:
            if(C_AutoTrolly_Stop[iRowTrack]->Push())
            {
                SwEmptyRowCCW[iRowTrack]->Off();
                SwEmptyRowCW[iRowTrack]->On();
                C_AutoRow_Stop[iRowTrack]->Reset();
                Task=300;
            }
            break;
        case 300:
            if(C_AutoRow_Stop[iRowTrack]->Push())
            {
                C_AutoTrolly_Fix[iRowTrack]->Reset();
                Task=400;
            }
            break;
        case 400:
            if(C_AutoTrolly_Fix[iRowTrack]->Pop())
            {
                SwEmptyRowCW[iRowTrack]->On();
                C_Empty_PushTray[iRowTrack]->Reset();
                Task=500;
            }
            break;
        case 500:
            SwEmptyRowCW[iRowTrack]->On();
            if(C_Empty_PushTray[iRowTrack]->Push())
            {
                tEmpty1ToTrackDelay[iRowTrack].SetSecAndOn(8);
                Task=600;
            }
            break;
        case 600:
            SwEmptyRowCW[iRowTrack]->On();
            if(SnAutoTrollyHasTray[iRowTrack]->IsOff()==false)
            {
                C_Empty_PushTray[iRowTrack]->Off();                             //Sam 20250704 : Fix C_Empty_PushTray 沒有收回來
                C_AutoTrolly_Fix[iRowTrack]->Reset();
                Task=700;
            }
            else if(tEmpty1ToTrackDelay[iRowTrack].Off())
            {
                ShowSystemError(SnAutoTrollyHasTray[iRowTrack]->Name, K_RETRY); //"Auto Tray Load Tray異常"
            }
            break;
        case 700:
            SwEmptyRowCW[iRowTrack]->On();
            if(C_AutoTrolly_Fix[iRowTrack]->Push())
            {
                SwEmptyRowCW[iRowTrack]->Off();
                Task=800;
            }
            break;
        case 800:
            if(MCarrierStoreX[iRowTrack]->MotorMove(iPos_AutoXPush[iRowTrack]-5000))
            {
                MCarrierStoreX[iRowTrack]->SetPersentSpeed(5, false);
                Task=900;
            }
            break;
        case 900:
            if(MCarrierStoreX[iRowTrack]->MotorMove(iPos_AutoXPush[iRowTrack]))
            {
                MCarrierStoreX[iRowTrack]->SetPersentSpeed(MCarrierStoreX[iRowTrack]->GetPersentSpeed());
                C_AutoTrolly_Fix[iRowTrack]->Reset();
                Task=1000;
            }
            break;
        case 1000:
            if(C_AutoTrolly_Fix[iRowTrack]->Pop())
            {
                C_AutoTrolly_Stop[iRowTrack]->Reset();
                Task=1100;
            }
            break;
        case 1100:
            if(C_AutoTrolly_Stop[iRowTrack]->Pop())
            {
                Auto->MTrackToStore[iRowTrack]->MoveTrayFrom(MotTrack_Work[iRowTrack]);
                Auto->MTrackToStore[iRowTrack]->Tray.iTrackCol=eTrackCol1;
                Auto->MTrackToStore[iRowTrack]->Tray.iTrackRow=iRowTrack;
                Task=9900;
            }
            break;
        case 9900:
            bResult=true;
            break;
    }
    return bResult;

}
//------------------------------------------------------------------------------
bool TLoaderModule::GetEmpty1HasTray(int iAuto)
{
    bool bret=false;
    if(iAuto==-1)
    {
        for(int i=eTrack1; i<=eTrack4; i++)
        {
            if(MotTrack_Work[i]->fHasTray==true)
            {
                bret=true;
                break;
            }
        }
    }
    else if(iAuto>=eTrack1 && iAuto<=eTrack4)
    {
        bret=MotTrack_Work[iAuto]->fHasTray;
    }
    else
    {
        bret=false;
    }
    return bret;
}
//------------------------------------------------------------------------------
int TLoaderModule::SetTrayMapByMapList(AnsiString asID)
{
    int iRet=SUMMARY_SUCCESS;
    if(asID=="")
        return TRAY_ID_IS_NULL;

    if(mapTrayMapList.size()==0)
    {
        if(tSimuData.bRunSimulation)
        {
            DecodeTrayMapBySimulation();
        }
        else if(HSys.FuncT.iA01_InputDataFormat==eEKeeperFormat)
        {
            iRet=DecodeTrayMapByEKeeperFormat(SummaryFileName);
        }
        else if(HSys.FuncT.iA01_InputDataFormat==eJHTFormat ||
                HSys.FuncT.iA01_InputDataFormat==ePTIFormat)
        {
            iRet=DecodeTrayMapByJHTFormat(SummaryFileName);                     //要改成Lot Start
        }
        else if(HSys.FuncT.iA01_InputDataFormat==eWinstekFormat)
        {
            iRet=SUMMARY_FILE_NOT_EXIST;                     //要改成Lot Start
            ShowMyMessage("Tray Map List is empty! Check SECS Gem command!");
        }
        if(iRet!=SUMMARY_SUCCESS)
            return iRet;
    }
    int iBin;
    MyBinToTrayStruct mBtoT;
    IterTrayMapList=mapTrayMapList.find(asID);
    if(IterTrayMapList!=mapTrayMapList.end())
    {
        TTrayMap myTraymap=IterTrayMapList->second;
        MotLoader_Car->Tray.CopyFromTrayMap(myTraymap);
        for(int iR=0; iR<MotLoader_Car->Tray.YItem; iR++)
        {
            for(int iC=0; iC<MotLoader_Car->Tray.XItem; iC++)
            {
                if(iC>=IterTrayMapList->second.iDevMaxCol ||
                   iR>=IterTrayMapList->second.iDevMaxRow)
                {
                    iBin=0;
                }
                else
                {
                    iBin=IterTrayMapList->second.DevInfo[iR][iC].iBin;
                }
                if(iBin!=0)                                                     //KenHsieh 20240514 : 新增!=0才寫入data
                {
                    mBtoT.ConvertBinToAuto(iBin);
                    MotLoader_Car->SetTraySingleData(iC, iR, HAS_IC, IterTrayMapList->second.DevInfo[iR][iC]);
                    SortArmPara->iInputIC_LoadCar[mBtoT.iWhichAuto]++;
                }
                else
                {
                    MotLoader_Car->SetTraySingleData(iC, iR, NULL_IC);
                }
            }
        }
        return SUMMARY_SUCCESS;
    }
    else
    {
        return TRAY_MAP_FILE_IS_NULL;
    }
}
//------------------------------------------------------------------------------
int TLoaderModule::LoadClipIDMapping(AnsiString asID)
{
    int iRet=0;
    for(int i=0; i<eTrayCount; i++)
        SortArmPara->iInputIC_LoadCar[i]=0;

    MotLoader_Car->SetTrayXYItem(iXDivision, iYDivision);
    MotLoader_Car->InitNewTray(NULL_IC);

    if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eEKeeperFormat)
    {
        AnsiString asSchedule   =eKeeperSchedule();
        fMain->edLotNo->Text    =asSchedule;
    }
    iRet=SetTrayMapByMapList(asID);
    fMain->CleanBinCount();
    AnsiString sMsg="";
    AnsiString sTotal="";
    if(CosFunction.bUseBinAutoConvert)
    {
        int iBin=0;
        for(int iauto=eAuto1; iauto<eTrayCount; iauto++)
        {
            if(SortArmPara->iInputIC_LoadCar[iauto]>0)
            {
                iBin=cBinAutoTrans.GetBinbyAuto(iauto);
                sMsg.sprintf("Bin %d : %d", iBin, SortArmPara->iInputIC_LoadCar[iauto]);
                if(CUSTOMER_CODE==CC_WINSTEK)
                {}
                else
                {
                    fMain->ShowLoadCarBinCount(sMsg);
                }
                if(sTotal=="")
                    sTotal=asID;
                sTotal+=" ";
                sTotal+=sMsg;
            }
        }
    }
    else
    {
        MyBinToTrayStruct mBtoT;
        for(int iB=0; iB<TEST_MAX_BIN; iB++)
        {
            mBtoT.ConvertBinToAuto(iB);
            if(SortArmPara->iInputIC_LoadCar[mBtoT.iWhichAuto]!=0)
            {
                sMsg.sprintf("Bin %d : %d", iB, SortArmPara->iInputIC_LoadCar[mBtoT.iWhichAuto]);
                if(CUSTOMER_CODE==CC_WINSTEK)
                {}
                else
                {
                    fMain->ShowLoadCarBinCount(sMsg);
                }

                if(sTotal=="")
                    sTotal=asID;

                sTotal+=" ";
                sTotal+=sMsg;
            }
        }
    }
    if(sTotal!="")
        RecordProcess(sTotal);

    MotLoader_Car->SetTrayID(asID);                                             //有ID表示做完了
    return iRet;
}
//------------------------------------------------------------------------------
int TLoaderModule::CheckDuplicateLotID(AnsiString sFile,AnsiString asID)
{
    if(CUSTOMER_CODE!=CC_PTI)
    {
        return SUMMARY_SUCCESS;
    }
    AnsiString Str="";
    TStringList *SList;
    bool bHasFile=false, bDuplicate=false;
    SList=new TStringList();
    SList->Clear();

    bHasFile=FileExists(sFile);
    if(bHasFile==false)
    {
        SList->Add(asID);
        SList->SaveToFile(sFile);
    }
    else
    {
        SList->LoadFromFile(sFile);
        bDuplicate=false;
        for(int i=0; i<SList->Count; i++)
        {
            Str=SList->Strings[i];
            if(Str==asID)
                bDuplicate=true;
        }

        if(bDuplicate)
        {
            return TRAY_ID_DUPLICATE;                                           //KenHsieh 20250806 : 修改重複碼報警
        }
        else
        {
            SList->Add(asID);
            SList->SaveToFile(sFile);
        }
    }

    SList->Clear();
    delete SList;

    return SUMMARY_SUCCESS;
}
//------------------------------------------------------------------------------
AnsiString TLoaderModule::eKeeperSchedule()
{
    AnsiString sFile="D:\\BarcodeSorter\\batch.txt";
    AnsiString asSchedule="";
    if(FileExists(sFile)==false)
    {
        asSchedule="NA";
        return asSchedule;
    }
    asSchedule=ReadIniData(sFile, "Data", "Schedule", AnsiString("NA"));
    return asSchedule;
}
//------------------------------------------------------------------------------
void TLoaderModule::SetStatus(int is, AnsiString sFunc)
{
    iStatus=is;
    sTemp=AnsiString().sprintf("%s %s", sFunc, GetStatusStr());
    RecordProcess(sTemp);
}
//------------------------------------------------------------------------------
int TLoaderModule::GetStatus()
{
    return iStatus;
}
//------------------------------------------------------------------------------
AnsiString TLoaderModule::GetStatusStr()
{
    sTemp="None";
    switch(iStatus)
    {
        case (int)eHS_Busy:
            sTemp="Busy";
        case (int)eHS_WorkAreaReady:
            sTemp="WorkAreaReady";
    }
    return sTemp;
}
//------------------------------------------------------------------------------
int TAutoModule::TrackHasTray()
{
    int iTrack=eTrackNotUse;

    for(int iT=eTrack1; iT<=eTrack4; iT++)
    {
        if(iTrack==eTrackNotUse && MTrackToStore[iT]->fHasTray)
        {
            iTrack=iT;
        }
    }

    return iTrack;
}
//------------------------------------------------------------------------------
bool TLoaderModule::AutoCylinderUp(bool bReset, int iAuto)
{
    if(bReset)
    {
        iCynUpTask[iAuto]=1;
        AutoCylinderMiddle(bReset, iAuto);
        return false;
    }
    int &Task=iCynUpTask[iAuto];

    switch(Task)
    {
        case 1:
            {
                #ifndef SOFT_SIMULATE                                           //Sam 20240515 : 修正 Dummy Run
                bool b=Sn_CarHasTray[iAuto]->IsOn();
                if(b)
                {
                    ShowSystemError(Sn_CarHasTray[iAuto]->OnAlarmCode, K_RETRY);    //"check tray in the %s car"
                    break;
                }
                #endif
                Task=30;
            }
            break;
        case 30:
            C_Up[iAuto]->On();
            C_Middle[iAuto]->On();
            CynUpDelay[iAuto].SetSecAndOn(1);
            Task=50;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 50:
            if(CynUpDelay[iAuto].Off())
            {
                CynUpDelay[iAuto].SetMSAndOn(C_Up[iAuto]->OnAlarmTime);         //Sam 20240810 : 修正 TimeOut 問題
                Task=100;
            }
            break;
        case 70:
            if(C_Separatory[iAuto]->Pop())
            {
                TrayToCarDelay[iAuto].SetSecAndOn(3);
                Task=80;
            }
            break;
        case 80:
            if(TrayToCarDelay[iAuto].Off())
            {
                C_Up[iAuto]->On();
                Task=100;
            }
            break;
        case 100:
            #ifdef SOFT_SIMULATE
            {
                Task=1;
                return true;
            }
            #else
            {
                if(C_Up[iAuto]->IsOn()==true       &&
                   C_Middle[iAuto]->IsOn()==false  &&
                   C_Middle[iAuto]->IsOff()==false)
                {
                    CynUpDelay[iAuto].SetMSAndOn(C_Up[iAuto]->OnDelayTime);
                    Task=200;
                }
                else if(CynUpDelay[iAuto].Off())
                {
                    ShowSystemError(C_Up[iAuto]->ErrorName[eOnNotOnErr], K_RETRY);
                    Task=1;
                }
                break;
            }
            #endif
        case 200:
            if(CynUpDelay[iAuto].Off())
                Task=201;
            break;
        case 201:
            if(C_Up[iAuto]->IsOn()==false)
            {
                Task=1;
                return false;
            }
            else if(CynUpDelay[iAuto].Off())
            {
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TLoaderModule::AutoCylinderMiddle(bool bReset, int iAuto)
{
    if(bReset)
    {
        iCynMidTask[iAuto]=1;
        return false;
    }
    int &Task=iCynMidTask[iAuto];

    switch(Task)
    {
        case 1:
            C_Up[iAuto]->On();
            C_Middle[iAuto]->Off();                                             //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
            CynMidDelay[iAuto].SetMSAndOn(C_Up[iAuto]->OnAlarmTime);            //Sam 20240810 : 修正 TimeOut 問題
            Task=100;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 100:
            #ifdef SOFT_SIMULATE
            {
                Task=1;
                return true;
            }
            #else
            {
                if(C_Up[iAuto]->IsOn()==false && C_Middle[iAuto]->IsOn())
                {
                    CynMidDelay[iAuto].SetSecAndOn(dLoadMidd);
                    Task=200;
                }
                else if(CynMidDelay[iAuto].Off())
                {
                    ShowSystemError(C_Middle[iAuto]->ErrorName[eOnNotOnErr], K_RETRY);
                    Task=1;
                }
                break;
            }
            #endif
         case 200:
            if(CynMidDelay[iAuto].Off())
                Task++;
            break;
         case 201:
            if(!((C_Up[iAuto]->IsOn()==false) && C_Middle[iAuto]->IsOn()))
            {
                Task=1;
                return false;
            }
            else if(CynMidDelay[iAuto].Off())
            {
                Task=1;
                return true;
            }
            break;

    }
    return false;
}
//------------------------------------------------------------------------------
//kevin 20120718 修改輸入氣缸因代號位置不同共用程式
//------------------------------------------------------------------------------
bool TLoaderModule::AutoCylinderLower(bool bReset, int iAuto)
{
    AnsiString Str;

    if(bReset)
    {
        iCynLowTask[iAuto]=1;
        return false;
    }
    int &Task=iCynLowTask[iAuto];

    switch(Task)
    {
        case 1:
            C_Up[iAuto]->Off();
            C_Middle[iAuto]->Off();                                             //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
            CynMidDelay[iAuto].SetMSAndOn(C_Up[iAuto]->OffAlarmTime);           //Sam 20240810 : 修正 TimeOut 問題
            Task=100;
            if(HSys.LastSet.iRealDummy==DUMMY)
                break;
        case 100:
            #ifdef SOFT_SIMULATE
            {
                Task=1;
                return true;
            }
            #else
            {
                if(C_Middle[iAuto]->IsOff())
                {
                    CynMidDelay[iAuto].SetSecAndOn(dLoadDown);
                    Task=200;
                }
                else if(CynMidDelay[iAuto].Off())
                {
                    ShowSystemError(C_Middle[iAuto]->ErrorName[eOffNotOffErr], K_RETRY);
                    Task=1;
                }
                break;
            }
            #endif
        case 200:
            if(CynMidDelay[iAuto].Off())
                Task=201;
            break;
         case 201:
            if(!C_Middle[iAuto]->IsOff())
            {
                Task=1;
                return false;
            }
            else if(CynMidDelay[iAuto].Off())
            {
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TLoaderModule::LoaderCylinderPreOn(bool bReset, int iAuto)                   //Sam 20240810 : Loader Cylinder PreOn
{
    int &Task=iLoaderPreOnTask[iAuto];
    bool bResult=false;

    if(bReset)
    {
        Task=1;
        iLoaderPreOnCount[iAuto]=0;
        return bResult;
    }
    switch(Task)
    {
        case 1:
            C_Fixer[iAuto]->Off();
            C_EdgePush[iAuto]->Off();
            iLoaderPreOnCount[iAuto]=0;
            Task=100;
            break;
        case 100:
            iLoaderPreOnCount[iAuto]++;
            C_Fixer[iAuto]->On();
            C_EdgePush[iAuto]->On();
            LoaderPreOnDelay[iAuto].SetSecAndOn(1.0);
            Task=200;
            break;
        case 200:
            if(LoaderPreOnDelay[iAuto].Off())
            {
                C_Fixer[iAuto]->Off();
                C_EdgePush[iAuto]->Off();
                LoaderPreOnDelay[iAuto].SetSecAndOn(0.5);
                Task=300;
            }
            break;
        case 300:
            if(LoaderPreOnDelay[iAuto].Off())
            {
                if(iLoaderPreOnCount[iAuto]>=1)
                    bResult=true;
                else
                    Task=100;
            }
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
MainChannel::MainChannel()
{
    iStackStatus=eHS_StackArea_Idle;
    iCCDStatus=eHS_CCD_Idle;
}
//------------------------------------------------------------------------------
MainChannel::~MainChannel()
{
}
//------------------------------------------------------------------------------
LoaderChannel::LoaderChannel()
{
}
//------------------------------------------------------------------------------
LoaderChannel::~LoaderChannel()
{
}
//------------------------------------------------------------------------------
Empty1Channel::Empty1Channel()
{
}
//------------------------------------------------------------------------------
Empty1Channel::~Empty1Channel()
{
}
//------------------------------------------------------------------------------
Empty2Channel::Empty2Channel()
{
}
//------------------------------------------------------------------------------
Empty2Channel::~Empty2Channel()
{
}
//------------------------------------------------------------------------------
TransforChannel::TransforChannel()
{
}
//------------------------------------------------------------------------------
TransforChannel::~TransforChannel()
{
}
//------------------------------------------------------------------------------
//TCCD::TCCD(AnsiString sname,bool bcanmove)
//{
//    bCanMove=bcanmove;
//    iDoTakePhotoTask=0;
//    sName=sname;
//    sClipID="";
//}
////------------------------------------------------------------------------------
//TCCD::~TCCD()
//{
//}
////------------------------------------------------------------------------------
//void TCCD::SetCCDArm(TTrayMotor *ccdarmX)
//{
//    CCDArmX=ccdarmX;
//}
////------------------------------------------------------------------------------
//void TCCD::SetMainCh(MainChannel *mch)
//{
//    mainch=mch;
//}
////------------------------------------------------------------------------------
//void TCCD::SetCCD(TMyCCDIDPanel *ccd)
//{
//    MyCCD=ccd;
//}
////------------------------------------------------------------------------------
//void TCCD::DoAllAction(int &Task)
//{
//    switch(Task)
//    {
//        case 1:
//            iStatus=eHS_Idle;
//            if(mainch->iCCDStatus==mainch->eHS_CCD_NeedPhoto)
//            {
//                DoTakePhoto(true);
//                Task=100;
//            }
//            else //Idle
//            {
//            }
//            break;
//        case 100: //eHS_StackArea_NeedPhoto
//            if(DoTakePhoto(false))
//            {
//                iStatus=eHS_Ready;
//                if(mainch->iCCDStatus!=mainch->eHS_CCD_NeedPhoto)
//                {
//                    if(bCanMove)
//                        Task=200;
//                    else
//                        Task=1;
//                }
//            }
//            break;
//        case 200:
//            iStatus=eHS_Idle;
//            if(bCanMove)
//            {
//                if(CCDArmX->MotorMove(-10))                                     //Sam 20240710 : CCD 拍完照要回到 Home 點位置
//                {
//                    iStatus=eHS_Idle;
//                    Task=1;
//                }
//            }
//            break;
//    }
//}
////------------------------------------------------------------------------------
//bool TCCD::DoTakePhoto(bool bReset)
//{
//    int &Task=iDoTakePhotoTask;
//    bool bResult=false;
//    if(bReset)
//    {
//        Task=1;
//        return bResult;
//    }
//    switch(Task)
//    {
//        case 1:
//            sClipID="";
//            if(bCanMove)                                                        //Sam 20240710 : CCD 拍完照要回到 Home 點位置
//            {
//                if(CCDArmX->MotorMove(iMin))
//                    Task=25;
//            }
//            else
//            {
//                Task=25;
//            }
//            break;
//        case 25:
//            if(HSys.LastSet.iRealDummy>=HAS_TRAY)                               //Sam 20240515 : 修正 Dummy Run
//            {
//                if(MyCCD->bEnableCCD==true)
//                    MyCCD->SendCCDCommand("", "LON");  //JerryYang 20240802 : modify
//            }
//            Task=50;
//            break;
//        case 50:
//            if(bCanMove)
//            {
//                if(sClipID!="")
//                {
//                    Task=9999;
//                }
//                else if(CCDArmX->MotorMove(iMin))
//                {
//                    Task=100;
//                }
//            }
//            else
//            {
//                Task=200;
//            }
//            break;
//        case 100:
//            if(bCanMove)
//            {
//                if(sClipID!="")
//                {
//                    Task=9999;
//                }
//                else if(CCDArmX->MotorMove(iMax))
//                {
//                    Task=200;
//
//                }
//            }
//            else
//            {
//                Task=200;
//            }
//            break;
//        case 200:    //eHS_StackArea_NeedPhoto
//            TrayToCarDelay.SetSecAndOn(3);
//            Task=201;
////            break;
//        case 201:
//            if(HSys.LastSet.iRealDummy==DUMMY)                                  //JerryYang 20240607 : modify   //Sam 20240515 : 修正 Dummy Run
//            {
//                Task=9999;
//            }
//            else if(sClipID!="")
//            {
//                Task=9999;
//            }
//            else if(TrayToCarDelay.Off())
//            {
//                Task=9999;
//            }
//            else
//            {}//wait
//            break;
//        case 9999://finish
//            MyCCD->SendCCDCommand("", "LOFF");                                  //JerryYang 20240802 : modify
//            bResult=true;
//            break;
//    }
//    return bResult;
//}
//------------------------------------------------------------------------------
bool TLoaderModule::DoLoadEmpty2Mag(bool bReset)
{
    bool bResult=false;
    int &Task=iLoadEmpty2MagTask;
    if(bReset)
    {
        Task=1;
        return bResult;
    }
    int iEmpty2=(int)eEmpty2;
    AnsiString asStr="";
    static int iRetry=0;
    switch(Task)
    {
        case 1:
            if(MotEmpty2_Car->fHasTray)
            {
                C_CCD_Stop[iEmpty2]->Off();
                C_Empty2_EmptyStop->Reset();
                iRetry=0;
                Task=100;
            }
            break;
        case 100:                                                               //Read Tray ID
            if(frm2DID->MyCCD[iEmpty2]->bEnableCCD==true && HSys.LastSet.iRealDummy!=DUMMY)
            {
                if(HSys.FuncT.iT09_TrayIDReadPos==1)
                    Task=105;
                else
                {
                    C_CCD_Stop[iEmpty2]->On();
                    SwMagCW->On();
                    Task=109;
                }
            }
            else
            {
                if(HSys.LastSet.iRealDummy!=REALLY)
                {
                    if(MotEmpty2_Car->Tray.ClipID=="")
                        sEmpty2ID=fMain->edtEmpty2ID->Text;
                    else
                        sEmpty2ID=MotEmpty2_Car->Tray.ClipID;
                    Task=110;
                }
                else
                {
                    Task=200;
                }
            }
            break;
        case 105:
            if(C_Empty2_EmptyStop->Push())
            {
                SwMagCW->On();
                LoadTrayDelay.SetSecAndOn(3);
                Task=106;
            }
            break;
        case 106:
            if(SwMagCW->Status()==false)                                        //KenHsieh 20250821 : 暫停後須重新計數
                LoadTrayDelay.SetSecAndOn(3);
            SwMagCW->On();                                                      //Jimmychiu 20240721 : add empty2 keep move and timeout alarm
            if(SnEmpty2HasEmptyTray->IsOn() ||
               HSys.LastSet.iRealDummy<HAS_TRAY)
            {
                Task=109;
            }
            else if(LoadTrayDelay.Off())
            {
                Task=106;
                ShowMyMessage("Empty2 Empty Tray is not arrival! Check sensor SnEmpty2HasEmptyTray!");
            }
            break;
        case 109:
            sEmpty2ID="";
            frm2DID->StartCCDAuto(iEmpty2);
            Task=110;
            break;
        case 110:
            SwMagCW->On();
            if(frm2DID->GetBarcode(iEmpty2)!="")
            {
                sEmpty2ID=frm2DID->GetBarcode(iEmpty2);
                Task=115;
            }
            else if(frm2DID->UpdateCCDAuto(iEmpty2))
            {
                if(frm2DID->MyCCD[iEmpty2]->bEnableCCD==true)                   //JerryYang 20240607 : modify
                {
                    sEmpty2ID=frm2DID->GetBarcode(iEmpty2);
                }
                Task=115;
            }
            break;
        case 115:
            if(sEmpty2ID!="")
            {
                if(CheckClipIDCode(sEmpty2ID)==false)
                {
                    SwMagCW->Off();
                    asStr.sprintf("ClipID=%s", sEmpty2ID);
                    ShowSystemError("80022", K_RETRY, 0, asStr);
                    iRetry=0;
                    Task=109;
                }
                else if(CheckDuplicateLotID(sCheckDuplicateLotIDEmpty2Path(),sEmpty2ID)==TRAY_ID_DUPLICATE)
                {
                    SwMagCW->Off();
                    asStr.sprintf("Duplicate ID check at Empty2 ClipID=%s", sEmpty2ID);
                    ShowMyMessage(asStr);
                    iRetry=0;
                    Task=109;
                }
                else
                {
                    MotEmpty2_Car->SetTrayID(sEmpty2ID);
                    C_CCD_Stop[iEmpty2]->Off();
                    Task=200;
                }
            }
            else if(CatchTrayDelay.Off())
            {
                iRetry++;
                if(iRetry>3)
                {
                    SwMagCW->Off();
                    ShowSystemError("80021", K_RETRY);                                  //"Empty2 CCD 讀取異常"
                    iRetry=0;
                    Task=109;
                }
                else
                {
                    Task=100;
                }
            }
            break;
        case 200:
            if(C_Empty2_EmptyStop->Push() ||
               HSys.LastSet.iRealDummy<HAS_TRAY)
            {
                SwMagCW->On();
                LoadTrayDelay.SetSecAndOn(3);
                Task=300;
            }
            break;
        case 300:
            if(SwMagCW->Status()==false)                                        //KenHsieh 20250821 : 暫停後須重新計數
                LoadTrayDelay.SetSecAndOn(3);
            SwMagCW->On();                                                      //Jimmychiu 20240721 : add empty2 keep move and timeout alarm
            if(SnEmpty2HasEmptyTray->IsOn() ||
               HSys.LastSet.iRealDummy<HAS_TRAY)
            {
                Task=400;
                LoadTrayDelay.SetSecAndOn(3);
            }
            else if(LoadTrayDelay.Off())
            {
                Task=200;
                ShowMyMessage("Empty2 Empty Tray is not arrival! Check sensor SnEmpty2HasEmptyTray!");
            }
            break;
        case 400:
            SwMagCW->On();                                                      //Jimmychiu 20240721 : add empty2 keep move and timeout alarm
            if(LoadTrayDelay.Off() || HSys.LastSet.iRealDummy==DUMMY)
            {
                C_Empty2_EmptyStop->Reset();
                SwMagCW->Off();
                Task=500;
            }
            break;
        case 500:
            if(C_Empty2_EmptyStop->Pop())
            {
                MotEmpty2_Tray->MoveTrayFrom(MotEmpty2_Car);
                Task=9900;
            }
            break;
        case 9900:
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
void TLoaderModule::DownStopAtEmpty1(int iFrom,int iTo)
{
    for(int i=iFrom; i<iTo; i++)
    {
        C_Empty_Stop[i]->Off();
    }
}
//------------------------------------------------------------------------------
bool TLoaderModule::bTransforMoving()
{
    bool bret=false;
    int iNowPos=MotTransferX->ReadEncoderPos();
    if(iNowPos<-5000 && iNowPos>-30000)
        bret=true;
    return bret;
}
//------------------------------------------------------------------------------
bool TLoaderModule::CheckClipIDCode(AnsiString sClipID)
{
    if(CUSTOMER_CODE!=CC_PTI ||
       tSimuData.bRunSimulation)
    {
        return true;
    }
    #ifdef SOFT_SIMULATE
    return true;
    #else
    int iTemp=0;
    AnsiString sTemp="";
    if(sClipID.Length()!=6)
        return false;       //NG 長度不等於6
    iTemp=atoi(sClipID.c_str());
    sTemp=AnsiString().sprintf("%06d",iTemp);
    if(sClipID!=sTemp)
        return false;       //NG 裡面有英文

    return true;        //OK
    #endif
}
//------------------------------------------------------------------------------
