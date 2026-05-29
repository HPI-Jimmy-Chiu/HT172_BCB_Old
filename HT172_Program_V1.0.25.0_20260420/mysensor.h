//---------------------------------------------------------------------------
#ifndef mysensorH
#define mysensorH
//---------------------------------------------------------------------------
#include "myio.h"
bool GetTrayBuildState(int pos);
void SetTrayBuildState(int pos);
void ClrTrayBuildState(int pos);
class TMySensor
{
    public:
    _fastcall TMySensor::TMySensor();
    TMyIo *Input;

    AnsiString Name;
    AnsiString OnAlarmCode;
    AnsiString OffAlarmCode;
    AnsiString AlarmType;
    int  Tag;
    AnsiString CardModal;
    AnsiString IOPos;

    int  Card;
    int  Port;
    int  Bit;
    int  Type;
    bool Status();
    bool IsOn();
    bool IsOff();
    bool Enable;
    bool EnableAtDataBase;
    bool bErroHappen;
    int  iStatus;
};

//#define MAX_SENSOR_ITEM 200
//extern class TMySensor Sen[MAX_SENSOR_ITEM];
//extern class TMySensor SenBackUp[20];
void CopySensor( TMySensor *Source ,TMySensor *Target);
#endif
