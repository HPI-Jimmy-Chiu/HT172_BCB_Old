#include "IncludeAllHeader.h"
#pragma hdrstop

//------------------------------------------------------------------------------
#include "aMagArm.h"
#include "aLoader.h"
#pragma package(smart_init)
//------------------------------------------------------------------------------
//AI(PTI-MagArm) 20260521 : remember last picked magazine for PTI round-robin priority
int gLastPickedMag = -1;
//------------------------------------------------------------------------------
MyMagArmParameter *MagArmPara=new MyMagArmParameter();

enum eMagPreload
    {
        eMagNoPreload       =0,
        eMagPreloadTransfer =1,
        eMagPreloadLoadY    =2,
        eMagPreloadLoadCar  =3
    };
bool bMagSuckEnd[4][2]={{false, false}, {false, false}, {false, false}, {false, false}};    //JerryYang 20240623 : pick up error可以SKIP
//------------------------------------------------------------------------------
void InitMagArm()
{
    MagArmPara->MagArmSuck         =&HSys.Suck.MagArmSuck;

    MagArmPara->MMagArmZ[0][0]     =HSys.Mot.SortArm2ZA;
    MagArmPara->MMagArmZ[0][1]     =HSys.Mot.SortArm2ZB;
    MagArmPara->MMagArmZ[1][0]     =HSys.Mot.SortArm2ZC;
    MagArmPara->MMagArmZ[1][1]     =HSys.Mot.SortArm2ZD;
    MagArmPara->MMagArmZ[2][0]     =HSys.Mot.SortArm2ZE;
    MagArmPara->MMagArmZ[2][1]     =HSys.Mot.SortArm2ZF;
    MagArmPara->MMagArmZ[3][0]     =HSys.Mot.SortArm2ZG;
    MagArmPara->MMagArmZ[3][1]     =HSys.Mot.SortArm2ZH;

    MagArmPara->MMagArmX           =HSys.Mot.SortArm2X;
    MagArmPara->MMagArmY           =HSys.Mot.SortArm2Y;

    MagArmPara->MMagArmPitchX      =HSys.Mot.SortArm2PitchX;
    MagArmPara->MMagArmPitchY1     =HSys.Mot.SortArm2PitchY1;
    MagArmPara->MMagArmPitchY2     =HSys.Mot.SortArm2PitchY2;

    MagArmPara->MMagSortTray       =HSys.VMot.Mag_LD_Work_Y;
    MagArmPara->MMagNowSortTray    =HSys.VMot.Mag_NowSortTray;
    MagArmPara->MMagSortTopTray[0] =HSys.VMot.MMag1SortTopTray;
    MagArmPara->MMagSortTopTray[1] =HSys.VMot.MMag2SortTopTray;
    MagArmPara->MMagSortTopTray[2] =HSys.VMot.MMag3SortTopTray;

    MagArmPara->SnMGZ2TopHasTray   =&HSys.Sen.SnMGZ2TopHasTray;
    MagArmPara->C_MGZ_TopFixTray   =&HSys.Cyn.C_MGZ2_TopFixTray;

    MagArmPara->MagTransferLD      =HSys.VMot.Transfer_LoaderY;

    for(int iR=0; iR<MagArmPara->MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmPara->MagArmSuck->MaxItemC; iC++)
        {
            MagArmPara->bCheckMagArmDestroyActive[iR][iC]=false;
            MagArmPara->bCheckMagArmDestroyActiveFinish[iR][iC]=true;
            MagArmPara->iTask[iR][iC]=0;
            MagArmPara->iTask[iR][iC]=0;
            MagArmPara->bMagArmCheckDestroyACT[iR][iC]=false;
            MagArmPara->bMagArmCheckDestroyACTBuf[iR][iC]=false;
        }
    }
    //
    MagArmPara->C_MagArm_UpDown        =&HSys.Cyn.C_SortArm2_UpDown;
    MagArmPara->C_CatchTray            =&HSys.Cyn.C_CatchTray_Fix;
    MagArmPara->bNeedPlaceFirst        =false;                                  //KenHsieh(JerryYang) 20240826 : ARM上已經有IC就先放料, 不要重新search
}
//------------------------------------------------------------------------------
void UpdateMagArmParameter()
{
    int iPosZ_Offset[4][2], iOffsetPosZ_Offset_Pick[4][2], iOffsetPosZ_Offset_Place[4][2];

    iPosZ_Offset[0][0]=atoi(fTeach->edSortArm2ZA_Offset->Text.c_str());
    iPosZ_Offset[0][1]=atoi(fTeach->edSortArm2ZB_Offset->Text.c_str());
    iPosZ_Offset[1][0]=atoi(fTeach->edSortArm2ZC_Offset->Text.c_str());
    iPosZ_Offset[1][1]=0;
    iPosZ_Offset[2][0]=atoi(fTeach->edSortArm2ZE_Offset->Text.c_str());
    iPosZ_Offset[2][1]=atoi(fTeach->edSortArm2ZF_Offset->Text.c_str());
    iPosZ_Offset[3][0]=atoi(fTeach->edSortArm2ZG_Offset->Text.c_str());
    iPosZ_Offset[3][1]=atoi(fTeach->edSortArm2ZH_Offset->Text.c_str());

    iOffsetPosZ_Offset_Pick[0][0]=atoi(fOffset->edSortArm2ZA_Offset_pick->Text.c_str());
    iOffsetPosZ_Offset_Pick[0][1]=atoi(fOffset->edSortArm2ZB_Offset_pick->Text.c_str());
    iOffsetPosZ_Offset_Pick[1][0]=atoi(fOffset->edSortArm2ZC_Offset_pick->Text.c_str());
    iOffsetPosZ_Offset_Pick[1][1]=0;                                                       //KenHsieh 20240514 : 刪除基準吸嘴Ofs
    iOffsetPosZ_Offset_Pick[2][0]=atoi(fOffset->edSortArm2ZE_Offset_pick->Text.c_str());
    iOffsetPosZ_Offset_Pick[2][1]=atoi(fOffset->edSortArm2ZF_Offset_pick->Text.c_str());
    iOffsetPosZ_Offset_Pick[3][0]=atoi(fOffset->edSortArm2ZG_Offset_pick->Text.c_str());
    iOffsetPosZ_Offset_Pick[3][1]=atoi(fOffset->edSortArm2ZH_Offset_pick->Text.c_str());

    iOffsetPosZ_Offset_Place[0][0]=atoi(fOffset->edSortArm2ZA_Offset_place->Text.c_str());
    iOffsetPosZ_Offset_Place[0][1]=atoi(fOffset->edSortArm2ZB_Offset_place->Text.c_str());
    iOffsetPosZ_Offset_Place[1][0]=atoi(fOffset->edSortArm2ZC_Offset_place->Text.c_str());
    iOffsetPosZ_Offset_Place[1][1]=0;                                                      //KenHsieh 20240514 : 刪除基準吸嘴Ofs
    iOffsetPosZ_Offset_Place[2][0]=atoi(fOffset->edSortArm2ZE_Offset_place->Text.c_str());
    iOffsetPosZ_Offset_Place[2][1]=atoi(fOffset->edSortArm2ZF_Offset_place->Text.c_str());
    iOffsetPosZ_Offset_Place[3][0]=atoi(fOffset->edSortArm2ZG_Offset_place->Text.c_str());
    iOffsetPosZ_Offset_Place[3][1]=atoi(fOffset->edSortArm2ZH_Offset_place->Text.c_str());

    MagArmPara->iPos_MagLoader_X = atoi(fTeach->edSortArm2X_LDPick->Text.c_str())+atoi(fOffset->edSortArm2X_LDPick->Text.c_str());
    MagArmPara->iPos_MagLoader_Y = atoi(fTeach->edSortArm2Y_LDPick->Text.c_str())+atoi(fOffset->edSortArm2Y_LDPick->Text.c_str());
    //eMagazine1
    MagArmPara->iPos_MagTop_X[eMagazine1] = atoi(fTeach->edSortArm2X_Mgz1TopPlace->Text.c_str())+atoi(fOffset->edSortArm2X_Mag1TopPlace->Text.c_str());
    MagArmPara->iPos_MagTop_Y[eMagazine1] = atoi(fTeach->edSortArm2Y_Mgz1TopPlace->Text.c_str())+atoi(fOffset->edSortArm2Y_Mag1TopPlace->Text.c_str());
    //eMagazine2
    MagArmPara->iPos_MagTop_X[eMagazine2] = atoi(fTeach->edSortArm2X_Mgz2TopPlace->Text.c_str())+atoi(fOffset->edSortArm2X_Mag2TopPlace->Text.c_str());
    MagArmPara->iPos_MagTop_Y[eMagazine2] = atoi(fTeach->edSortArm2Y_Mgz2TopPlace->Text.c_str())+atoi(fOffset->edSortArm2Y_Mag2TopPlace->Text.c_str());
    //eMagazine3
    MagArmPara->iPos_MagTop_X[eMagazine3] = atoi(fTeach->edSortArm2X_Mgz3TopPlace->Text.c_str())+atoi(fOffset->edSortArm2X_Mag3TopPlace->Text.c_str());
    MagArmPara->iPos_MagTop_Y[eMagazine3] = atoi(fTeach->edSortArm2Y_Mgz3TopPlace->Text.c_str())+atoi(fOffset->edSortArm2Y_Mag3TopPlace->Text.c_str());
    //
    for(int iC=0; iC<MagArmPara->MagArmSuck->MaxItemC; iC++)
    {
        for(int iR=0; iR<MagArmPara->MagArmSuck->MaxItemR; iR++)
        {
            MagArmPara->iPos_MagLoader_Z[iR][iC]=atoi(fTeach->edSortArm2ZD_LDPick->Text.c_str())+
                                                 atoi(fOffset->edSortArm2ZD_LDPick->Text.c_str())+       //KenHsieh 20240514 : 新增整體Z Ofs
                                                 iPosZ_Offset[iR][iC]+iOffsetPosZ_Offset_Pick[iR][iC];

            MagArmPara->iPos_MagTop_Z[eMagazine1][iR][iC]  =atoi(fTeach->edSortArm2ZD_Mgz1TopPlace->Text.c_str())+
                                                            atoi(fOffset->edSortArm2ZD_Mag1TopPlace->Text.c_str())+   //KenHsieh 20240514 : 新增整體Z Ofs
                                                            iPosZ_Offset[iR][iC]+iOffsetPosZ_Offset_Place[iR][iC];

            MagArmPara->iPos_MagTop_Z[eMagazine2][iR][iC]  =atoi(fTeach->edSortArm2ZD_Mgz2TopPlace->Text.c_str())+
                                                            atoi(fOffset->edSortArm2ZD_Mag2TopPlace->Text.c_str())+   //KenHsieh 20240514 : 新增整體Z Ofs
                                                            iPosZ_Offset[iR][iC]+iOffsetPosZ_Offset_Place[iR][iC];

            MagArmPara->iPos_MagTop_Z[eMagazine3][iR][iC]  =atoi(fTeach->edSortArm2ZD_Mgz3TopPlace->Text.c_str())+
                                                            atoi(fOffset->edSortArm2ZD_Mag3TopPlace->Text.c_str())+   //KenHsieh 20240514 : 新增整體Z Ofs
                                                            iPosZ_Offset[iR][iC]+iOffsetPosZ_Offset_Place[iR][iC];


            MagArmPara->MagArmSuck->Suck[iR][iC].OnDelayTime =atof(fSetup->edMagPickDelay->Text.c_str())*1000.0;
            MagArmPara->MagArmSuck->Suck[iR][iC].OffDelayTime=atof(fSetup->edMagPlaceDelay->Text.c_str())*1000.0;
        }
    }

    MagArmPara->iPos_Y120Pitch1 = atoi(fTeach->edSortArm2PitchY1_P_120mm->Text.c_str());
    MagArmPara->iPos_Y40Pitch1  = atoi(fTeach->edSortArm2PitchY1_P_40mm->Text.c_str()) ;
    MagArmPara->iPos_Y120Pitch2 = atoi(fTeach->edSortArm2PitchY2_P_120mm->Text.c_str());
    MagArmPara->iPos_Y40Pitch2  = atoi(fTeach->edSortArm2PitchY2_P_40mm->Text.c_str()) ;
    MagArmPara->iPos_X60Pitch   = atoi(fTeach->edSortArm2PitchX_P_60mm->Text.c_str())  ;
    MagArmPara->iPos_X15Pitch   = atoi(fTeach->edSortArm2PitchX_P_15mm->Text.c_str())  ;

    MagArmPara->iTrayXDivision  = atoi(fSetup->edXDivision->Text.c_str());
    MagArmPara->iTrayYDivision  = atoi(fSetup->edYDivision->Text.c_str());
    MagArmPara->iTrayXPitch     = atof(fSetup->edXPitch->Text.c_str())*100.0;
    MagArmPara->iTrayYPitch     = atof(fSetup->edYPitch->Text.c_str())*100.0;
    MagArmPara->iTrayXStart     = atof(fSetup->edXStart->Text.c_str())*100.0;
    MagArmPara->iTrayYStart     = atof(fSetup->edYStart->Text.c_str())*100.0;
    MagArmPara->iColPitch       =MagArmPara->AutoCalculateOutArmClosePitchCol(MagArmPara->iStepCol);
    MagArmPara->iRowPitch       =MagArmPara->AutoCalculateOutArmClosePitchRow(MagArmPara->iStepRow);
    //Sam 20240607 : Suck 預吸吹功能
    //==>
    MagArmPara->bPreSuckOn =fSetup->cbMagPreSuckOn->Checked;
    MagArmPara->iPreSuckOn =atof(fSetup->edMagPreSuckOn->Text.c_str())*100.0;
    MagArmPara->bPreSuckOff=fSetup->cbMagPreSuckOff->Checked;
    MagArmPara->iPreSuckOff=atof(fSetup->edMagPreSuckOff->Text.c_str())*100.0;
    MagArmPara->bPreZDown=fSetup->cbMagPreZDown->Checked;
    MagArmPara->iPreZDown=atof(fSetup->edMagPreZDown->Text.c_str())*100.0;
    //<==
    //Sam 20240607 : Suck 預吸吹功能
    MagArmPara->dDeviceConfirm=atof(fSetup->edtDestroyCheckTime->Text.c_str()); //Sam20240627 : 修正回黏問題
    MagArmPara->iMaxSpaceOnePlace=((MagArmPara->iStepRow*3)+1)*MagArmPara->MMagSortTray->Tray.XItem;      //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞

    MagArmPara->iPos_MagPickTray_X                 =atoi(fTeach->edSortArm2X_TrayPick->Text.c_str())+atoi(fOffset->edSortArm2X_TrayPick->Text.c_str());
    MagArmPara->iPos_MagPickTray_Y                 =atoi(fTeach->edSortArm2Y_TrayPick->Text.c_str())+atoi(fOffset->edSortArm2Y_TrayPick->Text.c_str());
    //eMagazine1
    MagArmPara->iPos_MagPlaceTray_X[eMagazine1]    =atoi(fTeach->edSortArm2X_Mgz1TrayTopPlace->Text.c_str())+atoi(fOffset->edSortArm2X_ToMag1Tray->Text.c_str());
    MagArmPara->iPos_MagPlaceTray_Y[eMagazine1]    =atoi(fTeach->edSortArm2Y_Mgz1TrayTopPlace->Text.c_str())+atoi(fOffset->edSortArm2Y_ToMag1Tray->Text.c_str());
    //eMagazine2
    MagArmPara->iPos_MagPlaceTray_X[eMagazine2]    =atoi(fTeach->edSortArm2X_Mgz2TrayTopPlace->Text.c_str())+atoi(fOffset->edSortArm2X_ToMag2Tray->Text.c_str());
    MagArmPara->iPos_MagPlaceTray_Y[eMagazine2]    =atoi(fTeach->edSortArm2Y_Mgz2TrayTopPlace->Text.c_str())+atoi(fOffset->edSortArm2Y_ToMag2Tray->Text.c_str());
    //eMagazine3
    MagArmPara->iPos_MagPlaceTray_X[eMagazine3]    =atoi(fTeach->edSortArm2X_Mgz3TrayTopPlace->Text.c_str())+atoi(fOffset->edSortArm2X_ToMag3Tray->Text.c_str());
    MagArmPara->iPos_MagPlaceTray_Y[eMagazine3]    =atoi(fTeach->edSortArm2Y_Mgz3TrayTopPlace->Text.c_str())+atoi(fOffset->edSortArm2Y_ToMag3Tray->Text.c_str());
}
//---------------------------------------------------------------------------
__fastcall MyMagArmParameter::MyMagArmParameter()
{
    InitialFlag();
}
//---------------------------------------------------------------------------
void MyMagArmParameter::InitialFlag()
{
    iBaseSuckX=1;
    iBaseSuckY=1;
    mBinToTray.ConvertBinToAuto(0);
    mNowBin.ConvertBinToAuto(0);
    iPickAndPlace2TopMagTask=0;
    iCatchEmptyTrayTask     =0;
    iEmptyMagStatus         =0;
    iMagStatus              =eHS_MAG_Idle;
}
//---------------------------------------------------------------------------
void MyMagArmParameter::InitialSuckFlag()
{

}
//---------------------------------------------------------------------------
void MyMagArmParameter::InitMagArmXYMoveTask()
{
    iMagArmXYMoveTask=1;
}
//---------------------------------------------------------------------------
int MyMagArmParameter::MagArmXYMove(int iXComPos, int iYComPos)
{
    static bool bMagArmFlag[3]={false, false, false};
    int &iTask=iMagArmXYMoveTask;

    if(HSys.Cyn.C_SortArm2_UpDown.IsOn() || HSys.Cyn.C_SortArm2_UpDown.GetOutBit()==true)
    {
        ShowMyMessage("C_SortArm2_UpDown need Up when MagArm Move!");
        return PNP_DOING;
    }

    switch(iTask)
    {
        case 1:
            bMagArmFlag[0]=false;
            bMagArmFlag[1]=false;
            bMagArmFlag[2]=false;
            iTask=100;
        case 100:
            if(MagArmDeviceDropCheck()==0)
            {
                if(bMagArmFlag[0]==false)
                    bMagArmFlag[0]=MMagArmX->MotorMove(iXComPos);
                if(bMagArmFlag[1]==false)
                    bMagArmFlag[1]=MMagArmY->MotorMove(iYComPos);

                if(bMagArmFlag[0] && bMagArmFlag[1])
                {
                    return PNP_DONE;
                }
            }
            else
            {
                bMagArmFlag[0]=false;
                bMagArmFlag[1]=false;
                bMagArmFlag[2]=false;
            }

            break;
    }
    return PNP_DOING;
}
//---------------------------------------------------------------------------
void MyMagArmParameter::CheckMagArmDestroyActive()
{
    int iPos[4][2];

    AnsiString str;

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(HSys.LastSet.iRealDummy==DUMMY ||
               HSys.LastSet.iRealDummy==HAS_TRAY)
                bMagArmCheckDestroyACT[iR][iC]=false;

            if(bMagArmCheckDestroyACT[iR][iC]==false)
            {
                iTask[iR][iC]=1;
                continue;
            }

            switch(iTask[iR][iC])
            {
                case 1:
                    bCheckMagArmDestroyActiveFinish[iR][iC]=false;              //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成

                    iPos[iR][iC]=MMagArmZ[iR][iC]->ReadPos();
                       if(MagArmSuck->Suck[iR][iC].Item==NULL_IC)               //Steven 20120507 : 要持續吹氣
                          MagArmSuck->Suck[iR][iC].OffDestroy();

                    if(iPos[iR][iC]>=0)
                    {
                        iTask[iR][iC]=200;
                        MagArmSuck->Suck[iR][iC].OffDestroy();
                    }
                    break;
                case 200:
                    MagArmSuck->Suck[iR][iC].OnSuck();
                    CheckMagArmDestroyActiveDelay[iR][iC].SetSecAndOn(MagArmPara->dDeviceConfirm);
                    bCheckMagArmDestroyActive[iR][iC]=true;
                    iTask[iR][iC]=300;
                    break;
                case 300:
                    if(CheckMagArmDestroyActiveDelay[iR][iC].Off())
                    {
                        if(MagArmSuck->Suck[iR][iC].GetStatus())
                        {
                            ShowSystemError(MagArmSuck->AlarmName[eDestoryErr], K_RETRY);   //破壞錯誤, 請將IC取下
                            bCheckMagArmDestroyActive[iR][iC]=true;
                            iTask[iR][iC]=200;                                  //Steven 20120507 : 有出錯就繼續吸吸看，看IC有沒有被拿掉。
                        }
                        else
                        {
                            bCheckMagArmDestroyActiveFinish[iR][iC]=true;       //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                            MagArmSuck->Suck[iR][iC].Normal();
                            bMagArmCheckDestroyACT[iR][iC]=false;
                            bCheckMagArmDestroyActive[iR][iC]=false;
                            //iTask[i][j]=999;
                            iTask[iR][iC]=1;                                    //ChungHung 20111229 add 防止跳出後bInArmCheckDestroyACT馬上被設為true 會照成有一次無效
                        }
                    }
                    break;
                default :
                    break;
            }
        }
    }
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::MagArmPitchMove(int VariX, int VariY1, int VariY2, bool bInit)     //Steven for HT1032
{
    static bool bFirstIn=true;
    static bool bFlag[4]={false, false, false, false};

    if(bInit || bFirstIn)
    {
        bFirstIn=false;
        ZeroMemory(bFlag, sizeof(bFlag));
        return 0;
    }

    if(bFlag[0]==false)
        bFlag[0]=MMagArmPitchX->MotorMove(VariX);
    if(bFlag[1]==false)
        bFlag[1]=MMagArmPitchY1->MotorMove(VariY1);
    if(bFlag[2]==false)
        bFlag[2]=MMagArmPitchY2->MotorMove(VariY2);

    if(bFlag[0] && bFlag[1] && bFlag[2])
    {
        ZeroMemory(bFlag, sizeof(bFlag));
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::MagArmContinuousMove(int iXPos, int iYPos, int VariX, int VariY1, int VariY2, bool ZNeedDown, bool bPick)   //Sam 20240607 : Suck 預吸吹功能
{
    AnsiString str;
    int iCheckX=0, iCheckY=0, iEncodeX=0, iEncodeY=0;
    int ret=PNP_DOING;
    static bool MagSafeCheck[4][2]={{false, false}, {false, false}, {false, false}, {false, false}};
    static bool bPitchMoveOk=false;
    static bool bFlag[5]={false, false, false, false, false};
    static bool bFlagZ[4][2]={{false, false}, {false, false}, {false, false}, {false, false}};
    static int iMagArmAllZSafe=1;
    static bool bXYMoveFinish=false;
    static bool bXYPosErrorFlag=false;

    static int iLastPosX=-99999;
    static int iLastPosY=-99999;
    int iZCurPos=0, iXCurPos=0, iYCurPos=0;                                     //Sam 20240607 : Suck 預吸吹功能

    if(iLastPosX!=iXPos || iLastPosY!=iYPos)
    {
        iLastPosX=iXPos;
        iLastPosY=iYPos;
        iMagArmAllZSafe=1;
    }

    if(HSys.Cyn.C_SortArm2_UpDown.IsOn() || HSys.Cyn.C_SortArm2_UpDown.GetOutBit()==true)
    {
        ShowMyMessage("C_SortArm2_UpDown need Up when MagArm Move!");
        return false;
    }

    //jou 980313 safe protect
    if(iXPos>=MMagArmX->GetSoftLimitP() || iXPos<=MMagArmX->GetSoftLimitN())
    {
        ShowSystemError(MMagArmX->AlarmName[eMotOverLimitErr], K_RETRY);        //"Mag Arm X axis motor will out of limit !"
        return false;
    }
    if(iYPos>=MMagArmY->GetSoftLimitP() || iYPos<=MMagArmY->GetSoftLimitN())
    {
        ShowSystemError(MMagArmY->AlarmName[eMotOverLimitErr], K_RETRY);        //"Mag Arm Y axis motor will out of limit !"
        return false;
    }

//    if(HSys.Sen.SnSortArm2XInterfere.IsOn() && HSys.Sen.SnSortArm2YInterfere.IsOn())    //JerryYang 20240527 : add
//    {                                                                         //Steven 20240728 : Mark
//        ShowMyMessage("Arm 2 X and Y Interfere sensor are on!");
//        return false;
//    }

    if(MMagArmX->GetEnable() && MMagArmY->GetEnable())
    {
        if(bPauseMagMotor)
        {
            iMagArmAllZSafe=1;
            bPauseMagMotor=false;
            OutArmIdle.SetSecAndOn(10);
        }

        switch(iMagArmAllZSafe)
        {
            case 1:
                if(MoveMagArmToAutoSafe())
                {
                    OutArmIdle.On();
                    iMagArmAllZSafe=100;
                    bXYMoveFinish=false;
                    bPitchMoveOk=false;
                    MagArmPitchMove(VariX, VariY1, VariY2, true);

                    InitMagArmXYMoveTask();  //jou 2011-04-03
                    if(bXYPosErrorFlag)
                    {
                        iMagArmAllZSafe=150;
                        return false;
                    }
                }
                else
                {
                    ZeroMemory(bFlag, sizeof(bFlag));
                    ZeroMemory(bFlagZ, sizeof(bFlagZ));
                    return false;
                }
            case 100:
                if(bPitchMoveOk==false)
                    bPitchMoveOk=MagArmPitchMove(VariX, VariY1, VariY2);

                if(bPitchMoveOk)
                {
                    ZeroMemory(bFlag, sizeof(bFlag));
                    ZeroMemory(bFlagZ, sizeof(bFlagZ));
                    iMagArmAllZSafe=200;
                }
                break;
            case 150:
                if(bFlag[0]==false)
                    bFlag[0]=MMagArmX->MotorMove(iXPos+100);                    //Frank 20150604 發現阿洲抖一下.....
                if(bFlag[1]==false)
                    bFlag[1]=MMagArmY->MotorMove(iYPos+100);

                if(bFlag[0] && bFlag[1])
                {
                    ZeroMemory(bFlag, sizeof(bFlag));
                    ZeroMemory(bFlagZ, sizeof(bFlagZ));
                    bXYPosErrorFlag=false;
                    iMagArmAllZSafe=100;
                }
                break;
            case 200:
                if(bXYMoveFinish)
                {
                    if(ZNeedDown)
                    {
                        for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
                        {
                            for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                            {
                                if(bZNeedSuck[iR][iC])
                                {
                                    if(bFlagZ[iR][iC]==false)
                                    {
                                        //Sam 20240607 : Suck 預吸吹功能
                                        //==>
                                        MMagArmZ[iR][iC]->ReadPos();
                                        if(HSys.Sen.SnRKManualStep.IsOff()==false ||
                                           fMain->cbMagStep->Checked)
                                        {
                                            //使用 T.Step 時不能預吸吹
                                        }
                                        else
                                        {
                                            if(bPreSuckOn && (abs(iZPos[iR][iC]-iZCurPos)<iPreSuckOn) && bPick==true)
                                                MagArmSuck->Suck[iR][iC].On();
                                            else if(bPreSuckOff && (abs(iZPos[iR][iC]-iZCurPos)<iPreSuckOff) && bPick==false)
                                                MagArmSuck->Suck[iR][iC].Off();
                                        }
                                        //<==
                                        //Sam 20240607 : Suck 預吸吹功能
                                        bFlagZ[iR][iC]=MMagArmZ[iR][iC]->MotorMove(iZPos[iR][iC]);
                                    }
                                }
                                else
                                {
                                    bFlagZ[iR][iC]=true;
                                }
                            }
                        }

                        for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
                        {
                            for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                            {
                                if(bFlagZ[iR][iC]==false)
                                    return false;
                            }
                        }
                    }

                    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                        {
                            if(ZNeedDown &&
                               bZNeedSuck[iR][iC] &&
                               MMagArmZ[iR][iC]->Led[iHomeLed])
                            {
                                MMagArmX->Stop();
                                MMagArmY->Stop();
                                iMagArmAllZSafe=1;
                                bPitchMoveOk=false;
                                ShowMyMessage(MMagArmZ[iR][iC]->NumberAlias+" Home sensor error, if suck is down, maybe sensor fail!");
                                return false;
                            }
                        }
                    }

                    iMagArmAllZSafe=1;
                    InitMagArmXYMoveTask();  //jou 2011-04-03
                    ZeroMemory(bFlag, sizeof(bFlag));
                    bPitchMoveOk=false;
                    return true;
                }

                if(bPitchMoveOk==false)
                {
                    ZeroMemory(bFlag, sizeof(bFlag));
                    iMagArmAllZSafe=1;
                }
                break;
            default :
                iMagArmAllZSafe=1;
                return false;
        }

        if(bXYPosErrorFlag)
            return false;

        if(bXYMoveFinish==false &&                                              //Steven 20231214 : 避免重複進去
           bPitchMoveOk==true)                                                  //避免Pitch Alarm時, XY還在動
        {
            ret=MagArmXYMove(iXPos, iYPos);
            //Sam 20240607 : Suck 預吸吹功能
            //==>
            if(ZNeedDown && bPreZDown && ret!=PNP_DONE)
            {
                iXCurPos=MMagArmX->ReadPos();
                iYCurPos=MMagArmY->ReadPos();
                if(abs(iXCurPos-iXPos)<iPreZDown && abs(iYCurPos-iYPos)<iPreZDown)
                {
                    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                        {
                            if(bZNeedSuck[iR][iC])
                              MMagArmZ[iR][iC]->MotorMove(iZPos[iR][iC]/2);    //Z偷跑只要先跑一半，不要全下去。
                        }
                    }
                }
            }
            //<==
            //Sam 20240607 : Suck 預吸吹功能
        }

        if(ret==PNP_DONE)
        {
            if(bPitchMoveOk)
            {
                iCheckX=MMagArmX->ReadPos();
                iCheckY=MMagArmY->ReadPos();

                if((iXPos!=iCheckX) || (iYPos!=iCheckY))
                {
                    iEncodeX=MMagArmX->ReadEncoderPos();                        //Steven 20110314 : 比對Encoder位置
                    iEncodeY=MMagArmY->ReadEncoderPos();                        //Steven 20110314 : 比對Encoder位置

                    if(iXPos>(iEncodeX+10) || iXPos<(iEncodeX-10) ||
                       iYPos>(iEncodeY+10) || iYPos<(iEncodeY-10))              //Steven 20110314 : 比對Encoder位置
                    {
                        iMagArmAllZSafe=1;
                        bPitchMoveOk=false;
                        MMagArmX->Stop();
                        MMagArmY->Stop();

                        bXYPosErrorFlag=true;
                        return false;
                    }
                }

                bXYMoveFinish=true;

                if(ZNeedDown)
                {
                    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                        {
                            if(bZNeedSuck[iR][iC])
                            {
                                //Sam 20240607 : Suck 預吸吹功能
                                //==>
                                MMagArmZ[iR][iC]->ReadPos();
                                if(HSys.Sen.SnRKManualStep.IsOff()==false || fMain->cbMagStep->Checked)
                                {
                                    //使用 T.Step 時不能預吸吹
                                }
                                else
                                {
                                    if(bPreSuckOn && (abs(iZPos[iR][iC]-iZCurPos)<iPreSuckOn) && bPick==true)
                                        MagArmSuck->Suck[iR][iC].On();
                                    else if(bPreSuckOff && (abs(iZPos[iR][iC]-iZCurPos)<iPreSuckOff) && bPick==false)
                                        MagArmSuck->Suck[iR][iC].Off();
                                }
                                //<==
                                //Sam 20240607 : Suck 預吸吹功能
                                bFlagZ[iR][iC]=MMagArmZ[iR][iC]->MotorMove(iZPos[iR][iC]);
                            }
                        }
                    }
                }

                return false;
            }
        }

        if(OutArmIdle.Off())
        {
            MMagArmX->Stop();
            MMagArmY->Stop();
            iMagArmAllZSafe=1;
            bPitchMoveOk=false;
            OutArmIdle.On();
        }
    }
    else
    {
       //動作模擬
        for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
            {
                if(MagSafeCheck[iR][iC]==false)
                    MagSafeCheck[iR][iC]=MMagArmZ[iR][iC]->MotorMove(10);
            }
        }

        for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
            {
                if(MagSafeCheck[iR][iC]==false)
                    return false;
            }
        }

        bFlag[0]=MMagArmX->MotorMove(iXPos);
        bFlag[1]=MMagArmY->MotorMove(iYPos);
        bFlag[2]=MagArmPitchMove(VariX, VariY1, VariY2);

        if(bFlag[0] && bFlag[1] && bFlag[2])
        {
            for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                {
                    if(ZNeedDown)
                    {
                        if(bZNeedSuck[iR][iC]==false)
                            iZPos[iR][iC]=0;
                    }
                    else
                    {
                        iZPos[iR][iC]=0;
                    }
                    bFlagZ[iR][iC]=MMagArmZ[iR][iC]->MotorMove(iZPos[iR][iC]);
                }
            }

            for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                {
                    if(bFlagZ[iR][iC]==false)
                        return false;
                }
            }

            bFlag[0]=false;
            bFlag[1]=false;
            bFlag[2]=false;
            for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                {
                    bFlagZ[iR][iC]=false;
                    MagSafeCheck[iR][iC]=false;
                }
            }
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void MyMagArmParameter::DoMagArm(int &Task)
{
    AnsiString Str="";
    int iBin=0;
    static int iWhichMag=0;
    MyBinToTrayStruct BtoT;
    CheckMagArmDestroyActive();
    static bool btest=false;

    if(bNeedPickTrayToMagTop || bMagNeedAutoLoadTray)
        return;

    if(btest)
    {
        MMagSortTray->fHasTray=true;
        MMagSortTray->SetTrayVisible(true);
        fMain->sbPause->Click();
    }

    switch(Task)
    {
        case 1:
            if(MagArmSuck->HasIC() &&                                           //JerryYang 20240817 : ARM上已經有IC就先放料, 不要重新search
               MMagSortTopTray[eMagazine2]->fHasTray==true &&
               MagazineModule[eMagazine2]->mTopTray.iBin==mNowBin.iBin)
            {
                DoPnPToMagTop(true,eMagazine2);
                bNeedPlaceFirst=true;                                           //KenHsieh(JerryYang) 20240826 : ARM上已經有IC就先放料, 不要重新search
                Task=1100;
                break;
            }
            else if(MagArmSuck->HasIC() &&                                      //JerryYang 20240817 : ARM上已經有IC就先放料, 不要重新search
                    MMagSortTopTray[eMagazine1]->fHasTray==true &&
                    MagazineModule[eMagazine1]->mTopTray.iBin==mNowBin.iBin)
            {
                DoPnPToMagTop(true,eMagazine1);
                bNeedPlaceFirst=true;                                           //KenHsieh(JerryYang) 20240826 : ARM上已經有IC就先放料, 不要重新search
                Task=1100;
                break;
            }
            else if(MagArmSuck->HasIC() &&                                      //JerryYang 20240817 : ARM上已經有IC就先放料, 不要重新search
                    MMagSortTopTray[eMagazine3]->fHasTray==true &&
                    MagazineModule[eMagazine3]->mTopTray.iBin==mNowBin.iBin)
            {
                DoPnPToMagTop(true,eMagazine3);
                bNeedPlaceFirst=true;                                           //KenHsieh(JerryYang) 20240826 : ARM上已經有IC就先放料, 不要重新search
                Task=1100;
                break;
            }
            else if(HSys.Sys.RunMode==Run_OneCycle &&
                    MagArmSuck->HasRealIC()==false)                             //Steven 20240707 : Fixed for one cycle
            {
                bSortArm2OneCycleFinish=true;
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed)
            {
                if(MMagArmX->ReadEncoderPos()!=0 ||
                   MMagArmX->ReadEncoderPos()!=0)
                {
                    Task=8000;
                }
                else
                {
                    iMagStatus=eHS_MAG_SafePos;
                }
            }
            else if(MMagSortTray->fHasTray==true)
            {
                mNowBin.ConvertBinToAuto(0);
                bNeedLoadTrayToTransfer=false;
                Task=100;
            }
            else //if(MMagSortTray->fHasTray==false)
            {
                if(MMagSortTray->fHasTray==false &&
                   MagTransferLD->fHasTray==false &&
                   MagArmSuck->HasIC()==false)                                  //JerryYang 20240615 : fix還沒做完就跳CLEAN OUT FINISH
                {
                    if(bFlagAutoArmCleanoutFinish==true)
                        bFlagMagArmCleanoutFinish=true;
                    else
                        bFlagMagArmCleanoutFinish=false;
                }
            }
            break;
        case 100:
            if(HSys.Sys.RunMode==Run_OneCycle &&
               MagArmSuck->HasRealIC()==false)                                  //Steven 20240707 : Fixed for one cycle
            {
                Task=1;
            }
            else if(MMagSortTray->fHasTray)
            {
                if(MMagSortTray->HasIC() || btest)
                {
                    Task=1000;
                }
                else
                {
                    Task=5000;
                }
            }
            else if(bFlagAutoArmCleanoutFinish==true)
            {
                Task=1;
            }
            break;
        case 1000:
            if(MagTopTrayHasMapBin(iWhichMag))
            {
                SetWorkBin(MagazineModule[iWhichMag]->mTopTray.iBin);
                DoPnPToMagTop(iWhichMag, true);
                Task=1100;
            }
            else
            {
                Task=1200;
            }
            break;
        case 1100:
            if(DoPnPToMagTop(iWhichMag))
            {
                Task=1000;
            }
            break;
        case 1200:
            //AI(HT172-Maintainer) 20260415 : Round 1 - prefer magazine whose Top already has matching bin tray (zero swap wait)
            if(CosFunction.bMagCrossMagTopReadyFirst && MAGAZINE_CNT>1)
            {
                iBin=Find_SortBin_Mag_TopReady(iWhichMag);
            }
            //Round 2 - try different magazine (original cross-mag logic)
            if(iBin<=0 && MAGAZINE_CNT>1)
            {
                iBin=Find_SortBin_Mag_Random(iWhichMag,true);
            }
            if(iBin<=0)
                iBin=Find_SortBin_Mag_Random(iWhichMag,false);
            if(iBin>0)
            {
                Find_SortBin_Mag(iBin);
                SetWorkBin(iBin);
                DoPnPToMagTop(iWhichMag, true);
                Task=2000;
            }
            else
            {
                Task=5000;
            }
            break;
        case 2000:
            if(DoPnPToMagTop(iWhichMag))
            {
                Task=1200;
            }
            break;
        case 5000:
            mBinToTray.ConvertBinToAuto(0);
            mNowBin.ConvertBinToAuto(0);

            if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eWinstekFormat || HSys.LastSet.iRealDummy==HAS_TRAY)
            {

            }
            else
            {
                if(MMagSortTray->fHasTray && MMagSortTray->HasIC())
                {
                    ShowMyMessage("Mag LD 有殘留IC");
                }
            }
            iEmptyMagStatus=eHS_EM_WorkTrayOut;
            Task=7000;
            break;
        case 7000:
            if(LoaderModule->iMagArmStatus==(int)LoaderModule->eHS_MA_WorkTrayOutReady)
            {
                iEmptyMagStatus=eHS_EM_Idle;
                Task=1;
            }
            break;
        case 8000:
            if(MoveMagArmToAutoSafe())
            {
                bXMoveFinish=false;
                bYMoveFinish=false;
                Task=8500;
            }
            break;
        case 8500:
            if(bXMoveFinish==false)
                bXMoveFinish=MMagArmX->MotorMove(0);
            if(bYMoveFinish==false)
                bYMoveFinish=MMagArmY->MotorMove(0);

            if(bXMoveFinish && bYMoveFinish)
            {
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::DoPickICFromMagLoader(int iWhichMag, bool bReset)
{
    bool bResult=false;
    AnsiString ErrorPart;
    static int iRetryCT=0, iWhitchSuckR=0, iWhitchSuckC=0;
    int iTrayPosRow=0, iTrayPosCol=0, iRet=0;
    if(bReset)
    {
        iPickFromMagLoaderTask=1;
        iRetryCT=0;
        iWhitchSuckR=0;
        iWhitchSuckC=0;
        return false;
    }

    int &Task=iPickFromMagLoaderTask;

    switch(Task)
    {
        case 1:
            if(CheckHasSpaceToPlace(iWhichMag,iUseSuckNo))
            {
                Task=100;
                MagArmSuckReset();                                              //JerryYang 20240623 : pick up error可以SKIP
            }
            else
            {
                Task=50;
            }
            break;
        case 50:
            bResult=true;
            break;
        case 100:
            if(MMagNowSortTray->HasIC() && CheckZNeedSuck(iUseSuckNo))
            {
                iRetryCT=0;
                Task=200;
            }
            else
            {
                Task=50;
            }
            break;
        case 200:
            if(MoveMagArmXYToPick())
            {
                if(HSys.Sen.SnRKManualStep.IsOff()==false ||
                   fMain->cbMagStep->Checked)                                   //JerryYang 20240528
                {
                    fMain->sbPause->Click();
                    Task=400;
                }
                else
                {
                    MagArmSuck->ResetAll();                                     //JerryYang 20240623 : pick up error可以SKIP
                    Task=300;
                }
            }
            break;
        case 300:
            flag=true;
            for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                {
                    if(bMagArmSuckActive[iR][iC] &&
                       MagArmSuck->Suck[iR][iC].Item==NULL_IC)
                    {
                        if(bMagSuckEnd[iR][iC]==false &&
                           MagArmSuck->Suck[iR][iC].Suck())                     //JerryYang 20240623 : pick up error可以SKIP
                        {
                            GetMapTrayPosFromEncorder(iMagArmMapTrayPos[iR][iC], iTrayPosRow, iTrayPosCol);
                            AddLoadingCount(iR, iC, iTrayPosCol, iTrayPosRow);
                            bMagArmSuckActive[iR][iC]=false;
                        }
                        else if(bMagSuckEnd[iR][iC]==false &&
                                MagArmSuck->Suck[iR][iC].Error==false)
                        {
                            flag=false;
                        }
                        else if(MagArmSuck->Suck[iR][iC].Error==true)
                        {
                            bMagSuckEnd[iR][iC]=true;
                        }
                    }
                    else
                    {
                        bMagSuckEnd[iR][iC]=true;
                    }
                }
            }

            if(flag==false)
                break;

            for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                {
                    if(MagArmSuck->Suck[iR][iC].Error==true)
                    {
                        iRetryCT++;
                        if(iRetryCT>3)                                          //Steven 20141113 : Loader SKIP時在原地Retry
                        {
                            iWhitchSuckR=iR;
                            iWhitchSuckC=iC;
                            iRetryCT=0;
                            Task=350;
                        }
                        else                                                    //JerryYang 20240601 : Fixed for MGZ
                        {
                            Task=400;
                        }
                        return false;
                    }
                }
            }

            if(flag)
            {
                iRetryCT=0;                                                     //KenHsieh 20240612 : 歸零後流程才正常
                Task=400;
            }
            break;
        case 350:
            if(MoveMagArmToAutoSafe())
            {
                ErrorPart=MagArmSuck->GetErrorPart(Vaccum_On);
                iRet=ShowSuckError(HSys.Suck.MagArmSuck, Vaccum_On, K_RETRY|K_SKIP|K_TRAY_END|K_HOME, ErrorPart, 0);  //JerryYang 20240623 : pick up error可以SKIP

                if(iRet==K_RETRY)
                {
                    Task=400;
                    MagArmSuck->ResetAll();
                }
                else if(iRet==K_HOME)                                           //JerryYang 20240704 : Pick up error Y軸一起回HOME
                {
                    Task=400;
                    bMagArmNeedHome=true;
                    fHome->SortArm2ZHome(true);
                }
                else if(iRet==K_SKIP)
                {
                    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                        {
                            if(MagArmSuck->Suck[iR][iC].Error==true)
                            {
                                GetMapTrayPosFromEncorder(iMagArmMapTrayPos[iR][iC], iTrayPosRow, iTrayPosCol);
                                MMagNowSortTray->SetTraySingleData(iTrayPosCol, iTrayPosRow, NULL_IC);
                                MMagSortTray->SetTraySingleData(iTrayPosCol, iTrayPosRow, NULL_IC);
                            }
                        }
                    }
                    Task=1;
                    return true;
                }
                else if(iRet==K_TRAY_END)
                {
                    MMagSortTray->InitNewTray(NULL_IC);
                    MMagNowSortTray->InitNewTray(NULL_IC);
                    ZeroMemory(iTrayIC, sizeof(iTrayIC));
                    Task=400;
                }
            }
            break;
        case 400:
            if(MoveMagArmToAutoSafe())
            {
                MagArmSuckReset();                                              //JerryYang 20240623 : pick up error可以SKIP
                MagArmSuck->ClearAllError();
                if(iRetryCT>0)
                    Task=200;
                else
                    Task=100;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::DoMagArmPlaceToTop(int iTray, int iWhichMag)
{
    static bool bDuplicateErr10000=false;                                       //Steven 20091218 : Avoid duplicate message
    AnsiString ErrorPart="";
    bool bHasErr=false;
    int iTrayPosRow=0, iTrayPosCol=0;
    MyBinToTrayStruct BtoT;
    AnsiString sErrMes="";
    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(bMagArmSuckActive[iR][iC])
            {
                if(MagArmSuck->Suck[iR][iC].Destroy())
                {
                    BtoT.ConvertAutoToBin(iTray);
                    GetMapTrayPosFromEncorder(iMagArmMapTrayPos[iR][iC], iTrayPosRow, iTrayPosCol);
                    MMagSortTopTray[iWhichMag]->SetTraySingleData(iTrayPosCol, iTrayPosRow, HAS_IC, MagArmSuck->Suck[iR][iC].DeviceInfo);
                    MMagSortTopTray[iWhichMag]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].AddOutputInfo(iTray,MMagSortTopTray[iWhichMag]->Tray.ClipID, iTrayPosCol, iTrayPosRow);
                    if(atoi(MMagSortTopTray[iWhichMag]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].SoftBin.c_str())!=BtoT.iBin)  //Sam 20250903 : 避免上游機台 SoftBin 資料缺失
                    {
                        MMagSortTopTray[iWhichMag]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].SoftBin=AnsiString(BtoT.iBin);
                    }
                    if(MMagSortTopTray[iWhichMag]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].iBin!=BtoT.iBin)                   //Sam 20250903 : 增加分Bin異常保護
                    {
                        sErrMes.sprintf("Place to %s is bin%d", fMain->sTrayNameBin[iTray], MMagSortTopTray[iWhichMag]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].iBin);
                        ShowSystemError("80006", K_SKIP, 0, sErrMes);
                    }
                    MMagSortTopTray[iWhichMag]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].SaveRecord();
                    MagArmSuck->SetItemData(iR, iC, NULL_IC);
                    MagArmSuck->Suck[iR][iC].DeviceInfo.Clear();

                    bMagArmSuckActive[iR][iC]=false;
                    bMagArmCheckDestroyACT[iR][iC]=true;                        //jou 981130 確認device確實destroy完成

                    tRunData.BinICCnt[BtoT.iBin]++;
                    tRunData.TrayICCnt[iTray]++;
                    tRunData.TotalIC++;
                }

                if(MagArmSuck->Suck[iR][iC].Error)
                    bHasErr=true;
            }
        }
    }

    if(bHasErr)
    {
        ErrorPart=MagArmSuck->GetErrorPart(Vaccum_Off);
        ShowSuckError(*MagArmSuck, Vaccum_Off, K_RETRY, ErrorPart, bDuplicateErr10000);

        bDuplicateErr10000=true;
    }
    else
    {
        bDuplicateErr10000=false;
    }

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(bMagArmSuckActive[iR][iC])
                return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::DoPlaceICToMagTop(int iWhichMag, bool bReset)
{
    bool bResult=false;
    if(bReset)
    {
        iPlaceToTopMagTask=1;
        return false;
    }

    int &Task=iPlaceToTopMagTask;
    switch(Task)
    {
        case 1:

            if(CheckMagArmSameBin()==false)                                     //JerryYang 20240817 : 檢查ARM上的BIN必須相同
            {
                if(ManualTakeOutMagArmDevice()==false)
                {
                    break;
                }
                else
                {
                    Task=50;
                    break;
                }
            }
            else if(CheckMagArmBin(mBinToTray.iBin)==false || MagazineModule[iWhichMag]->mTopTray.iBin!=mBinToTray.iBin)
            {
                if(ManualTakeOutMagArmDevice()==false)
                {
                    break;
                }
                else
                {
                    Task=50;
                    break;
                }
            }
            if(CheckHasSpaceToPlace(iWhichMag, iUseSuckNo))
            {
                Task=100;
            }
            else
            {
                Task=9900;
            }
            break;
        case 100:
            if(MoveMagArmXYToPlace(iWhichMag))
            {
                if(HSys.Sen.SnRKManualStep.IsOff()==false ||
                   fMain->cbMagStep->Checked)                                   //JerryYang 20240528
                {
                    fMain->sbPause->Click();
                    Task=500;
                }
                else
                {
                    Task=200;
                }
            }
            break;
        case 200:
            if(DoMagArmPlaceToTop(mBinToTray.iWhichAuto, iWhichMag))
            {
                Task=300;
            }
            break;
        case 300:
            if(MoveMagArmToAutoSafe())
            {
                if(CheckZNeedPlace(8))
                    Task=100;
                else
                    Task=9900;
            }
            break;
        case 500:
            if(MoveMagArmToAutoSafe())
            {
                Task=100;
            }
            break;
        case 9900:
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::CheckHasSpaceToPlace(int iWhichMag, int &iCount)
{
    iCount=0;
    if(MMagSortTopTray[iWhichMag]->fHasTray)
    {
        for(int i=0; i<iTrayXDivision; i++)
        {
            for(int j=0; j<iTrayYDivision; j++)
            {
                if(MMagSortTopTray[iWhichMag]->Tray.Data[i][j]==NULL_IC)
                {
                    iCount++;
                }
            }
        }
    }

    if(iCount>0)
    {
        if(iCount<8)
            bOneByOnePick=true;                                                 //只吸取跟Tray剩餘IC一樣多的位置
        else
            bOneByOnePick=false;
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
int MyMagArmParameter::Find_SortBinCount_Mag(int *iTrayIC, int iPreLoad)
{
    int iTray=0;
    int iTrayICCount[eTrayCount];
    int iBin;
    ZeroMemory(iTrayICCount, sizeof(iTrayICCount));
    MyBinToTrayStruct BtoT;

    if(iPreLoad==eMagPreloadTransfer)
    {
        if(MagTransferLD->fHasTray)                                             //預判載台上的Tray
        {
            for(int i=0; i<iTrayXDivision; i++)
            {
                for(int j=0; j<iTrayYDivision; j++)
                {
                    if(MagTransferLD->Tray.Data[i][j]!=NULL_IC)
                    {
                        iBin=MagTransferLD->Tray.DeviceInfo[i][j].iBin;
                        BtoT.ConvertBinToAuto(iBin);
                        iTray=BtoT.iWhichAuto;
                        if(iTray>eBinNotUse && iTray<eTrayCount)
                        {
                            iTrayICCount[iTray]++;
                        }
                        else
                        {
                            iBin=0;
                        }
                    }
                }
            }
        }
    }
    else if(iPreLoad==eMagPreloadLoadY)
    {
        if(LoaderModule->MotWorkLoader->fHasTray)                               //預判Loader上的Tray
        {
            for(int i=0; i<iTrayXDivision; i++)
            {
                for(int j=0; j<iTrayYDivision; j++)
                {
                    if(LoaderModule->MotWorkLoader->Tray.Data[i][j]!=NULL_IC)
                    {
                        iBin=LoaderModule->MotWorkLoader->Tray.DeviceInfo[i][j].iBin;
                        BtoT.ConvertBinToAuto(iBin);
                        iTray=BtoT.iWhichAuto;
                        if(iTray>eBinNotUse && iTray<eTrayCount)
                        {
                            iTrayICCount[iTray]++;
                        }
                        else
                        {
                            iBin=0;
                        }
                    }
                }
            }
        }
    }
    else if(iPreLoad==eMagPreloadLoadCar)
    {
        if(LoaderModule->MotLoader_Car->fHasTray &&                             //預判Loader Car上的Tray
           LoaderModule->MotLoader_Car->Tray.ClipID!="")
        {
            for(int i=0; i<iTrayXDivision; i++)
            {
                for(int j=0; j<iTrayYDivision; j++)
                {
                    if(LoaderModule->MotLoader_Car->Tray.Data[i][j]!=NULL_IC)
                    {
                        iBin=LoaderModule->MotLoader_Car->Tray.DeviceInfo[i][j].iBin;
                        BtoT.ConvertBinToAuto(iBin);
                        iTray=BtoT.iWhichAuto;
                        if(iTray>eBinNotUse && iTray<eTrayCount)
                        {
                            iTrayICCount[iTray]++;
                        }
                        else
                        {
                            iBin=0;
                        }
                    }
                }
            }
        }
    }
    else
    {
        for(int i=0; i<iTrayXDivision; i++)
        {
            for(int j=0; j<iTrayYDivision; j++)
            {
                if(MMagSortTray->Tray.Data[i][j]!=NULL_IC)
                {
                    iBin=MMagSortTray->Tray.DeviceInfo[i][j].iBin;
                    BtoT.ConvertBinToAuto(iBin);
                    iTray=BtoT.iWhichAuto;
                    if(iTray>eBinNotUse && iTray<eTrayCount)
                    {
                        iTrayICCount[BtoT.iWhichAuto]++;
                    }
                    else
                    {
                        iBin=0;
                    }
                }
            }
        }
    }

    for(int iT=eBinNotUse; iT<eTrayCount; iT++)
    {
        iTrayIC[iT]=iTrayICCount[iT];
    }

    return 0;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::Find_SortBin_Mag(int iBin)
{
    MMagNowSortTray->InitNewTray(NULL_IC);
    MMagNowSortTray->Tray.iBin=0;
    int iBinDevice=0;
    bool bHasBin=false;
    for(int i=0; i<iTrayXDivision; i++)
    {
        for(int j=0; j<iTrayYDivision; j++)
        {
            if(MMagSortTray->Tray.Data[i][j]==HAS_IC)                           //KenHsieh 20240514 : 有HAS_IC才進來
            {
                iBinDevice=MMagSortTray->Tray.DeviceInfo[i][j].iBin;
                if(iBinDevice>0 && iBinDevice<=TEST_MAX_BIN && iBinDevice==iBin)     //JerryYang 20240918 : modify
                {
                    MMagNowSortTray->SetTraySingleData(i, j, HAS_IC, MMagSortTray->Tray.DeviceInfo[i][j]);
                    MMagNowSortTray->Tray.iBin=iBinDevice;
                    bHasBin=true;
                }
                else
                {
                    MMagNowSortTray->SetTraySingleData(i, j, NULL_IC);
                }
            }
        }
    }
    return bHasBin;
}
//------------------------------------------------------------------------------
int MyMagArmParameter::Find_SortBin_Mag_Random(int &iWhichMag,bool bFindNexMag)
{
    int iBin=0;
    if(CosFunction.bMagCrossMagTopReadyFirst)
    {
        int iFirstBin=0;
        int iFirstMag=-1;
        for(int i=0; i<iTrayXDivision; i++)
        {
            for(int j=0; j<iTrayYDivision; j++)
            {
                if(MMagSortTray->Tray.Data[i][j]==HAS_IC)
                {
                    iBin=MMagSortTray->Tray.DeviceInfo[i][j].iBin;
                    if(GetWhichMag(iBin, iWhichMag)==true)
                    {
                        if(bFindNexMag==true &&
                           iWhichMag==mBinToTray.iWhichAuto)
                        {
                            continue;
                        }
                        if(iFirstBin<=0)
                        {
                            iFirstBin=iBin;
                            iFirstMag=iWhichMag;
                        }
                        if(iWhichMag!=gLastPickedMag)
                        {
                            gLastPickedMag=iWhichMag;
                            return iBin;
                        }
                    }
                }
            }
        }
        if(iFirstBin>0)
        {
            iWhichMag=iFirstMag;
            gLastPickedMag=iWhichMag;
            return iFirstBin;
        }
        return 0;
    }
    for(int i=0; i<iTrayXDivision; i++)
    {
        for(int j=0; j<iTrayYDivision; j++)
        {
            if(MMagSortTray->Tray.Data[i][j]==HAS_IC)
            {
                iBin=MMagSortTray->Tray.DeviceInfo[i][j].iBin;
                if(GetWhichMag(iBin, iWhichMag)==true)
                {
                    if(bFindNexMag==true &&
                       iWhichMag==mBinToTray.iWhichAuto)
                    {
                        continue;
                    }
                    return iBin;
                }
            }
        }
    }
    iBin=0;
    return iBin;
}
//------------------------------------------------------------------------------
//AI(HT172-Maintainer) 20260415 : cross-mag Top-ready priority scan
int MyMagArmParameter::Find_SortBin_Mag_TopReady(int &iWhichMag)
{
    int iBin=0;
    int iMag=0;
    int iFirstBin=0;
    int iFirstMag=-1;
    for(int i=0; i<iTrayXDivision; i++)
    {
        for(int j=0; j<iTrayYDivision; j++)
        {
            if(MMagSortTray->Tray.Data[i][j]==HAS_IC)
            {
                iBin=MMagSortTray->Tray.DeviceInfo[i][j].iBin;
                if(iBin>0 && iBin<=TEST_MAX_BIN && GetWhichMag(iBin, iMag))
                {
                    if(MMagSortTopTray[iMag]->fHasTray==true &&
                       MagazineModule[iMag]->mTopTray.iBin==iBin)
                    {
                        if(CosFunction.bMagCrossMagTopReadyFirst)
                        {
                            if(iFirstBin<=0)
                            {
                                iFirstBin=iBin;
                                iFirstMag=iMag;
                            }
                            if(iMag!=gLastPickedMag)
                            {
                                iWhichMag=iMag;
                                gLastPickedMag=iWhichMag;
                                return iBin;
                            }
                        }
                        else
                        {
                            iWhichMag=iMag;
                            return iBin;
                        }
                    }
                }
            }
        }
    }
    if(iFirstBin>0)
    {
        iWhichMag=iFirstMag;
        gLastPickedMag=iWhichMag;
        return iFirstBin;
    }
    return 0;
}
//------------------------------------------------------------------------------
int MyMagArmParameter::Find_MagArm_PlaceMaxUseCountOnTime(TTrayMotor *Motor, int iStepRow, int iStepCol, bool bPick, bool bZDown)   //Sam 20240727 : Magazine 換 Tray 時XY提前移動
{
    int trayIndexRow=0, trayIndexCol=0;
    int iCT=0, itStartX=-1, itStartY=-1;
    int ixStart=0, ixEnd=0, ixStep=0;
    int iyStart=0, iyEnd=0, iyStep=0;
    int iCheckICStatus=bPick?HAS_IC:NULL_IC;
    int Direction=0;

    if(Direction>=4)
        Direction=0;
    ixStart=0;
    ixEnd=Motor->Tray.XItem;
    ixStep=1;
    iyStart=0;
    iyEnd=Motor->Tray.YItem;
    iyStep=1;
    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            bMagArmSuckActive[iR][iC]=false;
            iMagArmMapTrayPos[iR][iC]=0;
        }
    }

    for(int y=iyStart; y!=iyEnd; y+=iyStep)
    {
        for(int x=ixStart; x!=ixEnd; x+=ixStep)
        {
            if(Motor->Tray.Data[x][y]==iCheckICStatus)
            {
                itStartX=x;
                itStartY=y;
                break;
            }
        }
        if(itStartX>-1 || itStartY>-1)
            break;
    }

    if(itStartX==-1 || itStartY==-1)                                            //JerryYang 20240603 : 避免itStartX錯誤
        return iCT;

    iCT=0;
    iActSuckR=-1;
    iActSuckC=-1;

    int iHasICCnt=0;
    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(MagArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                iHasICCnt++;
            }
        }
    }

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(MagArmSuck->Suck[iR][iC].Item==iCheckICStatus)
            {
                continue;
            }
            else
            {
                if(iActSuckR==-1 || iActSuckC==-1)
                {
                    iActSuckR=iR;
                    iActSuckC=iC;
                }
                trayIndexCol=itStartX+(iC-iActSuckC)*iStepCol;
                trayIndexRow=itStartY+(iR-iActSuckR)*iStepRow;

                if(bPick)
                {
                    if(iCT>=iUseSuckNo-iHasICCnt)
                        return iCT;
                }

                if(trayIndexRow>=0 && trayIndexCol>=0 &&
                   trayIndexCol<Motor->Tray.XItem &&
                   trayIndexRow<Motor->Tray.YItem &&
                   Motor->Tray.Data[trayIndexCol][trayIndexRow]==iCheckICStatus)
                {
                    iCT++;
                    if(bZDown) //Sam 20240727 : Magazine 換 Tray 時XY提前移動
                    bMagArmSuckActive[iR][iC]=true;
                    iMagArmMapTrayPos[iR][iC]=GetMapTrayPosFromRowCol(trayIndexRow, trayIndexCol);

                    if((bOneByOnePick && iCT==iUseSuckNo) ||                    //只吸取跟Tray剩餘IC一樣多的位置
                        (iByLotTrayCnt[mBinToTray.iWhichAuto]>0 &&              //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞
                        (iByLotTrayCnt[mBinToTray.iWhichAuto]-tRunData.TrayICCnt[mBinToTray.iWhichAuto]<MagArmPara->iMaxSpaceOnePlace)))
                        return iCT;

                }
            }
        }
    }
    return iCT;
}
//---------------------------------------------------------------------------
void MyMagArmParameter::Find_MagArm_Single(TTrayMotor *Motor, int &iX, int &iY)
{
    int ixStart=0, ixEnd=0, ixStep=0;
    int iyStart=0, iyEnd=0, iyStep=0;
    int Direction=0;

    int itStartX=-1;
    int itStartY=-1;

    if(Direction>=4)
        Direction=0;

    if(Direction<4)
    {
        if(Direction==0)
        {
            ixStart=0;
            ixEnd=Motor->Tray.XItem;
            ixStep=1;

            iyStart=0;
            iyEnd=Motor->Tray.YItem;
            iyStep=1;
        }
        else if(Direction==1)
        {
            ixStart=Motor->Tray.XItem-1;
            ixEnd=-1;
            ixStep=-1;

            iyStart=0;
            iyEnd=Motor->Tray.YItem;
            iyStep=1;
        }
        else if(Direction==2)
        {
            ixStart=0;
            ixEnd=Motor->Tray.XItem;
            ixStep=1;

            iyStart=Motor->Tray.YItem-1;
            iyEnd=-1;
            iyStep=-1;
        }
        else
        {
            ixStart=Motor->Tray.XItem-1;
            ixEnd=-1;
            ixStep=-1;

            iyStart=Motor->Tray.YItem-1;
            iyEnd=-1;
            iyStep=-1;
        }

        for(int y=iyStart; y!=iyEnd; y+=iyStep)
        {
            for(int x=ixStart; x!=ixEnd; x+=ixStep)
            {
                if(Motor!=MMagNowSortTray)
                {
                    if(Motor->Tray.Data[x][y]==NULL_IC)
                    {
                        itStartX=x;
                        itStartY=y;
                        break;
                    }
                }
                else
                {
                    if(Motor->Tray.Data[x][y]==HAS_IC)
                    {
                        itStartX=x;
                        itStartY=y;
                        break;
                    }
                }
            }
            if(itStartX!=-1 && itStartY!=-1)
                break;
        }
    }

    if(Motor!=MMagNowSortTray)
    {
        iX=itStartX;
        iY=itStartY;

        iMagArmXPosition=itStartX;
        iMagArmYPosition=itStartY;
        return;
    }
    else
    {
        for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
            {
                bMagArmSuckActive[iR][iC]=false;
            }
        }

        for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
        {
           for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
            {
                if(fSetup->rgUseSuck->ItemIndex==1 && bSortArm2UseSuck[iR][iC]==false)
                {
                    continue;
                }
                else
                {
                    if(MagArmSuck->Suck[iR][iC].Item==NULL_IC)
                    {
                        bMagArmSuckActive[iR][iC]=true;
                        iX=itStartX;
                        iY=itStartY;
                        return;
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
int MyMagArmParameter::AutoCalculateOutArmClosePitch()
{
    return 6000;
}
//------------------------------------------------------------------------------
int MyMagArmParameter::AutoCalculateOutArmClosePitchCol(int &iStep)
{
    int x=0, i=0;
    do
    {
        i++;
        x=iTrayXPitch*i;
        if(x>=IN_OUT_ARM_X_PITCH_MIN)
            break;
    }while(x<IN_OUT_ARM_X_PITCH_MAX && iTrayXPitch!=0);
    if(x>=IN_OUT_ARM_X_PITCH_MIN && x<=IN_OUT_ARM_X_PITCH_MAX && iTrayXPitch>0)
    {
        iStep=x/iTrayXPitch;
        return x;
    }
    iStep=0;
    return 6000;
}
//------------------------------------------------------------------------------
int MyMagArmParameter::AutoCalculateOutArmClosePitchRow(int &iStep)
{
    int y=0, i=0;
    do
    {
        i++;
        y=iTrayYPitch*i;
        if(y>=IN_OUT_ARM_Y_PITCH_MIN)
            break;
    }while(y<IN_OUT_ARM_Y_PITCH_MAX && iTrayYPitch!=0);
    if(y>=IN_OUT_ARM_Y_PITCH_MIN && y<=IN_OUT_ARM_Y_PITCH_MAX && iTrayYPitch>0)
    {
        iStep=(y/iTrayYPitch)/3;
        return y;
    }
    iStep=0;
    return 12000;
}
//==============================================================================
// 取得判斷入料變 pitch 距離
//==============================================================================
int MyMagArmParameter::GetMagArmPitchY1(int w)
{
    double m,r;
    m= ((double)(iPos_Y120Pitch1-iPos_Y40Pitch1))/6000.0;                       //Steven 20131002 : XY變距
    r=(double)iPos_Y40Pitch1+m*((double)w-6000.0);
    return (int)r;
}
//==============================================================================
int MyMagArmParameter::GetMagArmPitchY2(int w)
{
    double m,r;
    m= ((double)(iPos_Y120Pitch2-iPos_Y40Pitch2))/6000.0;                       //Steven 20131002 : XY變距
    r=(double)iPos_Y40Pitch2+m*((double)w-6000.0);
    return (int)r;
}
//==============================================================================
int MyMagArmParameter::GetMagArmPitchX(int w)
{
    double m,r;
    m= ((double)(iPos_X60Pitch-iPos_X15Pitch))/4500.0;                          //Steven 20131002 : XY變距
    r=(double)iPos_X15Pitch+m*((double)w-1500.0);
    return (int)r;
}
//==============================================================================
bool MyMagArmParameter::DoMoveMagArmXYToPick(int iRowPitch, int iColPitch, bool ReadMove)
{
    int iXPos=0, iYPos=0;//, Ppos=0, iWhichTray=0;
    int iVari[3]={0, 0, 0};
    int iXpitch=0,iYPitch1=0, iYPitch2=0;
    int iTrayPosRow=0, iTrayPosCol=0;

    if(iActSuckR==-1 || iActSuckC==-1)
    {
        return true;
    }
    //to tray cell(0,0)
    iXPos=iPos_MagLoader_X-1000+iTrayXStart;
    iYPos=iPos_MagLoader_Y+1000-iTrayYStart;

    //to tray cell(iy,ix)
    GetMapTrayPosFromEncorder(iMagArmMapTrayPos[iActSuckR][iActSuckC], iTrayPosRow, iTrayPosCol);
    iXPos+=(iTrayPosCol*iTrayXPitch);
    iYPos-=(iTrayPosRow*iTrayYPitch);

    //suck xy to cell(iy,ix)
    iXPos-=(iActSuckC-iBaseSuckX)*iColPitch;
    iYPos+=(iActSuckR-iBaseSuckY)*iRowPitch/3;

    iAutoX=iTrayPosCol;
    iAutoY=iTrayPosRow;

    iXpitch =iColPitch+atoi(fOffset->edSortArm2PitchX_Pick->Text.c_str());
    iYPitch1=iRowPitch+atoi(fOffset->edSortArm2PitchY1_P_120mm_Pick->Text.c_str());
    iYPitch2=iRowPitch+atoi(fOffset->edSortArm2PitchY2_P_120mm_Pick->Text.c_str());

    iVari[0]=GetMagArmPitchX(iXpitch);
    iVari[1]=GetMagArmPitchY1(iYPitch1);
    iVari[2]=GetMagArmPitchY2(iYPitch2);

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(bMagArmSuckActive[iR][iC])
            {
                bZNeedSuck[iR][iC]=true;
                iZPos[iR][iC]     =iPos_MagLoader_Z[iR][iC];
            }
            else
            {
                bZNeedSuck[iR][iC]=false;
                iZPos[iR][iC]     =10;
            }
        }
    }
    if(IsCheckMagArmDestroyActiveFinish(iXPos, iYPos)==false)                   //Sam 20240627 : 回吸偵測做完才能去吸 IC
        return false;

    if(MagArmContinuousMove(iXPos, iYPos, iVari[0], iVari[1], iVari[2], true, true))
    {
        return true;
    }
    return false;
}
//==============================================================================
bool MyMagArmParameter::DoMoveMagArmXYToPlace(int iRowPitch, int iColPitch, bool ReadMove, int iWhichMag)
{
    int iXPos=0, iYPos=0;
    int iVari[3]={0, 0, 0};
    int iXpitch=0, iYPitch1=0, iYPitch2=0;
    int iTrayPosRow=0, iTrayPosCol=0;

    if(iActSuckR==-1 || iActSuckC==-1)
    {
        return true;
    }
    //to tray cell(0,0)
    iXPos=iPos_MagTop_X[iWhichMag]-1000+iTrayXStart;
    iYPos=iPos_MagTop_Y[iWhichMag]+1000-iTrayYStart;

    //to tray cell(iy,ix)
    GetMapTrayPosFromEncorder(iMagArmMapTrayPos[iActSuckR][iActSuckC], iTrayPosRow, iTrayPosCol);
    iXPos+=(iTrayPosCol*iTrayXPitch);
    iYPos-=(iTrayPosRow*iTrayYPitch);

    //suck xy to cell(iy,ix)
    iXPos-=(iActSuckC-iBaseSuckX)*iColPitch;
    iYPos+=(iActSuckR-iBaseSuckY)*iRowPitch/3;

    iAutoX=iTrayPosCol;
    iAutoY=iTrayPosRow;

    iXpitch =iColPitch+atoi(fOffset->edSortArm2PitchX_Place->Text.c_str());
    iYPitch1=iRowPitch+atoi(fOffset->edSortArm2PitchY1_P_120mm_Place->Text.c_str());
    iYPitch2=iRowPitch+atoi(fOffset->edSortArm2PitchY2_P_120mm_Place->Text.c_str());

    iVari[0]=GetMagArmPitchX(iXpitch);
    iVari[1]=GetMagArmPitchY1(iYPitch1);
    iVari[2]=GetMagArmPitchY2(iYPitch2);

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(bMagArmSuckActive[iR][iC])
            {
                bZNeedSuck[iR][iC]=true;
                iZPos[iR][iC]     =iPos_MagTop_Z[iWhichMag][iR][iC];
            }
            else
            {
                bZNeedSuck[iR][iC]=false;
                iZPos[iR][iC]     =10;
            }
        }
    }

    if(MagArmContinuousMove(iXPos, iYPos, iVari[0], iVari[1], iVari[2], ReadMove, false))
    {
        return true;
    }
    return false;
}
//==============================================================================
bool MyMagArmParameter::MoveMagArmXYToPick()
{
    Find_MagArm_PlaceMaxUseCountOnTime(MMagNowSortTray, iStepRow, iStepCol,true , true);    //Sam 20240727 : Magazine 換 Tray 時XY提前移動
    if(iActSuckR==-1 || iActSuckC==-1)
    {
        return false;
    }
    return DoMoveMagArmXYToPick(iRowPitch, iColPitch, true);
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::MoveMagArmXYToPlace(int iWhichMag)
{
    Find_MagArm_PlaceMaxUseCountOnTime(MMagSortTopTray[iWhichMag], iStepRow, iStepCol, false, true);
    return  DoMoveMagArmXYToPlace(iRowPitch, iColPitch, true, iWhichMag);
}
//==============================================================================
bool MyMagArmParameter::MoveMagArmXYToPickNoZDown()                             //Sam 20240727 : Magazine 換 Tray 時XY提前移動
{
    if(HSys.FuncS.bS03_MagArmXYPreMove==false)
        return false;

    if(MMagSortTopTray[1]->fHasTray && MMagSortTopTray[1]->FullIC())            //Sam 20240810 : 避免滿 Tray 讓位失敗。
        return false;

    Find_MagArm_PlaceMaxUseCountOnTime(MMagNowSortTray, iStepRow, iStepCol,true , false);
    if(iActSuckR==-1 || iActSuckC==-1)
    {
        return false;
    }
    return DoMoveMagArmXYToPick(iRowPitch, iColPitch, true);
}

//------------------------------------------------------------------------------
bool MyMagArmParameter::CheckZNeedSuck(int iUseSuckNo)
{
    int ct=0, iHasICCnt=0;
    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(MagArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                iHasICCnt++;
            }
        }
    }

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(ct<iUseSuckNo-iHasICCnt)
            {
                if(fSetup->rgUseSuck->ItemIndex==1 &&
                   bSortArm2UseSuck[iR][iC]==false)                             //JerryYang 20240706 : fix 關SITE
                {
                    continue;
                }
                else if(MagArmSuck->Suck[iR][iC].Item==NULL_IC)
                {
                    bMagArmSuckActive[iR][iC]=true;
                    ct++;
                }
            }
        }
    }

    if(ct>0)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::CheckZNeedPlace(int iUseSuckNo)
{
    int ct=0;

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(ct<iUseSuckNo)
            {
                if(MagArmSuck->Suck[iR][iC].Item==HAS_IC)
                {
                    bMagArmSuckActive[iR][iC]=true;
                    ct++;
                }
            }
        }
    }

    if(ct>0)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::MoveMagArmToAutoSafe()                                  //Sam 20240618 : MagArm Z auto home
{
    bool bResult=false;
    bool OK=true, bNeedHome=false;
    static iRetryCount=0;
    #ifndef SOFT_SIMULATE
    bool bHome[4][2]={{false, false}, {false, false},
                      {false, false}, {false, false}};
    #endif

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(MMagArmZ[iR][iC]->MotorMove(10)==false)
            {
               OK=false;
            }
        }
    }

    if(OK)
    {
        bNeedHome=false;
        for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
            {
                MMagArmZ[iR][iC]->ScanMotorStatus();
                #ifndef SOFT_SIMULATE
                    bHome[iR][iC]=MMagArmZ[iR][iC]->Led[iHomeLed];
                    if(bHome[iR][iC]==false)
                    {
                        bNeedHome=true;
                    }
                #endif
            }
        }

        if(bNeedHome)
        {
            if(iRetryCount>100)
            {
                iRetryCount=0;
                bMagArmNeedHome=true;
                fHome->SortArm2ZHome(true);
            }
            else
            {
                iRetryCount++;
            }
            bResult=false;
        }
        else
        {
            iRetryCount=0;
            bResult=true;
        }
    }
    else
    {
        bResult=false;
    }
    return bResult;
}
//------------------------------------------------------------------------------
int MyMagArmParameter::MagArmDeviceDropCheck()                                  //0:OK ,1:In 異常 ,2:Out  異常  //Steven 20130808 :掉料檢查
{
    AnsiString ErrorPart="", asErr="";
    bool bOutHasErr=false;
    int iSuckRow=0,iSuckCol=0;
    if(HSys.LastSet.iRealDummy==REALLY)
    {
        bOutHasErr=MagArmSuck->CheckHasError(Vaccum_FallDown,iSuckCol,iSuckRow);
    }
    else
    {
        bOutHasErr=false;
    }

    if(bOutHasErr)
    {
        HSys.DecStopAllMotor();
        ErrorPart=MagArmSuck->GetErrorPart(Vaccum_FallDown);
        ShowSuckError(*MagArmSuck, Vaccum_FallDown, K_SKIP,ErrorPart);
        MagArmSuck->SkipError(Vaccum_FallDown, NULL_IC);
        int iTrayR=MagArmSuck->Suck[iSuckRow][iSuckCol].DeviceInfo.iInputR;
        int iTrayC=MagArmSuck->Suck[iSuckRow][iSuckCol].DeviceInfo.iInputC;
        asErr.sprintf("MagArmSuck-%s Drop IC, clean data,iTrayR=%d,iTrayC=%d", ErrorPart,iTrayR,iTrayC);          //KenHsieh 20240515 : fix 中途掉料不轉換資料
        RecordProcess(asErr);                                                   //KenHsieh 20240515 : fix 中途掉料不轉換資料

        for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
            {
                if(MagArmSuck->Suck[iR][iC].GetStatus()==false)
                {
                    MagArmSuck->Suck[iR][iC].Normal();
                }
            }
        }

        return 2;
    }

    return 0;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::CheckCanAction(int iWitchMGZ)
{
    int iBin=MMagSortTopTray[1]->Tray.DeviceInfo[0][0].iBin;
    int iTray=0;
    if(CosFunction.bUseBinAutoConvert)
    {
        iTray=cBinAutoTrans.GetAutobyBin(iBin);
    }
    else
    {
        iTray=BinSelect[0].iCategData[iBin];
    }
    if(iBin>=0 && iTray==iWitchMGZ)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
void MyMagArmParameter::AddLoadingCount(int iSuckR, int iSuckC, int iAutoX, int iAutoY)  //Steven 20161123 : 將Loader吸取的數量計算統一成Function
{
    tRunData.LoaderIC++;                                                        //JerryYang 20240616 : add loading count
    iUPH_LoaderCount++;                                                         //JerryYang 20240625 : fix UPH不準
    MagArmSuck->MoveSingleItem(MMagSortTray, iSuckR, iSuckC, iAutoX, iAutoY);
    MagArmSuck->Suck[iSuckR][iSuckC].DeviceInfo.AddInputInfo(MMagSortTray->Tray.ClipID, iAutoX, iAutoY, 2, iSuckR, iSuckC);
    MMagNowSortTray->SetTraySingleData(iAutoX, iAutoY, NULL_IC);
    bMagSuckEnd[iSuckR][iSuckC]=true;                                           //JerryYang 20240623 : pick up error可以SKIP
}
//------------------------------------------------------------------------------
void MyMagArmParameter::GetMapTrayPosFromEncorder(int idata, int &iRow, int &iCol)
{
    iRow=idata/100;
    iCol=idata%100;
}
//------------------------------------------------------------------------------
int MyMagArmParameter::GetMapTrayPosFromRowCol(int iRow, int iCol)
{
    return iRow*100+iCol;
}
//------------------------------------------------------------------------------
void MyMagArmParameter::MagArmSuckReset()                                       //JerryYang 20240623 : pick up error可以SKIP
{
    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            bMagSuckEnd[iR][iC]=false;
        }
    }
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::IsCheckMagArmDestroyActiveFinish(int iX, int iY)
{
    int iPosX=0, iPosY=0;
    int iPreRange=100;
    iPosX=MMagArmX->ReadPos();
    iPosY=MMagArmY->ReadPos();
    if(iPosX>=iX-iPreRange && iPosX<=iX+iPreRange &&
       iPosY>=iY-iPreRange && iPosY<=iY+iPreRange)
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<2; j++)
            {
               if(bCheckMagArmDestroyActiveFinish[i][j]==false)
                  return false;
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool __fastcall MyMagArmParameter::CheckMagArmSameBin()                         //JerryYang 20240817 : 檢查ARM上的BIN必須相同
{
    int iBin=-1;
    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(MagArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                if(iBin==-1)
                {
                    iBin=MagArmPara->MagArmSuck->Suck[iR][iC].DeviceInfo.iBin;
                }
                else
                {
                    if(iBin!=MagArmPara->MagArmSuck->Suck[iR][iC].DeviceInfo.iBin)  //Sort arm同時有兩個bin
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool __fastcall MyMagArmParameter::ManualTakeOutMagArmDevice(bool bHome)        //JerryYang 20240817 : 檢查ARM上的BIN必須相同
{
    bool bAlarm=false;
    AnsiString asErr="";

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(MagArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                if(MagArmSuck->Suck[iR][iC].GetStatus())
                {
                    bAlarm=true;
                }
                else
                {
                    MagArmSuck->SetItemData(iR, iC, NULL_IC);
                    MagArmSuck->Suck[iR][iC].DeviceInfo.Clear();
                    asErr.sprintf("MagArmSuck[%d,%d] Manual take out IC, clean data", iR, iC);         //KenHsieh 20240515 : fix 中途掉料不轉換資料
                    RecordProcess(asErr);                                                   //KenHsieh 20240515 : fix 中途掉料不轉換資料
                }
            }
        }
    }

    if(bAlarm)
    {
        if(bHome==true)
        {
            ShowMyMessage("Please take out all device on Mag Arm!!");
        }
        else
        {
            ShowMyMessage("Bin data error, please take out all device on Mag Arm!!");
        }
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool __fastcall MyMagArmParameter::CheckMagArmBin(int iBin)                     //JerryYang 20240817 : 增加保護確保iBin相同
{

    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
        {
            if(MagArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                if(iBin!=MagArmPara->MagArmSuck->Suck[iR][iC].DeviceInfo.iBin)  //Sort arm同時有兩個bin
                {
                    return false;
                }
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::DoCatchEmptyTray(int iWhichMag, bool bReset)
{
    bool bResult=false;
    int &Task=iCatchEmptyTrayTask;
    if(bReset)
    {
        Task=1;
        return bResult;
    }
    bool bFalg=false;
    switch(Task)
    {
        case 1:
            if(MoveMagArmToAutoSafe())
            {
                Task=100;
            }
            break;
        case 100:
            if(HSys.LastSet.iRealDummy==REALLY)
            {
                for(int iC=0; iC<MagArmSuck->MaxItemC; iC++)
                {
                    for(int iR=0; iR<MagArmSuck->MaxItemR; iR++)
                    {
                        if(MagArmSuck->Suck[iR][iC].Item!=NULL_IC ||
                           MagArmSuck->Suck[iR][iC].GetStatus())
                        {
                            bFalg=true;
                        }
                    }
                }
            }
            if(bFalg)
            {
                ShowSystemError(MagArmSuck->AlarmName[eInitOnErr], K_RETRY);   //"吸嘴上有IC"
            }
            else
            {
                bXMoveFinish=false;
                bYMoveFinish=false;
                Task=200;
            }
            break;
        case 200:
            iEmptyMagStatus=eHS_EM_NeedTray;
            Task=300;
            break;
        case 300:
            if(LoaderModule->iMagArmStatus==LoaderModule->eHS_MA_EmptyTrayReady)
            {
                iEmptyMagStatus=eHS_EM_Idle;
                bXMoveFinish=false;
                bYMoveFinish=false;
                Task=1000;
            }
            break;
        case 1000:
            if(bXMoveFinish==false)
                bXMoveFinish=MMagArmX->MotorMove(iPos_MagPickTray_X);
            if(bYMoveFinish==false)
                bYMoveFinish=MMagArmY->MotorMove(iPos_MagPickTray_Y);

            if(bXMoveFinish && bYMoveFinish)
            {
                bXMoveFinish=false;
                bYMoveFinish=false;
                C_MagArm_UpDown->Reset();
                C_CatchTray->Reset();
                Task=1100;
            }
            break;
        case 1100:
            if(C_CatchTray->Pop())
            {
                C_CatchTray->Reset();
                OutArmIdle.SetSecAndOn(2);
                C_MagArm_UpDown->On();
                Task=1120;
            }
            break;
        case 1120:
            if(OutArmIdle.Off() || HSys.LastSet.iRealDummy==DUMMY)
            {
                Task=1200;
            }
            break;
        case 1200:
            C_CatchTray->On();
            C_MagArm_UpDown->Reset();
            OutArmIdle.SetSecAndOn(2);
            Task=1300;
            break;
        case 1300:
            if(OutArmIdle.Off() || HSys.LastSet.iRealDummy==DUMMY)
            {
                C_MagArm_UpDown->Off();
                C_CatchTray->On();
                OutArmIdle.SetSecAndOn(5);
                Task=1310;
            }
            break;
        case 1310:
            if(OutArmIdle.Off()==false &&
               C_MagArm_UpDown->IsOff()==false)
                break;

            if(C_MagArm_UpDown->IsOff() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                OutArmIdle.SetSecAndOn(2);
                Task=1320;
            }
            else
            {
                ShowSystemError(C_MagArm_UpDown->ErrorName[eOffNotOnErr], K_RETRY); //"Tray Arm 上升未到位"
                Task=1200;
            }
            break;
        case 1320:
            if(C_CatchTray->IsOn() ||                                           //KenHsieh 20240521 : 第二台改為off -> 張開, On -> 夾取
               HSys.LastSet.iRealDummy==DUMMY)
            {
                bXMoveFinish=false;
                bYMoveFinish=false;
                MMagArmX->SetPersentSpeed(50, false);                           //避免Tray 滑掉要減速
                Task=1350;
            }
            else if(OutArmIdle.Off())
            {
                ShowSystemError(C_CatchTray->ErrorName[eOffNotOnErr], K_RETRY); //"Tray Arm 未夾取到Tray");
                C_MagArm_UpDown->Reset();
                C_CatchTray->Reset();
                Task=1100;
            }
            break;
        case 1350:
            //get tray finish
            if(bXMoveFinish==false)
                bXMoveFinish=MMagArmX->MotorMove(iPos_MagPlaceTray_X[iWhichMag]);
            if(bYMoveFinish==false)
                bYMoveFinish=MMagArmY->MotorMove(iPos_MagPlaceTray_Y[iWhichMag]);

            if(bXMoveFinish && bYMoveFinish)
            {
                MMagArmX->SetPersentSpeed(MMagArmX->GetPersentSpeed());
                if(HSys.FuncT.iT07_MGZDirection==0)
                {
                    C_MGZ_TopFixTray->Reset();
                    Task=1450;
                }
                else
                {
                    Task=1500;
                }
            }
            break;
        case 1450:
            C_MGZ_TopFixTray->Off();
            OutArmIdle.SetMSAndOn(500);
            Task=1455;
            break;
        case 1455:
            if(OutArmIdle.Off() || HSys.LastSet.iRealDummy==DUMMY)
            {
                if(C_MGZ_TopFixTray->IsOff() ||
                   HSys.LastSet.iRealDummy==DUMMY)
                {
                    C_MagArm_UpDown->Reset();
                    Task=1500;
                }
                else
                {
                    ShowSystemError(C_MGZ_TopFixTray->ErrorName[eOffNotOnErr], K_RETRY);
                    Task=1455;
                }
            }
            break;
        case 1500:
            C_MagArm_UpDown->On();
            C_CatchTray->Reset();
            OutArmIdle.SetSecAndOn(3);
            Task=1600;
            break;
        case 1600:
            if(OutArmIdle.Off()==false)                                         //KenHsieh 20240514 : 改為下降到位延遲
                break;

            if(C_MagArm_UpDown->IsOn() ||
               HSys.LastSet.iRealDummy!=REALLY)
            {
                C_CatchTray->Off();
                OutArmIdle.SetSecAndOn(2);
                Task=1700;
            }
            else if(OutArmIdle.Off())
            {
                ShowSystemError(C_MagArm_UpDown->ErrorName[eOnNotOnErr], K_RETRY);//("Tray Arm 下降未到位");
            }
            break;
        case 1700:
            if(OutArmIdle.Off() || HSys.LastSet.iRealDummy==DUMMY)
            {
                C_MagArm_UpDown->Reset();
                Task=1710;
            }
            break;
        case 1710:
            C_MagArm_UpDown->Off();
            OutArmIdle.SetMSAndOn(1500);
            Task=1800;
            break;
        case 1800:
            if(C_MagArm_UpDown->IsOff() ||
               HSys.LastSet.iRealDummy==DUMMY)
            {
                Task=9900;
            }
            else if(OutArmIdle.Off())
            {
                ShowSystemError(C_MagArm_UpDown->ErrorName[eOffNotOnErr], K_RETRY); //("Tray Arm 上升未到位");
                Task=1700;
            }
            break;
        case 9900:                                                              //finish
            bResult=true;
            break;
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool MyMagArmParameter::SetWorkBin(int iBin)
{
    mBinToTray.ConvertBinToAuto(iBin);
    if(iBin==0)
    {
        return false;
    }
    else
    {
        mNowBin.ConvertBinToAuto(mBinToTray.iBin);
        fMain->lblMgzCurrBin_2->Caption=AnsiString("Bin : ")+AnsiString(mBinToTray.iBin);
        fMain->lblMgzCurrBin_1->Caption=AnsiString("Current Sorting Bin : ")+AnsiString(mBinToTray.iBin);
        return true;
    }
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::GetWhichMag(int iBin, int &iMag)
{
    MyBinToTrayStruct mTemp;
    mTemp.ConvertBinToAuto(iBin);
    if(mTemp.iAutoRow==eTrackMgz1 &&
       (mTemp.iWhichAuto>=eMag1_01 && mTemp.iWhichAuto<=eMag1_27))
    {
        iMag=eMagazine1;
        return true;
    }
    else if(mTemp.iAutoRow==eTrackMgz2 &&
            (mTemp.iWhichAuto>=eMag2_01 && mTemp.iWhichAuto<=eMag2_27))
    {
        iMag=eMagazine2;
        return true;
    }
    else if(mTemp.iAutoRow==eTrackMgz3 &&
            (mTemp.iWhichAuto>=eMag3_01 && mTemp.iWhichAuto<=eMag3_27))
    {
        iMag=eMagazine3;
        return true;
    }
    else
    {
        iMag=0;
        return false;
    }
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::DoPnPToMagTop(int iWhichMag, bool bReset)
{
    bool bResult=false;
    int &Task=iPickAndPlace2TopMagTask;
    if(bReset)
    {
        Task=1;
        return bResult;
    }
    switch(Task)
    {
        case 1:
            if(HSys.Sys.RunMode==Run_OneCycle &&
                MagArmSuck->HasRealIC()==false)                                 //Sam 20250722 : Fixed for one cycle
            {
               bSortArm2OneCycleFinish=true;
            }
            else if(MagazineModule[iWhichMag]->iMagArmStatus==MagazineModule[iWhichMag]->eHS_MA_NeedEmptyTray)
            {
                DoCatchEmptyTray(iWhichMag, true);
                Task=8000;
            }
            else if(bNeedPlaceFirst     &&                                      //KenHsieh(JerryYang) 20240826 : ARM上已經有IC就先放料, 不要重新search
                    MagArmSuck->HasIC() &&
                    MMagSortTopTray[iWhichMag]->fHasTray==true &&
                    MagazineModule[iWhichMag]->mTopTray.iBin==mNowBin.iBin)     //JerryYang 20240817 : ARM上已經有IC就先放料, 不要重新search
            {
                DoPlaceICToMagTop(true,iWhichMag);
                Task=4000;
                break;
            }
            else if(MMagNowSortTray->HasIC())
            {
                if(MMagSortTopTray[iWhichMag]->fHasTray &&
                   MagazineModule[iWhichMag]->mTopTray.iBin==mNowBin.iBin)
                {
                    MMagNowSortTray->SetTrayVisible(true);
                    if(CheckHasSpaceToPlace(iWhichMag, iUseSuckNo))
                    {
                        DoPickICFromMagLoader(iWhichMag, true);
                        Task=1000;
                        break;
                    }
                    else
                    {
                        Task=6000;
                        break;
                    }
                }
                else
                {
                    Task=1;
                }
            }
            else
            {
                Task=9900;
            }
            break;
        case 1000:
            if(DoPickICFromMagLoader(iWhichMag))
            {
                Task=4000;
            }
            break;
        case 4000:
            DoPlaceICToMagTop(iWhichMag, true);
            Task=4001;
//            break;
        case 4001:
            if(DoPlaceICToMagTop(iWhichMag))
            {
                bNeedPlaceFirst=false;                                          //KenHsieh(JerryYang) 20240826 : ARM上已經有IC就先放料, 不要重新search
                Task=1;
            }
            break;
        case 6000:
            DoTopFullTray(iWhichMag, true);
            Task=7000;
            break;
        case 7000:
            if(DoTopFullTray(iWhichMag))
            {
                Task=1;
            }
            break;
        case 8000:
            if(DoCatchEmptyTray(iWhichMag))
            {
                iMagStatus=eHS_MAG_EmptyTrayReady1+iWhichMag;
                Task=8500;
            }
            break;
        case 8500:                                                              //等待的Flag時候, 不要在case 8000裡面
            if(MagazineModule[iWhichMag]->iMagArmStatus==MagazineModule[iWhichMag]->eHS_MA_Idle)
            {
                iMagStatus=eHS_MAG_Idle;
                Task=1;
            }
            break;
        case 9900:                                                              //finish
            MMagNowSortTray->ClearTray();
            bNeedPlaceFirst=false;                                              //KenHsieh(JerryYang) 20240826 : ARM上已經有IC就先放料, 不要重新search
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::DoTopFullTray(int iWhichMag, bool bReset)
{
    int &Task=iTopFullTrayTask;
    bool bResult=false;

    if(bReset)
    {
        bXMoveFinish=false;
        bYMoveFinish=false;
        Task=1;
        return bResult;
    }
    switch(Task)
    {
        case 1:
            if(MoveMagArmToAutoSafe())
            {
                bXMoveFinish=false;
                bYMoveFinish=false;
                Task=100;
            }
            break;
        case 100:
            if(bXMoveFinish==false)
                bXMoveFinish=MMagArmX->MotorMove(0);
            if(bYMoveFinish==false)
                bYMoveFinish=MMagArmY->MotorMove(0);
            if(bXMoveFinish && bYMoveFinish)
            {
                iMagStatus=eHS_MAG_FullTrayAlarm1+iWhichMag;
                Task=200;
            }
            break;
        case 200:
            if(MagazineModule[iWhichMag]->iMagArmStatus==MagazineModule[iWhichMag]->eHS_MA_FullTrayAlarmFinish)
            {
                iMagStatus=eHS_MAG_Idle;
                Task=9900;
            }
            else
            {
                break;
            }
        case 9900:
            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool MyMagArmParameter::MagTopTrayHasMapBin(int &iWhichMag)
{
    if(CosFunction.bMagCrossMagTopReadyFirst && MAGAZINE_CNT>1)
    {
        int iFirstMag=-1;
        for(int i=eMagazine1; i<eMGZTotal; i++)
        {
            if(Find_SortBin_Mag(MagazineModule[i]->mTopTray.iBin))
            {
                if(iFirstMag<0)
                {
                    iFirstMag=i;
                }
                if(i!=gLastPickedMag)
                {
                    iWhichMag=i;
                    gLastPickedMag=iWhichMag;
                    return true;
                }
            }
        }
        if(iFirstMag>=0)
        {
            Find_SortBin_Mag(MagazineModule[iFirstMag]->mTopTray.iBin);
            iWhichMag=iFirstMag;
            gLastPickedMag=iWhichMag;
            return true;
        }
        return false;
    }
    for(int i=eMagazine1; i<eMGZTotal; i++)
    {
        if(Find_SortBin_Mag(MagazineModule[i]->mTopTray.iBin))
        {
            iWhichMag=i;
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
