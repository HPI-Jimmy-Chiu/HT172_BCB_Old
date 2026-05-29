//---------------------------------------------------------------------------
#ifndef mytimeH
#define mytimeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TDataModule2 : public TDataModule
{
__published:    // IDE-managed Components
    void __fastcall Timer1Timer(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TDataModule2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule2 *DataModule2;
//---------------------------------------------------------------------------
#endif
