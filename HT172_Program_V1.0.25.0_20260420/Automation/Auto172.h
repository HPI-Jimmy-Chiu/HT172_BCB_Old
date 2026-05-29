//---------------------------------------------------------------------------

#ifndef Auto172H
#define Auto172H
//---------------------------------------------------------------------------
bool CheckSystemStart();
AnsiString GetHandlerType();
int ShowOLPState(int iState);
bool CheckInitialOK();
AnsiString GetMainStatus();
AnsiString GetSoftwareVersion();
AnsiString GetSetUpFileName();
AnsiString GetJamCount();
AnsiString GetUPH();
AnsiString GetBinSetMode();

int GetCategory(AnsiString *Data);
int GetIP(AnsiString *Data);
int GetPort(AnsiString *Data);
int GetCusCode(AnsiString *Data);
int GetTesterInfo(AnsiString *Data);
int SetTesterInfo(AnsiString *Data);
int GetLotTotal(AnsiString *Data);
int GetLotTotalQuantity(AnsiString *Data);
int GetLotEndTime(AnsiString *Data);
void DoOnLineReply(AnsiString *Data);
void DoClearReportRequest();
int SetCategory(AnsiString *Data);
int SetSetUpFileName(AnsiString *Data);
AnsiString GetLotStartProgram();
AnsiString GetLotEndProgram();
int SetLotStartProgram(AnsiString *Data);
int SetLotEndProgram(AnsiString *Data);
int SetBinSetMode(AnsiString *Data);
int GetSortBin(AnsiString sLotID, AnsiString sTrayID, AnsiString sTrayX, AnsiString sTrayY);
int CheckCategoryName(AnsiString sName);
int LoadTrayMap(AnsiString *Data);
extern int CallLotProgram(AnsiString cmd);
#endif
