#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <Filectrl.hpp>
#include <inifiles.hpp>
#include <fcntl.h>
#include <assert.h>
#include <io.h>
#include <string.h>
#include <mmsystem.h>
#include <mem.h>
#include <map>
#include <algorithm>
#include <windows.h>
#include <vector>
#include <IniFiles.hpp>
#include "SPComm.hpp"
#include <math.h>
#include <winbase.h>

#include <dos.h>
#include <dir.h>
#include <fcntl.h>

using namespace std;
//---------------------------------------------------------------------------
#define MOTOR_MAX_WAIT 1000

//---------------------------------------------------------------------------
template<typename T>
inline void vec_clr(vector<T> & v)
{
    vector<T> t;
    v.swap(t);
}
//---------------------------------------------------------------------------
enum eTimeData
{
    eEjection2PAlignTimer       =1,
    eEjection4PAlignTimer       =2,
    eEjectionSortingTimer1      =3,
    eEjectionSortingTimer2      =4,
    eEjectionSortingTimer3      =5,
    eEjectionSortingTimer4      =6,
    eFixWaferTime               =7,
    eReleaseWaferTime           =8,
    eDoMoveToPickTimer1         =9,
    eDoMoveToPickTimer2         =10,
    eDoMoveToPickTimer3         =11,
    eDoMoveToPickTimer4         =12,
    eDoMoveToSortTimer1         =13,
    eDoMoveToSortTimer2         =14,
    eDoMoveToSortTimer3         =15,
    eDoMoveToSortTimer4         =16,
    eDoMoveToSortTimer5         =17,
    eDoMoveToSortTimer6         =18,
    eDoMoveToSortTimer7         =19,
    eDoMoveToSortTimer8         =20,
    eDoMoveToSortTimer9         =21,
    eDoMoveDownToSortTimer      =22,
    eCycleTime                  =23,
    eDownCCDGrabTimer           =24,                                            //Sam 20170922 : Down CCD 停拍拍照時間紀錄
    eDownCCDAnalysisTimer       =25,                                            //Sam 20170921 : Down CCD 停拍分析時間紀錄
    eTimeDataTotal
};


enum eLoaderNo{eLoader1=0, eEmpty1=1, eEmpty2=2, eLoaderTotal};
enum eTubeNo{eTube1=0, eTube2=1, eTubeTotal};
enum eTrayArea{eTrayStart=0, eTrayMiddle=1, eTrayEnd=2, eTrayTotal};

enum eUserLanguage{eulEnglish=0,    //純英文版
                   eulChinese=1,    //繁體中文
                   eulChina=2,      //簡體中文
                   eulKorea=3,      //韓文版
                   eulSingapore=4,  //新加坡版
                   eulJapan};       //日文版

enum eSuckType {eSuckTypePtoV=0,      //真空產生器
                eSuckTypeVtoV=1,      //真空切換閥
                eSuckTypeTotal
               };
// Alarm原始定義：
// 0:mess_jam  1:mess_err  2:mess_fuc  3:sensor detect  4:cylinder error
// 5: motor error  6:suck error  7:Magazine error  8:shuttle error
// 9: user motion system sensor

enum eAlarmType{eJamErr         =0,
                eMessageErr     =1,
                eFunErr         =2,
                eSystemMess     =3,
                eCynAlarm       =4,
                eMotorAlarm     =5,
                eSuckAlarm      =6,
                eRecordProcess  =7,
                eOther          =8,
                eAlatmTypeTotal
               };

enum eSystemTime{stStartTime=0,
                 stPauseTime=1,
                 stPowerOn=2,
                 stProductTime=3,
                 stJamTime=4,
                 stContactTest=5,
                 stHomeTime=6,
                 stMTBA=7,
                 stTotalCnt};
enum eProductInfo
{
    eEmpty=0,
    eLotStart,
    eLotEnd,
    eAlarmTime,
    ePauseTime,
    eUPH
};

enum eBinDispName                                                               //JerryYang 20220909 : //使用的Bin顯示器名稱
{   eBinDispStore1  =0,
    eBinDispStore2  =1,                                                         //20240430 Bin test
    eBinDispStore3  =2,
    eBinDispStore4  =3,
    eBinDispStore5  =4,
    eBinDispStore6  =5,
    eBinDispStore7  =6,
    eBinDispStore8  =7,
    eBinDispStore9  =8,
    eBinDispStore10 =9,
    eBinDispStore11 =10,
    eBinDispStore12 =11,
    eBinDispStore13 =12,
    eBinDispStore14 =13,
    eBinDispStore15 =14,
    eBinDispStore16 =15,
    eBinDispStore17 =16,
    eBinDispStore18 =17,
    eBinDispStore19 =18,
    eBinDispStore20 =19,
    eBinDispTotal
};

enum eBinDispMagName                                                            //JerryYang 20220909 : //使用的Bin顯示器名稱
{
    eBinDispMag1    =0,
    eBinDispMag2    =1,
    eBinDispMag3    =2,
    eBinDispMagTotal
};
enum ePushTrayToTrayArmType       //JerryYang 20220909 : //使用的Bin顯示器名稱
{
    ePushByConveyor    =0,
    ePushByMotor       =1,
    ePushTypeTotal
};
//#define MAX_BIN_UNIT eBinDispTotal                                              //20240430 Bin test
//#define MAX_BIN_UNIT 1                                                          //20240430 Bin test

//#include "PComm.h"
//#include "CMNETError.h"
#include "uHGemEquipment.h"
#include "uHGemForm.h"
#include "MachineType.h"
#include "HMotor.h"
#include "CSmc.h"
#include "language.h"
#include "myio.h"
#include "mn200.h"
#include "myio_MN200.h"
#include "mySMCmotor.h"
#include "myMN200motor.h"
#include "HSensor.h"
#include "halarm.h"
#include "HTimer.h"
#include "myTimer.h"
#include "HTray.h"
#include "MyMotor.h"
#include "mysensor.h"
#include "myswitch.h"
#include "MyKitSuck.h"
#include "mytray.h"
#include "mycylin.h"
#include "HTMotor.h"
#include "handlerlog.h"
#include "uruncontrol.h"
#include "cinitial.h"
#include "cpublic.h"
#include "setup.h"
#include "data.h"
#include "iosetview.h"
#include "database.h"
#include "maintenance.h"
#include "mymessbox.h"
#include "uhome.h"
#include "uteach.h"
#include "Password.h"
#include "note.h"
#include "ckernel.h"
#include "myQwertyKeyBoard.h"
#include "csystem.h"
#include "uOffset.h"
#include "uspeed.h"
#include "systools.h"
#include "cmydef.h"
#include "cprod.h"
#include "LevelSetup.h"
#include "uMotorTest.h"
#include "aled.hpp"
#include "main.h"
#include "aSortArm.h"
#include "aTrayArm.h"
#include "aMagArm.h"

#include "aLoader.h"
#include "Barcode.h"
#include "uTrayEditForm.h"
#include "UsecegemMainFrom.h"
#include "uHGemClass.h"
#include "common.h"

#include "uHGemHT172.h"
#include "aMagazine.h"

#include "MyBinDisp.h"
#include "ComPort.h"
#include "MyStringList.h"
#include "RegExpr.hpp"
#include "ModbusTCPClient.h"
#include "MyPLC_IO_Modbus.h"

//---------------------------------------------------------------------------

