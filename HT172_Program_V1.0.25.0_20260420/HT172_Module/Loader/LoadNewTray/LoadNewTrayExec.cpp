//---------------------------------------------------------------------------


#pragma hdrstop

#include "LoadNewTrayExec.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "LoadNewTrayStep.h"
#include "TLoaderInterface.h"

extern TLoaderInterface* g_pLoader;

FSM_RESULT ExecLoadNewTray(int step)
{
    switch(step)
    {
        case LNT_INIT:
            return FSM_DONE;

        case LNT_CHECK_SENSOR:
            return g_pLoader->SensorTrayReady() ? FSM_DONE : FSM_RUNNING;

        case LNT_CYL_UP:
            return g_pLoader->CylinderUp() ? FSM_DONE : FSM_RUNNING;

        case LNT_SEPARATE:
            return g_pLoader->SeparateTray() ? FSM_DONE : FSM_RUNNING;

        case LNT_MOVE_TO_CCD:
            return g_pLoader->MoveToCCD() ? FSM_DONE : FSM_RUNNING;

        case LNT_READ_ID:
            return g_pLoader->ReadTrayID() ? FSM_DONE : FSM_RUNNING;

        case LNT_VERIFY:
            return g_pLoader->VerifyTray() ? FSM_DONE : FSM_ERROR;

        case LNT_DONE:
            return FSM_DONE;
    }

    return FSM_ERROR;
}
//--------------------------------------------------------------------------- 
