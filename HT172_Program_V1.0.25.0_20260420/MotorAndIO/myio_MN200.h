//---------------------------------------------------------------------------

#ifndef MyIOMN200H
#define MyIOMN200H
#include "myio.h"
//---------------------------------------------------------------------------
class TMyMN200_IO :public TMyIo
{
    private:
//        int  iCard;
//        int  iLane;
//        int  iIP;
//        int  iPort;
//        int  iBit;
    protected:
        void DO_Process(Byte value);
        Byte DI_Process(bool bByte=false);
    public:
        __fastcall TMyMN200_IO();
//        ~TMyMN200_IO();
        virtual void SetPortInformation(int Card, int Port, int Bit);
        virtual void SetPortInformation(int Line, AnsiString IP, int Port, int Bit);
        virtual void SetPortInformation(AnsiString Card, AnsiString Port, AnsiString Bit);
        virtual bool IsOn();
        virtual bool IsOff();
        virtual void On();
        virtual void Off();
//        virtual int  GetCard() {return iCard;};
//        virtual int  GetPort() {return iPort;};
//        virtual int  GetBit() {return iBit;};
        byte IOInputByte();
        void IOByteOut(byte Byte);
        void InitialMyOutIOData();
};
extern int CheckPCI_L112State();
extern void ResetMNet(int iRingNo, AnsiString EngMessage, AnsiString ChtMessage, bool bShowMess);
extern void OpenPCI132Card();
extern void MNetLog(AnsiString Str);
extern bool bOpenCard;
#endif
