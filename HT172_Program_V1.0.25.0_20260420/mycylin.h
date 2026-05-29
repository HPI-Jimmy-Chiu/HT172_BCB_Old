//---------------------------------------------------------------------------
#ifndef mycylinH
#define mycylinH
//---------------------------------------------------------------------------

#include "htimer.h"
#include "myswitch.h"
#include "mysensor.h"

#define MaxCylinderItem 200

enum eCynMotion{eOffNotOnErr    =0,         //Pop時,  Off Sensor不亮
                eOffNotOffErr   =1,         //Push時, Off Sensor不滅
                eOffIsOnErr     =2,         //Pop時,  Off Sensor亮, 表示機構跑過頭
                eOnNotOnErr     =3,         //Push時, On Sensor不亮
                eOnNotOffErr    =4,         //Pop時,  On Sensor不滅
                eOnIsOnErr      =5,         //Push時, On Sensor亮, 表示機構跑過頭
                eCynErrTotal
               };

class TMyCylinder{
    private:
        int Task;
        int OnOff;
        int iOnLeft;
        int iOnTop;
        int iOffLeft;
        int iOffTop;
//        bool    bCylinderOn;                                                    // ben edit //
    protected:
        TControl *PTempWinCtrl;
    public:
        _fastcall TMyCylinder::TMyCylinder();

        TMySensor  OnSensor;
        TMySensor  OffSensor;
        TMySwitch  Switch;


        AnsiString CylinderName;
        AnsiString OnSensorName;
        AnsiString OffSensorName;
        AnsiString FlushPanelName;
        AnsiString ErrorName[eCynErrTotal];
        HTimer  Delay;
        bool    Enable;
        bool    EnableAtDataBase;
        bool    bInitialOk;
        int     OnAlarmCode;
        int     OffAlarmCode;
        int     OnAlarmTime;
        int     OffAlarmTime;

        int     OnDelayTime;
        int     OffDelayTime;

        int     Tag;
        bool    Push();
        bool    Pop();
        void    On();
        void    Off();
        bool    Reset();
        bool    IsOn();
        bool    IsOff();
        bool    GetOutBit();
//        void    SetSimulateCompoment(TObject *PCtrl, int Left, int Top, int Right, int Bottom);

        void    SetSimulateCompoment(TObject *PCtrl, TAnchorKind Alignment, int simuStart, int simuEnd);
        void    UpdateSimulateCompomentPosition(bool bFlag);

};

extern class TMyCylinder Cylinder[MaxCylinderItem];
void InitialCylinderName();
#endif
