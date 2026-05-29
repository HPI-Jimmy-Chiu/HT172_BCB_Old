#include "IncludeAllHeader.h"
#pragma hdrstop

#include "MyStringList.h"
//#include "cmydef.h"
//#include "MemoryAlarm.h"
//#include "HonNewDel.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMyStringList::TMyStringList()
{
    MaxLineCount        =1000;
    Path                ="D:\\HandlerLog";
    FirstRow            ="";
    SaveType            =TByDay;
    AutoSave            =true;
    bUseFTRT            =false;
    bFilePathWithDate   =true;          //Steven 20210623 : ¹w³]¦sÀÉ­n¦³¤é´Á·í¸ê®Æ§¨
    MyList              =new TStringList();
//    MyList              =HonNew(MyList, __FILE__, __FUNC__, __LINE__);
}
//---------------------------------------------------------------------------
__fastcall TMyStringList::TMyStringList(AnsiString sPath, AnsiString sFileName, AnsiString sFirstRow)
{
    MaxLineCount        =1;
    Path                =sPath;
    FileName            =sFileName;
    FirstRow            =sFirstRow;

//    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
//        SaveType            =TByHour;         //kevin 20210518 Data too large    TByHour=TByDay;
//    else
        SaveType            =TByDay;

    AutoSave            =true;
    bUseFTRT            =false;
    bFilePathWithDate   =true;          //Steven 20210623 : ¹w³]¦sÀÉ­n¦³¤é´Á·í¸ê®Æ§¨
    MyList              =new TStringList();
//    MyList              =HonNew(MyList, __FILE__, __FUNC__, __LINE__);
}
//---------------------------------------------------------------------------
__fastcall TMyStringList::~TMyStringList()
{
    try
    {
        MySaveToFile();
        MyList->Clear();    //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        delete MyList;
//        HonDelete(MyList);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMyStringList");
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetPath(AnsiString P)
{
    HTPath=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetFileName(AnsiString P)
{
    HTFileName=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetFirstRow(AnsiString P)
{
    HTFirstRow=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetMaxLineCount(int Cnt)
{
    HTMaxLineCount=Cnt;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetSaveType(TSaveType Type)
{
    HTSaveType=Type;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetAutoSave(bool P)
{
    HTAutoSave=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::AddText(AnsiString Msg)
{
    GetTimeInfo();

    if(FileName=="")
        FileName="";

    if(MyList->Count>HTMaxLineCount)
    {
        MySaveToFile();
        MyList->Clear();
    }
    MyList->Add(Msg);
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::AddTextWithDateTime(AnsiString Msg)
{
    AnsiString Str;
    GetTimeInfo();
    Str.sprintf("%04d-%02d-%02d, %02d:%02d:%02d.%03d, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Msg);

    if(FileName=="")
        FileName="";

    if(MyList->Count>HTMaxLineCount)
    {
        MySaveToFile();
        MyList->Clear();
    }
    MyList->Add(Str);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMyStringList::AddTextWithDateTime2(AnsiString Msg)
{
    AnsiString Str;
    GetTimeInfo();
    Str.sprintf("%04d/%02d/%02d,%02d:%02d:%02d,%s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, Msg);

    if(FileName=="")
        FileName="";

    if(MyList->Count>HTMaxLineCount)
    {
        MySaveToFile();
        MyList->Clear();
    }
    MyList->Add(Str);
    return Str;
}//---------------------------------------------------------------------------
void TMyStringList::GetTimeInfo()
{
    static TDateTime dtPresent;
    dtPresent=Now();
    DecodeDate(dtPresent, SystemYear, SystemMonth, SystemDate);
    DecodeTime(dtPresent, SystemHour, SystemMin, SystemSec, SystemMSec);
}
//---------------------------------------------------------------------------
void TMyStringList::MySaveToFile()
{
    AnsiString sPathName;
    AnsiString sFileName;
    AnsiString Str;
    FILE *pFile;
    GetTimeInfo();

    if(HTAutoSave==false || MyList==NULL || MyList->Count==0)   //¨S¸ê®Æ´N¤£¥Î¦sÀÉ
        return;

    sFileName=GetFileName();

    if(FileExists(sFileName)==false && HTFirstRow!="")
    {
        Str=HTFirstRow+"\r\n"+MyList->Text;
    }
    else
    {
        Str=MyList->Text;
    }

    Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);
    pFile=fopen(sFileName.c_str(), "a");
    if(pFile!=NULL)
    {
        fputs(Str.c_str(), pFile);
        fclose(pFile);
    }
    pFile=NULL;
    MyList->Clear();
}
//---------------------------------------------------------------------------
void TMyStringList::MySaveFileByFileNameAndType(AnsiString asFileName, AnsiString asLotId, AnsiString asFileType) //Sam 20171215 (wei) : ÀÉ¦W§À¤Ú¼W¥[JAM/INPUT //Sam 20170428 (wei) ±a¤J LotID //RogerYang 20170406 (Steven) å¸¶å…¥æª”æ??ç¨±
{
    AnsiString sPathName;
    AnsiString sFileName;
    AnsiString Str;
    FILE *pFile;
    GetTimeInfo();

    if(HTAutoSave==false || MyList->Count==0)
        return;

    sPathName.sprintf("D:\\HT9045_Log\\JamAlarmLogTxt\\%s", asLotId);

    if(DirectoryExists(sPathName)==false)
    {
        ForceDirectories(sPathName);
    }

    sFileName.sprintf("%s\\%s_%04d%02d%02d_%s_%s.csv", sPathName, asFileName, SystemYear, SystemMonth, SystemDate, asLotId, asFileType); //Sam 20171215 (wei) : ÀÉ¦W§À¤Ú¼W¥[JAM/INPUT

    if(FileExists(sFileName)==false && HTFirstRow!="")
    {
        Str=HTFirstRow+"\r\n"+MyList->Text;
    }
    else
    {
        Str=MyList->Text;
    }

    Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);
    pFile=fopen(sFileName.c_str(), "a");
    if(pFile!=NULL)
    {
        fputs(Str.c_str(), pFile);
        fclose(pFile);
    }
    pFile=NULL;
    MyList->Clear();
}
//---------------------------------------------------------------------------
void TMyStringList::MySaveFileByFileName(AnsiString asPathName, AnsiString asFileName)   //Sam 20170516 (wei) ¤O¦¨ SocketID Count «O¦s
{
    AnsiString Str;
    FILE *pFile;

    if(DirectoryExists(asPathName)==false)
    {
        ForceDirectories(asPathName);
    }

    asFileName.sprintf("%s\\%s", asPathName, asFileName);

    if(FileExists(asFileName)==false && HTFirstRow!="")
    {
        Str=HTFirstRow+"\r\n"+MyList->Text;
    }
    else
    {
        Str=MyList->Text;
    }

    Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);
    pFile=fopen(asFileName.c_str(), "a");
    if(pFile!=NULL)
    {
        fputs(Str.c_str(), pFile);
        fclose(pFile);
    }
    pFile=NULL;
    MyList->Clear();
}
//---------------------------------------------------------------------------
AnsiString TMyStringList::GetFileName()
{
    AnsiString sPathName;
    AnsiString sFileName="";
    AnsiString Str;
    int iHour;
    GetTimeInfo();

    if(HTPath=="")
        Path="D:\\HandlerLog";

    if(FileName=="")
        FileName="";

    if(bFilePathWithDate)          //Steven 20210623 : ¹w³]¦sÀÉ­n¦³¤é´Á·í¸ê®Æ§¨
    {
        if(HTSaveType>=TByMonth)    //¨Ï¥Î¦~¤ë¦sÀÉªº¸Ü,´NBy¦~¤ÀÃþ
        {
            sPathName.sprintf("%s\\%04d", HTPath, SystemYear);
        }
        else if(HTSaveType>=TByDay) //¨Ï¥Î¨C¤é¦sÀÉªº¸Ü,´NBy¤ë¤ÀÃþ
        {
            sPathName.sprintf("%s\\%04d\\%02d", HTPath, SystemYear, SystemMonth);
        }
        else                        //¨ä¥L´N¨C¦~¦s¦¨365­Ó¸ê®Æ§¨
        {
//            if(IniConfig.iN10UploadProductMethod==0)    //JerryYang 20190131 ¤W¶Çproduction log¥i¿ï¾Ü00:00 or 08:00
//            {
//                sPathName.sprintf("%s\\%04d\\%02d\\%02d", HTPath, SystemYear, SystemMonth, SystemDate);
//            }
//            else
            {
                if(SystemHour<8)
                {
                    GetYesterdayInfo();
                    sPathName.sprintf("%s\\%04d\\%02d\\%02d", HTPath, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);
                }
                else
                {
                    sPathName.sprintf("%s\\%04d\\%02d\\%02d", HTPath, SystemYear, SystemMonth, SystemDate);
                }
            }
        }
    }
    else
    {
        sPathName=HTPath;
    }

    if(DirectoryExists(sPathName)==false)
    {
        ForceDirectories(sPathName);
    }

    if(HTSaveType==TByMaxLineCount)
    {
        sFileName.sprintf("%s\\%s_%04d%02d%02d %02d%02d%02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    }
    else
    {
        if(HTSaveType==TByHour)
        {
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour);
        }
        else if(HTSaveType==TBy2Hour)
        {
            iHour=SystemHour-SystemHour%2;
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
        }
        else if(HTSaveType==TBy4Hour)
        {
            iHour=SystemHour-SystemHour%4;
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
        }
        else if(HTSaveType==TBy6Hour)
        {
            iHour=SystemHour-SystemHour%6;
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
        }
        else if(HTSaveType==TBy8Hour)
        {
            iHour=SystemHour-SystemHour%8;
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
        }
        else if(HTSaveType==TBy12Hour)
        {
//            if(IniConfig.iN10UploadProductMethod==0)    //JerryYang 20190131 ¤W¶Çproduction log¥i¿ï¾Ü00:00 or 08:00
//            {
//                iHour=SystemHour-SystemHour%12;
//                sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
//            }
//            else
            {
                if(SystemHour<8)
                {
                    GetYesterdayInfo();
                    sFileName.sprintf("%s\\%s_%04d%02d%02d%02d00.csv", sPathName, HTFileName, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday, 20);
                }
                else if(SystemHour>=20)
                {
                    sFileName.sprintf("%s\\%s_%04d%02d%02d%02d00.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, 20);
                }
                else
                {
                    sFileName.sprintf("%s\\%s_%04d%02d%02d%02d00.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, 8);
                }
            }
        }
        else if(HTSaveType==TByDay)
        {
            sFileName.sprintf("%s\\%s_%04d%02d%02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate);
        }
        else if(HTSaveType==TByMonth)
        {
            sFileName.sprintf("%s\\%s_%04d%02d.csv", sPathName, HTFileName, SystemYear, SystemMonth);
        }
        else if(HTSaveType==TByMin)
        {
            if(bUseFTRT)
            {
//                if(LastSet.iRunStartMode==rsmContinuRetest || LastSet.iRunStartMode==rsmCInitialRetest)
//                    sFileName.sprintf("%s\\%s_%04d%02d%02d_%02d%02d_RT.TXT", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
//                else
                    sFileName.sprintf("%s\\%s_%04d%02d%02d_%02d%02d_FT.TXT", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
            }
            else
            {
                sFileName.sprintf("%s\\%s_%04d%02d%02d_%02d%02d.TXT", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
            }
        }
        else //if(HTSaveType==TByYear)
        {
            sFileName.sprintf("%s\\%s_%04d.csv", sPathName, HTFileName, SystemYear);
        }
    }
    return sFileName;
}
//---------------------------------------------------------------------------
int TMyStringList::GetLastLine()     //Steven 20191016 : ¨ú±o¥Ø«eÀÉ®×ªº¦æ¼Æ
{
    AnsiString sFileName=GetFileName();
    TStringList *File;
    int iCount=0;
    GetTimeInfo();

    if(FileExists(sFileName))       //Steven 20200221 : ­×¥¿¨S¦³ÀÉ®×´N¤£­nÅªÀÉ
    {
        File=new TStringList();
        File->LoadFromFile(sFileName);
        iCount=File->Count;
        File->Clear();
        delete File;
    }
    sLastFileName=sFileName;           //Steven 20191107 : ¬ö¿ý²{¦bªºÀÉ¦W

    return iCount;
}
//---------------------------------------------------------------------------
void TMyStringList::MyInsertToFile(int iCount)
{
    if(HTAutoSave==false || MyList->Count==0)   //¨S¸ê®Æ´N¤£¥Î¦sÀÉ
        return;

    AnsiString Str;
    TStringList *File;
    File=new TStringList();

    if(FileExists(sLastFileName)==false)
    {
        sLastFileName=GetFileName();
    }
    else        //Steven 20200221 : ­×¥¿¨S¦³ÀÉ®×´N¤£­nÅªÀÉ
    {
        File->LoadFromFile(sLastFileName);
    }

    Str=MyList->Text.SubString(0, MyList->Text.Length()-2);
    Str.Insert(' ', 12);
    Str.Insert(' ', 26);

    if(FileExists(sLastFileName) && File->Count>iCount && iCount>0)              //Steven 20200215 : ¥[¤W«OÅ@¾÷¨î
        File->Insert(iCount, Str);
    else
        File->Add(Str);
//    File->Delete(iCount+1);
    File->SaveToFile(sLastFileName);    //Steven 20191107 : ¬ö¿ý²{¦bªºÀÉ¦W
    File->Clear();
    MyList->Clear();
    delete File;
}
//---------------------------------------------------------------------------
