//---------------------------------------------------------------------------


#pragma hdrstop

#include "TrayToWorkExec.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#include "TrayToWorkStep.h"
#include "TLoaderInterface.h"
FSM_RESULT ExecTrayToWork(int step)
{
    switch(step)
    {
        case TTW_INIT:
            return FSM_DONE;

//        case TTW_RELEASE_STOP:
//            return g_pLoader->ReleaseStopper() ? FSM_DONE : FSM_RUNNING;
//
//        case TTW_MOVE_CW:
//            return g_pLoader->MotorCW() ? FSM_DONE : FSM_RUNNING;
//
//        case TTW_TRAY_IN:
//            return g_pLoader->TrayInPosition() ? FSM_DONE : FSM_RUNNING;
//
//        case TTW_FIX:
//            return g_pLoader->FixTray() ? FSM_DONE : FSM_RUNNING;

        case TTW_DONE:
            return FSM_DONE;
    }

    return FSM_ERROR;
}
//---------------------------------------------------------------------------
