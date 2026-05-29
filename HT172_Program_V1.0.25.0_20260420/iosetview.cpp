//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

//---------------------------------------------------------------------------
#include "iosetview.h"
#include "MyPLC_IO_Modbus.h"
#include "uPadInterface.h"
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "butPa1"
#pragma link "MyLed"
#pragma link "BtnPanelLane"
#pragma link "MyLedLane"
#pragma resource "*.dfm"

Tfiosetview *fiosetview;
//TMyPISO64_IO   myPISOIO[4];
TMyMN200_IO   myMN200IO[4];
bool bCheckEMG=false;
bool bFirstShow=false;
//==============================================================================
typedef struct IO_Led_Sensor_Struct                                             // input led struct //
{
    TMyLed    *Led;
    TMySensor  *Sen;
}IO_Led_Sensor;
//==============================================================================
typedef struct IO_Panel_Switch_Struct                                           // output panel struct //
{
    TBtnPanel  *Pal;
    TMySwitch  *Sw;
}IO_Panel_Switch;
//==============================================================================
typedef struct                                                                  // ioSortStru結構 //
{
    int card;                                                                   //20111207 Daver add
    int port;
    int bit;
    int type;
    char name[32];
}ioSortStru;
//---------------------------------------------------------------------------
SUCKER_PANEL::SUCKER_PANEL(TGroupBox *GroupBox, TMySucker *SuckPtr)
{
    AnsiString str;
    int iLeft       =7;
    int iTop        =22;
    int iLeftPitch  =56;
    int iTopPitch   =86;
//    int iColCount   =12;    //每幾個一排
    int iRow        =SuckPtr->iMyRow;
    int iCol        =SuckPtr->iMyCol;

    palBackGround               =new TPanel(GroupBox);
    palBackGround->Parent       =GroupBox;
    str.sprintf("palBackGround_%s", SuckPtr->SuckerName);
    palBackGround->Name         =str;
    palBackGround->Caption      ="";
    palBackGround->Color        =(TColor)0x00DFD9CC;
    palBackGround->Height       =80;
    palBackGround->Left         =iLeft+iLeftPitch*iCol;
    palBackGround->Top          =iTop +iTopPitch *iRow;
    palBackGround->Width        =52;
    palBackGround->BevelInner   =bvLowered;
    palBackGround->BevelOuter   =bvRaised;
    palBackGround->BevelWidth   =1;
    palBackGround->Tag          =iRow*10+iCol;

    palNumber               =new TPanel(palBackGround);
    palNumber->Parent       =palBackGround;
    str.sprintf("palNumber_%s", SuckPtr->SuckerName);
    palNumber->Name         =str;
    palNumber->Color        =(TColor)0x00917B51;
    palNumber->Height       =30;
    palNumber->Left         =2;
    palNumber->Top          =4;
    palNumber->Width        =48;
    palNumber->BevelInner   =bvLowered;
    palNumber->BevelOuter   =bvRaised;
    palNumber->BevelWidth   =1;
    palNumber->Tag          =iRow*10+iCol;
    palNumber->Caption      =SuckPtr->Alias;
    palNumber->Font->Charset=DEFAULT_CHARSET;
    palNumber->Font->Color  =clWhite;
    palNumber->Font->Name   ="Arial";
    palNumber->Font->Size   =17;

    OnPanel                 =new TBtnPanel(palBackGround);
    OnPanel->Parent         =palBackGround;
    str.sprintf("OnPanel_%s", SuckPtr->OnPortName);
    OnPanel->Name           =str;
    OnPanel->Color          =(TColor)0x00804000;
    OnPanel->TrueColor      =(TColor)0x00FF8000;
    OnPanel->FalseColor     =(TColor)0x00804000;
    OnPanel->FalseFontColor =clWhite;
    OnPanel->TrueFontColor  =clBlack;
    OnPanel->Height         =24;
    OnPanel->Left           =2;
    OnPanel->Top            =36;
    OnPanel->Width          =24;
    OnPanel->BevelInner     =bvRaised;
    OnPanel->BevelOuter     =bvRaised;
    OnPanel->BevelWidth     =1;
    OnPanel->Tag            =iRow*10+iCol;
    OnPanel->Caption        ="^";
    OnPanel->ShowHint       =false;
    OnPanel->Alias          =SuckPtr->OnPortName;
    OnPanel->Font->Charset  =DEFAULT_CHARSET;
    OnPanel->Font->Color    =clWhite;
    OnPanel->Font->Name     ="Arial";
    OnPanel->Font->Size     =10;

    OffPanel                =new TBtnPanel(palBackGround);
    OffPanel->Parent        =palBackGround;
    str.sprintf("OffPanel_%s", SuckPtr->OffPortName);
    OffPanel->Name          =str;
    OffPanel->Color         =(TColor)0x00804000;
    OffPanel->TrueColor     =(TColor)0x00FF8000;
    OffPanel->FalseColor    =(TColor)0x00804000;
    OffPanel->FalseFontColor=clWhite;
    OffPanel->TrueFontColor =clBlack;
    OffPanel->Height        =24;
    OffPanel->Left          =26;
    OffPanel->Top           =36;
    OffPanel->Width         =24;
    OffPanel->BevelInner    =bvRaised;
    OffPanel->BevelOuter    =bvRaised;
    OffPanel->BevelWidth    =1;
    OffPanel->Tag           =iRow*10+iCol;
    OffPanel->Caption       ="v";
    OffPanel->ShowHint      =false;
    OffPanel->Alias         =SuckPtr->OffPortName;
    OffPanel->Font->Charset =DEFAULT_CHARSET;
    OffPanel->Font->Color   =clWhite;
    OffPanel->Font->Name    ="Arial";
    OffPanel->Font->Size    =10;

    OnSensor                =new TMyLed(palBackGround);
    OnSensor->Parent        =palBackGround;
    str.sprintf("OnSensor_%s", SuckPtr->SensorName);
    OnSensor->Name          =str;
    OnSensor->TrueColor     =clLime;
    OnSensor->FalseColor    =clSilver;
    OnSensor->Height        =14;
    OnSensor->Left          =14;
    OnSensor->Top           =62;
    OnSensor->Width         =22;
    OnSensor->Tag           =iRow*10+iCol;
    OnSensor->ShowHint      =true;
    OnSensor->Alias         =SuckPtr->SensorName;
    OnSensor->Blink         =false;
    OnSensor->LEDStyle      =LEDHorizontal;
    bEnable=true;
}
//------------------------------------------------------------------------------
SUCKER_GROUP::SUCKER_GROUP(TTabSheet *TabSheet, TMyKitSuck *SuckPtr, int index)
{
    int iHeight     =110;
    int iHeightPitch=86;
    int iWidth      =65;
    int iWidthPitch =56;
//    int iColCount   =12;    //每幾個一排

    AnsiString str;
    iSuckerCount=SuckPtr->MaxItem;
    str.sprintf("SuckerGroup_%s", SuckPtr->Name);

    SuckerGroup                 =new TGroupBox(TabSheet);
    SuckerGroup->Parent         =TabSheet;
    SuckerGroup->Name           =str;
    SuckerGroup->Caption        =SuckPtr->Name;
    SuckerGroup->Color          =(TColor)0x00C2B8A6;
    SuckerGroup->Height         =iHeight+iHeightPitch*(SuckPtr->MaxItemR-1);
    SuckerGroup->Width          =iWidth +iWidthPitch *(SuckPtr->MaxItemC);

    SuckerGroup->Tag            =index;
    SuckerGroup->Font->Charset  =DEFAULT_CHARSET;
    SuckerGroup->Font->Color    =clWindowText;
    SuckerGroup->Font->Name     ="Times New Roman";
    SuckerGroup->Font->Size     =14;
    SuckerGroup->Font->Style    =TFontStyles()<<fsBold;

    for(int iR=0; iR<SuckPtr->MaxItemR; iR++)
    {
        for(int iC=0; iC<SuckPtr->MaxItemC; iC++)
        {
            SuckerPanel.push_back(new SUCKER_PANEL(SuckerGroup, &SuckPtr->Suck[iR][iC]));
//            SuckerPanel[iR][iC]->bEnable=SuckPtr->Suck[iR][iC].Enable;
        }
    }

    int iLeft=SuckerPanel[iSuckerCount-1]->palBackGround->Left+SuckerPanel[iSuckerCount-1]->palBackGround->Width+4;

    palAllOn                =new TPanel(SuckerGroup);
    palAllOn->Parent        =SuckerGroup;
    str.sprintf("palAllOn_%s", SuckPtr->Name);
    palAllOn->Name          =str;
    palAllOn->Color         =(TColor)0x00804000;
    palAllOn->Height        =20;
    palAllOn->Left          =iLeft;
    palAllOn->Top           =22;
    palAllOn->Width         =48;
    palAllOn->BevelInner    =bvRaised;
    palAllOn->BevelOuter    =bvRaised;
    palAllOn->BevelWidth    =1;
    palAllOn->Tag           =index;
    palAllOn->Caption       ="Suck";
    palAllOn->Font->Charset =DEFAULT_CHARSET;
    palAllOn->Font->Color   =clWhite;
    palAllOn->Font->Name    ="Arial";
    palAllOn->Font->Size    =10;
    palAllOn->OnMouseDown   =fiosetview->palAllOnMouseDown;
    palAllOn->OnMouseUp     =fiosetview->palAllOnMouseUp;
    palAllOn->OnClick       =palAllSuckClick;

    palAllOff               =new TPanel(SuckerGroup);
    palAllOff->Parent       =SuckerGroup;
    str.sprintf("palAllOff_%s", SuckPtr->Name);
    palAllOff->Name         =str;
    palAllOff->Color        =(TColor)0x00804000;
    palAllOff->Height       =20;
    palAllOff->Left         =iLeft;
    palAllOff->Top          =52;
    palAllOff->Width        =48;
    palAllOff->BevelInner   =bvRaised;
    palAllOff->BevelOuter   =bvRaised;
    palAllOff->BevelWidth   =1;
    palAllOff->Tag          =index;
    palAllOff->Caption      ="Destroy";
    palAllOff->Font->Charset=DEFAULT_CHARSET;
    palAllOff->Font->Color  =clWhite;
    palAllOff->Font->Name   ="Arial";
    palAllOff->Font->Size   =10;
    palAllOff->OnMouseDown  =fiosetview->palAllOnMouseDown;
    palAllOff->OnMouseUp    =fiosetview->palAllOnMouseUp;
    palAllOff->OnClick      =palAllDestroyClick;

    palCloseAll               =new TPanel(SuckerGroup);
    palCloseAll->Parent       =SuckerGroup;
    str.sprintf("palCloseAll_%s", SuckPtr->Name);
    palCloseAll->Name         =str;
    palCloseAll->Color        =(TColor)0x00804000;
    palCloseAll->Height       =20;
    palCloseAll->Left         =iLeft;
    palCloseAll->Top          =82;
    palCloseAll->Width        =48;
    palCloseAll->BevelInner   =bvRaised;
    palCloseAll->BevelOuter   =bvRaised;
    palCloseAll->BevelWidth   =1;
    palCloseAll->Tag          =index;
    palCloseAll->Caption      ="All Off";
    palCloseAll->Font->Charset=DEFAULT_CHARSET;
    palCloseAll->Font->Color  =clWhite;
    palCloseAll->Font->Name   ="Arial";
    palCloseAll->Font->Size   =10;
    palCloseAll->OnMouseDown  =fiosetview->palAllOnMouseDown;
    palCloseAll->OnMouseUp    =fiosetview->palAllOnMouseUp;
    palCloseAll->OnClick      =palAllOffClick;
}
//------------------------------------------------------------------------------
void __fastcall SUCKER_GROUP::palAllSuckClick(TObject *Sender)
{
    unsigned int iSize=SuckerPanel.size();
    for(unsigned int i=0; i<iSize; i++)
    {
        if(SuckerPanel[i]->bEnable)
        {
            SuckerPanel[i]->OnPanel ->Down=false;
            SuckerPanel[i]->OffPanel->Down=true;
            fiosetview->IOSetViewOutput(SuckerPanel[i]->OnPanel);
            fiosetview->IOSetViewOutput(SuckerPanel[i]->OffPanel);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall SUCKER_GROUP::palAllDestroyClick(TObject *Sender)
{
    unsigned int iSize=SuckerPanel.size();
    for(unsigned int i=0; i<iSize; i++)
    {
        if(SuckerPanel[i]->bEnable)
        {
            SuckerPanel[i]->OnPanel ->Down=true;
            SuckerPanel[i]->OffPanel->Down=false;
            fiosetview->IOSetViewOutput(SuckerPanel[i]->OnPanel);
            fiosetview->IOSetViewOutput(SuckerPanel[i]->OffPanel);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall SUCKER_GROUP::palAllOffClick(TObject *Sender)
{
    unsigned int iSize=SuckerPanel.size();
    for(unsigned int i=0; i<iSize; i++)
    {
        SuckerPanel[i]->OnPanel ->Down=true;
        SuckerPanel[i]->OffPanel->Down=true;
        fiosetview->IOSetViewOutput(SuckerPanel[i]->OnPanel);
        fiosetview->IOSetViewOutput(SuckerPanel[i]->OffPanel);
    }
}
//------------------------------------------------------------------------------
void SUCKER_GROUP::SetPanelPos(int Top, int Left)
{
    SuckerGroup->Left=Left;
    SuckerGroup->Top =Top;
}
//------------------------------------------------------------------------------
__fastcall SUCKER_GROUP::~SUCKER_GROUP()
{
    for(vector<SUCKER_PANEL *>::iterator iter=SuckerPanel.begin(); iter!=SuckerPanel.end(); ++iter)
    {
        delete *iter;
    }
    SuckerPanel.clear();
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::FormDestroy(TObject *Sender)
{
//    for(vector<SUCKER_GROUP *>::iterator iter=MySuckerGroup.begin(); iter!=MySuckerGroup.end(); ++iter)
//    {
//        delete *iter;
//    }
//    MySuckerGroup.clear();

    for(vector<SUCKER_GROUP *>::iterator iter=MySuckerGroup.begin(); iter!=MySuckerGroup.end(); ++iter)
    {
        delete *iter;
    }
    MySuckerGroup.clear();

    delete InputTableList;
    delete OutputTableList;
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::FormCreate(TObject *Sender)
{
    for(int i=0; i<PageIO->PageCount; i++)
        PageIO->Pages[i]->TabVisible=false;

    if(MachineTypeChoice==1||MachineTypeChoice==2)                              // HT-1501A & HT-1501A_Plus
    {                                                                           // 關閉 Tube 顯示
//        gbTube1->Visible =false;                                                // HT-1051A & HT-1501A_Plus 已拿掉
//        BtnPanel12->Visible=false;
    }

    sbIOChange0->Click();
    sbIOChange0->Down;

    MySuckerGroup.push_back(new SUCKER_GROUP(tsIO5, &HSys.Suck.MagArmSuck,  1));
    MySuckerGroup.push_back(new SUCKER_GROUP(tsIO5, &HSys.Suck.AutoArmSuck, 0));
   

    //重新排列位置 Start
    for(int i=1; i<int(MySuckerGroup.size()); i++)         //偷雞,只靠Parent判別,所以上面的push back時,相同TabSheet的要排一起
    {
        if(MySuckerGroup[i]->SuckerGroup->Parent==MySuckerGroup[i-1]->SuckerGroup->Parent)
        {
            MySuckerGroup[i]->SuckerGroup->Left=MySuckerGroup[i-1]->SuckerGroup->Left+MySuckerGroup[i-1]->SuckerGroup->Width+8;
        }
    }
}
//---------------------------------------------------------------------------
__fastcall Tfiosetview::Tfiosetview(TComponent* Owner)
     : TForm(Owner)
{
    //20111207 Daver modify
    InputInformationGrid->Cells[0][0]="card";
    InputInformationGrid->Cells[1][0]="port";
    InputInformationGrid->Cells[2][0]="bit";
    InputInformationGrid->Cells[3][0]="name";
    InputInformationGrid->Cells[4][0]="type";

    OutputInformationGrid->Cells[0][0]="card";
    OutputInformationGrid->Cells[1][0]="port";
    OutputInformationGrid->Cells[2][0]="bit";
    OutputInformationGrid->Cells[3][0]="name";
    OutputInformationGrid->Cells[4][0]="type";
    InputInformationGrid->ColWidths[2]=25;
    InputInformationGrid->ColWidths[3]=218;
    OutputInformationGrid->ColWidths[2]=25;
    OutputInformationGrid->ColWidths[3]=218;
    //----------------------------------
    fShow=false;
//    PageIO->Top=palIOForm->Top+10;
    #ifdef SOFT_SIMULATE
    spbTerminalProgram->Visible=true;
    #endif
    sbIOChange0->OnClick(this);
    sbIOChange0->Down=true;
    ComboBox2->ItemIndex=0;
    RadioButton1->Checked=true;
    iMacroTask=1;
    InputTableList=new TList;
    OutputTableList=new TList;
    TimeTick=20;
}
//==============================================================================
void __fastcall Tfiosetview::SetSensorClick(TWinControl *PCtrl)                 // set io database //
{
    AnsiString S="";
    //int i,j,k;
    //bool ret;
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
            SetSensorClick((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
        }
        TBtnPanel *PBtnPanel               = dynamic_cast <TBtnPanel *>(P);
        TMyLed    *PMyLed                  = dynamic_cast <TMyLed *>(P);
        if (PBtnPanel != NULL)                 // 如果是TLabel型態
        {
            PBtnPanel->OnClick=BtnPanelClick;
        }
        else if( PMyLed!=NULL)
        {
            PMyLed->OnClick=SensorClick;
        }
    }
}
//==============================================================================
bool __fastcall Tfiosetview::WaitMacroSensorOn(AnsiString data)                 // macro input on //
{
    for(int i=0; i<HSys.iTotalSensor; i++)
        if(HSys.SenPtr[i].Name==data)
            return HSys.SenPtr[i].IsOn();

    for(int i=0; i<HSys.iTotalCylinder; i++)
        if(HSys.CynPtr[i].OnSensor.Name==data)
            return HSys.CynPtr[i].IsOn();

    for(int i=0;i<HSys.iTotalCylinder;i++)
        if(HSys.CynPtr[i].OffSensor.Name==data)
            return HSys.CynPtr[i].IsOff();

    for(int j=0; j<HSys.iTotalSucker; j++)
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
                if(HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name==data)
                    return HSys.SuckPtr[j].Suck[iR][iC].GetStatus();
    return false;
}
//==============================================================================
bool __fastcall Tfiosetview::WaitMacroSensorOff(AnsiString data)                // macro input off //
{
    for(int i=0; i<HSys.iTotalSensor; i++)
        if(HSys.SenPtr[i].Name==data)
            return HSys.SenPtr[i].IsOff();

    for(int i=0; i<HSys.iTotalCylinder; i++)
        if(HSys.CynPtr[i].OnSensor.Name==data)
            return !HSys.CynPtr[i].IsOn();

    for(int i=0; i<HSys.iTotalCylinder; i++)
        if(HSys.CynPtr[i].OffSensor.Name==data)
            return !HSys.CynPtr[i].IsOff();

    for(int j=0; j<HSys.iTotalSucker; j++)
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
                if(HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name==data)
                    return !HSys.SuckPtr[j].Suck[iR][iC].GetStatus();
    return false;
}
//==============================================================================
void __fastcall Tfiosetview::MacroSwitchOn(AnsiString data)                     // macro output on //
{
    for(int i=0; i<HSys.iTotalSwitch; i++)
    {
        if(HSys.SwPtr[i].Name==data)
        {
            HSys.SwPtr[i].On();                                                 // switch on //
            return;
        }
    }

    for(int i=0; i<HSys.iTotalCylinder; i++)
    {
        if(HSys.CynPtr[i].CylinderName==data)
        {
            HSys.CynPtr[i].On();                                                // cylinder on //
            return;
        }
    }

    for(int j=0; j<HSys.iTotalSucker; j++)
    {
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
            {
                if(HSys.SuckPtr[j].Suck[iR][iC].OnPortName==data)
                {
                    HSys.SuckPtr[j].Suck[iR][iC].OnSuck();                      // suck vacuum on //
                    return;
                }
            }
        }
    }

    for(int j=0; j<HSys.iTotalSucker; j++)
    {
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
            {
                if(HSys.SuckPtr[j].Suck[iR][iC].OffPortName==data)
                {
                    HSys.SuckPtr[j].Suck[iR][iC].OnDestroy();                   // suck destroy on //
                    return;
                }
            }
        }
    }
}
//==============================================================================
void __fastcall Tfiosetview::MacroSwitchOff(AnsiString data)                    // macro output off //
{
    for(int i=0; i<HSys.iTotalSwitch; i++)
    {
        if(HSys.SwPtr[i].Name==data)
        {
            HSys.SwPtr[i].Off();                                                // switch off //
            return;
        }
    }

    for(int i=0; i<HSys.iTotalCylinder; i++)
    {
        if(HSys.CynPtr[i].CylinderName==data)
        {
            HSys.CynPtr[i].Off();                                               // cylinder off //
            return;
        }
    }

    for(int j=0; j<HSys.iTotalSucker; j++)
    {
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
            {
                if(HSys.SuckPtr[j].Suck[iR][iC].OnPortName==data)
                {
                    HSys.SuckPtr[j].Suck[iR][iC].OffSuck();                     // suck vacuum off //
                    return;
                }
            }
        }
    }

    for(int j=0; j<HSys.iTotalSucker; j++)
    {
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
            {
                if(HSys.SuckPtr[j].Suck[iR][iC].OffPortName==data)
                {
                    HSys.SuckPtr[j].Suck[iR][iC].OffDestroy();                  // suck destroy off //
                    return;
                }
            }
        }
    }
}
//==============================================================================
void __fastcall Tfiosetview::DoRunMacro()                                       // do macro //
{
    int &Task=iMacroTask;
    static int LineIndex;
    static AnsiString mcommand,mdata;
    static HTimer Delay;
    int delaytime;
    switch(Task)
    {
        case 1:
            LineIndex=0;
            Task=100;
        case 100:
            mcommand=ListBoxMacro->Items->Strings[LineIndex];
            mdata=ListBoxMacro->Items->Strings[LineIndex+1];
            if(mcommand=="[1] wait senso on")
                Task=200;
            else if(mcommand=="[2] wait sensor off")
                Task=300;
            else if(mcommand=="[3] output on")
                Task=400;
            else if(mcommand=="[4] output off")
                Task=500;
            else if(mcommand=="[5] insert delay time")
                Task=600;
            else if(mcommand=="loop")
                Task=700;
            break;
        case 200:
            if( WaitMacroSensorOn(mdata) )
                Task=1000;
            break;
        case 300:
            if( WaitMacroSensorOff(mdata) )
                Task=1000;
            break;
        case 400:
            MacroSwitchOn(mdata);
            Task=1000;
            break;
        case 500:
            MacroSwitchOff(mdata);
            Task=1000;
            break;
        case 600:
            delaytime=atoi(mdata.c_str());
            Delay.Set(delaytime);
            Delay.On();
            Task=620;
            break;
        case 620:
            if(Delay.Off() )
                Task=1000;
            break;
        case 700:
            Task=1;
            if(mdata=="end")
                sbRunMacro->Down=false;
            break;
        case 1000:
            LineIndex+=2;
            if(LineIndex>=ListBoxMacro->Items->Count)
            {
                Task=1;
                sbRunMacro->Down=false;
            }
            else
            {
                Task=100;
            }
            break;
    }
}
//==============================================================================
void __fastcall Tfiosetview::DoToolLoop()                                       //20111124 Daver modify
{
    static bool Flag=false;
    static int i,j,k,Port1,Port2;
    static char Mask[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    char Byte1=0,Byte2=0;

    TimeCT++;
    TALed *LPtr1[8]={
        ALed1,ALed2 ,ALed3 ,ALed4 ,ALed5 ,ALed6 ,ALed7 ,ALed8 };
    TALed *LPtr2[8]={
        ALed9,ALed10,ALed11,ALed12,ALed13,ALed14,ALed15,ALed16};

    //Input
    //Port1=HexStrToInt(Edit3->Text.c_str());
    //Port2=HexStrToInt(Edit4->Text.c_str());

    if(HSys.bUseRemoteIO)
    {
        Byte1=myMN200IO[2].IOInputByte();
        Byte2=myMN200IO[3].IOInputByte();
    }
    else
    {
        Port1=HexStrToInt(ed_InPort_1->Text.c_str());                               //V1.4
        Port2=HexStrToInt(ed_InPort_2->Text.c_str());                               //V1.4

        // 20111207 Daver add
        if (Port1<0x210)
            Port1-=1;
        if (Port2<0x210)
            Port2-=1;
        //--------------------

//        Byte1=myPISOIO[2].IOInputByte(Port1);
//        Byte2=myPISOIO[3].IOInputByte(Port2);
    }

    for(i=0;i<8;i++)
    {
        if( Byte1 & Mask[i] )
            LPtr1[i]->Value=true;
        else
            LPtr1[i]->Value=false;

        if( Byte2 & Mask[i] )
            LPtr2[i]->Value=true;
        else
            LPtr2[i]->Value=false;
    }

    if(TimeCT<TimeTick)
        return;

    TimeCT=0;
    Flag=!Flag;
    static TCheckBox *Ptr1[16]={
        CheckBox1  ,CheckBox2,
        CheckBox3  ,CheckBox4,
        CheckBox5  ,CheckBox6,
        CheckBox7  ,CheckBox8,
        CheckBox9  ,CheckBox10,
        CheckBox11 ,CheckBox12,
        CheckBox13 ,CheckBox14,
        CheckBox15 ,CheckBox16
    };

    static TCheckBox *Ptr2[16]={
        CheckBox17 ,CheckBox18,
        CheckBox26 ,CheckBox25,
        CheckBox27 ,CheckBox24,
        CheckBox28 ,CheckBox23,
        CheckBox29 ,CheckBox22,
        CheckBox30 ,CheckBox21,
        CheckBox31 ,CheckBox20,
        CheckBox32 ,CheckBox19
    };

    //Output
    Byte1=0;
    Byte2=0;
    //--------------------

    for(i=0;i<8;i++)
    {
        j=i*2;
        k=j+1;
        if(Ptr1[j]->Checked)
        {
            if(Ptr1[k]->Checked)
            {
                if(Flag)
                    Byte1|=Mask[i];
                else
                    Byte1&=~Mask[i];
            }
            else
            {
                Byte1|=Mask[i];
            }
        }
        else
        {
            Byte1&=~Mask[i];
        }

        if(Ptr2[j]->Checked)
        {
            if(Ptr2[k]->Checked)
            {
                if(Flag)
                    Byte2|=Mask[i];
                else
                    Byte2&=~Mask[i];
            }
            else
            {
                Byte2|=Mask[i];
            }
        }
        else
        {
            Byte2&=~Mask[i];
        }
    }

    if(HSys.bUseRemoteIO)
    {
        myMN200IO[0].IOByteOut(Byte1);
        myMN200IO[1].IOByteOut(Byte2);
    }
    else
    {
        Port1=HexStrToInt(ed_OutPort_1->Text.c_str());                              //V1.4
        Port2=HexStrToInt(ed_OutPort_2->Text.c_str());                              //V1.4

        // 20111207 Daver add
        if (Port1<0x240)
            Port1-=1;
        if (Port2<0x240)
            Port2-=1;

//        myPISOIO[0].IOByteOut(Port1, Byte1);
//        myPISOIO[1].IOByteOut(Port2, Byte2);
    }

}
//==============================================================================
void __fastcall Tfiosetview::Timer1Timer(TObject *Sender)                       // timer 1 //
{
    if( fShow==false)
        return;
    ScanIOTable();                                                              // scan input sensor status //
    if(RadioButton4->Checked)                                                   // run io macro //
    {
        if(sbRunMacro->Down==true && SpeedButton9->Down==false)                 // run marco need stop loop //
            DoRunMacro();
    }
    if(PageIO->ActivePage!=tsIOTool)                                            // 離開tool頁面停止loop //
        SpeedButton9->Down=false;
    if(SpeedButton9->Down==true && sbRunMacro->Down==false)
    {
        DoToolLoop();
    }
    //ben 20230823 for SafePLC
    //==>
    if(Enable_PLCSafety_IO==true)
    {
        RefreshSafePLCLed();
    }
    //<==
    //ben 20230823 for SafePLC

    if(bAutoAreaMode)                                                           //Sam 20240605 : 新增 AutoArea IO Mode
    {
        DoAutoAreaIO(0);
        DoAutoAreaIO(1);
    }
    if(bLoopAutoUpDownMode)                                                     //Sam 20240626 : Loop Auto UpDown
    {
        if(AutoTrackModule[0]->bTrackNeedTest())
            AutoTrackModule[0]->DoLoopAutoUpDown();
        if(AutoTrackModule[1]->bTrackNeedTest())
            AutoTrackModule[1]->DoLoopAutoUpDown();
        if(AutoTrackModule[2]->bTrackNeedTest())
            AutoTrackModule[2]->DoLoopAutoUpDown();
        if(AutoTrackModule[3]->bTrackNeedTest())
            AutoTrackModule[3]->DoLoopAutoUpDown();
    }
}
//==============================================================================
void __fastcall Tfiosetview::FormClose(TObject *Sender,                         // form close //
      TCloseAction &Action)
{
    for(int iP=0; iP<Panel14->ControlCount; iP++)
    {
        TControl *P=Panel14->Controls[iP];
        TSpeedButton    *PSpeedButton = dynamic_cast <TSpeedButton  *>(P);
        if(PSpeedButton!=NULL)
        {
            PSpeedButton->Down=false;
        }
    }
    fShow=false;
}
//==============================================================================
void __fastcall Tfiosetview::ShowInputInformation()                             // show input information in input map //
{                                                                               //20111207 Daver modify
    TList *ioList;
    ioSortStru *is,*p1,*p2,p3;
    char str[256];
    AnsiString S;

    ioList=new TList;
    ioList->Clear();
    for(int i=0; i<HSys.iTotalSensor; i++)                                      // sensor //
    {
        if(HSys.SenPtr[i].Port==0 || HSys.SenPtr[i].Name==AnsiString(""))
            continue;
        if(HSys.SenPtr[i].EnableAtDataBase==false)
            continue;

        is=new ioSortStru;
        is->card=HSys.SenPtr[i].Input->GetCard();
        is->port=HSys.SenPtr[i].Input->GetPort();
        is->bit =HSys.SenPtr[i].Input->GetBit();
        is->type=0;
        strcpy(is->name,HSys.SenPtr[i].Name.c_str());
        ioList->Add((ioSortStru *)is);
    };

    for(int i=0; i<HSys.iTotalCylinder; i++)                                    // cylinder on sensor //
    {
//        if( HSys.CynPtr[i].OnSensor.Port==0 || HSys.CynPtr[i].OnSensorName==AnsiString(""))   //V1.6
        if( HSys.CynPtr[i].OnSensor.Port==0 || HSys.CynPtr[i].OnSensor.Name==AnsiString(""))    //V1.7
            continue;
        if( HSys.CynPtr[i].EnableAtDataBase==false)
            continue;

        is=new ioSortStru;
        is->card=HSys.CynPtr[i].OnSensor.Input->GetCard();
        is->port=HSys.CynPtr[i].OnSensor.Input->GetPort();
        is->bit =HSys.CynPtr[i].OnSensor.Input->GetBit();
        is->type=1;
        strcpy(is->name,HSys.CynPtr[i].OnSensor.Name.c_str());
        ioList->Add((ioSortStru *)is);
    };

    for(int i=0; i<HSys.iTotalCylinder; i++)                                    // cylinder off sensor //
    {
//        if( HSys.CynPtr[i].OffSensor.Port==0 || HSys.CynPtr[i].OffSensorName==AnsiString(""))   //V1.6
        if( HSys.CynPtr[i].OffSensor.Port==0 || HSys.CynPtr[i].OffSensor.Name==AnsiString(""))    //V1.7
            continue;
        if( HSys.CynPtr[i].EnableAtDataBase==false)
            continue;

        is=new ioSortStru;
        is->card=HSys.CynPtr[i].OffSensor.Input->GetCard();
        is->port=HSys.CynPtr[i].OffSensor.Input->GetPort();
        is->bit =HSys.CynPtr[i].OffSensor.Input->GetBit();
        is->type=1;
        strcpy(is->name,HSys.CynPtr[i].OffSensor.Name.c_str());
        ioList->Add((ioSortStru *)is);
    };

    for(int j=0; j<HSys.iTotalSucker; j++)
    {
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
            {
                //if( HSys.SuckPtr[j].Suck[j].Sensor!=NULL)
                {
                    S=HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name;
                    if(HSys.SuckPtr[j].Suck[iR][iC].Sensor.Port==0 || S=="")
                        continue;
                    if(HSys.SuckPtr[j].Suck[iR][iC].EnableAtDataBase==false)
                        continue;

                    is=new ioSortStru;
                    is->card=HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetCard();
                    is->port=HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetPort();
                    is->bit =HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetBit();
                    is->type=2;
                    strcpy(is->name,HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name.c_str());
                    ioList->Add((ioSortStru *)is);
                }
            }
        }
    };

    for(int i=0; i<(ioList->Count-1); i++)                                      // ioList 由小到大排序 //
    {
        for(int j=i+1; j<ioList->Count; j++)
        {
            p1=(ioSortStru *)ioList->Items[i];
            p2=(ioSortStru *)ioList->Items[j];
            if( (p1->card>p2->card) ||
                (p1->card==p2->card && p1->port>p2->port) ||
                (p1->card==p2->card && p1->port==p2->port && p1->bit>p2->bit))
            {
                memcpy( (char *)&p3.card, (char *)&p1->card,sizeof(ioSortStru));
                memcpy( (char *)&p1->card, (char *)&p2->card,sizeof(ioSortStru));
                memcpy( (char *)&p2->card, (char *)&p3.card,sizeof(ioSortStru));
            }
        }
    }

    //p1=(ioSortStru *)ioList->Items[0];                                        //V1.8
    if(ioList->Count>0)                                                         //V1.9
        p1=(ioSortStru *)ioList->Items[0];
    else
        return;
    int iStartCard=p1->card;
    int iStartPort=p1->port;
    int ict=0;
    AnsiString asShowStr;
    for(int j=0; j<8; j++)                                                      // input map //
    {
        if(HSys.bUseRemoteIO)
        {
            asShowStr.sprintf("%03d", iStartCard);
        }
        else
        {
            asShowStr.sprintf("0x%03X",iStartCard);
        }
        InputInformationGrid->Cells[0][ict*8+j+1]=asShowStr;
        InputInformationGrid->Cells[1][ict*8+j+1]=iStartPort;
        InputInformationGrid->Cells[2][ict*8+j+1]=j;
        InputInformationGrid->Cells[3][ict*8+j+1]="";
    }

    for(int i=0; i<ioList->Count; i++)
    {
        p1=(ioSortStru *)ioList->Items[i];
        if( iStartPort!=p1->port )
        {
            iStartCard=p1->card;
            iStartPort=p1->port;
            ict++;
            InputInformationGrid->RowCount=ict*8+8+1;
            for(int j=0; j<8; j++)
            {
                if(HSys.bUseRemoteIO)
                {
                    asShowStr.sprintf("%03d", iStartCard);
                }
                else
                {
                    asShowStr.sprintf("0x%03X",iStartCard);
                }
                InputInformationGrid->Cells[0][ict*8+j+1]=asShowStr;
                InputInformationGrid->Cells[1][ict*8+j+1]=iStartPort;
                InputInformationGrid->Cells[2][ict*8+j+1]=j;
                InputInformationGrid->Cells[3][ict*8+j+1]="";
            }
        }
    }
    Memo1->Lines->Clear();
    for(int i=0; i<ioList->Count; i++)
    {
        p1=(ioSortStru *)ioList->Items[i];
        for(int j=0; j<InputInformationGrid->RowCount; j++)
        {
            if( InputInformationGrid->Cells[1][j]==p1->port &&
                InputInformationGrid->Cells[2][j]==p1->bit )
            {
                if( InputInformationGrid->Cells[3][j]=="")
                {
                    InputInformationGrid->Cells[3][j]=p1->name;
                    InputInformationGrid->Cells[4][j]=p1->type;
                }
                else
                {
                    sprintf(str,"sensor name=%s is repeat error with name=%s",p1->name,InputInformationGrid->Cells[3][j].c_str());
                    Memo1->Lines->Add(str);
                }
            }
        }
        delete p1;
    }

    asShowStr="";
    for(int j=1; j<InputInformationGrid->RowCount; j++)
    {
        if (InputInformationGrid->Cells[0][j]!=asShowStr)
            asShowStr=InputInformationGrid->Cells[0][j];
        else
            InputInformationGrid->Cells[0][j]="";

        sprintf(str, "0x%x", atoi(InputInformationGrid->Cells[1][j].c_str()));
        InputInformationGrid->Cells[1][j]=str;
        if(InputInformationGrid->Cells[2][j]!="0")
            InputInformationGrid->Cells[1][j]="";
    }

    delete ioList;
}
//==============================================================================
void __fastcall Tfiosetview::ShowOutputInformation()                            // show output information in input map //
{                                                                               //20111207 Daver modify
    TList *ioList;
    ioSortStru *is,*p1,*p2,p3;
    char str[256];
    AnsiString S;
    ioList=new TList;
    ioList->Clear();
    for(int i=0; i<HSys.iTotalSwitch; i++)                                      // switch //
    {
        if( HSys.SwPtr[i].Port==0 || HSys.SwPtr[i].Name==AnsiString(""))
            continue;
        if( HSys.SwPtr[i].EnableAtDataBase==false)
            continue;

        is=new ioSortStru;
        is->card=HSys.SwPtr[i].Output->GetCard();
        is->port=HSys.SwPtr[i].Output->GetPort();
        is->bit =HSys.SwPtr[i].Output->GetBit();
        is->type=3;
        strcpy(is->name,HSys.SwPtr[i].Name.c_str());
        ioList->Add((ioSortStru *)is);
    };

    for(int i=0; i<HSys.iTotalCylinder; i++)                                    // cylinder switch //
    {
        if(HSys.CynPtr[i].Switch.Port==0 || HSys.CynPtr[i].CylinderName==AnsiString(""))
            continue;
        if(HSys.CynPtr[i].EnableAtDataBase==false)
            continue;

        is=new ioSortStru;
        is->card=HSys.CynPtr[i].Switch.Output->GetCard();
        is->port=HSys.CynPtr[i].Switch.Output->GetPort();
        is->bit= HSys.CynPtr[i].Switch.Output->GetBit();
        is->type=1;
        strcpy(is->name,HSys.CynPtr[i].CylinderName.c_str());
        ioList->Add((ioSortStru *)is);
    };

    for(int j=0; j<HSys.iTotalSucker; j++)
    {
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
            {
                //if( HSys.SuckPtr[i].Suck[j].OnSw!=NULL)
                {
                    S=HSys.SuckPtr[j].Suck[iR][iC].OnPortName;
                    if( HSys.SuckPtr[j].Suck[iR][iC].OnSw.Port==0 || S=="")
                        continue;
                    if( HSys.SuckPtr[j].Suck[iR][iC].EnableAtDataBase==false)
                        continue;

                    is=new ioSortStru;
                    is->card=HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetCard();
                    is->port=HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetPort();
                    is->bit= HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetBit();
                    is->type=2;
                    strcpy(is->name,HSys.SuckPtr[j].Suck[iR][iC].OnPortName.c_str());
                    ioList->Add((ioSortStru *)is);
                }
            }
        }
    };

    for(int j=0; j<HSys.iTotalSucker; j++)
    {
        for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
        {
            for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
            {
                //if(HSys.SuckPtr[j].Suck[iR][iC].OffSw!=NULL)
                {
                    //if( HSys.SuckPtr[i].Suck[iR][iC]->SensorOff Sw->Port
                    S=HSys.SuckPtr[j].Suck[iR][iC].OffPortName;
                    if(HSys.SuckPtr[j].Suck[iR][iC].OffSw.Port==0 || S=="")
                        continue;
                    if(HSys.SuckPtr[j].Suck[iR][iC].EnableAtDataBase==false)
                        continue;

                    is=new ioSortStru;
                    is->card=HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetCard();
                    is->port=HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetPort();
                    is->bit =HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetBit();
                    is->type=2;
                    strcpy(is->name,HSys.SuckPtr[j].Suck[iR][iC].OffPortName.c_str());
                    ioList->Add((ioSortStru *)is);
                }
            }
        }
    };

    for(int i=0; i<(ioList->Count-1); i++)                                      // ioList 由小到大排序 //
    {
        for(int j=i+1; j<ioList->Count; j++)
        {
            p1=(ioSortStru *)ioList->Items[i];
            p2=(ioSortStru *)ioList->Items[j];
            if( (p1->card>p2->card) ||
                (p1->card==p2->card && p1->port>p2->port) ||
                (p1->card==p2->card && p1->port==p2->port && p1->bit>p2->bit ) )
            {
                memcpy( (char *)&p3.card, (char *)&p1->card,sizeof(ioSortStru));
                memcpy( (char *)&p1->card, (char *)&p2->card,sizeof(ioSortStru));
                memcpy( (char *)&p2->card, (char *)&p3.card,sizeof(ioSortStru));
            }
        }
    }

    //p1=(ioSortStru *)ioList->Items[0];                                        //V1.8
    if(ioList->Count>0)                                                         //V1.9
        p1=(ioSortStru *)ioList->Items[0];
    else
        return;
    int iStartCard=p1->card;
    int iStartPort=p1->port;
    int ict=0;
    AnsiString asShowStr;

    for(int j=0; j<8; j++)                                                      // output map //
    {
        asShowStr.sprintf("0x%03X",iStartCard);
        OutputInformationGrid->Cells[0][ict*8+j+1]=asShowStr;
        OutputInformationGrid->Cells[1][ict*8+j+1]=iStartPort;
        OutputInformationGrid->Cells[2][ict*8+j+1]=j;
        OutputInformationGrid->Cells[3][ict*8+j+1]="";
    }

    for(int i=0; i<ioList->Count; i++)
    {
        p1=(ioSortStru *)ioList->Items[i];
        if(iStartPort!=p1->port)
        {
            iStartCard=p1->card;
            iStartPort=p1->port;
            ict++;
            OutputInformationGrid->RowCount=ict*8+8+1;
            for(int j=0; j<8; j++)
            {
                if(HSys.bUseRemoteIO)
                {
                    asShowStr.sprintf("%03d", iStartCard);
                }
                else
                {
                    asShowStr.sprintf("0x%03X",iStartCard);
                }
                OutputInformationGrid->Cells[0][ict*8+j+1]=asShowStr;
                OutputInformationGrid->Cells[1][ict*8+j+1]=iStartPort;
                OutputInformationGrid->Cells[2][ict*8+j+1]=j;
                OutputInformationGrid->Cells[3][ict*8+j+1]="";
            }
        }
    }
    Memo1->Lines->Clear();
    for(int i=0; i<ioList->Count; i++)
    {
        p1=(ioSortStru *)ioList->Items[i];
        for(int j=0; j<OutputInformationGrid->RowCount; j++)
        {
            if(OutputInformationGrid->Cells[1][j]==p1->port &&
               OutputInformationGrid->Cells[2][j]==p1->bit )
            {
                if(OutputInformationGrid->Cells[3][j]=="")
                {
                    OutputInformationGrid->Cells[3][j]=p1->name;
                    OutputInformationGrid->Cells[4][j]=p1->type;
                }
                else
                {
                    sprintf(str,"sensor name=%s is repeat error with name=%s",p1->name,InputInformationGrid->Cells[3][j].c_str());
                    Memo1->Lines->Add(str);
                }
            }
        }
        delete p1;
    }

    asShowStr="";
    for(int j=1; j<OutputInformationGrid->RowCount; j++)
    {
        if (OutputInformationGrid->Cells[0][j]!=asShowStr)
            asShowStr=OutputInformationGrid->Cells[0][j];
        else
            OutputInformationGrid->Cells[0][j]="";

        if(HSys.bUseRemoteIO)
        {
            sprintf(str, "%02d", atoi(OutputInformationGrid->Cells[1][j].c_str()));
        }
        else
        {
            sprintf(str, "0x%x", atoi(OutputInformationGrid->Cells[1][j].c_str()));
        }

        OutputInformationGrid->Cells[1][j]=str;
        if(OutputInformationGrid->Cells[2][j]!="0")
            OutputInformationGrid->Cells[1][j]="";
    }

    delete ioList;
}
//==============================================================================
void __fastcall Tfiosetview::SearchInput(TWinControl *PCtrl)                    // search input //
{
    IO_Led_Sensor *SenP;
    AnsiString S="";
    bool flag=false;//,ret;
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
            dynamic_cast <TRadioGroup*>(P) != NULL)
        {
            SearchInput((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
        }
        TMyLed    *PMyLed                  = dynamic_cast <TMyLed *>(P);
        if(PMyLed!=NULL)
        {
            S=PMyLed->Alias;
            flag=false;
            for(int i=0; i<HSys.iTotalSensor; i++)                              // search sensor //
            {
                if(S==HSys.SenPtr[i].Name)
                {
                    SenP=new IO_Led_Sensor;
                    SenP->Led=PMyLed;
                    SenP->Sen=&HSys.SenPtr[i];
                    //2011.04.14s    klutter
                    //SenP->Led->InPort=HSys.SenPtr[i].Port;                    //20111122 Daver
                    if(HSys.bUseRemoteIO)
                    {
                        int G1=HSys.SenPtr[i].Input->GetCard();
                        int GP=HSys.SenPtr[i].Input->GetPort();
                        SenP->Led->InPort=G1*100+GP;
                    }
                    else
                    {
                        SenP->Led->InPort=HSys.SenPtr[i].Card*0x10+HSys.SenPtr[i].Port;
                    }

                    SenP->Led->InBit =HSys.SenPtr[i].Input->GetBit();
                    //2011.04.14e    klutter
                    InputTableList->Add((IO_Led_Sensor*)SenP);
                    //PMyLed->InPort=HSys.SenPtr[i].Port;
                    //PMyLed->InBit =HSys.SenPtr[i].Bit;
                    //PMyLed->InType=HSys.SenPtr[i].Type;

                    if(SenP->Sen->Enable)
                        flag=true;
                    else if(fPadInterface->IsPadKey(S))
                        flag=true;
                    break;
                }
            }

            if(flag==false)
            {
                for(int i=0; i<HSys.iTotalCylinder; i++)                        // search cylinder //
                {
                    if(S==HSys.CynPtr[i].OnSensor.Name)
                    {
                        SenP=new IO_Led_Sensor;
                        SenP->Led=PMyLed;
                        SenP->Sen=&HSys.CynPtr[i].OnSensor;
                        //2011.04.14s    klutter
                        //SenP->Led->InPort=HSys.CynPtr[i].OnSensor.Port;           //20111122 Daver
                        if(HSys.bUseRemoteIO)
                        {
                            SenP->Led->InPort=HSys.CynPtr[i].OnSensor.Input->GetCard()*100+HSys.CynPtr[i].OnSensor.Input->GetPort();
                        }
                        else
                        {
                            SenP->Led->InPort=HSys.CynPtr[i].OnSensor.Card*0x10+HSys.CynPtr[i].OnSensor.Port;
                        }
                        SenP->Led->InBit =HSys.CynPtr[i].OnSensor.Input->GetBit();

                        //2011.04.14e    klutter
                        InputTableList->Add( (IO_Led_Sensor*)SenP );

                        //PMyLed->InPort=HSys.SenPtr[i].Port;
                        //PMyLed->InBit =HSys.SenPtr[i].Bit;
                        //PMyLed->InType=HSys.SenPtr[i].Type;

                        if(SenP->Sen->Enable)
                            flag=true;
                        break;

                        //if(PMyLed->InPort!=0 && HSys.SenPtr[i].EnableAtDataBase==true)
                        //    flag=true;
                        //break;
                    }
                    else if(S==HSys.CynPtr[i].OffSensor.Name)
                    {
                        SenP=new IO_Led_Sensor;
                        SenP->Led=PMyLed;
                        SenP->Sen=&HSys.CynPtr[i].OffSensor;
                        //2011.04.14s    klutter
                        //SenP->Led->InPort=HSys.CynPtr[i].OffSensor.Port;          //20111122 Daver
                        if(HSys.bUseRemoteIO)
                        {
                            SenP->Led->InPort=HSys.CynPtr[i].OffSensor.Input->GetCard()*100+HSys.CynPtr[i].OffSensor.Input->GetPort();
                        }
                        else
                        {
                            SenP->Led->InPort=HSys.CynPtr[i].OffSensor.Card*0x10+HSys.CynPtr[i].OffSensor.Port;
                        }

                        SenP->Led->InBit =HSys.CynPtr[i].OffSensor.Input->GetBit();
                        //2011.04.14e    klutter
                        //SenP->Sen=&HSys.SenPtr[i];
                        InputTableList->Add( (IO_Led_Sensor*)SenP );

                        //PMyLed->InPort=HSys.SenPtr[i].Port;
                        //PMyLed->InBit =HSys.SenPtr[i].Bit;
                        //PMyLed->InType=HSys.SenPtr[i].Type;

                        if(SenP->Sen->Enable)
                            flag=true;
                        break;
    /*
                        if(PMyLed->InPort!=0 && HSys.SenPtr[i].EnableAtDataBase==true)
                            flag=true;
                        break;
    */
                    }
                }
            }

            if(flag==false)
            {
                for(int j=0; j<HSys.iTotalSucker; j++)
                {
                    for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
                    {
                        for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
                        {
                            if(S==HSys.SuckPtr[j].Suck[iR][iC].Sensor.Name)
                            {
                                SenP=new IO_Led_Sensor;
                                SenP->Led=PMyLed;
                                SenP->Sen=&HSys.SuckPtr[j].Suck[iR][iC].Sensor;
                                // 20111206 Daver add
                                if(HSys.bUseRemoteIO)
                                {
                                    SenP->Led->InPort=HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetCard()*100+HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetPort();
                                }
                                else
                                {
                                    SenP->Led->InPort=HSys.SuckPtr[j].Suck[iR][iC].Sensor.Card*0x10+HSys.SuckPtr[j].Suck[iR][iC].Sensor.Port;
                                }

                                SenP->Led->InBit =HSys.SuckPtr[j].Suck[iR][iC].Sensor.Input->GetBit();
                                //--------------------
                                InputTableList->Add( (IO_Led_Sensor*)SenP );
                                if(SenP->Sen->Enable)
                                    flag=true;
                                break;
                            }
                        }
                    }
                }
            }

            if(flag==false)
            {
                PMyLed->FalseColor=clRed;
                PMyLed->TrueColor=clRed;
            }
        }
    }
}
//==============================================================================
void __fastcall Tfiosetview::SearchOutput(TWinControl *PCtrl)                   // search output //
{
    IO_Panel_Switch *SwP;
    AnsiString S="";
    bool flag=false,flag1=false;//,ret;                                            // ben add flag1 20111017 //
    bool bSearchOK;

    for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
    {
        bSearchOK=false;
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
            SearchOutput((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
        }
        TBtnPanel    *PMyPanel                  = dynamic_cast <TBtnPanel *>(P);
        if( PMyPanel!=NULL)
        {
            S=PMyPanel->Alias;
            flag=false;
            flag1=false;                                                        // ben add 20111017 //
            for(int i=0; i<HSys.iTotalSwitch; i++)
            {
                if(S==HSys.SwPtr[i].Name)
                {
                    SwP=new IO_Panel_Switch;
                    SwP->Pal=PMyPanel;
                    SwP->Sw=&HSys.SwPtr[i];
                    //2011.04.14s    klutter
                    //SwP->Pal->OutPort=HSys.SwPtr[i].Port; //20111122 Daver
                    if(HSys.bUseRemoteIO)
                    {
                        SwP->Pal->OutPort=HSys.SwPtr[i].Output->GetCard()*100+HSys.SwPtr[i].Output->GetPort();
                    }
                    else
                    {
                        SwP->Pal->OutPort=HSys.SwPtr[i].Card*0x10+HSys.SwPtr[i].Port;
                    }
                    SwP->Pal->OutBit=HSys.SwPtr[i].Output->GetBit();
                    //2011.04.14e   klutter
                    OutputTableList->Add( (IO_Panel_Switch*)SwP );
                    bSearchOK=true;
/*                                                                              //V1.2
                    if(SwP->Sw->Name=="SwServerON")
                    {
                        int abc;
                        abc=1;
                    }
*/
                    if(SwP->Sw->Enable)
                        flag=true;
                    if(SwP->Sw->Status())                                       // ben add 20111017 - s //
                        flag1=true;
                    else
                        flag1=false;                                            // ben add 20111017 - e //
                    break ;
                }
            }
            if( bSearchOK )
            {
                if(flag==false)
                {
                    PMyPanel->FalseColor=clRed;
                    PMyPanel->TrueColor=clRed;
                }
                else                                                            // ben add 20111017 - s //
                {
                    if(flag1)
                        PMyPanel->Down=true;
                    else
                        PMyPanel->Down=false;
                }                                                               // ben add 20111017 - e //
                continue;
            }

            for(int i=0; i<HSys.iTotalCylinder; i++)                            // cylinder //
            {
                if(S==HSys.CynPtr[i].CylinderName)
                {
                    SwP=new IO_Panel_Switch;
                    SwP->Pal=PMyPanel;
                    SwP->Sw=&HSys.CynPtr[i].Switch;
                    //2011.04.14s    klutter
                    //SwP->Pal->OutPort=HSys.CynPtr[i].Switch.Port;             //20111122 Daver
                    if(HSys.bUseRemoteIO)
                    {
                        SwP->Pal->OutPort=HSys.CynPtr[i].Switch.Output->GetCard()*100+HSys.CynPtr[i].Switch.Output->GetPort();
                    }
                    else
                    {
                        SwP->Pal->OutPort=HSys.CynPtr[i].Switch.Card*0x10+HSys.CynPtr[i].Switch.Port;
                    }
                    SwP->Pal->OutBit=HSys.CynPtr[i].Switch.Output->GetBit();
                    SwP->Sw->Name=HSys.CynPtr[i].CylinderName;                   //Sam 20170606 修正汽缸卡號沒有顯示問題
                    //2011.04.14e   klutter
                    OutputTableList->Add( (IO_Panel_Switch*)SwP );
                    if(SwP->Sw->Enable)
                        flag=true;
                    if(SwP->Sw->Status())                                       // ben add 20111017 - s //
                        flag1=true;
                    else
                        flag1=false;                                            // ben add 20111017 - e //
                    bSearchOK=true;
                    break;
                }
            }
            if( bSearchOK )
            {
                if(flag==false)
                {
                    PMyPanel->FalseColor=clRed;
                    PMyPanel->TrueColor=clRed;
                }
                else                                                            // ben add 20111017 - s //
                {
                    if(flag1)
                        PMyPanel->Down=true;
                    else
                        PMyPanel->Down=false;
                }                                                               // ben add 20111017 - e //
                continue;
            }

            for(int j=0; j<HSys.iTotalSucker; j++)
            {
                for(int iR=0; iR<HSys.SuckPtr[j].MaxItemR; iR++)
                {
                    for(int iC=0; iC<HSys.SuckPtr[j].MaxItemC; iC++)
                    {
                        if(S.AnsiPos("Needle")!=0)
                        {
                            int abc;
                            abc++;
                        }
                        if(S==HSys.SuckPtr[j].Suck[iR][iC].OnSw.Name)
                        {
                            SwP=new IO_Panel_Switch;
                            SwP->Pal=PMyPanel;
                            SwP->Sw=&HSys.SuckPtr[j].Suck[iR][iC].OnSw;
                            // 20111206 Daver add
                            if(HSys.bUseRemoteIO)
                            {
                                SwP->Pal->OutPort=HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetCard()*100+HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetPort();
                            }
                            else
                            {
                                SwP->Pal->OutPort=HSys.SuckPtr[j].Suck[iR][iC].OnSw.Card*0x10+HSys.SuckPtr[j].Suck[iR][iC].OnSw.Port;
                            }
                            SwP->Pal->OutBit=HSys.SuckPtr[j].Suck[iR][iC].OnSw.Output->GetBit();
                            //------------------
                            OutputTableList->Add( (IO_Panel_Switch*)SwP );
                            if(SwP->Sw->Enable)
                                flag=true;
                            //2012-06-18 Dell Fix-------------
                            if(SwP->Sw->Status())                                   // ben add 20111017 - s //
                                flag1=true;
                            else
                                flag1=false;                                        // ben add 20111017 - e //
                            //2012-06-18 Dell Fix-------------
                            bSearchOK=true;
                            break;
                        }
                        else if(S==HSys.SuckPtr[j].Suck[iR][iC].OffSw.Name)
                        {
                            SwP=new IO_Panel_Switch;
                            SwP->Pal=PMyPanel;
                            SwP->Sw=&HSys.SuckPtr[j].Suck[iR][iC].OffSw;
                            // 20111206 Daver add
                            if(HSys.bUseRemoteIO)
                            {
                                SwP->Pal->OutPort=HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetCard()*100+HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetPort();
                            }
                            else
                            {
                                SwP->Pal->OutPort=HSys.SuckPtr[j].Suck[iR][iC].OffSw.Card*0x10+HSys.SuckPtr[j].Suck[iR][iC].OffSw.Port;
                            }
                            SwP->Pal->OutBit=HSys.SuckPtr[j].Suck[iR][iC].OffSw.Output->GetBit();
                            //------------------
                            OutputTableList->Add( (IO_Panel_Switch*)SwP );
                            if(SwP->Sw->Enable)
                                flag=true;
                            if(SwP->Sw->Status())                                   // ben add 20111017 - s //
                                flag1=true;
                            else
                                flag1=false;                                        // ben add 20111017 - e //
                            bSearchOK=true;
                            break;
                        }
                    }
                }
                if(bSearchOK)
                    break;
            }

            if(bSearchOK)
            {
                if(flag==false)
                {
                    PMyPanel->FalseColor=clRed;
                    PMyPanel->TrueColor=clRed;
                }
                else                                                            // ben add 20111017 - s //
                {
                    if(flag1)
                        PMyPanel->Down=true;
                    else
                        PMyPanel->Down=false;
                }                                                               // ben add 20111017 - e //
                continue;
            }
            if(flag==false)
            {
                PMyPanel->FalseColor=clRed;
                PMyPanel->TrueColor=clRed;
            }
        }
    }
}
//==============================================================================
void __fastcall Tfiosetview::BuildIOTable()                                     // build input and output table //
{
    InputTableList->Clear();
    OutputTableList->Clear();
    SearchInput(this);                                                          // search input sensor //
    SearchOutput(this);                                                         // search output panel //
}
//==============================================================================
void __fastcall Tfiosetview::ScanIOTable()                                      // scan input sensor status //
{
//    int iCard,iPort,iBit;
    bool flag;
    IO_Led_Sensor *SenP;
    for(int i=0;i<InputTableList->Count;i++)
    {
        SenP=(IO_Led_Sensor *)InputTableList->Items[i];
        if(fPadInterface!=NULL &&
           fPadInterface->IsPadKey(SenP->Led->Alias))                                                //Ken 20210702 AddPadInterface
        {
            flag=fPadInterface->ProcessScanKey(SenP->Led->Alias);
        }
        else
        {
            flag=SenP->Sen->IsOn();
        }
        SenP->Led->Value=flag;
    }
}
//==============================================================================
void __fastcall Tfiosetview::FormShow(TObject *Sender)                          // form show //
{
    Left=(1280-Width)/2;
//    Top =(1024-Height)/2;
    Top =0;
    Timer1->Enabled = true; //2011.04.13    klutter
    btnRefresh->Click();
    btnRefresh->Down=false;                                                     // load io data to table //
    BuildIOTable();                                                             // build input and output table //
    SetCompomentHint(fiosetview);                                               // set panel and sensor show hint //
    SetSensorClick(PageIO);                                                     // set io database //
    bOutDataChange=false;
    bNoRestoreIO=false;
    ShowInputInformation();                                                     // show input information in input map //
    ShowOutputInformation();                                                    // show output information in input map //
    fShow=true;
    fPadInterface->SendCommand("t051490000000");                                //Front Pad 初始化
    InitialPadCtrl();
    fPadInterface->SendCommand("t051491000000");

    InitialAutoAreaIOMode();                                                    //Sam 20240605 : 新增 AutoArea IO Mode
    InitialAutoIO();                                                            //Sam 20240626 : Loop Auto UpDown
    tsMagazine1->TabVisible=(MAGAZINE_CNT>=2);                                  //Sam 20240613 : 新增 Magazine 1&3 IO
    tsMagazine2->TabVisible=(MAGAZINE_CNT>=1);
    tsMagazine3->TabVisible=(MAGAZINE_CNT>=3);
    gbBufTray->Visible=(MAGAZINE_CNT==1);
    sbIOChange0->OnClick(sbIOChange0);
    sbIOChange0->Down=true;
    if(HSys.FuncT.iT06_AutoRowType==1)                                          //KenHsieh 20250613 : add T06 Auto Row Type
    {
        pnlAutoRow1_1->Visible=false;
        pnlAutoRow1_2->Visible=false;
        pnlAutoRow1_3->Visible=false;
        ledSnTrackTrayDetect1->Visible=false;
        pnlAutoTrolly1->Visible=true;

        pnlAutoRow2_1->Visible=false;
        pnlAutoRow2_2->Visible=false;
        pnlAutoRow2_3->Visible=false;
        ledSnTrackTrayDetect2->Visible=false;
        pnlAutoTrolly2->Visible=true;

        pnlAutoRow3_1->Visible=false;
        pnlAutoRow3_2->Visible=false;
        pnlAutoRow3_3->Visible=false;
        ledSnTrackTrayDetect3->Visible=false;
        pnlAutoTrolly3->Visible=true;

        pnlAutoRow4_1->Visible=false;
        pnlAutoRow4_2->Visible=false;
        pnlAutoRow4_3->Visible=false;
        ledSnTrackTrayDetect4->Visible=false;
        pnlAutoTrolly4->Visible=true;

        palAutoAreaIO->Visible=false;
        palLoopAuto->Visible=false;
    }
    else
    {
        pnlAutoRow1_1->Visible=true;
        pnlAutoRow1_2->Visible=true;
        pnlAutoRow1_3->Visible=true;
        ledSnTrackTrayDetect1->Visible=true;
        pnlAutoTrolly1->Visible=false;

        pnlAutoRow2_1->Visible=true;
        pnlAutoRow2_2->Visible=true;
        pnlAutoRow2_3->Visible=true;
        ledSnTrackTrayDetect2->Visible=true;
        pnlAutoTrolly2->Visible=false;

        pnlAutoRow3_1->Visible=true;
        pnlAutoRow3_2->Visible=true;
        pnlAutoRow3_3->Visible=true;
        ledSnTrackTrayDetect3->Visible=true;
        pnlAutoTrolly3->Visible=false;

        pnlAutoRow4_1->Visible=true;
        pnlAutoRow4_2->Visible=true;
        pnlAutoRow4_3->Visible=true;
        ledSnTrackTrayDetect4->Visible=true;
        pnlAutoTrolly4->Visible=false;

        palAutoAreaIO->Visible=true;
        palLoopAuto->Visible=true;
    }

    if(MAGAZINE_CNT==1)
    {
        bplSwBreak_MGZ2Arm_Z->Caption="SwBreak_MGZArm_Z";
        bplSwBreak_MGZ1Arm_Z->Visible=false;
        bplSwBreak_MGZ3Arm_Z->Visible=false;
    }
    else if(MAGAZINE_CNT==3)
    {
        bplSwBreak_MGZ2Arm_Z->Caption="SwBreak_MGZ2Arm_Z";
        bplSwBreak_MGZ1Arm_Z->Visible=true;
        bplSwBreak_MGZ3Arm_Z->Visible=true;
    }
}
//==============================================================================
void __fastcall Tfiosetview::IOSetViewOutput(TObject *Sender)                   // output panel view //
{
    int i;
    TBtnPanel *Ptr;
    IO_Panel_Switch *SwP;
    Ptr=(TBtnPanel *) Sender;
    bOutDataChange=true;                                                        // ben add 20110802 output already change //
    for(i=0;i<OutputTableList->Count;i++)
    {
        SwP=(IO_Panel_Switch *)OutputTableList->Items[i];
        if( SwP->Pal==Ptr )
        {
            if(Ptr->Down)
            {
                Ptr->Down=false;
                Ptr->Color=Ptr->FalseColor;
                Ptr->Font->Color=Ptr->FalseFontColor;
                Ptr->BevelInner=bvRaised;
                Ptr->BevelOuter=bvRaised;
                SwP->Sw->Off();
            }
            else
            {
                Ptr->Down=true;

                Ptr->Color=Ptr->TrueColor;
                Ptr->Font->Color=Ptr->TrueFontColor;
                Ptr->BevelInner=bvLowered;
                Ptr->BevelOuter=bvLowered;
                SwP->Sw->On();
            }
        }
    }
}
//==============================================================================
void __fastcall Tfiosetview::OutputEdit(TObject *Sender)                        // edit output data //
{

}
//==============================================================================
void __fastcall Tfiosetview::OutputMacro(TObject *Sender)                       // output巨集 //
{
    TBtnPanel *P;
    P=(TBtnPanel *)Sender;
    IOSetViewOutput(Sender);
    if(P->Down)
        ComboBox2->ItemIndex=2;
    else
        ComboBox2->ItemIndex=3;
    ListBoxMacro->Items->Add(ComboBox2->Items->Strings[ComboBox2->ItemIndex]);
    ListBoxMacro->Items->Add(P->Alias);
    ListBoxMacro->ItemIndex=ListBoxMacro->Items->Count-1;
}
//==============================================================================
void __fastcall Tfiosetview::BtnPanelClick(TObject *Sender)                     // click panel //
{
    if(RadioButton1->Checked)
        IOSetViewOutput(Sender);                                                // output panel view //
    else if(RadioButton2->Checked)
        OutputEdit(Sender);                                                     // edit output data //
    else if(RadioButton3->Checked)
        OutputMacro(Sender);                                                    // output巨集 //
}
//==============================================================================
void __fastcall Tfiosetview::sbIOExitClick(TObject *Sender)                     // exit //
{
    sbIOExit->Down=false;
    Close();
}
//==============================================================================
void __fastcall Tfiosetview::spbTerminalProgramClick(TObject *Sender)           // stop thread and close program //
{
    int ret=Application->MessageBox("Close Program", "Sure close??", MB_YESNO);
    if(ret==IDYES)
    {
        fMaintenance->SaveData();
        FormSysTools->TerminalProgram();
    }
}
//==============================================================================
void __fastcall Tfiosetview::sbIOChange0Click(TObject *Sender)                 // change tabsheet
{
    TSpeedButton *P;
    P =(TSpeedButton *)Sender;
    PageIO->ActivePageIndex=P->Tag-1;
    palIOForm->Caption=P->Caption;
    if(P->Tag-1==8)                                                             // open tool tabsheet //
    {
        ComboBox1->ItemIndex=4;                                                  // 1sec //
        TimeCT=0;
        TimeTick=20;
    }
}
//==============================================================================
void __fastcall Tfiosetview::btnRefreshClick(TObject *Sender)
{
    HSys.LoadIoData();
    LoadIoTable(0, 0, 0);

    HSys.InitialHardwareNameAndLoadDatabaseIO();                                // initial and load all io database //
    SetCompomentHint(fiosetview);                                               // set panel and sensor show hint //
    ShowInputInformation();                                                     // show input information in input map //
    ShowOutputInformation();                                                    // show output information in input map //
    Timer1->Enabled = true;                                                     //2011.04.13    klutter
}
//==============================================================================
void __fastcall Tfiosetview::SpeedButton1Click(TObject *Sender)                 // button refresh //
{
    for(int i=0;i<HSys.iTotalCylinder;i++)
    {
        if(HSys.CynPtr[i].Enable)
        {
            HSys.LastSet.bCynStatus[i]=HSys.CynPtr[i].Switch.OutValue;
        }
    }

    for(int i=0; i<HSys.iTotalSwitch; i++)
    {
        if(HSys.SwPtr[i].Enable)
        {
            HSys.LastSet.bSwStatus[i]=HSys.SwPtr[i].OutValue;
        }
    }
    WriteLastDataIni();
    HSys.InitialHardwareNameAndLoadDatabaseIO();                                // initial and load all io database //
    SetCompomentHint(fiosetview);                                               // set panel and sensor show hint //
    ShowInputInformation();                                                     // show input information in input map //
    ShowOutputInformation();                                                    // show output information in input map //
    Timer1->Enabled = true;                                                     //2011.04.13    klutter
}
//==============================================================================
void __fastcall Tfiosetview::InputEdit(TObject *Sender)                         // edit input data //
{

}
//==============================================================================
void __fastcall Tfiosetview::InputMacro(TObject *Sender)                        // input巨集 //
{
    TMyLed *P;
    P=(TMyLed *)Sender;
    if( ComboBox2->ItemIndex!=0 && ComboBox2->ItemIndex!=1 )                    // != wait sensor on or off //
    {
        ComboBox2->ItemIndex=0;
    }
    ListBoxMacro->Items->Add(ComboBox2->Items->Strings[ComboBox2->ItemIndex]);
    ListBoxMacro->Items->Add(P->Alias);
    ListBoxMacro->ItemIndex=ListBoxMacro->Items->Count-1;
}
//==============================================================================
void __fastcall Tfiosetview::sbClearMacroClick(TObject *Sender)                 // clear macro listbox //
{
    ListBoxMacro->Items->Clear();
}
//==============================================================================
void __fastcall Tfiosetview::sbUndoMacroClick(TObject *Sender)                  // indo macro //
{
    if(ListBoxMacro->Items->Count>=2)
    {
        ListBoxMacro->Items->Delete(ListBoxMacro->Items->Count-1);
        ListBoxMacro->Items->Delete(ListBoxMacro->Items->Count-1);
    }
}
//==============================================================================
void __fastcall Tfiosetview::sbRunMacroClick(TObject *Sender)                   // run macro with timer 1 //
{
    if(ListBoxMacro->Items->Count==0)
        sbRunMacro->Down=false;
    else
    {
        SpeedButton9->Down=false;                                               // stop loop tool //
        sbRunMacro->Down=true;
    }
}
//==============================================================================
void __fastcall Tfiosetview::sbStopMacroClick(TObject *Sender)                  // stop macro //
{
    iMacroTask=1;
    sbRunMacro->Down=false;
}
//==============================================================================
void __fastcall Tfiosetview::ComboBox2Change(TObject *Sender)                   // set macro data //
{
    if(ComboBox2->ItemIndex==4)                                                 // insert delay time //
    {
        ListBoxMacro->Items->Add(ComboBox2->Items->Strings[ComboBox2->ItemIndex]);
        ListBoxMacro->Items->Add(ComboBox3->Text);
    }
    if(ComboBox2->ItemIndex==5)                                                 // loop again
    {
        ListBoxMacro->Items->Add("loop");
        ListBoxMacro->Items->Add("start");
    }
    if(ComboBox2->ItemIndex==6)                                                 // end //
    {
        ListBoxMacro->Items->Add("loop");
        ListBoxMacro->Items->Add("end");
    }
}
//==============================================================================
void __fastcall Tfiosetview::RadioButton1Click(TObject *Sender)                 // select general io //
{
    iMacroTask=1;
    sbRunMacro->Down=false;
}
//==============================================================================
void __fastcall Tfiosetview::SensorClick(TObject *Sender)                       // click sensor //
{
    if(RadioButton2->Checked==true)
        InputEdit(Sender);                                                      // edit input data //
    else if(RadioButton3->Checked==true)
        InputMacro(Sender);                                                     // input巨集 //
}
//==============================================================================
void __fastcall Tfiosetview::SpeedButton9Click(TObject *Sender)                 // enable io change //
{
    TEdit *PtrLane[4]={ed_OutLane_1, ed_OutLane_2, ed_InLane_1, ed_InLane_2};
    TEdit *PtrIP  [4]={ed_OutIP_1,   ed_OutIP_2,   ed_InIP_1,   ed_InIP_2};
    TEdit *PtrPort[4]={ed_OutPort_1, ed_OutPort_2, ed_InPort_1, ed_InPort_2};

    if(SpeedButton9->Down)
    {
        for(int i=0; i<4; i++)                                                  //20111124 Daver add
        {
            PtrLane[i]->Enabled=false;
            PtrIP  [i]->Enabled=false;
            PtrPort[i]->Enabled=false;

            if(HSys.bUseRemoteIO)
            {
                myMN200IO[i].SetPortInformation(atoi(PtrLane[i]->Text.c_str()), AnsiString(PtrIP[i]->Text.c_str()),
                                                atoi(PtrPort[i]->Text.c_str()), 0);
            }
            else
            {
//                myMN200IO[i].SetPortInformation(HexStrToInt(PtrCard[i]->Text.c_str()), 0, 0);
            }
        }
        sbStopMacro->Click();                                                   // stop macro //
    }
    else
    {
        for(int i=0; i<4; i++)
        {
            PtrLane[i]->Enabled=true;
            PtrIP  [i]->Enabled=true;
            PtrPort[i]->Enabled=true;
        }
    }
}
//==============================================================================
void __fastcall Tfiosetview::ComboBox1Change(TObject *Sender)                   // set loop time //
{
    if( ComboBox1->Text =="0.1")
        TimeTick=2;
    else if( ComboBox1->Text =="0.3")
        TimeTick=6;
    else if( ComboBox1->Text =="0.5")
        TimeTick=10;
    else if( ComboBox1->Text =="0.6")
        TimeTick=12;
    else if( ComboBox1->Text =="1")
        TimeTick=20;
    else if( ComboBox1->Text =="2")
        TimeTick=40;
    else if( ComboBox1->Text =="3")
        TimeTick=60;
    else if( ComboBox1->Text =="5")     //Sam 20240515 :  新增IOTool長秒數
        TimeTick=100;
    else if( ComboBox1->Text =="10")
        TimeTick=200;
    else
        TimeTick=20;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::palAllOnMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *Ptr;
    Ptr=(TPanel *)Sender;
    Ptr->BevelInner=bvLowered;
    Ptr->BevelOuter=bvLowered;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::palAllOnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *Ptr;
    Ptr=(TPanel *)Sender;
    Ptr->BevelInner=bvRaised;
    Ptr->BevelOuter=bvRaised;
}
//---------------------------------------------------------------------------
void Tfiosetview::SetCompomentHint(TWinControl *PCtrl)                                       // set panel and sensor show hint //
{
    AnsiString S="";
    char str[256]="";
    //int i;
    bool ret;
    IO_Panel_Switch *SwP;
    IO_Led_Sensor *SenP;

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
            SetCompomentHint((TWinControl *) P);       // 找該物件附屬的物件(遞迴)
        }
        TBtnPanel *PBtnPanel               = dynamic_cast <TBtnPanel *>(P);
        TMyLed    *PMyLed                  = dynamic_cast <TMyLed *>(P);
        if(PBtnPanel != NULL)                 // 如果是TLabel型態
        {
            ret=false;
            for(int j=0; j<OutputTableList->Count; j++)
            {
                if(ret==false)
                {
                    SwP=(IO_Panel_Switch *)OutputTableList->Items[j];
                    if(PBtnPanel->Alias!="" && PBtnPanel->Alias==SwP->Sw->Name)
                    {
                        sprintf(str,"( %s )  %s", SwP->Sw->Output->GetHint(), PBtnPanel->Alias.c_str());
                        ret=true;
                        break;
                    }
                }
            }

            if(PBtnPanel->Alias!="")
            {
                if(ret==false)
                {
                    sprintf(str,"(  )  %s", PBtnPanel->Alias.c_str());
                }

                PBtnPanel->ShowHint=true;
                PBtnPanel->Hint=str;
            }
        }
        else if(PMyLed!=NULL)
        {
            ret=false;
            for(int j=0; j<InputTableList->Count; j++)
            {
                if(ret==false)
                {
                    SenP=(IO_Led_Sensor *)InputTableList->Items[j];
                    if(PMyLed->Alias!="" && PMyLed->Alias==SenP->Sen->Name)
                    {
                        sprintf(str,"( %s )  %s", SenP->Sen->Input->GetHint(), PMyLed->Alias.c_str());
                        ret=true;
                        break;
                    }
                }
            }

            if(PMyLed->Alias!="")
            {
                if(ret==false)
                {
                    sprintf(str,"(  )  %s", PMyLed->Alias.c_str());
                }

                PMyLed->ShowHint=true;
                PMyLed->Hint=str;
            }
        }
    }
}
//---------------------------------------------------------------------------
void Tfiosetview::RefreshSafePLCLed()
{
    aLedSPLC000->Value = bPLCInData[0][1][0];
    aLedSPLC001->Value = bPLCInData[0][1][1];
    aLedSPLC002->Value = bPLCInData[0][1][2];
    aLedSPLC003->Value = bPLCInData[0][1][3];
    aLedSPLC004->Value = bPLCInData[0][1][4];
    aLedSPLC005->Value = bPLCInData[0][1][5];
    aLedSPLC006->Value = bPLCInData[0][1][6];
    aLedSPLC007->Value = bPLCInData[0][1][7];

    aLedSPLC010->Value = bPLCInData[0][2][0];
    aLedSPLC011->Value = bPLCInData[0][2][1];
    aLedSPLC012->Value = bPLCInData[0][2][2];
    aLedSPLC013->Value = bPLCInData[0][2][3];
    aLedSPLC014->Value = bPLCInData[0][2][4];
    aLedSPLC015->Value = bPLCInData[0][2][5];
    aLedSPLC016->Value = bPLCInData[0][2][6];
    aLedSPLC017->Value = bPLCInData[0][2][7];

    aLedSPLC020->Value = bPLCInData[0][3][0];
    aLedSPLC021->Value = bPLCInData[0][3][1];
    aLedSPLC022->Value = bPLCInData[0][3][2];
    aLedSPLC023->Value = bPLCInData[0][3][3];
    aLedSPLC024->Value = bPLCInData[0][3][4];
    aLedSPLC025->Value = bPLCInData[0][3][5];
    aLedSPLC026->Value = bPLCInData[0][3][6];
    aLedSPLC027->Value = bPLCInData[0][3][7];

    aLedSPLC030->Value = bPLCInData[0][4][0];
    aLedSPLC031->Value = bPLCInData[0][4][1];
    aLedSPLC032->Value = bPLCInData[0][4][2];
    aLedSPLC033->Value = bPLCInData[0][4][3];
    aLedSPLC034->Value = bPLCInData[0][4][4];
    aLedSPLC035->Value = bPLCInData[0][4][5];
    aLedSPLC036->Value = bPLCInData[0][4][6];
    aLedSPLC037->Value = bPLCInData[0][4][7];

    aLedSPLC040->Value = bPLCInData[0][5][0];
    aLedSPLC041->Value = bPLCInData[0][5][1];
    aLedSPLC042->Value = bPLCInData[0][5][2];
    aLedSPLC043->Value = bPLCInData[0][5][3];
    aLedSPLC044->Value = bPLCInData[0][5][4];
    aLedSPLC045->Value = bPLCInData[0][5][5];
    aLedSPLC046->Value = bPLCInData[0][5][6];
    aLedSPLC047->Value = bPLCInData[0][5][7];

    aLedSPLC050->Value = bPLCInData[0][6][0];
    aLedSPLC051->Value = bPLCInData[0][6][1];
    aLedSPLC052->Value = bPLCInData[0][6][2];
    aLedSPLC053->Value = bPLCInData[0][6][3];
    aLedSPLC054->Value = bPLCInData[0][6][4];
    aLedSPLC055->Value = bPLCInData[0][6][5];
    aLedSPLC056->Value = bPLCInData[0][6][6];
    aLedSPLC057->Value = bPLCInData[0][6][7];

    aLedSPLC060->Value = bPLCInData[0][7][0];
    aLedSPLC061->Value = bPLCInData[0][7][1];
    aLedSPLC062->Value = bPLCInData[0][7][2];
    aLedSPLC063->Value = bPLCInData[0][7][3];
    aLedSPLC064->Value = bPLCInData[0][7][4];
    aLedSPLC065->Value = bPLCInData[0][7][5];
    aLedSPLC066->Value = bPLCInData[0][7][6];
    aLedSPLC067->Value = bPLCInData[0][7][7];

    aLedSPLC070->Value = bPLCInData[0][8][0];
    aLedSPLC071->Value = bPLCInData[0][8][1];
    aLedSPLC072->Value = bPLCInData[0][8][2];
    aLedSPLC073->Value = bPLCInData[0][8][3];
    aLedSPLC074->Value = bPLCInData[0][8][4];
    aLedSPLC075->Value = bPLCInData[0][8][5];
    aLedSPLC076->Value = bPLCInData[0][8][6];
    aLedSPLC077->Value = bPLCInData[0][8][7];
}
//<==
//ben 20230823 for SafePLC
//---------------------------------------------------------------------------
void Tfiosetview::InitialPadCtrl()
{
    for(int iP=0; iP<pn_PadInterface_Rear->ControlCount; iP++)
    {
        TControl *P=pn_PadInterface_Rear->Controls[iP];
        TBtnPanelLane *BtnPanelLane=dynamic_cast <TBtnPanelLane *>(P);
        if(BtnPanelLane!=NULL)
        {
            BtnPanelLane->OnMouseDown=sb_PadInterface_RearPowerOffMouseDown;
        }
        TMyLed *myL=dynamic_cast <TMyLed *>(P);
        if(myL!=NULL)
        {
            myL->Enabled=true;
        }
    }
}
//---------------------------------------------------------------------------
void Tfiosetview::InitialAll()
{
    InitialPadCtrl();
}
//---------------------------------------------------------------------------

void __fastcall Tfiosetview::sb_PadInterface_RearPowerOffMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    TBtnPanelLane *BtnPanelLane=dynamic_cast <TBtnPanelLane *>(Sender);
    BtnPanelLane->Down=!BtnPanelLane->Down;
    for(int i=0; i<fPadInterface->CheckPadItem; i++)
    {
        if(fPadInterface->PadItem[i].btnEvent->Name.Pos(BtnPanelLane->Name)>0)
        {
            fPadInterface->PadItem[i].btnEvent->Down=BtnPanelLane->Down;
            fPadInterface->PadButtonClick(fPadInterface->PadItem[i].btnEvent);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::sb_IO_CommunicationPadClick(TObject *Sender)
{
    fPadInterface->ShowModal();
}
//---------------------------------------------------------------------------
void Tfiosetview::LoadIoTable(int iType, int iLane, int iIP)
{
    AnsiString sType[4]={AnsiString("Sensor"), AnsiString("Sucker"), AnsiString("Switch"), AnsiString("Cylinder")};
    int iSize=HSys.IOTable.size();
    int iRowCount=0;
    bool bAdd=false;
    iLane--;
    iType--;

    TStringList *Strs=new TStringList();

    strngrdIoTable->ColCount=16;
    strngrdIoTable->ColWidths[ 0]=50;
    strngrdIoTable->ColWidths[ 1]=100;
    strngrdIoTable->ColWidths[ 2]=200;
    strngrdIoTable->ColWidths[ 3]=50;
    strngrdIoTable->ColWidths[ 4]=50;
    strngrdIoTable->ColWidths[ 5]=50;
    strngrdIoTable->ColWidths[ 6]=50;
    strngrdIoTable->ColWidths[ 7]=50;
    strngrdIoTable->ColWidths[ 8]=50;
    strngrdIoTable->ColWidths[ 9]=50;
    strngrdIoTable->ColWidths[10]=50;
    strngrdIoTable->ColWidths[11]=50;
    strngrdIoTable->ColWidths[12]=50;
    strngrdIoTable->ColWidths[13]=50;
    strngrdIoTable->ColWidths[14]=50;
    strngrdIoTable->ColWidths[15]=300;

    strngrdIoTable->Cells[ 0][0]="No";
    strngrdIoTable->Cells[ 1][0]="Type";
    strngrdIoTable->Cells[ 2][0]="Alias";
    strngrdIoTable->Cells[ 3][0]="Lane";
    strngrdIoTable->Cells[ 4][0]="ModuleType";
    strngrdIoTable->Cells[ 5][0]="IP";
    strngrdIoTable->Cells[ 6][0]="Port";
    strngrdIoTable->Cells[ 7][0]="Bit";
    strngrdIoTable->Cells[ 8][0]="InType";
    strngrdIoTable->Cells[ 9][0]="ISA Base";
    strngrdIoTable->Cells[10][0]="Enable";
    strngrdIoTable->Cells[11][0]="OnAlarmTime";
    strngrdIoTable->Cells[12][0]="OffAlarmTime";
    strngrdIoTable->Cells[13][0]="OnDelayTime";
    strngrdIoTable->Cells[14][0]="OffDelayTime";
    strngrdIoTable->Cells[15][0]="Note";

    for(int i=1; i<strngrdIoTable->RowCount; i++)
    {
        for(int j=0; j<16; j++)
            strngrdIoTable->Cells[j][i]="";
    }

    if(iType<0 && iLane<0)
    {
        strngrdIoTable->RowCount=iSize+1;
    }
    else
    {
        strngrdIoTable->RowCount=1;
        iRowCount=0;
    }

    for(int i=0; i<iSize; i++)
    {
        if(iType<0 && iLane<0)
        {
            Strs->CommaText=HSys.IOTable[i]->_CommaText;
            for(int j=0; j<Strs->Count; j++)
            {
                strngrdIoTable->Cells[j+1][i+1]=Strs->Strings[j];
            }
            strngrdIoTable->Cells[0][i+1]=HSys.IOTable[i]->Tag;
        }
        else
        {
            if(i==0)
            {
                iRowCount++;
            }
            else if(iType>=0)
            {
                if(HSys.IOTable[i]->Type.AnsiPos(sType[iType])!=0)
                {
                    if(iLane<0 || HSys.IOTable[i]->iLane==iLane)
                    {
                        bAdd=true;
                    }
                }
            }
            else
            {
                if(iLane<0 || HSys.IOTable[i]->iLane==iLane)
                {
                    bAdd=true;
                }
            }

            if(bAdd)
            {
                strngrdIoTable->RowCount=iRowCount+1;
                strngrdIoTable->Cells[ 0][iRowCount]=HSys.IOTable[i]->Tag;
                strngrdIoTable->Cells[ 1][iRowCount]=HSys.IOTable[i]->Type;
                strngrdIoTable->Cells[ 2][iRowCount]=HSys.IOTable[i]->Alias;
                strngrdIoTable->Cells[ 3][iRowCount]=(HSys.IOTable[i]->iLane        ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iLane);
                strngrdIoTable->Cells[ 4][iRowCount]=(HSys.IOTable[i]->iModuleType  ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iModuleType);
                if(HSys.IOTable[i]->iISABase==eMotionNet)
                {
                    if(HSys.IOTable[i]->iIP==-1)
                    {
                        strngrdIoTable->Cells[5][iRowCount]=AnsiString("");
                    }
                    else if(HSys.IOTable[i]->iIP>=0 && HSys.IOTable[i]->iIP<10)
                    {
                        strngrdIoTable->Cells[5][iRowCount]=AnsiString(HSys.IOTable[i]->iIP);
                    }
                    else
                    {
                        char IP=HSys.IOTable[i]->iIP-10+'A';
                        strngrdIoTable->Cells[5][iRowCount]=IP;
                    }
                }
                strngrdIoTable->Cells[ 7][iRowCount]=(HSys.IOTable[i]->iBit         ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iBit);
                strngrdIoTable->Cells[ 8][iRowCount]=(HSys.IOTable[i]->iInType      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iInType);
                strngrdIoTable->Cells[ 9][iRowCount]=(HSys.IOTable[i]->iISABase     ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iISABase);

                //jou 20231016 : 修正CE PLC IO load table error
                if(HSys.IOTable[i]->iISABase==e_PLCbase)    //Sam 20240705 : 修正 PLC IO讀寫異常
                    strngrdIoTable->Cells[ 6][iRowCount]=(HSys.IOTable[i]->iPort    ==-1)?AnsiString(""):"0x"+IntToHex(HSys.IOTable[i]->iPort, 3);
                else
                    strngrdIoTable->Cells[ 6][iRowCount]=(HSys.IOTable[i]->iPort    ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iPort);

                strngrdIoTable->Cells[10][iRowCount]=(HSys.IOTable[i]->iEnable      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iEnable);
                strngrdIoTable->Cells[11][iRowCount]=(HSys.IOTable[i]->iOnAlarmTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnAlarmTime);
                strngrdIoTable->Cells[12][iRowCount]=(HSys.IOTable[i]->iOffAlarmTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffAlarmTime);
                strngrdIoTable->Cells[13][iRowCount]=(HSys.IOTable[i]->iOnDelayTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnDelayTime);
                strngrdIoTable->Cells[14][iRowCount]=(HSys.IOTable[i]->iOffDelayTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffDelayTime);
                iRowCount++;
                bAdd=false;
            }
        }
    }
    if(strngrdIoTable->RowCount<=1)
        strngrdIoTable->RowCount=2;
    strngrdIoTable->FixedRows=1;
    Strs->Clear();
    delete Strs;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::sbUpdateClick(TObject *Sender)
{
    int Tag;
    TStringList *sList=new TStringList();
    TStringList *SL=new TStringList();

    SL->Clear();
    SL->Add("IOType");
    SL->Add("Alias");
    SL->Add("Lane");
    SL->Add("ModuleType");
    SL->Add("IP");
    SL->Add("Port");
    SL->Add("Bit");
    SL->Add("InType");
    SL->Add("ISABase");
    SL->Add("Enable");
    SL->Add("OnAlarmTime");
    SL->Add("OffAlarmTime");
    SL->Add("OnDelayTime");
    SL->Add("OffDelayTime");
    SL->Add("Note");
    sList->Add(SL->CommaText);

    for(int i=1; i<strngrdIoTable->RowCount; i++)
    {
        if(strngrdIoTable->Cells[0][i]=="")
        {
            HSys.IOTable.push_back(new TIODATA(""));
            Tag=HSys.IOTable[HSys.IOTable.size()-1]->Tag;
        }
        else
        {
            Tag=atoi(strngrdIoTable->Cells[0][i].c_str());
        }

        HSys.IOTable[Tag]->Type=strngrdIoTable->Cells[1][i];
        HSys.IOTable[Tag]->Alias=strngrdIoTable->Cells[2][i];
        if(strngrdIoTable->Cells[ 3][i]!="")
            HSys.IOTable[Tag]->iLane=atoi(strngrdIoTable->Cells[3][i].c_str());
        else
            HSys.IOTable[Tag]->iLane=-1;
        if(strngrdIoTable->Cells[ 4][i]!="")
            HSys.IOTable[Tag]->iModuleType=atoi(strngrdIoTable->Cells[4][i].c_str());
        else
            HSys.IOTable[Tag]->iModuleType=-1;

        if(strngrdIoTable->Cells[ 7][i]!="")
            HSys.IOTable[Tag]->iBit=atoi(strngrdIoTable->Cells[7][i].c_str());
        else
            HSys.IOTable[Tag]->iBit=-1;
        if(strngrdIoTable->Cells[ 8][i]!="")
            HSys.IOTable[Tag]->iInType=atoi(strngrdIoTable->Cells[8][i].c_str());
        else
            HSys.IOTable[Tag]->iInType=-1;

        if(strngrdIoTable->Cells[ 9][i]!="")
            HSys.IOTable[Tag]->iISABase=atoi(strngrdIoTable->Cells[9][i].c_str());
        else
            HSys.IOTable[Tag]->iISABase=-1;

        //往下移到取得iISABase
        if(strngrdIoTable->Cells[ 5][i]!="")
        {
            if(HSys.IOTable[Tag]->iISABase==eISABase ||                         //Nickliu 20230310 add Suck ISABase Type
               HSys.IOTable[Tag]->iISABase==ePCI1735U || HSys.IOTable[Tag]->iISABase==e_PLCbase)    //JerryYang 20240724 : fix
            {
                HSys.IOTable[Tag]->iIP=-1;
            }
            else
            {
                AnsiString sIP=strngrdIoTable->Cells[5][i];
                if(sIP.AnsiCompare("A")>=0 && sIP.AnsiCompare("Z")<=0)
                {
                    char *QQ=sIP.c_str();
                    HSys.IOTable[Tag]->iIP=int(QQ[0]-'A')+10;
                }
                else if(sIP=="")
                {
                    HSys.IOTable[Tag]->iIP=-1;
                }
                else //if(sIP.AnsiCompare("0")>=0 && sIP.AnsiCompare("9")<=0)
                {
                    HSys.IOTable[Tag]->iIP=atoi(sIP.c_str());
                }
            }
        }
        else
        {
            HSys.IOTable[Tag]->iIP=-1;
        }

        if(strngrdIoTable->Cells[ 6][i]!="")
        {
            if(HSys.IOTable[Tag]->iISABase==eISABase ||                         //Nickliu 20230310 add Suck ISABase Type
               HSys.IOTable[Tag]->iISABase==ePCI1735U || HSys.IOTable[Tag]->iISABase==e_PLCbase)    //Sam 20240705 : 修正 PLC IO讀寫異常
                HSys.IOTable[Tag]->iPort=HexStrToInt(strngrdIoTable->Cells[6][i].c_str());
            else
                HSys.IOTable[Tag]->iPort=atoi(strngrdIoTable->Cells[6][i].c_str());
        }
        else
        {
            HSys.IOTable[Tag]->iPort=-1;
        }

        if(strngrdIoTable->Cells[10][i]!="")
            HSys.IOTable[Tag]->iEnable=atoi(strngrdIoTable->Cells[10][i].c_str());
        else
            HSys.IOTable[Tag]->iEnable=-1;
        if(strngrdIoTable->Cells[11][i]!="")
            HSys.IOTable[Tag]->iOnAlarmTime=atoi(strngrdIoTable->Cells[11][i].c_str());
        else
            HSys.IOTable[Tag]->iOnAlarmTime=-1;
        if(strngrdIoTable->Cells[12][i]!="")
            HSys.IOTable[Tag]->iOffAlarmTime=atoi(strngrdIoTable->Cells[12][i].c_str());
        else
            HSys.IOTable[Tag]->iOffAlarmTime=-1;
        if(strngrdIoTable->Cells[13][i]!="")
            HSys.IOTable[Tag]->iOnDelayTime=atoi(strngrdIoTable->Cells[13][i].c_str());
        else
            HSys.IOTable[Tag]->iOnDelayTime=-1;
        if(strngrdIoTable->Cells[14][i]!="")
            HSys.IOTable[Tag]->iOffDelayTime=atoi(strngrdIoTable->Cells[14][i].c_str());
        else
            HSys.IOTable[Tag]->iOffDelayTime=-1;
    }

    for(unsigned int i=0; i<HSys.IOTable.size(); i++)
    {
        if(HSys.IOTable[i]->bDelete==false)
        {
            SL->Clear();
            SL->Add(HSys.IOTable[i]->Type);
            SL->Add(HSys.IOTable[i]->Alias);
            SL->Add((HSys.IOTable[i]->iLane        ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iLane));
            SL->Add((HSys.IOTable[i]->iModuleType  ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iModuleType));

            if(HSys.IOTable[i]->iISABase==eMotionNet)
            {
                if(HSys.IOTable[i]->iIP==-1)
                {
                    SL->Add(AnsiString(""));
                }
                else if(HSys.IOTable[i]->iIP>=0 && HSys.IOTable[i]->iIP<10)
                {
                    SL->Add(AnsiString(HSys.IOTable[i]->iIP));
                }
                else
                {
                    char IP=HSys.IOTable[i]->iIP-10+'A';
                    SL->Add(AnsiString(IP));
                }
            }
            else
            {
                SL->Add((HSys.IOTable[i]->iIP      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iIP));
            }

            //jou 20231016 : 修正CE PLC IO load table error
            if(HSys.IOTable[i]->iISABase==e_PLCbase)    //Sam 20240705 : 修正 PLC IO讀寫異常
                SL->Add((HSys.IOTable[i]->iPort    ==-1)?AnsiString(""):"0x"+IntToHex(HSys.IOTable[i]->iPort,3));
            else
                SL->Add((HSys.IOTable[i]->iPort    ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iPort));

            SL->Add((HSys.IOTable[i]->iBit         ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iBit));
            SL->Add((HSys.IOTable[i]->iInType      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iInType));
            SL->Add((HSys.IOTable[i]->iISABase     ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iISABase));
            SL->Add((HSys.IOTable[i]->iEnable      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iEnable));
            SL->Add((HSys.IOTable[i]->iOnAlarmTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnAlarmTime));
            SL->Add((HSys.IOTable[i]->iOffAlarmTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffAlarmTime));
            SL->Add((HSys.IOTable[i]->iOnDelayTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnDelayTime));
            SL->Add((HSys.IOTable[i]->iOffDelayTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffDelayTime));
            SL->Add("");
            sList->Add(SL->CommaText);
        }
    }

    sList->SaveToFile(HSys.IoTablePath);
    sList->Clear();
    SL->Clear();
    delete sList;
    delete SL;

    btnRefresh->Click();
    sbUpdate->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnModifyClick(TObject *Sender)
{
    if(iSelectRow>0 && iSelectCol>0)
    {
        edtTemp->Text=strngrdIoTable->Cells[iSelectCol][iSelectRow];
        if(strngrdIoTable->Cells[iSelectCol][0].AnsiPos(AnsiString("Type"))!=0 ||
           strngrdIoTable->Cells[iSelectCol][0].AnsiPos(AnsiString("Alias"))!=0     ||
           strngrdIoTable->Cells[iSelectCol][0].AnsiPos(AnsiString("Note"))!=0)
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_NO_SYMBOL|N_NO_SPACE);
        }
        else
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_INTEGER);
        }
        strngrdIoTable->Cells[iSelectCol][iSelectRow]=edtTemp->Text;
    }
    btnModify->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnAddIOClick(TObject *Sender)
{
    int iRow=strngrdIoTable->RowCount;
    strngrdIoTable->RowCount=iRow+1;
    for(int j=0; j<strngrdIoTable->ColCount; j++)
    {
        strngrdIoTable->Cells[j][iRow]="";
    }
    btnAddIO->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnDeleteIOClick(TObject *Sender)
{
    if(iSelectRow==0)
        return;
    Tag=atoi(strngrdIoTable->Cells[0][iSelectRow].c_str());
    for(int i=iSelectRow; i<strngrdIoTable->RowCount-1; i++)
    {
        for(int j=0; j<strngrdIoTable->ColCount; j++)
        {
            strngrdIoTable->Cells[j][i]=strngrdIoTable->Cells[j][i+1];
        }
    }
    strngrdIoTable->RowCount=strngrdIoTable->RowCount-1;
    HSys.IOTable[Tag]->bDelete=true;
    btnDeleteIO->Down=false;
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::cbbTypeChange(TObject *Sender)
{
    LoadIoTable(cbbType->ItemIndex, cbbLane->ItemIndex, 0);
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::edtSearchIOChange(TObject *Sender)
{
    if(edtSearchIO->Text!="" && edtSearchIO->Text.Length()>=2)
    {
        int iSize=HSys.IOTable.size();
        int iRowCount=0;
        bool bAdd=false;
        AnsiString S;
        TStringList *Strs=new TStringList();

        strngrdIoTable->ColCount=16;
        strngrdIoTable->ColWidths[ 0]=50;
        strngrdIoTable->ColWidths[ 1]=100;
        strngrdIoTable->ColWidths[ 2]=200;
        strngrdIoTable->ColWidths[ 3]=50;
        strngrdIoTable->ColWidths[ 4]=50;
        strngrdIoTable->ColWidths[ 5]=50;
        strngrdIoTable->ColWidths[ 6]=50;
        strngrdIoTable->ColWidths[ 7]=50;
        strngrdIoTable->ColWidths[ 8]=50;
        strngrdIoTable->ColWidths[ 9]=50;
        strngrdIoTable->ColWidths[10]=50;
        strngrdIoTable->ColWidths[11]=50;
        strngrdIoTable->ColWidths[12]=50;
        strngrdIoTable->ColWidths[13]=50;
        strngrdIoTable->ColWidths[14]=50;
        strngrdIoTable->ColWidths[15]=300;

        strngrdIoTable->Cells[ 0][0]="No";
        strngrdIoTable->Cells[ 1][0]="Type";
        strngrdIoTable->Cells[ 2][0]="Alias";
        strngrdIoTable->Cells[ 3][0]="Lane";
        strngrdIoTable->Cells[ 4][0]="ModuleType";
        strngrdIoTable->Cells[ 5][0]="IP";
        strngrdIoTable->Cells[ 6][0]="Port";
        strngrdIoTable->Cells[ 7][0]="Bit";
        strngrdIoTable->Cells[ 8][0]="InType";
        strngrdIoTable->Cells[ 9][0]="ISA Base";
        strngrdIoTable->Cells[10][0]="Enable";
        strngrdIoTable->Cells[11][0]="OnAlarmTime";
        strngrdIoTable->Cells[12][0]="OffAlarmTime";
        strngrdIoTable->Cells[13][0]="OnDelayTime";
        strngrdIoTable->Cells[14][0]="OffDelayTime";
        strngrdIoTable->Cells[15][0]="Note";

        for(int i=1; i<strngrdIoTable->RowCount; i++)
        {
            for(int j=0; j<16; j++)
                strngrdIoTable->Cells[j][i]="";
        }

        strngrdIoTable->RowCount=1;
        iRowCount=0;

        for(int i=0; i<iSize; i++)
        {
            if(i==0)
            {
                iRowCount++;
            }
            else
            {
                Strs->CommaText=HSys.IOTable[i]->_CommaText;
                if(Strs->Count>2)
                    S=Strs->Strings[1].UpperCase();
                else
                    S="";

                if(S!="" && S.AnsiPos(edtSearchIO->Text.UpperCase())>=1)
                {
                    bAdd=true;
                }
            }

            if(bAdd)
            {
                strngrdIoTable->RowCount=iRowCount+1;
                strngrdIoTable->Cells[ 0][iRowCount]=HSys.IOTable[i]->Tag;
                strngrdIoTable->Cells[ 1][iRowCount]=HSys.IOTable[i]->Type;
                strngrdIoTable->Cells[ 2][iRowCount]=HSys.IOTable[i]->Alias;
                strngrdIoTable->Cells[ 3][iRowCount]=(HSys.IOTable[i]->iLane        ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iLane);
                strngrdIoTable->Cells[ 4][iRowCount]=(HSys.IOTable[i]->iModuleType  ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iModuleType);
                strngrdIoTable->Cells[ 5][iRowCount]=(HSys.IOTable[i]->iIP          ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iIP);
                if(HSys.IOTable[i]->iISABase==e_PLCbase)    //Sam 20240705 : 修正 PLC IO讀寫異常
                    strngrdIoTable->Cells[ 6][iRowCount]=(HSys.IOTable[i]->iPort    ==-1)?AnsiString(""):"0x"+IntToHex(HSys.IOTable[i]->iPort, 3);
                else
                    strngrdIoTable->Cells[ 6][iRowCount]=(HSys.IOTable[i]->iPort    ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iPort);
                strngrdIoTable->Cells[ 7][iRowCount]=(HSys.IOTable[i]->iBit         ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iBit);
                strngrdIoTable->Cells[ 8][iRowCount]=(HSys.IOTable[i]->iInType      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iInType);
                strngrdIoTable->Cells[ 9][iRowCount]=(HSys.IOTable[i]->iISABase     ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iISABase);
                strngrdIoTable->Cells[10][iRowCount]=(HSys.IOTable[i]->iEnable      ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iEnable);
                strngrdIoTable->Cells[11][iRowCount]=(HSys.IOTable[i]->iOnAlarmTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnAlarmTime);
                strngrdIoTable->Cells[12][iRowCount]=(HSys.IOTable[i]->iOffAlarmTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffAlarmTime);
                strngrdIoTable->Cells[13][iRowCount]=(HSys.IOTable[i]->iOnDelayTime ==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOnDelayTime);
                strngrdIoTable->Cells[14][iRowCount]=(HSys.IOTable[i]->iOffDelayTime==-1)?AnsiString(""):AnsiString(HSys.IOTable[i]->iOffDelayTime);
                iRowCount++;
                bAdd=false;
            }
        }
        if(strngrdIoTable->RowCount<=1)
            strngrdIoTable->RowCount=2;
        strngrdIoTable->FixedRows=1;
        Strs->Clear();
        delete Strs;
    }
    else if(edtSearchIO->Text=="")
    {
        LoadIoTable(cbbType->ItemIndex, cbbLane->ItemIndex, 0);
    }
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::edtTempMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_NO_SYMBOL);
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::strngrdIoTableDblClick(TObject *Sender)
{
    btnModify->Click();
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::strngrdIoTableSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    iSelectRow=ARow;
    iSelectCol=ACol;
}
//------------------------------------------------------------------------------
void __fastcall Tfiosetview::btnAutoAreaIOClick(TObject *Sender)                //Sam 20240605 : 新增 AutoArea IO Mode
{
    int ret=ShowMyMessageBox_YES_NO("確定啟動 Auot Area IO 模式??");
    if(ret==TMyMessageBox::msgrtnYES)
    {
        if(rbAutoAreaIOModeRow1->Checked)
        {
            iRowDet[0]=eTrack2;
            iRowAct[0]=eTrack1;
        }
        else
        {
            iRowDet[0]=eTrack1;
            iRowAct[0]=eTrack2;
        }

        if(rbAutoAreaIOModeRow3->Checked)
        {
            iRowDet[1]=eTrack4;
            iRowAct[1]=eTrack3;
        }
        else
        {
            iRowDet[1]=eTrack3;
            iRowAct[1]=eTrack4;
        }
        bAutoAreaMode=true;
        DoAutoAreaIO(0,true);
        DoAutoAreaIO(1,true);
        ShowMyMessage("啟動 Auot Area IO 模式中");
        RecordProcess("關閉  Auot Area IO 模式");
        bAutoAreaMode=false;
    }
}
//------------------------------------------------------------------------------
void Tfiosetview::InitialAutoAreaIOMode()                                       //Sam 20240605 : 新增 AutoArea IO Mode
{
    C_Auto_Zup[eTrack1][eTrackCol1]         =&HSys.Cyn.C_Auto1_Zup;
    C_Auto_Zup[eTrack1][eTrackCol2]         =&HSys.Cyn.C_Auto2_Zup;
    C_Auto_Zup[eTrack1][eTrackCol3]         =&HSys.Cyn.C_Auto3_Zup;
    C_Auto_Zup[eTrack1][eTrackCol4]         =&HSys.Cyn.C_Auto4_Zup;
    C_Auto_Zup[eTrack1][eTrackCol5]         =&HSys.Cyn.C_Auto5_Zup;
    C_Auto_Zup[eTrack2][eTrackCol1]         =&HSys.Cyn.C_Auto6_Zup;
    C_Auto_Zup[eTrack2][eTrackCol2]         =&HSys.Cyn.C_Auto7_Zup;
    C_Auto_Zup[eTrack2][eTrackCol3]         =&HSys.Cyn.C_Auto8_Zup;
    C_Auto_Zup[eTrack2][eTrackCol4]         =&HSys.Cyn.C_Auto9_Zup;
    C_Auto_Zup[eTrack2][eTrackCol5]         =&HSys.Cyn.C_Auto10_Zup;
    C_Auto_Zup[eTrack3][eTrackCol1]         =&HSys.Cyn.C_Auto11_Zup;
    C_Auto_Zup[eTrack3][eTrackCol2]         =&HSys.Cyn.C_Auto12_Zup;
    C_Auto_Zup[eTrack3][eTrackCol3]         =&HSys.Cyn.C_Auto13_Zup;
    C_Auto_Zup[eTrack3][eTrackCol4]         =&HSys.Cyn.C_Auto14_Zup;
    C_Auto_Zup[eTrack3][eTrackCol5]         =&HSys.Cyn.C_Auto15_Zup;
    C_Auto_Zup[eTrack4][eTrackCol1]         =&HSys.Cyn.C_Auto16_Zup;
    C_Auto_Zup[eTrack4][eTrackCol2]         =&HSys.Cyn.C_Auto17_Zup;
    C_Auto_Zup[eTrack4][eTrackCol3]         =&HSys.Cyn.C_Auto18_Zup;
    C_Auto_Zup[eTrack4][eTrackCol4]         =&HSys.Cyn.C_Auto19_Zup;
    C_Auto_Zup[eTrack4][eTrackCol5]         =&HSys.Cyn.C_Auto20_Zup;

    C_Auto_Stop[eTrack1][eTrackCol1]        =&HSys.Cyn.C_Auto1_Stop;
    C_Auto_Stop[eTrack1][eTrackCol2]        =&HSys.Cyn.C_Auto2_Stop;
    C_Auto_Stop[eTrack1][eTrackCol3]        =&HSys.Cyn.C_Auto3_Stop;
    C_Auto_Stop[eTrack1][eTrackCol4]        =&HSys.Cyn.C_Auto4_Stop;
    C_Auto_Stop[eTrack1][eTrackCol5]        =&HSys.Cyn.C_Auto5_Stop;
    C_Auto_Stop[eTrack2][eTrackCol1]        =&HSys.Cyn.C_Auto6_Stop;
    C_Auto_Stop[eTrack2][eTrackCol2]        =&HSys.Cyn.C_Auto7_Stop;
    C_Auto_Stop[eTrack2][eTrackCol3]        =&HSys.Cyn.C_Auto8_Stop;
    C_Auto_Stop[eTrack2][eTrackCol4]        =&HSys.Cyn.C_Auto9_Stop;
    C_Auto_Stop[eTrack2][eTrackCol5]        =&HSys.Cyn.C_Auto10_Stop;
    C_Auto_Stop[eTrack3][eTrackCol1]        =&HSys.Cyn.C_Auto11_Stop;
    C_Auto_Stop[eTrack3][eTrackCol2]        =&HSys.Cyn.C_Auto12_Stop;
    C_Auto_Stop[eTrack3][eTrackCol3]        =&HSys.Cyn.C_Auto13_Stop;
    C_Auto_Stop[eTrack3][eTrackCol4]        =&HSys.Cyn.C_Auto14_Stop;
    C_Auto_Stop[eTrack3][eTrackCol5]        =&HSys.Cyn.C_Auto15_Stop;
    C_Auto_Stop[eTrack4][eTrackCol1]        =&HSys.Cyn.C_Auto16_Stop;
    C_Auto_Stop[eTrack4][eTrackCol2]        =&HSys.Cyn.C_Auto17_Stop;
    C_Auto_Stop[eTrack4][eTrackCol3]        =&HSys.Cyn.C_Auto18_Stop;
    C_Auto_Stop[eTrack4][eTrackCol4]        =&HSys.Cyn.C_Auto19_Stop;
    C_Auto_Stop[eTrack4][eTrackCol5]        =&HSys.Cyn.C_Auto20_Stop;

    C_Auto_InclinePush[eTrack1][eTrackCol1] =&HSys.Cyn.C_Auto1_InclinePush;
    C_Auto_InclinePush[eTrack1][eTrackCol2] =&HSys.Cyn.C_Auto2_InclinePush;
    C_Auto_InclinePush[eTrack1][eTrackCol3] =&HSys.Cyn.C_Auto3_InclinePush;
    C_Auto_InclinePush[eTrack1][eTrackCol4] =&HSys.Cyn.C_Auto4_InclinePush;
    C_Auto_InclinePush[eTrack1][eTrackCol5] =&HSys.Cyn.C_Auto5_InclinePush;
    C_Auto_InclinePush[eTrack2][eTrackCol1] =&HSys.Cyn.C_Auto6_InclinePush;
    C_Auto_InclinePush[eTrack2][eTrackCol2] =&HSys.Cyn.C_Auto7_InclinePush;
    C_Auto_InclinePush[eTrack2][eTrackCol3] =&HSys.Cyn.C_Auto8_InclinePush;
    C_Auto_InclinePush[eTrack2][eTrackCol4] =&HSys.Cyn.C_Auto9_InclinePush;
    C_Auto_InclinePush[eTrack2][eTrackCol5] =&HSys.Cyn.C_Auto10_InclinePush;
    C_Auto_InclinePush[eTrack3][eTrackCol1] =&HSys.Cyn.C_Auto11_InclinePush;
    C_Auto_InclinePush[eTrack3][eTrackCol2] =&HSys.Cyn.C_Auto12_InclinePush;
    C_Auto_InclinePush[eTrack3][eTrackCol3] =&HSys.Cyn.C_Auto13_InclinePush;
    C_Auto_InclinePush[eTrack3][eTrackCol4] =&HSys.Cyn.C_Auto14_InclinePush;
    C_Auto_InclinePush[eTrack3][eTrackCol5] =&HSys.Cyn.C_Auto15_InclinePush;
    C_Auto_InclinePush[eTrack4][eTrackCol1] =&HSys.Cyn.C_Auto16_InclinePush;
    C_Auto_InclinePush[eTrack4][eTrackCol2] =&HSys.Cyn.C_Auto17_InclinePush;
    C_Auto_InclinePush[eTrack4][eTrackCol3] =&HSys.Cyn.C_Auto18_InclinePush;
    C_Auto_InclinePush[eTrack4][eTrackCol4] =&HSys.Cyn.C_Auto19_InclinePush;
    C_Auto_InclinePush[eTrack4][eTrackCol5] =&HSys.Cyn.C_Auto20_InclinePush;

    C_Auto_AntiDrop[eTrack1][eTrackCol1]    =&HSys.Cyn.C_Auto1_AntiDrop;
    C_Auto_AntiDrop[eTrack1][eTrackCol2]    =&HSys.Cyn.C_Auto2_AntiDrop;
    C_Auto_AntiDrop[eTrack1][eTrackCol3]    =&HSys.Cyn.C_Auto3_AntiDrop;
    C_Auto_AntiDrop[eTrack1][eTrackCol4]    =&HSys.Cyn.C_Auto4_AntiDrop;
    C_Auto_AntiDrop[eTrack1][eTrackCol5]    =&HSys.Cyn.C_Auto5_AntiDrop;
    C_Auto_AntiDrop[eTrack2][eTrackCol1]    =&HSys.Cyn.C_Auto6_AntiDrop;
    C_Auto_AntiDrop[eTrack2][eTrackCol2]    =&HSys.Cyn.C_Auto7_AntiDrop;
    C_Auto_AntiDrop[eTrack2][eTrackCol3]    =&HSys.Cyn.C_Auto8_AntiDrop;
    C_Auto_AntiDrop[eTrack2][eTrackCol4]    =&HSys.Cyn.C_Auto9_AntiDrop;
    C_Auto_AntiDrop[eTrack2][eTrackCol5]    =&HSys.Cyn.C_Auto10_AntiDrop;
    C_Auto_AntiDrop[eTrack3][eTrackCol1]    =&HSys.Cyn.C_Auto11_AntiDrop;
    C_Auto_AntiDrop[eTrack3][eTrackCol2]    =&HSys.Cyn.C_Auto12_AntiDrop;
    C_Auto_AntiDrop[eTrack3][eTrackCol3]    =&HSys.Cyn.C_Auto13_AntiDrop;
    C_Auto_AntiDrop[eTrack3][eTrackCol4]    =&HSys.Cyn.C_Auto14_AntiDrop;
    C_Auto_AntiDrop[eTrack3][eTrackCol5]    =&HSys.Cyn.C_Auto15_AntiDrop;
    C_Auto_AntiDrop[eTrack4][eTrackCol1]    =&HSys.Cyn.C_Auto16_AntiDrop;
    C_Auto_AntiDrop[eTrack4][eTrackCol2]    =&HSys.Cyn.C_Auto17_AntiDrop;
    C_Auto_AntiDrop[eTrack4][eTrackCol3]    =&HSys.Cyn.C_Auto18_AntiDrop;
    C_Auto_AntiDrop[eTrack4][eTrackCol4]    =&HSys.Cyn.C_Auto19_AntiDrop;
    C_Auto_AntiDrop[eTrack4][eTrackCol5]    =&HSys.Cyn.C_Auto20_AntiDrop;

    SnAutoCyAntiDrop1[eTrack1][eTrackCol1]  =&HSys.Sen.SnAuto1CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack1][eTrackCol2]  =&HSys.Sen.SnAuto2CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack1][eTrackCol3]  =&HSys.Sen.SnAuto3CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack1][eTrackCol4]  =&HSys.Sen.SnAuto4CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack1][eTrackCol5]  =&HSys.Sen.SnAuto5CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack2][eTrackCol1]  =&HSys.Sen.SnAuto6CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack2][eTrackCol2]  =&HSys.Sen.SnAuto7CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack2][eTrackCol3]  =&HSys.Sen.SnAuto8CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack2][eTrackCol4]  =&HSys.Sen.SnAuto9CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack2][eTrackCol5]  =&HSys.Sen.SnAuto10CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack3][eTrackCol1]  =&HSys.Sen.SnAuto11CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack3][eTrackCol2]  =&HSys.Sen.SnAuto12CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack3][eTrackCol3]  =&HSys.Sen.SnAuto13CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack3][eTrackCol4]  =&HSys.Sen.SnAuto14CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack3][eTrackCol5]  =&HSys.Sen.SnAuto15CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack4][eTrackCol1]  =&HSys.Sen.SnAuto16CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack4][eTrackCol2]  =&HSys.Sen.SnAuto17CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack4][eTrackCol3]  =&HSys.Sen.SnAuto18CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack4][eTrackCol4]  =&HSys.Sen.SnAuto19CyAntiDrop1;
    SnAutoCyAntiDrop1[eTrack4][eTrackCol5]  =&HSys.Sen.SnAuto20CyAntiDrop1;

    SnAutoCyAntiDrop2[eTrack1][eTrackCol1]  =&HSys.Sen.SnAuto1CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack1][eTrackCol2]  =&HSys.Sen.SnAuto2CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack1][eTrackCol3]  =&HSys.Sen.SnAuto3CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack1][eTrackCol4]  =&HSys.Sen.SnAuto4CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack1][eTrackCol5]  =&HSys.Sen.SnAuto5CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack2][eTrackCol1]  =&HSys.Sen.SnAuto6CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack2][eTrackCol2]  =&HSys.Sen.SnAuto7CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack2][eTrackCol3]  =&HSys.Sen.SnAuto8CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack2][eTrackCol4]  =&HSys.Sen.SnAuto9CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack2][eTrackCol5]  =&HSys.Sen.SnAuto10CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack3][eTrackCol1]  =&HSys.Sen.SnAuto11CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack3][eTrackCol2]  =&HSys.Sen.SnAuto12CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack3][eTrackCol3]  =&HSys.Sen.SnAuto13CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack3][eTrackCol4]  =&HSys.Sen.SnAuto14CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack3][eTrackCol5]  =&HSys.Sen.SnAuto15CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack4][eTrackCol1]  =&HSys.Sen.SnAuto16CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack4][eTrackCol2]  =&HSys.Sen.SnAuto17CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack4][eTrackCol3]  =&HSys.Sen.SnAuto18CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack4][eTrackCol4]  =&HSys.Sen.SnAuto19CyAntiDrop2;
    SnAutoCyAntiDrop2[eTrack4][eTrackCol5]  =&HSys.Sen.SnAuto20CyAntiDrop2;

    SnAutoCyAntiDrop3[eTrack1][eTrackCol1]  =&HSys.Sen.SnAuto1CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack1][eTrackCol2]  =&HSys.Sen.SnAuto2CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack1][eTrackCol3]  =&HSys.Sen.SnAuto3CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack1][eTrackCol4]  =&HSys.Sen.SnAuto4CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack1][eTrackCol5]  =&HSys.Sen.SnAuto5CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack2][eTrackCol1]  =&HSys.Sen.SnAuto6CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack2][eTrackCol2]  =&HSys.Sen.SnAuto7CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack2][eTrackCol3]  =&HSys.Sen.SnAuto8CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack2][eTrackCol4]  =&HSys.Sen.SnAuto9CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack2][eTrackCol5]  =&HSys.Sen.SnAuto10CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack3][eTrackCol1]  =&HSys.Sen.SnAuto11CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack3][eTrackCol2]  =&HSys.Sen.SnAuto12CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack3][eTrackCol3]  =&HSys.Sen.SnAuto13CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack3][eTrackCol4]  =&HSys.Sen.SnAuto14CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack3][eTrackCol5]  =&HSys.Sen.SnAuto15CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack4][eTrackCol1]  =&HSys.Sen.SnAuto16CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack4][eTrackCol2]  =&HSys.Sen.SnAuto17CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack4][eTrackCol3]  =&HSys.Sen.SnAuto18CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack4][eTrackCol4]  =&HSys.Sen.SnAuto19CyAntiDrop3;
    SnAutoCyAntiDrop3[eTrack4][eTrackCol5]  =&HSys.Sen.SnAuto20CyAntiDrop3;

    SnAutoHasTray[eTrack1][eTrackCol1]      =&HSys.Sen.SnAuto1HasTray;
    SnAutoHasTray[eTrack1][eTrackCol2]      =&HSys.Sen.SnAuto2HasTray;
    SnAutoHasTray[eTrack1][eTrackCol3]      =&HSys.Sen.SnAuto3HasTray;
    SnAutoHasTray[eTrack1][eTrackCol4]      =&HSys.Sen.SnAuto4HasTray;
    SnAutoHasTray[eTrack1][eTrackCol5]      =&HSys.Sen.SnAuto5HasTray;
    SnAutoHasTray[eTrack2][eTrackCol1]      =&HSys.Sen.SnAuto6HasTray;
    SnAutoHasTray[eTrack2][eTrackCol2]      =&HSys.Sen.SnAuto7HasTray;
    SnAutoHasTray[eTrack2][eTrackCol3]      =&HSys.Sen.SnAuto8HasTray;
    SnAutoHasTray[eTrack2][eTrackCol4]      =&HSys.Sen.SnAuto9HasTray;
    SnAutoHasTray[eTrack2][eTrackCol5]      =&HSys.Sen.SnAuto10HasTray;
    SnAutoHasTray[eTrack3][eTrackCol1]      =&HSys.Sen.SnAuto11HasTray;
    SnAutoHasTray[eTrack3][eTrackCol2]      =&HSys.Sen.SnAuto12HasTray;
    SnAutoHasTray[eTrack3][eTrackCol3]      =&HSys.Sen.SnAuto13HasTray;
    SnAutoHasTray[eTrack3][eTrackCol4]      =&HSys.Sen.SnAuto14HasTray;
    SnAutoHasTray[eTrack3][eTrackCol5]      =&HSys.Sen.SnAuto15HasTray;
    SnAutoHasTray[eTrack4][eTrackCol1]      =&HSys.Sen.SnAuto16HasTray;
    SnAutoHasTray[eTrack4][eTrackCol2]      =&HSys.Sen.SnAuto17HasTray;
    SnAutoHasTray[eTrack4][eTrackCol3]      =&HSys.Sen.SnAuto18HasTray;
    SnAutoHasTray[eTrack4][eTrackCol4]      =&HSys.Sen.SnAuto19HasTray;
    SnAutoHasTray[eTrack4][eTrackCol5]      =&HSys.Sen.SnAuto20HasTray;

    SwEmptyRowCW [eTrack1]                  =&HSys.Sw.SwACAutoRow1CW;
    SwEmptyRowCW [eTrack2]                  =&HSys.Sw.SwACAutoRow2CW;
    SwEmptyRowCW [eTrack3]                  =&HSys.Sw.SwACAutoRow3CW;
    SwEmptyRowCW [eTrack4]                  =&HSys.Sw.SwACAutoRow4CW;

    SwEmptyRowCCW[eTrack1]                  =&HSys.Sw.SwACAutoRow1CCW;
    SwEmptyRowCCW[eTrack2]                  =&HSys.Sw.SwACAutoRow2CCW;
    SwEmptyRowCCW[eTrack3]                  =&HSys.Sw.SwACAutoRow3CCW;
    SwEmptyRowCCW[eTrack4]                  =&HSys.Sw.SwACAutoRow4CCW;
}

