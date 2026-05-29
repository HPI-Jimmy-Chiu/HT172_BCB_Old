//---------------------------------------------------------------------------


#pragma hdrstop

#include "LoadNewTrayTable.h"
#include "LoadNewTrayStep.h"
#include "FSMRunner.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
FSM_STEP LoadNewTraySteps[] =
{
    { LNT_INIT,            LNT_CHECK_SENSOR,   0, 0, -1 },
    { LNT_CHECK_SENSOR,    LNT_CYL_UP,         3000, 0, -1 },
    { LNT_CYL_UP,          LNT_SEPARATE,       3000, 0, -1 },
    { LNT_SEPARATE,        LNT_MOVE_TO_CCD,    3000, 0, -1 },
    { LNT_MOVE_TO_CCD,     LNT_READ_ID,        3000, 0, -1 },
    { LNT_READ_ID,         LNT_VERIFY,         5000, 0, -1 },
    { LNT_VERIFY,          LNT_DONE,           0, 0, -1 },
    { LNT_DONE,            LNT_END,            0, 0, -1 },
    { -1,                  -1,                 0, 0, -1 }
};
 