//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "database.h"
#include "MyPLC_IO_Modbus.h"
#include "cJSON.h"
#include "FileInfo.h"
#include "main.h"
//==============================================================================
#pragma package(smart_init)
#pragma link "SPComm"
#pragma resource "*.dfm"
TDataModule1 *DataModule1=new TDataModule1(NULL);
cBinAutoConvert cBinAutoTrans;
SYSTEM_MODULAR HSys;
TSimulationData tSimuData;                                                           // 電控元件及共用參數集合
//==============================================================================
__fastcall TDataModule1::TDataModule1(TComponent* Owner)
    : TDataModule(Owner)
{
}
//==============================================================================
//  建構子 _
//  不須改變 _
//==============================================================================
// 選擇溫控器品牌
void __fastcall SYSTEM_MODULAR::ChoiceTempController()                          // 選擇溫控器 //
{
    #ifndef UseTempController
    return;
    #endif
}
//==============================================================================
SYSTEM_MODULAR::~SYSTEM_MODULAR()
{
    HSys.BinDisCtrl->InitialOK=false;
    for(int i=iTotalMotor-1; i>=0; i--)
        delete MotPtr[i];

    for(int i=iTotalVMotor-1; i>=0; i--)
        delete VMotPtr[i];

    for(int i=iTotalCylinder-1; i>=0; i--)
    {
        delete CynPtr[i].Switch.Output;
        delete CynPtr[i].OnSensor.Input;
        delete CynPtr[i].OffSensor.Input;
    }
    for(int i=iTotalSensor-1; i>=0; i--)
        delete SenPtr[i].Input;

    for(int i=iTotalSwitch-1; i>=0; i--)
        delete SwPtr[i].Output;

    for(int i=iTotalSucker-1; i>=0; i--)
    {
        for(int iR=HSys.SuckPtr[i].MaxItemR-1; iR>=0; iR--)
        {
            for(int iC=HSys.SuckPtr[i].MaxItemC-1; iC>=0; iC--)
            {
                delete SuckPtr[i].Suck[iR][iC].OnSw.Output;
                delete SuckPtr[i].Suck[iR][iC].OffSw.Output;
                delete SuckPtr[i].Suck[iR][iC].Sensor.Input;
            }
        }
    }

//    if(BinDisCtrl!=NULL)
//    {
//        delete BinDisCtrl;
//        BinDisCtrl=NULL;
//    }

    mapAlarmCodeList.clear();
    mapNameToAlarm.clear();

    try
    {
        for(vector<TIODATA *>::iterator iter=IOTable.begin(); iter!=IOTable.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(IOTable);

        for(vector<TMOTDATA *>::iterator iter=MotTable.begin(); iter!=MotTable.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(MotTable);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfHome::FormDestroy");
    }
}
//==============================================================================
void __fastcall SYSTEM_MODULAR::Initial()                                       // MOTOR INITIAL //
{
    int Count=0;
    CUSTOMER_CODE   =CheckAndReadIniData(asGeneralPath, "System", "CUSTOMER_CODE", 0);
    AUTO_TRAY_CNT   =CheckAndReadIniData(asGeneralPath, "HARDWARE", "AUTO_TRAY_CNT", 20);
    USE_SECS_GEM    =CheckAndReadIniData(asGeneralPath, "System", "USE_SECS_GEM", 0);  //Steven 20240619 : Add for SECS/GEM

    USE_PUSH_TRAY    =CheckAndReadIniData(asGeneralPath, "System", "USE_PUSH_TRAY", 0);  //Steven 20240619 : Add for SECS/GEM    

    if(AUTO_TRAY_CNT<0 || AUTO_TRAY_CNT>eAuto20)
        AUTO_TRAY_CNT=eAuto20;

    FIX_TRAY_CNT    =CheckAndReadIniData(asGeneralPath, "HARDWARE", "FIX_TRAY_CNT", 2);

    if(FIX_TRAY_CNT<0 || FIX_TRAY_CNT>2)
        FIX_TRAY_CNT=2;

    MAGAZINE_CNT    =CheckAndReadIniData(asGeneralPath, "HARDWARE", "MAGAZINE_CNT", 1);

    if(MAGAZINE_CNT<0 || MAGAZINE_CNT>3)
        MAGAZINE_CNT=1;

    if(MAGAZINE_CNT==3)
    {
        Enable_MAG1=true;
        Enable_MAG3=true;
    }

    MAGAZINE_LAYER  =CheckAndReadIniData(asGeneralPath, "HARDWARE", "MAGAZINE_LAYER", 27);

    if(MAGAZINE_LAYER<0 || MAGAZINE_LAYER>eMagTotal)
        MAGAZINE_LAYER=eMagTotal;

    TrayUsage[eBinNotUse]=eNoTray;
    for(int i=eAuto1; i<=eAuto20; i++)
    {
        if(Count<AUTO_TRAY_CNT)
            TrayUsage[i]=eHasTray;
        else
            TrayUsage[i]=eNoTray;
        Count++;
    }

    if(FIX_TRAY_CNT==0)
    {
        TrayUsage[eFix1]=eNoTray;
        TrayUsage[eFix2]=eNoTray;
    }
    else if(FIX_TRAY_CNT==1)
    {
        TrayUsage[eFix1]=eHasTray;
        TrayUsage[eFix2]=eNoTray;
    }
    else if(FIX_TRAY_CNT==2)
    {
        TrayUsage[eFix1]=eHasTray;
        TrayUsage[eFix2]=eHasTray;
    }

    Count=0;
    for(int i=eMag01; i<eMagTotal; i++)
    {
        if(MAGAZINE_CNT==0)
        {
            TrayUsage[eMag1_01+i]=eNoTray;
            TrayUsage[eMag2_01+i]=eNoTray;
            TrayUsage[eMag3_01+i]=eNoTray;
        }
        else if(MAGAZINE_CNT==1)
        {
            if(Count<MAGAZINE_LAYER)
                TrayUsage[eMag1_01+i]=eHasTray;
            else
                TrayUsage[eMag1_01+i]=eNoTray;

            TrayUsage[eMag2_01+i]=eNoTray;
            TrayUsage[eMag3_01+i]=eNoTray;
        }
        else if(MAGAZINE_CNT==2)
        {
            if(Count<MAGAZINE_LAYER)
            {
                TrayUsage[eMag1_01+i]=eHasTray;
                TrayUsage[eMag2_01+i]=eHasTray;
            }
            else
            {
                TrayUsage[eMag1_01+i]=eNoTray;
                TrayUsage[eMag2_01+i]=eNoTray;
            }

            TrayUsage[eMag3_01+i]=eNoTray;
        }
        else if(MAGAZINE_CNT==3)
        {
            if(Count<MAGAZINE_LAYER)
            {
                TrayUsage[eMag1_01+i]=eHasTray;
                TrayUsage[eMag2_01+i]=eHasTray;
                TrayUsage[eMag3_01+i]=eHasTray;
            }
            else
            {
                TrayUsage[eMag1_01+i]=eNoTray;
                TrayUsage[eMag2_01+i]=eNoTray;
                TrayUsage[eMag3_01+i]=eNoTray;
            }
        }
        Count++;
    }

    bUseRemoteIO=true;
    Sys.SystemStart=false;
    Sys.RunMode=Run_Normal;
    Sys.bNeedHome=true;
    Sys.bHeaterReady=false;

    MyGem=new HT172Gem("HT172", HGem);
    MotPtr=(TTrayMotor **)&Mot;
    VMotPtr=(TTrayMotor **)&VMot;

    iTotalMotor=sizeof(MOTOR_MODULAR)/sizeof(TTrayMotor *);
    for(int i=0; i<iTotalMotor; i++)
    {
        HSys.MotPtr[i]=new TTrayMotor;
    }

    iTotalVMotor=sizeof(VIRTUAL_MOTOR_MODULAR)/sizeof(TTrayMotor *);            //20111111 terry S//
    for(int i=0; i<iTotalVMotor; i++)
    {
        HSys.VMotPtr[i]=new TTrayMotor;
    }                                                                           //20111111 terry E//
    Sys.bDummyRunSumulateHardware=false;
    CurrentDir="D:\\HT172";
    ChoiceTempController();
    bHasTimeString=false;
    CustomerFunctionSelect();
}
//==============================================================================
void SYSTEM_MODULAR::InstallColorBinDisplay()
{
    BinDisCtrl                      =new TMyBinDispHT9046;
    BinDisCtrlMag                   =new TMyBinDispHT9046;                      //JerryYang 20240601 : Add MGZ Bin Display
    BinDisCtrlTFT                   =new TMyBinDispHT9046;
    BinDisCtrlMagTFT                =new TMyBinDispHT9046;

    BinDisCtrl->SetUsedBinNumber(eBinDispTotal+eBinDispMagTotal);
    BinDisCtrlMag->SetUsedBinNumber(eBinDispMagTotal);
    BinDisCtrlTFT->SetUsedBinNumber(eBinDispTotal+eBinDispMagTotal);
    BinDisCtrlMagTFT->SetUsedBinNumber(eBinDispMagTotal);

    for(int i=0; i<eBinDispTotal+eBinDispMagTotal; i++)                         //JerryYang 20220909 : 12->eBinDispTotal
    {
        BinDisCtrl->InstalledUnit(i);
        BinDisCtrlTFT->InstalledUnit(i);
    }

    for(int i=0; i<eBinDispMagTotal; i++)                                       //JerryYang 20240601 : Add MGZ Bin Display
    {
        BinDisCtrlMag->InstalledUnit(i);
        BinDisCtrlMagTFT->InstalledUnit(i);
    }
}
//---------------------------------------------------------------------------
TIODATA::TIODATA(AnsiString Str)
{
    bool bHasNullData=false;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count>=14)
    {
        _CommaText=Str;
        Type=SL->Strings[HSys.IoNo.eioType];

        if(Type.AnsiPos(AnsiString("Cylinder_On"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Cylinder_Off"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Sucker_On"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Sucker_Off"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else
            Alias=SL->Strings[HSys.IoNo.eioAlias];

        if(SL->Strings[HSys.IoNo.eioLane]==AnsiString(""))
        {
            iLane=-1;
            if(iISABase==eMotionNet)                                            //Steven 20231218 HT7080B
                bHasNullData=true;
        }
        else
        {
            iLane=atoi(SL->Strings[HSys.IoNo.eioLane].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioModuleType]==AnsiString(""))
        {
            iModuleType=-1;
            //bHasNullData=true;
        }
        else
        {
            iModuleType=atoi(SL->Strings[HSys.IoNo.eioModuleType].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioIP]==AnsiString(""))
        {
            iIP=-1;
            if(iISABase==eMotionNet)                                            //Steven 20231218 HT7080B
                bHasNullData=true;
        }
        else
        {
            sIP=SL->Strings[HSys.IoNo.eioIP];
            if(sIP.AnsiCompare("A")>=0 && sIP.AnsiCompare("Z")<=0)
            {
                char *QQ=sIP.c_str();
                iIP=int(QQ[0]-'A')+10;
            }
            else if(sIP=="")
            {
                iIP=-1;
            }
            else //if(sIP.AnsiCompare("0")>=0 && sIP.AnsiCompare("9")<=0)
            {
                iIP=atoi(sIP.c_str());
            }
        }

        if(SL->Strings[HSys.IoNo.eioISABase]==AnsiString("") ||
           SL->Strings[HSys.IoNo.eioISABase]==AnsiString("MN200") ||
           SL->Strings[HSys.IoNo.eioISABase]==AnsiString("SynTek"))
        {
            iISABase=eMotionNet;
        }
        else
        {
            iISABase=atoi(SL->Strings[HSys.IoNo.eioISABase].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioPort]==AnsiString(""))
        {
            iPort=-1;
            bHasNullData=true;
        }
        else
        {
            if(iISABase==eISABase ||                                            //Nickliu 20230310 add Suck ISABase Type
               iISABase==ePCI1735U ||
               iISABase==e_PLCbase)
                iPort=HexStrToInt(SL->Strings[HSys.IoNo.eioPort].c_str());
            else
                iPort=atoi(SL->Strings[HSys.IoNo.eioPort].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioBit]==AnsiString(""))
        {
            iBit=-1;
            bHasNullData=true;
        }
        else
        {
            iBit=atoi(SL->Strings[HSys.IoNo.eioBit].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioInType]==AnsiString(""))
        {
            iInType=0;
        }
        else
        {
            iInType=atoi(SL->Strings[HSys.IoNo.eioInType].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioEnable]==AnsiString("") || bHasNullData)
        {
            iEnable=0;
        }
        else
        {
            iEnable=atoi(SL->Strings[HSys.IoNo.eioEnable].c_str());
        }

        if(iISABase==eISABase ||                                                //Nickliu 20230310 add Suck ISABase Type
           iISABase==ePCI1735U ||
           iISABase==e_PLCbase)
        {
            IOPos.sprintf("%s%s", SL->Strings[HSys.IoNo.eioPort], SL->Strings[HSys.IoNo.eioBit]);
        }
        else
        {
            IOPos.sprintf("%s%s%s%s%s", SL->Strings[HSys.IoNo.eioLane],
                                        SL->Strings[HSys.IoNo.eioModuleType],
                                        SL->Strings[HSys.IoNo.eioIP],
                                        SL->Strings[HSys.IoNo.eioPort],
                                        SL->Strings[HSys.IoNo.eioBit]);
        }

        if(SL->Strings[HSys.IoNo.eioOnAlarmTime]==AnsiString(""))
            iOnAlarmTime=-1;
        else
            iOnAlarmTime=atoi(SL->Strings[HSys.IoNo.eioOnAlarmTime].c_str());

        if(SL->Strings[HSys.IoNo.eioOffAlarmTime]==AnsiString(""))
            iOffAlarmTime=-1;
        else
            iOffAlarmTime=atoi(SL->Strings[HSys.IoNo.eioOffAlarmTime].c_str());

        if(SL->Strings[HSys.IoNo.eioOnDelayTime]==AnsiString(""))

            iOnDelayTime=-1;
        else
            iOnDelayTime=atoi(SL->Strings[HSys.IoNo.eioOnDelayTime].c_str());

        if(SL->Strings[HSys.IoNo.eioOffDelayTime]==AnsiString(""))
            iOffDelayTime=-1;
        else
            iOffDelayTime=atoi(SL->Strings[HSys.IoNo.eioOffDelayTime].c_str());
    }
    else
    {
        IOPos           ="";
        sIP             ="";
        _CommaText      ="";
        Type            ="";
        Alias           ="";
        iLane           =-1;
        iModuleType     =-1;
        iIP             =-1;
        iPort           =-1;
        iBit            =-1;
        iInType         =-1;
        iISABase        =-1;
        iEnable         =-1;
        iOnAlarmTime    =-1;
        iOffAlarmTime   =-1;
        iOnDelayTime    =-1;
        iOffDelayTime   =-1;
    }
    Tag=HSys.IOTable.size();
    bDelete=false;
    delete SL;
}
//---------------------------------------------------------------------------
TIOTABLENO::TIOTABLENO()
{
    eioType             =0;
    eioAlias            =1;
    eioLane             =2;
    eioModuleType       =3;
    eioIP               =4;
    eioPort             =5;
    eioBit              =6;
    eioInType           =7;
    eioISABase          =8;
    eioEnable           =9;
    eioOnAlarmTime      =10;
    eioOffAlarmTime     =11;
    eioOnDelayTime      =12;
    eioOffDelayTime     =13;
    eioNote             =14;
    eioTotal            =15;
}
//---------------------------------------------------------------------------
int TIOTABLENO::SetIOTableNo(AnsiString Str)
{
    int bResult=eioTotal;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count!=eioTotal)
    {
        bResult=false;
    }
    else
    {
        eioType=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("IOType"))!=0)
                eioType=i;
        if(eioType==-1)
            bResult=0;

        eioAlias=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Alias"))!=0)
                eioAlias=i;
        if(eioAlias==-1)
            bResult=1;

        eioLane=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Lane"))!=0)
                eioLane=i;
        if(eioLane==-1)
            bResult=2;

        eioModuleType=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("ModuleType"))!=0)
                eioModuleType=i;
        if(eioModuleType==-1)
            bResult=3;

        eioIP=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("IP"))!=0)
                eioIP=i;
        if(eioIP==-1)
            bResult=4;

        eioPort=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Port"))!=0)
                eioPort=i;
        if(eioPort==-1)
            bResult=5;

        eioBit=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Bit"))!=0)
                eioBit=i;
        if(eioBit==-1)
            bResult=6;

        eioInType=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("InType"))!=0)
                eioInType=i;
        if(eioInType==-1)
            bResult=7;

        eioISABase=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("ISABase"))!=0)
                eioISABase=i;
        if(eioISABase==-1)
            bResult=8;

        eioEnable=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Enable"))!=0)
                eioEnable=i;
        if(eioEnable==-1)
            bResult=9;

        eioOnAlarmTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("OnAlarmTime"))!=0)
                eioOnAlarmTime=i;
        if(eioOnAlarmTime==-1)
            bResult=10;

        eioOffAlarmTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("OffAlarmTime"))!=0)
                eioOffAlarmTime=i;
        if(eioOffAlarmTime==-1)
            bResult=11;

        eioOnDelayTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("OnDelayTime"))!=0)
                eioOnDelayTime=i;
        if(eioOnDelayTime==-1)
            bResult=12;

        eioOffDelayTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("OffDelayTime"))!=0)
                eioOffDelayTime=i;
        if(eioOffDelayTime==-1)
            bResult=13;

        eioNote=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Note"))!=0)
                eioNote=i;
        if(eioNote==-1)
            bResult=14;
    }

    delete SL;
    return bResult;
}
//---------------------------------------------------------------------------
TMOTNO::TMOTNO()
{
    emotNo               =0;
    emotAlias            =1;
    emotDirection        =2;
    emotGearRatio        =3;
    emotHomeDirectior    =4;
    emotHomeHighSpeed    =5;
    emotHomeLowSpeed     =6;
    emotInitSpeed        =7;
    emotJogHighSpeed     =8;
    emotJogLowSpeed      =9;
    emotRate             =10;
    emotSoftLimitN       =11;
    emotSoftLimitP       =12;
    emotEnable           =13;
    emotServoAlarmOn     =14;
    emotRange            =15;
    emot1P2P             =16;
    emotSensorType       =17;
    emotSimulateSpeed    =18;
    emotCardModel        =19;
    emotBoardID          =20;
    emotPort             =21;
    emotAcc              =22;
    emotDec              =23;
    emotMotorKind        =24;
    emoFlushPanel        =25;
    emoHomeOrder         =26;
    emotLimitLogic       =27;
    emotIn1Logic         =28;
    emotTotal            =29;
}
//---------------------------------------------------------------------------
int TMOTNO::SetMOTTableNo(AnsiString Str)
{
    int bResult=emotTotal;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count>=HSys.MotNo.emotTotal)
    {
        bResult=-2;
    }
    {
        emotNo=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Motorname"))!=0)
                emotNo=i;
        if(emotNo==-1)
            bResult=0;

        emotAlias=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Alias"))!=0)
                emotAlias=i;
        if(emotAlias==-1)
            bResult=1;

        emotEnable=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Enable"))!=0)
                emotEnable=i;
        if(emotEnable==-1)
            bResult=2;

        emotBoardID=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("BoardID"))!=0)
                emotBoardID=i;
        if(emotBoardID==-1)
            bResult=3;

        emotPort=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Port"))!=0)
                emotPort=i;
        if(emotPort==-1)
            bResult=4;

        emotGearRatio=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("GearRatio"))!=0)
                emotGearRatio=i;
        if(emotGearRatio==-1)
            bResult=6;

        emotSoftLimitN=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("SoftLimitN"))!=0)
                emotSoftLimitN=i;
        if(emotSoftLimitN==-1)
            bResult=7;

        emotSoftLimitP=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("SoftLimitP"))!=0)
                emotSoftLimitP=i;
        if(emotSoftLimitP==-1)
            bResult=8;

        emotCardModel=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("CardModel"))!=0)
                emotCardModel=i;
        if(emotCardModel==-1)
            bResult=9;

        emotDirection=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Direction"))!=0)
                emotDirection=i;
        if(emotDirection==-1)
            bResult=10;

        emotHomeDirectior=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("HomeDirectior"))!=0)
                emotHomeDirectior=i;
        if(emotHomeDirectior==-1)
            bResult=11;

        emotAcc=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Acc"))!=0)
                emotAcc=i;
        if(emotAcc==-1)
            bResult=12;

        emotDec=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotDec==-1 && SL->Strings[i].AnsiPos(AnsiString("Dec"))!=0)
                emotDec=i;
        if(emotDec==-1)
            bResult=13;

        emotInitSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("InitSpeed"))!=0)
                emotInitSpeed=i;
        if(emotInitSpeed==-1)
            bResult=14;

        emotHomeHighSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotHomeHighSpeed==-1 && SL->Strings[i].AnsiPos(AnsiString("HomeHighSpeed"))!=0)
                emotHomeHighSpeed=i;
        if(emotHomeHighSpeed==-1)
            bResult=15;

        emotHomeLowSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotHomeLowSpeed==-1 && SL->Strings[i].AnsiPos(AnsiString("HomeLowSpeed"))!=0)
                emotHomeLowSpeed=i;
        if(emotHomeLowSpeed==-1)
            bResult=16;

        emotJogHighSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotJogHighSpeed==-1 && SL->Strings[i].AnsiPos(AnsiString("JogHighSpeed"))!=0)
                emotJogHighSpeed=i;
        if(emotJogHighSpeed==-1)
            bResult=17;

        emotJogLowSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotJogLowSpeed==-1 && SL->Strings[i].AnsiPos(AnsiString("JogLowSpeed"))!=0)
                emotJogLowSpeed=i;
        if(emotJogLowSpeed==-1)
            bResult=18;

        emotRange=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotRange==-1 && SL->Strings[i].AnsiPos(AnsiString("Range"))!=0)
                emotRange=i;
        if(emotRange==-1)
            bResult=19;

        emotRate=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotRate==-1 && SL->Strings[i].AnsiPos(AnsiString("Rate"))!=0)
                emotRate=i;
        if(emotRate==-1)
            bResult=20;

        emotServoAlarmOn=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotServoAlarmOn==-1 && SL->Strings[i].AnsiPos(AnsiString("ServoAlarmOn"))!=0)
                emotServoAlarmOn=i;
        if(emotServoAlarmOn==-1)
            bResult=21;

        emot1P2P=-1;
        for(int i=0; i<SL->Count; i++)
            if(emot1P2P==-1 && SL->Strings[i].AnsiPos(AnsiString("1P2P"))!=0)
                emot1P2P=i;
        if(emot1P2P==-1)
            bResult=22;

        emotSensorType=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotSensorType==-1 && SL->Strings[i].AnsiPos(AnsiString("SensorType"))!=0)
                emotSensorType=i;
        if(emotSensorType==-1)
            bResult=23;

        emotMotorKind=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotMotorKind==-1 && SL->Strings[i].AnsiPos(AnsiString("MotorKind"))!=0)
                emotMotorKind=i;
        if(emotMotorKind==-1)
            bResult=24;

        emoFlushPanel=-1;
        for(int i=0; i<SL->Count; i++)
            if(emoFlushPanel==-1 && SL->Strings[i].AnsiPos(AnsiString("FlushPanel"))!=0)
                emoFlushPanel=i;
        if(emoFlushPanel==-1)
            bResult=25;

        emotLimitLogic=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotLimitLogic==-1 && SL->Strings[i].AnsiPos(AnsiString("LimitLogic"))!=0)
                emotLimitLogic=i;
        if(emotLimitLogic==-1)
            bResult=26;

        emotIn1Logic=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotIn1Logic==-1 && SL->Strings[i].AnsiPos(AnsiString("In1Logic"))!=0)
                emotIn1Logic=i;
        if(emotIn1Logic==-1)
            bResult=27;

        emotSimulateSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(emotSimulateSpeed==-1 && SL->Strings[i].AnsiPos(AnsiString("SimulateSpeed"))!=0)
                emotSimulateSpeed=i;
        if(emotSimulateSpeed==-1)
            bResult=28;

        emoHomeOrder=-1;
        for(int i=0; i<SL->Count; i++)
            if(emoHomeOrder==-1 && SL->Strings[i].AnsiPos(AnsiString("HomeOrder"))!=0)
                emoHomeOrder=i;
        if(emoHomeOrder==-1)
            bResult=29;

        if(bResult==-2)
            bResult=emotTotal;
    }
    delete SL;
    return bResult;
}
//---------------------------------------------------------------------------
TMOTDATA::TMOTDATA(AnsiString Str)
{
    AnsiString Msg;
    bool bHasNullData=false;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count>=HSys.MotNo.emotTotal)
    {
        _CommaText=Str;

        if(HSys.MotNo.emotEnable<0 ||                                       //Steven 20240103 : 加入保護
           SL->Strings[HSys.MotNo.emotEnable]==AnsiString(""))
        {
            iEnable=0;
        }
        else
        {
            iEnable=atoi(SL->Strings[HSys.MotNo.emotEnable].c_str());
        }

        if(HSys.MotNo.emotNo<0 ||                                           //Steven 20240103 : 加入保護
           SL->Strings[HSys.MotNo.emotNo]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("MotorName is NULL in %s", HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            No="";
        }
        else
        {
            No=SL->Strings[HSys.MotNo.emotNo];
        }

        if(HSys.MotNo.emotAlias<0 ||                                        //Steven 20240103 : 加入保護
           SL->Strings[HSys.MotNo.emotAlias]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Alias of %s is NULL in %s", No, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            Alias="";
        }
        else
        {
            Alias=SL->Strings[HSys.MotNo.emotAlias];
        }

        if(HSys.MotNo.emotCardModel<0 ||                                    //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotCardModel]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("CardModel of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            CardModel="";
        }
        else
        {
            CardModel=SL->Strings[HSys.MotNo.emotCardModel];
        }

        if(CardModel=="MC88X1")                                             //Steven 20231218 HT7080B
        {
            iBoardID=0;
        }
        else
        {
            if(HSys.MotNo.emotBoardID<0 ||
               SL->Strings[HSys.MotNo.emotBoardID]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("Alias of %s is NULL in %s", Alias, HSys.MotTablePath);
                    ShowMyMessage(Msg);
                }
                bHasNullData=true;
                iBoardID=-1;
            }
            else
            {
                iBoardID=atoi(SL->Strings[HSys.MotNo.emotBoardID].c_str());
            }
        }

        if(HSys.MotNo.emotPort<0 ||                                             //Steven 20240103 : 加入保護
           SL->Strings[HSys.MotNo.emotPort]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Port of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iPort=-1;
        }
        else
        {
            iPort=atoi(SL->Strings[HSys.MotNo.emotPort].c_str());
        }

        if(CardModel=="MC88X1")                                         //20231218 Steven HT7080B
        {
            dAcc=1.0;
            dDec=1.0;
            iLimitLogic=0;
            iIn1Logic=0;
        }
        else
        {
            if(HSys.MotNo.emotAcc<0 ||
               SL->Strings[HSys.MotNo.emotAcc]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("ACC of %s is NULL in %s", Alias, HSys.MotTablePath);
                    ShowMyMessage(Msg);
                }
                bHasNullData=true;
                dAcc=1.0;
            }
            else
            {
                dAcc=atof(SL->Strings[HSys.MotNo.emotAcc].c_str());
            }

            if(HSys.MotNo.emotDec<0 ||
               SL->Strings[HSys.MotNo.emotDec]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("DEC of %s is NULL in %s", Alias, HSys.MotTablePath);
                    ShowMyMessage(Msg);
                }
                bHasNullData=true;
                dDec=1.0;
            }
            else
            {
                dDec=atof(SL->Strings[HSys.MotNo.emotDec].c_str());
            }

            if(HSys.MotNo.emotLimitLogic<0 ||
               SL->Strings[HSys.MotNo.emotLimitLogic]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("LimitLogic of %s is NULL in %s", Alias, HSys.MotTablePath);
                    ShowMyMessage(Msg);
                }
                bHasNullData=true;
                iLimitLogic=0;
            }
            else
            {
                iLimitLogic=atoi(SL->Strings[HSys.MotNo.emotLimitLogic].c_str());
            }

            if(HSys.MotNo.emotIn1Logic<0 ||
               SL->Strings[HSys.MotNo.emotIn1Logic]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("In1Logic of %s is NULL in %s", Alias, HSys.MotTablePath);
                    ShowMyMessage(Msg);
                }
                bHasNullData=true;
                iIn1Logic=0;
            }
            else
            {
                iIn1Logic=atoi(SL->Strings[HSys.MotNo.emotIn1Logic].c_str());
            }
        }

        if(HSys.MotNo.emotGearRatio<0 ||                                          //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotGearRatio]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("GearRatio of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            dGearRatio=1.0;
        }
        else
        {
            dGearRatio=atof(SL->Strings[HSys.MotNo.emotGearRatio].c_str());
        }

        if(HSys.MotNo.emotDirection<0 ||                                        //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotDirection]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Direction of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iDirection=0;
        }
        else
        {
            iDirection=atoi(SL->Strings[HSys.MotNo.emotDirection].c_str());
        }

        if(HSys.MotNo.emotHomeDirectior<0 ||                                    //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotHomeDirectior]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeDirectior of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iHomeDirectior=0;
        }
        else
        {
            iHomeDirectior=atoi(SL->Strings[HSys.MotNo.emotHomeDirectior].c_str());
        }

        if(HSys.MotNo.emotSoftLimitN<0 ||                                       //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotSoftLimitN]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SoftLimitN of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iSoftLimitN=-999999;
        }
        else
        {
            iSoftLimitN=atoi(SL->Strings[HSys.MotNo.emotSoftLimitN].c_str());
        }

        if(HSys.MotNo.emotSoftLimitP<0 ||                                      //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotSoftLimitP]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SoftLimitP of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iSoftLimitP=999999;
        }
        else
        {
            iSoftLimitP=atoi(SL->Strings[HSys.MotNo.emotSoftLimitP].c_str());
        }

        if(HSys.MotNo.emotInitSpeed<0 ||                                        //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotInitSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("InitSpeed of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iInitSpeed=100;
        }
        else
        {
            iInitSpeed=atoi(SL->Strings[HSys.MotNo.emotInitSpeed].c_str());
        }

        if(HSys.MotNo.emotHomeHighSpeed<0 ||                                    //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotHomeHighSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeHighSpeed of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iHomeHighSpeed=100;
        }
        else
        {
            iHomeHighSpeed=atoi(SL->Strings[HSys.MotNo.emotHomeHighSpeed].c_str());
        }

        if(HSys.MotNo.emotHomeLowSpeed<0 ||                                     //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotHomeLowSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeLowSpeed of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iHomeLowSpeed=100;
        }
        else
        {
            iHomeLowSpeed=atoi(SL->Strings[HSys.MotNo.emotHomeLowSpeed].c_str());
        }

        if(HSys.MotNo.emotJogHighSpeed<0 ||                                     //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotJogHighSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("JogHighSpeed of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iJogHighSpeed=100;
        }
        else
        {
            iJogHighSpeed=atoi(SL->Strings[HSys.MotNo.emotJogHighSpeed].c_str());
        }

        if(HSys.MotNo.emotJogLowSpeed<0 ||                                      //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotJogLowSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("JogLowSpeed of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iJogLowSpeed=100;
        }
        else
        {
            iJogLowSpeed=atoi(SL->Strings[HSys.MotNo.emotJogLowSpeed].c_str());
        }

        if(CardModel=="MC88X1")                                                 //Steven 20231218 HT7080B
        {
            iRange=10;
        }
        else
        {
            if(HSys.MotNo.emotRange<0 ||
               SL->Strings[HSys.MotNo.emotRange]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("Range of %s is NULL in %s", Alias, HSys.MotTablePath);
                    ShowMyMessage(Msg);
                }
                bHasNullData=true;
                iRange=1;
            }
            else
            {
                iRange=atoi(SL->Strings[HSys.MotNo.emotRange].c_str());
            }
        }

        if(HSys.MotNo.emotRate<0 ||                                              //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotRate]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Rate of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iRate=1;
        }
        else
        {
            iRate=atoi(SL->Strings[HSys.MotNo.emotRate].c_str());
        }

        if(HSys.MotNo.emotServoAlarmOn<0 ||                                     //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotServoAlarmOn]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("ServoAlarmOn of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iServoAlarmOn=0;
        }
        else
        {
            iServoAlarmOn=atoi(SL->Strings[HSys.MotNo.emotServoAlarmOn].c_str());
        }

        if(HSys.MotNo.emot1P2P<0 ||                                             //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emot1P2P]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("1P2P of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            i1P2P=0;
        }
        else
        {
            i1P2P=atoi(SL->Strings[HSys.MotNo.emot1P2P].c_str());
        }

        if(HSys.MotNo.emotSensorType<0 ||                                       //20231218 Steven HT7080B
           SL->Strings[HSys.MotNo.emotSensorType]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SensorType of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            iSensorType=0;
        }
        else
        {
            iSensorType=atoi(SL->Strings[HSys.MotNo.emotSensorType].c_str());
        }

        if(HSys.MotNo.emotMotorKind<0 ||                                   //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotMotorKind]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("MotorKind of %s is NULL in %s", Alias, HSys.MotTablePath);
                ShowMyMessage(Msg);
            }
            bHasNullData=true;
            MotorKind=0;
        }
        else
        {
            MotorKind=atoi(SL->Strings[HSys.MotNo.emotMotorKind].c_str());
        }
        FlushPanel=atoi(SL->Strings[HSys.MotNo.emoFlushPanel].c_str());
        HomeOrder=atoi(SL->Strings[HSys.MotNo.emoHomeOrder].c_str());

        if(CardModel=="MC88X1")                                                 //Steven 20231218 HT7080B
        {
            iSimulateSpeed=1000;
        }
        else
        {
            iSimulateSpeed=atoi(SL->Strings[HSys.MotNo.emotSimulateSpeed].c_str());
        }

        if(bHasNullData)
        {
            iEnable=0;
        }
    }
    else
    {
        _CommaText      ="";
        No              ="";
        Alias           ="";
        iEnable         =-1;
        iBoardID        =-1;
        iPort           =-1;
        dGearRatio      =1.0;
        iSoftLimitN     =-999999;
        iSoftLimitP     =999999;
        CardModel       ="";
        FlushPanel      ="";
        HomeOrder       ="";
        iDirection      =0;
        iHomeDirectior  =0;
        dAcc            =1.0;                               //Steven 20231218 HT7080B
        dDec            =1.0;                               //Steven 20231218 HT7080B
        iInitSpeed      =100;
        iHomeHighSpeed  =100;
        iHomeLowSpeed   =100;
        iJogHighSpeed   =100;
        iJogLowSpeed    =100;
        iRange          =1;
        iRate           =1;
        iServoAlarmOn   =0;
        i1P2P           =0;
        iSensorType     =0;
        MotorKind       =0;
        iLimitLogic     =0;
        iIn1Logic       =0;
        iSimulateSpeed  =10000;
    }
    delete SL;
}
//==============================================================================
SYSTEM_MODULAR::SYSTEM_MODULAR()
{
    MotTablePath    ="D:\\HT172\\System\\Mot_Table.csv";
    IoTablePath     ="D:\\HT172\\System\\IO_Table.csv";
    TrayTablePath   ="D:\\HT172\\System\\TrayForm.csv";
    AlarmTablePath  ="D:\\HT172\\System\\AlarmList.csv";
    AlarmMapPath    ="D:\\HT172\\System\\AlarmListMap.csv";
    Initial();
    BinDisCtrl=NULL;
    InstallColorBinDisplay();
    InitialHardwareNameAndLoadDatabase();
    OpenGeneralIniFile();           //Steven 20141120 : Add Read/Write IniFile Speed
}
//---------------------------------------------------------------------------
void SYSTEM_MODULAR::LoadMotData()
{
    AnsiString Str;
    int iResult;
    if(FileExists(MotTablePath))
    {
        TStringList *StrList=new TStringList();

        mapMotTable.clear();
        MotTable.clear();
        try
        {
            StrList->LoadFromFile(MotTablePath);

            if(StrList->Count<=1)
            {
                Str.sprintf("File %s data is lose!", MotTablePath);
                ShowMyMessage(Str);
            }
            else
            {
                iResult=HSys.MotNo.SetMOTTableNo(StrList->Strings[0]);
                if(iResult>=HSys.MotNo.emotTotal-1)     //Steven 20231218 HT7080B
                {
                    for(int i=1; i<StrList->Count; i++)
                    {
                        MotTable.push_back(new TMOTDATA(StrList->Strings[i]));
                        mapMotTableIter=mapMotTable.find(MotTable[i-1]->Alias);
                        Str=MotTable[i-1]->Alias;
                        if(Str!="")
                        {
                            if(mapMotTableIter==mapMotTable.end())
                            {
                                mapMotTable[MotTable[i-1]->Alias]=i-1;
                            }
                            else
                            {
                                Str.sprintf("Motor %s alias is duplicated!", MotTable[i-1]->Alias);
                                ShowMyMessage(Str);
                            }
                        }
                    }
                }
                else
                {
                    Str.sprintf("File %s data is mistake! (%d)", MotTablePath, iResult);
                    ShowMyMessage(Str);
                }
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "SYSTEM_MODULAR::LoadIoData");
            Str.sprintf("File %s is opened by other software!", MotTablePath);
            ShowMyMessage(Str);
        }
        delete StrList;

    }
    else
    {
        Str.sprintf("File %s is not exist!", MotTablePath);
        ShowMyMessage(Str);
    }
}
//==============================================================================
//  001
//  使用者依不同機器修改 _                                                      // NEED EDIT BY USER //
//==============================================================================
void __fastcall TDataModule1::InitialMotorNameExecute(TObject *Sender)          // 真實及虛擬馬達 //
{
    HSys.Mot.SortArm1X        ->Alias="SortArm1X"       ;
    HSys.Mot.SortArm1Y        ->Alias="SortArm1Y"       ;
    HSys.Mot.SortArm1ZA       ->Alias="SortArm1ZA"      ;
    HSys.Mot.SortArm1ZB       ->Alias="SortArm1ZB"      ;
    HSys.Mot.SortArm1ZC       ->Alias="SortArm1ZC"      ;
    HSys.Mot.SortArm1ZD       ->Alias="SortArm1ZD"      ;
    HSys.Mot.SortArm1ZE       ->Alias="SortArm1ZE"      ;
    HSys.Mot.SortArm1ZF       ->Alias="SortArm1ZF"      ;
    HSys.Mot.SortArm1ZG       ->Alias="SortArm1ZG"      ;
    HSys.Mot.SortArm1ZH       ->Alias="SortArm1ZH"      ;
    HSys.Mot.SortArm1PitchX   ->Alias="SortArm1PitchX"  ;
    HSys.Mot.SortArm1PitchY1  ->Alias="SortArm1PitchY1" ;
    HSys.Mot.SortArm1PitchY2  ->Alias="SortArm1PitchY2" ;
    HSys.Mot.LoaderCCDArmX    ->Alias="LoaderCCDArmX"   ;
    HSys.Mot.StoreArmX        ->Alias="StoreArmX"       ;
    HSys.Mot.StoreArmY        ->Alias="StoreArmY"       ;
    HSys.Mot.StoreArmZ        ->Alias="StoreArmZ"       ;
    HSys.Mot.TrayShiftStageX  ->Alias="TrayShiftStageX" ;
    HSys.Mot.SortArm2X        ->Alias="SortArm2X"       ;
    HSys.Mot.SortArm2Y        ->Alias="SortArm2Y"       ;
    HSys.Mot.SortArm2ZA       ->Alias="SortArm2ZA"      ;
    HSys.Mot.SortArm2ZB       ->Alias="SortArm2ZB"      ;
    HSys.Mot.SortArm2ZC       ->Alias="SortArm2ZC"      ;
    HSys.Mot.SortArm2ZD       ->Alias="SortArm2ZD"      ;
    HSys.Mot.SortArm2ZE       ->Alias="SortArm2ZE"      ;
    HSys.Mot.SortArm2ZF       ->Alias="SortArm2ZF"      ;
    HSys.Mot.SortArm2ZG       ->Alias="SortArm2ZG"      ;
    HSys.Mot.SortArm2ZH       ->Alias="SortArm2ZH"      ;
    HSys.Mot.SortArm2PitchX   ->Alias="SortArm2PitchX"  ;
    HSys.Mot.SortArm2PitchY1  ->Alias="SortArm2PitchY1" ;
    HSys.Mot.SortArm2PitchY2  ->Alias="SortArm2PitchY2" ;
    HSys.Mot.Magazine1X       ->Alias="Magazine1X"      ;
    HSys.Mot.Magazine1Z       ->Alias="Magazine1Z"      ;
    HSys.Mot.Magazine2X       ->Alias="Magazine2X"      ;
    HSys.Mot.Magazine2Z       ->Alias="Magazine2Z"      ;
    HSys.Mot.Magazine3X       ->Alias="Magazine3X"      ;
    HSys.Mot.Magazine3Z       ->Alias="Magazine3Z"      ;
    HSys.Mot.PushStoreArmX1   ->Alias="PushStoreArmX1"  ;                       //推Tray 機構改為馬達
    HSys.Mot.PushStoreArmX2   ->Alias="PushStoreArmX2"  ;                       //推Tray 機構改為馬達
    HSys.Mot.PushStoreArmX3   ->Alias="PushStoreArmX3"  ;                       //推Tray 機構改為馬達
    HSys.Mot.PushStoreArmX4   ->Alias="PushStoreArmX4"  ;                       //推Tray 機構改為馬達

    HSys.Mot.AutoTrolly1Z     ->Alias="AutoTrolly1Z"    ;                  
    HSys.Mot.AutoTrolly2Z     ->Alias="AutoTrolly2Z"    ;                  
    HSys.Mot.AutoTrolly3Z     ->Alias="AutoTrolly3Z"    ;
    HSys.Mot.AutoTrolly4Z     ->Alias="AutoTrolly4Z"    ;
    HSys.Mot.AutoTrolly1X     ->Alias="AutoTrolly1X"    ;                  
    HSys.Mot.AutoTrolly2X     ->Alias="AutoTrolly2X"    ;                  
    HSys.Mot.AutoTrolly3X     ->Alias="AutoTrolly3X"    ;                  
    HSys.Mot.AutoTrolly4X     ->Alias="AutoTrolly4X"    ;

    HSys.Mot.SortArm1X        ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1Y        ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1ZA       ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1ZB       ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1ZC       ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1ZD       ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1ZE       ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1ZF       ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1ZG       ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1ZH       ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1PitchX   ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1PitchY1  ->FlushPanelName="palSortArm1";
    HSys.Mot.SortArm1PitchY2  ->FlushPanelName="palSortArm1";
    HSys.Mot.LoaderCCDArmX    ->FlushPanelName="palLoadCarID";
    HSys.Mot.StoreArmX        ->FlushPanelName="palTrayArm";
    HSys.Mot.StoreArmY        ->FlushPanelName="palTrayArm";
    HSys.Mot.StoreArmZ        ->FlushPanelName="palTrayArm";
    HSys.Mot.TrayShiftStageX  ->FlushPanelName="palShiftStage";
    HSys.Mot.SortArm2X        ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2Y        ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2ZA       ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2ZB       ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2ZC       ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2ZD       ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2ZE       ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2ZF       ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2ZG       ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2ZH       ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2PitchX   ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2PitchY1  ->FlushPanelName="palSortArm2";
    HSys.Mot.SortArm2PitchY2  ->FlushPanelName="palSortArm2";
    HSys.Mot.Magazine1X       ->FlushPanelName="palMag1";
    HSys.Mot.Magazine1Z       ->FlushPanelName="palMag1";
    HSys.Mot.Magazine2X       ->FlushPanelName="palMag2";
    HSys.Mot.Magazine2Z       ->FlushPanelName="palMag2";
    HSys.Mot.Magazine3X       ->FlushPanelName="palMag3";
    HSys.Mot.Magazine3Z       ->FlushPanelName="palMag3";
    HSys.Mot.PushStoreArmX1   ->FlushPanelName="";                              //推Tray 機構改為馬達
    HSys.Mot.PushStoreArmX2   ->FlushPanelName="";                              //推Tray 機構改為馬達
    HSys.Mot.PushStoreArmX3   ->FlushPanelName="";                              //推Tray 機構改為馬達
    HSys.Mot.PushStoreArmX4   ->FlushPanelName="";                              //推Tray 機構改為馬達
}//找馬達名稱
//==============================================================================
//  002
//  不須改變 _
//==============================================================================
void SYSTEM_MODULAR::LoadSingleMotorParameterFromDataBase(int Index,bool bInitial)            //20111111 terry//
{
    LoadMotorParameterFromDataBase(Index,bInitial);
}
//==============================================================================
void SYSTEM_MODULAR::LoadMotorParameterFromDataBase(int Index,bool bInitial)                  //20111111 terry//
{
    AnsiString S;
    AnsiString BoardID,Port;
    AnsiString CardModel, Mot_Name;
    int iAdder, iMot;
    AnsiString MStr;
    HSys.LoadMotData();
    bool bHasMotor=false;
    MotPtr=(TTrayMotor **)&Mot;
    int iCount=(Index==-1)?iTotalMotor:Index+1;
    int iStart=(Index==-1)?0:Index;

    for(int i=iStart; i<iCount; i++)
    {
        bHasMotor=false;
        Mot_Name.sprintf("M%02d", i);
        MStr=MotPtr[i]->Alias;
        HSys.mapMotTableIter=HSys.mapMotTable.find(MStr);
        if(HSys.mapMotTableIter!=HSys.mapMotTable.end())
        {
            bHasMotor=true;
            iMot=atoi(HSys.mapMotTable[MStr].c_str());
        }
        else
        {
            S.sprintf("Can not find motor %s", MStr);
        }

        if(iMot<0)
        {
            S.sprintf("Can not find motor %s", MStr);
        }

        if(bHasMotor)
        {
            CardModel          =HSys.MotTable[iMot]->CardModel;
        }
        else
        {
            CardModel          ="";
        }
        MotPtr[i]->Number       =Mot_Name;
        MotPtr[i]->NumberAlias  =AnsiString("[")+AnsiString(Mot_Name)+AnsiString("] ")+MotPtr[i]->Alias;

        MotPtr[i]->AlarmName[eMotPwrErr    ] =MotPtr[i]->Alias+"_MotPwrErr";
        MotPtr[i]->AlarmName[eMotTorqueErr ] =MotPtr[i]->Alias+"_MotTorqueErr";
        MotPtr[i]->AlarmName[eMotCWOnErr   ] =MotPtr[i]->Alias+"_MotCWOnErr";
        MotPtr[i]->AlarmName[eMotCCWOnErr  ] =MotPtr[i]->Alias+"_MotCCWOnErr";
        MotPtr[i]->AlarmName[eMotSoftPErr  ] =MotPtr[i]->Alias+"_MotSoftPErr";
        MotPtr[i]->AlarmName[eMotSoftNErr  ] =MotPtr[i]->Alias+"_MotSoftNErr";
        MotPtr[i]->AlarmName[eMotPosErr    ] =MotPtr[i]->Alias+"_MotPosErr";
        MotPtr[i]->AlarmName[eMotUnDefErr  ] =MotPtr[i]->Alias+"_MotUnDefErr";
        MotPtr[i]->AlarmName[eMotOverLimitErr] =MotPtr[i]->Alias+"_MotOverLimitErr";

        if(CardModel=="SMC")
        {
            if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1)
                iAdder=HSys.MotTable[iMot]->iBoardID*10+HSys.MotTable[iMot]->iPort;
            else
                iAdder=-1;
        }
        else if(CardModel=="MN200" ||
                CardModel=="SynTek")
        {
            if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1)
                iAdder=HSys.MotTable[iMot]->iBoardID*100+HSys.MotTable[iMot]->iPort;
            else
                iAdder=-1;
        }
        else                                                                    // unknow
        {
            ShowMessage("Undefine card model in motor.db");
        }
        MotPtr[i]->CardModel=CardModel;
        if(Index==-1)                                                           //Steven 20240502 : fixed for motor
        {
            MotPtr[i]->InitialMotorObject(iAdder);
            MotPtr[i]->SetMotNo(i);
        }
        #ifdef SOFT_SIMULATE                                                    //Steven : 暫時馬克掉,記得改回來
            MotPtr[i]->SetEnable(false);
        #else
            if(bHasMotor)
                MotPtr[i]->SetEnable(HSys.MotTable[iMot]->iEnable);
            else
                MotPtr[i]->SetEnable(false);
        #endif
        if(CardModel=="SMC")
        {
            MotPtr[i]->SetMotionCardType(eSMC);
        }
        else if(CardModel=="MN200")
        {
            MotPtr[i]->SetMotionCardType(eMN200);
        }
        else                                                            // unknow
        {
            MotPtr[i]->SetMotionCardType(eMotionCardUnknown);
        }

        MotPtr[i]->SetDirection                 (HSys.MotTable[iMot]->iDirection);
        MotPtr[i]->SetHomeDirection             (HSys.MotTable[iMot]->iHomeDirectior);
        MotPtr[i]->SetGearRatio                 (HSys.MotTable[iMot]->dGearRatio);
        MotPtr[i]->SetHomeHighSpeed             (HSys.MotTable[iMot]->iHomeHighSpeed);
        MotPtr[i]->SetHomeLowSpeed              (HSys.MotTable[iMot]->iHomeLowSpeed);
        MotPtr[i]->SetJogHighSpeed              (HSys.MotTable[iMot]->iJogHighSpeed);
        MotPtr[i]->SetJogLowSpeed               (HSys.MotTable[iMot]->iJogLowSpeed);
        MotPtr[i]->SetSoftLimitN                (HSys.MotTable[iMot]->iSoftLimitN);
        MotPtr[i]->SetSoftLimitP                (HSys.MotTable[iMot]->iSoftLimitP);
        MotPtr[i]->SetMotorType                 (HSys.MotTable[iMot]->i1P2P);
        MotPtr[i]->SetSensorType                (HSys.MotTable[iMot]->iSensorType);

        double dAcc=HSys.MotTable[iMot]->dAcc;
        double dDec=HSys.MotTable[iMot]->dDec;
        if(CardModel=="MN200")                                                  //Steven 20230616 : MN200的加減速單位是秒
        {
            if(dAcc>1)
                dAcc=HSys.MotTable[iMot]->dAcc/100.0;
            if(dDec>1)
                dDec=HSys.MotTable[iMot]->dDec/100.0;
        }
        else if(CardModel=="MC88X1")                                            //Nickliu 20240108 add MCXX8 Set Rate
        {
            dAcc=HSys.MotTable[iMot]->iRate;
            dDec=HSys.MotTable[iMot]->iRate;
        }

        MotPtr[i]->SetAcc                       (dAcc);
        MotPtr[i]->SetDec                       (dDec);
        MotPtr[i]->SetRange                     (HSys.MotTable[iMot]->iRange);
        MotPtr[i]->SetInitSpeed                 (HSys.MotTable[iMot]->iInitSpeed);
        MotPtr[i]->SetHomeOrder                 (HSys.MotTable[iMot]->HomeOrder );
        MotPtr[i]->SetServoAlarmOn              (HSys.MotTable[iMot]->iServoAlarmOn);
        MotPtr[i]->OriginRange                  =HSys.MotTable[iMot]->iRange;
        MotPtr[i]->OriginRate                   =HSys.MotTable[iMot]->iRate;
        MotPtr[i]->SimulateSpeed                =HSys.MotTable[iMot]->iSimulateSpeed;
        MotPtr[i]->bIsServoMotor                =HSys.MotTable[iMot]->iServoAlarmOn;
        MotPtr[i]->SetLimitLogic                ((HSys.MotTable[iMot]->iLimitLogic==1)?true:false);
        MotPtr[i]->SetIn1Logic                  ((HSys.MotTable[iMot]->iIn1Logic==1)?true:false);
        MotPtr[i]->bHomeFlag                    =false;
        if(bInitial)
            MotPtr[i]->InitMotor(iAdder);
    }

    for(int i=0; i<iTotalMotor; i++)                                                 // ben edit 20110721 //
    {
        if(MotPtr[i]==NULL)
            continue;

        MotPtr[i]->Tag=i;
        if(MotPtr[i]->GetEnable()==true)
            MotPtr[i]->Stop();
    }

    fAllMotorHome=false;
}
//==============================================================================
//  003
//  不須改變 _
//==============================================================================
void SYSTEM_MODULAR::InitialVMotorParameter()                                   // BEN EDIT 20110721 //
{
    for(int i=0; i<iTotalVMotor; i++)                                           // ben edit 20110721 //
    {
        if(VMotPtr[i]==NULL)
        continue;
        VMotPtr[i]->Tag=i;
    }
}
//==============================================================================
//  004
//  使用者依不同機器修改 _                                                      // NEED EDIT BY USER //
//==============================================================================
void __fastcall TDataModule1::SpecificSetupForMotorParameterExecute(TObject *Sender)
{
    for(int i=0; i<HSys.iTotalMotor; i++)
          if(HSys.MotPtr[i]!=NULL && HSys.MotPtr[i]->GetEnable()==true)
               HSys.MotPtr[i]->Stop();
     fAllMotorHome=false;
}
//---------------------------------------------------------------------------
void SYSTEM_MODULAR::LoadIoData()
{
    int iResult;
    AnsiString Str;

    if(FileExists(IoTablePath))
    {
        TStringList *StrList=new TStringList();

        mapIOTable.clear();
        IOTable.clear();
        try
        {
            StrList->LoadFromFile(IoTablePath);

            if(StrList->Count<=1)
            {
                Str.sprintf("File %s data is lose!", HSys.IoTablePath);
                ShowMyMessage(Str);
            }
            else
            {
                iResult=HSys.IoNo.SetIOTableNo(StrList->Strings[0]);
                if(iResult==HSys.IoNo.eioTotal)
                {
                    for(int i=1; i<StrList->Count; i++)
                    {
                        IOTable.push_back(new TIODATA(StrList->Strings[i]));
                        mapIOTableIter=mapIOTable.find(IOTable[i-1]->Alias);
                        Str=IOTable[i-1]->Alias;
                        if(Str!="")
                        {
                            if(mapIOTableIter==mapIOTable.end())
                            {
                                mapIOTable[IOTable[i-1]->Alias]=i-1;
                            }
                            else
                            {
                                Str.sprintf("IO %s alias is duplicated!", IOTable[i-1]->Alias);
                                ShowMyMessage(Str);
                            }
                        }
                    }
                }
                else
                {
                    Str.sprintf("File %s data is mistake! (%d)", IoTablePath, iResult);
                    ShowMyMessage(Str);
                }
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "SYSTEM_MODULAR::LoadIoData");
            Str.sprintf("File %s is opened by other software!", IoTablePath);
            ShowMyMessage(Str);
        }
        delete StrList;

    }
    else
    {
        Str.sprintf("File %s is not exist!", IoTablePath);
        ShowMyMessage(Str);
    }
}
//==============================================================================
//  005
//  使用者依不同機器修改 _                                                      // NEED EDIT BY USER //
//==============================================================================
void __fastcall TDataModule1::InitialCylinderNameExecute(TObject *Sender)       // 氣缸 //
{
    HSys.Cyn.C_LoaderEdgePush      .CylinderName="C_LoaderEdgePush"     ;
    HSys.Cyn.C_TrayY_Fixer         .CylinderName="C_TrayY_Fixer"        ;
    HSys.Cyn.C_Load_Middle         .CylinderName="C_Load_Middle"        ;
    HSys.Cyn.C_Load_Up             .CylinderName="C_Load_Up"            ;
    HSys.Cyn.C_TrayZ_Selector      .CylinderName="C_TrayZ_Selector"     ;
    HSys.Cyn.C_Loader_Stop         .CylinderName="C_Loader_Stop"        ;
    HSys.Cyn.C_Loader_CCDStop      .CylinderName="C_Loader_CCDStop"     ;
    HSys.Cyn.C_Empty1_Middle       .CylinderName="C_Empty1_Middle"      ;
    HSys.Cyn.C_Empty1_Up           .CylinderName="C_Empty1_Up"          ;
    HSys.Cyn.C_Empty1Z_Selector    .CylinderName="C_Empty1Z_Selector"   ;
    HSys.Cyn.C_Empty1_Stop1        .CylinderName="C_Empty1_Stop1"       ;
    HSys.Cyn.C_Empty1_Stop2        .CylinderName="C_Empty1_Stop2"       ;
    HSys.Cyn.C_Empty1_Stop3        .CylinderName="C_Empty1_Stop3"       ;
    HSys.Cyn.C_Empty1_Stop4        .CylinderName="C_Empty1_Stop4"       ;
    HSys.Cyn.C_Empty1_PushTray1    .CylinderName="C_Empty1_PushTray1"   ;
    HSys.Cyn.C_Empty1_PushTray2    .CylinderName="C_Empty1_PushTray2"   ;
    HSys.Cyn.C_Empty1_PushTray3    .CylinderName="C_Empty1_PushTray3"   ;
    HSys.Cyn.C_Empty1_PushTray4    .CylinderName="C_Empty1_PushTray4"   ;
    HSys.Cyn.C_Empty1_CCDStop      .CylinderName="C_Empty1_CCDStop"     ;
    HSys.Cyn.C_Empty2_Middle       .CylinderName="C_Empty2_Middle"      ;
    HSys.Cyn.C_Empty2_Up           .CylinderName="C_Empty2_Up"          ;
    HSys.Cyn.C_Empty2Z_Selector    .CylinderName="C_Empty2Z_Selector"   ;
    HSys.Cyn.C_Empty2EdgePush      .CylinderName="C_Empty2EdgePush"     ;
    HSys.Cyn.C_Empty2_Fixer        .CylinderName="C_Empty2_Fixer"       ;
    HSys.Cyn.C_Empty2_Stop         .CylinderName="C_Empty2_Stop"        ;
    HSys.Cyn.C_Empty2_EmptyStop    .CylinderName="C_Empty2_EmptyStop"   ;
    HSys.Cyn.C_Empty2_CCDStop      .CylinderName="C_Empty2_CCDStop"     ;
    HSys.Cyn.C_ShiftStageEdgePush  .CylinderName="C_ShiftStageEdgePush" ;
    HSys.Cyn.C_Auto1_InclinePush   .CylinderName="C_Auto1_InclinePush"  ;
    HSys.Cyn.C_Auto1_AntiDrop      .CylinderName="C_Auto1_AntiDrop"     ;
    HSys.Cyn.C_Auto2_InclinePush   .CylinderName="C_Auto2_InclinePush"  ;
    HSys.Cyn.C_Auto2_AntiDrop      .CylinderName="C_Auto2_AntiDrop"     ;
    HSys.Cyn.C_Auto3_InclinePush   .CylinderName="C_Auto3_InclinePush"  ;
    HSys.Cyn.C_Auto3_AntiDrop      .CylinderName="C_Auto3_AntiDrop"     ;
    HSys.Cyn.C_Auto4_InclinePush   .CylinderName="C_Auto4_InclinePush"  ;
    HSys.Cyn.C_Auto4_AntiDrop      .CylinderName="C_Auto4_AntiDrop"     ;
    HSys.Cyn.C_Auto5_InclinePush   .CylinderName="C_Auto5_InclinePush"  ;
    HSys.Cyn.C_Auto5_AntiDrop      .CylinderName="C_Auto5_AntiDrop"     ;
    HSys.Cyn.C_Auto6_InclinePush   .CylinderName="C_Auto6_InclinePush"  ;
    HSys.Cyn.C_Auto6_AntiDrop      .CylinderName="C_Auto6_AntiDrop"     ;
    HSys.Cyn.C_Auto7_InclinePush   .CylinderName="C_Auto7_InclinePush"  ;
    HSys.Cyn.C_Auto7_AntiDrop      .CylinderName="C_Auto7_AntiDrop"     ;
    HSys.Cyn.C_Auto8_InclinePush   .CylinderName="C_Auto8_InclinePush"  ;
    HSys.Cyn.C_Auto8_AntiDrop      .CylinderName="C_Auto8_AntiDrop"     ;
    HSys.Cyn.C_Auto9_InclinePush   .CylinderName="C_Auto9_InclinePush"  ;
    HSys.Cyn.C_Auto9_AntiDrop      .CylinderName="C_Auto9_AntiDrop"     ;
    HSys.Cyn.C_Auto10_InclinePush  .CylinderName="C_Auto10_InclinePush" ;
    HSys.Cyn.C_Auto10_AntiDrop     .CylinderName="C_Auto10_AntiDrop"    ;
    HSys.Cyn.C_Auto11_InclinePush  .CylinderName="C_Auto11_InclinePush" ;
    HSys.Cyn.C_Auto11_AntiDrop     .CylinderName="C_Auto11_AntiDrop"    ;
    HSys.Cyn.C_Auto12_InclinePush  .CylinderName="C_Auto12_InclinePush" ;
    HSys.Cyn.C_Auto12_AntiDrop     .CylinderName="C_Auto12_AntiDrop"    ;
    HSys.Cyn.C_Auto13_InclinePush  .CylinderName="C_Auto13_InclinePush" ;
    HSys.Cyn.C_Auto13_AntiDrop     .CylinderName="C_Auto13_AntiDrop"    ;
    HSys.Cyn.C_Auto14_InclinePush  .CylinderName="C_Auto14_InclinePush" ;
    HSys.Cyn.C_Auto14_AntiDrop     .CylinderName="C_Auto14_AntiDrop"    ;
    HSys.Cyn.C_Auto15_InclinePush  .CylinderName="C_Auto15_InclinePush" ;
    HSys.Cyn.C_Auto15_AntiDrop     .CylinderName="C_Auto15_AntiDrop"    ;
    HSys.Cyn.C_Auto16_InclinePush  .CylinderName="C_Auto16_InclinePush" ;
    HSys.Cyn.C_Auto16_AntiDrop     .CylinderName="C_Auto16_AntiDrop"    ;
    HSys.Cyn.C_Auto17_InclinePush  .CylinderName="C_Auto17_InclinePush" ;
    HSys.Cyn.C_Auto17_AntiDrop     .CylinderName="C_Auto17_AntiDrop"    ;
    HSys.Cyn.C_Auto18_InclinePush  .CylinderName="C_Auto18_InclinePush" ;
    HSys.Cyn.C_Auto18_AntiDrop     .CylinderName="C_Auto18_AntiDrop"    ;
    HSys.Cyn.C_Auto19_InclinePush  .CylinderName="C_Auto19_InclinePush" ;
    HSys.Cyn.C_Auto19_AntiDrop     .CylinderName="C_Auto19_AntiDrop"    ;
    HSys.Cyn.C_Auto20_InclinePush  .CylinderName="C_Auto20_InclinePush" ;
    HSys.Cyn.C_Auto20_AntiDrop     .CylinderName="C_Auto20_AntiDrop"    ;
    HSys.Cyn.C_Auto1_Stop          .CylinderName="C_Auto1_Stop"         ;
    HSys.Cyn.C_Auto2_Stop          .CylinderName="C_Auto2_Stop"         ;
    HSys.Cyn.C_Auto3_Stop          .CylinderName="C_Auto3_Stop"         ;
    HSys.Cyn.C_Auto4_Stop          .CylinderName="C_Auto4_Stop"         ;
    HSys.Cyn.C_Auto5_Stop          .CylinderName="C_Auto5_Stop"         ;
    HSys.Cyn.C_Auto6_Stop          .CylinderName="C_Auto6_Stop"         ;
    HSys.Cyn.C_Auto7_Stop          .CylinderName="C_Auto7_Stop"         ;
    HSys.Cyn.C_Auto8_Stop          .CylinderName="C_Auto8_Stop"         ;
    HSys.Cyn.C_Auto9_Stop          .CylinderName="C_Auto9_Stop"         ;
    HSys.Cyn.C_Auto10_Stop         .CylinderName="C_Auto10_Stop"        ;
    HSys.Cyn.C_Auto11_Stop         .CylinderName="C_Auto11_Stop"        ;
    HSys.Cyn.C_Auto12_Stop         .CylinderName="C_Auto12_Stop"        ;
    HSys.Cyn.C_Auto13_Stop         .CylinderName="C_Auto13_Stop"        ;
    HSys.Cyn.C_Auto14_Stop         .CylinderName="C_Auto14_Stop"        ;
    HSys.Cyn.C_Auto15_Stop         .CylinderName="C_Auto15_Stop"        ;
    HSys.Cyn.C_Auto16_Stop         .CylinderName="C_Auto16_Stop"        ;
    HSys.Cyn.C_Auto17_Stop         .CylinderName="C_Auto17_Stop"        ;
    HSys.Cyn.C_Auto18_Stop         .CylinderName="C_Auto18_Stop"        ;
    HSys.Cyn.C_Auto19_Stop         .CylinderName="C_Auto19_Stop"        ;
    HSys.Cyn.C_Auto20_Stop         .CylinderName="C_Auto20_Stop"        ;
    HSys.Cyn.C_Auto1_Zup           .CylinderName="C_Auto1_Zup"          ;
    HSys.Cyn.C_Auto2_Zup           .CylinderName="C_Auto2_Zup"          ;
    HSys.Cyn.C_Auto3_Zup           .CylinderName="C_Auto3_Zup"          ;
    HSys.Cyn.C_Auto4_Zup           .CylinderName="C_Auto4_Zup"          ;
    HSys.Cyn.C_Auto5_Zup           .CylinderName="C_Auto5_Zup"          ;
    HSys.Cyn.C_Auto6_Zup           .CylinderName="C_Auto6_Zup"          ;
    HSys.Cyn.C_Auto7_Zup           .CylinderName="C_Auto7_Zup"          ;
    HSys.Cyn.C_Auto8_Zup           .CylinderName="C_Auto8_Zup"          ;
    HSys.Cyn.C_Auto9_Zup           .CylinderName="C_Auto9_Zup"          ;
    HSys.Cyn.C_Auto10_Zup          .CylinderName="C_Auto10_Zup"         ;
    HSys.Cyn.C_Auto11_Zup          .CylinderName="C_Auto11_Zup"         ;
    HSys.Cyn.C_Auto12_Zup          .CylinderName="C_Auto12_Zup"         ;
    HSys.Cyn.C_Auto13_Zup          .CylinderName="C_Auto13_Zup"         ;
    HSys.Cyn.C_Auto14_Zup          .CylinderName="C_Auto14_Zup"         ;
    HSys.Cyn.C_Auto15_Zup          .CylinderName="C_Auto15_Zup"         ;
    HSys.Cyn.C_Auto16_Zup          .CylinderName="C_Auto16_Zup"         ;
    HSys.Cyn.C_Auto17_Zup          .CylinderName="C_Auto17_Zup"         ;
    HSys.Cyn.C_Auto18_Zup          .CylinderName="C_Auto18_Zup"         ;
    HSys.Cyn.C_Auto19_Zup          .CylinderName="C_Auto19_Zup"         ;
    HSys.Cyn.C_Auto20_Zup          .CylinderName="C_Auto20_Zup"         ;
    HSys.Cyn.C_AutoRow1_Stop       .CylinderName="C_AutoRow1_Stop"      ;
    HSys.Cyn.C_AutoRow2_Stop       .CylinderName="C_AutoRow2_Stop"      ;
    HSys.Cyn.C_AutoRow3_Stop       .CylinderName="C_AutoRow3_Stop"      ;
    HSys.Cyn.C_AutoRow4_Stop       .CylinderName="C_AutoRow4_Stop"      ;
    HSys.Cyn.C_AutoRow1_PushTray   .CylinderName="C_AutoRow1_PushTray"  ;
    HSys.Cyn.C_AutoRow2_PushTray   .CylinderName="C_AutoRow2_PushTray"  ;
    HSys.Cyn.C_AutoRow3_PushTray   .CylinderName="C_AutoRow3_PushTray"  ;
    HSys.Cyn.C_AutoRow4_PushTray   .CylinderName="C_AutoRow4_PushTray"  ;
    HSys.Cyn.C_StoreArm_EdgeFixer  .CylinderName="C_StoreArm_EdgeFixer" ;
    HSys.Cyn.C_MGZ1_CatchTray      .CylinderName="C_MGZ1_CatchTray"     ;
    HSys.Cyn.C_MGZ1_EdgeFixer      .CylinderName="C_MGZ1_EdgeFixer"     ;
    HSys.Cyn.C_MGZ1_SidePush_Up    .CylinderName="C_MGZ1_SidePush_Up"   ;
    HSys.Cyn.C_MGZ1_SidePush       .CylinderName="C_MGZ1_SidePush"      ;
    HSys.Cyn.C_MGZ1_LayerTrayPush  .CylinderName="C_MGZ1_LayerTrayPush" ;
    HSys.Cyn.C_MGZ1_TrayPush       .CylinderName="C_MGZ1_TrayPush"      ;
    HSys.Cyn.C_MGZ1_TopFixTray     .CylinderName="C_MGZ1_TopFixTray"    ;
    HSys.Cyn.C_MGZ2_CatchTray      .CylinderName="C_MGZ2_CatchTray"     ;
    HSys.Cyn.C_MGZ2_EdgeFixer      .CylinderName="C_MGZ2_EdgeFixer"     ;
    HSys.Cyn.C_MGZ2_SidePush_Up    .CylinderName="C_MGZ2_SidePush_Up"   ;
    HSys.Cyn.C_MGZ2_SidePush       .CylinderName="C_MGZ2_SidePush"      ;
    HSys.Cyn.C_MGZ2_LayerTrayPush  .CylinderName="C_MGZ2_LayerTrayPush" ;
    HSys.Cyn.C_MGZ2_TrayPush       .CylinderName="C_MGZ2_TrayPush"      ;
    HSys.Cyn.C_MGZ2_TopFixTray     .CylinderName="C_MGZ2_TopFixTray"    ;
    HSys.Cyn.C_MGZ3_CatchTray      .CylinderName="C_MGZ3_CatchTray"     ;
    HSys.Cyn.C_MGZ3_EdgeFixer      .CylinderName="C_MGZ3_EdgeFixer"     ;
    HSys.Cyn.C_MGZ3_SidePush_Up    .CylinderName="C_MGZ3_SidePush_Up"   ;
    HSys.Cyn.C_MGZ3_SidePush       .CylinderName="C_MGZ3_SidePush"      ;
    HSys.Cyn.C_MGZ3_LayerTrayPush  .CylinderName="C_MGZ3_LayerTrayPush" ;
    HSys.Cyn.C_MGZ3_TrayPush       .CylinderName="C_MGZ3_TrayPush"      ;
    HSys.Cyn.C_MGZ3_TopFixTray     .CylinderName="C_MGZ3_TopFixTray"    ;
    HSys.Cyn.C_SortArm2_UpDown     .CylinderName="C_SortArm2_UpDown"    ;
    HSys.Cyn.C_CatchTray_Fix       .CylinderName="C_CatchTray_Fix"      ;

    HSys.Cyn.C_AutoTrolly1_Fixer   .CylinderName="C_AutoTrolly1_Fixer"  ;
    HSys.Cyn.C_AutoTrolly2_Fixer   .CylinderName="C_AutoTrolly2_Fixer"  ;
    HSys.Cyn.C_AutoTrolly3_Fixer   .CylinderName="C_AutoTrolly3_Fixer"  ;
    HSys.Cyn.C_AutoTrolly4_Fixer   .CylinderName="C_AutoTrolly4_Fixer"  ;
    HSys.Cyn.C_AutoTrolly1_Stop    .CylinderName="C_AutoTrolly1_Stop"   ;
    HSys.Cyn.C_AutoTrolly2_Stop    .CylinderName="C_AutoTrolly2_Stop"   ;
    HSys.Cyn.C_AutoTrolly3_Stop    .CylinderName="C_AutoTrolly3_Stop"   ;
    HSys.Cyn.C_AutoTrolly4_Stop    .CylinderName="C_AutoTrolly4_Stop"   ;

    HSys.Cyn.C_LoaderEdgePush      .FlushPanelName="palWorkLoader"      ;
    HSys.Cyn.C_TrayY_Fixer         .FlushPanelName="palWorkLoader"      ;
    HSys.Cyn.C_Load_Middle         .FlushPanelName="palLoadCar"         ;
    HSys.Cyn.C_Load_Up             .FlushPanelName="palLoadCar"         ;
    HSys.Cyn.C_TrayZ_Selector      .FlushPanelName="palLoadCar"         ;
    HSys.Cyn.C_Loader_Stop         .FlushPanelName="palWorkLoader"      ;
    HSys.Cyn.C_Loader_CCDStop      .FlushPanelName="palLoadCar"         ;
    HSys.Cyn.C_Empty1_Middle       .FlushPanelName="palEmpty1Car"       ;
    HSys.Cyn.C_Empty1_Up           .FlushPanelName="palEmpty1Car"       ;
    HSys.Cyn.C_Empty1Z_Selector    .FlushPanelName="palEmpty1Car"       ;
    HSys.Cyn.C_Empty1_Stop1        .FlushPanelName="palWorkEmpty1"      ;
    HSys.Cyn.C_Empty1_Stop2        .FlushPanelName="palWorkEmpty2"      ;
    HSys.Cyn.C_Empty1_Stop3        .FlushPanelName="palWorkEmpty3"      ;
    HSys.Cyn.C_Empty1_Stop4        .FlushPanelName="palWorkEmpty4"      ;
    HSys.Cyn.C_Empty1_PushTray1    .FlushPanelName="palWorkEmpty1"      ;
    HSys.Cyn.C_Empty1_PushTray2    .FlushPanelName="palWorkEmpty2"      ;
    HSys.Cyn.C_Empty1_PushTray3    .FlushPanelName="palWorkEmpty3"      ;
    HSys.Cyn.C_Empty1_PushTray4    .FlushPanelName="palWorkEmpty4"      ;
    HSys.Cyn.C_Empty1_CCDStop      .FlushPanelName="palEmpty1Car"       ;
    HSys.Cyn.C_Empty2_Middle       .FlushPanelName="palEmpty2Car"       ;
    HSys.Cyn.C_Empty2_Up           .FlushPanelName="palEmpty2Car"       ;
    HSys.Cyn.C_Empty2Z_Selector    .FlushPanelName="palEmpty2Car"       ;
    HSys.Cyn.C_Empty2EdgePush      .FlushPanelName="palMagLD"           ;
    HSys.Cyn.C_Empty2_Fixer        .FlushPanelName="palMagLD"           ;
    HSys.Cyn.C_Empty2_Stop         .FlushPanelName="palMagLD"           ;
    HSys.Cyn.C_Empty2_EmptyStop    .FlushPanelName="palMagEmptyTray"    ;
    HSys.Cyn.C_Empty2_CCDStop      .FlushPanelName="palEmpty2Car"       ;
    HSys.Cyn.C_ShiftStageEdgePush  .FlushPanelName="palShiftStage"      ;
    HSys.Cyn.C_Auto1_InclinePush   .FlushPanelName="palAuto1"           ;
    HSys.Cyn.C_Auto1_AntiDrop      .FlushPanelName="palAuto1"           ;
    HSys.Cyn.C_Auto2_InclinePush   .FlushPanelName="palAuto2"           ;
    HSys.Cyn.C_Auto2_AntiDrop      .FlushPanelName="palAuto2"           ;
    HSys.Cyn.C_Auto3_InclinePush   .FlushPanelName="palAuto3"           ;
    HSys.Cyn.C_Auto3_AntiDrop      .FlushPanelName="palAuto3"           ;
    HSys.Cyn.C_Auto4_InclinePush   .FlushPanelName="palAuto4"           ;
    HSys.Cyn.C_Auto4_AntiDrop      .FlushPanelName="palAuto4"           ;
    HSys.Cyn.C_Auto5_InclinePush   .FlushPanelName="palAuto5"           ;
    HSys.Cyn.C_Auto5_AntiDrop      .FlushPanelName="palAuto5"           ;
    HSys.Cyn.C_Auto6_InclinePush   .FlushPanelName="palAuto6"           ;
    HSys.Cyn.C_Auto6_AntiDrop      .FlushPanelName="palAuto6"           ;
    HSys.Cyn.C_Auto7_InclinePush   .FlushPanelName="palAuto7"           ;
    HSys.Cyn.C_Auto7_AntiDrop      .FlushPanelName="palAuto7"           ;
    HSys.Cyn.C_Auto8_InclinePush   .FlushPanelName="palAuto8"           ;
    HSys.Cyn.C_Auto8_AntiDrop      .FlushPanelName="palAuto8"           ;
    HSys.Cyn.C_Auto9_InclinePush   .FlushPanelName="palAuto9"           ;
    HSys.Cyn.C_Auto9_AntiDrop      .FlushPanelName="palAuto9"           ;
    HSys.Cyn.C_Auto10_InclinePush  .FlushPanelName="palAuto10"          ;
    HSys.Cyn.C_Auto10_AntiDrop     .FlushPanelName="palAuto10"          ;
    HSys.Cyn.C_Auto11_InclinePush  .FlushPanelName="palAuto11"          ;
    HSys.Cyn.C_Auto11_AntiDrop     .FlushPanelName="palAuto11"          ;
    HSys.Cyn.C_Auto12_InclinePush  .FlushPanelName="palAuto12"          ;
    HSys.Cyn.C_Auto12_AntiDrop     .FlushPanelName="palAuto12"          ;
    HSys.Cyn.C_Auto13_InclinePush  .FlushPanelName="palAuto13"          ;
    HSys.Cyn.C_Auto13_AntiDrop     .FlushPanelName="palAuto13"          ;
    HSys.Cyn.C_Auto14_InclinePush  .FlushPanelName="palAuto14"          ;
    HSys.Cyn.C_Auto14_AntiDrop     .FlushPanelName="palAuto14"          ;
    HSys.Cyn.C_Auto15_InclinePush  .FlushPanelName="palAuto15"          ;
    HSys.Cyn.C_Auto15_AntiDrop     .FlushPanelName="palAuto15"          ;
    HSys.Cyn.C_Auto16_InclinePush  .FlushPanelName="palAuto16"          ;
    HSys.Cyn.C_Auto16_AntiDrop     .FlushPanelName="palAuto16"          ;
    HSys.Cyn.C_Auto17_InclinePush  .FlushPanelName="palAuto17"          ;
    HSys.Cyn.C_Auto17_AntiDrop     .FlushPanelName="palAuto17"          ;
    HSys.Cyn.C_Auto18_InclinePush  .FlushPanelName="palAuto18"          ;
    HSys.Cyn.C_Auto18_AntiDrop     .FlushPanelName="palAuto18"          ;
    HSys.Cyn.C_Auto19_InclinePush  .FlushPanelName="palAuto19"          ;
    HSys.Cyn.C_Auto19_AntiDrop     .FlushPanelName="palAuto19"          ;
    HSys.Cyn.C_Auto20_InclinePush  .FlushPanelName="palAuto20"          ;
    HSys.Cyn.C_Auto20_AntiDrop     .FlushPanelName="palAuto20"          ;
    HSys.Cyn.C_Auto1_Stop          .FlushPanelName="palAuto1"           ;
    HSys.Cyn.C_Auto2_Stop          .FlushPanelName="palAuto2"           ;
    HSys.Cyn.C_Auto3_Stop          .FlushPanelName="palAuto3"           ;
    HSys.Cyn.C_Auto4_Stop          .FlushPanelName="palAuto4"           ;
    HSys.Cyn.C_Auto5_Stop          .FlushPanelName="palAuto5"           ;
    HSys.Cyn.C_Auto6_Stop          .FlushPanelName="palAuto6"           ;
    HSys.Cyn.C_Auto7_Stop          .FlushPanelName="palAuto7"           ;
    HSys.Cyn.C_Auto8_Stop          .FlushPanelName="palAuto8"           ;
    HSys.Cyn.C_Auto9_Stop          .FlushPanelName="palAuto9"           ;
    HSys.Cyn.C_Auto10_Stop         .FlushPanelName="palAuto10"          ;
    HSys.Cyn.C_Auto11_Stop         .FlushPanelName="palAuto11"          ;
    HSys.Cyn.C_Auto12_Stop         .FlushPanelName="palAuto12"          ;
    HSys.Cyn.C_Auto13_Stop         .FlushPanelName="palAuto13"          ;
    HSys.Cyn.C_Auto14_Stop         .FlushPanelName="palAuto14"          ;
    HSys.Cyn.C_Auto15_Stop         .FlushPanelName="palAuto15"          ;
    HSys.Cyn.C_Auto16_Stop         .FlushPanelName="palAuto16"          ;
    HSys.Cyn.C_Auto17_Stop         .FlushPanelName="palAuto17"          ;
    HSys.Cyn.C_Auto18_Stop         .FlushPanelName="palAuto18"          ;
    HSys.Cyn.C_Auto19_Stop         .FlushPanelName="palAuto19"          ;
    HSys.Cyn.C_Auto20_Stop         .FlushPanelName="palAuto20"          ;
    HSys.Cyn.C_Auto1_Zup           .FlushPanelName="palAuto1"           ;
    HSys.Cyn.C_Auto2_Zup           .FlushPanelName="palAuto2"           ;
    HSys.Cyn.C_Auto3_Zup           .FlushPanelName="palAuto3"           ;
    HSys.Cyn.C_Auto4_Zup           .FlushPanelName="palAuto4"           ;
    HSys.Cyn.C_Auto5_Zup           .FlushPanelName="palAuto5"           ;
    HSys.Cyn.C_Auto6_Zup           .FlushPanelName="palAuto6"           ;
    HSys.Cyn.C_Auto7_Zup           .FlushPanelName="palAuto7"           ;
    HSys.Cyn.C_Auto8_Zup           .FlushPanelName="palAuto8"           ;
    HSys.Cyn.C_Auto9_Zup           .FlushPanelName="palAuto9"           ;
    HSys.Cyn.C_Auto10_Zup          .FlushPanelName="palAuto10"          ;
    HSys.Cyn.C_Auto11_Zup          .FlushPanelName="palAuto11"          ;
    HSys.Cyn.C_Auto12_Zup          .FlushPanelName="palAuto12"          ;
    HSys.Cyn.C_Auto13_Zup          .FlushPanelName="palAuto13"          ;
    HSys.Cyn.C_Auto14_Zup          .FlushPanelName="palAuto14"          ;
    HSys.Cyn.C_Auto15_Zup          .FlushPanelName="palAuto15"          ;
    HSys.Cyn.C_Auto16_Zup          .FlushPanelName="palAuto16"          ;
    HSys.Cyn.C_Auto17_Zup          .FlushPanelName="palAuto17"          ;
    HSys.Cyn.C_Auto18_Zup          .FlushPanelName="palAuto18"          ;
    HSys.Cyn.C_Auto19_Zup          .FlushPanelName="palAuto19"          ;
    HSys.Cyn.C_Auto20_Zup          .FlushPanelName="palAuto20"          ;
    HSys.Cyn.C_AutoRow1_Stop       .FlushPanelName="palWorkEmpty1"      ;
    HSys.Cyn.C_AutoRow2_Stop       .FlushPanelName="palWorkEmpty2"      ;
    HSys.Cyn.C_AutoRow3_Stop       .FlushPanelName="palWorkEmpty3"      ;
    HSys.Cyn.C_AutoRow4_Stop       .FlushPanelName="palWorkEmpty4"      ;
    HSys.Cyn.C_AutoRow1_PushTray   .FlushPanelName="palWorkEmpty1"      ;
    HSys.Cyn.C_AutoRow2_PushTray   .FlushPanelName="palWorkEmpty2"      ;
    HSys.Cyn.C_AutoRow3_PushTray   .FlushPanelName="palWorkEmpty3"      ;
    HSys.Cyn.C_AutoRow4_PushTray   .FlushPanelName="palWorkEmpty4"      ;
    HSys.Cyn.C_StoreArm_EdgeFixer  .FlushPanelName="palTrayArm"         ;
    HSys.Cyn.C_MGZ1_CatchTray      .FlushPanelName="palMag1"            ;
    HSys.Cyn.C_MGZ1_EdgeFixer      .FlushPanelName="palMag1"            ;
    HSys.Cyn.C_MGZ1_SidePush_Up    .FlushPanelName="palMag1"            ;
    HSys.Cyn.C_MGZ1_SidePush       .FlushPanelName="palMag1"            ;
    HSys.Cyn.C_MGZ1_LayerTrayPush  .FlushPanelName="palMag1"            ;
    HSys.Cyn.C_MGZ1_TrayPush       .FlushPanelName="palMag1"            ;
    HSys.Cyn.C_MGZ1_TopFixTray     .FlushPanelName="palMag1"            ;
    HSys.Cyn.C_MGZ2_CatchTray      .FlushPanelName="palMag2"            ;
    HSys.Cyn.C_MGZ2_EdgeFixer      .FlushPanelName="palMag2"            ;
    HSys.Cyn.C_MGZ2_SidePush_Up    .FlushPanelName="palMag2"            ;
    HSys.Cyn.C_MGZ2_SidePush       .FlushPanelName="palMag2"            ;
    HSys.Cyn.C_MGZ2_LayerTrayPush  .FlushPanelName="palMag2"            ;
    HSys.Cyn.C_MGZ2_TrayPush       .FlushPanelName="palMag2"            ;
    HSys.Cyn.C_MGZ2_TopFixTray     .FlushPanelName="palMag2"            ;
    HSys.Cyn.C_MGZ3_CatchTray      .FlushPanelName="palMag3"            ;
    HSys.Cyn.C_MGZ3_EdgeFixer      .FlushPanelName="palMag3"            ;
    HSys.Cyn.C_MGZ3_SidePush_Up    .FlushPanelName="palMag3"            ;
    HSys.Cyn.C_MGZ3_SidePush       .FlushPanelName="palMag3"            ;
    HSys.Cyn.C_MGZ3_LayerTrayPush  .FlushPanelName="palMag3"            ;
    HSys.Cyn.C_MGZ3_TrayPush       .FlushPanelName="palMag3"            ;
    HSys.Cyn.C_MGZ3_TopFixTray     .FlushPanelName="palMag3"            ;
    HSys.Cyn.C_SortArm2_UpDown     .FlushPanelName="palTrayArm"         ;
    HSys.Cyn.C_CatchTray_Fix       .FlushPanelName="palMagTrayArm"      ;
}
//==============================================================================
//  006
//  不須改變 _
//==============================================================================
void SYSTEM_MODULAR::LoadCylinderParameterFromDataBase()                        // ben ok //
{
    AnsiString S;
    AnsiString str;
    AnsiString sPort, sBit, sCard;
    CynPtr=(TMyCylinder *)&Cyn;
    iTotalCylinder=sizeof(Cyn)/sizeof(TMyCylinder);

    bool bHasCyn, bHasOnSensor, bHasOffSensor;
    int iCy=0, iCyOn=0, iCyOff=0;

    for(int i=0; i<iTotalCylinder; i++)
    {
        bHasCyn=false;
        bHasOnSensor=false;
        bHasOffSensor=false;
        CynPtr[i].Enable=false;
        if(CynPtr[i].CylinderName=="")
            continue;

        CynPtr[i].OnSensor.Name             =CynPtr[i].CylinderName+"_On";
        CynPtr[i].OffSensor.Name            =CynPtr[i].CylinderName+"_Off";
        CynPtr[i].ErrorName[eOffNotOnErr ]  =CynPtr[i].CylinderName+"_OffNotOnErr";
        CynPtr[i].ErrorName[eOffNotOffErr]  =CynPtr[i].CylinderName+"_OffNotOffErr";
        CynPtr[i].ErrorName[eOffIsOnErr  ]  =CynPtr[i].CylinderName+"_OffIsOnErr";
        CynPtr[i].ErrorName[eOnNotOnErr  ]  =CynPtr[i].CylinderName+"_OnNotOnErr";
        CynPtr[i].ErrorName[eOnNotOffErr ]  =CynPtr[i].CylinderName+"_OnNotOffErr";
        CynPtr[i].ErrorName[eOnIsOnErr   ]  =CynPtr[i].CylinderName+"_OnIsOnErr";

        HSys.mapIOTableIter=HSys.mapIOTable.find(CynPtr[i].CylinderName);
        if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
        {
            bHasCyn=true;
            iCy=atoi(HSys.mapIOTable[CynPtr[i].CylinderName].c_str());
        }
        else
        {
            bHasCyn=false;
            str.sprintf("Can not find cylinder %s", CynPtr[i].CylinderName);
        }

        HSys.mapIOTableIter=HSys.mapIOTable.find(CynPtr[i].OnSensor.Name);
        if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
        {
            bHasOnSensor=true;
            iCyOn=atoi(HSys.mapIOTable[CynPtr[i].OnSensor.Name].c_str());
        }
        else
        {
            bHasOnSensor=false;
            str.sprintf("Can not find cylinder on %s", CynPtr[i].OnSensor.Name);
        }

        HSys.mapIOTableIter=HSys.mapIOTable.find(CynPtr[i].OffSensor.Name);
        if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
        {
            bHasOffSensor=true;
            iCyOff=atoi(HSys.mapIOTable[CynPtr[i].OffSensor.Name].c_str());
        }
        else
        {
            bHasOffSensor=false;
            str.sprintf("Can not find cylinder off %s", CynPtr[i].OffSensor.Name);
        }

        if(CynPtr[i].Switch.Output!=NULL)
            delete CynPtr[i].Switch.Output;

        CynPtr[i].Switch.Output=new TMyMN200_IO;
        if(bHasCyn)
        {
            //------------ Cylinder output port-------------------
            CynPtr[i].Switch.Output->ISABase    =HSys.IOTable[iCy]->iISABase;
            CynPtr[i].Switch.EnableAtDataBase   =HSys.IOTable[iCy]->iEnable;
            CynPtr[i].Switch.Output->SetHint    (HSys.IOTable[iCy]->IOPos);

            if(CynPtr[i].Switch.Output->ISABase==eMotionNet)
                CynPtr[i].Switch.Output->SetPortInformation(HSys.IOTable[iCy]->iLane, HSys.IOTable[iCy]->sIP, HSys.IOTable[iCy]->iPort, HSys.IOTable[iCy]->iBit);
            else
                CynPtr[i].Switch.Output->SetPortInformation(AnsiString(""), HSys.IOTable[iCy]->iPort, HSys.IOTable[iCy]->iBit);

            CynPtr[i].Switch.Card       =CynPtr[i].Switch.Output->GetCard();
            CynPtr[i].Switch.Port       =CynPtr[i].Switch.Output->GetPort();
            CynPtr[i].Switch.Bit        =CynPtr[i].Switch.Output->GetBit();
            CynPtr[i].Switch.Type       =HSys.IOTable[iCy]->iInType;

            #ifdef SOFT_SIMULATE
                CynPtr[i].Switch.Enable=false;
            #else
                if(CynPtr[i].Switch.Output->ISABase==eMotionNet &&
                   CynPtr[i].Switch.Output->GetLane()==0 &&
                   CynPtr[i].Switch.Output->GetIP()==0   &&
                   CynPtr[i].Switch.Output->GetPort()==0 &&
                   CynPtr[i].Switch.Output->GetBit()==0)
                {
                    CynPtr[i].Switch.Enable=false;
                }
                else if(CynPtr[i].Switch.Output->ISABase!=eMotionNet &&
                        CynPtr[i].Switch.Output->GetPort()==0 &&
                        CynPtr[i].Switch.Output->GetBit()==0)
                {
                    CynPtr[i].Switch.Enable=false;
                }
                else
                {
                    CynPtr[i].Switch.Enable=HSys.IOTable[iCy]->iEnable;
                }
            #endif

        }
        else
        {
            CynPtr[i].Enable=false;
        }

        //------------ Cylinder on sensor port-------------------
        CynPtr[i].OnSensor.Input=new TMyMN200_IO;
        if(bHasOnSensor)
        {
            CynPtr[i].OnSensor.Input->ISABase    =HSys.IOTable[iCyOn]->iISABase;
            CynPtr[i].OnSensor.EnableAtDataBase   =HSys.IOTable[iCyOn]->iEnable;
            CynPtr[i].OnSensor.Input->SetHint    (HSys.IOTable[iCyOn]->IOPos);

            if(CynPtr[i].OnSensor.Input->ISABase==eMotionNet)
                CynPtr[i].OnSensor.Input->SetPortInformation(HSys.IOTable[iCyOn]->iLane, HSys.IOTable[iCyOn]->sIP, HSys.IOTable[iCyOn]->iPort, HSys.IOTable[iCyOn]->iBit);
            else
                CynPtr[i].OnSensor.Input->SetPortInformation(AnsiString(""), HSys.IOTable[iCyOn]->iPort, HSys.IOTable[iCyOn]->iBit);

            CynPtr[i].OnSensor.Card       =CynPtr[i].OnSensor.Input->GetCard();
            CynPtr[i].OnSensor.Port       =CynPtr[i].OnSensor.Input->GetPort();
            CynPtr[i].OnSensor.Bit        =CynPtr[i].OnSensor.Input->GetBit();
            CynPtr[i].OnSensor.Type       =HSys.IOTable[iCyOn]->iInType;

            #ifdef SOFT_SIMULATE
                CynPtr[i].OnSensor.Enable=false;
            #else
                if(CynPtr[i].OnSensor.Input->ISABase==eMotionNet &&
                   CynPtr[i].OnSensor.Input->GetLane()==0 &&
                   CynPtr[i].OnSensor.Input->GetIP()==0   &&
                   CynPtr[i].OnSensor.Input->GetPort()==0 &&
                   CynPtr[i].OnSensor.Input->GetBit()==0)
                {
                    CynPtr[i].OnSensor.Enable=false;
                }
                else if(CynPtr[i].OnSensor.Input->ISABase!=eMotionNet &&
                        CynPtr[i].OnSensor.Input->GetPort()==0 &&
                        CynPtr[i].OnSensor.Input->GetBit()==0)
                {
                    CynPtr[i].OnSensor.Enable=false;
                }
                else
                {
                    CynPtr[i].OnSensor.Enable=HSys.IOTable[iCyOn]->iEnable;
                }
            #endif
        }

        //------------ Cylinder off sensor port-------------------
        CynPtr[i].OffSensor.Input=new TMyMN200_IO;
        if(bHasOffSensor)
        {
            CynPtr[i].OffSensor.Input->ISABase    =HSys.IOTable[iCyOff]->iISABase;
            CynPtr[i].OffSensor.EnableAtDataBase   =HSys.IOTable[iCyOff]->iEnable;
            CynPtr[i].OffSensor.Input->SetHint    (HSys.IOTable[iCyOff]->IOPos);

            if(CynPtr[i].OffSensor.Input->ISABase==eMotionNet)
                CynPtr[i].OffSensor.Input->SetPortInformation(HSys.IOTable[iCyOff]->iLane, HSys.IOTable[iCyOff]->sIP, HSys.IOTable[iCyOff]->iPort, HSys.IOTable[iCyOff]->iBit);
            else
                CynPtr[i].OffSensor.Input->SetPortInformation(AnsiString(""), HSys.IOTable[iCyOff]->iPort, HSys.IOTable[iCyOff]->iBit);

            CynPtr[i].OffSensor.Card       =CynPtr[i].OffSensor.Input->GetCard();
            CynPtr[i].OffSensor.Port       =CynPtr[i].OffSensor.Input->GetPort();
            CynPtr[i].OffSensor.Bit        =CynPtr[i].OffSensor.Input->GetBit();
            CynPtr[i].OffSensor.Type       =HSys.IOTable[iCyOff]->iInType;

            #ifdef SOFT_SIMULATE
                CynPtr[i].OffSensor.Enable=false;
            #else
                if(CynPtr[i].OffSensor.Input->ISABase==eMotionNet &&
                   CynPtr[i].OffSensor.Input->GetLane()==0 &&
                   CynPtr[i].OffSensor.Input->GetIP()==0   &&
                   CynPtr[i].OffSensor.Input->GetPort()==0 &&
                   CynPtr[i].OffSensor.Input->GetBit()==0)
                {
                    CynPtr[i].OffSensor.Enable=false;
                }
                else if(CynPtr[i].OffSensor.Input->ISABase!=eMotionNet &&
                        CynPtr[i].OffSensor.Input->GetPort()==0 &&
                        CynPtr[i].OffSensor.Input->GetBit()==0)
                {
                    CynPtr[i].OffSensor.Enable=false;
                }
                else
                {
                    CynPtr[i].OffSensor.Enable=HSys.IOTable[iCyOff]->iEnable;
                }
            #endif
        }

        S.printf("%d%03d%1d", eCynAlarm, i, 3);
        CynPtr[i].OnAlarmCode  =S.ToIntDef(0);
        S.printf("%d%03d%1d", eCynAlarm, i, 0);
        CynPtr[i].OffAlarmCode =S.ToIntDef(0);

        if(bHasCyn)
        {
            CynPtr[i].OnAlarmTime  =HSys.IOTable[iCy]->iOnAlarmTime;
            CynPtr[i].OffAlarmTime =HSys.IOTable[iCy]->iOffAlarmTime;
            CynPtr[i].OnDelayTime  =HSys.IOTable[iCy]->iOnDelayTime;
            CynPtr[i].OffDelayTime =HSys.IOTable[iCy]->iOffDelayTime;
        }

        if(CynPtr[i].OnAlarmTime<=0)      CynPtr[i].OnAlarmTime=10;
        if(CynPtr[i].OffAlarmTime<=0)     CynPtr[i].OffAlarmTime=10;
        if(CynPtr[i].OnDelayTime<=0)      CynPtr[i].OnDelayTime=1;
        if(CynPtr[i].OffDelayTime<=0)     CynPtr[i].OffDelayTime=1;

        if(HSys.IOTable[iCy]->iEnable)
            CynPtr[i].Enable=true;
        else
            CynPtr[i].Enable=false;
        CynPtr[i].EnableAtDataBase=CynPtr[i].Enable;

        #ifdef SOFT_SIMULATE
            CynPtr[i].Enable=false;
            CynPtr[i].Switch.Enable=false;                                      //20111214 Daver add
            CynPtr[i].OnSensor.Enable=false;                                    // ben add 20110726 //
            CynPtr[i].OffSensor.Enable=false;                                   // ben add 20110726 //
            CynPtr[i].OnDelayTime  =0;
            CynPtr[i].OffDelayTime =0;
        #endif
    }
    for(int i=0; i<iTotalCylinder; i++)
        CynPtr[i].Tag=i;
}
//==============================================================================
//  007
//  使用者依不同機器修改 _                                                      // NEED EDIT BY USER //
//==============================================================================
void __fastcall TDataModule1::SpecificSetupForCylinderParameterExecute(
      TObject *Sender)
{
    #ifdef SOFT_SIMULATE
        return;
    #else
        ReadLastDataIni();
        for(int i=0; i<HSys.iTotalCylinder; i++)
        {
            if(HSys.CynPtr[i].Enable)
            {
                if(HSys.LastSet.bCynStatus[i])
                {
                    HSys.CynPtr[i].Switch.On();
                }
                else
                {
                    HSys.CynPtr[i].Switch.Off();
                }
            }
        }
    #endif
}
//==============================================================================
//  008
//  使用者依不同機器修改 _                                                      // NEED EDIT BY USER //
//==============================================================================
void __fastcall TDataModule1::InitialSensorNameExecute(TObject *Sender)         // 輸入 //
{
    HSys.Sen.SnFKPowerOff         .Name="SnFKPowerOff"         ;
    HSys.Sen.SnFKPowerOn          .Name="SnFKPowerOn"          ;
    HSys.Sen.SnFKReset            .Name="SnFKReset"            ;
    HSys.Sen.SnFKPause            .Name="SnFKPause"            ;
    HSys.Sen.SnFKHome             .Name="SnFKHome"             ;
    HSys.Sen.SnFKStart            .Name="SnFKStart"            ;
    HSys.Sen.SnFKOneCycle         .Name="SnFKOneCycle"         ;
    HSys.Sen.SnFKRetry            .Name="SnFKRetry"            ;
    HSys.Sen.SnFKSkip             .Name="SnFKSkip"             ;
    HSys.Sen.SnFKCleanOut         .Name="SnFKCleanOut"         ;
    HSys.Sen.SnFKTrayFeed         .Name="SnFKTrayFeed"         ;
    HSys.Sen.SnFKTrayEnd          .Name="SnFKTrayEnd"          ;
    HSys.Sen.SnFKAlarmReset       .Name="SnFKAlarmReset"       ;
    HSys.Sen.SnFKCoverOpen        .Name="SnFKCoverOpen"        ;
    HSys.Sen.SnRKPowerOff         .Name="SnRKPowerOff"         ;
    HSys.Sen.SnRKPowerOn          .Name="SnRKPowerOn"          ;
    HSys.Sen.SnRKReset            .Name="SnRKReset"            ;
    HSys.Sen.SnRKPause            .Name="SnRKPause"            ;
    HSys.Sen.SnRKHome             .Name="SnRKHome"             ;
    HSys.Sen.SnRKStart            .Name="SnRKStart"            ;
    HSys.Sen.SnRKOneCycle         .Name="SnRKOneCycle"         ;
    HSys.Sen.SnRKRetry            .Name="SnRKRetry"            ;
    HSys.Sen.SnRKSkip             .Name="SnRKSkip"             ;
    HSys.Sen.SnRKCleanOut         .Name="SnRKCleanOut"         ;
    HSys.Sen.SnRKTrayFeed         .Name="SnRKTrayFeed"         ;
    HSys.Sen.SnRKTrayEnd          .Name="SnRKTrayEnd"          ;
    HSys.Sen.SnRKAlarmReset       .Name="SnRKAlarmReset"       ;
    HSys.Sen.SnRKCoverOpen        .Name="SnRKCoverOpen"        ;
    HSys.Sen.SnRKManualStep       .Name="SnRKManualStep"       ;
    HSys.Sen.SnRKManualTStart     .Name="SnRKManualTStart"     ;
    HSys.Sen.SnLoaderIsFull       .Name="SnLoaderIsFull"       ;
    HSys.Sen.SnLoaderTrayHasTray  .Name="SnLoaderTrayHasTray"  ;
    HSys.Sen.SnLoaderCarHasTray   .Name="SnLoaderCarHasTray"   ;
    HSys.Sen.SnLoaderCCDPos       .Name="SnLoaderCCDPos"       ;
    HSys.Sen.SnLoaderFixCyPush    .Name="SnLoaderFixCyPush"    ;
    HSys.Sen.SnLoaderEdgePush     .Name="SnLoaderEdgePush"     ;
    HSys.Sen.SnLoaderPreDete      .Name="SnLoaderPreDete"      ;
    HSys.Sen.SnLoaderSureTray     .Name="SnLoaderSureTray"     ;
    HSys.Sen.SnEmpty1IsFull       .Name="SnEmpty1IsFull"       ;
    HSys.Sen.SnEmpty1SelectHasTray.Name="SnEmpty1SelectHasTray";
    HSys.Sen.SnEmpty1CarHasTray   .Name="SnEmpty1CarHasTray"   ;
    HSys.Sen.SnEmpty1CCDPosition  .Name="SnEmpty1CCDPosition"  ;
    HSys.Sen.SnEmpty1HasTray1     .Name="SnEmpty1HasTray1"     ;
    HSys.Sen.SnEmpty1FCyPushTray1 .Name="SnEmpty1FCyPushTray1" ;
    HSys.Sen.SnEmpty1RCyPushTray1 .Name="SnEmpty1RCyPushTray1" ;
    HSys.Sen.SnEmpty1HasTray2     .Name="SnEmpty1HasTray2"     ;
    HSys.Sen.SnEmpty1FCyPushTray2 .Name="SnEmpty1FCyPushTray2" ;
    HSys.Sen.SnEmpty1RCyPushTray2 .Name="SnEmpty1RCyPushTray2" ;
    HSys.Sen.SnEmpty1HasTray3     .Name="SnEmpty1HasTray3"     ;
    HSys.Sen.SnEmpty1FCyPushTray3 .Name="SnEmpty1FCyPushTray3" ;
    HSys.Sen.SnEmpty1RCyPushTray3 .Name="SnEmpty1RCyPushTray3" ;
    HSys.Sen.SnEmpty1HasTray4     .Name="SnEmpty1HasTray4"     ;
    HSys.Sen.SnEmpty1FCyPushTray4 .Name="SnEmpty1FCyPushTray4" ;
    HSys.Sen.SnEmpty1RCyPushTray4 .Name="SnEmpty1RCyPushTray4" ;
    HSys.Sen.SnEmpty2IsFull       .Name="SnEmpty2IsFull"       ;
    HSys.Sen.SnEmpty2SelectHasTray.Name="SnEmpty2SelectHasTray";
    HSys.Sen.SnEmpty2CarHasTray   .Name="SnEmpty2CarHasTray"   ;
    HSys.Sen.SnEmpty2CCDPosition  .Name="SnEmpty2CCDPosition"  ;
    HSys.Sen.SnEmpty2HasTray      .Name="SnEmpty2HasTray"      ;
    HSys.Sen.SnEmpty2CCWDete      .Name="SnEmpty2CCWDete"      ;
    HSys.Sen.SnEmpty2FixCyPush    .Name="SnEmpty2FixCyPush"    ;
    HSys.Sen.SnEmpty2EdgePush     .Name="SnEmpty2EdgePush"     ;
    HSys.Sen.SnEmpty2HasEmptyTray .Name="SnEmpty2HasEmptyTray" ;
    HSys.Sen.SnShiftStageHasTray  .Name="SnShiftStageHasTray"  ;
    HSys.Sen.SnShiftStageCWPreDete.Name="SnShiftStageCWPreDete";
    HSys.Sen.SnShiftStageEdgePush .Name="SnShiftStageEdgePush" ;
    HSys.Sen.SnAuto1HasTray       .Name="SnAuto1HasTray"       ;
    HSys.Sen.SnAuto2HasTray       .Name="SnAuto2HasTray"       ;
    HSys.Sen.SnAuto3HasTray       .Name="SnAuto3HasTray"       ;
    HSys.Sen.SnAuto4HasTray       .Name="SnAuto4HasTray"       ;
    HSys.Sen.SnAuto5HasTray       .Name="SnAuto5HasTray"       ;
    HSys.Sen.SnAuto6HasTray       .Name="SnAuto6HasTray"       ;
    HSys.Sen.SnAuto7HasTray       .Name="SnAuto7HasTray"       ;
    HSys.Sen.SnAuto8HasTray       .Name="SnAuto8HasTray"       ;
    HSys.Sen.SnAuto9HasTray       .Name="SnAuto9HasTray"       ;
    HSys.Sen.SnAuto10HasTray      .Name="SnAuto10HasTray"      ;
    HSys.Sen.SnAuto11HasTray      .Name="SnAuto11HasTray"      ;
    HSys.Sen.SnAuto12HasTray      .Name="SnAuto12HasTray"      ;
    HSys.Sen.SnAuto13HasTray      .Name="SnAuto13HasTray"      ;
    HSys.Sen.SnAuto14HasTray      .Name="SnAuto14HasTray"      ;
    HSys.Sen.SnAuto15HasTray      .Name="SnAuto15HasTray"      ;
    HSys.Sen.SnAuto16HasTray      .Name="SnAuto16HasTray"      ;
    HSys.Sen.SnAuto17HasTray      .Name="SnAuto17HasTray"      ;
    HSys.Sen.SnAuto18HasTray      .Name="SnAuto18HasTray"      ;
    HSys.Sen.SnAuto19HasTray      .Name="SnAuto19HasTray"      ;
    HSys.Sen.SnAuto20HasTray      .Name="SnAuto20HasTray"      ;
    HSys.Sen.SnAuto1CyAntiDrop1   .Name="SnAuto1CyAntiDrop1"   ;
    HSys.Sen.SnAuto1CyAntiDrop2   .Name="SnAuto1CyAntiDrop2"   ;
    HSys.Sen.SnAuto1CyAntiDrop3   .Name="SnAuto1CyAntiDrop3"   ;
    HSys.Sen.SnAuto2CyAntiDrop1   .Name="SnAuto2CyAntiDrop1"   ;
    HSys.Sen.SnAuto2CyAntiDrop2   .Name="SnAuto2CyAntiDrop2"   ;
    HSys.Sen.SnAuto2CyAntiDrop3   .Name="SnAuto2CyAntiDrop3"   ;
    HSys.Sen.SnAuto3CyAntiDrop1   .Name="SnAuto3CyAntiDrop1"   ;
    HSys.Sen.SnAuto3CyAntiDrop2   .Name="SnAuto3CyAntiDrop2"   ;
    HSys.Sen.SnAuto3CyAntiDrop3   .Name="SnAuto3CyAntiDrop3"   ;
    HSys.Sen.SnAuto4CyAntiDrop1   .Name="SnAuto4CyAntiDrop1"   ;
    HSys.Sen.SnAuto4CyAntiDrop2   .Name="SnAuto4CyAntiDrop2"   ;
    HSys.Sen.SnAuto4CyAntiDrop3   .Name="SnAuto4CyAntiDrop3"   ;
    HSys.Sen.SnAuto5CyAntiDrop1   .Name="SnAuto5CyAntiDrop1"   ;
    HSys.Sen.SnAuto5CyAntiDrop2   .Name="SnAuto5CyAntiDrop2"   ;
    HSys.Sen.SnAuto5CyAntiDrop3   .Name="SnAuto5CyAntiDrop3"   ;
    HSys.Sen.SnAuto6CyAntiDrop1   .Name="SnAuto6CyAntiDrop1"   ;
    HSys.Sen.SnAuto6CyAntiDrop2   .Name="SnAuto6CyAntiDrop2"   ;
    HSys.Sen.SnAuto6CyAntiDrop3   .Name="SnAuto6CyAntiDrop3"   ;
    HSys.Sen.SnAuto7CyAntiDrop1   .Name="SnAuto7CyAntiDrop1"   ;
    HSys.Sen.SnAuto7CyAntiDrop2   .Name="SnAuto7CyAntiDrop2"   ;
    HSys.Sen.SnAuto7CyAntiDrop3   .Name="SnAuto7CyAntiDrop3"   ;
    HSys.Sen.SnAuto8CyAntiDrop1   .Name="SnAuto8CyAntiDrop1"   ;
    HSys.Sen.SnAuto8CyAntiDrop2   .Name="SnAuto8CyAntiDrop2"   ;
    HSys.Sen.SnAuto8CyAntiDrop3   .Name="SnAuto8CyAntiDrop3"   ;
    HSys.Sen.SnAuto9CyAntiDrop1   .Name="SnAuto9CyAntiDrop1"   ;
    HSys.Sen.SnAuto9CyAntiDrop2   .Name="SnAuto9CyAntiDrop2"   ;
    HSys.Sen.SnAuto9CyAntiDrop3   .Name="SnAuto9CyAntiDrop3"   ;
    HSys.Sen.SnAuto10CyAntiDrop1  .Name="SnAuto10CyAntiDrop1"  ;
    HSys.Sen.SnAuto10CyAntiDrop2  .Name="SnAuto10CyAntiDrop2"  ;
    HSys.Sen.SnAuto10CyAntiDrop3  .Name="SnAuto10CyAntiDrop3"  ;
    HSys.Sen.SnAuto11CyAntiDrop1  .Name="SnAuto11CyAntiDrop1"  ;
    HSys.Sen.SnAuto11CyAntiDrop2  .Name="SnAuto11CyAntiDrop2"  ;
    HSys.Sen.SnAuto11CyAntiDrop3  .Name="SnAuto11CyAntiDrop3"  ;
    HSys.Sen.SnAuto12CyAntiDrop1  .Name="SnAuto12CyAntiDrop1"  ;
    HSys.Sen.SnAuto12CyAntiDrop2  .Name="SnAuto12CyAntiDrop2"  ;
    HSys.Sen.SnAuto12CyAntiDrop3  .Name="SnAuto12CyAntiDrop3"  ;
    HSys.Sen.SnAuto13CyAntiDrop1  .Name="SnAuto13CyAntiDrop1"  ;
    HSys.Sen.SnAuto13CyAntiDrop2  .Name="SnAuto13CyAntiDrop2"  ;
    HSys.Sen.SnAuto13CyAntiDrop3  .Name="SnAuto13CyAntiDrop3"  ;
    HSys.Sen.SnAuto14CyAntiDrop1  .Name="SnAuto14CyAntiDrop1"  ;
    HSys.Sen.SnAuto14CyAntiDrop2  .Name="SnAuto14CyAntiDrop2"  ;
    HSys.Sen.SnAuto14CyAntiDrop3  .Name="SnAuto14CyAntiDrop3"  ;
    HSys.Sen.SnAuto15CyAntiDrop1  .Name="SnAuto15CyAntiDrop1"  ;
    HSys.Sen.SnAuto15CyAntiDrop2  .Name="SnAuto15CyAntiDrop2"  ;
    HSys.Sen.SnAuto15CyAntiDrop3  .Name="SnAuto15CyAntiDrop3"  ;
    HSys.Sen.SnAuto16CyAntiDrop1  .Name="SnAuto16CyAntiDrop1"  ;
    HSys.Sen.SnAuto16CyAntiDrop2  .Name="SnAuto16CyAntiDrop2"  ;
    HSys.Sen.SnAuto16CyAntiDrop3  .Name="SnAuto16CyAntiDrop3"  ;
    HSys.Sen.SnAuto17CyAntiDrop1  .Name="SnAuto17CyAntiDrop1"  ;
    HSys.Sen.SnAuto17CyAntiDrop2  .Name="SnAuto17CyAntiDrop2"  ;
    HSys.Sen.SnAuto17CyAntiDrop3  .Name="SnAuto17CyAntiDrop3"  ;
    HSys.Sen.SnAuto18CyAntiDrop1  .Name="SnAuto18CyAntiDrop1"  ;
    HSys.Sen.SnAuto18CyAntiDrop2  .Name="SnAuto18CyAntiDrop2"  ;
    HSys.Sen.SnAuto18CyAntiDrop3  .Name="SnAuto18CyAntiDrop3"  ;
    HSys.Sen.SnAuto19CyAntiDrop1  .Name="SnAuto19CyAntiDrop1"  ;
    HSys.Sen.SnAuto19CyAntiDrop2  .Name="SnAuto19CyAntiDrop2"  ;
    HSys.Sen.SnAuto19CyAntiDrop3  .Name="SnAuto19CyAntiDrop3"  ;
    HSys.Sen.SnAuto20CyAntiDrop1  .Name="SnAuto20CyAntiDrop1"  ;
    HSys.Sen.SnAuto20CyAntiDrop2  .Name="SnAuto20CyAntiDrop2"  ;
    HSys.Sen.SnAuto20CyAntiDrop3  .Name="SnAuto20CyAntiDrop3"  ;
    HSys.Sen.SnAuto1CarHasTray    .Name="SnAuto1CarHasTray"    ;
    HSys.Sen.SnAuto2CarHasTray    .Name="SnAuto2CarHasTray"    ;
    HSys.Sen.SnAuto3CarHasTray    .Name="SnAuto3CarHasTray"    ;
    HSys.Sen.SnAuto4CarHasTray    .Name="SnAuto4CarHasTray"    ;
    HSys.Sen.SnAuto5CarHasTray    .Name="SnAuto5CarHasTray"    ;
    HSys.Sen.SnAuto6CarHasTray    .Name="SnAuto6CarHasTray"    ;
    HSys.Sen.SnAuto7CarHasTray    .Name="SnAuto7CarHasTray"    ;
    HSys.Sen.SnAuto8CarHasTray    .Name="SnAuto8CarHasTray"    ;
    HSys.Sen.SnAuto9CarHasTray    .Name="SnAuto9CarHasTray"    ;
    HSys.Sen.SnAuto10CarHasTray   .Name="SnAuto10CarHasTray"   ;
    HSys.Sen.SnAuto11CarHasTray   .Name="SnAuto11CarHasTray"   ;
    HSys.Sen.SnAuto12CarHasTray   .Name="SnAuto12CarHasTray"   ;
    HSys.Sen.SnAuto13CarHasTray   .Name="SnAuto13CarHasTray"   ;
    HSys.Sen.SnAuto14CarHasTray   .Name="SnAuto14CarHasTray"   ;
    HSys.Sen.SnAuto15CarHasTray   .Name="SnAuto15CarHasTray"   ;
    HSys.Sen.SnAuto16CarHasTray   .Name="SnAuto16CarHasTray"   ;
    HSys.Sen.SnAuto17CarHasTray   .Name="SnAuto17CarHasTray"   ;
    HSys.Sen.SnAuto18CarHasTray   .Name="SnAuto18CarHasTray"   ;
    HSys.Sen.SnAuto19CarHasTray   .Name="SnAuto19CarHasTray"   ;
    HSys.Sen.SnAuto20CarHasTray   .Name="SnAuto20CarHasTray"   ;
    HSys.Sen.SnAutoRow1HasTray    .Name="SnAutoRow1HasTray"    ;
    HSys.Sen.SnAutoRow2HasTray    .Name="SnAutoRow2HasTray"    ;
    HSys.Sen.SnAutoRow3HasTray    .Name="SnAutoRow3HasTray"    ;
    HSys.Sen.SnAutoRow4HasTray    .Name="SnAutoRow4HasTray"    ;
    HSys.Sen.SnAutoRow1SlowDown   .Name="SnAutoRow1SlowDown"   ;
    HSys.Sen.SnAutoRow2SlowDown   .Name="SnAutoRow2SlowDown"   ;
    HSys.Sen.SnAutoRow3SlowDown   .Name="SnAutoRow3SlowDown"   ;
    HSys.Sen.SnAutoRow4SlowDown   .Name="SnAutoRow4SlowDown"   ;
    HSys.Sen.SnStoreArmTakeTray   .Name="SnStoreArmTakeTray"   ;
    HSys.Sen.SnStoreArmHasTray    .Name="SnStoreArmHasTray"    ;
    HSys.Sen.SnStoreArmTakePos    .Name="SnStoreArmTakePos"    ;
    HSys.Sen.SnStoreArmPutPos1    .Name="SnStoreArmPutPos1"    ;
    HSys.Sen.SnStoreArmPutPos2    .Name="SnStoreArmPutPos2"    ;
    HSys.Sen.SnStore1Full         .Name="SnStore1Full"         ;
    HSys.Sen.SnStore1Door         .Name="SnStore1Door"         ;
    HSys.Sen.SnStore2Full         .Name="SnStore2Full"         ;
    HSys.Sen.SnStore2Door         .Name="SnStore2Door"         ;
    HSys.Sen.SnStore3Full         .Name="SnStore3Full"         ;
    HSys.Sen.SnStore3Door         .Name="SnStore3Door"         ;
    HSys.Sen.SnStore4Full         .Name="SnStore4Full"         ;
    HSys.Sen.SnStore4Door         .Name="SnStore4Door"         ;
    HSys.Sen.SnStore5Full         .Name="SnStore5Full"         ;
    HSys.Sen.SnStore5Door         .Name="SnStore5Door"         ;
    HSys.Sen.SnStore6Full         .Name="SnStore6Full"         ;
    HSys.Sen.SnStore6Door         .Name="SnStore6Door"         ;
    HSys.Sen.SnStore7Full         .Name="SnStore7Full"         ;
    HSys.Sen.SnStore7Door         .Name="SnStore7Door"         ;
    HSys.Sen.SnStore8Full         .Name="SnStore8Full"         ;
    HSys.Sen.SnStore8Door         .Name="SnStore8Door"         ;
    HSys.Sen.SnStore9Full         .Name="SnStore9Full"         ;
    HSys.Sen.SnStore9Door         .Name="SnStore9Door"         ;
    HSys.Sen.SnStore10Full        .Name="SnStore10Full"        ;
    HSys.Sen.SnStore10Door        .Name="SnStore10Door"        ;
    HSys.Sen.SnStore11Full        .Name="SnStore11Full"        ;
    HSys.Sen.SnStore11Door        .Name="SnStore11Door"        ;
    HSys.Sen.SnStore12Full        .Name="SnStore12Full"        ;
    HSys.Sen.SnStore12Door        .Name="SnStore12Door"        ;
    HSys.Sen.SnStore13Full        .Name="SnStore13Full"        ;
    HSys.Sen.SnStore13Door        .Name="SnStore13Door"        ;
    HSys.Sen.SnStore14Full        .Name="SnStore14Full"        ;
    HSys.Sen.SnStore14Door        .Name="SnStore14Door"        ;
    HSys.Sen.SnStore15Full        .Name="SnStore15Full"        ;
    HSys.Sen.SnStore15Door        .Name="SnStore15Door"        ;
    HSys.Sen.SnStore16Full        .Name="SnStore16Full"        ;
    HSys.Sen.SnStore16Door        .Name="SnStore16Door"        ;
    HSys.Sen.SnStore17Full        .Name="SnStore17Full"        ;
    HSys.Sen.SnStore17Door        .Name="SnStore17Door"        ;
    HSys.Sen.SnStore18Full        .Name="SnStore18Full"        ;
    HSys.Sen.SnStore18Door        .Name="SnStore18Door"        ;
    HSys.Sen.SnStore19Full        .Name="SnStore19Full"        ;
    HSys.Sen.SnStore19Door        .Name="SnStore19Door"        ;
    HSys.Sen.SnStore20Full        .Name="SnStore20Full"        ;
    HSys.Sen.SnStore20Door        .Name="SnStore20Door"        ;
    HSys.Sen.SnMGZ1ArmHasTray     .Name="SnMGZ1ArmHasTray"     ;
    HSys.Sen.SnMGZ1DetectTray     .Name="SnMGZ1DetectTray"     ;
    HSys.Sen.SnMGZ1LayerHasTray   .Name="SnMGZ1LayerHasTray"   ;
    HSys.Sen.SnMGZ1SidePushUp     .Name="SnMGZ1SidePushUp"     ;
    HSys.Sen.SnMGZ1TrayFloating   .Name="SnMGZ1TrayFloating"   ;
    HSys.Sen.SnMGZ1TopHasTray     .Name="SnMGZ1TopHasTray"     ;
    HSys.Sen.SnMGZ1TrayPositionR  .Name="SnMGZ1TrayPositionR"  ;
    HSys.Sen.SnMGZ1TrayPositionF  .Name="SnMGZ1TrayPositionF"  ;
    HSys.Sen.SnMGZ1TrayDirection  .Name="SnMGZ1TrayDirection"  ;
    HSys.Sen.SnMGZ1CatchCyOff     .Name="SnMGZ1CatchCyOff"     ;
    HSys.Sen.SnMGZ1Step           .Name="SnMGZ1Step"      ;
    HSys.Sen.SnMGZ1ArmXTakeTray   .Name="SnMGZ1ArmXTakeTray"   ;
    HSys.Sen.SnMGZ2ArmHasTray     .Name="SnMGZ2ArmHasTray"     ;
    HSys.Sen.SnMGZ2DetectTray     .Name="SnMGZ2DetectTray"     ;
    HSys.Sen.SnMGZ2LayerHasTray   .Name="SnMGZ2LayerHasTray"   ;
    HSys.Sen.SnMGZ2SidePushUp     .Name="SnMGZ2SidePushUp"     ;
    HSys.Sen.SnMGZ2TrayFloating   .Name="SnMGZ2TrayFloating"   ;
    HSys.Sen.SnMGZ2TopHasTray     .Name="SnMGZ2TopHasTray"     ;
    HSys.Sen.SnMGZ2TrayPositionR  .Name="SnMGZ2TrayPositionR"  ;
    HSys.Sen.SnMGZ2TrayPositionF  .Name="SnMGZ2TrayPositionF"  ;
    HSys.Sen.SnMGZ2TrayDirection  .Name="SnMGZ2TrayDirection"  ;
    HSys.Sen.SnMGZ2CatchCyOff     .Name="SnMGZ2CatchCyOff"     ;
    HSys.Sen.SnMGZ2Step           .Name="SnMGZ2Step"      ;
    HSys.Sen.SnMGZ2ArmXTakeTray   .Name="SnMGZ2ArmXTakeTray"   ;
    HSys.Sen.SnMGZ3ArmHasTray     .Name="SnMGZ3ArmHasTray"     ;
    HSys.Sen.SnMGZ3DetectTray     .Name="SnMGZ3DetectTray"     ;
    HSys.Sen.SnMGZ3LayerHasTray   .Name="SnMGZ3LayerHasTray"   ;
    HSys.Sen.SnMGZ3SidePushUp     .Name="SnMGZ3SidePushUp"     ;
    HSys.Sen.SnMGZ3TrayFloating   .Name="SnMGZ3TrayFloating"   ;
    HSys.Sen.SnMGZ3TopHasTray     .Name="SnMGZ3TopHasTray"     ;
    HSys.Sen.SnMGZ3TrayPositionR  .Name="SnMGZ3TrayPositionR"  ;
    HSys.Sen.SnMGZ3TrayPositionF  .Name="SnMGZ3TrayPositionF"  ;
    HSys.Sen.SnMGZ3TrayDirection  .Name="SnMGZ3TrayDirection"  ;
    HSys.Sen.SnMGZ3CatchCyOff     .Name="SnMGZ3CatchCyOff"     ;
    HSys.Sen.SnMGZ3Step           .Name="SnMGZ3Step"      ;
    HSys.Sen.SnMGZ3ArmXTakeTray   .Name="SnMGZ3ArmXTakeTray"   ;
    HSys.Sen.SnMotorPower         .Name="SnMotorPower"         ;
    HSys.Sen.SnAirIsEnough        .Name="SnAirIsEnough"        ;
    HSys.Sen.SnRKSafeLock         .Name="SnRKSafeLock"         ;
    HSys.Sen.SnIonFan_01          .Name="SnIonFan_01"          ;
    HSys.Sen.SnIonFan_02          .Name="SnIonFan_02"          ;
    HSys.Sen.SnIonFan_03          .Name="SnIonFan_03"          ;
    HSys.Sen.SnIonFan_04          .Name="SnIonFan_04"          ;
    HSys.Sen.SnIonFan_05          .Name="SnIonFan_05"          ;
    HSys.Sen.SnIonFan_06          .Name="SnIonFan_06"          ;
    HSys.Sen.SnIonFan_07          .Name="SnIonFan_07"          ;
    HSys.Sen.SnIonFan_08          .Name="SnIonFan_08"          ;
    HSys.Sen.SnSafeDoor_01        .Name="SnSafeDoor_01"        ;
    HSys.Sen.SnSafeDoor_02        .Name="SnSafeDoor_02"        ;
    HSys.Sen.SnSafeDoor_03        .Name="SnSafeDoor_03"        ;
    HSys.Sen.SnSafeDoor_04        .Name="SnSafeDoor_04"        ;
    HSys.Sen.SnSafeDoor_05        .Name="SnSafeDoor_05"        ;
    HSys.Sen.SnSafeDoor_06        .Name="SnSafeDoor_06"        ;
    HSys.Sen.SnSafeDoor_07        .Name="SnSafeDoor_07"        ;
    HSys.Sen.SnSafeDoor_08        .Name="SnSafeDoor_08"        ;
    HSys.Sen.SnSafeDoor_09        .Name="SnSafeDoor_09"        ;
    HSys.Sen.SnSafeDoor_10        .Name="SnSafeDoor_10"        ;
    HSys.Sen.SnSafeDoor_11        .Name="SnSafeDoor_11"        ;
    HSys.Sen.SnSafeDoor_12        .Name="SnSafeDoor_12"        ;
    HSys.Sen.SnEMG01              .Name="SnEMG01"              ;
    HSys.Sen.SnEMG02              .Name="SnEMG02"              ;
    HSys.Sen.SnEMG03              .Name="SnEMG03"              ;
    HSys.Sen.SnEMG04              .Name="SnEMG04"              ;
    HSys.Sen.SnPLCSafer           .Name="SnPLCSafer"           ;
    HSys.Sen.SnSortArm1XInterfere .Name="SnSortArm1XInterfere" ;
    HSys.Sen.SnSortArm1YInterfere .Name="SnSortArm1YInterfere" ;
    HSys.Sen.SnSortArm2XInterfere .Name="SnSortArm2XInterfere" ;
    HSys.Sen.SnSortArm2YInterfere .Name="SnSortArm2YInterfere" ;
    HSys.Sen.SnManual_StoreArm_Z  .Name="SnManual_StoreArm_Z"  ;
    HSys.Sen.SnTrackTrayDetect1   .Name="SnTrackTrayDetect1"   ;
    HSys.Sen.SnTrackTrayDetect2   .Name="SnTrackTrayDetect2"   ;
    HSys.Sen.SnTrackTrayDetect3   .Name="SnTrackTrayDetect3"   ;
    HSys.Sen.SnTrackTrayDetect4   .Name="SnTrackTrayDetect4"   ;
    HSys.Sen.SnAutoRow1ULDCheck   .Name="SnAutoRow1ULDCheck"   ;
    HSys.Sen.SnAutoRow2ULDCheck   .Name="SnAutoRow2ULDCheck"   ;
    HSys.Sen.SnAutoRow3ULDCheck   .Name="SnAutoRow3ULDCheck"   ;
    HSys.Sen.SnAutoRow4ULDCheck   .Name="SnAutoRow4ULDCheck"   ;
    HSys.Sen.SnAutoRow1PushCheck  .Name="SnAutoRow1PushCheck"  ;                //推Tray 機構改為馬達
    HSys.Sen.SnAutoRow2PushCheck  .Name="SnAutoRow2PushCheck"  ;                //推Tray 機構改為馬達
    HSys.Sen.SnAutoRow3PushCheck  .Name="SnAutoRow3PushCheck"  ;                //推Tray 機構改為馬達
    HSys.Sen.SnAutoRow4PushCheck  .Name="SnAutoRow4PushCheck"  ;                //推Tray 機構改為馬達
    HSys.Sen.SnAutoRow1CyPushTray .Name="SnAutoRow1CyPushTray" ;                //推Tray 機構改為馬達
    HSys.Sen.SnAutoRow2CyPushTray .Name="SnAutoRow2CyPushTray" ;                //推Tray 機構改為馬達
    HSys.Sen.SnAutoRow3CyPushTray .Name="SnAutoRow3CyPushTray" ;                //推Tray 機構改為馬達
    HSys.Sen.SnAutoRow4CyPushTray .Name="SnAutoRow4CyPushTray" ;                //推Tray 機構改為馬達
    HSys.Sen.SnAutoTrolly1HasTray .Name="SnAutoTrolly1HasTray" ;
    HSys.Sen.SnAutoTrolly2HasTray .Name="SnAutoTrolly2HasTray" ;
    HSys.Sen.SnAutoTrolly3HasTray .Name="SnAutoTrolly3HasTray" ;
    HSys.Sen.SnAutoTrolly4HasTray .Name="SnAutoTrolly4HasTray" ;
    HSys.Sen.SnLoaderCarDete      .Name="SnLoaderCarDete"      ;
    HSys.Sen.SnStore1HasTray      .Name="SnStore1HasTray"      ;
    HSys.Sen.SnStore2HasTray      .Name="SnStore2HasTray"      ;
    HSys.Sen.SnStore3HasTray      .Name="SnStore3HasTray"      ;
    HSys.Sen.SnStore4HasTray      .Name="SnStore4HasTray"      ;
    HSys.Sen.SnStore5HasTray      .Name="SnStore5HasTray"      ;
    HSys.Sen.SnStore6HasTray      .Name="SnStore6HasTray"      ;
    HSys.Sen.SnStore7HasTray      .Name="SnStore7HasTray"      ;
    HSys.Sen.SnStore8HasTray      .Name="SnStore8HasTray"      ;
    HSys.Sen.SnStore9HasTray      .Name="SnStore9HasTray"      ;
    HSys.Sen.SnStore10HasTray     .Name="SnStore10HasTray"     ;
    HSys.Sen.SnStore11HasTray     .Name="SnStore11HasTray"     ;
    HSys.Sen.SnStore12HasTray     .Name="SnStore12HasTray"     ;
    HSys.Sen.SnStore13HasTray     .Name="SnStore13HasTray"     ;
    HSys.Sen.SnStore14HasTray     .Name="SnStore14HasTray"     ;
    HSys.Sen.SnStore15HasTray     .Name="SnStore15HasTray"     ;
    HSys.Sen.SnStore16HasTray     .Name="SnStore16HasTray"     ;
    HSys.Sen.SnStore17HasTray     .Name="SnStore17HasTray"     ;
    HSys.Sen.SnStore18HasTray     .Name="SnStore18HasTray"     ;
    HSys.Sen.SnStore19HasTray     .Name="SnStore19HasTray"     ;
    HSys.Sen.SnStore20HasTray     .Name="SnStore20HasTray"     ;
}
//==============================================================================
//  009
//  不須改變 _
//==============================================================================
void SYSTEM_MODULAR::LoadSensorParameterFromDataBase()                          // ben ok //
{
    AnsiString S;
    int iSn;
    AnsiString str;
    bool bHasSensor;
    SenPtr=(TMySensor *)&Sen;
    AnsiString sPort, sBit, sCard;
    iTotalSensor=sizeof(Sen)/sizeof(TMySensor);

    for(int i=0; i<iTotalSensor; i++)
    {
        bHasSensor=false;
        SenPtr[i].Enable=false;
        if(SenPtr[i].Name=="")
            continue;

        HSys.mapIOTableIter=HSys.mapIOTable.find(SenPtr[i].Name);
        if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
        {
            bHasSensor=true;
            iSn=atoi(HSys.mapIOTable[SenPtr[i].Name].c_str());
        }
        else
        {
            str.sprintf("Can not find sensor %s", SenPtr[i].Name);
        }

        if(SenPtr[i].Input!=NULL)
            delete SenPtr[i].Input;
        SenPtr[i].Input=new TMyMN200_IO;
        if(bHasSensor)
        {
            SenPtr[i].Input->ISABase    =HSys.IOTable[iSn]->iISABase;
            SenPtr[i].EnableAtDataBase  =HSys.IOTable[iSn]->iEnable;
            SenPtr[i].Input->SetHint    (HSys.IOTable[iSn]->IOPos);

            if(SenPtr[i].Input->ISABase==eMotionNet)
                SenPtr[i].Input->SetPortInformation(HSys.IOTable[iSn]->iLane, HSys.IOTable[iSn]->sIP, HSys.IOTable[iSn]->iPort, HSys.IOTable[iSn]->iBit);
            else
                SenPtr[i].Input->SetPortInformation(AnsiString(""), HSys.IOTable[iSn]->iPort, HSys.IOTable[iSn]->iBit);

            SenPtr[i].Card  =SenPtr[i].Input->GetCard();
            SenPtr[i].Port  =SenPtr[i].Input->GetPort();
            SenPtr[i].Bit   =SenPtr[i].Input->GetBit();

            SenPtr[i].Type  =HSys.IOTable[iSn]->iInType;
            if(HSys.IOTable[iSn]->iEnable==1)
                SenPtr[i].Enable   =true;
        }
        else
        {
            SenPtr[i].Enable=false;
        }
        #ifdef SOFT_SIMULATE        //Sam 20240515 : 修正 Dummy Run
            #ifdef SOFT_SIMULTE_EtherCAT
                if(SenPtr[i].Input->ISABase==ePCI1203)                          //Sam 20230724 : 測試 SensorEtherCAT 用
                {

                }
                else
                {
                    SenPtr[i].Enable=false;
                }
            #else
                SenPtr[i].Enable=false;
            #endif
        #endif
        //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
        //==>
        if(Enable_PLCSafety_IO && SenPtr[i].Input->ISABase==e_PLCbase)                 // 20230821 Joseph , Add Heater door //
        {
            bPLCIO[SenPtr[i].Port][SenPtr[i].Bit]=true;
            SenPtr[i].Type=1;
            SenPtr[i].Enable=1;
        }
        //<==
        //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
    }

    for(int i=0; i<iTotalSensor; i++)
        SenPtr[i].Tag=i;
}
//==============================================================================
//  010
//  使用者依不同機器修改 _                                                      // NEED EDIT BY USER //
//==============================================================================
void __fastcall TDataModule1::SpecificSetupForSensorParameterExecute(
      TObject *Sender)
{
    #ifdef SOFT_SIMULATE
    return;
    #endif
}
//==============================================================================
//  011
//  使用者依不同機器修改 _                                                      // NEED EDIT BY USER //
//==============================================================================
void __fastcall TDataModule1::InitialSwitchNameExecute(TObject *Sender)         // 輸出 //
{
    HSys.Sw.SwACLoaderCCW      .Name="SwACLoaderCCW"       ;
    HSys.Sw.SwACLoaderCW       .Name="SwACLoaderCW"        ;
    HSys.Sw.SwLoaderCWDete     .Name="SwLoaderCWDete"      ;
    HSys.Sw.SwACEmpty1CCW      .Name="SwACEmpty1CCW"       ;
    HSys.Sw.SwACEmpty1CW       .Name="SwACEmpty1CW"        ;
    HSys.Sw.SwACEmpty2CCW      .Name="SwACEmpty2CCW"       ;
    HSys.Sw.SwACEmpty2CW       .Name="SwACEmpty2CW"        ;
    HSys.Sw.SwEmpty2CCWDete    .Name="SwEmpty2CCWDete"     ;
    HSys.Sw.SwACAutoRow1CW     .Name="SwACAutoRow1CW"      ;
    HSys.Sw.SwACAutoRow1CCW    .Name="SwACAutoRow1CCW"     ;
    HSys.Sw.SwACAutoRow2CW     .Name="SwACAutoRow2CW"      ;
    HSys.Sw.SwACAutoRow2CCW    .Name="SwACAutoRow2CCW"     ;
    HSys.Sw.SwACAutoRow3CW     .Name="SwACAutoRow3CW"      ;
    HSys.Sw.SwACAutoRow3CCW    .Name="SwACAutoRow3CCW"     ;
    HSys.Sw.SwACAutoRow4CW     .Name="SwACAutoRow4CW"      ;
    HSys.Sw.SwACAutoRow4CCW    .Name="SwACAutoRow4CCW"     ;
    HSys.Sw.SwACShiftStageCCW  .Name="SwACShiftStageCCW"   ;
    HSys.Sw.SwACShiftStageCW   .Name="SwACShiftStageCW"    ;
    HSys.Sw.Sw_MGZ1Step        .Name="Sw_MGZ1Step"         ;
    HSys.Sw.Sw_MGZ2Step        .Name="Sw_MGZ2Step"         ;
    HSys.Sw.Sw_MGZ3Step        .Name="Sw_MGZ3Step"         ;
    HSys.Sw.SwLight            .Name="SwLight"             ;
    HSys.Sw.SwMotorRelay       .Name="SwMotorRelay"        ;
    HSys.Sw.SwBreak_StoreArm_Z .Name="SwBreak_StoreArm_Z"  ;
    HSys.Sw.SwManual_StoreArm_Z.Name="SwManual_StoreArm_Z" ;
    HSys.Sw.SwBreak_MGZ1Arm_Z  .Name="SwBreak_MGZ1Arm_Z"   ;
    HSys.Sw.SwBreak_MGZ2Arm_Z  .Name="SwBreak_MGZ2Arm_Z"   ;
    HSys.Sw.SwBreak_MGZ3Arm_Z  .Name="SwBreak_MGZ3Arm_Z"   ;
    HSys.Sw.SwTowerRed         .Name="SwTowerRed"          ;
    HSys.Sw.SwTowerYellow      .Name="SwTowerYellow"       ;
    HSys.Sw.SwTowerGreen       .Name="SwTowerGreen"        ;
    HSys.Sw.SwMusic1           .Name="SwMusic1"            ;
    HSys.Sw.SwMusic2           .Name="SwMusic2"            ;
    HSys.Sw.SwMusic3           .Name="SwMusic3"            ;
    HSys.Sw.SwMusic4           .Name="SwMusic4"            ;
    HSys.Sw.SwInterLock        .Name="SwInterLock"         ;
    HSys.Sw.SwFKPowerOff       .Name="SwFKPowerOff"        ;
    HSys.Sw.SwFKPowerOn        .Name="SwFKPowerOn"         ;
    HSys.Sw.SwFKReset          .Name="SwFKReset"           ;
    HSys.Sw.SwFKPause          .Name="SwFKPause"           ;
    HSys.Sw.SwFKHome           .Name="SwFKHome"            ;
    HSys.Sw.SwFKStart          .Name="SwFKStart"           ;
    HSys.Sw.SwFKOneCycle       .Name="SwFKOneCycle"        ;
    HSys.Sw.SwFKRetry          .Name="SwFKRetry"           ;
    HSys.Sw.SwFKSkip           .Name="SwFKSkip"            ;
    HSys.Sw.SwFKCleanOut       .Name="SwFKCleanOut"        ;
    HSys.Sw.SwFKTrayFeed       .Name="SwFKTrayFeed"        ;
    HSys.Sw.SwFKTrayEnd        .Name="SwFKTrayEnd"         ;
    HSys.Sw.SwFKAlarmReset     .Name="SwFKAlarmReset"      ;
    HSys.Sw.SwFKCoverOpen      .Name="SwFKCoverOpen"       ;
    HSys.Sw.SwRKPowerOff       .Name="SwRKPowerOff"        ;
    HSys.Sw.SwRKPowerOn        .Name="SwRKPowerOn"         ;
    HSys.Sw.SwRKReset          .Name="SwRKReset"           ;
    HSys.Sw.SwRKPause          .Name="SwRKPause"           ;
    HSys.Sw.SwRKHome           .Name="SwRKHome"            ;
    HSys.Sw.SwRKStart          .Name="SwRKStart"           ;
    HSys.Sw.SwRKOneCycle       .Name="SwRKOneCycle"        ;
    HSys.Sw.SwRKRetry          .Name="SwRKRetry"           ;
    HSys.Sw.SwRKSkip           .Name="SwRKSkip"            ;
    HSys.Sw.SwRKCleanOut       .Name="SwRKCleanOut"        ;
    HSys.Sw.SwRKTrayFeed       .Name="SwRKTrayFeed"        ;
    HSys.Sw.SwRKTrayEnd        .Name="SwRKTrayEnd"         ;
    HSys.Sw.SwRKAlarmReset     .Name="SwRKAlarmReset"      ;
    HSys.Sw.SwRKCoverOpen      .Name="SwRKCoverOpen"       ;
    HSys.Sw.SwRKManualStep     .Name="SwRKManualStep"      ;
    HSys.Sw.SwRKManualTStart   .Name="SwRKManualTStart"    ;
    HSys.Sw.SwAutoRow1Vib1     .Name="SwAutoRow1Vib1"      ;
    HSys.Sw.SwAutoRow1Vib2     .Name="SwAutoRow1Vib2"      ;
    HSys.Sw.SwAutoRow2Vib1     .Name="SwAutoRow2Vib1"      ;
    HSys.Sw.SwAutoRow2Vib2     .Name="SwAutoRow2Vib2"      ;
    HSys.Sw.SwAutoRow3Vib1     .Name="SwAutoRow3Vib1"      ;
    HSys.Sw.SwAutoRow3Vib2     .Name="SwAutoRow3Vib2"      ;
    HSys.Sw.SwAutoRow4Vib1     .Name="SwAutoRow4Vib1"      ;
    HSys.Sw.SwAutoRow4Vib2     .Name="SwAutoRow4Vib2"      ;
    HSys.Sw.SwAutoRow1Dete     .Name="SwAutoRow1Dete"      ;
    HSys.Sw.SwAutoRow2Dete     .Name="SwAutoRow2Dete"      ;
    HSys.Sw.SwAutoRow3Dete     .Name="SwAutoRow3Dete"      ;
    HSys.Sw.SwAutoRow4Dete     .Name="SwAutoRow4Dete"      ;
    HSys.Sw.SwTFTBinRelay      .Name="SwTFTBinRelay"       ;
}
//==============================================================================
//  012
//  不須改變 _
//==============================================================================
void SYSTEM_MODULAR::LoadSwitchParameterFromDataBase()
{
    AnsiString S,Test;
    AnsiString str;
    AnsiString sPort, sBit, sCard;
    bool bHasSwitch=false;
    int iSn;
    SwPtr=(TMySwitch *)&Sw;
    iTotalSwitch=sizeof(Sw)/sizeof(TMySwitch);

    for(int i=0; i<iTotalSwitch; i++)
    {
        bHasSwitch=false;
        SwPtr[i].Enable=false;
        if(SwPtr[i].Name=="")
            continue;

        HSys.mapIOTableIter=HSys.mapIOTable.find(SwPtr[i].Name);
        if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
        {
            bHasSwitch=true;
            iSn=atoi(HSys.mapIOTable[SwPtr[i].Name].c_str());
        }
        else
        {
            str.sprintf("Can not find sensor %s", SwPtr[i].Name);
        }
        if(SwPtr[i].Output!=NULL)
            delete SwPtr[i].Output;
        SwPtr[i].Output=new TMyMN200_IO;
        if(bHasSwitch)
        {
            SwPtr[i].Output->ISABase    =HSys.IOTable[iSn]->iISABase;
            SwPtr[i].EnableAtDataBase   =HSys.IOTable[iSn]->iEnable;
            SwPtr[i].Output->SetHint    (HSys.IOTable[iSn]->IOPos);

            if(SwPtr[i].Output->ISABase==eMotionNet)
                SwPtr[i].Output->SetPortInformation(HSys.IOTable[iSn]->iLane, HSys.IOTable[iSn]->sIP, HSys.IOTable[iSn]->iPort, HSys.IOTable[iSn]->iBit);
            else
                SwPtr[i].Output->SetPortInformation(AnsiString(""), HSys.IOTable[iSn]->iPort, HSys.IOTable[iSn]->iBit);

            SwPtr[i].Card       =SwPtr[i].Output->GetCard();
            SwPtr[i].Port       =SwPtr[i].Output->GetPort();
            SwPtr[i].Bit        =SwPtr[i].Output->GetBit();
            SwPtr[i].Type       =HSys.IOTable[iSn]->iInType;

            #ifdef SOFT_SIMULATE
                SwPtr[i].Enable=false;
            #else
                SwPtr[i].Enable=HSys.IOTable[iSn]->iEnable;
            #endif
        }
        else
        {
            SwPtr[i].Enable=false;
        }
    }

    for(int i=0; i<iTotalSwitch; i++)
        SwPtr[i].Tag=i;
}
//==============================================================================
//  013
//  使用者依不同機器修改 _                                                      // need edit by user //
//==============================================================================
void __fastcall TDataModule1::SpecificSetupForSwitchParameterExecute(
      TObject *Sender)
{
    #ifdef SOFT_SIMULATE
    return;
    #else
        ReadLastDataIni();
        for(int i=0;i<HSys.iTotalSwitch;i++)
        {
            if(HSys.SwPtr[i].Enable)
            {
                if(HSys.LastSet.bSwStatus[i])
                {
                    HSys.SwPtr[i].On();
                }
                else
                {
                    HSys.SwPtr[i].Off();
                }
            }
        }

        if(HSys.LastSet.iStartMode==0)
        {
            HSys.Sw.SwMusic1.Off();
            HSys.Sw.SwMusic2.Off();
            HSys.Sw.SwMusic3.Off();
            HSys.Sw.SwMusic4.Off();
        }
    #endif
}
//==============================================================================
//  014
//  使用者依不同機器修改 _                                                      // need edit by user //
//==============================================================================
void __fastcall TDataModule1::InitialSuckerNameExecute(TObject *Sender)
{
    HSys.Suck.AutoArmSuck.Suck[0][0].SuckerName="AutoArmSuck_01";
    HSys.Suck.AutoArmSuck.Suck[1][0].SuckerName="AutoArmSuck_03";
    HSys.Suck.AutoArmSuck.Suck[2][0].SuckerName="AutoArmSuck_05";
    HSys.Suck.AutoArmSuck.Suck[3][0].SuckerName="AutoArmSuck_07";
    HSys.Suck.AutoArmSuck.Suck[0][1].SuckerName="AutoArmSuck_02";
    HSys.Suck.AutoArmSuck.Suck[1][1].SuckerName="AutoArmSuck_04";
    HSys.Suck.AutoArmSuck.Suck[2][1].SuckerName="AutoArmSuck_06";
    HSys.Suck.AutoArmSuck.Suck[3][1].SuckerName="AutoArmSuck_08";

    HSys.Suck.MagArmSuck.Suck[0][0].SuckerName="MagArmSuck_01";
    HSys.Suck.MagArmSuck.Suck[1][0].SuckerName="MagArmSuck_03";
    HSys.Suck.MagArmSuck.Suck[2][0].SuckerName="MagArmSuck_05";
    HSys.Suck.MagArmSuck.Suck[3][0].SuckerName="MagArmSuck_07";
    HSys.Suck.MagArmSuck.Suck[0][1].SuckerName="MagArmSuck_02";
    HSys.Suck.MagArmSuck.Suck[1][1].SuckerName="MagArmSuck_04";
    HSys.Suck.MagArmSuck.Suck[2][1].SuckerName="MagArmSuck_06";
    HSys.Suck.MagArmSuck.Suck[3][1].SuckerName="MagArmSuck_08";
}
//==============================================================================
//  015
//  不須改變 _
//==============================================================================
void SYSTEM_MODULAR::LoadSuckerParameterFromDataBase()
{
    AnsiString S, TempStr;
    AnsiString sPort, sBit, sCard;

    int iSu=0, iSuOn=0, iSuOff=0;
    bool bHasSuckSen, bHasSuckOn, bHasSuckOff;                                  //Stevem 20210625 : 修正新的CSV IO表讀檔方式

    HSys.SuckPtr=(TMyKitSuck *)&HSys.Suck;
    iTotalSucker=sizeof(Suck)/sizeof(TMyKitSuck);
    iTotalSucker-=1;

    for(int j=0; j<iTotalSucker; j++)
    {
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
            {
                bHasSuckSen=false;
                bHasSuckOn =false;
                bHasSuckOff=false;
                HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name   =HSys.SuckPtr[j].Suck[iR][iC].SuckerName;
                HSys.SuckPtr[j].Suck[iR][iC].SensorName    =HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name;
                HSys.SuckPtr[j].Suck[iR][iC].OnSw.Name     =HSys.SuckPtr[j].Suck[iR][iC].SuckerName+AnsiString("_On");
                HSys.SuckPtr[j].Suck[iR][iC].OnPortName    =HSys.SuckPtr[j].Suck[iR][iC].SuckerName+AnsiString("_On");
                HSys.SuckPtr[j].Suck[iR][iC].OffSw.Name    =HSys.SuckPtr[j].Suck[iR][iC].SuckerName+AnsiString("_Off");
                HSys.SuckPtr[j].Suck[iR][iC].OffPortName   =HSys.SuckPtr[j].Suck[iR][iC].SuckerName+AnsiString("_Off");

                HSys.mapIOTableIter=HSys.mapIOTable.find(HSys.SuckPtr[j].Suck[iR][iC].SuckerName);
                if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
                {
                    bHasSuckSen=true;
                    iSu=atoi(HSys.mapIOTable[HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name].c_str());
                }
                else
                {
                    S.sprintf("Can not find sucker sensor %s", HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name.c_str());
    //                continue;
                }

                HSys.mapIOTableIter=HSys.mapIOTable.find(HSys.SuckPtr[j].Suck[iR][iC].OnSw.Name);
                if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
                {
                    bHasSuckOn=true;
                    iSuOn=atoi(HSys.mapIOTable[HSys.SuckPtr[j].Suck[iR][iC].OnSw.Name].c_str());
                }
                else
                {
                    S.sprintf("Can not find sucker on %s", HSys.SuckPtr[j].Suck[iR][iC].OnSw.Name.c_str());
    //                continue;
                }

                HSys.mapIOTableIter=HSys.mapIOTable.find(HSys.SuckPtr[j].Suck[iR][iC].OffSw.Name);
                if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
                {
                    bHasSuckOff=true;
                    iSuOff=atoi(HSys.mapIOTable[HSys.SuckPtr[j].Suck[iR][iC].OffSw.Name].c_str());
                }
                else
                {
                    S.sprintf("Can not find sucker off %s", HSys.SuckPtr[j].Suck[iR][iC].OffSw.Name.c_str());
    //                continue;
                }

                if(HSys.IOTable[iSu]->iEnable==0)
                    HSys.SuckPtr[j].Suck[iR][iC].Enable=false;
                else
                    HSys.SuckPtr[j].Suck[iR][iC].Enable=true;

                HSys.SuckPtr[j].Suck[iR][iC].EnableAtDataBase=HSys.SuckPtr[j].Suck[iR][iC].Enable;
                HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output=new TMyMN200_IO;

                if(bHasSuckOn)
                {
                    HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->ISABase=HSys.IOTable[iSuOn]->iISABase;
                    if(HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->ISABase==eMotionNet)
                        HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->SetPortInformation(HSys.IOTable[iSuOn]->iLane, HSys.IOTable[iSuOn]->sIP, HSys.IOTable[iSuOn]->iPort, HSys.IOTable[iSuOn]->iBit);
                    else
                        HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->SetPortInformation(AnsiString(""), HSys.IOTable[iSuOn]->iPort, HSys.IOTable[iSuOn]->iBit);

                    HSys.SuckPtr[j].Suck[iR][iC].OnSw.Type      =HSys.IOTable[iSuOn]->iInType;
                    HSys.SuckPtr[j].Suck[iR][iC].OnSw.Card      =HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetCard();
                    HSys.SuckPtr[j].Suck[iR][iC].OnSw.Port      =HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetPort();
                    HSys.SuckPtr[j].Suck[iR][iC].OnSw.Bit       =HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetBit();
                    HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->SetHint(HSys.IOTable[iSuOn]->IOPos);
                }

                HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output=new TMyMN200_IO;
                if(bHasSuckOff)
                {
                    HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->ISABase=HSys.IOTable[iSuOff]->iISABase;
                    if(HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->ISABase==eMotionNet)
                        HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->SetPortInformation(HSys.IOTable[iSuOff]->iLane, HSys.IOTable[iSuOff]->sIP, HSys.IOTable[iSuOff]->iPort, HSys.IOTable[iSuOff]->iBit);
                    else
                        HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->SetPortInformation(AnsiString(""), HSys.IOTable[iSuOff]->iPort, HSys.IOTable[iSuOff]->iBit);
                    HSys.SuckPtr[j].Suck[iR][iC].OffSw.Card      =HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetCard();
                    HSys.SuckPtr[j].Suck[iR][iC].OffSw.Port      =HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetPort();
                    HSys.SuckPtr[j].Suck[iR][iC].OffSw.Bit       =HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetBit();
                    HSys.SuckPtr[j].Suck[iR][iC].OffSw.Type      =HSys.IOTable[iSuOff]->iInType;

                    HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->SetHint(HSys.IOTable[iSuOff]->IOPos);
                }

                HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input=new TMyMN200_IO;
                if(bHasSuckSen)
                {
                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Tag =iR*10+iC;
                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->ISABase=HSys.IOTable[iSu]->iISABase;
                    if(HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->ISABase==eMotionNet)
                        HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->SetPortInformation(HSys.IOTable[iSu]->iLane, HSys.IOTable[iSu]->sIP, HSys.IOTable[iSu]->iPort, HSys.IOTable[iSu]->iBit);
                    else
                        HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->SetPortInformation(AnsiString(""), HSys.IOTable[iSu]->iPort, HSys.IOTable[iSu]->iBit);

                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Card      =HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetCard();
                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Port      =HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetPort();
                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Bit       =HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetBit();
                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Type       =HSys.IOTable[iSu]->iInType;

                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->SetHint(HSys.IOTable[iSu]->IOPos);

                    //Enable-----
                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Enable   =HSys.IOTable[iSu]->iEnable;
                    HSys.SuckPtr[j].Suck[iR][iC].Enable          =HSys.IOTable[iSu]->iEnable;
                    HSys.SuckPtr[j].Suck[iR][iC].OnSw.Enable     =HSys.SuckPtr[j].Suck[iR][iC].Enable;
                    HSys.SuckPtr[j].Suck[iR][iC].OffSw.Enable    =HSys.SuckPtr[j].Suck[iR][iC].Enable;
    //                HSys.SuckPtr[j].Suck[iR][iC].Sensor.CardModal=S;
                }

                HSys.SuckPtr[j].Suck[iR][iC].OnAlarmTime     =HSys.IOTable[iSu]->iOnAlarmTime;
                HSys.SuckPtr[j].Suck[iR][iC].OffAlarmTime    =HSys.IOTable[iSu]->iOffAlarmTime;
                HSys.SuckPtr[j].Suck[iR][iC].OnDelayTime     =HSys.IOTable[iSu]->iOnDelayTime;
                if(HSys.SuckPtr[j].Suck[iR][iC].OnDelayTime<100)
                    HSys.SuckPtr[j].Suck[iR][iC].OnDelayTime =100;              //20130613 wei       tryrun
                HSys.SuckPtr[j].Suck[iR][iC].OffDelayTime    =HSys.IOTable[iSu]->iOffDelayTime;
                if(HSys.SuckPtr[j].Suck[iR][iC].OffDelayTime<100)
                    HSys.SuckPtr[j].Suck[iR][iC].OffDelayTime=100;              //20130613 wei       tryrun

                #ifdef SOFT_SIMULATE                                            //2012-06-16  Dell Modify
                    HSys.SuckPtr[j].Suck[iR][iC].Enable=false;
                #endif
                if(HSys.SuckPtr[j].Suck[iR][iC].Enable==false)
                {
                    HSys.SuckPtr[j].Suck[iR][iC].OnDelayTime     =0;
                    HSys.SuckPtr[j].Suck[iR][iC].OffDelayTime    =0;
                    HSys.SuckPtr[j].Suck[iR][iC].Enable          =false;
                    //20111214 Daver add
                    HSys.SuckPtr[j].Suck[iR][iC].Sensor.Enable   =false;
                    HSys.SuckPtr[j].Suck[iR][iC].OnSw.Enable     =false;
                    HSys.SuckPtr[j].Suck[iR][iC].OffSw.Enable    =false;
                    //----------------
                }
            }
        }
    }

    for(int i=0; i<iTotalSucker; i++)
        SuckPtr[i].Tag=i;
}
//==============================================================================
//  016
//  使用者依不同機器修改 _                                                      // need edit by user //
//==============================================================================
void __fastcall TDataModule1::SpecificSetupForSuckerParameterExecute(
      TObject *Sender)
{
    HSys.Suck.AutoArmSuck.initMyKitSuck("AutoArmSuck", "palSortArm1", 4, 2);
    HSys.Suck.MagArmSuck.initMyKitSuck("MagArmSuck", "palSortArm2", 4, 2);

    HSys.Suck.TrayArmSuck.initMyKitSuck("TrayArmSuck", "palTrayArm", 1, 1);
    HSys.Suck.MagTrayArmSuck.initMyKitSuck("MagTrayArmSuck", "palMagTrayArm", 1, 1);
}
//==============================================================================
//  017
//  使用者依不同機器修改 ,當第一次執行必須把真空汽缸或部份IO做初始化            // need edit by user //
//==============================================================================
void __fastcall TDataModule1::Initial_IO_SetupExecute(TObject *Sender)
{
    static bool bFirstEnter=true;
    if(bFirstEnter==true)
    {
        bFirstEnter=false;
        HSys.Sw.SwMotorRelay.On();                                              // ben disable 20111018 //
        AllBreakFree();
    }
}
//==============================================================================
//  電控原件初始化
//  不須改變 _
//==============================================================================
void SYSTEM_MODULAR::InitialHardwareNameAndLoadDatabaseIO()
{
    LoadIoData();
    // cylinder
    DataModule1->InitialCylinderNameExecute(NULL);               // 005
    LoadCylinderParameterFromDataBase();                         // 006
//    DataModule1->SpecificSetupForCylinderParameterExecute(NULL); // 007

    // sensor
    DataModule1->InitialSensorNameExecute(NULL);                 // 008
    LoadSensorParameterFromDataBase();                           // 009
    DataModule1->SpecificSetupForSensorParameterExecute(NULL);   // 010

    //switch
    DataModule1->InitialSwitchNameExecute(NULL);                 // 011
    LoadSwitchParameterFromDataBase();                           // 012
//    DataModule1->SpecificSetupForSwitchParameterExecute(NULL);   // 013

    //sucker
    DataModule1->InitialSuckerNameExecute(NULL);                 // 014
    DataModule1->SpecificSetupForSuckerParameterExecute(NULL);   // 016
    LoadSuckerParameterFromDataBase();                           // 015
}
//==============================================================================
void SYSTEM_MODULAR::InitialHardwareNameAndLoadDatabaseMotor()
{
    // motor
    DataModule1->InitialMotorNameExecute(NULL);                  // 001
    LoadMotorParameterFromDataBase();                            // 002
    InitialVMotorParameter();                                    // 003
    DataModule1->SpecificSetupForMotorParameterExecute(NULL);    // 004
}
//==============================================================================
void SYSTEM_MODULAR::InitialHardwareNameAndLoadDatabase()
{
    //Sam 20170628 (jou) : 若有使用 "Database Desktop" 軟體直接修改資料庫，可能會造成 "PDOXUSRS.NET" 資料異常導致 Network initialization failed 而無法開啟程式。
    //開啟 DataBase 都會自動產生此文件，所以執行資料庫前先將檔案刪除。
    //-->
    AnsiString asFileName="C:\\PDOXUSRS.NET";
    if (FileExists(asFileName))
        DeleteFile(asFileName);
    //<--
    InitialHardwareNameAndLoadDatabaseMotor();
    InitialHardwareNameAndLoadDatabaseIO();
    #ifndef SOFT_SIMULATE                                                       //Maxwu 20230727 mark test
    if(Enable_PLCSafety_IO)                                                     //Austin 20190531.01 增加PLC_IO模組及安全相關IO. //Jason 20230619 增加安全PLC部分
    {
        bScanSlave[0]=true;
        InitPLCIO("172.16.8.120", 502);                                         //固定
    }
    #endif
}
//==============================================================================
void SYSTEM_MODULAR::StopAllMotor()                                             // all motor stop //
{
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        if(HSys.MotPtr[i]!=NULL &&
           HSys.MotPtr[i]->GetEnable()==true)
        {
            HSys.MotPtr[i]->Stop();                                             //wei 20131003   Stop();
        }
    }

    HSys.Sw.SwACEmpty1CCW.Off();
    HSys.Sw.SwACEmpty1CW.Off();

    HSys.Sw.SwACEmpty2CCW.Off();
    HSys.Sw.SwACEmpty2CW.Off();
    HSys.Sw.SwACLoaderCCW.Off();
    HSys.Sw.SwACLoaderCW.Off();

    HSys.Sw.SwACAutoRow1CW.Off();
    HSys.Sw.SwACAutoRow1CCW.Off();
    HSys.Sw.SwACAutoRow2CW.Off();
    HSys.Sw.SwACAutoRow2CCW.Off();
    HSys.Sw.SwACAutoRow3CW.Off();
    HSys.Sw.SwACAutoRow3CCW.Off();
    HSys.Sw.SwACAutoRow4CW.Off();
    HSys.Sw.SwACAutoRow4CCW.Off();

    HSys.Sw.SwACShiftStageCCW.Off();
    HSys.Sw.SwACShiftStageCW.Off();

    HSys.Sw.SwAutoRow1Vib1.Off();
    HSys.Sw.SwAutoRow1Vib2.Off();
    HSys.Sw.SwAutoRow2Vib1.Off();
    HSys.Sw.SwAutoRow2Vib2.Off();
    HSys.Sw.SwAutoRow3Vib1.Off();
    HSys.Sw.SwAutoRow3Vib2.Off();
    HSys.Sw.SwAutoRow4Vib1.Off();
    HSys.Sw.SwAutoRow4Vib2.Off();
}
//==============================================================================
void SYSTEM_MODULAR::DecStopAllMotor()                                             // all motor stop //
{
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        if(HSys.MotPtr[i]!=NULL &&
           HSys.MotPtr[i]->GetEnable()==true)
        {
            HSys.MotPtr[i]->DecStop();
        }
    }

    HSys.Sw.SwACEmpty1CCW.Off();
    HSys.Sw.SwACEmpty1CW.Off();

    HSys.Sw.SwACEmpty2CCW.Off();
    HSys.Sw.SwACEmpty2CW.Off();
    HSys.Sw.SwACLoaderCCW.Off();
    HSys.Sw.SwACLoaderCW.Off();

    HSys.Sw.SwACAutoRow1CW.Off();
    HSys.Sw.SwACAutoRow1CCW.Off();
    HSys.Sw.SwACAutoRow2CW.Off();
    HSys.Sw.SwACAutoRow2CCW.Off();
    HSys.Sw.SwACAutoRow3CW.Off();
    HSys.Sw.SwACAutoRow3CCW.Off();
    HSys.Sw.SwACAutoRow4CW.Off();
    HSys.Sw.SwACAutoRow4CCW.Off();

    HSys.Sw.SwACShiftStageCCW.Off();
    HSys.Sw.SwACShiftStageCW.Off();

    HSys.Sw.SwAutoRow1Vib1.Off();
    HSys.Sw.SwAutoRow1Vib2.Off();
    HSys.Sw.SwAutoRow2Vib1.Off();
    HSys.Sw.SwAutoRow2Vib2.Off();
    HSys.Sw.SwAutoRow3Vib1.Off();
    HSys.Sw.SwAutoRow3Vib2.Off();
    HSys.Sw.SwAutoRow4Vib1.Off();
    HSys.Sw.SwAutoRow4Vib2.Off();
}
//==============================================================================
// 機器動作區                                                                   // need edit by user //
//==============================================================================
TQPF_Timer ShowBinCountInterval;
void __fastcall TDataModule1::Timer1Timer(TObject *Sender)
{
    static bool bRun=false;
    if(bRun)
        return;
    bRun=true;
    #ifndef SOFT_SIMULATE   //Sam 20240515 : 修正 Dummy Run
    fComPort->Spin();
    #endif
    if(ShowBinCountInterval.Off())
    {
        fMain->ShowBinCount();                                              //Sam 20170907 (jou): 資料清空之後 UI 更新
        ShowBinCountInterval.SetSecAndOn(10.0);
    }
    bRun=false;
}
//==============================================================================
void MyFunctionA::Update()
{
    //------------------------------Setup---------------------------------------
    iA01_LoaderTraySpeed        =atoi(fSetup->edLDSpeed->Text.c_str());
    iA01_LoaderTray2ndSpeed     =atoi(fSetup->edLD2ndSpeed->Text.c_str());

    iA01_Empty1TraySpeed        =atoi(fSetup->edEmpty1Speed->Text.c_str());
    iA01_Empty1Tray2ndSpeed     =atoi(fSetup->edEmpty12ndSpeed->Text.c_str());

    iA01_Empty2TraySpeed        =atoi(fSetup->edEmpty2Speed->Text.c_str());
    iA01_Empty2Tray2ndSpeed     =atoi(fSetup->edEmpty22ndSpeed->Text.c_str());

    iA01_TransferTraySpeed      =atoi(fSetup->edTransferSpeed->Text.c_str());
    iA01_TransferTray2ndSpeed   =atoi(fSetup->edTransfer2ndSpeed->Text.c_str());

    iA01_Track1TraySpeed        =atoi(fSetup->edTrack1Speed->Text.c_str());
    iA01_Track1Tray2ndSpeed     =atoi(fSetup->edTrack12ndSpeed->Text.c_str());

    iA01_Track2TraySpeed        =atoi(fSetup->edTrack2Speed->Text.c_str());
    iA01_Track2Tray2ndSpeed     =atoi(fSetup->edTrack22ndSpeed->Text.c_str());

    iA01_Track3TraySpeed        =atoi(fSetup->edTrack3Speed->Text.c_str());
    iA01_Track3Tray2ndSpeed     =atoi(fSetup->edTrack32ndSpeed->Text.c_str());

    iA01_Track4TraySpeed        =atoi(fSetup->edTrack4Speed->Text.c_str());
    iA01_Track4Tray2ndSpeed     =atoi(fSetup->edTrack42ndSpeed->Text.c_str());
}
//---------------------------------------------------------------------------
void MyFunctionB::Update()
{
    asHandlerID =fMaintenance->edN01_MachineID->Text;
    asFactory   =fMaintenance->edN01_FactoryID->Text;
    asSerialNo  =fMaintenance->edSerialNo->Text;

    asUserID    =fMaintenance->edN02_ID->Text;                                  //JerryYang 20240610 : ASECL download bin tray map
    asPassword  =fMaintenance->ed_N02_PWD->Text;
    asHost      =fMaintenance->ed_N02_Host->Text;
    asFTPPath   =fMaintenance->ed_N02_Path->Text;
    asFTPPathUpload=fMaintenance->ed_N02_Path_Upload->Text;

    bN03_UploadTrayData=fMaintenance->cbN03->Checked;
    asTrayPathUpload=fMaintenance->GetUnloaderTrayDataFilePath();

    bN04_RunCheck=fMaintenance->cbN04->Checked;             //JerryYang 20250106 : Run check


    fMain->statMain->Panels->Items[1]->Text = asModel;
    fMain->statMain->Panels->Items[2]->Text = asHandlerID;                      //Steven 20240627 : 顯示機號
    fMain->statMain->Panels->Items[3]->Text = asSerialNo;
}
//---------------------------------------------------------------------------
void MyFunctionC::Update()
{

}
//---------------------------------------------------------------------------
void MyFunctionN::Update()
{
    iN02_JHTFormatLoadSelect=fMaintenance->rgN02->ItemIndex;
    bN03_OLP_Log            =fMaintenance->cbN03_Log->Checked;
    sN03_OLP_IP             =fMaintenance->edN03_IP->Text;
    sN03_OLP_Port           =fMaintenance->edN03_Port->Text;
    sN03_LotStartPatch      =fMaintenance->edN03_LotStartPatch->Text;
    sN03_LotEndPatch        =fMaintenance->edN03_LotEndPatch->Text;
}
//---------------------------------------------------------------------------
void MyFunctionT::Update()
{
    bT01_AutoRow1Vib1   =fMaintenance->cbT01_AutoRow1Vib1->Checked;
    bT01_AutoRow1Vib2   =fMaintenance->cbT01_AutoRow1Vib2->Checked;
    bT01_AutoRow2Vib1   =fMaintenance->cbT01_AutoRow2Vib1->Checked;
    bT01_AutoRow2Vib2   =fMaintenance->cbT01_AutoRow2Vib2->Checked;
    bT01_AutoRow3Vib1   =fMaintenance->cbT01_AutoRow3Vib1->Checked;
    bT01_AutoRow3Vib2   =fMaintenance->cbT01_AutoRow3Vib2->Checked;
    bT01_AutoRow4Vib1   =fMaintenance->cbT01_AutoRow4Vib1->Checked;
    bT01_AutoRow4Vib2   =fMaintenance->cbT01_AutoRow4Vib2->Checked;
    iT02_MagTraySource  =fMaintenance->rgT02_MagTraySource->ItemIndex;
    iT03_AutoBinSet     =fMaintenance->rgT03->ItemIndex;
    iT04_BinDisptype    =fMaintenance->rgT04_BinDisptype->ItemIndex;
    iT05_BinDispComHardwareType=fMaintenance->rgT05_ComportHardware->ItemIndex;
    if(iT04_BinDisptype<0 || iT04_BinDisptype>1)
        iT04_BinDisptype=0;
    if(iT05_BinDispComHardwareType<0 || iT05_BinDispComHardwareType>1)
        iT05_BinDispComHardwareType=1;
    fMaintenance->rgT04_BinDisptype->ItemIndex=iT04_BinDisptype;
    fMaintenance->rgT05_ComportHardware->ItemIndex=iT05_BinDispComHardwareType; //JerryYang 20250621 : Auto跟Magazine使用不同COM Port
    iA01_InputDataFormat=fMaintenance->rgA01_InputDataFormat->ItemIndex;
    iT06_AutoRowType    =fMaintenance->rgT06_AutoRowType->ItemIndex;            //KenHsieh 20250613 : add T06 Auto Row Type
    iT07_MGZDirection   =fMaintenance->rgT07_MGZDirection->ItemIndex;           //KenHsieh 20250614 : add 判斷Magazine direction
    iT08_Empty1PushTray =fMaintenance->rgT08_Empty1PushTray->ItemIndex;
    iT09_TrayIDReadPos  =fMaintenance->rgT09_TrayIDReadPos->ItemIndex;

    if(CUSTOMER_CODE==CC_WINSTEK)
    {
        iA01_InputDataFormat=LoaderModule->eWinstekFormat;
        fMaintenance->rgA01_InputDataFormat->ItemIndex=iA01_InputDataFormat;
    }
    bA02_LoginTimeout           =fMaintenance->cbA02->Checked;
    iA02_LoginTimeout           =atoi(fMaintenance->edA02->Text.c_str());
    bA03_ChkTrollyHasTray       =fMaintenance->cbA03->Checked;
}
//---------------------------------------------------------------------------
void MyFunctionS::Update()                                                      //Sam 20240619 : 速度獨立設定
{
    bS01_MagX           =fMaintenance->cbS01_MagX->Checked;
    iS01_MagX           =atoi(fMaintenance->edS01_MagX->Text.c_str());
    bS01_SortArmTrayZ   =fMaintenance->cbS01_SortArmTrayZ->Checked;
    iS01_SortArmTrayZ   =atoi(fMaintenance->edS01_SortArmTrayZ->Text.c_str());
    bS02_SearchShortestMode=fMaintenance->chkS02->Checked;
    bS03_MagArmXYPreMove=fMaintenance->chkS03_MagArmXYPreMove->Checked;         //Sam 20240727 : Magazine 換 Tray 時XY提前移動
    bS03_ShortDistanceMove= fMaintenance->chkS03_ShortDistanceMove->Checked;    //Sam 20240727 : 新增短距離加減速減半
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::SortArm1Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal) // || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            SortArmPara->DoSortArm1(P->Tag);
            fMain->sr->LogTask((int)cStateRecord::eLNT_SortArm1Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::SortArm2Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            MagArmPara->DoMagArm(P->Tag);
            fMain->sr->LogTask((int)cStateRecord::eLNT_SortArm2Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::LoaderExecute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            LoaderModule->DoLoader(P->Tag);
            fMain->sr->LogTask((int)cStateRecord::eLNT_LoaderExecute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionTrack1Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            LoaderModule->DoTrackAreaToStoreTrayArm(P->Tag, eTrack1);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionTrack1Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionTrack2Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            LoaderModule->DoTrackAreaToStoreTrayArm(P->Tag, eTrack2);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionTrack2Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionTrack3Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            LoaderModule->DoTrackAreaToStoreTrayArm(P->Tag, eTrack3);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionTrack3Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionTrack4Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            LoaderModule->DoTrackAreaToStoreTrayArm(P->Tag, eTrack4);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionTrack4Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionEmpty1Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            LoaderModule->DoEmpty1(P->Tag);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionEmpty1Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionEmpty1_TrackExecute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            LoaderModule->DoSupportAuto(P->Tag);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionEmpty1_TrackExecute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionEmpty2Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            LoaderModule->DoEmpty2(P->Tag);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionEmpty2Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionStoreArmExecute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            TrayArmPara->DoStoreArm(P->Tag);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionStoreArmExecute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionMag2Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            MagazineModule2->DoMagazine(P->Tag);
            fMain->sr->LogTask((int)cStateRecord::eLNT_ActionMag2Execute, P->Tag);
        }
    }
}
//---------------------------------------------------------------------------
__fastcall MyAlarmCodeStruct::MyAlarmCodeStruct()
{
    AlarmCode   ="";
    AlarmType   =eOther;
    E_ErrMessage="";
    C_ErrMessage="";
    E_Description="";
    C_Description="";
    FlushPanelName="";
}
//---------------------------------------------------------------------------
__fastcall MyAlarmCodeStruct::MyAlarmCodeStruct(AnsiString _AlarmCode, int _AlarmType, AnsiString _E_ErrMessage, AnsiString _C_ErrMessage, AnsiString _E_Description, AnsiString _C_Description, AnsiString _FlushPanelName)
{
    AlarmCode   =_AlarmCode;
    AlarmType   =_AlarmType;
    E_ErrMessage=_E_ErrMessage;
    C_ErrMessage=_C_ErrMessage;
    E_Description=_E_Description;
    C_Description=_C_Description;
    FlushPanelName=_FlushPanelName;
}
//---------------------------------------------------------------------------
__fastcall MyAlarmCodeStruct::~MyAlarmCodeStruct()
{
//    E_Description->Clear();
//    E_Description->Clear();
//    delete E_Description;
//    delete C_Description;
}
//------------------------------------------------------------------------------
AnsiString MyAlarmCodeStruct::CommaText()
{
    AnsiString  Str=AlarmCode               +AnsiString(",")+
                    AnsiString(AlarmType)   +AnsiString(",")+
                    E_ErrMessage            +AnsiString(",")+
                    C_ErrMessage            +AnsiString(",")+
                    E_Description           +AnsiString(",")+
                    C_Description;
    return Str;
}
//------------------------------------------------------------------------------
__fastcall MyBinToTrayStruct::MyBinToTrayStruct()
{
    ResetData();
}
//------------------------------------------------------------------------------
__fastcall MyBinToTrayStruct::MyBinToTrayStruct(int _Bin)
{
    iBin=_Bin;
    ConvertBinToAuto(_Bin);
}
//------------------------------------------------------------------------------
void MyBinToTrayStruct::ResetData()
{
    iWhichAuto  =eBinNotUse;
    iBin        =0;
    iAutoRow    =eTrackNotUse;
    iAutoCol    =0;
}
//------------------------------------------------------------------------------
void MyBinToTrayStruct::ConvertAutoToBin(int _Auto, bool bReset)
{
    iBin=0;
    iWhichAuto=_Auto;
    if(_Auto<=eBinNotUse ||
       _Auto>=eTrayCount)
    {
        ResetData();
    }
    else
    {
        if(CosFunction.bUseBinAutoConvert)
        {
            iBin=cBinAutoTrans.GetBinbyAuto(iWhichAuto);
        }
        else
        {
            for(int iB=1; iB<TEST_MAX_BIN; iB++)
            {
                if(_Auto==BinSelect[0].iCategData[iB])
                    iBin=iB;
            }
        }

        if((iBin<=0 && bReset==true) || iBin>=TEST_MAX_BIN)
        {
            ResetData();
        }
        else if(iWhichAuto>eBinNotUse &&
                iWhichAuto<=eAuto20)
        {
            iAutoRow=(iWhichAuto-eTrack1)/eTrackColTotal+eTrack1;
            iAutoCol=(iWhichAuto-eTrack1)%eTrackColTotal;
        }
        else if(iWhichAuto>=eMag1_01 &&
                iWhichAuto<=eMag1_27)
        {
            iAutoRow=eTrackMgz1;
            iAutoCol=iWhichAuto-eMag1_01;
        }
        else if(iWhichAuto>=eMag2_01 &&
                iWhichAuto<=eMag2_27)
        {
            iAutoRow=eTrackMgz2;
            iAutoCol=iWhichAuto-eMag2_01;
        }
        else if(iWhichAuto>=eMag3_01 &&
                iWhichAuto<=eMag3_27)
        {
            iAutoRow=eTrackMgz3;
            iAutoCol=iWhichAuto-eMag3_01;
        }
        else if(iWhichAuto==eFix1)
        {
            iAutoRow=eFixRow1;
            iAutoCol=0;
        }
        else if(iWhichAuto==eFix2)
        {
            iAutoRow=eFixRow2;
            iAutoCol=0;
        }
//        else
//        {
//            ResetData();
//        }
    }
}
//------------------------------------------------------------------------------
void MyBinToTrayStruct::ConvertBinToAuto(int _Bin)
{
    if(_Bin<=0 || iBin>=TEST_MAX_BIN)
    {
        ResetData();
    }
    else
    {
        iBin=_Bin;
        if(CosFunction.bUseBinAutoConvert)
        {
            iWhichAuto=cBinAutoTrans.GetAutobyBin(iBin);
        }
        else
        {
            iWhichAuto=BinSelect[0].iCategData[iBin];
        }
        if(iWhichAuto<=eBinNotUse ||
           iWhichAuto>=eTrayCount)
        {
            ResetData();
        }
        else if(iWhichAuto<=eAuto20)
        {
            iAutoRow=(iWhichAuto-eAuto1)/eTrackColTotal+eTrack1;
            iAutoCol=(iWhichAuto-eAuto1)%eTrackColTotal;
        }
        else if(iWhichAuto>=eMag1_01 &&
                iWhichAuto<=eMag1_27)
        {
            iAutoRow=eTrackMgz1;
            iAutoCol=iWhichAuto-eMag1_01;
        }
        else if(iWhichAuto>=eMag2_01 &&
                iWhichAuto<=eMag2_27)
        {
            iAutoRow=eTrackMgz2;
            iAutoCol=iWhichAuto-eMag2_01;
        }
        else if(iWhichAuto>=eMag3_01 &&
                iWhichAuto<=eMag3_27)
        {
            iAutoRow=eTrackMgz3;
            iAutoCol=iWhichAuto-eMag3_01;
        }
        else if(iWhichAuto==eFix1)
        {
            iAutoRow=eFixRow1;
            iAutoCol=0;
        }
        else if(iWhichAuto==eFix2)
        {
            iAutoRow=eFixRow2;
            iAutoCol=0;
        }
    }
}
//------------------------------------------------------------------------------
void MyBinToTrayStruct::ConvertRowColToBin(int _iRow, int _iCol)
{
    if(_iRow<=eTrackNotUse ||
       _iRow>=eTrackCount  ||
       _iCol<0             ||
       _iCol>=eMagTotal)
    {
        ResetData();
    }
    else if(_iRow<=eTrack4 && _iCol>=5)                                         //Auto軌道只有5個col
    {
        ResetData();
    }
    else if(_iRow<=eTrackMgz3 && _iCol>=eMagTotal)                              //Mag軌道只有eMagTotal個col
    {
        ResetData();
    }
    else
    {
        iBin        =0;
        iAutoRow    =_iRow;
        iAutoCol    =_iCol;

        if(iAutoRow==eTrackMgz1)
        {
            iWhichAuto=iAutoCol+eMag1_01;
        }
        else if(iAutoRow==eTrackMgz2)
        {
            iWhichAuto=iAutoCol+eMag2_01;
        }
        else if(iAutoRow==eTrackMgz3)
        {
            iWhichAuto=iAutoCol+eMag3_01;
        }
        else if(iAutoRow==eFixRow1)
        {
            iWhichAuto=eFix1;
            iAutoCol  =0;
        }
        else if(iAutoRow==eFixRow2)
        {
            iWhichAuto=eFix2;
            iAutoCol  =0;
        }
        else if(iAutoRow>=eTrack1 &&
                iAutoRow<=eTrack4)
        {
            iWhichAuto=(iAutoRow-eTrack1)*eTrackColTotal+iAutoCol+eAuto1;
        }
        else
        {
            iWhichAuto=eBinNotUse;
        }

        if(iWhichAuto<=eBinNotUse ||
           iWhichAuto>=eTrayCount)
        {
            ResetData();
        }
        else
        {
            if(CosFunction.bUseBinAutoConvert)
            {
                iBin=cBinAutoTrans.GetBinbyAuto(iWhichAuto);
            }
            else
            {
                for(int iB=1; iB<TEST_MAX_BIN; iB++)
                {
                    if(iWhichAuto==BinSelect[0].iCategData[iB])
                        iBin=iB;
                }
            }
            if(iBin<=0 || iBin>=TEST_MAX_BIN)
            {
                ResetData();
            }
        }
    }
}
//------------------------------------------------------------------------------
cBinAutoConvert::cBinAutoConvert()
{
    Clear();
}
//------------------------------------------------------------------------------
cBinAutoConvert::~cBinAutoConvert()
{
    Clear();
}
//------------------------------------------------------------------------------
void cBinAutoConvert::Clear()
{
    mapBinToAuto.clear();
    mapAutoToBin.clear();
    iTotalSize=0;
}
//------------------------------------------------------------------------------
void cBinAutoConvert::AddBinAuto(int iBin, int iAuto)
{
    mapBinToAuto[iBin]=iAuto;
    mapAutoToBin[iAuto]=iBin;
    iTotalSize=mapBinToAuto.size();
}
//------------------------------------------------------------------------------
int cBinAutoConvert::GetBinbyAuto(int iAuto)
{
    int icount=mapAutoToBin.count(iAuto);
//    int isize=mapBinToAuto.size();
    if(icount>0)
    {
        return mapAutoToBin[iAuto];
    }
    return 0;
}
//------------------------------------------------------------------------------
void cBinAutoConvert::SetBinbyAuto(int iSetBin,int iByAuto)
{
    int icount=mapAutoToBin.count(iByAuto);
//    int isize=mapBinToAuto.size();
    if(icount>0)
    {
        int oldBin=mapAutoToBin[iByAuto];
        mapAutoToBin.erase(iByAuto);
        mapBinToAuto.erase(oldBin);
        AddBinAuto(iSetBin,iByAuto);
    }
}
//------------------------------------------------------------------------------
int cBinAutoConvert::GetAutobyBin(int iBin)
{
    if(mapBinToAuto.count(iBin)>0)
    {
        return mapBinToAuto[iBin];
    }
    return 0;
}
//------------------------------------------------------------------------------
void cBinAutoConvert::SetAutobyBin(int iSetAuto,int iByBin)
{
    if(mapBinToAuto.count(iByBin)>0)
    {
        int oldArea=mapBinToAuto[iByBin];
        mapAutoToBin.erase(oldArea);
        mapBinToAuto.erase(iByBin);
        AddBinAuto(iByBin,iSetAuto);
    }
}
//------------------------------------------------------------------------------
int cBinAutoConvert::GetMapCount()
{
    return mapBinToAuto.size();
}
//------------------------------------------------------------------------------
AnsiString cBinAutoConvert::SaveToJson()
{
    cJSON* root = cJSON_CreateObject();
    if (!root)
        return "";
    cJSON* binToAutoArray = cJSON_CreateArray();
    if(!binToAutoArray)
    {
        cJSON_Delete(root);
        return "";
    }
    for (std::map<int, int>::iterator it = mapBinToAuto.begin(); it != mapBinToAuto.end();it++)
    {
        cJSON* item = cJSON_CreateObject();
        cJSON_AddNumberToObject(item, "bin", it->first);
        cJSON_AddNumberToObject(item, "auto", it->second);
        cJSON_AddItemToArray(binToAutoArray, item);
    }
    cJSON_AddItemToObject(root, "binToAuto", binToAutoArray);
    char* jsonString = cJSON_PrintUnformatted(root);
    AnsiString result = jsonString ? AnsiString(jsonString) : AnsiString(); 
    cJSON_free(jsonString);                                                     // 重要！
    cJSON_Delete(root);
    return result;
}
//------------------------------------------------------------------------------
void cBinAutoConvert::LoadFromJson(AnsiString sFilePath)
{
    Clear();
    if(sFilePath.IsEmpty())
        return;
    char* file_buf=ReadDataFromFile(sFilePath);
    cJSON* root = cJSON_Parse(file_buf);
    if (!root)
        return;
    cJSON* binToAutoArray = cJSON_GetObjectItem(root, "binToAuto");
    if (binToAutoArray && cJSON_IsArray(binToAutoArray))
    {
        for (int i = 0; i < cJSON_GetArraySize(binToAutoArray); ++i)
        {
            cJSON* item = cJSON_GetArrayItem(binToAutoArray, i);
            if(!cJSON_IsObject(item))
                continue;

            cJSON* bin = cJSON_GetObjectItem(item, "bin");
            cJSON* autoVal = cJSON_GetObjectItem(item, "auto");
            if (bin && autoVal && cJSON_IsNumber(bin) && cJSON_IsNumber(autoVal))
            {
                int jbin  = bin->valueint;
                int jauto_ = autoVal->valueint;
                mapBinToAuto[jbin] = jauto_;
            }
        }
    }
    for (std::map<int, int>::iterator it = mapBinToAuto.begin(); it != mapBinToAuto.end();it++)
    {
        mapAutoToBin[it->second]=it->first;
    }
    cJSON_Delete(root);                                                         // 重要！一定要放
}
//------------------------------------------------------------------------------
void __fastcall TDataModule1::ActionMag1Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            if(MAGAZINE_CNT>1)
            {
                MagazineModule1->DoMagazine(P->Tag);
                fMain->sr->LogTask((int)cStateRecord::eLNT_ActionMag1Execute, P->Tag);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::ActionMag3Execute(TObject *Sender)
{
    if(HSys.Sys.RunMode==Run_OneCycle || HSys.Sys.RunMode==Run_CleanOut || HSys.Sys.RunMode==Run_Normal || HSys.Sys.RunMode==Run_TrayFeed)
    {
        TContainedAction *P;
        P=dynamic_cast<TContainedAction *>(Sender);
        if(P!=NULL)
        {
            if(MAGAZINE_CNT>2)
            {
                MagazineModule3->DoMagazine(P->Tag);
                fMain->sr->LogTask((int)cStateRecord::eLNT_ActionMag3Execute, P->Tag);
            }
        }
    }
}
//---------------------------------------------------------------------------

