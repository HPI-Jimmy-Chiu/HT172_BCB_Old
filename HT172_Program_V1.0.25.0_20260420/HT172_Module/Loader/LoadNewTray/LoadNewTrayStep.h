//---------------------------------------------------------------------------

#ifndef LoadNewTrayStepH
#define LoadNewTrayStepH
//---------------------------------------------------------------------------
enum LOAD_NEW_TRAY_STEP
{
    LNT_INIT = 0,
    LNT_CHECK_SENSOR,
    LNT_CYL_UP,
    LNT_SEPARATE,
    LNT_MOVE_TO_CCD,
    LNT_READ_ID,
    LNT_VERIFY,
    LNT_DONE,
    LNT_END = -1
};
#endif
 