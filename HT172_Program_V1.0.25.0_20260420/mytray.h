//---------------------------------------------------------------------------
#ifndef mytrayH
#define mytrayH
#include "cmydef.h"
#include "deviceinfo.h"
#include "DecodeXML.h"
//---------------------------------------------------------------------------

class TMyTray{
    private:
        int iStripNoMapXStart;
        int iStripNoMapYStart;
        int iStripNoMapXPitch;
        int iStripNoMapYPitch;
        int iStripNoMapXOffset;
        int iStripNoMapYOffset;

        int iStrip1PXOffset;
        int iStrip1PYOffset;

        double dStripNoMapXScale;
        double dStripNoMapYScale;
        double dStripNoMapTheta;

    public :
        __fastcall TMyTray();
        __fastcall ~TMyTray();
        int XItem;
        int YItem;
        int iTrackCol;
        int iTrackRow;
        int iBin;
        int iTotalCount;
        int iCurrCount;
        int iEmptyCount;
        AnsiString ClipID;
        int Data[MAX_X_ITEM][MAX_Y_ITEM];
        TDeviceInfo DeviceInfo[MAX_X_ITEM][MAX_Y_ITEM];
        //char iErrorCode[MAX_X_ITEM][MAX_Y_ITEM][16];                          //V1.0
        char cErrorCode[MAX_X_ITEM][MAX_Y_ITEM][16];                            //V1.1
        int iPixelDrop[MAX_X_ITEM][MAX_Y_ITEM];
        int iWhichSite[MAX_X_ITEM][MAX_Y_ITEM];
        int iBinCode[MAX_X_ITEM][MAX_Y_ITEM];

        int iStrip1PAlignPX[MAX_X_ITEM][MAX_Y_ITEM];
        int iStrip1PAlignPY[MAX_X_ITEM][MAX_Y_ITEM];
        void CountAllPointForPickStripAlignment(int XOffset, int YOffset);
        void CountAllPointForPickStripAlignment_iPitchY1(int XOffset, int YOffset, int iPitchY1);
        void ClearStrip1PAlignmentXY();

        int iStripNoMapPX[MAX_X_ITEM][MAX_Y_ITEM];
        int iStripNoMapPY[MAX_X_ITEM][MAX_Y_ITEM];
        void CountAllPointForPickStripNoMap(int StartX, int StartY, int iPitchX, int iPitchY);
        void CountAllPointForPickStripNoMap_iPitchY1(int StartX, int StartY, int iPitchX, int iPitchY, int iPitchY1);     //Sam 20180620 : Map Strip Frame Type5 使用 PitchY1 變數。

        void CountAllPointForPickStripNoMap_iNullICWidth(int StartX, int StartY, int iPitchX, int iPitchY, int iNullICWidth);
        void ClearStripXY();

        int iStripNoMapAlignPX[MAX_X_ITEM][MAX_Y_ITEM];
        int iStripNoMapAlignPY[MAX_X_ITEM][MAX_Y_ITEM];
        void SetPickStripAlignmentRotate(double theta);

        int GetPickStripAlignmentRotate();
        void CountAllPointForPickStripAlignmentScale(int XOffset, int YOffset, double XScale, double YScale);
        void CountAllPointForPickStripAlignmentScale_iPitchY1(int XOffset, int YOffset, double XScale, double YScale, int iPitchY1);
        void ClearStripAlignmentXY();

        void SetTrayXYItem(int ItemX, int ItemY);
        void CaculateCount();
        void ClearData();
        void SetData();
        void SetData(int data);
        void SetData(int iX, int iY, int data);
        bool HasIC();
        bool HasRing();
        bool HasRealIC();
        bool FullIC();
        bool RowYFullIC(int Y);
        int HowManyIC();

        int GetXPitch()     {return iStripNoMapXPitch;};
        int GetYPitch()     {return iStripNoMapYPitch;};
        double GetXScale()  {return dStripNoMapXScale;};
        double GetYScale()  {return dStripNoMapYScale;};

        //TrayMap
        AnsiString sStartTime   ;
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
        void ClearTrayMap();
        void CopyFromTrayMap(const TTrayMap &traymap);
        void CopyFromMyTray(const TMyTray   &mytray);
};
#endif
