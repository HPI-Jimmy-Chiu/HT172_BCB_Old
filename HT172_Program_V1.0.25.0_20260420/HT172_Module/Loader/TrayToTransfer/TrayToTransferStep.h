//---------------------------------------------------------------------------

#ifndef TrayToTransferStepH
#define TrayToTransferStepH
//---------------------------------------------------------------------------
enum TRAY_TO_TRANSFER_STEP
{
    TTT_INIT = 0,
    TTT_SHIFT_STAGE,
    TTT_MOVE_X,
    TTT_RELEASE_CYL,
    TTT_MOVE_TO_TRANSFER,
    TTT_COPY_DATA,
    TTT_DONE,
    TTT_ERROR,
    TTT_END = -1
};
#endif
 