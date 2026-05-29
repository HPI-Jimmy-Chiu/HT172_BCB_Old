//---------------------------------------------------------------------------


#pragma hdrstop

#include "LoaderMainTable.h"

//------------------------------------------------------------------------------

#pragma package(smart_init)

#include "LoaderMainStep.h"
#include "FSMRunner.h"

FSM_STEP LoaderMainSteps[] =
{
    { LDR_IDLE,            LDR_CHECK,          0, 0, -1 },
    { LDR_CHECK,           LDR_IDLE,           0, 0, -1 },
    { LDR_LOAD_NEW_TRAY,   LDR_CHECK,          0, 0, -1 },
    { LDR_TO_WORK,         LDR_CHECK,          0, 0, -1 },
    { LDR_TO_TRANSFER,     LDR_CHECK,          0, 0, -1 },
    { LDR_CLEAN_OUT,       LDR_CHECK,          0, 0, -1 },
    { -1,                  -1,                 0, 0, -1 }
};
//------------------------------------------------------------------------------ 
