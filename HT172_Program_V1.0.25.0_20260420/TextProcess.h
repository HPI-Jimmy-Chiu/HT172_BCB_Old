//---------------------------------------------------------------------------

#ifndef TextProcessH
#define TextProcessH

//---------------------------------------------------------------------------
#define swap16(x) ((((x) & 0xFF)<<8) | (((x)>>8) & 0xFF))
//---------------------------------------------------------------------------
AnsiString MyDeCodeASCII(int iInPut);                       //10進制 轉 ASCII Code
byte       MyASCIIToDec(AnsiString asCode);                 //ASCII Code 轉 10進制
AnsiString CountHexToByte(AnsiString asHCode);              //16進制轉2進制
int        HexStrToInt(char *str);                          //16進制轉10進制
int        HexStrToInt(AnsiString str);                     //16進制轉10進制
int        SetBCC(AnsiString SendText);                     //計算BCC
int        CountByteToInt(AnsiString asBCode);              //2進制轉10進制
AnsiString CountByteToHex(AnsiString asBCode);              //2進制轉16進制

int Convert2Byte(char *sData);
int ConvertByte(char *sData);
//unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii);
//unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex);
//unsigned char A_Create_LCR(unsigned char *Sptr, unsigned char length);
unsigned float2hex(unsigned char typecode, double value, int dataformat);

#endif
