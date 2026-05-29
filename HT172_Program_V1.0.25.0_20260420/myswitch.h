//---------------------------------------------------------------------------
#ifndef myswitchH
#define myswitchH
#include "myio.h"
//---------------------------------------------------------------------------

class TMySwitch{
    public:
    _fastcall TMySwitch::TMySwitch();
    AnsiString Name;
    TMyIo *Output;

    int  Tag;
    AnsiString CardModal;
    AnsiString IOPos;
    int  Card;
    int  Port;
    int  Bit;
    bool OutValue;
    bool SetValue;
    int  Type;
    void On();
    void Off();
    bool Status();
    bool Enable;
    bool EnableAtDataBase;
    void OnOff(bool);
};
//#define MAX_SWITCH_ITEM 200
//extern class TMySwitch SW[MAX_SWITCH_ITEM];
extern class TMySwitch SWBackup[8];
void CopySwitch( TMySwitch *Source ,TMySwitch *Target);
#endif
