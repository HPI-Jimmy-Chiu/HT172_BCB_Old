//---------------------------------------------------------------------------
#ifndef MyMotorH
#define MyMotorH
#include "mytray.h"
#include "htimer.h"
#include "htray.h"
#include "HTMotor.h"

enum eMotError{eMotPwrErr       =0,         //{"--Motor Power Off Error"},
               eMotTorqueErr    =1,         //{"--Motor Out Of Torque Error"},
               eMotCWOnErr      =2,         //{"--Motor CW sensor ON Error"},
               eMotCCWOnErr     =3,         //{"--Motor CCW sensor ON Error"},
               eMotSoftPErr     =4,         //{"--Motor Soft P position Error"},
               eMotSoftNErr     =5,         //{"--Motor Soft N positionError"},
               eMotPosErr       =6,         //{"--Motor position Error,Home and restart"},
               eMotUnDefErr     =7,         //{"--Motor Undefine Error"}};
               eMotOverLimitErr =8,         //{"--Motor Out of limit"},
               eMotErrTotal
               };
//---------------------------------------------------------------------------
typedef struct MyMotorSimulateStruct
{
    TControl *PWinCtrl;
    int     RefStart   ;
    int     RefEnd     ;
    int     FactStart  ;
    int     FactEnd;
    double   Scale;
    bool     bUpDownMove;
}MyMotorSimulateList;

typedef struct MyLockStruct
{
    AnsiString MotorAlias;
    AnsiString LockFunc;
    int LockTask;
}MyLockList;

//---------------------------------------------------------------------------
class TMyMotor
{
    private:
        HTimer      ResetTime;
        HTimer      htWaitHomeSensorOnDelay;
        bool        bShowSimulateCompoment;
        int         iHomeControlTask;
        int         iHomeRetryCT;
        bool        MotorMoveSub(int p,bool bCheckLed);
        bool        MotorMovePosition(int &Position,int iSpeed,int Target);
        bool        SimulateMotorMovePosition(int &Position,int iSpeed,int Tar);
        int         HomeControl(bool);
        void        InitHomeControlTask();
        int         speed;                                                      // run speed
        HTMotor     *Motor;
        //新增變數//
        bool        bLinearStatus;
        bool        VoiceCoilMotorHome();
        int         LastHomePos;
        HTimer      HomeDelay;
    protected:
        TList       *LockList;
        TList       *SimuCtrlList;
        int         iHomeTask;
        bool        MoveTo(int Tar);
        bool        MoveToPosShortDistance(int Tar);    //Sam 20240727 : 新增短距離加減速減半
        bool        HomeObject(void);
        bool        GetAlarm(void);
        int         OldSpeed;
    public:
        __fastcall  TMyMotor();
        ~TMyMotor() ;
        bool        Led[iMotLedTotalCnt];
        bool        bHomeFlag;
        bool        bHomeFinish;
        int         SimulateSpeed;
        int         Position;
        int         EncoderPosition;
        bool        bErrorMove;                 //下MotroMove前，如遇到軟體極限，會設定成true //
        int         OriginRate;                 //記錄原始的加減速，某些機台移動會改變加減速  //
        int         OriginRange;                //記錄原始的倍率，某些機台移動會改變倍率      //
        bool        bIsServoMotor;              //是否為伺服馬達，伺服馬達會設成true          //
        bool        (*MoveCheckCallBack)();     //不知道有何作用                              //
        int         Tag;                        //記錄為第幾軸馬達                            //
        int         TargetPosition;             //目的位置                                    //
        AnsiString  Alias;                      //記錄馬達名稱                                //
        AnsiString  Number;                     //記錄馬達號碼                                //
        AnsiString  NumberAlias;                //記錄馬達名稱，名稱範例[M0X]MXXXXX           //
        AnsiString  AlarmName[eMotErrTotal];
        bool        bQuickHome;                 //碰到Home sensor時設成true，其它馬達

        AnsiString  CardModel;
        void        InitMotor(int IoAddress);
        bool        JogP();
        bool        JogN();
        void        Stop();
        bool        MotorMove(int p);
        int         MotorMove(int p, int PreDonePos, bool JogP);
        bool        MotorMoveSKLED(int p);
        bool        Home(AnsiString &sErr);
        void        InitHomeTask();
        int         ReadPos();
        int         ReadEncoderPos();
        void        ScanMotorStatus();

        //Speed---
        int         iPersentSpeed;
        void        SetPersentSpeed(int persent, bool bSave=true);              //JerryYang 20240527 : 加快出料速度
        void        SetSpeed(unsigned int p);
        void        SetInitSpeed(unsigned int x);
        void        SetRange(unsigned int a);
        void        SetRate(unsigned int a);
        void        SetAcc(double a);
        void        SetDec(double a);
        void        SetHomeHighSpeed(unsigned int a);
        void        SetHomeLowSpeed(unsigned int a);
        void        SetJogHighSpeed(unsigned int a);
        void        SetJogLowSpeed(unsigned int a);

        int         GetInitSpeed();
        int         GetPersentSpeed();
        int         GetSpeed();
        int         GetRange();
        int         GetRate();
        double      GetAcc();
        double      GetDec();
        int         GetHomeHighSpeed();
        int         GetHomeLowSpeed();
        int         GetJogHighSpeed();
        int         GetJogLowSpeed();

