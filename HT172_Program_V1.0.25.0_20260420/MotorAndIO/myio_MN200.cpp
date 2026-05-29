#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "..\Include\mn200.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

const int MAXRing   =2;
const int MAXIP     =64;
const int MAXPort   =4;
const int MAXBit    =8;

bool bOpenMN200Card=false;

byte OutPortData[MAXRing][MAXIP][MAXPort];
bool bModuleInstall[MAXRing][MAXIP];
DWORD MN_200_ErrorTable[2];

enum eMNetCardType{eMNetUninstall=0, eMNetPcil112=1, eMNetPcil122, eMNetMN200};      //Steven 20111004 : 先達卡總類
bool bResetMNet=false;  //jou 2012-01-10 避免24V還沒開啟就在重置先達模組
eMNetCardType iMNetCardType=eMNetUninstall;
bool bOpenCard=false;
//------------------------------------------------------------------------------
 _fastcall TMyMN200_IO::TMyMN200_IO()
{
    if(bOpenCard==false)
        OpenPCI132Card();
    bOpenCard=true;
}
//==============================================================================
byte TMyMN200_IO::IOInputByte()
{
    if((iLane<0) || (iLane>=MAXRing) || (iIP<0) || (iIP>=MAXIP) || (iPort<0) || (iPort>=MAXPort))
        return false;

    return DI_Process(true);
}
//==============================================================================
void TMyMN200_IO::IOByteOut(byte Byte)
{
    if((iLane<0) || (iLane>=MAXRing) || (iIP<0) || (iIP>=MAXIP) || (iPort<0) || (iPort>=MAXPort))
        return;

    OutPortData[iLane][iIP][iPort]=Byte;
    DO_Process(OutPortData[iLane][iIP][iPort]);
}
//==============================================================================
void TMyMN200_IO::SetPortInformation(int Line, AnsiString IP, int Port, int Bit)
{
    iLane=Line;
    if(IP.AnsiCompare("A")>=0 && IP.AnsiCompare("Z")<=0)
    {
        char *QQ=IP.c_str();
        iIP=int(QQ[0]-'A')+10;
    }
    else if(IP=="")
    {
        iIP=-1;
    }
    else //if(IP.AnsiCompare("0")>=0 && IP.AnsiCompare("9")<=0)
    {
        iIP=atoi(IP.c_str());
    }
    iPort=Port;
    iBit=Bit;


}
//==============================================================================
void TMyMN200_IO::SetPortInformation(AnsiString Card, AnsiString Port, AnsiString Bit)
{
    if(Card.Length()==1 || Card=="")
    {
        iLane=0;
        iIP  =atoi(Card.c_str());
    }
    else if(Card.Length()==2)
    {
        iLane=0;
        AnsiString str=Card.SubString(2, 1).c_str();
        if(str.AnsiCompare("A")>=0 && str.AnsiCompare("Z")<=0)
        {
            char *QQ=str.c_str();
            iIP=int(QQ[0]-'A')+10;
        }
        else //if(str.AnsiCompare("0")>=0 && str.AnsiCompare("9")<=0)
        {
            iIP=atoi(str.c_str());
        }
    }
    else
    {
        iLane=atoi(Card.SubString(1, 1).c_str());
        AnsiString str=Card.SubString(3, 1).c_str();
        if(str.AnsiCompare("A")>=0 && str.AnsiCompare("Z")<=0)
        {
            char *QQ=str.c_str();
            iIP=int(QQ[0]-'A')+10;
        }
        else //if(str.AnsiCompare("0")>=0 && str.AnsiCompare("9")<=0)
        {
            iIP=atoi(str.c_str());
        }
    }

    iCard   =iLane*100+iIP;
    iPort   =atoi(Port.c_str());
    iBit    =atoi(Bit.c_str());
}
//==============================================================================
void TMyMN200_IO::SetPortInformation(int Card, int Port, int Bit)
{
    iCard   =Card;
    iLane   =iCard/100;
    iIP     =iCard%100;
    iPort   =Port;
    iBit    =Bit;
}
//==============================================================================
bool TMyMN200_IO::IsOn()                                                       // check input on //
{
    byte b;
    if(ISABase!=e_PLCbase)
    {
        if((iLane<0) || (iLane>=MAXRing) || (iIP<0) || (iIP>=MAXIP) || (iPort<0) || (iPort>=MAXPort) || (iBit<0) || (iBit>=MAXBit))
            return false;
    }

    b=DI_Process(false);
//    if(b&MyBitMask[iBit])
        return b;
//    else
//        return false;
}
//==============================================================================
bool TMyMN200_IO::IsOff()                                                      // check input off //
{
    return !IsOn();
}
//==============================================================================
void TMyMN200_IO::On()                                                         // do on //
{
    AnsiString str;
    if((iLane<0) || (iLane>=MAXRing) || (iIP<0) || (iIP>=MAXIP) || (iPort<0) || (iPort>=MAXPort) || (iBit<0) || (iBit>=MAXBit))
        return;

    OutPortData[iLane][iIP][iPort]|=MyBitMask[iBit];
//    DO_Process(OutPortData[iLane][iIP][iPort]);

    int ret = mn_set_port_bit(iLane, iIP, iPort , iBit, 1);
    if(ret<0)
    {
        #ifdef SOFT_SIMULATE
            return;
        #else
            str.sprintf("IOByteOut Ring %d, IP %d, Port %d, Byte %d Fail, Code: %d", iLane, iIP, iPort, 0, ret);
            MNetLog(str);   //Steven 20110406
            return;
        #endif
    }
}
//==============================================================================
void TMyMN200_IO::Off()                                                        // do off //
{
    AnsiString str;
    if((iLane<0) || (iLane>=MAXRing) || (iIP<0) || (iIP>=MAXIP) || (iPort<0) || (iPort>=MAXPort) || (iBit<0) || (iBit>=MAXBit))
        return;

    OutPortData[iLane][iIP][iPort]&=~MyBitMask[iBit];
//    DO_Process(OutPortData[iLane][iIP][iPort]);
    int ret = mn_set_port_bit(iLane, iIP, iPort , iBit, 0);
    if(ret<0)
    {
        #ifdef SOFT_SIMULATE
            return;
        #else
            str.sprintf("IOByteOut Ring %d, IP %d, Port %d, Byte %d Fail, Code: %d", iLane, iIP, iPort, 1, ret);
            MNetLog(str);                                                       //Steven 20110406
            return;
        #endif
    }
}
//==============================================================================
void TMyMN200_IO::DO_Process(Byte value)                             // output //
{
    int ret;
    AnsiString str;
    OutPortData[iLane][iIP][iPort]=value;
    ret=mn_set_port_byte(iLane, iIP, iPort, OutPortData[iLane][iIP][iPort]);

    if(ret<0)
    {
        #ifdef SOFT_SIMULATE
            return;
        #else
            str.sprintf("IOByteOut Ring %d, IP %d, Port %d, Byte %d Fail, Code: %d", iLane, iIP, iPort, value, ret);
            MNetLog(str);   //Steven 20110406
            return;
        #endif
    }
}
//==============================================================================
Byte TMyMN200_IO::DI_Process(bool bByte)                                         // input //
{
    int ret;
    unsigned char Value=0;
    unsigned char pData=0;
    AnsiString str;

    //Austin 20190218.01 增加PLC IO for CE
    //==>
    if(bPLCIO[iPort][iBit] && ISABase==e_PLCbase)                                             //Jason 20230619 增加安全PLC部分
    {
        if(iPort>=PLCIO_INPUT_START_PORT_NUMBER && iPort<=0x304)
        {
            if(iPLCSafetyVer==202204)
            {
                return bPLCInData[0][iPort-PLCIO_INPUT_START_PORT_NUMBER+2][iBit];//joe 20220406.01 debug safe plc
            }
            else
            {
//                if(Port==0x302 || Port==0x303)                                  //大小門一個點位
//                {
//                    if(Port==0x302 && Bit==6)
//                    {
//                        return bPLCInData[0][Port-PLCIO_INPUT_START_PORT_NUMBER+1][Bit]; //ben 20230822 index 大小門獨立偵測
//                    }
//                    else
//                    {
//                        return (bPLCInData[0][Port-PLCIO_INPUT_START_PORT_NUMBER+1][Bit] && bPLCInData[0][Port-PLCIO_INPUT_START_PORT_NUMBER+3][Bit]);
//                    }
//                }
//                else
//                {
                    return bPLCInData[0][iPort-PLCIO_INPUT_START_PORT_NUMBER+1][iBit];//第1個Byte為系統狀態 預留
//                }
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        if(bByte)
            ret=mn_get_di_byte(iLane, iIP, iPort, &pData);
        else
            ret=mn_get_port_bit(iLane, iIP, iPort, iBit, &Value);
    }

    if(ret<0)
    {
        #ifdef SOFT_SIMULATE
            return 1;
        #else
            str.sprintf("IOInputByte Ring %d, IP %d, Port %d Fail, Code: %d", iLane, iIP, iPort, ret);
            MNetLog(str);   //Steven 20110406

            return 0;
        #endif
    }
    else
    {
//        Value=byte(ret);
    }

    if(bByte)
        return pData;
    else
        return Value;
}
//------------------------------------------------------------------------------
bool bCheckPCI_L112StateRun=false;
// 0: running
// 1: OK
// 2: NG
//------------------------------------------------------------------------------
//DEll 2012-12-07 Start
//bool GetMN200_Error_Code(int iRing ,int iCode ,AnsiString *ChStr ,AnsiString *EngStr)
//{
//    bool bResult = true;
//
//    switch(iCode)
//    {
//        case ERROR_NO_CARD_FOUND:
//            EngStr->sprintf("MNet Ring %d ERROR_NO_CARD_FOUND!!!" ,iRing);
//            ChStr->sprintf("MNet系統 Ring %d IO 模組錯誤" ,iRing);
//        break;
//
//        case ERROR_INVALID_LINE_NO:
//            EngStr->sprintf("MNet Ring %d ERROR_INVALID_LINE_NO!!!" ,iRing);
//            ChStr->sprintf("MNet系統 Ring %d 對應不到系統IO 模組錯誤" ,iRing);
//        break;
//
//        case ERROR_COMM_NOT_START:
//            EngStr->sprintf("MNet Ring %d ERROR_COMM_NOT_START!!!" ,iRing);
//            ChStr->sprintf("MNet系統 Ring %d IO 模組錯誤 ,沒有開始  Motionnet 通訊傳輸。" ,iRing);
//        break;
//
//        case ERROR_INVALID_DEV_NO:
//            EngStr->sprintf("MNet Ring %d ERROR_INVALID_DEV_NO!!!" ,iRing);
//            ChStr->sprintf("MNet系統 Ring %d IO 模組錯誤 ,裝置編號，超出設定範圍值: 0~63。" ,iRing);
//        break;
//
//        case ERROR_NO_DEV_FOUND:
//            EngStr->sprintf("MNet Ring %d ERROR_NO_DEV_FOUND!!!" ,iRing);
//            ChStr->sprintf("MNet系統 Ring %d IO 模組錯誤 ,通訊線上找不到可以使用的裝置。" ,iRing);
//        break;
//
//        case ERROR_INVALID_COMM_SPEED:
//            EngStr->sprintf("MNet Ring %d ERROR_INVALID_COMM_SPEED!!!" ,iRing);
//            ChStr->sprintf("MNet系統 Ring %d IO 模組錯誤 ,傳輸速度的設定值並不在指定的範圍" ,iRing);
//        break;
//
//        case ERROR_COMM_DISCONNECT:
//            EngStr->sprintf("MNet Ring %d ERROR_COMM_DISCONNECT!!!" ,iRing);
//            ChStr->sprintf("MNet系統 Ring %d IO 模組錯誤 ,Motionnet 通訊傳輸中斷" ,iRing);
//        break;
//
//        default :
//            bResult =false;
//        break;
//
//    }
//
//    return bResult;
//}
//------------------------------------------------------------------------------
//DEll 2012-12-07 Start
bool MN200_Line_status_OK(int iRing ,WORD pD ,AnsiString *ChStr ,AnsiString *EngStr)
{
    bool bResult =true;

    if (pD & 0x08)      //bit3
    {
        EngStr->sprintf("MNet Ring %d ERROR_COMM!!!" ,iRing);
        ChStr->sprintf("MNet系統 Ring %d I/O裝置通訊異常發生 " ,iRing);
        bResult = false;
    }
    else if (pD & 0x20)//bit5
    {
        EngStr->sprintf("MNet Ring %d ERROR_IO!!!" ,iRing);
        ChStr->sprintf("MNet系統 Ring %d I/O裝置操作異常發生 " ,iRing);
        bResult =false;
    }

    return bResult;
}
//------------------------------------------------------------------------------
//DEll 2012-12-07 Start
//0 : Running
//1 : OK
//2 : NG
bool bCheckPCI_MN200StateRun=false;
int CheckPCI_MN200State()
{
    if(InitialOK==false)
        return 0;

    //jou 2011-12-16 沒歸零完成不檢查CheckPCI_L112State
    //jou 2011-12-25 中途修機沒辦法關24V時,沒辦法重置先達卡
//    if(fAllMotorHome==false)
//    {
//        return 0;
//    }

    if(bCheckPCI_L112StateRun==true)
    {
        return 0;
    }
    bCheckPCI_L112StateRun=true;        //MN200盜用112 參數

    #ifdef SOFT_SIMULATE
    {
        bCheckPCI_L112StateRun=false;
        return 1;
    }
    #else
    {
        int iError=0;
        static int iErrorTime=0;
        static int iErrorTime_0=0;
        static int iErrorTime_1=0;
        static int iWriteErrorLogCT=0;

        AnsiString Str1, Str2;
        int iRet  ,iRing;//,iIO_Number[2];
        WORD pData ,cWord;
        //BYTE pDataByte;
        bool bError = false;
        int MotionnetLineMax =2;

        for (int i =0; i < MotionnetLineMax ;i++)
        {
            iRing = i;
            iError =iRing;

            iRet = mn_get_line_status(iRing, &pData);

            if (iRet == SUCCESS)
            {
                if (MN200_Line_status_OK(iRing ,pData ,&Str1 ,&Str2) ==false)
                {
                    bError =true;
                    (iRing == 0)? iErrorTime_0++ : iErrorTime_1++;
                }
            }
            else
            {
                bError =true;
                (iRing == 0)? iErrorTime_0++ : iErrorTime_1++;
                GetMN200_Error_Code( iRing ,iRet ,&Str1 ,&Str2);
                break;
            }

            iRet = mn_get_slave_error_table(iRing ,MN_200_ErrorTable);

            if (iRet == SUCCESS)
            {

                if ((MN_200_ErrorTable[0] > 0) || (MN_200_ErrorTable[1] >0))
                {
                    for(int j=0; j<32; j++)
                    {
                        cWord = 0x01<<j;
                        if(MN_200_ErrorTable[0] & cWord)  //發生錯誤的Device
                        {
                            bError =true;
                            Str1.sprintf("MNet Ring %d IP %d IO Device Error!! ", i, j);
                            Str2.sprintf("MNet系統 Ring %d IP %d IO 模組錯誤", i);
                            break;
                        }
                        if(MN_200_ErrorTable[1] & cWord)  //發生錯誤的Device
                        {
                            bError =true;
                            Str1.sprintf("MNet Ring %d IP %d IO Device Error!! ", i, (j+32));
                            Str2.sprintf("MNet系統 Ring %d IP %d IO 模組錯誤", (j+32));
                            break;
                        }
                    }
                }
            }
            else
            {
                bError =true;
                (iRing == 0)? iErrorTime_0++ : iErrorTime_1++;
                GetMN200_Error_Code( iRing ,iRet ,&Str1 ,&Str2);
                break;
            }
        }

        if (bError)
        {
            if (iRing == 0)
                iErrorTime = iErrorTime_0;
            else
                iErrorTime = iErrorTime_1;

            if(iErrorTime >5)
            {
                ResetMNet(iError, Str1, Str2,true); //重置MNet系統
                fAllMotorHome=false;     //強制要求歸零
                iErrorTime=0;
                iErrorTime_0 =0;
                iErrorTime_1 =0;
            }
            else
            {
            }
            bCheckPCI_L112StateRun=false;
            return 2;
        }
        else
        {
            ResetMNet(iError, Str1, Str2,false); //重置MNet系統
            iErrorTime=0;
            iErrorTime_0 =0;
            iErrorTime_1 =0;
        }

        bCheckPCI_L112StateRun=false;
        iWriteErrorLogCT=0;
        return 1;
    }
    #endif
}
//==============================================================================
void ResetMNet(int iRingNo, AnsiString EngMessage, AnsiString ChtMessage, bool bShowMess)
{
    if(bShowMess)   //Steven 20110527
    {
        ShowMyMessage(EngMessage);
        bResetMNet=true;
    }
    else
    {
        if(bResetMNet==true)
        {
            bOpenMN200Card=false;
            OpenPCI132Card();
            AllBreakLock();
            HSys.Sw.SwMotorRelay.Off();
            Sleep(1000);
            HSys.Sw.SwMotorRelay.On();
//            AllBreakFree();
            bResetMNet=false;
        }
    }
}
//==============================================================================
void OpenPCI132Card()
{
#ifdef SOFT_SIMULATE
    return;
#else
    BYTE NumLine;
    AnsiString Str;
    short nErrCode;
    BYTE p_Data1;

    if(bOpenMN200Card==false)                                                   //只能開一次卡
    {
        if((nErrCode=mn_open_all(&NumLine))==SUCCESS)                         //開卡成功
        {
            byte NumDev;
            for(int i=0; i<NumLine; i++)
            {
                nErrCode=mn_stop_line(i);                                       //Steven 20170519 (Jou) : add MN_STOP_LINE
                nErrCode=mn_set_comm_speed(i, COMMSPEED_10M);
                nErrCode=mn_reset(i);
                nErrCode=mn_start_line(i, &NumDev);

                for(byte k=0 ; k<MAXIP; k++)
                {
                    nErrCode=mn_get_dev_info(i, k, &p_Data1);
                    if(nErrCode==0 && p_Data1!=0)                               //讀取成功(==0) && 有TYPE資料
                    {
                        if(p_Data1==DEV_INF_MOTION_DEV)                         //Steven 20221005 : 嘗試修正速度設定異常
                        {
                            mn_set_customization(i, k, CUST_REPLACE_SPEED_PAR, true);
                            mn_set_customization(i, k, CUST_FIXED_MAX_SPEED, MAXSPEED_100K);
                        }
                    }
                }

                if(nErrCode<0)
                {
                    Str.sprintf("MN200 Ring %d set_comm_speed fail (Error Code : %d)!! Program closed.", i, nErrCode);
                    Application->MessageBox("Close Program", Str.c_str(), MB_OK);
                    FormSysTools->TerminalProgram();
                }

                nErrCode =mn_reset(i);
                if(nErrCode<0)
                {
                    Str.sprintf("MN200 Ring %d reset fail (Error Code : %d)!! Program closed.", i, nErrCode);
                    Application->MessageBox("Close Program", Str.c_str(), MB_OK);
                    FormSysTools->TerminalProgram();
                }

                Sleep(20);
                nErrCode = mn_start_line(i, &NumDev);
                if(nErrCode<0)
                {
                    Str.sprintf("MN200 Ring %d start_line fail (Error Code : %d)!! Program closed.", i, nErrCode);
                    Application->MessageBox("Close Program", Str.c_str(), MB_OK);
                    FormSysTools->TerminalProgram();
                }

                nErrCode = mn_get_slave_error_table(i ,MN_200_ErrorTable);
                if (nErrCode < 0 )
                {
                    Str.sprintf("MN200 get_slave_error_table Ring %d fail (Error Code : %d)!! Program closed.", i, nErrCode);
                    Application->MessageBox("Close Program", Str.c_str(), MB_OK);
                    FormSysTools->TerminalProgram();
                }

                nErrCode = mn_clear_slave_error_flag(i ,MN_200_ErrorTable);
                if (nErrCode < 0 )
                {
                    Str.sprintf("MN200 clear_slave_error_flag Ring %d fail (Error Code : %d)!! Program closed.", i, nErrCode);
                    Application->MessageBox("Close Program", Str.c_str(), MB_OK);
                    FormSysTools->TerminalProgram();
                }

                if(tMotionnetIO.bInitialScan==false)                            //2012-12-08    Dell  開機的時候要先Scan Motionnet IO 數量
                {
                    tMotionnetIO.IO_Dev_Num[i] = NumDev;
                }
            }
            tMotionnetIO.bInitialScan=true;
        }
        else
        {
            Str.sprintf("MN200 mn_open_all fail (Error Code : %d)!! Program closed.", nErrCode);
            Application->MessageBox("Close Program", Str.c_str(), MB_OK);
            return;
        }
        bOpenMN200Card=true;
    }
#endif
}
//------------------------------------------------------------------------------
void MNetLog(AnsiString Str)
{
    AnsiString sFile="", sTimeData="";

    sFile.sprintf("%s\\MNetLog_%04d-%02d-%02d_%02d%02d%02d.txt", sMNetLogPath, SystemYear, SystemMonth, SystemDate,SystemHour,SystemMin,SystemSec);
    sTimeData.sprintf("%04d-%02d-%02d_%02d:%02d:%02d_%s", SystemYear, SystemMonth, SystemDate,SystemHour,SystemMin,SystemSec, Str);
    if(fMain!=NULL)
    {
        if(fMain->memoIOStatus->Lines->Count>4096)
        {
            fMain->memoIOStatus->Lines->SaveToFile(sFile);
            fMain->memoIOStatus->Clear();
        }

        fMain->memoIOStatus->Lines->Add(sTimeData);
    }
}
//------------------------------------------------------------------------------
