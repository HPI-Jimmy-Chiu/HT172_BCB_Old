//---------------------------------------------------------------------------

#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "mySMCmotor.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#define MAX_SMC_CARD 16
#define MAX_LATCH_COUNT 1024

//-------------
// SmcWSetCtrlTypeIn : Sets the type of control input signal.
//-------------
const int SMC_CtrlIn_ALM            =0x01;
const int SMC_CtrlIn_INP            =0x02;
const int SMC_CtrlIn_SD             =0x04;
const int SMC_CtrlIn_LTC            =0x08;
const int SMC_CtrlIn_CTR            =0x10;
const int SMC_CtrlIn_CLR            =0x20;

//-------------
// SmcWSetCtrlTypeOut : Sets the type of control output signal
//-------------
const int SMC_CtrlOut_General       =0;
const int SMC_CtrlOut_AlarmClear    =1;
const int SMC_CtrlOut_ERC           =2;
const int SMC_CtrlOut_OutPulseSignal=3;
const int SMC_CtrlOut_EncoderSignal =4;
const int SMC_CtrlOut_HoldOffSignal =5;

//-------------
// SmcWSetCtrlInOutLog : Sets the logic of I/O control signal.  for Positive logic
//-------------
const int SMC_CtrlInOutLog_IN1      =0x0001;
const int SMC_CtrlInOutLog_IN2      =0x0002;
const int SMC_CtrlInOutLog_IN3      =0x0004;
const int SMC_CtrlInOutLog_IN4      =0x0008;
const int SMC_CtrlInOutLog_IN5      =0x0010;
const int SMC_CtrlInOutLog_IN6      =0x0020;
const int SMC_CtrlInOutLog_IN7      =0x0040;
const int SMC_CtrlInOutLog_LIM      =0x0080;
const int SMC_CtrlInOutLog_OUT1     =0x0100;
const int SMC_CtrlInOutLog_OUT2     =0x0200;
const int SMC_CtrlInOutLog_OUT3     =0x0400;

//-------------
// SmcWSetOrgLog : Sets the logic of origin input.
//-------------
const int SMC_OrgLog_ORG_Negative   =0x00;
const int SMC_OrgLog_ORG_Positive   =0x01;
const int SMC_OrgLog_Z_RisingEdge   =0x02;

//-------------
// SmcWSetOrgMode : Sets the mode of origin return (origin return motion pattern).
//-------------
    //-------------
    // LimitTurn  : Set whether perform limit turning over or not.
    //-------------
    const int SMC_OrgMode_LimitTurn_Off =0;
    const int SMC_OrgMode_LimitTurn_On  =1;
    const int SMC_OrgMode_LimitReversed =2; //碰到極限就反轉
    const int SMC_OrgMode_LimitAsOrg    =3; //使用極限當歸零點
    //-------------
    // OrgType  : Set whether use Z-phase or not.
    //-------------
    const int SMC_OrgMode_OrgType_UnUseZ=0;
    const int SMC_OrgMode_OrgType_UseZ  =1;
    //-------------
    // EndDir  : Set origin rushing direction (End direction to move to origin) when the origin returned.
    //-------------
    const int SMC_OrgMode_EndDir_UnSpec =0;
    const int SMC_OrgMode_EndDir_PosCW  =1;
    const int SMC_OrgMode_EndDir_NegCCW =2;
    //-------------
    // ZCount  : Set the number of Z-phase when the origin returned. The setting is invalid when "0: Unused" is selected for OrgType.
    //-------------
    const int SMC_OrgMode_NoUseZ        =0;
    const int SMC_OrgMode_ZCount        =1;

//-------------
// SmcWSetPulseType : Sets output mode of pulse.
//-------------
    //-------------
    // DirTimer : If the common pulse method is being set, when the direction changes depending on the change of DIR change,
    //            insert 200[u sec] wait (delay) before the pulse output. (Only used in the case of common pulse method)
    //-------------
    const int SMC_Pulse_DirTimerOn      =1;
    const int SMC_Pulse_DirTimerOff     =0;
    //-------------
    // PulseMode : Set pulse output mode.
    //-------------
    const int SMC_Pulse_OutNeg_DirHigh  =0;
    const int SMC_Pulse_OutPos_DirHigh  =1;
    const int SMC_Pulse_OutNeg_DirLow   =2;
    const int SMC_Pulse_OutPos_DirLow   =3;
    const int SMC_Pulse_2Pulse_Neg      =4;
    const int SMC_Pulse_2Pulse_Pos      =5;
    const int SMC_Pulse_PhaseDiff       =6;
    const int SMC_Pulse_PhaseDiffDelay  =7;

