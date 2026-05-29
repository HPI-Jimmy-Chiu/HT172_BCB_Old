//---------------------------------------------------------------------------
#ifndef mymessboxH
#define mymessboxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TMyMessageBox : public TForm
{
__published:    // IDE-managed Components
    TPanel *palPause;
    TTimer *Timer1;
    TPanel *Panel1;
    TLabel *Label1;
    TLabel *Label2;
    TMemo *Memo1;
    TEdit *Edit1;
    TLabel *Label3;
    TLabel *Label4;
        TButton *Button2;
        TButton *Button1;
    TPanel *palYes;
    TPanel *palNo;
   void __fastcall palPauseClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
    void __fastcall palYesClick(TObject *Sender);

private:    // User declarations
    int Status;
public:     // User declarations
    __fastcall TMyMessageBox(TComponent* Owner);
    int ret;
    bool flushState;
    int flushCT;
    bool fShow;
    bool fScanPanel;
    char Message[256];
    char CHMessage[256];                                                        // ben add 20110706 //
    char ENMessage[256];                                                        // ben add 20110706 //
    bool bFormShowNoStop;
    enum eMyMessageRtn
    {
        msgrtnPAUSE=0,
        msgrtnYES=1,
        msgrtnNO=2,
    };
};
//---------------------------------------------------------------------------
extern PACKAGE TMyMessageBox *MyMessageBox;
void ShowMyMessage(AnsiString S);
void ShowMyMessage_Run(AnsiString S1, AnsiString S2);
int  ShowMyMessageBox_YES_NO(AnsiString str);
extern void CloseBuzzerOff();                                                   // ben add 20110706 //
//---------------------------------------------------------------------------
#endif
