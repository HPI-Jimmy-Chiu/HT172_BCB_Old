#ifndef cpublicH
#define cpublicH

#include <stdio.h>
//---------------------------------------------------------------------------
void AddCharAtTail(char *S,char c,int len);
void AddSpace(char *S,int len);
void CutSpace(char *S);
void CutSpaceAtTail(char *S);
void FilterErrorAscii(char * str);
int IsAllSpace( char *str);
char DataToLED2(char  data);
void ConvertSDKString(char *Source,char *Taget);
bool ReadLineFromFile(FILE *Ptr,char *s,int Max);
void CutSpaceAtHead(char *S);
//char *GetDirectory();
bool SplitUpperStrBySpace(char *str,char *dest,int Max);
bool SplitStrBySpaceOnly(char *str,char *dest,int Max);
int HexStrToInt(char *str );
bool IsMCIDeviceExit();
extern bool fMCIDeviceExit;
void SystemShutDown(void);
double GetScale(int s1,int e1,int s2,int e2);
int GetReference(int s1,int e1,int s2,int e2,int p);
char *ConvertSecondToTime(long s);
TColor GetDeviceColor(int Type);
//void SetCompomentIO(TWinControl *PCtrl);
void ScanLed(TWinControl *PCtrl);
bool ShellExecuteProgram( char *str);
void ConvertGearValue(int *Ref, double GearRatio);
//void SetCompomentHint(TWinControl *PCtrl);
char *GetFloatFormatString(double v,int P1,int P2);
char *GetIntFormatString(int v,int P1);
void ReplaceCharExceptAlpha(char *str);
char *ConvertToMMType(int i);
int Get0_01MMType(char *str);
int Get0_01MMType(double f);                                                     // ben add 20110802 //

char *ConvertTouMType(int i);
int Get0_001MMType(char *str);
int Get0_01MMTypeLimit(char *str,int Limit);
void GetTimeInfo();
void GetYesterdayInfo();
bool SplitStrByDotSpaceOnly(char *str,char *dest,int Max);
void ConvertTimeFormat(char *str,int &H, int &M,int &S);
void Swap(AnsiString &S1,AnsiString &S2);
long GetTotalSecond(AnsiString S);
long GetTotalSecond(AnsiString S);
int GetScreenPos(int NowPos,int ScreenTop,int ScreenBottom,int RealTop,int RealBottom);
AnsiString SetPassTimeString(DWORD &StartTimeCount);
bool CheckDataIsNum(AnsiString asS, bool bHasDot);
AnsiString GetSocketErrorMsg(int ErrorCode);
AnsiString GetErrorMsg(TObject *Sender, int ErrorCode);
void LogClientSocketExceptionError(TObject *Sender, AnsiString Msg);            //Steven 20231113 : 紀錄網路連線例外
AnsiString GetDateInfoByString(AnsiString asSign="");                           //ChungHung 20151125 modify for KYEC //ChungHung 20150902 add
AnsiString GetOnlyTimeInfoByString(AnsiString asSign="");                       //ChungHung 20151125 modify for KYEC  //ChungHung 20150902 add
void GetArmRowCol(int &iRow, int &iCol);
void GetTrayRowCol(int &iRow, int &iCol);
//---------------------------------------------------------------------------
bool DeleteDirectory(AnsiString sDir);

void ShowRecordMessage(char *S)        ;
void ShowRecordMessage(AnsiString S)   ;
class HonPrecFunctionCollection
{
    public:
    bool CheckFileExist(char *cFName);
};
extern HonPrecFunctionCollection HFC;
unsigned char A_Create_LCR(unsigned char *Sptr,unsigned char length);
bool GetCOMPortStatus(int i);
bool GetCOMPortStatus(AnsiString Com);    //Steven 20120217 : Com Port改成可定義

template<class T>
T CheckRange(const T Value, const T Maximum, const T Minimum)
{
    if(Maximum<Minimum) //如果大小相反
    {
        if(Value>Minimum)
            return Minimum;
        else if(Value<Maximum)
            return Maximum;
        else
            return Value;
    }
    else
    {
        if(Value>Maximum)
            return Maximum;
        else if(Value<Minimum)
            return Minimum;
        else
            return Value;
    }
};
//---------------------------------------------------------------------------
bool __fastcall OnlyNumberInPut(int iKey);//only for Number
bool __fastcall OnlyNumberAndDotInPut(int iKey);//only for Number and char '.'
bool __fastcall OnlyMakeFileDataInPut(int iKey);

//---------------------------------------------------------------------------
//Steven 20090803
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor, TColor FontColor);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int i);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, int left, int right, int top, int bottom);
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int left, int right, int top, int bottom);

bool __fastcall CheckSectionExist(AnsiString FileName, AnsiString Group);

//---------------------------------------------------------------------------
//Steven 20110711 : 把General.ini的存取改成獨立的
//---------------------------------------------------------------------------
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
TDateTime   __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value);
AnsiString  __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
int         __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value);
bool        __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value);
double      __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value);
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
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value);
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value);

