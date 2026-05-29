//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "mycylin.h"
#pragma package(smart_init)
class TMyCylinder Cylinder[MaxCylinderItem];
//==============================================================================
void SetAlarm(int AlarmCode)                                                    // set alarm //
{
    Alarm->Set(AlarmCode);
}
//==============================================================================
void ClearAlarm(int AlarmCode)                                                  // clear alarm //
{
    Alarm->Clear(AlarmCode);
}
//==============================================================================
_fastcall TMyCylinder::TMyCylinder()
{
    Enable=false;
//    bCylinderOn=false;
    bInitialOk=false;
    Task=1;
    OnOff=2;
    OnAlarmCode=0;
    OffAlarmCode=0;
    OnDelayTime=0;
    OffDelayTime=0;
    CylinderName="";
    OnSensorName="";
    OffSensorName="";
    FlushPanelName="";
    for(int j=0; j<eCynErrTotal; j++)
        ErrorName[j]="";
    PTempWinCtrl=NULL;
}
//==============================================================================
bool TMyCylinder::Reset()                                                       // reset //
{
    Task=1;
    return true;
}
//==============================================================================
bool TMyCylinder::GetOutBit()                                                   // get out bit //
{
    return Switch.OutValue;
}
//==============================================================================
bool TMyCylinder::IsOn()                                                        // check on sensor //
{
    //return OnSensor.Status();
    return OnSensor.IsOn();
}
//==============================================================================
bool TMyCylinder::IsOff()                                                       // check off sensor //
{
    //return OffSensor.Status();
    return OffSensor.IsOn();
}
//==============================================================================
void TMyCylinder::On()      // no delay,no alarm                                // only on no check //
{
    Switch.On();
    Task=1;
//    bCylinderOn=true;
    UpdateSimulateCompomentPosition(true);
}
//==============================================================================
void TMyCylinder::Off()     // no delay,no alarm                                // only off no check //
{
    Task=1;
    Switch.Off();
//    bCylinderOn=false;
    UpdateSimulateCompomentPosition(false);
}
//==============================================================================
bool TMyCylinder::Push()                                                        // push //
{
    /*
    if( Enable==false )                                                         V1.6
    {
        bCylinderOn=true;                                                       // ben edit 20110726 //
        UpdateSimulateCompomentPosition();                                      // ben edit 20110726 //
        return true;
    }
    */
    if( Enable==false && Task==1)                                               //V1.6
        Task=100;

    Switch.On();
    ClearAlarm(OnAlarmCode);
    if(Task==1 || Task==2)
    {
        if( OnSensor.Enable==true ) //  has install onsensor
        {
            if( OnSensor.IsOn() )     // already on
            {
                Delay.Clear();
                Delay.SetMS(OnDelayTime);
                Delay.On();
                Task=100;
            }
            else
            {
                Delay.Clear();
                Delay.SetMS(OnAlarmTime);
                Delay.On();
                Task=50;
            }
        }
        else
        {
            Delay.Clear();
            Delay.SetMS(OnDelayTime);
            Delay.On();
            Task=100;      //need delay
        }
    }
    if(Task==50)
    {
        if( OnSensor.IsOn() )
        {
            Delay.Clear();
            Delay.SetMS(OnDelayTime);
            Delay.On();
            Task=100;      // do on delay
        }
        else
        {
            //if( OnAlarmTime || Delay.Off() )
            if( Delay.Off() )
            {
                Task=1;
//                bCylinderOn=true;                                               // ben edit 20110726 //
                UpdateSimulateCompomentPosition(true);
                SetAlarm(OnAlarmCode);
                return false;
            }
            else
            {
                return false;
            }
        }
    }

    if(Task>=100)
    {
        switch(Task)
        {
            case 100:
                if( OnDelayTime==0 )
                {
                    Task=1;
                    ClearAlarm(OnAlarmCode);
//                    bCylinderOn=true;                                           // ben edit 20110726 //
                    UpdateSimulateCompomentPosition(true);
                    return true;
                }
                else
                {
                    Delay.Clear();
                    Delay.SetMS(OnDelayTime);
                    Delay.On();
                    Task=101;
                }
            case 101:
                if( Delay.Off() )
                {
                    ClearAlarm(OnAlarmCode);
//                    bCylinderOn=true;                                           // ben edit 20110726 //
                    UpdateSimulateCompomentPosition(true);
                    Task=1;
                    return true;
                }
                else
                {
                    return false;
                }
        }
    }

    Task=1;
    ClearAlarm(OnAlarmCode);
//    bCylinderOn=true;                                                       // ben edit 20110726 //
    UpdateSimulateCompomentPosition(true);
    return true;
}
//==============================================================================
bool TMyCylinder::Pop()                                                         // pop //
{
    /*                                                                          V1.6
    if( Enable==false )
    {
        bCylinderOn=false;                                                      // ben edit 20110726 //
        UpdateSimulateCompomentPosition();                                      // ben edit 20110726 //
        return true;
    }
    */
    if(Enable==false && Task==1)                                               //V1.6
        Task=100;

    Switch.Off();
    ClearAlarm(OffAlarmCode);
    if(Task==1 || Task==2)
    {
        if( OffSensor.Enable==true ) //  has install onsensor
        {
            if( OffSensor.IsOn() )     // already on
            {
                Delay.Clear();
                Delay.SetMS(OffDelayTime);
                Delay.On();
                Task=100;
            }
            else
            {
                Delay.Clear();
                Delay.SetMS(OffAlarmTime);
                Delay.On();
                Task=50;
            }
        }
        else
        {
            Delay.Clear();
            Delay.SetMS(OffDelayTime);
            Delay.On();
            Task=100;      //need delay
        }
    }
    if(Task==50)
    {
        if( OffSensor.IsOn() )
        {
            Delay.Clear();
            Delay.SetMS(OffDelayTime);
            Delay.On();
            Task=100;      // do on delay
        }
        else
        {
            if( Delay.Off() )
            {
                Task=1;
                SetAlarm(OffAlarmCode);
//                bCylinderOn=false;                                              // ben edit 20110726 //
                UpdateSimulateCompomentPosition(false);
                return false;
            }
            else
            {
                return false;
            }
        }
    }

    if(Task>=100)
    {
        switch(Task)
        {
            case 100:
                if( OffDelayTime==0 )
                {
                    ClearAlarm(OffAlarmCode);
//                    bCylinderOn=false;                                          // ben edit 20110726 //
                    UpdateSimulateCompomentPosition(false);
                    Task=1;
                    return true;
                }
                else
                {
                    Delay.Clear();
                    Delay.SetMS(OffDelayTime);
                    Delay.On();
                    Task=101;
                }
            case 101:
                if( Delay.Off() )
                {
                    Task=1;
                    ClearAlarm(OffAlarmCode);
//                    bCylinderOn=false;                                          // ben edit 20110726 //
                    UpdateSimulateCompomentPosition(false);
                    return true;
                }
                else
                {
                    return false;
                }
        }
    }

    Task=1;
    ClearAlarm(OffAlarmCode);
//    bCylinderOn=false;                                                      // ben edit 20110726 //
    UpdateSimulateCompomentPosition(false);

    return true;
}
//==============================================================================
void TMyCylinder::UpdateSimulateCompomentPosition(bool bCylinderOn)             // Simulate run //
{
    if( PTempWinCtrl!=NULL)
    {
        if( bCylinderOn )                                                       // ben edit 20110726 - s //
        {
            PTempWinCtrl->Left=iOnLeft;
            PTempWinCtrl->Top=iOnTop;
        }
        else
        {
            PTempWinCtrl->Left=iOffLeft;
            PTempWinCtrl->Top=iOffTop;
        }                                                                       // ben edit 20110726 - e //
    }
}
//==============================================================================// Simulate set //
//void TMyCylinder::SetSimulateCompoment(TObject *PCtrl,int Left,int Top,int Right,int Bottom)
//{
//    PTempWinCtrl=dynamic_cast<TControl *> (PCtrl);    // new code ,old is TWinctontrol,
//    if( PTempWinCtrl!=NULL)
//    {
//        iOnLeft=Left;
//        iOnTop=Top;
//        iOffLeft=Right;
//        iOffTop=Bottom;
//        UpdateSimulateCompomentPosition();
//    }
//}
//==============================================================================
void TMyCylinder::SetSimulateCompoment(TObject *PCtrl, TAnchorKind Alignment, int simuStart, int simuEnd)
{
    PTempWinCtrl=dynamic_cast<TControl *> (PCtrl);    // new code ,old is TWinctontrol,
    if(PTempWinCtrl!=NULL)
    {
        iOnLeft =(Alignment==akLeft || Alignment==akRight )?PTempWinCtrl->Left:simuStart;
        iOffLeft=(Alignment==akLeft || Alignment==akRight )?PTempWinCtrl->Left:simuEnd;
        iOnTop  =(Alignment==akTop  || Alignment==akBottom)?PTempWinCtrl->Top:simuStart;
        iOffTop =(Alignment==akTop  || Alignment==akBottom)?PTempWinCtrl->Top:simuEnd;
        UpdateSimulateCompomentPosition(false);
    }
}
