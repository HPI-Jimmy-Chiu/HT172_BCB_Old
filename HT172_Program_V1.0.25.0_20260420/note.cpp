//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "note.h"
#pragma package(smart_init)
#pragma link "butPa1"
#pragma link "ALed"
#pragma link "HTray"
#pragma link "MyLed"
#pragma resource "*.dfm"
#include "database.h"
TfNote *fNote;
//---------------------------------------------------------------------------
//char ErrorCode[256];
int XPos, YPos, mess_jam=0, mess_err=1, mess_fuc=2;                             // 0:jam 1:錯誤 2:事件
DWORD RecordHappenTime,BackRecordHappenTime;
int iTargetOrder;
static DWORD StartTime,EndTime,PassTime;
AnsiString Recovery;
int iDuplicateError=0;
int iEventID;   //Steven 20090818
TPanel *FlushPanel=NULL;
bool bStartFromNote=false;
AnsiString ShuttleJamReg;                                                       //2008/07/22 lee
AnsiString ErrPicPitch,ErrPicPitch1,ErrPicPitch2;

//---------------------------------------------------------------------------
__fastcall TfNote::TfNote(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    bHasChangeNewTray=true;    //JerryYang 20240623 : Magazine手動補Tray
}
//---------------------------------------------------------------------------
void __fastcall TfNote::FormShow(TObject *Sender)
{
    TBtnPanel *Ptr[]={BtnSkip,BtnRetry,BtnTrayFeed,BtnTrayEnd,BtnCleanOut,BtnHome}; //JerryYang 20240704 : Pick up error Y軸一起回HOME
    int    KeyComp[]={K_SKIP, K_RETRY, K_TRAY_FEED, K_TRAY_END, K_CLEAN_OUT, K_HOME};
    int ct=0,i;
    static int BtnHeight,flag=1,BtnTop;

    StartTime=GetTickCount();
    fShow=true;
    Left=(1280-Width)/2;
    Top = 0;
    AnsiString Str;
    AnsiString sAlarmCode=fNote->edtAlarmCode->Text;
    HSys.DecStopAllMotor();                                                        // all motor stop //
    if( fHome->fShow )
        fHome->SpeedButton1Click(fHome);
    fMemoPos=false;                                                             // no use //
    bAlarmBuzzer=true;
    bLampAlarmReset=true;
    bHandlerPause=true;
    iHandlerStartCount=0;

    if(flag)
    {
        flag=0;
        BtnTop=BtnSkip->Top;
        BtnHeight=BtnRetry->Top-BtnSkip->Top;
    }

    for(i=0;i<6;i++)
    {
        Select[i]=false;
        Ptr[i]->FalseColor=(TColor)0x00804000;
        if( KeyCode & KeyComp[i])
        {
            Ptr[i]->Visible=true;
            ct++;
        }
        else
        {
            Ptr[i]->Visible=false;
        }
    }

    if(sAlarmCode=="32361" && HSys.FuncT.iT02_MagTraySource==1)  //JerryYang 20240623 : Magazine手動補Tray
    {
        bHasChangeNewTray=false;
        palNoticeEmptyTray->Visible=true;
    }
    else
    {
        palNoticeEmptyTray->Visible=false;
    }

    if(edtAlarmMsg->Text=="One Cycle Finish")
        UpdateButtonStatus(BtnRetry);

    if(CUSTOMER_CODE==CC_WINSTEK)
    {
        if(sAlarmCode=="60000")
        {
            tRunData.JamCount++;
        }
    }
    else if(sAlarmCode.AnsiPos('J') >0)
        tRunData.JamCount++;

    if(FlushPanel!=NULL)
        FlushPanelColor=FlushPanel->Color;
    Recovery="";                                                                //Steven 20120209 : 每次進來都要初始化
    EventReport(SECS_EVENT.ShowAlarm);                                          //Steven 20130723
    HGem->ReportAlarm(sAlarmCode, false, 0, edtAlarmMsg->Text);
    if(fTeach->fShow==true)
    {
        fTeach->sbMove->Down=false;
        fTeach->sbMoveP->Down=false;
        fTeach->sbMoveN->Down=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::FormClose(TObject *Sender, TCloseAction &Action)
{
    //char str[256];
    bool *bPtr[6]={&bLampSkip,&bLampRetry,&bLampTrayFeed,&bLampTrayEnd,&bLampCleanOut, &bLampHome};
    int i;//,j;

    fShow=false;
    for(i=0; i<6; i++)
        *bPtr[i]=Select[i];
    edtAlarmMsg->Text="";

    Alarm->Clear();
    if(iGali_VsSpeed==0)
        iGali_VsSpeed=30000;
    if(iGali_SpSpeed==0)
        iGali_SpSpeed=10000;
    bLampSkip=false;
    bLampRetry=false;
    bLampOneCycle=false;
    bLampCleanOut=false;
    bLampTrayFeed=false;
    bLampTrayEnd=false;
    bOffBuzzer=false;                                                           // ben add 20110706 //
    bSECSGEM_NoteAlarm=false;                                                   //Ifor 20170616 (wei) add note From Close時清除SECSGEM Note Alarm 旗標

    for(i=0; i<4; i++)                                                          // ben 20111026 //
        HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].Off();

    if(FlushPanel!=NULL)
        FlushPanel->Color=FlushPanelColor;

    if(KeyCode==0)                                                              //Steven : 沒有KeyCode的話,不用Recovery
        Recovery="";
    EndTime = GetTickCount();
    PassTime = (EndTime - StartTime)/1000;
//    UpdateAlarmPassTime();
    EventReport(SECS_EVENT.ReleaseAlarm);                                       //Steven 20130723
}
//---------------------------------------------------------------------------
void TfNote::ChangePalPos(TPanel *Panel, int Height, int Left, int Top, int Width, bool Visible)
{
    Panel->Height   =Height;
    Panel->Left     =Left;
    Panel->Top      =Top;
    Panel->Width    =Width;
    Panel->Visible  =Visible;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::UpdateButtonStatus(TObject *Sender)                     // 偵測按鈕狀態 //
{
    TBtnPanel *Ptr[]={BtnSkip, BtnRetry, BtnTrayFeed, BtnTrayEnd, BtnCleanOut, BtnHome};
    AnsiString KeyName[]={"SKIP", "RETRY", "TRAY_FEED", "TRAY_END", "CLEAN_OUT", "HOME & RETRY"};

    TBtnPanel *Ptr2;
    Ptr2=(TBtnPanel *) Sender;
    Tag=Ptr2->Tag-BtnSkip->Tag;

    if(bHasChangeNewTray==false)  //JerryYang 20240623 : Magazine手動補Tray
        return;

    for(int i=0; i<6; i++)
    {
        if(i!=Tag)
        {
            Select[i]=false;
            Ptr[i]->FalseColor=(TColor)0x00804000;
        }
        else
        {
            Select[i]=true;
            Ptr[i]->FalseColor=clRed;
            Recovery = KeyName[i];
        }
    }

    if(Select[BtnCleanOut->Tag-1])                                              //jimmychiu 20250701 : fixed
    {
        fMain->sbCleanOutClick(Owner);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::ScanKey()                                               // ben edit 20110706 //
{                                                                               // 偵測實體面板按鍵 //
    TBtnPanel *Ptr[]={BtnSkip, BtnRetry, BtnTrayFeed, BtnTrayEnd, BtnCleanOut, BtnHome};
    int Index2[]={HSys.Sen.SnRKSkip.Tag, HSys.Sen.SnRKRetry.Tag,HSys.Sen.SnRKTrayFeed.Tag,
                  HSys.Sen.SnRKTrayEnd.Tag, HSys.Sen.SnRKCleanOut.Tag, HSys.Sen.SnRKHome.Tag};
    int Key, i;
    Key=ScanPannelKey();
    if(Key==-1)
        return;
    for(i=0; i<sizeof(Ptr)/sizeof(Ptr[0]); i++)
    {
        if(Ptr[i]->Visible &&  Key==Index2[i])
        {
            UpdateButtonStatus(Ptr[i]);
            bAlarmBuzzer=false;
            bLampAlarmReset=false;
        }
    }

    if(Key==HSys.Sen.SnRKStart.Tag)
    {
        RecordProcess("START pressed");
        if(HasICUnderMachine())
            EventReport(SECS_EVENT.PressStartWithIC);
        else
            EventReport(SECS_EVENT.PressStartWithoutIC);
        BtnStartClick(this);
    }
    else if(Key==HSys.Sen.SnRKPause.Tag)
    {
        RecordProcess("PAUSE pressed");
        EventReport(SECS_EVENT.PressPause);
        BtnPauseClick(this);
    }
    else if(Key==HSys.Sen.SnRKSkip.Tag)
    {
        RecordProcess("SKIP pressed");
        EventReport(SECS_EVENT.PressSkip);
    }
    else if(Key==HSys.Sen.SnRKRetry.Tag)
    {
        RecordProcess("RETRY pressed");
        EventReport(SECS_EVENT.PressRetry);
    }
    else if(Key==HSys.Sen.SnRKTrayFeed.Tag)
    {
        RecordProcess("TRAY FEED pressed");
        EventReport(SECS_EVENT.PressTrayFeed);
    }
    else if(Key==HSys.Sen.SnRKTrayEnd.Tag)
    {
        RecordProcess("TRAY END pressed");
        EventReport(SECS_EVENT.PressTrayEnd);
    }
    else if(Key==HSys.Sen.SnRKCleanOut.Tag)
    {
        RecordProcess("CLEAN OUT pressed");
        EventReport(SECS_EVENT.PressCleanOut);
    }
    else if((bOffBuzzer && bAlarmBuzzer) || (Key==HSys.Sen.SnRKAlarmReset.Tag))
    {
        RecordProcess("ALARM RESET pressed");
        EventReport(SECS_EVENT.PressAlarmReset);
        bAlarmBuzzer=false;
        bLampAlarmReset=false;
        CloseBuzzerOff();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::FlushLabel()                                            // 刷新按鍵 //
{
    TBtnPanel *Ptr[]={BtnSkip, BtnRetry, BtnTrayFeed, BtnTrayEnd, BtnCleanOut, BtnHome};
    bool      *bPtr[6]={&bLampSkip, &bLampRetry, &bLampTrayFeed, &bLampTrayEnd, &bLampCleanOut, &bLampHome};
    static bool OldFlushFlag,f=false;

    if(OldFlushFlag==FlushFlag)
        return;
    OldFlushFlag=FlushFlag;
    f=false;
    for(int i=0; i<6; i++)
    {
        if(Select[i])
        {
            f=true;
            *bPtr[i]=true;
        }
        else
        {
            if(OldFlushFlag)
            {
                Ptr[i]->FalseColor=clBlue;
                if( Ptr[i]->Visible)
                    *bPtr[i]=true;
            }
            else
            {
                Ptr[i]->FalseColor=(TColor)0x00804000;
                *bPtr[i]=false;
            }
        }
    }
    if((f ||(KeyCode==0))&& OldFlushFlag)
    {
        BtnStart->FalseColor=clBlue;
        bLampStart=true;
    }
    else
    {
        BtnStart->FalseColor=(TColor)0x00804000;
        bLampStart=false;
    }
    if((f ||(KeyCode==0))&& OldFlushFlag)
    {
        BtnPause->FalseColor=clBlue;
        bLampPause=true;
    }
    else
    {
        BtnPause->FalseColor=(TColor)0x00804000;
        bLampPause=false;
    }

    if(bAlarmBuzzer)
        bLampAlarmReset=OldFlushFlag;
    else
        bLampAlarmReset=false;
}
//---------------------------------------------------------------------------
void CInitNote()                                                                // 初始化動作,清除第一次開啟的alarm //
{
    static bool flag=true;
    if(flag)
    {
        flag=false;
        fNote->Close();
    }
}
TQPF_Timer tOpenDoorTimer;
//---------------------------------------------------------------------------
void __fastcall TfNote::Timer1Timer(TObject *Sender)                            // ben edit 20110706 //
{
    TBtnPanel *Ptr[]={BtnSkip,BtnRetry,BtnTrayFeed,BtnTrayEnd,BtnCleanOut, BtnHome};
    static int i,kct[6]={0,0,0,0,0,0};
    static bool oldFlushFlag, bSet=false;
    static int iChangeTrayTask=1;
    //char str[256];
    //HWND d;
    //HDC dc;
    if(!bNoteTimer1Check)
    {
        bNoteTimer1Check=true;
        if(!fShow)
        {
            bNoteTimer1Check=false;
            iChangeTrayTask=1;
            return;
        }

        if(fNote->edtAlarmCode->Text=="32361" && bHasChangeNewTray==false && HSys.FuncT.iT02_MagTraySource==1)   //JerryYang 20240623 : Magazine手動補Tray
        {
            PLCIOTaskCycle();
            PLCStatusCheck();       //Sam 20240705 : 修正手動換Tray卡住問題
            PlcComm.Cycle();        //Sam 20240705 : 修正手動換Tray卡住問題
            if((HSys.Sen.SnSafeDoor_02.Enable==true && HSys.Sen.SnSafeDoor_02.IsOn()==false) ||
               (HSys.Sen.SnSafeDoor_03.Enable==true && HSys.Sen.SnSafeDoor_03.IsOn()==false) ||
               (HSys.Sen.SnMGZ2Step.Enable   ==true && HSys.Sen.SnMGZ2Step.IsOff()  ==false))    //Sam 20240705 : 增加 SnMGZ2Step 的隱藏按鈕避免 PLC 掛掉無法解除
            {
                switch(iChangeTrayTask)
                {
                    case 1:
                        if(HSys.Sen.SnMGZ2TopHasTray.IsOn()==false)
                        {
                            if(bSet==false)
                            {
                                tOpenDoorTimer.SetSecAndOn(0.5);
                                bSet=true;
                            }

                            if(tOpenDoorTimer.Off()==true)
                            {
                                iChangeTrayTask=2;
                                bSet=false;
                            }
                            break;
                        }
                        else
                        {
                            bSet=false;
                        }
                    break;
                    case 2:
                        if(HSys.Sen.SnMGZ2TopHasTray.IsOn())
                        {
                            if(bSet==false)
                            {
                                tOpenDoorTimer.SetSecAndOn(0.5);
                                bSet=true;
                            }

                            if(tOpenDoorTimer.Off()==true)
                            {
                                RecordProcess("Place new tray!!");
                                iChangeTrayTask=1;
                                bHasChangeNewTray=true;
                                palNoticeEmptyTray->Visible=false;
                            }
                        }
                        else
                        {
                            bSet=false;
                        }
                    break;
                }
            }
        }
        //jou 2011-10-14 end

        if(Top>=600 || Top<-10 || Left<0 || Left>900)
        {
            Left=0;
            Top=0;
        }
        for(i=0;i<6;i++)
        {
            if(Ptr[i]->Down==false)
            {
                kct[i]=0;
            }
            else
            {
                kct[i]++;
                if(kct[i]>10)
                    Ptr[i]->Down=false;
            }
        }
        FlushLabel();                                                           // 刷新按鍵 //
        ScanKey();                                                              // 偵測實體面板按鍵 //
        DoSystemMessage();                                                      // 三色燈、音樂、按鈕燈 //
        //CheckManualTrayChange();                                              // ben disable 20110720 //
        if(bOffBuzzer)                                                          // off music //
            CloseBuzzerOff();

        if(oldFlushFlag!=FlushFlag)
        {
            oldFlushFlag=FlushFlag;
            if(FlushPanel!=NULL)
            {
                if(FlushFlag)
                    FlushPanel->Color=clRed;
                else
                    FlushPanel->Color=FlushPanelColor;
            }
        }
        //CInitNote();
        bNoteTimer1Check=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::Start()                                                 // do start //
{                                                                               // ben edit 20110720 //
    if(fSetup->fShow)
    {
        bStartFromNote=true;                                                    //2008/07/22 lee
        BtnPauseClick(this);                                                    //2008/07/22 lee
        return;
    }
    //char str[256];
    int  KeyComp[]={K_SKIP ,K_RETRY ,K_TRAY_FEED,K_TRAY_END,K_CLEAN_OUT,K_HOME};
    int i;//,j;

    BtnStart->Down=false;
    for(i=0; i<6; i++)
    {
        if(Select[i])
        {
            ReturnCode=KeyComp[i];
            SoftStart=true;
            SoftStop=false;
            Close();
            return;
        }
    }
    if(KeyCode==0)
    {
        SoftStart=true;
        SoftStop=false;
        ReturnCode=0;
        RecordProcess("START pressed");
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::BtnStartClick(TObject *Sender)                          // button start //
{
    #ifdef SOFT_SIMULATE
        Start();
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfNote::BtnPauseClick(TObject *Sender)                          // button pause~cleanout //
{
    int KeyComp[]={K_SKIP ,K_RETRY ,K_TRAY_FEED,K_TRAY_END,K_CLEAN_OUT, K_HOME};
    int i;//,j;
    BtnPause->Down=false;
    for(i=0; i<6; i++)
    {
        if(Select[i])
        {
            ReturnCode=KeyComp[i];
            SoftStop=true;
            SoftStart=false;
            Close();
            return;
        }
    }

    if(KeyCode==0)
    {
        SoftStop=true;
        ReturnCode=0;
        SoftStart=false;
        Close();
    }
}
//---------------------------------------------------------------------------
void ShowMotorError(AnsiString Code,AnsiString sFunc)                                            // motor error 10000 Start //
{
    AnsiString Mess="",Desc="";
    int iAlarmType;
    if(ProgramFlag==true)
        return;
    if(fNote->fShow)
        return;
    fNote->Memo1->Clear();
    fNote->Memo1->Visible=true;
    iAlarmType=HexStrToInt(Code.SubString(1, 1).c_str());
    FlushPanel=fNote->palSysErr;
    Desc=AnsiString().sprintf("Func:%s",sFunc);
    if(iAlarmType==eJamErr)
    {
        Mess="Jam Code Undefine Error";
    }
    else if(iAlarmType==eMessageErr)
    {
        Mess="Message Code Undefine Error";
    }
    else if(iAlarmType==eFunErr)
    {
        Mess="Function Code Undefine Error";
    }
    else if(iAlarmType==eSystemMess)
    {
        Mess="System Message Code Undefine Error";
    }
    else if(iAlarmType==eCynAlarm)
    {
        Mess="Cylinder Error Code Undefine Error";
    }
    else if(iAlarmType==eMotorAlarm)
    {
        int iMot=atoi(Code.SubString(2, 3).c_str());
        AnsiString sMotName=HSys.MotPtr[iMot]->NumberAlias;
        Mess=AnsiString().sprintf("Motor Error Code，Mot Name=%s",sMotName);
    }
    else if(iAlarmType==eSuckAlarm)
    {
        Mess="Suck Error Code Undefine Error";
    }
    else if(iAlarmType==eRecordProcess)
    {
        Mess="Record Process Code Undefine Error";
    }
    else if(iAlarmType==eOther)
    {
        Mess="Other Code Undefine Error";
    }
    else
    {
        Mess="System Error--System Code Unknown Error";
    }
    fNote->edtAlarmCode->Text=Code;
    fNote->edtAlarmMsg->Text=Mess;                                                    // ErrMessage //
    fNote->Memo1->SetTextBuf(Desc.c_str());                                     // Description //

    MyRecordAlarmMessage R;
    R.AddRecord(Code, Mess);    //得在Form Show之前,才能抓到時間

    fNote->ProcessErrMessage(Code, Mess, iAlarmType);                             // record histroy //
    HSys.DecStopAllMotor();

    SoftStop=false;
    SoftStart=false;
    HSys.Sys.SystemStart=false;

    fNote->KeyCode=K_RETRY;                                                           // 20131018
    fNote->ShowModal();
    fNote->trayPtr=fNote->NullTray;

    fAllMotorHome=false;
    ChangeRunMode(Run_Home);                                                    // set home //

    R.SetProcessData(Recovery, PassTime, 0);
    FormSysTools->RecordAlarmMessage(R);                                        // 錯誤訊息存到log檔 //
}
//------------------------------------------------------------------------------
int ShowSuckError(TMyKitSuck &Ptr, int CodeType, int KCode, AnsiString errPart, int iDuplicate)// need edit by user //
{
    AnsiString S, SuckName, Event="", SuckErrorCode;
    if(ProgramFlag==true)
        return 0;
    if(fNote->fShow)
        return 0;
    SuckName=Ptr.Name;
    HSys.DecStopAllMotor();
    fNote->KeyCode=KCode;
    fNote->Memo1->Clear();

    SuckErrorCode.sprintf("%d%03d%d", (int)eSuckAlarm, Ptr.Tag, CodeType);

    fNote->GetFlushPanel(fNote->PanelMain6, Ptr.FlushPanelName);

    if(CodeType==Vaccum_On)
    {
        Event=AnsiString("--Device pick up error");
    }
    else if(CodeType==Vaccum_Off)
    {
        Event=AnsiString("--Vacuum sensor OFF error");
    }
    else if(CodeType==Vaccum_FallDown)
    {
        Event=AnsiString("--Device be droped error");
    }
    else if(CodeType==Vaccum_Initial_Off)
    {
        Event=AnsiString("--Initial start sensor off error");
    }
    else if(CodeType==Vaccum_Initial_On)
    {
        Event=AnsiString("--Initial start sensor on error");
    }
    else
    {
        Event=AnsiString("--Vacuum undefine error");
    }

    if(errPart=="")
        errPart=" ";

    if(iDuplicate!=0)
        S.sprintf("%s %s at %s, (Again!)", SuckName, Event, errPart);
    else
        S.sprintf("%s %s at %s", SuckName, Event, errPart);

    fNote->edtAlarmMsg->Text=S;
    fNote->edtAlarmCode->Text=SuckErrorCode;
    if(HSys.LastSet.iLanguageCountry==1)
    {
        if(CodeType==Vaccum_On)
        {
            fNote->Memo1->Lines->Add("[2] 真空感測器無開啟,調整真空感測器,檢查線路及管路");
            fNote->Memo1->Lines->Add("[3] 吸嘴負極限太高,調整極限");
            fNote->Memo1->Lines->Add("[4] 吸嘴上下速度太快,調整速度");
            fNote->Memo1->Lines->Add("[5] 無真空(壓力太弱)    -確認產生器之功能");
            fNote->Memo1->Lines->Add("[6] 確認電磁閥之功能。    -檢查管路");
            fNote->Memo1->Lines->Add("[7] IC沒放好。    -如為腳彎，請移除IC後再按SKIP忽略繼續");
            fNote->Memo1->Lines->Add("    如IC完好，請放好IC後再按 RETRY 繼續");
            fNote->Memo1->Lines->Add("[8] 吸取時間太短。按PAUSE暫停,通知工程師加長吸取時間,重新啟動機台");
        }
        else if(CodeType==Vaccum_Off)
        {
            fNote->Memo1->Lines->Add("[1] 真空感測器一直為開啟狀態,調整感測器");
            fNote->Memo1->Lines->Add("[2] 吹氣量太弱    -確認吹氣量");
            fNote->Memo1->Lines->Add("[3] 確認電磁閥之功能。    -檢查管路");
        }
        else if(CodeType==Vaccum_FallDown)
        {
            fNote->Memo1->Lines->Add("[1] 檢查是否阻礙");
            fNote->Memo1->Lines->Add("[2] 真空感測器不正常-調整真空感測器及檢查線路");
            fNote->Memo1->Lines->Add("[3] 確認真空感測器");
        }
        else if(CodeType==Vaccum_Initial_On)
        {
            fNote->Memo1->Lines->Add("[1] Iinital 時發生感測器一直為開啟狀態,調整感測器");
            fNote->Memo1->Lines->Add("[2] 真空感測器不正常-調整真空感測器及檢查線路");
            fNote->Memo1->Lines->Add("[3] 確認電磁閥之功能。    -檢查管路");
        }
        else if(CodeType==Vaccum_Initial_Off)
        {
            fNote->Memo1->Lines->Add("[1] Iinital 時發生感測器一直為關閉狀態,調整感測器");
            fNote->Memo1->Lines->Add("[2] 真空感測器不正常-調整真空感測器及檢查線路");
            fNote->Memo1->Lines->Add("[3] 確認電磁閥之功能。    -檢查管路");
        }
    }
    else
    {
        fNote->Memo1->Lines->Add("[1] No compoment under position");
        fNote->Memo1->Lines->Add("[2] Adjust the vacuum sensor");
        fNote->Memo1->Lines->Add("[3] Check wire and pipe");
        fNote->Memo1->Lines->Add("[4] Adjust pick height");
        fNote->Memo1->Lines->Add("[5] Check parameter of vacuum ");
    }
    SoftStop=false;
    SoftStart=false;

    MyRecordAlarmMessage R;
    R.AddRecord(SuckErrorCode, S);                                              //得在Form Show之前,才能抓到時間

    iDuplicateError=iDuplicate;
    fNote->ProcessErrMessage(SuckErrorCode, S, CodeType);                       // record histroy //
    fNote->ShowModal();
    fNote->trayPtr=fNote->NullTray;

    R.SetProcessData(Recovery, PassTime, iDuplicateError);
    FormSysTools->RecordAlarmMessage(R);                                        // 錯誤訊息存到log檔 //

    return fNote->ReturnCode;
}
//------------------------------------------------------------------------------ // need edit by user //
int ShowSystemError(AnsiString Name, int KCode, int iDuplicate, AnsiString Message)                                     // ben edit 20110723 //
{
    int iAlarmType;
    AnsiString PanelName, Code, Mess, Desc;
    if(ProgramFlag==true)
        return 0;
    if(fNote->fShow)
        return 0;
    SoftStop=false;
    SoftStart=false;
    HSys.Sys.SystemStart=false;
    HSys.DecStopAllMotor();
    fNote->KeyCode=KCode;
    fNote->Memo1->Clear();
    fNote->Memo1->Visible=true;
    fNote->trayPtr=fNote->NullTray;
    Code=HSys.mapNameToAlarm[Name];
    HSys.IterAlarmCodeList=HSys.mapAlarmCodeList.find(Code);
    AnsiString sRec=AnsiString().sprintf("Name:%s KCode:%d iDuplicate:%d Message:%s ",Name,KCode,iDuplicate,Message);
    if(HSys.IterAlarmCodeList==HSys.mapAlarmCodeList.end())
    {
        Code="-"+Code;
        Desc=AnsiString().sprintf("Func:%s",sRec);
        iAlarmType=HexStrToInt(Code.SubString(1, 1).c_str());
        FlushPanel=fNote->palSysErr;
        if(iAlarmType==eJamErr)
        {
            Mess="Jam Code Undefine Error";
        }
        else if(iAlarmType==eMessageErr)
        {
            Mess="Message Code Undefine Error";
        }
        else if(iAlarmType==eFunErr)
        {
            Mess="Function Code Undefine Error";
        }
        else if(iAlarmType==eSystemMess)
        {
            Mess="System Message Code Undefine Error";
        }
        else if(iAlarmType==eCynAlarm)
        {
            Mess="Cylinder Error Code Undefine Error";
        }
        else if(iAlarmType==eMotorAlarm)
        {
            Mess="Motor Error Code Undefine Error";
        }
        else if(iAlarmType==eSuckAlarm)
        {
            Mess="Suck Error Code Undefine Error";
        }
        else if(iAlarmType==eRecordProcess)
        {
            Mess="Record Process Code Undefine Error";
        }
        else if(iAlarmType==eOther)
        {
            Mess="Other Code Undefine Error";
        }
        else
        {
            Mess="System Error--System Code Unknown Error";
        }
    }
    else
    {
        PanelName =HSys.IterAlarmCodeList->second.FlushPanelName;
        fNote->GetFlushPanel(fNote->PanelMain6, PanelName);
        if(HSys.LastSet.iLanguageCountry==0)
        {
            Mess=HSys.IterAlarmCodeList->second.E_ErrMessage;
            Desc=HSys.IterAlarmCodeList->second.E_Description;
        }
        else
        {
            Mess=HSys.IterAlarmCodeList->second.C_ErrMessage;
            Desc=HSys.IterAlarmCodeList->second.C_Description;
        }
        Desc+=AnsiString().sprintf("Func:%s",sRec);
    }

    fNote->edtAlarmCode->Text=Code;                                                    // Code //
    if(iDuplicate!=0)
        fNote->edtAlarmMsg->Text=StringReplace(Mess, "\"", "", TReplaceFlags()<<rfReplaceAll)+" (Again!)";                                    // ErrMessage //
    else
        fNote->edtAlarmMsg->Text=StringReplace(Mess, "\"", "", TReplaceFlags()<<rfReplaceAll);                                                // ErrMessage //

    Desc=StringReplace(Desc, "\\r\\n", "\r\n", TReplaceFlags()<<rfReplaceAll);
    Desc=StringReplace(Desc, "\"", "", TReplaceFlags()<<rfReplaceAll);
    fNote->Memo1->SetTextBuf(Desc.c_str());                                     // Description //
    fNote->Memo1->Lines->Add(Message);

    MyRecordAlarmMessage R;
    R.AddRecord(Code, Mess);                                                    //得在Form Show之前,才能抓到時間

    iDuplicateError=iDuplicate;
    fNote->ProcessErrMessage(Code, Mess, iAlarmType);
    fNote->ShowModal();
    fNote->trayPtr=fNote->NullTray;

    R.SetProcessData(Recovery, PassTime, iDuplicateError);
    FormSysTools->RecordAlarmMessage(R);                                        // 錯誤訊息存到log檔 //

    return fNote->ReturnCode;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::ProcessErrMessage(AnsiString AlarmCode, AnsiString Str, int Type)      //type:0 Jam  type:1 error type2:motion
{
    static AnsiString S,S1,S2,TS="";
    static bool bBusy=false;

    if(bBusy==true)
        return;
    if(Str=="" || AlarmCode=="")
        return;
    if(AlarmCode=="")
        return;

    bBusy=true;
    if(Type==0 || Type==1)                                                      // jam count //
    {
        for(int i=0; i<3; i++)
        {
            HSys.LastSet.iJamCount[i]++;
        }
    }
    if(Type>=0 && Type<=9)                                                      // ben count all error //
    {
        HSys.LastSet.iErrorCount[Type]++;
    }
    bBusy=false;
}
//==============================================================================
void __fastcall TfNote::LevelProcessErrMessage()      //type:0 Jam  type:1 error type2:motion
{
}
//==============================================================================
AnsiString GetRefrenceCode(AnsiString S)                                        // ben add 20110718 //
{
    if(S.Pos("Program Start")!=0)
        S="Program Start";

    if(HSys.mapNameToAlarm[S]=="")
        return "No Code";
    else
        return HSys.mapNameToAlarm[S];
}
//---------------------------------------------------------------------------
void RecordProcess(AnsiString S)                                                // ben edit 20110718 //
{
    static AnsiString S1="";
    AnsiString Code;
    if(S==S1 && S.Pos("Continus fail ararm==")==1)
        return;

    if(S==S1 && S.Pos(" pressed")!=0)                                           //2008/06/24 lee avoid same record repeat be record
        return;

    S1=S;
    Code=GetRefrenceCode(S);
    MyRecordAlarmMessage R;
    R.AddRecord(Code, S);
    FormSysTools->RecordAlarmMessage(R);                                        // 錯誤訊息存到log檔 //

    fNote->ProcessErrMessage(Code, S, mess_fuc);
    S1=S;
}
//---------------------------------------------------------------------------
void __fastcall MyDBIProcess(AnsiString asTable, AnsiString S, AnsiString subMess)  //Steven 202111016 : copy from 9045
{
    MyRecordAlarmMessage R;
    R.AddRecord(asTable, S, subMess);                                           //得在Form Show之前,才能抓到時間
    FormSysTools->RecordAlarmMessage(R);
}
//---------------------------------------------------------------------------
void LevelRecordProcess()
{
}
//---------------------------------------------------------------------------
void __fastcall TfNote::BtnOffBuzzerClick(TObject *Sender)                      // ben add 20110706 //
{
    bOffBuzzer=true;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::BtnSkipClick(TObject *Sender)
{
    UpdateButtonStatus(Sender);
}
//---------------------------------------------------------------------------
void TfNote::GetFlushPanel(TWinControl *PCtrl, AnsiString PanelName)
{
    AnsiString S="",S1="";
    TPanel *PanelPtr=dynamic_cast <TPanel *>(PCtrl);

    if(PanelPtr!=NULL)
    {
        if(PanelPtr->Name==PanelName)
        {
            FlushPanel=PanelPtr;
            return;
        }

        for(int iP=0; iP<PCtrl->ControlCount; iP++)
        {
            TControl *P=PCtrl->Controls[iP];
            if(dynamic_cast <TWinControl *>(P)!=NULL)
            {
                GetFlushPanel((TWinControl *) P, PanelName);
            }
        }
    }
}
//---------------------------------------------------------------------------
