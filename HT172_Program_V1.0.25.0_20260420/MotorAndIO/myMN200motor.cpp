#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "myMN200motor.h"

#include "cMyDef.h"

#define MAX_LATCH_COUNT 1024

//-------------
//MotorType
//-------------
    const int Step_Motor  =0;
    const int Servo_Motor =1;
    const int Rotate_Motor=2;           //Steven 20170425 (wei) : Add rotate motor
    const int YASKAWA_Servo_Motor=3;    //Steven 20181111 : Yaskawa馬達
    const int YASKAWA_Liner_Motor=4;
extern bool bOpenCard;
//------------------------------------------------------------------------------
extern bool bResetMNet;  //jou 2012-01-10 避免24V還沒開啟就在重置先達模組
BYTE NumLine;           //Steven 20190918 : NumLine改成全域變數
//------------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TMyMN200Motor::TMyMN200Motor(int Addr)
{
    if(Addr==-1)
    {
        Address  =-1;
        iBoardID =-1;
        iPortID  =-1;
    }
    else
    {
        Address =Addr/100;
        iBoardID=Addr/100;
        iPortID=Addr%100;

        MN200OpenCard();
    }
}
//==============================================================================
TMyMN200Motor::~TMyMN200Motor()
{

}
//==============================================================================
void TMyMN200Motor::GetMN200ErrorMessage(int nErrCode, AnsiString FUNC)                          //Steven 20220520 : 針對MN200增加錯誤訊息的紀錄
{
    bool bResult;
    AnsiString StrCh="", StrEng="";
    if(nErrCode<0)
    {
        bResult=GetMN200_Error_Code(iBoardID, nErrCode, &StrEng, &StrCh, iPortID);
        if(bResult)
        {
            MNetLog(StrEng);
//            if(nErrCode==ERROR_SET_ACC_OUT_RANGE || nErrCode==ERROR_SET_DEC_OUT_RANGE)  //Steven 20221006 : 針對MNet速度寫不進去進行紀錄
//            {
//                StrEng.sprintf("CorrSpd=%d, ACC=%d, DEC=%d",
//                                MN200SpeedPar.Correction_Speed, MN200SpeedPar.Acc, MN200SpeedPar.Dec);
//                MNetLog(StrEng);
//            }
        }
        else
        {
            StrCh.sprintf("MN200 Error Board %d, Port %d, Error Code %d, FUNC %s", iBoardID, iPortID, nErrCode, FUNC);
            MNetLog(StrEng);
        }
    }
}
//==============================================================================
//設定軟體極限位置時直接Enable//
void TMyMN200Motor::SetSoftLimit(int iPLimit, int iNLimit)                      // set softlimit //
{
    if(!Enable)
        return;

    int nErrCode=0;
    if(Direction)
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
    nErrCode=mn_set_softlimit(iBoardID, iPortID, ENABLE_FEATURE, PULSE_COMMAND, SUDDEN_STOP, LP, LN);
    GetMN200ErrorMessage(nErrCode, "mn_set_softlimit");
}
//==============================================================================
//將軟體極限Disable，如需Enable，直接使用SetMN200SoftLimit//
void TMyMN200Motor::MN200SoftLimitEnable(bool bFlag)                            // softlimit enable or disable //
{
    if(!Enable)
        return;

    int nErrCode=0;
    if(bFlag)
        nErrCode=mn_set_softlimit(iBoardID, iPortID, ENABLE_FEATURE, PULSE_COMMAND, SUDDEN_STOP, LP, LN);
    else
        nErrCode=mn_set_softlimit(iBoardID, iPortID, DISABLE_FEATURE, PULSE_COMMAND, SUDDEN_STOP, LP, LN);
    GetMN200ErrorMessage(nErrCode, "mn_set_softlimit");
}
//==============================================================================
int TMyMN200Motor::InitMotor(int IoAddress)
{
    int ret=0;
    if(!Enable)
        return true;

    ret=mn_set_motion_cfg(iBoardID, iPortID, EL_PROC,     SUDDEN_STOP);                                   //設定遇到極限點時所反應的動作
    if(ret<0)
    {
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg - EL_PROC");
//        ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                    "EL_PROC: SUDDEN_STOP", "return:"+IntToStr(ret));
    }
    ret=mn_set_motion_cfg(iBoardID, iPortID, ALM_PROC,    SUDDEN_STOP);                                   //設定Alarm訊號觸發所反應的動作
    if(ret<0)
    {
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg - ALM_PROC");
//        ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                    "ALM_PROC: SUDDEN_STOP", "return:"+IntToStr(ret));
    }
    ret=mn_set_motion_cfg(iBoardID, iPortID, SD_ENA,      DISABLE_FEATURE);                               //致/除能 減速點的功能
    if(ret<0)
    {
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg - SD_ENA");
//        ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                    "SD_ENA: DISABLE_FEATURE", "return:"+IntToStr(ret));
    }
    if(bSensorType)
        ret=mn_set_motion_cfg(iBoardID, iPortID, ORG_LOGIC,   LOGIC_ACTIVE_LOW);                          //設定原點的觸發邏輯
    else
        ret=mn_set_motion_cfg(iBoardID, iPortID, ORG_LOGIC,   LOGIC_ACTIVE_HIGH);
    if(ret<0)
    {
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg - ORG_LOGIC");
//        ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                    "ORG_LOGIC: LOGIC_ACTIVE_LOW", "return:"+IntToStr(ret));
    }
    SetMN200InType();
    SetCommand(0);
    SetPosition(0);

    if(MotorType==Servo_Motor || MotorType==YASKAWA_Servo_Motor)
    {
        ret=mn_set_motion_cfg(iBoardID, iPortID, ENC_MODE,    ENCODER_MODE_AB_MULT_4);                  //將Encode乘4倍，Panasonic伺服//
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg - ENC_MODE");
        ret=mn_set_motion_cfg(iBoardID, iPortID, PULSE_MODE,  PULSE_MODE_CW_LOGIC_LOW);                  //Pulse mode共有7種模式//
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg - PULSE_MODE");
    }
    else if(MotorType==Rotate_Motor)        //Steven 20170425 (wei) : Add rotate motor
    {
        ret=mn_set_motion_cfg(iBoardID, iPortID, ENC_MODE,    ENCODER_MODE_CW_CCW);
        if(ret<0)
        {
            GetMN200ErrorMessage(ret, "mn_set_motion_cfg - ENC_MODE");
//            ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                        "ENC_MODE: ENCODER_MODE_CW_CCW", "return:"+IntToStr(ret));
        }
        ret=mn_set_motion_cfg(iBoardID, iPortID, PULSE_MODE,  PULSE_MODE_PULSE_LOGIC_HIGH_DIR_FORWARD_LOW);
        if(ret<0)
        {
            GetMN200ErrorMessage(ret, "mn_set_motion_cfg - PULSE_MODE");
//            ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                        "PULSE_MODE: PULSE_MODE_PULSE_LOGIC_HIGH_DIR_FORWARD_LOW", "return:"+IntToStr(ret));
        }
    }
    else
    {
        ret=mn_set_motion_cfg(iBoardID, iPortID, ENC_MODE,    ENCODER_MODE_CW_CCW);
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg - ENC_MODE");
        ret=mn_set_motion_cfg(iBoardID, iPortID, PULSE_MODE,  PULSE_MODE_PULSE_LOGIC_LOW_DIR_FORWARD_LOW);
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg - PULSE_MODE");
    }

//    MotOutputOn(1);
    SetServoOn(true);
    SetServoAlarm();
    if(ret!=0)
    {
        return false;
    }
    return true;
}
//==============================================================================
extern void OpenPCI132Card();
bool TMyMN200Motor::MN200OpenCard()
{
    if(bOpenCard==false)
        OpenPCI132Card();
    bOpenCard=true;

    return true;
}
//==============================================================================
int TMyMN200Motor::ReadRealPos()                                                  // command //
{
    int iRet=0;
    long lGetCommandPos;
    iRet=mn_get_cmdcounter(iBoardID, iPortID, &lGetCommandPos);
    GetMN200ErrorMessage(iRet, "mn_get_cmdcounter");
    if(Direction)
        lGetCommandPos=-lGetCommandPos;
    return(lGetCommandPos);
}
//==============================================================================
int TMyMN200Motor::ReadEnCoderRealPos()                                           // EnCoder //
{
    long lGetFeedbackPos;
    int iRet=0;
    if(MotorType==Step_Motor)
        iRet=mn_get_cmdcounter(iBoardID, iPortID, &lGetFeedbackPos);
    else
        iRet=mn_get_enccounter(iBoardID, iPortID, &lGetFeedbackPos);
    if(iRet!=0)
    {
        GetMN200ErrorMessage(iRet, "mn_get_enccounter");
    }
    if(Direction)
        lGetFeedbackPos=-lGetFeedbackPos;
    return(lGetFeedbackPos);
}
//==============================================================================
bool TMyMN200Motor::MotionDone()                                                  // motion done //
{
    int iRet=0;
    BYTE MoveSts;

    if(Enable)
    {
        iRet=mn_motion_done(iBoardID, iPortID, &MoveSts);

        if(iRet!=0)
        {
            GetMN200ErrorMessage(iRet, "mn_motion_done");
        }

        if(MoveSts==MOTION_DONE)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}
//==============================================================================
int TMyMN200Motor::SetCommand(int p)                                             // Set Command //
{
    int iRe=0;
    if(Enable)
    {
        iRe=mn_set_cmdcounter(iBoardID, iPortID, p);
        GetMN200ErrorMessage(iRe, "mn_set_cmdcounter");
    }
    return iRe;
}
//==============================================================================
int TMyMN200Motor::SetPosition(int p)                                            // Set encoder //
{
    int iRe=0;
    if(Enable)
    {
        iRe=mn_set_enccounter(iBoardID, iPortID, p);
        GetMN200ErrorMessage(iRe, "mn_set_enccounter");
    }
    return iRe;
}
//==============================================================================
void TMyMN200Motor::SetSpeed(unsigned int x)                                      // set speed (pps) //
{

    if(x>6553500/Range)                                                      //DL單軸最大可達9.8Mpps//
        x=6553500/Range;                                                     //DF單軸最大可達6.5Mpps//
    else if(x==0)
        x=1;
    iSpeed=x;
    double dStart_Speed=0.0, dDrive_Speed=0.0;//, dSCurveAcc_Sect=0.0, dSCurveDec_Sect=0.0;
    BYTE AccDec_Mode=0, SCurve_Enable=0;

    dStart_Speed=InitSpeed*Range;
    dDrive_Speed=iSpeed*Range;
    AccDec_Mode=ADC_MODE_TIME;
    SCurve_Enable=DISABLE_FEATURE;

    MN200SpeedPar.Start_Speed=dStart_Speed;          // Initial & Stop Speed (PPS)
    MN200SpeedPar.Drive_Speed=dDrive_Speed;             // Operation Speed (PPS)

    if(MN200SpeedPar.Drive_Speed<MN200SpeedPar.Start_Speed)     //Steven 20170504 (wei) : Add protection for motor speed
        MN200SpeedPar.Drive_Speed=MN200SpeedPar.Start_Speed;


    MN200SpeedPar.Correction_Speed=MN200SpeedPar.Drive_Speed;   // Correction Speed (PPS)
    MN200SpeedPar.Acc=dAcc;                             // Acceleration data (s/ PPS/S)
    MN200SpeedPar.Dec=dDec;                             // Deceleration data (s/ PPS/S)
    MN200SpeedPar.AccDec_Mode=AccDec_Mode;            // ADC_MODE_RATE / ADC_MODE_TIME
    MN200SpeedPar.SCurve_Enable=SCurve_Enable;        // DISABLE / Enable
    MN200SpeedPar.SCurveAcc_Sect=0.0;
    MN200SpeedPar.SCurveDec_Sect=0.0;
//    if(MotorType==Rotate_Motor)                         //Steven 20221005 : 嘗試修正速度設定異常
        MN200SpeedPar.Max_Speed=MAXSPEED_100K;            // selecting MaxSpeed to set the speed magnification rate
//    else
//        MN200SpeedPar.Max_Speed=MAXSPEED_5M;
}
//==============================================================================
int TMyMN200Motor::ReadPos()                                                      // READ POS //
{
    EncoderPos=ReadEnCoderRealPos() * GearRatio;
    return ReadRealPos()*GearRatio;
}
//==============================================================================
void TMyMN200Motor::ScanMotorStatus(bool *Led)                                    // motor status //
{
    MOTION_IO MotionIO;
    int ret;

    if(Enable==true)  //Steven 20141016 : Disable時,不要掃LED
    {
        ret=mn_get_mdio_status(iBoardID, iPortID, &MotionIO);

        Led[iServoalarmLed] =  false;
        Led[iCwLed]         =  MotionIO.PEL;
        Led[iCcwLed]        =  MotionIO.MEL;
        Led[iHomeLed]       =  MotionIO.ORG;

        Led[iAlarmLed]      =  MotionIO.ALM;
        Led[iInposLed]      =!(MotionIO.INP);
        if(MotorType==Servo_Motor || MotorType==YASKAWA_Servo_Motor)
        {
            Led[iServoOn]   =  MotionIO.SVON;
            Led[iEmgLed]    =!(MotionIO.EMG);
        }
        else if(MotorType==Rotate_Motor)        //Steven 20170425 (wei) : Add rotate motor
        {
            Led[iServoOn]   = MotionIO.SVON;
            Led[iEmgLed]    =(MotionIO.EMG);
        }
        else
        {
            Led[iServoOn]   = MotionIO.SVON;  //步進強制為亮燈  //步進五軸卡可以SERVO on/off
            Led[iEmgLed]    = MotionIO.EMG;
        }
    }
    else
    {
        Led[iHomeLed]       =true;
        Led[iInposLed]      =false;
    }

    if(ret!=0)
    {
        GetMN200ErrorMessage(ret, "ScanMotorStatus");
    }

    if(Led[iCwLed] || Led[iCcwLed])
        Led[iAlarmLed]=true;

    Led[iSoftcwLed] =false;                                                     //無此訊號，只有count
    Led[iSoftccwLed]=false;                                                     //無此訊號，只有count
    Led[iZPhaseLed]=false;
}
//==============================================================================
bool TMyMN200Motor::MoveTo(int Tar)                                            // Move to //
{
    if(Enable==false)
        return true;

    if(!MotionDone())
        return false;

    int iPos,iP1,iP2;
    int iRet=0;
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

    iRet=mn_fix_move(iBoardID, iPortID, MN200SpeedPar, iP1, FIX_MOVE_MODE_ABS_BY_OUTPLS);

    if(iRet!=0)
    {
        GetMN200ErrorMessage(iRet, "mn_fix_move");
    }

    if(MotionDone() && Tar==iPos)
        return true;
    else
        return false;
}
//==============================================================================
bool TMyMN200Motor::JogP()                                                        // JOG P //
{
    int iRet=0;
    if(Enable==false)
        return false;
    if(Direction)
    {
        iRet=mn_velocity_move(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_REVERSE);    //wei 20161020 MN200 JogP方向相反
    }
    else
    {
        iRet=mn_velocity_move(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_FORWARD);    //wei 20161020 MN200 JogP方向相反
    }

    if(iRet!=0)
    {
        GetMN200ErrorMessage(iRet, "mn_velocity_move");
    }

    return true;
}
//==============================================================================
bool TMyMN200Motor::JogN()                                                        // JOG N //
{
    int iRet=0;
    if(Enable==false)
        return false;

    if(Direction)
    {
        iRet=mn_velocity_move(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_FORWARD);
    }
    else
    {
        iRet=mn_velocity_move(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_REVERSE);
    }

    if(iRet!=0)
    {
        GetMN200ErrorMessage(iRet, "mn_velocity_move");
    }

    return true;
}
//==============================================================================
void TMyMN200Motor::Stop()                                                     // STOP //
{
    int iRet=0;
    iRet=mn_stop_move(iBoardID, iPortID, SUDDEN_STOP);
    GetMN200ErrorMessage(iRet, "mn_stop_move");
}
//==============================================================================
void TMyMN200Motor::DecStop()                                                  // STOP //
{
    int iRet=0;
    iRet=mn_stop_move(iBoardID, iPortID, SLOWDOWN_STOP);
    GetMN200ErrorMessage(iRet, "mn_stop_move");
}
//==============================================================================
bool TMyMN200Motor::HomeObject()
{
    return MN200MotHome();
}
//==============================================================================
bool TMyMN200Motor::MN200MotHome()
{
    int &Task=iHomeObjectTask;
    int Status=0;
//    MOTION_IO MotionIO;
    switch(Task)
    {
        case 1:
            if(!MotionDone())
            {
                Stop();
                break;
            }
            SetServoOn(true);   //JerryYang 20170605 (wei) MN200 set servo on
            SetCommand(0);
            SetPosition(0);
            SetSpeed(HomeHighSpeed);
            OldSpeed=iSpeed;
            MN200SoftLimitEnable(false);
            Stop();

            InitMotor(Address);
            Task=10;
            break;
        case 10:
//            Status=mn_get_mdio_status(iBoardID, iPortID, &MotionIO);    //wei 20161020 MN200 Home遮板已被遮住，需往反方向走離開
//            GetMN200ErrorMessage(Status, "mn_get_mdio_status");
//            if(MotionIO.ORG)
//            {
//                if(HomeDirection)
//                    Status=mn_home_start(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_FORWARD, 0, 0);
//                else
//                    Status=mn_home_start(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_REVERSE, 0, 0);
//                GetMN200ErrorMessage(Status, "mn_home_start");
//            }
//
//            if(MotionIO.ORG==false)
//            {
//                Status=mn_stop_move(iBoardID, iPortID, SUDDEN_STOP);
//                GetMN200ErrorMessage(Status, "mn_stop_move");
//                Task=11;
//            }
//            break;
//        case 11:
//            if(HomeDirection)
//                Status=mn_home_start(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_REVERSE, 0, 0);    //wei 20161020 MN200 方向相反
//            else
//                Status=mn_home_start(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_FORWARD, 0, 0);    //wei 20161020 MN200 方向相反
            if(HomeDirection)
                Status=mn_home_search(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_REVERSE, 500, 0, 0);      //jimmychiu 20220715 : add new home method for MN200
            else
                Status=mn_home_search(iBoardID, iPortID, MN200SpeedPar, MOVE_DIRECTION_FORWARD, 500, 0, 0);      //jimmychiu 20220715 : add new home method for MN200

            GetMN200ErrorMessage(Status, "mn_home_start");
            if(Status==SUCCESS)
            {
                Task=20;
            }
            break;
        case 20:
            if(MotionDone())
                Task=30;
            break;
        case 30:
            //LastHomePos=-ReadPos();
            LastHomePos=ReadRealPos();
            SetCommand(0);
            ::Sleep(100);
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
void TMyMN200Motor::SetInitSpeed(unsigned int x)                                  //set start speed//
{
    InitSpeed=x;
}
//==============================================================================
void TMyMN200Motor::SetServoAlarm()
{
    if(ServoAlarmOn==false)
    {
        SetMN200InType();
    }
}
//==============================================================================
void TMyMN200Motor::SetServoAlarmOn(bool Value)
{
    ServoAlarmOn=Value;
}
//==============================================================================
void TMyMN200Motor::SetAcc(double a)
{
    dAcc = a;
}
//==============================================================================
void TMyMN200Motor::SetDec(double a)
{
    dDec = a;
}
//==============================================================================
void TMyMN200Motor::SetMN200InType()
{
    int ret;

    if(MotorType==Servo_Motor || MotorType==YASKAWA_Servo_Motor)
    {
        ret=mn_set_motion_cfg(iBoardID, iPortID, INP_LOGIC,       LOGIC_ACTIVE_HIGH);       //設定INP訊號的觸發邏輯
        if(ret<0)
        {
            GetMN200ErrorMessage(ret, "mn_set_motion_cfg -- INP_LOGIC");
//            ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                        "INP_LOGIC: LOGIC_ACTIVE_HIGH", "return:"+IntToStr(ret));
        }
        if(bIn1Logic)
            ret=mn_set_motion_cfg(iBoardID, iPortID, ALM_LOGIC,       LOGIC_ACTIVE_LOW);       //Steven 20170503 : LOGIC_ACTIVE_HIGH --> LOGIC_ACTIVE_LOW
        else
            ret=mn_set_motion_cfg(iBoardID, iPortID, ALM_LOGIC,       LOGIC_ACTIVE_HIGH);
        if(ret<0)
        {
            GetMN200ErrorMessage(ret, "mn_set_motion_cfg -- ALM_LOGIC");
//            ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                        "ALM_LOGIC: LOGIC_ACTIVE_HIGH", "return:"+IntToStr(ret));
        }
    }
    else if(MotorType==Rotate_Motor)                                                    //Steven 20170425 : Add rotate motor
    {
        ret=mn_set_motion_cfg(iBoardID, iPortID, INP_LOGIC,       LOGIC_ACTIVE_LOW);
        if(ret<0)
        {
            GetMN200ErrorMessage(ret, "mn_set_motion_cfg -- INP_LOGIC");
//            ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                        "INP_LOGIC: LOGIC_ACTIVE_LOW", "return:"+IntToStr(ret));
        }
        ret=mn_set_motion_cfg(iBoardID, iPortID, ALM_LOGIC,       LOGIC_ACTIVE_LOW);  //JerryYang 20170608 rotate logic要設low
        if(ret<0)
        {
            GetMN200ErrorMessage(ret, "mn_set_motion_cfg -- ALM_LOGIC");
//            ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                        "ALM_LOGIC: LOGIC_ACTIVE_LOW", "return:"+IntToStr(ret));
        }
    }
    else
    {
        ret=mn_set_motion_cfg(iBoardID, iPortID, INP_LOGIC,       LOGIC_ACTIVE_LOW);        //設定INP訊號的觸發邏輯
        if(ret<0)
        {
            GetMN200ErrorMessage(ret, "mn_set_motion_cfg -- INP_LOGIC");
//            ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                        "INP_LOGIC: LOGIC_ACTIVE_LOW", "return:"+IntToStr(ret));
        }

        if(bIn1Logic)   //JerryYang 20181217 : MN200依照type設定ALM LOGIC
            ret=mn_set_motion_cfg(iBoardID, iPortID, ALM_LOGIC,       LOGIC_ACTIVE_LOW);       //Steven 20170503 : LOGIC_ACTIVE_HIGH --> LOGIC_ACTIVE_LOW
        else
            ret=mn_set_motion_cfg(iBoardID, iPortID, ALM_LOGIC,       LOGIC_ACTIVE_HIGH);

        if(ret<0)
        {
            GetMN200ErrorMessage(ret, "mn_set_motion_cfg -- ALM_LOGIC");
//            ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                        "ALM_LOGIC: LOGIC_ACTIVE_LOW", "return:"+IntToStr(ret));
        }
    }

    ret=mn_set_motion_cfg(iBoardID, iPortID, ENC_Z_LOGIC,     LOGIC_ACTIVE_HIGH);           //設定Z訊號的輸入邏輯
    if(ret<0)
    {
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg -- ENC_Z_LOGIC");
//        ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                    "ENC_Z_LOGIC: LOGIC_ACTIVE_HIGH", "return:"+IntToStr(ret));
    }
    ret=mn_set_motion_cfg(iBoardID, iPortID, ERC_LOGIC,       LOGIC_ACTIVE_HIGH);           //設定ERC訊號的輸出邏輯
    if(ret<0)
    {
        GetMN200ErrorMessage(ret, "mn_set_motion_cfg -- ERC_LOGIC");
//        ShowMyMessage("iBoardID:"+IntToStr(iBoardID)+"iPortID"+IntToStr(iPortID),
//                    "ERC_LOGIC: LOGIC_ACTIVE_HIGH", "return:"+IntToStr(ret));
    }
}
//==============================================================================
void TMyMN200Motor::SetRange(unsigned int a)
{
    if(a>1000)
        a=1000;
    Range=a;
}
//==============================================================================
bool TMyMN200Motor::GetAlarm(void)
{
    DWORD Status;
    int ret;
    ret=mn_get_error_status(iBoardID, iPortID, &Status);
    GetMN200ErrorMessage(ret, "mn_get_error_status");
    if(0x0020 & Status)
        return true;
    else
        return false;
}
//==============================================================================
bool TMyMN200Motor::HomeFlag(void)
{
    MOTION_IO Status;
    int ret=0;
    ret=mn_get_mdio_status(iBoardID, iPortID, &Status);
    GetMN200ErrorMessage(ret, "mn_get_mdio_status");
    if(Status.ORG)
        return(true);
    else
        return(false);
}
//==============================================================================
void TMyMN200Motor::MotInputStatus(bool *bInputPort)                           //讀DI訊號//
{
    ;
}
//==============================================================================
void TMyMN200Motor::MotOutputOn(int iOutPort)                                     //將DO on//
{
    ;
}
//==============================================================================
void TMyMN200Motor::MotOutputOff(int iOutPort)                                    //將DO off//
{
    ;
}
//==============================================================================
bool TMyMN200Motor::LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag)
{
    return true;
}
//==============================================================================
void TMyMN200Motor::EnableTrigger(bool bFlag, int iMode, long lValue)
{
    ;
}
//==============================================================================
bool TMyMN200Motor::ResetPos(int p)
{
    long iR1, iR2;

    iR1=SetCommand(p);
    iR2=SetPosition(p);

    if((iR1==0) && (iR2==0))
        return true;
    return false;
}
//==============================================================================
void TMyMN200Motor::ResetLatch()
{
    ;
}
//==============================================================================
int TMyMN200Motor::GetLatchTotalLen()
{
    return 0;
}
//==============================================================================
int TMyMN200Motor::GetLatchBuffer(int LatchInputNo, unsigned short *AxisCounterNo, long *LatchDataTable)
{
    return 0;
}
//--------------------------------------------------------------------------
bool TMyMN200Motor::GetLatchIOStatus(unsigned short LatchInputNo)
{
    return false;
}
//--------------------------------------------------------------------------
void TMyMN200Motor::SetArmMaxSpeed()
{
//    if(Enable==false)
//        return;
//    _Hon_m4_set_velocity_limit(Address, iBoardID, iPortID, PJogHighSpeed*65);
}
//--------------------------------------------------------------------------
void TMyMN200Motor::SetServoOn(bool IsOn)
{
    if(Enable==false)
        return;
    int ret=0;
    if(IsOn)
    {
        ret=mn_servo_on(iBoardID, iPortID, TURN_ON);
    }
    else
    {
        ret=mn_servo_on(iBoardID, iPortID, TURN_OFF);
    }
    GetMN200ErrorMessage(ret, "mn_servo_on");
}
//--------------------------------------------------------------------------
void TMyMN200Motor::SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int Enable)
{
//    if(Enable==false)
//        return;
//    _Hon_m4_set_FIFO_latch_src(Address, iBoardID, AxisCounterNo, LatchInputNo, Enable);
}
//------------------------------------------------------------------------------
//DEll 2012-12-07 Start
bool GetMN200_Error_Code(int iRing, int iCode, AnsiString *EngStr, AnsiString *ChStr, int iIP)
{
    bool bResult=true;

    switch(iCode)
    {
        case ERROR_NO_CARD_FOUND:                                               //-100
            EngStr->sprintf("MNet Ring %d ERROR_NO_CARD_FOUND!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, 找不到Motionnet板卡", iRing);
            break;
        case ERROR_IOCTL_FAILED:                                                //-101
            EngStr->sprintf("MNet Ring %d IP %d ERROR_IOCTL_FAILED!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, 傳送控制代碼到驅動發生錯誤。", iRing, iIP);
            break;
        case ERROR_INVALID_LINE_NO:                                             //-102
            EngStr->sprintf("MNet Ring %d ERROR_INVALID_LINE_NO!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, 對應不到作業系統所找到的 Motionnet 板卡", iRing);
            break;
        case ERROR_COMM_NOT_START:                                              //-103
            EngStr->sprintf("MNet Ring %d ERROR_COMM_NOT_START!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, 沒有開始  Motionnet 通訊傳輸。", iRing);
            break;
        case ERROR_INVALID_DEV_NO:                                              //-104
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_DEV_NO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, 裝置編號, 超出設定範圍值: 0~63。", iRing, iIP);
            break;
        case ERROR_NO_DEV_FOUND:                                                //-105
            EngStr->sprintf("MNet Ring %d IP %d ERROR_NO_DEV_FOUND!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, 通訊線上找不到可以使用的裝置。", iRing, iIP);
            break;
        case ERROR_SET_IO_DEV:                                                  //-106
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_IO_DEV!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 模組錯誤, 裝置編號, 不為運動控制模組, 屬於串列I/O模組。", iRing, iIP);
            break;
        case ERROR_SET_MOTION_DEV:                                              //-107
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_MOTION_DEV!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 模組錯誤, 裝置編號, 不為串列I/O模組, 屬於運動控制模組。", iRing, iIP);
            break;
        case ERROR_START_SPEED_EXCEED_DRIVING_SPEED:                            //-108
            EngStr->sprintf("MNet Ring %d IP %d ERROR_START_SPEED_EXCEED_DRIVING_SPEED!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給 SpeedPar 結構參數中的Start_Speed 速度大於 Drive_Speed 的速度值。", iRing, iIP);
            break;
        case ERROR_INVALID_MAX_SPEED_SELECTION:                                 //-109
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_MAX_SPEED_SELECTION!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的Max_Speed不屬於MaxSpeed列舉成員。", iRing, iIP);
            break;
        case ERROR_SET_START_SPEED_OUT_RANGE:                                   //-110
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_START_SPEED_OUT_RANGE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的Start_Speed超過所選擇Max_Speed的範圍。", iRing, iIP);
            break;
        case ERROR_SET_DRIVING_SPEED_OUT_RANGE:                                 //-111
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_DRIVING_SPEED_OUT_RANGE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的Drive_Speed超過所選擇Max_Speed的範圍。", iRing, iIP);
            break;
        case ERROR_INVALID_SCURVE_ENABLE:                                       //-112
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_SCURVE_ENABLE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的SCurve_Enable不是DISABLE_FEATURE或ENABLE_FEATURE。", iRing, iIP);
            break;
        case ERROR_INVALID_ADC_MODE:                                            //-113
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_ADC_MODE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的AccDec_Mode不是ADC_MODE_RATE或ADC_MODE_TIME。", iRing, iIP);
            break;
        case ERROR_INVALID_ACC_DATA:                                            //-114
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_ACC_DATA!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的Acc加速度值為零。", iRing, iIP);
            break;
        case ERROR_SET_ACC_DOUBLE_DEC:                                          //-115
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_ACC_DOUBLE_DEC!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的Acc加速度值大於Dec減速度值的兩倍。", iRing, iIP);
            break;
        case ERROR_SET_ACC_OUT_RANGE:                                           //-116
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_ACC_OUT_RANGE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的Acc加速度值超過所選擇Max_Speed的範圍。", iRing, iIP);
            break;
        case ERROR_SET_DEC_OUT_RANGE:                                           //-117
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_DEC_OUT_RANGE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的Dec加速度值超過所選擇Max_Speed的範圍。", iRing, iIP);
            break;
        case ERROR_SET_ACC_SECT_OUT_RANGE:                                      //-118
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_ACC_SECT_OUT_RANGE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, SCureAcc_Sect不在0~(Drive_Speed-Start_Speed範圍內)。", iRing, iIP);
            break;
        case ERROR_SET_DEC_SECT_OUT_RANGE:                                      //-119
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_DEC_SECT_OUT_RANGE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, SCureDec_Sect不在0~(Drive_Speed-Start_Speed範圍內)。", iRing, iIP);
            break;
        case ERROR_SET_CORRECTION_SPD_OUT_RANGE:                                //-120
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_CORRECTION_SPD_OUT_RANGE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給SpeedPar結構參數中的Correction_Speed速度值超過所選擇Max_Speed的範圍)。", iRing, iIP);
            break;
        case ERROR_SET_SCAN_INDEX_OUT_RANGE:                                    //-121
            EngStr->sprintf("MNet Ring %d ERROR_SET_SCAN_INDEX_OUT_RANGE!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, 索引值(ScannedIndex)不在合理範圍內，超出系統可使用的板卡數目)。", iRing);
            break;
        case ERROR_INVALID_COMM_SPEED:                                          //-122
            EngStr->sprintf("MNet Ring %d ERROR_INVALID_COMM_SPEED!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, 傳輸速度的設定值並不在指定的範圍。", iRing);
            break;
        case ERROR_COMM_NOT_STOP:                                               //-123
            EngStr->sprintf("MNet Ring %d ERROR_COMM_NOT_STOP!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, 無法停止Motionnet通訊傳輸。", iRing);
            break;
        case ERROR_SET_DATA:                                                    //-124
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_DATA!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, 設定給bSWLimitEnable參數不是ENABLE_FEATURE 或 DISABLE_FEATURE。", iRing, iIP);
            break;
        case ERROR_INVALID_CONFIG_ITEM:                                         //-125
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_CONFIG_ITEM!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 參數CfgItem的值不屬於MotionConfig列舉的變數值。", iRing, iIP);
            break;
        case ERROR_INVALID_FILTER_ITEM:                                         //-126
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_FILTER_ITEM!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給bFilterSelection參數不是FILTER_DATA_ENCODER 或 FILTER_DATA_EMG_EL_SD_ORG_ALM_INP。", iRing, iIP);
            break;
        case ERROR_INVALID_SOFTWARE_LIMIT_SOURCE:                               //-127
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_SOFTWARE_LIMIT_SOURCE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤 ,設定給bCmpSource參數不是PULSE_COMMAND或ENCODER_POSITION。", iRing, iIP);
            break;
        case ERROR_INVALID_STOP_MODE:                                           //-128
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_STOP_MODE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤 ,設定給bStopMode參數的值不是SLOWDOWN_STOP 或 SUDDEN_STOP。", iRing, iIP);
            break;
        case ERROR_CONFLICT_WITH_VRING:                                         //-129
            EngStr->sprintf("MNet Ring %d IP %d ERROR_CONFLICT_WITH_VRING!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤 ,運動晶片內部比較器目前規劃為Variable-Ring counter。", iRing, iIP);
            break;
        case ERROR_INVALID_MOVE_DIRECTION:                                      //-130
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_MOVE_DIRECTION!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤 ,設定給bDirection參數的值不是MOVE_DIRECTION_FORWARD 或 MOVE_DIRECTION_REVERSE。", iRing, iIP);
            break;
        case ERROR_INVALID_HOME_MODE:                                           //-131
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_HOME_MODE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤 ,設定給bHomeMode參數的值不在0~12範圍內。", iRing, iIP);
            break;
        case ERROR_INVALID_EZ_COUNT:                                            //-132
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_EZ_COUNT!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤 ,設定給bEZcount參數的值不在0~15範圍內。", iRing, iIP);
            break;
        case ERROR_MOVE_HOLD:                                                   //-133
            EngStr->sprintf("MNet Ring %d IP %d ERROR_MOVE_HOLD!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤 ,指定裝置目前處於暫停模式(呼叫mn_group_start_move啟動)。", iRing, iIP);
            break;
        case ERROR_EMG_SIGNAL_ON:                                               //-134
            EngStr->sprintf("MNet Ring %d IP %d ERROR_EMG_SIGNAL_ON!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定裝置 ,EMG訊號被觸發。", iRing, iIP);
            break;
        case ERROR_ALM_SIGNAL_ON:                                               //-135
            EngStr->sprintf("MNet Ring %d IP %d ERROR_ALM_SIGNAL_ON!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定裝置 ,ALARM訊號被觸發。", iRing, iIP);
            break;
        case ERROR_MEL_SIGNAL_ON:                                               //-136
            EngStr->sprintf("MNet Ring %d IP %d ERROR_MEL_SIGNAL_ON!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定裝置 ,硬體負極限訊號被觸發。", iRing, iIP);
            break;
        case ERROR_PEL_SIGNAL_ON:                                               //-137
            EngStr->sprintf("MNet Ring %d IP %d ERROR_PEL_SIGNAL_ON!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定裝置 ,硬體正極限訊號被觸發。", iRing, iIP);
            break;
        case ERROR_WAIT_INP:                                                    //-138
            EngStr->sprintf("MNet Ring %d IP %d ERROR_WAIT_INP!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定裝置 ,等待INP訊號輸入", iRing, iIP);
            break;
        case ERROR_WAIT_ERC:                                                    //-139
            EngStr->sprintf("MNet Ring %d IP %d ERROR_WAIT_ERC!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定裝置 ,等待ERC輸出完成", iRing, iIP);
            break;
        case ERROR_WAIT_BACKLASH_CORRECT:                                       //-140
            EngStr->sprintf("MNet Ring %d IP %d ERROR_WAIT_BACKLASH_CORRECT!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定裝置 ,等待背隙補償完成", iRing, iIP);
            break;
//        case ERROR_WAIT_PULSE_IN:                                             //-141
//        case ERROR_MOTION_NOT_COMPLETE:                                       //-142
//        case ERROR_INVALID_FIX_MOVE_MODE:                                     //-143
        case ERROR_REGISTER_FULL:                                               //-144
            EngStr->sprintf("MNet Ring %d IP %d ERROR_REGISTER_FULL!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定裝置 ,連續運動的暫存器已使用。", iRing, iIP);
            break;
        case ERROR_INVALID_POSITION:                                            //-145
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_POSITION!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給Position參數的值不在-134217728~134217727範圍內。", iRing, iIP);
            break;
        case ERROR_INVALID_GROUPNO:                                             //-146
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_GROUPNO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給bGrpNo參數的值不在0~7範圍內。", iRing, iIP);
            break;
        case ERROR_INVALID_NUM_DEV:                                             //-147
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_NUM_DEV!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給bNumDev參數的值不存在。", iRing, iIP);
            break;
        case ERROR_GROUP_ALREADY_HOLD:                                          //-148
            EngStr->sprintf("MNet Ring %d IP %d ERROR_GROUP_ALREADY_HOLD!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定bGrpNo參數的群組目前處於暫停模式(需呼叫mn_group_start_move啟動)。", iRing, iIP);
            break;
        case ERROR_SET_ARC_FINISH_POS:                                          //-149
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_ARC_FINISH_POS!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給Dev1FinishPos與Dev2FinishPos的位置超出此圓弧運動可達範圍。", iRing, iIP);
            break;
        case ERROR_SET_BIT_DUMMY_DEV:                                           //-150
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_BIT_DUMMY_DEV!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給Low32BitDummyDevNo 或 High32BitDummyDevNo的虛擬輸出裝置編號包含bDev1No 或 bDev2No。", iRing, iIP);
            break;
        case ERROR_INVALID_DEV_HOLD:                                            //-151
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_DEV_HOLD!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定bGrpNo編號群組內，不含任何運動控制模組 ,可以暫停脈波輸出。", iRing, iIP);
            break;
        case ERROR_GROUP_NOT_HOLD:                                              //-152
            EngStr->sprintf("MNet Ring %d IP %d ERROR_GROUP_NOT_HOLD!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定bGrpNo編號群組內尚未暫停脈波輸出(需用mn_group_hold_move() 暫停))。", iRing, iIP);
            break;
        case ERROR_INVALID_VRING_SOURCE:                                        //-153
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_VRING_SOURCE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給bVRingSource的值不是PULSE_COMMAND 或 ENCODER_POSITION。", iRing, iIP);
            break;
        case ERROR_INVALID_VRING_VALUE:                                         //-154
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_VRING_VALUE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給dwVRingValue的值大於134217727。", iRing, iIP);
            break;
//        case ERROR_INVALID_CMPTRIG_SOURCE:                                    //-155
        case ERROR_INVALID_CMPTRIG_DIRECTION:                                   //-156
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_CMPTRIG_DIRECTION!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給bDirection的值不是CMPTRIG_DIRECTION_BOTH,CMPTRIG_DIRECTION_FORWARD 或 CMPTRIG_DIRECTION_REVERSE。", iRing, iIP);
            break;
        case ERROR_INVALID_CMPTRIG_PITCH:                                       //-157
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_CMPTRIG_PITCH!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 設定給wCnstPitch的值大於32767。", iRing, iIP);
            break;
        case ERROR_MOTION_IS_COMPLETED:                                         //-158
            EngStr->sprintf("MNet Ring %d IP %d ERROR_MOTION_IS_COMPLETED!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 指定的裝置已經完成運動。", iRing, iIP);
            break;
        case ERROR_INVALID_CARD_ID:                                             //-159
            EngStr->sprintf("MNet Ring %d ERROR_INVALID_CARD_ID!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, 作業系統找不到可使用的Motionnet板卡。", iRing);
            break;
        case ERROR_INVALID_OUTPUT_DATA:                                         //-160
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_OUTPUT_DATA!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d IO 模組錯誤, bData所設定數值大於0x0F。", iRing, iIP);
            break;
        case ERROR_INVALID_BITNO:                                               //-161
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_BITNO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d IO 模組錯誤, bBitNo設定超出範圍。", iRing, iIP);
            break;
        case ERROR_SET_BITNO:                                                   //-162
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_BITNO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d IO 模組錯誤, bBitNo設定超出MN3254可接受範圍。", iRing, iIP);
            break;
        case ERROR_INVALID_BYTENO:                                              //-163       ERROR_INVALID_PORTNO->ERROR_INVALID_BYTENO
            EngStr->sprintf("MNet Ring %d IP %d ERROR_INVALID_BYTENO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d IO 模組錯誤, bByteNo設定超出範圍。", iRing, iIP);
            break;
        case ERROR_SET_BYTENO:                                                  //-164
            EngStr->sprintf("MNet Ring %d IP %d ERROR_SET_BYTENO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d IO 模組錯誤, bByteNo或bPortNo設定超出可接受範圍。", iRing, iIP);
            break;
        case ERROR_CARD_ID_DUPLICATED:                                          //-165
            EngStr->sprintf("MNet Ring %d ERROR_CARD_ID_DUPLICATED!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, 有兩張以上的PISO-MN200板卡設定相同的Card ID。", iRing);
            break;
        case ERROR_CONFIG_FILE_LOAD:                                            //-166
            EngStr->sprintf("MNet Ring %d ERROR_CONFIG_FILE_LOAD!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d錯誤, 無法載入設定檔。", iRing);
            break;
        case ERROR_CONFIG_FILE_MATCH:                                           //-167
            EngStr->sprintf("MNet Ring %d ERROR_CONFIG_FILE_MATCH!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 模組錯誤, 設定檔所記錄的運動控制模組與bLineNo指定的模組數量或模組編號不相符。", iRing);
            break;
        case ERROR_COMM_DISCONNECT:                                             //-168
            EngStr->sprintf("MNet Ring %d ERROR_COMM_DISCONNECT!!!", iRing);
            ChStr->sprintf("MNet系統 Ring %d 錯誤, Motionnet 通訊傳輸中斷。", iRing);
            break;
        case ERROR_INVALID_WORDNO:                                              //-169
            EngStr->sprintf("MNet Ring %d ERROR_INVALID_WORDNO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d IO 模組錯誤, bWordNo 參數的設定超出範圍。", iRing, iIP);
            break;
        case ERROR_SET_WORDNO:                                                  //-170
            EngStr->sprintf("MNet Ring %d ERROR_SET_WORDNO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d IO 模組錯誤, bWordNo 參數的設定超出可接受範圍。", iRing, iIP);
            break;
        case ERROR_STEP_HOME_FAILED:                                            //-171
            EngStr->sprintf("MNet Ring %d ERROR_STEP_HOME_FAILED!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, 表示超過尋找原點的次數。", iRing, iIP);
            break;
        case ERROR_CONFLICT_WITH_CMPTRIG:                                       //-172
            EngStr->sprintf("MNet Ring %d ERROR_CONFLICT_WITH_CMPTRIG!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 運動模組錯誤, mn_cmp_stop表示非用作 IDX（同步）訊號的輸出或 mn_set_cmp表示非PA 、PB 訊號。", iRing, iIP);
            break;
        case ERROR_SET_CPU_EMU_DEV:                                             //-173
            EngStr->sprintf("MNet Ring %d ERROR_SET_CPU_EMU_DEV!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, ERROR_SET_CPU_EMU_DEV。", iRing, iIP);
            break;
        case ERROR_INVALID_CPU_EMU_START_ADDRESS:                               //-174
            EngStr->sprintf("MNet Ring %d ERROR_INVALID_CPU_EMU_START_ADDRESS!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, 不符合CPU emulation 的地址範圍。", iRing, iIP);
            break;
        case ERROR_MATCH_CPU_EMU_CMD:                                           //-175
            EngStr->sprintf("MNet Ring %d ERROR_MATCH_CPU_EMU_CMD!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, CPU emulation 的命令錯誤。", iRing, iIP);
            break;
        case ERROR_SET_CPU_EMU_ADDRESS_REFRESH:                                 //-176
            EngStr->sprintf("MNet Ring %d ERROR_SET_CPU_EMU_ADDRESS_REFRESH!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, CPU emulation 的刷新次數參數設定不為0，且刷新模式參數設定不為CPU_EMU_CMD_ADDR_REFRESH_ADD和 CPU_EMU_CMD_ADDR_REFRESH_SUB。", iRing, iIP);
            break;
        case ERROR_SET_CPU_EMU_COMM_WAIT:                                       //-177
            EngStr->sprintf("MNet Ring %d ERROR_SET_CPU_EMU_COMM_WAIT!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, CPU emulation 的等待通訊參數的設定不為0，且通訊等待參數的設定不為CPU_EMU_CMD_COMM_WAIT。", iRing, iIP);
            break;
        case ERROR_SET_CPU_EMU_PROCESS_MODE:                                    //-178
            EngStr->sprintf("MNet Ring %d ERROR_SET_CPU_EMU_PROCESS_MODE!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d 錯誤, CPU emulation 的處理模式參數的設定不為CPU_EMU_CMD_PROCESS_WRITE和CPU_EMU_CMD_PROCESS_READ。", iRing, iIP);
            break;
        case ERROR_INVALID_CHANNELNO:                                           //-179
            EngStr->sprintf("MNet Ring %d ERROR_INVALID_CHANNELNO!!!", iRing, iIP);
            ChStr->sprintf("MNet系統 Ring %d IP %d IO 模組錯誤, mn_set_ao、mn_set_ao_offset、mn_set_ao_gain表示所給予的通道編號，超出設定範圍值: 0~1。mn_get_ai表示所給予的通道編號，超出設定範圍值: 0~8。", iRing, iIP);
            break;
        default :
            bResult=false;
        break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
//Isaac 20181212 (Steven) : Baud Rate防呆功能
//void ShowMNetTree(TTreeView *TView)
//{
//    AnsiString Str, StrIP, StrType, StrBaudRate;
//    int iTreeTemp=0;
//    TTreeNode *RootNode[MAXRing], *Node;
//    TView->Items->Clear();
//    for(int i=0; i<MAXRing; i++)
//    {
//        Str.sprintf("Ring%d", i);
//        if(i==0)
//        {
//            TView->Items->Add(NULL, Str);
//            RootNode[i]=TView->Items->Item[iTreeTemp];
//        }
//        else
//        {
//            TView->Items->Add(RootNode[i-1], Str);
//            iTreeTemp++;
//            RootNode[i]=TView->Items->Item[iTreeTemp];
//        }

//        for(int j=0; j<MAXIP; j++)
//        {
//            if(myLine[i].Type[j]!=0)
//            {
//                switch(myLine[i].Type[j])
//                {
//                    case 0x8b:
//                        StrType="Type: Motion module";
//                        break;
//                    case 0x80:
//                        StrType="Type: Digital Output module";
//                        break;
//                    case 0x82:
//                        StrType="Type: Digital Input/Output module:";
//                        break;
//                    case 0x87:
//                        StrType="Type: Digital input module";
//                        break;
//                    case 0x8f:
//                        StrType="Type: Analog input module";
//                        break;
//                    default:
//                        StrType="Type: Unknown module type";
//                        break;
//                }
//
//                switch(myLine[i].RaudRate[j])
//                {
//                    case 0:
//                        StrBaudRate="BaudRate:2.5M";
//                        break;
//                    case 1:
//                        StrBaudRate="BaudRate:5M";
//                        break;
//                    case 2:
//                        StrBaudRate="BaudRate:10M";
//                        break;
//                    default:
//                        StrBaudRate="BaudRate:20M";
//                        break;
//                }
//
//                StrIP.sprintf("IP No.%d  ", j);
//                TView->Items->AddChild(RootNode[i], StrIP);
//                iTreeTemp++;
//                Node=TView->Items->Item[iTreeTemp];
//
//                TView->Items->AddChild(Node, StrType);
//                iTreeTemp++;
//
//                TView->Items->AddChild(Node, StrBaudRate);
//                iTreeTemp++;
//            }
//        }
//    }
//    TView->FullExpand();        //全部展開樹狀圖
//}
//------------------------------------------------------------------------------
