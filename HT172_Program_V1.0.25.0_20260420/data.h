//---------------------------------------------------------------------------
#ifndef dataH
#define dataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "ALed.hpp"
#include <MPlayer.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <DBCGrids.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include "HTray.h"
#include <NMFtp.hpp>
#include <Psock.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TfData : public TForm
{
__published:    // IDE-managed Components
        TPageControl *PageData;
        TTabSheet *tsData0;
        TTabSheet *tsData1;
        TPanel *Panel1;
    TTabSheet *tsHandlerInfo;
    TLabel *Label35;
    TLabel *Label38;
    TLabel *Label40;
    TLabel *Label6;
    TLabel *Label11;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label1;
    TLabel *Label5;
    TLabel *Label7;
    TTimer *Timer1;
    TPanel *labSerialNo;
    TPanel *labModel;
    TPanel *labFactory;
    TPanel *labVersion;
    TPanel *labPowerOnTime;
    TPanel *labRunningTime;
    TPanel *labProductTime;
    TPanel *labPauseTime;
    TPanel *labJamTime;
    TPanel *labHomeTime;
    TPanel *labMTBA;
    TPanel *labMachineId;
    TLabel *Label8;
    TPanel *Panel3;
    TSpeedButton *sbDataExit;
    TMemo *Memo1;
    TTabSheet *tsRunInfo;
    TPanel *Panel2;
    TSpeedButton *sbHandlerInfo;
    TPanel *Panel4;
    TPanel *Panel5;
    TSpeedButton *sbDataChange0;
    TPanel *Panel6;
    TSpeedButton *sbRunInfo;
    TSpeedButton *sbDataChange1;
    TLabel *Label9;
    TPanel *plTotalIC;
    TLabel *Label10;
    TPanel *plJamCount;
    TLabel *Label12;
    TPanel *plJamRate;
    TLabel *lbJamratedenom;
    void __fastcall sbDataChange0Click(TObject *Sender);
    void __fastcall sbDataExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer1Timer(TObject *Sender);
private:    // User declarations
    TStringList *PtrStringList;
public:     // User declarations
     __fastcall TfData(TComponent* Owner);
     bool bShowTTLChart;
     bool fShow;
     int iTTLChartCount;
     int iTotoalTestTime;
     void UpdateHandlerInfoUI();
     void UpdateRunInfoUI();
};
//---------------------------------------------------------------------------
extern PACKAGE TfData *fData;
extern bool bOnLineEnable;//Eliot 2007_11_14
//---------------------------------------------------------------------------
#endif
