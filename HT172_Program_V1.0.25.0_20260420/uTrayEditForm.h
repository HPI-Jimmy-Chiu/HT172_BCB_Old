//------------------------------------------------------------------------------
#ifndef uTrayEditFormH
#define uTrayEditFormH
//------------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "HTray.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "mymotor.h"
//------------------------------------------------------------------------------
class TTrayEditForm : public TForm
{
__published:    // IDE-managed Components
    TTMyTray *mtLoaderBuffer;
    TTimer *Timer1;
    TSpeedButton *btnUpdate;
    TSpeedButton *btnAbort;
    TButton *btnManualInput;
    TComboBox *cbBinCount;
    TLabel *lblBin;
    TLabeledEdit *edtID;
    TLabeledEdit *edtXPos;
    TLabeledEdit *edtYPos;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall mtLoaderBufferMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtLoaderBufferMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall mtLoaderBufferMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btnUpdateClick(TObject *Sender);
    void __fastcall btnAbortClick(TObject *Sender);
    void __fastcall btnManualInputClick(TObject *Sender);
    void __fastcall edtXPosClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall edtYPosClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TTrayEditForm(TComponent* Owner);
    void __fastcall Change();
    void __fastcall ShowTray();
    void __fastcall SetTray();
    int iSendCT;
    bool fShow;
    bool bSetBin;
    void EditTray(int MotorIndexIndex, bool _bSetBin=true);
};
//------------------------------------------------------------------------------
extern PACKAGE TTrayEditForm *TrayEditForm;

//------------------------------------------------------------------------------
#endif
