//---------------------------------------------------------------------------

#ifndef MyStringListH
#define MyStringListH

#include "MyMemo.h"
//---------------------------------------------------------------------------
/*enum TMySaveType
{
    TByMaxLineCount=0,
    TByHour=1,
    TByDay=2,
    TByMonth=3,
    TByYear=4
};*/
//---------------------------------------------------------------------------
class TMyStringList : public TStringList
{
    private:
        AnsiString  HTPath;
        AnsiString  HTFileName;
        AnsiString  HTFirstRow;
        int         HTMaxLineCount;
        TSaveType   HTSaveType;
        bool        HTAutoSave;
        void __fastcall SetPath(AnsiString P);
        void __fastcall SetFirstRow(AnsiString P);
        void __fastcall SetMaxLineCount(int Cnt);
        void __fastcall SetSaveType(TSaveType Type);
        void __fastcall SetFileName(AnsiString P);
        void __fastcall SetAutoSave(bool P);

        Word SystemHour, SystemMin, SystemSec, SystemMSec;
        Word SystemYear, SystemMonth, SystemDate;
        void GetTimeInfo();
    protected:
    public:
        __fastcall TMyStringList();
        __fastcall TMyStringList(AnsiString sPath, AnsiString sFileName, AnsiString sFirstRow);
        __fastcall ~TMyStringList();
        void __fastcall AddText(AnsiString Msg);
        void __fastcall AddTextWithDateTime(AnsiString Msg);
        AnsiString __fastcall AddTextWithDateTime2(AnsiString Msg);
        void MySaveToFile();
        void MyInsertToFile(int iCount);    //Steven 20191016 : 紀錄目前Alarm在檔案裡面的行數
        int GetLastLine();                  //Steven 20191016 : 取得目前檔案的行數
        AnsiString sLastFileName;           //Steven 20191107 : 紀錄現在的檔名
        AnsiString GetFileName();
        bool bFilePathWithDate;
        bool bUseFTRT;
        void MySaveFileByFileNameAndType(AnsiString asFileName, AnsiString asLotId, AnsiString asFileType);  //Sam 20171215 (wei) : 檔名尾巴增加JAM/INPUT //Sam 20170428 (wei) 增加帶入 LotId //RogerYAng 20170406 帶入檔案名稱
        void MySaveFileByFileName(AnsiString asPathName, AnsiString asFileName);   //Sam 20170516 (wei) 力成 SocketID Count 保存
        TStringList *MyList;    //因為建構跟解構時,還沒有畫面,用到VCL得東西會死翹翹,所以用StringList做中介

    __published:
        __property AnsiString   Path        ={read=HTPath,          write=SetPath,          default=NULL};
        __property AnsiString   FileName    ={read=HTFileName,      write=SetFileName,      default=NULL};
        __property AnsiString   FirstRow    ={read=HTFirstRow,      write=SetFirstRow,      default=NULL};
        __property int          MaxLineCount={read=HTMaxLineCount,  write=SetMaxLineCount,  default=1};
        __property TSaveType    SaveType    ={read=HTSaveType,      write=SetSaveType,      default=TByDay};
        __property bool         AutoSave    ={read=HTAutoSave,      write=SetAutoSave,      default=true};
};

#endif
