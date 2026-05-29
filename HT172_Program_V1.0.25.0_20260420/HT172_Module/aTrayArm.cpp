#include "IncludeAllHeader.h"
#pragma hdrstop

//------------------------------------------------------------------------------
#include "aTrayArm.h"
#pragma package(smart_init)
//------------------------------------------------------------------------------
MyTrayArmParameter *MyTrayArm=new MyTrayArmParameter();

MyTrayArmParameter *TrayArmPara=MyTrayArm;
int iTrayPushStartPos=-20;
//------------------------------------------------------------------------------
void InitTrayArm()
{
    TrayArmPara->MStoreArmX                     =HSys.Mot.StoreArmX             ;
    TrayArmPara->MStoreArmY                     =HSys.Mot.StoreArmY             ;
    TrayArmPara->MStoreArmZ                     =HSys.Mot.StoreArmZ             ;

    TrayArmPara->MPushStoreArmX[eTrack1]        =HSys.Mot.PushStoreArmX1        ;   //推Tray 機構改為馬達
    TrayArmPara->MPushStoreArmX[eTrack2]        =HSys.Mot.PushStoreArmX2        ;
    TrayArmPara->MPushStoreArmX[eTrack3]        =HSys.Mot.PushStoreArmX3        ;
    TrayArmPara->MPushStoreArmX[eTrack4]        =HSys.Mot.PushStoreArmX4        ;

    TrayArmPara->MtTrayArmTray                  =HSys.VMot.TrayArmTray          ;
    TrayArmPara->MCarrierStoreX[eTrack1]        =HSys.Mot.AutoTrolly1X          ;
    TrayArmPara->MCarrierStoreX[eTrack2]        =HSys.Mot.AutoTrolly2X          ;
    TrayArmPara->MCarrierStoreX[eTrack3]        =HSys.Mot.AutoTrolly3X          ;
    TrayArmPara->MCarrierStoreX[eTrack4]        =HSys.Mot.AutoTrolly4X          ;
    TrayArmPara->MCarrierStoreZ[eTrack1]        =HSys.Mot.AutoTrolly1Z          ;
    TrayArmPara->MCarrierStoreZ[eTrack2]        =HSys.Mot.AutoTrolly2Z          ;
    TrayArmPara->MCarrierStoreZ[eTrack3]        =HSys.Mot.AutoTrolly3Z          ;
    TrayArmPara->MCarrierStoreZ[eTrack4]        =HSys.Mot.AutoTrolly4Z          ;

    TrayArmPara->C_AutoTrolly_Stop[eTrack1]     =&HSys.Cyn.C_AutoTrolly1_Stop   ;
    TrayArmPara->C_AutoTrolly_Stop[eTrack2]     =&HSys.Cyn.C_AutoTrolly2_Stop   ;
    TrayArmPara->C_AutoTrolly_Stop[eTrack3]     =&HSys.Cyn.C_AutoTrolly3_Stop   ;
    TrayArmPara->C_AutoTrolly_Stop[eTrack4]     =&HSys.Cyn.C_AutoTrolly4_Stop   ;

    TrayArmPara->Sen_PushTrayCheck[eTrack1]     =&HSys.Sen.SnAutoRow1PushCheck  ;   //推Tray 機構改為馬達
    TrayArmPara->Sen_PushTrayCheck[eTrack2]     =&HSys.Sen.SnAutoRow2PushCheck  ;
    TrayArmPara->Sen_PushTrayCheck[eTrack3]     =&HSys.Sen.SnAutoRow3PushCheck  ;
    TrayArmPara->Sen_PushTrayCheck[eTrack4]     =&HSys.Sen.SnAutoRow4PushCheck  ;
    TrayArmPara->Sen_TrackCyPushTray[eTrack1]   =&HSys.Sen.SnAutoRow1CyPushTray ;
    TrayArmPara->Sen_TrackCyPushTray[eTrack2]   =&HSys.Sen.SnAutoRow2CyPushTray ;
    TrayArmPara->Sen_TrackCyPushTray[eTrack3]   =&HSys.Sen.SnAutoRow3CyPushTray ;
    TrayArmPara->Sen_TrackCyPushTray[eTrack4]   =&HSys.Sen.SnAutoRow4CyPushTray ;

    TrayArmPara->TrayArmSuck                    =&HSys.Suck.TrayArmSuck         ;
    TrayArmPara->C_StoreArm_EdgeFixer           =&HSys.Cyn.C_StoreArm_EdgeFixer ;

    TrayArmPara->SnStoreArmTakeTray             =&HSys.Sen.SnStoreArmTakeTray   ;
    TrayArmPara->SnStoreArmHasTray              =&HSys.Sen.SnStoreArmHasTray    ;
    TrayArmPara->SnStoreArmPutPos1              =&HSys.Sen.SnStoreArmPutPos1    ;
    TrayArmPara->SnStoreArmPutPos2              =&HSys.Sen.SnStoreArmPutPos2    ;

    TrayArmPara->Sen_AutoRowULDCheck[eTrack1]   =&HSys.Sen.SnAutoRow1ULDCheck   ;
    TrayArmPara->Sen_AutoRowULDCheck[eTrack2]   =&HSys.Sen.SnAutoRow2ULDCheck   ;
    TrayArmPara->Sen_AutoRowULDCheck[eTrack3]   =&HSys.Sen.SnAutoRow3ULDCheck   ;
    TrayArmPara->Sen_AutoRowULDCheck[eTrack4]   =&HSys.Sen.SnAutoRow4ULDCheck   ;

    TrayArmPara->C_Track_Stop[eTrack1]          =&HSys.Cyn.C_AutoRow1_Stop      ;
    TrayArmPara->C_Track_Stop[eTrack2]          =&HSys.Cyn.C_AutoRow2_Stop      ;
    TrayArmPara->C_Track_Stop[eTrack3]          =&HSys.Cyn.C_AutoRow3_Stop      ;
    TrayArmPara->C_Track_Stop[eTrack4]          =&HSys.Cyn.C_AutoRow4_Stop      ;

    TrayArmPara->C_Track_PushTray[eTrack1]      =&HSys.Cyn.C_AutoRow1_PushTray  ;
    TrayArmPara->C_Track_PushTray[eTrack2]      =&HSys.Cyn.C_AutoRow2_PushTray  ;
    TrayArmPara->C_Track_PushTray[eTrack3]      =&HSys.Cyn.C_AutoRow3_PushTray  ;
    TrayArmPara->C_Track_PushTray[eTrack4]      =&HSys.Cyn.C_AutoRow4_PushTray  ;

    TrayArmPara->Track_Dete[eTrack1]            =&HSys.Sw.SwAutoRow1Dete        ;
    TrayArmPara->Track_Dete[eTrack2]            =&HSys.Sw.SwAutoRow2Dete        ;
    TrayArmPara->Track_Dete[eTrack3]            =&HSys.Sw.SwAutoRow3Dete        ;
    TrayArmPara->Track_Dete[eTrack4]            =&HSys.Sw.SwAutoRow4Dete        ;

    TrayArmPara->AutoRowVib1[eTrack1]           =&HSys.Sw.SwAutoRow1Vib1        ;
    TrayArmPara->AutoRowVib1[eTrack2]           =&HSys.Sw.SwAutoRow2Vib1        ;
    TrayArmPara->AutoRowVib1[eTrack3]           =&HSys.Sw.SwAutoRow3Vib1        ;
    TrayArmPara->AutoRowVib1[eTrack4]           =&HSys.Sw.SwAutoRow4Vib1        ;

    TrayArmPara->AutoRowVib2[eTrack1]           =&HSys.Sw.SwAutoRow1Vib2        ;
    TrayArmPara->AutoRowVib2[eTrack2]           =&HSys.Sw.SwAutoRow2Vib2        ;
    TrayArmPara->AutoRowVib2[eTrack3]           =&HSys.Sw.SwAutoRow3Vib2        ;
    TrayArmPara->AutoRowVib2[eTrack4]           =&HSys.Sw.SwAutoRow4Vib2        ;

    TrayArmPara->Track_CW[eTrack1]              =&HSys.Sw.SwACAutoRow1CW        ;
    TrayArmPara->Track_CW[eTrack2]              =&HSys.Sw.SwACAutoRow2CW        ;
    TrayArmPara->Track_CW[eTrack3]              =&HSys.Sw.SwACAutoRow3CW        ;
    TrayArmPara->Track_CW[eTrack4]              =&HSys.Sw.SwACAutoRow4CW        ;

    TrayArmPara->Track_CCW[eTrack1]             =&HSys.Sw.SwACAutoRow1CCW       ;
    TrayArmPara->Track_CCW[eTrack2]             =&HSys.Sw.SwACAutoRow2CCW       ;
    TrayArmPara->Track_CCW[eTrack3]             =&HSys.Sw.SwACAutoRow3CCW       ;
    TrayArmPara->Track_CCW[eTrack4]             =&HSys.Sw.SwACAutoRow4CCW       ;

    TrayArmPara->Sen_Store_Full[eTrack1][eTrackCol1]=&HSys.Sen.SnStore1Full     ;
    TrayArmPara->Sen_Store_Full[eTrack1][eTrackCol2]=&HSys.Sen.SnStore2Full     ;
    TrayArmPara->Sen_Store_Full[eTrack1][eTrackCol3]=&HSys.Sen.SnStore3Full     ;
    TrayArmPara->Sen_Store_Full[eTrack1][eTrackCol4]=&HSys.Sen.SnStore4Full     ;
    TrayArmPara->Sen_Store_Full[eTrack1][eTrackCol5]=&HSys.Sen.SnStore5Full     ;
    TrayArmPara->Sen_Store_Full[eTrack2][eTrackCol1]=&HSys.Sen.SnStore6Full     ;
    TrayArmPara->Sen_Store_Full[eTrack2][eTrackCol2]=&HSys.Sen.SnStore7Full     ;
    TrayArmPara->Sen_Store_Full[eTrack2][eTrackCol3]=&HSys.Sen.SnStore8Full     ;
    TrayArmPara->Sen_Store_Full[eTrack2][eTrackCol4]=&HSys.Sen.SnStore9Full     ;
    TrayArmPara->Sen_Store_Full[eTrack2][eTrackCol5]=&HSys.Sen.SnStore10Full    ;
    TrayArmPara->Sen_Store_Full[eTrack3][eTrackCol1]=&HSys.Sen.SnStore11Full    ;
    TrayArmPara->Sen_Store_Full[eTrack3][eTrackCol2]=&HSys.Sen.SnStore12Full    ;
    TrayArmPara->Sen_Store_Full[eTrack3][eTrackCol3]=&HSys.Sen.SnStore13Full    ;
    TrayArmPara->Sen_Store_Full[eTrack3][eTrackCol4]=&HSys.Sen.SnStore14Full    ;
    TrayArmPara->Sen_Store_Full[eTrack3][eTrackCol5]=&HSys.Sen.SnStore15Full    ;
    TrayArmPara->Sen_Store_Full[eTrack4][eTrackCol1]=&HSys.Sen.SnStore16Full    ;
    TrayArmPara->Sen_Store_Full[eTrack4][eTrackCol2]=&HSys.Sen.SnStore17Full    ;
    TrayArmPara->Sen_Store_Full[eTrack4][eTrackCol3]=&HSys.Sen.SnStore18Full    ;
    TrayArmPara->Sen_Store_Full[eTrack4][eTrackCol4]=&HSys.Sen.SnStore19Full    ;
    TrayArmPara->Sen_Store_Full[eTrack4][eTrackCol5]=&HSys.Sen.SnStore20Full    ;

    TrayArmPara->Sen_Store_Door[eTrack1][eTrackCol1]=&HSys.Sen.SnStore1Door     ;
    TrayArmPara->Sen_Store_Door[eTrack1][eTrackCol2]=&HSys.Sen.SnStore2Door     ;
    TrayArmPara->Sen_Store_Door[eTrack1][eTrackCol3]=&HSys.Sen.SnStore3Door     ;
    TrayArmPara->Sen_Store_Door[eTrack1][eTrackCol4]=&HSys.Sen.SnStore4Door     ;
    TrayArmPara->Sen_Store_Door[eTrack1][eTrackCol5]=&HSys.Sen.SnStore5Door     ;
    TrayArmPara->Sen_Store_Door[eTrack2][eTrackCol1]=&HSys.Sen.SnStore6Door     ;
    TrayArmPara->Sen_Store_Door[eTrack2][eTrackCol2]=&HSys.Sen.SnStore7Door     ;
    TrayArmPara->Sen_Store_Door[eTrack2][eTrackCol3]=&HSys.Sen.SnStore8Door     ;
    TrayArmPara->Sen_Store_Door[eTrack2][eTrackCol4]=&HSys.Sen.SnStore9Door     ;
    TrayArmPara->Sen_Store_Door[eTrack2][eTrackCol5]=&HSys.Sen.SnStore10Door    ;
    TrayArmPara->Sen_Store_Door[eTrack3][eTrackCol1]=&HSys.Sen.SnStore11Door    ;
    TrayArmPara->Sen_Store_Door[eTrack3][eTrackCol2]=&HSys.Sen.SnStore12Door    ;
    TrayArmPara->Sen_Store_Door[eTrack3][eTrackCol3]=&HSys.Sen.SnStore13Door    ;
    TrayArmPara->Sen_Store_Door[eTrack3][eTrackCol4]=&HSys.Sen.SnStore14Door    ;
    TrayArmPara->Sen_Store_Door[eTrack3][eTrackCol5]=&HSys.Sen.SnStore15Door    ;
    TrayArmPara->Sen_Store_Door[eTrack4][eTrackCol1]=&HSys.Sen.SnStore16Door    ;
    TrayArmPara->Sen_Store_Door[eTrack4][eTrackCol2]=&HSys.Sen.SnStore17Door    ;
    TrayArmPara->Sen_Store_Door[eTrack4][eTrackCol3]=&HSys.Sen.SnStore18Door    ;
    TrayArmPara->Sen_Store_Door[eTrack4][eTrackCol4]=&HSys.Sen.SnStore19Door    ;
    TrayArmPara->Sen_Store_Door[eTrack4][eTrackCol5]=&HSys.Sen.SnStore20Door    ;

    TrayArmPara->Auto_Store[eTrack1][eTrackCol1]=fMain->palAutoStore01          ;
    TrayArmPara->Auto_Store[eTrack1][eTrackCol2]=fMain->palAutoStore02          ;
    TrayArmPara->Auto_Store[eTrack1][eTrackCol3]=fMain->palAutoStore03          ;
    TrayArmPara->Auto_Store[eTrack1][eTrackCol4]=fMain->palAutoStore04          ;
    TrayArmPara->Auto_Store[eTrack1][eTrackCol5]=fMain->palAutoStore05          ;
    TrayArmPara->Auto_Store[eTrack2][eTrackCol1]=fMain->palAutoStore06          ;
    TrayArmPara->Auto_Store[eTrack2][eTrackCol2]=fMain->palAutoStore07          ;
    TrayArmPara->Auto_Store[eTrack2][eTrackCol3]=fMain->palAutoStore08          ;
    TrayArmPara->Auto_Store[eTrack2][eTrackCol4]=fMain->palAutoStore09          ;
    TrayArmPara->Auto_Store[eTrack2][eTrackCol5]=fMain->palAutoStore10          ;
    TrayArmPara->Auto_Store[eTrack3][eTrackCol1]=fMain->palAutoStore11          ;
    TrayArmPara->Auto_Store[eTrack3][eTrackCol2]=fMain->palAutoStore12          ;
    TrayArmPara->Auto_Store[eTrack3][eTrackCol3]=fMain->palAutoStore13          ;
    TrayArmPara->Auto_Store[eTrack3][eTrackCol4]=fMain->palAutoStore14          ;
    TrayArmPara->Auto_Store[eTrack3][eTrackCol5]=fMain->palAutoStore15          ;
    TrayArmPara->Auto_Store[eTrack4][eTrackCol1]=fMain->palAutoStore16          ;
    TrayArmPara->Auto_Store[eTrack4][eTrackCol2]=fMain->palAutoStore17          ;
    TrayArmPara->Auto_Store[eTrack4][eTrackCol3]=fMain->palAutoStore18          ;
    TrayArmPara->Auto_Store[eTrack4][eTrackCol4]=fMain->palAutoStore19          ;
    TrayArmPara->Auto_Store[eTrack4][eTrackCol5]=fMain->palAutoStore20          ;
}
//------------------------------------------------------------------------------
void UpdateTrayArmParameter()
{
                                            /*eTrackNotUse                      eTrack1                             eTrack2                             eTrack3                             eTrack4     */
    TEdit *Track_X[eTrackCount]             ={fTeach->edStoreArmX_Track1,       fTeach->edStoreArmX_Track1,         fTeach->edStoreArmX_Track2,         fTeach->edStoreArmX_Track3,         fTeach->edStoreArmX_Track4};
    TEdit *Track_Y[eTrackCount]             ={fTeach->edStoreArmY_Track1,       fTeach->edStoreArmY_Track1,         fTeach->edStoreArmY_Track2,         fTeach->edStoreArmY_Track3,         fTeach->edStoreArmY_Track4};
    TEdit *Track_Z[eTrackCount]             ={fTeach->edStoreArmZ_Track1,       fTeach->edStoreArmZ_Track1,         fTeach->edStoreArmZ_Track2,         fTeach->edStoreArmZ_Track3,         fTeach->edStoreArmZ_Track4};
    TEdit *Track_X_Push[eTrackCount]        ={fTeach->edPushStoreArmX1_Track,   fTeach->edPushStoreArmX1_Track,     fTeach->edPushStoreArmX2_Track,     fTeach->edPushStoreArmX3_Track,     fTeach->edPushStoreArmX4_Track};    //推Tray 機構改為馬達

    TEdit *AutoTrolly_Track_X_Push[eTrackCount]={fTeach->edAutoTrolly1X_Track1, fTeach->edAutoTrolly1X_Track1,      fTeach->edAutoTrolly2X_Track2,      fTeach->edAutoTrolly3X_Track3,      fTeach->edAutoTrolly4X_Track4};
    TEdit *AutoTrolly_Track_Z_Down[eTrackCount]={fTeach->edAutoTrolly1Z_Down,   fTeach->edAutoTrolly1Z_Down,        fTeach->edAutoTrolly2Z_Down,        fTeach->edAutoTrolly3Z_Down,        fTeach->edAutoTrolly4Z_Down};
    //    TEdit *Track_X_Offset[eTrackCount]      ={fOffset->edStoreArmX_Track1,      fOffset->edStoreArmX_Track1,        fOffset->edStoreArmX_Track2,        fOffset->edStoreArmX_Track3,        fOffset->edStoreArmX_Track4};
//    TEdit *Track_Y_Offset[eTrackCount]      ={fOffset->edStoreArmY_Track1,      fOffset->edStoreArmY_Track1,        fOffset->edStoreArmY_Track2,        fOffset->edStoreArmY_Track3,        fOffset->edStoreArmY_Track4};
//    TEdit *Track_Z_Offset[eTrackCount]      ={fOffset->edStoreArmZ_Track1,      fOffset->edStoreArmZ_Track1,        fOffset->edStoreArmZ_Track2,        fOffset->edStoreArmZ_Track3,        fOffset->edStoreArmZ_Track4};
    TEdit *Track_X_Push_Offset[eTrackCount] ={fOffset->edPushStoreArmX1_Track,  fOffset->edPushStoreArmX1_Track,    fOffset->edPushStoreArmX2_Track,    fOffset->edPushStoreArmX3_Track,    fOffset->edPushStoreArmX4_Track};   //推Tray 機構改為馬達

    TEdit *Storage_X[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/                  {{fTeach->edStoreArmX_Auto1,  fTeach->edStoreArmX_Auto2,  fTeach->edStoreArmX_Auto3,  fTeach->edStoreArmX_Auto4,  fTeach->edStoreArmX_Auto5},
    /*eTrack1     */                   {fTeach->edStoreArmX_Auto1,  fTeach->edStoreArmX_Auto2,  fTeach->edStoreArmX_Auto3,  fTeach->edStoreArmX_Auto4,  fTeach->edStoreArmX_Auto5},
    /*eTrack2     */                   {fTeach->edStoreArmX_Auto6,  fTeach->edStoreArmX_Auto7,  fTeach->edStoreArmX_Auto8,  fTeach->edStoreArmX_Auto9,  fTeach->edStoreArmX_Auto10},
    /*eTrack3     */                   {fTeach->edStoreArmX_Auto11, fTeach->edStoreArmX_Auto12, fTeach->edStoreArmX_Auto13, fTeach->edStoreArmX_Auto14, fTeach->edStoreArmX_Auto15},
    /*eTrack4     */                   {fTeach->edStoreArmX_Auto16, fTeach->edStoreArmX_Auto17, fTeach->edStoreArmX_Auto18, fTeach->edStoreArmX_Auto19, fTeach->edStoreArmX_Auto20}};

    TEdit *Storage_Y[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/                  {{fTeach->edStoreArmY_Auto1,  fTeach->edStoreArmY_Auto2,  fTeach->edStoreArmY_Auto3,  fTeach->edStoreArmY_Auto4,  fTeach->edStoreArmY_Auto5},
    /*eTrack1     */                   {fTeach->edStoreArmY_Auto1,  fTeach->edStoreArmY_Auto2,  fTeach->edStoreArmY_Auto3,  fTeach->edStoreArmY_Auto4,  fTeach->edStoreArmY_Auto5},
    /*eTrack2     */                   {fTeach->edStoreArmY_Auto6,  fTeach->edStoreArmY_Auto7,  fTeach->edStoreArmY_Auto8,  fTeach->edStoreArmY_Auto9,  fTeach->edStoreArmY_Auto10},
    /*eTrack3     */                   {fTeach->edStoreArmY_Auto11, fTeach->edStoreArmY_Auto12, fTeach->edStoreArmY_Auto13, fTeach->edStoreArmY_Auto14, fTeach->edStoreArmY_Auto15},
    /*eTrack4     */                   {fTeach->edStoreArmY_Auto16, fTeach->edStoreArmY_Auto17, fTeach->edStoreArmY_Auto18, fTeach->edStoreArmY_Auto19, fTeach->edStoreArmY_Auto20}};

    TEdit *Storage_Z[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/                  {{fTeach->edStoreArmZ_Auto1,  fTeach->edStoreArmZ_Auto2,  fTeach->edStoreArmZ_Auto3,  fTeach->edStoreArmZ_Auto4,  fTeach->edStoreArmZ_Auto5},
    /*eTrack1     */                   {fTeach->edStoreArmZ_Auto1,  fTeach->edStoreArmZ_Auto2,  fTeach->edStoreArmZ_Auto3,  fTeach->edStoreArmZ_Auto4,  fTeach->edStoreArmZ_Auto5},
    /*eTrack2     */                   {fTeach->edStoreArmZ_Auto6,  fTeach->edStoreArmZ_Auto7,  fTeach->edStoreArmZ_Auto8,  fTeach->edStoreArmZ_Auto9,  fTeach->edStoreArmZ_Auto10},
    /*eTrack3     */                   {fTeach->edStoreArmZ_Auto11, fTeach->edStoreArmZ_Auto12, fTeach->edStoreArmZ_Auto13, fTeach->edStoreArmZ_Auto14, fTeach->edStoreArmZ_Auto15},
    /*eTrack4     */                   {fTeach->edStoreArmZ_Auto16, fTeach->edStoreArmZ_Auto17, fTeach->edStoreArmZ_Auto18, fTeach->edStoreArmZ_Auto19, fTeach->edStoreArmZ_Auto20}};

    for(int iR=eTrack1; iR<=eTrack4; iR++)
    {
        TrayArmPara->iPush_AutoTrolly_Track_X[iR]=atoi(AutoTrolly_Track_X_Push[iR]->Text.c_str());
        TrayArmPara->iPush_AutoTrolly_ZDown[iR]  =atoi(AutoTrolly_Track_Z_Down[iR]->Text.c_str());

        TrayArmPara->iPos_Track_X[iR]=atoi(Track_X[iR]->Text.c_str());//+atoi(Track_X_Offset[iR]->Text.c_str());            //KenHsieh 20240830 : 推Tray 改為馬達不給調整Tray Arm offset
        TrayArmPara->iPos_Track_Y[iR]=atoi(Track_Y[iR]->Text.c_str());//+atoi(Track_Y_Offset[iR]->Text.c_str());            //KenHsieh 20240830 : 推Tray 改為馬達不給調整Tray Arm offset
        TrayArmPara->iPos_Track_Z[iR]=atoi(Track_Z[iR]->Text.c_str());//+atoi(Track_Z_Offset[iR]->Text.c_str());            //KenHsieh 20240830 : 推Tray 改為馬達不給調整Tray Arm offset

        TrayArmPara->iPush_Track_X[iR]=atoi(Track_X_Push[iR]->Text.c_str())+atoi(Track_X_Push_Offset[iR]->Text.c_str());    //推Tray 機構改為馬達

        for(int iC=eTrackCol1; iC<eTrackColTotal; iC++)
        {
            TrayArmPara->iPos_Storage_X[iR][iC]=atoi(Storage_X[iR][iC]->Text.c_str());
            TrayArmPara->iPos_Storage_Y[iR][iC]=atoi(Storage_Y[iR][iC]->Text.c_str());
            TrayArmPara->iPos_Storage_Z[iR][iC]=atoi(Storage_Z[iR][iC]->Text.c_str());
        }
    }

    TrayArmPara->iPos_Storage_Z_Up=atoi(fTeach->edStoreArmZ_Up->Text.c_str());
}
//------------------------------------------------------------------------------
__fastcall MyTrayArmParameter::MyTrayArmParameter()
{
    iNowTrack=eTrackNotUse;
    iNowAutoTray=eTrackNotUse;
    iTrayCount=0;
    bTrayArmTrayFeedFinish=false;
}
//------------------------------------------------------------------------------
void MyTrayArmParameter::DoStoreArm(int &Task)
{
    bool bFlag=false;
    switch(Task)
    {
        case 1:
            if(Auto->TrackHasTray()!=eTrackNotUse &&
               bSortStoreArmOneCycleFinish==false)
            {
                Task=100;
            }
            else if(HSys.Sys.RunMode==Run_CleanOut &&
                    bFlagAutoBinCleanoutFinish==false)                          //避免Auto Row尚有料，Cleanout完成，導致流程清零
            {
                Task=100;
            }
            else if(HSys.Sys.RunMode==Run_OneCycle)                             //Steven 20240707 : Fixed for one cycle
            {
                bSortStoreArmOneCycleFinish=true;
                if(Auto->TrackHasTray()!=eTrackNotUse)                          //JerryYang 20241211 : fix one cycle
                {
                    bSortStoreArmOneCycleFinish=false;
                }                
            }
            else if(HSys.Sys.RunMode==Run_CleanOut)                             //避免Auto Row尚有料，Cleanout完成，導致流程清零
            {
                for(int iR=eTrack1; iR<=eTrack4; iR++)
                {
                    for(int iC=eTrackCol1; iC<eTrackColTotal; iC++)
                    {
                        if(Auto->Mot_Auto_Work[iR][iC]!=NULL &&
                           Auto->Mot_Auto_Work[iR][iC]->fHasTray==true)
                            return;
                    }
                }
                if(TrayArmSuck->Suck[0][0].Item!=NULL_IC)
                    return;
                if(HSys.Sys.RunMode==Run_CleanOut)
                    bFlagTrackCleanoutFinish=true;
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed &&
                    bTrayArmTrayFeedFinish==false)                              //避免Auto Row尚有料，Cleanout完成，導致流程清零
            {
                Task=100;
            }
            break;
        case 100:
            DoLoaderTrayToStorage(true);
            Task=200;
            break;
        case 200:
            bFlag=DoLoaderTrayToStorage();
            if(bFlag)
            {
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
bool MyTrayArmParameter::DoLoaderTrayToStorage(bool bReset)
{
    AnsiString Str="";
    bool bResult=false;
    static bool bXCanMove=false, bPushTrayError=false, bFlag=false;             //推Tray 機構改為馬達
    bool bSoft_Simulate=(HSys.LastSet.iRealDummy==DUMMY);
    int &Task=iLoaderTrayToStorageTask;
    static int iTrayCount;
    int iRet=0;

    static bool bXMove=false, bZMove=false, bCylin=false;

    if(bReset)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            iNowTrack=Auto->TrackHasTray();

            if(TrayArmSuck->Suck[0][0].Item!=NULL_IC &&
               SnStoreArmTakeTray->IsOff() &&
               SnStoreArmHasTray->IsOff() &&
               HSys.LastSet.iRealDummy!=DUMMY)                                  //Sam 20240709 : 增加保護確認 SortTrayArm 資料
            {
                int iRet= ShowSystemError("80100", K_RETRY|K_SKIP);
                if(iRet==K_SKIP)
                    TrayArmSuck->SetItemData(0, 0, NULL_IC);
                else
                    break;
            }
            if(iNowTrack!=eTrackNotUse && TrayArmSuck->Suck[0][0].Item==NULL_IC)
            {
                Task=100;
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed &&
                    bTrayArmTrayFeedFinish==false)                          //避免Auto Row尚有料，Cleanout完成，導致流程清零
            {
                if(bEmpty1TrayFeedFinish==true)
                {
                    iNowTrack=Auto->TrackHasTray();
                    if(iNowTrack!=eTrackNotUse && TrayArmSuck->Suck[0][0].Item==NULL_IC)
                    {
                        Task=100;
                    }
                    else
                    {
                        bTrayArmTrayFeedFinish=true;
                        bResult=true;
                    }
                }
            }
            else
            {
                bResult=true;
            }
            break;
        case 100:
            MStoreArmX->ScanMotorStatus();
        #ifdef SOFT_SIMULATE
            bXCanMove=true;
        #else
            bXCanMove=MStoreArmX->Led[iHomeLed];
        #endif
            if(bXCanMove)
            {
                Task=150;
            }
            else
            {
                ShowMyMessage("Please Check that the Store Arm X is not at home point!");
            }
            break;
        case 150:
            if(DoMoveTrayArmX((int)taxp_Standby)==true)
            {
                Task=200;
            }
            break;
        case 200:
            if(TrayArmToTrackNo(iNowTrack,__FUNC__,Task)==true)
            {
                C_Track_Stop[iNowTrack]->Reset();
                Track_Dete[iNowTrack]->On();

                if(HSys.FuncT.iT06_AutoRowType==1)
                {
                    bXMove=false;
                    bZMove=false;
                    Task=210;
                }
                else
                {
                    Task=300;
                }
            }
            break;
        case 210:
            if(bZMove==false)
                bZMove=MCarrierStoreZ[iNowTrack]->MotorMove(iPush_AutoTrolly_ZDown[iNowTrack]);

            if(bZMove)
            {
                bXMove=false;
                bZMove=false;
                Task=215;
            }
            break;
        case 215:
            if(bXMove==false)
                bXMove=MCarrierStoreX[iNowTrack]->MotorMove(iPush_AutoTrolly_Track_X[iNowTrack]-(6800*2+3000));

            if(bXMove)
            {
                C_AutoTrolly_Stop[iNowTrack]->Reset();
                MCarrierStoreX[iNowTrack]->SetPersentSpeed(5, false);
                bCylin=false;
                bXMove=false;
                Task=220;
            }
            break;
        case 220:
            if(bCylin==false)
                bCylin=C_AutoTrolly_Stop[iNowTrack]->Push();

            if(bCylin==true && bXMove==false)
                bXMove=MCarrierStoreX[iNowTrack]->MotorMove(iPush_AutoTrolly_Track_X[iNowTrack]);

            if(bCylin && bXMove)
            {
                MCarrierStoreX[iNowTrack]->SetPersentSpeed(MCarrierStoreX[iNowTrack]->GetPersentSpeed());
                bCylin=false;
                bXMove=false;
                C_AutoTrolly_Stop[iNowTrack]->Reset();
                Task=230;
            }
            break;
        case 230:
            if(bXMove==false)
                bXMove=MCarrierStoreX[iNowTrack]->MotorMove(iPush_AutoTrolly_Track_X[iNowTrack]-(6800*2+3000));

            if(bXMove==true && bCylin==false)
                bCylin=C_AutoTrolly_Stop[iNowTrack]->Pop();

            if(bCylin && bXMove)
            {
                bCylin=false;
                bXMove=false;

                Task=500;
            }
            break;
        case 300:
            if(C_Track_Stop[iNowTrack]->Pop())
            {
                if((SnStoreArmTakeTray->IsOn()==false && SnStoreArmHasTray->IsOn()==false) || bSoft_Simulate)
                {
                    if(USE_PUSH_TRAY==ePushByMotor)
                    {
                        bPushTrayError=false;
                        C_StoreArm_EdgeFixer->Reset();
                        C_Track_PushTray[iNowTrack]->Reset();
                        Task=310;
                    }
                    else
                    {
                        Track_CW[iNowTrack]->On();
                        C_StoreArm_EdgeFixer->Reset();
                        C_Track_PushTray[iNowTrack]->Reset();
                        tTrayToStoreTimerOut.SetSecAndOn(10);                       //Sam 20240618 : auto to store track timeout
                        Task=305;
                    }
                }
                else
                {
                    ShowMyMessage(AnsiString().sprintf("SortTrayArm has unknown tray. Func:%s Task:%d",__FUNC__,Task));          //上方無Tray盤
                }
            }
            break;
        case 305:
            Track_CW[iNowTrack]->On();
            if((SnStoreArmTakeTray->IsOn() &&
                SnStoreArmHasTray->IsOn()  &&
               (Sen_AutoRowULDCheck[iNowTrack]->Enable==false || Sen_AutoRowULDCheck[iNowTrack]->IsOff())) || bSoft_Simulate)
            {
                if(C_Track_PushTray[iNowTrack]->Push())
                {
                    C_Track_PushTray[iNowTrack]->Reset();
                    Task=420;
                }
            }
            else if(tTrayToStoreTimerOut.Off())                                 //Sam 20240618 : auto to store track timeout
            {
                Str.sprintf("Track %d to store timeout!!!", iNowTrack);
                ShowSystemError(HSys.Sen.SnStoreArmHasTray.Name , K_RETRY, 0, Str);    //Sam 20240709 : 新增 Alarm Code
                tTrayToStoreTimerOut.SetSecAndOn(10);
            }
            break;
        case 310:                                                               //推Tray 機構改為馬達
            if(TrayArmToTrackNo(iNowTrack,__FUNC__,Task)==true)
            {
                if(C_Track_PushTray[iNowTrack]->Push())
                {
                    if(Sen_TrackCyPushTray[iNowTrack]->IsOn() || bSoft_Simulate)
                    {
                        C_Track_PushTray[iNowTrack]->Reset();
                        Task=320;
                    }
                    else
                    {
                        Str.sprintf("Track %d push tray cylinder on error", iNowTrack);
                        ShowMyMessage(Str);
                    }
                }
            }
            break;
        case 320:                                                               //推Tray 機構改為馬達
            if(MPushStoreArmX[iNowTrack]->MotorMove(iPush_Track_X[iNowTrack])==true)
            {
                Task=340;
            }
            else if(Sen_PushTrayCheck[iNowTrack]->IsOn())
            {
                Str.sprintf("Track %d push tray error",iNowTrack);
                ShowMyMessage(Str);
                fHome->SingleMotorHome(true, MPushStoreArmX[iNowTrack]->Tag);   //KenHsieh 20240830 : 推Tray 馬達退縮後須回Home 重跑
                Task=345;
            }
            break;
        case 340:                                                               //推Tray 機構改為馬達
            if(MPushStoreArmX[iNowTrack]->MotorMove(iTrayPushStartPos))
            {
                if(bFlag==false)                                                //KenHsieh 20240830 : add delay判斷推Tray 馬達SnHome
                {
                    bFlag=true;
                    PushTrayHomeDelay.SetSecAndOn(0.5);
                }
                #ifdef SOFT_SIMULATE
                Task=400;
                #else
                MPushStoreArmX[iNowTrack]->ScanMotorStatus();
                if(MPushStoreArmX[iNowTrack]->Led[iHomeLed]==true || bSoft_Simulate)
                {
                    Task=400;
                }
                else if(PushTrayHomeDelay.Off())                                //KenHsieh 20240830 : add delay判斷推Tray 馬達SnHome
                {
                    ShowMyMessage(AnsiString().sprintf("Track %d push tray motor home sensor is not on, please check!", iNowTrack));
                    Task=350;
                }
                #endif
#ifdef SOFT_SIMULATE
                Task=400;
#endif
            }
            break;
        case 345:                                                               //KenHsieh 20240830 : 推Tray 馬達退縮後須回Home 重跑
            if(fHome->SingleMotorHome(false, MPushStoreArmX[iNowTrack]->Tag))
            {
                Task=310;
            }
            break;
        case 350:                                                               //KenHsieh 20240830 : 推Tray 馬達SnHome 未亮後需判斷是否回Home
            MPushStoreArmX[iNowTrack]->ScanMotorStatus();
            if(MPushStoreArmX[iNowTrack]->Led[iHomeLed]==true)
            {
                Task=400;
            }
            else
            {
                iRet=ShowMyMessageBox_YES_NO(AnsiString().sprintf("If Track %d push tray motor need home, please press YES", iNowTrack));
                if(iRet==1)
                {
                    fHome->SingleMotorHome(true, MPushStoreArmX[iNowTrack]->Tag);
                    Task=355;
                }
                else
                {
                    MPushStoreArmX[iNowTrack]->ScanMotorStatus();
                    if(MPushStoreArmX[iNowTrack]->Led[iHomeLed]==true)
                    {
                        Task=400;
                    }
                    else
                    {
                        ShowMyMessage(AnsiString().sprintf("Track %d push tray motor home sensor is not on, please check!", iNowTrack));
                        Task=350;
                    }
                }
            }
            break;
        case 355:                                                               //KenHsieh 20240830 : 推Tray 馬達SnHome 未亮後需判斷是否回Home
            if(fHome->SingleMotorHome(false, MPushStoreArmX[iNowTrack]->Tag))
            {
                Task=400;
            }
            break;
        case 400:
            if((SnStoreArmTakeTray->IsOn() &&
                SnStoreArmHasTray->IsOn()  &&
               (Sen_AutoRowULDCheck[iNowTrack]->Enable==false || Sen_AutoRowULDCheck[iNowTrack]->IsOff())) ||
                bSoft_Simulate)
            {
                Task=420;
            }
            else
            {
                Str.sprintf("Track %d to Tray Arm is not in Position!", iNowTrack);
                ShowSystemError(HSys.Sen.SnStoreArmHasTray.Name , K_RETRY, 0, Str);    //Sam 20240709 : 新增 Alarm Code
                Task=400;
            }
            break;
        case 420:                                                               //推Tray 機構改為馬達
            if(C_Track_PushTray[iNowTrack]->Pop())
            {
                if(USE_PUSH_TRAY==ePushByMotor)
                {
                    if(Sen_TrackCyPushTray[iNowTrack]->IsOff() || bSoft_Simulate)
                    {
                        Task=500;
                    }
                    else
                    {
                        Str.sprintf("Track %d push tray cylinder off error", iNowTrack);
                        ShowMyMessage(Str);
                        Task=420;
                    }
                }
                else
                {
                    Task=500;
                }
            }
            break;
        case 500:                                                               //KenHsieh(Jimmychiu) 20240722 : Auto Full Tray to tray arm off EdgeFixer switch when EdgeFixer on timeout
            C_StoreArm_EdgeFixer->On();
            TrayArmDelay.SetMSAndOn(C_StoreArm_EdgeFixer->OnAlarmTime);         //Jimmychiu 20240721 : Auto Full Tray to tray arm off EdgeFixer switch when EdgeFixer on timeout
            Task=510;
            break;
        case 510:                                                               //Track 資料移載
            if(C_StoreArm_EdgeFixer->IsOff()==false)                                    //Jimmychiu 20240721 : Auto Full Tray to tray arm off EdgeFixer switch when EdgeFixer on timeout
            {
                Track_CW[iNowTrack]->Off();
                Track_Dete[iNowTrack]->Off();
                iNowAutoTray=Auto->MTrackToStore[iNowTrack]->Tray.iTrackCol;
                TrayArmSuck->SetItemData(0, 0, HAS_IC);
                MtTrayArmTray->MoveTrayFrom(Auto->MTrackToStore[iNowTrack]);
                //Auto->MTrackToStore[iNowTrack]->ClearTray();
                TrayArmDelay.SetMSAndOn(C_StoreArm_EdgeFixer->OnDelayTime);     //延遲確認到位後後再移動
                Task=520;
            }
            else if(TrayArmDelay.Off())                                         //Jimmychiu 20240721 : Auto Full Tray to tray arm off EdgeFixer switch when EdgeFixer on timeout
            {
                C_StoreArm_EdgeFixer->Off();
                ShowMyMessage(AnsiString().sprintf("Track %d Store Arm Edge Fixer Timeout!",iNowTrack));
                Task=500;   //JerryYang 20240918 : modify
            }
            break;
        case 520:
            if(TrayArmDelay.Off())
            {
                if(DoMoveTrayArmToStorageStandby(iNowTrack,iNowAutoTray)==true)
                {
                    if(CosFunction.bAutoStoreFullAlarmByCount)                 //Jimmychiu 20260112 : Auto Store Full Alarm By Count
                    {
                        iTrayCount=atoi(Auto_Store[iNowTrack][iNowAutoTray]->Caption.c_str());
                        RecordProcess(AnsiString().sprintf("LTS520 AlarmByCount Track=%d Col=%d Cnt=%d", iNowTrack+1, iNowAutoTray+1, iTrayCount));   //AI(HT172-Maintainer) 20260420 : diag missing 10-tray alarm
                        bool bNeedAlarm=(iTrayCount>0 && iTrayCount%10==0);
                        if(bNeedAlarm)
                        {
                            mBinToTray.ConvertRowColToBin(iNowTrack, iNowAutoTray);
                            Str.sprintf("Alarm by count=%d ,Store[%d, %d] = Bin:%d",iTrayCount, iNowTrack+1, iNowAutoTray+1, mBinToTray.iBin);
                           ShowSystemError(Sen_Store_Full[iNowTrack][iNowAutoTray]->Name, K_RETRY, 0, Str);
                        }
                    }
                    Task=600;
                }
            }
            break;
        case 600:
            TrayArmDelay.SetSecAndOn(iNormalDelayTimeSec);
            Task=601;
//            break;
        case 601:
            if(SnStoreArmPutPos1->IsOn() || bSoft_Simulate)
            {
                #ifndef DISABLE_STORE
                if(Sen_Store_Full[iNowTrack][iNowAutoTray]->IsOn())             //JerryYang 20240615 : 提前判斷Store區滿TRAY, 不然OP取料有風險       //該Store已滿 請清除
                {
                    mBinToTray.ConvertRowColToBin(iNowTrack, iNowAutoTray);
                    Str.sprintf("Store[%d, %d] = Bin:%d", iNowTrack+1, iNowAutoTray+1, mBinToTray.iBin);
                    ShowSystemError(Sen_Store_Full[iNowTrack][iNowAutoTray]->Name, K_RETRY, 0, Str);
                    break;
                }
                #endif

                if(SnStoreArmHasTray->Enable  &&
                   SnStoreArmHasTray->IsOff() &&
                   bSoft_Simulate==false)                                       //KenHsieh 20240718 : 補上SnStoreArmHasTray 異常
                {
                    ShowSystemError(SnStoreArmHasTray->OffAlarmCode, K_RETRY);
                    break;
                }

                if(DoMoveTrayArmX((int)taxp_Storage,iNowTrack,iNowAutoTray))
                {
                    if(SnStoreArmPutPos2->IsOn() || bSoft_Simulate)
                    {
                        Task=700;
                    }
                    else
                    {
                        Task=600;
                        ShowTrayArmMessage("The sensor named SnStoreArmPutPos2 must be True.",__FUNC__,Task);   //Jimmychiu 20250512 : The sensor named SnStoreArmPutPos1 must be True
                    }
                }
            }
            else if(TrayArmDelay.Off())                                         //Jimmychiu 20250512 : The sensor named SnStoreArmPutPos1 must be True
            {
                ShowMyMessage(AnsiString().sprintf("The sensor named SnStoreArmPutPos1 must be True. Task:%d",Task));
                Task=600;
            }
            break;
        case 700:
            #ifdef DISABLE_STORE
                TrayArmDelay.SetSecAndOn(3);
                C_StoreArm_EdgeFixer->Off();
                Task=800;
            #else
            if((Sen_Store_Full[iNowTrack][iNowAutoTray]->IsOff() &&
                Sen_Store_Door[iNowTrack][iNowAutoTray]->IsOn()) ||
                bSoft_Simulate)
            {
                TrayArmDelay.SetSecAndOn(3);
                C_StoreArm_EdgeFixer->Off();
                Task=800;
            }
            else if(Sen_Store_Full[iNowTrack][iNowAutoTray]->IsOn())            //該Store已滿 請清除
            {
                mBinToTray.ConvertRowColToBin(iNowTrack, iNowAutoTray);
                EventReport(SECS_EVENT.Store01Full+(mBinToTray.iWhichAuto-eAuto1));
                Str.sprintf("Store[%d, %d] = Bin:%d", iNowTrack+1, iNowAutoTray+1, mBinToTray.iBin);

                ShowSystemError(Sen_Store_Full[iNowTrack][iNowAutoTray]->Name, K_RETRY, 0, Str);
            }
            else if(Sen_Store_Door[iNowTrack][iNowAutoTray]->IsOff())           //該Store 門未關 請關閉
            {
                mBinToTray.ConvertRowColToBin(iNowTrack, iNowAutoTray);
                Str.sprintf("Store[%d, %d] = Bin:%d", iNowTrack+1, iNowAutoTray+1, mBinToTray.iBin);

                ShowSystemError(Sen_Store_Door[iNowTrack][iNowAutoTray]->Name, K_RETRY, 0, Str);
            }
            #endif
            break;
        case 800:
            if(C_StoreArm_EdgeFixer->Pop() || bSoft_Simulate==true)
            {
                Task=810;
            }
            break;
        case 810:
            if(SnStoreArmHasTray->IsOff() &&
               SnStoreArmTakeTray->IsOff() &&
               bSoft_Simulate==false)                                           //KenHsieh 20240718 : 補上SnStoreArmHasTray 異常
            {
                ShowSystemError(SnStoreArmHasTray->OffAlarmCode, K_RETRY);
                break;
            }
            if(DoMoveTrayArmZInStorage(true,iNowTrack,iNowAutoTray)==true)
            {
                TrayArmDelay.SetSecAndOn(2);
                Task=900;
            }
            break;
        case 900:
            if(TrayArmDelay.Off())
            {
                if(DoMoveTrayArmZInStorage(false,iNowTrack,iNowAutoTray)==true)                                                    //交換資料
                {
                    Task=950;
                }
            }
            break;
        case 950:
            if(SnStoreArmHasTray->IsOn() || SnStoreArmTakeTray->IsOn())
            {
                ShowMyMessage("Please remove the tray from Store Arm");
            }
            else
            {
                iTrayCount=atoi(Auto_Store[iNowTrack][iNowAutoTray]->Caption.c_str());
                iTrayCount++;
                Auto_Store[iNowTrack][iNowAutoTray]->Caption=iTrayCount;
                TrayArmSuck->SetItemData(0, 0, NULL_IC);
                MtTrayArmTray->ClearTray();
                Task=1000;
            }
            break;
        case 1000:
            if(DoMoveTrayArmZInStorage(false,iNowTrack,iNowAutoTray)==true)                                                    //交換資料
            {
                if(DoMoveTrayArmX((int)taxp_Standby)==true)
                {
                    Task=1;
                    bResult=true;
                }
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
void MyTrayArmParameter::NewDoAutoTrayEdgeCylinderLoop()                        //JerryYang 20170531 (wei) 敲擊方式改成可以分別設定on off時間
{
    if(HSys.Sys.SystemStart==false)
        return;

    static bool EdgePushLoop=false;
    static bool bFirst=true;

    if(bFirst==true)
    {
        NewLoopEdgeDelay.SetSecAndOn(0.5);
        EdgePushLoop=false;
        bFirst=false;
    }

    if(NewLoopEdgeDelay.Off() && EdgePushLoop==false)
    {
        NewLoopEdgeOnDelay.SetSecAndOn(2);
        EdgePushLoop=true;

        if(HSys.FuncT.bT01_AutoRow1Vib1)
            AutoRowVib1[eTrack1]->On();
        if(HSys.FuncT.bT01_AutoRow1Vib2)
            AutoRowVib2[eTrack1]->On();
        if(HSys.FuncT.bT01_AutoRow2Vib1)
            AutoRowVib1[eTrack2]->On();
        if(HSys.FuncT.bT01_AutoRow2Vib2)
            AutoRowVib2[eTrack2]->On();
        if(HSys.FuncT.bT01_AutoRow3Vib1)
            AutoRowVib1[eTrack3]->On();
        if(HSys.FuncT.bT01_AutoRow3Vib2)
            AutoRowVib2[eTrack3]->On();
        if(HSys.FuncT.bT01_AutoRow4Vib1)
            AutoRowVib1[eTrack4]->On();
        if(HSys.FuncT.bT01_AutoRow4Vib2)
            AutoRowVib2[eTrack4]->On();
    }

    if(NewLoopEdgeOnDelay.Off() && EdgePushLoop==true)
    {
        NewLoopEdgeDelay.SetSecAndOn(0.5);
        EdgePushLoop=false;

        for(int i=eTrack1; i<=eTrack4; i++)
        {
            AutoRowVib1[i]->Off();
            AutoRowVib2[i]->Off();
        }
    }
}
//------------------------------------------------------------------------------
void MyTrayArmParameter::ClearTrayCount()
{
    RecordProcess("ClearTrayCount called");   //AI(HT172-Maintainer) 20260420 : diag tray count clear path
    for(int i=eTrack1; i<=eTrack4; i++)
    {
        for(int j=eTrackCol1; j<eTrackColTotal; j++)
        {
            Auto_Store[i][j]->Caption=0;
        }
    }
}
//------------------------------------------------------------------------------
bool MyTrayArmParameter::InitialPushTray(bool bReset, int iNowTrack)            //推Tray 機構改為馬達
{
    bool bResult=false;
    static bool bCyFin[eTrackCount], bMotorFin[eTrackCount];
    AnsiString Str="";

    if(bReset)
    {
        bCyFin[iNowTrack]=false;
        bMotorFin[iNowTrack]=false;
        PushTrayCyDelay[iNowTrack].SetSecAndOn(5);
        PushTrayMotDelay[iNowTrack].SetSecAndOn(5);
        bResult=true;
    }
    else
    {
        if(bCyFin[iNowTrack]==false)
        {
#ifndef SOFT_SIMULATE
            if(C_Track_PushTray[iNowTrack]->Pop() && Sen_TrackCyPushTray[iNowTrack]->IsOff())
            {
                bCyFin[iNowTrack]=true;
            }
            else if(PushTrayCyDelay[iNowTrack].Off())
            {
                Str.sprintf("Track %d push tray cylinder off error", iNowTrack);
                ShowMyMessage(Str);
            }
#else
            bCyFin[iNowTrack]=true;
#endif
        }


        if(bMotorFin[iNowTrack]==false)
        {
            MPushStoreArmX[iNowTrack]->ScanMotorStatus();
            if(MPushStoreArmX[iNowTrack]->MotorMove(iTrayPushStartPos) &&
               MPushStoreArmX[iNowTrack]->Led[iHomeLed]==true)
            {
                bMotorFin[iNowTrack]=true;
            }
            else if(PushTrayMotDelay[iNowTrack].Off())
            {
                Str.sprintf("Track %d push tray motor home sensor is not on.", iNowTrack);
                ShowMyMessage(Str);
            }
            #ifdef SOFT_SIMULATE
            bMotorFin[iNowTrack]=true;
            #endif
        }

        if(bCyFin[iNowTrack] && bMotorFin[iNowTrack])
            bResult=true;
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool MyTrayArmParameter::TrayArmToTrackNo(int iTrackNum,AnsiString sFunc,int iTask)
{
    if(SnStoreArmHasTray->IsOn() || SnStoreArmTakeTray->IsOn())
    {
        ShowSystemError("80101", K_RETRY,0,AnsiString().sprintf("Func:%s Task:%d",sFunc,iTask));  //Sam 20240709 : 新增 Alarm Code
        return false;
    }
    bool bYMove=false,bZMove=false;
    bYMove=MStoreArmY->MotorMove(iPos_Track_Y[iTrackNum]);
    bZMove=MStoreArmZ->MotorMove(iPos_Track_Z[iTrackNum]);
    if(bYMove && bZMove)
    {
        if(DoMoveTrayArmX((int)taxp_TrackReceive,iTrackNum)==true)
        {
           return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool MyTrayArmParameter::DoMoveTrayArmX(int iPos,int iTrackNum,int iAutoTray)
{
    bool bXMove=false;
    if(iPos==(int)taxp_Standby)
    {
        bXMove=MStoreArmX->MotorMove(iTrayPushStartPos);
    }
    else if(iPos==(int)taxp_TrackReceive && iTrackNum>=0)
    {
        bXMove=MStoreArmX->MotorMove(iPos_Track_X[iTrackNum]);
    }
    else if(iPos==(int)taxp_Storage && iTrackNum>=0 && iAutoTray>=0)
    {
        bXMove=MStoreArmX->MotorMove(GetStorageX(iTrackNum,iAutoTray));
    }
    else
    {
        ShowTrayArmMessage(AnsiString().sprintf("Wrong Cmd iPos=%d,iTrackNum=%d,iAutoTray=%d",iPos,iTrackNum,iAutoTray),__FUNC__,0);
    }
    return bXMove;
}
//------------------------------------------------------------------------------
bool MyTrayArmParameter::DoMoveTrayArmToStorageStandby(int iTrackNum,int iAutoTray)
{
    bool bYMove=false,bZMove=false;
    if(DoMoveTrayArmX((int)taxp_Standby)==true)
    {
        bYMove=MStoreArmY->MotorMove(GetStorageY(iTrackNum,iAutoTray));
        bZMove=MStoreArmZ->MotorMove(GetStorageZ(iTrackNum,iAutoTray));
    }
    return (bYMove==true && bZMove==true);
}
//------------------------------------------------------------------------------
bool MyTrayArmParameter::DoMoveTrayArmZInStorage(bool bUp,int iTrackNum,int iAutoTray)
{
    bool bflag=false;
    int iTarPos=GetStorageZ(iTrackNum,iAutoTray);
    if(bUp)
    {
        iTarPos+=iPos_Storage_Z_Up;
    }
    if(MStoreArmZ->MotorMove(iTarPos))
    {
        bflag=true;
    }
    return bflag;
}
//------------------------------------------------------------------------------
void MyTrayArmParameter::ShowTrayArmMessage(AnsiString sMsg,AnsiString sFunc,int iTask)
{
    ShowMyMessage(AnsiString().sprintf("%s Func:%s Task:%d",sMsg,sFunc,iTask));
}
//------------------------------------------------------------------------------
int MyTrayArmParameter::GetStorageX(int iTrackNum,int iAutoTray)
{
    int iPos=0;
    if(IsTrayAutoInRange(iTrackNum,iAutoTray))
    {
        iPos=iPos_Storage_X[iTrackNum][iAutoTray];
    }
    else
    {
        iPos=iPos_Storage_X[0][0];
    }
    return iPos;
}
//------------------------------------------------------------------------------
int MyTrayArmParameter::GetStorageY(int iTrackNum,int iAutoTray)
{
    int iPos=0;
    if(IsTrayAutoInRange(iTrackNum,iAutoTray))
    {
        iPos=iPos_Storage_Y[iTrackNum][iAutoTray];
    }
    else
    {
        iPos=iPos_Storage_Y[0][0];
    }
    return iPos;
}
//------------------------------------------------------------------------------
int MyTrayArmParameter::GetStorageZ(int iTrackNum,int iAutoTray)
{
    int iPos=0;
    if(IsTrayAutoInRange(iTrackNum,iAutoTray))
    {
        iPos=iPos_Storage_Z[iTrackNum][iAutoTray];
    }
    else
    {
        iPos=iPos_Storage_Z[0][0];
    }
    return iPos;
}
//------------------------------------------------------------------------------
bool MyTrayArmParameter::IsTrayAutoInRange(int iTrackNum,int iAutoTray)
{
    if(0<=iTrackNum && iTrackNum<eTrackCount &&
       0<=iAutoTray && iAutoTray<eTrackColTotal)
    {
        return true;
    }
    else
    {
        ShowTrayArmMessage(AnsiString().sprintf("Storage Position error. iTrackNum=%d,iAutoTray=%d",iTrackNum,iAutoTray),__FUNC__,0);
        return false;
    }
}
//------------------------------------------------------------------------------
