#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop
//------------------------------------------------------------------------------
#include "cinitial.h"
#pragma package(smart_init)
//------------------------------------------------------------------------------
bool bLoadMotorParameter=false;
//------------------------------------------------------------------------------
// 設定馬達比例速度,每一次 [START] 就會設定一次
//------------------------------------------------------------------------------
void SetMotorSpeed(bool bSet)                                                   // ben ok //
{                                                                               // NEED EDIT BY USER //
    if(bSet==false && (HSys.Sys.RunMode==Run_Home || HSys.Sys.SystemStart==true))
        return;

    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        HSys.MotPtr[i]->SetPersentSpeed(HSys.MotPtr[i]->GetPersentSpeed());
    }

    if(HSys.FuncS.bS01_SortArmTrayZ && bSortArmTrayZFixSpeed)                   //Sam 20240619 : 速度獨立設定
    {
        HSys.Mot.StoreArmZ->SetPersentSpeed(HSys.FuncS.iS01_SortArmTrayZ,false);
    }

    if(HSys.FuncS.bS01_MagX)
    {
        if(MagazineModule1->bMagazineXFixSpeed)
            HSys.Mot.Magazine1X->SetPersentSpeed(HSys.FuncS.iS01_MagX, false);

        if(MagazineModule2->bMagazineXFixSpeed)
            HSys.Mot.Magazine2X->SetPersentSpeed(HSys.FuncS.iS01_MagX, false);

        if(MagazineModule3->bMagazineXFixSpeed)
            HSys.Mot.Magazine3X->SetPersentSpeed(HSys.FuncS.iS01_MagX, false);
    }

}
//------------------------------------------------------------------------------
// 設定 Tray 是 SEND,RECEIVE ,吸嘴數量,MOT[]指標
//------------------------------------------------------------------------------
//******************************************************************************
//
//  注意!! Handler 安全門相關, 不可任意修改!!
//
//******************************************************************************
void Initial()
{
    #ifndef SOFT_SIMULATE
    HSys.Sen.SnSafeDoor_01.Enable=true;
    HSys.Sen.SnSafeDoor_02.Enable=true;
    HSys.Sen.SnSafeDoor_03.Enable=true;
    HSys.Sen.SnSafeDoor_04.Enable=true;
    HSys.Sen.SnSafeDoor_05.Enable=true;
    HSys.Sen.SnSafeDoor_06.Enable=true;
    HSys.Sen.SnSafeDoor_07.Enable=true;
    HSys.Sen.SnSafeDoor_08.Enable=true;
    HSys.Sen.SnSafeDoor_09.Enable=true;
    HSys.Sen.SnSafeDoor_10.Enable=true;

    HSys.Sen.SnSafeDoor_01.Type=1;
    HSys.Sen.SnSafeDoor_02.Type=1;
    HSys.Sen.SnSafeDoor_03.Type=1;
    HSys.Sen.SnSafeDoor_04.Type=1;
    HSys.Sen.SnSafeDoor_05.Type=1;
    HSys.Sen.SnSafeDoor_06.Type=1;
    HSys.Sen.SnSafeDoor_07.Type=1;
    HSys.Sen.SnSafeDoor_08.Type=1;
    HSys.Sen.SnSafeDoor_09.Type=1;
    HSys.Sen.SnSafeDoor_10.Type=1;
    #endif
    ReadPassword();
};
//------------------------------------------------------------------------------
// 設定畫面監視刻度比例,更換不同工作檔,應設定一次
//------------------------------------------------------------------------------
//  can not delete ,sample