//-------------
// SmcWSetCounterMode : Sets the counter mode.
//-------------
    //-------------
    // ClearCntLtc : Set the type of counter you want to clear when the LTC signal changs from OFF to ON.
    //-------------
    const int SMC_ClearCntLtc_Off       =0;
    const int SMC_ClearCntLtc_OutPulse  =1;
    const int SMC_ClearCntLtc_Encoder   =2;
    const int SMC_ClearCntLtc_Both      =3;
    //-------------
    // LtcMode : Set the type of counter you want to latch when the LTC signal comes in.
    //-------------
    const int SMC_LtcMode_Off           =0;
    const int SMC_LtcMode_OutPulse      =1;
    const int SMC_LtcMode_Encoder       =2;
    const int SMC_LtcMode_Both          =3;
    //-------------
    // ClearCntClr : Set the type of counter you want to clear when the CLR signal changes from OFF to ON.
    //-------------
    const int SMC_ClearCntClr_Off       =0;
    const int SMC_ClearCntClr_OutPulse  =1;
    const int SMC_ClearCntClr_Encoder   =2;
    const int SMC_ClearCntClr_Both      =3;
    //-------------
    // ClrMode : ClrMode (Reserved) 0 fixed.
    //-------------
    const int SMC_ClrMode               =0;

//-------------
// SmcWSetFIFOLatchSrc : Set the source of latch counter and enable the external latch input signals.
//-------------
    //-------------
    // LatchAxisNo  : Specifies the axis number to latch
    // Enable       : Specifies the axis number to enable
    //-------------
    const int SMC_FIFOLtcSrc_Axis1      =0x01;
    const int SMC_FIFOLtcSrc_Axis2      =0x02;
    const int SMC_FIFOLtcSrc_Axis3      =0x04;
    const int SMC_FIFOLtcSrc_Axis4      =0x08;
    const int SMC_FIFOLtcSrc_Axis5      =0x10;
    const int SMC_FIFOLtcSrc_Axis6      =0x20;
    const int SMC_FIFOLtcSrc_Axis7      =0x40;
    const int SMC_FIFOLtcSrc_Axis8      =0x80;

//-------------
// SmcWGetCtrlInOutStatus : Retrieves the status of I/O control signal.
//-------------
    const int SMC_CtrlInOutSts_PCS      =0x01;
    const int SMC_CtrlInOutSts_ERC      =0x02;
    const int SMC_CtrlInOutSts_EZ       =0x04;
    const int SMC_CtrlInOutSts_CLR      =0x08;
    const int SMC_CtrlInOutSts_LTC      =0x10;
    const int SMC_CtrlInOutSts_SD       =0x20;
    const int SMC_CtrlInOutSts_INP      =0x40;
    const int SMC_CtrlInOutSts_DIRCCW   =0x80;

//-------------
// SmcWSetStopPosition : Sets stop position of the motor(total number of output pulses).
//-------------
const int SMC_StopPosition_Absolute     =0;
const int SMC_StopPosition_Relative     =1;

//-------------
// SmcWSetReady : Set beginning preparation and motor operation type/direction where basic operation begins operation.
//-------------
    //-------------
    // MotionType : Set motor motion type.
    //-------------
    const int SMC_MotionType_NoMotion   =0;
    const int SMC_MotionType_PTPMotion  =1;
    const int SMC_MotionType_JogMotion  =2;
    const int SMC_MotionType_OrgMotion  =3;
    const int SMC_MotionType_ZPhaMotion =6;
    const int SMC_MotionType_NoDecMotion=7;     //取代PTP功能
    //-------------
    // StartDir : Set start direction of motor motion. This parameter is invalid when performing PTP motion.
    //-------------
    const int SMC_Ready_StartDir_Pos    =0;
    const int SMC_Ready_StartDir_Neg    =1;

//-------------
// SmcWSetEncType : Specify the address of a variable to store the type of encoder input.
//-------------
    const int SMC_MotionType_AB_1X      =0;
    const int SMC_MotionType_AB_2X      =1;
    const int SMC_MotionType_AB_4X      =2;
    const int SMC_MotionType_UD         =3;
    const int SMC_MotionType_Unused     =4;

//-------------
//SmcWCountEvent : Sets count match event.      //就是Trigger拉
//-------------
    //-------------
    // EventMode : Set the event to valid/invalid.
    //-------------
    const int SMC_EventMode_InvalidateEvent =0;
    const int SMC_EventMode_ValidateEvent   =1;
    //-------------
    // CountType : Set the counter that does the count agreement.
    //-------------
    const int SMC_CountType_OutputPulses    =0;
    const int SMC_CountType_EncodersPulses  =1;

//-------------
//MotorType
//-------------
    const int Step_Motor  =0;
    const int Servo_Motor =1;

