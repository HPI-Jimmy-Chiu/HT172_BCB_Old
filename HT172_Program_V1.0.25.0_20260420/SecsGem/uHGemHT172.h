//---------------------------------------------------------------------------

#ifndef uHGemHT172H
#define uHGemHT172H
//---------------------------------------------------------------------------

struct ETypeStruct
{
    enum
    {
        HandlerStatus       = 1,
        RecipeChange        = 2,
        ClearCount          = 3,
        PressStartWithoutIC = 4,
        PressStartWithIC    = 5,
        PressPause          = 6,
        PressHome           = 7,
        PressOneCycle       = 8,
        PressCleanOut       = 9,
        PressTrayFeed       =10,
        PressLotStart       =11,
        PressLotEnd         =12,
        PressExit           =13,
        PressRetry          =14,
        PressSkip           =15,
        PressAlarmReset     =16,
        PressTrayEnd        =17,
        PressReset          =18,
        ShowAlarm           =19,
        ReleaseAlarm        =20,
        ShowMessage         =21,
        ReleaseMessage      =22,
        RealDummy           =23,
        ChangeUser          =24,
        EnterSetup          =25,
        EnterToolPage       =26,
        EnterMaintenPage    =27,
        EnterOffset         =28,
        EnterSpeed          =29,
        EnterIOPage         =30,
        EnterMessage        =31,
        EnterTeach          =32,
        EnterMotorTest      =33,
        EnterSECSPage       =34,
        OneCycleOK          =35,
        CleanOutOK          =36,
        TrayFeedOK          =37,
        TimeEvent           =38,
        Store01Full         =39,
        Store02Full         =40,
        Store03Full         =41,
        Store04Full         =42,
        Store05Full         =43,
        Store06Full         =44,
        Store07Full         =45,
        Store08Full         =46,
        Store09Full         =47,
        Store10Full         =48,
        Store11Full         =49,
        Store12Full         =50,
        Store13Full         =51,
        Store14Full         =52,
        Store15Full         =53,
        Store16Full         =54,
        Store17Full         =55,
        Store18Full         =56,
        Store19Full         =57,
        Store20Full         =58,
        Mag1FullIC          =59,
        Mag2FullIC          =60,
        Mag3FullIC          =61,
        Empty1FullTray      =62,
        Empty2FullTray      =63,
        Empty3FullTray      =64,
        LoaderCar_ReadTrayID=65,
        TotalEvent     //一定要在最下面
    };
};
extern struct ETypeStruct SECS_EVENT;

class HT172Gem:public HTGem
{
    private:
    protected:
        AnsiString EventDescription[SECS_EVENT.TotalEvent];
    public:
        HT172Gem(AnsiString Path, THGem *HGemTmp);
//        virtual ~HT1501Gem()                    ;
        virtual void AddSV()                    ;
        virtual void AddEC()                    ;
        virtual void AddAlarmList()             ;
        virtual void AddCEID()                  ;
        virtual void AddReprot()                ;
        virtual int  S2F15_CheckNewEquipmentConstant(); //wei 20170417 (Steven) add S2F15
        virtual int  S2F15_UpdateNewEquipmentConstant();
        virtual int  S2F42_Host_Command_Acknowledge();
        virtual void S5F6_ListAlarmData();
        virtual int  S7F2_ProcessProgramLoadGrant();
        virtual void S7F4_ProcessProgramAcknowledge();
        virtual void S7F6_ProcessProgramData();
        virtual void S7F6_ProcessProgramData(AnsiString FileName);
        virtual void ProcessS14F1_GetAttrRequest(AnsiString asTrayID);
        virtual unsigned char ProcessS14F2_GetAttrData();
//        virtual void S14F2_GetAttrData();
//        virtual void S14F4_Get2DID_BinCode();

        virtual void ReloadParameter();
        virtual void LookForFile()              ;
};

//==============================================================================
// GEM function Start
//==============================================================================
extern char GEM_MachineName[16];


#endif
