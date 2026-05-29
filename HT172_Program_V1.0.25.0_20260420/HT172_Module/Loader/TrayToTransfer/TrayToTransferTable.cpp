//---------------------------------------------------------------------------


#pragma hdrstop

#include "TrayToTransferTable.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#include "TrayToTransferStep.h"
#include "FSMRunner.h"

FSM_STEP TrayToTransferSteps[] =
{
    // stepId               nextStep               timeout   retry  abort
    { TTT_INIT,             TTT_SHIFT_STAGE,       0,        0,     TTT_ERROR },

    { TTT_SHIFT_STAGE,      TTT_MOVE_X,            3000,     1,     TTT_ERROR },

    { TTT_MOVE_X,           TTT_RELEASE_CYL,       5000,     1,     TTT_ERROR },

    { TTT_RELEASE_CYL,      TTT_MOVE_TO_TRANSFER,  3000,     1,     TTT_ERROR },

    { TTT_MOVE_TO_TRANSFER, TTT_COPY_DATA,         5000,     1,     TTT_ERROR },

    { TTT_COPY_DATA,        TTT_DONE,              1000,     0,     TTT_ERROR },

    { TTT_DONE,             TTT_END,               0,        0,     -1 },

    { TTT_ERROR,            TTT_END,               0,        0,     -1 },

    { -1,                   -1,                    0,        0,     -1 }
};
 