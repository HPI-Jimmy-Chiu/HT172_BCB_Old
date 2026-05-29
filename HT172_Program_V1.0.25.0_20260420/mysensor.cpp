//---------------------------------------------------------------------------

//==============================================================================
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop
#include "mysensor.h"
#include "uPadInterface.h"
#pragma package(smart_init)
//==============================================================================
_fastcall TMySensor::TMySensor()
{
    Name="";
    OnAlarmCode="";
    OffAlarmCode="";
    AlarmType="";
    Port=0;
    Bit=0;
    Type=0;
    Enable=0;
    bErroHappen=false;
    iStatus=false;
    Input=NULL;
}
//==============================================================================
bool TMySensor::Status()                                                        // SENSOR STATUS //
{
    return IsOn();
}
//==============================================================================
bool TMySensor::IsOn()                                                          // CHECK ON //
{
    bool bOn=false;
    if(fPadInterface!=NULL &&
       iControlPanelMode==1 && fPadInterface->IsPadKey(Name))                   //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 通訊面板不卡Database
    {
        return fPadInterface->ProcessScanKey(Name);
    }
    else
    {
        if(Enable==false)
        {
            iStatus=-1;
            return bOn;
        }
        else
        {
            if(Type==1)
            {
                bOn=Input->IsOn();
            }
            else
            {
                bOn=!Input->IsOn();
            }
        }

        if(bOn)
        {
            iStatus=1;
        }
        else
        {
            iStatus=0;
        }

        return bOn;
    }
}
//==============================================================================
bool TMySensor::IsOff()                                                         // CHECK OFF //
{
    bool bOff=false;
    bool ret;
    if(fPadInterface!=NULL &&
       iControlPanelMode==1 && fPadInterface->IsPadKey(Name))                   //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 通訊面板不卡Database
    {
        ret=fPadInterface->ProcessScanKey(Name);
        return !ret;
    }
    else
    {
        if(Enable==false)
        {
            iStatus=-1;
            return bOff;
        }
        else
        {
            if(Type==1)
                bOff=Input->IsOff();
            else
                bOff=!Input->IsOff();
        }

        if(bOff)
        {
            iStatus=0;
        }
        else
        {
            iStatus=1;
        }

        return bOff;
    }
}
//==============================================================================
void CopySensor( TMySensor *Source ,TMySensor *Target)
{
//    Target->Port=Source->Port;
//    Target->Bit=Source->Bit;
//    Target->Type=Source->Type;
}
//==============================================================================
