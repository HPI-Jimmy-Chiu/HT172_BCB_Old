//---------------------------------------------------------------------------
#ifndef HTMotorH
#define HTMotorH
//---------------------------------------------------------------------------
#include "halarm.h"
//---------------------------------------------------------------------------
#define     ALM_MOTOR_MOVE  55555
//---------------------------------------------------------------------------
enum {  iCwLed          =0,
        iHomeLed        =1,
        iCcwLed         =2,
        iEmgLed         =3,
        iAlarmLed       =4,
        iSoftcwLed      =5,
        iSoftccwLed     =6,
        iServoalarmLed  =7,
        iInposLed       =8,
        iZPhaseLed      =9,
        iServoOn        =10,
        iMotLedTotalCnt  //擺最下面,不要動,有新增時, MotorTest跟Teach都要一起改
     };

enum eMotionCardType{eMC8040A       =0,
                     ePCI885X       =1,
                     eMC88x1        =2,
                     eSMC           =3,
                     eMN200         =4,
                     ePLCbase       =5,
                     eMotionCardUnknown,    //擺最下面,不要動
                     eMotionCardTypeTotal   //擺最下面,不要動
                    };

enum eMotorKind{eMotor          =0,
                eLinerMotor     =1,
                eCylinderMotor  =2,
                eVoiceCoilMotor =3,
                eStepServo      =4,     //東方步進伺服
                eYASKAWA        =5,
                eMotorKindTotal     //保持在最下面
               };

//---------------------------------------------------------------------------
class HTMotor
{
    private:

    protected:
        unsigned int    iSpeed                                                  ;
        unsigned int    InitSpeed                                               ;
        unsigned int    Rate                                                    ;
        unsigned int    Range                                                   ;
        int             EncoderPos                                              ;
        int             iHomeObjectTask                                         ;
        double          dAcc                                                    ;
        double          dDec                                                    ;
        eMotorKind      MotorKind                                               ;
        eMotionCardType MotionCardType                                          ;

    public:
        HTMotor();
        virtual ~HTMotor()                                                      ;
        unsigned int    Address                                                 ;
        unsigned int    iBoardID                                                ;
        unsigned int    iPortID                                                 ;
        unsigned int    HomeHighSpeed                                           ;
        unsigned int    HomeLowSpeed                                            ;
        unsigned int    JogHighSpeed                                            ;
        unsigned int    JogLowSpeed                                             ;
        bool            Enable                                                  ;
        bool            Direction                                               ;
        bool            HomeDirection                                           ;
        bool            MotorType                                               ;
        bool            bSensorType                                             ;
        bool            bLimitLogic                                             ;
        bool            bIn1Logic                                               ;
        double          GearRatio                                               ;
        int             SoftLimitP                                              ;
        int             SoftLimitN                                              ;
        int             LastHomePos                                             ;
        int             EncoderType                                             ;
        bool            ServoAlarmOn                                            ;
        int             iMotNo                                                  ;   //Steven :方便Debug

        unsigned int    ReadSpeed()                                             ;
        unsigned int    ReadInitSpeed()                                         ;
        unsigned int    ReadRate()                                              ;
        unsigned int    ReadRange()                                             ;
        double          ReadAcc()                                               ;
        double          ReadDec()                                               ;
        bool            ReadServoAlarmOn()                                      ;
        int             ReadEncoderPos()                                        ;
        void            SetHomeobjectTask(int Task)                             ;
        void            SetMotNo(int No) {iMotNo=No;}                           ;   //Steven :方便Debug
        void            SetMotorKind(eMotorKind Kind){MotorKind=Kind;}          ;
        eMotorKind      GetMotorKind()               {return MotorKind;}        ;
        void            SetMotionCardType(eMotionCardType Type){MotionCardType=Type;};
        eMotionCardType GetMotionCardType()               {return MotionCardType;};

        virtual void    Stop()                       {}                         ;
        virtual bool    JogP()                       {return false;}            ;
        virtual bool    JogN()                       {return false;}            ;
        virtual int     ReadPos()                    {return 0;}                ;
        virtual bool    MoveTo(int Tar)              {return false;}            ;
        virtual bool    MoveToPosShortDistance(int Tar) {return MoveTo(Tar);};      //Sam 20240727 : 新增短距離加減速減半
        virtual bool    HomeObject()                 {return false;}            ;
        virtual bool    HomeFlag(void)               {return false;}            ;
        virtual bool    GetAlarm(void)               {return false;}            ;
        virtual void    SetSpeed(unsigned int x)     {}                         ;
        virtual void    SetInitSpeed(unsigned int x) {}                         ;
        virtual void    SetServoAlarmOn(bool Value)  {}                         ;
        virtual int     InitMotor(int IoAddress)     {return 0;}                ;
        virtual void    SetRange(unsigned int a)     {}                         ;
        virtual void    SetRate(unsigned int a)      {}                         ;
        virtual void    SetAcc(double a)             {}                         ;
        virtual void    SetDec(double a)             {}                         ;
        virtual int     ReadRealPos()                {return 0;}                ;
        virtual int     ReadEnCoderRealPos()         {return 0;}                ;
        virtual int     SetCommand(int p)            {return 0;}                ;
        virtual int     SetPosition(int p)           {return 0;}                ;
        virtual void    SetServoOn(bool IsOn)        {}                         ;

        //新增函數//
        virtual bool    ResetPos(int Pulse)             {return false;}         ;
        virtual void    SoftLimitEnable(bool bFlag)     {}                      ;
//        virtual void    ServoOnOff(bool bStatus)        {}                      ;
        virtual void    MotOutputOn(int iOutPort)       {}                      ;
        virtual void    MotOutputOff(int iOutPort)      {}                      ;
        virtual void    MotInputStatus(bool *bInputPort){}                      ;
        virtual void    EnableTrigger(int bFalg,int iMode,long lValue){}       ;
        virtual void    ManualTestTrigger(bool bOn){};
        virtual bool    LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag){return false;} ;
        virtual void    ScanMotorStatus(bool *Led)   {};
        virtual void    DecStop(void){};
        bool CheckArmPosInRange(int iNowPos,int iMin,int iMax);                     //jimmychiu 20220815 for index arm checking y pos before z down
        bool CheckArmPosArrival(int iNowPos,int iDestination,int iTolerance);       //jimmychiu 20220815 for index arm checking y pos before z down
};
//---------------------------------------------------------------------------

#endif
