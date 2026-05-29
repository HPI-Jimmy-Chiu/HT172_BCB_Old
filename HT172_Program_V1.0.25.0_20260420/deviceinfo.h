//---------------------------------------------------------------------------

#ifndef deviceinfoH
#define deviceinfoH 
//---------------------------------------------------------------------------
#include "cpublic.h"                                                         
//---------------------------------------------------------------------------
class TDeviceInfo
{
    private:
        //------------------------------------------------------------------------------
        enum eMyProdRec
        {
            eStartTime      =0,
            eLoadX          ,
            eLoadY          ,
            eLoadTime       ,
            eLoadTrayID     ,
            eWhichArm       ,
            eSuckX          ,
            eSuckY          ,
            eWhichAuto      ,
            eBin            ,
            eOutTrayID      ,            //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
            eUnloadX        ,
            eUnloadY        ,
            eUnloadTime     ,
            eErrorCode      ,
            eDataTotal                      //擺在最下面
        };
        //------------------------------------------------------------------------------
    public:
        TDeviceInfo();

        int  iBin;
        int  iAuto;
        AnsiString CodeReader;
        AnsiString ClipID;
        AnsiString InputClipID;
        AnsiString OutputClipID;
        AnsiString ErrorCode;

        AnsiString Arm;
        AnsiString SiteNO;
        AnsiString Indext_Time;
        AnsiString Test_Time;
        AnsiString OrgStr;
        AnsiString Picker;
        AnsiString DeviceName;
        AnsiString LotID;
        AnsiString SoftBin;
        AnsiString TimeFlag;
        AnsiString TEST_PROGRAM;
        AnsiString TESTER_ID;
        AnsiString HANDLER_ID;
        AnsiString LOADBOARD_ID;
        AnsiString RUN_CARD_ID;
        AnsiString STAGE;
        AnsiString SUB_STAGE;
        AnsiString ATTRIBUTE;
        AnsiString START_TIME;
        AnsiString END_TIME;
        AnsiString Device2DID;                                                  //Jimmychiu 20240106 : Add Device2DID in device info
        int  iInputR;
        int  iInputC;
        int  iOutputR;
        int  iOutputC;
        int  iSuckArm;
        int  iSuckR;
        int  iSuckC;

        AnsiString sStartTime;
        //Loader
        AnsiString sLoadTime;
        //Unloader
        AnsiString sWhichAuto;
        AnsiString sUnloaderTime;
        AnsiString sErrorCode;

        void Clear();
        void MoveFrom(TDeviceInfo &Source);
        void CopyFrom(TDeviceInfo &Source);
        void AddSourceDataByJHTFormat(AnsiString Str);
        void AddSourceDataByWinstekFormat(AnsiString Str);
        void AddSourceDataByHonPrecCSVFormat(int iTrayR, int iTrayC, int iBin);
        void AddInputInfo(AnsiString _ClipID, int iTrayR, int iTrayC, int _iSuckArm, int _iSuckR, int _iSuckC);
        void AddOutputInfo(int _iWhichAuto,AnsiString _ClipID, int iTrayR, int iTrayC);
        void SetErrorCode(AnsiString sErr);
        AnsiString GetDataFilePath();
        void SaveRecord();
        AnsiString GetTitleList();
        AnsiString GetDatasList();
};
#endif


