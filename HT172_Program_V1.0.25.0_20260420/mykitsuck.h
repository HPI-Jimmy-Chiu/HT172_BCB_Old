//---------------------------------------------------------------------------
#ifndef MyKitSuckH
#define MyKitSuckH


//---------------------------------------------------------------------------
#include "htimer.h"
#include "myled.h"

#include "myswitch.h"
#include "mysensor.h"
#include "deviceinfo.h"
#include "cmydef.h"
#include "MyMotor.h"

#define MAX_SUCKER_ROW 4
#define MAX_SUCKER_COL 2

enum eSuckError{ePickUpErr      =0,         //{"--Pick Up Error"},
                eDestoryErr     =1,         //{"--Destory Error"},
                eVacSenOffErr   =2,         //{"--Vacuum Sensor Off Error"},
                eDropErr        =3,         //{"--Device Be Droped Error"},
                eInitOffErr     =4,         //{"--Initial Sensor Off Error"},
                eInitOnErr      =5,         //{"--Initial Sensor On Error"}};
                eSuckErrTotal
               };

class TMySucker{

    private:
        int Task;
        int iMyBravoSuckTask;

        DWORD StartTick;
        DWORD EndTick;
    protected:
    public:
        _fastcall TMySucker::TMySucker();
        TMySensor  Sensor;
        TMySwitch  OnSw;
        TMySwitch  OffSw;

        DWORD   VacuumOnTime;
        DWORD   VacuumOffTime;
        DWORD   VacuumOnTimeBuffer[20];
        DWORD   VacuumOffTimeBuffer[20];

        HTimer  Delay;               // 真空產生
        AnsiString Alias;
        AnsiString SensorName;
        AnsiString OnPortName;
        AnsiString OffPortName;
        AnsiString SuckerName;

        int     iMyRow;
        int     iMyCol;
        int     iMyMot;

        bool    Error;
        bool    Enable;
        bool    EnableAtDataBase;
        bool    Status;
        AnsiString OnAlarmCode;
        AnsiString OffAlarmCode;
        int     OnAlarmTime;
        int     OffAlarmTime;
        int     OnDelayTime;
        int     OffDelayTime;
        int     iManualOffTask;

        int     iSuckType;          // 使用正壓或是負壓
        bool    Suck();             // 真空產生動作
        bool    Destroy();          // 真空破壞動作
        bool    Destroy(bool bDelayOff); //真空破壞動作,由自己下off()關閉真空
        void    On();               // 真空產生電磁閥 ON
        void    Off();              // 真空破壞電磁閥 ON
        void    OnSuck();           // 真空產生電磁閥 ON
        void    OffSuck();          // 真空破壞電磁閥 ON
        void    OnDestroy();        // 真空產生電磁閥 ON
        void    OffDestroy();       // 真空破壞電磁閥 ON
        void    Normal();

        void    Reset();
        bool    GetStatus();
        bool    GetOnBit();
        bool    GetOffBit();
        void    ResetSuckTask();
        void    SetRetryCount(int);
        void    CheckIsFallDown();
        bool    HasRealIC();
        bool    NoRealIC();

        void    PushOnTime();
        void    PushOffTime();
        bool    RealTimeRefreshVacuumOnOffTime;
        bool    bSuckOk;
        int     Tag;
        int     iWhichAuto;

        TDeviceInfo DeviceInfo;
        int     Item;
        TMyLed *pLed;
};
//---------------------------------------------------------------------------
class TMyKitSuck
{
    private:
    protected:
    public:
    __fastcall TMyKitSuck();
    __fastcall ~TMyKitSuck();
    TMySucker Suck[MAX_SUCKER_ROW][MAX_SUCKER_COL];
    AnsiString Name;
    AnsiString AlarmName[eSuckErrTotal];
    AnsiString FlushPanelName;
    void initMyKitSuck(AnsiString sName, AnsiString sFlushPanel, int iRow, int iCol);
    int  MaxItem;
    int  MaxItemR;
    int  MaxItemC;
    void SetMyLed(int iR, int iC, TMyLed *ledPtr);
    void SetItemData(int iR, int iC, int Data);
    void SetItemData(int iR, int iC, int Data, TDeviceInfo DInfo);
    void SetItemAmount(int iR, int iC);
    bool NoIC();
    bool HasIC();
    bool HasType(int p);
    int  GetCountOfDeiveType(int data);
    bool AllIs_HAS_NULL_IC();
    bool HasRealIC();
    void ClearSingle(int iR, int iC);
    void ClearAll();
    void SetAll(int Type);
    bool HasTestedDevice();
    bool HasDeviceNotTested();
    void MoveSingleItem(TTrayMotor *MotPtr, int iR, int iC, int X, int Y);
    void MoveSingleItem(TMyKitSuck &Source, int iR, int iC);
    void CopySingleItem(TMyKitSuck &Source, int iR, int iC);
//    void MoveAllItem(class TMyKitSuck &Source);
//    void CopyFrom(class TMyKitSuck &Source);
    void MoveSuckData(TMyKitSuck &Source, int iR, int iC);
//    void MoveSuckDataDiff(class TMyKitSuck &Source, int iR, int iC, int tag);
    bool AllDeviceTested();
    void ResetAll();
    void ClearAllError();
    void CheckVaccumIsIniaialON(int iR, int iC, bool &flag);
    int  Tag;                                                                   //V1.1

    //Steven 20130419 : 新增
    AnsiString GetErrorPart(int AlarmStatus);                                   //回傳有出錯的位置,傳入值是要檢查是掉料還是吸取異常
    void SkipError(int AlarmStatus, int iICType=EMPTY_IC);                      //將有錯誤的位置改為NULL_IC
    void SkipError(int AlarmStatus, TMyKitSuck &Source, int iICType=EMPTY_IC);  //將有錯誤的位置if(bSetToNull==true)改為 NULL else改為 傳入的Kit的狀態
    bool CheckHasError(int AlarmStatus,int &iSuckCol,int &iSuckRow);            //確認吸嘴有沒有出錯
    bool CheckHasError(int AlarmStatus, TMyKitSuck &Source);                    //確認吸嘴有沒有出錯, 並將無錯誤的進行資料交換

};
#endif
