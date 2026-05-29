#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build

#pragma hdrstop

#include "common.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
AnsiString asProjectPath          ="D:\\HT172";
AnsiString asGeneralPath          ="D:\\HT172\\system\\Gerneral.ini";
AnsiString LastDataPath           ="D:\\HT172\\SetUp.inf";
AnsiString SecsGemPath            ="D:\\HT172\\SECS";
AnsiString asKeyStr               ="HontechPassword";                                     //密碼金鑰, 不能改
AnsiString asSaveEventLogPath     ="D:\\HT-172_Log\\EventLog";
AnsiString asOLPLogPath           ="D:\\HT-172_Log\\Automation";
char ConverStr[16];

TIniFile *INIFileGeneral ; //Steven 20141120 : Add Read/Write IniFile Speed
TIniFile *INIFile ;        //Steven 20141120 : Add Read/Write IniFile Speed

SHFILEOPSTRUCT oFile;
int CheckPtrItem=0;
int iNormalDelayTimeSec=20;
AnsiString MainVersion               ="";
//---------------------------------------------------------------------------
AnsiString EncodeStr(AnsiString sourceStr)  //jou 2013-01-04 Password Txt 加密
{   //編碼
    int l1,l2,p1,p2;
    byte ch;
    AnsiString Result;
    l1 = sourceStr.Length();
    p1 = 1;
    l2= asKeyStr.Length();
    p2= 1;
    Result= "";
    while (p1<=l1)
    {
        ch =(byte(sourceStr[p1])-1) ^ byte(asKeyStr[p2]);
        if (ch==0) ch= !(byte(asKeyStr[p2]));
        Result = Result+ char(ch);
        p1++;
        if (p2==l2) p2=1; else p2++;
    }
    return Result;
}
//---------------------------------------------------------------------------
AnsiString DecodeStr(AnsiString sourceStr)  //jou 2013-01-04 Password Txt 加密
{   //解碼
    int l1,l2,p1,p2;
    byte ch;
    AnsiString Result;
    l1 = sourceStr.Length();
    p1 = 1;
    l2= asKeyStr.Length();
    p2= 1;
    Result= "";
    while (p1<=l1)
    {
        ch=byte(sourceStr[p1]);
        if (ch==(!byte(asKeyStr[p2]))) ch=byte(asKeyStr[p2]);
        else ch=ch ^ byte(asKeyStr[p2]);
        Result=Result+CHAR(ch+1);
        p1++;
        if (p2==l2) p2=1; else p2++;
    }
    return Result;
}
//---------------------------------------------------------------------------
void OpenIniFile(AnsiString FileName)              //Steven 20141120 : Add Read/Write IniFile Speed
{
    if(INIFile==NULL || INIFile->FileName!=FileName)
    {
        CloseIniFile();
        INIFile=new TIniFile(FileName);
    }
}
//---------------------------------------------------------------------------
void CloseIniFile()             //Steven 20141120 : Add Read/Write IniFile Speed
{
    if(INIFile!=NULL)
    {
        INIFile->UpdateFile();
        delete INIFile;
    }
}
//---------------------------------------------------------------------------
bool __fastcall CheckIniData(AnsiString FileName, AnsiString Group, AnsiString Name)
{
    bool bTemp;
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    bTemp = INIFile->ValueExists(Group, Name);
    return bTemp;
}
//---------------------------------------------------------------------------
double __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteFloat(Group, Name, Value);
    else
        Value=INIFile->ReadFloat(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
unsigned long __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value)
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteInteger(Group, Name, Value);
    else
        Value=INIFile->ReadInteger(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
int __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteInteger(Group, Name, Value);
    else
        Value=INIFile->ReadInteger(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value)
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteBool(Group, Name, Value);
    else
        Value=INIFile->ReadBool(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteString(Group, Name, Value);
    else
        Value=INIFile->ReadString(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
TDateTime __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value)
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    if(!INIFile->ValueExists(Group, Name))
        INIFile->WriteDateTime(Group, Name, Value);
    else
        Value=INIFile->ReadDateTime(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)
{
    bool ret;
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    ret=INIFile->ReadBool(Group, Name, bValue);
    return ret;
}
//---------------------------------------------------------------------------
double __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)
{
    double ret;
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    ret=INIFile->ReadFloat(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
int __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)
{
    int ret;
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    ret=INIFile->ReadInteger(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
AnsiString __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)
{
    AnsiString ret;
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    ret=INIFile->ReadString(Group, Name, Value);
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)  //Steven 20090731
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    INIFile->WriteBool(Group, Name, bValue);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)  //Steven 20090731
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    INIFile->WriteInteger(Group, Name, Value);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)  //Steven 20090731
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    INIFile->WriteFloat(Group, Name, Value);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, unsigned long Value)
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    INIFile->WriteInteger(Group, Name, Value);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value) //Steven 20090731
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    INIFile->WriteString(Group, Name, Value);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value)
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    INIFile->WriteDateTime(Group, Name, Value);
}
//---------------------------------------------------------------------------
void __fastcall ReplaceIniData(AnsiString FileName, AnsiString Group, AnsiString Name1, AnsiString Name2) //Steven 20090731
{
    OpenIniFile(FileName);  //Steven 20141120 : Add Read/Write IniFile Speed
    AnsiString  ret=INIFile->ReadString(Group, Name2, " ");
    INIFile->WriteString(Group, Name1, ret);
}
//---------------------------------------------------------------------------
AnsiString __fastcall GetLastOpenFN()
{
    AnsiString Str="Fail Open";
    TStringList *MyList= new TStringList();        //Steven 20141120 : delete MyList

    if(FileExists(LastDataPath))
    {
        MyList->LoadFromFile(LastDataPath);
        Str=MyList->Strings[0];
    }
    else
    {
        ShowMyMessage("LastData does not exist");
    }

    delete MyList;

    return Str;
}
//---------------------------------------------------------------------------
bool __fastcall CheckSectionExist(AnsiString FileName, AnsiString Group)
{
    if(Group=="")
        return false;

    TIniFile *INIFile = new TIniFile(FileName);
    bool  ret=INIFile->SectionExists(Group);
    delete INIFile;
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall WriteLastDataFN(AnsiString SName)
{
    int iFileHandle;
    TStringList *MyList= new TStringList();                                     //Steven 20141120 : delete MyList

    if(FileExists(LastDataPath))
    {
        MyList->LoadFromFile(LastDataPath);
        MyList->Clear();
        MyList->Add(SName);
        MyList->SaveToFile(LastDataPath);
    }
    else
    {
        iFileHandle=FileCreate(LastDataPath);
        FileWrite(iFileHandle, SName.c_str(), SName.Length());
        FileClose(iFileHandle);
    }
    delete MyList;
}
//---------------------------------------------------------------------------
void __fastcall AddSpace(AnsiString File)
{
    return;
//    char *ptr;
//    int i,Datalength=0;
//    bool FirstChar=false, FileHead=false;
//    bool bNoAdd=false;
//    AnsiString S="";
//    MyList->LoadFromFile(File);
//    ptr=MyList->GetText();
//    Datalength=strlen(ptr);
//    MyList->Clear();
//    for(i=0;i<Datalength;i++)
//    {
//        if(FileHead==false)
//        {
//            if(*ptr==' ')
//                return;
//            FileHead=true;
//        }
//        if(*ptr=='\r' && *(ptr+1)=='\n')
//        {
//            MyList->Add(S);
//            S="";
//            ptr+=2;
//            FirstChar=false;
//        }
//        else
//        {
//            if(FirstChar==false)
//            {
//                if(*ptr!='[' && *ptr!=' ' && *ptr!='\t')
//                {
//                    S+="\t";
//                }
//                else if(*ptr==' ')
//                {
//                    bNoAdd=true;
//                }
//                else if(*ptr=='\t')
//                {
//                    bNoAdd=true;
//                }
//                else if(*ptr=='[')
//                {
//                    MyList->Add("  ");
//                }
//
//                if(bNoAdd==false)
//                    FirstChar=true;
//            }
//
//            if(bNoAdd)
//            {
//                bNoAdd=false;
//            }
//            else
//            {
//                S+=*ptr;
//            }
//            ptr++;
//        }
//    }
//    DeleteFile(File);
//    MyList->SaveToFile(File);
}
//---------------------------------------------------------------------------
bool __fastcall OnlyNumberInPut(int iKey)//only for Number
{
    if((iKey>='0' && iKey<='9') || iKey==8 || iKey=='-')//iKey==8 back space
        return true;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall OnlyNumberAndDotInPut(int iKey)//only for Number and char '.'
{
    if((iKey>='0' && iKey<='9') || iKey=='.' || iKey==8|| iKey=='-')//iKey==8 back space
        return true;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall OnlyMakeFileDataInPut(int iKey)
{
    if(iKey=='\'' ||  iKey=='/' || iKey==':'  || iKey=='*' ||
       iKey=='?'  ||  iKey=='"' ||  iKey=='<' || iKey=='|')
            return false;
    return true;
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str)
{
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, str, -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor)
{
    pCanvas->Brush->Color=BrushColor;
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, str, -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, TColor BrushColor, TColor FontColor)
{
    pCanvas->Brush->Color=BrushColor;
    pCanvas->Font->Color =FontColor;
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, str, -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int str)
{
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, AnsiString(str).c_str(), -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, char *str, int left, int right, int top, int bottom)
{
    Rect.Left=left;
    Rect.Top=top;
    Rect.Right=right;
    Rect.Bottom=bottom;
    pCanvas->FillRect(Rect);
    DrawText(pCanvas->Handle, str, -1, &Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
//---------------------------------------------------------------------------
void __fastcall MyDrawText(TCanvas *pCanvas, TRect &Rect, int left, int right, int top, int bottom)
{
    Rect.Left=left;
    Rect.Top=top;
    Rect.Right=right;
    Rect.Bottom=bottom;
    pCanvas->FillRect(Rect);
}
//---------------------------------------------------------------------------
//Steven 20110711 : 把General.ini的存取改成獨立的
//---------------------------------------------------------------------------
void OpenGeneralIniFile()           //Steven 20141120 : Add Read/Write IniFile Speed
{
    CloseGeneralIniFile();
    INIFileGeneral = new TIniFile(asGeneralPath);
}
//---------------------------------------------------------------------------
void CloseGeneralIniFile()          //Steven 20141120 : Add Read/Write IniFile Speed
{
    if(INIFileGeneral!=NULL)
    {
        INIFileGeneral->UpdateFile();
        delete INIFileGeneral;
    }
}
//---------------------------------------------------------------------------
double __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, double Value)
{
    if(!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteFloat(Group, Name, Value);
    else
        Value=INIFileGeneral->ReadFloat(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
int __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, int Value)
{
    if(!INIFileGeneral->ValueExists(Group, Name))
    {
        INIFileGeneral->WriteInteger(Group, Name, Value);
    }
    else
    {
        Value=INIFileGeneral->ReadInteger(Group, Name, Value);
    }
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, bool Value)
{
    if(!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteBool(Group, Name, Value);
    else
        Value=INIFileGeneral->ReadBool(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall CheckAndReadIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value)
{
    if(!INIFileGeneral->ValueExists(Group, Name))
        INIFileGeneral->WriteString(Group, Name, Value);
    else
        Value=INIFileGeneral->ReadString(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, bool bValue)  //Steven 20090731
{
    INIFileGeneral->WriteBool(Group, Name, bValue);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, int Value)  //Steven 20090731
{
    INIFileGeneral->WriteInteger(Group, Name, Value);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, double Value)  //Steven 20090731
{
    INIFileGeneral->WriteFloat(Group, Name, Value);
}
//---------------------------------------------------------------------------
void __fastcall WriteIniDataGeneral(AnsiString Group, AnsiString Name, AnsiString Value) //Steven 20090731
{
    INIFileGeneral->WriteString(Group, Name, Value);
}
//---------------------------------------------------------------------------
void WriteDataToFile(AnsiString cFilePath, AnsiString cData, bool bOverWrite)
{
    FILE *pFile;
    if(bOverWrite)
    {
        pFile=fopen(cFilePath.c_str(), "w");
    }
    else
    {
        pFile=fopen(cFilePath.c_str(), "a");
    }
    if(pFile!=NULL)
    {
        fputs(cData.c_str(), pFile);
        fputs("\n", pFile);             //kevin 20170118 Add分行符號
        fclose(pFile);
    }
}
//---------------------------------------------------------------------------
char* ReadDataFromFile(AnsiString cFilePath)
{
    FILE* pFile=fopen(cFilePath.c_str(), "r");
    if(pFile!=NULL)
    {
        fseek(pFile, 0, SEEK_END);
        long file_size=ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        char* file_buf=(char*)malloc(file_size + 1);
        fread(file_buf, file_size, 1, pFile);
        file_buf[file_size]='\0';
        fclose(pFile);
        return file_buf;
    }
    return NULL;
}
//---------------------------------------------------------------------------
//Steven 20210112 : 針對資料夾加上保護
//==>
int MyForceDirectories(AnsiString Directory)
{
    AnsiString Str;
    if(Directory=="")
    {
        ShowMyMessage("Directory value is NULL!");
        return -1;
    }
    else
    {
        try
        {
            if(DirectoryExists(Directory)==false)
            {
                ForceDirectories(Directory);
            }
        }
        catch(Exception& e)      //Steven 20140505 : 試著抓出連線異常的訊息
        {
            ShowMyMessage(e.Message);
            return -1;
        }
        catch(...)
        {
            ShowMyMessage("Create directory fail!");
            return -1;
        }
    }

    return 1;
}
//<==
//Steven 20210112 : 針對資料夾加上保護
//---------------------------------------------------------------------------
AnsiString GetDoubleToStr(double dValue)
{
    return AnsiString().sprintf("%02.2f",dValue);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void DoRunExecutFile(AnsiString sExecutFullFileName, AnsiString sParameters)    //Mylin 20180911 Add Lead Scan Function Tray Report Defect Code
{
    if(sExecutFullFileName.Trim()=="")
    {
        ShowMyMessage("Do Run Execut File Path Is Empty Error!");
        return;
    }
    try
    {
        SHELLEXECUTEINFO execinfo;
        memset(&execinfo, 0, sizeof(execinfo));
        execinfo.cbSize=sizeof(execinfo);
        execinfo.lpVerb="open";
        execinfo.lpFile=sExecutFullFileName.c_str();
        if(FileExists(execinfo.lpFile)==true)
        {
            MySleep(100);                                                       //Jimmychiu 20220901 降低ACM交握影響主程序效能
            if(sParameters.Trim()!="")
            {
                execinfo.lpParameters=sParameters.c_str();
            }
            else
            {
                execinfo.lpParameters="";
            }
            execinfo.fMask=SEE_MASK_NOCLOSEPROCESS;
            execinfo.nShow=SW_SHOWMINIMIZED;
            ShellExecuteEx(&execinfo);
            MySleep(100);                                                       //Jimmychiu 20220901 降低ACM交握影響主程序效能
        }
        else
        {
            AnsiString sMsg="Run Execut File Is Not Exists! "+sExecutFullFileName;;
            ShowMyMessage(sMsg);
        }
        CloseHandle(execinfo.hProcess);
    }
    catch(...)
    {
        //
    }
}
