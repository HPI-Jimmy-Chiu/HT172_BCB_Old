//---------------------------------------------------------------------------

#ifndef FileInfoH
#define FileInfoH
//---------------------------------------------------------------------------
#include <StdCtrls.hpp>
#include <map>
#include <vector>
#include <deque>
//---------------------------------------------------------------------------
//#include <DBTables.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class ParameterInfo
{
public:     // User declarations
    AnsiString* asParaValue;
    AnsiString asFileParaName;
    ParameterInfo(AnsiString &_asParaValue,AnsiString _asFileParaName);                 //Jimmychiu 20220830 add ParameterInfo Class
    ~ParameterInfo();
};
//---------------------------------------------------------------------------
class FileInfo                                                                          //JimmyChiu 20220122 add FileInfo class
{
private:    // User declarations
    AnsiString asFilePath;
    std::map<int,ParameterInfo*> mapGroup;
public:     // User declarations
    AnsiString GetFilePath(){return asFilePath;};
    FileInfo(AnsiString FilePath);                                                      //Jimmychiu 20220830 add FileInfo Class
    FileInfo(){}
    ~FileInfo();
    void Add(AnsiString &_asParaValue, AnsiString _asFileParaName)                      //Jimmychiu 20220830 解碼並讀取資料
    {
        mapGroup[GetSize()]=new ParameterInfo(_asParaValue,_asFileParaName);
    }
    int GetSize(){return mapGroup.size();}
    bool ReadFile(AnsiString sSeparatedValues);
    bool ReadFile(AnsiString sSeparatedValues,int idecode);                             //Jimmychiu 20220830 解碼並讀取資料
    bool WriteFile(AnsiString sSeparatedValues);                                        //Jimmychiu 20220830 解碼並讀取資料
    bool IsFilePathExist(AnsiString sPath);                                                             //Jimmychiu 20220830 解碼並讀取資料
    AnsiString DecodeASCII(char *cStream,int iLen,int iTransfor);                       //Jimmychiu 20220830 解碼並讀取資料
    AnsiString DecodeASCII(AnsiString asFileName,int iTransfor);                        //Jimmychiu 20220830 解碼並讀取資料
    void DecodeReadMap2Parameter(AnsiString asLineValue,AnsiString sSeparatedValues);   //Jimmychiu 20220830 解碼並讀取資料
    void SaveAsTxtFile(AnsiString asContent,AnsiString asNewFileWithPath);                      //Jimmychiu 20230223 帳密權限加密，提升資訊安全
    void GetNameAndExtension(AnsiString asFileName,AnsiString &asFile,AnsiString &asExtension); //Jimmychiu 20230223 帳密權限加密，提升資訊安全
    std::vector<AnsiString> GetAllFileNamesInFolder(AnsiString folderPath,int &iErr);   //Jimmychiu 20240224 : Get all file name in the folder
    std::vector<AnsiString> GetAllFolderNamesInFolder(AnsiString folderPath,int &iErr); //Jimmychiu 20250715 : Get all folder name in the folder
    bool DeleteFolderContents(AnsiString folderPath,int &iErr);                         //Jimmychiu 20240224 : Delete Folder Contents
    void SplitPath(const AnsiString& fullPath, AnsiString& path, AnsiString& filename, AnsiString& extension);    //Jimmychiu 20240318 : add Split path function
    bool DirectoryExist(AnsiString folderPath);
    AnsiString RemoveAllTrailingBackslashes(const AnsiString& input);
    AnsiString PathCombin(const AnsiString& sPath, const AnsiString& sFile);
    void EnsureDirectoriesExist(AnsiString sPath);
    AnsiString GetNewLine(){return "\n";}
    void DeleteFolder(const AnsiString &folderPath);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------   //Jimmychiu 20240318 : add read write Paradox DB
class cDBTStringGrid;
class cDatabaseMin
{
private:
public:     // User declarations
    cDatabaseMin();
    ~cDatabaseMin();
    virtual void Clear(){}
    virtual bool LoadFile(AnsiString dbpath,AnsiString dbName);
    virtual void SaveFile();
    virtual void CloseFile(){}
};
//---------------------------------------------------------------------------
class cDBTStringGrid
{
private:
    TStringGrid *sg;
    int iRowHeight;
    void SetCell(int iRow,int iCol,const AnsiString str);
    AnsiString GetCell(int iRow,int iCol);
    void SetColTitle(int iCol,const AnsiString str);
    AnsiString GetColTitle(int iCol);
    void SetRowTitle(int iRow,AnsiString str);
    AnsiString GetRowTitle(int iRow);
public:     // User declarations
    cDBTStringGrid(TStringGrid* _sg);
    ~cDBTStringGrid();
    void Clear();
    void SetRowCol(int iRow,int iCol);
    void GetRowCol(int &iRow,int &iCol);
    int GetRowCount(){return sg->RowCount-1;}
    int GetColCount(){return sg->ColCount-1;}
    __property AnsiString ColTitle[int iCol]={read=GetColTitle, write=SetColTitle};
    __property AnsiString RowTitle[int iRow]={read=GetRowTitle, write=SetRowTitle};
    __property AnsiString Cells[int iCol][int iRow]={read=GetCell, write=SetCell};
    void AddRow();
    void AddCol();
    void CloneTo(TStringGrid* destination);
    void ClearRow(int iRow);
    void HideRow(bool bOn,int iIndex);
};
//---------------------------------------------------------------------------
class cMathTool
{
private:
public:     // User declarations
    cMathTool();
    ~cMathTool();
    void Clear();
    bool IsDoubleEqual(const double a,const double b,const double tolerance=1e-6);
    double Division(double Numerator,double Denominator);
    void EnsureMinMax(int &iMin, int &iMax);
    int Ceil(double Numerator,double Denominator);                               //無條件進位
};
//---------------------------------------------------------------------------
struct TTaskHistory
{
    int    Task;         // Task 值
    TDateTime Time;      // 變化時間
    TTaskHistory(int task)
    {
        Task = task;
        Time = Now();
    }
};
//---------------------------------------------------------------------------
const int MAX_TASK_HISTORY = 10;
struct TThreadState
{
    AnsiString ThreadName;                    // 例如 "LoaderModule", "UnloaderModule"...
    int        CurrentTask;                   // 當前 Task
    std::deque<TTaskHistory> History;         // 最近 10 筆
    TThreadState(AnsiString name = "Unknown")
    {
        ThreadName = name;
        CurrentTask = 0;
    }
    // 記錄 Task 變化
    void LogTask(int newTask);
    // 取得目前狀態的字串（給 ListBox 或 Memo 用）
    AnsiString GetCurrentStatus() const
    {
        AnsiString timeStr = FormatDateTime("yyyy/mm/dd hh:nn:ss.zzz", History.empty() ? Now() : History.front().Time);
        AnsiString taskStr = Format("Task = %d", ARRAYOFCONST((CurrentTask)));  // 先單獨格式化 Task
        return Format("%-23s | %-40s | %-10s",  // 用 | 分隔欄位，視覺更清楚
                  ARRAYOFCONST((timeStr, ThreadName, taskStr)));
//        return Format("%s -> %s -> Task = %d",
//                      ARRAYOFCONST(( FormatDateTime("yyyy/mm/dd hh:nn:ss.zzz", History.empty() ? Now() : History.front().Time),ThreadName, CurrentTask)));
    }
};
//---------------------------------------------------------------------------
class cStateRecord
{
private:
    AnsiString saveFolder;
    AnsiString sFileName;
    TCriticalSection* cs;

