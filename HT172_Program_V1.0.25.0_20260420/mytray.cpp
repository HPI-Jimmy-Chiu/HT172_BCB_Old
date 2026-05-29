//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop
#include "mytray.h"
//==============================================================================
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TMyTray::TMyTray()
{
    iStripNoMapXStart=0;
    iStripNoMapYStart=0;
    iStripNoMapXPitch=0;
    iStripNoMapYPitch=0;
    iStripNoMapXOffset=0;
    iStripNoMapYOffset=0;
    iStrip1PXOffset=0;
    iStrip1PYOffset=0;
    dStripNoMapXScale=0.0;
    dStripNoMapYScale=0.0;
    dStripNoMapTheta=0.0;
    XItem=0;
    YItem=0;
    iTrackCol=0;
    iTrackRow=0;
    iBin=0;
    iTotalCount=0;
    iCurrCount=0;
    iEmptyCount=0;
    ClipID="";
}
//---------------------------------------------------------------------------
__fastcall TMyTray::~TMyTray()
{

}
//---------------------------------------------------------------------------
void TMyTray::SetTrayXYItem(int ItemX, int ItemY)
{
   XItem=(ItemX>=0 && ItemX<MAX_X_ITEM)?ItemX:0;                                //Steven 20240707 : 修正顯示
   YItem=(ItemY>=0 && ItemY<MAX_Y_ITEM)?ItemY:0;
   iTotalCount=XItem*YItem;
}
//==============================================================================
void TMyTray::ClearData()                                                       // SET NULL_IC //
{
    ClipID="";
    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<YItem; j++)
        {
            Data[i][j]=NULL_IC;
            DeviceInfo[i][j].Clear();                                           //KenHsieh 20240514 : 新增ClearTray 清除DeviceInfo
        }
    }
    iBin=0;
    ClearTrayMap();
    CaculateCount();
}
//==============================================================================
void TMyTray::CaculateCount()
{
    iCurrCount=HowManyIC();
    iEmptyCount=iTotalCount-iCurrCount;
}
//==============================================================================
void TMyTray::SetData()                                                         // SET HAS_IC //
{
    for(int i=0; i<XItem; i++)
        for(int j=0; j<YItem; j++)
            Data[i][j]=HAS_IC;

    CaculateCount();
}
//==============================================================================
void TMyTray::SetData(int data)                                                 // SET IC = DATA //
{
    for(int i=0; i<XItem; i++)
        for(int j=0; j<YItem; j++)
            Data[i][j]=data;

    CaculateCount();
}
//==============================================================================
void TMyTray::SetData(int iX, int iY, int data)
{
    Data[iX][iY]=data;
    if(data==NULL_IC)
    {
        iEmptyCount++;
        iCurrCount--;
    }
    else
    {
        iEmptyCount--;
        iCurrCount++;
    }
}
//==============================================================================
bool TMyTray::HasIC()                                                           // CHECK HAS IC //
{
    for(int i=0; i<XItem; i++)
        for(int j=0; j<YItem; j++)
            if(Data[i][j]!=NULL_IC)
                return true;
    return false;
}
//==============================================================================
bool TMyTray::RowYFullIC(int Y)                                                  // CHECK HAS IC //
{
    for(int i=0; i<XItem; i++)
        if(Data[i][Y]==NULL_IC)
            return false;
    return true;
}
//==============================================================================
bool TMyTray::HasRing()                                                        // CHECK HAS IC //
{
    for(int i=0; i<XItem; i++)
        for(int j=0; j<YItem; j++)
            if(Data[i][j]==HAS_RING_HAS_IC || Data[i][j]==HAS_RING_NO_IC)
                return true;
    return false;
}
//==============================================================================
bool TMyTray::HasRealIC()                                                       // CHECK HAS REAL IC //
{
    for(int i=0; i<XItem; i++)
        for(int j=0; j<YItem; j++)
            if(Data[i][j] && Data[i][j]!=HAS_NULL_IC)
                return true;
    return false;
}
//==============================================================================
bool TMyTray::FullIC()                                                          // CHECK FULL IC //
{
    for(int i=0; i<XItem; i++)
        for(int j=0; j<YItem; j++)
            if(Data[i][j]==NULL_IC)
                return false;
    return true;
}
//==============================================================================
int TMyTray::HowManyIC()                                                        // COUNT IC //
{
    int sum=0;
    for(int i=0; i<XItem; i++)
        for(int j=0; j<YItem; j++)
            if(Data[i][j]!=NULL_IC)
                sum++;
    return sum;
}
//------------------------------------------------------------------------------
void TMyTray::CountAllPointForPickStripAlignment(int XOffset, int YOffset)
{
    iStrip1PXOffset=XOffset;
    iStrip1PYOffset=YOffset;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<YItem; j++)
        {
            iStrip1PAlignPX[i][j]=iStripNoMapXStart-XOffset-iStripNoMapXPitch*i;
            iStrip1PAlignPY[i][j]=iStripNoMapYStart-YOffset+iStripNoMapYPitch*j;
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::CountAllPointForPickStripAlignment_iPitchY1(int XOffset, int YOffset,int iPitchY1)
{
    iStrip1PXOffset=XOffset;
    iStrip1PYOffset=YOffset;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<(YItem/2); j++)
        {
            iStrip1PAlignPX[i][j]=iStripNoMapXStart-XOffset-iStripNoMapXPitch*i;
            iStrip1PAlignPY[i][j]=iStripNoMapYStart-YOffset+iStripNoMapYPitch*j;
        }
    }
    for(int i=0; i<XItem; i++)
    {
        for(int j=(YItem/2); j<YItem; j++)
        {
            iStrip1PAlignPX[i][j]=iStripNoMapXStart-XOffset-iStripNoMapXPitch*i;
            iStrip1PAlignPY[i][j]=iStripNoMapYStart-YOffset+iStripNoMapYPitch*j+iPitchY1;
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::ClearStrip1PAlignmentXY()
{
    iStrip1PXOffset=0;
    iStrip1PYOffset=0;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<YItem; j++)
        {
            iStrip1PAlignPX[i][j]=0;
            iStrip1PAlignPY[i][j]=0;
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::CountAllPointForPickStripNoMap(int StartX, int StartY, int iPitchX, int iPitchY)
{
    iStripNoMapXStart=StartX;
    iStripNoMapYStart=StartY;
    iStripNoMapXPitch=iPitchX;
    iStripNoMapYPitch=iPitchY;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<YItem; j++)
        {
            iStripNoMapPX[i][j]=StartX-iPitchX*i;
            iStripNoMapPY[i][j]=StartY+iPitchY*j;
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::CountAllPointForPickStripNoMap_iPitchY1(int StartX, int StartY, int iPitchX, int iPitchY, int iPitchY1)
{
    iStripNoMapXStart=StartX;
    iStripNoMapYStart=StartY;
    iStripNoMapXPitch=iPitchX;
    iStripNoMapYPitch=iPitchY;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<(YItem/2); j++)
        {
            iStripNoMapPX[i][j]=StartX-iPitchX*i;
            if(j<YItem/2)
            {
                iStripNoMapPY[i][j]=StartY+iPitchY*j;
            }
            else
            {
                iStripNoMapPY[i][j]=StartY+iPitchY*j+iPitchY1;
            }
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::CountAllPointForPickStripNoMap_iNullICWidth(int StartX, int StartY, int iPitchX, int iPitchY, int iNullICWidth)
{
    iStripNoMapXStart=StartX;
    iStripNoMapYStart=StartY;
    iStripNoMapXPitch=iPitchX;
    iStripNoMapYPitch=iPitchY;

    for(int i=0; i<(XItem/2); i++)
    {
        for(int j=0; j<YItem; j++)
        {
            if(i<XItem/2)
            {
                iStripNoMapPX[i][j]=StartX-iPitchX*i;
            }
            else
            {
                iStripNoMapPX[i][j]=StartX-iPitchX*i+iNullICWidth;
            }

            iStripNoMapPY[i][j]=StartY+iPitchY*j;
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::ClearStripXY()
{
    iStripNoMapXStart=0;
    iStripNoMapYStart=0;
    iStripNoMapXPitch=0;
    iStripNoMapYPitch=0;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<YItem; j++)
        {
            iStripNoMapPX[i][j]=0;
            iStripNoMapPY[i][j]=0;
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::CountAllPointForPickStripAlignmentScale(int XOffset, int YOffset, double XScale, double YScale)
{
    iStripNoMapXOffset=XOffset;
    iStripNoMapYOffset=YOffset;
    dStripNoMapXScale=XScale;
    dStripNoMapYScale=YScale;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<YItem; j++)
        {
            iStripNoMapAlignPX[i][j]=iStripNoMapXStart-iStrip1PXOffset-XOffset-iStripNoMapXPitch*i/XScale;
            iStripNoMapAlignPY[i][j]=iStripNoMapYStart-iStrip1PYOffset-YOffset+iStripNoMapYPitch*j/YScale;
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::CountAllPointForPickStripAlignmentScale_iPitchY1(int XOffset, int YOffset, double XScale, double YScale, int iPitchY1)
{
    iStripNoMapXOffset=XOffset;
    iStripNoMapYOffset=YOffset;
    dStripNoMapXScale=XScale;
    dStripNoMapYScale=YScale;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<YItem; j++)
        {
            iStripNoMapAlignPX[i][j]=iStripNoMapXStart-iStrip1PXOffset-XOffset-iStripNoMapXPitch*i/XScale;
            if(j<YItem/2)
            {
                iStripNoMapAlignPY[i][j]=iStripNoMapYStart-iStrip1PYOffset-YOffset+iStripNoMapYPitch*j/YScale;
            }
            else
            {
                iStripNoMapAlignPY[i][j]=iPitchY1+iStripNoMapYStart-iStrip1PYOffset-YOffset+iStripNoMapYPitch*j/YScale;
            }
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::ClearStripAlignmentXY()
{
    iStripNoMapXOffset=0;
    iStripNoMapYOffset=0;
    dStripNoMapXScale=0.0;
    dStripNoMapYScale=0.0;

    for(int i=0; i<XItem; i++)
    {
        for(int j=0; j<YItem; j++)
        {
            iStripNoMapAlignPX[i][j]=0;
            iStripNoMapAlignPY[i][j]=0;
        }
    }
}
//------------------------------------------------------------------------------
void TMyTray::SetPickStripAlignmentRotate(double theta)
{
    dStripNoMapTheta=-200000.0/360.0*theta;
}
//------------------------------------------------------------------------------
int TMyTray::GetPickStripAlignmentRotate()
{
    return dStripNoMapTheta;                                                    //需要根據齒輪比修改
}
//------------------------------------------------------------------------------
void TMyTray::ClearTrayMap()
{
    sStartTime   ="";
    DEVICE_NAME  ="";
    TEST_PROGRAM ="";
    TESTER_ID    ="";
    HANDLER_ID   ="";
    LOADBOARD_ID ="";
    LOT_ID       ="";
    RUN_CARD_ID  ="";
    STAGE        ="";
    SUB_STAGE    ="";
    ATTRIBUTE    ="";
    TRAY_ID      ="";
    START_TIME   ="";
    END_TIME     ="";
}
//------------------------------------------------------------------------------
void TMyTray::CopyFromTrayMap(const TTrayMap &traymap)
{
    DEVICE_NAME  =traymap.DEVICE_NAME;
    TEST_PROGRAM =traymap.TEST_PROGRAM;
    TESTER_ID    =traymap.TESTER_ID;
    HANDLER_ID   =traymap.HANDLER_ID;
    LOADBOARD_ID =traymap.LOADBOARD_ID;
    LOT_ID       =traymap.LOT_ID;
    RUN_CARD_ID  =traymap.RUN_CARD_ID;
    STAGE        =traymap.STAGE;
    SUB_STAGE    =traymap.SUB_STAGE;
    ATTRIBUTE    =traymap.ATTRIBUTE;
    TRAY_ID      =traymap.TRAY_ID;
    START_TIME   =traymap.START_TIME;
    END_TIME     =traymap.END_TIME;
}
//------------------------------------------------------------------------------
void TMyTray::CopyFromMyTray(const TMyTray &mytray)
{
    ClipID       =mytray.ClipID;
    iBin         =mytray.iBin;
    DEVICE_NAME  =mytray.DEVICE_NAME;
    TEST_PROGRAM =mytray.TEST_PROGRAM;
    TESTER_ID    =mytray.TESTER_ID;
    HANDLER_ID   =mytray.HANDLER_ID;
    LOADBOARD_ID =mytray.LOADBOARD_ID;
    LOT_ID       =mytray.LOT_ID;
    RUN_CARD_ID  =mytray.RUN_CARD_ID;
    STAGE        =mytray.STAGE;
    SUB_STAGE    =mytray.SUB_STAGE;
    ATTRIBUTE    =mytray.ATTRIBUTE;
    TRAY_ID      =mytray.TRAY_ID;
    START_TIME   =mytray.START_TIME;
    END_TIME     =mytray.END_TIME;
}
//------------------------------------------------------------------------------
















