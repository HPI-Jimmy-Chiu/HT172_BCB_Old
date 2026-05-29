#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "uHGemHT172.h"

#include "DecodeXML.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)

struct ETypeStruct SECS_EVENT;
//------------------------------------------------------------------------------
HT172Gem::HT172Gem(AnsiString Path, THGem *HGemTmp)                             // Tmp
{
    HGemPtr=HGemTmp;
    HandlerPath=Path;

    EventDescription[SECS_EVENT.HandlerStatus       ]="1 Handler change status";
    EventDescription[SECS_EVENT.RecipeChange        ]="2 Recipe Change";
    EventDescription[SECS_EVENT.ClearCount          ]="3 Press Clear Count button";
    EventDescription[SECS_EVENT.PressStartWithoutIC ]="4 Press Start button without IC inside handler";
    EventDescription[SECS_EVENT.PressStartWithIC    ]="5 Press Start button with IC inside handler";
    EventDescription[SECS_EVENT.PressPause          ]="6 Press Pause button";
    EventDescription[SECS_EVENT.PressHome           ]="7 Press Home button";
    EventDescription[SECS_EVENT.PressOneCycle       ]="8 Press One Cycle button";
    EventDescription[SECS_EVENT.PressCleanOut       ]="9 Press Clean Out button";
    EventDescription[SECS_EVENT.PressTrayFeed       ]="10 Press Tray Feed button";
    EventDescription[SECS_EVENT.PressLotStart       ]="11 Press Lot Start button";
    EventDescription[SECS_EVENT.PressLotEnd         ]="12 Press Lot End button";
    EventDescription[SECS_EVENT.PressExit           ]="13 Press Exit button";
    EventDescription[SECS_EVENT.PressRetry          ]="14 Press Retry button";
    EventDescription[SECS_EVENT.PressSkip           ]="15 Press Skip button";
    EventDescription[SECS_EVENT.PressAlarmReset     ]="16 Press Alarm Reset button";
    EventDescription[SECS_EVENT.PressTrayEnd        ]="17 Press Tray End button";
    EventDescription[SECS_EVENT.PressReset          ]="18 Press Reset button";
    EventDescription[SECS_EVENT.ShowAlarm           ]="19 Show Alarm";
    EventDescription[SECS_EVENT.ReleaseAlarm        ]="20 Release Alarm";
    EventDescription[SECS_EVENT.ShowMessage         ]="21 Show Message";
    EventDescription[SECS_EVENT.ReleaseMessage      ]="22 Release Message";
    EventDescription[SECS_EVENT.RealDummy           ]="23 Switching Real/Dummy Mode";
    EventDescription[SECS_EVENT.ChangeUser          ]="24 Switching User Level";
    EventDescription[SECS_EVENT.EnterSetup          ]="25 Enter Setup Page";
    EventDescription[SECS_EVENT.EnterToolPage       ]="26 Enter Tool Page";
    EventDescription[SECS_EVENT.EnterMaintenPage    ]="27 Enter Maintenance Page";
    EventDescription[SECS_EVENT.EnterOffset         ]="28 Enter Offset Page";
    EventDescription[SECS_EVENT.EnterSpeed          ]="29 Enter Speed Page";
    EventDescription[SECS_EVENT.EnterIOPage         ]="30 Enter I/O Page";
    EventDescription[SECS_EVENT.EnterMessage        ]="31 Enter Message Page";
    EventDescription[SECS_EVENT.EnterTeach          ]="32 Enter Teach Page";
    EventDescription[SECS_EVENT.EnterMotorTest      ]="33 Enter Motor Test Page";
    EventDescription[SECS_EVENT.EnterSECSPage       ]="34 Enter SECS GEM Page";
    EventDescription[SECS_EVENT.OneCycleOK          ]="35 One Cycle Finish";
    EventDescription[SECS_EVENT.CleanOutOK          ]="36 Clean Out Finish";
    EventDescription[SECS_EVENT.TrayFeedOK          ]="37 Tray Feed Finish";
    EventDescription[SECS_EVENT.TimeEvent           ]="38 Time Event";
    EventDescription[SECS_EVENT.Store01Full         ]="39 Store 1 Full";
    EventDescription[SECS_EVENT.Store02Full         ]="40 Store 2 Full";
    EventDescription[SECS_EVENT.Store03Full         ]="41 Store 3 Full";
    EventDescription[SECS_EVENT.Store04Full         ]="42 Store 4 Full";
    EventDescription[SECS_EVENT.Store05Full         ]="43 Store 5 Full";
    EventDescription[SECS_EVENT.Store06Full         ]="44 Store 6 Full";
    EventDescription[SECS_EVENT.Store07Full         ]="45 Store 7 Full";
    EventDescription[SECS_EVENT.Store08Full         ]="46 Store 8 Full";
    EventDescription[SECS_EVENT.Store09Full         ]="47 Store 9 Full";
    EventDescription[SECS_EVENT.Store10Full         ]="48 Store 10 Full";
    EventDescription[SECS_EVENT.Store11Full         ]="49 Store 11 Full";
    EventDescription[SECS_EVENT.Store12Full         ]="50 Store 12 Full";
    EventDescription[SECS_EVENT.Store13Full         ]="51 Store 13 Full";
    EventDescription[SECS_EVENT.Store14Full         ]="52 Store 14 Full";
    EventDescription[SECS_EVENT.Store15Full         ]="53 Store 15 Full";
    EventDescription[SECS_EVENT.Store16Full         ]="54 Store 16 Full";
    EventDescription[SECS_EVENT.Store17Full         ]="55 Store 17 Full";
    EventDescription[SECS_EVENT.Store18Full         ]="56 Store 18 Full";
    EventDescription[SECS_EVENT.Store19Full         ]="57 Store 19 Full";
    EventDescription[SECS_EVENT.Store20Full         ]="58 Store 20 Full";
    EventDescription[SECS_EVENT.Mag1FullIC          ]="59 Mag 1 Tray Full with IC";
    EventDescription[SECS_EVENT.Mag2FullIC          ]="60 Mag 2 Tray Full with IC";
    EventDescription[SECS_EVENT.Mag3FullIC          ]="60 Mag 3 Tray Full with IC";
    EventDescription[SECS_EVENT.Empty1FullTray      ]="61 Empty 1 Full with Tray";
    EventDescription[SECS_EVENT.Empty2FullTray      ]="62 Empty 2 Full with Tray";
    EventDescription[SECS_EVENT.Empty3FullTray      ]="63 Empty 3 Full with Tray";
    EventDescription[SECS_EVENT.LoaderCar_ReadTrayID]="64 Loader Car Read Tray ID";

};
//------------------------------------------------------------------------------
void HT172Gem::AddSV()
{
    //============================================================================================================================================
    // MAINFORM
    //============================================================================================================================================
    // SV                                                                       fData->labVersion             fData->labMachineId
    HGemPtr=HGem;
    HGemPtr->SetSVDataPointer(1000, HType.ASCII_TYPE, "Machine Model",          "",         &asModel                        , "");
    HGemPtr->SetSVDataPointer(1001, HType.ASCII_TYPE, "Machine Serial No",      "",         fMaintenance->edSerialNo        , "");
    HGemPtr->SetSVDataPointer(1002, HType.ASCII_TYPE, "Machine ID",             "",         fMaintenance->edN01_MachineID   , "");
    HGemPtr->SetSVDataPointer(1003, HType.ASCII_TYPE, "Software Version",       "",         fData->labVersion               , "");

    HGemPtr->SetSVDataPointer(1011, HType.ASCII_TYPE, "MachineRunStatus",       "",         fMain->palMainStatus_En         , "");     //20140124 wei 修改sces_gam 中文問題
    HGemPtr->SetSVDataPointer(1020, HType.ASCII_TYPE, "SetupFileName",          "",         fMain->cb_WorkFile              , "");
    HGemPtr->SetSVDataPointer(1021, HType.ASCII_TYPE, "User Level",             "",         fMain->cbbUserSelect            , "");
    HGemPtr->SetSVDataPointer(1022, HType.INT_4_TYPE, "RunMode",                "",         &HSys.LastSet.iRealDummy        , "");
    HGemPtr->SetSVDataPointer(1023, HType.INT_4_TYPE, "StartMode",              "",         &HSys.LastSet.iStartMode        , "");

    HGemPtr->SetSVDataPointer(1027, HType.INT_4_TYPE, "System Time",            "",         fMain->statMain->Panels->Items[7]        , "");
//    HGemPtr->SetSVDataPointer(1029, HType.INT_4_TYPE, "System Date",            "",         &HSys.LastSet.iStartMode        , "");
//    HGemPtr->SetSVDataPointer(1030, HType.INT_4_TYPE, "System Date & Time",     "",         &HSys.LastSet.iStartMode        , "");
    HGemPtr->SetSVDataPointer(1032, HType.ASCII_TYPE, "Power ON Time",          "Sec",      fData->labPowerOnTime           , "");
    HGemPtr->SetSVDataPointer(1033, HType.ASCII_TYPE, "RunningTime",            "Sec",      fData->labRunningTime           , "");
    HGemPtr->SetSVDataPointer(1034, HType.ASCII_TYPE, "ProductTime",            "Sec",      fData->labProductTime           , "");
    HGemPtr->SetSVDataPointer(1035, HType.ASCII_TYPE, "PauseTime",              "Sec",      fData->labPauseTime             , "");
    HGemPtr->SetSVDataPointer(1036, HType.ASCII_TYPE, "JamTime",                "Sec",      fData->labJamTime               , "");
    HGemPtr->SetSVDataPointer(1037, HType.ASCII_TYPE, "HomeTime",               "Sec",      fData->labHomeTime              , "");
    HGemPtr->SetSVDataPointer(1038, HType.ASCII_TYPE, "Loading Count",          "",         fMain->palloadingCount          , "");
    HGemPtr->SetSVDataPointer(1039, HType.ASCII_TYPE, "Unloading Count",        "",         fMain->palUnloadingCount        , "");

    HGemPtr->SetSVDataPointer(1080, HType.ASCII_TYPE, "Loader Car Tray ID",     "",         fMain->lblLoaderCarID           , "");
    HGemPtr->SetSVDataPointer(1081, HType.ASCII_TYPE, "Loader Tray ID",         "",         fMain->lblLoadCurrID_1          , "");
    HGemPtr->SetSVDataPointer(1082, HType.ASCII_TYPE, "Transfer Tray ID",       "",         fMain->lblTransferID            , "");
    HGemPtr->SetSVDataPointer(1083, HType.ASCII_TYPE, "Empty 1 Car Tray ID",    "",         fMain->lblEmpty1_CarID          , "");
    HGemPtr->SetSVDataPointer(1084, HType.ASCII_TYPE, "Empty 1 Track 1 Tray ID","",         fMain->lblEmpty1_Track1ID       , "");
    HGemPtr->SetSVDataPointer(1085, HType.ASCII_TYPE, "Empty 1 Track 2 Tray ID","",         fMain->lblEmpty1_Track2ID       , "");
    HGemPtr->SetSVDataPointer(1086, HType.ASCII_TYPE, "Empty 1 Track 3 Tray ID","",         fMain->lblEmpty1_Track3ID       , "");
    HGemPtr->SetSVDataPointer(1087, HType.ASCII_TYPE, "Empty 1 Track 4 Tray ID","",         fMain->lblEmpty1_Track4ID       , "");
    HGemPtr->SetSVDataPointer(1088, HType.ASCII_TYPE, "Empty 2 Car Tray ID",    "",         fMain->lblEmpty2_CarID          , "");
    HGemPtr->SetSVDataPointer(1089, HType.ASCII_TYPE, "Magazine Loader Tray ID","",         fMain->lblMgzCurrID_1           , "");

    HGemPtr->SetSVDataPointer(1101, HType.ASCII_TYPE, "Auto 1 Tray ID",         "",         fMain->palAuto01ID              , "");
    HGemPtr->SetSVDataPointer(1102, HType.ASCII_TYPE, "Auto 2 Tray ID",         "",         fMain->palAuto02ID              , "");
    HGemPtr->SetSVDataPointer(1103, HType.ASCII_TYPE, "Auto 3 Tray ID",         "",         fMain->palAuto03ID              , "");
    HGemPtr->SetSVDataPointer(1104, HType.ASCII_TYPE, "Auto 4 Tray ID",         "",         fMain->palAuto04ID              , "");
    HGemPtr->SetSVDataPointer(1105, HType.ASCII_TYPE, "Auto 5 Tray ID",         "",         fMain->palAuto05ID              , "");
    HGemPtr->SetSVDataPointer(1106, HType.ASCII_TYPE, "Auto 6 Tray ID",         "",         fMain->palAuto06ID              , "");
    HGemPtr->SetSVDataPointer(1107, HType.ASCII_TYPE, "Auto 7 Tray ID",         "",         fMain->palAuto07ID              , "");
    HGemPtr->SetSVDataPointer(1108, HType.ASCII_TYPE, "Auto 8 Tray ID",         "",         fMain->palAuto08ID              , "");
    HGemPtr->SetSVDataPointer(1109, HType.ASCII_TYPE, "Auto 9 Tray ID",         "",         fMain->palAuto09ID              , "");
    HGemPtr->SetSVDataPointer(1110, HType.ASCII_TYPE, "Auto 10 Tray ID",        "",         fMain->palAuto10ID              , "");
    HGemPtr->SetSVDataPointer(1111, HType.ASCII_TYPE, "Auto 11 Tray ID",        "",         fMain->palAuto11ID              , "");
    HGemPtr->SetSVDataPointer(1112, HType.ASCII_TYPE, "Auto 12 Tray ID",        "",         fMain->palAuto12ID              , "");
    HGemPtr->SetSVDataPointer(1113, HType.ASCII_TYPE, "Auto 13 Tray ID",        "",         fMain->palAuto13ID              , "");
    HGemPtr->SetSVDataPointer(1114, HType.ASCII_TYPE, "Auto 14 Tray ID",        "",         fMain->palAuto14ID              , "");
    HGemPtr->SetSVDataPointer(1115, HType.ASCII_TYPE, "Auto 15 Tray ID",        "",         fMain->palAuto15ID              , "");
    HGemPtr->SetSVDataPointer(1116, HType.ASCII_TYPE, "Auto 16 Tray ID",        "",         fMain->palAuto16ID              , "");
    HGemPtr->SetSVDataPointer(1117, HType.ASCII_TYPE, "Auto 17 Tray ID",        "",         fMain->palAuto17ID              , "");
    HGemPtr->SetSVDataPointer(1118, HType.ASCII_TYPE, "Auto 18 Tray ID",        "",         fMain->palAuto18ID              , "");
    HGemPtr->SetSVDataPointer(1119, HType.ASCII_TYPE, "Auto 19 Tray ID",        "",         fMain->palAuto19ID              , "");
    HGemPtr->SetSVDataPointer(1120, HType.ASCII_TYPE, "Auto 20 Tray ID",        "",         fMain->palAuto20ID              , "");

    HGemPtr->SetSVDataPointer(1131, HType.ASCII_TYPE, "Fix 1 / Mag 1 Top Tray ID", "",      fMain->pnlMgz1TopID             , "");
    HGemPtr->SetSVDataPointer(1132, HType.ASCII_TYPE, "Fix 2 / Mag 2 Top Tray ID", "",      fMain->pnlMgz2TopID             , "");
    HGemPtr->SetSVDataPointer(1133, HType.ASCII_TYPE, "Fix 3 / Mag 3 Top Tray ID", "",      fMain->pnlMgz3TopID             , "");

    HGemPtr->SetSVDataPointer(1141, HType.ASCII_TYPE, "Mag 1 Tray 1 ID",        "",         fMain->palMgz1_01ID             , "");
    HGemPtr->SetSVDataPointer(1142, HType.ASCII_TYPE, "Mag 1 Tray 2 ID",        "",         fMain->palMgz1_02ID             , "");
    HGemPtr->SetSVDataPointer(1143, HType.ASCII_TYPE, "Mag 1 Tray 3 ID",        "",         fMain->palMgz1_03ID             , "");
    HGemPtr->SetSVDataPointer(1144, HType.ASCII_TYPE, "Mag 1 Tray 4 ID",        "",         fMain->palMgz1_04ID             , "");
    HGemPtr->SetSVDataPointer(1145, HType.ASCII_TYPE, "Mag 1 Tray 5 ID",        "",         fMain->palMgz1_05ID             , "");
    HGemPtr->SetSVDataPointer(1146, HType.ASCII_TYPE, "Mag 1 Tray 6 ID",        "",         fMain->palMgz1_06ID             , "");
    HGemPtr->SetSVDataPointer(1147, HType.ASCII_TYPE, "Mag 1 Tray 7 ID",        "",         fMain->palMgz1_07ID             , "");
    HGemPtr->SetSVDataPointer(1148, HType.ASCII_TYPE, "Mag 1 Tray 8 ID",        "",         fMain->palMgz1_08ID             , "");
    HGemPtr->SetSVDataPointer(1149, HType.ASCII_TYPE, "Mag 1 Tray 9 ID",        "",         fMain->palMgz1_09ID             , "");
    HGemPtr->SetSVDataPointer(1150, HType.ASCII_TYPE, "Mag 1 Tray 10 ID",       "",         fMain->palMgz1_10ID             , "");
    HGemPtr->SetSVDataPointer(1151, HType.ASCII_TYPE, "Mag 1 Tray 11 ID",       "",         fMain->palMgz1_11ID             , "");
    HGemPtr->SetSVDataPointer(1152, HType.ASCII_TYPE, "Mag 1 Tray 12 ID",       "",         fMain->palMgz1_12ID             , "");
    HGemPtr->SetSVDataPointer(1153, HType.ASCII_TYPE, "Mag 1 Tray 13 ID",       "",         fMain->palMgz1_13ID             , "");
    HGemPtr->SetSVDataPointer(1154, HType.ASCII_TYPE, "Mag 1 Tray 14 ID",       "",         fMain->palMgz1_14ID             , "");
    HGemPtr->SetSVDataPointer(1155, HType.ASCII_TYPE, "Mag 1 Tray 15 ID",       "",         fMain->palMgz1_15ID             , "");
    HGemPtr->SetSVDataPointer(1156, HType.ASCII_TYPE, "Mag 1 Tray 16 ID",       "",         fMain->palMgz1_16ID             , "");
    HGemPtr->SetSVDataPointer(1157, HType.ASCII_TYPE, "Mag 1 Tray 17 ID",       "",         fMain->palMgz1_17ID             , "");
    HGemPtr->SetSVDataPointer(1158, HType.ASCII_TYPE, "Mag 1 Tray 18 ID",       "",         fMain->palMgz1_18ID             , "");
    HGemPtr->SetSVDataPointer(1159, HType.ASCII_TYPE, "Mag 1 Tray 19 ID",       "",         fMain->palMgz1_19ID             , "");
    HGemPtr->SetSVDataPointer(1160, HType.ASCII_TYPE, "Mag 1 Tray 20 ID",       "",         fMain->palMgz1_20ID             , "");
    HGemPtr->SetSVDataPointer(1161, HType.ASCII_TYPE, "Mag 1 Tray 21 ID",       "",         fMain->palMgz1_21ID             , "");
    HGemPtr->SetSVDataPointer(1162, HType.ASCII_TYPE, "Mag 1 Tray 22 ID",       "",         fMain->palMgz1_22ID             , "");
    HGemPtr->SetSVDataPointer(1163, HType.ASCII_TYPE, "Mag 1 Tray 23 ID",       "",         fMain->palMgz1_23ID             , "");
    HGemPtr->SetSVDataPointer(1164, HType.ASCII_TYPE, "Mag 1 Tray 24 ID",       "",         fMain->palMgz1_24ID             , "");
    HGemPtr->SetSVDataPointer(1165, HType.ASCII_TYPE, "Mag 1 Tray 25 ID",       "",         fMain->palMgz1_25ID             , "");
    HGemPtr->SetSVDataPointer(1166, HType.ASCII_TYPE, "Mag 1 Tray 26 ID",       "",         fMain->palMgz1_26ID             , "");
    HGemPtr->SetSVDataPointer(1167, HType.ASCII_TYPE, "Mag 1 Tray 27 ID",       "",         fMain->palMgz1_27ID             , "");

    HGemPtr->SetSVDataPointer(1168, HType.ASCII_TYPE, "Mag 2 Tray 1 ID",        "",         fMain->palMgz2_01ID             , "");
    HGemPtr->SetSVDataPointer(1169, HType.ASCII_TYPE, "Mag 2 Tray 2 ID",        "",         fMain->palMgz2_02ID             , "");
    HGemPtr->SetSVDataPointer(1170, HType.ASCII_TYPE, "Mag 2 Tray 3 ID",        "",         fMain->palMgz2_03ID             , "");
    HGemPtr->SetSVDataPointer(1171, HType.ASCII_TYPE, "Mag 2 Tray 4 ID",        "",         fMain->palMgz2_04ID             , "");
    HGemPtr->SetSVDataPointer(1172, HType.ASCII_TYPE, "Mag 2 Tray 5 ID",        "",         fMain->palMgz2_05ID             , "");
    HGemPtr->SetSVDataPointer(1173, HType.ASCII_TYPE, "Mag 2 Tray 6 ID",        "",         fMain->palMgz2_06ID             , "");
    HGemPtr->SetSVDataPointer(1174, HType.ASCII_TYPE, "Mag 2 Tray 7 ID",        "",         fMain->palMgz2_07ID             , "");
    HGemPtr->SetSVDataPointer(1175, HType.ASCII_TYPE, "Mag 2 Tray 8 ID",        "",         fMain->palMgz2_08ID             , "");
    HGemPtr->SetSVDataPointer(1176, HType.ASCII_TYPE, "Mag 2 Tray 9 ID",        "",         fMain->palMgz2_09ID             , "");
    HGemPtr->SetSVDataPointer(1177, HType.ASCII_TYPE, "Mag 2 Tray 10 ID",       "",         fMain->palMgz2_10ID             , "");
    HGemPtr->SetSVDataPointer(1178, HType.ASCII_TYPE, "Mag 2 Tray 11 ID",       "",         fMain->palMgz2_11ID             , "");
    HGemPtr->SetSVDataPointer(1179, HType.ASCII_TYPE, "Mag 2 Tray 12 ID",       "",         fMain->palMgz2_12ID             , "");
    HGemPtr->SetSVDataPointer(1180, HType.ASCII_TYPE, "Mag 2 Tray 13 ID",       "",         fMain->palMgz2_13ID             , "");
    HGemPtr->SetSVDataPointer(1181, HType.ASCII_TYPE, "Mag 2 Tray 14 ID",       "",         fMain->palMgz2_14ID             , "");
    HGemPtr->SetSVDataPointer(1182, HType.ASCII_TYPE, "Mag 2 Tray 15 ID",       "",         fMain->palMgz2_15ID             , "");
    HGemPtr->SetSVDataPointer(1183, HType.ASCII_TYPE, "Mag 2 Tray 16 ID",       "",         fMain->palMgz2_16ID             , "");
    HGemPtr->SetSVDataPointer(1184, HType.ASCII_TYPE, "Mag 2 Tray 17 ID",       "",         fMain->palMgz2_17ID             , "");
    HGemPtr->SetSVDataPointer(1185, HType.ASCII_TYPE, "Mag 2 Tray 18 ID",       "",         fMain->palMgz2_18ID             , "");
    HGemPtr->SetSVDataPointer(1186, HType.ASCII_TYPE, "Mag 2 Tray 19 ID",       "",         fMain->palMgz2_19ID             , "");
    HGemPtr->SetSVDataPointer(1187, HType.ASCII_TYPE, "Mag 2 Tray 20 ID",       "",         fMain->palMgz2_20ID             , "");
    HGemPtr->SetSVDataPointer(1188, HType.ASCII_TYPE, "Mag 2 Tray 21 ID",       "",         fMain->palMgz2_21ID             , "");
    HGemPtr->SetSVDataPointer(1189, HType.ASCII_TYPE, "Mag 2 Tray 22 ID",       "",         fMain->palMgz2_22ID             , "");
    HGemPtr->SetSVDataPointer(1190, HType.ASCII_TYPE, "Mag 2 Tray 23 ID",       "",         fMain->palMgz2_23ID             , "");
    HGemPtr->SetSVDataPointer(1191, HType.ASCII_TYPE, "Mag 2 Tray 24 ID",       "",         fMain->palMgz2_24ID             , "");
    HGemPtr->SetSVDataPointer(1192, HType.ASCII_TYPE, "Mag 2 Tray 25 ID",       "",         fMain->palMgz2_25ID             , "");
    HGemPtr->SetSVDataPointer(1193, HType.ASCII_TYPE, "Mag 2 Tray 26 ID",       "",         fMain->palMgz2_26ID             , "");
    HGemPtr->SetSVDataPointer(1194, HType.ASCII_TYPE, "Mag 2 Tray 27 ID",       "",         fMain->palMgz2_27ID             , "");

    HGemPtr->SetSVDataPointer(1195, HType.ASCII_TYPE, "Mag 3 Tray 1 ID",        "",         fMain->palMgz3_01ID             , "");
    HGemPtr->SetSVDataPointer(1196, HType.ASCII_TYPE, "Mag 3 Tray 2 ID",        "",         fMain->palMgz3_02ID             , "");
    HGemPtr->SetSVDataPointer(1197, HType.ASCII_TYPE, "Mag 3 Tray 3 ID",        "",         fMain->palMgz3_03ID             , "");
    HGemPtr->SetSVDataPointer(1198, HType.ASCII_TYPE, "Mag 3 Tray 4 ID",        "",         fMain->palMgz3_04ID             , "");
    HGemPtr->SetSVDataPointer(1199, HType.ASCII_TYPE, "Mag 3 Tray 5 ID",        "",         fMain->palMgz3_05ID             , "");
    HGemPtr->SetSVDataPointer(1200, HType.ASCII_TYPE, "Mag 3 Tray 6 ID",        "",         fMain->palMgz3_06ID             , "");
    HGemPtr->SetSVDataPointer(1201, HType.ASCII_TYPE, "Mag 3 Tray 7 ID",        "",         fMain->palMgz3_07ID             , "");
    HGemPtr->SetSVDataPointer(1202, HType.ASCII_TYPE, "Mag 3 Tray 8 ID",        "",         fMain->palMgz3_08ID             , "");
    HGemPtr->SetSVDataPointer(1203, HType.ASCII_TYPE, "Mag 3 Tray 9 ID",        "",         fMain->palMgz3_09ID             , "");
    HGemPtr->SetSVDataPointer(1204, HType.ASCII_TYPE, "Mag 3 Tray 10 ID",       "",         fMain->palMgz3_10ID             , "");
    HGemPtr->SetSVDataPointer(1205, HType.ASCII_TYPE, "Mag 3 Tray 11 ID",       "",         fMain->palMgz3_11ID             , "");
    HGemPtr->SetSVDataPointer(1206, HType.ASCII_TYPE, "Mag 3 Tray 12 ID",       "",         fMain->palMgz3_12ID             , "");
    HGemPtr->SetSVDataPointer(1207, HType.ASCII_TYPE, "Mag 3 Tray 13 ID",       "",         fMain->palMgz3_13ID             , "");
    HGemPtr->SetSVDataPointer(1208, HType.ASCII_TYPE, "Mag 3 Tray 14 ID",       "",         fMain->palMgz3_14ID             , "");
    HGemPtr->SetSVDataPointer(1209, HType.ASCII_TYPE, "Mag 3 Tray 15 ID",       "",         fMain->palMgz3_15ID             , "");
    HGemPtr->SetSVDataPointer(1210, HType.ASCII_TYPE, "Mag 3 Tray 16 ID",       "",         fMain->palMgz3_16ID             , "");
    HGemPtr->SetSVDataPointer(1211, HType.ASCII_TYPE, "Mag 3 Tray 17 ID",       "",         fMain->palMgz3_17ID             , "");
    HGemPtr->SetSVDataPointer(1212, HType.ASCII_TYPE, "Mag 3 Tray 18 ID",       "",         fMain->palMgz3_18ID             , "");
    HGemPtr->SetSVDataPointer(1213, HType.ASCII_TYPE, "Mag 3 Tray 19 ID",       "",         fMain->palMgz3_19ID             , "");
    HGemPtr->SetSVDataPointer(1214, HType.ASCII_TYPE, "Mag 3 Tray 20 ID",       "",         fMain->palMgz3_20ID             , "");
    HGemPtr->SetSVDataPointer(1215, HType.ASCII_TYPE, "Mag 3 Tray 21 ID",       "",         fMain->palMgz3_21ID             , "");
    HGemPtr->SetSVDataPointer(1216, HType.ASCII_TYPE, "Mag 3 Tray 22 ID",       "",         fMain->palMgz3_22ID             , "");
    HGemPtr->SetSVDataPointer(1217, HType.ASCII_TYPE, "Mag 3 Tray 23 ID",       "",         fMain->palMgz3_23ID             , "");
    HGemPtr->SetSVDataPointer(1218, HType.ASCII_TYPE, "Mag 3 Tray 24 ID",       "",         fMain->palMgz3_24ID             , "");
    HGemPtr->SetSVDataPointer(1219, HType.ASCII_TYPE, "Mag 3 Tray 25 ID",       "",         fMain->palMgz3_25ID             , "");
    HGemPtr->SetSVDataPointer(1220, HType.ASCII_TYPE, "Mag 3 Tray 26 ID",       "",         fMain->palMgz3_26ID             , "");
    HGemPtr->SetSVDataPointer(1221, HType.ASCII_TYPE, "Mag 3 Tray 27 ID",       "",         fMain->palMgz3_27ID             , "");

    HGemPtr->SetSVDataPointer(1301, HType.ASCII_TYPE, "Auto 1 Tray Count",      "",         fMain->palAuto01Cnt             , "");
    HGemPtr->SetSVDataPointer(1302, HType.ASCII_TYPE, "Auto 2 Tray Count",      "",         fMain->palAuto02Cnt             , "");
    HGemPtr->SetSVDataPointer(1303, HType.ASCII_TYPE, "Auto 3 Tray Count",      "",         fMain->palAuto03Cnt             , "");
    HGemPtr->SetSVDataPointer(1304, HType.ASCII_TYPE, "Auto 4 Tray Count",      "",         fMain->palAuto04Cnt             , "");
    HGemPtr->SetSVDataPointer(1305, HType.ASCII_TYPE, "Auto 5 Tray Count",      "",         fMain->palAuto05Cnt             , "");
    HGemPtr->SetSVDataPointer(1306, HType.ASCII_TYPE, "Auto 6 Tray Count",      "",         fMain->palAuto06Cnt             , "");
    HGemPtr->SetSVDataPointer(1307, HType.ASCII_TYPE, "Auto 7 Tray Count",      "",         fMain->palAuto07Cnt             , "");
    HGemPtr->SetSVDataPointer(1308, HType.ASCII_TYPE, "Auto 8 Tray Count",      "",         fMain->palAuto08Cnt             , "");
    HGemPtr->SetSVDataPointer(1309, HType.ASCII_TYPE, "Auto 9 Tray Count",      "",         fMain->palAuto09Cnt             , "");
    HGemPtr->SetSVDataPointer(1310, HType.ASCII_TYPE, "Auto 10 Tray Count",     "",         fMain->palAuto10Cnt             , "");
    HGemPtr->SetSVDataPointer(1311, HType.ASCII_TYPE, "Auto 11 Tray Count",     "",         fMain->palAuto11Cnt             , "");
    HGemPtr->SetSVDataPointer(1312, HType.ASCII_TYPE, "Auto 12 Tray Count",     "",         fMain->palAuto12Cnt             , "");
    HGemPtr->SetSVDataPointer(1313, HType.ASCII_TYPE, "Auto 13 Tray Count",     "",         fMain->palAuto13Cnt             , "");
    HGemPtr->SetSVDataPointer(1314, HType.ASCII_TYPE, "Auto 14 Tray Count",     "",         fMain->palAuto14Cnt             , "");
    HGemPtr->SetSVDataPointer(1315, HType.ASCII_TYPE, "Auto 15 Tray Count",     "",         fMain->palAuto15Cnt             , "");
    HGemPtr->SetSVDataPointer(1316, HType.ASCII_TYPE, "Auto 16 Tray Count",     "",         fMain->palAuto16Cnt             , "");
    HGemPtr->SetSVDataPointer(1317, HType.ASCII_TYPE, "Auto 17 Tray Count",     "",         fMain->palAuto17Cnt             , "");
    HGemPtr->SetSVDataPointer(1318, HType.ASCII_TYPE, "Auto 18 Tray Count",     "",         fMain->palAuto18Cnt             , "");
    HGemPtr->SetSVDataPointer(1319, HType.ASCII_TYPE, "Auto 19 Tray Count",     "",         fMain->palAuto19Cnt             , "");
    HGemPtr->SetSVDataPointer(1320, HType.ASCII_TYPE, "Auto 20 Tray Count",     "",         fMain->palAuto20Cnt             , "");

    HGemPtr->SetSVDataPointer(1331, HType.ASCII_TYPE, "Fix 1 / Mag 1 Top Tray Count", "",   fMain->palMag1TopCnt            , "");
    HGemPtr->SetSVDataPointer(1332, HType.ASCII_TYPE, "Fix 2 / Mag 2 Top Tray Count", "",   fMain->palMag2TopCnt            , "");
    HGemPtr->SetSVDataPointer(1333, HType.ASCII_TYPE, "Fix 3 / Mag 3 Top Tray Count", "",   fMain->palMag3TopCnt            , "");

    HGemPtr->SetSVDataPointer(1341, HType.ASCII_TYPE, "Mag 1 Tray 1 Count",     "",         fMain->palMag1_01Cn             , "");
    HGemPtr->SetSVDataPointer(1342, HType.ASCII_TYPE, "Mag 1 Tray 2 Count",     "",         fMain->palMag1_02Cn             , "");
    HGemPtr->SetSVDataPointer(1343, HType.ASCII_TYPE, "Mag 1 Tray 3 Count",     "",         fMain->palMag1_03Cn             , "");
    HGemPtr->SetSVDataPointer(1344, HType.ASCII_TYPE, "Mag 1 Tray 4 Count",     "",         fMain->palMag1_04Cn             , "");
    HGemPtr->SetSVDataPointer(1345, HType.ASCII_TYPE, "Mag 1 Tray 5 Count",     "",         fMain->palMag1_05Cn             , "");
    HGemPtr->SetSVDataPointer(1346, HType.ASCII_TYPE, "Mag 1 Tray 6 Count",     "",         fMain->palMag1_06Cn             , "");
    HGemPtr->SetSVDataPointer(1347, HType.ASCII_TYPE, "Mag 1 Tray 7 Count",     "",         fMain->palMag1_07Cn             , "");
    HGemPtr->SetSVDataPointer(1348, HType.ASCII_TYPE, "Mag 1 Tray 8 Count",     "",         fMain->palMag1_08Cn             , "");
    HGemPtr->SetSVDataPointer(1349, HType.ASCII_TYPE, "Mag 1 Tray 9 Count",     "",         fMain->palMag1_09Cn             , "");
    HGemPtr->SetSVDataPointer(1350, HType.ASCII_TYPE, "Mag 1 Tray 10 Count",    "",         fMain->palMag1_10Cn             , "");
    HGemPtr->SetSVDataPointer(1351, HType.ASCII_TYPE, "Mag 1 Tray 11 Count",    "",         fMain->palMag1_11Cn             , "");
    HGemPtr->SetSVDataPointer(1352, HType.ASCII_TYPE, "Mag 1 Tray 12 Count",    "",         fMain->palMag1_12Cn             , "");
    HGemPtr->SetSVDataPointer(1353, HType.ASCII_TYPE, "Mag 1 Tray 13 Count",    "",         fMain->palMag1_13Cn             , "");
    HGemPtr->SetSVDataPointer(1354, HType.ASCII_TYPE, "Mag 1 Tray 14 Count",    "",         fMain->palMag1_14Cn             , "");
    HGemPtr->SetSVDataPointer(1355, HType.ASCII_TYPE, "Mag 1 Tray 15 Count",    "",         fMain->palMag1_15Cn             , "");
    HGemPtr->SetSVDataPointer(1356, HType.ASCII_TYPE, "Mag 1 Tray 16 Count",    "",         fMain->palMag1_16Cn             , "");
    HGemPtr->SetSVDataPointer(1357, HType.ASCII_TYPE, "Mag 1 Tray 17 Count",    "",         fMain->palMag1_17Cn             , "");
    HGemPtr->SetSVDataPointer(1358, HType.ASCII_TYPE, "Mag 1 Tray 18 Count",    "",         fMain->palMag1_18Cn             , "");
    HGemPtr->SetSVDataPointer(1359, HType.ASCII_TYPE, "Mag 1 Tray 19 Count",    "",         fMain->palMag1_19Cn             , "");
    HGemPtr->SetSVDataPointer(1360, HType.ASCII_TYPE, "Mag 1 Tray 20 Count",    "",         fMain->palMag1_20Cn             , "");
    HGemPtr->SetSVDataPointer(1361, HType.ASCII_TYPE, "Mag 1 Tray 21 Count",    "",         fMain->palMag1_21Cn             , "");
    HGemPtr->SetSVDataPointer(1362, HType.ASCII_TYPE, "Mag 1 Tray 22 Count",    "",         fMain->palMag1_22Cn             , "");
    HGemPtr->SetSVDataPointer(1363, HType.ASCII_TYPE, "Mag 1 Tray 23 Count",    "",         fMain->palMag1_23Cn             , "");
    HGemPtr->SetSVDataPointer(1364, HType.ASCII_TYPE, "Mag 1 Tray 24 Count",    "",         fMain->palMag1_24Cn             , "");
    HGemPtr->SetSVDataPointer(1365, HType.ASCII_TYPE, "Mag 1 Tray 25 Count",    "",         fMain->palMag1_25Cn             , "");
    HGemPtr->SetSVDataPointer(1366, HType.ASCII_TYPE, "Mag 1 Tray 26 Count",    "",         fMain->palMag1_26Cn             , "");
    HGemPtr->SetSVDataPointer(1367, HType.ASCII_TYPE, "Mag 1 Tray 27 Count",    "",         fMain->palMag1_27Cn             , "");

    HGemPtr->SetSVDataPointer(1368, HType.ASCII_TYPE, "Mag 2 Tray 1 Count",     "",         fMain->palMag2_01Cn             , "");
    HGemPtr->SetSVDataPointer(1369, HType.ASCII_TYPE, "Mag 2 Tray 2 Count",     "",         fMain->palMag2_02Cn             , "");
    HGemPtr->SetSVDataPointer(1370, HType.ASCII_TYPE, "Mag 2 Tray 3 Count",     "",         fMain->palMag2_03Cn             , "");
    HGemPtr->SetSVDataPointer(1371, HType.ASCII_TYPE, "Mag 2 Tray 4 Count",     "",         fMain->palMag2_04Cn             , "");
    HGemPtr->SetSVDataPointer(1372, HType.ASCII_TYPE, "Mag 2 Tray 5 Count",     "",         fMain->palMag2_05Cn             , "");
    HGemPtr->SetSVDataPointer(1373, HType.ASCII_TYPE, "Mag 2 Tray 6 Count",     "",         fMain->palMag2_06Cn             , "");
    HGemPtr->SetSVDataPointer(1374, HType.ASCII_TYPE, "Mag 2 Tray 7 Count",     "",         fMain->palMag2_07Cn             , "");
    HGemPtr->SetSVDataPointer(1375, HType.ASCII_TYPE, "Mag 2 Tray 8 Count",     "",         fMain->palMag2_08Cn             , "");
    HGemPtr->SetSVDataPointer(1376, HType.ASCII_TYPE, "Mag 2 Tray 9 Count",     "",         fMain->palMag2_09Cn             , "");
    HGemPtr->SetSVDataPointer(1377, HType.ASCII_TYPE, "Mag 2 Tray 10 Count",    "",         fMain->palMag2_10Cn             , "");
    HGemPtr->SetSVDataPointer(1378, HType.ASCII_TYPE, "Mag 2 Tray 11 Count",    "",         fMain->palMag2_11Cn             , "");
    HGemPtr->SetSVDataPointer(1379, HType.ASCII_TYPE, "Mag 2 Tray 12 Count",    "",         fMain->palMag2_12Cn             , "");
    HGemPtr->SetSVDataPointer(1380, HType.ASCII_TYPE, "Mag 2 Tray 13 Count",    "",         fMain->palMag2_13Cn             , "");
    HGemPtr->SetSVDataPointer(1381, HType.ASCII_TYPE, "Mag 2 Tray 14 Count",    "",         fMain->palMag2_14Cn             , "");
    HGemPtr->SetSVDataPointer(1382, HType.ASCII_TYPE, "Mag 2 Tray 15 Count",    "",         fMain->palMag2_15Cn             , "");
    HGemPtr->SetSVDataPointer(1383, HType.ASCII_TYPE, "Mag 2 Tray 16 Count",    "",         fMain->palMag2_16Cn             , "");
    HGemPtr->SetSVDataPointer(1384, HType.ASCII_TYPE, "Mag 2 Tray 17 Count",    "",         fMain->palMag2_17Cn             , "");
    HGemPtr->SetSVDataPointer(1385, HType.ASCII_TYPE, "Mag 2 Tray 18 Count",    "",         fMain->palMag2_18Cn             , "");
    HGemPtr->SetSVDataPointer(1386, HType.ASCII_TYPE, "Mag 2 Tray 19 Count",    "",         fMain->palMag2_19Cn             , "");
    HGemPtr->SetSVDataPointer(1387, HType.ASCII_TYPE, "Mag 2 Tray 20 Count",    "",         fMain->palMag2_20Cn             , "");
    HGemPtr->SetSVDataPointer(1388, HType.ASCII_TYPE, "Mag 2 Tray 21 Count",    "",         fMain->palMag2_21Cn             , "");
    HGemPtr->SetSVDataPointer(1389, HType.ASCII_TYPE, "Mag 2 Tray 22 Count",    "",         fMain->palMag2_22Cn             , "");
    HGemPtr->SetSVDataPointer(1390, HType.ASCII_TYPE, "Mag 2 Tray 23 Count",    "",         fMain->palMag2_23Cn             , "");
    HGemPtr->SetSVDataPointer(1391, HType.ASCII_TYPE, "Mag 2 Tray 24 Count",    "",         fMain->palMag2_24Cn             , "");
    HGemPtr->SetSVDataPointer(1392, HType.ASCII_TYPE, "Mag 2 Tray 25 Count",    "",         fMain->palMag2_25Cn             , "");
    HGemPtr->SetSVDataPointer(1393, HType.ASCII_TYPE, "Mag 2 Tray 26 Count",    "",         fMain->palMag2_26Cn             , "");
    HGemPtr->SetSVDataPointer(1394, HType.ASCII_TYPE, "Mag 2 Tray 27 Count",    "",         fMain->palMag2_27Cn             , "");

    HGemPtr->SetSVDataPointer(1395, HType.ASCII_TYPE, "Mag 3 Tray 1 Count",     "",         fMain->palMag3_01Cn             , "");
    HGemPtr->SetSVDataPointer(1396, HType.ASCII_TYPE, "Mag 3 Tray 2 Count",     "",         fMain->palMag3_02Cn             , "");
    HGemPtr->SetSVDataPointer(1397, HType.ASCII_TYPE, "Mag 3 Tray 3 Count",     "",         fMain->palMag3_03Cn             , "");
    HGemPtr->SetSVDataPointer(1398, HType.ASCII_TYPE, "Mag 3 Tray 4 Count",     "",         fMain->palMag3_04Cn             , "");
    HGemPtr->SetSVDataPointer(1399, HType.ASCII_TYPE, "Mag 3 Tray 5 Count",     "",         fMain->palMag3_05Cn             , "");
    HGemPtr->SetSVDataPointer(1400, HType.ASCII_TYPE, "Mag 3 Tray 6 Count",     "",         fMain->palMag3_06Cn             , "");
    HGemPtr->SetSVDataPointer(1401, HType.ASCII_TYPE, "Mag 3 Tray 7 Count",     "",         fMain->palMag3_07Cn             , "");
    HGemPtr->SetSVDataPointer(1402, HType.ASCII_TYPE, "Mag 3 Tray 8 Count",     "",         fMain->palMag3_08Cn             , "");
    HGemPtr->SetSVDataPointer(1403, HType.ASCII_TYPE, "Mag 3 Tray 9 Count",     "",         fMain->palMag3_09Cn             , "");
    HGemPtr->SetSVDataPointer(1404, HType.ASCII_TYPE, "Mag 3 Tray 10 Count",    "",         fMain->palMag3_10Cn             , "");
    HGemPtr->SetSVDataPointer(1405, HType.ASCII_TYPE, "Mag 3 Tray 11 Count",    "",         fMain->palMag3_11Cn             , "");
    HGemPtr->SetSVDataPointer(1406, HType.ASCII_TYPE, "Mag 3 Tray 12 Count",    "",         fMain->palMag3_12Cn             , "");
    HGemPtr->SetSVDataPointer(1407, HType.ASCII_TYPE, "Mag 3 Tray 13 Count",    "",         fMain->palMag3_13Cn             , "");
    HGemPtr->SetSVDataPointer(1408, HType.ASCII_TYPE, "Mag 3 Tray 14 Count",    "",         fMain->palMag3_14Cn             , "");
    HGemPtr->SetSVDataPointer(1409, HType.ASCII_TYPE, "Mag 3 Tray 15 Count",    "",         fMain->palMag3_15Cn             , "");
    HGemPtr->SetSVDataPointer(1410, HType.ASCII_TYPE, "Mag 3 Tray 16 Count",    "",         fMain->palMag3_16Cn             , "");
    HGemPtr->SetSVDataPointer(1411, HType.ASCII_TYPE, "Mag 3 Tray 17 Count",    "",         fMain->palMag3_17Cn             , "");
    HGemPtr->SetSVDataPointer(1412, HType.ASCII_TYPE, "Mag 3 Tray 18 Count",    "",         fMain->palMag3_18Cn             , "");
    HGemPtr->SetSVDataPointer(1413, HType.ASCII_TYPE, "Mag 3 Tray 19 Count",    "",         fMain->palMag3_19Cn             , "");
    HGemPtr->SetSVDataPointer(1414, HType.ASCII_TYPE, "Mag 3 Tray 20 Count",    "",         fMain->palMag3_20Cn             , "");
    HGemPtr->SetSVDataPointer(1415, HType.ASCII_TYPE, "Mag 3 Tray 21 Count",    "",         fMain->palMag3_21Cn             , "");
    HGemPtr->SetSVDataPointer(1416, HType.ASCII_TYPE, "Mag 3 Tray 22 Count",    "",         fMain->palMag3_22Cn             , "");
    HGemPtr->SetSVDataPointer(1417, HType.ASCII_TYPE, "Mag 3 Tray 23 Count",    "",         fMain->palMag3_23Cn             , "");
    HGemPtr->SetSVDataPointer(1418, HType.ASCII_TYPE, "Mag 3 Tray 24 Count",    "",         fMain->palMag3_24Cn             , "");
    HGemPtr->SetSVDataPointer(1419, HType.ASCII_TYPE, "Mag 3 Tray 25 Count",    "",         fMain->palMag3_25Cn             , "");
    HGemPtr->SetSVDataPointer(1420, HType.ASCII_TYPE, "Mag 3 Tray 26 Count",    "",         fMain->palMag3_26Cn             , "");
    HGemPtr->SetSVDataPointer(1421, HType.ASCII_TYPE, "Mag 3 Tray 27 Count",    "",         fMain->palMag3_27Cn             , "");
}
//------------------------------------------------------------------------------
void HT172Gem::AddEC()
{
    HGemPtr->SetECDataPointer(2000, HType.ASCII_TYPE, "Lot No",             "", fMain->edLotNo,                "", "", "", "");
    HGemPtr->SetECDataPointer(2001, HType.ASCII_TYPE, "Setup Up File",      "", fMain->cb_WorkFile,            "", "", "", "Lot No");
    HGemPtr->SetECDataPointer(2002, HType.ASCII_TYPE, "User Name",          "", fMain->cbbUserSelect,          "", "", "", "User Name");
    HGemPtr->SetECDataPointer(2003, HType.INT_4_TYPE, "Running Mode",       "", &HSys.LastSet.iRealDummy,      "", "", "", "Running Mode");
    HGemPtr->SetECDataPointer(2004, HType.INT_4_TYPE, "Start Mode",         "", &HSys.LastSet.iStartMode,      "", "", "", "Start Mode");
    HGemPtr->SetECDataPointer(2005, HType.ASCII_TYPE, "Bin To Tray Setting","", "",   "", "", "", "CSV format from bin 0 to bin 47.");
}
//------------------------------------------------------------------------------
void HT172Gem::ReloadParameter()
{
    fMain->LoadRunModePicture();
    fMain->LoadStartModePicture();
    UpdateAllParameter();
}
//------------------------------------------------------------------------------
void HT172Gem::AddAlarmList()
{
    int iCount=1;
    HGem->strGrdAlarm->Cells[0][0]="No.";
    HGem->strGrdAlarm->Cells[1][0]="ID_AlarmList";          //ID_AlarmList
    HGem->strGrdAlarm->Cells[2][0]="AlarmCode";
    HGem->strGrdAlarm->Cells[3][0]="UnitNo";
    HGem->strGrdAlarm->Cells[4][0]="UnitName";
    HGem->strGrdAlarm->Cells[5][0]="Type";
    HGem->strGrdAlarm->Cells[6][0]="Message";
    HGem->strGrdAlarm->Cells[7][0]="Enable";
    HGem->strGrdAlarm->Cells[8][0]="AlarmID";
    HGem->strGrdAlarm->Cells[9][0]="Class";
    HGem->strGrdAlarm->Cells[10][0]="Position";

    HGemPtr->ReadAlamData();
    for(HSys.IterAlarmCodeList=HSys.mapAlarmCodeList.begin(); HSys.IterAlarmCodeList!=HSys.mapAlarmCodeList.end(); HSys.IterAlarmCodeList++)
    {
        HGemPtr->SetAlamData(iCount, HSys.IterAlarmCodeList->second.AlarmCode, HSys.IterAlarmCodeList->second.AlarmType, HSys.IterAlarmCodeList->second.E_ErrMessage, HSys.IterAlarmCodeList->second.AlarmType);
        iCount++;
    }

    HGemPtr->WriteAlamData();
    HGemPtr->ReadEventReportData();
}
//------------------------------------------------------------------------------
void HT172Gem::AddCEID()
{
//DATAID    CEID    Report ID   Remark                                      Report ID   SVID
//1         1       1           按下 Start                                    1           100,101,102,103,104,000

    //Steven 20130723 : 偷懶用For迴圈..
    int EquDefault=1;
    for(int i=SECS_EVENT.HandlerStatus; i<SECS_EVENT.TotalEvent; i++)
    {
        unsigned CEDIDContent[]={i};
        HGemPtr->SetCEIDContent(i, sizeof(CEDIDContent)/sizeof(unsigned), CEDIDContent, EquDefault); // Ceid=1 ,有 1個 ReportID ,分別是 1
    }
}
//------------------------------------------------------------------------------
void HT172Gem::AddReprot()
{
    int EquDefault=1;
    unsigned ReportIDContent[]={1027}; //System Time

    for(int i=SECS_EVENT.HandlerStatus; i<SECS_EVENT.TotalEvent; i++)
    {
        HGemPtr->SetReportIDContent(1, sizeof(ReportIDContent)/sizeof(unsigned), ReportIDContent, EquDefault);
    }

    HGem->SaveEventReportData();
}
//------------------------------------------------------------------------------
void HT172Gem::LookForFile()
{
    fMain->LookForFile();
}
//==============================================================================
//  V 1.0
//     ret
//     0   = Acknowledge
//     1   = Denied. At least one constant does not exist
//     2   = Denied. Busy
//     3   = Denied. At least one constant out of range
//    >3   = Other equipment-specific error
//    4-63   Reserved
//==============================================================================
int HT172Gem::S2F15_CheckNewEquipmentConstant()                                 //wei 20170417 (Steven) add S2F15
{
    int EClen,i,len,ret;
    unsigned char Type;

    __int64  int8EC;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4EC;
    short int2EC;
    char  int1EC;
    unsigned __int64  uint8EC;                                                  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4EC;
    AnsiString ECID;
//    int iECID;
    unsigned short uint2EC;
    unsigned char  uint1EC;
    float          floatEC;
    double         doubleEC;
//    void *P;

    /*
        L,n
        1. L,2
        1. <ECID1>
        2. <ECV1>
        2. L,2
        .
        .
        n. L,2
        1. <ECIDn>
        2. <ECVn>
    */

    if(HGem->GetDataItemLenAndTypeAndDelete(EClen, Type)!=1)
        return -1;
    if(Type!=HType.LIST_TYPE)
        return -1;
    if(EClen<1)
        return -1;
    for(i=0; i<EClen; i++)
    {
        if(HGem->GetDataItemLenAndTypeAndDelete(len, Type)!=1)
            return -1;
        if(Type!=HType.LIST_TYPE || len!=2)
            return -1;

        ret=1;
        if(HGem->GetDataItemLenAndType(len, Type)==1)
        {
            if(HGem->DataItemIn(len, Type, ECID)!=1)
                return -1;
        }
        else
        {
            return -1;
        }

//        iECID=atoi(ECID.c_str());

        if(HGem->GetDataItemLenAndType(len, Type)==1)
        {
            if(Type==HType.UINT_1_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint1EC)==1)
                    ret=CheckECValue(ECID, &uint1EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint2EC)==1)
                    ret=CheckECValue(ECID, &uint2EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint4EC)==1)
                    ret=CheckECValue(ECID, &uint4EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                if(HGem->DataItemIn(1, Type, &uint8EC)==1)
                    ret=CheckECValue(ECID, &uint8EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int2EC)==1)
                    ret=CheckECValue(ECID, &int2EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int4EC)==1)
                    ret=CheckECValue(ECID, &int4EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                if(HGem->DataItemIn(1, Type, &int8EC)==1)
                    ret=CheckECValue(ECID, &int8EC);
                else
                    return -1;
            }
            else if(Type==HType.ASCII_TYPE)
            {
                char *Str;
                Str=new char [len+100];
                if(HGem->DataItemIn(len+100, Type, Str)==1)
                {
                    ret=CheckECValue(ECID, Str);
                }
                else
                {
//                    delete[] Str;     //Steven 20160912 : Add delete for save memory, 先mark
                    return -1;
                }
            }
            else if(Type==HType.BINARY_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &floatEC)==1)
                    ret=CheckECValue(ECID, &floatEC);
                else
                    return -1;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &doubleEC)==1)
                {
                    ret=CheckECValue(ECID, &doubleEC);
                }
                else
                {
                    return -1;
                }
            }
            else  // error format (SECS-II ASCII code ir correct ,but ITRI is failure,need confirm with ITRI
            {
                HGem->SendInvalidDataMessageToHost("error format");
                return -1;
            }
            if(ret!=0)
            {
                return ret;
            }
        }
        else
        {
            HGem->SendInvalidDataMessageToHost("GetDataItemLenAndType Error");
            return -1;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
int HT172Gem::S2F15_UpdateNewEquipmentConstant()
{
    int EClen, i, len;
    unsigned char Type;

    __int64  int8EC;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4EC;
    short int2EC;
    char  int1EC;
    unsigned __int64 uint8EC;                                                   //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4EC, ECID;
    unsigned short uint2EC;
    unsigned char  uint1EC;
    float          floatEC;
    double         doubleEC;
    AnsiString S, FileName="", SitMapData="", SitMapDataBuffer="", SData="";   //kevin 20181107 add
//    int iArry[64]={0};
//    void *P;
//    bool b151920=false;
//    bool bspeed=false;            //kevin 20181210 add speed
    FileName=GetLastOpenFN();
    AnsiString szDir="";
    AnsiString str;

    if(HGem->GetDataItemLenAndTypeAndDelete(EClen, Type)==1)
    {
        if(Type==HType.LIST_TYPE)
        {
            for(i=0; i<EClen; i++)
            {
                if(HGem->GetDataItemLenAndTypeAndDelete(len, Type)==1)
                {
                    if(Type!=HType.LIST_TYPE || len!=2)
                        break;
                    // get ECID

                    HGem->GetDataItemLenAndType(len, Type);
                    HGem->DataItemIn(len, Type, S);
                    ECID=atoi(S.c_str());

                    HGem->GetDataItemLenAndType(len, Type);

                    if(Type==HType.UINT_1_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint1EC)==1)
                            SetECValue(ECID, &uint1EC);
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint2EC)==1)
                            SetECValue(ECID, &uint2EC);
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint4EC)==1)
                            SetECValue(ECID, &uint4EC);
                    }
                    else if(Type==HType.UINT_8_TYPE)    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        if(HGem->DataItemIn(1, Type, &uint8EC)==1)
                            SetECValue(ECID, &uint8EC);
                    }
                    else if(Type==HType.INT_1_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int1EC)==1)
                            SetECValue(ECID, &int1EC);
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int2EC)==1)
                            SetECValue(ECID, &int2EC);
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int4EC)==1)
                            SetECValue(ECID, &int4EC);
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        if(HGem->DataItemIn(1, Type, &int8EC)==1)
                            SetECValue(ECID, &int8EC);
                    }
                    else if(Type==HType.ASCII_TYPE)
                    {
                        char *Str;
                        Str=new char [len+100];
                        if(HGem->DataItemIn(len+100, Type, Str)==1)
                        {
                            SetECValue(ECID, Str);
                            SData= Str;                 //kevin 20181116 add
                        }
                        delete[] Str;                   //kevin 20181116 add
                    }
                    else if(Type==HType.BINARY_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &int1EC)==1)
                            SetECValue(ECID, &int1EC);
                    }
                    else if(Type==HType.BOOLEAN_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &int1EC)==1)
                        {
//                            if(int1EC==0x00)            //Steven 20230815 : 將ASCII轉成CHAR避免0x00或0x01造成不可視字元
//                                int1EC='0';
//                            else
//                                int1EC='1';
                            SetECValue(ECID, &int1EC);
                        }
                    }
                    else if(Type==HType.FT_4_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &floatEC)==1)
                            SetECValue(ECID, &floatEC);
                    }
                    else if(Type==HType.FT_8_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &doubleEC)==1)
                            SetECValue(ECID, &doubleEC);
                    }
                    else  // error format (SECS-II ASCII code ir correct ,but ITRI is failure,need confirm with ITRI
                    {
                        HGem->SendInvalidDataMessageToHost("error format");
                        return -1;
                    }
                }
                else
                {
                    return -1;
                }
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    ReloadParameter();
    return 1;
}
//------------------------------------------------------------------------------
int HT172Gem::S2F42_Host_Command_Acknowledge()
{
    unsigned char HCACK=1;
    char *CommandStr;
    CommandStr=new char [10000000];

    char str[256];
    bool bHasFile;
    AnsiString PPID, sLotNo, sLOTCount, asStr, sFile, DataID, sFullPath, MapData;
    int ret, len, SVlen;
    unsigned char Type;
    AnsiString S, S1, S2, S3, sLotInfo, strLotID, sLotCnt, str1;
    ZeroMemory(CommandStr, sizeof(CommandStr));
    ZeroMemory(str, sizeof(str));

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)                        // 需要補充多重 Command
    {
        HGemPtr->GetDataItemLenAndType(len, Type);
        if(Type==HType.ASCII_TYPE)
        {
            ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
            if(ret==-1)
                S="";
        }
        else
        {
            S="";
        }
        S=CommandStr;
        S=S.UpperCase();

        if(S.AnsiPos("PAUSE")==1)
        {
            fMain->sbPauseClick(fMain);
            HCACK=0;
        }
        else if(S.AnsiPos("TIME_EVENT")==1)
        {
            fMain->btnTimeEventClick(fMain);
            HCACK=0;
        }
        else if(S.AnsiPos("ONLINE_REMOTE")==1)
        {
            HGem->GemBtnOnlineRemote->OnClick(HGem);
            HCACK=0;
        }
        else if(S.AnsiPos("ONLINE_LOCAL")==1)
        {
            HGem->GemBtnOnlineLocal->OnClick(HGem);
            HCACK=0;
        }
		else if(S.AnsiPos("SET_USE_BIN")==1)			//Ian20241106
		{
			if(HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL)==1)
			{
				if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
				{
					HGem->GetDataItemLenAndType(len, Type);
					if(Type==HType.ASCII_TYPE)
					{
                        ret=HGemPtr->DataItemIn(7, HType.ASCII_TYPE, DataID);
                        if(ret==-1)
                        {
                            DataID="";
                        }
                        if(DataID=="USE_BIN")
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, MapData);
                            if(ret==-1)
                            {
                                MapData="";
                            }
                            else
                            {
                                fSetup->AutoBinSetAutoAreaOnly(MapData);
                            }
                        }
					}
                    else
                    {
                        DataID="";
                        MapData="";
                    }

				}
			}
		}
        else if(S.AnsiPos("PP_SELECT")==1 || S.AnsiPos("PP-SELECT")==1)
        {
            if(HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL)==1)
            {
                if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                {

                    HGem->GetDataItemLenAndType(len, Type);                 //取得資料長度與格式
                    if(Type==HType.ASCII_TYPE)
                    {
                        ret=HGem->DataItemIn(len, Type, str);                       //取值
                        S1=str;
                        if(S1!="")        //JerryYang 20250106 : modify PP-SELECT
                        {
                            HGemPtr->GetDataItemLenAndType(len, Type);
                            if(Type==HType.ASCII_TYPE)
                            {
                                ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, PPID);
                                if(ret==-1)
                                    PPID="";
                            }
                            else
                            {
                                PPID="";
                            }
                        }
                        else
                        {
                            PPID="";
                        }
                    }
                    else
                    {
                        HCACK=1;
                    }
                    bHasFile=false;
                    for(int i=0; i<fMain->cb_WorkFile->Items->Count; i++)
                    {
                        if(PPID!="" && PPID==fMain->cb_WorkFile->Items->Strings[i])
                            bHasFile=true;
                    }

                    if(bHasFile==false)
                    {
                        HCACK=6;  //Steven 20160425 : 改成HCACK=5-->6
                    }
                    else
                    {
                        if(HasICUnderMachine()==true || HSys.Sys.SystemStart==true)
                        {
                            HCACK=4;
                        }
                        else if(PPID=="")
                        {
                            HCACK=5;
                        }
                        else
                        {
                            fMain->cb_WorkFile->Text=PPID;
                            fMain->cb_WorkFileChange(fMain);
                            HCACK=0;
                        }
                    }

                    HGemPtr->GetDataItemLenAndType(len, Type);
                    if(Type==HType.ASCII_TYPE)
                    {
                        ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, PPID);
                    }
                }
            }
            else                                                                    //20140124 wei
            {
                HCACK=1;
            }
        }
        else if(S.AnsiPos("CLEAR_LOT_INFO")==1)
        {
            if(HSys.Sys.SystemStart==true)
            {
                HCACK=2;
            }
            else if(HasICUnderMachine()==true)
            {
                HCACK=2;
            }
            else
            {
                fMain->btnLotStart->Enabled=true;
                fMain->edLotNo->Enabled=true;
                fMain->edWaferLot->Enabled=true;
                fMain->edCusDevice->Enabled=true;
                fMain->edInsertion->Enabled=true;
                fMain->edFlowID->Enabled=true;
                fMain->edOperator->Enabled=true;

                RunInfo.LotStart=false;

                fMain->edLotNo->Text="";
                fMain->edWaferLot->Text="";
                fMain->edCusDevice->Text="";
                fMain->edInsertion->Text="";
                fMain->edFlowID->Text="";
                fMain->edOperator->Text="";
                HCACK=0;
            }
        }
        else if(S.AnsiPos("START")==1)  //JerryYang 20250106 : Run check
        {
            if(USE_SECS_GEM && HSys.FuncB.bN04_RunCheck && bPhysicalStart==true)
            {                                                                   
                SoftStart=true;
                HSys.Sys.SystemStart=true;
                HCACK=0;
            }
            else
            {
                HCACK=1;
            }
            bPhysicalStart=false;
        }        
        else if(S.AnsiPos("SET_LOT_INFO")==1)
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVlen,HType.LIST_TYPE)==1)
            {
                if(SVlen==0)
                {
                    HCACK=2;
                }
                else
                {
                    if(SVlen==5)
                    {
                        for(int i=0; i<SVlen; i++)
                        {
                            if( HGem->DataItemIn(2,HType.LIST_TYPE,NULL)==1)
                            {
                                HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                if(Type==HType.ASCII_TYPE)
                                {
                                    HGem->DataItemIn(len,Type,str); //取值
                                    S1=str;
                                    if(i==0)
                                    {
                                        if(S1=="LOT_ID")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S2); //取值

                                                fMain->edLotNo->Text=S2;
                                                HCACK=0;

                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==1)
                                    {
                                        if(S1=="RC_NO")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3); //取值
                                                fMain->edtRunCard->Text=S3;
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==2)
                                    {
                                        if(S1=="OP_ID")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3); //取值
                                                fMain->edOperator->Text=S3;
                                                HCACK=0;
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==3)
                                    {
                                        if(S1=="LOT_SUM")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3); //取值
                                                fXML->XMLDoc->LoadFromXML(S3);
                                                sFile.sprintf("D:\\BarcodeSorter\\%s.xml", fMain->edLotNo->Text);
                                                fXML->XMLDoc->SaveToFile(sFile);
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                    else if(i==4)
                                    {
                                        if(S1=="TRAY_MAP_PATH")
                                        {
                                            HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                            if(Type==HType.ASCII_TYPE)
                                            {
                                                HGem->DataItemIn(len,Type, S3); //取值
                                                asNetDisk=S3;
                                                DecodeTrayMapByWinstekFormat(fMain->edLotNo->Text);
                                            }
                                            else
                                            {
                                                HCACK=2;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            HCACK=2;
                                            break;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                HCACK=1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        HCACK=1;
                    }
                }
            }
            else
            {
                HCACK=1;
            }
        }
        else                                                                    //20140124 wei
        {
            HCACK=1;
        }

        delete[] CommandStr;    //Steven 20160912 : Add delete for save memory
        CommandStr=NULL;

        HGemPtr->InitLocalHead(2,42,0);

        if(HCACK==0)
        {
            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        }
        else
        {
            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        }

        HGemPtr->SendLocalData();
        return 1;
    }
    else
    {
        HCACK=3;
        HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
        HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
        HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
    }
    return HCACK;
}
//------------------------------------------------------------------------------
void HT172Gem::S5F6_ListAlarmData()                                             //Steven 20150520 : 修正S5F6 ListAlarmData
{
    int SVlen,i, j,ret;
    unsigned char Type;
    AnsiString S;

    unsigned char ALCD=0x80;
    unsigned int  ALID;
    AnsiString    ALTX;

    if(HGemPtr->GetDataItemLenAndType(SVlen, Type)==1)
    {
        if(SVlen!=0)
        {
            if(Type==HType.UINT_8_TYPE || Type==HType.UINT_4_TYPE || Type==HType.UINT_2_TYPE ||
               Type==HType.INT_8_TYPE  || Type==HType.INT_4_TYPE  || Type==HType.INT_2_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                unsigned __int64 *uint8Ptr;
                unsigned *uint4Ptr;
                unsigned short *uint2Ptr;
                __int64  *int8Ptr;
                int      *int4Ptr;
                short    *int2Ptr;

                ret=HGem->GetDataItemLenAndType(SVlen, Type);
                if(ret!=1)
                {
                    S9F7_IllegalData("S5,F5 Data Format error !!!");
                    return;
                }

                if(Type==HType.UINT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                {
                    uint8Ptr=new unsigned __int64 [SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.UINT_8_TYPE, uint8Ptr);
                }
                else if(Type==HType.UINT_4_TYPE)
                {
                    uint4Ptr=new unsigned [SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.UINT_4_TYPE, uint4Ptr);
                }
                else if(Type==HType.UINT_2_TYPE)
                {
                    uint2Ptr=new unsigned short [SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.UINT_2_TYPE, uint2Ptr);
                }
                else if(Type==HType.INT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                {
                    int8Ptr=new __int64[SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.INT_8_TYPE, int8Ptr);
                }
                else if(Type==HType.INT_4_TYPE)
                {
                    int4Ptr=new int[SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.INT_4_TYPE, int4Ptr);
                }
                else if(Type==HType.INT_2_TYPE)
                {
                    int2Ptr=new short[SVlen];
                    ret=HGem->DataItemIn(SVlen, HType.INT_2_TYPE, int2Ptr);
                }
                if(ret==1)
                {
                    HGem->InitLocalHead(5, 6, 0);
                    HGem->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                    for(i=0; i<SVlen; i++)
                    {
                        if(Type==HType.UINT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            S=uint8Ptr[i];
                        else if(Type==HType.UINT_4_TYPE)
                            S=uint4Ptr[i];
                        else if(Type==HType.UINT_2_TYPE)
                            S=uint2Ptr[i];
                        else if(Type==HType.INT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            S=int8Ptr[i];
                        else if(Type==HType.INT_4_TYPE)
                            S=int4Ptr[i];
                        else if(Type==HType.INT_2_TYPE)
                            S=int2Ptr[i];
                          j=HGem->GetAlarmIndex(S);

                        if(j>=0)
                        {
                            HGem->DataItemOut(3, HType.LIST_TYPE, NULL);
                            ALCD=atoi(HGem->strGrdAlarm->Cells[7][j].c_str())+0x80;
                            HGem->DataItemOut(1, HType.BINARY_TYPE, &ALCD);       // mode

                            ALID=_atoi64(HGem->strGrdAlarm->Cells[8][j].c_str());
                            HGem->DataItemOut(1, HType.INT_8_TYPE, &ALID);     // ID

                            ALTX=HGem->strGrdAlarm->Cells[6][j];           // message
                            HGem->DataItemOut(ALTX.Length(), HType.ASCII_TYPE, ALTX.c_str());
                        }
                        else
                        {
                            HGem->DataItemOut(3, HType.LIST_TYPE, NULL);
                            ALCD=0x80;
                            HGem->DataItemOut(0, HType.BINARY_TYPE, &ALCD);       // mode

                            ALID=42;
                            HGem->DataItemOut(1, HType.INT_8_TYPE, &ALID);     // ID

                            ALTX="Unknown Alarm Code";           // message
                            HGem->DataItemOut(ALTX.Length(), HType.ASCII_TYPE, ALTX.c_str());
                        }
                    }
                    HGem->SendLocalData();
                    if(Type==HType.UINT_8_TYPE) //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] uint8Ptr;  //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint8Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        delete[] uint4Ptr;  //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint4Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        delete[] uint2Ptr;  //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint2Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.INT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] int8Ptr;   //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int8Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        delete[] int4Ptr;   //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int4Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        delete[] int2Ptr;   //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int2Ptr=NULL;  //kevin 20180517
                    }

                    return;
                }
                else
                {
                    S9F7_IllegalData("S5,F5 Data Format error !!!");
                    if(Type==HType.UINT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] uint8Ptr;  //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint8Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        delete[] uint4Ptr;  //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint4Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        delete[] uint2Ptr;  //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint2Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.INT_8_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] int8Ptr;   //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int8Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        delete[] int4Ptr;   //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int4Ptr=NULL;  //kevin 20180517
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        delete[] int2Ptr;   //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int2Ptr=NULL;  //kevin 20180517
                    }
                    return;
                }
            }
            else
            {
                S9F7_IllegalData("S5,F5 Data Format error !!!");
            }
        }
        else
        {
            HGem->InitLocalHead(5, 6, 0);
            HGem->DataItemOut(HGem->strGrdAlarm->RowCount-1, HType.LIST_TYPE, NULL);
            for(int i=1; i<HGem->strGrdAlarm->RowCount; i++)
            {
                HGem->DataItemOut(3, HType.LIST_TYPE, NULL);
                ALCD=atoi(HGem->strGrdAlarm->Cells[7][i].c_str())+0x80;
                HGem->DataItemOut(1, HType.BINARY_TYPE, &ALCD);       // mode

                ALID=atoi(HGem->strGrdAlarm->Cells[8][i].c_str());
                HGem->DataItemOut(1, HType.UINT_4_TYPE, &ALID);     // ID

                ALTX=HGem->strGrdAlarm->Cells[6][i];           // message
                HGem->DataItemOut(ALTX.Length(), HType.ASCII_TYPE, ALTX.c_str());
            }
        }
    }

    HGem->SendLocalData();
    return;
}
//------------------------------------------------------------------------------
int HT172Gem::S7F2_ProcessProgramLoadGrant()
{
    unsigned char HCACK=1;
    int ret, len;
    unsigned char Type;
    AnsiString PPID;

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        ret=HGemPtr->GetDataItemLenAndType(len, Type);
        if(ret==1 && Type==HType.ASCII_TYPE)
        {
            HGemPtr->DataItemIn(len, Type, PPID);
            HGemPtr->GetDataItemLenAndTypeAndDelete(len, Type);
        }
    }

    TRegExpr * regex    =new TRegExpr;
    regex->Expression   ="[/|\\*|\\?|:|\"|>|<|\\|]";  //不等於裡面指定的文字
    regex->InputString=PPID;

    //Steven 20220422 : 加強S7F2_ProcessProgramLoadGrant的回覆
    if(HSys.Sys.SystemStart==true)       //20140124 wei
    {
        HCACK=6;
    }
    else if(PPID.Length()<=0)
    {
        HCACK=7;
    }
    else if(regex->Exec())
    {
        HCACK=8;
    }
    else if(HasICUnderMachine())
    {
        HCACK=9;
    }
    else
    {
        HCACK=0;
    }

    HGemPtr->InitLocalHead(7, 2, 0);
    HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
    HGemPtr->SendLocalData();
    return HCACK;
}
//-----------------------------------------------------------------------------
void HT172Gem::S7F4_ProcessProgramAcknowledge()                                 //Steven 20140911 : Modify S7F4 for HT9045
{
//L,2
//    1. <PPID>
//    2. <PPBODY>    int ret,len;
    unsigned char Type;
    char *PPBODY;
    AnsiString PPID, S, Str, PPIDFolder, str1, str2, strSetupfile, sDir;
    int ret,len;

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE ,NULL)==1)
    {
        ret=HGemPtr->GetDataItemLenAndType(len, Type);
        if(ret==1 && Type==HType.ASCII_TYPE)
        {
            HGemPtr->DataItemIn(len, Type, PPID);  // PPID=filename;
            ret=HGemPtr->GetDataItemLenAndType(len, Type);
            if(ret==1 && (Type==HType.BINARY_TYPE || Type==HType.ASCII_TYPE))
            {
                PPBODY=new char [len+1];    //JerryYang 20190411 因為有結束位元，長度+1

                if(HGemPtr->DataItemIn(len, Type, PPBODY))
                {
                    PPIDFolder.sprintf("D:\\HT172\\Data", PPID);

                    S.sprintf("%s\\%s.ini", PPIDFolder, PPID);

                    TStringList *TempPPBody;
                    TempPPBody= new TStringList();
                    TempPPBody->Clear();
                    TempPPBody->Text=PPBODY;
                    TempPPBody->SaveToFile(S);


                    AnsiString TempString,TempString3;
                    TStringList *TempString2;
                    TempString2= new TStringList();

                    Str="";
                    TempString=TempPPBody->Text;
                    AnsiString sTemp;
                    {
                        {
                            TempString2->Text=TempString;
                            int iSize=0;
                            char *buff;
                            AnsiString asInsterData1;
                            AnsiString asInsterData2;
                            TMemoryStream* pms = new TMemoryStream();
                            TMemoryStream* newpms = new TMemoryStream();
                            pms->Write((void*)TempString2->Text.c_str(), TempString2->Text.Length());
                            iSize=pms->Size;
                            pms->Position=0;
                            buff=new char[iSize+1];
                            pms->Read(buff,iSize);
                            newpms->Position=0;
                            asInsterData1="\r";
                            asInsterData2="\n";
                            for(int i=0;i<iSize;i++)
                            {
                                if(i+1<iSize && buff[i]=='\\' && buff[i+1]=='r')
                                {
                                    newpms->Write(asInsterData1.c_str(),asInsterData1.Length());
                                    i++;
                                }
                                else if(i+1<iSize && buff[i]=='\\' && buff[i+1]=='n')
                                {
                                    newpms->Write(asInsterData2.c_str(),asInsterData2.Length());
                                    i++;
                                }
                                else
                                {
                                    newpms->Write(buff+i,1);
                                }

                            }
                            TempString2->Text=AnsiString((char*)newpms->Memory).SubString(1, newpms->Size);
                            TempString2->SaveToFile(S);

                            delete pms;
                            delete newpms;
                            delete[] buff;  //ChungHung 20131231 alter
                            buff=NULL;  //kevin 20180517
                            TempString2->Clear();

                        }
                    }

                    LookForFile();
                    TempString2->Clear();   //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                    delete TempString2;
                    TempString2 =NULL;

                    TempPPBody->Clear();   //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                    delete TempPPBody;
                    TempPPBody =NULL;
                }
                delete[] PPBODY;        //Steven 20160912 : Add delete for save memory
                PPBODY=NULL;  //kevin 20180517


                HGemPtr->LocalAcknowledge(7, 4, 0);      //Steven 20211013 : 換位置
                return;
            }
        }
    }
    S9F7_IllegalData("S7,F3 Data Format error !!!");
}
//------------------------------------------------------------------------------
void HT172Gem::S7F6_ProcessProgramData(AnsiString FileName)                     //Steven 20210510 : S7F6上傳工作檔
{
    AnsiString S, Str, str1, str2;
    char PPID[1024];
    sprintf(PPID, "%s", FileName.c_str());
    S.sprintf("D:\\HT172\\data\\%s.ini", PPID);

    if(FileExists(S)==false)
    {
        S9F7_IllegalData("S7,F5 PPID Not Exists!!!");
        return;
    }
    HGemPtr->memoPPBody->Clear();
    TStringList *TempString;
    TempString= new TStringList();
    AnsiString sBuffer="", sBuffer1="";     //kevin 20180823 add ASE KH

    TempString->Clear();
    TempString->LoadFromFile(S);

    HGemPtr->memoPPBody->Lines->Add(TempString->Text);
    HGemPtr->memoPPBody->Lines->Add("");

    Str=HGemPtr->memoPPBody->Lines->Text;
    Str=StringReplace(Str, "\r\n", "\\r\\n", TReplaceFlags()<<rfReplaceAll);
    HGemPtr->InitLocalHead(7, 6, 0);
    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(strlen(PPID), HType.ASCII_TYPE, PPID);
    HGemPtr->DataItemOut(Str.Length(), HType.ASCII_TYPE, Str.c_str());
    HGemPtr->SendLocalData();

    LookForFile();
    TempString->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete TempString;
    return;
}
//------------------------------------------------------------------------------
void HT172Gem::S7F6_ProcessProgramData()                     //Steven 20210510 : S7F6上傳工作檔
{
//    unsigned char Type=HType.ASCII_TYPE;
    AnsiString S, Str, str1, str2;
    char PPID[1024];
    unsigned char Type;
    int ret, len;


    ret=HGemPtr->GetDataItemLenAndType(len, Type);
    if(ret==1 && Type==HType.ASCII_TYPE)
    {
        HGemPtr->DataItemIn(1024, Type, PPID);  // PPID=filename;

        S.sprintf("D:\\HT172\\data\\%s.ini", PPID);

        if(FileExists(S)==false)
        {
            S9F7_IllegalData("S7,F5 PPID Not Exists!!!");
            return;
        }

        HGemPtr->memoPPBody->Clear();
        TStringList *TempString;
        TempString= new TStringList();
        AnsiString sBuffer="", sBuffer1="";     //kevin 20180823 add ASE KH



        TempString->Clear();
        TempString->LoadFromFile(S);

        HGemPtr->memoPPBody->Lines->Add(TempString->Text);
        HGemPtr->memoPPBody->Lines->Add("");


        Str=HGemPtr->memoPPBody->Lines->Text;
        Str=StringReplace(Str, "\r\n", "\\r\\n", TReplaceFlags()<<rfReplaceAll);
        HGemPtr->InitLocalHead(7, 6, 0);
        HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
        HGemPtr->DataItemOut(strlen(PPID), HType.ASCII_TYPE, PPID);
        HGemPtr->DataItemOut(Str.Length(), HType.ASCII_TYPE, Str.c_str());
        HGemPtr->SendLocalData();


        LookForFile();
        TempString->Clear();    //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete TempString;
    }
}
//------------------------------------------------------------------------------
void HT172Gem::ProcessS14F1_GetAttrRequest(AnsiString asTrayID)                 //Jou 20240620 : Add S14F1
{
    AnsiString asString="";
    HGemPtr->InitLocalHead(14,1,0);
    HGemPtr->DataItemOut(5, HType.LIST_TYPE, NULL);

    HGemPtr->DataItemOut(asString.Length(), HType.ASCII_TYPE, asString.c_str());

    asString="LOT_ID";
    HGemPtr->DataItemOut(asString.Length(), HType.ASCII_TYPE, asString.c_str());

    HGemPtr->DataItemOut(1, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(fMain->edLotNo->Text.Length(), HType.ASCII_TYPE, fMain->edLotNo->Text.c_str());

    HGemPtr->DataItemOut(1, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);

    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
    asString="LotSum";
    HGemPtr->DataItemOut(asString.Length(), HType.ASCII_TYPE, asString.c_str());
    asString="TrayMap";
    HGemPtr->DataItemOut(asString.Length(), HType.ASCII_TYPE, asString.c_str());

    HGemPtr->SendLocalData();
}
//------------------------------------------------------------------------------
unsigned char HT172Gem::ProcessS14F2_GetAttrData()                              //Jou 20240620 : Add S14F2
{
    //接收XML
    char str[256];
    int len=0;
    unsigned char Type,Command;
    unsigned char HCACK=0;
    AnsiString asStr, S2, sFile, str2;
    ZeroMemory(str, sizeof(str));
    char *CommandStr;
    CommandStr=new char [10000000];

    if(HasICUnderMachine()==true || HSys.Sys.SystemStart==true)
    {
        HCACK=1;
    }
    else
    {
        if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
        {
            if(HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL)==1)
            {
                if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                {
                    HGemPtr->GetDataItemLenAndType(len, Type);
                    if(Type==HType.ASCII_TYPE)
                    {
                        HGemPtr->DataItemIn(len, HType.ASCII_TYPE, str);
                        asStr=str;
                        if(asStr!=fMain->edLotNo->Text)
                        {
                            //Lot ID比對失敗
                            HCACK=5;
                        }
                    }

                    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                    {
                        if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                        {
                            HGemPtr->DataItemIn(len, HType.ASCII_TYPE, str);
                            asStr=str;
                            if(asStr=="LOT_SUM")
                            {
                                HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                if(Type==HType.ASCII_TYPE)
                                {
                                    HGem->DataItemIn(len,Type, S2); //取值
                                }
                                fXML->XMLDoc->LoadFromXML(S2);
                                sFile.sprintf("D:\\BarcodeSorter\\%s.xml", fMain->edLotNo->Text);
                                fXML->XMLDoc->SaveToFile(sFile);


                            }
                            else
                            {
                                HCACK=6;
                            }
                        }
                        if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
                        {
                            HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                            if(Type==HType.ASCII_TYPE)
                            {
                                HGemPtr->DataItemIn(len, HType.ASCII_TYPE, str);
                                asStr=str;

                                    HGem->GetDataItemLenAndType(len,Type);//取得資料長度與格式
                                if(Type==HType.ASCII_TYPE)
                                {
                                    HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
                                    if(asStr=="TRAY_MAP_PATH")
                                    {
                                        asNetDisk=CommandStr;
                                        DecodeTrayMapByWinstekFormat(fMain->edLotNo->Text);

                                        fMain->Memo1->Lines->Clear();
                                        AnsiString str;


                                        for(IterTrayMapList=mapTrayMapList.begin(); IterTrayMapList!=mapTrayMapList.end(); IterTrayMapList++)
                                        {
                                            fMain->Memo1->Lines->Add(IterTrayMapList->second.TRAY_ID);

                                            for(int R=0; R<20; R++)
                                            {
                                                str="";
                                                for(int C=0; C<20; C++)
                                                {
                                                    str=str+AnsiString(IterTrayMapList->second.DevInfo[R][C].iBin)+AnsiString(" ");
                                                }
                                                fMain->Memo1->Lines->Add(str);
                                            }
                                            fMain->Memo1->Lines->Add("//-----------------------------------------\n");
                                        }
                                    }
                                    else
                                    {
                                        HCACK=7;
                                    }
                                }
                                else
                                {
                                    HCACK=6;
                                }
                            }
                            else
                            {
                                HCACK=6;
                            }                            
                        }
                    }
                }
                else
                {
                    HCACK=4;
                }
            }
            else
            {
                HCACK=3;
            }

            if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
            {
                HGemPtr->DataItemIn(1, HType.UINT_1_TYPE,&Command);
                HGemPtr->DataItemIn(1, HType.LIST_TYPE, NULL);
            }
            else
            {
                HCACK=3;
            }
        }
        else
        {
            HCACK=2;
        }
    }

    delete[] CommandStr;
    CommandStr=NULL;

    return HCACK;
}
//------------------------------------------------------------------------------