//==============================================================================

bool MCSMCCardInstall[MAX_SMC_CARD]={false,false,false,false,false,false,false,false,
                                         false,false,false,false,false,false,false,false};
byte SMCDestructorCount[MAX_SMC_CARD]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

short CardId[MAX_SMC_CARD];

//==============================================================================
__fastcall TMySMCMotor::TMySMCMotor(int Addr)
{
    if(Addr==-1)
    {
        iBoardID=MAX_SMC_CARD-1;
        iPortID =MAX_SMC_CARD-1;
        Id      =MAX_SMC_CARD-1;
    }
    else
    {
        iBoardID=Addr/10;
        iPortID=Addr%10+1;
        sprintf(cDeviceName,"%s%02d","SMC0",iBoardID);
        Open_SMCCard();

        Id=CardId[iBoardID];
    }

    MotorType =Servo_Motor;
    EncoderType=SMC_MotionType_AB_4X;
}
//==============================================================================
TMySMCMotor::~TMySMCMotor()
{
    Close_SMCCard();
}
//==============================================================================
bool TMySMCMotor::Open_SMCCard()                                                // open card //
{
//    if( (iBoardID < 0) || (iBoardID >= MAX_SMC_CARD)) //unsigned int不可能小於0
    if(iBoardID>=MAX_SMC_CARD)   //2012-05-03 Dell  unsigned int    iBoardID
        return false;

    int ret=0;

    if(MCSMCCardInstall[iBoardID]==false)
    {
        ret=SmcWInit(cDeviceName, &CardId[iBoardID]);
        if(ret!=0)
            return false;
        else
        {
            MCSMCCardInstall[iBoardID]=true;
            if(SMCDestructorCount[iBoardID]==0)
                SMCDestructorCount[iBoardID]=1<<1;
            else
                SMCDestructorCount[iBoardID]=SMCDestructorCount[iBoardID]<<1;
            return true;
        }
    }
    return false;
}
//==============================================================================
void TMySMCMotor::Close_SMCCard()                                               // close card //
{
    SMCDestructorCount[iBoardID]=SMCDestructorCount[iBoardID]>>1;
    if(SMCDestructorCount[iBoardID]!=0)
        return;
    if( MCSMCCardInstall[iBoardID] == true )
    {
        MCSMCCardInstall[iBoardID]=false;
        SmcWExit(Id);
    }
}
//==============================================================================
//設定軟體極限位置時直接Enable//
void TMySMCMotor::SetSoftLimit(int iPLimit, int iNLimit)                      // set softlimit //
{
    int LP, LN;
    if (Direction)
    {
        LP = -iNLimit;
        LN = -iPLimit;
    }
    else
    {
        LP = iPLimit;
        LN = iNLimit;
    }
    LP=LP/GearRatio;
    LN=LN/GearRatio;
    SmcWSetSoftLimit(Id, iPortID, 0x01, 0x01, LP, LN);
}
//==============================================================================
//將軟體極限Disable，如需Enable，直接使用SetSoftLimit//
void TMySMCMotor::SoftLimitEnable(bool bFlag)                                // softlimit enable or disable //
{
    SmcWSetSoftLimit(Id, iPortID, bFlag, bFlag, 0, 0);
}
//==============================================================================
int TMySMCMotor::InitMotor(int IoAddress)
{
    //int Ret;
    if (!Enable)
        return true;
    SetEncodeMultiple(EncoderType);                                                     //將Encode乘4倍，Panasonic伺服//


//    SmcWSetCtrlTypeIn(Id, iPortID, SMC_CtrlIn_ALM|SMC_CtrlIn_INP);       // ALM & INP

    if(MotorKind==eLinerMotor)
    {
        SmcWSetCtrlTypeIn(Id, iPortID, SMC_CtrlIn_INP|SMC_CtrlIn_LTC);         // LTC & INP
        SmcWSetCtrlTypeOut(Id, iPortID, SMC_CtrlOut_General, SMC_CtrlOut_EncoderSignal, SMC_CtrlOut_General);
    }
    else
    {
        SmcWSetCtrlTypeIn(Id, iPortID, SMC_CtrlIn_ALM|SMC_CtrlIn_INP|SMC_CtrlIn_LTC);       // ALM & INP & LTC
        SmcWSetCtrlTypeOut(Id, iPortID, SMC_CtrlOut_General, SMC_CtrlOut_General, SMC_CtrlOut_General);
    }

    if(bSensorType)
    {
        SmcWSetOrgLog(Id, iPortID, SMC_OrgLog_ORG_Negative);                            //設定home type//
    }
    else
    {
        SmcWSetOrgLog(Id, iPortID, SMC_OrgLog_ORG_Positive);                            //設定home type//
    }


    SetSMCInType();
    SetCommand(0);
    SetPosition(0);

    //Pulse mode共有7種模式//
    if(MotorKind==eLinerMotor || MotorKind==eYASKAWA)
        SmcWSetPulseType(Id, iPortID, SMC_Pulse_2Pulse_Pos,    SMC_Pulse_DirTimerOn);          //線馬
    else if(MotorKind==eVoiceCoilMotor)
        SmcWSetPulseType(Id, iPortID, SMC_Pulse_2Pulse_Pos,    SMC_Pulse_DirTimerOn);
    else if(ServoAlarmOn==Servo_Motor && MotorType==Servo_Motor)
        SmcWSetPulseType(Id, iPortID, SMC_Pulse_2Pulse_Pos,    SMC_Pulse_DirTimerOn);          //安川要用SMC_Pulse_2Pulse_Pos
    else if(ServoAlarmOn==Step_Motor && MotorType==Step_Motor)
        SmcWSetPulseType(Id, iPortID, SMC_Pulse_2Pulse_Pos,    SMC_Pulse_DirTimerOn);              //東方步進
    else
        SmcWSetPulseType(Id, iPortID, SMC_Pulse_OutPos_DirLow, SMC_Pulse_DirTimerOn);           //東方步進伺服

//    MotOutputOn(3);
    MotOutputOn(1);
    SetServoAlarm();
    SmcWSetInitParam(Id, iPortID);

    return true;
}
//==============================================================================
int TMySMCMotor::ReadRealPos()                                                  // command //
{
    long lGetCommandPos;
    SmcWGetOutPulse(Id,iPortID, &lGetCommandPos );
    if(Direction)
        lGetCommandPos=-lGetCommandPos;
    return(lGetCommandPos);
}
//==============================================================================
int TMySMCMotor::ReadEnCoderRealPos()                                           // EnCoder //
{
    if(ServoAlarmOn==Step_Motor && MotorType==Step_Motor)
    {
        return ReadRealPos();
    }

    long lGetFeedbackPos;
    SmcWGetCountPulse(Id, iPortID, &lGetFeedbackPos);

    if(MotorKind==eStepServo)       //東方步進伺服
        lGetFeedbackPos/=4;

    if(Direction)
        lGetFeedbackPos=-lGetFeedbackPos;
    return(lGetFeedbackPos);
}
//==============================================================================
bool TMySMCMotor::MotionDone()                                                  // motion done //
{
    short MoveSts ;
    bool bStatus=true;

    if(Enable)
    {
        SmcWGetMoveStatus(Id, iPortID, &MoveSts);
        bStatus=(0x00==MoveSts);

        if(bStatus)
            return true;
        else
            return false;
    }
    else
        return false;
}
//==============================================================================
int TMySMCMotor::SetCommand(int p)                                             // Set Command //
{
    int iRet=0;
    if(Enable)
    {
        iRet=SmcWSetOutPulse(Id,iPortID ,p);
    }
    return iRet;
}
//==============================================================================
int TMySMCMotor::SetPosition(int p)                                            // Set encoder //
{
    int iRet=0;
    if(Enable)
    {
        iRet=SmcWSetCountPulse(Id,iPortID,p);
    }
    return iRet;
}
//==============================================================================
void TMySMCMotor::SetSpeed(unsigned int x)                                      // set speed (pps) //
{
    int Ret;

    if(x>6553500/Range)                                                      //DL單軸最大可達9.8Mpps//
        x=6553500/Range;                                                     //DF單軸最大可達6.5Mpps//
    else if(x==0)
        x=1;
    iSpeed=x;

    //-----2012-06-01   Dell Fix 設定速度失敗-----------
    //**Contect motion card 一定要同時設定Resolve/start/target/acc/dec
    //這些參數且順序不可變

    Ret = SmcWSetResolveSpeed(Id, iPortID, Range);
    Ret = SmcWSetStartSpeed  (Id, iPortID, InitSpeed*Range);
    Ret = SmcWSetTargetSpeed (Id, iPortID, iSpeed*Range);
    Ret = SmcWSetAccelTime   (Id, iPortID, dAcc);
    Ret = SmcWSetDecelTime   (Id, iPortID, dDec);
    //--------------------------------------------------

    if(Ret==0)
    {
        //0 : Terminated normally;  Non-zero : Terminated abnormally
    }
}
//==============================================================================
void TMySMCMotor::SetSpeedShortDistance(unsigned int x)       //Sam 20240727 : 新增短距離加減速減半                               // set speed (pps) //
{
    int Ret;

    if(x>6553500/Range)                                                      //DL單軸最大可達9.8Mpps//
        x=6553500/Range;                                                     //DF單軸最大可達6.5Mpps//
    else if(x==0)
        x=1;
    iSpeed=x;

    //-----2012-06-01   Dell Fix 設定速度失敗-----------
    //**Contect motion card 一定要同時設定Resolve/start/target/acc/dec
    //這些參數且順序不可變

    Ret = SmcWSetResolveSpeed(Id, iPortID, Range);
    Ret = SmcWSetStartSpeed  (Id, iPortID, InitSpeed*Range);
    Ret = SmcWSetTargetSpeed (Id, iPortID, iSpeed*Range);
    Ret = SmcWSetAccelTime   (Id, iPortID, dAcc/1.6);
    Ret = SmcWSetDecelTime   (Id, iPortID, dDec/1.6);
    //--------------------------------------------------

    if(Ret==0)
    {
        //0 : Terminated normally;  Non-zero : Terminated abnormally
    }
}
//==============================================================================
int TMySMCMotor::ReadPos()                                                      // READ POS //
{
    EncoderPos=ReadEnCoderRealPos() * GearRatio;
    return ReadRealPos() * GearRatio;
}
//==============================================================================
void TMySMCMotor::ScanMotorStatus(bool *Led)                                    // motor status //
{
    short Status;//, CtrlOut1, CtrlOut2, CtrlOut3 ;

    if(Enable==true)
    {
        SmcWGetLimitStatus(Id, iPortID, &Status);

        Led[iServoalarmLed] =  0x01 & Status;
        Led[iCwLed]         =  0x02 & Status;
        Led[iCcwLed]        =  0x04 & Status;
        Led[iHomeLed]       =  0x08 & Status;
    //                                                                HSys.Mot.Wafer_R->Motor.MotorType
        SmcWGetDigitalIn(Id, iPortID, &Status);
        Led[iAlarmLed]  =  0x01 & Status;
        Led[iInposLed]  =!(0x02 & Status);

        if(MotorType==Servo_Motor)
        {
            Led[iServoOn]   =  0x20 & Status;
            Led[iEmgLed]    =!(0x40 & Status);
        }
        else
        {
            Led[iServoOn]   =true;  //步進強制為亮燈
            Led[iEmgLed]    =false; //步進強制為滅燈
        }

        Led[iZPhaseLed] =false;
        if(MotorKind==eVoiceCoilMotor)
        {
            SmcWGetCtrlInOutStatus(Id, iPortID, &Status);
            Led[iZPhaseLed]=SMC_CtrlInOutSts_EZ & Status;
        }
    }
    else
    {
        Led[iHomeLed]   =true;
        Led[iInposLed]  =false;
        Led[iZPhaseLed] =false;
    }

//    SmcWGetCtrlTypeOut(Id, iPortID, &CtrlOut1, &CtrlOut2, &CtrlOut3);
//    Led[iServoOn]       =  0x05 & CtrlOut1;

    if(Led[iCwLed] || Led[iCcwLed])
        Led[iAlarmLed]=true;


    Led[iSoftcwLed] =false;                                                     //無此訊號，只有count
    Led[iSoftccwLed]=false;                                                     //無此訊號，只有count
}
//==============================================================================
bool TMySMCMotor::MoveTo(int Tar)                                            // Move to //
{
    if(MCSMCCardInstall[iBoardID]==false)
        return false;
    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;
    int iPos,iP1,iP2;
    iPos=ReadPos();
    iP1=Tar/GearRatio;
    iP2=iP1*GearRatio;
    if(iP2<Tar)
    {
        while(1)
        {
            iP1++;
            iP2=iP1*GearRatio;
            if(iP2>=Tar)
                break;
        }
    }
    else if(iP2>Tar)
    {
        while(1)
        {
            iP1--;
            iP2=iP1*GearRatio;
            if(iP2<=Tar)
                break;
        }
    }
    if(Direction)
        iP1=-iP1;
    else
        iP1=iP1;

    SetSpeed(iSpeed);
    SmcWSetStopPosition(Id,iPortID,0x00,iP1);
    SmcWSetReady(Id,iPortID,0x01,0x00);

    SmcWMotionStart(Id,iPortID);
    if( MotionDone() && CheckArmPosArrival(iPos,Tar,2))
        return true;
    else
        return false;
}
//==============================================================================
bool TMySMCMotor::MoveToPosShortDistance(int Tar)                               // Move to //
{
    if(MCSMCCardInstall[iBoardID]==false)
        return false;
    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;
    int iPos,iP1,iP2;
    iPos=ReadPos();
    iP1=Tar/GearRatio;
    iP2=iP1*GearRatio;
    if(iP2<Tar)
    {
        while(1)
        {
            iP1++;
            iP2=iP1*GearRatio;
            if(iP2>=Tar)
                break;
        }
    }
    else if(iP2>Tar)
    {
        while(1)
        {
            iP1--;
            iP2=iP1*GearRatio;
            if(iP2<=Tar)
                break;
        }
    }
    if(Direction)
        iP1=-iP1;
    else
        iP1=iP1;

    SetSpeedShortDistance(iSpeed);
    SmcWSetStopPosition(Id,iPortID,0x00,iP1);
    SmcWSetReady(Id,iPortID,0x01,0x00);

    SmcWMotionStart(Id,iPortID);
    if( MotionDone() && CheckArmPosArrival(iPos,Tar,2))
        return true;
    else
        return false;
}
//==============================================================================
bool TMySMCMotor::JogP()                                                        // JOG P //
{
    int ret=0;
    if(Enable==false)
        return false;
    if(Direction)
    {
        ret=SmcWSetReadyEx(Id, iPortID, SMC_MotionType_JogMotion, SMC_Ready_StartDir_Neg);
    }
    else
    {
        ret=SmcWSetReadyEx(Id, iPortID, SMC_MotionType_JogMotion, SMC_Ready_StartDir_Pos);
    }
    ret=SmcWMotionStart(Id, iPortID);

    if(ret!=0)
    {

    }

    return true;
}
//==============================================================================
bool TMySMCMotor::JogN()                                                        // JOG N //
{
    if(Enable==false)
        return false;
    if(Direction)
    {
        SmcWSetReadyEx(Id, iPortID, SMC_MotionType_JogMotion, SMC_Ready_StartDir_Pos);
    }
    else
    {
        SmcWSetReadyEx(Id, iPortID, SMC_MotionType_JogMotion, SMC_Ready_StartDir_Neg);
    }
    SmcWMotionStart(Id, iPortID);
    return true;
}
//==============================================================================
void TMySMCMotor::Stop()                                                     // STOP //
{
    SmcWMotionStop(Id,iPortID);
}
//==============================================================================
void TMySMCMotor::DecStop()                                                  // STOP //
{
    SmcWMotionDecStop(Id,iPortID);
}
//==============================================================================
bool TMySMCMotor::HomeObject()
{
    return SMCMotHome();
}
//==============================================================================
bool TMySMCMotor::SMCMotHome()
{
    int &Task=iHomeObjectTask;
    int ret;
    switch(Task)
    {
        case 1:
            if(!MotionDone())
            {
                Stop();
                break;
            }
            SetCommand(0);
            SetPosition(0);
            SetSpeed(HomeHighSpeed);
            OldSpeed=iSpeed;
            SoftLimitEnable(false);
            Stop();

//            if(MotorKind==eVoiceCoilMotor)
//            {
//                if(HomeDirection)
//                    SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitTurn_On, SMC_OrgMode_OrgType_UseZ, SMC_OrgMode_EndDir_NegCCW, SMC_OrgMode_ZCount);
//                else
//                    SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitTurn_On, SMC_OrgMode_OrgType_UseZ, SMC_OrgMode_EndDir_PosCW , SMC_OrgMode_ZCount);
//            }
//            else
            {
//                if(MotorKind==eStepServo)
//                {
//                    if(HomeDirection)
//                        ret=SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitAsOrg, SMC_OrgMode_OrgType_UnUseZ, SMC_OrgMode_EndDir_NegCCW, SMC_OrgMode_ZCount);
//                    else
//                        ret=SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitAsOrg, SMC_OrgMode_OrgType_UnUseZ, SMC_OrgMode_EndDir_PosCW, SMC_OrgMode_ZCount);
//                }
//                else
                {
                    if(HomeDirection)
                            ret=SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitReversed, SMC_OrgMode_OrgType_UnUseZ, SMC_OrgMode_EndDir_NegCCW, SMC_OrgMode_ZCount);
                    else
                            ret=SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitReversed, SMC_OrgMode_OrgType_UnUseZ, SMC_OrgMode_EndDir_PosCW , SMC_OrgMode_ZCount);
                }
//                else
//                {
//                    if(HomeDirection)
//                        ret=SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitTurn_On, SMC_OrgMode_OrgType_UnUseZ, SMC_OrgMode_EndDir_PosCW, SMC_OrgMode_ZCount);
//                    else
//                        ret=SmcWSetOrgMode(Id, iPortID, SMC_OrgMode_LimitTurn_On, SMC_OrgMode_OrgType_UnUseZ, SMC_OrgMode_EndDir_NegCCW , SMC_OrgMode_ZCount);
//                }

            }
            InitMotor(Address);

            ret=SmcWSetReadyEx(Id, iPortID, SMC_MotionType_OrgMotion, HomeDirection);
            Task=10;
            break;
        case 10:
            ret=SmcWMotionStart(Id, iPortID);
            if(0==ret)
            {
                Task=20;
            }
            break;
        case 20:
            if(MotionDone())
            {
                Stop();
                Task=30;
            }
            break;
        case 30:
            LastHomePos=ReadRealPos();

            SetCommand(0);

            Sleep(100);

            SetPosition(0);
            SetSpeed(OldSpeed);
            SetSoftLimit(SoftLimitP, SoftLimitN);

            Task=1;
            return true;
        default:
            Task=1;
            return false;
    }
    return false;
}
//==============================================================================
void TMySMCMotor::SetInitSpeed(unsigned int x)                                  //set start speed//
{
    InitSpeed = x;
    //2012-06-01    Dell 參數寫入時機必需與(Basic Motion Setting)同步
//    Ret =SmcWSetStartSpeed(Id, iPortID, InitSpeed * Range);
}
//==============================================================================
void TMySMCMotor::SetServoAlarmOn(bool Value)
{
    ServoAlarmOn = Value;
}
//==============================================================================
void TMySMCMotor::SetServoAlarm()
{
    if(ServoAlarmOn==false)
    {
        SmcWSetCtrlTypeIn(Id, iPortID, SMC_CtrlIn_INP|SMC_CtrlIn_LTC);
        SetSMCInType();
    }
}
//==============================================================================
void TMySMCMotor::SetAcc(double a)
{
    dAcc = a;
    //2012-06-01    Dell 參數寫入時機必需與(Basic Motion Setting)同步
//    SmcWSetAccelTime(Id,iPortID,dAcc);
}
//==============================================================================
void TMySMCMotor::SetDec(double a)
{
    dDec = a;
        //2012-06-01    Dell 參數寫入時機必需與(Basic Motion Setting)同步
//    SmcWSetDecelTime(Id,iPortID,dDec);
}
//==============================================================================
void TMySMCMotor::SetSMCInType()
{
    int iLimitLogic, iIn1ALMLogic, iIn2INPLogic;
    if(bLimitLogic)
        iLimitLogic=SMC_CtrlInOutLog_LIM;
    else
        iLimitLogic=0x00;

    if(bIn1Logic)
        iIn1ALMLogic=SMC_CtrlInOutLog_IN1;
    else
        iIn1ALMLogic=0x00;

    if(MotorType == Servo_Motor)
        iIn2INPLogic=0x00;
    else
        iIn2INPLogic=SMC_CtrlInOutLog_IN2;

    SmcWSetCtrlInOutLog(Id, iPortID, iLimitLogic|iIn1ALMLogic|iIn2INPLogic);    //設定極限type//
}
//==============================================================================
void TMySMCMotor::SetRange(unsigned int a)
{
    if( a > 1000 )
        a=1000;
    Range = a;
//        2012-06-01    Dell 參數寫入時機必需與(Basic Motion Setting)同步
//    SmcWSetResolveSpeed(Id,iPortID,Range);
}
//==============================================================================
bool TMySMCMotor::GetAlarm(void)
{
    short Status ;
    SmcWGetLimitStatus(Id,iPortID, &Status );
    if(0x01 & Status)
        return true;
    else
        return false;
}
//==============================================================================
/*
設定Encode倍率
iMultiple設0等1倍
iMultiple設1等2倍
iMultiple設2等4倍
*/
void TMySMCMotor::SetEncodeMultiple(int iMultiple)
{
    SmcWSetEncType(Id,iPortID,iMultiple);
}
//==============================================================================
bool TMySMCMotor::HomeFlag(void)
{
    short Status ;
    SmcWGetLimitStatus(Id,iPortID, &Status );

    if(MotorKind==eStepServo)       //東方步進伺服
    {
//        if(Status & 0x02 || Status & 0x04)
            return(true);
//        else
//            return(false);
    }
    else
    {
        if(Status & 0x08 )
            return(true);
        else
            return(false);
    }
}
//==============================================================================
/*
bInputPort是7個陣列bool
*/
void TMySMCMotor::MotInputStatus(bool *bInputPort)                           //讀DI訊號//
{
    short InData ;
    SmcWGetDigitalIn(Id, iPortID, &InData);
    bInputPort[0]=InData & 0x01;
    bInputPort[1]=InData & 0x02;
    bInputPort[2]=InData & 0x04;
    bInputPort[3]=InData & 0x08;
    bInputPort[4]=InData & 0x10;
    bInputPort[5]=InData & 0x20;
    bInputPort[6]=InData & 0x40;
}
//==============================================================================
/*
iOutPort請代入1~3，軸卡上的編號為Out1~Out3
*/
void TMySMCMotor::MotOutputOn(int iOutPort)                                     //將DO on//
{
    short OutData=0,OutDataEnable=0;
    OutData = 1<<(iOutPort-1);
    OutDataEnable = (1<<iOutPort-1);
    SmcWSetDigitalOut(Id,iPortID, OutData ,OutDataEnable);
}
//==============================================================================
void TMySMCMotor::MotOutputOff(int iOutPort)                                    //將DO off//
{
    short OutData=0,OutDataEnable=0;
    OutData = 0<<(iOutPort-1);
    OutDataEnable = 1<<(iOutPort-1);
    SmcWSetDigitalOut(Id,iPortID, OutData ,OutDataEnable);
}
//==============================================================================
bool TMySMCMotor::LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag)
{
    if(bFlag==false)
    {
        short InterAxis;
        short InterType=1; // line interpolation
        short BankNum=1;

        short AxisNo=1;
        short MotionType=4;
        InterAxis = 0x03;

        SmcWSetBankNumber(Id, AxisNo,    BankNum);
        SmcWSetBankInterpolation(Id,AxisNo, BankNum, InterType, InterAxis, Range);
        SmcWSetBankDistance(Id,AxisNo, 1,lPos[0]);
        SmcWSetBankDistance(Id,AxisNo + 1, 1,lPos[1]);
        SmcWSetBankStartSpeed(Id,AxisNo, 1,iSpeed*Range);
        SmcWSetBankTargetSpeed(Id, AxisNo, 1,iSpeed*Range);
        SmcWSetBankAccelTime(Id, AxisNo, 1, dAcc);
        SmcWSetBankDecelTime(Id, AxisNo, 1, dDec);
        SmcWSetBankSSpeed(Id, AxisNo, 1, 200);
        SmcWSetBankReady(Id, AxisNo, MotionType);
        SmcWMotionStart(Id, AxisNo);
    }
    else
    {
        if(MotionDone())
            return true;
    }
    return false;
}
//==============================================================================
/*
1、mode=0 pulse command
2、mode=1 encode
3、需設定output defind，設定那一個Out port輸出，預設用Out2
   SmcWSetCtrlTypeOut(Id, AxisNo, CtrlOut1, CtrlOut2, CtrlOut3)
4、需設定Windows handle
*/
void TMySMCMotor::EnableTrigger(int iFlag, int iMode, long lValue)
{
    int ret;
    HWND hWnd=FindWindow("TfMain", "HT1501");
    ret=SmcWStopEvent( Id , iPortID , hWnd , 1 );
    if(iFlag==SMC_EventMode_ValidateEvent)
    {
        long lPitch=lValue/GearRatio;
        if(Direction)
            lPitch=-lPitch;
        else
            lPitch=lPitch;

//        ret=SmcWSetTrgOutData(Id, 0, 0, 0x01, lValue);
//        ret=SmcWSetTrgOutStart(Id);
        ret=SmcWIrqEvent(Id, 1, hWnd, 1, 0);
        ret=SmcWCountEvent(Id, iPortID, hWnd, SMC_EventMode_ValidateEvent, iMode, lPitch);
    }
    else
    {
//        ret=SmcWSetTrgOutStop(Id);
        ret=SmcWCountEvent(Id, iPortID, hWnd, SMC_EventMode_InvalidateEvent, iMode, 0);
        ret=SmcWIrqEvent(Id, 1, hWnd, 0, 0);
    }
//        SmcWSetInitParam(Id, iPortID);

    if(ret)
    {

    }

}
//==============================================================================
void TMySMCMotor::ManualTestTrigger(bool bOn)
{
    if(bOn==false)
    {
        MotOutputOff(2);
    }

    if(bOn==false)
    {
        SmcWSetCtrlTypeOut(Id, iPortID, SMC_CtrlOut_General, SMC_CtrlOut_EncoderSignal, SMC_CtrlOut_General);
    }
    else
    {
        SmcWSetCtrlTypeOut(Id, iPortID, SMC_CtrlOut_General, SMC_CtrlOut_General, SMC_CtrlOut_General);
    }
    SmcWSetInitParam(Id, iPortID);

    if(bOn)
    {
        MotOutputOn(2);
    }
}
//==============================================================================
//2012-05-28    Add
bool  TMySMCMotor::ResetPos(int Pulse=0)
{
    int iR1, iR2;

    iR1=SetCommand(Pulse);
    iR2=SetPosition(Pulse);

    if((iR1==0) && (iR2==0))
        return true;
    return false;
}
//==============================================================================
void TMySMCMotor::SetServoOn(bool IsOn)
{
    if(Enable==false)
        return;

    if(IsOn)
    {
        MotOutputOn(1);
    }
    else
    {
        MotOutputOff(1);
    }
}
//==============================================================================
