
#pragma hdrstop
//---------------------------------------------------------------------------

#include "FTPClient.h"

#include "mymessbox.h"
#include "note.h"
//#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
TfFTPClient *fFTPClient;
TStringList *tmpList;//Landam
void delay(int ms);
bool bTempList=false;
//bool bError=false; //KaiChen 20190530 ：改為public  //Landam
bool FTP_DownloadFail = false;      // 2013.12.03 , Joye , KYEC FTP

bool bCanExit=true;  //ChungHung 20150413 add for TSMC
//---------------------------------------------------------------------------
__fastcall TfFTPClient::TfFTPClient(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;
    bListOk=false;
    bIsFtpRunning=false;

}
//---------------------------------------------------------------------------
void delay(int ms)
{

}
//---------------------------------------------------------------------------
double dblFileSize(const char* fname)                                           //Ifor 20160907 FTP Download File Size
{
    return 0.0;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::LoadFileFormServer2(AnsiString FtpPath, AnsiString Source)
{

}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::UploadFileToServer2(AnsiString FtpPath, AnsiString Source, bool bZip) //ChungHung 20140108 add FTP unload jam code
{

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfFTPClient::FormCreate(TObject *Sender)
{
    ProgressBar1->Parent = sbFTPStatus;
    ProgressBar1->Top = 2;
    ProgressBar1->Left = 1;
}
//---------------------------------------------------------------------------
TNMFTP *NMFTP3;
void __fastcall TfFTPClient::ShowFTPModal(int HD)
{

}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1ListItem(AnsiString Listing)
{
    //列出目錄內的檔案-----
    if(bTempList==false)
    {
        if(tmpList==NULL)
            tmpList=new TStringList;

        if(Listing.AnsiPos(".zip")>0 && (Listing.AnsiPos(".Offset")==0 || Listing.AnsiPos("ATC_Recipe.zip")==0))  //JerryYang 20190906 ATC工作檔經由handler上傳/下載
        {
            lstServerFile->Items->Add(Listing.SubString(1, Listing.Length()-4));
            tmpList->Add(Listing.SubString(1, Listing.Length()-4));//Landam
        }
        else
        {
            tmpList->Add(Listing);//Landam
        }
    }
    else
    {
        ListBox1->Items->Add(Listing);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1Failure(bool &Handled, TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : memoFTP->Lines->Add("Failure -- ChangeDir failed"      ); break;
        case cmdMakeDir     : memoFTP->Lines->Add("Failure -- MakeDir failed"        ); break;
        case cmdDelete      : memoFTP->Lines->Add("Failure -- Delete failed"         ); break;
        case cmdRemoveDir   : memoFTP->Lines->Add("Failure -- RemoveDir failed"      ); break;
        case cmdList        : memoFTP->Lines->Add("Failure -- List failed"           ); break;
        case cmdRename      : memoFTP->Lines->Add("Failure -- Rename failed"         ); break;
        case cmdUpRestore   : memoFTP->Lines->Add("Failure -- UploadRestore failed"  ); break;
        case cmdDownRestore : memoFTP->Lines->Add("Failure -- DownloadRestore failed"); break;
        case cmdDownload    : memoFTP->Lines->Add("Failure -- Download failed"       ); break;
        case cmdUpload      : memoFTP->Lines->Add("Failure -- Upload failed"         ); break;
        case cmdAppend      : memoFTP->Lines->Add("Failure -- UploadAppend failed"   ); break;
        case cmdReInit      : memoFTP->Lines->Add("Failure -- Reinitialize failed"   ); break;
        case cmdAllocate    : memoFTP->Lines->Add("Failure -- Allocate failed"       ); break;
        case cmdNList       : memoFTP->Lines->Add("Failure -- NList failed"          ); break;
        case cmdDoCommand   : memoFTP->Lines->Add("Failure -- DoCommand failed"      ); break;
        case cmdCurrentDir  : memoFTP->Lines->Add("Failure -- CurrentDir failed"     ); break;
    }
    bError=true;
}
void __fastcall TfFTPClient::NMFTP1Success(TCmdType Trans_Type)
{
    AnsiString Str="";
    switch(Trans_Type)
    {
        case cmdChangeDir   : memoFTP->Lines->Add("Success -- ChangeDir successful"      );break;
        case cmdMakeDir     : memoFTP->Lines->Add("Success -- MakeDir successful"        );break;
        case cmdRemoveDir   : memoFTP->Lines->Add("Success -- RemoveDir successful"      );break;
        case cmdDelete      : memoFTP->Lines->Add("Success -- Delete successful"         );break;
        case cmdRename      : memoFTP->Lines->Add("Success -- Rename successful"         );break;
        case cmdReInit      : memoFTP->Lines->Add("Success -- Reinitialize successful"   );break;
        case cmdCurrentDir  : memoFTP->Lines->Add("Success -- CurrentDir successful"     );break;
        case cmdList        : memoFTP->Lines->Add("Success -- List successful"           ); bListOk=true; break;
        case cmdNList       : memoFTP->Lines->Add("Success -- NList successful"          ); bListOk=true; break;
        case cmdUpRestore   : memoFTP->Lines->Add("Success -- UpRestore successful"      );break;
        case cmdDownRestore : memoFTP->Lines->Add("Success -- DownRestore successful"    );break;
        case cmdDownload    : memoFTP->Lines->Add("Success -- Download successful"       );break;
        case cmdUpload      : memoFTP->Lines->Add("Success -- Upload successful"         );break;
        case cmdAppend      : memoFTP->Lines->Add("Success -- Append successful"         );break;
        case cmdAllocate    : memoFTP->Lines->Add("Success -- Allocate successful"       );break;
        case cmdDoCommand   : memoFTP->Lines->Add("Success -- DoCommand successful"      );break;
    }
    bError=false;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1UnSupportedFunction(TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : memoFTP->Lines->Add("UnSupported -- ChangeDir not supported by this server"       ); break;
        case cmdMakeDir     : memoFTP->Lines->Add("UnSupported -- MakeDir not supported by this server"         ); break;
        case cmdDelete      : memoFTP->Lines->Add("UnSupported -- Delete not supported by this server"          ); break;
        case cmdRemoveDir   : memoFTP->Lines->Add("UnSupported -- RemoveDir not supported by this server"       ); break;
        case cmdList        : memoFTP->Lines->Add("UnSupported -- List not supported by this server"            ); break;
        case cmdRename      : memoFTP->Lines->Add("UnSupported -- Rename not supported by this server"          ); break;
        case cmdUpRestore   : memoFTP->Lines->Add("UnSupported -- UploadRestore not supported by this server"   ); break;
        case cmdDownRestore : memoFTP->Lines->Add("UnSupported -- DownloadRestore not supported by this server" ); break;
        case cmdDownload    : memoFTP->Lines->Add("UnSupported -- Download not supported by this server"        ); break;
        case cmdUpload      : memoFTP->Lines->Add("UnSupported -- Upload not supported by this server"          ); break;
        case cmdAppend      : memoFTP->Lines->Add("UnSupported -- UploadAppend not supported by this server"    ); break;
        case cmdReInit      : memoFTP->Lines->Add("UnSupported -- Reinitialize not supported by this server"    ); break;
        case cmdAllocate    : memoFTP->Lines->Add("UnSupported -- Allocate not supported by this server"        ); break;
        case cmdNList       : memoFTP->Lines->Add("UnSupported -- NList not supported by this server"           ); break;
        case cmdDoCommand   : memoFTP->Lines->Add("UnSupported -- DoCommand not supported by this server"       ); break;
        case cmdCurrentDir  : memoFTP->Lines->Add("UnSupported -- CurrentDir not supported by this server"      ); break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::memoFTPDblClick(TObject *Sender)
{
    memoFTP->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1Error(TComponent *Sender, WORD Errno,
      AnsiString Errmsg)
{
    ShowMyMessage(Errmsg);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1Status(TComponent *Sender,
      AnsiString Status)
{
    ShowMyMessage(Status);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::FilterList(TObject *Sender)
{
//    int i;
//    TEdit *edit=dynamic_cast <TEdit *>   (Sender);
//    AnsiString str1,str2;
//    lstHDFile->Clear();
//    lstServerFile->Clear();
//    for(i=0; i<tmpList->Count; ++i)
//    {
//        str1=tmpList->Strings[i];
//        if(str1==edit->Text || edit->Text=="")   //Sam 20190925 : 修正 FTP 相似檔名無法下載問題
//        {
//            lstHDFile->Items->Add(str1);
//            lstServerFile->Items->Add(str1);
//        }
//    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtHDWaferNameChange(TObject *Sender)
{
    FilterList(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::edtServerWaferNameChange(TObject *Sender)
{
    FilterList(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::GetTesterType()    //Steven 20121018 : 取消不用MDB, 改用直接讀取文字檔
{

}
//---------------------------------------------------------------------------
AnsiString __fastcall TfFTPClient::SaveJamCodeFile(AnsiString aHandlerID, TDateTime  tDate, AnsiString aJamCode, AnsiString aMessage)  //ChungHung 20140108 add FTP unload jam code
{
    return "";
}
//-----------------------------------------------------------------


//Sam 20170525 (wei) 力成 add Socket ID Product Data Upload To FTP
//==>
void __fastcall TfFTPClient::UpSocketIdPoductDataToServerByFTP(AnsiString asDirPath, AnsiString sFileName)
{

}
//<==
//---------------------------------------------------------------------------
//Steven 20181224 : For ASE-CL
//==>
//pig 20180928 AddASECLFTP ==>
void TfFTPClient::DownloadWorkFileSettingFormServer()
{

}
//------------------------------------------------------------------------------
void TfFTPClient::DoUpdateSetupFileData(AnsiString FileName)
{

}
//pig 20180928 AddASECLFTP <==
//<==
//Steven 20181224 : For ASE-CL
//---------------------------------------------------------------------------
bool __fastcall TfFTPClient::DoPassword()   //JerryYang 20151028 add for Amkor
{
    return false;
}
//---------------------------------------------------------------------------
//Steven 20210413 : 南茂的自動Start功能, -1:Alarm, 0:不能跑, 1:可以跑
//==>
int TfFTPClient::N25_ReadAutoStartFileFromFTP()
{
    return 0;
}
//---------------------------------------------------------------------------
void TfFTPClient::N25_UploadTempDataToFTP(int iType) //Steven 20210413 : 南茂的自動Start功能, 0:上傳十筆, 1:只記錄不上傳, 2:整日
{

}
//---------------------------------------------------------------------------
//<==
//Steven 20210413 : 南茂的自動Start功能, -1:Alarm, 0:不能跑, 1:可以跑
//---------------------------------------------------------------------------
bool __fastcall TfFTPClient::CheckSetupFileNameFromServer(AnsiString asCheckSetupFileName)  //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
{
    return false;
}
//---------------------------------------------------------------------------
void TfFTPClient::Upload_JHT_log_FTP(AnsiString asSchedule)                     //JerryYang 20240620 : Add JHT_log_FTP
{
//
//
    AnsiString sFTPPath, sFTPFile, sUploadFile, sContent, sContent1, str, asPath, FtpPath;
    AnsiString sFTPPathHour, sFTPFileHour, sUploadFileHour, sFile;


    asPath="D:\\BarcodeSorter\\Summary\\";
    ForceDirectories(asPath);

//    asSchedule.sprintf("%s@%s@%s@99@HWBIN@", fMain->edLotNo->Text, fMain->edWaferLot->Text, fMain->edInsertion->Text, fMain->edFlowID->Text);
                                               //KenHsieh 20240522 : 無資料夾時需先建立
    sFile.sprintf("%s.txt", asSchedule);
    sFTPFile=sFile;
    sUploadFile=asPath+sFile;


    TNMFTP *pFTP;
    pFTP=new TNMFTP(this);


    try
    {
        pFTP->Vendor    = NMOS_AUTO;
        pFTP->TimeOut   = 20000;
        pFTP->Host                    =asHost;
        pFTP->UserID                  =asUserID;//edtUserName->Text;
        pFTP->Password                =asPassword;//edtPassword->Text;
        pFTP->OnListItem= NMFTP1ListItem;
//            pFTP->OnFailure = NMFTP1Failure;
        //Steven 20220510 : 針對FTP動作做紀錄
        //==>
        pFTP->OnSuccess               =NMFTP2Success;
        pFTP->OnFailure               =NMFTP2Failure;
        pFTP->OnError                 =NMFTP2Error;
        pFTP->OnStatus                =NMFTP2Status;
        pFTP->OnConnectionFailed      =NMFTP2ConnectionFailed;
        pFTP->OnTransactionStart      =NMFTP2TransactionStart;
        pFTP->OnTransactionStop       =NMFTP2TransactionStop;
        pFTP->OnAuthenticationFailed  =NMFTP2AuthenticationFailed;
        pFTP->OnUnSupportedFunction   =NMFTP2UnSupportedFunction;
        pFTP->OnDisconnect            =NMFTP2Disconnect;
        pFTP->OnConnect               =NMFTP2Connect;
        //<==
        //Steven 20220510 : 針對FTP動作做紀錄
        pFTP->Connect();

        for(int i=0; i<300; i++)     // 300 x 10 = 1000 ( 3 Sec )
        {
            Sleep(10);
            if(pFTP->Connected==true)
                break;
        }

        if(pFTP->Connected==false)
        {
            ShowMyMessage("FTP Server is not connected");
            delete pFTP;
            return ;
        }

        FtpPath=asFTPPathUpload;

        if(FtpPath.SubString(FtpPath.Length(), 1)!="/")
        {
            pFTP->ChangeDir(FtpPath + "/");
        }
        else
        {
            pFTP->ChangeDir(FtpPath);
        }




        try
        {
            pFTP->Upload(sUploadFile, sFTPFile);
        }
        catch(Exception &e)     //Steven 20220510 : 針對FTP動作做紀錄
        {
            str.sprintf("TfFTPClient::Upload JHT log file fail : %s to %s ", sUploadFile, sFTPFile);
            MyDBIProcess("Exception", str, e.Message);
            ShowMyMessage("FTP upload summary Fail");
        }


        pFTP->Abort();
        pFTP->RequestCloseSocket();
    }
    catch(Exception &e)     //Steven 20220510 : 針對FTP動作做紀錄
    {
        MyDBIProcess("Exception", "TfFTPClient::Upload_JHT_log_FTP", e.Message);
        delete pFTP;
        ShowMyMessage("FTP upload summary Fail");
        return ;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::Upload_JHT_log_FTP");
        delete pFTP;
        ShowMyMessage("FTP upload summary Fail");
        return ;
    }
    delete pFTP;
}
//---------------------------------------------------------------------------
void TfFTPClient::UploadFileFTP(AnsiString asSourcePath, AnsiString asSourceFileName, AnsiString asTargetPath, AnsiString asTargetFileName, AnsiString asUserID, AnsiString asPassword, AnsiString asHost, AnsiString asFun) //Sam 20210120 : Auto Upload FTP Temperature Info 功能
{

}
//---------------------------------------------------------------------------
//Steven 20220510 : 針對FTP動作做紀錄
//==>
void __fastcall TfFTPClient::NMFTP2Success(TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : RecordProcess( "Success -- ChangeDir successful"   );break;
        case cmdMakeDir     : RecordProcess( "Success -- MakeDir successful"     );break;
        case cmdRemoveDir   : RecordProcess( "Success -- RemoveDir successful"   );break;
        case cmdDelete      : RecordProcess( "Success -- Delete successful"      );break;
        case cmdRename      : RecordProcess( "Success -- Rename successful"      );break;
        case cmdReInit      : RecordProcess( "Success -- Reinitialize successful");break;
        case cmdCurrentDir  : RecordProcess( "Success -- CurrentDir successful"  );break;
        case cmdList        : RecordProcess( "Success -- List successful"        );break;
        case cmdNList       : RecordProcess( "Success -- NList successful"       );break;
        case cmdUpRestore   : RecordProcess( "Success -- UpRestore successful"   );break;
        case cmdDownRestore : RecordProcess( "Success -- DownRestore successful" );break;
        case cmdDownload    : RecordProcess( "Success -- Download successful"    );break;
        case cmdUpload      : RecordProcess( "Success -- Upload successful"      );break;
        case cmdAppend      : RecordProcess( "Success -- Append successful"      );break;
        case cmdAllocate    : RecordProcess( "Success -- Allocate successful"    );break;
        case cmdDoCommand   : RecordProcess( "Success -- DoCommand successful"   );break;
    }
    bError=false;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2AuthenticationFailed(bool &Handled)
{
    MyDBIProcess("FTP", "Authentication Failed");
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2TransactionStop(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "Data transfer end", Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2TransactionStart(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "Data transfer start", Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2ConnectionFailed(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "Failure -- Connection Failed", Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Failure(bool &Handled, TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : MyDBIProcess("FTP", "Failure -- ChangeDir failed"      ); break;
        case cmdMakeDir     : MyDBIProcess("FTP", "Failure -- MakeDir failed"        ); break;
        case cmdDelete      : MyDBIProcess("FTP", "Failure -- Delete failed"         ); break;
        case cmdRemoveDir   : MyDBIProcess("FTP", "Failure -- RemoveDir failed"      ); break;
        case cmdList        : MyDBIProcess("FTP", "Failure -- List failed"           ); break;
        case cmdRename      : MyDBIProcess("FTP", "Failure -- Rename failed"         ); break;
        case cmdUpRestore   : MyDBIProcess("FTP", "Failure -- UploadRestore failed"  ); break;
        case cmdDownRestore : MyDBIProcess("FTP", "Failure -- DownloadRestore failed"); break;
        case cmdDownload    : MyDBIProcess("FTP", "Failure -- Download failed"       ); break;
        case cmdUpload      : MyDBIProcess("FTP", "Failure -- Upload failed"         ); break;
        case cmdAppend      : MyDBIProcess("FTP", "Failure -- UploadAppend failed"   ); break;
        case cmdReInit      : MyDBIProcess("FTP", "Failure -- Reinitialize failed"   ); break;
        case cmdAllocate    : MyDBIProcess("FTP", "Failure -- Allocate failed"       ); break;
        case cmdNList       : MyDBIProcess("FTP", "Failure -- NList failed"          ); break;
        case cmdDoCommand   : MyDBIProcess("FTP", "Failure -- DoCommand failed"      ); break;
        case cmdCurrentDir  : MyDBIProcess("FTP", "Failure -- CurrentDir failed"     ); break;
    }
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2UnSupportedFunction(TCmdType Trans_Type)
{
    switch(Trans_Type)
    {
        case cmdChangeDir   : MyDBIProcess("FTP", "UnSupported -- ChangeDir not supported by this server"       ); break;
        case cmdMakeDir     : MyDBIProcess("FTP", "UnSupported -- MakeDir not supported by this server"         ); break;
        case cmdDelete      : MyDBIProcess("FTP", "UnSupported -- Delete not supported by this server"          ); break;
        case cmdRemoveDir   : MyDBIProcess("FTP", "UnSupported -- RemoveDir not supported by this server"       ); break;
        case cmdList        : MyDBIProcess("FTP", "UnSupported -- List not supported by this server"            ); break;
        case cmdRename      : MyDBIProcess("FTP", "UnSupported -- Rename not supported by this server"          ); break;
        case cmdUpRestore   : MyDBIProcess("FTP", "UnSupported -- UploadRestore not supported by this server"   ); break;
        case cmdDownRestore : MyDBIProcess("FTP", "UnSupported -- DownloadRestore not supported by this server" ); break;
        case cmdDownload    : MyDBIProcess("FTP", "UnSupported -- Download not supported by this server"        ); break;
        case cmdUpload      : MyDBIProcess("FTP", "UnSupported -- Upload not supported by this server"          ); break;
        case cmdAppend      : MyDBIProcess("FTP", "UnSupported -- UploadAppend not supported by this server"    ); break;
        case cmdReInit      : MyDBIProcess("FTP", "UnSupported -- Reinitialize not supported by this server"    ); break;
        case cmdAllocate    : MyDBIProcess("FTP", "UnSupported -- Allocate not supported by this server"        ); break;
        case cmdNList       : MyDBIProcess("FTP", "UnSupported -- NList not supported by this server"           ); break;
        case cmdDoCommand   : MyDBIProcess("FTP", "UnSupported -- DoCommand not supported by this server"       ); break;
        case cmdCurrentDir  : MyDBIProcess("FTP", "UnSupported -- CurrentDir not supported by this server"      ); break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Error(TComponent *Sender, WORD Errno,
      AnsiString Errmsg)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", Errmsg, Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Status(TComponent *Sender,
      AnsiString Status)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", Status, Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Connect(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "FTP Connect", Ptr->Name);
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP2Disconnect(TObject *Sender)
{
    TNMFTP *Ptr;
    Ptr=(TNMFTP *)Sender;
    MyDBIProcess("FTP", "FTP Disconnect", Ptr->Name);
}
//---------------------------------------------------------------------------
//<==
//Steven 20220510 : 針對FTP動作做紀錄
//---------------------------------------------------------------------------
AnsiString TfFTPClient::N31_AtuoTempureOfsByFTP()    //Sam 20220406 : 溫度自動補償功能 By FTP
{
    return "";
}
//---------------------------------------------------------------------------
AnsiString TfFTPClient::N31_AtuoTempureOfsByLocal()    //Sam 20220427 : 溫度自動補償功能 By Loacl
{
    return "";
}
//---------------------------------------------------------------------------
bool TfFTPClient::DownloadPasswordFormServer()  //Sam 20210526 : 從 N06 DownloadPath 下載密碼本
{
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::CheckFTPConnection()   //Isaac 20200710 : 全智科技，從FTP下載後要切換到initial start
{

}
//---------------------------------------------------------------------------
void TfFTPClient::DownloadUpdateAutomatically(bool bhint)   //Sam 20220824 : FTP 自動下載安裝更新包
{

}
//---------------------------------------------------------------------------
AnsiString TfFTPClient::N32_DownloadUpdateAutomatically(AnsiString sPatch,bool bDeleteFile)    //Sam 20220824 : FTP 自動下載安裝更新包
{
    return "";
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1ConnectionFailed(TObject *Sender)
{
    memoFTP->Lines->Add("Failure -- Connection Failed");
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1AuthenticationFailed(bool &Handled)
{
    memoFTP->Lines->Add("Authentication Failed");
    bError=true;
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1TransactionStop(TObject *Sender)
{
    memoFTP->Lines->Add("Data transfer end");
}
//---------------------------------------------------------------------------
void __fastcall TfFTPClient::NMFTP1TransactionStart(TObject *Sender)
{
    memoFTP->Lines->Add("Data transfer start");
}
//------------------------------------------------------------------------------
AnsiString __fastcall TfFTPClient::Download_2DSortingList(AnsiString sLotID)    //Frank 20221122 : 2DID sorting for ATK
{
//    if(CosFunction.PassworDownloadByFTP==false || IniConfig.bFtpPasswordDownload==false)
//        return false;

     //Landam 用動態元件 才穩定
    TNMFTP *NMFTP2;
    AnsiString Source="", FtpPath="";
    TStringList *SL=new TStringList();                                          //Sam 20240814 : ASECL 資料下載要抓最新時間檔案
    AnsiString s1="", Str="";
    int iDate=0, iTime=0, iNowDate, iNowTime=0;

//    bool bRunOffset=true;
//    double dFileSize[3]={0,0,0};  //Ifor 20160907 FTP Download File Size
    try
    {
        NMFTP2=new TNMFTP(this);

        //設定參數--------
        NMFTP2->Vendor                  =NMOS_AUTO;
        NMFTP2->TimeOut                 =20000; //Landam
        NMFTP2->Passive                 =true;  //Steven 20121020 : 實驗看看

        NMFTP2->Host                    =asHost;
        NMFTP2->UserID                  =asUserID;//edtUserName->Text;
        NMFTP2->Password                =asPassword;//edtPassword->Text;

        //設定Event--------
        NMFTP2->OnListItem              =NMFTP1ListItem;
        NMFTP2->OnSuccess               =NMFTP1Success;
        NMFTP2->OnFailure               =NMFTP1Failure;
        NMFTP2->OnError                 =NMFTP1Error;
        NMFTP2->OnStatus                =NMFTP1Status;
        NMFTP2->OnConnectionFailed      =NMFTP1ConnectionFailed;
        NMFTP2->OnTransactionStart      =NMFTP1TransactionStart;
        NMFTP2->OnTransactionStop       =NMFTP1TransactionStop;
        NMFTP2->OnAuthenticationFailed  =NMFTP1AuthenticationFailed;

        //連線--------
        NMFTP2->Connect();

        for(int i=0; i<10; ++i)
        {
            delay(50);
            if(NMFTP2->Connected)
                break;
        }

        if(!NMFTP2->Connected)
        {
            ShowMyMessage("FTP Server is not connected");
            delete NMFTP2;
            bError=true;
            FTP_DownloadFail = true;
            return "";
        }



        //下載下來的工作檔要加上_NET結尾------
        AnsiString str01, str02, str03, str04;
        AnsiString sDLFileName=Source;
        AnsiString DataPath="D:\\BarcodeSorter\\";

//        str02=Source+".txt";

        FtpPath=asFTPPath;

        if(FtpPath.SubString(FtpPath.Length(), 1)!="/")
        {
            NMFTP2->ChangeDir(FtpPath + "/");
        }
        else
        {
            NMFTP2->ChangeDir(FtpPath);
        }

        NMFTP2->Nlist();

        for(int i=0; i<tmpList->Count; ++i)
        {
            if(tmpList->Strings[i].AnsiPos(sLotID)>0)
            {
                Str=StringReplace(tmpList->Strings[i], "_", ",", TReplaceFlags()<<rfReplaceAll);    //Sam 20240814 : ASECL 資料下載要抓最新時間檔案
                SL->Clear();
                SL->CommaText=Str;          //"PH200L340P_20240604140156.log"
                if(SL->Count>=2)
                {
                    s1=SL->Strings[1];
                    if(s1.Length()==18)     //"20240604140156.log"
                    {
                        iDate=atoi(s1.SubString(0,s1.Length()-10).c_str());     //"20240604"
                        iTime=atoi(s1.SubString(9,s1.Length()-4).c_str());      //"140156"
                        if(iNowDate==0 && iNowTime==0)
                        {
                            iNowDate=iDate;
                            iNowTime=iTime;
                            Source=tmpList->Strings[i];
                        }
                        else if(iDate>iNowDate ||
                                (iDate==iNowDate && iTime>=iNowTime))
                        {
                            iNowDate=iDate;
                            iNowTime=iTime;
                            Source=tmpList->Strings[i];
                        }
                    }
                }
            }
        }
        str01=DataPath+Source;

        SL->Clear();    //Sam 20240814 : ASECL 資料下載要抓最新時間檔案
        delete SL;

        if(Source=="")
        {
            ShowMyMessage("Can not find the bin map file on server.");
            delete NMFTP2;
            return Source;
        }

        NMFTP2->Download(Source, str01);

        NMFTP2->Abort();
        delay(100);
        NMFTP2->RequestCloseSocket();
        delay(500);

        FTP_DownloadFail=false;       // 2013.12.03 , Joye , KYEC FTP  20140103 wei
        delete NMFTP2;
        return Source;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfFTPClient::DownloadPasswordFormServer");
        FTP_DownloadFail=true;        // 2013.12.03 , Joye , KYEC FTP       20140103 wei
        bError=true;
        delete NMFTP2;

        if(SL!=NULL)    //Sam 20240814 : ASECL 資料下載要抓最新時間檔案
        {
            SL->Clear();
            delete SL;
        }

    }
    return "";
}
//---------------------------------------------------------------------------

