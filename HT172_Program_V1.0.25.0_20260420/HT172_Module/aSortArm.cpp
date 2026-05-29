#include "IncludeAllHeader.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#include "aSortArm.h"

#include "DecodeXML.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
MySortArmParameter *MySortArm=new MySortArmParameter();

MySortArmParameter *SortArmPara=MySortArm;
bool bSuckEnd[4][2]={{false, false}, {false, false}, {false, false}, {false, false}}; //JerryYang 20240623 : pick up error可以SKIP
//---------------------------------------------------------------------------
void InitSortArm()
{
    SortArmPara->SortArmSuck        =&HSys.Suck.AutoArmSuck;

    SortArmPara->MSortArmX          =HSys.Mot.SortArm1X;
    SortArmPara->MSortArmY          =HSys.Mot.SortArm1Y;

    SortArmPara->MSortArmPitchX     =HSys.Mot.SortArm1PitchX;
    SortArmPara->MSortArmPitchY1    =HSys.Mot.SortArm1PitchY1;
    SortArmPara->MSortArmPitchY2    =HSys.Mot.SortArm1PitchY2;

    SortArmPara->MSortArmZ[0][0]    =HSys.Mot.SortArm1ZA;
    SortArmPara->MSortArmZ[0][1]    =HSys.Mot.SortArm1ZB;
    SortArmPara->MSortArmZ[1][0]    =HSys.Mot.SortArm1ZC;
    SortArmPara->MSortArmZ[1][1]    =HSys.Mot.SortArm1ZD;
    SortArmPara->MSortArmZ[2][0]    =HSys.Mot.SortArm1ZE;
    SortArmPara->MSortArmZ[2][1]    =HSys.Mot.SortArm1ZF;
    SortArmPara->MSortArmZ[3][0]    =HSys.Mot.SortArm1ZG;
    SortArmPara->MSortArmZ[3][1]    =HSys.Mot.SortArm1ZH;

    SortArmPara->MotWorkLoader      =HSys.VMot.MotWorkLoader;
    SortArmPara->MotNowSortTray     =HSys.VMot.NowSortTray;
    SortArmPara->MotLoader_Car      =HSys.VMot.LoadTray_Car;

    SortArmPara->SnAutoTrayDetect[0]=&HSys.Sen.SnTrackTrayDetect1;
    SortArmPara->SnAutoTrayDetect[1]=&HSys.Sen.SnTrackTrayDetect2;
    SortArmPara->SnAutoTrayDetect[2]=&HSys.Sen.SnTrackTrayDetect3;
    SortArmPara->SnAutoTrayDetect[3]=&HSys.Sen.SnTrackTrayDetect4;

    for(int iC=0; iC<SortArmPara->SortArmSuck->MaxItemC; iC++)
    {
        for(int iR=0; iR<SortArmPara->SortArmSuck->MaxItemR; iR++)
        {
            SortArmPara->bCheckSortArmDestroyActive[iR][iC]=false;
            SortArmPara->bCheckSortArmDestroyActive[iR][iC]=false;
            SortArmPara->bCheckSortArmDestroyActiveFinish[iR][iC]=true;
            SortArmPara->bCheckSortArmDestroyActiveFinish[iR][iC]=true;
            SortArmPara->iTask[iR][iC]=0;
            SortArmPara->iTask[iR][iC]=0;
            SortArmPara->bSortArmCheckDestroyACT[iR][iC]=false;
            SortArmPara->bSortArmCheckDestroyACTBuf[iR][iC]=false;

            SortArmPara->bSortArmCheckDestroyACT[iR][iC]=false;
            SortArmPara->bSortArmCheckDestroyACTBuf[iR][iC]=false;
        }
    }
}
//---------------------------------------------------------------------------
void UpdateSortArmParameter()
{
    int iPosZ_Offset[4][2], iOffsetPosZ_Offset[4][2], iOffsetAutoZ[4][2];

    TEdit *Auto_X[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fTeach->edSortArm1X_Auto1,  fTeach->edSortArm1X_Auto2,  fTeach->edSortArm1X_Auto3,  fTeach->edSortArm1X_Auto4,  fTeach->edSortArm1X_Auto5 },
    /*eTrack1     */     {fTeach->edSortArm1X_Auto1,  fTeach->edSortArm1X_Auto2,  fTeach->edSortArm1X_Auto3,  fTeach->edSortArm1X_Auto4,  fTeach->edSortArm1X_Auto5 },
    /*eTrack2     */     {fTeach->edSortArm1X_Auto6,  fTeach->edSortArm1X_Auto7,  fTeach->edSortArm1X_Auto8,  fTeach->edSortArm1X_Auto9,  fTeach->edSortArm1X_Auto10},
    /*eTrack3     */     {fTeach->edSortArm1X_Auto11, fTeach->edSortArm1X_Auto12, fTeach->edSortArm1X_Auto13, fTeach->edSortArm1X_Auto14, fTeach->edSortArm1X_Auto15},
    /*eTrack4     */     {fTeach->edSortArm1X_Auto16, fTeach->edSortArm1X_Auto17, fTeach->edSortArm1X_Auto18, fTeach->edSortArm1X_Auto19, fTeach->edSortArm1X_Auto20}};

    TEdit *Auto_Y[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fTeach->edSortArm1Y_Auto1,  fTeach->edSortArm1Y_Auto2,  fTeach->edSortArm1Y_Auto3,  fTeach->edSortArm1Y_Auto4,  fTeach->edSortArm1Y_Auto5 },
    /*eTrack1     */     {fTeach->edSortArm1Y_Auto1,  fTeach->edSortArm1Y_Auto2,  fTeach->edSortArm1Y_Auto3,  fTeach->edSortArm1Y_Auto4,  fTeach->edSortArm1Y_Auto5 },
    /*eTrack2     */     {fTeach->edSortArm1Y_Auto6,  fTeach->edSortArm1Y_Auto7,  fTeach->edSortArm1Y_Auto8,  fTeach->edSortArm1Y_Auto9,  fTeach->edSortArm1Y_Auto10},
    /*eTrack3     */     {fTeach->edSortArm1Y_Auto11, fTeach->edSortArm1Y_Auto12, fTeach->edSortArm1Y_Auto13, fTeach->edSortArm1Y_Auto14, fTeach->edSortArm1Y_Auto15},
    /*eTrack4     */     {fTeach->edSortArm1Y_Auto16, fTeach->edSortArm1Y_Auto17, fTeach->edSortArm1Y_Auto18, fTeach->edSortArm1Y_Auto19, fTeach->edSortArm1Y_Auto20}};

    TEdit *Auto_Z[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fTeach->edSortArm1ZD_Auto1,  fTeach->edSortArm1ZD_Auto2,  fTeach->edSortArm1ZD_Auto3,  fTeach->edSortArm1ZD_Auto4,  fTeach->edSortArm1ZD_Auto5 },
    /*eTrack1     */     {fTeach->edSortArm1ZD_Auto1,  fTeach->edSortArm1ZD_Auto2,  fTeach->edSortArm1ZD_Auto3,  fTeach->edSortArm1ZD_Auto4,  fTeach->edSortArm1ZD_Auto5 },
    /*eTrack2     */     {fTeach->edSortArm1ZD_Auto6,  fTeach->edSortArm1ZD_Auto7,  fTeach->edSortArm1ZD_Auto8,  fTeach->edSortArm1ZD_Auto9,  fTeach->edSortArm1ZD_Auto10},
    /*eTrack3     */     {fTeach->edSortArm1ZD_Auto11, fTeach->edSortArm1ZD_Auto12, fTeach->edSortArm1ZD_Auto13, fTeach->edSortArm1ZD_Auto14, fTeach->edSortArm1ZD_Auto15},
    /*eTrack4     */     {fTeach->edSortArm1ZD_Auto16, fTeach->edSortArm1ZD_Auto17, fTeach->edSortArm1ZD_Auto18, fTeach->edSortArm1ZD_Auto19, fTeach->edSortArm1ZD_Auto20}};

    TEdit *OffSet_Auto_X[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fOffset->edSortArm1X_Auto1,  fOffset->edSortArm1X_Auto2,  fOffset->edSortArm1X_Auto3,  fOffset->edSortArm1X_Auto4,  fOffset->edSortArm1X_Auto5 },
    /*eTrack1     */     {fOffset->edSortArm1X_Auto1,  fOffset->edSortArm1X_Auto2,  fOffset->edSortArm1X_Auto3,  fOffset->edSortArm1X_Auto4,  fOffset->edSortArm1X_Auto5 },
    /*eTrack2     */     {fOffset->edSortArm1X_Auto6,  fOffset->edSortArm1X_Auto7,  fOffset->edSortArm1X_Auto8,  fOffset->edSortArm1X_Auto9,  fOffset->edSortArm1X_Auto10},
    /*eTrack3     */     {fOffset->edSortArm1X_Auto11, fOffset->edSortArm1X_Auto12, fOffset->edSortArm1X_Auto13, fOffset->edSortArm1X_Auto14, fOffset->edSortArm1X_Auto15},
    /*eTrack4     */     {fOffset->edSortArm1X_Auto16, fOffset->edSortArm1X_Auto17, fOffset->edSortArm1X_Auto18, fOffset->edSortArm1X_Auto19, fOffset->edSortArm1X_Auto20}};

    TEdit *OffSet_Auto_Y[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fOffset->edSortArm1Y_Auto1,  fOffset->edSortArm1Y_Auto2,  fOffset->edSortArm1Y_Auto3,  fOffset->edSortArm1Y_Auto4,  fOffset->edSortArm1Y_Auto5 },
    /*eTrack1     */     {fOffset->edSortArm1Y_Auto1,  fOffset->edSortArm1Y_Auto2,  fOffset->edSortArm1Y_Auto3,  fOffset->edSortArm1Y_Auto4,  fOffset->edSortArm1Y_Auto5 },
    /*eTrack2     */     {fOffset->edSortArm1Y_Auto6,  fOffset->edSortArm1Y_Auto7,  fOffset->edSortArm1Y_Auto8,  fOffset->edSortArm1Y_Auto9,  fOffset->edSortArm1Y_Auto10},
    /*eTrack3     */     {fOffset->edSortArm1Y_Auto11, fOffset->edSortArm1Y_Auto12, fOffset->edSortArm1Y_Auto13, fOffset->edSortArm1Y_Auto14, fOffset->edSortArm1Y_Auto15},
    /*eTrack4     */     {fOffset->edSortArm1Y_Auto16, fOffset->edSortArm1Y_Auto17, fOffset->edSortArm1Y_Auto18, fOffset->edSortArm1Y_Auto19, fOffset->edSortArm1Y_Auto20}};

    TEdit *OffSet_Auto_Z[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fOffset->edSortArm1ZD_Auto1,  fOffset->edSortArm1ZD_Auto2,  fOffset->edSortArm1ZD_Auto3,  fOffset->edSortArm1ZD_Auto4,  fOffset->edSortArm1ZD_Auto5 },
    /*eTrack1     */     {fOffset->edSortArm1ZD_Auto1,  fOffset->edSortArm1ZD_Auto2,  fOffset->edSortArm1ZD_Auto3,  fOffset->edSortArm1ZD_Auto4,  fOffset->edSortArm1ZD_Auto5 },
    /*eTrack2     */     {fOffset->edSortArm1ZD_Auto6,  fOffset->edSortArm1ZD_Auto7,  fOffset->edSortArm1ZD_Auto8,  fOffset->edSortArm1ZD_Auto9,  fOffset->edSortArm1ZD_Auto10},
    /*eTrack3     */     {fOffset->edSortArm1ZD_Auto11, fOffset->edSortArm1ZD_Auto12, fOffset->edSortArm1ZD_Auto13, fOffset->edSortArm1ZD_Auto14, fOffset->edSortArm1ZD_Auto15},
    /*eTrack4     */     {fOffset->edSortArm1ZD_Auto16, fOffset->edSortArm1ZD_Auto17, fOffset->edSortArm1ZD_Auto18, fOffset->edSortArm1ZD_Auto19, fOffset->edSortArm1ZD_Auto20}};

    TEdit *OffSet_Auto_PitchX[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fOffset->edSortArm1PitchX_Auto1,  fOffset->edSortArm1PitchX_Auto2,  fOffset->edSortArm1PitchX_Auto3,  fOffset->edSortArm1PitchX_Auto4,  fOffset->edSortArm1PitchX_Auto5 },
    /*eTrack1     */     {fOffset->edSortArm1PitchX_Auto1,  fOffset->edSortArm1PitchX_Auto2,  fOffset->edSortArm1PitchX_Auto3,  fOffset->edSortArm1PitchX_Auto4,  fOffset->edSortArm1PitchX_Auto5 },
    /*eTrack2     */     {fOffset->edSortArm1PitchX_Auto6,  fOffset->edSortArm1PitchX_Auto7,  fOffset->edSortArm1PitchX_Auto8,  fOffset->edSortArm1PitchX_Auto9,  fOffset->edSortArm1PitchX_Auto10},
    /*eTrack3     */     {fOffset->edSortArm1PitchX_Auto11, fOffset->edSortArm1PitchX_Auto12, fOffset->edSortArm1PitchX_Auto13, fOffset->edSortArm1PitchX_Auto14, fOffset->edSortArm1PitchX_Auto15},
    /*eTrack4     */     {fOffset->edSortArm1PitchX_Auto16, fOffset->edSortArm1PitchX_Auto17, fOffset->edSortArm1PitchX_Auto18, fOffset->edSortArm1PitchX_Auto19, fOffset->edSortArm1PitchX_Auto20}};

    TEdit *OffSet_Auto_PitchY1[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fOffset->edSortArm1PitchY1_Auto1,  fOffset->edSortArm1PitchY1_Auto2,  fOffset->edSortArm1PitchY1_Auto3,  fOffset->edSortArm1PitchY1_Auto4,  fOffset->edSortArm1PitchY1_Auto5 },
    /*eTrack1     */     {fOffset->edSortArm1PitchY1_Auto1,  fOffset->edSortArm1PitchY1_Auto2,  fOffset->edSortArm1PitchY1_Auto3,  fOffset->edSortArm1PitchY1_Auto4,  fOffset->edSortArm1PitchY1_Auto5 },
    /*eTrack2     */     {fOffset->edSortArm1PitchY1_Auto6,  fOffset->edSortArm1PitchY1_Auto7,  fOffset->edSortArm1PitchY1_Auto8,  fOffset->edSortArm1PitchY1_Auto9,  fOffset->edSortArm1PitchY1_Auto10},
    /*eTrack3     */     {fOffset->edSortArm1PitchY1_Auto11, fOffset->edSortArm1PitchY1_Auto12, fOffset->edSortArm1PitchY1_Auto13, fOffset->edSortArm1PitchY1_Auto14, fOffset->edSortArm1PitchY1_Auto15},
    /*eTrack4     */     {fOffset->edSortArm1PitchY1_Auto16, fOffset->edSortArm1PitchY1_Auto17, fOffset->edSortArm1PitchY1_Auto18, fOffset->edSortArm1PitchY1_Auto19, fOffset->edSortArm1PitchY1_Auto20}};

    TEdit *OffSet_Auto_PitchY2[eTrackCount][eTrackColTotal]=
    /*eTrackNotUse*/    {{fOffset->edSortArm1PitchY2_Auto1,  fOffset->edSortArm1PitchY2_Auto2,  fOffset->edSortArm1PitchY2_Auto3,  fOffset->edSortArm1PitchY2_Auto4,  fOffset->edSortArm1PitchY2_Auto5 },
    /*eTrack1     */     {fOffset->edSortArm1PitchY2_Auto1,  fOffset->edSortArm1PitchY2_Auto2,  fOffset->edSortArm1PitchY2_Auto3,  fOffset->edSortArm1PitchY2_Auto4,  fOffset->edSortArm1PitchY2_Auto5 },
    /*eTrack2     */     {fOffset->edSortArm1PitchY2_Auto6,  fOffset->edSortArm1PitchY2_Auto7,  fOffset->edSortArm1PitchY2_Auto8,  fOffset->edSortArm1PitchY2_Auto9,  fOffset->edSortArm1PitchY2_Auto10},
    /*eTrack3     */     {fOffset->edSortArm1PitchY2_Auto11, fOffset->edSortArm1PitchY2_Auto12, fOffset->edSortArm1PitchY2_Auto13, fOffset->edSortArm1PitchY2_Auto14, fOffset->edSortArm1PitchY2_Auto15},
    /*eTrack4     */     {fOffset->edSortArm1PitchY2_Auto16, fOffset->edSortArm1PitchY2_Auto17, fOffset->edSortArm1PitchY2_Auto18, fOffset->edSortArm1PitchY2_Auto19, fOffset->edSortArm1PitchY2_Auto20}};

    iPosZ_Offset[0][0]=atoi(fTeach->edSortArm1ZA_Offset->Text.c_str());
    iPosZ_Offset[0][1]=atoi(fTeach->edSortArm1ZB_Offset->Text.c_str());
    iPosZ_Offset[1][0]=atoi(fTeach->edSortArm1ZC_Offset->Text.c_str());
    iPosZ_Offset[1][1]=0;
    iPosZ_Offset[2][0]=atoi(fTeach->edSortArm1ZE_Offset->Text.c_str());
    iPosZ_Offset[2][1]=atoi(fTeach->edSortArm1ZF_Offset->Text.c_str());
    iPosZ_Offset[3][0]=atoi(fTeach->edSortArm1ZG_Offset->Text.c_str());
    iPosZ_Offset[3][1]=atoi(fTeach->edSortArm1ZH_Offset->Text.c_str());

    iOffsetPosZ_Offset[0][0]=atoi(fOffset->edSortArm1ZA_Offset->Text.c_str());
    iOffsetPosZ_Offset[0][1]=atoi(fOffset->edSortArm1ZB_Offset->Text.c_str());
    iOffsetPosZ_Offset[1][0]=atoi(fOffset->edSortArm1ZC_Offset->Text.c_str());
    iOffsetPosZ_Offset[1][1]=0;                                                    //KenHsieh 20240514 : 刪除基準吸嘴Ofs
    iOffsetPosZ_Offset[2][0]=atoi(fOffset->edSortArm1ZE_Offset->Text.c_str());
    iOffsetPosZ_Offset[2][1]=atoi(fOffset->edSortArm1ZF_Offset->Text.c_str());
    iOffsetPosZ_Offset[3][0]=atoi(fOffset->edSortArm1ZG_Offset->Text.c_str());
    iOffsetPosZ_Offset[3][1]=atoi(fOffset->edSortArm1ZH_Offset->Text.c_str());

    iOffsetAutoZ[0][0]=atoi(fOffset->edSortArm1ZA_Auto->Text.c_str());
    iOffsetAutoZ[0][1]=atoi(fOffset->edSortArm1ZB_Auto->Text.c_str());
    iOffsetAutoZ[1][0]=atoi(fOffset->edSortArm1ZC_Auto->Text.c_str());
    iOffsetAutoZ[1][1]=0;
    iOffsetAutoZ[2][0]=atoi(fOffset->edSortArm1ZE_Auto->Text.c_str());
    iOffsetAutoZ[2][1]=atoi(fOffset->edSortArm1ZF_Auto->Text.c_str());
    iOffsetAutoZ[3][0]=atoi(fOffset->edSortArm1ZG_Auto->Text.c_str());
    iOffsetAutoZ[3][1]=atoi(fOffset->edSortArm1ZH_Auto->Text.c_str());

    SortArmPara->iPos_Loader_X = atoi(fTeach->edSortArm1X_Loader->Text.c_str())+atoi(fOffset->edSortArm1X_Loader->Text.c_str());
    SortArmPara->iPos_Loader_Y = atoi(fTeach->edSortArm1Y_Loader->Text.c_str())+atoi(fOffset->edSortArm1Y_Loader->Text.c_str());

    for(int iC=0; iC<SortArmPara->SortArmSuck->MaxItemC; iC++)
    {
        for(int iR=0; iR<SortArmPara->SortArmSuck->MaxItemR; iR++)
        {
            SortArmPara->iPos_Loader_Z[iR][iC]=atoi(fTeach->edSortArm1Z_Loader->Text.c_str())+
                                               atoi(fOffset->edSortArm1Z_Loader->Text.c_str())+   //KenHsieh 20240514 : 新增整體Z Ofs
                                               iPosZ_Offset[iR][iC]+
                                               iOffsetPosZ_Offset[iR][iC];

            SortArmPara->SortArmSuck->Suck[iR][iC].OnDelayTime =atof(fSetup->edAutoPickDelay->Text.c_str())*1000.0;
            SortArmPara->SortArmSuck->Suck[iR][iC].OffDelayTime=atof(fSetup->edAutoPlaceDelay->Text.c_str())*1000.0;
        }
    }

    for(int iR=eTrack1; iR<=eTrack4; iR++)
    {
        for(int iC=0; iC<eTrackColTotal; iC++)
        {
            SortArmPara->iPos_Unload_X[iR][iC]=atoi(Auto_X[iR][iC]->Text.c_str())+atoi(OffSet_Auto_X[iR][iC]->Text.c_str());
            SortArmPara->iPos_Unload_Y[iR][iC]=atoi(Auto_Y[iR][iC]->Text.c_str())+atoi(OffSet_Auto_Y[iR][iC]->Text.c_str());

            SortArmPara->iULPitchOffset[0][iR][iC]=atoi(OffSet_Auto_PitchX[iR][iC]->Text.c_str());
            SortArmPara->iULPitchOffset[1][iR][iC]=atoi(OffSet_Auto_PitchY1[iR][iC]->Text.c_str());
            SortArmPara->iULPitchOffset[2][iR][iC]=atoi(OffSet_Auto_PitchY2[iR][iC]->Text.c_str());

            for(int iPickR=0; iPickR<SortArmPara->SortArmSuck->MaxItemR; iPickR++)
            {
                for(int iPickC=0; iPickC<SortArmPara->SortArmSuck->MaxItemC; iPickC++)
                {
                    SortArmPara->iPos_Unload_Z[iR][iC][iPickR][iPickC]=atoi(Auto_Z[iR][iC]->Text.c_str())+
                                                                       atoi(OffSet_Auto_Z[iR][iC]->Text.c_str())+
                                                                       iPosZ_Offset[iPickR][iPickC]+
                                                                       iOffsetAutoZ[iPickR][iPickC];
                }
            }
        }
    }

    SortArmPara->iPos_Y120Pitch1 = atoi(fTeach->edSortArm1PitchY1_P_120mm->Text.c_str()) ;
    SortArmPara->iPos_Y40Pitch1  = atoi(fTeach->edSortArm1PitchY1_P_40mm->Text.c_str())  ;
    SortArmPara->iPos_Y120Pitch2 = atoi(fTeach->edSortArm1PitchY2_P_120mm->Text.c_str()) ;
    SortArmPara->iPos_Y40Pitch2  = atoi(fTeach->edSortArm1PitchY2_P_40mm->Text.c_str())  ;
    SortArmPara->iPos_X60Pitch   = atoi(fTeach->edSortArm1PitchX_P_60mm->Text.c_str())   ;
    SortArmPara->iPos_X15Pitch   = atoi(fTeach->edSortArm1PitchX_P_15mm->Text.c_str())   ;

    SortArmPara->iTrayXDivision  = atoi(fSetup->edXDivision->Text.c_str()) ;
    SortArmPara->iTrayYDivision  = atoi(fSetup->edYDivision->Text.c_str()) ;

    SortArmPara->iTrayXPitch     = atof(fSetup->edXPitch->Text.c_str())*100.0;
    SortArmPara->iTrayYPitch     = atof(fSetup->edYPitch->Text.c_str())*100.0;
    SortArmPara->iTrayXStart      =atof(fSetup->edXStart->Text.c_str())*100.0;
    SortArmPara->iTrayYStart      =atof(fSetup->edYStart->Text.c_str())*100.0;

    SortArmPara->iSafePosX       = atoi(fTeach->edSortArm1X_Auto2->Text.c_str()) ;
    SortArmPara->iSafePosY       = atoi(fTeach->edSortArm1Y_Auto2->Text.c_str()) ;

    SortArmPara->iColPitch=SortArmPara->AutoCalculateSortArmClosePitchCol(SortArmPara->iStepCol);
    SortArmPara->iRowPitch=SortArmPara->AutoCalculateSortArmClosePitchRow(SortArmPara->iStepRow);
    //Sam 20240607 : Suck 預吸吹功能
    //==>
    SortArmPara->bPreSuckOn =fSetup->cbAutoPreSuckOn->Checked;
    SortArmPara->iPreSuckOn =atof(fSetup->edAutoPreSuckOn->Text.c_str())*100.0;
    SortArmPara->bPreSuckOff=fSetup->cbAutoPreSuckOff->Checked;
    SortArmPara->iPreSuckOff=atof(fSetup->edAutoPreSuckOff->Text.c_str())*100.0;
    SortArmPara->bPreZDown=fSetup->cbAutoPreZDown->Checked;
    SortArmPara->iPreZDown=atof(fSetup->edAutoPreZDown->Text.c_str())*100.0;
    //<==
    //Sam 20240607 : Suck 預吸吹功能
    SortArmPara->dDestroyCheckTime=atof(fSetup->edtDestroyCheckTime->Text.c_str());         //Sam20240627 : 修正回黏問題
    SortArmPara->iMaxSpaceOnePlace=((SortArmPara->iStepRow*3)+1)*SortArmPara->MotWorkLoader->Tray.XItem;      //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞
}
//---------------------------------------------------------------------------
__fastcall MySortArmParameter::MySortArmParameter()
{
    iBaseSuckX=1;
    iBaseSuckY=1;
    iActSuckR=1;
    iActSuckC=1;
    iStepCol=0;
    iStepRow=0;
    iColPitch=0;
    iRowPitch=0;
    iMaxSpaceOnePlace=0;                                                        //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞
    mBinToTray.ConvertBinToAuto(0);
    iSearchAuto=eBinNotUse;
    ZeroMemory(iTrayIC, sizeof(iTrayIC));
    for(int iR=0; iR<MAX_ARM_Row; iR++)
    {
        for(int iC=0; iC<MAX_ARM_Col; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            bSortArm1UseSuck[iR][iC]=true;
        }
    }
}
//---------------------------------------------------------------------------
void MySortArmParameter::InitialFlag()
{
//    bXMoveOK=false;
//    bYMoveOK=false;
//    bZMoveOK=false;
//    bZPick_OK[0][0]=false;
//    bZPick_OK[0][1]=false;
//    bZPick_OK[0][2]=false;
//    bZPick_OK[0][3]=false;
//    bZPick_OK[1][0]=false;
//    bZPick_OK[1][1]=false;
//    bZPick_OK[1][2]=false;
//    bZPick_OK[1][3]=false;
}
//---------------------------------------------------------------------------
void MySortArmParameter::InitialSuckFlag()
{
    bZNeedSuck[0][0]=false;
    bZNeedSuck[0][1]=false;
    bZNeedSuck[0][2]=false;
    bZNeedSuck[0][3]=false;
    bZNeedSuck[1][0]=false;
    bZNeedSuck[1][1]=false;
    bZNeedSuck[1][2]=false;
    bZNeedSuck[1][3]=false;
}
//---------------------------------------------------------------------------
void MySortArmParameter::InitSortArmXYMoveTask()
{
    iSortArmXYMoveTask=1;
}
//---------------------------------------------------------------------------
int MySortArmParameter::SortArmXYMove(int iXComPos, int iYComPos, bool bPick)
{
    static bool bOutArmFlag[3]={false, false, false};
    int &iTask=iSortArmXYMoveTask;
    static int iX, iY;

    if((SortArmPara->SnAutoTrayDetect[0]->Enable && SortArmPara->SnAutoTrayDetect[0]->IsOn()==false) ||
       (SortArmPara->SnAutoTrayDetect[1]->Enable && SortArmPara->SnAutoTrayDetect[1]->IsOn()==false) ||
       (SortArmPara->SnAutoTrayDetect[2]->Enable && SortArmPara->SnAutoTrayDetect[2]->IsOn()==false) ||
       (SortArmPara->SnAutoTrayDetect[3]->Enable && SortArmPara->SnAutoTrayDetect[3]->IsOn()==false))
    {
        for(int iTrace=0;iTrace<4;iTrace++)                                     //Jimmychiu 20240721 : Auto tray floating message show trace name
        {
            if(SortArmPara->SnAutoTrayDetect[iTrace]->Enable && SortArmPara->SnAutoTrayDetect[iTrace]->IsOn()==false)
            {
                ShowMyMessage(AnsiString().sprintf("確認Auto區 %d 是否有Tray盤翹曲",iTrace+1));
            }
        }
    }
    else
    {
        switch(iTask)
        {
            case 1:
                bOutArmFlag[0]=false;
                bOutArmFlag[1]=false;
                bOutArmFlag[2]=false;
                iTask=100;
            case 100:
                iX=MSortArmX->ReadEncoderPos();
                iY=MSortArmY->ReadEncoderPos();
                if(iYComPos<=iSafePosY || iY<=iSafePosY)
                {
                    {
                        if(bPick)
                        {
                            iTask=2000;                                         //先走Y再走X
                        }
                        else
                        {
                            iTask=3000;                                         //先走X再走Y
                        }
                    }
                }
                else
                {
                    iTask=1000;                                                 //安全 Y 上的位置都正常跑
                }
                if(iTask!=1000)
                    break;
            case 1000:
                if(OutArmDeviceDropCheck()==0)
                {
                    if(bOutArmFlag[0]==false)
                        bOutArmFlag[0]=MSortArmX->MotorMove(iXComPos);
                    if(bOutArmFlag[1]==false)
                        bOutArmFlag[1]=MSortArmY->MotorMove(iYComPos);

                    if(bOutArmFlag[0] && bOutArmFlag[1])
                    {
                        return PNP_DONE;
                    }
                }
                else
                {
                    bOutArmFlag[0]=false;
                    bOutArmFlag[1]=false;
                    bOutArmFlag[2]=false;
                }
                break;
            case 2000:                                                              //先走Y再走X
                iY=MSortArmY->ReadEncoderPos();
                if(OutArmDeviceDropCheck()==0)
                {
                    if(bOutArmFlag[0]==false)
                        bOutArmFlag[0]=MSortArmY->MotorMove(iYComPos);
                    if((bOutArmFlag[0]==true || iY>=iSafePosY) && bOutArmFlag[1]==false)    //Sam 20240526 : 過了安全點就可以兩軸同動
                        bOutArmFlag[1]=MSortArmX->MotorMove(iXComPos);

                    if(bOutArmFlag[0] && bOutArmFlag[1])
                    {
                        return PNP_DONE;
                    }
                }
                else
                {
                    bOutArmFlag[0]=false;
                    bOutArmFlag[1]=false;
                    bOutArmFlag[2]=false;
                }
                break;
            case 3000:                                                              //先走X再走Y
                if(OutArmDeviceDropCheck()==0)
                {
                    iX=MSortArmX->ReadEncoderPos();                                 //Sam 20240619 : 修正錯誤
                    if(bOutArmFlag[0]==false)
                        bOutArmFlag[0]=MSortArmX->MotorMove(iXComPos);
                    if((bOutArmFlag[0]==true || iX>=iSafePosX) && bOutArmFlag[1]==false)    //Sam 20240526 : 過了安全點就可以兩軸同動
                        bOutArmFlag[1]=MSortArmY->MotorMove(iYComPos);

                    if(bOutArmFlag[0] && bOutArmFlag[1])
                    {
                        return PNP_DONE;
                    }
                }
                else
                {
                    bOutArmFlag[0]=false;
                    bOutArmFlag[1]=false;
                    bOutArmFlag[2]=false;
                }
                break;
        }
    }
    return PNP_DOING;
}
//---------------------------------------------------------------------------
void MySortArmParameter::CheckSortArmDestroyActive()
{
    int iPos[4][2]={0};
    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
        {
            if(HSys.LastSet.iRealDummy==DUMMY ||
               HSys.LastSet.iRealDummy==HAS_TRAY)
                bSortArmCheckDestroyACT[iR][iC]=false;

            if(bSortArmCheckDestroyACT[iR][iC]==false)
            {
                iTask[iR][iC]=1;
                continue;
            }

            switch(iTask[iR][iC])
            {
                case 1:
                    bCheckSortArmDestroyActiveFinish[iR][iC]=false;             //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成

                    iPos[iR][iC]=MSortArmZ[iR][iC]->ReadPos();
                    if(SortArmSuck->Suck[iR][iC].Item==NULL_IC)                 //Steven 20120507 : 要持續吹氣
                        SortArmSuck->Suck[iR][iC].OffDestroy();

                    if(iPos[iR][iC]>=0)
                    {
                        iTask[iR][iC]=200;
                        SortArmSuck->Suck[iR][iC].OffDestroy();
                    }
                    break;
                case 200:
                    SortArmSuck->Suck[iR][iC].OnSuck();
                    CheckSortArmDestroyActiveDelay[iR][iC].SetSecAndOn(dDestroyCheckTime);
                    bCheckSortArmDestroyActive[iR][iC]=true;                    //Steven 20120507 : 要偵測完才可以動，會影響UPH

                    iTask[iR][iC]=300;
                    break;
                case 300:
                    if(CheckSortArmDestroyActiveDelay[iR][iC].Off())
                    {
                        if(SortArmSuck->Suck[iR][iC].GetStatus())
                        {
                            ShowSystemError(SortArmSuck->AlarmName[eDestoryErr], K_RETRY);   //破壞錯誤, 請將IC取下
                            bCheckSortArmDestroyActive[iR][iC]=true;
                            iTask[iR][iC]=200;                                  //Steven 20120507 : 有出錯就繼續吸吸看，看IC有沒有被拿掉。
                        }
                        else
                        {
                            bCheckSortArmDestroyActiveFinish[iR][iC]=true;      //ChungHung 201112296 add Pick 前先確認CheckInArmDestroyActive 已完成
                            SortArmSuck->Suck[iR][iC].Normal();
                            bSortArmCheckDestroyACT[iR][iC]=false;
                            bCheckSortArmDestroyActive[iR][iC]=false;
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
bool MySortArmParameter::SortArmPitchMove(int VariX, int VariY1, int VariY2, bool bInit)
{
    static bool bFirstIn=true;
    static bool bFlag[4]={false, false, false, false};

    if(bInit || bFirstIn)
    {
        ZeroMemory(bFlag, sizeof(bFlag));
        bFirstIn=false;
        return 0;
    }

    if(bFlag[0]==false)
        bFlag[0]=MSortArmPitchX->MotorMove(VariX);
    if(bFlag[1]==false)
        bFlag[1]=MSortArmPitchY1->MotorMove(VariY1);
    if(bFlag[2]==false)
        bFlag[2]=MSortArmPitchY2->MotorMove(VariY2);

    if(bFlag[0] && bFlag[1] && bFlag[2])
    {
        ZeroMemory(bFlag, sizeof(bFlag));
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::SortArmContinuousMove(int iXPos, int iYPos, int VariX, int VariY1, int VariY2, bool ZNeedDown, bool bPick)
{
    AnsiString str;
    int iCheckX=0, iCheckY=0, iEncodeX=0, iEncodeY=0;
    int ret=PNP_DOING;
    static bool SortSafeCheck[4][2]={{false, false}, {false, false}, {false, false}, {false, false}};
    static bool bPitchMoveOk=false;
    static bool bFlag[5]={false, false, false, false, false};
    static bool bFlagZ[4][2]={{false, false}, {false, false}, {false, false}, {false, false}};
    static int iSortArmAllZSafe=1;
    static bool bXYMoveFinish=false;
    static bool bXYPosErrorFlag=false;
    int iZCurPos=0, iXCurPos=0, iYCurPos=0; //Sam 20240607 : Suck 預吸吹功能

    static int iLastPosX=-99999;
    static int iLastPosY=-99999;

    if(iLastPosX!=iXPos || iLastPosY!=iYPos)
    {
        iLastPosX=iXPos;
        iLastPosY=iYPos;
        iSortArmAllZSafe=1;
    }

    if(iXPos>=MSortArmX->GetSoftLimitP() || iXPos<=MSortArmX->GetSoftLimitN())
    {
        ShowSystemError(MSortArmX->AlarmName[eMotOverLimitErr], K_RETRY);       //"In Arm X axis motor will out of limit !"
        return false;
    }
    if(iYPos>=MSortArmY->GetSoftLimitP() || iYPos<=MSortArmY->GetSoftLimitN())
    {
        ShowSystemError(MSortArmY->AlarmName[eMotOverLimitErr], K_RETRY);       //"In Arm Y axis motor will out of limit !"
        return false;
    }

    if(MSortArmX->GetEnable() && MSortArmY->GetEnable())
    {
        if(bPauseSortMotor)
        {
            iSortArmAllZSafe=1;
            bPauseSortMotor=false;
            OutArmIdle.SetSecAndOn(10);
        }

        switch(iSortArmAllZSafe)
        {
            case 1:
                if(MoveSortArmToAutoSafe())
                {
                    OutArmIdle.On();
                    iSortArmAllZSafe=100;
                    bXYMoveFinish=false;
                    bPitchMoveOk=false;
                    SortArmPitchMove(VariX, VariY1, VariY2, true);
                    ZeroMemory(bFlag, sizeof(bFlag));
                    ZeroMemory(bFlagZ, sizeof(bFlagZ));

                    InitSortArmXYMoveTask();                                    //jou 2011-04-03
                    if(bXYPosErrorFlag)
                    {
                        iSortArmAllZSafe=150;
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
                    bPitchMoveOk=SortArmPitchMove(VariX, VariY1, VariY2);

                if(bPitchMoveOk)
                {
                    ZeroMemory(bFlag, sizeof(bFlag));
                    ZeroMemory(bFlagZ, sizeof(bFlagZ));
                    iSortArmAllZSafe=200;
                }
                break;
            case 150:
                if(bFlag[0]==false)
                    bFlag[0]=MSortArmX->MotorMove(iXPos+100);                   //Frank 20150604 發現阿洲抖一下.....
                if(bFlag[1]==false)
                    bFlag[1]=MSortArmY->MotorMove(iYPos+100);

                if(bFlag[0] && bFlag[1])
                {
                    ZeroMemory(bFlag, sizeof(bFlag));
                    bXYPosErrorFlag=false;
                    iSortArmAllZSafe=100;
                }
                break;
            case 200:
                if(bXYMoveFinish)
                {
                    if(ZNeedDown)
                    {
                        for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
                        {
                            for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                            {
                                if(bZNeedSuck[iR][iC])
                                {
                                    if(bFlagZ[iR][iC]==false)
                                    {
                                        //Sam 20240607 : Suck 預吸吹功能
                                        //==>
                                        MSortArmZ[iR][iC]->ReadPos();
                                        if(HSys.Sen.SnRKManualTStart.IsOff()==false || fMain->cdStep->Checked)
                                        {
                                            //使用 T.Start 時不能預吸吹
                                        }
                                        else
                                        {
                                            if(SortArmPara->bPreSuckOn && (abs(iZPos[iR][iC]-iZCurPos)<SortArmPara->iPreSuckOn) && bPick==true)
                                                SortArmSuck->Suck[iR][iC].On();
                                            else if(SortArmPara->bPreSuckOff && (abs(iZPos[iR][iC]-iZCurPos)<SortArmPara->iPreSuckOff) && bPick==false)
                                                SortArmSuck->Suck[iR][iC].Off();
                                        }
                                        //<==
                                        //Sam 20240607 : Suck 預吸吹功能
                                        bFlagZ[iR][iC]=MSortArmZ[iR][iC]->MotorMove(iZPos[iR][iC]);
                                    }
                                }
                                else
                                {
                                    bFlagZ[iR][iC]=true;
                                }
                            }
                        }

                        for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
                        {
                            for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                            {
                                if(bFlagZ[iR][iC]==false)
                                    return false;
                            }
                        }
                    }

                    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                        {
                            if(ZNeedDown && bZNeedSuck[iR][iC] && MSortArmZ[iR][iC]->Led[iHomeLed])
                            {
                                MSortArmX->Stop();
                                MSortArmY->Stop();
                                iSortArmAllZSafe=1;
                                ZeroMemory(bFlag, sizeof(bFlag));
                                ShowMyMessage(MSortArmZ[iR][iC]->NumberAlias+" Home sensor error, if suck is down, maybe sensor fail!");
                                return false;
                            }
                        }
                    }

                    iSortArmAllZSafe=1;
                    bPitchMoveOk=false;
                    InitSortArmXYMoveTask();                                    //jou 2011-04-03
                    ZeroMemory(bFlag, sizeof(bFlag));
                    return true;
                }

                if(bPitchMoveOk==false)
                {
                    ZeroMemory(bFlag, sizeof(bFlag));
                    iSortArmAllZSafe=1;
                }
                break;
            default :
                iSortArmAllZSafe=1;
                return false;
        }

        if(bXYPosErrorFlag)
            return false;

        if(bXYMoveFinish==false &&
           bPitchMoveOk)
        {
            ret=SortArmXYMove(iXPos, iYPos, bPick);
            //Sam 20240607 : Suck 預吸吹功能
            //==>
            if(ZNeedDown && SortArmPara->bPreZDown && ret!=PNP_DONE)
            {
                iXCurPos=MSortArmX->ReadPos();
                iYCurPos=MSortArmY->ReadPos();
                if(abs(iXCurPos-iXPos)<SortArmPara->iPreZDown && abs(iYCurPos-iYPos)<SortArmPara->iPreZDown)
                {
                    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                        {
                            if(bZNeedSuck[iR][iC])
                              MSortArmZ[iR][iC]->MotorMove(iZPos[iR][iC]/2);    //Z偷跑只要先跑一半，不要全下去。
                        }
                    }
                }
            }
            //<==
            //Sam 20240607 : Suck 預吸吹功能
        }

        if(ret==PNP_DONE)
        {
            if(bPitchMoveOk==true)
            {
                iCheckX=MSortArmX->ReadPos();
                iCheckY=MSortArmY->ReadPos();

                if((iXPos!=iCheckX) || (iYPos!=iCheckY))
                {
                    iEncodeX=MSortArmX->ReadEncoderPos();                       //Steven 20110314 : 比對Encoder位置
                    iEncodeY=MSortArmY->ReadEncoderPos();                       //Steven 20110314 : 比對Encoder位置

                    if(iXPos>(iEncodeX+10) || iXPos<(iEncodeX-10) ||
                       iYPos>(iEncodeY+10) || iYPos<(iEncodeY-10))              //Steven 20110314 : 比對Encoder位置
                    {
                        iSortArmAllZSafe=1;
                        bPitchMoveOk=false;
                        bFlag[0]=false;
                        bFlag[1]=false;
                        bFlag[2]=false;
                        MSortArmX->Stop();
                        MSortArmY->Stop();

                        bXYPosErrorFlag=true;
                        return false;
                    }
                }

                bXYMoveFinish=true;

                if(ZNeedDown)
                {
                    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                        {
                            if(bZNeedSuck[iR][iC])
                            {
                                //Sam 20240607 : Suck 預吸吹功能
                                //==>
                                MSortArmZ[iR][iC]->ReadPos();
                                if(HSys.Sen.SnRKManualTStart.IsOff()==false || fMain->cdStep->Checked)
                                {
                                    //使用 T.Start 時不能預吸吹
                                }
                                else
                                {
                                    if(SortArmPara->bPreSuckOn && (abs(iZPos[iR][iC]-iZCurPos)<SortArmPara->iPreSuckOn) && bPick==true)
                                        SortArmSuck->Suck[iR][iC].On();
                                    else if(SortArmPara->bPreSuckOff && (abs(iZPos[iR][iC]-iZCurPos)<SortArmPara->iPreSuckOff) && bPick==false)
                                        SortArmSuck->Suck[iR][iC].Off();
                                }
                                //<==
                                //Sam 20240607 : Suck 預吸吹功能
                                bFlagZ[iR][iC]=MSortArmZ[iR][iC]->MotorMove(iZPos[iR][iC]);
                            }
                        }
                    }
                }

                return false;
            }
        }
    }
    else                                                                        //動作模擬
    {
        for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
            {
                if(SortSafeCheck[iR][iC]==false)
                    SortSafeCheck[iR][iC]=MSortArmZ[iR][iC]->MotorMove(10);
            }
        }

        for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
            {
                if(SortSafeCheck[iR][iC]==false)
                    return false;
            }
        }

        bFlag[0]=MSortArmX->MotorMove(iXPos);
        bFlag[1]=MSortArmY->MotorMove(iYPos);
        bFlag[2]=SortArmPitchMove(VariX, VariY1, VariY2);

        if(bFlag[0] && bFlag[1] && bFlag[2])
        {
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    if(ZNeedDown)
                    {
                        if(bZNeedSuck[iR][iC]==false)
                            iZPos[iR][iC]=10;
                    }
                    else
                    {
                        iZPos[iR][iC]=10;
                    }
                    bFlagZ[iR][iC]=MSortArmZ[iR][iC]->MotorMove(iZPos[iR][iC]);
                }
            }

            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    if(bFlagZ[iR][iC]==false)
                        return false;
                }
            }

            bFlag[0]=false;
            bFlag[1]=false;
            bFlag[2]=false;
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    bFlagZ[iR][iC]=false;
                    SortSafeCheck[iR][iC]=false;
                }
            }
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::DoSortArmPlaceToAuto(int iTray)
{
    static bool bDuplicateErr10000=false;                                       //Steven 20091218 : Avoid duplicate message
    AnsiString ErrorPart="";
    bool bHasErr=false;
    int iTrayPosRow=0, iTrayPosCol=0;
    MyBinToTrayStruct BtoT;
    SortArmSuck->ClearAllError();
    AnsiString sErrMes="";
    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            if(bSortArmSuckActive[iR][iC])
            {
                if(SortArmSuck->Suck[iR][iC].Destroy())
                {
                    BtoT.ConvertAutoToBin(iTray);
                    GetMapTrayPosFromEncorder(iSortArmMapTrayPos[iR][iC], iTrayPosRow, iTrayPosCol);
                    Auto->MAutoTray[iTray]->SetTraySingleData(iTrayPosCol, iTrayPosRow, HAS_IC, SortArmSuck->Suck[iR][iC].DeviceInfo);
                    Auto->MAutoTray[iTray]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].AddOutputInfo(iTray,Auto->MAutoTray[iTray]->Tray.ClipID, iTrayPosCol, iTrayPosRow);


                    if(atoi(Auto->MAutoTray[iTray]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].SoftBin.c_str())!=BtoT.iBin)  //Sam 20250903 : 避免上游機台 SoftBin 資料缺失
                    {
                        Auto->MAutoTray[iTray]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].SoftBin=AnsiString(BtoT.iBin);
                    }
                    if(Auto->MAutoTray[iTray]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].iBin!=BtoT.iBin)                   //Sam 20250903 : 增加分Bin異常保護
                    {
                        sErrMes.sprintf("Place to %s is bin%d", fMain->sTrayNameBin[iTray], Auto->MAutoTray[iTray]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].iBin);
                        ShowSystemError("80006", K_SKIP, 0, sErrMes);
                    }

                    Auto->MAutoTray[iTray]->Tray.DeviceInfo[iTrayPosCol][iTrayPosRow].SaveRecord();
                    SortArmSuck->SetItemData(iR, iC, NULL_IC);
                    SortArmSuck->Suck[iR][iC].DeviceInfo.Clear();

                    bSortArmSuckActive[iR][iC]=false;
                    bSortArmCheckDestroyACT[iR][iC]=true;                       //jou 981130 確認device確實destroy完成
                    tRunData.BinICCnt[BtoT.iBin]++;
                    tRunData.TrayICCnt[iTray]++;
                    tRunData.TotalIC++;
                }
                if(SortArmSuck->Suck[iR][iC].Error)
                    bHasErr=true;
            }
        }
    }

    if(bHasErr)
    {
        ErrorPart=SortArmSuck->GetErrorPart(Vaccum_Off);
        ShowSuckError(*SortArmSuck, Vaccum_Off, K_RETRY, ErrorPart, bDuplicateErr10000);

        bDuplicateErr10000=true;
    }

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            if(bSortArmSuckActive[iR][iC])
                return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::FindHasAutoSortBin()
{
    Find_SortBinCount_Auto(&iTrayIC[0]);
    for(int i=eAuto1; i<=eAuto20; i++)
    {
        if(iTrayIC[i]>0)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void MySortArmParameter::DoSortArm1(int &Task)
{
    AnsiString asString="";
    CheckSortArmDestroyActive();
    if(CheckAllAutoTrayDetectSensor()==false)                                   //JerryYang 20240718 : real time 檢查AUTO TRAY
    {
        return;
    }
    switch(Task)
    {
        case 1:
            if(SortArmPara->SortArmSuck->HasIC())                               //JerryYang 20240817 : ARM上已經有IC就先放料, 不要重新search
            {
                DoPlaceToAuto(true);
                Task=4000;
            }
            else if(HSys.Sys.RunMode==Run_OneCycle &&
               SortArmSuck->HasRealIC()==false)                                 //Steven 20240707 : Fixed for one cycle
            {
                bSortArm1OneCycleFinish=true;
            }
            else if(MotWorkLoader->fHasTray==false &&
                    MotLoader_Car->fHasTray==true)                              //Tray還沒進來, 先預判
            {
                mBinToTray.ConvertAutoToBin(1);
                ZeroMemory(iTrayIC, sizeof(iTrayIC));
                bNeedLoadTrayToTransfer=false;
                Task=50;
            }
            else if(MotWorkLoader->fHasTray)
            {
                if(MotWorkLoader->HasIC())
                {
                    mBinToTray.ConvertAutoToBin(1);
                    ZeroMemory(iTrayIC, sizeof(iTrayIC));
                    bNeedLoadTrayToTransfer=false;
                    Task=100;
                }
                else
                {
                    Task=5000;
                }
            }
            else
            {
                if(bFlagLoaderCleanoutFinish==true)
                    bFlagAutoArmCleanoutFinish=true;
                else
                    bFlagAutoArmCleanoutFinish=false;
            }
            break;
        case 50:
            if(MotLoader_Car->HasIC())
            {
                Find_SortBinCount_Auto(&iTrayIC[0], true);
                for(int iT=eAuto1; iT<=eAuto20;  iT++)
                {
                    if(iTrayIC[iT]>0)
                    {
                        mBinToTray.ConvertAutoToBin(iT);
                        fMain->lblLoadCurrBin_2->Caption=AnsiString("Bin : ")+AnsiString(mBinToTray.iBin);
                        fMain->lblLoadCurrBin_1->Caption=AnsiString("Current Sorting Bin : ")+AnsiString(mBinToTray.iBin);
                        Task=60;
                        return;
                    }
                }

                mBinToTray.ConvertAutoToBin(eBinNotUse);
                Task=60;
            }
            else if(MotWorkLoader->fHasTray && MotLoader_Car->HasIC()==false)   //JerryYang 20240611 : fix hang up
            {
                Task=1;
            }
            break;
        case 60:
            if(MotWorkLoader->fHasTray)
            {
                Task=100;
            }
            else if(MotWorkLoader->fHasTray==false &&
                    MotLoader_Car->fHasTray==false)                             //JerryYang 20240615 : fix clean out hang up
            {
                Task=1;
            }
            break;
        case 100:
            if(SortArmPara->SortArmSuck->HasIC())                               //JerryYang 20240817 : ARM上已經有IC就先放料, 不要重新search
            {
                DoPlaceToAuto(true);
                Task=4000;
            }
            else if(HSys.Sys.RunMode==Run_OneCycle &&
               SortArmSuck->HasRealIC()==false)                                 //Steven 20240707 : Fixed for one cycle
            {
                Task=1;
            }
            else if(LoaderModule->LoadAreaWorkReady() &&
                    MotWorkLoader->fHasTray)
            {
                if(MotWorkLoader->HasIC())
                {
                    Find_SortBinCount_Auto(&iTrayIC[0]);
                    Task=1000;
                }
                else
                {
                    Task=5000;
                }
            }
            else if(HSys.Sys.RunMode==Run_CleanOut)
            {
                Task=1;
            }
            break;
        case 1000:
            if(HSys.Sys.RunMode==Run_OneCycle &&
               SortArmSuck->HasRealIC()==false)                                 //Steven 20240707 : Fixed for one cycle
            {
                Task=1;
            }
            else if(iTrayIC[mBinToTray.iWhichAuto]>0)
            {
                if(Auto->MAutoTray[mBinToTray.iWhichAuto]->fHasTray)            //有Tray就放
                {
                    fMain->lblLoadCurrBin_2->Caption=AnsiString("Bin : ")+AnsiString(mBinToTray.iBin);
                    fMain->lblLoadCurrBin_1->Caption=AnsiString("Current Sorting Bin : ")+AnsiString(mBinToTray.iBin);
                    Find_SortBin_Auto();
                    Task=2000;
                }
                else                                                            //沒Tray就先跳下一個
                {
                    fMain->lblLoadCurrBin_2->Caption=AnsiString("Bin : ");
                    fMain->lblLoadCurrBin_1->Caption=AnsiString("Current Sorting Bin : ");
                    Task=2500;
                }
            }
            else
            {
                Task=2500;                                                      //換Tray
            }
            break;
        case 2000:
            Task=2500;
            if(HSys.Sys.RunMode==Run_OneCycle &&
               SortArmSuck->HasRealIC()==false)                                 //Steven 20240707 : Fixed for one cycle
            {
                Task=1;
            }
            else if(MotNowSortTray->HasIC())
            {
                MotNowSortTray->SetTrayVisible(true);
                if(Auto->MAutoTray[mBinToTray.iWhichAuto]->fHasTray)
                {
                    if(CheckHasSpaceToPlace(iUseSuckNo))
                    {
                        DoPickFromLoader(true);
                        Task=3000;
                    }
                }
                else
                {
                    Task=2000;
                }
            }
            else
            {
                MotNowSortTray->ClearTray();
            }
            break;
        case 2500:
            iSearchAuto=GetNextAutoArea(iSearchAuto);
            mBinToTray.ConvertAutoToBin(iSearchAuto);
            if(HSys.Sys.RunMode==Run_OneCycle &&
               SortArmSuck->HasRealIC()==false)                                 //Steven 20240707 : Fixed for one cycle
            {
                Task=1;
            }
            else if(mBinToTray.iWhichAuto>eAuto20)
            {
                fMain->lblLoadCurrBin_2->Caption=AnsiString("Bin : ");
                fMain->lblLoadCurrBin_1->Caption=AnsiString("Current Sorting Bin : ");
                Task=5000;                                                      //換Tray
            }
            else if(iTrayIC[mBinToTray.iWhichAuto]>0)
            {
                if(Auto->MAutoTray[mBinToTray.iWhichAuto]->fHasTray)            //有Tray就放
                {
                    fMain->lblLoadCurrBin_2->Caption=AnsiString("Bin : ")+AnsiString(mBinToTray.iBin);
                    fMain->lblLoadCurrBin_1->Caption=AnsiString("Current Sorting Bin : ")+AnsiString(mBinToTray.iBin);
                    Task=1000;
                }
                else                                                            //沒Tray就先跳下一個
                {
                    fMain->lblLoadCurrBin_2->Caption=AnsiString("Bin : ");
                    fMain->lblLoadCurrBin_1->Caption=AnsiString("Current Sorting Bin : ");
                    Task=2500;
                }
            }
            else if(LoaderModule->LoadAreaWorkReady() &&
                    MotWorkLoader->fHasTray &&
                    MotWorkLoader->HasIC()==false)
            {
                 Task=5000;
            }
            else if(LoaderModule->LoadAreaWorkReady() &&
                    MotWorkLoader->fHasTray &&
                    MotWorkLoader->HasIC() &&
                    FindHasAutoSortBin()==false)
            {
                Task=5000;
            }
            break;
        case 3000:
            if(DoPickFromLoader())
            {
                if(CheckZNeedPlace(iUseSuckNo))
                {
                    DoPlaceToAuto(true);
                    Task=4000;
                }
                else
                {
                    Task=2000;
                }
            }
            break;
        case 4000:
            if(DoPlaceToAuto())
            {
                if(HSys.Sys.RunMode==Run_OneCycle &&
                   SortArmSuck->HasRealIC()==false)                             //Steven 20240707 : Fixed for one cycle
                    Task=1;
                else
                    Task=100;
            }
            break;
        case 5000:
            if(MotWorkLoader->fHasTray && MotWorkLoader->HasIC() && FindHasAutoSortBin())
            {
                Task=1;
            }
            else
            {
                bNeedLoadTrayToTransfer=true;
                Task=6000;
            }
            break;
        case 6000:
            if(HSys.Sys.RunMode==Run_OneCycle &&                                //JerryYang 20240802 : modify
               SortArmSuck->HasRealIC()==false)                                 //Steven 20240707 : Fixed for one cycle
            {
                bSortArm1OneCycleFinish=true;
            }
            if(bNeedLoadTrayToTransfer==false)
            {
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
bool MySortArmParameter::CheckHasSpaceToPlace(int &iCount)
{
    iCount=0;
    int iTray=mBinToTray.iWhichAuto;
    if(iTray==eBinNotUse)
        return false;

    if(Auto->MAutoTray[iTray]->fHasTray)
    {
        for(int i=0; i<iTrayXDivision; i++)
        {
            for(int j=0; j<iTrayYDivision; j++)
            {
                if(Auto->MAutoTray[iTray]->Tray.Data[i][j]==NULL_IC)
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
int MySortArmParameter::Find_SortBinCount_Auto(int *iTrayIC, bool bPreLoad)
{
    int iBin=0;
    int iTrayICCount[eTrayCount];
    int iTray=eBinNotUse;
    ZeroMemory(iTrayICCount, sizeof(iTrayICCount));

    if(bPreLoad)
    {
        if(MotLoader_Car->fHasTray &&
           MotLoader_Car->Tray.ClipID!="")
        {
            for(int i=0; i<iTrayXDivision; i++)
            {
                for(int j=0; j<iTrayYDivision; j++)
                {
                    if(MotLoader_Car->Tray.Data[i][j]!=NULL_IC)
                    {
                        iBin=MotLoader_Car->Tray.DeviceInfo[i][j].iBin;
                        if(iBin>0 && iBin<TEST_MAX_BIN)                         //進去else就是有bug
                        {
                            if(CosFunction.bUseBinAutoConvert)
                            {
                                iTray=cBinAutoTrans.GetAutobyBin(iBin);
                            }
                            else
                            {
                                iTray=BinSelect[0].iCategData[iBin];
                            }
                            if(iTray!=eBinNotUse)                               //進去else就是有bug
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
                if(MotWorkLoader->Tray.Data[i][j]!=NULL_IC)
                {
                    iBin=MotWorkLoader->Tray.DeviceInfo[i][j].iBin;
                    if(iBin>0 && iBin<TEST_MAX_BIN)
                    {
                        if(CosFunction.bUseBinAutoConvert)
                        {
                            iTray=cBinAutoTrans.GetAutobyBin(iBin);
                        }
                        else
                        {
                            iTray=BinSelect[0].iCategData[iBin];
                        }
                        if(iTray!=eBinNotUse)                                   //進去else就是有bug
                            iTrayICCount[iTray]++;
                    }
                    else                                                        //進去else就是有bug
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
void MySortArmParameter::Find_SortBin_Auto()
{
    MotNowSortTray->InitNewTray(NULL_IC);
    int iTray=mBinToTray.iWhichAuto;
    int iBin=0;
    int iICMapTray=0;

    if(iTray==0)
        return;                                                                 //進去就是有bug

    for(int i=0; i<iTrayXDivision; i++)
    {
        for(int j=0; j<iTrayYDivision; j++)
        {
            if(MotWorkLoader->Tray.Data[i][j]==HAS_IC)                          //KenHsieh 20240514 : 有HAS_IC才進來
            {
                iBin=MotWorkLoader->Tray.DeviceInfo[i][j].iBin;
                if(CosFunction.bUseBinAutoConvert)
                {
                    iICMapTray=cBinAutoTrans.GetAutobyBin(iBin);
                }
                else
                {
                    iICMapTray=BinSelect[0].iCategData[iBin];
                }
                if(iBin>0 && iBin<TEST_MAX_BIN &&
                   iICMapTray==iTray)
                {
                    MotNowSortTray->SetTraySingleData(i, j, HAS_IC, MotWorkLoader->Tray.DeviceInfo[i][j]);
                }
                else
                {
                    MotNowSortTray->SetTraySingleData(i, j, NULL_IC);
                }
            }
            else
            {
                iBin=0;
            }
        }
    }
}
//------------------------------------------------------------------------------
int MySortArmParameter::Find_SortArm_PlaceMaxUseCountOnTime(TTrayMotor *Motor, int iStepRow, int iStepCol, bool bPick)
{
    int trayIndexRow=0, trayIndexCol=0;
    int iCT=0, itStartX=-1, itStartY=-1;
    int ixStart=0, ixEnd=0, ixStep=0;
    int iyStart=0, iyEnd=0, iyStep=0;
    int iCheckICStatus=bPick?HAS_IC:NULL_IC;
    int Direction=0;
    int iOneByOne=0;
    bool bCanPlace=false;

    if(Direction>=4)
        Direction=0;

    ixStart=0;
    ixEnd=Motor->Tray.XItem;
    ixStep=1;
    iyStart=0;
    iyEnd=Motor->Tray.YItem;
    iyStep=1;

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            bSortArmSuckActive[iR][iC]=false;
            iSortArmMapTrayPos[iR][iC]=0;
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

    if(bPick==false)                                                            //放的時候要one by one
    {
        if(myXML.iTotalBinCount[mBinToTray.iBin]>0)
        {
            int iLeftCount=myXML.iTotalBinCount[mBinToTray.iBin]-tRunData.TrayICCnt[mBinToTray.iWhichAuto];
            if(iLeftCount<iPos_Loader_X)                                        //只剩下一排的數量就one by one
            {
                iOneByOne=1;
            }
            else if(iLeftCount>=iPos_Loader_X &&                                //只剩最後七列, 可以一次兩顆 (同一個Col)
                    iLeftCount<SortArmPara->iMaxSpaceOnePlace)
            {
                iOneByOne=2;
            }
            else
            {
                iOneByOne=0;
            }
        }
        else
        {
            iOneByOne=0;
        }
    }

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        bCanPlace=false;
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            if(SortArmSuck->Suck[iR][iC].Item==iCheckICStatus)
            {
                iCT++;
                continue;
            }
            else if(fSetup->rgUseSuck->ItemIndex==1 && bSortArm1UseSuck[iR][iC]==false)
            {
                continue;
            }
            else if(mBinToTray.iWhichAuto==1 && bSortArm1UseSuck[iR][iC]==false)//Ian 20241112 : 修正auto1放不滿問題
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

                if(trayIndexRow>=0 && trayIndexCol>=0 &&
                   trayIndexCol<Motor->Tray.XItem &&
                   trayIndexRow<Motor->Tray.YItem &&
                   Motor->Tray.Data[trayIndexCol][trayIndexRow]==iCheckICStatus)
                {
                    iCT++;
                    bSortArmSuckActive[iR][iC]=true;
                    iSortArmMapTrayPos[iR][iC]=GetMapTrayPosFromRowCol(trayIndexRow, trayIndexCol);

                    if(bPick==true)
                    {
                        if(iCT==iUseSuckNo)              //Steven 20240708 : 只吸取跟Tray剩餘IC一樣多的位置
                        {
                            return iCT;
                        }
                    }
                    else
                    {
                        if(iOneByOne==1)                                        //放的時候要one by one
                        {
                            return iCT;
                        }
                        bCanPlace=true;
                    }
                }
            }
        }
        if(bPick==false && iOneByOne==2 && bCanPlace==true)
        {
            return iCT;
        }
    }
    return iCT;
}
//------------------------------------------------------------------------------
int MySortArmParameter::Find_SortArm_SearchShortedPosForPick(TTrayMotor *Motor, int iStepRow, int iStepCol)
{
    int trayIndexRow=0, trayIndexCol=0;
    int iCT=0, itStartX=-1, itStartY=-1;
    int ixStart=0, ixEnd=0, ixStep=0;
    int iyStart=0, iyEnd=0, iyStep=0;
    int iCheckICStatus=HAS_IC;
    int iUnsortCount=0;
    int Direction=0;

    if(Direction>=4)
        Direction=0;

    ixStart=0;
    ixEnd=Motor->Tray.XItem;
    ixStep=1;
    iyStart=0;
    iyEnd=Motor->Tray.YItem;
    iyStep=1;

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            bSortArmSuckActive[iR][iC]=false;
            iSortArmMapTrayPos[iR][iC]=0;
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

    if(mBinToTray.iWhichAuto==1)                                            //Ian 20241112 : 當auto1最後一半則使用g,h吸嘴
    {
        int iEmptyCNT=Auto->MAutoTray[eAuto1]->Tray.iEmptyCount;
        int iLoaderCNT=Auto->MAutoTray[eAuto1]->Tray.iCurrCount;
        if(iLoaderCNT>iEmptyCNT)
        {
            bSortArm1UseSuck[0][0]=false;
            bSortArm1UseSuck[0][1]=false;
            bSortArm1UseSuck[1][0]=false;
            bSortArm1UseSuck[1][1]=false;
            bSortArm1UseSuck[2][0]=false;
            bSortArm1UseSuck[2][1]=false;
        }
    }
    else
    {
            bSortArm1UseSuck[0][0]=true;
            bSortArm1UseSuck[0][1]=true;
            bSortArm1UseSuck[1][0]=true;
            bSortArm1UseSuck[1][1]=true;
            bSortArm1UseSuck[2][0]=true;
            bSortArm1UseSuck[2][1]=true;
    }

    iCT=0;
    iActSuckR=-1;
    iActSuckC=-1;
    iUnsortCount=iByLotTrayCnt[mBinToTray.iWhichAuto]-tRunData.TrayICCnt[mBinToTray.iWhichAuto];
    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            if(SortArmSuck->Suck[iR][iC].Item==iCheckICStatus)
            {
                continue;
            }
            else if(fSetup->rgUseSuck->ItemIndex==1 && bSortArm1UseSuck[iR][iC]==false)
            {
                continue;
            }
            else if(mBinToTray.iWhichAuto==1 && bSortArm1UseSuck[iR][iC]==false)//Ian 20241112 : 修正auto1放不滿問題
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

                if(trayIndexRow>=0 && trayIndexCol>=0 &&
                   trayIndexCol<Motor->Tray.XItem &&
                   trayIndexRow<Motor->Tray.YItem &&
                   Motor->Tray.Data[trayIndexCol][trayIndexRow]==iCheckICStatus)
                {
                    iCT++;
                    bSortArmSuckActive[iR][iC]=true;
                    iSortArmMapTrayPos[iR][iC]=GetMapTrayPosFromRowCol(trayIndexRow, trayIndexCol);

                    if((bOneByOnePick && iCT==iUseSuckNo) ||                    //JerryYang 20240630 : 計算By lot數量避免Unloader tray有空洞
                        (iByLotTrayCnt[mBinToTray.iWhichAuto]>0 &&
                        (iUnsortCount<SortArmPara->iMaxSpaceOnePlace)))         //只吸取跟Tray剩餘IC一樣多的位置
                        return iCT;
                }
            }
        }
    }
    return iCT;
}
//------------------------------------------------------------------------------
int MySortArmParameter::AutoCalculateSortArmClosePitch()
{
    int y=0, i=0;
    do
    {
        i++;
        y=iTrayYPitch*i;
        if(y>=IN_OUT_ARM_Y_PITCH_MIN)
            break;
    }while(y<IN_OUT_ARM_Y_PITCH_MAX && iTrayYPitch!=0);
    if(y>=IN_OUT_ARM_Y_PITCH_MIN && y<=IN_OUT_ARM_Y_PITCH_MAX)
    {
        return y;
    }
    return -1;
}
//------------------------------------------------------------------------------
int MySortArmParameter::AutoCalculateSortArmClosePitchCol(int &iStep)
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
int MySortArmParameter::AutoCalculateSortArmClosePitchRow(int &iStep)
{
    int y=0, i=0;
    do
    {
        i++;
        y=iTrayYPitch*i;
        if(y>=IN_OUT_ARM_Y_PITCH_MIN &&
           ((i%3)==0))
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
//------------------------------------------------------------------------------
// 取得判斷入料變 pitch 距離
//==============================================================================
int MySortArmParameter::GetSortArmPitchY1(int w)
{
    double m,r;
    m= ((double)(iPos_Y120Pitch1-iPos_Y40Pitch1))/6000.0;                       //Steven 20131002 : XY變距
    r=(double)iPos_Y40Pitch1+m*((double)w-6000.0);
    return (int)r;
}
//==============================================================================
int MySortArmParameter::GetSortArmPitchY2(int w)
{
    double m,r;
    m= ((double)(iPos_Y120Pitch2-iPos_Y40Pitch2))/6000.0;                       //Steven 20131002 : XY變距
    r=(double)iPos_Y40Pitch2+m*((double)w-6000.0);
    return (int)r;
}
//==============================================================================
int MySortArmParameter::GetSortArmPitchX(int w)
{
    double m,r;
    m= ((double)(iPos_X60Pitch-iPos_X15Pitch))/4500.0;                          //Steven 20131002 : XY變距
    r=(double)iPos_X15Pitch+m*((double)w-1500.0);
    return (int)r;
}
//==============================================================================
bool MySortArmParameter::DoMoveSortArmXYToPick(int iRowPitch, int iColPitch, bool ReadMove)
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
    iXPos=iPos_Loader_X-1000+iTrayXStart;
    iYPos=iPos_Loader_Y+1000-iTrayYStart;                                       //基準軸在右側

    //to tray cell(iy,ix)
    GetMapTrayPosFromEncorder(iSortArmMapTrayPos[iActSuckR][iActSuckC], iTrayPosRow, iTrayPosCol);
    iXPos+=(iTrayPosCol*iTrayXPitch);
    iYPos-=(iTrayPosRow*iTrayYPitch);

    //suck xy to cell(iy,ix)
    iXPos-=(iActSuckC-iBaseSuckX)*iColPitch;
    iYPos+=(iActSuckR-iBaseSuckY)*iRowPitch/3;

    iAutoX=iTrayPosCol;
    iAutoY=iTrayPosRow;

    iXpitch =iColPitch+atoi(fOffset->edSortArm1PitchX_P_60mm_Pick->Text.c_str());
    iYPitch1=iRowPitch+atoi(fOffset->edSortArm1PitchY1_P_120mm_Pick->Text.c_str());
    iYPitch2=iRowPitch+atoi(fOffset->edSortArm1PitchY2_P_120mm_Pick->Text.c_str());

    iVari[0]=GetSortArmPitchX(iXpitch);
    iVari[1]=GetSortArmPitchY1(iYPitch1);
    iVari[2]=GetSortArmPitchY2(iYPitch2);

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            if(bSortArmSuckActive[iR][iC])
            {
                bZNeedSuck[iR][iC]=true;
                iZPos[iR][iC]     =iPos_Loader_Z[iR][iC];
            }
            else
            {
                bZNeedSuck[iR][iC]=false;
                iZPos[iR][iC]     =10;
            }
        }
    }

    if(IsCheckSortArmDestroyActiveFinish(iXPos, iYPos)==false)                  //Sam 20240627 : 回吸偵測做完才能去吸 IC
        return false;

    if(SortArmContinuousMove(iXPos, iYPos, iVari[0], iVari[1], iVari[2], true, true))
    {
        return true;
    }
    return false;
}
//==============================================================================
bool MySortArmParameter::DoMoveSortArmXYToPlace(int iRowPitch, int iColPitch, bool ReadMove)
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
    iXPos=iPos_Unload_X[mBinToTray.iAutoRow][mBinToTray.iAutoCol]-1000+iTrayXStart;
    iYPos=iPos_Unload_Y[mBinToTray.iAutoRow][mBinToTray.iAutoCol]+1000-iTrayYStart;                            //基準軸在右側

    //to tray cell(iy,ix)
    GetMapTrayPosFromEncorder(iSortArmMapTrayPos[iActSuckR][iActSuckC], iTrayPosRow, iTrayPosCol);
    iXPos+=(iTrayPosCol*iTrayXPitch);
    iYPos-=(iTrayPosRow*iTrayYPitch);

    //suck xy to cell(iy,ix)
    iXPos-=(iActSuckC-iBaseSuckX)*iColPitch;
    iYPos+=(iActSuckR-iBaseSuckY)*iRowPitch/3;

    iAutoX=iTrayPosCol;
    iAutoY=iTrayPosRow;

    iXpitch =iColPitch+iULPitchOffset[0][mBinToTray.iAutoRow][mBinToTray.iAutoCol];
    iYPitch1=iRowPitch+iULPitchOffset[1][mBinToTray.iAutoRow][mBinToTray.iAutoCol];
    iYPitch2=iRowPitch+iULPitchOffset[2][mBinToTray.iAutoRow][mBinToTray.iAutoCol];

    iVari[0]=GetSortArmPitchX(iXpitch);
    iVari[1]=GetSortArmPitchY1(iYPitch1);
    iVari[2]=GetSortArmPitchY2(iYPitch2);

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            if(bSortArmSuckActive[iR][iC])
            {
                bZNeedSuck[iR][iC]=true;
                iZPos[iR][iC]     =iPos_Unload_Z[mBinToTray.iAutoRow][mBinToTray.iAutoCol][iR][iC];
            }
            else
            {
                bZNeedSuck[iR][iC]=false;
                iZPos[iR][iC]     =10;
            }
        }
    }

    if(SortArmContinuousMove(iXPos, iYPos, iVari[0], iVari[1], iVari[2], ReadMove, false))
    {
        return true;
    }
    return false;
}
//==============================================================================
bool MySortArmParameter::MoveSortArmXYToPick()
{
    if(HSys.FuncS.bS02_SearchShortestMode)
    {
        Find_SortArm_SearchShortedPosForPick(MotNowSortTray, iStepRow, iStepCol);
    }
    else
    {
        Find_SortArm_PlaceMaxUseCountOnTime(MotNowSortTray, iStepRow, iStepCol,true);
    }

    if(iActSuckR==-1 || iActSuckC==-1)
    {
        return false;
    }
    return DoMoveSortArmXYToPick(iRowPitch, iColPitch, true);
}
//------------------------------------------------------------------------------
bool MySortArmParameter::MoveSortArmXYToPlace()
{
    Find_SortArm_PlaceMaxUseCountOnTime(Auto->MAutoTray[mBinToTray.iWhichAuto], iStepRow, iStepCol, false);
    return  DoMoveSortArmXYToPlace(iRowPitch, iColPitch, true);
}
//------------------------------------------------------------------------------
bool MySortArmParameter::DoPlaceToAuto(bool bReset)
{
    bool bResult=false;
    if(bReset)
    {
        iPlaceToAutoTask=1;
        InitialFlag();
        return false;
    }

    int &Task=iPlaceToAutoTask;
    switch(Task)
    {
        case 1:
            if(CheckSortArmSameBin()==false)                                    //JerryYang 20240817 : 檢查ARM上的BIN必須相同
            {
                if(ManualTakeOutSortArmDevice()==false)
                {
                    break;
                }
                else
                {
                    Task=50;
                    break;
                }
            }
            else if(CheckSortArmBin(mBinToTray.iBin)==false)
            {
                if(ManualTakeOutSortArmDevice()==false)
                {
                    break;
                }
                else
                {
                    Task=50;
                    break;
                }
            }

            if(CheckHasSpaceToPlace(iUseSuckNo))
            {
                Task=100;
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
            if(mBinToTray.iWhichAuto==eBinNotUse)
            {
                Task=1;                                                         //進去就是有bug
                ShowMyMessage("Plase Auto iWhichAuto error");
                break;
            }

            if(Auto->MAutoTray[mBinToTray.iWhichAuto]->fHasTray==false ||       //沒tray
               Auto->MAutoTray[mBinToTray.iWhichAuto]->FullIC()        ||       //有Tray沒IC的時候
               bCheckAutoTrayDetectSensor(mBinToTray.iWhichAuto))               //判斷Auto HasTray Sensor
            {
                Task=100;                                                       //要等待換Tray
                break;
            }
            else if(MoveSortArmXYToPlace())
            {
                if(HSys.Sen.SnRKManualTStart.IsOff()==false || fMain->cdStep->Checked)
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
            if(Auto->MAutoTray[mBinToTray.iWhichAuto]->fHasTray==false)         //JerryYang 20240603 : add
            {
                ShowMyMessage("Auto Area data is no tray!");
                Task=1;
                break;
            }
            if(DoSortArmPlaceToAuto(mBinToTray.iWhichAuto))
            {
                Task=300;
            }
            break;
        case 300:
            if(MoveSortArmToAutoSafe())
            {
                if(CheckZNeedPlace(8))
                    Task=100;
                else
                    Task=50;
            }
            break;
        case 500:
            if(MoveSortArmToAutoSafe())
            {
                Task=100;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::DoPickFromLoader(bool bReset)
{
    bool bResult=false;
    int iRet;
    AnsiString ErrorPart;
    static int iRetryCT=0, iWhitchSuckR=0, iWhitchSuckC=0;
    int iTrayPosRow=0, iTrayPosCol=0;
    if(bReset)
    {
        iPickFromLoaderTask=1;
        InitialFlag();
        iRetryCT=0;
        iWhitchSuckR=0;
        iWhitchSuckC=0;
        return false;
    }

    int &Task=iPickFromLoaderTask;

    switch(Task)
    {
        case 1:
            if(CheckHasSpaceToPlace(iUseSuckNo))
            {
                SortArmSuckReset();                                             //JerryYang 20240623 : pick up error可以SKIP
                Task=100;
            }
            else
            {
                Task=400;
            }
            break;
        case 100:
            if(MotNowSortTray->HasIC() && CheckZNeedSuck(iUseSuckNo))
            {
                if(bOneByOnePick)                                               //只吸取跟Tray剩餘IC一樣多的位置
                {
                    iRet=0;

                    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                        {
                            if(SortArmSuck->Suck[iR][iC].Item!=NULL_IC)
                            {
                                iRet++;
                            }
                        }
                    }

                    if(iRet==iUseSuckNo)
                    {
                        Task=400;
                    }
                    else
                    {
                        iRetryCT=0;
                        Task=200;
                    }
                }
                else
                {
                    iRetryCT=0;
                    Task=200;
                }
            }
            else
            {
                Task=400;
            }
            break;
        case 200:
            if(MoveSortArmXYToPick())
            {
                if(iUPH_PickCnt==0)
                {
                    CalculateUPH();
                }
                iUPH_PickCnt++;
                if(HSys.Sen.SnRKManualTStart.IsOff()==false || fMain->cdStep->Checked)
                {
                    fMain->sbPause->Click();
                    Task=500;
                }
                else
                {
                    SortArmSuck->ResetAll();                                    //JerryYang 20240623 : pick up error可以SKIP
                    Task=300;
                }
            }
            break;
        case 300:
            flag=true;
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                   //Sam 20240705 : 修改搜尋方向
                {
                    if(bSortArmSuckActive[iR][iC] && SortArmSuck->Suck[iR][iC].Item==NULL_IC)
                    {
                        if(bSuckEnd[iR][iC]==false &&
                           SortArmSuck->Suck[iR][iC].Suck())                    //JerryYang 20240623 : pick up error可以SKIP
                        {
                            GetMapTrayPosFromEncorder(iSortArmMapTrayPos[iR][iC], iTrayPosRow, iTrayPosCol);
                            AddLoadingCount(iR, iC, iTrayPosCol, iTrayPosRow);
                            iInputIC_Loader[mBinToTray.iWhichAuto]--;
                            bSortArmSuckActive[iR][iC]=false;
                        }
                        else if(bSuckEnd[iR][iC]==false &&
                                SortArmSuck->Suck[iR][iC].Error==false)
                        {
                            flag=false;
                        }
                        else if(SortArmSuck->Suck[iR][iC].Error==true)
                        {
                            bSuckEnd[iR][iC]=true;
                        }
                    }
                    else
                    {
                        bSuckEnd[iR][iC]=true;                                  //JerryYang 20240623 : pick up error可以SKIP
                    }
                }
            }

            if(flag==false)
                break;

            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                   //Sam 20240705 : 修改搜尋方向
                {
                    if(SortArmSuck->Suck[iR][iC].Error==true)
                    {
                        iRetryCT++;

                        if(iRetryCT>3)                                          //Steven 20141113 : Loader SKIP時在原地Retry
                        {
                            iWhitchSuckC=iC;
                            iWhitchSuckR=iR;
                            iRetryCT=0;
                            Task=350;
                        }
                        else
                        {
                            Task=500;
                        }
                        return false;
                    }
                }
            }

            if(flag)
            {
                iRetryCT=0;
                Task=500;
            }
            break;
        case 350:
            if(MoveSortArmToAutoSafe())
            {
                ErrorPart=SortArmSuck->GetErrorPart(Vaccum_On);
                int KCode=0;
                if(CUSTOMER_CODE==CC_WINSTEK &&
                   HSys.AccessLevel<3)
                {
                    KCode=K_RETRY|K_TRAY_END|K_HOME;
                }
                else
                {
                    KCode=K_RETRY|K_SKIP|K_TRAY_END|K_HOME;
                }
                iRet=ShowSuckError(HSys.Suck.AutoArmSuck, Vaccum_On, KCode, ErrorPart, 0);
                if(iRet==K_RETRY)
                {
                    Task=500;
                }
                else if(iRet==K_HOME)                                           //JerryYang 20240704 : Pick up error Y軸一起回HOME
                {
                    Task=500;
                    bSortArmNeedHome=true;
                    bSortArmYNeedHome=true;
                    fHome->SortArm1ZHome(true);
                }
                else if(iRet==K_SKIP)
                {
                    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
                    {
                        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)           //Sam 20240705 : 修改搜尋方向
                        {
                            if(SortArmSuck->Suck[iR][iC].Error==true)
                            {
                                GetMapTrayPosFromEncorder(iSortArmMapTrayPos[iR][iC], iTrayPosRow, iTrayPosCol);
                                MotNowSortTray->SetTraySingleData(iTrayPosCol, iTrayPosRow, NULL_IC);  //JerryYang 20240623 : pick up error可以SKIP
                                MotWorkLoader->SetTraySingleData(iTrayPosCol, iTrayPosRow, NULL_IC);
                            }
                        }
                    }
                    Task=400;
                }
                else if(iRet==K_TRAY_END)
                {
                    if(HSys.FuncT.iA01_InputDataFormat==LoaderModule->eEKeeperFormat)
                    {
                        RecordTrayCount();
                    }
                    MotWorkLoader->InitNewTray(NULL_IC);
                    MotNowSortTray->InitNewTray(NULL_IC);
                    ZeroMemory(iTrayIC, sizeof(iTrayIC));
                    Task=400;
                }
            }
            break;
        case 400:
            iRetryCT=0;
            bResult=true;
            Task=1;
            break;
        case 500:
            if(MoveSortArmToAutoSafe())
            {
                SortArmSuckReset();                                             //JerryYang 20240623 : pick up error可以SKIP
                SortArmSuck->ClearAllError();
                if(iRetryCT>0)
                    Task=200;
                else
                    Task=1;
            }
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
void MySortArmParameter::RecordTrayCount()
{
    for(int i=0; i<iTrayXDivision; i++)
    {
        for(int j=0; j<iTrayYDivision; j++)
        {
            if(MotWorkLoader->Tray.Data[i][j]==HAS_IC)
            {
                iHasSkipTrayDataNeedHold++;
            }
        }
    }
}
//------------------------------------------------------------------------------
bool MySortArmParameter::CheckZNeedSuck(int iUseSuckNo)
{
    int ct=0, iHasICCnt=0;
    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
        {
            if(SortArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                iHasICCnt++;
            }
        }
    }
    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
        {
            if(ct<(iUseSuckNo-iHasICCnt))
            {
                if(fSetup->rgUseSuck->ItemIndex==1 &&
                   bSortArm1UseSuck[iR][iC]==false)                             //JerryYang 20240706 : fix 關SITE
                {
                    continue;
                }
                else if(mBinToTray.iWhichAuto==1 && bSortArm1UseSuck[iR][iC]==false)//Ian 20241112 : 修正auto1放不滿問題
                {
                    continue;
                }
                else if(SortArmSuck->Suck[iR][iC].Item==NULL_IC)
                {
                    ct++;
                    return true;
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
bool MySortArmParameter::CheckZNeedPlace(int iUseSuckNo)
{
    int ct=0;
    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
        {
            if(ct<iUseSuckNo)
            {
                if(SortArmSuck->Suck[iR][iC].Item==HAS_IC)
                {
                    ct++;
                    return true;
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
bool MySortArmParameter::MoveSortArmToAutoSafe()
{
    bool bResult=false;
    bool OK=true, bNeedHome=false;
    static iRetryCount=0;
    #ifndef SOFT_SIMULATE
    bool bHome[4][2]={{false, false}, {false, false},
                      {false, false}, {false, false}};
    #endif

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                           //Sam 20240705 : 修改搜尋方向
        {
            if(MSortArmZ[iR][iC]->MotorMove(10)==false)
            {
                OK=false;
            }
        }
    }

    if(OK)
    {
        bNeedHome=false;
        for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                       //Sam 20240705 : 修改搜尋方向
            {
                MSortArmZ[iR][iC]->ScanMotorStatus();
                #ifndef SOFT_SIMULATE
                bHome[iR][iC]=MSortArmZ[iR][iC]->Led[iHomeLed];
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
                bSortArmNeedHome=true;
                fHome->SortArm1ZHome(true);
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
int MySortArmParameter::OutArmDeviceDropCheck()                                 //0:OK ,1:In 異常 ,2:Out  異常  //Steven 20130808 :掉料檢查
{
    AnsiString ErrorPart="", asErr="";
    bool bOutHasErr=false;
    int iSuckRow=0,iSuckCol=0;
    if(HSys.LastSet.iRealDummy==REALLY)
        bOutHasErr=SortArmSuck->CheckHasError(Vaccum_FallDown,iSuckCol,iSuckRow);
    else
        return 0;

    if(bOutHasErr)                                                              //KenHsieh 20240515 : 解Mark 測試
    {
        HSys.DecStopAllMotor();
        ErrorPart=SortArmSuck->GetErrorPart(Vaccum_FallDown);
        ShowSuckError(*SortArmSuck, Vaccum_FallDown, K_SKIP, ErrorPart);
        SortArmSuck->SkipError(Vaccum_FallDown, NULL_IC);
        int iTrayR=SortArmSuck->Suck[iSuckRow][iSuckCol].DeviceInfo.iInputR;
        int iTrayC=SortArmSuck->Suck[iSuckRow][iSuckCol].DeviceInfo.iInputC;
        asErr.sprintf("AutoSortArmSuck-%s Drop IC, clean data,iTrayR=%d,iTrayC=%d", ErrorPart,iTrayR,iTrayC);     //KenHsieh 20240515 : fix 中途掉料不轉換資料
        RecordProcess(asErr);                                                   //KenHsieh 20240515 : fix 中途掉料不轉換資料

        for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
        {
           for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)                        //Sam 20240705 : 修改搜尋方向
            {
                if(SortArmSuck->Suck[iR][iC].GetStatus()==false)
                {
                    SortArmSuck->Suck[iR][iC].Normal();
                }
            }
        }

        return 2;
    }

    return bOutHasErr;
}
//------------------------------------------------------------------------------
void MySortArmParameter::AddLoadingCount(int iSuckR, int iSuckC, int iAutoX, int iAutoY)     //Steven 20161123 : 將Loader吸取的數量計算統一成Function
{
    iUPH_LoaderCount++;                                                         //Sam 20240526 : 新增每盤的UPH
    tRunData.LoaderIC++;                                                        //JerryYang 20240616 : add loading count
    SortArmSuck->MoveSingleItem(MotWorkLoader, iSuckR, iSuckC, iAutoX, iAutoY);
    SortArmSuck->Suck[iSuckR][iSuckC].DeviceInfo.AddInputInfo(MotWorkLoader->Tray.ClipID, iAutoX, iAutoY, 1, iSuckR, iSuckC);
    MotNowSortTray->SetTraySingleData(iAutoX, iAutoY, NULL_IC);
    bSuckEnd[iSuckR][iSuckC]=true;                                              //JerryYang 20240623 : pick up error可以SKIP
}
//------------------------------------------------------------------------------
void MySortArmParameter::GetMapTrayPosFromEncorder(int idata, int &iRow, int &iCol)
{
    iRow=idata/100;
    iCol=idata%100;
}
//------------------------------------------------------------------------------
int MySortArmParameter::GetMapTrayPosFromRowCol(int iRow, int iCol)
{
    return iRow*100+iCol;
}
//------------------------------------------------------------------------------
// 計算最近10次的UPH與平均數
//------------------------------------------------------------------------------
void __fastcall MySortArmParameter::CalculateUPH()                              //Sam 20240526 : 新增每盤的UPH
{
    int iCount=0, iTotalUPH=0;
    double fConsumeSecond,fTimerMultiple;
    TDateTime tTempTime, tConsumeSecond;
    Word Hour, Min, Sec, MSec;
    static bool bOneTimes=true;
    tTempTime=Now();
    AnsiString sAvgUPH="0";
    if(bOneTimes)
    {
        bOneTimes=false;
        RunInfo.iUPH=0;
        tUPH_StartTime=tTempTime;
        tUPH_PauseTime=0;
        iUPH_LoaderCount=0;
        fMain->UPH_StringGrid->Cells[0][0]="Start Time";
        fMain->UPH_StringGrid->Cells[1][0]="End Time";
        fMain->UPH_StringGrid->Cells[2][0]="Pause Time";
        fMain->UPH_StringGrid->Cells[3][0]="UPH";
        for(int i=1; i<=12; i++)
        {
            fMain->UPH_StringGrid->Cells[0][i]="";
            fMain->UPH_StringGrid->Cells[1][i]="";
            fMain->UPH_StringGrid->Cells[2][i]="";
            fMain->UPH_StringGrid->Cells[3][i]="";
        }
    }
    else
    {
        if(tTempTime>tUPH_StartTime)
        {
            for(int i=10; i>=1; i--)
            {
                fMain->UPH_StringGrid->Cells[0][i]=fMain->UPH_StringGrid->Cells[0][i-1];
                fMain->UPH_StringGrid->Cells[1][i]=fMain->UPH_StringGrid->Cells[1][i-1];
                fMain->UPH_StringGrid->Cells[2][i]=fMain->UPH_StringGrid->Cells[2][i-1];
                fMain->UPH_StringGrid->Cells[3][i]=fMain->UPH_StringGrid->Cells[3][i-1];
            }

            tUPH_EndTime=tTempTime;
            tConsumeSecond=(tUPH_EndTime-tUPH_StartTime);
            fMain->UPH_StringGrid->Cells[0][1]=tUPH_StartTime.FormatString("hh:nn:ss");
            fMain->UPH_StringGrid->Cells[1][1]=tUPH_EndTime.FormatString("hh:nn:ss");
            fMain->UPH_StringGrid->Cells[2][1]=tUPH_PauseTime.FormatString("hh:nn:ss");
            tUPH_StartTime=tUPH_EndTime;
            tConsumeSecond=tConsumeSecond-tUPH_PauseTime;
            tUPH_PauseTime=0;
            fConsumeSecond=0;
            DecodeTime(tConsumeSecond, Hour, Min, Sec, MSec);
            fConsumeSecond+=Hour*3600.0;
            fConsumeSecond+=Min*60.0;
            fConsumeSecond+=Sec;
            fConsumeSecond+=MSec/(double)1000.0;
            if(fConsumeSecond>0)
                fTimerMultiple=3600/fConsumeSecond;
            else
                fTimerMultiple=0;

            RunInfo.iUPH=fTimerMultiple*iUPH_LoaderCount;
            iUPH_LoaderCount=0;
            fMain->UPH_StringGrid->Cells[3][1]=RunInfo.iUPH;  //Steven 20090714

            iTotalUPH=0;
            iCount=0;

            for(int i=1; i<=10; i++)
            {
#ifdef DEBUG_TRY_CATCH
                try
                {
#endif
                    if(fMain->UPH_StringGrid->Cells[3][i]!="")
                    {
                        iTotalUPH+=atoi(AnsiString(fMain->UPH_StringGrid->Cells[3][i]).c_str());
                        iCount++;
                    }
#ifdef DEBUG_TRY_CATCH
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "CalculateUPH");
                }
#endif
            }
            fMain->UPH_StringGrid->Cells[2][12]="Avg UPH :";
            sAvgUPH=AnsiString((iCount==0)?0:iTotalUPH/iCount);
            fMain->UPH_StringGrid->Cells[3][12]=sAvgUPH;
        }
        else
        {
            RunInfo.iUPH=0;
            tUPH_StartTime=tTempTime;
            fMain->UPH_StringGrid->Cells[0][0]="Start Time";
            fMain->UPH_StringGrid->Cells[1][0]="End Time";
            fMain->UPH_StringGrid->Cells[2][0]="Pause Time";
            fMain->UPH_StringGrid->Cells[3][0]="UPH";
            for(int i=1; i<=12; i++)
            {
                fMain->UPH_StringGrid->Cells[0][i]="";
                fMain->UPH_StringGrid->Cells[1][i]="";
                fMain->UPH_StringGrid->Cells[2][i]="";
                fMain->UPH_StringGrid->Cells[3][i]="";
            }
        }
    }
}
//------------------------------------------------------------------------------
void MySortArmParameter::SortArmSuckReset()                                     //JerryYang 20240623 : pick up error可以SKIP
{
    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
        {
            bSuckEnd[iR][iC]=false;
        }
    }
}
//------------------------------------------------------------------------------
bool MySortArmParameter::IsCheckSortArmDestroyActiveFinish(int iX, int iY)
{                                                                                       
    int iPosX=0, iPosY=0;
    int iPreRange=100;
    iPosX=MSortArmX->ReadPos();
    iPosY=MSortArmY->ReadPos();
    if(iPosX>=iX-iPreRange && iPosX<=iX+iPreRange &&
       iPosY>=iY-iPreRange && iPosY<=iY+iPreRange)
    {
        for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
        {
            for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
            {
               if(bCheckSortArmDestroyActiveFinish[iR][iC]==false)
                  return false;
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::bCheckAutoTrayDetectSensor(int iWhichAuto)
{
    MyBinToTrayStruct mAutoHasTray;
    bool bResult=false;

    mAutoHasTray.ConvertAutoToBin(iWhichAuto);
    if(HSys.LastSet.iRealDummy==DUMMY)
    {
        bResult=false;
    }
    else if(LoaderModule->Sen_Auto_HasTray[mAutoHasTray.iAutoRow][mAutoHasTray.iAutoCol]->IsOff()==false)    //Sam 20240727 : 修正模擬
    {
        bResult=false;
    }
    else
    {
        ShowSystemError(LoaderModule->Sen_Auto_HasTray[mAutoHasTray.iAutoRow][mAutoHasTray.iAutoCol]->OffAlarmCode, K_RETRY);
        bResult=true;
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::CheckAllAutoTrayDetectSensor()                         //JerryYang 20240718 : real time 檢查AUTO TRAY
{
    if(HSys.LastSet.iRealDummy==DUMMY)
    {
        return true;
    }
    #ifndef SOFT_SIMULATE
    MyBinToTrayStruct mAutoHasTray;
    for(int i=eAuto1; i<=eAuto20; i++)
    {
        mAutoHasTray.ConvertAutoToBin(i);

        if(mAutoHasTray.iAutoRow>=eTrack1)
        {
            if(Auto->MAutoTray[mAutoHasTray.iWhichAuto]->fHasTray==true && LoaderModule->bAutoUnloadingTray[mAutoHasTray.iAutoRow][mAutoHasTray.iAutoCol]==false)
            {
                if(LoaderModule->Sen_Auto_HasTray[mAutoHasTray.iAutoRow][mAutoHasTray.iAutoCol]->IsOn()==false)
                {
                    ShowSystemError(LoaderModule->Sen_Auto_HasTray[mAutoHasTray.iAutoRow][mAutoHasTray.iAutoCol]->OffAlarmCode, K_RETRY);
                    return false;
                }
            }
        }
    }
    #endif
    return true;
}
//------------------------------------------------------------------------------
bool __fastcall MySortArmParameter::CheckSortArmSameBin()                       //JerryYang 20240817 : 檢查ARM上的BIN必須相同
{
    int iBin=-1;
    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
        {
            if(SortArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                if(iBin==-1)
                {
                    iBin=SortArmPara->SortArmSuck->Suck[iR][iC].DeviceInfo.iBin;
                }
                else
                {
                    if(iBin!=SortArmPara->SortArmSuck->Suck[iR][iC].DeviceInfo.iBin)  //Sort arm同時有兩個bin
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
bool __fastcall MySortArmParameter::ManualTakeOutSortArmDevice(bool bHome)      //JerryYang 20240817 : 檢查ARM上的BIN必須相同
{
    bool bAlarm=false;
    AnsiString asErr="";

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
        {
            if(SortArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                if(SortArmSuck->Suck[iR][iC].GetStatus())
                {
                    bAlarm=true;
                }
                else
                {
                    SortArmSuck->SetItemData(iR, iC, NULL_IC);
                    SortArmSuck->Suck[iR][iC].DeviceInfo.Clear();
                    asErr.sprintf("SortArmSuck[%d,%d] Manual take out IC, clean data", iR, iC);         //KenHsieh 20240515 : fix 中途掉料不轉換資料
                    RecordProcess(asErr);                                                   //KenHsieh 20240515 : fix 中途掉料不轉換資料
                }
            }
        }
    }

    if(bAlarm)
    {
        if(bHome==true)
        {
            ShowMyMessage("Please take out all device on Sort Arm1!!");
        }
        else
        {
            ShowMyMessage("Bin data error, please take out all device on Sort Arm1!!");
        }
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool __fastcall MySortArmParameter::CheckSortArmBin(int iBin)                   //JerryYang 20240817 : 增加保護確保iBin相同
{

    for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
    {
        for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
        {
            if(SortArmSuck->Suck[iR][iC].Item==HAS_IC)
            {
                if(iBin!=SortArmPara->SortArmSuck->Suck[iR][iC].DeviceInfo.iBin)  //Sort arm同時有兩個bin
                {
                    return false;
                }
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::RecTrayICStatus(TTrayMotor *myMotor,int irow,int icol,bool &bHasMidEmpty,uPoint2D &pTargetPos,uPoint2D &pSourcePos) //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    if(bHasMidEmpty==false && myMotor->Tray.Data[icol][irow]==HAS_NULL_IC)
    {
        pTargetPos.X=icol;
        pTargetPos.Y=irow;
        bHasMidEmpty=true;
    }
    else if(bHasMidEmpty==true && myMotor->Tray.Data[icol][irow]==HAS_IC)
    {
        pSourcePos.X=icol;
        pSourcePos.Y=irow;
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::HasGapsInTheTray(int iWhichAuto,uPoint2D &pTargetPos,uPoint2D &pSourcePos) //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    bool bHasMidEmpty=false;
    TTrayMotor *myMotor=Auto->MAutoTray[iWhichAuto];
    if(myMotor->fHasTray==false || myMotor->HasIC()==false)
        return false;
    for(int iR=0; iR<myMotor->Tray.YItem; iR++)
    {
        for(int iC=0; iC<myMotor->Tray.XItem; iC++)
        {
            if(RecTrayICStatus(myMotor,iR,iC,bHasMidEmpty,pTargetPos,pSourcePos))
            {
                return true;
            }
        }
    }
    pTargetPos.Clear();
    pSourcePos.Clear();
    return false;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::DetermineFeasibilityOfSuck(ArmAndSuckInfo &ssp,uPoint2D pArmSuckActive)                       //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    ssp.iArmXV1=iColPitch;
    ssp.iArmYV1=iRowPitch;
    ssp.iArmYV2=iRowPitch;
    uPoint2D pStartPos;
    GetWhichAutoStart(ssp.iWhichAuto,pStartPos);
    //to tray cell(0,0)
    ssp.pArmXYPos.X=pStartPos.X;
    ssp.pArmXYPos.Y=pStartPos.Y;
    //to tray cell(iy,ix)
    ssp.pArmXYPos.X+=(ssp.pTrayRowCol.X*iTrayXPitch);
    ssp.pArmXYPos.Y-=(ssp.pTrayRowCol.Y*iTrayYPitch);
    //suck xy to cell(iy,ix)
    ssp.pArmXYPos.X-=(pArmSuckActive.X-iBaseSuckX)*iColPitch;
    ssp.pArmXYPos.Y+=(pArmSuckActive.Y-iBaseSuckY)*(iRowPitch/3);
    if(IsOutArmPosOutLimit(ssp.pArmXYPos)==true)
    {
        return false;
    }
    else
    {
        return true;
    }
}
//------------------------------------------------------------------------------
void MySortArmParameter::GetWhichAutoStart(int iWhichAuto,uPoint2D &pStartPos)                      //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    pStartPos.X=iPos_Unload_X[mBinToTray.iAutoRow][mBinToTray.iAutoCol]-1000+iTrayXStart;
    pStartPos.Y=iPos_Unload_Y[mBinToTray.iAutoRow][mBinToTray.iAutoCol]+1000-iTrayYStart;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::DoInArmAutoCalSuckZ(bool bReset)
{
    bool bResult=false;
    int &Task=iDoSortArmAutoCalSuckZTask;    
    int iInArmSearchStartZ=-500;
    TEdit *edZ[MAX_SUCKER_ROW][MAX_SUCKER_COL]=
    {
        {fTeach->edSortArm1ZA_Offset, fTeach->edSortArm1ZB_Offset},
        {fTeach->edSortArm1ZC_Offset, fTeach->edSortArm1ZD_Offset},
        {fTeach->edSortArm1ZE_Offset, fTeach->edSortArm1ZF_Offset},
        {fTeach->edSortArm1ZG_Offset, fTeach->edSortArm1ZH_Offset}
    };
    bool bflag=false;
    if(bReset)
    {
        Task=1;
        return true;
    }
    switch(Task)
    {
        case 1:   //Initial
            DoSuckZAllHome(true);
            Task=50;
            break;
        case 50:
            if(DoSuckZAllHome(false))
            {
                Task=100;
            }
            break;
        case 100:   //home
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    iInArmZVacuumbufferPos[iR][iC]=iInArmSearchStartZ;
                    iInArmZHeightDiff[iR][iC]=0;
                    SortArmSuck->Suck[iR][iC].Off();
                }
            }
            Task=500;
            break;
        case 500:
            bflag=true;
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    SortArmSuck->Suck[iR][iC].On();
                    if(MSortArmZ[iR][iC]->MotorMove(iInArmZVacuumbufferPos[iR][iC])==false)
                    {
                        bflag=false;
                    }
                }
            }
            if(bflag)
            {
                SortArmDelay.SetSecAndOn(0.3);
                Task=600;
            }
            break;
        case 600:    //check suck status
            if(SortArmDelay.Off()==false)
                break;
            bflag=true;
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    if(SortArmSuck->Suck[iR][iC].GetStatus()==false)
                    {
                        iInArmZVacuumbufferPos[iR][iC]-=10;
                        bflag=false;
                        #ifdef SOFT_SIMULATE
                        if(iInArmZVacuumbufferPos[iR][iC]>=-2000+(iR*iC+iC))
                        {
                            bflag=true;
                        }
                        #endif
                    }
                }
            }
            if(bflag)
            {
                Task=1000;
            }
            else
            {
                Task=650;
            }
            break;
        case 650:    //check motor step loss
            bflag=true;
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    if(iInArmZVacuumbufferPos[iR][iC]<-2000)
                    {
                        Task=8000;
                        bflag=false;
                        ShowMyMessage("SortArm suck motor step loss : "+SortArmSuck->Suck[iR][iC].Alias);
                        break;
                    }
                }
            }
            if(bflag)
                Task=500;
            break;
        case 1000:           //record pos
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    if(iR==iBaseSuckY && iC==iBaseSuckX)
                    {
                        iInArmZHeightDiff[iR][iC]=0;
                    }
                    else
                    {
                        iInArmZHeightDiff[iR][iC]=iInArmZVacuumbufferPos[iR][iC]-iInArmZVacuumbufferPos[iBaseSuckY][iBaseSuckX];
                    }
                    edZ[iR][iC]->Text=IntToStr(iInArmZHeightDiff[iR][iC]);
                }
            }
            Task=8000;
            break;
        case 8000: //z to 0
            bflag=true;
            for(int iR=0; iR<SortArmSuck->MaxItemR; iR++)
            {
                for(int iC=0; iC<SortArmSuck->MaxItemC; iC++)
                {
                    SortArmSuck->Suck[iR][iC].Off();
                    if(MSortArmZ[iR][iC]->MotorMove(0)==false)
                    {
                        bflag=false;
                    }
                }
            }
            if(bflag)
            {
                Task=9999;
            }
            break;
        case 9999: //finish
            bResult=true;
            break;
        default:
        	bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::DoSuckZAllHome(bool bReset)
{
    bool bret=false;
    int &Task=iDoSuckZAllHomeTask;
    bool bSortArmWait=false;
    AnsiString sErr="";
    if(bReset)
    {
        Task=1;
        bret=true;
        return bret;
    }
    switch(Task)
    {
        case 1:
            for(int iR=0; iR<MAX_ARM_Row; iR++)
            {
                for(int iC=0; iC<MAX_ARM_Col; iC++)
                {
                    MSortArmZ[iR][iC]->InitHomeTask_forSingleAxis();
                    bSortArmZHome[iR][iC]=false;
                }
            }
            Task=100;
            break;
        case 100:
            bSortArmWait=false;
            for(int iR=0; iR<MAX_ARM_Row; iR++)
            {
                for(int iC=0; iC<MAX_ARM_Col; iC++)
                {
                    if(bSortArmZHome[iR][iC]==false)
                    {
                        sErr="";
                        bSortArmZHome[iR][iC]=MSortArmZ[iR][iC]->Home(sErr);
                        if(sErr!="")
                        {
                            Task=9900;
                        }
                        bSortArmWait=true;
                    }
                }
            }
            if(bSortArmWait==false)
            {
                Task=9900;
            }
            break;
        case 9900:
            bret=true;
            break;
    }
    return bret;
}
//------------------------------------------------------------------------------
bool MySortArmParameter::GetSortArm1NeedSuck(int iSuckR, int iSuckC)
{
    bool bret=false;
    bret=bSortArmSuckActive[iSuckR][iSuckC] && bSortArm1UseSuck[iSuckR][iSuckC];
    return bret;
}
//------------------------------------------------------------------------------
int MySortArmParameter::GetNextAutoArea(int inowAuto)
{
    //AI(HT172-Maintainer) 20260413 : prioritize near-full bins when enabled
    if(CosFunction.bPrioritizeNearFullBin)
    {
        // Scan all Auto areas, pick the one with smallest positive iLeftCount
        int iBestAuto = -1;
        int iMinLeft = 0x7FFFFFFF;
        MyBinToTrayStruct tmpBtoT;

        for(int iA = eAuto1; iA <= eAuto20; iA++)
        {
            if(HSys.TrayUsage[iA] == eNoTray)
                continue;
            if(iTrayIC[iA] <= 0)
                continue;
            if(Auto->MAutoTray[iA] == NULL || Auto->MAutoTray[iA]->fHasTray == false)
                continue;

            tmpBtoT.ConvertAutoToBin(iA);
            if(tmpBtoT.iBin <= 0)
                continue;

            int iTotalForBin = myXML.iTotalBinCount[tmpBtoT.iBin];
            if(iTotalForBin <= 0)
                continue; // no lot plan data, skip priority calc

            int iLeftCount = iTotalForBin - tRunData.TrayICCnt[iA];
            if(iLeftCount <= 0)
                continue; // already full

            if(iLeftCount < iMinLeft)
            {
                iMinLeft = iLeftCount;
                iBestAuto = iA;
            }
        }

        if(iBestAuto >= eAuto1)
            return iBestAuto;

        // Fallback: no near-full bin found, use round-robin below
    }

    //AI(HT172-Maintainer) 20260410 : skip disabled areas (TrayUsage check)
    int iStartAuto=inowAuto;
    bool bWrapped=false;
    do
    {
        inowAuto++;
        if(inowAuto>eAuto20)
        {
            inowAuto=eAuto1;
            FindHasAutoSortBin();
            if(bWrapped)
                break;
            bWrapped=true;
        }
        if(HSys.TrayUsage[inowAuto]!=eNoTray)
            return inowAuto;
    } while(inowAuto!=iStartAuto);
    return inowAuto;
}
//------------------------------------------------------------------------------
