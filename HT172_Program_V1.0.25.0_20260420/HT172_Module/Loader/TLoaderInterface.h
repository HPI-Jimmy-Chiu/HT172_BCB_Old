//---------------------------------------------------------------------------

#ifndef TLoaderInterfaceH
#define TLoaderInterfaceH
//---------------------------------------------------------------------------
#include "FSMRunner.h"
//---------------------------------------------------------------------------
class TLoaderInterface
{
private:
    FSMRunner m_mainFSM;
    FSMRunner m_loadNewTrayFSM;
    FSMRunner m_toWorkFSM;
    FSMRunner m_toTransferFSM;

public:
    void Init();
    void Start();
    void Update();

    // 動作函式（簡化）
    bool SensorTrayReady();
    bool CylinderUp();
    bool SeparateTray();
    bool MoveToCCD();
    bool ReadTrayID();
    bool VerifyTray();

    bool ReleaseStopper();
    bool MotorCW();
    bool TrayInPosition();
    bool FixTray();

    bool MoveX();
    bool ReleaseCylinder();
    bool MoveToTransfer();
};
#endif