    int DoShellExecute(AnsiString cmd);
public:     // User declarations
    cStateRecord();
    ~cStateRecord();
    void Clear();
    void SetFolder(AnsiString sFolder);
    void SetZipFileName(AnsiString sfileName=AnsiString(""));
    AnsiString GetZipFileName(){return sFileName;}
    AnsiString GetTempZipFolder();
    AnsiString GetDefaultFolder(){return AnsiString("D:\\HT172_StateRecord\\");}
    void CopyFileToFolder(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFileName);
    void CopyFolderToFolder(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFolderName);
    bool CompressWithExternalTool(AnsiString folderToCompress, AnsiString zipFileName);
    AnsiString Get7ZipPath(){return AnsiString("C:\\Program Files\\7-Zip\\7z.exe");}
    void DoFinish();
    //record thread task
    std::vector<TThreadState> AllStates;
    void InitialThreadRecord();
    void LogTask(int moduleIndex, int newTask);
    enum eLogNumTask
    {
        eLNT_SortArm1Execute=0,
        eLNT_SortArm2Execute,
        eLNT_LoaderExecute,
        eLNT_ActionTrack1Execute,
        eLNT_ActionTrack2Execute,
        eLNT_ActionTrack3Execute,
        eLNT_ActionTrack4Execute,
        eLNT_ActionEmpty1Execute,
        eLNT_ActionEmpty1_TrackExecute,
        eLNT_ActionEmpty2Execute,
        eLNT_ActionStoreArmExecute,
        eLNT_ActionMag1Execute,
        eLNT_ActionMag2Execute,
        eLNT_ActionMag3Execute,
        eLNT_CCDLoaderExecute,
        eLNT_CCDEmpty1Execute,
        eLNT_CCDEmptyMGZExecute,
        eLNT_Total
    };//須同步InitialThreadRecord()
    void ExportCurrentStateToCSV();
    bool ExportTaskHistoryToCSV(const AnsiString& csvFilePath);
    TStringList* GetAllCurrentTasks(bool bWithTime = true);
    AnsiString GetTaskName(int moduleIdx, int taskID);              //AI(HT172-Maintainer) 20260420 : Phase1 step name lookup
    void CaptureCurrentTasks(AnsiString txtFullPath);                //AI(HT172-Maintainer) 20260420 : Phase1 dump 17 module tasks
    void CaptureMachineState(AnsiString iniFullPath);                //AI(HT172-Maintainer) 20260420 : Phase1 dump RunMode/flags/HasIC
    void TriggerSnapshot(AnsiString triggerReason);                  //AI(HT172-Maintainer) 20260420 : Phase1 one-shot zip snapshot
};
//---------------------------------------------------------------------------
#endif
