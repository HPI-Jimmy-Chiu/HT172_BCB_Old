//---------------------------------------------------------------------------

#ifndef DecodeXMLH
#define DecodeXMLH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include "deviceinfo.h"
//---------------------------------------------------------------------------
extern const int SUMMARY_SUCCESS            ;
extern const int SUMMARY_FILE_NOT_EXIST     ;
extern const int SUMMARY_DUPLICATE_TRAY_ID  ;
//extern const int SUMMARY_FILE_NULL          ;
extern const int TRAY_MAP_FILE_IS_NULL      ;
extern const int TRAY_ID_IS_NULL            ;
extern const int TRAY_ID_NOT_EXIST          ;
extern const int TRAY_ID_DUPLICATE          ;

class TfXML : public TForm
{
__published:    // IDE-managed Components
    TXMLDocument *XMLDoc;
private:    // User declarations
public:     // User declarations
    __fastcall TfXML(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfXML *fXML;
//---------------------------------------------------------------------------
#include <map>
using namespace std;
//-----------------------------------------*------------------------------------
class TTrayMap
{
   private:
   public:
    TTrayMap()
    {
        iDevMaxRow=0;
        iDevMaxCol=0;
        iDevRealRow=0;
        iDevRealCol=0;
    };
    TTrayMap(AnsiString ID);
    ~TTrayMap();

    AnsiString DEVICE_NAME  ;
    AnsiString TEST_PROGRAM ;
    AnsiString TESTER_ID    ;
    AnsiString HANDLER_ID   ;
    AnsiString LOADBOARD_ID ;
    AnsiString LOT_ID       ;
    AnsiString RUN_CARD_ID  ;
    AnsiString STAGE        ;
    AnsiString SUB_STAGE    ;
    AnsiString ATTRIBUTE    ;
    AnsiString TRAY_ID      ;
    AnsiString START_TIME   ;
    AnsiString END_TIME     ;

    int iDevMaxRow;
    int iDevMaxCol;
    int iDevRealRow;
    int iDevRealCol;
    TDeviceInfo DevInfo[MAX_TRAY_Row][MAX_TRAY_Col];
    void InitialData();
};
//------------------------------------------------------------------------------
class TBinInfo
{
   public:
    TBinInfo(){};
    TBinInfo(AnsiString pass, int iCnt);
    int iCount;
    AnsiString sPassFail;
    bool bPass;
};
//------------------------------------------------------------------------------
class TXMLSummary
{
  private:
  public:
    TXMLSummary();
    TXMLSummary(AnsiString FileName);
    ~TXMLSummary();

    int ConvertXMLToStrusture(AnsiString FileName);
    void ResetData();
    int iTotalBinCount[TEST_MAX_BIN];

    map<AnsiString, AnsiString>TRAY_MAP_LIST;
    map<AnsiString, AnsiString>::iterator IterTRAY_MAP_LIST;
    map<int, TBinInfo>HBINSUM;
    map<int, TBinInfo>::iterator iterHBINSUM;
    map<int, TBinInfo>SBINSUM;
    map<int, TBinInfo>::iterator iterSBINSUM;
};
extern TXMLSummary myXML;
//------------------------------------------------------------------------------
extern int DecodeTrayMapByJHTFormat(AnsiString sFileName);
extern int DecodeTrayMapByWinstekFormat(AnsiString sFileName);
extern int DecodeTrayMapByEKeeperFormat(AnsiString sFileName);
extern int DecodeTrayMapBySimulation();
extern map<AnsiString, TTrayMap>mapTrayMapList;
extern map<AnsiString, TTrayMap>::iterator IterTrayMapList;
//------------------------------------------------------------------------------
#endif
