//------------------------------------------------------------------------------
#include "IncludeAllHeader.h"                                                   //Dell 將.h統一,可加速build
#pragma hdrstop

#include "uTrayEditForm.h"
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TTrayEditForm *TrayEditForm;
bool bMouseDown=false;
int iStartX, iStartY, iEndX, iEndY, iOldStartX, iOldStartY;
int iEditBackTray[MAX_X_ITEM][MAX_Y_ITEM];
AnsiString sEditBackTrayBin[MAX_X_ITEM][MAX_Y_ITEM];
TTrayMotor *iEditMotorIndex;
int ICType;
//==============================================================================
__fastcall TTrayEditForm::TTrayEditForm(TComponent* Owner)
    : TForm(Owner)
{
    bSetBin=false;
    fShow=false;

    cbBinCount->Clear();
    for(int i=0; i<TEST_MAX_BIN; i++)
        cbBinCount->Items->Add(i);
}
//==============================================================================
void __fastcall TTrayEditForm::FormShow(TObject *Sender)
{
    for(int i=0; i<MAX_X_ITEM; i++)
    {
        for(int j=0; j<MAX_Y_ITEM; j++)
        {
            iEditBackTray[i][j]=0;
            sEditBackTrayBin[i][j]="";
        }
    }


    Left=(1280-Width)/2;
    Top =50;
    fShow=true;

    if(bSetBin==true)
    {
        cbBinCount->Enabled =true;
        cbBinCount->Text    ="1";
    }
    else
    {
        cbBinCount->Enabled =false;
        cbBinCount->Text    =iEditMotorIndex->Tray.iBin;
    }

    mtLoaderBuffer->XItem   =iEditMotorIndex->Tray.XItem;
    mtLoaderBuffer->YItem   =iEditMotorIndex->Tray.YItem;
    edtID->Text             =iEditMotorIndex->Tray.ClipID;

    for(int i=0; i<iEditMotorIndex->Tray.XItem; i++)
    {
        for(int j=0; j<iEditMotorIndex->Tray.YItem; j++)
        {
            if(iEditMotorIndex->Tray.Data[i][j]!=NULL_IC)
            {
                mtLoaderBuffer->SetCellColorIndex(i, j, 1);
                mtLoaderBuffer->SetCellNumber(i, j, iEditMotorIndex->Tray.DeviceInfo[i][j].iBin);
                sEditBackTrayBin[i][j]=iEditMotorIndex->Tray.DeviceInfo[i][j].iBin;
            }
            else
            {
                mtLoaderBuffer->SetCellColorIndex(i, j, 0);
                mtLoaderBuffer->SetCellNumber(i, j, "");
                sEditBackTrayBin[i][j]="";
            }

            if(iEditMotorIndex->Tray.Data[i][j]!=NULL_IC)
                iEditBackTray[i][j]=1;
            else
                iEditBackTray[i][j]=0;
        }
    }
}
//==============================================================================
void __fastcall TTrayEditForm::mtLoaderBufferMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    mtLoaderBuffer->ConvertIndexCells(X, Y);
    if(X<0 || X>=mtLoaderBuffer->XItem || Y<0 || Y>=mtLoaderBuffer->YItem)
        return;
    bMouseDown=true;
    iStartX=X;
    iStartY=Y;
    iEndX=X;
    iEndY=Y;
}
//==============================================================================
void __fastcall TTrayEditForm::mtLoaderBufferMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if(bMouseDown)
    {
        mtLoaderBuffer->ConvertIndexCells(X, Y);
        if(X<0 || X>=mtLoaderBuffer->XItem || Y<0 || Y>=mtLoaderBuffer->YItem)
            return;
        iEndX=X;
        iEndY=Y;
        ShowTray();
    }
}
//==============================================================================
void __fastcall TTrayEditForm::mtLoaderBufferMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(bMouseDown)
    {
        bMouseDown=false;
        SetTray();
    }
}
//==============================================================================
void __fastcall TTrayEditForm::Change()                                         // check change pos //
{
    int iSX, iSY, iEX, iEY;
    if(iStartX>iEndX)
    {
        iSX=iEndX;
        iEX=iStartX;
    }
    else
    {
        iSX=iStartX;
        iEX=iEndX;
    }
    if(iStartY>iEndY)
    {
        iSY=iEndY;
        iEY=iStartY;
    }
    else
    {
        iSY=iStartY;
        iEY=iEndY;
    }
    iStartX=iSX;
    iStartY=iSY;
    iEndX=iEX;
    iEndY=iEY;

}
//==============================================================================
void __fastcall TTrayEditForm::ShowTray()                                       // show tray cell color //
{
    int iSX, iSY, iEX, iEY;
    if(iStartX>iEndX)
    {
        iSX=iEndX;
        iEX=iStartX;
    }
    else
    {
        iSX=iStartX;
        iEX=iEndX;
    }
    if(iStartY>iEndY)
    {
        iSY=iEndY;
        iEY=iStartY;
    }
    else
    {
        iSY=iStartY;
        iEY=iEndY;
    }
    //============
    for(int i=0; i<iEditMotorIndex->Tray.XItem; i++)
    {
        for(int j=0; j<iEditMotorIndex->Tray.YItem; j++)
        {
            if(i>=iSX && i<=iEX)
            {
                if(j>=iSY && j<=iEY)
                {
                    mtLoaderBuffer->SetCellColorIndex(i, j, 2);
                }
                else
                {
                    if(iEditBackTray[i][j]==1)
                        mtLoaderBuffer->SetCellColorIndex(i, j, 1);
                    else
                        mtLoaderBuffer->SetCellColorIndex(i, j, 0);
                }
            }
            else
            {
                if(iEditBackTray[i][j]==1)
                    mtLoaderBuffer->SetCellColorIndex(i, j, 1);
                else
                    mtLoaderBuffer->SetCellColorIndex(i, j, 0);
            }
        }
    }
}
//==============================================================================
void __fastcall TTrayEditForm::SetTray()                                        // set tray data //
{
    Change();
    for(int i=iStartX; i<=iEndX; i++)
    {
        for(int j=iStartY; j<=iEndY; j++)
        {
            if(iEditBackTray[i][j]==1)
            {
                iEditBackTray[i][j]=0;
                sEditBackTrayBin[i][j]="";   
                mtLoaderBuffer->SetCellNumber(i, j, sEditBackTrayBin[i][j]);

            }
            else
            {
                iEditBackTray[i][j]=1;
                sEditBackTrayBin[i][j]=cbBinCount->Text;
                mtLoaderBuffer->SetCellNumber(i, j, sEditBackTrayBin[i][j]);
            }
            mtLoaderBuffer->SetCellColorIndex(i, j, iEditBackTray[i][j]);
        }
    }
}
//==============================================================================
void __fastcall TTrayEditForm::Timer1Timer(TObject *Sender)
{
    AnsiString Str;
    if(fShow==false)
        return;

    Str.sprintf("(%d,%d)...(%d,%d)", iStartX, iStartY, iEndX, iEndY);
    Caption=Str;
}
//==============================================================================
void __fastcall TTrayEditForm::btnUpdateClick(TObject *Sender)                  // update button //
{
    MyBinToTrayStruct BtoT;
    for(int i=0; i<iEditMotorIndex->Tray.XItem; i++)
    {
        for(int j=0; j<iEditMotorIndex->Tray.YItem; j++)
        {
            if(iEditBackTray[i][j]==1)
            {
                BtoT.ConvertBinToAuto(atoi(sEditBackTrayBin[i][j].c_str()));

                iEditMotorIndex->SetTraySingleData(i, j, HAS_IC);
                iEditMotorIndex->Tray.DeviceInfo[i][j].iBin =BtoT.iBin;
                iEditMotorIndex->Tray.DeviceInfo[i][j].iAuto=BtoT.iWhichAuto;
            }
            else
            {
                iEditMotorIndex->SetTraySingleData(i, j, NULL_IC);
                iEditMotorIndex->Tray.DeviceInfo[i][j].iBin =0;
                iEditMotorIndex->Tray.DeviceInfo[i][j].iAuto=0;
            }


        }
    }
    iEditMotorIndex->SetTrayID(edtID->Text);
    Close();
}
//==============================================================================
void __fastcall TTrayEditForm::btnAbortClick(TObject *Sender)                   // about //
{
    Close();
}
//==============================================================================
void  TTrayEditForm::EditTray(int MotorIndexIndex, bool _bSetBin)               // tray edit //
{
    AnsiString asLog="", asMotorName="";

    if(MotorIndexIndex==LoaderModule->MotWorkLoader->Tag)
    {
        iEditMotorIndex=LoaderModule->MotWorkLoader;
        asMotorName="MotWorkLoader";                                            //KenHsieh 20240725 : add edit tray log
    }
    else if(MotorIndexIndex==LoaderModule->MotLoader_Car->Tag)
    {
        iEditMotorIndex=LoaderModule->MotLoader_Car;
        asMotorName="MotLoader_Car";                                            //KenHsieh 20240725 : add edit tray log
    }
    else if(MotorIndexIndex==MagArmPara->MMagSortTray->Tag)
    {
        iEditMotorIndex=MagArmPara->MMagSortTray;
        asMotorName="MMagSortTray";                                             //KenHsieh 20240725 : add edit tray log
    }
    else if(MotorIndexIndex==MagArmPara->MMagNowSortTray->Tag)  //JerryYang 20240607 : add
    {
        iEditMotorIndex=MagArmPara->MMagNowSortTray;
        asMotorName="MMagNowSortTray";                                          //KenHsieh 20240725 : add edit tray log
    }
    else if(MotorIndexIndex==LoaderModule->MotNowSortTray->Tag)                 //JerryYang 20240601 : Add Edit Tray of MotNowSortTray
    {
        iEditMotorIndex=LoaderModule->MotNowSortTray;
        asMotorName="MotNowSortTray";                                           //KenHsieh 20240725 : add edit tray log
    }
    else if(MotorIndexIndex==MagArmPara->MMagSortTopTray[eMagazine1]->Tag)
    {
        iEditMotorIndex=MagArmPara->MMagSortTopTray[eMagazine1];
        asMotorName="MMag1SortTopTray";                                         //KenHsieh 20240725 : add edit tray log
    }
    else if(MotorIndexIndex==MagArmPara->MMagSortTopTray[eMagazine2]->Tag)
    {
        iEditMotorIndex=MagArmPara->MMagSortTopTray[eMagazine2];
        asMotorName="MMag2SortTopTray";                                         //KenHsieh 20240725 : add edit tray log
    }
    else if(MotorIndexIndex==MagArmPara->MMagSortTopTray[eMagazine3]->Tag)
    {
        iEditMotorIndex=MagArmPara->MMagSortTopTray[eMagazine3];
        asMotorName="MMag3SortTopTray";                                         //KenHsieh 20240725 : add edit tray log
    }
    else
    {
        for(int i=eBinNotUse; i<eTrayCount; i++)
        {
            if(MotorIndexIndex==Auto->MAutoTray[i]->Tag)
            {
                iEditMotorIndex=Auto->MAutoTray[i];
                asMotorName.sprintf("MAutoTray %d", i);                         //KenHsieh 20240725 : add edit tray log
            }
        }
    }

    asLog.sprintf("Enter Tray Edit Form - %s", asMotorName);                    //KenHsieh 20240725 : add edit tray log
    RecordProcess(asLog);

    bSetBin=_bSetBin;
    ICType=HAS_IC;
    TrayEditForm->ShowModal();
}
//==============================================================================
void __fastcall TTrayEditForm::btnManualInputClick(TObject *Sender)
{
    int iFromY, iFromX;
    iFromX=atoi(edtXPos->Text.c_str());
    iFromY=atoi(edtYPos->Text.c_str());
    if(iFromX<0 || iFromX>iEditMotorIndex->Tray.XItem)
    {
        ShowRecordMessage("X parameter error");
        return;
    }
    if(iFromY<0 || iFromY>iEditMotorIndex->Tray.YItem)
    {
        ShowRecordMessage("X parameter error");
        return;
    }

    for(int j=0; j<iEditMotorIndex->Tray.YItem; j++)
    {
        for(int i=0; i<iEditMotorIndex->Tray.XItem; i++)
        {
            if(((j+1)<iFromY) || ((j+1)==iFromY && (i+1)<iFromX))
            {
                iEditBackTray[i][j]=0;
                iEditMotorIndex->SetTraySingleData(i, j, NULL_IC);
            }
            else
            {
                iEditBackTray[i][j]=1;
                iEditMotorIndex->SetTraySingleData(i, j, ICType);
            }
        }
    }
    Close();
}
//==============================================================================
void __fastcall TTrayEditForm::edtXPosClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TLabeledEdit*)Sender, N_INTEGER, 0, true, 0, iEditMotorIndex->Tray.XItem-1);
}
//==============================================================================
void __fastcall TTrayEditForm::edtYPosClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TLabeledEdit*)Sender, N_INTEGER, 0, true, 0, iEditMotorIndex->Tray.YItem-1);
}
//==============================================================================
void __fastcall TTrayEditForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
    bSetBin=false;
}
//==============================================================================





