//---------------------------------------------------------------------------

#ifndef CosFunctionH
#define CosFunctionH
//---------------------------------------------------------------------------
typedef struct
{
    bool bUseBinAutoConvert;                                                    //Jimmychiu 20250524 : Use Bin Auto Convert
    bool bUseMagzineArea;                                                       //Jimmychiu 20250619 : Use Mag Area
    bool bClearEmpty1TrayAtTrayFeed;                                            //Jimmychiu 20250622 : bClearEmpty1TrayAtTrayFeed
    bool bRemoveICofSuckerWhenHome;                                             //Jimmychiu 20251224 : PTI要求回Home不取下吸嘴IC
    bool bSaveJamCodeLog;                                                       //Stevenhong 20251231 : 力成要把JAM獨立額外生成
    bool bAutoStoreFullAlarmByCount;                                            //Jimmychiu 20260112 : Auto Store Full Alarm By Count
    bool bUseSecsGem;                                                           //AI(HT172-Maintainer) 20260326 : Enable SECS/GEM (WINSTEK only)
    bool bEmpty1TrayFeedToCarStack;                                             //AI(HT172-Maintainer) 20260413 : PTI TrayFeed: evacuate Empty1 trays to Car stacker
    bool bPrioritizeNearFullBin;                                                //AI(HT172-Maintainer) 20260413 : PTI priority: sort near-full bins first
    bool bMagCrossMagTopReadyFirst;                                             //AI(HT172-Maintainer) 20260415 : Magazine cross-mag: prefer Top-ready magazine
} HT172_COUSTOMER_FUNCTION;
extern HT172_COUSTOMER_FUNCTION CosFunction;
extern void InitialCosFunction();
#endif
 