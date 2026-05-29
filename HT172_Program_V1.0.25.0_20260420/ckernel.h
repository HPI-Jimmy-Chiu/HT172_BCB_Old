//---------------------------------------------------------------------------
#ifndef ckernelH
#define ckernelH
//---------------------------------------------------------------------------

bool CheckReceiveTrayBuildFull(int pos,int flag);
bool CheckEmptyTrayBuildIsNull(int pos,int flag);
bool CheckHasOtherEmptyTray();
bool SendTrayBufferIsFull();
void SetWarn(int code);
void ClearWarn(int code);
bool SendTrayBufferIsFull();
void ClearAllWarn();
void CheckManualTrayChange();
void ProcessRunStatus(bool bProgramStart);
int ScanPannelKey();
bool ScanRetryButton();
bool ScanSkipButton();
bool ScanAlarmButton();
void ProcessAlarm();
void StopAllDestroy();
void DoSystemMessage();
void count_seconds(double *seconds);
#endif
