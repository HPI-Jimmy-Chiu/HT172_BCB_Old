#ifndef csystemH
#define csystemH
//---------------------------------------------------------------------------
void MainProc();
void AddNoNeedHomeSensorList();
void DoTemptureControl();

int  IsEMGPressed();
int  IsSafeDoorOpen();
int  IsIonFanAlarm();
bool IsSafeLock();
bool IsSystemPowerOff();
bool AllBreakFree();
void AllBreakLock();
void InitialAllModule();
bool CheckCleanOutFinish();
bool CheckOneCycleFinish();
void CCDLoadJobs();
bool HasICUnderMachine();
bool HasICUnderFrontMachine();
bool HasICUnderMachineForCleanOut();
bool CheckEmpty1TrayFeedFinish();
bool CheckAllTrayFeedFinish(bool reset=false);
void ChangeRunMode(RunModeEnum RunMode);
int GetCalculateUPH(TDateTime tEndTime);
extern bool HasAutoICInMachine();
extern void InitialAllTask();

extern void ScanAllMotorStatus();
extern void RecordSafeDoorStates();
#endif

