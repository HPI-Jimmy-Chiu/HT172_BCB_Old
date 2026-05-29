
//==============================================================================
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "uMotorTest.h"
#pragma package(smart_init)
#pragma link "ALed"
#pragma resource "*.dfm"
TfMotorTest *fMotorTest;
//==============================================================================
static HTimer  LoopWait;
//==============================================================================
__fastcall TfMotorTest::TfMotorTest(TComponent* Owner)
    : TForm(Owner)
{
    Width=1332;
    Height=1032;
    ActiveIndex=-1;
    char str[256];
    coboSourceMotorIndex->Clear();
    for(int i=0; i<MAX_MOTOR_TEST_COUNT; i++)
    {
        sprintf(str, "M%02d", i);
        if(i<HSys.iTotalMotor)
            coboSourceMotorIndex->Items->Add(str);

        MNameLab[i]=new TLabel(this);
        MNameLab[i]->Name="lbMotorName"+AnsiString(i);
        MNameLab[i]->Parent=palMotorSelect;
        MNameLab[i]->Font->Name="MS Sans Serif";
        MNameLab[i]->Font->Size=12;
        MNameLab[i]->Caption=str;
        MNameLab[i]->Tag=i;
        MNameLab[i]->OnClick=lM00Click;
        MNameLab[i]->Left=38+200*(i/10);                                        //推Tray 機構改為馬達
        MNameLab[i]->Top =28+27*(i%10);
        if(i>=HSys.iTotalMotor)
            MNameLab[i]->Visible=false;

        AllMotLoopMove[i]=new TCheckBox(this);
        AllMotLoopMove[i]->Name="cbMotorSelect"+AnsiString(i);
        AllMotLoopMove[i]->Parent=palMotorSelect;
        AllMotLoopMove[i]->Left=16+200*(i/10);                                  //推Tray 機構改為馬達
        AllMotLoopMove[i]->Top =29+27*(i%10);
        AllMotLoopMove[i]->Caption="";
        AllMotLoopMove[i]->Width=15;
        if(i>=HSys.iTotalMotor)
            AllMotLoopMove[i]->Visible=false;

        MPos1[i]=new TEdit(this);
        MPos1[i]->Name="edLoopPos1"+AnsiString(i);
        MPos1[i]->Parent=palMotorSelect;
        MPos1[i]->Font->Name="MS Sans Serif";
        MPos1[i]->Font->Size=12;
        MPos1[i]->Tag=i;
        MPos1[i]->OnClick=lpA00Click;
        MPos1[i]->Left=78+200*(i/10);                                           //推Tray 機構改為馬達
        MPos1[i]->Top =26+27*(i%10);
        MPos1[i]->Width=62;
        MPos1[i]->Height=24;
        MPos1[i]->PopupMenu=PopupMenuMotorPositionClear;

        if(i>=HSys.iTotalMotor)
            MPos1[i]->Visible=false;

        MPos2[i]=new TEdit(this);
        MPos2[i]->Name="edLoopPos2"+AnsiString(i);
        MPos2[i]->Parent=palMotorSelect;
        MPos2[i]->Font->Name="MS Sans Serif";
        MPos2[i]->Font->Size=12;
        MPos2[i]->Tag=i;
        MPos2[i]->OnClick=lpA00Click;
        MPos2[i]->Left=143+200*(i/10);                                          //推Tray 機構改為馬達
        MPos2[i]->Top =26+27*(i%10);
        MPos2[i]->Width=62;
        MPos2[i]->Height=24;
        MPos2[i]->PopupMenu=PopupMenuMotorPositionClear;
        if(i>=HSys.iTotalMotor)
            MPos2[i]->Visible=false;

        AllMotMoveFlag[i]=true;
    };

    if(0<HSys.iTotalMotor && HSys.iTotalMotor<11)                               //KenHsieh 20240808 : Fix 顯示  // ben 20110704 - s //
    {
        Label8->Visible=true;
    }
    else if(0<HSys.iTotalMotor && HSys.iTotalMotor<21)                          //KenHsieh 20240808 : Fix 顯示
    {
        Label8->Visible=true;
        Label9->Visible=true;
    }
    else if(0<HSys.iTotalMotor && HSys.iTotalMotor<31)                          //KenHsieh 20240808 : Fix 顯示
    {
        Label8->Visible=true;
        Label9->Visible=true;
        Label10->Visible=true;
    }
    else if(0<HSys.iTotalMotor && HSys.iTotalMotor<41)                          //KenHsieh 20240808 : Fix 顯示
    {
        Label8->Visible=true;
        Label9->Visible=true;
        Label10->Visible=true;
        Label11->Visible=true;
    }                                                                           // ben 20110704 - e //
    else if(0<HSys.iTotalMotor && HSys.iTotalMotor<51)                          //推Tray 機構改為馬達
    {
        Label8->Visible=true;
        Label9->Visible=true;
        Label10->Visible=true;
        Label11->Visible=true;
        Label3->Visible=true;
    }

    SelMotSpeed=0;
    coboMoveInterval->ItemIndex=0;
    coboSourceMotorIndex->ItemIndex=0;
    #ifdef SOFT_SIMULATE
        spbTerminalProgram->Visible=true;
    #endif
    MotorInformationGrid->Cells[0][0]="Card Index";
    MotorInformationGrid->Cells[1][0]="Address";
    MotorInformationGrid->Cells[2][0]="Motor Name";

    strngrdMotor->ColWidths[0] = strngrdMotor->DefaultColWidth+20;
    strngrdMotor->ColWidths[1] = strngrdMotor->DefaultColWidth-20;

//    bStartReadMotorReg=false;
//    bStartWriteMotorReg=false;
    bMotorParameterChange=false;
}
//==============================================================================
void __fastcall TfMotorTest::DoLoopMoveMulti()                                  // 多軸loop //
{
    int p,Wait;//,nowPos;
    DWORD Average;
    int &Task=LoopTask;
    bool AllMot_Flag;
    AnsiString S;

    switch(Task)
    {
        case 1:
            AllMot_Flag=true;
            for(int i=0; i<HSys.iTotalMotor; i++)
            {
                if(AllMotLoopMove[i]->Checked)
                {
                    p=atoi(MPos1[i]->Text.c_str());
                    AllMotMoveFlag[i]=HSys.MotPtr[i]->MotorMove(p);

                }
                else
                {
                    AllMotMoveFlag[i]=true;
                }
                if(AllMotMoveFlag[i]==false)
                    AllMot_Flag=false;
            }
            if(AllMot_Flag)
            {
                Wait=atoi(coboWaitTime->Text.c_str());
                if(Wait>100 || Wait<=0)
                {
                    Task=50;
                    break;
                }
                else
                {
                    Task=10;
                    LoopWait.Clear();
                    LoopWait.Set(Wait);
                    LoopWait.On();
                    break;
                }
            }
            break;
        case 10:
            if(LoopWait.Off())
                Task=50;
            break;
        case 50:
            AllMot_Flag=true;
            for(int i=0; i<HSys.iTotalMotor; i++)
            {
                if(AllMotLoopMove[i]->Checked)
                {
                    p=atoi(MPos2[i]->Text.c_str());
                    AllMotMoveFlag[i]=HSys.MotPtr[i]->MotorMove(p);
                }
                else
                {
                    AllMotMoveFlag[i]=true;
                }
                if(AllMotMoveFlag[i]==false)
                    AllMot_Flag=false;
            }

            if(AllMot_Flag)
            {
                Wait=atoi(coboWaitTime->Text.c_str());
                dwLoopCount++;
                dwNowTickCount=GetTickCount();
                lbTotalLoopCount->Caption=dwLoopCount;
                Average=(dwNowTickCount-dwStartTickCount)/dwLoopCount;

                S.printf("%7.3f sec",Average/1000.0);
                lbAverageLoopTime->Caption=S;
                if(Wait>100 || Wait<=0)
                {
                    Task=1;
                    break;
                }
                else
                {
                    Task=60;
                    LoopWait.Clear();
                    LoopWait.Set(Wait);
                    LoopWait.On();
                    break;
                }
            }
            break;
        case 60:
            if(LoopWait.Off())
                Task=1;
            break;
    }
}
//==============================================================================
void __fastcall TfMotorTest::DoLoopMoveSingle()                                 // 單軸loop //
{
    int p;
    DWORD Average;
    int &Task=LoopTask;
    static DWORD Start;
    AnsiString S;
    static int LoopWaitTime;
    double Wait;
    StartSingleLoop:

    switch(Task)
    {
        case 1:
            p=atoi(MPos1[ActiveIndex]->Text.c_str());
            if( HSys.MotPtr[ActiveIndex]->MotorMove(p))                         // move to start
            {
                dwStartTickCount=GetTickCount();
                Start           =GetTickCount();
                dwLoopCount=0;
                Task=2;
            }
            else
            {
                break;
            }
        case 2:
            p=atoi(MPos2[ActiveIndex]->Text.c_str());
            if( HSys.MotPtr[ActiveIndex]->MotorMove(p))
            {
                S.printf("%7.3f sec",(GetTickCount()-Start)/1000.0);
                lbTimeOfOneTrack->Caption=S;
                Wait=atof( coboWaitTime->Text.c_str());
                if(Wait>100 || Wait<=0)
                    Wait=0;
                LoopWaitTime=Wait*1000.0;
                LoopWait.SetMSAndOn(LoopWaitTime);
                Task=10;
                break;
            }
            else
                break;
        case 10:
            if(LoopWait.Off())
            {
                Start=GetTickCount();
                Task=50;
            }
            else
            {
                break;
            }
        case 50:
            p=atoi( MPos1[ActiveIndex]->Text.c_str());
            if( HSys.MotPtr[ActiveIndex]->MotorMove(p))
            {
                S.printf("%7.3f sec",(GetTickCount()-Start)/1000.0);
                lbTimeOfOneTrack->Caption=S;
                dwLoopCount++;
                lbTotalLoopCount->Caption=dwLoopCount;
                Average=(GetTickCount()-dwStartTickCount)/dwLoopCount;
                S.printf("%7.3f sec",Average/1000.0);
                lbAverageLoopTime->Caption=S;
                Wait=atof( coboWaitTime->Text.c_str());
                if(Wait>100 || Wait<=0)
                    Wait=0;
                LoopWaitTime=Wait*1000.0;
                LoopWait.SetMSAndOn(LoopWaitTime);
                Task=60;
                break;
            }
            else
            {
                break;
            }
        case 60:
            if(LoopWait.Off())
            {
                Task=2;
                Start=GetTickCount();
                goto StartSingleLoop;
            }
            else
            {
                break;
            }
    }
}
//==============================================================================
void __fastcall TfMotorTest::DoLoopMove()                                       // check loop type //
{
    if( radSingle->Checked )
        DoLoopMoveSingle();
    else
        DoLoopMoveMulti();
}
//==============================================================================
void __fastcall TfMotorTest::UpdateMotorLed()                                   // 更新motor status //
{
    TALed *P[]={ledCW, ledHome, ledCCW, ledEmg, ledAlarm, ledSoftCW, ledSoftCCW, ledServoAlarm, ledInPos, ledZPhase, ledServoOn};
    int i;

    if(ActiveIndex==-1)
    {
        for(i=0; i<iMotLedTotalCnt; i++)
            P[i]->Value=false;
        return;
    }
    HSys.MotPtr[ActiveIndex]->ScanMotorStatus();
    for(i=0; i<iMotLedTotalCnt; i++)
        P[i]->Value=HSys.MotPtr[ActiveIndex]->Led[i];

    if(ledAlarm->Value==true && fNote->fShow)
    {
        HSys.MotPtr[ActiveIndex]->DecStop();
        HSys.MotPtr[ActiveIndex]->bHomeFlag=false;
        if(spbLoop->Down )
            spbLoop->Down=false;
        if( spbHome->Down)
            spbHome->Down=false;
    }
}
//==============================================================================
void __fastcall TfMotorTest::ShowMotorSelect(int Index, int Attr)               // 顯示選擇的馬達資訊 //
{
    //char MName[]={"M00:"};
    //MName[1]='0'+(Index+1)/10;
    //MName[2]='0'+(Index+1)%10;
    AnsiString MName;
    MName.sprintf("M%02d:%s", Index, HSys.MotPtr[Index]->Alias);
    palActiveMotor->Caption=MName;
    if(Attr)
    {
        MNameLab[Index]->Font->Color=clRed;
        MPos1[Index]->Font->Color=clWhite;
        MPos2[Index]->Font->Color=clWhite;
        MPos1[Index]->Color=clRed;
        MPos2[Index]->Color=clRed;
    }
    else
    {
        MNameLab[Index]->Font->Color=clBlack;
        MPos1[Index]->Font->Color=clBlack;
        MPos2[Index]->Font->Color=clBlack;
        MPos1[Index]->Color=clWindow;
        MPos2[Index]->Color=clWindow;
    }

    //unsigned Speed;
    //int Max,Min,t;
    ScrollBar1->Position    =HSys.MotPtr[Index]->GetPersentSpeed();
    edActiveMotorSpeed->Text=HSys.MotPtr[Index]->GetSpeed();
    Label14->Caption        =AnsiString(ScrollBar1->Position)+AnsiString(" %");
    HSys.MotPtr[ActiveIndex]->SetPersentSpeed(ScrollBar1->Position);
    FindMotorParameter(Index);
    UpdateMotorParameter();
}
//==============================================================================
void __fastcall TfMotorTest::lM00Click(TObject *Sender)                         // motor label select //
{
    TLabel *Ptr;
    Ptr=(TLabel *)Sender;

    #ifndef SOFT_SIMULATE
    if(HSys.MotPtr[Ptr->Tag]->GetEnable()==false)
        return;
    #endif
    spbLoop->Down=false;
    spbHome->Down=false;
    if(ActiveIndex!=-1)
    {
        ShowMotorSelect(ActiveIndex, 0);
    }
    ActiveIndex=Ptr->Tag;
    bMotorParameterChange=true;                                                 // ben 0618 //
    CheckNeedReloadMotorParameter();                                            // ben 0618 //
    ShowMotorSelect(ActiveIndex, 1);
}
//==============================================================================
void __fastcall TfMotorTest::lpA00Click(TObject *Sender)                        // select pos1 pos2 edit //
{
    TEdit *Ptr;
    Ptr=(TEdit *)Sender;
    #ifndef SOFT_SIMULATE
    if(HSys.MotPtr[Ptr->Tag]->GetEnable()==false)
        return;
    #endif

    spbLoop->Down=false;
    spbHome->Down=false;
    if(ActiveIndex!=-1)
        ShowMotorSelect(ActiveIndex, 0);
    ActiveIndex=Ptr->Tag;
    ShowMotorSelect(ActiveIndex, 1);
    ScrollBar1->Position=HSys.MotPtr[Ptr->Tag]->GetPersentSpeed();
    edActiveMotorSpeed->Text=HSys.MotPtr[Ptr->Tag]->GetSpeed();
    Label14->Caption=AnsiString(ScrollBar1->Position) + AnsiString(" %");
//    Label14->Caption=AnsiString(ScrollBar1->Position);
//    MyInputBox( (TEdit*)Sender);
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER);
}
//==============================================================================
void __fastcall TfMotorTest::ScrollBar1Scroll(TObject *Sender,                  // set persent speed //
      TScrollCode ScrollCode, int &ScrollPos)
{
    int i;
    if(ActiveIndex==-1)
        return;
    if(spbHome->Down)
        return;
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->SetPersentSpeed(ScrollBar1->Position);
    edActiveMotorSpeed->Text=HSys.MotPtr[ActiveIndex]->GetSpeed();
    Label14->Caption=AnsiString(ScrollBar1->Position)+AnsiString(" %");
    if( spbLoop->Down)
    {
        for(i=0;i<HSys.iTotalMotor;i++)
            if(AllMotLoopMove[i]->Checked)
                HSys.MotPtr[i]->SetPersentSpeed(ScrollBar1->Position);
    }
}
//==============================================================================
void __fastcall TfMotorTest::spbJogNMouseDown(TObject *Sender,                  // jog //
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveIndex==-1 || spbHome->Down || spbLoop->Down)
        return;
    if(IsMotorCanRun(true)==false)
        return;
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->JogN();
}
//==============================================================================
void __fastcall TfMotorTest::spbJogNMouseUp(TObject *Sender,                    // jog stop //
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveIndex==-1 || spbHome->Down ||spbLoop->Down )
        return;

    HSys.MotPtr[ActiveIndex]->DecStop();
}
//==============================================================================
void __fastcall TfMotorTest::spbJogPMouseDown(TObject *Sender,                  // jog //
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveIndex==-1 || spbHome->Down ||spbLoop->Down )
        return;
    if(IsMotorCanRun(true)==false)
       return;
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->JogP();
}
//==============================================================================
void __fastcall TfMotorTest::spbJogPMouseUp(TObject *Sender,                    // jog stop //
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveIndex==-1 || spbHome->Down ||spbLoop->Down )
        return;
    HSys.MotPtr[ActiveIndex]->DecStop();
}
//==============================================================================
void __fastcall TfMotorTest::DoReadWriteMotorReg()                              // read write motor driver //
{
}
//==============================================================================
void __fastcall TfMotorTest::Timer1Timer(TObject *Sender)
{
    if(fShow==false)
        return;
    if(fNote->fShow||MyMessageBox->fShow)                                       // ben edit 20110715 //
    {
        spbLoop->Down=false;
        spbHome->Down=false;
        bSingleHome=false;
    }
    if(ActiveIndex==-1)  return;
    edActiveMotorPosition->Text=HSys.MotPtr[ActiveIndex]->ReadPos();
    edActiveMotorEncoder->Text =HSys.MotPtr[ActiveIndex]->EncoderPosition;

    UpdateMotorLed();
    if(spbLoop->Down)                                                           // do loop //
    {
        DoLoopMove();
    }
    else if(spbHome->Down)                                                      // do home //
    {
        edActiveMotorSpeed->Text=HSys.MotPtr[ActiveIndex]->GetSpeed();
        if( HSys.MotPtr[ActiveIndex]->bHomeFlag==false)
        {
            AnsiString sErr="";
            HSys.MotPtr[ActiveIndex]->Home(sErr);
            if(HSys.MotPtr[ActiveIndex]->bHomeFlag)
            {
                spbHome->Down=false;
                Edit1->Text=-(int)(HSys.MotPtr[ActiveIndex]->GetLastHomePos()*HSys.MotPtr[ActiveIndex]->GetGearRatio());
            }
        }
    }
    DoReadWriteMotorReg();
}
//==============================================================================
void __fastcall TfMotorTest::ShowMotorInformation()                             // motor information //
{
    int iMotorAddr[128], ct=0, Addr, iStartAddress;
    int iStepMotorCount=0,iServoMotorCount=0;
    bool error=false,bOK;
    char str[256],szMotorIndex[128][8];
    AnsiString sPort[128];

    Memo1->Lines->Clear();
    // find repeat,incorrect Motor Address ,and build all motor address table
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        if(HSys.MotPtr[i]==NULL)
            continue;

        #ifndef SOFT_SIMULATE
            if(HSys.MotPtr[i]->GetEnable()==false)
                continue;
        #endif
        if(HSys.MotPtr[i]->bIsServoMotor)
            iServoMotorCount++;
        else
            iStepMotorCount++;

        Addr=HSys.MotPtr[i]->GetAddress();
        error=false;
        if(HSys.MotPtr[i]->CardModel!="MN200" &&
           HSys.MotPtr[i]->CardModel!="SynTek" &&
           Addr%8!=0)
        {
            sprintf(str,"M%02d address is out of rule !!", i);
            Memo1->Lines->Add(str);
            error=true;
        }

        if(error==true)
            continue;

        error=false;
        for(int j=0; j<ct; j++)
        {
            if(Addr==iMotorAddr[j])
            {
                sprintf(str, "%s address is repeat with M%02d !!", szMotorIndex[j], i);
                Memo1->Lines->Add(str);
                error=true;
                break;
            }
        }
        if(error==false)
        {
            iMotorAddr[ct]=Addr;
            sprintf(szMotorIndex[ct], "M%02d", i);
            sPort[ct]=HSys.MotPtr[i]->GetAddress();
            ct++;
        }
    }
    TStringList *MotorAddrList;
    TStringList *MotorIndexName;
    TStringList *MotorPort;
    TStringList *AllMotorPort;

    MotorAddrList   =new TStringList;
    MotorIndexName  =new TStringList;
    MotorPort       =new TStringList;
    AllMotorPort    =new TStringList;

    MotorAddrList->Clear();
    MotorIndexName->Clear();
    lbServoMotorCount->Caption="";
    lbStepMotorCount->Caption="";
    lbMotionCardCount->Caption="";
    lbReservedMotor->Caption="";
    if(ct==0)
    {
        delete MotorAddrList;
        delete MotorIndexName;
        delete MotorPort;
        delete AllMotorPort;
        return;
    }
    MotorPort->Clear();
    MotorAddrList->Add(AnsiString(iMotorAddr[0]));
    MotorIndexName->Add(AnsiString(szMotorIndex[0]));
    MotorPort->Add(sPort[0]);
    // sort motor table
    for(int i=1; i<ct; i++)
    {
        Addr=iMotorAddr[i];
        bOK=false;
        for(int j=0; j<MotorAddrList->Count; j++)
        {
            if(Addr<atoi(MotorAddrList->Strings[j].c_str()))
            {
                MotorAddrList->Insert(j, AnsiString(iMotorAddr[i]));
                MotorIndexName->Insert(j, AnsiString(szMotorIndex[i]));
                MotorPort->Insert(j, AnsiString(sPort[i]));
                bOK=true;
                break;
            }
        }
        if(bOK==false)
        {
            MotorAddrList->Add(AnsiString(iMotorAddr[i]));
            MotorIndexName->Add(AnsiString(szMotorIndex[i]));
            MotorPort->Add(AnsiString(sPort[i]));
        }
    }

    if(MotorAddrList->Count==0)
    {
        delete MotorAddrList;
        delete MotorIndexName;
        delete MotorPort;
        delete AllMotorPort;
        return;
    }

    iStartAddress=atoi(MotorPort->Strings[0].c_str());                          //2011.04.18    klutter
    iStartAddress/=32;
    iStartAddress*=32;
    AllMotorPort->Clear();
    // find all motor address location
    for(int j=0; j<4; j++)
    {
        sprintf(str, "%04X", iStartAddress+j*8);
        AllMotorPort->Add(str);
    }
    for(int i=0; i<MotorPort->Count; i++)
    {
        Addr=HexStrToInt(MotorPort->Strings[i].c_str());
        Addr/=32;
        Addr*=32;
        if(Addr!=iStartAddress)
        {
            iStartAddress=Addr;
            for(int j=0; j<4; j++)
            {
                sprintf(str, "%04X", iStartAddress+j*8);
                AllMotorPort->Add(str);
            }
        }
    }
    lbMotionCardCount->Caption=AllMotorPort->Count/4;
    lbServoMotorCount->Caption=iServoMotorCount;
    lbStepMotorCount->Caption=iStepMotorCount;
    lbReservedMotor->Caption=AllMotorPort->Count-iStepMotorCount-iServoMotorCount;
    // create information
    for(int i=0; i<AllMotorPort->Count; i++)
    {
        if((i%4)==0)
            MotorInformationGrid->Cells[0][1+i]=i/4+1;
        MotorInformationGrid->Cells[1][1+i]=AnsiString("0x")+AllMotorPort->Strings[i];
        for(int j=0; j<MotorPort->Count; j++)
            if(MotorInformationGrid->Cells[1][1+i].UpperCase()==MotorPort->Strings[j].UpperCase())
                MotorInformationGrid->Cells[2][1+i]=MotorIndexName->Strings[j];
    }
    delete MotorAddrList;
    delete MotorIndexName;
    delete MotorPort;
    delete AllMotorPort;

    SendMessage(MotorInformationGrid->Handle, WM_VSCROLL, SB_PAGEDOWN, 0);
    SendMessage(MotorInformationGrid->Handle, WM_VSCROLL, SB_PAGEUP, 0);

}
//==============================================================================
void __fastcall TfMotorTest::FormShow(TObject *Sender)
{
    bMotorParameterChange=false;
    fShow=true;
    Left=0;
    Top =0;
    OpenWorkFile();
    bSingleHome=false;
    spbHome->Down=false;
    spbLoop->Down=false;

    if(HSys.LastSet.iLanguageCountry==0)
    {
        strngrdMotor->Cells[0][0]=" Item";
        strngrdMotor->Cells[1][0]=" Value";

        strngrdMotor->Cells[0][ 1]=" InitialSpeed";
        strngrdMotor->Cells[0][ 2]=" JogHighSpeed";
        strngrdMotor->Cells[0][ 3]=" JogLowSpeed";
        strngrdMotor->Cells[0][ 4]=" HomeHighSpeed";
        strngrdMotor->Cells[0][ 5]=" HomeLowSpeed";
        strngrdMotor->Cells[0][ 6]=" SoftLimitP";
        strngrdMotor->Cells[0][ 7]=" SoftLimitN";
        strngrdMotor->Cells[0][ 8]=" Acc";
        strngrdMotor->Cells[0][ 9]=" Dec";
        strngrdMotor->Cells[0][10]=" Range";
    }
    else if(HSys.LastSet.iLanguageCountry==1)
    {
        strngrdMotor->Cells[0][0]=" 項目";
        strngrdMotor->Cells[1][0]=" 設定值";

        strngrdMotor->Cells[0][ 1]=" 初始速度";
        strngrdMotor->Cells[0][ 2]=" 寸動最高速";
        strngrdMotor->Cells[0][ 3]=" 寸動最低速";
        strngrdMotor->Cells[0][ 4]=" 歸零最高速";
        strngrdMotor->Cells[0][ 5]=" 歸零最低速";
        strngrdMotor->Cells[0][ 6]=" 軟體最大點位";
        strngrdMotor->Cells[0][ 7]=" 軟體最小點位";
        strngrdMotor->Cells[0][ 8]=" 加速度";
        strngrdMotor->Cells[0][ 9]=" 減速度";
        strngrdMotor->Cells[0][10]=" 放大倍率";
    }

    if(HSys.Sw.SwMotorRelay.OutValue)
    {
        for(int i=0; i<HSys.iTotalMotor; i++)                                   // server on off //
        {
            if(HSys.MotPtr[i]->ReadServoAlarmOn()==true &&
               HSys.MotPtr[i]->Led[iServoOn]==false)
                HSys.MotPtr[i]->ServoOnOff(Server_On);
        }

        btnMotorPower->Caption="Motor Power On";
        btnMotorPower->Down=true;
    }
    else
    {
        AllBreakLock();
        HSys.Sw.SwMotorRelay.Off();
        for(int i=0; i<HSys.iTotalMotor; i++)                                   // server on off //
        {
            if(HSys.MotPtr[i]->ReadServoAlarmOn()==true)
                HSys.MotPtr[i]->ServoOnOff(Server_Off);
        }
        btnMotorPower->Caption="Motor Power Off";
        btnMotorPower->Down=false;
    }
    radSingle->Checked=true;

//    MotorTable->TableName=HSys.CurrentDir+"\\system\\motor.db";
//    tbMotorCommunction->TableName=HSys.CurrentDir+"\\system\\MotoParaParameter.db";
//    MotorTable->Active=true;
//    tbMotorCommunction->Active=true;
//    strngrdMotor->RowCount=tbMotorCommunction->FieldCount-1;
//    for(i=0;i<tbMotorCommunction->FieldCount-1;i++)
//    {
//        strngrdMotor->Cells[0][i]=tbMotorCommunction->Fields->Fields[i+1]->FieldName;
//    }

    fAllMotorHome=false;
//    MotorTable->Edit();
    ShowMotorInformation();
    sbtReload->Click();
    PageControl1->ActivePageIndex=0;
    PageControl1->ActivePage=TabSheet6;
}
//==============================================================================
void __fastcall TfMotorTest::spbSetPos1Click(TObject *Sender)                   // set pos 1 //
{
    if(ActiveIndex==-1)
        return;
    MPos1[ActiveIndex]->Text=edActiveMotorPosition->Text;
}
//==============================================================================
void __fastcall TfMotorTest::spbSetPos2Click(TObject *Sender)                   // set pos 2 //
{
    if(ActiveIndex==-1)
        return;
    MPos2[ActiveIndex]->Text=edActiveMotorPosition->Text;
}
//==============================================================================
void __fastcall TfMotorTest::spbGotoSoftPositiveClick(TObject *Sender)          // go softlinit p //
{
    if(ActiveIndex==-1 || spbHome->Down ||spbLoop->Down)
        return;
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->MotorMove(HSys.MotPtr[ActiveIndex]->GetSoftLimitP());
}
//==============================================================================
void __fastcall TfMotorTest::spbGotoSoftNegitiveClick(TObject *Sender)          // go softlimit n //
{
    if(ActiveIndex==-1 || spbHome->Down ||spbLoop->Down)
        return;
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->MotorMove(HSys.MotPtr[ActiveIndex]->GetSoftLimitN());
}
//==============================================================================
void __fastcall TfMotorTest::spbHomeClick(TObject *Sender)                      // home button //
{
    if(ActiveIndex==-1)
    {
        spbHome->Down=false;
        return;
    }
    if(IsMotorCanRun(true)==false)
        return;
    CheckNeedReloadMotorParameter();
    if(spbHome->Down==true)
    {
        HSys.MotPtr[ActiveIndex]->InitHomeTask();
        bSingleHome=true;
    }
    else
    {
        bSingleHome=false;
        HSys.MotPtr[ActiveIndex]->DecStop();
    }
}
//==============================================================================
void __fastcall TfMotorTest::spbMoveNegtiveClick(TObject *Sender)               // move n //
{
    int Pos=atoi(coboMoveInterval->Text.c_str());//,NowPos;
    if(ActiveIndex==-1 || spbHome->Down || spbLoop->Down)
        return;
    if(IsMotorCanRun(true)==false)
        return;
//    NowPos=atoi(edActiveMotorPosition->Text.c_str());
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->MotorMove(  HSys.MotPtr[ActiveIndex]->ReadPos()-Pos   );
}
//==============================================================================
void __fastcall TfMotorTest::spbMovePositiveClick(TObject *Sender)              // move p //
{
    int Pos=atoi(coboMoveInterval->Text.c_str());//,NowPos;
    if(ActiveIndex==-1 || spbHome->Down || spbLoop->Down)
        return;
    if(IsMotorCanRun(true)==false)
        return;
//    NowPos=atoi(edActiveMotorPosition->Text.c_str());
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->MotorMove(  HSys.MotPtr[ActiveIndex]->ReadPos()+Pos   );
}
//==============================================================================
bool __fastcall TfMotorTest::IsMotorCanRun(bool attr)                           // ben edit 20110715 //
{
    int iEmg=IsEMGPressed();

    if(attr && iEmg!=0)
        ShowSystemError(HSys.SenPtr[iEmg].Name, K_RETRY);
    return (iEmg==0);
}
//==============================================================================
void __fastcall TfMotorTest::spbLoopClick(TObject *Sender)                      // loop button //
{
    int ret=0;
    if(ActiveIndex==-1)
    {
        spbLoop->Down=false;
        return;
    }
    if(IsMotorCanRun(true)==false)
        return;
    CheckNeedReloadMotorParameter();                                            // ben 0618 //
    spbHome->Down=false;
    if(spbLoop->Down)
    {
        LoopTask=1;
        spbLoop->Down=false;
        if( HSys.MotPtr[ActiveIndex]->bHomeFlag==false)
        {
            ret=ShowMyMessageBox_YES_NO("Motor not home yet,sure to loop test?");
            if(ret==TMyMessageBox::msgrtnNO)
                spbLoop->Down=false;
            else
            {
                spbLoop->Down=true;
                dwStartTickCount=GetTickCount();
                dwLoopCount=0;
            }
        }
        else
        {
            spbLoop->Down=true;
            dwStartTickCount=GetTickCount();
            dwLoopCount=0;
        }
        if( radSingle->Checked )
        {
            HSys.MotPtr[ActiveIndex]->SetPersentSpeed(HSys.MotPtr[ActiveIndex]->GetPersentSpeed());
        }
        else
        {
            for(int i=0; i<HSys.iTotalMotor; i++)
            {
                if(AllMotLoopMove[i]->Checked)
                {
                    HSys.MotPtr[i]->SetPersentSpeed(HSys.MotPtr[i]->GetPersentSpeed());
                }
            }
        }
    }
    else
    {
        HSys.MotPtr[ActiveIndex]->DecStop();
    }
}
//==============================================================================
void __fastcall TfMotorTest::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    for(int i=0;i<HSys.iTotalMotor;i++)                                     // server on off //
    {
        if(HSys.MotPtr[i]->ReadServoAlarmOn()==true && HSys.MotPtr[i]->Led[iServoOn]==false)
            HSys.MotPtr[i]->ServoOnOff(Server_On);
    }

    fShow=false;
    SaveWorkFile();