//---------------------------------------------------------------------------
void Tfiosetview::DoAutoAreaIO(int iAuto, bool bReset)                          //Sam 20240605 : 新增 AutoArea IO Mode
{
    int &Task=iAutoAreaModeTask[iAuto];
    if(bReset)
    {
        Task=1;
        return;
    }
    int iTrackDet=iCurDetRow[iAuto];
    int iTrackAct=iCurActRow[iAuto];
    int iTrackIOCol=iCurCol[iAuto];
    switch(Task)
    {
        case 1:
            for(int iRow=eTrack1; iRow<=eTrack4; iRow++)
            {
                SwEmptyRowCW[iRow]->Off();
                SwEmptyRowCCW[iRow]->Off();
                for(int iCol=eTrackCol1; iCol<=eTrackCol5; iCol++)
                {
                    C_Auto_AntiDrop[iRow][iCol]->Off();
                }
            }
            Task=100;
            break;
        case 100:
            iCurDetRow[iAuto]=0;
            iCurActRow[iAuto]=0;
            iCurCol[iAuto]=0;
            for(int iCol=eTrackCol1; iCol<=eTrackCol5; iCol++)
            {
                if(SnAutoCyAntiDrop1[iRowDet[iAuto]][iCol]->IsOff())
                {
                    iCurDetRow[iAuto]=iRowDet[iAuto];
                    iCurActRow[iAuto]=iRowAct[iAuto];
                    iCurCol[iAuto]=iCol;
                    Task=200;
                    break;
                }
                else if(SnAutoCyAntiDrop2[iRowDet[iAuto]][iCol]->IsOff())
                {
                    iCurDetRow[iAuto]=iRowDet[iAuto];
                    iCurActRow[iAuto]=iRowAct[iAuto];
                    iCurCol[iAuto]=iCol;
                    Task=300;
                    break;
                }
                else if(SnAutoCyAntiDrop3[iRowDet[iAuto]][iCol]->IsOff())
                {
                    iCurDetRow[iAuto]=iRowDet[iAuto];
                    iCurActRow[iAuto]=iRowAct[iAuto];
                    iCurCol[iAuto]=iCol;
                    Task=400;
                    break;
                }
                else if(C_Auto_AntiDrop[iRowDet[iAuto]][iCol]->IsOn()==false)
                {
                    iCurDetRow[iAuto]=iRowDet[iAuto];
                    iCurActRow[iAuto]=iRowAct[iAuto];
                    iCurCol[iAuto]=iCol;
                    Task=500;
                    break;
                }
                else if(C_Auto_InclinePush[iRowDet[iAuto]][iCol]->IsOn()==false)
                {
                    iCurDetRow[iAuto]=iRowDet[iAuto];
                    iCurActRow[iAuto]=iRowAct[iAuto];
                    iCurCol[iAuto]=iCol;
                    Task=600;
                    break;
                }
            }
            break;
        case 200:
            if(SnAutoCyAntiDrop1[iTrackDet][iTrackIOCol]->IsOn())
            {
                if(C_Auto_InclinePush[iTrackAct][iTrackIOCol]->Switch.Status()==true)
                    C_Auto_InclinePush[iTrackAct][iTrackIOCol]->Off();
                else
                    C_Auto_InclinePush[iTrackAct][iTrackIOCol]->On();
                Task=100;
            }
            break;
        case 300:
            if(SnAutoCyAntiDrop2[iTrackDet][iTrackIOCol]->IsOn())
            {
                if(C_Auto_Stop[iTrackAct][iTrackIOCol]->Switch.Status()==true)
                    C_Auto_Stop[iTrackAct][iTrackIOCol]->Off();
                else
                    C_Auto_Stop[iTrackAct][iTrackIOCol]->On();
                Task=100;
            }
            break;
        case 400:
            if(SnAutoCyAntiDrop3[iTrackDet][iTrackIOCol]->IsOn())
            {
                if(C_Auto_Zup[iTrackAct][iTrackIOCol]->Switch.Status()==true)
                    C_Auto_Zup[iTrackAct][iTrackIOCol]->Off();
                else
                    C_Auto_Zup[iTrackAct][iTrackIOCol]->On();
                Task=100;
            }
            break;
        case 500:
            if(C_Auto_AntiDrop[iTrackDet][iTrackIOCol]->IsOn())
            {
                if(C_Auto_AntiDrop[iTrackAct][iTrackIOCol]->Switch.Status()==true)
                    C_Auto_AntiDrop[iTrackAct][iTrackIOCol]->Off();
                else
                    C_Auto_AntiDrop[iTrackAct][iTrackIOCol]->On();
                Task=100;
            }
            break;
        case 600:
            if(C_Auto_InclinePush[iTrackDet][iTrackIOCol]->IsOn())
            {
                if(SnAutoHasTray[iTrackAct][iTrackIOCol]->IsOff())
                    Task=700;   //AutoArea 沒有 Tray 跑上升組合動作
                else
                    Task=800;   //AutoArea   有 Tray 跑下降組合動作
            }
            break;

        //跑上升組合動作
        case 700:   //InclinePus、AntiDrop打開、Stop上升
            C_Auto_InclinePush[iTrackAct][iTrackIOCol]->On();
            C_Auto_AntiDrop[iTrackAct][iTrackIOCol]->On();
            C_Auto_Stop[iTrackAct][iTrackIOCol]->On();
            tAutoDelay[iAuto].SetSecAndOn(0.3);
            Task=710;
            break;
        case 710:   //傳送帶正轉
            if(tAutoDelay[iAuto].Off())
            {
                SwEmptyRowCW[iTrackAct]->On();
                tAutoDelay[iAuto].SetSecAndOn(2.0);
                Task=720;
            }
        case 720:   //Z汽缸上升
            if(tAutoDelay[iAuto].Off())
            {
                C_Auto_Zup[iTrackAct][iTrackIOCol]->On();
                tAutoDelay[iAuto].SetSecAndOn(3.0);
                Task=730;
            }
            break;
        case 730:   //AntiDrop 關閉、Stop下降、皮帶正轉停止
            if(tAutoDelay[iAuto].Off())
            {
                C_Auto_AntiDrop[iTrackAct][iTrackIOCol]->Off();
                C_Auto_Stop[iTrackAct][iTrackIOCol]->Off();
                SwEmptyRowCW[iTrackAct]->Off();
                tAutoDelay[iAuto].SetSecAndOn(0.3);
                Task=740;
            }
            break;
        case 740:   //Z汽缸下降
            if(tAutoDelay[iAuto].Off())
            {
                C_Auto_Zup[iTrackAct][iTrackIOCol]->Off();
                tAutoDelay[iAuto].SetSecAndOn(0.3);
                Task=750;
            }
            break;
        case 750:   //InclinePus 關閉
            if(tAutoDelay[iAuto].Off())
            {
                C_Auto_InclinePush[iTrackAct][iTrackIOCol]->Off();
                Task=100;
            }
            break;

        //跑下降組合動作
        case 800:   //Z汽缸上升、Stop 上升 InclinePush打開
            C_Auto_Zup[iTrackAct][iTrackIOCol]->On();
            C_Auto_Stop[iTrackAct][iTrackIOCol]->On();
            C_Auto_InclinePush[iTrackAct][iTrackIOCol]->On();
            tAutoDelay[iAuto].SetSecAndOn(3.0);
            Task=810;
            break;
        case 810:   //AntiDrop打開
            if(tAutoDelay[iAuto].Off())
            {
                C_Auto_AntiDrop[iTrackAct][iTrackIOCol]->On();
                tAutoDelay[iAuto].SetSecAndOn(0.3);
                Task=820;
            }
            break;
        case 820:   //Z汽缸下降
            if(tAutoDelay[iAuto].Off())
            {
                C_Auto_Zup[iTrackAct][iTrackIOCol]->Off();
                tAutoDelay[iAuto].SetSecAndOn(3.0);
                Task=830;
            }
        case 830:   //傳送帶正反轉、InclinePush關閉、AntiDrop關閉
            if(tAutoDelay[iAuto].Off())
            {
                SwEmptyRowCCW[iTrackAct]->On();
                C_Auto_InclinePush[iTrackAct][iTrackIOCol]->Off();
                C_Auto_AntiDrop[iTrackAct][iTrackIOCol]->Off();
                tAutoDelay[iAuto].SetSecAndOn(1.0);
                Task=840;
            }
            break;
        case 840:   //傳送帶反轉停止、Stop下降
            if(tAutoDelay[iAuto].Off())
            {
                SwEmptyRowCCW[iTrackAct]->Off();
                C_Auto_Stop[iTrackAct][iTrackIOCol]->Off();
                Task=100;
            }
    }
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnLoopAutoClick(TObject *Sender)   //Sam 20240626 : Loop Auto UpDown
{
    int ret=ShowMyMessageBox_YES_NO("確定啟動 Loop Auto Up Down ??");
    if(ret==TMyMessageBox::msgrtnYES)
    {
        AutoTrackModule[0]->bNeedTest[0]=cbAuto01->Checked;
        AutoTrackModule[0]->bNeedTest[1]=cbAuto02->Checked;
        AutoTrackModule[0]->bNeedTest[2]=cbAuto03->Checked;
        AutoTrackModule[0]->bNeedTest[3]=cbAuto04->Checked;
        AutoTrackModule[0]->bNeedTest[4]=cbAuto05->Checked;

        AutoTrackModule[1]->bNeedTest[0]=cbAuto06->Checked;
        AutoTrackModule[1]->bNeedTest[1]=cbAuto07->Checked;
        AutoTrackModule[1]->bNeedTest[2]=cbAuto08->Checked;
        AutoTrackModule[1]->bNeedTest[3]=cbAuto09->Checked;
        AutoTrackModule[1]->bNeedTest[4]=cbAuto10->Checked;

        AutoTrackModule[2]->bNeedTest[0]=cbAuto11->Checked;
        AutoTrackModule[2]->bNeedTest[1]=cbAuto12->Checked;
        AutoTrackModule[2]->bNeedTest[2]=cbAuto13->Checked;
        AutoTrackModule[2]->bNeedTest[3]=cbAuto14->Checked;
        AutoTrackModule[2]->bNeedTest[4]=cbAuto15->Checked;

        AutoTrackModule[3]->bNeedTest[0]=cbAuto16->Checked;
        AutoTrackModule[3]->bNeedTest[1]=cbAuto17->Checked;
        AutoTrackModule[3]->bNeedTest[2]=cbAuto18->Checked;
        AutoTrackModule[3]->bNeedTest[3]=cbAuto19->Checked;
        AutoTrackModule[3]->bNeedTest[4]=cbAuto20->Checked;
        for(int i=0; i<4; i++)
        {
            AutoTrackModule[i]->DoLoopAutoUpDown(true);
        }
        bLoopAutoUpDownMode=true;
        ShowMyMessage("啟動 Loop Auto Up Down 中");
        RecordProcess("關閉 Loop Auto Up Down模式");
        bLoopAutoUpDownMode=false;
    }
}
//---------------------------------------------------------------------------
TAutoTrackModule *AutoTrackModule1=new TAutoTrackModule(0);
TAutoTrackModule *AutoTrackModule2=new TAutoTrackModule(1);
TAutoTrackModule *AutoTrackModule3=new TAutoTrackModule(2);
TAutoTrackModule *AutoTrackModule4=new TAutoTrackModule(3);
TAutoTrackModule *AutoTrackModule[4]={AutoTrackModule1, AutoTrackModule2, AutoTrackModule3, AutoTrackModule4};
void InitialAutoIO()    //Sam 20240626 : Loop Auto UpDown
{
    //AutoTrack 1 --------------------------------------------------------------------
    AutoTrackModule1->C_Auto_Zup        [eTrackCol1]=&HSys.Cyn.C_Auto1_Zup;
    AutoTrackModule1->C_Auto_Stop       [eTrackCol1]=&HSys.Cyn.C_Auto1_Stop;
    AutoTrackModule1->C_Auto_InclinePush[eTrackCol1]=&HSys.Cyn.C_Auto1_InclinePush;
    AutoTrackModule1->C_Auto_AntiDrop   [eTrackCol1]=&HSys.Cyn.C_Auto1_AntiDrop;
    AutoTrackModule1->SnAutoCyAntiDrop1 [eTrackCol1]=&HSys.Sen.SnAuto1CyAntiDrop1;
    AutoTrackModule1->SnAutoCyAntiDrop2 [eTrackCol1]=&HSys.Sen.SnAuto1CyAntiDrop2;
    AutoTrackModule1->SnAutoCyAntiDrop3 [eTrackCol1]=&HSys.Sen.SnAuto1CyAntiDrop3;
    AutoTrackModule1->SnAutoHasTray     [eTrackCol1]=&HSys.Sen.SnAuto1HasTray;
    AutoTrackModule1->SenAutoCarHasTray [eTrackCol1]=&HSys.Sen.SnAuto1CarHasTray;

    AutoTrackModule1->C_Auto_Zup        [eTrackCol2]=&HSys.Cyn.C_Auto2_Zup;
    AutoTrackModule1->C_Auto_Stop       [eTrackCol2]=&HSys.Cyn.C_Auto2_Stop;
    AutoTrackModule1->C_Auto_InclinePush[eTrackCol2]=&HSys.Cyn.C_Auto2_InclinePush;
    AutoTrackModule1->C_Auto_AntiDrop   [eTrackCol2]=&HSys.Cyn.C_Auto2_AntiDrop;
    AutoTrackModule1->SnAutoCyAntiDrop1 [eTrackCol2]=&HSys.Sen.SnAuto2CyAntiDrop1;
    AutoTrackModule1->SnAutoCyAntiDrop2 [eTrackCol2]=&HSys.Sen.SnAuto2CyAntiDrop2;
    AutoTrackModule1->SnAutoCyAntiDrop3 [eTrackCol2]=&HSys.Sen.SnAuto2CyAntiDrop3;
    AutoTrackModule1->SnAutoHasTray     [eTrackCol2]=&HSys.Sen.SnAuto2HasTray;
    AutoTrackModule1->SenAutoCarHasTray [eTrackCol2]=&HSys.Sen.SnAuto2CarHasTray;

    AutoTrackModule1->C_Auto_Zup        [eTrackCol3]=&HSys.Cyn.C_Auto3_Zup;
    AutoTrackModule1->C_Auto_Stop       [eTrackCol3]=&HSys.Cyn.C_Auto3_Stop;
    AutoTrackModule1->C_Auto_InclinePush[eTrackCol3]=&HSys.Cyn.C_Auto3_InclinePush;
    AutoTrackModule1->C_Auto_AntiDrop   [eTrackCol3]=&HSys.Cyn.C_Auto3_AntiDrop;
    AutoTrackModule1->SnAutoCyAntiDrop1 [eTrackCol3]=&HSys.Sen.SnAuto3CyAntiDrop1;
    AutoTrackModule1->SnAutoCyAntiDrop2 [eTrackCol3]=&HSys.Sen.SnAuto3CyAntiDrop2;
    AutoTrackModule1->SnAutoCyAntiDrop3 [eTrackCol3]=&HSys.Sen.SnAuto3CyAntiDrop3;
    AutoTrackModule1->SnAutoHasTray     [eTrackCol3]=&HSys.Sen.SnAuto3HasTray;
    AutoTrackModule1->SenAutoCarHasTray [eTrackCol3]=&HSys.Sen.SnAuto3CarHasTray;

    AutoTrackModule1->C_Auto_Zup        [eTrackCol4]=&HSys.Cyn.C_Auto4_Zup;
    AutoTrackModule1->C_Auto_Stop       [eTrackCol4]=&HSys.Cyn.C_Auto4_Stop;
    AutoTrackModule1->C_Auto_InclinePush[eTrackCol4]=&HSys.Cyn.C_Auto4_InclinePush;
    AutoTrackModule1->C_Auto_AntiDrop   [eTrackCol4]=&HSys.Cyn.C_Auto4_AntiDrop;
    AutoTrackModule1->SnAutoCyAntiDrop1 [eTrackCol4]=&HSys.Sen.SnAuto4CyAntiDrop1;
    AutoTrackModule1->SnAutoCyAntiDrop2 [eTrackCol4]=&HSys.Sen.SnAuto4CyAntiDrop2;
    AutoTrackModule1->SnAutoCyAntiDrop3 [eTrackCol4]=&HSys.Sen.SnAuto4CyAntiDrop3;
    AutoTrackModule1->SnAutoHasTray     [eTrackCol4]=&HSys.Sen.SnAuto4HasTray;
    AutoTrackModule1->SenAutoCarHasTray [eTrackCol4]=&HSys.Sen.SnAuto4CarHasTray;

    AutoTrackModule1->C_Auto_Zup        [eTrackCol5]=&HSys.Cyn.C_Auto5_Zup;
    AutoTrackModule1->C_Auto_Stop       [eTrackCol5]=&HSys.Cyn.C_Auto5_Stop;
    AutoTrackModule1->C_Auto_InclinePush[eTrackCol5]=&HSys.Cyn.C_Auto5_InclinePush;
    AutoTrackModule1->C_Auto_AntiDrop   [eTrackCol5]=&HSys.Cyn.C_Auto5_AntiDrop;
    AutoTrackModule1->SnAutoCyAntiDrop1 [eTrackCol5]=&HSys.Sen.SnAuto5CyAntiDrop1;
    AutoTrackModule1->SnAutoCyAntiDrop2 [eTrackCol5]=&HSys.Sen.SnAuto5CyAntiDrop2;
    AutoTrackModule1->SnAutoCyAntiDrop3 [eTrackCol5]=&HSys.Sen.SnAuto5CyAntiDrop3;
    AutoTrackModule1->SnAutoHasTray     [eTrackCol5]=&HSys.Sen.SnAuto5HasTray;
    AutoTrackModule1->SenAutoCarHasTray [eTrackCol5]=&HSys.Sen.SnAuto5CarHasTray;

    AutoTrackModule1->SwEmptyRowCW                  =&HSys.Sw.SwACAutoRow1CW;
    AutoTrackModule1->SwEmptyRowCCW                 =&HSys.Sw.SwACAutoRow1CCW;

    //AutoTrack 2 --------------------------------------------------------------------
    AutoTrackModule2->C_Auto_Zup        [eTrackCol1]=&HSys.Cyn.C_Auto6_Zup;
    AutoTrackModule2->C_Auto_Stop       [eTrackCol1]=&HSys.Cyn.C_Auto6_Stop;
    AutoTrackModule2->C_Auto_InclinePush[eTrackCol1]=&HSys.Cyn.C_Auto6_InclinePush;
    AutoTrackModule2->C_Auto_AntiDrop   [eTrackCol1]=&HSys.Cyn.C_Auto6_AntiDrop;
    AutoTrackModule2->SnAutoCyAntiDrop1 [eTrackCol1]=&HSys.Sen.SnAuto6CyAntiDrop1;
    AutoTrackModule2->SnAutoCyAntiDrop2 [eTrackCol1]=&HSys.Sen.SnAuto6CyAntiDrop2;
    AutoTrackModule2->SnAutoCyAntiDrop3 [eTrackCol1]=&HSys.Sen.SnAuto6CyAntiDrop3;
    AutoTrackModule2->SnAutoHasTray     [eTrackCol1]=&HSys.Sen.SnAuto6HasTray;
    AutoTrackModule2->SenAutoCarHasTray [eTrackCol1]=&HSys.Sen.SnAuto6CarHasTray;

    AutoTrackModule2->C_Auto_Zup        [eTrackCol2]=&HSys.Cyn.C_Auto7_Zup;
    AutoTrackModule2->C_Auto_Stop       [eTrackCol2]=&HSys.Cyn.C_Auto7_Stop;
    AutoTrackModule2->C_Auto_InclinePush[eTrackCol2]=&HSys.Cyn.C_Auto7_InclinePush;
    AutoTrackModule2->C_Auto_AntiDrop   [eTrackCol2]=&HSys.Cyn.C_Auto7_AntiDrop;
    AutoTrackModule2->SnAutoCyAntiDrop1 [eTrackCol2]=&HSys.Sen.SnAuto7CyAntiDrop1;
    AutoTrackModule2->SnAutoCyAntiDrop2 [eTrackCol2]=&HSys.Sen.SnAuto7CyAntiDrop2;
    AutoTrackModule2->SnAutoCyAntiDrop3 [eTrackCol2]=&HSys.Sen.SnAuto7CyAntiDrop3;
    AutoTrackModule2->SnAutoHasTray     [eTrackCol2]=&HSys.Sen.SnAuto7HasTray;
    AutoTrackModule2->SenAutoCarHasTray [eTrackCol2]=&HSys.Sen.SnAuto7CarHasTray;

    AutoTrackModule2->C_Auto_Zup        [eTrackCol3]=&HSys.Cyn.C_Auto8_Zup;
    AutoTrackModule2->C_Auto_Stop       [eTrackCol3]=&HSys.Cyn.C_Auto8_Stop;
    AutoTrackModule2->C_Auto_InclinePush[eTrackCol3]=&HSys.Cyn.C_Auto8_InclinePush;
    AutoTrackModule2->C_Auto_AntiDrop   [eTrackCol3]=&HSys.Cyn.C_Auto8_AntiDrop;
    AutoTrackModule2->SnAutoCyAntiDrop1 [eTrackCol3]=&HSys.Sen.SnAuto8CyAntiDrop1;
    AutoTrackModule2->SnAutoCyAntiDrop2 [eTrackCol3]=&HSys.Sen.SnAuto8CyAntiDrop2;
    AutoTrackModule2->SnAutoCyAntiDrop3 [eTrackCol3]=&HSys.Sen.SnAuto8CyAntiDrop3;
    AutoTrackModule2->SnAutoHasTray     [eTrackCol3]=&HSys.Sen.SnAuto8HasTray;
    AutoTrackModule2->SenAutoCarHasTray [eTrackCol3]=&HSys.Sen.SnAuto8CarHasTray;

    AutoTrackModule2->C_Auto_Zup        [eTrackCol4]=&HSys.Cyn.C_Auto9_Zup;
    AutoTrackModule2->C_Auto_Stop       [eTrackCol4]=&HSys.Cyn.C_Auto9_Stop;
    AutoTrackModule2->C_Auto_InclinePush[eTrackCol4]=&HSys.Cyn.C_Auto9_InclinePush;
    AutoTrackModule2->C_Auto_AntiDrop   [eTrackCol4]=&HSys.Cyn.C_Auto9_AntiDrop;
    AutoTrackModule2->SnAutoCyAntiDrop1 [eTrackCol4]=&HSys.Sen.SnAuto9CyAntiDrop1;
    AutoTrackModule2->SnAutoCyAntiDrop2 [eTrackCol4]=&HSys.Sen.SnAuto9CyAntiDrop2;
    AutoTrackModule2->SnAutoCyAntiDrop3 [eTrackCol4]=&HSys.Sen.SnAuto9CyAntiDrop3;
    AutoTrackModule2->SnAutoHasTray     [eTrackCol4]=&HSys.Sen.SnAuto9HasTray;
    AutoTrackModule2->SenAutoCarHasTray [eTrackCol4]=&HSys.Sen.SnAuto9CarHasTray;

    AutoTrackModule2->C_Auto_Zup        [eTrackCol5]=&HSys.Cyn.C_Auto10_Zup;
    AutoTrackModule2->C_Auto_Stop       [eTrackCol5]=&HSys.Cyn.C_Auto10_Stop;
    AutoTrackModule2->C_Auto_InclinePush[eTrackCol5]=&HSys.Cyn.C_Auto10_InclinePush;
    AutoTrackModule2->C_Auto_AntiDrop   [eTrackCol5]=&HSys.Cyn.C_Auto10_AntiDrop;
    AutoTrackModule2->SnAutoCyAntiDrop1 [eTrackCol5]=&HSys.Sen.SnAuto10CyAntiDrop1;
    AutoTrackModule2->SnAutoCyAntiDrop2 [eTrackCol5]=&HSys.Sen.SnAuto10CyAntiDrop2;
    AutoTrackModule2->SnAutoCyAntiDrop3 [eTrackCol5]=&HSys.Sen.SnAuto10CyAntiDrop3;
    AutoTrackModule2->SnAutoHasTray     [eTrackCol5]=&HSys.Sen.SnAuto10HasTray;
    AutoTrackModule2->SenAutoCarHasTray [eTrackCol5]=&HSys.Sen.SnAuto10CarHasTray;

    AutoTrackModule2->SwEmptyRowCW                  =&HSys.Sw.SwACAutoRow2CW;
    AutoTrackModule2->SwEmptyRowCCW                 =&HSys.Sw.SwACAutoRow2CCW;

    //AutoTrack 3 --------------------------------------------------------------------
    AutoTrackModule3->C_Auto_Zup        [eTrackCol1]=&HSys.Cyn.C_Auto11_Zup;
    AutoTrackModule3->C_Auto_Stop       [eTrackCol1]=&HSys.Cyn.C_Auto11_Stop;
    AutoTrackModule3->C_Auto_InclinePush[eTrackCol1]=&HSys.Cyn.C_Auto11_InclinePush;
    AutoTrackModule3->C_Auto_AntiDrop   [eTrackCol1]=&HSys.Cyn.C_Auto11_AntiDrop;
    AutoTrackModule3->SnAutoCyAntiDrop1 [eTrackCol1]=&HSys.Sen.SnAuto11CyAntiDrop1;
    AutoTrackModule3->SnAutoCyAntiDrop2 [eTrackCol1]=&HSys.Sen.SnAuto11CyAntiDrop2;
    AutoTrackModule3->SnAutoCyAntiDrop3 [eTrackCol1]=&HSys.Sen.SnAuto11CyAntiDrop3;
    AutoTrackModule3->SnAutoHasTray     [eTrackCol1]=&HSys.Sen.SnAuto11HasTray;
    AutoTrackModule3->SenAutoCarHasTray [eTrackCol1]=&HSys.Sen.SnAuto11CarHasTray;

    AutoTrackModule3->C_Auto_Zup        [eTrackCol2]=&HSys.Cyn.C_Auto12_Zup;
    AutoTrackModule3->C_Auto_Stop       [eTrackCol2]=&HSys.Cyn.C_Auto12_Stop;
    AutoTrackModule3->C_Auto_InclinePush[eTrackCol2]=&HSys.Cyn.C_Auto12_InclinePush;
    AutoTrackModule3->C_Auto_AntiDrop   [eTrackCol2]=&HSys.Cyn.C_Auto12_AntiDrop;
    AutoTrackModule3->SnAutoCyAntiDrop1 [eTrackCol2]=&HSys.Sen.SnAuto12CyAntiDrop1;
    AutoTrackModule3->SnAutoCyAntiDrop2 [eTrackCol2]=&HSys.Sen.SnAuto12CyAntiDrop2;
    AutoTrackModule3->SnAutoCyAntiDrop3 [eTrackCol2]=&HSys.Sen.SnAuto12CyAntiDrop3;
    AutoTrackModule3->SnAutoHasTray     [eTrackCol2]=&HSys.Sen.SnAuto12HasTray;
    AutoTrackModule3->SenAutoCarHasTray [eTrackCol2]=&HSys.Sen.SnAuto12CarHasTray;

    AutoTrackModule3->C_Auto_Zup        [eTrackCol3]=&HSys.Cyn.C_Auto13_Zup;
    AutoTrackModule3->C_Auto_Stop       [eTrackCol3]=&HSys.Cyn.C_Auto13_Stop;
    AutoTrackModule3->C_Auto_InclinePush[eTrackCol3]=&HSys.Cyn.C_Auto13_InclinePush;
    AutoTrackModule3->C_Auto_AntiDrop   [eTrackCol3]=&HSys.Cyn.C_Auto13_AntiDrop;
    AutoTrackModule3->SnAutoCyAntiDrop1 [eTrackCol3]=&HSys.Sen.SnAuto13CyAntiDrop1;
    AutoTrackModule3->SnAutoCyAntiDrop2 [eTrackCol3]=&HSys.Sen.SnAuto13CyAntiDrop2;
    AutoTrackModule3->SnAutoCyAntiDrop3 [eTrackCol3]=&HSys.Sen.SnAuto13CyAntiDrop3;
    AutoTrackModule3->SnAutoHasTray     [eTrackCol3]=&HSys.Sen.SnAuto13HasTray;
    AutoTrackModule3->SenAutoCarHasTray [eTrackCol3]=&HSys.Sen.SnAuto13CarHasTray;

    AutoTrackModule3->C_Auto_Zup        [eTrackCol4]=&HSys.Cyn.C_Auto14_Zup;
    AutoTrackModule3->C_Auto_Stop       [eTrackCol4]=&HSys.Cyn.C_Auto14_Stop;
    AutoTrackModule3->C_Auto_InclinePush[eTrackCol4]=&HSys.Cyn.C_Auto14_InclinePush;
    AutoTrackModule3->C_Auto_AntiDrop   [eTrackCol4]=&HSys.Cyn.C_Auto14_AntiDrop;
    AutoTrackModule3->SnAutoCyAntiDrop1 [eTrackCol4]=&HSys.Sen.SnAuto14CyAntiDrop1;
    AutoTrackModule3->SnAutoCyAntiDrop2 [eTrackCol4]=&HSys.Sen.SnAuto14CyAntiDrop2;
    AutoTrackModule3->SnAutoCyAntiDrop3 [eTrackCol4]=&HSys.Sen.SnAuto14CyAntiDrop3;
    AutoTrackModule3->SnAutoHasTray     [eTrackCol4]=&HSys.Sen.SnAuto14HasTray;
    AutoTrackModule3->SenAutoCarHasTray [eTrackCol4]=&HSys.Sen.SnAuto14CarHasTray;

    AutoTrackModule3->C_Auto_Zup        [eTrackCol5]=&HSys.Cyn.C_Auto15_Zup;
    AutoTrackModule3->C_Auto_Stop       [eTrackCol5]=&HSys.Cyn.C_Auto15_Stop;
    AutoTrackModule3->C_Auto_InclinePush[eTrackCol5]=&HSys.Cyn.C_Auto15_InclinePush;
    AutoTrackModule3->C_Auto_AntiDrop   [eTrackCol5]=&HSys.Cyn.C_Auto15_AntiDrop;
    AutoTrackModule3->SnAutoCyAntiDrop1 [eTrackCol5]=&HSys.Sen.SnAuto15CyAntiDrop1;
    AutoTrackModule3->SnAutoCyAntiDrop2 [eTrackCol5]=&HSys.Sen.SnAuto15CyAntiDrop2;
    AutoTrackModule3->SnAutoCyAntiDrop3 [eTrackCol5]=&HSys.Sen.SnAuto15CyAntiDrop3;
    AutoTrackModule3->SnAutoHasTray     [eTrackCol5]=&HSys.Sen.SnAuto15HasTray;
    AutoTrackModule3->SenAutoCarHasTray [eTrackCol5]=&HSys.Sen.SnAuto15CarHasTray;

    AutoTrackModule3->SwEmptyRowCW                  =&HSys.Sw.SwACAutoRow3CW;
    AutoTrackModule3->SwEmptyRowCCW                 =&HSys.Sw.SwACAutoRow3CCW;

    //AutoTrack 4 --------------------------------------------------------------------
    AutoTrackModule4->C_Auto_Zup        [eTrackCol1]=&HSys.Cyn.C_Auto16_Zup;
    AutoTrackModule4->C_Auto_Stop       [eTrackCol1]=&HSys.Cyn.C_Auto16_Stop;
    AutoTrackModule4->C_Auto_InclinePush[eTrackCol1]=&HSys.Cyn.C_Auto16_InclinePush;
    AutoTrackModule4->C_Auto_AntiDrop   [eTrackCol1]=&HSys.Cyn.C_Auto16_AntiDrop;
    AutoTrackModule4->SnAutoCyAntiDrop1 [eTrackCol1]=&HSys.Sen.SnAuto16CyAntiDrop1;
    AutoTrackModule4->SnAutoCyAntiDrop2 [eTrackCol1]=&HSys.Sen.SnAuto16CyAntiDrop2;
    AutoTrackModule4->SnAutoCyAntiDrop3 [eTrackCol1]=&HSys.Sen.SnAuto16CyAntiDrop3;
    AutoTrackModule4->SnAutoHasTray     [eTrackCol1]=&HSys.Sen.SnAuto16HasTray;
    AutoTrackModule4->SenAutoCarHasTray [eTrackCol1]=&HSys.Sen.SnAuto16CarHasTray;

    AutoTrackModule4->C_Auto_Zup        [eTrackCol2]=&HSys.Cyn.C_Auto17_Zup;
    AutoTrackModule4->C_Auto_Stop       [eTrackCol2]=&HSys.Cyn.C_Auto17_Stop;
    AutoTrackModule4->C_Auto_InclinePush[eTrackCol2]=&HSys.Cyn.C_Auto17_InclinePush;
    AutoTrackModule4->C_Auto_AntiDrop   [eTrackCol2]=&HSys.Cyn.C_Auto17_AntiDrop;
    AutoTrackModule4->SnAutoCyAntiDrop1 [eTrackCol2]=&HSys.Sen.SnAuto17CyAntiDrop1;
    AutoTrackModule4->SnAutoCyAntiDrop2 [eTrackCol2]=&HSys.Sen.SnAuto17CyAntiDrop2;
    AutoTrackModule4->SnAutoCyAntiDrop3 [eTrackCol2]=&HSys.Sen.SnAuto17CyAntiDrop3;
    AutoTrackModule4->SnAutoHasTray     [eTrackCol2]=&HSys.Sen.SnAuto17HasTray;
    AutoTrackModule4->SenAutoCarHasTray [eTrackCol2]=&HSys.Sen.SnAuto17CarHasTray;

    AutoTrackModule4->C_Auto_Zup        [eTrackCol3]=&HSys.Cyn.C_Auto18_Zup;
    AutoTrackModule4->C_Auto_Stop       [eTrackCol3]=&HSys.Cyn.C_Auto18_Stop;
    AutoTrackModule4->C_Auto_InclinePush[eTrackCol3]=&HSys.Cyn.C_Auto18_InclinePush;
    AutoTrackModule4->C_Auto_AntiDrop   [eTrackCol3]=&HSys.Cyn.C_Auto18_AntiDrop;
    AutoTrackModule4->SnAutoCyAntiDrop1 [eTrackCol3]=&HSys.Sen.SnAuto18CyAntiDrop1;
    AutoTrackModule4->SnAutoCyAntiDrop2 [eTrackCol3]=&HSys.Sen.SnAuto18CyAntiDrop2;
    AutoTrackModule4->SnAutoCyAntiDrop3 [eTrackCol3]=&HSys.Sen.SnAuto18CyAntiDrop3;
    AutoTrackModule4->SnAutoHasTray     [eTrackCol3]=&HSys.Sen.SnAuto18HasTray;
    AutoTrackModule4->SenAutoCarHasTray [eTrackCol3]=&HSys.Sen.SnAuto18CarHasTray;

    AutoTrackModule4->C_Auto_Zup        [eTrackCol4]=&HSys.Cyn.C_Auto19_Zup;
    AutoTrackModule4->C_Auto_Stop       [eTrackCol4]=&HSys.Cyn.C_Auto19_Stop;
    AutoTrackModule4->C_Auto_InclinePush[eTrackCol4]=&HSys.Cyn.C_Auto19_InclinePush;
    AutoTrackModule4->C_Auto_AntiDrop   [eTrackCol4]=&HSys.Cyn.C_Auto19_AntiDrop;
    AutoTrackModule4->SnAutoCyAntiDrop1 [eTrackCol4]=&HSys.Sen.SnAuto19CyAntiDrop1;
    AutoTrackModule4->SnAutoCyAntiDrop2 [eTrackCol4]=&HSys.Sen.SnAuto19CyAntiDrop2;
    AutoTrackModule4->SnAutoCyAntiDrop3 [eTrackCol4]=&HSys.Sen.SnAuto19CyAntiDrop3;
    AutoTrackModule4->SnAutoHasTray     [eTrackCol4]=&HSys.Sen.SnAuto19HasTray;
    AutoTrackModule4->SenAutoCarHasTray [eTrackCol4]=&HSys.Sen.SnAuto19CarHasTray;

    AutoTrackModule4->C_Auto_Zup        [eTrackCol5]=&HSys.Cyn.C_Auto20_Zup;
    AutoTrackModule4->C_Auto_Stop       [eTrackCol5]=&HSys.Cyn.C_Auto20_Stop;
    AutoTrackModule4->C_Auto_InclinePush[eTrackCol5]=&HSys.Cyn.C_Auto20_InclinePush;
    AutoTrackModule4->C_Auto_AntiDrop   [eTrackCol5]=&HSys.Cyn.C_Auto20_AntiDrop;
    AutoTrackModule4->SnAutoCyAntiDrop1 [eTrackCol5]=&HSys.Sen.SnAuto20CyAntiDrop1;
    AutoTrackModule4->SnAutoCyAntiDrop2 [eTrackCol5]=&HSys.Sen.SnAuto20CyAntiDrop2;
    AutoTrackModule4->SnAutoCyAntiDrop3 [eTrackCol5]=&HSys.Sen.SnAuto20CyAntiDrop3;
    AutoTrackModule4->SnAutoHasTray     [eTrackCol5]=&HSys.Sen.SnAuto20HasTray;
    AutoTrackModule4->SenAutoCarHasTray [eTrackCol5]=&HSys.Sen.SnAuto20CarHasTray;

    AutoTrackModule4->SwEmptyRowCW                  =&HSys.Sw.SwACAutoRow4CW;
    AutoTrackModule4->SwEmptyRowCCW                 =&HSys.Sw.SwACAutoRow4CCW;
}
//------------------------------------------------------------------------------
bool TAutoTrackModule::bTrackNeedTest()  //Sam 20240626 : Loop Auto UpDown
{
    for(int i=0; i<eTrackColTotal; i++)
    {
        if(bNeedTest[i]==true)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
__fastcall TAutoTrackModule::TAutoTrackModule(int iTag)
{
    Tag         =iTag;
   // InitialFlag();
}
//------------------------------------------------------------------------------
bool TAutoTrackModule::DoLoopAutoUpDown(bool bReset)    //Sam 20240626 : Loop Auto UpDown
{
    AnsiString str="";
    int &Task=iAutoUpDownTask;
    if(bReset)
    {
        Task=1;
        iTrack=0;
        ZeroMemory(bCylinderOK, sizeof(bCylinderOK));
        return false;
    }
    switch(Task)
    {
        case 1:
            for(int i=0; i<eTrackColTotal; i++)
            {
                C_Auto_Zup[i]->Reset();
                C_Auto_Stop[i]->Reset();
                C_Auto_InclinePush[i]->Reset();
                C_Auto_AntiDrop[i]->Reset();
            }
            SwEmptyRowCW->Off();
            SwEmptyRowCCW->Off();
            bCylinderOK[0]=false;
            if(bNeedTest[iTrack])
                Task=100;
            else
                Task=1300;
            break;
        case 100:
            if(bCylinderOK[0]==false)
               bCylinderOK[0]=C_Auto_Zup[iTrack]->Pop();
            C_Auto_Stop[iTrack]->Off();
            C_Auto_InclinePush[iTrack]->Off();
            C_Auto_AntiDrop[iTrack]->Off();

            if(bCylinderOK[0])
            {
                bCylinderOK[0]=false;
                bCylinderOK[1]=false;
                Task=200;
            }
            break;
        case 200:
            if(bCylinderOK[0]==false)
               bCylinderOK[0]=C_Auto_Stop[iTrack]->Push();
            if(bCylinderOK[1]==false)
                bCylinderOK[1]=C_Auto_Zup[iTrack]->Pop();
            C_Auto_InclinePush[iTrack]->On();
            C_Auto_AntiDrop[iTrack]->On();
            if(bCylinderOK[0] && bCylinderOK[1])
            {
                bCylinderOK[0]=false;
                bCylinderOK[1]=false;
                SwEmptyRowCW->On();
                tHasTrayTimeOut.SetSecAndOn(10.0);
                Task=300;
            }
            break;
        case 300:
            SwEmptyRowCW->On();
            if(SenAutoCarHasTray[iTrack]->IsOff()==false)
            {
                tMoveToStopDelay.SetSecAndOn(0.5);
                Task=400;
            }
            else if(tHasTrayTimeOut.Off())
            {
                str.sprintf("Track%d AutoCarHasTray%d sensor not on time out", Tag+1, Tag*5+(iTrack+1));
                ShowMyMessage(str);
                tHasTrayTimeOut.SetSecAndOn(10.0);
            }
            break;
        case 400:
            if(tMoveToStopDelay.Off())
            {
                C_Auto_Zup[iTrack]->Reset();
                Task=500;
            }
            break;
        case 500:
            if(bCylinderOK[0]==false)
                bCylinderOK[0]=C_Auto_Zup[iTrack]->Push();
            if(bCylinderOK[0])
            {
                bCylinderOK[0]=false;
                SwEmptyRowCW->Off();
                C_Auto_Stop[iTrack]->Off();
                C_Auto_AntiDrop[iTrack]->Off();
                tAntiDropDelay.SetSecAndOn(0.5);
                Task=600;
            }
            break;
        case 600:
            if(tAntiDropDelay.Off())
            {
                C_Auto_Zup[iTrack]->Reset();
                Task=700;
            }
            break;
        case 700:
            if(bCylinderOK[0]==false)
                bCylinderOK[0]=C_Auto_Zup[iTrack]->Pop();
            if(bCylinderOK[0])
            {
                bCylinderOK[0]=false;
                C_Auto_InclinePush[iTrack]->Off();
                tHasTrayTimeOut.SetSecAndOn(5.0);
                tInclinePushDelay.SetSecAndOn(0.5);
                Task=800;
            }
            break;
        case 800:
            if(tInclinePushDelay.Off())
            {
                if(SnAutoHasTray[iTrack]->IsOff()==false)
                {
                    C_Auto_Zup[iTrack]->Reset();
                    C_Auto_Stop[iTrack]->Reset();
                    bCylinderOK[0]=false;
                    bCylinderOK[1]=false;
                    C_Auto_InclinePush[iTrack]->On();

                    Task=900;
                }
                else if(tHasTrayTimeOut.Off())
                {
                    str.sprintf("Track%d SnAutoHasTray%d sensor not on time out", Tag+1, Tag*5+(iTrack+1));
                    ShowMyMessage(str);
                    tHasTrayTimeOut.SetSecAndOn(5.0);
                }
            }
            break;
        case 900:
            if(bCylinderOK[0]==false)
                bCylinderOK[0]=C_Auto_Zup[iTrack]->Push();
            if(bCylinderOK[0])
            {
                bCylinderOK[0]=false;
                bCylinderOK[1]=false;
                C_Auto_AntiDrop[iTrack]->On();
                tAntiDropDelay.SetSecAndOn(0.5);
                Task=1000;
            }
            break;
        case 1000:
            if(tAntiDropDelay.Off())
            {
                C_Auto_Zup[iTrack]->Reset();
                Task=1100;
            }
            break;
        case 1100:
            if(bCylinderOK[0]==false)
                bCylinderOK[0]=C_Auto_Zup[iTrack]->Pop();
            if(bCylinderOK[0])
            {
                tHasTrayTimeOut.SetSecAndOn(5.0);
                Task=1200;
            }
            break;
        case 1200:
            if(SenAutoCarHasTray[iTrack]->IsOff()==false)
            {
                C_Auto_Stop[iTrack]->Off();
                C_Auto_AntiDrop[iTrack]->Off();
                C_Auto_InclinePush[iTrack]->Off();
                tCycleFinishDelay.SetSecAndOn(1);
                Task=1300;
            }
            else if(tHasTrayTimeOut.Off())
            {
                str.sprintf("Track%d AutoCarHasTray%d sensor not on time out", Tag+1, Tag*5+(iTrack+1));
                ShowMyMessage(str);
                tHasTrayTimeOut.SetSecAndOn(5.0);
            }
            break;
        case 1300:
            if(tCycleFinishDelay.Off())
            {
                iTrack++;
                if(iTrack>4)
                {
                    Task=2000;
                }
                else
                {
                    Task=1;
                }
            }
            break;
        case 2000:
            tMoveCol5ToCol1Delay.SetSecAndOn(5.0);
            SwEmptyRowCCW->On();
            Task=2100;
            break;
        case 2100:
            SwEmptyRowCCW->On();
            if(tMoveCol5ToCol1Delay.Off())
            {
                SwEmptyRowCCW->Off();
                iTrack=0;
                Task=1;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall Tfiosetview::btnNoRestoreClick(TObject *Sender)
{
    bNoRestoreIO=true;
}
//---------------------------------------------------------------------------

