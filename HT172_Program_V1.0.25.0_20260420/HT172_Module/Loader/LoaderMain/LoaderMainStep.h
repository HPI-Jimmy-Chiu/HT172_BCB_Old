//---------------------------------------------------------------------------

#ifndef LoaderMainStepH
#define LoaderMainStepH
//---------------------------------------------------------------------------
enum LOADER_MAIN_STEP
{
    LDR_IDLE = 0,
    LDR_CHECK,
    LDR_LOAD_NEW_TRAY,
    LDR_TO_WORK,
    LDR_TO_TRANSFER,
    LDR_CLEAN_OUT,
    LDR_END = -1
};
#endif
