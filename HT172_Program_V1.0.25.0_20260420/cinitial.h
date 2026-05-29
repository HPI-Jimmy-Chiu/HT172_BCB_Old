//---------------------------------------------------------------------------
#ifndef cinitialH
#define cinitialH

#include "mymotor.h"
#include "MyKitSuck.h"
//---------------------------------------------------------------------------

void Initial();
void SetMotorSpeed(bool bSet=false);
void SetMotorScaleSpeed(TTrayMotor *MotPtr,int ScaleSpeed);
bool SetWorkParameter();
void SetTechDataToProd();
void SetSimuScreenPara();
#endif
