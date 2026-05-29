//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "myio.h"
#pragma package(smart_init)
#define MAX_ISA_BYTE   128
//#include "myio.h"
////#include "gwiopm.h"
////#include "hsensor.h"
//#include "cmydef.h"
//==============================================================================

static int  OutPortIndex[MAX_ISA_BYTE];
static int  InPortIndex[MAX_ISA_BYTE];
static byte OutPortData[MAX_ISA_BYTE];
static int  OutPos=0;
static int  InPos=0;
static bool bInitNTPort=false;
static bool bInitialISAIO=false;
//==============================================================================
void TMyIo::InitialMyOutIOData()                                                // initial output //
{
    int i;
    for(i=0;i<MAX_ISA_BYTE;i++)
    {
        OutPortIndex[i]=0;
        OutPortData[i]=0;
    }
}
//==============================================================================
int TMyIo::IOSetOutport(int port)                                           // set out port //
{
    int i;
    if(bInitialISAIO==false)
    {
        //InitialMyOutIOData();
        bInitialISAIO=true;
    }

    if(port==0) return 999;
    for(i=0;i<OutPos;i++)
        if( OutPortIndex[i]==port)
            return i;
    if(OutPos>=MAX_ISA_BYTE)
        return 999; // 不能發生 !!!
    if( bInitNTPort==false )
    {
        bInitNTPort=true;
        //InitNTPort();
    }
    //EnableNTPort(port,port);
    OutPortIndex[OutPos]=port;
    OutPos++;
    return (OutPos-1);
}
//==============================================================================
bool TMyIo::IOSetInport(int port)                                           // set in port //
{
    int i;
    if(port==0) return false;
    for(i=0;i<InPos;i++)
        if( InPortIndex[i]==port)
            return true;
    if(InPos>=MAX_ISA_BYTE)
        return false; // 不能發生 !!!
    if( bInitNTPort==false )
    {
        bInitNTPort=true;
        //InitNTPort();
    }
    //EnableNTPort(port,port);
    InPortIndex[InPos]=port;
    InPos++;
    return true;
}
//==============================================================================
byte TMyIo::IOInputByte(int port)                                           // get in port //
{
    return true;
}
//==============================================================================
void IOBitOn(int port,int bit)
{
    return;
}
//==============================================================================
void IOBitOff(int port,int bit)
{
    return;
}
//==============================================================================
void TMyIo::IOByteOut(int port, byte Byte)                                   // out port byte out //
{
    return;
}
//==============================================================================
bool IOOutBitStatus(int port, int bit)
{
    return false;
}
byte IOOutByteStatus(int port)
{
    return 0;
}
bool IOInputBit(int port, int bit)
{
    return false;
}
//HSL
unsigned int IOInputLongByte(int port)
{
//    unsigned long OutData;
//
//    return OutData;
    return 0;
}

int  BackOutPortIndex[MAX_ISA_BYTE];
byte BackOutPortData[MAX_ISA_BYTE];
unsigned int BackOutLongData[MAX_ISA_BYTE];
void BackUpOutputData()
{
    return;
}

void RestoreOutputData()
{
    return;
}
//==============================================================================
void TMyIo::SetPortInformation(int Card, int Port, int Bit)                   // set io card data //
{
    iCard=Card;
    iPort=Port;
    iBit=Bit;
}
//==============================================================================
void TMyIo::SetPortInformation(int Line, AnsiString IP, int Port, int Bit)      // set io card data //
{
    int iIP;
    if(IP.AnsiCompare("A")>=0 && IP.AnsiCompare("Z")<=0)
    {
        char *QQ=IP.c_str();
        iIP=int(QQ[0]-'A')+10;
    }
    else if(IP=="")
    {
        iIP=0;
    }
    else //if(IP.AnsiCompare("0")>=0 && IP.AnsiCompare("9")<=0)
    {
        iIP=atoi(IP.c_str());
    }

    iCard=Line*100+iIP;
    iPort=Port;
    iBit=Bit;
}
//==============================================================================
void TMyIo::SetPortInformation(AnsiString Card, AnsiString Port, AnsiString Bit)
{
    iCard=0;
    iPort=atoi(Port.c_str());
    iBit=atoi(Bit.c_str());
}
//==============================================================================
bool TMyIo::IsOn()
{
    return false;
}
//==============================================================================
bool TMyIo::IsOff()                                                         // check off //
{
    return false;
}
//==============================================================================
void TMyIo::On()                                                            // output on //
{
    return;
}
//==============================================================================
void TMyIo::Off()                                                           // output off //
{
    return;
}
//==============================================================================
void TMyIo::SetHint(AnsiString Hint)
{
    if(Hint=="")
    {
        IOHint="00000";
    }
    else
    {
        if(Hint.Length()!=5)
        {
            while(Hint.Length()<5)
            {
                Hint="0"+Hint;
            }
        }

        IOHint=Hint;
    }
}
//==============================================================================




