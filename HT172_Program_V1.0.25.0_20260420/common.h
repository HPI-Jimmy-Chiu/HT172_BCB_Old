//---------------------------------------------------------------------------

#ifndef commonH
#define commonH

#include <Classes.hpp>
#include <Controls.hpp>
#include <Buttons.hpp>
//#include <vector>
//---------------------------------------------------------------------------
AnsiString EncodeStr(AnsiString sourceStr); //jou 2013-01-04 Password Txt 加密
AnsiString DecodeStr(AnsiString sourceStr); //jou 2013-01-04 Password Txt 加密
//---------------------------------------------------------------------------
bool        __fastcall CheckIniData(AnsiString FileName, AnsiString Group, AnsiString Name);
//---------------------------------------------------------------------------
//Steven 20110711 : 把General.ini的存取改成獨立的
//---------------------------------------------------------------------------
void OpenGeneralIniFile();              //Steven 20141120 : Add Read/Write IniFile Speed
void CloseGeneralIniFile();             //Steven 20141120 : Add Read/Write IniFile Speed

AnsiString  __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value);
int         __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, int Value);
bool        __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, bool Value);
double      __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, double Value);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, bool bValue);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, int Value);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, double Value);
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value);

//---------------------------------------------------------------------------
//Steven 20090810 : 先檢查有沒有資料,沒有就會存入ini檔並回傳取得的資料
//---------------------------------------------------------------------------
void OpenIniFile(AnsiString FileName);              //Steven 20141120 : Add Read/Write IniFile Speed
void CloseIniFile();                                //Steven 20141120 : Add Read/Write IniFile Speed

TDateTime   __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value);
AnsiString  __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
int         __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
bool        __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value);
double      __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
unsigned long __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value);
//---------------------------------------------------------------------------
//讀取ini資料,若無附值則以傳入值回傳
//---------------------------------------------------------------------------
bool        __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue);
int         __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
double      __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
AnsiString  __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
AnsiString  __fastcall GetLastOpenFN();
//---------------------------------------------------------------------------
//檢查該群組是否存在
//---------------------------------------------------------------------------
bool        __fastcall CheckSectionExist(AnsiString FileName, AnsiString Group);

void __fastcall WriteLastDataFN(AnsiString SName);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue);  //Steven 20090731
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value);

//---------------------------------------------------------------------------
//Steven 20090803
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor, TColor FontColor);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int i);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, int left, int right, int top, int bottom);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int left, int right, int top, int bottom);

//---------------------------------------------------------------------------
//Steven 20090811 : 用Name2的值去取代Name1的值
//---------------------------------------------------------------------------
void __fastcall ReplaceIniData(AnsiString FileName, AnsiString Group, AnsiString Name1, AnsiString Name2);

void __fastcall AddSpace(AnsiString File);
bool __fastcall OnlyMakeFileDataInPut(int iKey);

extern bool __fastcall OnlyNumberInPut(int iKey);
extern bool __fastcall OnlyNumberAndDotInPut(int iKey);//only for Number and char '.'
extern void WriteDataToFile(AnsiString cFilePath, AnsiString cData, bool bOverWrite=false);
extern char* ReadDataFromFile(AnsiString cFilePath);                            //JimmyChiu 20230428 : add Read file data
extern AnsiString asProjectPath;
extern AnsiString DefaultPath;
extern AnsiString asSystemPath;
extern AnsiString asGeneralPath;
extern AnsiString asBarCodeLogPath;
extern AnsiString asDBPath;
extern AnsiString AuthPath;
extern AnsiString DataPath;
extern AnsiString OffsetPath;
extern AnsiString BmpPath;
extern AnsiString GrapicPath;
extern AnsiString DIOPath;
extern AnsiString DIOCFGPath;
extern AnsiString asErrNotePath;
extern AnsiString ConfigMemoPath;
//extern AnsiString RunPath;
extern AnsiString LastDataPath;
extern AnsiString SecsGemPath;
extern AnsiString asOLPLogPath;

extern SHFILEOPSTRUCT oFile;
extern int CheckPtrItem;
extern int iNormalDelayTimeSec;
extern AnsiString MainVersion;

typedef struct
{
    TCheckBox *ChkPtr;
    bool *Parameter;
} CHECT_PTR;

typedef struct
{
    TSpeedButton *SButton;
    bool Visible;
} SPEEDBUTTON_PTR;
extern AnsiString GetDoubleToStr(double dValue);
int MyForceDirectories(AnsiString Directory);
extern void DoRunExecutFile(AnsiString sExecutFullFileName, AnsiString sParameters);
#endif
