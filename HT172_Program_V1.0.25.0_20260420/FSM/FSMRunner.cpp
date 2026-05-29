//---------------------------------------------------------------------------


#pragma hdrstop

#include "FSMRunner.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
FSMRunner::FSMRunner()
{
    m_steps   = NULL;
    m_execFn  = NULL;
    m_execCtx = NULL;
    m_running = false;

    m_ctx.currentStep   = -1;
    m_ctx.retryLeft     = 0;
    m_ctx.stepStartTick = 0;
}
//---------------------------------------------------------------------------
void FSMRunner::SetStepTable(const FSM_STEP* steps)
{
    m_steps = steps;
}
//---------------------------------------------------------------------------
void FSMRunner::SetExecutor(PFN_EXEC_STEP fnExec, void* ctx)
{
    m_execFn = fnExec;
    m_execCtx  = ctx;
}
//---------------------------------------------------------------------------
void FSMRunner::Start(int startStep)
{
    m_ctx.currentStep   = startStep;
    m_ctx.retryLeft     = 0;
    m_ctx.stepStartTick = GetTickCount();
    m_running           = true;
}
//---------------------------------------------------------------------------
bool FSMRunner::IsRunning() const
{
    return m_running;
}
//---------------------------------------------------------------------------
int FSMRunner::GetCurrentStep() const
{
    return m_ctx.currentStep;
}
//---------------------------------------------------------------------------
const FSM_STEP* FSMRunner::FindStep(int stepId) const
{
    if (!m_steps)
        return NULL;

    for (int i = 0; ; i++)
    {
        if (m_steps[i].stepId == stepId)
            return &m_steps[i];

        // 慣例：stepId < 0 作為表結束
        if (m_steps[i].stepId < 0)
            break;
    }
    return NULL;
}
//---------------------------------------------------------------------------
void FSMRunner::EnterStep(const FSM_STEP* step)
{
    m_ctx.retryLeft     = step->retryCount;
    m_ctx.stepStartTick = GetTickCount();
}
//---------------------------------------------------------------------------
FSM_RESULT FSMRunner::Update()
{
    if (!m_running || !m_steps || !m_execFn)
        return FSM_ABORT;

    const FSM_STEP* step = FindStep(m_ctx.currentStep);
    if (!step)
    {
        m_running = false;
        return FSM_ABORT;
    }

    FSM_RESULT r = m_execFn(step->stepId, m_execCtx);

    // ===== Step still running =====
    if (r == FSM_RUNNING)
    {
        if (step->timeoutMs > 0)
        {
            DWORD elapsed = GetTickCount() - m_ctx.stepStartTick;
            if (elapsed > (DWORD)step->timeoutMs)
            {
                if (m_ctx.retryLeft > 0)
                {
                    m_ctx.retryLeft--;
                    m_ctx.stepStartTick = GetTickCount();
                    return FSM_RUNNING;
                }

                // timeout + no retry
                if (step->abortStep >= 0)
                {
                    m_ctx.currentStep = step->abortStep;
                    EnterStep(FindStep(m_ctx.currentStep));
                    return FSM_RUNNING;
                }

                m_running = false;
                return FSM_ABORT;
            }
        }
        return FSM_RUNNING;
    }

    // ===== Step done =====
    if (r == FSM_DONE)
    {
        if (step->nextStep < 0)
        {
            m_running = false;
            return FSM_DONE;
        }

        m_ctx.currentStep = step->nextStep;
        EnterStep(FindStep(m_ctx.currentStep));
        return FSM_RUNNING;
    }

    // ===== Manual intervention =====
    if (r == FSM_MANUAL_REQUIRED)
    {
        m_running = false;
        return FSM_MANUAL_REQUIRED;
    }

    // ===== Error / Abort =====
    if (step->abortStep >= 0)
    {
        m_ctx.currentStep = step->abortStep;
        EnterStep(FindStep(m_ctx.currentStep));
        return FSM_RUNNING;
    }

    m_running = false;
    return FSM_ABORT;
}
//---------------------------------------------------------------------------
