//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "cCCDCtrl.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------      
//---------------------------------------------------------------------------
FSM_RESULT CCDExecTrampoline(int stepId, void* ctx)
{
    TMyCCDIDPanel* exec =
        static_cast<TMyCCDIDPanel*>(ctx);
    if (!exec)
        return FSM_ERROR;                
    return exec->DoExecCCDStep(stepId);  
}
//---------------------------------------------------------------------------
