//---------------------------------------------------------------------------
#ifndef myioH
#define myioH
//---------------------------------------------------------------------------

// 2009/7/7
class TMyIo
{
    private:
    protected:
    public:
        int  iCard;
        int  iLane;
        int  iIP;
        int  iPort;
        int  iBit;
        __fastcall TMyIo(){IOHint="";};
        ~TMyIo(){};
        virtual void SetPortInformation(int Card, int Port, int Bit);
        virtual void SetPortInformation(int Line, AnsiString IP, int Port, int Bit);
        virtual void SetPortInformation(AnsiString Card, AnsiString Port, AnsiString Bit);
        virtual bool IsOn();
        virtual bool IsOff();
        virtual void On();
        virtual void Off();
        virtual int  GetCard() {return iCard;};
        virtual int  GetLane() {return iLane;};
        virtual int  GetIP()   {return iIP;};
        virtual int  GetPort() {return iPort;};
        virtual int  GetBit()  {return iBit;};

        byte IOInputByte(int port);             //2011.03.18 s
        int IOInputLongByte(int port);
        void IOByteOut(int port, byte Byte);
        void InitialMyOutIOData();
        int  IOSetOutport(int port);
        bool IOSetInport(int port);
        void SetHint(AnsiString Hint);
        AnsiString GetHint() {return IOHint;};                                  //Frank 20150716
        AnsiString IOHint;
        int ISABase;
};                                               //2011.03.18 e
#endif























