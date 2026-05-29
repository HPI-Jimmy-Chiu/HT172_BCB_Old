//---------------------------------------------------------------------------

#ifndef PowerSavingModeH
#define PowerSavingModeH

#include "myswitch.h"
//---------------------------------------------------------------------------
class TModule
{
    public:
    __fastcall TModule();
    __fastcall ~TModule();

    bool Enabled;                                          //Module啟用
    TMySwitch *mSW;
    TDateTime AlarmTmr;                                    //設定警報時間

    bool TimeOut(TDateTime StartTmr);                      //倒數
    virtual bool Doing(bool Reset =false);                 //時間到,Module進入處理工作
};
//---------------------------------------------------------------------------
class TMtrModule : public TModule
{
    public:
    bool Doing(bool Reset =false);
};
//---------------------------------------------------------------------------
class TPumpModule : public TModule
{
    public:
    bool Doing(bool Reset =false);
};
//---------------------------------------------------------------------------
class TPowerSaving
{
    private:
    bool flagStartTmr;                                    //機台閒置進入休眠倒數旗標
    bool flagUseFunc;                                     //休眠功能啟用旗標
    TDateTime StartTmr;

    public:
    __fastcall TPowerSaving();
    __fastcall ~TPowerSaving();

    TTimer *ScanTmr;
    TMtrModule  *MtrModule;                               //控制馬達休眠Moduel
    TPumpModule *PumpModule;                              //控制幫浦裝置休眠Moduel
    TModule *tModule;                                     //處理進入休眠ShowMessage

    void __fastcall OnScanTmr(TObject *Sender);           //Scan Timer
    void SetFunction(bool OnOff);                         //設定啟用的Module
    void SetAlarmTime(TDateTime &Module ,int Minute);     //設定警報時間
    bool CheckSystemRun();                                //檢查是否進入休眠
    void Restart();                                       //通知謷報重新計數

    bool bRestart;                                        //重新計數
};
//---------------------------------------------------------------------------
extern class TPowerSaving *tPowerSaving;
#endif

