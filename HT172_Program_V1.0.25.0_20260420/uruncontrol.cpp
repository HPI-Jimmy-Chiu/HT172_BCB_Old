//---------------------------------------------------------------------------

#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "uruncontrol.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TRunControl::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
TRunControl *MyThread;
extern  bool InitialOK;
//==============================================================================
__fastcall TRunControl::TRunControl(bool CreateSuspended)
    : TThread(CreateSuspended)
{
}
//==============================================================================
void __fastcall TRunControl::ThreadProcess(void)
{
    MainProc();
}
//==============================================================================
void __fastcall TRunControl::Execute()
{
    //---- Place thread code here ----
    static int ict=0;
    timeBeginPeriod(1);
    do{
//        ict++;
//        Synchronize( ThreadProcess );
//        if(ict>=10)
//        {
//            SleepEx(1,true);
//            ict=0;
//        }
        Synchronize( ThreadProcess );
        if(HSys.Sys.SystemStart==false )
        {
            SleepEx(1,true);
        }
        else
        {
            ict++;
            if(ict>2)
            {
                ict=0;
            }
            else
            {
                SleepEx(1,true);
            }
        }
    } while (!Terminated);    //do

}
//==============================================================================
