//---------------------------------------------------------------------------
#ifndef uhomeH
#define uhomeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ALed.hpp"
#include "butPa1.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------

class TfHome : public TForm
{
__published:    // IDE-managed Components
     TPanel *Panel1;
    TListBox *lstHomeMsg;
     TPanel *Panel104;
    TTimer *Timer1;
    TPanel *Panel2;
    TSpeedButton *SpeedButton1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer1Timer(TObject *Sender);
private:    // User declarations
     TALed  *LedPtr[128];
     TEdit  *EditMotorPos[128];
     TLabel *LabelMotorName[128];

     bool fLedInitial;
     int TestZTask;
     int iSingleMotHomeTask[MAX_MOTOR_TEST_COUNT];                              //KenHsieh 20240830 : add 運作中單軸回Home
public:     // User declarations
     enum eHomeLedColor{eHomeUnuse  =0,
                        eHomeOk     =1,
                        eHomeError  =2,
                        eHomeBusy   =3};

     __fastcall TfHome(TComponent* Owner);
     void ResetAllMotorLed();
     void ShowLed(int index, eHomeLedColor attr);
     void ShowMotorHomePos(int i);
     int iHomeStep;
     int iVoiceHomeStep;
     bool fShow;
     bool fAbort;
     int Data;
     void __fastcall ScanKey();
     bool __fastcall DoTestZHome(int Index);
     bool Gali_Z1Z2Home();
     bool Gali_YHome();
     void SetWidth();
     bool flagCheckVCMSafety;
     bool bCheckMotorPower;

     bool SortArm1ZHome(bool bReset, bool bRestHomeFlag=false, bool bYHome=false);
     bool SortArm2ZHome(bool bReset, bool bRestHomeFlag=false);
     bool TrayArmHome(bool bReset);
     bool PushStoreArmHome(bool bReset);
     bool MagazineX_Home(bool bReset, int iWhichMGZ);
     bool SingleMotorHome(bool bReset, int iMotor);                             //KenHsieh 20240830 : add 運作中單軸回Home
     bool TrollyZHome(bool bReset);
};
//---------------------------------------------------------------------------
extern PACKAGE TfHome *fHome;
bool ProcessMotorHome(bool Flag);
bool ProcessOnlyVoiceMotorHome(bool Flag);
bool ProcessSingleMotorHome(int);
extern void UpdateHomeParameter();
//---------------------------------------------------------------------------
#endif
