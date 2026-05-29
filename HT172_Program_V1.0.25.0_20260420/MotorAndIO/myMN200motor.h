//---------------------------------------------------------------------------

#ifndef myMN200motorH
#define myMN200motorH
#include "HTMotor.h"

#include "HTimer.h"
#include "MN200.h"

//const int MAXRing=4;      //Steven 20140828 : 2 --> 4 for XY-Pitch
//const int MAXIP=64;
//const int MAXPort=4;

//---------------------------------------------------------------------------
class TMyMN200Motor:public HTMotor
{
    private:
        char  cDeviceName[16];
        short Id ;
        int   iMN200HomeObjectTask;
        int   OldSpeed;
        HTimer HomeDelay;
        void  MN200SoftLimitEnable(bool bFlag);
//        bool  MN200MotHome();
        bool  MN200MotHome();
        bool  MN200OpenCard();
        void  SetServoAlarm();
        void  SetMN200InType();
        void  GetMN200ErrorMessage(int nErrCode, AnsiString FUNC);
        SPEED_PAR MN200SpeedPar;
        int LP, LN;
    protected:
    public:
        __fastcall TMyMN200Motor(int Addr);
        ~TMyMN200Motor();
        virtual int     InitMotor(int IoAddress);
        virtual void    SetSpeed(unsigned int x);
        virtual void    SetInitSpeed(unsigned int x);
        virtual void    SetServoAlarmOn(bool Value);
                void    SetAcc(double a);
                void    SetDec(double a);
        virtual int     ReadPos();
        virtual void    ScanMotorStatus(bool *Led);
        virtual bool    MoveTo(int Tar);
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
        virtual void    EnableTrigger(bool bFlag,int iMode,long lValue);
        virtual bool    ResetPos(int p =0);
        virtual bool    MotionDone();
        virtual void    SetArmMaxSpeed();
        virtual int     ReadRealPos();
        virtual int     ReadEnCoderRealPos();
        virtual int     SetCommand(int p);
        virtual int     SetPosition(int p);
        virtual void    SetServoOn(bool IsOn);
        virtual void    SetSoftLimit(int iPLimit,int iNLimit);

        //-----------------------------
//        virtual void    SetEnableLatch(bool a);
        virtual void    ResetLatch();
        virtual int     GetLatchTotalLen();
        virtual int     GetLatchBuffer(int LatchInputNo, unsigned short *AxisCounterNo, long *LatchDataTable);
        virtual int     GetLatchBuffer(int LatchInputNo, short *AxisCounterNo, long *LatchDataTable) {return 0;};
        virtual bool    GetLatchIOStatus(unsigned short LatchInputNo);
        virtual void    SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int Enable);
};

extern bool GetMN200_Error_Code(int iRing, int iCode, AnsiString *EngStr, AnsiString *ChStr, int iIP=-1);
extern void ShowMNetTree(TTreeView *TView);
#endif
