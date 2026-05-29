//---------------------------------------------------------------------------


#pragma hdrstop

#include "TrayToWorkTable.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#include "TrayToWorkStep.h"
#include "FSMRunner.h"

FSM_STEP TrayToWorkSteps[] =
{
    // stepId              nextStep              timeout   retry  abort
    { TTW_INIT,            TTW_RELEASE_STOP,     0,        0,     TTW_ERROR },
    { TTW_RELEASE_STOP,    TTW_MOVE_CW,          3000,     1,     TTW_ERROR },
    { TTW_MOVE_CW,         TTW_WAIT_IN_POS,      5000,     1,     TTW_ERROR },
    { TTW_WAIT_IN_POS,     TTW_FIX_TRAY,         5000,     0,     TTW_ERROR },
    { TTW_FIX_TRAY,        TTW_MOVE_TO_WORK,     3000,     1,     TTW_ERROR },
    { TTW_MOVE_TO_WORK,    TTW_DONE,             5000,     1,     TTW_ERROR },
    { TTW_DONE,            TTW_END,              0,        0,     -1 },
    { TTW_ERROR,           TTW_END,              0,        0,     -1 },       
    { -1,                  -1,                   0,        0,     -1 }
};
 