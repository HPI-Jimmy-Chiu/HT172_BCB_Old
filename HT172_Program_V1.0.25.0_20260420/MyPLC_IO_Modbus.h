//---------------------------------------------------------------------------

#ifndef MyPLC_IO_ModbusH
#define MyPLC_IO_ModbusH
//#include "myio.h"
#include "ModbusTCPClient.h"
#define INPUT_MAX_REGISTER 22 //掃30001-30022          22
#define INPUT_MAX_Slave 1     //目前只用一個          1
#define PLCIO_INPUT_START_PORT_NUMBER 0x300
//---------------------------------------------------------------------------

extern int iTransactionID;
extern void InitPLCIO(AnsiString asIP,int iPort);
extern void PLCIOTaskCycle();
extern int iPLCIOTask;
extern bool bScanSlave[INPUT_MAX_Slave];
extern bool bPLCInData[INPUT_MAX_Slave][INPUT_MAX_REGISTER][8];
extern bool bPLCIO[2048][8]; //for 舊架構
extern bool bPLCIOEffect;   //已經接收過資料
extern bool bIOPowered;     //是否有供電? 急停旋鈕解除後按下Reset才算完成解除急停狀態
//ben 20230913 add
//==>
extern int iPLCStatusTask;
extern bool bSafePLCThread;
extern void PLCStatusCheck();
//<==
//ben 20230913 add
extern ModbusTCPClient PlcComm;

class TPLCIOThread : public TThread
{
    private:
    protected:
        void __fastcall Execute();
    public:
        __fastcall TPLCIOThread(bool CreateSuspended);
        void __fastcall TPLCIOThread::PLCIOProcess(void);
};
extern TPLCIOThread *MyPLCIOThread;

class TMyPLC_IO_Modbus //: public TMyIo
{
    private:
        bool bOn;
        int iSlave;
        int iPort;
        int iBit;

    protected:
        void DO_Process(byte value);
        Byte DI_Process();

    public:
        static std::vector<TMyPLC_IO_Modbus*> PLC_IO_Members;
        int GetSlave(){return iSlave;};
        int GetPort(){return iPort;};
        int GetBit(){return iBit;};

        __fastcall TMyPLC_IO_Modbus();

        virtual void SetPortInformation(int Card, int Port, int Bit);
        virtual bool IsOn();
        virtual bool IsOff();
        virtual void On();
        virtual void Off();
//        virtual byte IOInputByte();
        virtual void IOByteOut(int port,byte Byte);
        virtual void InitialMyOutIOData();

};
#endif