//    MotorTable->Active=false;
}
//==============================================================================
void __fastcall TfMotorTest::SpeedButton4Click(TObject *Sender)                 // exit button //
{
//    MotorTable->Edit();
//    MotorTable->Post();
//    MotorTable->Close();
    SpeedButton4->Down=false;
    Close();
}
//==============================================================================
void __fastcall TfMotorTest::spbGoClick(TObject *Sender)                        // go button //
{
    if(ActiveIndex==-1)
        return;
    if(IsMotorCanRun(true)==false)
        return;
    if(HSys.MotPtr[ActiveIndex]->bHomeFlag==false)
    {
        ShowMyMessage("Motor not home");
        return;
    }
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->MotorMove( atoi(Edit1->Text.c_str()));
}
//==============================================================================
void __fastcall TfMotorTest::btnMotorPowerClick(TObject *Sender)                 // motor power button //
{
    bool f=HSys.Sw.SwMotorRelay.OutValue;
    //int ret;
    if(f)
    {
        int ret=ShowMyMessageBox_YES_NO("Sure to turn off motor power ?");
        if(ret==TMyMessageBox::msgrtnNO)
        {
            return;
        }
    }
    f=!f;
    if(f)
    {
        HSys.Sw.SwMotorRelay.On();
        AllBreakFree();
        btnMotorPower->Caption="Motor Power On";
        btnMotorPower->Down=true;
    }
    else
    {
        AllBreakLock();
        HSys.Sw.SwMotorRelay.Off();
        for(int i=0; i<HSys.iTotalMotor; i++)                                   // server on off //
        {
            if(HSys.MotPtr[i]->ReadServoAlarmOn()==true)
                HSys.MotPtr[i]->ServoOnOff(Server_Off);
        }
        btnMotorPower->Caption="Motor Power Off";
        btnMotorPower->Down=false;
    }
}
//==============================================================================
void __fastcall TfMotorTest::spbStopClick(TObject *Sender)                      // stop //
{
    if( ActiveIndex==-1)
        return;
    spbLoop->Down=false;
    spbHome->Down=false;
    CheckNeedReloadMotorParameter();                                            // ben 0618 //
    HSys.MotPtr[ActiveIndex]->DecStop();
}
//==============================================================================
bool __fastcall TfMotorTest::DoGaliHome(int ActiveIndex)
{
/*
        if(HSys.MotPtr[ActiveIndex]->Gali_SingalHome())
            return true;
        else
            return false;
*/
    return true;
}
//==============================================================================
void __fastcall TfMotorTest::OpenWorkFile()                                     // Load MotorTest Parameter //
{
    AnsiString S;
    S=HSys.CurrentDir+AnsiString("\\system\\motor_test.ini");
    FormSysTools->OpenFormData(S);
    FormSysTools->LoadFormData(palMotorSelect, "Motor_Test_Data");
    FormSysTools->CloseFormData();
}
//==============================================================================
void __fastcall TfMotorTest::SaveWorkFile()                                     // Save MotorTest Parameter //
{
    AnsiString S;
    S=HSys.CurrentDir+AnsiString("\\system\\motor_test.ini");
    FormSysTools->OpenFormData(S);
    FormSysTools->SaveFormData(palMotorSelect, "Motor_Test_Data");
    FormSysTools->CloseFormData();
}
//==============================================================================
void __fastcall TfMotorTest::Clear1Click(TObject *Sender)
{
    TEdit *P;
    P=dynamic_cast<TEdit *>(PopupMenuMotorPositionClear->PopupComponent);
    if(P!=NULL)
        P->Text="0";
}
//==============================================================================
void __fastcall TfMotorTest::ClearAll1Click(TObject *Sender)
{
    for(int i=0;i<MAX_MOTOR_TEST_COUNT;i++)
    {
        MPos1[i]->Text="0";
        MPos2[i]->Text="0";
    };
}
//==============================================================================
int __fastcall TfMotorTest::FindMotorParameter(AnsiString MotorName)
{
    for(int i=0; i<HSys.iTotalMotor; i++)
    {
        if(HSys.MotPtr[i]->Number==MotorName)
            return i;
    }
    return -1;
}
//==============================================================================
int __fastcall TfMotorTest::FindMotorParameter(int Index)
{
    AnsiString str;
    str.sprintf("M%02d", Index+1);
    return FindMotorParameter(str);
}
//==============================================================================
void __fastcall TfMotorTest::UpdateMotorParameter()                             //Steven 20240502 : fixed for motor
{
    if(ActiveIndex==-1)                                                         //Steven 20230731 : Add protection
        return;

    strngrdMotor->Cells[1][ 1]=HSys.MotPtr[ActiveIndex]->GetInitSpeed();
    strngrdMotor->Cells[1][ 2]=HSys.MotPtr[ActiveIndex]->GetJogHighSpeed();
    strngrdMotor->Cells[1][ 3]=HSys.MotPtr[ActiveIndex]->GetJogLowSpeed();
    strngrdMotor->Cells[1][ 4]=HSys.MotPtr[ActiveIndex]->GetHomeHighSpeed();
    strngrdMotor->Cells[1][ 5]=HSys.MotPtr[ActiveIndex]->GetHomeLowSpeed();
    strngrdMotor->Cells[1][ 6]=HSys.MotPtr[ActiveIndex]->GetSoftLimitP();
    strngrdMotor->Cells[1][ 7]=HSys.MotPtr[ActiveIndex]->GetSoftLimitN();
    strngrdMotor->Cells[1][ 8]=HSys.MotPtr[ActiveIndex]->GetAcc();
    strngrdMotor->Cells[1][ 9]=HSys.MotPtr[ActiveIndex]->GetDec();
    strngrdMotor->Cells[1][10]=HSys.MotPtr[ActiveIndex]->GetRange();
}
//==============================================================================
void __fastcall TfMotorTest::spbCopyMotorParameterClick(TObject *Sender)        // copy motor database //
{
    AnsiString S;

    if(ActiveIndex==-1)
        return;

    S=coboSourceMotorIndex->Items->Strings[coboSourceMotorIndex->ItemIndex];
    int iFrom=FindMotorParameter(S);
    if(iFrom>=0)
    {
        strngrdMotor->Cells[1][ 1]=HSys.MotPtr[iFrom]->GetInitSpeed();
        strngrdMotor->Cells[1][ 2]=HSys.MotPtr[iFrom]->GetJogHighSpeed();
        strngrdMotor->Cells[1][ 3]=HSys.MotPtr[iFrom]->GetJogLowSpeed();
        strngrdMotor->Cells[1][ 4]=HSys.MotPtr[iFrom]->GetHomeHighSpeed();
        strngrdMotor->Cells[1][ 5]=HSys.MotPtr[iFrom]->GetHomeLowSpeed();
        strngrdMotor->Cells[1][ 6]=HSys.MotPtr[iFrom]->GetSoftLimitP();
        strngrdMotor->Cells[1][ 7]=HSys.MotPtr[iFrom]->GetSoftLimitN();
        strngrdMotor->Cells[1][ 8]=HSys.MotPtr[iFrom]->GetAcc();
        strngrdMotor->Cells[1][ 9]=HSys.MotPtr[iFrom]->GetDec();
        strngrdMotor->Cells[1][10]=HSys.MotPtr[iFrom]->GetRange();
    }
}
//==============================================================================
void __fastcall TfMotorTest::CheckNeedReloadMotorParameter()
{
    if(bMotorParameterChange==true)
    {
        HSys.LoadSingleMotorParameterFromDataBase(ActiveIndex,false);
        HSys.MotPtr[ActiveIndex]->SetPersentSpeed(ScrollBar1->Position);
        FindMotorParameter(ActiveIndex);
        bMotorParameterChange=false;
    }
}
//==============================================================================
void __fastcall TfMotorTest::edActiveMotorSpeedChange(TObject *Sender)          //2011.04.18Ray
{
    if(HSys.Sys.SystemStart && HSys.Sys.RunMode==Run_Home )
        return;
    CheckNeedReloadMotorParameter();
    HSys.MotPtr[ActiveIndex]->SetSpeed(atoi(edActiveMotorSpeed->Text.c_str()));       // ben 2011 0630 -s //
    int i=(HSys.MotPtr[ActiveIndex]->GetSpeed()-HSys.MotPtr[ActiveIndex]->GetJogLowSpeed())*100;
    i=i/(HSys.MotPtr[ActiveIndex]->GetJogHighSpeed()-HSys.MotPtr[ActiveIndex]->GetJogLowSpeed());
    ScrollBar1->Position=i;
    Label14->Caption=AnsiString(ScrollBar1->Position)+AnsiString(" %");               // ben 2011 0630 -e //
}
//==============================================================================
void __fastcall TfMotorTest::SpeedButton13Click(TObject *Sender)
{
    RecordProcess("Enter IO");
    fiosetview->Show();
    fiosetview->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::spbServoOnClick(TObject *Sender)
{
    static bool bServoOn=true;
    if(ActiveIndex!=-1)
    {
        HSys.MotPtr[ActiveIndex]->ScanMotorStatus();
    //    MOT[ActiveIndex].ServoOnOff(!MOT[ActiveIndex].Led[iServoOn]);

        bServoOn=!bServoOn;
        HSys.MotPtr[ActiveIndex]->ServoOnOff(bServoOn);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    iSelectRow=ARow;
    iSelectCol=ACol;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnAddMotorClick(TObject *Sender)
{
    int iRow=strngrdMotorData->RowCount;
    strngrdMotorData->RowCount=iRow+1;
    for(int j=0; j<strngrdMotorData->ColCount; j++)
    {
        strngrdMotorData->Cells[j][iRow]="";
    }
    btnAddMotor->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnDeleteMotorClick(TObject *Sender)
{
    if(iSelectRow==0)
        return;
    for(int i=iSelectRow; i<strngrdMotorData->RowCount-1; i++)
    {
        for(int j=0; j<strngrdMotorData->ColCount; j++)
        {
            strngrdMotorData->Cells[j][i]=strngrdMotorData->Cells[j][i+1];
        }
    }
    strngrdMotorData->RowCount=strngrdMotorData->RowCount-1;
    btnDeleteMotor->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnModifyClick(TObject *Sender)
{
    if(iSelectRow>0)
    {
        Edit2->Text=strngrdMotorData->Cells[iSelectCol][iSelectRow];
        if(strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("Motorname"))!=0 ||
           strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("Alias"))!=0     ||
           strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("CardModel"))!=0)
        {
            fQwertyKey->ShowQwertyKey(Edit2, N_NO_SYMBOL|N_NO_SPACE);
        }
        else if(strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("GearRatio"))!=0 ||
                strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("Acc"))!=0     ||
                strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("Dec"))!=0)
        {
            fQwertyKey->ShowQwertyKey(Edit2, N_DOUBLE);
        }
        else
        {
            fQwertyKey->ShowQwertyKey(Edit2, N_INTEGER);
        }
        strngrdMotorData->Cells[iSelectCol][iSelectRow]=Edit2->Text;
    }
    btnModify->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbtReloadClick(TObject *Sender)
{
    TStringList *sList=new TStringList();
    TStringList *SL=new TStringList();

    int iCol=0, iRow=0;

    for(int i=0; i<strngrdMotorData->RowCount; i++)
    {
        for(int j=0; j<strngrdMotorData->ColCount; j++)
        {
            strngrdMotorData->Cells[j][i]="";
        }
    }

    if(FileExists(HSys.MotTablePath))
    {
        sList->LoadFromFile(HSys.MotTablePath);
        iRow=sList->Count;
        if(iRow>0)
        {
            strngrdMotorData->RowCount=iRow;
            for(int i=0; i<iRow; i++)
            {
                SL->CommaText=sList->Strings[i];

                if(SL->Count>iCol)
                {
                    iCol=SL->Count;
                    strngrdMotorData->ColCount=iCol;
                }

                for(int j=0; j<SL->Count; j++)
                {
                    strngrdMotorData->Cells[j][i]=SL->Strings[j];
                }
            }
        }
    }
    sList->Clear();
    SL->Clear();
    delete sList;
    delete SL;
    sbtReload->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbUpdateClick(TObject *Sender)
{
    TStringList *sList=new TStringList();
    TStringList *SL=new TStringList();

    int iCol=0, iRow=0;

    iRow=strngrdMotorData->RowCount;
    iCol=strngrdMotorData->ColCount;

    for(int i=0; i<iRow; i++)
    {
        SL->Clear();

        for(int j=0; j<iCol; j++)
        {
            SL->Add(strngrdMotorData->Cells[j][i]);
        }
        sList->Add(SL->CommaText);
    }

    sList->SaveToFile(HSys.MotTablePath);
    sList->Clear();
    SL->Clear();
    delete sList;
    delete SL;

    sbUpdate->Down=false;
}
//------------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
    TStringList *sList=new TStringList();
    int DestCol, DestRow;
    strngrdMotorData->MouseToCell(X, Y, DestCol, DestRow);
    if(iDragCol!=DestCol)
    {
        for(int i=0; i<strngrdMotorData->RowCount; i++)
        {
            sList->Add(strngrdMotorData->Cells[iDragCol][i]);
        }

        if(iDragCol>DestCol)
        {
            for(int j=iDragCol; j>DestCol; j--)
            {
                for(int i=0; i<strngrdMotorData->RowCount; i++)
                {
                    strngrdMotorData->Cells[j][i]=strngrdMotorData->Cells[j-1][i];
                }
            }
        }
        else if(iDragCol<DestCol)
        {
            for(int j=iDragCol; j<DestCol; j++)
            {
                for(int i=0; i<strngrdMotorData->RowCount; i++)
                {
                    strngrdMotorData->Cells[j][i]=strngrdMotorData->Cells[j+1][i];
                }
            }
        }

        for(int i=0; i<strngrdMotorData->RowCount; i++)
        {
            strngrdMotorData->Cells[DestCol][i]=sList->Strings[i];
        }
    }

    sList->Clear();
    delete sList;
}
//------------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataDblClick(TObject *Sender)
{
    btnModify->Click();
}
//------------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    int CurrentCol, CurrentRow;
    strngrdMotorData->MouseToCell(X, Y, CurrentCol, CurrentRow);

    if(Sender==strngrdMotorData && CurrentRow>=0 && CurrentCol>=0)
    {
        Accept=true;
    }
    else
    {
        Accept=false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    strngrdMotorData->MouseToCell(X, Y, iDragCol, iDragRow);

    if(iDragRow==0)
        strngrdMotorData->BeginDrag(false, 4);

    lblMotorName->Caption="Motor Name : "+ strngrdMotorData->Cells[1][iDragRow];
}
//------------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    if(ACol!=1 || ARow==0)                                                      //Steven 20240502 : fixed for motor
        return;

    if(ActiveIndex==-1)
        return;

    double ret;
    Edit2->Text=strngrdMotor->Cells[ACol][ARow];
    if(ARow==8 || ARow==9)
    {
        fQwertyKey->ShowQwertyKey(Edit2, N_DOUBLE, 0);
        ret=atof(Edit2->Text.c_str());
    }
    else
    {
        fQwertyKey->ShowQwertyKey(Edit2, N_INTEGER, 0);
        ret=atoi(Edit2->Text.c_str());
    }

    strngrdMotor->Cells[ACol][ARow]=ret;
    if(ARow==1)
        HSys.MotPtr[ActiveIndex]->SetInitSpeed(ret);
    else if(ARow==2)
        HSys.MotPtr[ActiveIndex]->SetJogHighSpeed(ret);
    else if(ARow==3)
        HSys.MotPtr[ActiveIndex]->SetJogLowSpeed(ret);
    else if(ARow==4)
        HSys.MotPtr[ActiveIndex]->SetHomeHighSpeed(ret);
    else if(ARow==5)
        HSys.MotPtr[ActiveIndex]->SetHomeLowSpeed(ret);
    else if(ARow==6)
        HSys.MotPtr[ActiveIndex]->SetSoftLimitP(ret);
    else if(ARow==7)
        HSys.MotPtr[ActiveIndex]->SetSoftLimitN(ret);
    else if(ARow==8)
        HSys.MotPtr[ActiveIndex]->SetAcc(ret);
    else if(ARow==9)
        HSys.MotPtr[ActiveIndex]->SetDec(ret);
    else if(ARow==10)
        HSys.MotPtr[ActiveIndex]->SetRange(ret);
    spbLoop->Down=false;
    HSys.MotPtr[ActiveIndex]->bHomeFlag=false;
    UpdateMotorParameter();
}
//------------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSaveToFileClick(TObject *Sender)
{
    iSelectRow=-1;                                                              //Steven 20240502 : fixed for motor
    int iMotorName=-1, iInitSpeed=-1, iJogHighSpeed=-1, iJogLowSpeed=-1;
    int iHomeHighSpeed=-1, iHomeLowSpeed=-1, iSoftLimitN=-1, iSoftLimitP=-1;
    int iAcc=-1, iDec=-1, iRange=-1;

    for(int j=0; j<strngrdMotorData->ColCount; j++)
    {
        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("Motorname"))!=0)
            iMotorName=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("InitSpeed"))!=0)
            iInitSpeed=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("JogHighSpeed"))!=0)
            iJogHighSpeed=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("JogLowSpeed"))!=0)
            iJogLowSpeed=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("HomeHighSpeed"))!=0)
            iHomeHighSpeed=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("HomeLowSpeed"))!=0)
            iHomeLowSpeed=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("SoftLimitN"))!=0)
            iSoftLimitN=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("SoftLimitP"))!=0)
            iSoftLimitP=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("Acc"))!=0)
            iAcc=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("Dec"))!=0)
            iDec=j;

        if(strngrdMotorData->Cells[j][0].AnsiPos(AnsiString("Range"))!=0)
            iRange=j;
    }

    if(iMotorName!=-1)
    {
        for(int i=0; i<strngrdMotorData->RowCount; i++)
        {
            if(iSelectRow==-1 &&
               HSys.MotPtr[ActiveIndex]->Number==strngrdMotorData->Cells[iMotorName][i])
            {
                iSelectRow=i;
            }
        }
    }

    if(iSelectRow>0)
    {
        if(iInitSpeed       !=-1)  strngrdMotorData->Cells[iInitSpeed       ][iSelectRow]=strngrdMotor->Cells[1][ 1];
        if(iJogHighSpeed    !=-1)  strngrdMotorData->Cells[iJogHighSpeed    ][iSelectRow]=strngrdMotor->Cells[1][ 2];
        if(iJogLowSpeed     !=-1)  strngrdMotorData->Cells[iJogLowSpeed     ][iSelectRow]=strngrdMotor->Cells[1][ 3];
        if(iHomeHighSpeed   !=-1)  strngrdMotorData->Cells[iHomeHighSpeed   ][iSelectRow]=strngrdMotor->Cells[1][ 4];
        if(iHomeLowSpeed    !=-1)  strngrdMotorData->Cells[iHomeLowSpeed    ][iSelectRow]=strngrdMotor->Cells[1][ 5];
        if(iSoftLimitP      !=-1)  strngrdMotorData->Cells[iSoftLimitP      ][iSelectRow]=strngrdMotor->Cells[1][ 6];
        if(iSoftLimitN      !=-1)  strngrdMotorData->Cells[iSoftLimitN      ][iSelectRow]=strngrdMotor->Cells[1][ 7];
        if(iAcc             !=-1)  strngrdMotorData->Cells[iAcc             ][iSelectRow]=strngrdMotor->Cells[1][ 8];
        if(iDec             !=-1)  strngrdMotorData->Cells[iDec             ][iSelectRow]=strngrdMotor->Cells[1][ 9];
        if(iRange           !=-1)  strngrdMotorData->Cells[iRange           ][iSelectRow]=strngrdMotor->Cells[1][10];

        sbUpdate->Click();
        sbtReload->Click();
    }
    btnSaveToFile->Down=false;
}
//------------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSaveToFileMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btnSaveToFile->Down=false;
}
//---------------------------------------------------------------------------