void DoAddGearRefToProd() // 因為齒輪比超過 1
{
    if(bLoadMotorParameter==false)
        return;
    #ifdef SOFT_SIMULATE
        return;
    #endif
//    ConvertGearValue(&Prod.ZInArmSafe[0],HSys.Mot.MInArmZ1->GearRatio);
}
//------------------------------------------------------------------------------
void SetSimuScreenPara()                                                        // set object simulate view //
{
    HSys.Mot.TrayShiftStageX->SetSimulateCompoment(fMain->palShiftStage, akTop, HSys.Mot.TrayShiftStageX->GetSoftLimitN() , HSys.Mot.TrayShiftStageX->GetSoftLimitP(), 409, 721);

    HSys.VMot.LoadTray_Car->SetHTrayPanel(fMain->mtSortRecv);                   //KenHsieh 20240514 : LoadTray_Car 補上SetHTrayPanel
    HSys.VMot.LoadTray_Car->SetTrayPanel(fMain->palLoadCar);
    HSys.VMot.LoadTray_Car->SetIDPanel(fMain->lblLoaderCarID);

    HSys.VMot.MotWorkLoader->SetHTrayPanel(fMain->mtWorkLoader);
    HSys.VMot.MotWorkLoader->SetSubHTrayPanel(fMain->mtWorkArea);               //Steven 20240520 : 同步顯示
    HSys.VMot.MotWorkLoader->SetTrayPanel(fMain->palWorkLoader);
    HSys.VMot.MotWorkLoader->SetIDPanel(fMain->lblLoadCurrID_1);

    HSys.VMot.NowSortTray->SetHTrayPanel(fMain->mtNowSortTray);
    HSys.VMot.NowSortTray->SetTrayPanel(fMain->palNowSortTray);

    HSys.VMot.Mag_NowSortTray->SetHTrayPanel(fMain->mtMagNowSortTray);
    HSys.VMot.Mag_NowSortTray->SetTrayPanel(fMain->palMagNowSortTray);

    HSys.VMot.MMag1SortTopTray->SetHTrayPanel(fMain->mtMag1);
    HSys.VMot.MMag1SortTopTray->SetTrayPanel(fMain->palMag1);
    HSys.VMot.MMag1SortTopTray->SetIDPanel(fMain->pnlMgz1TopID);

    HSys.VMot.MMag2SortTopTray->SetHTrayPanel(fMain->mtMag2);
    HSys.VMot.MMag2SortTopTray->SetTrayPanel(fMain->palMag2);
    HSys.VMot.MMag2SortTopTray->SetIDPanel(fMain->pnlMgz2TopID);

    HSys.VMot.MMag3SortTopTray->SetHTrayPanel(fMain->mtMag3);
    HSys.VMot.MMag3SortTopTray->SetTrayPanel(fMain->palMag3);
    HSys.VMot.MMag3SortTopTray->SetIDPanel(fMain->pnlMgz3TopID);

    HSys.VMot.Empty_1->SetHTrayPanel(fMain->mtEmptyCar);
    HSys.VMot.Empty_1->SetTrayPanel(fMain->palEmpty1Car);
    HSys.VMot.Empty_1->SetIDPanel(fMain->lblEmpty1_CarID);

    HSys.VMot.Mag_LD_Car->SetHTrayPanel(fMain->mtMagEmpty_Car);
    HSys.VMot.Mag_LD_Car->SetTrayPanel(fMain->palEmpty2Car);
    HSys.VMot.Mag_LD_Car->SetIDPanel(fMain->lblEmpty2_CarID);

    HSys.VMot.Mag_LD_Work_Y->SetHTrayPanel(fMain->mtMagLD);
    HSys.VMot.Mag_LD_Work_Y->SetSubHTrayPanel(fMain->mtMgzArea);                //Steven 20240520 : 同步顯示
    HSys.VMot.Mag_LD_Work_Y->SetTrayPanel(fMain->palMagLD);
    HSys.VMot.Mag_LD_Work_Y->SetIDPanel(fMain->lblMgzCurrID_1);

    HSys.VMot.MagEmptyTray->SetHTrayPanel(fMain->mtMagEmptyTray);
    HSys.VMot.MagEmptyTray->SetTrayPanel(fMain->palMagEmptyTray);

    HSys.VMot.Track_1->SetHTrayPanel(fMain->mtWorkEmpty1);
    HSys.VMot.Track_2->SetHTrayPanel(fMain->mtWorkEmpty2);
    HSys.VMot.Track_3->SetHTrayPanel(fMain->mtWorkEmpty3);
    HSys.VMot.Track_4->SetHTrayPanel(fMain->mtWorkEmpty4);

    HSys.VMot.Track_1->SetIDPanel(fMain->lblEmpty1_Track1ID);
    HSys.VMot.Track_2->SetIDPanel(fMain->lblEmpty1_Track2ID);
    HSys.VMot.Track_3->SetIDPanel(fMain->lblEmpty1_Track3ID);
    HSys.VMot.Track_4->SetIDPanel(fMain->lblEmpty1_Track4ID);

    HSys.VMot.Track_1->SetTrayPanel(fMain->palWorkEmpty1);
    HSys.VMot.Track_2->SetTrayPanel(fMain->palWorkEmpty2);
    HSys.VMot.Track_3->SetTrayPanel(fMain->palWorkEmpty3);
    HSys.VMot.Track_4->SetTrayPanel(fMain->palWorkEmpty4);

    HSys.VMot.Transfer_LoaderY->SetHTrayPanel(fMain->mtShiftStage);
    HSys.VMot.Transfer_LoaderY->SetTrayPanel(fMain->palShiftStage);
    HSys.VMot.Transfer_LoaderY->SetIDPanel(fMain->lblTransferID);

    for(int i=0; i<eTrayCount; i++)
    {
        HSys.VMot.tmAutoAreaAuto[i]->SetHTrayPanel(fMain->mtPanel[i]);
        HSys.VMot.tmAutoAreaAuto[i]->SetTrayPanel(fMain->tTPanelMot[i]);
        HSys.VMot.tmAutoAreaAuto[i]->SetIDPanel(fMain->tTPanelID[i]);
    }

    HSys.VMot.MagazineTrayArm[eMagazine1]->SetHTrayPanel(fMain->mtMagBuf1);
    HSys.VMot.MagazineTrayArm[eMagazine1]->SetTrayPanel(fMain->palMagBuf1);
    HSys.VMot.MagazineTrayArm[eMagazine2]->SetHTrayPanel(fMain->mtMagBuf2);
    HSys.VMot.MagazineTrayArm[eMagazine2]->SetTrayPanel(fMain->palMagBuf2);
    HSys.VMot.MagazineTrayArm[eMagazine3]->SetHTrayPanel(fMain->mtMagBuf3);
    HSys.VMot.MagazineTrayArm[eMagazine3]->SetTrayPanel(fMain->palMagBuf3);

    HSys.VMot.Track_1_TrayArm->SetHTrayPanel(fMain->mtTrack1TrayArm);
    HSys.VMot.Track_2_TrayArm->SetHTrayPanel(fMain->mtTrack2TrayArm);
    HSys.VMot.Track_3_TrayArm->SetHTrayPanel(fMain->mtTrack3TrayArm);
    HSys.VMot.Track_4_TrayArm->SetHTrayPanel(fMain->mtTrack4TrayArm);

    HSys.VMot.Track_1_TrayArm->SetTrayPanel(fMain->palTrack1TrayArm);
    HSys.VMot.Track_2_TrayArm->SetTrayPanel(fMain->palTrack2TrayArm);
    HSys.VMot.Track_3_TrayArm->SetTrayPanel(fMain->palTrack3TrayArm);
    HSys.VMot.Track_4_TrayArm->SetTrayPanel(fMain->palTrack4TrayArm);

    HSys.VMot.TrayArmTray->SetHTrayPanel(fMain->mtTrayArmTray);
    HSys.VMot.TrayArmTray->SetTrayPanel(fMain->palTrayArmTray);

    HSys.Suck.AutoArmSuck.SetMyLed(0, 0, (TMyLed *)fMain->ledSortArm1ZA);
    HSys.Suck.AutoArmSuck.SetMyLed(0, 1, (TMyLed *)fMain->ledSortArm1ZB);
    HSys.Suck.AutoArmSuck.SetMyLed(1, 0, (TMyLed *)fMain->ledSortArm1ZC);
    HSys.Suck.AutoArmSuck.SetMyLed(1, 1, (TMyLed *)fMain->ledSortArm1ZD);
    HSys.Suck.AutoArmSuck.SetMyLed(2, 0, (TMyLed *)fMain->ledSortArm1ZE);
    HSys.Suck.AutoArmSuck.SetMyLed(2, 1, (TMyLed *)fMain->ledSortArm1ZF);
    HSys.Suck.AutoArmSuck.SetMyLed(3, 0, (TMyLed *)fMain->ledSortArm1ZG);
    HSys.Suck.AutoArmSuck.SetMyLed(3, 1, (TMyLed *)fMain->ledSortArm1ZH);

    HSys.Suck.MagArmSuck.SetMyLed(0, 0, (TMyLed *)fMain->ledSortArm2ZA);
    HSys.Suck.MagArmSuck.SetMyLed(0, 1, (TMyLed *)fMain->ledSortArm2ZB);
    HSys.Suck.MagArmSuck.SetMyLed(1, 0, (TMyLed *)fMain->ledSortArm2ZC);
    HSys.Suck.MagArmSuck.SetMyLed(1, 1, (TMyLed *)fMain->ledSortArm2ZD);
    HSys.Suck.MagArmSuck.SetMyLed(2, 0, (TMyLed *)fMain->ledSortArm2ZE);
    HSys.Suck.MagArmSuck.SetMyLed(2, 1, (TMyLed *)fMain->ledSortArm2ZF);
    HSys.Suck.MagArmSuck.SetMyLed(3, 0, (TMyLed *)fMain->ledSortArm2ZG);
    HSys.Suck.MagArmSuck.SetMyLed(3, 1, (TMyLed *)fMain->ledSortArm2ZH);

    HSys.Mot.SortArm1X->SetSimulateCompoment(fMain->palSortArm1, akTop , HSys.Mot.SortArm1X->GetSoftLimitN() , HSys.Mot.SortArm1X->GetSoftLimitP(), 680, 1240);
    HSys.Mot.SortArm1Y->SetSimulateCompoment(fMain->palSortArm1, akLeft, HSys.Mot.SortArm1Y->GetSoftLimitN() , HSys.Mot.SortArm1Y->GetSoftLimitP(), 650, 0);

    HSys.Mot.SortArm1ZA->SetSimulateCompoment(fMain->ledSortArm1ZA, akLeft, -2500, 10, 14, 7);
    HSys.Mot.SortArm1ZB->SetSimulateCompoment(fMain->ledSortArm1ZB, akLeft, -2500, 10, 14, 7);
    HSys.Mot.SortArm1ZC->SetSimulateCompoment(fMain->ledSortArm1ZC, akLeft, -2500, 10, 37, 30);
    HSys.Mot.SortArm1ZD->SetSimulateCompoment(fMain->ledSortArm1ZD, akLeft, -2500, 10, 37, 30);
    HSys.Mot.SortArm1ZE->SetSimulateCompoment(fMain->ledSortArm1ZE, akLeft, -2500, 10, 59, 52);
    HSys.Mot.SortArm1ZF->SetSimulateCompoment(fMain->ledSortArm1ZF, akLeft, -2500, 10, 59, 52);
    HSys.Mot.SortArm1ZG->SetSimulateCompoment(fMain->ledSortArm1ZG, akLeft, -2500, 10, 82, 75);
    HSys.Mot.SortArm1ZH->SetSimulateCompoment(fMain->ledSortArm1ZH, akLeft, -2500, 10, 82, 75);

    HSys.Mot.SortArm2X->SetSimulateCompoment(fMain->palSortArm2, akTop , HSys.Mot.SortArm2X->GetSoftLimitN() , HSys.Mot.SortArm2X->GetSoftLimitP(), 264, 472);
    HSys.Mot.SortArm2Y->SetSimulateCompoment(fMain->palSortArm2, akLeft, HSys.Mot.SortArm2Y->GetSoftLimitN() , HSys.Mot.SortArm2Y->GetSoftLimitP(), 604, 0);

    HSys.Mot.SortArm2ZA->SetSimulateCompoment(fMain->ledSortArm2ZA, akLeft, -2500, 10, 14, 7);
    HSys.Mot.SortArm2ZB->SetSimulateCompoment(fMain->ledSortArm2ZB, akLeft, -2500, 10, 14, 7);
    HSys.Mot.SortArm2ZC->SetSimulateCompoment(fMain->ledSortArm2ZC, akLeft, -2500, 10, 37, 30);
    HSys.Mot.SortArm2ZD->SetSimulateCompoment(fMain->ledSortArm2ZD, akLeft, -2500, 10, 37, 30);
    HSys.Mot.SortArm2ZE->SetSimulateCompoment(fMain->ledSortArm2ZE, akLeft, -2500, 10, 59, 52);
    HSys.Mot.SortArm2ZF->SetSimulateCompoment(fMain->ledSortArm2ZF, akLeft, -2500, 10, 59, 52);
    HSys.Mot.SortArm2ZG->SetSimulateCompoment(fMain->ledSortArm2ZG, akLeft, -2500, 10, 82, 75);
    HSys.Mot.SortArm2ZH->SetSimulateCompoment(fMain->ledSortArm2ZH, akLeft, -2500, 10, 82, 75);

    HSys.Suck.TrayArmSuck.SetMyLed(0, 0, (TMyLed *)fMain->ledTrayArm);
    HSys.Suck.MagTrayArmSuck.SetMyLed(0, 0, (TMyLed *)fMain->ledMagTrayArm);
}
//------------------------------------------------------------------------------
void SetTechDataToProd()                                                        // need edit ben //
{
    int iXDiv=atoi(fSetup->edXDivision->Text.c_str());
    int iYDiv=atoi(fSetup->edYDivision->Text.c_str());

    for(int i=0; i<eTrayCount; i++)
    {
        HSys.VMot.tmAutoAreaAuto[i]->SetTrayXYItem(iXDiv, iYDiv);
    }

//    HSys.VMot.Loader_Up->Tray.XItem      =iXDiv;
//    HSys.VMot.Loader_Up->Tray.YItem      =iYDiv;

    HSys.VMot.MotWorkLoader->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.LoadTray_Car->SetTrayXYItem(iXDiv, iYDiv);    //JerryYang 20240630 :add

    HSys.VMot.NowSortTray->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.Mag_NowSortTray->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.Empty_1->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.MagEmptyTray->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.MMag1SortTopTray->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.MMag2SortTopTray->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.MMag3SortTopTray->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.MagazineTrayArm[eMagazine1]->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.MagazineTrayArm[eMagazine2]->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.MagazineTrayArm[eMagazine3]->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.Track_1->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.Track_2->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.Track_3->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.Track_4->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.Mag_LD_Car->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.Transfer_LoaderY->SetTrayXYItem(iXDiv, iYDiv);

    HSys.VMot.Track_1_TrayArm->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.Track_2_TrayArm->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.Track_3_TrayArm->SetTrayXYItem(iXDiv, iYDiv);
    HSys.VMot.Track_4_TrayArm->SetTrayXYItem(iXDiv, iYDiv);
}
//------------------------------------------------------------------------------
bool SetWorkParameter()                                                         // need edit ben //
{
    SetTechDataToProd();
//    SetSimuScreenPara();                                                      //JerryYang 20240610 : 執行這個花了2秒,改到FormShow做一次就好                                                     // 軟體模擬 //
    SetMotorSpeed();
    UpdateAllParameter();
    return true;
}
//------------------------------------------------------------------------------