        int         GetAddress();
        int         GetErrorIndex();
        void        UpdateSimulateCompomentPosition();
        void        SetShowSimulateCompomentFlag(bool flag);
//        void        SetSimulateCompoment(TObject *PCtrl, int StartPos,int EndPos,int Left,int Top,int Right,int Bottom);
        void        SetSimulateCompoment(TObject *PCtrl, TAnchorKind Alignment, int StartPos, int EndPos, int simuStartPos, int simuEndPos);
        void        Lock(AnsiString MotorAlias,AnsiString FunctionName,int Task);
        void        UnLock(AnsiString MotorAlias,AnsiString FunctionName);
        void        ClearLock();
        int         GetLockCount();
        AnsiString  GetLockString(int Index);

        //Direction---
        void        SetDirection(bool Value);
        void        SetHomeDirection(bool Value);
        void        SetGearRatio(double a);
        void        SetSoftLimitN(int a);
        void        SetSoftLimitP(int a);
        void        GetHomeDirection();
        int         GetLastHomePos();
        double      GetGearRatio();
        int         GetSoftLimitP();
        int         GetSoftLimitN();

        //Type-------
        void        SetServoAlarmOn(bool Value);
        void        SetMotorType(bool Value);
        void        SetSensorType(bool Value);
        void        SetEnable(bool Value);
        bool        ReadServoAlarmOn();
        bool        GetEnable();
        void        SetLimitLogic(bool logic);
        void        SetIn1Logic(bool logic);

        void        InitialMotorObject(int addr);

        //新增函數//
        void        SetMotorKind(eMotorKind Kind)                           ;
        eMotorKind  GetMotorKind()                                          ;
        void            SetMotionCardType(eMotionCardType Type)             ;
        eMotionCardType GetMotionCardType()                                 ;
        void        SetHomeOrder(AnsiString OrderString)                    ;   //Steven : 歸零的順序
        TStringList *HomeOrder                                              ;   //Steven : 歸零的順序
        void        SetMotNo(int No) {Motor->iMotNo=No;}                        ;   //Steven :方便Debug
        bool        ResetPos(int p);
        void        SetEncoderToCommand();
        void        SoftLimitEnable(bool bFlag);
//        void        ServoOnOff(bool bStatus);
        void        MotOutputOn(int iOutPort);
        void        MotOutputOff(int iOutPort);
        void        MotInputStatus(bool *bInputPort);
        bool        GetDirection();
        bool        LinearAxisMoveTo(TMyMotor* LineMotPtr[8],long lPos[8]);
//        bool        LinearAxisMoveTo(int iAxis[8],long lPos[8]);
        void        SetEncoderType(int a);
//        bool        SafetyMotorMove(int p);         //2012-06-01    Dell    Add for VCM safe
        void        InitHomeTask_forSingleAxis();   //2012-07-17    Dell    for 運轉中Single Home
        void        DecStop();                      //2012-07-17    Dell    減速停止
        void        ServoOnOff(bool IsOn);
        void        ServoOnResetPos();       //Steven 20110628 : Servo On後,重設Command Pos
        void        ClearPosition(int cmd);    //清空Command & Encoder Pos
        AnsiString  FlushPanelName;

        void        EnableTrigger(int iFlag, int iMode, long lValue);
        void        ManualTestTrigger(bool bOn);
        int         CompareCommandPos(int iPos, int iGap);      //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
        bool        CheckArmPosInRange(int iNowPos,int iMin,int iMax);                 //jimmychiu 20220815 for index arm checking y pos before z down
        bool        CheckArmPosArrival(int iNowPos,int iDestination,int iTolerance);   //jimmychiu 20220815 for index arm checking y pos before z down
        bool        CheckSoftLimit(int p,bool bAlarm=true);
};
class TTrayMotor: public TMyMotor
{
    private:
    protected:
        bool     fHTary;
        bool     fPanel;
        bool     fSubHTary;
        TTMyTray *pHTray;
        TTMyTray *pSubHTray;
        TPanel   *pPanel;
        bool     fPanelID;
        TPanel   *pPalTrayID;

    public:
        __fastcall TTrayMotor();
        ~TTrayMotor();
        bool  fHasTray;
        bool  bHasCover;
        class TMyTray Tray;
        bool  HasIC();
        bool  HasRing();
        bool  HasRealIC();
        bool  RowYFullIC(int Y);
        bool  FullIC();
        bool  FullThisIC(int data);
        bool  HasThisIC(int data);

        void  SetTrayInfo(int iRow, int iCol);
        void  SetPTrayData(int x, int y, int iBin);
        void  SetTraySingleData(int x, int y, int data);
        void  SetTraySingleData(int x, int y, int data, int iBin);
        void  SetTraySingleData(int x, int y, int data, TDeviceInfo DInfo);
        void  Refresh();
        void  InitNewTray(int data);
        void  InitEmptyTray();
        void  SetIDPanel(TPanel *ptr);
        void  SetTrayPanel(TPanel *ptr);
        void  SetHTrayPanel(TTMyTray *ptr);
        void  SetSubHTrayPanel(TTMyTray *ptr);
        void  CopyTrayFrom(int Index);
        void  MoveTrayFrom(int Index);
        void  SetTray(int data, bool bWithCover=false);
        void  ClearTray();
        void  SetTrayID(AnsiString ID);
        void  SetTrayVisible(bool bVisible);
        void  SetTrayXYItem(int iXItem, int iYItem);
        void  SetHTrayParameter(int iWidth, int iHeight, int iTop, int iLeft);
        void  SetSubHTrayParameter(int iWidth, int iHeight, int iTop, int iLeft);
        void  CopyTrayFrom(TTrayMotor *MotPtr);                                 //V1.1
        void  MoveTrayFrom(TTrayMotor *MotPtr);                                 //V1.1
        bool  Find1stIC(int &iRow,int &iCol);
        void  SaveWinstekLog();
        void  GetNewNullTray();
};
#endif
