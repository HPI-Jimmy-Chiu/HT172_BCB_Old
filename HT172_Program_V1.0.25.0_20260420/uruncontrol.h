//---------------------------------------------------------------------------
#ifndef uruncontrolH
#define uruncontrolH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TRunControl : public TThread
{
private:
protected:
    void __fastcall Execute();
public:
    __fastcall TRunControl(bool CreateSuspended);
    void __fastcall TRunControl::ThreadProcess(void);
};
extern TRunControl *MyThread;
//---------------------------------------------------------------------------
#endif
