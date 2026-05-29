//---------------------------------------------------------------------------


#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "cpublic.h"

#include "aseTest.h"
#include "uTimeTool.h"
#define VERSION_INFO_KEY_ROOT TEXT("\\StringFileInfo\\")
#define VERSION_INFO_KEY_TRANS TEXT("\\VarFileInfo\\Translation")   //Sam 20230328 : 改使用更新包的產品版本來判別是否更新。
//==============================================================================
#pragma package(smart_init)
//==============================================================================
bool fMCIDeviceExit=false;
//==============================================================================
void AddCharAtTail(char *S,char c,int len)
{
    bool flag=false;
    int i;//,j;
    for(i=0;i<len;i++)
    {
        if(S[i]=='\0')
        {
            flag=true;
        }
        if( flag )
        {
            S[i]=c;
        }
    }
    S[len]='\0';
}
//==============================================================================
void AddSpace(char *S,int len)
{
    AddCharAtTail(S,' ',len);
}
//==============================================================================
void CutSpace(char *S)
{
    int i=0;
    while(1)
    {
        if( S[i]=='\0' || S[i]==' ')
        {
            S[i]='\0';
            return;
        }
        i++;
    }
}
//==============================================================================
void CutSpaceAtTail(char *S)
{
    int len;
    len=strlen(S);
    while(len)
    {
        if( S[len]==' ')
            S[len]='\0';
        else if( S[len]!='\0')
            return;
        len--;
    }
}
//==============================================================================
void CutSpaceAtHead(char *S)
{
    char str2[256]={""};
    int i=0,pos=0;
    while(1)
    {
        if( S[i]!=' ')
            break;
        i++;
    }
    if(S[i]==0)
        return;
    while(1)
    {
        str2[pos]=S[i];
        if(S[i]==0)
            break;
        pos++;
        i++;
    }
    strcpy(S,str2);
}
//==============================================================================
void FilterErrorAscii(char * str)
{
    for(unsigned int i=0;i<strlen(str);i++)
    {
        if(str[i]=='\0' || str[i]>=100)
        {
            str[i]='\0';
            return;
        }
    }
}
//==============================================================================
int IsAllSpace( char *str)
{
    int flag=1;
    while(1)
    {
        if( *str=='\0')
            return flag;
        if( *str!=' ' )
            return 0;
    }
}
//==============================================================================
char DataToLED2(char data)
{
    char Value=' ';
    switch(data)
    {
        case 0x0e:Value = '0';break;
        case 0x00:Value = '1';break;
        case 0x0a:Value = '2';break;
        case 0x0c:Value = '3';break;
        case 0x01:Value = '4';break;
        case 0x05:Value = '5';break;
        case 0x07:Value = '6';break;
        case 0x08:Value = '7';break;
        case 0x0f:Value = '8';break;
        case 0x0d:Value = '9';break;
    }
    return Value;
}
//==============================================================================
void ConvertSDKString(char *Source,char *Taget)
{
    int i;
    char low,high;
    for(i=0;i<5;i++)
    {
        low=Source[i]&0x0f;
        high=Source[i]>>4;
        Taget[(4-i)*2+1]=DataToLED2( low  &0x0f );
        Taget[(4-i)*2+0]=DataToLED2( high &0x0f );
    }
}
//==============================================================================
bool ReadLineFromFile(FILE *Ptr,char *s,int Max)
{
    int ct=0;
    char c;
    while( !feof(Ptr) )
    {
        c=fgetc(Ptr);
        if(c==0x0d || c=='\n')
        {
            if(c==0x0d)
                c=fgetc(Ptr);
            s[ct]=0;
            return true;
        }
        else
        {
            s[ct++]=c;
            if(ct>Max)
            {
                s[ct]=0;
                return  true;
            }
        }
    }
    return false;
}
//==============================================================================
// 只留  A..Z   a..z   0..9
bool SplitUpperStrBySpace(char *str,char *dest,int Max)
{
    char Buffer[1024];
    int ct1=0,ct2=0,i;

    while(1)         // find first character
    {
        if( str[ct1]=='\x0')
            return false;
        if( (str[ct1] >='0' && str[ct1]<='9') ||
            (str[ct1] >='a' && str[ct1]<='z') ||
            (str[ct1] >='A' && str[ct1]<='Z') )
            break;
        ct1++;
    }
    while(1)
    {
        dest[ct2]=str[ct1];
        ct2++;
        ct1++;
        dest[ct2]='\x0';
        if( (ct2+1 ) >=Max )
            break;
        if( (str[ct1] >='0' && str[ct1]<='9') ||
            (str[ct1] >='a' && str[ct1]<='z') ||
            (str[ct1] >='A' && str[ct1]<='Z') )
        {
        }
        else
        {
            break;
        }
    }
    for(i=0;i<ct2;i++)
    {
        if( dest[i] >='a' && dest[i]<='z')
            dest[i]=dest[i]-'a'+'A';
    }
    ct2=0;
    while(1)
    {
        Buffer[ct2]=str[ct1];
        if( str[ct1]=='\x0')
            break;
        ct1++;
        ct2++;
        Buffer[ct2]='\x0';
        if(ct2>=1023)
            break;
    }
    strcpy(str,Buffer);
    return true;
}
//==============================================================================
int HexStrToInt(char *str )
{
    int i,len,sum=0,Scale=1,j;
    char c;
    if(strlen(str)==0)  return 0;
    CutSpaceAtHead(str);
    CutSpaceAtTail(str);
    strcpy( str,strupr(str) );
    len=strlen(str);
    sum=0;
    for( i=(len-1); i>=0;i--)
    {
        c=str[i];
        if( (c>='0' && c<='9'))
            j=(int)(c-'0');
        else if( (c>='A' && c<='F') )
            j=10+(int)(c-'A');
        else
            break;
        sum+=j*Scale;
        Scale*=16;
    }
    return sum;
}
//==============================================================================
bool IsMCIDeviceExit()
{
    int WaveCT,MidiCT;
    WaveCT=(int)waveOutGetNumDevs();
    MidiCT=(int)midiOutGetNumDevs();
    if( WaveCT && MidiCT)
        fMCIDeviceExit=true;
    else
        fMCIDeviceExit=false;
    return fMCIDeviceExit;
}
//==============================================================================
void SystemShutDown(void)
{
    HANDLE            hToken;
    LUID              takeOwnershipValue;
    TOKEN_PRIVILEGES  tkp;
    if (!OpenProcessToken(GetCurrentProcess(),
                                                      TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        MessageBox(0, "Exit Error", "OpenProcessToken() failed", MB_OK);
        return;
    }
    if (!LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &takeOwnershipValue))
    {
        MessageBox(0, "Exit Error", "LookupPrivilegeValue() failed", MB_OK);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = takeOwnershipValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, false, &tkp, sizeof(TOKEN_PRIVILEGES), 0, 0);
    if (GetLastError())
    {
        MessageBox(0, "Exit Error", "AdjustTokenPrivileges() failed", MB_OK);
        return;
    }
    ExitWindowsEx(EWX_SHUTDOWN,0);
}
//==============================================================================
double GetScale(int s1,int e1,int s2,int e2)
{
    double P,S;
    S=(double)(s2-e2);
    if(S==0)
        return 1.0;
    P=(double)(s1-e1);
    if(S==0)
        ShowRecordMessage("GetScale_Fail");
    return (double) P/S;
}
//==============================================================================
int GetReference(int s1,int e1,int s2,int e2,int p)                             // RefStart=s1;  RefEnd=e1; FactStart=s2;  FactEnd=e2;
{
    //    RefStart=s1;  RefEnd=e1; FactStart=s2;  FactEnd=e2;
    double Scale=GetScale(s1,e1,s2,e2);
    return  (int)(Scale*(p-s2))+s1;
}
//==============================================================================
char *ConvertSecondToTime(long s)
{
    static char str[256];
    long secs, mins, hours, days;
    secs=s%60L;
    mins=s/60L;
    hours=mins/60L;
    mins=mins%60L;
    days=hours/24L;
    hours=hours%24L;

    if(days>0)
        sprintf(str,"%04d days  %02d:%02d:%02d", (int)days, int(hours), int(mins), int(secs));
    else
        sprintf(str,"%02d:%02d:%02d", int(hours), int(mins), int(secs));
    return (char *) &str[0];
}
//==============================================================================
TColor GetDeviceColor(int Type)                                                 // beed edit by user //
{
    if(     Type==NULL_IC)              return  clSilver;
    else if(Type==HAS_IC)               return  clGreen;
    else if(Type==HAS_NULL_IC)          return  clWhite;
    else if(Type==HAS_PASS_IC)          return  clLime;
    else if(Type==HAS_RING_HAS_IC)      return  clGreen;
    else if(Type==HAS_RING_NO_IC)       return  clYellow;
    else if(Type==HAS_FAIL_IC)          return  clRed;
    else                                return  (TColor) 0x00FF0080;
}
//==============================================================================
//void SetCompomentHint(TWinControl *PCtrl)                                       // set panel and sensor show hint //
//{
//    AnsiString S="";
//    char str[256];
//    int i;
//    bool ret;
//
//    for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
//    {
//        TControl *P = PCtrl->Controls[iP];
//        if (dynamic_cast <TPanel *>(P) != NULL          ||
//            dynamic_cast <TPageControl *>(P) != NULL    ||
//            dynamic_cast <TTabSheet *>(P) != NULL       ||
//            dynamic_cast <TTabControl *>(P) != NULL     ||
//            dynamic_cast <TForm *>(P) != NULL           ||
//            dynamic_cast <THeader *>(P) != NULL         ||
//            dynamic_cast <TPage *>(P) != NULL           ||
//            dynamic_cast <TGroupBox*>(P) != NULL        ||
//            dynamic_cast <TScrollBox*>(P) != NULL       ||
//            dynamic_cast <TRadioGroup*>(P) != NULL  )
//        {
//            SetCompomentHint((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
//        }
//        TBtnPanel *PBtnPanel               = dynamic_cast <TBtnPanel *>(P);
//        TMyLed    *PMyLed                  = dynamic_cast <TMyLed *>(P);
//        if (PBtnPanel != NULL)                 // 如果是TLabel型態
//        {
//            //sprintf(str,"( %3X0%d )  %s",PBtnPanel->OutPort,PBtnPanel->OutBit,PBtnPanel->Alias.c_str() );  //2011122 Daver
//            if(HSys.bUseRemoteIO)
//            {
//                sprintf(str,"( %03d%d%d )  %s", PBtnPanel->OutPort/100, PBtnPanel->OutPort%100, PBtnPanel->OutBit, PBtnPanel->Alias.c_str() );
//            }
//            else
//            {
//                sprintf(str,"( %04X%d )  %s",PBtnPanel->OutPort,PBtnPanel->OutBit,PBtnPanel->Alias.c_str() );
//            }
//            PBtnPanel->ShowHint=true;
//            PBtnPanel->Hint=str;
//        }
//        else if( PMyLed!=NULL)
//        {
//            //sprintf(str,"( %3X0%d )  %s",PMyLed->InPort,PMyLed->InBit,PMyLed->Alias.c_str() );            //2011122 Daver
//            if(HSys.bUseRemoteIO)
//            {
//                sprintf(str,"( %03d%d%d )  %s", PMyLed->InPort/100, PMyLed->InPort%100, PMyLed->InBit, PMyLed->Alias.c_str() );
//            }
//            else
//            {
//                sprintf(str,"( %04X%d )  %s",PMyLed->InPort,PMyLed->InBit,PMyLed->Alias.c_str() );
//            }
//            PMyLed->ShowHint=true;
//            PMyLed->Hint=str;
//        }
//    }
//}
//==============================================================================
void ScanLed(TWinControl *PCtrl)
{
    int i;
    //bool flag;
    for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
    {
        TControl *P = PCtrl->Controls[iP];
        if (dynamic_cast <TPanel *>(P) != NULL          ||
            dynamic_cast <TPageControl *>(P) != NULL    ||
            dynamic_cast <TTabSheet *>(P) != NULL       ||
            dynamic_cast <TTabControl *>(P) != NULL     ||
            dynamic_cast <TForm *>(P) != NULL           ||
            dynamic_cast <THeader *>(P) != NULL         ||
            dynamic_cast <TPage *>(P) != NULL           ||
            dynamic_cast <TGroupBox*>(P) != NULL        ||
            dynamic_cast <TScrollBox*>(P) != NULL       ||
            dynamic_cast <TRadioGroup*>(P) != NULL  )
        {
            ScanLed((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
        }
        TMyLed    *PMyLed                  = dynamic_cast <TMyLed *>(P);
        /*
        if( PMyLed!=NULL )                                                      //V1.1
        {
            //flag=IOInputBit(PMyLed->InPort,PMyLed->InBit);
                if(PMyLed->InType==0)
                    flag=!flag;
                if(flag)
                    PMyLed->Value=true;
                else
                    PMyLed->Value=false;
        }
        */
        if( PMyLed!=NULL )                                                      //V1.2
        {
            for(i=0;i<HSys.iTotalSensor;i++)
            {
                if(PMyLed->Alias==HSys.SenPtr[i].Name)
                    PMyLed->Value=HSys.SenPtr[i].IsOn();
            }

            for(i=0;i<HSys.iTotalCylinder;i++)
            {
                if(PMyLed->Alias==HSys.CynPtr[i].OnSensor.Name)
                    PMyLed->Value=HSys.CynPtr[i].OnSensor.IsOn();
                else if(PMyLed->Alias==HSys.CynPtr[i].OffSensor.Name)
                    PMyLed->Value=HSys.CynPtr[i].OffSensor.IsOn();
            }
        }
    }
}
//==============================================================================
bool ShellExecuteProgram( char *str)
{
    SHELLEXECUTEINFO exeinfo;
    memset(&exeinfo,0,sizeof(exeinfo) );
    exeinfo.cbSize=sizeof(exeinfo);
    exeinfo.lpVerb="open";
    exeinfo.lpFile=str;
    exeinfo.fMask=SEE_MASK_NOCLOSEPROCESS;
    exeinfo.nShow=SW_SHOWDEFAULT;
    ShellExecuteEx(&exeinfo);
    return true;
}
//==============================================================================
void ConvertGearValue(int *Ref, double GearRatio)
{
    int i;
    if( GearRatio >20 )
        return;
    i=*Ref;
    i=i/GearRatio;
    i*=GearRatio;
    *Ref=i;
}
//==============================================================================
char *GetFloatFormatString(double v, int P1, int P2)
{
    static char str[256];
    char fstr[256]={"%5.3f"};
    fstr[1]='0'+(P1+P2);
    fstr[3]='0'+P2;
    sprintf(str,fstr,v);

    CutSpaceAtHead(str);

    return str;
}
//==============================================================================
char *GetIntFormatString(int v,int P1)
{
    static char str[256];
    char fstr[256]={"%5d"};
    fstr[1]='0'+P1;
    sprintf(str,fstr,v);
    return str;
}
//==============================================================================
bool SplitStrBySpaceOnly(char *str,char *dest,int Max)
{
    char Buffer[10240];
    int ct1=0,ct2=0;//,i;

    while(1)         // find first character
    {
        if( str[ct1]=='\x0')
            return false;
        if( (str[ct1] !=' ' && str[ct1]!='\t' && str[ct1]!='\r') )
            break;
        ct1++;
    }
    while(1)
    {
        dest[ct2]=str[ct1];
        ct2++;
        ct1++;
        dest[ct2]='\x0';
        if( (ct2+1 ) >=Max ) break;

        if( str[ct1] !=' ' && str[ct1]!='\t' && str[ct1]!='\0' && str[ct1]!='\r')
        {
        }
        else
        {
            break;
        }
    }
    ct2=0;
    while(1)
    {
        Buffer[ct2]=str[ct1];
        if( str[ct1]=='\x0')
            break;
        ct1++;
        ct2++;
        Buffer[ct2]='\x0';
        if(ct2>=10240)
            break;
    }
    strcpy(str,Buffer);
    return true;
}
//==============================================================================
void ReplaceCharExceptAlpha(char *str)
{
    for(unsigned int i=0;i<strlen(str);i++)
    {
        if(str[i]<'0' || str[i]>'9')
            str[i]='_';
    }
}
//==============================================================================
char *ConvertToMMType(int i)
{
    return GetFloatFormatString( (double) i/100.0,4,2);
}
//==============================================================================
int Get0_01MMType(char *str)
{
    double i=atof(str);
    if(i>=0.0)
        return (i*100.0+0.5);
    else
        return (i*100.0-0.5);
}
//==============================================================================
int Get0_01MMType(double f)                                                      // ben add 20110802 //
{
    if(f>=0.0)
        return (f*100.0+0.5);
    else
        return (f*100.0-0.5);
}
//==============================================================================
int Get0_01MMTypeLimit(char *str,int Limit)
{
    int v;
    double i=atof(str);
    if(i>=0.0)
        v=(i*100.0+0.5);
    else
        v=(i*100.0-0.5);
    if(v>Limit) v=Limit;
    if(v<-Limit) v=-Limit;
    return v;
}
//==============================================================================
char *ConvertTouMType(int i)
{
    return GetFloatFormatString( (double) i/1000.0,6,3);
}
//==============================================================================
int Get0_001MMType(char *str)
{
    double i=atof(str);
    if(i>=0)
        return i*1000.0+0.5;
    else
        return (i*1000.0-0.5);
}
//==============================================================================
// V2.1 START
//==============================================================================
int CheckRange(int &V,int Max,int Min,int Default)
{
    if(V>Max || V<Min)
        V=Default;
    return V;
}
//==============================================================================
// V2.1 END
//==============================================================================
void GetTimeInfo()
{
    static TDateTime dtPresent;
    dtPresent= Now();
    DecodeDate(dtPresent, SystemYear, SystemMonth, SystemDate);
    DecodeTime(dtPresent,SystemHour,SystemMin,SystemSec,SystemMSec);
}
//---------------------------------------------------------------------------
void GetYesterdayInfo()
{
    static TDateTime dtPresent;
    dtPresent=Now()-1;
    DecodeDate(dtPresent, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);
}
//==============================================================================
bool SplitStrByDotSpaceOnly(char *str,char *dest,int Max)
{
    char Buffer[10240];
    int ct1=0,ct2=0;//,i;

    while(1)         // find first character
    {
        if( str[ct1]=='\x0')
            return false;
        if( (str[ct1] !=' ' && str[ct1] !=','&& str[ct1]!='\t' && str[ct1]!='\r' && str[ct1] !=':') )
            break;
        ct1++;
    }
    while(1)
    {
        dest[ct2]=str[ct1];
        ct2++;
        ct1++;
        dest[ct2]='\x0';
        if( (ct2+1 ) >=Max ) break;

        if( str[ct1] !=' '&& str[ct1] !=',' && str[ct1]!='\t' && str[ct1]!='\0' && str[ct1]!='\r' && str[ct1]!=':')
        {
        }
        else
        {
            break;
        }
    }
    ct2=0;
    while(1)
    {
        Buffer[ct2]=str[ct1];
        if( str[ct1]=='\x0')
            break;
        ct1++;
        ct2++;
        Buffer[ct2]='\x0';
        if(ct2>=10240)
            break;
    }
    strcpy(str,Buffer);
    return true;
}
//==============================================================================
void ConvertTimeFormat(char *s,int &H,int &M,int &S)
{
    char str2[256];
    char str[256];
    strcpy(str,s);
    SplitStrByDotSpaceOnly(str,str2,20);
    H=atoi(str2);
    SplitStrByDotSpaceOnly(str,str2,20);
    M=atoi(str2);
    SplitStrByDotSpaceOnly(str,str2,20);
    S=atoi(str2);
}
//==============================================================================
void Swap(AnsiString &S1,AnsiString &S2)
{
    AnsiString S;
    S=S1;
    S1=S2;
    S2=S;
}
//==============================================================================
long GetTotalSecond(AnsiString Str)
{
    int H,M,S;
    ConvertTimeFormat(Str.c_str(),H,M,S);
    return (long)H*60L*60L+(long)M*60L+(long) S;
}
//---------------------------------------------------------------------------
// convert .DB (paradox) to text file
//---------------------------------------------------------------------------
bool DBFileToTxtFile(AnsiString dbname,AnsiString txtname)
{
    TTable *tb;
    TStringList *sl;
    int i,linesize=0,k,min_k=16;
    AnsiString fmt,s;

    try
    {
        tb=new TTable(NULL);
        sl=new TStringList;
        tb->TableName=dbname;
        try
        {
            tb->Open();
            tb->First();

            s="";
            for(i=0; i<tb->Fields->Count; i++)
            {
                k=tb->Fields->Fields[i]->DataSize;
                if( k<min_k ) k=min_k;
                fmt="%-"+AnsiString(k)+"s ";
                s.cat_printf(fmt.c_str(), tb->Fields->Fields[i]->FieldName.c_str() );
                linesize+=k+3;
            }
            sl->Add(s);
            sl->Add( AnsiString::StringOfChar('=', linesize) );

            while( !tb->Eof )
            {
                s="";
                for(i=0; i<tb->Fields->Count; i++)
                {
                    k=tb->Fields->Fields[i]->DataSize;
                    if( k<min_k ) k=min_k;
                    fmt="%-"+AnsiString(k)+"s ";
                    s.cat_printf(fmt.c_str(), tb->Fields->Fields[i]->AsString.c_str() );
                }
                sl->Add(s);
                tb->Next();
            }

            sl->SaveToFile(txtname);
            tb->Close();
        }
        catch(...)
        {
            return false;
        }
    }
    __finally
    {
        delete tb;
        delete sl;
    }
    return true;
}
//==============================================================================
// input last start time,and return pass time
// format : HH:MM:SS
// and reset last start time
//==============================================================================
AnsiString SetPassTimeString(DWORD &StartTimeCount)
{
    DWORD PT;
    AnsiString S;
    int iSec,iMin,iHour;
    PT=GetTickCount()-StartTimeCount;
    StartTimeCount=GetTickCount();
    PT/=1000;
    iSec=PT%60;
    PT/=60;
    iMin=PT%60;
    PT/=60;
    iHour=PT;
    S=S.sprintf("%d:%02d:%02d",iHour,iMin,iSec);
    return S;
}
//==============================================================================
int GetScreenPos(int NowPos,int ScreenTop,int ScreenBottom,int RealTop,int RealBottom)
{
    int ScreenPos;
//    ScreenPos    - ScreenTop     NowPos     -RealTop
//    ----------------------    = -----------------------
//    ScreenBottom - ScreenTop     RealBottom -RealTop
//
//           NowPos     -RealTop
//    Scale= -----------------------
//           RealBottom -RealTop
//
//    ScreenPos=(ScreenBottom - ScreenTop)*Scale+ScreenTop;
    double Scale;
    if( (RealBottom -RealTop)==0 )
        return 0;
    Scale=(double)(NowPos-RealTop)/(double)(RealBottom -RealTop);
    ScreenPos=(double)(ScreenBottom - ScreenTop)*Scale+ScreenTop;
    return ScreenPos;
}
//==============================================================================
extern HonPrecFunctionCollection HFC;
//==============================================================================
bool HonPrecFunctionCollection::CheckFileExist(char *cFName)
{
    return CheckFileExist(cFName);
};
//==============================================================================
void ShowRecordMessage(char *S)
{
    ShowMessage((AnsiString) S);
    RecordProcess(S);
}
//==============================================================================
void ShowRecordMessage(AnsiString S)
{
    ShowMessage(S);
    RecordProcess(S);
}
//==============================================================================
bool GetCOMPortStatus(int i)
{
    HANDLE h=INVALID_HANDLE_VALUE;
    AnsiString CN="\\\\.\\COM";
    CN+=AnsiString(i);
    h=::CreateFile(CN.c_str(),
        GENERIC_READ|GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);
    CloseHandle(h);
    if(h==INVALID_HANDLE_VALUE)
        return false;
    return true;
}
//---------------------------------------------------------------------------
bool GetCOMPortStatus(AnsiString Com)    //Steven 20120217 : Com Port改成可定義
{
    AnsiString asString;
    if(Com.Length()==4)
        asString=Com.SubString(4,1);
    else
        asString=Com.SubString(4,2);

    HANDLE h=INVALID_HANDLE_VALUE;
    AnsiString CN="";
    CN=Com;
    h=::CreateFile(CN.c_str(),
        GENERIC_READ|GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        0);

    if(h==INVALID_HANDLE_VALUE)
    {
        CN="\\\\.\\"+Com;
        h=::CreateFile(CN.c_str(),
            GENERIC_READ|GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            0);

        if(h==INVALID_HANDLE_VALUE)
        {
            return false;
        }
        else
        {
            CloseHandle(h);
            return true;
        }
    }
    CloseHandle(h);
    return true;
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
bool __fastcall CheckSectionExist(AnsiString FileName, AnsiString Group)
{
    if(Group=="") return false;

    TIniFile *INIFile = new TIniFile(FileName);
    bool  ret=INIFile->SectionExists(Group);
    delete INIFile;
    return ret;
}
//---------------------------------------------------------------------------
bool __fastcall CheckIniData(AnsiString FileName, AnsiString Group, AnsiString Name)
{
    bool bTemp;
    TIniFile *INIFile=new TIniFile(FileName);
    bTemp = INIFile->ValueExists(Group, Name);
    delete INIFile;
    return bTemp;
}
//---------------------------------------------------------------------------
double __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)
{
    TIniFile *INIFile=new TIniFile(FileName);
    if(!INIFile->ValueExists(Group, Name))  INIFile->WriteFloat(Group, Name, Value);
    else                                    Value=INIFile->ReadFloat(Group, Name, Value);
    delete INIFile;
    return Value;
}
//---------------------------------------------------------------------------
int __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)
{
    TIniFile *INIFile=new TIniFile(FileName);
    if(!INIFile->ValueExists(Group, Name))  INIFile->WriteInteger(Group, Name, Value);
    else                                    Value=INIFile->ReadInteger(Group, Name, Value);
    delete INIFile;
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool Value)
{
    TIniFile *INIFile=new TIniFile(FileName);
    if(!INIFile->ValueExists(Group, Name))  INIFile->WriteBool(Group, Name, Value);
    else                                    Value=INIFile->ReadBool(Group, Name, Value);
    delete INIFile;
    return Value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)
{
    TIniFile *INIFile = new TIniFile(FileName);
    if(!INIFile->ValueExists(Group, Name))  INIFile->WriteString(Group, Name, Value);
    else                                    Value=INIFile->ReadString(Group, Name, Value);
    delete INIFile;
    return Value;
}
//---------------------------------------------------------------------------
TDateTime __fastcall CheckAndReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value)
{
    TIniFile *INIFile = new TIniFile(FileName);
    if(!INIFile->ValueExists(Group, Name))  INIFile->WriteDateTime(Group, Name, Value);
    else                                    Value=INIFile->ReadDateTime(Group, Name, Value);
    delete INIFile;
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)
{
    bool ret;
    TIniFile *INIFile = new TIniFile(FileName);
    ret=INIFile->ReadBool(Group, Name, bValue);
    delete INIFile;
    return ret;
}
//---------------------------------------------------------------------------
double __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)
{
    double ret;
    TIniFile *INIFile = new TIniFile(FileName);
    ret=INIFile->ReadFloat(Group, Name, Value);
    delete INIFile;
    return ret;
}
//---------------------------------------------------------------------------
int __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)
{
    int ret;
    TIniFile *INIFile = new TIniFile(FileName);
    ret=INIFile->ReadInteger(Group, Name, Value);
    delete INIFile;
    return ret;
}
//---------------------------------------------------------------------------
AnsiString __fastcall ReadIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value)
{
    AnsiString ret;
    TIniFile *INIFile = new TIniFile(FileName);
    ret=INIFile->ReadString(Group, Name, Value);
    delete INIFile;
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, bool bValue)  //Steven 20090731
{
    TIniFile *INIFile = new TIniFile(FileName);
    INIFile->WriteBool(Group, Name, bValue);
    delete INIFile;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, int Value)  //Steven 20090731
{
    TIniFile *INIFile = new TIniFile(FileName);
    INIFile->WriteInteger(Group, Name, Value);
    delete INIFile;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, double Value)  //Steven 20090731
{
    TIniFile *INIFile = new TIniFile(FileName);
    INIFile->WriteFloat(Group, Name, Value);
    delete INIFile;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, AnsiString Value) //Steven 20090731
{
    TIniFile *INIFile = new TIniFile(FileName);
    INIFile->WriteString(Group, Name, Value);
    delete INIFile;
}
//---------------------------------------------------------------------------
void __fastcall WriteIniData(AnsiString FileName, AnsiString Group, AnsiString Name, TDateTime Value)
{
    TIniFile *INIFile = new TIniFile(FileName);
    INIFile->WriteDateTime(Group, Name, Value);
    delete INIFile;
}
//---------------------------------------------------------------------------
void __fastcall ReplaceIniData(AnsiString FileName, AnsiString Group, AnsiString Name1, AnsiString Name2) //Steven 20090731
{
    TIniFile *INIFile = new TIniFile(FileName);
    AnsiString  ret=INIFile->ReadString(Group, Name2, " ");
    INIFile->WriteString(Group, Name1, ret);
    delete INIFile;
}
//---------------------------------------------------------------------------
bool CheckDataIsNum(AnsiString asS, bool bHasDot)
{
    bool bIsOK=false;

    if (asS=="")
        return bIsOK;

    for (int iDataID=1;iDataID<=asS.Length();iDataID++)
    {
        bIsOK=false;
        if (asS.SubString(iDataID,1)=="0")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="1")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="2")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="3")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="4")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="5")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="6")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="7")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="8")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="9")
            bIsOK=true;
        if (asS.SubString(iDataID,1)=="-")
            bIsOK=true;
        if (bHasDot)
            if (asS.SubString(iDataID,1)==".")
                bIsOK=true;
        if (!bIsOK)
             break;
    }

    return bIsOK;
}
//---------------------------------------------------------------------------
void UDPErrorLog(AnsiString aTitle, AnsiString Command)                         //kevin 20211020 UDP error log
{
    AnsiString sFileName="", sMegTime="", asLog, asUDPLogPath="D:\\HT-172_Log\\UDP";
    GetTimeInfo();
    sFileName.sprintf("%s\\%04d\\%02d", asUDPLogPath, SystemYear, SystemMonth);//, SystemDate);
    MyForceDirectories(sFileName);
    sFileName.sprintf("%s\\%04d\\%02d\\%02d%02d%02d%02d.txt", asUDPLogPath, SystemYear, SystemMonth,SystemYear, SystemMonth, SystemDate, SystemHour);
    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
    asLog.sprintf("%s,   %s, %s", sMegTime, aTitle, Command);
    WriteDataToFile(sFileName.c_str(), asLog.c_str());
}
//---------------------------------------------------------------------------
bool RespondASECom(AnsiString S1)
{
    ASESendMessage->SendToASEData(S1);//kevin 20150415 回應 ase Reset finish
    return true;
}
//---------------------------------------------------------------------------
AnsiString GetSocketErrorMsg(int ErrorCode)
{
    AnsiString asResultStr="";
    switch(ErrorCode)
    {
        case 6://WSA_INVALID_HANDLE:
            asResultStr ="Socket Error (6) Specified event object handle is invalid. An application attempts to use an event object, but the specified handle is not valid. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 8://WSA_NOT_ENOUGH_MEMORY:
            asResultStr ="Socket Error (8) Insufficient memory available. An application used a Windows Sockets function that directly maps to a Windows function. The Windows function is indicating a lack of required memory resources. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 87://WSA_INVALID_PARAMETER:
            asResultStr ="Socket Error (87) One or more parameters are invalid. An application used a Windows Sockets function which directly maps to a Windows function. The Windows function is indicating a problem with one or more parameters. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 995://WSA_OPERATION_ABORTED:
            asResultStr ="Socket Error (995) Overlapped operation aborted. An overlapped operation was canceled due to the closure of the socket, or the execution of the SIO_FLUSH command in WSAIoctl. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 996://WSA_IO_INCOMPLETE:
            asResultStr ="Socket Error (996) Overlapped I/O event object not in signaled state. The application has tried to determine the status of an overlapped operation which is not yet completed. Applications that use WSAGetOverlappedResult (with the fWait flag set to FALSE) in a polling mode to determine when an overlapped operation has completed, get this error code until the operation is complete. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 997://WSA_IO_INCOMPLETE:
            asResultStr ="Socket Error (997) Overlapped operations will complete later. The application has initiated an overlapped operation that cannot be completed immediately. A completion indication will be given later when the operation has been completed. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case WSAEINTR:
            asResultStr = "Socket Error (10004) Interrupted function call. A blocking operation was interrupted by a call to WSACancelBlockingCall.";
            break;
        case WSAEBADF:
            asResultStr = "Socket Error (10009) File handle is not valid. The file handle supplied is not valid.";
            break;
        case WSAEACCES:
            asResultStr = "Socket Error (10013) Permission denied. An attempt was made to access a socket in a way forbidden by its access permissions. An example is using a broadcast address for sendto without broadcast permission being set using setsockopt(SO_BROADCAST). Another possible reason for the WSAEACCES error is that when the bind function is called (on Windows NT 4.0 with SP4 and later), another application, service, or kernel mode driver is bound to the same address with exclusive access. Such exclusive access is a new feature of Windows NT 4.0 with SP4 and later, and is implemented by using the SO_EXCLUSIVEADDRUSE option.";
            break;
        case WSAEFAULT:
            asResultStr = "Socket Error (10014) Bad address. The system detected an invalid pointer address in attempting to use a pointer argument of a call. This error occurs if an application passes an invalid pointer value, or if the length of the buffer is too small. For instance, if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr).";
            break;
        case WSAEINVAL:
            asResultStr = "Socket Error (10022) Invalid argument. Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function). In some instances, it also refers to the current state of the socket—for instance, calling accept on a socket that is not listening.";
            break;
        case WSAEMFILE:
            asResultStr = "Socket Error (10024) Too many open files. Too many open sockets. Each implementation may have a maximum number of socket handles available, either globally, per process, or per thread.";
            break;
        case WSAEWOULDBLOCK:
            asResultStr = "Socket Error (10035) Resource temporarily unavailable. This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operation should be retried later. It is normal for WSAEWOULDBLOCK to be reported as the result from calling connect on a nonblocking SOCK_STREAM socket, since some time must elapse for the connection to be established.";
            break;
        case WSAEINPROGRESS:
            asResultStr = "Socket Error (10036) Operation now in progress. A blocking operation is currently executing. Windows Sockets only allows a single blocking operation—per- task or thread—to be outstanding, and if any other function call is made (whether or not it references that or any other socket) the function fails with the WSAEINPROGRESS error.";
            break;
        case WSAEALREADY:
            asResultStr = "Socket Error (10037) Operation already in progress. An operation was attempted on a nonblocking socket with an operation already in progress—that is, calling connect a second time on a nonblocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already been canceled or completed.";
            break;
        case WSAENOTSOCK:
            asResultStr = "Socket Error (10038) Socket operation on nonsocket. An operation was attempted on something that is not a socket. Either the socket handle parameter did not reference a valid socket, or for select, a member of an fd_set was not valid.";
            break;
        case WSAEDESTADDRREQ:
            asResultStr = "Socket Error (10039) Destination address required. A required address was omitted from an operation on a socket. For example, this error is returned if sendto is called with the remote address of ADDR_ANY.";
            break;
        case WSAEMSGSIZE:
            asResultStr = "Socket Error (10040) Message too long. A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram was smaller than the datagram itself.";
            break;
        case WSAEPROTOTYPE:
            asResultStr = "Socket Error (10041) Protocol wrong type for socket. A protocol was specified in the socket function call that does not support the semantics of the socket type requested. For example, the ARPA Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM.";
            break;
        case WSAENOPROTOOPT:
            asResultStr = "Socket Error (10042) Bad protocol option. An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call.";
            break;
        case WSAEPROTONOSUPPORT:
            asResultStr = "Socket Error (10043) Protocol not supported. The requested protocol has not been configured into the system, or no implementation for it exists. For example, a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol.";
            break;
        case WSAESOCKTNOSUPPORT:
            asResultStr = "Socket Error (10044) Socket type not supported. The support for the specified socket type does not exist in this address family. For example, the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all.";
            break;
        case WSAEOPNOTSUPP:
            asResultStr = "Socket Error (10045) Operation not supported. The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptor to a socket that cannot support this operation is trying to accept a connection on a datagram socket.";
            break;
        case WSAEPFNOSUPPORT:
            asResultStr = "Socket Error (10046) Protocol family not supported. The protocol family has not been configured into the system or no implementation for it exists. This message has a slightly different meaning from WSAEAFNOSUPPORT. However, it is interchangeable in most cases, and all Windows Sockets functions that return one of these messages also specify WSAEAFNOSUPPORT.";
            break;
        case WSAEAFNOSUPPORT:
            asResultStr = "Socket Error (10047) Address family not supported by protocol family. An address incompatible with the requested protocol was used. All sockets are created with an associated address family (that is, AF_INET for Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). This error is returned if an incorrect protocol is explicitly requested in the socket call, or if an address of the wrong family is used for a socket, for example, in sendto.";
            break;
        case WSAEADDRINUSE:
            asResultStr = "Socket Error (10048) Address already in use. Typically, only one usage of each socket address (protocol/IP address/port) is permitted. This error occurs if an application attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socket that was not closed properly, or one that is still in the process of closing. For server applications that need to bind multiple sockets to the same port number, consider using setsockopt (SO_REUSEADDR). Client applications usually need not call bind at all—connect chooses an unused port automatically. When bind is called with a wildcard address (involving ADDR_ANY), a WSAEADDRINUSE error could be delayed until the specific address is committed. This could happen with a call to another function later, including connect, listen, WSAConnect, or WSAJoinLeaf.";
            break;
        case WSAEADDRNOTAVAIL:
            asResultStr = "Socket Error (10049) Cannot assign requested address. The requested address is not valid in its context. This normally results from an attempt to bind to an address that is not valid for the local computer. This can also result from connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for a remote computer (for example, address or port 0).";
            break;
        case WSAENETDOWN:
            asResultStr = "Socket Error (10050) Network is down. A socket operation encountered a dead network. This could indicate a serious failure of the network system (that is, the protocol stack that the Windows Sockets DLL runs over), the network interface, or the local network itself.";
            break;
        case WSAENETUNREACH:
            asResultStr = "Socket Error (10051) Network is unreachable. A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host.";
            break;
        case WSAENETRESET:
            asResultStr = "Socket Error (10052) Network dropped connection on reset. The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. It can also be returned by setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that has already failed.";
            break;
        case WSAECONNABORTED:
            asResultStr = "Socket Error (10053) Software caused connection abort. An established connection was aborted by the software in your host computer, possibly due to a data transmission time-out or protocol error.";
            break;
        case WSAECONNRESET:
            asResultStr = "Socket Error (10054) Connection reset by peer. An existing connection was forcibly closed by the remote host. This normally results if the peer application on the remote host is suddenly stopped, the host is rebooted, the host or remote network interface is disabled, or the remote host uses a hard close (see setsockopt for more information on the SO_LINGER option on the remote socket). This error may also result if a connection was broken due to keep-alive activity detecting a failure while one or more operations are in progress. Operations that were in progress fail with WSAENETRESET. Subsequent operations fail with WSAECONNRESET.";
            break;
        case WSAENOBUFS:
            asResultStr = "Socket Error (10055) No buffer space available. An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.";
            break;
        case WSAEISCONN:
            asResultStr = "Socket Error (10056) Socket is already connected. A connect request was made on an already-connected socket. Some implementations also return this error if sendto is called on a connected SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter in sendto is ignored) although other implementations treat this as a legal occurrence.";
            break;
        case WSAENOTCONN:
            asResultStr = "Socket Error (10057) Socket is not connected. A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using sendto) no address was supplied. Any other type of operation might also return this error—for example, setsockopt setting SO_KEEPALIVE if the connection has been reset.";
            break;
        case WSAESHUTDOWN:
            asResultStr = "Socket Error (10058) Cannot send after socket shutdown. A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. By calling shutdown a partial close of a socket is requested, which is a signal that sending or receiving, or both have been discontinued.";
            break;
        case WSAETOOMANYREFS:
            asResultStr = "Socket Error (10059) Too many references. Too many references to some kernel object.";
            break;
        case WSAETIMEDOUT:
            asResultStr = "Socket Error (10060) Connection timed out. A connection attempt failed because the connected party did not properly respond after a period of time, or the established connection failed because the connected host has failed to respond.";
            break;
        case WSAECONNREFUSED:
            asResultStr = "Socket Error (10061) Connection refused. No connection could be made because the target computer actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host—that is, one with no server application running.";
            break;
        case WSAELOOP:
            asResultStr = "Socket Error (10062) Cannot translate name. Cannot translate a name.";
            break;
        case WSAENAMETOOLONG:
            asResultStr = "Socket Error (10063) Name too long. A name component or a name was too long.";
            break;
        case WSAEHOSTDOWN:
            asResultStr = "Socket Error (10064) Host is down. A socket operation failed because the destination host is down. A socket operation encountered a dead host. Networking activity on the local host has not been initiated. These conditions are more likely to be indicated by the error WSAETIMEDOUT.";
            break;
        case WSAEHOSTUNREACH:
            asResultStr = "Socket Error (10065) No route to host. A socket operation was attempted to an unreachable host. See WSAENETUNREACH.";
            break;
        case WSAENOTEMPTY:
            asResultStr = "Socket Error (10066) Directory not empty. Cannot remove a directory that is not empty.";
            break;
        case WSAEPROCLIM:
            asResultStr = "Socket Error (10067) Too many processes. A Windows Sockets implementation may have a limit on the number of applications that can use it simultaneously. WSAStartup may fail with this error if the limit has been reached.";
            break;
        case WSAEUSERS:
            asResultStr = "Socket Error (10068) User quota exceeded. Ran out of user quota.";
            break;
        case WSAEDQUOT:
            asResultStr = "Socket Error (10069) Disk quota exceeded. Ran out of disk quota.";
            break;
        case WSAESTALE:
            asResultStr = "Socket Error (10070) Stale file handle reference. The file handle reference is no longer available.";
            break;
        case WSAEREMOTE:
            asResultStr = "Socket Error (10071) Item is remote. The item is not available locally.";
            break;
        case WSASYSNOTREADY:
            asResultStr = "Socket Error (10091) Network subsystem is unavailable. This error is returned by WSAStartup if the Windows Sockets implementation cannot function at this time because the underlying system it uses to provide network services is currently unavailable. Users should check: That the appropriate Windows Sockets DLL file is in the current path. That they are not trying to use more than one Windows Sockets implementation simultaneously. If there is more than one Winsock DLL on your system, be sure the first one in the path is appropriate for the network subsystem currently loaded. The Windows Sockets implementation documentation to be sure all necessary components are currently installed and configured correctly.";
            break;
        case WSAVERNOTSUPPORTED:
            asResultStr = "Socket Error (10092) Winsock.dll version out of range. The current Windows Sockets implementation does not support the Windows Sockets specification version requested by the application. Check that no old Windows Sockets DLL files are being accessed.";
            break;
        case WSANOTINITIALISED:
            asResultStr = "Socket Error (10093) Successful WSAStartup not yet performed. Either the application has not called WSAStartup or WSAStartup failed. The application may be accessing a socket that the current active task does not own (that is, trying to share a socket between tasks), or WSACleanup has been called too many times.";
            break;
        case WSAEDISCON:
            asResultStr = "Socket Error (10101) Graceful shutdown in progress. Returned by WSARecv and WSARecvFrom to indicate that the remote party has initiated a graceful shutdown sequence.";
            break;
        case WSAENOMORE:
            asResultStr = "Socket Error (10102) No more results. No more results can be returned by the WSALookupServiceNext function.";
            break;
        case WSAECANCELLED:
            asResultStr = "Socket Error (10103) Call has been canceled. A call to the WSALookupServiceEnd function was made while this call was still processing. The call has been canceled.";
            break;
        case WSAEINVALIDPROCTABLE:
            asResultStr = "Socket Error (10104) Procedure call table is invalid. The service provider procedure call table is invalid. A service provider returned a bogus procedure table to Ws2_32.dll. This is usually caused by one or more of the function pointers being NULL.";
            break;
        case WSAEINVALIDPROVIDER:
            asResultStr = "Socket Error (10105) Service provider is invalid. The requested service provider is invalid. This error is returned by the WSCGetProviderInfo and WSCGetProviderInfo32 functions if the protocol entry specified could not be found. This error is also returned if the service provider returned a version number other than 2.0.";
            break;
        case WSAEPROVIDERFAILEDINIT:
            asResultStr = "Socket Error (10106) Service provider failed to initialize. The requested service provider could not be loaded or initialized. This error is returned if either a service provider's DLL could not be loaded (LoadLibrary failed) or the provider's WSPStartup or NSPStartup function failed.";
            break;
        case WSASYSCALLFAILURE:
            asResultStr = "Socket Error (10107) System call failure. A system call that should never fail has failed. This is a generic error code, returned under various conditions. Returned when a system call that should never fail does fail. For example, if a call to WaitForMultipleEvents fails or one of the registry functions fails trying to manipulate the protocol/namespace catalogs. Returned when a provider does not return SUCCESS and does not provide an extended error code. Can indicate a service provider implementation error.";
            break;
        case WSASERVICE_NOT_FOUND:
            asResultStr = "Socket Error (10108) Service not found. No such service is known. The service cannot be found in the specified name space.";
            break;
        case WSATYPE_NOT_FOUND:
            asResultStr = "Socket Error (10109) Class type not found. The specified class was not found.";
            break;
        case WSA_E_NO_MORE:
            asResultStr = "Socket Error (10110) No more results. No more results can be returned by the WSALookupServiceNext function.";
            break;
        case WSA_E_CANCELLED:
            asResultStr = "Socket Error (10111) Call was canceled. A call to the WSALookupServiceEnd function was made while this call was still processing. The call has been canceled.";
            break;
        case WSAEREFUSED:
            asResultStr = "Socket Error (10112) Database query was refused. A database query failed because it was actively refused.";
            break;
        case WSAHOST_NOT_FOUND:
            asResultStr = "Socket Error (11001) Host not found. No such host is known. The name is not an official host name or alias, or it cannot be found in the database(s) being queried. This error may also be returned for protocol and service queries, and means that the specified name could not be found in the relevant database.";
            break;
        case WSATRY_AGAIN:
            asResultStr = "Socket Error (11002) Nonauthoritative host not found. This is usually a temporary error during host name resolution and means that the local server did not receive a response from an authoritative server. A retry at some time later may be successful.";
            break;
        case WSANO_RECOVERY:
            asResultStr = "Socket Error (11003) This is a nonrecoverable error. This indicates that some sort of nonrecoverable error occurred during a database lookup. This may be because the database files (for example, BSD-compatible HOSTS, SERVICES, or PROTOCOLS files) could not be found, or a DNS request was returned by the server with a severe error.";
            break;
        case WSANO_DATA:
            asResultStr = "Socket Error (11004) Valid name, no data record of requested type. The requested name is valid and was found in the database, but it does not have the correct associated data being resolved for. The usual example for this is a host name-to-address translation attempt (using gethostbyname or WSAAsyncGetHostByName) which uses the DNS (Domain Name Server). An MX record is returned but no A record—indicating the host itself exists, but is not directly reachable.";
            break;
        case WSA_QOS_RECEIVERS:
            asResultStr = "Socket Error (11005) QoS receivers. At least one QoS reserve has arrived.";
            break;
        case WSA_QOS_SENDERS:
            asResultStr = "Socket Error (11006) QoS senders. At least one QoS send path has arrived.";
            break;
        case WSA_QOS_NO_SENDERS:
            asResultStr = "Socket Error (11007) No QoS senders. There are no QoS senders.";
            break;
        case WSA_QOS_NO_RECEIVERS:
            asResultStr = "Socket Error (11008) QoS no receivers. There are no QoS receivers.";
            break;
        case WSA_QOS_REQUEST_CONFIRMED:
            asResultStr = "Socket Error (11009) QoS request confirmed. The QoS reserve request has been confirmed.";
            break;
        case WSA_QOS_ADMISSION_FAILURE:
            asResultStr = "Socket Error (11010) QoS admission error. A QoS error occurred due to lack of resources.";
            break;
        case WSA_QOS_POLICY_FAILURE:
            asResultStr = "Socket Error (11011) QoS policy failure. The QoS request was rejected because the policy system couldn't allocate the requested resource within the existing policy.";
            break;
        case WSA_QOS_BAD_STYLE:
            asResultStr = "Socket Error (11012) QoS bad style. An unknown or conflicting QoS style was encountered.";
            break;
        case WSA_QOS_BAD_OBJECT:
            asResultStr = "Socket Error (11013) QoS bad object. A problem was encountered with some part of the filterspec or the provider-specific buffer in general.";
            break;
        case WSA_QOS_TRAFFIC_CTRL_ERROR:
            asResultStr = "Socket Error (11014) QoS traffic control error. An error with the underlying traffic control (TC) API as the generic QoS request was converted for local enforcement by the TC API. This could be due to an out of memory error or to an internal QoS provider error.";
            break;
        case WSA_QOS_GENERIC_ERROR:
            asResultStr = "Socket Error (11015) QoS generic error. A general QoS error.";
            break;
        case WSA_QOS_ESERVICETYPE:
            asResultStr = "Socket Error (11016) QoS service type error. An invalid or unrecognized service type was found in the QoS flowspec.";
            break;
        case WSA_QOS_EFLOWSPEC:
            asResultStr = "Socket Error (11017) QoS flowspec error. An invalid or inconsistent flowspec was found in the QOS structure.";
            break;
        case WSA_QOS_EPROVSPECBUF:
            asResultStr = "Socket Error (11018) Invalid QoS provider buffer. An invalid QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFILTERSTYLE:
            asResultStr = "Socket Error (11019) Invalid QoS filter style. An invalid QoS filter style was used.";
            break;
        case WSA_QOS_EFILTERTYPE:
            asResultStr = "Socket Error (11020) Invalid QoS filter type. An invalid QoS filter type was used.";
            break;
        case WSA_QOS_EFILTERCOUNT:
            asResultStr = "Socket Error (11021) Incorrect QoS filter count. An incorrect number of QoS FILTERSPECs were specified in the FLOWDESCRIPTOR.";
            break;
        case WSA_QOS_EOBJLENGTH:
            asResultStr = "Socket Error (11022) Invalid QoS object length. An object with an invalid ObjectLength field was specified in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFLOWCOUNT:
            asResultStr = "Socket Error (11023) Incorrect QoS flow count. An incorrect number of flow descriptors was specified in the QoS structure.";
            break;
        case WSA_QOS_EUNKOWNPSOBJ:
            asResultStr = "Socket Error (11024) Unrecognized QoS object. An unrecognized object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EPOLICYOBJ:
            asResultStr = "Socket Error (11025) Invalid QoS policy object. An invalid policy object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFLOWDESC:
            asResultStr = "Socket Error (11026) Invalid QoS flow descriptor. An invalid QoS flow descriptor was found in the flow descriptor list.";
            break;
        case WSA_QOS_EPSFLOWSPEC:
            asResultStr = "Socket Error (11027) Invalid QoS provider-specific flowspec. An invalid or inconsistent flowspec was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EPSFILTERSPEC:
            asResultStr = "Socket Error (11028) Invalid QoS provider-specific filterspec. An invalid FILTERSPEC was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_ESDMODEOBJ:
            asResultStr = "Socket Error (11029) Invalid QoS shape discard mode object. An invalid shape discard mode object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_ESHAPERATEOBJ:
            asResultStr = "Socket Error (11030) Invalid QoS shaping rate object. An invalid shaping rate object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_RESERVED_PETYPE:
            asResultStr = "Socket Error (11031) Reserved policy QoS element type. A reserved policy element was found in the QoS provider-specific buffer.";
            break;
        default    :
            asResultStr = "Socket Error ("+IntToStr(ErrorCode)+") Unknown error code.";
            break;
    }
    return asResultStr;
}
//------------------------------------------------------------------------------
AnsiString GetErrorMsg(TObject *Sender, int ErrorCode)
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *) Sender;
    AnsiString asResultStr="", Str;
    switch(ErrorCode)
    {
        case 6://WSA_INVALID_HANDLE:
            asResultStr ="Socket Error (6) Specified event object handle is invalid. An application attempts to use an event object, but the specified handle is not valid. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 8://WSA_NOT_ENOUGH_MEMORY:
            asResultStr ="Socket Error (8) Insufficient memory available. An application used a Windows Sockets function that directly maps to a Windows function. The Windows function is indicating a lack of required memory resources. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 87://WSA_INVALID_PARAMETER:
            asResultStr ="Socket Error (87) One or more parameters are invalid. An application used a Windows Sockets function which directly maps to a Windows function. The Windows function is indicating a problem with one or more parameters. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 995://WSA_OPERATION_ABORTED:
            asResultStr ="Socket Error (995) Overlapped operation aborted. An overlapped operation was canceled due to the closure of the socket, or the execution of the SIO_FLUSH command in WSAIoctl. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 996://WSA_IO_INCOMPLETE:
            asResultStr ="Socket Error (996) Overlapped I/O event object not in signaled state. The application has tried to determine the status of an overlapped operation which is not yet completed. Applications that use WSAGetOverlappedResult (with the fWait flag set to FALSE) in a polling mode to determine when an overlapped operation has completed, get this error code until the operation is complete. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 997://WSA_IO_INCOMPLETE:
            asResultStr ="Socket Error (997) Overlapped operations will complete later. The application has initiated an overlapped operation that cannot be completed immediately. A completion indication will be given later when the operation has been completed. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case WSAEINTR:
            asResultStr = "Socket Error (10004) Interrupted function call. A blocking operation was interrupted by a call to WSACancelBlockingCall.";
            break;
        case WSAEBADF:
            asResultStr = "Socket Error (10009) File handle is not valid. The file handle supplied is not valid.";
            break;
        case WSAEACCES:
            asResultStr = "Socket Error (10013) Permission denied. An attempt was made to access a socket in a way forbidden by its access permissions. An example is using a broadcast address for sendto without broadcast permission being set using setsockopt(SO_BROADCAST). Another possible reason for the WSAEACCES error is that when the bind function is called (on Windows NT 4.0 with SP4 and later), another application, service, or kernel mode driver is bound to the same address with exclusive access. Such exclusive access is a new feature of Windows NT 4.0 with SP4 and later, and is implemented by using the SO_EXCLUSIVEADDRUSE option.";
            break;
        case WSAEFAULT:
            asResultStr = "Socket Error (10014) Bad address. The system detected an invalid pointer address in attempting to use a pointer argument of a call. This error occurs if an application passes an invalid pointer value, or if the length of the buffer is too small. For instance, if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr).";
            break;
        case WSAEINVAL:
            asResultStr = "Socket Error (10022) Invalid argument. Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function). In some instances, it also refers to the current state of the socket—for instance, calling accept on a socket that is not listening.";
            break;
        case WSAEMFILE:
            asResultStr = "Socket Error (10024) Too many open files. Too many open sockets. Each implementation may have a maximum number of socket handles available, either globally, per process, or per thread.";
            break;
        case WSAEWOULDBLOCK:
            asResultStr = "Socket Error (10035) Resource temporarily unavailable. This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operation should be retried later. It is normal for WSAEWOULDBLOCK to be reported as the result from calling connect on a nonblocking SOCK_STREAM socket, since some time must elapse for the connection to be established.";
            break;
        case WSAEINPROGRESS:
            asResultStr = "Socket Error (10036) Operation now in progress. A blocking operation is currently executing. Windows Sockets only allows a single blocking operation—per- task or thread—to be outstanding, and if any other function call is made (whether or not it references that or any other socket) the function fails with the WSAEINPROGRESS error.";
            break;
        case WSAEALREADY:
            asResultStr = "Socket Error (10037) Operation already in progress. An operation was attempted on a nonblocking socket with an operation already in progress—that is, calling connect a second time on a nonblocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already been canceled or completed.";
            break;
        case WSAENOTSOCK:
            asResultStr = "Socket Error (10038) Socket operation on nonsocket. An operation was attempted on something that is not a socket. Either the socket handle parameter did not reference a valid socket, or for select, a member of an fd_set was not valid.";
            break;
        case WSAEDESTADDRREQ:
            asResultStr = "Socket Error (10039) Destination address required. A required address was omitted from an operation on a socket. For example, this error is returned if sendto is called with the remote address of ADDR_ANY.";
            break;
        case WSAEMSGSIZE:
            asResultStr = "Socket Error (10040) Message too long. A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram was smaller than the datagram itself.";
            break;
        case WSAEPROTOTYPE:
            asResultStr = "Socket Error (10041) Protocol wrong type for socket. A protocol was specified in the socket function call that does not support the semantics of the socket type requested. For example, the ARPA Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM.";
            break;
        case WSAENOPROTOOPT:
            asResultStr = "Socket Error (10042) Bad protocol option. An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call.";
            break;
        case WSAEPROTONOSUPPORT:
            asResultStr = "Socket Error (10043) Protocol not supported. The requested protocol has not been configured into the system, or no implementation for it exists. For example, a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol.";
            break;
        case WSAESOCKTNOSUPPORT:
            asResultStr = "Socket Error (10044) Socket type not supported. The support for the specified socket type does not exist in this address family. For example, the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all.";
            break;
        case WSAEOPNOTSUPP:
            asResultStr = "Socket Error (10045) Operation not supported. The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptor to a socket that cannot support this operation is trying to accept a connection on a datagram socket.";
            break;
        case WSAEPFNOSUPPORT:
            asResultStr = "Socket Error (10046) Protocol family not supported. The protocol family has not been configured into the system or no implementation for it exists. This message has a slightly different meaning from WSAEAFNOSUPPORT. However, it is interchangeable in most cases, and all Windows Sockets functions that return one of these messages also specify WSAEAFNOSUPPORT.";
            break;
        case WSAEAFNOSUPPORT:
            asResultStr = "Socket Error (10047) Address family not supported by protocol family. An address incompatible with the requested protocol was used. All sockets are created with an associated address family (that is, AF_INET for Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). This error is returned if an incorrect protocol is explicitly requested in the socket call, or if an address of the wrong family is used for a socket, for example, in sendto.";
            break;
        case WSAEADDRINUSE:
            asResultStr = "Socket Error (10048) Address already in use. Typically, only one usage of each socket address (protocol/IP address/port) is permitted. This error occurs if an application attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socket that was not closed properly, or one that is still in the process of closing. For server applications that need to bind multiple sockets to the same port number, consider using setsockopt (SO_REUSEADDR). Client applications usually need not call bind at all—connect chooses an unused port automatically. When bind is called with a wildcard address (involving ADDR_ANY), a WSAEADDRINUSE error could be delayed until the specific address is committed. This could happen with a call to another function later, including connect, listen, WSAConnect, or WSAJoinLeaf.";
            break;
        case WSAEADDRNOTAVAIL:
            asResultStr = "Socket Error (10049) Cannot assign requested address. The requested address is not valid in its context. This normally results from an attempt to bind to an address that is not valid for the local computer. This can also result from connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for a remote computer (for example, address or port 0).";
            break;
        case WSAENETDOWN:
            asResultStr = "Socket Error (10050) Network is down. A socket operation encountered a dead network. This could indicate a serious failure of the network system (that is, the protocol stack that the Windows Sockets DLL runs over), the network interface, or the local network itself.";
            break;
        case WSAENETUNREACH:
            asResultStr = "Socket Error (10051) Network is unreachable. A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host.";
            break;
        case WSAENETRESET:
            asResultStr = "Socket Error (10052) Network dropped connection on reset. The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. It can also be returned by setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that has already failed.";
            break;
        case WSAECONNABORTED:
            asResultStr = "Socket Error (10053) Software caused connection abort. An established connection was aborted by the software in your host computer, possibly due to a data transmission time-out or protocol error.";
            break;
        case WSAECONNRESET:
            asResultStr = "Socket Error (10054) Connection reset by peer. An existing connection was forcibly closed by the remote host. This normally results if the peer application on the remote host is suddenly stopped, the host is rebooted, the host or remote network interface is disabled, or the remote host uses a hard close (see setsockopt for more information on the SO_LINGER option on the remote socket). This error may also result if a connection was broken due to keep-alive activity detecting a failure while one or more operations are in progress. Operations that were in progress fail with WSAENETRESET. Subsequent operations fail with WSAECONNRESET.";
            break;
        case WSAENOBUFS:
            asResultStr = "Socket Error (10055) No buffer space available. An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.";
            break;
        case WSAEISCONN:
            asResultStr = "Socket Error (10056) Socket is already connected. A connect request was made on an already-connected socket. Some implementations also return this error if sendto is called on a connected SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter in sendto is ignored) although other implementations treat this as a legal occurrence.";
            break;
        case WSAENOTCONN:
            asResultStr = "Socket Error (10057) Socket is not connected. A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using sendto) no address was supplied. Any other type of operation might also return this error—for example, setsockopt setting SO_KEEPALIVE if the connection has been reset.";
            break;
        case WSAESHUTDOWN:
            asResultStr = "Socket Error (10058) Cannot send after socket shutdown. A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. By calling shutdown a partial close of a socket is requested, which is a signal that sending or receiving, or both have been discontinued.";
            break;
        case WSAETOOMANYREFS:
            asResultStr = "Socket Error (10059) Too many references. Too many references to some kernel object.";
            break;
        case WSAETIMEDOUT:
            asResultStr = "Socket Error (10060) Connection timed out. A connection attempt failed because the connected party did not properly respond after a period of time, or the established connection failed because the connected host has failed to respond.";
            break;
        case WSAECONNREFUSED:
            asResultStr = "Socket Error (10061) Connection refused. No connection could be made because the target computer actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host—that is, one with no server application running.";
            break;
        case WSAELOOP:
            asResultStr = "Socket Error (10062) Cannot translate name. Cannot translate a name.";
            break;
        case WSAENAMETOOLONG:
            asResultStr = "Socket Error (10063) Name too long. A name component or a name was too long.";
            break;
        case WSAEHOSTDOWN:
            asResultStr = "Socket Error (10064) Host is down. A socket operation failed because the destination host is down. A socket operation encountered a dead host. Networking activity on the local host has not been initiated. These conditions are more likely to be indicated by the error WSAETIMEDOUT.";
            break;
        case WSAEHOSTUNREACH:
            asResultStr = "Socket Error (10065) No route to host. A socket operation was attempted to an unreachable host. See WSAENETUNREACH.";
            break;
        case WSAENOTEMPTY:
            asResultStr = "Socket Error (10066) Directory not empty. Cannot remove a directory that is not empty.";
            break;
        case WSAEPROCLIM:
            asResultStr = "Socket Error (10067) Too many processes. A Windows Sockets implementation may have a limit on the number of applications that can use it simultaneously. WSAStartup may fail with this error if the limit has been reached.";
            break;
        case WSAEUSERS:
            asResultStr = "Socket Error (10068) User quota exceeded. Ran out of user quota.";
            break;
        case WSAEDQUOT:
            asResultStr = "Socket Error (10069) Disk quota exceeded. Ran out of disk quota.";
            break;
        case WSAESTALE:
            asResultStr = "Socket Error (10070) Stale file handle reference. The file handle reference is no longer available.";
            break;
        case WSAEREMOTE:
            asResultStr = "Socket Error (10071) Item is remote. The item is not available locally.";
            break;
        case WSASYSNOTREADY:
            asResultStr = "Socket Error (10091) Network subsystem is unavailable. This error is returned by WSAStartup if the Windows Sockets implementation cannot function at this time because the underlying system it uses to provide network services is currently unavailable. Users should check: That the appropriate Windows Sockets DLL file is in the current path. That they are not trying to use more than one Windows Sockets implementation simultaneously. If there is more than one Winsock DLL on your system, be sure the first one in the path is appropriate for the network subsystem currently loaded. The Windows Sockets implementation documentation to be sure all necessary components are currently installed and configured correctly.";
            break;
        case WSAVERNOTSUPPORTED:
            asResultStr = "Socket Error (10092) Winsock.dll version out of range. The current Windows Sockets implementation does not support the Windows Sockets specification version requested by the application. Check that no old Windows Sockets DLL files are being accessed.";
            break;
        case WSANOTINITIALISED:
            asResultStr = "Socket Error (10093) Successful WSAStartup not yet performed. Either the application has not called WSAStartup or WSAStartup failed. The application may be accessing a socket that the current active task does not own (that is, trying to share a socket between tasks), or WSACleanup has been called too many times.";
            break;
        case WSAEDISCON:
            asResultStr = "Socket Error (10101) Graceful shutdown in progress. Returned by WSARecv and WSARecvFrom to indicate that the remote party has initiated a graceful shutdown sequence.";
            break;
        case WSAENOMORE:
            asResultStr = "Socket Error (10102) No more results. No more results can be returned by the WSALookupServiceNext function.";
            break;
        case WSAECANCELLED:
            asResultStr = "Socket Error (10103) Call has been canceled. A call to the WSALookupServiceEnd function was made while this call was still processing. The call has been canceled.";
            break;
        case WSAEINVALIDPROCTABLE:
            asResultStr = "Socket Error (10104) Procedure call table is invalid. The service provider procedure call table is invalid. A service provider returned a bogus procedure table to Ws2_32.dll. This is usually caused by one or more of the function pointers being NULL.";
            break;
        case WSAEINVALIDPROVIDER:
            asResultStr = "Socket Error (10105) Service provider is invalid. The requested service provider is invalid. This error is returned by the WSCGetProviderInfo and WSCGetProviderInfo32 functions if the protocol entry specified could not be found. This error is also returned if the service provider returned a version number other than 2.0.";
            break;
        case WSAEPROVIDERFAILEDINIT:
            asResultStr = "Socket Error (10106) Service provider failed to initialize. The requested service provider could not be loaded or initialized. This error is returned if either a service provider's DLL could not be loaded (LoadLibrary failed) or the provider's WSPStartup or NSPStartup function failed.";
            break;
        case WSASYSCALLFAILURE:
            asResultStr = "Socket Error (10107) System call failure. A system call that should never fail has failed. This is a generic error code, returned under various conditions. Returned when a system call that should never fail does fail. For example, if a call to WaitForMultipleEvents fails or one of the registry functions fails trying to manipulate the protocol/namespace catalogs. Returned when a provider does not return SUCCESS and does not provide an extended error code. Can indicate a service provider implementation error.";
            break;
        case WSASERVICE_NOT_FOUND:
            asResultStr = "Socket Error (10108) Service not found. No such service is known. The service cannot be found in the specified name space.";
            break;
        case WSATYPE_NOT_FOUND:
            asResultStr = "Socket Error (10109) Class type not found. The specified class was not found.";
            break;
        case WSA_E_NO_MORE:
            asResultStr = "Socket Error (10110) No more results. No more results can be returned by the WSALookupServiceNext function.";
            break;
        case WSA_E_CANCELLED:
            asResultStr = "Socket Error (10111) Call was canceled. A call to the WSALookupServiceEnd function was made while this call was still processing. The call has been canceled.";
            break;
        case WSAEREFUSED:
            asResultStr = "Socket Error (10112) Database query was refused. A database query failed because it was actively refused.";
            break;
        case WSAHOST_NOT_FOUND:
            asResultStr = "Socket Error (11001) Host not found. No such host is known. The name is not an official host name or alias, or it cannot be found in the database(s) being queried. This error may also be returned for protocol and service queries, and means that the specified name could not be found in the relevant database.";
            break;
        case WSATRY_AGAIN:
            asResultStr = "Socket Error (11002) Nonauthoritative host not found. This is usually a temporary error during host name resolution and means that the local server did not receive a response from an authoritative server. A retry at some time later may be successful.";
            break;
        case WSANO_RECOVERY:
            asResultStr = "Socket Error (11003) This is a nonrecoverable error. This indicates that some sort of nonrecoverable error occurred during a database lookup. This may be because the database files (for example, BSD-compatible HOSTS, SERVICES, or PROTOCOLS files) could not be found, or a DNS request was returned by the server with a severe error.";
            break;
        case WSANO_DATA:
            asResultStr = "Socket Error (11004) Valid name, no data record of requested type. The requested name is valid and was found in the database, but it does not have the correct associated data being resolved for. The usual example for this is a host name-to-address translation attempt (using gethostbyname or WSAAsyncGetHostByName) which uses the DNS (Domain Name Server). An MX record is returned but no A record—indicating the host itself exists, but is not directly reachable.";
            break;
        case WSA_QOS_RECEIVERS:
            asResultStr = "Socket Error (11005) QoS receivers. At least one QoS reserve has arrived.";
            break;
        case WSA_QOS_SENDERS:
            asResultStr = "Socket Error (11006) QoS senders. At least one QoS send path has arrived.";
            break;
        case WSA_QOS_NO_SENDERS:
            asResultStr = "Socket Error (11007) No QoS senders. There are no QoS senders.";
            break;
        case WSA_QOS_NO_RECEIVERS:
            asResultStr = "Socket Error (11008) QoS no receivers. There are no QoS receivers.";
            break;
        case WSA_QOS_REQUEST_CONFIRMED:
            asResultStr = "Socket Error (11009) QoS request confirmed. The QoS reserve request has been confirmed.";
            break;
        case WSA_QOS_ADMISSION_FAILURE:
            asResultStr = "Socket Error (11010) QoS admission error. A QoS error occurred due to lack of resources.";
            break;
        case WSA_QOS_POLICY_FAILURE:
            asResultStr = "Socket Error (11011) QoS policy failure. The QoS request was rejected because the policy system couldn't allocate the requested resource within the existing policy.";
            break;
        case WSA_QOS_BAD_STYLE:
            asResultStr = "Socket Error (11012) QoS bad style. An unknown or conflicting QoS style was encountered.";
            break;
        case WSA_QOS_BAD_OBJECT:
            asResultStr = "Socket Error (11013) QoS bad object. A problem was encountered with some part of the filterspec or the provider-specific buffer in general.";
            break;
        case WSA_QOS_TRAFFIC_CTRL_ERROR:
            asResultStr = "Socket Error (11014) QoS traffic control error. An error with the underlying traffic control (TC) API as the generic QoS request was converted for local enforcement by the TC API. This could be due to an out of memory error or to an internal QoS provider error.";
            break;
        case WSA_QOS_GENERIC_ERROR:
            asResultStr = "Socket Error (11015) QoS generic error. A general QoS error.";
            break;
        case WSA_QOS_ESERVICETYPE:
            asResultStr = "Socket Error (11016) QoS service type error. An invalid or unrecognized service type was found in the QoS flowspec.";
            break;
        case WSA_QOS_EFLOWSPEC:
            asResultStr = "Socket Error (11017) QoS flowspec error. An invalid or inconsistent flowspec was found in the QOS structure.";
            break;
        case WSA_QOS_EPROVSPECBUF:
            asResultStr = "Socket Error (11018) Invalid QoS provider buffer. An invalid QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFILTERSTYLE:
            asResultStr = "Socket Error (11019) Invalid QoS filter style. An invalid QoS filter style was used.";
            break;
        case WSA_QOS_EFILTERTYPE:
            asResultStr = "Socket Error (11020) Invalid QoS filter type. An invalid QoS filter type was used.";
            break;
        case WSA_QOS_EFILTERCOUNT:
            asResultStr = "Socket Error (11021) Incorrect QoS filter count. An incorrect number of QoS FILTERSPECs were specified in the FLOWDESCRIPTOR.";
            break;
        case WSA_QOS_EOBJLENGTH:
            asResultStr = "Socket Error (11022) Invalid QoS object length. An object with an invalid ObjectLength field was specified in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFLOWCOUNT:
            asResultStr = "Socket Error (11023) Incorrect QoS flow count. An incorrect number of flow descriptors was specified in the QoS structure.";
            break;
        case WSA_QOS_EUNKOWNPSOBJ:
            asResultStr = "Socket Error (11024) Unrecognized QoS object. An unrecognized object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EPOLICYOBJ:
            asResultStr = "Socket Error (11025) Invalid QoS policy object. An invalid policy object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFLOWDESC:
            asResultStr = "Socket Error (11026) Invalid QoS flow descriptor. An invalid QoS flow descriptor was found in the flow descriptor list.";
            break;
        case WSA_QOS_EPSFLOWSPEC:
            asResultStr = "Socket Error (11027) Invalid QoS provider-specific flowspec. An invalid or inconsistent flowspec was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EPSFILTERSPEC:
            asResultStr = "Socket Error (11028) Invalid QoS provider-specific filterspec. An invalid FILTERSPEC was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_ESDMODEOBJ:
            asResultStr = "Socket Error (11029) Invalid QoS shape discard mode object. An invalid shape discard mode object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_ESHAPERATEOBJ:
            asResultStr = "Socket Error (11030) Invalid QoS shaping rate object. An invalid shaping rate object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_RESERVED_PETYPE:
            asResultStr = "Socket Error (11031) Reserved policy QoS element type. A reserved policy element was found in the QoS provider-specific buffer.";
            break;
        default    :
            asResultStr = "Socket Error ("+IntToStr(ErrorCode)+") Unknown error code.";
            break;
    }

    if(Ptr!=NULL)
        Str.sprintf("Name:%s, IP:%s, Port:%d, %s", Ptr->Name, Ptr->Address, Ptr->Port, asResultStr);
    else
        Str.sprintf("Name:, IP:, Port:, %s", asResultStr);
    return asResultStr;
}
//------------------------------------------------------------------------------
void LogClientSocketExceptionError(TObject *Sender, AnsiString Msg)             //Steven 20231113 : 紀錄網路連線例外
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *) Sender;
    AnsiString Str;
    if(Ptr!=NULL)
        Str.sprintf("Name:%s, IP:%s, Port:%d, Exception Error:%s", Ptr->Name, Ptr->Address, Ptr->Port, Msg);
    else
        Str.sprintf("Name:, IP:, Port:, Exception Error:%s", Msg);

    MyDBIProcess("Exception", Str);
}
//------------------------------------------------------------------------------
AnsiString GetOnlyTimeInfoByString(AnsiString asSign)
{
    AnsiString Str;
    GetTimeInfo();
    if(asSign.IsEmpty())
        Str.sprintf("%02d%02d%02d", SystemHour, SystemMin , SystemSec);
    else
        Str.sprintf("%02d%s%02d%s%02d", SystemHour, asSign, SystemMin, asSign, SystemSec);
    return Str;
}
//---------------------------------------------------------------------------
AnsiString GetDateInfoByString(AnsiString asSign)
{
    AnsiString Str;
    GetTimeInfo();
    if(asSign.IsEmpty())
        Str.sprintf("%04d%02d%02d", SystemYear, SystemMonth, SystemDate);
    else
        Str.sprintf("%04d%s%02d%s%02d", SystemYear, asSign, SystemMonth, asSign, SystemDate);
    return Str;
}
//---------------------------------------------------------------------------
void GetArmRowCol(int &iRow, int &iCol)
{
    iRow=4;
    iCol=2;
}
//---------------------------------------------------------------------------
void GetTrayRowCol(int &iRow, int &iCol)
{
    iRow=atoi(fSetup->edYDivision->Text.c_str());
    iCol=atoi(fSetup->edXDivision->Text.c_str());
}
//---------------------------------------------------------------------------
DWORD MyTickCount()
{
    return GetTickCount();
}
//---------------------------------------------------------------------------
//Steven 20200807 : 趁著Sleep的時候去檢查EC change report
//==>
int iSleepCount=0;
bool bRunTimer=false;
bool bAlreadySleep=false;
TQPF_Timer MySpeelTimer;
DWORD MySleepEx(DWORD dwMilliseconds, bool bAlertable)
{
    #ifdef USE_EC_CHANGE
    int iFlag;
    bool bStopWhile=false;
    static int iInputCnt=0;
    int iLeftTime;

    if(bAlreadySleep==false)        //避免多執行緒進來
    {
        bAlreadySleep=true;
    }
    else
    {
        return SleepEx(dwMilliseconds, bAlertable);
    }

    if(bRunTimer==false)
    {
        MySpeelTimer.SetMSAndOn(dwMilliseconds);
        MySpeelTimer.LatchCycleTime(true);
        bRunTimer=true;
    }

    iInputCnt=iSleepCount;
    do
    {
        iFlag=HGem->DoReportECChange(iSleepCount);
        if(iFlag==1)
        {
            iSleepCount++;
            if(iSleepCount>=HGem->EC_ID->Count)
                iSleepCount=0;
        }

        if(iSleepCount==iInputCnt || iFlag!=1)  //假設已經繞了一圈, 或者不需要執行EC check, 就停止while loop
        {
            bStopWhile=true;
            break;
        }
    }
    while(MySpeelTimer.Off()==false && bStopWhile==false);

    if(bStopWhile==true)
    {
        iLeftTime=dwMilliseconds-MySpeelTimer.LatchCycleTime();

        if(iLeftTime>0)
            SleepEx(iLeftTime, bAlertable);
    }
    bRunTimer=false;
    bAlreadySleep=false;
    return 1;
    #else
    return SleepEx(dwMilliseconds, bAlertable);
    #endif
}
//<==
//Steven 20200807 : 趁著Sleep的時候去檢查EC change report
//---------------------------------------------------------------------------
void MySleep(DWORD dwMilliseconds)
{
    ::Sleep(dwMilliseconds);
}
//---------------------------------------------------------------------------
//<==
//Steven 20190918 : 避免Compile異常, 把STD Function包起來
//---------------------------------------------------------------------------
bool DeleteDirectory(AnsiString sDir)
{
    if(DirectoryExists(sDir)==false)
    {
        return true;
    }

    AnsiString sDirFile=sDir+"\\*.*";
    TSearchRec sr;
    if(FindFirst(sDirFile, faAnyFile, sr)==0)
    {
        do
        {
            if(!(sr.Attr & faDirectory))
            {
                AnsiString sFilePath=sDir+"\\"+sr.Name;
                DeleteFile(sFilePath);
            }
            else
            {
                if(sr.Name!="." && sr.Name!="..")
                {
                    AnsiString sSubDir=sDir+"\\"+sr.Name;
                    if(DeleteDirectory(sSubDir)==false)
                        break;
                }
            }
        }while(FindNext(sr)==0);
        FindClose(sr);
    }
    return RemoveDir(sDir);
}
//---------------------------------------------------------------------------
AnsiString GetNowTime()                                    
{
    TDateTime _now=uTimeTool().GetNow();
    return uTimeTool().TDatetmeToAnsiLog(_now);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Sam 20230328 : 自動更新增加版本檢查
//==>
AnsiString GetSoftwareFileVersion(AnsiString sFilePatch)                        //產品版本
{
    AnsiString sFileVer="";
    VerInfo *myVerInfo=new  VerInfo();
    myVerInfo->FileName=sFilePatch;
    sFileVer=myVerInfo->ProductVersion;
    delete myVerInfo;
    return sFileVer;
}
//------------------------------------------------------------------------------
VerInfo::VerInfo()
{
    m_ClearData();
}
//------------------------------------------------------------------------------
void VerInfo::m_ClearData(void)
{
    ZeroMemory((void *)(&m_stFileInfo), sizeof(m_stFileInfo));
    m_wLangID               =0;
    m_wCharsetID            =0;
    m_strProductName        ="";
    m_strProductVersion     ="";
    m_strOriginalFilename   ="";
    m_strFileDescription    ="";
    m_strFileVersion        ="";
    m_strCompanyName        ="";
    m_strLegalCopyright     ="";
    m_strLegalTrademarks    ="";
    m_strInternalName       ="";
    m_strComments           ="";
}
//------------------------------------------------------------------------------
void VerInfo::m_SetFileName(AnsiString strNewName)
{
    m_strFileName=strNewName;
    m_GetVerInfo();
}
//------------------------------------------------------------------------------
AnsiString VerInfo::m_strGetFixedFileVersion(void)
{
    AnsiString s0((int)HIWORD(m_stFileInfo.dwFileVersionMS));
    AnsiString s1((int)LOWORD(m_stFileInfo.dwFileVersionMS));
    AnsiString s2((int)HIWORD(m_stFileInfo.dwFileVersionLS));
    AnsiString s3((int)LOWORD(m_stFileInfo.dwFileVersionLS));
    return (s0+"."+s1+"."+s2+"."+s3);
}
//------------------------------------------------------------------------------
AnsiString VerInfo::m_strGetFixedProductVersion(void)
{
    AnsiString s0((int)HIWORD(m_stFileInfo.dwProductVersionMS));
    AnsiString s1((int)LOWORD(m_stFileInfo.dwProductVersionMS));
    AnsiString s2((int)HIWORD(m_stFileInfo.dwProductVersionLS));
    AnsiString s3((int)LOWORD(m_stFileInfo.dwProductVersionLS));
    return (s0+"."+s1+"."+s2+"."+s3);
}
//------------------------------------------------------------------------------
void VerInfo::GetAppVersion(AnsiString sAppExeName,WORD& major,WORD& minor,WORD& build,WORD& revision)
{
    DWORD handle = 0;
    AnsiString fileName=sAppExeName;
    DWORD size = GetFileVersionInfoSize(fileName.c_str(), &handle);
    if(size==0)
        return; //"Version Info Not Found"
    void* data = malloc(size);
    if(GetFileVersionInfo(fileName.c_str(), handle, size, data)==false)
    {
        free(data);
        return;// "Version Info Error";
    }
    VS_FIXEDFILEINFO* fileInfo=NULL;
    UINT len = 0;
    if(VerQueryValue(data, "\\", (void**)&fileInfo, &len)==false)
    {
        free(data);
        return;// "Version Info Parse Error";
    }
    major = HIWORD(fileInfo->dwFileVersionMS);
    minor = LOWORD(fileInfo->dwFileVersionMS);
    build = HIWORD(fileInfo->dwFileVersionLS);
    revision = LOWORD(fileInfo->dwFileVersionLS);
    free(data);
}
//------------------------------------------------------------------------------
AnsiString VerInfo::GetSVNRev()
{
    AnsiString sret="";
    unsigned short iFileVerMajor=0, iFileVerMinor=0, iFileVerRelease=0, iFileVerBuild=0;
    AnsiString strFilePath=Application->ExeName;
    VerInfo().GetAppVersion(strFilePath,iFileVerMajor, iFileVerMinor, iFileVerRelease, iFileVerBuild);
    sret=AnsiString().sprintf("%d.%d.%d.%d", iFileVerMajor, iFileVerMinor, iFileVerRelease, iFileVerBuild);
    return sret;
}
//------------------------------------------------------------------------------
void VerInfo::m_GetVerInfo(void)
{
    const static TCHAR *VersionKeys[]=
    {
        TEXT("ProductName"),
        TEXT("ProductVersion"),
        TEXT("OriginalFilename"),
        TEXT("FileDescription"),
        TEXT("FileVersion"),
        TEXT("CompanyName"),
        TEXT("LegalCopyright"),
        TEXT("LegalTrademarks"),
        TEXT("InternalName"),
        TEXT("Comments")
    };

    VKINFO gVKArray[10];
    m_ClearData();

    DWORD dwHandle=0;
    DWORD dwLength;
    dwLength = GetFileVersionInfoSize(m_strFileName.c_str(), &dwHandle);
    if(1>dwLength)
    {
        m_dwLastError=GetLastError();
        return;
    }

    HANDLE hMem;
    LPVOID lpvMem;
    hMem = GlobalAlloc(GMEM_MOVEABLE, dwLength);
    if(NULL==hMem)
    {
        m_dwLastError=GetLastError();
        return;
    }
    lpvMem=GlobalLock(hMem);
    if(NULL==lpvMem)
    {
        m_dwLastError=GetLastError();
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return;
    }

    BOOL fRet;
    fRet = GetFileVersionInfo(m_strFileName.c_str(), dwHandle, dwLength, (LPVOID)lpvMem);
    if(FALSE==fRet)
    {
        m_dwLastError=GetLastError();
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return;
    }

    LPVOID lpInfo;
    UINT cch;
    if(VerQueryValue(lpvMem, "\\", &lpInfo, &cch))
    {
        CopyMemory((void*)&m_stFileInfo,(const void*)lpInfo, sizeof(m_stFileInfo));
    }
    else
    {
        m_dwLastError=GetLastError();
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return;
    }

    AnsiString strLangID;
    AnsiString strCharset;
    if(VerQueryValue(lpvMem, VERSION_INFO_KEY_TRANS, &lpInfo, &cch))
    {
        m_wLangID   =((WORD *)lpInfo)[0];
        m_wCharsetID=((WORD *)lpInfo)[1];
        strLangID   =strLangID.IntToHex((int)m_wLangID, 4);
        strCharset  =strCharset.IntToHex((int)m_wCharsetID, 4);
    }
    else
    {
        m_dwLastError=GetLastError();
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return;
    }
    AnsiString strVerInfoLangID(strLangID+strCharset);

    TCHAR key[80];
    static TCHAR szNull[1]=TEXT("");
    for(UINT i=0; i<10; i++)
    {
        lstrcpy(key, VERSION_INFO_KEY_ROOT);
        lstrcat(key, strVerInfoLangID.c_str());
        lstrcat(key, "\\");
        lstrcat(key, VersionKeys[i]);
        gVKArray[i].szKey = VersionKeys[i];

        if(VerQueryValue(lpvMem, key, &lpInfo, &cch))
        {
            gVKArray[i].szValue = (char *)lpInfo;
        }
        else
        {
            gVKArray[i].szValue = szNull;
        }
    }
    m_strProductName        = gVKArray[0].szValue;
    m_strProductVersion     = gVKArray[1].szValue;
    m_strOriginalFilename   = gVKArray[2].szValue;
    m_strFileDescription    = gVKArray[3].szValue;
    m_strFileVersion        = gVKArray[4].szValue;
    m_strCompanyName        = gVKArray[5].szValue;
    m_strLegalCopyright     = gVKArray[6].szValue;
    m_strLegalTrademarks    = gVKArray[7].szValue;
    m_strInternalName       = gVKArray[8].szValue;
    m_strComments           = gVKArray[9].szValue;

    GlobalUnlock(hMem);
    GlobalFree(hMem);

    m_dwLastError = GetLastError();
}
//<==
//Sam 20230328 : 自動更新增加版本檢查
//------------------------------------------------------------------------------
TBasicAllAction::TBasicAllAction()
{
    iStatus=0;
}
//------------------------------------------------------------------------------
TBasicAllAction::~TBasicAllAction()
{
}
//------------------------------------------------------------------------------
void TBasicAllAction::SetActionStep(int iStep)
{
    iDoAllActionTask=iStep;
    iStatus=iStep;
}
//------------------------------------------------------------------------------
double uMotorRationCompute::pulseDividerDistance_mm(double PulsesS,double PulsesE,double Distance_mm_S,double Distance_mm_E)
{
    double dret=0.0;
    double dpulsegap=PulsesE-PulsesS;
    double ddistancegap=Distance_mm_E-Distance_mm_S;
    if(ddistancegap==0)
        return 1.0;
    dret=dpulsegap/ddistancegap;
    return dret;
}
//------------------------------------------------------------------------------
int uMotorRationCompute::GetTargetPulseByDistance(double PulsesS,double PulsesE,double DistanceS,double DistanceE,double dTarDistance)
{
    double dret=0.0;
    dpulse_dis_ratio=pulseDividerDistance_mm(PulsesS,PulsesE,DistanceS,DistanceE);
    dret=(dTarDistance-DistanceS)*dpulse_dis_ratio;
    return (int)dret;
}
//------------------------------------------------------------------------------
double uMotorRationCompute::GetMotorRatioFromSPosToEPos(double PulsesS,double PulsesE,double RealDistanceS,double RealDistanceE,double oldMotorRatio)
{
    double dret=0.0;
    MotorConfig motCon;
    initializeMotorConfig(PulsesS,RealDistanceS,PulsesE,RealDistanceE,motCon);
    if(motCon.pulses_per_mm==0.0)
        return 1.0;
    dret=1/motCon.pulses_per_mm;
    dret*=oldMotorRatio;
    return dret;
}
//------------------------------------------------------------------------------
void uMotorRationCompute::initializeMotorConfig(double pulses1, double distance1, double pulses2, double distance2, MotorConfig& motc)
{
    double dDistGap=distance2 - distance1;
    if(dDistGap==0.0)
        dDistGap=1;
    motc.pulses_per_mm = (pulses2 - pulses1) / dDistGap;
    motc.reference_pulses = pulses1;
    motc.reference_distance = distance1;
}
//------------------------------------------------------------------------------
double uMotorRationCompute::pulsesToDistance(MotorConfig config, double pulses)
{
    double dPPM=config.pulses_per_mm;
    if(dPPM==0.0)
        dPPM=1;
    return config.reference_distance + (pulses - config.reference_pulses) / dPPM;
}
//------------------------------------------------------------------------------
double uMotorRationCompute::distanceToPulses(MotorConfig config, double distance)
{
    return config.reference_pulses + (distance - config.reference_distance) * config.pulses_per_mm;
}
//------------------------------------------------------------------------------
void SetEditValue0(TWinControl *PCtrl)
{
    for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
    {
        TControl *P = PCtrl->Controls[iP];
        if (dynamic_cast <TPanel *>(P) != NULL          ||
            dynamic_cast <TPageControl *>(P) != NULL    ||
            dynamic_cast <TTabSheet *>(P) != NULL       ||
            dynamic_cast <TTabControl *>(P) != NULL     ||
            dynamic_cast <TForm *>(P) != NULL           ||
            dynamic_cast <THeader *>(P) != NULL         ||
            dynamic_cast <TPage *>(P) != NULL           ||
            dynamic_cast <TGroupBox*>(P) != NULL        ||
            dynamic_cast <TScrollBox*>(P) != NULL       ||
            dynamic_cast <TRadioGroup*>(P) != NULL  )
        {
            SetEditValue0((TWinControl *) P);    // 找該物件附屬的物件(遞迴)
        }
        TEdit *PBTEdit              = dynamic_cast <TEdit *>(P);
        if (PBTEdit != NULL)
        {
            PBTEdit->Text="0";
        }
    }
}
//---------------------------------------------------------------------------

