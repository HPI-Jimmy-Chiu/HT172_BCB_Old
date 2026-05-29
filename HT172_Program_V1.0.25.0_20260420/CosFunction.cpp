//---------------------------------------------------------------------------


#pragma hdrstop
#include "IncludeAllHeader.h"
#include "CosFunction.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//------------------------------------------------------------------------------
//  客戶功能選擇區
//------------------------------------------------------------------------------
void FUNC_CC_HONPREC_QC()
{
    CosFunction.bUseBinAutoConvert                                              =false;  //Jimmychiu 20250524:Use Bin Auto Convert
}
//------------------------------------------------------------------------------
void FUNC_CC_WINSTEK()
{
    CosFunction.bUseBinAutoConvert                                              =true;   //Jimmychiu 20250524:Use Bin Auto Convert
    CosFunction.bUseMagzineArea                                                 =true;   //Jimmychiu 20250619:Use Mag Area
    CosFunction.bClearEmpty1TrayAtTrayFeed                                      =true;   //Jimmychiu 20250622:bClearEmpty1TrayAtTrayFeed
    CosFunction.bAutoStoreFullAlarmByCount                                      =true;   //Jimmychiu 20260112 : Auto Store Full Alarm By Count
    CosFunction.bUseSecsGem                                                     =true;   //AI(HT172-Maintainer) 20260326 : Enable SECS/GEM for WINSTEK
}
//------------------------------------------------------------------------------
void FUNC_CC_PTI()
{
    CosFunction.bUseBinAutoConvert                                              =true;  //Jimmychiu 20250524:Use Bin Auto Convert
    CosFunction.bSaveJamCodeLog                                                 =true;  //Stevenhong 20251231 : 力成要把JAM獨立額外生成
    CosFunction.bRemoveICofSuckerWhenHome                                       =false; //Jimmychiu 20251224 : PTI要求回Home不取下吸嘴IC
    CosFunction.bEmpty1TrayFeedToCarStack                                       =true;  //AI(HT172-Maintainer) 20260413 : PTI TrayFeed Empty1 to Car stacker
    CosFunction.bClearEmpty1TrayAtTrayFeed                                      =false; //AI(HT172-Maintainer) 20260413 : Enable Empty1 evacuation for PTI
    CosFunction.bPrioritizeNearFullBin                                          =true;  //AI(HT172-Maintainer) 20260413 : PTI sort near-full bins first
    CosFunction.bMagCrossMagTopReadyFirst                                       =true;  //AI(HT172-Maintainer) 20260415 : Magazine cross-mag Top-ready first
    extern int gLastPickedMag;
    gLastPickedMag = -1; //AI(PTI-MagArm) 20260521 : reset PTI MagArm round-robin state
}
//------------------------------------------------------------------------------
void DoCustomerFunction()
{
    switch(CUSTOMER_CODE)
    {
        case CC_HONPREC_QC:                 FUNC_CC_HONPREC_QC();               break;
        case CC_WINSTEK:                    FUNC_CC_WINSTEK();                  break;
        case CC_PTI:                        FUNC_CC_PTI();                      break;
    }
}
//------------------------------------------------------------------------------
// funtion說明區
//------------------------------------------------------------------------------
void InitialCosFunction()
{
    CosFunction.bUseBinAutoConvert                                              =false;  //Jimmychiu 20250524:Use Bin Auto Convert
    CosFunction.bUseMagzineArea                                                 =true;   //Jimmychiu 20250619:Use Mag Area
    CosFunction.bClearEmpty1TrayAtTrayFeed                                      =false;  //Jimmychiu 20250622:bClearEmpty1TrayAtTrayFeed
    CosFunction.bRemoveICofSuckerWhenHome                                       =true;   //Jimmychiu 20251224 : PTI要求回Home不取下吸嘴IC
    CosFunction.bSaveJamCodeLog                                                 =false;  //Stevenhong 20251231 : 力成要把JAM獨立額外生成
    CosFunction.bAutoStoreFullAlarmByCount                                      =false;  //Jimmychiu 20260112 : Auto Store Full Alarm By Count
    CosFunction.bUseSecsGem                                                     =false;  //AI(HT172-Maintainer) 20260326 : SECS/GEM disabled by default
    CosFunction.bEmpty1TrayFeedToCarStack                                       =false;  //AI(HT172-Maintainer) 20260413 : PTI TrayFeed Empty1 to Car stacker
    CosFunction.bPrioritizeNearFullBin                                          =false;  //AI(HT172-Maintainer) 20260413 : PTI sort near-full bins first
    CosFunction.bMagCrossMagTopReadyFirst                                       =false;  //AI(HT172-Maintainer) 20260415 : Magazine cross-mag Top-ready first
    //----------------------
    DoCustomerFunction();                                                                //Steven 20240927 : 整合並保持在最下面
}
//------------------------------------------------------------------------------
