//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "mymessbox.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//============================================================================//
//   共用變數區                                                               //
//============================================================================//
TMyMessageBox *MyMessageBox;
bool bCanShow=false;
int iProgramRunTime=20;
bool bDisableKeypad=false;
//==============================================================================
__fastcall TMyMessageBox::TMyMessageBox(TComponent* Owner)
    : TForm(Owner)
{
    bCanShow=true;
    fShow=false;
}
//==============================================================================
AnsiString LoadLanguageString(char *str,int type)
{
    AnsiString ret;
    AnsiString S;
    TIniFile *INIFile = new TIniFile(HSys.CurrentDir+"\\system\\language.ini");
    S=str;
    if(type==0)
        S+="_English";
    else
        S+="_Chinese";
    ret=INIFile->ReadString("MESSAGEBOX",S,"NULL_STRING");
    if(ret=="NULL_STRING")
    {
        S=str;
        S+="_English";
        INIFile->WriteString("MESSAGEBOX",S,str);
        S=str;
        S+="_Chinese";
        INIFile->WriteString("MESSAGEBOX",S,str);
        ret=str;
    }
    delete INIFile;
    return ret;
}
//==============================================================================
void ShowMyMessage(AnsiString S)
{
    AnsiString S1;
    if(bCanShow==false)
        return;
    if(MyMessageBox->fShow==true)
        return;
    bHandlerPause=true;
    iHandlerStartCount=0;
    HSys.DecStopAllMotor();
//    HSys.StopAllMotor();
    S1=LoadLanguageString(S.c_str(), HSys.LastSet.iLanguageCountry);

    MyMessageBox->Width=480;
    MyMessageBox->Height=255;
    MyMessageBox->palPause->Left=168;
    MyMessageBox->palPause->Top=178;
    MyMessageBox->Label2->Visible=false;
    MyMessageBox->Label1->Caption=S1;
//    MyMessageBox->Label2->Caption=str2;
    MyMessageBox->palPause->Caption="Pause";
    MyMessageBox->fScanPanel=true;
    MyMessageBox->ShowModal();
}
//==============================================================================
//void ShowMyMessage(char *str)
//{
//    if( bCanShow ==false || iProgramRunTime<12)
//        return;
//    if(MyMessageBox->fShow==true)
//        return;
//
//    MyMessageBox->Width=480;
//    MyMessageBox->Height=255;
//    MyMessageBox->palPause->Left=168;
//    MyMessageBox->palPause->Top=178;
//    HSys.StopAllMotor();
//    char str1[256]="",str2[256]="";
//    int pos;
//    unsigned ct=0,i;
//    HSys.StopAllMotor();
//    for(i=0;i<strlen(str);i++)
//    {
//        if(str[i]=='#')
//        {
//            ct=i;
//            break;
//        }
//    }
//    if(ct)
//    {
//        for(i=0;i<ct;i++)
//            str1[i]=str[i];
//        str1[ct]=0;
//        for(i=ct+1;i<strlen(str);i++)
//        {
//            pos=i-ct-1;
//            str2[pos]=str[i];
//            str2[pos+1]=0;
//        }
//        MyMessageBox->Label1->Caption=str1;
//        MyMessageBox->Label2->Caption=str2;
//    }
//    else
//    {
//        MyMessageBox->Label1->Caption=str;
//        MyMessageBox->Label2->Caption="";
//    }
//    MyMessageBox->palPause->Caption="Pause";
//    MyMessageBox->fScanPanel=true;
//    MyMessageBox->ShowModal();
//}
//==============================================================================
void ShowMyMessage_Run(AnsiString S1, AnsiString S2)
{
    if(MyMessageBox->fShow==true)
        return;
    MyMessageBox->palPause->Caption="Alarm Reset";
    MyMessageBox->Label1->Caption=S1;
    MyMessageBox->Label2->Caption=S2;
//    MyDBIProcess("Message", S1);
    MyMessageBox->fScanPanel=true;
    MyMessageBox->bFormShowNoStop=true;
    MyMessageBox->Show();
}
//==============================================================================
//void ShowMyMessage(int Code)                                                    // ben add 20110706 //
//{
//    if( bCanShow ==false || iProgramRunTime<12)
//        return;
//
//    if(MyMessageBox->fShow==true)
//        return;
//
//    HSys.StopAllMotor();
//    MyMessageBox->GetNodeCode(Code);
//    if( HSys.LastSet.iLanguageCountry==0 )
//        ShowMyMessage(MyMessageBox->ENMessage);
//    else
//        ShowMyMessage(MyMessageBox->CHMessage);
//}
//==============================================================================
//void ShowMyOKMessage(char *str)
//{
//    int i,pos;
//    AnsiString S;
//    if( bCanShow ==false || iProgramRunTime<12)
//        return;
//    if(MyMessageBox->fShow==true)
//        return;
//    MyMessageBox->Panel1->Visible=true;
//    MyMessageBox->Width=480;
//    MyMessageBox->Height=255;
//    MyMessageBox->palPause->Left=168;
//    MyMessageBox->palPause->Top=178;
//    HSys.StopAllMotor();
//    S=LoadLanguageString(str,HSys.LastSet.iLanguageCountry);
//    MyMessageBox->Label1->Caption=S;
//
//    if( HSys.LastSet.iLanguageCountry==1 )
//        MyMessageBox->palPause->Caption="確定";
//    else
//        MyMessageBox->palPause->Caption="OK";
//    MyMessageBox->fScanPanel=false;
//    MyMessageBox->ShowModal();
//}
//==============================================================================
//void ShowMyOKMessage(int Code)                                                  // ben add 20110706 //
//{
//    if( bCanShow ==false || iProgramRunTime<12)
//        return;
//
//    if(MyMessageBox->fShow==true)
//        return;
//
//    MyMessageBox->GetNodeCode(Code);
//    if(HSys.LastSet.iLanguageCountry==0)
//        ShowMyOKMessage(MyMessageBox->ENMessage);
//    else
//        ShowMyOKMessage(MyMessageBox->CHMessage);
//}
//==============================================================================
void __fastcall TMyMessageBox::palPauseClick(TObject *Sender)
{
    Close();
    Now_Status_IsOneCycle=false;
}
//==============================================================================
void __fastcall TMyMessageBox::Timer1Timer(TObject *Sender)                     // ben edit 20110706 //
{
    if(!bMessageTimer1Check)
    {
        bMessageTimer1Check=true;

        if(fShow==false)
        {
            bMessageTimer1Check=false;                                          //KenHsieh 20221122 : 修改return後不會進入之問題
            return;
        }

        if(HSys.Sys.RunMode==Run_TrayFeed &&
           MagArmPara->iMagStatus==MagArmPara->eHS_MAG_SafePos)                 //KenHsieh 20240528 : 手動Magazine Tray Feed
        {
            if(MagazineModule1->TrayFeedFinish()==false)
                MagazineModule1->ManualStepBtnCanLight();

            if(MagazineModule2->TrayFeedFinish()==false)
                MagazineModule2->ManualStepBtnCanLight();

            if(MagazineModule3->TrayFeedFinish()==false)
                MagazineModule3->ManualStepBtnCanLight();
        }

        if(bDisableKeypad==false)
        {
            ret=ScanPannelKey();
            if(ret==HSys.Sen.SnFKPause.Tag ||
               ret==HSys.Sen.SnRKPause.Tag)
            {
                MyMessageBox->Close();
            }
            else if(ret==HSys.Sen.SnRKAlarmReset.Tag)                           //Steven 20240707 : Turn off buzzer of message
            {
                bOffBuzzer=true;
            }
            else if(HSys.Sys.RunMode==Run_TrayFeed &&
                    MagArmPara->iMagStatus==MagArmPara->eHS_MAG_SafePos)
            {
                if(ret==HSys.Sen.SnMGZ1Step.Tag &&
                   MagazineModule1->TrayFeedFinish()==false)
                {
                     MyMessageBox->Close();
                }

                if(ret==HSys.Sen.SnMGZ2Step.Tag &&
                   MagazineModule2->TrayFeedFinish()==false)
                {
                     MyMessageBox->Close();
                }

                if(ret==HSys.Sen.SnMGZ3Step.Tag &&
                   MagazineModule3->TrayFeedFinish()==false)
                {
                     MyMessageBox->Close();
                }
            }
        }
        if(bOffBuzzer)
            CloseBuzzerOff();
        bLampPause=FlushFlag;
        DoSystemMessage();

        bMessageTimer1Check=false;
    }
}
//==============================================================================
void __fastcall TMyMessageBox::FormShow(TObject *Sender)                        // ben edit 20110706 /
{
    Left=(fMain->Width -Width)/2;
    Top =(fMain->Height -Height)/2;

    AnsiString Str;
    if(MyMessageBox->bFormShowNoStop==false)
    {
        HSys.DecStopAllMotor();
    }

    if(MyMessageBox->bFormShowNoStop==false)
    {
        HSys.Sys.SystemStart=false;
    }
    MyMessageBox->fShow=true;

    bPauseMagMotor=true;
    bPauseSortMotor=true;

    EventReport(SECS_EVENT.ShowMessage);
    RecordProcess(Label1->Caption);
    bFormShowNoStop=false;
    if(fTeach->fShow==true)
    {
        fTeach->sbMove->Down=false;
        fTeach->sbMoveP->Down=false;
        fTeach->sbMoveN->Down=false;
    }
}
//==============================================================================
void __fastcall TMyMessageBox::FormClose(TObject *Sender,                       // ben edit 20110706 //
      TCloseAction &Action)
{
    MyMessageBox->fShow=false;
    bOffBuzzer=false;
    for(int i=0; i<4; i++)                                                      // ben 20111026 //
        HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].Off();
    EventReport(SECS_EVENT.ReleaseMessage);
    if(bSECSGEMAlarm)
        bSECSGEMAlarm=false;
}
//==============================================================================
//==============================================================================
//input -> str : Message
//output-> iValue : 1->Yes 2->No 3->Cancel
//---------------------------------------------------------------------------
int iValue =0;
void __fastcall TMyMessageBox::palYesClick(TObject *Sender)
{
    TPanel *P;
    P=(TPanel *)Sender;
    iValue = P->Tag;
    Close();
}
//---------------------------------------------------------------------------
int ShowMyMessageBox_YES_NO(AnsiString str)
{
    AnsiString S;
    if(MyMessageBox->fShow==true)
        return 3;
    HSys.DecStopAllMotor();
    bDisableKeypad=true;
//--
    MyMessageBox->Panel1->Visible=true;
    MyMessageBox->Width=480;
    MyMessageBox->Height=255;
    MyMessageBox->palPause->Visible=false;
    MyMessageBox->palYes->Visible=true;
    MyMessageBox->palNo->Visible=true;

    MyMessageBox->Label1->Caption=str;
    MyMessageBox->Label2->Visible=false;
    MyMessageBox->fScanPanel=false;
    MyMessageBox->ShowModal();

    MyMessageBox->palPause->Visible=true;
    MyMessageBox->palYes->Visible=false;
    MyMessageBox->palNo->Visible=false;
//--
    bDisableKeypad=false;
    return iValue;
}
//---------------------------------------------------------------------------
void CloseBuzzerOff()                                                           // ben 20110706 close buzzer //
{
    for(int i=0;i<4;i++)
        HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].Off();
}
//==============================================================================
void __fastcall TMyMessageBox::Button2Click(TObject *Sender)                    // ben 20110706 close buzzer //
{
    bOffBuzzer=true;

    for(int i=0;i<4;i++)
        HSys.SwPtr[HSys.Sw.SwMusic1.Tag+i].Off();
}
//==============================================================================
void __fastcall TMyMessageBox::Button1Click(TObject *Sender)                    // ben add 20110706 change language //
{
    if(MyMessageBox->Label1->Caption==MyMessageBox->CHMessage)
        MyMessageBox->Label1->Caption=MyMessageBox->ENMessage;
    else
        MyMessageBox->Label1->Caption=MyMessageBox->CHMessage;
}
//==============================================================================

