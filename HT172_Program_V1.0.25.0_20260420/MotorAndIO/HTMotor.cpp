//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell ï¿½N.hï¿½Î¤@,ï¿½iï¿½[ï¿½tbuild
#pragma hdrstop

#include "HTMotor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
HTMotor::HTMotor()
{
    Address         = 0x340   ;
    GearRatio       = 0       ;
    HomeLowSpeed    = 100     ;
    JogHighSpeed    = 100     ;
    JogLowSpeed     = 100     ;
    Rate            = 100     ;
    SoftLimitN      = -999999 ;
    SoftLimitP      = 999999  ;
    Range           = 20      ;
    InitSpeed       = 10      ;
    Enable          = false   ;
    ServoAlarmOn    = false   ;
    Direction       = true    ;
//    ServoAlarmOn    = true    ;
    MotorType       = true    ;
    MotorKind       = eMotor  ;
    bSensorType     = true    ;
    HomeDirection   = true    ;
}
//---------------------------------------------------------------------------
HTMotor::~HTMotor()
{
}
//---------------------------------------------------------------------------
unsigned int HTMotor::ReadSpeed()
{
    return iSpeed;
}
//---------------------------------------------------------------------------
unsigned int HTMotor::ReadRange()
{
    return Range;
}
//---------------------------------------------------------------------------
unsigned int HTMotor::ReadRate()
{
    return Rate;
}
//---------------------------------------------------------------------------
int HTMotor::ReadEncoderPos()
{
    return EncoderPos;
}
//---------------------------------------------------------------------------
bool HTMotor::ReadServoAlarmOn()
{
    return ServoAlarmOn;
}
//---------------------------------------------------------------------------
unsigned int HTMotor::ReadInitSpeed()
{
    return InitSpeed;
}
//---------------------------------------------------------------------------
double HTMotor::ReadAcc()
{
    return dAcc;
}
//---------------------------------------------------------------------------
double HTMotor::ReadDec()
{
    return dDec;
}
//---------------------------------------------------------------------------
void HTMotor::SetHomeobjectTask(int Task)
{
    iHomeObjectTask=Task;
}
//---------------------------------------------------------------------------
bool HTMotor::CheckArmPosInRange(int iNowPos, int iMin, int iMax)
{
    int iTemp;
    if(iMin>iMax)
    {
        iTemp=iMax;
        iMax=iMin;
        iMin=iTemp;
    }

    if(iMin<=iNowPos &&  //JerryYang 20230309 :­×¥¿index arm²¾°Ê«OÅ@
       iNowPos<=iMax)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//----------------------------------------------------------------------------
bool HTMotor::CheckArmPosArrival(int iNowPos, int iDestination, int iTolerance)
{
    if(CheckArmPosInRange(iNowPos, iDestination-iTolerance, iDestination+iTolerance))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//----------------------------------------------------------------------------
