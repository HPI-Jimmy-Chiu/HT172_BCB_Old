//------------------------------------------------------------------------------
#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "FileInfo.h"
#include <fstream>
#include <math.h>
#include "uTimeTool.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;
//---------------------------------------------------------------------------
ParameterInfo::ParameterInfo(AnsiString &_asParaValue, AnsiString _asFileName)  //Jimmychiu 20220830 add ParameterInfo
{
    asParaValue=&_asParaValue;
    asFileParaName=_asFileName;
}
//---------------------------------------------------------------------------
ParameterInfo::~ParameterInfo()                                                 //Jimmychiu 20220830 add ParameterInfo Class
{
}
//---------------------------------------------------------------------------
FileInfo::FileInfo(AnsiString FilePath)                                         //Jimmychiu 20220830 add FileInfo Class
{
    asFilePath=FilePath;
}
//---------------------------------------------------------------------------
FileInfo::~FileInfo()                                                           //Jimmychiu 20220830 add FileInfo Class
{
    for(int i=0; i<GetSize(); i++)
    {
        delete mapGroup[i];
    }
    mapGroup.clear();
    Sleep(1);                                                                   //避免非同步互搶資源
}
//---------------------------------------------------------------------------
bool FileInfo::ReadFile(AnsiString sSeparatedValues)
{
    return ReadFile(sSeparatedValues,0);                                        //Jimmychiu 20220830 解碼並讀取資料
}
//---------------------------------------------------------------------------
bool FileInfo::ReadFile(AnsiString sSeparatedValues, int idecode)               //Jimmychiu 20220830 解碼並讀取資料
{
    TStringList *slSourceFile=new TStringList();
    AnsiString asGetString="";
    if(IsFilePathExist(asFilePath)==false)
        return false;

    if(idecode!=0)
    {
        TStringStream *ts;
        ts=new TStringStream(DecodeASCII(asFilePath,idecode));
        slSourceFile->LoadFromStream(ts);
        delete ts;
    }
    else
    {
        slSourceFile->LoadFromFile(asFilePath);
    }

    for(int i=0; i<slSourceFile->Count; i++)
        DecodeReadMap2Parameter(slSourceFile->Strings[i], sSeparatedValues);
    delete slSourceFile;
    return true;

}
//---------------------------------------------------------------------------
bool FileInfo::WriteFile(AnsiString sSeparatedValues)                           //Jimmychiu 20220830 解碼並讀取資料
{
    TStringList *slSourceFile=new TStringList();
    AnsiString asSetString="";
    if(IsFilePathExist(asFilePath)==false)
        return false;

    slSourceFile->LoadFromFile(asFilePath);
    for(int i=0; i<GetSize(); i++)
    {
        ParameterInfo* _p=mapGroup.find(i)->second;
        asSetString=_p->asFileParaName+sSeparatedValues+*(_p->asParaValue);
        slSourceFile->Add(asSetString);
    }
    delete slSourceFile;
    return true;
}
//---------------------------------------------------------------------------
bool FileInfo::IsFilePathExist(AnsiString sPath)
{
    if(sPath=="" || FileExists(sPath)==false)
        return false;
    return true;
}
//---------------------------------------------------------------------------
AnsiString FileInfo::DecodeASCII(char *cStream, int iLen, int iTransfor)        //Jimmychiu 20220830 解碼並讀取資料
{
    for(int i=0; i<iLen; i++)
        cStream[i]+=iTransfor;
    return AnsiString(cStream);
}
//---------------------------------------------------------------------------
AnsiString FileInfo::DecodeASCII(AnsiString asFileName, int iTransfor)
{
    fstream fp;
    AnsiString asBack="";
    char *a;
    fp.open(asFileName.c_str(), ios::in);                                       //開啟檔案
    if(fp.is_open())
    {
        fp.seekg(0, ios::end);
        int n=fp.tellg();
        a=new char[n];
        fp.seekg(0,ios::beg);
        fp.read(a,n);
        asBack=DecodeASCII(a,n,iTransfor);
        fp.close();
    }
    delete[] a;
    return asBack;
}
//---------------------------------------------------------------------------
void FileInfo::DecodeReadMap2Parameter(AnsiString asLineValue, AnsiString sSeparatedValues)
{
    AnsiString asKey="";
    int iStart=-1;
    for(int j=0;j<GetSize();j++)
    {
        ParameterInfo* _p=mapGroup.find(j)->second;
        asKey=_p->asFileParaName;
        if(asKey=="")continue;
        if(asLineValue.Pos(asKey+sSeparatedValues)==1)
        {
            iStart=asKey.Length()+sSeparatedValues.Length()+1;
            *(_p->asParaValue)=asLineValue.SubString(iStart,asLineValue.Length()-iStart+1);
            break;
        }
    }
}
//---------------------------------------------------------------------------
void FileInfo::SaveAsTxtFile(AnsiString asContent, AnsiString asNewFileWithPath)//Jimmychiu 20230223 帳密權限加密，提升資訊安全
{
    ofstream outfile;
    outfile.open(asNewFileWithPath.c_str());
    outfile<<asContent.c_str();
    outfile.close();
}
//---------------------------------------------------------------------------
void FileInfo::GetNameAndExtension(AnsiString asFileName, AnsiString &asFile, AnsiString &asExtension) //Jimmychiu 20230223 帳密權限加密，提升資訊安全
{
    int dot_pos = asFileName.LastDelimiter('.');
    asFile=asExtension="";
    if(dot_pos>0)
    {
        asFile=asFileName.SubString(1,dot_pos-1);
        asExtension=asFileName.SubString(dot_pos+1, asFileName.Length()-dot_pos+1);
    }
}
//---------------------------------------------------------------------------   //Jimmychiu 20240224 : Get all file name in the folder
std::vector<AnsiString> FileInfo::GetAllFileNamesInFolder(AnsiString folderPath, int &iErr)
{
    //iErr=>  0=normal 1=No Folder
    iErr=0;
    std::vector<AnsiString> fileNames;
    TSearchRec searchRec;
    if(folderPath.SubString(folderPath.Length(),1) != "\\")
    {
        folderPath+="\\";
    }
    int findResult=FindFirst(folderPath+"*.*",faAnyFile,searchRec);
    if(findResult==0)
    {
        do
        {
            if((searchRec.Attr & faDirectory)==0)//remove folder name
            {
                fileNames.push_back(searchRec.Name);
            }
        }while(FindNext(searchRec)==0);
        FindClose(searchRec);
    }
    else
    {
        iErr=1;
    }
    return fileNames; 
}
//---------------------------------------------------------------------------
std::vector<AnsiString> FileInfo::GetAllFolderNamesInFolder(AnsiString folderPath, int &iErr)
{
    //iErr=>  0=normal 1=No Folder
    iErr=0;
    std::vector<AnsiString> folderNames;
    TSearchRec searchRec;
    if(folderPath.SubString(folderPath.Length(),1) != "\\")
    {
        folderPath+="\\";
    }
    int findResult=FindFirst(folderPath+"*.*",faDirectory,searchRec);
    if(findResult==0)
    {
        do
        {
            if ((searchRec.Attr & faDirectory) && 
                searchRec.Name != "." && 
                searchRec.Name != "..")
            {
                folderNames.push_back(searchRec.Name);
            }
        }while(FindNext(searchRec)==0);
        FindClose(searchRec);
    }
    else
    {
        iErr=1;
    }
    return folderNames; 
}
//---------------------------------------------------------------------------
bool FileInfo::DeleteFolderContents(AnsiString folderPath,int &iErr)
{
    if(folderPath.SubString(folderPath.Length(),1)!="\\")
    {
        folderPath+="\\";
    }
    iErr=0; //iErr=>  0=normal 1=No Folder 2=delete fail
    std::vector<AnsiString> fileNames =FileInfo().GetAllFileNamesInFolder(folderPath,iErr);   //"D:\\PM RECORD\\";
    if(iErr==1)
    {
        return false;
    }
    else
    {
        AnsiString sFileName="";
        for(unsigned int i=0;i<fileNames.size();i++)
        {
            try
            {
                if(DeleteFile(folderPath+fileNames[i])==false)
                {
                    iErr=2;
                }
            }
            catch (Exception& e)
            {
                iErr=2;
            }
        }
    }
    return true; 
}
//---------------------------------------------------------------------------
void FileInfo::SplitPath(const AnsiString& fullPath, AnsiString& path, AnsiString& filename, AnsiString& extension)
{
    int lastSlashIndex=fullPath.LastDelimiter("\\");
    if(lastSlashIndex!=0)
    {
        path=fullPath.SubString(1,lastSlashIndex-1);
        AnsiString filenameWithExtension=fullPath.SubString(lastSlashIndex+1,fullPath.Length()-lastSlashIndex);
        int lastDotIndex=filenameWithExtension.LastDelimiter(".");
        if(lastDotIndex!=0)
        {
            filename=filenameWithExtension.SubString(1,lastDotIndex-1);
            extension=filenameWithExtension.SubString(lastDotIndex+1,filenameWithExtension.Length()-lastDotIndex);
        }
        else
        {
            filename=filenameWithExtension;
            extension="";
        }
    }
    else
    {
        path="";
        filename=fullPath;
        extension="";
    }
}
//---------------------------------------------------------------------------
bool FileInfo::DirectoryExist(AnsiString folderPath)
{
    folderPath=RemoveAllTrailingBackslashes(folderPath);
    return DirectoryExists(folderPath);
}
//---------------------------------------------------------------------------
AnsiString FileInfo::RemoveAllTrailingBackslashes(const AnsiString& input)
{
    int ilen=input.Length();
    while(ilen>0 && input[ilen]=='\\')
    {
        ilen--;
    }
    return input.SubString(1, ilen);
}
//---------------------------------------------------------------------------
AnsiString FileInfo::PathCombin(const AnsiString& sPath, const AnsiString& sFile)
{
    AnsiString combinedPath=sPath;
    if(!combinedPath.IsEmpty())
    {
        if(combinedPath[combinedPath.Length()]=='/' || combinedPath.Pos("/"))   // FTP path format
        {
            if(combinedPath[combinedPath.Length()]!='/')
            {
                combinedPath+="/";
            }
        }
        else                                                                    // Local path
        {
            if(combinedPath[combinedPath.Length()] != '\\')
            {
                combinedPath+="\\";
            }
        }
    }
    combinedPath+=sFile;
    return combinedPath;
}
//---------------------------------------------------------------------------
void FileInfo::EnsureDirectoriesExist(AnsiString sPath)
{
    if(sPath[sPath.Length()]=='\\')
    {
        sPath=sPath.SubString(1,sPath.Length()-1);
    }
    int pos=1;
    while(pos<=sPath.Length())
    {
        int nextPos=sPath.SubString(pos, sPath.Length()-pos+1).Pos("\\");
        if(nextPos>0)
        {
            nextPos=pos+nextPos-1;
        }
        else
        {
            nextPos=sPath.Length()+1;
        }
        AnsiString currentPath=sPath.SubString(1, nextPos-1);
        if(GetFileAttributes(currentPath.c_str())==INVALID_FILE_ATTRIBUTES)
        {
            if(!CreateDirectory(currentPath.c_str(), NULL))
            {
                return;
            }
        }
        pos=nextPos+1;
    }
}
//---------------------------------------------------------------------------
void FileInfo::DeleteFolder(const AnsiString &folderPath)
{
    TSearchRec sr;
    try
    {
        if(DirectoryExists(folderPath))
        {
            if(FindFirst(folderPath + "\\*.*", faAnyFile, sr) == 0)
            {
                do
                {
                    if (sr.Name != "." && sr.Name != "..")
                    {
                        AnsiString fullPath = folderPath + "\\" + sr.Name;
                        if (sr.Attr & faDirectory)
                        {
                            DeleteFolder(fullPath);
                        }
                        else
                        {
                            DeleteFile(fullPath);
                        }
                    }
                }
                while (FindNext(sr) == 0);
                FindClose(sr);
            }
            RemoveDir(folderPath);
        }
    }
    catch (Exception &E)
    {
//        ShowMessage("刪除失敗：" + E.Message);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
cDatabaseMin::cDatabaseMin()
{
    Clear();
}
//---------------------------------------------------------------------------
cDatabaseMin::~cDatabaseMin()
{
//
}
//---------------------------------------------------------------------------
bool cDatabaseMin::LoadFile(AnsiString dbpath,AnsiString dbName)
{
    return true;
}
//---------------------------------------------------------------------------
void cDatabaseMin::SaveFile()
{
//
}
//---------------------------------------------------------------------------
cDBTStringGrid::cDBTStringGrid(TStringGrid* _sg)
{
    sg=_sg;
    iRowHeight=25;
}
//---------------------------------------------------------------------------
cDBTStringGrid::~cDBTStringGrid()
{
//
}
//---------------------------------------------------------------------------
void cDBTStringGrid::Clear()
{
    if(sg)
    {
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        for(int irow=0;irow<recordCount;irow++)
        {
            for(int icol=0;icol<fieldCount;icol++)
            {
                SetCell(irow,icol,"");
            }
        }
        SetRowCol(1,1);
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::SetRowCol(int iRow,int iCol)
{
    if(sg)
    {
        sg->RowCount =iRow+1;//title
        sg->ColCount =iCol+1;//title
        sg->DefaultColWidth=100;
        sg->ColWidths[0]=30;
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::GetRowCol(int &iRow,int &iCol)
{
    if(sg)
    {
        iRow=sg->RowCount-1;//remove title
        iCol=sg->ColCount-1;//remove title
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::SetColTitle(int iCol,const AnsiString str)
{
    if(sg)
    {
        sg->Cells[iCol+1][0]=str;
    }
}
//---------------------------------------------------------------------------
AnsiString cDBTStringGrid::GetColTitle(int iCol)
{
    AnsiString str="";
    if(sg)
    {
        str=sg->Cells[iCol+1][0];
    }
    return str;
}
//---------------------------------------------------------------------------
void cDBTStringGrid::SetRowTitle(int iRow,const AnsiString str)
{
    if(sg)
    {
        sg->Cells[0][iRow+1]=str;
    }
}
//---------------------------------------------------------------------------
AnsiString cDBTStringGrid::GetRowTitle(int iRow)
{
    AnsiString sBack="";
    if(sg)
    {
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        if(iRow<recordCount)
        {
            sBack=sg->Cells[0][iRow+1];
        }
    }
    return sBack;
}
//---------------------------------------------------------------------------
void cDBTStringGrid::SetCell(int iRow,int iCol,const AnsiString str)
{
    if(sg)
    {
        iRow=iRow+1;
        iCol=iCol+1;
        int recordCount=0;
        int fieldCount=0;
        bool bReset=false;
        GetRowCol(recordCount,fieldCount);
        if(iRow>=recordCount)
        {
            recordCount=iRow;
            bReset=true;
        }
        if(iCol>=fieldCount)
        {
            fieldCount=iCol;
            bReset=true;
        }
        if(bReset)
            SetRowCol(recordCount,fieldCount);
        sg->Cells[iCol][iRow]=str;
    }
}
//---------------------------------------------------------------------------
AnsiString cDBTStringGrid::GetCell(int iRow,int iCol)
{
    AnsiString sBack="";
    if(sg)
    {
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        if(iCol<fieldCount && iRow<recordCount)
        {
            sBack=sg->Cells[iCol+1][iRow+1];
        }
    }
    return sBack;
}
//---------------------------------------------------------------------------
void cDBTStringGrid::AddRow()
{
    if(sg)
    {
        sg->RowCount++;
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::AddCol()
{
    if(sg)
    {
        sg->ColCount++;
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::CloneTo(TStringGrid* destination)
{
    if(sg)
    {
        destination->RowCount=sg->RowCount;
        destination->ColCount=sg->ColCount;
        for(int row=0;row<sg->RowCount;row++)
        {
            for(int col=0;col<sg->ColCount;col++)
            {
                destination->Cells[col][row]=sg->Cells[col][row];
            }
        }
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::ClearRow(int iRow)
{
    if(sg)
    {
        iRow++;                                      //ignor title
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        if(iRow<=recordCount)
        {
            for(int row=iRow;row<recordCount;row++)
            {
                for(int col=0;col<fieldCount;col++)
                {
                    sg->Cells[col][row]=sg->Cells[col][row+1];
                }
            }
            sg->RowCount--;
        }
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::HideRow(bool bOn,int iIndex)
{
    if(sg)
    {
        int iRowH=(bOn)?0:iRowHeight;
        iIndex++;                                      //ignor title
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        if(iIndex==0)
        {
            for(int row=0;row<recordCount;row++)
            {
                sg->RowHeights[row]=iRowH;
            }
        }
        else if(iIndex<=recordCount)
        {
            sg->RowHeights[iIndex]=iRowH;
        }
    }
}
//---------------------------------------------------------------------------
cMathTool::cMathTool()
{
    //
}
//---------------------------------------------------------------------------
cMathTool::~cMathTool()
{
    //
}
//---------------------------------------------------------------------------
void cMathTool::Clear()
{
    //
}
//---------------------------------------------------------------------------
bool cMathTool::IsDoubleEqual(const double a,const double b,const double tolerance)
{
    return fabs(a-b)<=tolerance;
}
//---------------------------------------------------------------------------
double cMathTool::Division(double Numerator,double Denominator)
{
    double Value;
    if(Denominator==0)
    {
        Value=0;
    }
    else
    {
        Value=Numerator/Denominator;
    }
    return Value;
}
//---------------------------------------------------------------------------
void cMathTool::EnsureMinMax(int &iMin, int &iMax)
{
    if (iMin > iMax)
    {                             
        int temp = iMin;
        iMin = iMax;
        iMax = temp;
    }
}
//---------------------------------------------------------------------------
int cMathTool::Ceil(double Numerator,double Denominator)
{
    int result = static_cast<int>(ceil(Division(Numerator,Denominator)));
    return result;
}
//---------------------------------------------------------------------------
//TThreadState
//---------------------------------------------------------------------------
void TThreadState::LogTask(int newTask)
{
    if (newTask == CurrentTask)
        return;  // 同樣判斷
    CurrentTask = newTask;
    History.push_front(TTaskHistory(newTask));
    if (History.size() > MAX_TASK_HISTORY)
        History.pop_back();
}
//---------------------------------------------------------------------------
//cStateRecord
//---------------------------------------------------------------------------
cStateRecord::cStateRecord()
{
    SetFolder(GetDefaultFolder());
    SetZipFileName();
    InitialThreadRecord();
}
//---------------------------------------------------------------------------
cStateRecord::~cStateRecord()
{
    //
}
//---------------------------------------------------------------------------
void cStateRecord::Clear()
{
    //
}
//---------------------------------------------------------------------------
int cStateRecord::DoShellExecute(AnsiString cmd)
{
    return (int)ShellExecute(NULL, "open", "cmd.exe", ("/C " + cmd).c_str(), NULL, SW_HIDE);
}
//---------------------------------------------------------------------------
void cStateRecord::SetFolder(AnsiString sFolder)
{
    saveFolder=sFolder;
    FileInfo().EnsureDirectoriesExist(saveFolder);
}
//---------------------------------------------------------------------------
void cStateRecord::SetZipFileName(AnsiString sfileName)
{
    if(sfileName=="")
    {
        Word SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec;
        uTimeTool().GetNowDateTime(SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
        sFileName=AnsiString().sprintf("%04d-%02d-%02d %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    }
    else
    {
        sFileName=sfileName;
    }
}
//---------------------------------------------------------------------------
AnsiString cStateRecord::GetTempZipFolder()
{
    AnsiString sret=FileInfo().PathCombin(saveFolder,sFileName);
    FileInfo().EnsureDirectoriesExist(sret);
    return sret;
}
//---------------------------------------------------------------------------
void cStateRecord::CopyFileToFolder(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFileName)
{
    AnsiString sSource=FileInfo().PathCombin(sSourcesFilePath,asFileName);
    AnsiString sTarget=FileInfo().PathCombin(sTargetFilePath,asFileName);
    if(FileExists(sSource))
    {
        CopyFile(sSource.c_str(), sTarget.c_str(), false);
    }
}
//---------------------------------------------------------------------------
void cStateRecord::CopyFolderToFolder(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFolderName)
{
    AnsiString sSource=FileInfo().PathCombin(sSourcesFilePath,asFolderName);
    AnsiString sTarget=FileInfo().PathCombin(sTargetFilePath,asFolderName);
    AnsiString cmd=AnsiString().sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", sSource, sTarget);
    DoShellExecute(cmd);
}
//---------------------------------------------------------------------------
bool cStateRecord::CompressWithExternalTool(AnsiString folderToCompress, AnsiString zipFileName)
{
    bool bret=false,bZipExist=false;
    AnsiString sTarget=FileInfo().PathCombin(asProjectPath,AnsiString("7z.exe"));
    bZipExist=FileExists(sTarget);
    if(bZipExist==false)                                                        //Steven 20110222 : 改成先把log給壓縮然後才複製
    {
        bZipExist=CopyFile(Get7ZipPath().c_str(), sTarget.c_str(), false);
    }
    if(bZipExist)
    {
        AnsiString cmd=AnsiString().sprintf("%s a -tzip \"%s.zip\" \"%s\"", sTarget, folderToCompress, folderToCompress);
        int result=DoShellExecute(cmd);
        bret=(result > 32);
    }
    return bret;
}
//---------------------------------------------------------------------------
void cStateRecord::DoFinish()
{
    AnsiString sTarget=GetTempZipFolder();
    CompressWithExternalTool(sTarget,sFileName);
    Sleep(1000);
    FileInfo().DeleteFolder(sTarget);
}
//---------------------------------------------------------------------------
void cStateRecord::InitialThreadRecord()
{
    cs = new TCriticalSection;
    AllStates.clear();
    AllStates.push_back(TThreadState("SortArm1Execute"));
    AllStates.push_back(TThreadState("SortArm2Execute"));
    AllStates.push_back(TThreadState("LoaderExecute"));
    AllStates.push_back(TThreadState("ActionTrack1Execute"));
    AllStates.push_back(TThreadState("ActionTrack2Execute"));
    AllStates.push_back(TThreadState("ActionTrack3Execute"));
    AllStates.push_back(TThreadState("ActionTrack4Execute"));
    AllStates.push_back(TThreadState("ActionEmpty1Execute"));
    AllStates.push_back(TThreadState("ActionEmpty1_TrackExecute"));
    AllStates.push_back(TThreadState("ActionEmpty2Execute"));
    AllStates.push_back(TThreadState("ActionStoreArmExecute"));
    AllStates.push_back(TThreadState("ActionMag1Execute"));
    AllStates.push_back(TThreadState("ActionMag2Execute"));
    AllStates.push_back(TThreadState("ActionMag3Execute"));
    AllStates.push_back(TThreadState("CCDLoaderExecute"));
    AllStates.push_back(TThreadState("CCDEmpty1Execute"));
    AllStates.push_back(TThreadState("CCDEmptyMGZExecute"));
    
//須同步增加enum
//enum eLogNumTask
//    {
//        eLNT_SortArm1Execute=0,
//        eLNT_SortArm2Execute,
//        eLNT_LoaderExecute,
//        eLNT_ActionTrack1Execute,
//        eLNT_ActionTrack2Execute,
//        eLNT_ActionTrack3Execute,
//        eLNT_ActionTrack4Execute,
//        eLNT_ActionEmpty1Execute,
//        eLNT_ActionEmpty1_TrackExecute,
//        eLNT_ActionEmpty2Execute,
//        eLNT_ActionStoreArmExecute,
//        eLNT_ActionMag1Execute,
//        eLNT_ActionMag2Execute,
//        eLNT_ActionMag3Execute,
//        eLNT_CCDLoaderExecute,
//        eLNT_CCDEmpty1Execute,
//        eLNT_CCDEmptyMGZExecute,
//        eLNT_Total
//    };
    AnsiString FromCase = " InitialAllTask() ";
    for(size_t m = 0; m < AllStates.size(); m++)
    {
        LogTask(m, 1);
    }
}
//---------------------------------------------------------------------------
void cStateRecord::LogTask(int moduleIndex, int newTask)
{
   int iStateNum=(int)AllStates.size();
   if (moduleIndex < 0 || moduleIndex >= iStateNum)
        return;
    cs->Enter();
    __try
    {
        AllStates[moduleIndex].LogTask(newTask);
    }
    __finally
    {
        cs->Leave();
    }
}
//---------------------------------------------------------------------------
void cStateRecord::ExportCurrentStateToCSV()
{
    AnsiString folder = GetDefaultFolder();
    ForceDirectories(folder);

    AnsiString fileName = Format("%sTaskHistory_%s.csv",
        ARRAYOFCONST((folder, FormatDateTime("yyyymmdd_hhnnss", Now()))));
    ExportTaskHistoryToCSV(fileName);
}
//---------------------------------------------------------------------------
bool cStateRecord::ExportTaskHistoryToCSV(const AnsiString& csvFilePath)
{
    bool result = false;
    cs->Enter();
    try
    {
        TStringList* lines = new TStringList();
        __try
        {
            // 第一行：表頭（固定格式）
            AnsiString header = "模組名稱,最新時間,最新Task";
            for(int i = 2; i <= MAX_TASK_HISTORY; i++)
                header += Format(",第%02d新時間,第%02d新Task", ARRAYOFCONST((i, i)));
            lines->Add(header);

            // 每一行：每個模組的歷史（從新到舊）
            for(size_t m = 0; m < AllStates.size(); m++)
            {
                const TThreadState& state = AllStates[m];
                const std::deque<TTaskHistory>& hist = state.History;

                AnsiString line = state.ThreadName;

                // 如果完全沒記錄，至少給當前 Task + 現在時間
                if (hist.empty() && state.CurrentTask == 0)
                {
                    line += ",,,,";  // 空一行
                    for(int i = 1; i < MAX_TASK_HISTORY; i++) line += ",,,";
                }
                else
                {
                    // 補滿到 10 筆（不夠就補空）
                    std::deque<TTaskHistory> fullHist;
                    if (!hist.empty())
                        fullHist.assign(hist.begin(), hist.end());
                    
                    // 如果 CurrentTask 比歷史還新（例如剛變但還沒被下一輪記到），加在最前面
                    if (!fullHist.empty() && state.CurrentTask != 0 &&
                        fullHist.front().Task != state.CurrentTask)
                    {
                        fullHist.push_front(TTaskHistory(state.CurrentTask));
                        fullHist.front().Time = Now();  // 或用你自己的最新時間
                    }

                    for(int i = 0; i < MAX_TASK_HISTORY; i++)
                    {
                        if (i < (int)fullHist.size())
                        {
                            const TTaskHistory& h = fullHist[i];
                            line += "," + FormatDateTime("hh:nn:ss.zzz", h.Time);
                            line += "," + IntToStr(h.Task);
                        }
                        else
                        {
                            line += ",,";  // 補空
                        }
                    }
                }

                lines->Add(line);
            }

            // 存檔 + 加 BOM（Excel 不亂碼）
            lines->SaveToFile(csvFilePath);
            result = true;
        }
        __finally
        {
            delete lines;
        }
        return true;
    }
    __finally
    {
        cs->Leave();
    }
    return result;
}
//---------------------------------------------------------------------------
TStringList* cStateRecord::GetAllCurrentTasks(bool bWithTime)
{
    TStringList* taskList = new TStringList();
    cs->Enter();  // 鎖定，防止同時讀寫
    __try
    {
        for(size_t i = 0; i < AllStates.size(); i++)
        {
            const TThreadState& state = AllStates[i];
            AnsiString status = state.GetCurrentStatus();  // 用你現有的 GetCurrentStatus

            if (!bWithTime)
            {
                // 簡化版：只顯示名稱 + Task（例如 "LoaderModule -> Task = 100"）
                status = Format("%s -> Task = %d", ARRAYOFCONST((state.ThreadName, state.CurrentTask)));
            }

            taskList->Add(status);
        }
    }
    __finally
    {
        cs->Leave();  // 解鎖
    }
    return taskList;  // 返回 TStringList，外面用完 delete
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// AI(HT172-Maintainer) 20260420 : StateRecord Phase 1 enhancement
//   - Adds GetTaskName step name lookup (Loader DL_*, StoreArm LTS_*)
//   - Adds CaptureCurrentTasks (17 modules with step names)
//   - Adds CaptureMachineState (RunMode, global flags, key tray/HasIC)
//   - Adds TriggerSnapshot one-shot zip with timestamped folder
//---------------------------------------------------------------------------
#include "database.h"
#include "cmydef.h"
#include "common.h"
#include "HT172_Module\aLoader.h"
#include "HT172_Module\aMagArm.h"
#include "HT172_Module\aMagazine.h"
#include "HT172_Module\aSortArm.h"
#include "HT172_Module\aTrayArm.h"
#include "mykitsuck.h"
//---------------------------------------------------------------------------
extern int gLastPickedMag;
//---------------------------------------------------------------------------
AnsiString cStateRecord::GetTaskName(int moduleIdx, int taskID)
{
    switch(moduleIdx)
    {
        case eLNT_LoaderExecute:
            switch(taskID)
            {
                case 0:  return "DL_IDLE";
                case 10: return "DL_LOAD_NEW_TRAY_RESET";
                case 20: return "DL_LOAD_NEW_TRAY";
                case 30: return "DL_TRAY_TO_WORK_RESET";
                case 40: return "DL_TRAY_TO_WORK";
                case 50: return "DL_TRAY_TO_TRANSFER_RESET";
                case 60: return "DL_TRAY_TO_TRANSFER";
            }
            break;
        case eLNT_SortArm2Execute:
            switch(taskID)
            {
                case 1:    return "MAGARM_IDLE";
                case 100:  return "MAGARM_CHECK_SORT_TRAY";
                case 1000: return "MAGARM_FIND_TOP_TRAY_BIN";
                case 1100: return "MAGARM_PNP_TO_TOP_EXISTING";
                case 1200: return "MAGARM_SEARCH_BIN";
                case 2000: return "MAGARM_PNP_TO_TOP_SEARCHED";
                case 5000: return "MAGARM_WORK_TRAY_OUT";
                case 7000: return "MAGARM_WAIT_WORK_TRAY_OUT";
                case 8000: return "MAGARM_TRAYFEED_SAFE_POS";
                case 8500: return "MAGARM_TRAYFEED_RETURN_HOME";
            }
            break;
        case eLNT_ActionStoreArmExecute:
            switch(taskID)
            {
                case 1:    return "LTS_CHECK_INITIAL";
                case 100:  return "LTS_CHECK_MOTOR_HOME";
                case 150:  return "LTS_MOVE_X_STANDBY";
                case 200:  return "LTS_GO_TO_TRACK";
                case 210:  return "LTS_TROLLEY_Z_DOWN";
                case 215:  return "LTS_TROLLEY_X_APPROACH";
                case 220:  return "LTS_TROLLEY_PUSH";
                case 230:  return "LTS_TROLLEY_RETRACT";
                case 300:  return "LTS_TRACK_STOP_POP";
                case 305:  return "LTS_BELT_PUSH";
                case 310:  return "LTS_MOTOR_CY_PUSH";
                case 320:  return "LTS_MOTOR_PUSH_MOVE";
                case 340:  return "LTS_MOTOR_RETURN";
                case 345:  return "LTS_MOTOR_RETRY_HOME";
                case 350:  return "LTS_MOTOR_HOME_CHECK";
                case 355:  return "LTS_MOTOR_HOME_WAIT";
                case 400:  return "LTS_VERIFY_TRAY";
                case 420:  return "LTS_CY_POP";
                case 500:  return "LTS_EDGE_FIXER_ON";
                case 510:  return "LTS_DATA_TRANSFER";
                case 520:  return "LTS_MOVE_TO_STORAGE_STBY";
                case 600:  return "LTS_SENSOR_DELAY";
                case 601:  return "LTS_MOVE_X_TO_STORAGE";
                case 700:  return "LTS_CHECK_STORE";
                case 800:  return "LTS_EDGE_FIXER_OFF";
                case 810:  return "LTS_MOVE_Z_IN_STORAGE";
                case 900:  return "LTS_WAIT_Z_PLACE";
                case 950:  return "LTS_VERIFY_PLACED";
                case 1000: return "LTS_RETURN_HOME";
            }
            break;
    }
    return AnsiString().sprintf("Task_%d", taskID);
}
//---------------------------------------------------------------------------
void cStateRecord::CaptureCurrentTasks(AnsiString txtFullPath)
{
    TStringList* lines = new TStringList();
    try
    {
        lines->Add("Idx | Module Name                | TaskID | Step Name                    | Last Change Time");
        cs->Enter();
        __try
        {
            for(size_t i = 0; i < AllStates.size(); i++)
            {
                const TThreadState& s = AllStates[i];
                AnsiString stepName = GetTaskName((int)i, s.CurrentTask);
                AnsiString tStr = s.History.empty()
                    ? AnsiString("(no history)")
                    : FormatDateTime("yyyy/mm/dd hh:nn:ss.zzz", s.History.front().Time);
                lines->Add(AnsiString().sprintf("%02d  | %-26s | %6d | %-28s | %s",
                    (int)i, s.ThreadName.c_str(), s.CurrentTask, stepName.c_str(), tStr.c_str()));
            }
        }
        __finally
        {
            cs->Leave();
        }
        lines->SaveToFile(txtFullPath);
    }
    __finally
    {
        delete lines;
    }
}
//---------------------------------------------------------------------------
void cStateRecord::CaptureMachineState(AnsiString iniFullPath)
{
    TIniFile* ini = new TIniFile(iniFullPath);
    try
    {
        ini->WriteString("System", "CaptureTime", FormatDateTime("yyyy/mm/dd hh:nn:ss.zzz", Now()));
        ini->WriteInteger("System", "RunMode", (int)HSys.Sys.RunMode);
        ini->WriteBool("System", "SystemStart", HSys.Sys.SystemStart);
        ini->WriteInteger("System", "RealDummy", HSys.LastSet.iRealDummy);

        ini->WriteBool("GlobalFlags", "bCleanOut", bCleanOut);
        ini->WriteBool("GlobalFlags", "bNeedLoadTrayToTransfer", bNeedLoadTrayToTransfer);
        ini->WriteBool("GlobalFlags", "bNeedLoadTrayToMagEmpty", bNeedLoadTrayToMagEmpty);
        ini->WriteBool("GlobalFlags", "bFlagLoaderCleanoutFinish", bFlagLoaderCleanoutFinish);
        ini->WriteBool("GlobalFlags", "bFlagEmpty1CleanoutFinish", bFlagEmpty1CleanoutFinish);
        ini->WriteBool("GlobalFlags", "bFlagEmpty2CleanoutFinish", bFlagEmpty2CleanoutFinish);
        ini->WriteBool("GlobalFlags", "bSortArm1OneCycleFinish", bSortArm1OneCycleFinish);
        ini->WriteBool("GlobalFlags", "bSortArm2OneCycleFinish", bSortArm2OneCycleFinish);
        ini->WriteBool("GlobalFlags", "bSortStoreArmOneCycleFinish", bSortStoreArmOneCycleFinish);

        if(LoaderModule != NULL)
        {
            if(LoaderModule->MotWorkLoader != NULL)
            {
                ini->WriteBool("Loader", "WorkArea_fHasTray", LoaderModule->MotWorkLoader->fHasTray);
                ini->WriteBool("Loader", "WorkArea_HasIC", LoaderModule->MotWorkLoader->HasIC());
            }
            if(LoaderModule->MotLoader_Car != NULL)
                ini->WriteBool("Loader", "Car_fHasTray", LoaderModule->MotLoader_Car->fHasTray);
            if(LoaderModule->MotTransferX_Data != NULL)
                ini->WriteBool("Loader", "TransferX_fHasTray", LoaderModule->MotTransferX_Data->fHasTray);
        }

        if(MagArmPara != NULL)
        {
            //AI(staterecord-analysis) 20260525 : capture PTI MagArm bin-selection context
            ini->WriteInteger("MagArm", "LastPickedMagIndex", gLastPickedMag);
            ini->WriteInteger("MagArm", "LastPickedMagNo", gLastPickedMag+1);
            ini->WriteInteger("MagArm", "WorkBin_iBin", MagArmPara->mBinToTray.iBin);
            ini->WriteInteger("MagArm", "WorkBin_iWhichAuto", MagArmPara->mBinToTray.iWhichAuto);
            ini->WriteInteger("MagArm", "WorkBin_iAutoRow", MagArmPara->mBinToTray.iAutoRow);
            ini->WriteInteger("MagArm", "NowBin_iBin", MagArmPara->mNowBin.iBin);
            ini->WriteInteger("MagArm", "NowBin_iWhichAuto", MagArmPara->mNowBin.iWhichAuto);
            ini->WriteInteger("MagArm", "NowBin_iAutoRow", MagArmPara->mNowBin.iAutoRow);
            ini->WriteInteger("MagArm", "iMagStatus", MagArmPara->iMagStatus);
            ini->WriteInteger("MagArm", "iEmptyMagStatus", MagArmPara->iEmptyMagStatus);
            ini->WriteBool("MagArm", "bNeedPlaceFirst", MagArmPara->bNeedPlaceFirst);
            if(MagArmPara->MMagSortTray != NULL)
            {
                ini->WriteBool("MagArm", "SortTray_fHasTray", MagArmPara->MMagSortTray->fHasTray);
                ini->WriteBool("MagArm", "SortTray_HasIC", MagArmPara->MMagSortTray->HasIC());
            }
            if(MagArmPara->MMagNowSortTray != NULL)
            {
                ini->WriteBool("MagArm", "NowSortTray_fHasTray", MagArmPara->MMagNowSortTray->fHasTray);
                ini->WriteBool("MagArm", "NowSortTray_HasIC", MagArmPara->MMagNowSortTray->HasIC());
            }
            if(MagArmPara->MagTransferLD != NULL)
                ini->WriteBool("MagArm", "TransferLD_fHasTray", MagArmPara->MagTransferLD->fHasTray);
            if(MagArmPara->MagArmSuck != NULL)
                ini->WriteBool("MagArm", "Suck_HasIC", MagArmPara->MagArmSuck->HasIC());

            for(int i=0; i<eMGZTotal; i++)
            {
                AnsiString key;
                if(MagArmPara->MMagSortTopTray[i] != NULL)
                {
                    key.sprintf("Mag%d_TopTray_fHasTray", i+1);
                    ini->WriteBool("MagArm", key, MagArmPara->MMagSortTopTray[i]->fHasTray);
                    key.sprintf("Mag%d_TopTray_HasIC", i+1);
                    ini->WriteBool("MagArm", key, MagArmPara->MMagSortTopTray[i]->HasIC());
                    key.sprintf("Mag%d_TopTray_FullIC", i+1);
                    ini->WriteBool("MagArm", key, MagArmPara->MMagSortTopTray[i]->FullIC());
                }
                if(MagazineModule[i] != NULL)
                {
                    key.sprintf("Mag%d_iMagArmStatus", i+1);
                    ini->WriteInteger("MagArm", key, MagazineModule[i]->iMagArmStatus);
                    key.sprintf("Mag%d_TopTray_iBin", i+1);
                    ini->WriteInteger("MagArm", key, MagazineModule[i]->mTopTray.iBin);
                    key.sprintf("Mag%d_TopTray_iWhichAuto", i+1);
                    ini->WriteInteger("MagArm", key, MagazineModule[i]->mTopTray.iWhichAuto);
                    key.sprintf("Mag%d_TopTray_iAutoRow", i+1);
                    ini->WriteInteger("MagArm", key, MagazineModule[i]->mTopTray.iAutoRow);
                }
            }
        }

        if(SortArmPara != NULL && SortArmPara->SortArmSuck != NULL)
            ini->WriteBool("SortArm", "Suck_HasIC", SortArmPara->SortArmSuck->HasIC());
    }
    __finally
    {
        delete ini;
    }
}
//---------------------------------------------------------------------------
void cStateRecord::TriggerSnapshot(AnsiString triggerReason)
{
    Word y, mo, d, h, mi, s, ms;
    uTimeTool().GetNowDateTime(y, mo, d, h, mi, s, ms);
    AnsiString stamp = AnsiString().sprintf("%04d-%02d-%02d %02d_%02d_%02d", y, mo, d, h, mi, s);
    SetZipFileName(stamp);
    AnsiString tempDir = GetTempZipFolder();

    AnsiString snapPath = FileInfo().PathCombin(tempDir, "Snapshot.ini");
    TIniFile* ini = new TIniFile(snapPath);
    try
    {
        ini->WriteString("Snapshot", "TriggerReason", triggerReason);
        ini->WriteString("Snapshot", "TriggerTime", stamp);
        ini->WriteString("Snapshot", "MachineID", asHandlerID);
        ini->WriteString("Snapshot", "Version", "Phase1-20260420");
    }
    __finally
    {
        delete ini;
    }

    ExportTaskHistoryToCSV(FileInfo().PathCombin(tempDir, "TaskHistory.csv"));
    CaptureCurrentTasks(FileInfo().PathCombin(tempDir, "CurrentTasks.txt"));
    CaptureMachineState(FileInfo().PathCombin(tempDir, "MachineState.ini"));

    if(HSys.BinDisCtrl!=NULL)
        HSys.BinDisCtrl->FlushBinDisplayLog();
    if(HSys.BinDisCtrlMag!=NULL)
        HSys.BinDisCtrlMag->FlushBinDisplayLog();
    if(HSys.BinDisCtrlTFT!=NULL)
        HSys.BinDisCtrlTFT->FlushBinDisplayLog();
    if(HSys.BinDisCtrlMagTFT!=NULL)
        HSys.BinDisCtrlMagTFT->FlushBinDisplayLog();

    AnsiString srcDir = AnsiString().sprintf("D:\\HT-172_Log\\EventLog\\%04d_%02d", y, mo);
    AnsiString srcFile = AnsiString().sprintf("%s_%04d_%02d_%2d.csv", asHandlerID.c_str(), y, mo, d);
    CopyFileToFolder(srcDir, tempDir, srcFile);

    AnsiString binSrcDir = AnsiString().sprintf("D:\\HT-172_Log\\BinDisplayLog\\%04d\\%02d", y, mo);
    AnsiString binSrcFile = AnsiString().sprintf("BinDisplayLog_%04d%02d%02d.csv", y, mo, d);
    CopyFileToFolder(binSrcDir, tempDir, binSrcFile);

    DoFinish();
}
//---------------------------------------------------------------------------
