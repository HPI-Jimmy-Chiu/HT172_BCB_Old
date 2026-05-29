//---------------------------------------------------------------------------

#ifndef TrayToWorkStepH
#define TrayToWorkStepH
//---------------------------------------------------------------------------
enum TRAY_TO_WORK_STEP
{
    TTW_INIT = 0,
    TTW_RELEASE_STOP,
    TTW_MOVE_CW,
    TTW_WAIT_IN_POS,
    TTW_FIX_TRAY,
    TTW_MOVE_TO_WORK,
    TTW_DONE,
    TTW_ERROR,
    TTW_END = -1
};
#endif
 