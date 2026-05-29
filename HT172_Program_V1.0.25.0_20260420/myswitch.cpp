//---------------------------------------------------------------------------

//==============================================================================
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "myswitch.h"
#include "uPadInterface.h"
#pragma package(smart_init)
//==============================================================================
class TMySwitch SWBackup[8];

//==============================================================================
_fastcall TMySwitch::TMySwitch()
{
    Name="";
    Port=0;
    Bit=0;
    Type=0;
    Enable=false;
    OutValue=false;
    SetValue=false;
    Output=NULL;
}
//==============================================================================
void TMySwitch::On()                                                            // OUTPUT ON //
{
    OutValue=true;
    if(fPadInterface!=NULL &&
       iControlPanelMode==1 && fPadInterface->IsPadButton(Name))    //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 通訊面板不卡Database
    {
        fPadInterface->SendSwitchStatus(Name, true);
    }
    else
    {
        if(Enable==false)
            return;
        Output->On();
    }
}
//==============================================================================
void TMySwitch::Off()                                                           // OUTPUT OFF //
{
    OutValue=false;
    if(fPadInterface!=NULL &&
       iControlPanelMode==1 && fPadInterface->IsPadButton(Name))    //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 通訊面板不卡Database
    {
        fPadInterface->SendSwitchStatus(Name,false);
    }
    else
    {

        if(Enable==false)
            return;

        Output->Off();
    }
}
//==============================================================================
bool TMySwitch::Status()                                                        // OUTPUT STATUS //
{
    return OutValue;
}
//==============================================================================
void TMySwitch::OnOff(bool Type)                                                // ON OR OFF //
{
    if(Type)
        On();
    else
        Off();
}
//==============================================================================
void CopySwitch( TMySwitch *Source ,TMySwitch *Target)
{
    Target->Port=Source->Port;
    Target->Bit=Source->Bit;
    Target->Type=Source->Type;
}
//==============================================================================
