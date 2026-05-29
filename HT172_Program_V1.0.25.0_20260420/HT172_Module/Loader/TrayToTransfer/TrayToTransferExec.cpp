//---------------------------------------------------------------------------


#pragma hdrstop

#include "TrayToTransferExec.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#include "TrayToTransferStep.h"
#include "TLoaderInterface.h"

FSM_RESULT ExecTrayToTransfer(int step)
{
    switch(step)
    {
//        case TTW_RELEASE_STOP:
//            return g_pLoader->ReleaseStopper() ? FSM_DONE : FSM_RUNNING;
//
//        case TTW_MOVE_CW:
//            return g_pLoader->MotorCW() ? FSM_DONE : FSM_RUNNING;
//
//        case TTW_WAIT_IN_POS:
//            return g_pLoader->TrayInPosition() ? FSM_DONE : FSM_RUNNING;
//
//        case TTW_FIX_TRAY:
//            return g_pLoader->FixTray() ? FSM_DONE : FSM_RUNNING;
//
//        case TTW_MOVE_TO_WORK:
//            return g_pLoader->MoveToWork() ? FSM_DONE : FSM_RUNNING;
//
//        case TTW_DONE:
//            return FSM_DONE;
    }
    return FSM_ERROR;
}
 