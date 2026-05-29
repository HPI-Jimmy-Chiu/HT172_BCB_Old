//---------------------------------------------------------------------------

#ifndef mySMCmotorH
#define mySMCmotorH
#include "HTMotor.h"
#include "..\Include\CSmc.h"


extern const int SMC_CountType_OutputPulses    ;
extern const int SMC_CountType_EncodersPulses  ;
extern const int SMC_EventMode_InvalidateEvent ;
extern const int SMC_EventMode_ValidateEvent   ;
//---------------------------------------------------------------------------
class TMySMCMotor:public HTMotor
{
    private:
        char  cDeviceName[16];
        short Id ;
        int   iSMCHomeObjectTask;
        int   OldSpeed;
        bool  Open_SMCCard();
        void  Close_SMCCard();
        bool  SMCMotHome();
        void  SetEncodeMultiple(int iMultiple);
        void  SetServoAlarm();
        void  SetSMCInType();


    protected:
    public:
        __fastcall TMySMCMotor(int Addr);
        ~TMySMCMotor();
        virtual int     InitMotor(int IoAddress);
        virtual void    SetSpeed(unsigned int x);
        virtual void    SetSpeedShortDistance(unsigned int x);  //Sam 20240727 : 新增短距離加減速減半
        virtual void    SetInitSpeed(unsigned int x);
        virtual void    SetServoAlarmOn(bool Value);
        virtual void    SetAcc(double a);
        virtual void    SetDec(double a);
        virtual int     ReadPos();
        virtual void    ScanMotorStatus(bool *Led);
        virtual bool    MoveTo(int Tar);
        virtual bool    MoveToPosShortDistance(int Tar);    //Sam 20240727 : 新增短距離加減速減半
        virtual void    Stop();
        virtual void    DecStop();
        virtual bool    JogP();
        virtual bool    JogN();
        virtual bool    HomeObject();
        virtual void    SetRange(unsigned int a);
        virtual bool    GetAlarm(void);
        virtual bool    HomeFlag(void);

        virtual void    MotOutputOn(int iOutPort);
        virtual void    MotOutputOff(int iOutPort);
        virtual void    MotInputStatus(bool *bInputPort);
        virtual bool    LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag);
        virtual void    EnableTrigger(int iFlag, int iMode,long lValue);
        virtual bool    ResetPos(int Pulse);

        virtual bool    MotionDone();
        virtual int     ReadRealPos();
        virtual int     ReadEnCoderRealPos();
        virtual int     SetCommand(int p);
        virtual int     SetPosition(int p);
        virtual void    SetServoOn(bool IsOn);
        virtual void    SetSoftLimit(int iPLimit,int iNLimit);
        virtual void    SoftLimitEnable(bool bFlag);
        virtual void    ManualTestTrigger(bool bOn);
};
#endif
