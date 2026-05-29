//---------------------------------------------------------------------------

#ifndef FSMRunnerH
#define FSMRunnerH

#include <windows.h>
//---------------------------------------------------------------------------
enum FSM_RESULT
{
    FSM_RUNNING = 0,
    FSM_DONE,
    FSM_ERROR,
    FSM_MANUAL_REQUIRED,
    FSM_ABORT
};

//---------------------------------------------------------------------------
// FSM Step Definition
struct FSM_STEP
{
    int stepId;
    int nextStep;

    int timeoutMs;     // 0 = no timeout
    int retryCount;    // 0 = no retry
    int abortStep;     // -1 = FSM_ABORT
};

//---------------------------------------------------------------------------
struct FSMContext
{
    int currentStep;
    int retryLeft;
    DWORD stepStartTick;
};

typedef FSM_RESULT (*PFN_EXEC_STEP)(int stepId, void* ctx);
//---------------------------------------------------------------------------
// FSM Runner
class FSMRunner
{
private:
    const FSM_STEP* m_steps;
    PFN_EXEC_STEP   m_execFn;
    void*           m_execCtx;
    FSMContext      m_ctx;
    bool            m_running;


private:
    void EnterStep(const FSM_STEP* step);

public:
    FSMRunner();

    void SetStepTable(const FSM_STEP* steps);
    void SetExecutor(PFN_EXEC_STEP fnExec, void* ctx);

    void Start(int startStep);
    FSM_RESULT Update();

    int  GetCurrentStep() const;
    void SetCurrentStep(int iStep){m_ctx.currentStep=iStep;}
    bool IsRunning() const;
    const FSM_STEP* FindStep(int stepId) const;
};
//---------------------------------------------------------------------------
#endif
