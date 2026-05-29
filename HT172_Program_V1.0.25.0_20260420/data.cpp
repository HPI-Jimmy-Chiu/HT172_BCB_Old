//==============================================================================
// 最後檢查日:2006/04/18
// 檢查人    :lee
// 目的      :9016 &
//==============================================================================

#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "HTray"
#pragma resource "*.dfm"
TfData *fData;
//==============================================================================
__fastcall TfData::TfData(TComponent* Owner)                                    // ben edit 20110709 //
     : TForm(Owner)
{

}
//==============================================================================
void __fastcall TfData::sbDataChange0Click(TObject *Sender)                     // ben edit 20110709 //
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    PageData->ActivePageIndex=Ptr->Tag;
    Panel1->Caption=Ptr->Caption;
    Ptr->Down=false;

    TTabSheet* currentPage=PageData->ActivePage;
    if(currentPage!=NULL)
    {
        if(currentPage->Name=="tsData0")
        {
            AnsiString LogName=AnsiString().sprintf("D:\\HT-172_Log\\EventLog\\%04d_%02d\\%s_%04d_%02d_%2d.csv", SystemYear, SystemMonth, asHandlerID, SystemYear, SystemMonth, SystemDate);
            Memo1->Clear();
            if(FileExists(LogName))
                Memo1->Lines->LoadFromFile(LogName);
        }
    }
}
//==============================================================================
void __fastcall TfData::sbDataExitClick(TObject *Sender)                        // button exit //
{
    sbDataExit->Down=false;
    Close();
}
//==============================================================================
void __fastcall TfData::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;
    Top =0;
    for(int i=0; i<PageData->PageCount; i++)
        PageData->Pages[i]->TabVisible=false;

    labVersion      ->Caption=MainVersion;    //sVersion
    labModel        ->Caption=asModel;        //Sam 20170802 (jou): 顯示型號
    labSerialNo     ->Caption=asSerialNo;     //Sam 20170802 (jou): 顯示序號
    labMachineId    ->Caption=asHandlerID;    //Sam 20170802 (jou): 顯示機號
    labFactory      ->Caption=asFactory ;     //Sam 20170802 (jou): 顯示廠號

    sbHandlerInfo->Click();
    sbHandlerInfo->Down=true;
    Timer1->Enabled=true;
    lbJamratedenom->Caption=AnsiString().sprintf("/ %d",GetJamRateDenom());
}
//==============================================================================
void __fastcall TfData::FormClose(TObject *Sender, TCloseAction &Action)
{
    Timer1->Enabled=false;
}
//==============================================================================
void __fastcall TfData::Timer1Timer(TObject *Sender)
{
    static bool flag=false;
    if(flag==true)
        return;
    flag=true;
    //
    UpdateHandlerInfoUI();
    UpdateRunInfoUI();
    //
    flag=false;
}
//---------------------------------------------------------------------------
void TfData::UpdateHandlerInfoUI()
{
    TTabSheet* currentPage=PageData->ActivePage;
    if(currentPage!=NULL)
    {
        if(currentPage->Name=="tsHandlerInfo")
        {
            labPowerOnTime  ->Caption=ConvertSecondToTime(HSys.LastSet.SystemTimeRecord[stPowerOn]);
            labRunningTime  ->Caption=ConvertSecondToTime(HSys.LastSet.SystemTimeRecord[stStartTime]);
            labProductTime  ->Caption=ConvertSecondToTime(HSys.LastSet.SystemTimeRecord[stProductTime]);
            labPauseTime    ->Caption=ConvertSecondToTime(HSys.LastSet.SystemTimeRecord[stPauseTime]);
            labJamTime      ->Caption=ConvertSecondToTime(HSys.LastSet.SystemTimeRecord[stJamTime]);
            labMTBA         ->Caption=ConvertSecondToTime(HSys.LastSet.SystemTimeRecord[stMTBA]);
        }
    }
}
//---------------------------------------------------------------------------
void TfData::UpdateRunInfoUI()
{
    TTabSheet* currentPage=PageData->ActivePage;
    if(currentPage!=NULL)
    {
        if(currentPage->Name=="tsRunInfo")
        {
            plTotalIC  ->Caption=tRunData.TotalIC;
            plJamCount ->Caption=tRunData.JamCount;
            plJamRate  ->Caption=GetDoubleToStr(tRunData.JamRate);
        }
    }
}
//---------------------------------------------------------------------------