//---------------------------------------------------------------------------
void UDPErrorLog(AnsiString aTitle, AnsiString Command);                        //kevin 20211020 UDP error log
bool RespondASECom(AnsiString S1);                                              //kevin 20150415
extern DWORD MyTickCount();
extern DWORD MySleepEx(DWORD dwMilliseconds, bool bAlertable);
extern void MySleep(DWORD dwMilliseconds);
AnsiString GetNowTime();
//---------------------------------------------------------------------------
class uPoint2D
{   
public:     
    uPoint2D()
    {
        Clear();
    }
    uPoint2D(int x, int y)
    {
        Clear();
        X=x;
        Y=y;
    }
    ~uPoint2D(){}
    int X;
    int Y;
    void Clear()
    {
        X=0;
        Y=0;
    }
    uPoint2D& operator=(const uPoint2D& other)
    {
        if(this!=&other)
        {
            X=other.X;
            Y=other.Y;
        }
        return *this;
    }
};
//---------------------------------------------------------------------------
class VerInfo                                                                   //Sam 20230328 : 自動更新增加版本檢查
{
    public:
        VerInfo();
        __property AnsiString   FileName            ={read=m_strFileName,    write=m_SetFileName};   //檔案名稱
        __property AnsiString   FixedFileVersion    ={read=m_strGetFixedFileVersion};                //檔案版本
        __property WORD         LangID              ={read=m_wLangID};                               //語系
        __property WORD         CharsetID           ={read=m_wCharsetID};                            //字元表
        __property AnsiString   ProductName         ={read=m_strProductName};                        //產品名稱
        __property AnsiString   ProductVersion      ={read=m_strProductVersion};                     //產品版本
        __property AnsiString   OriginalFilename    ={read=m_strOriginalFilename };                  //原始檔名
        __property AnsiString   FileDescription     ={read=m_strFileDescription};                    //檔案說明
        __property AnsiString   CompanyName         ={read=m_strCompanyName};                        //公司名稱
        __property AnsiString   LegalCopyright      ={read=m_strLegalCopyright};                     //著作權
        __property AnsiString   LegalTrademarks     ={read=m_strLegalTrademarks};                    //合法商標
        __property AnsiString   InternalName        ={read=m_strInternalName};                       //內部名稱
        __property AnsiString   Comments            ={read=m_strComments};                           //說明
        void GetAppVersion(AnsiString sAppExeName,WORD& major,WORD& minor,WORD& build,WORD& revision);
        AnsiString GetSVNRev();
    private:
        AnsiString m_strFileName;
        void m_SetFileName(AnsiString);
        VS_FIXEDFILEINFO m_stFileInfo;
        AnsiString m_strGetFixedFileVersion(void);
        AnsiString m_strGetFixedProductVersion(void);
        WORD m_wLangID;
        WORD m_wCharsetID;
        AnsiString m_strProductName;
        AnsiString m_strProductVersion;
        AnsiString m_strOriginalFilename;
        AnsiString m_strFileDescription;
        AnsiString m_strFileVersion;
        AnsiString m_strCompanyName;
        AnsiString m_strLegalCopyright;
        AnsiString m_strLegalTrademarks;
        AnsiString m_strInternalName;
        AnsiString m_strComments;
        DWORD m_dwLastError;
        DWORD m_dwGetLastError(void);
        void m_GetVerInfo(void);
        void m_ClearData(void);
};
//------------------------------------------------------------------------------
typedef struct _VersionKeyInfo                                                  //Sam 20230328 : 自動更新增加版本檢查
{
    TCHAR const *szKey;
    TCHAR *szValue;
}VKINFO, *LPVKINFO;
//------------------------------------------------------------------------------
class TBasicAllAction
{
    private:
    public:
        TBasicAllAction();
        ~TBasicAllAction();
        int iDoAllActionTask;
        int iStatus;
        void SetActionStep(int iStep);
};
//------------------------------------------------------------------------------
class uMotorRationCompute
{
    private:
        double dpulse_dis_ratio;
        struct MotorConfig
        {
            double pulses_per_mm; // 每毫米脈衝數
            double reference_pulses; // 參考脈衝數（例如 -78953）
            double reference_distance; // 參考位移（例如 0 mm）
        };
    public:
        uMotorRationCompute()
        {
            dpulse_dis_ratio=0.0;
        }
        ~uMotorRationCompute(){}
        double pulseDividerDistance_mm(double PulsesS,double PulsesE,double Distance_mm_S,double Distance_mm_E);
        int GetTargetPulseByDistance(double PulsesS,double PulsesE,double DistanceS,double DistanceE,double dTarDistance);

        double GetMotorRatioFromSPosToEPos(double PulsesS,double PulsesE,double RealDistanceS,double RealDistanceE,double oldMotorRatio);
        void initializeMotorConfig(double pulses1, double distance1, double pulses2, double distance2, MotorConfig& motc);
        double pulsesToDistance(MotorConfig config, double pulses);
        double distanceToPulses(MotorConfig config, double distance);
};
//------------------------------------------------------------------------------
void SetEditValue0(TWinControl *PCtrl);
#endif
