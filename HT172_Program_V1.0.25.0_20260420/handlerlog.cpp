//==============================================================================
// Version & Date  :V1.02 2010/07/08
// Programmer      :Landam
// Objective     :Opration Log
//==============================================================================
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "handlerlog.h"
AnsiString Space_Num(AnsiString S,int n);

_fastcall TMyLog::TMyLog()
{
      fName="";
      sUser="";
      sFolder="";
      Delimiter="\t";
}

void TMyLog::Save_Log()
{
    TDateTime tt= Now();
    Word SystemYear,SystemMonth,SystemDate;
    DecodeDate(tt, SystemYear, SystemMonth, SystemDate);
    AnsiString str1,str2;

    str1.sprintf("%04d%s%02d%s",SystemYear,"_",SystemMonth,"\\");
    str2.sprintf("%s%04d%s%02d%s%02d%s","log_",SystemYear,"_",SystemMonth,"_",SystemDate,".ini");

    FILE * pFile;
    AnsiString fp="",s_cFolder;
    if (sFolder=="")
        sFolder.sprintf("%s%s", ExtractFilePath(Application->ExeName),"Log\\");

    if (sFolder.SubString(sFolder.Length(),1)!="\\")
        sFolder=sFolder+"\\";

    s_cFolder= sFolder + str1;
    fp= s_cFolder + str2;

    if(!(DirectoryExists(s_cFolder)))
        ForceDirectories(s_cFolder);

    pFile = fopen (fp.c_str() ,"a");
    if (pFile!=NULL)
    {
        AnsiString tmps;
        for (unsigned i=0;i<loglist.size();++i)
        {
          tmps=loglist[i]  + "\n";
          fputs (tmps.c_str(),pFile);

          if (i== loglist.size()-1)
             fputs ("\n",pFile);

        }
         fclose (pFile);
    }

    //----------------clear_log folder  ----------------------------------
    if (SystemMonth<=2)
    {
       SystemMonth=SystemMonth + 10;
       SystemYear--;
    }
    else
      SystemMonth=SystemMonth-2;

    str1.sprintf("%04d%s%02d",SystemYear,"_",SystemMonth);
    s_cFolder= sFolder + str1;

    if((DirectoryExists(s_cFolder)))
    {
        int len = _tcslen(s_cFolder.c_str());
        TCHAR *pszFrom = new TCHAR[len+2];
        _tcscpy(pszFrom, s_cFolder.c_str());
        pszFrom[len] = 0;
        pszFrom[len+1] = 0;

        SHFILEOPSTRUCT      opStr;

        opStr.hwnd          = NULL;
        opStr.pTo           = NULL;
        opStr.wFunc         = FO_DELETE;
        opStr.fFlags        = FOF_SILENT  | FOF_NOCONFIRMATION;
        opStr.pFrom         = pszFrom;
        SHFileOperation(&opStr);
        delete [] pszFrom;
    }
    //-------------end of clear log-----------------------------------------------
}
//---------------------------------------------------------------------------------
void TMyLog::Find_Parent(TObject *PCtrl)
{
   TForm          *FrmPtr     = dynamic_cast <TForm          *>(PCtrl);
   TWinControl    *pC         = dynamic_cast <TWinControl    *>(PCtrl);
   if (FrmPtr==NULL && pC!=NULL)
       Find_Parent(pC->Parent);
   else if (FrmPtr!=NULL)
       fName=FrmPtr->Name;
}
//---------------------------------------------------------------------------------
void TMyLog::Init_Log (TObject *PCtrl,AnsiString  tmpUser,AnsiString tmpFolder)
{
  if (tmpFolder!="")
      sFolder=tmpFolder;

  sUser=tmpUser;
  loglist.clear();
  TForm       *FrmPtr     = dynamic_cast <TForm       *>(PCtrl);

   if (FrmPtr!=NULL )
      fName= FrmPtr->Name;
   else
      Find_Parent(PCtrl);

  if (dynamic_cast <TWinControl *>(PCtrl) != NULL)
  {
      Control_Log_Value((TWinControl *)PCtrl,true);

  }
}
//---------------------------------------------------------------------------------
void TMyLog::Comp_Log(TObject *PCtrl)
{
   if (dynamic_cast <TWinControl *>(PCtrl) != NULL)
   {
      Control_Log_Value ((TWinControl *)PCtrl,false);
      Save_Log();
   }
}
//---------------------------------------------------------------------------------
void TMyLog::Control_Log_Value( TWinControl  *PCtrl,bool bInit)
{
//    TPanel      *PanelPtr   = dynamic_cast <TPanel      *>(PCtrl);
//    TTabSheet   *ShtPtr     = dynamic_cast <TTabSheet   *>(PCtrl);
//    TForm       *FrmPtr     = dynamic_cast <TForm       *>(PCtrl);
//    TGroupBox   *PGroupBox  = dynamic_cast <TGroupBox   *>(PCtrl);
//    TScrollBox  *ScrBox     = dynamic_cast <TScrollBox  *>(PCtrl);
    TRadioGroup *RadGup     = dynamic_cast <TRadioGroup *>(PCtrl);
    if (RadGup!=NULL)
    {
       AnsiString tmps;
       if (RadGup->ItemIndex!=-1)
           tmps=RadGup->Items->Strings[RadGup->ItemIndex];
       else
           tmps=" ";

       if (bInit)
           mapLogValue[fName+ "_" + RadGup->Name]=AnsiString (RadGup->ItemIndex) ;
       else
           Compare_Diff(fName+ "_" + RadGup->Name,AnsiString (RadGup->ItemIndex),tmps);
    }
    else
    {

        for (int iP = 0;iP < PCtrl->ControlCount;iP ++)
        {

            TControl *P = PCtrl->Controls[iP];

            if (dynamic_cast <TWinControl *>(P) != NULL )
            {
                Control_Log_Value((TWinControl *) P,bInit);      // recursive
            }

            //目前只記錄這些元件內容
            TScrollBar      *PScrollBar   = dynamic_cast <TScrollBar    *>(P);
            TTrackBar       *PTrackBar    = dynamic_cast <TTrackBar     *>(P);
            TCheckBox       *PCheckBox    = dynamic_cast <TCheckBox     *>(P);
            TRadioButton    *PRadioButton = dynamic_cast <TRadioButton  *>(P);
            TComboBox       *PTComboBox   = dynamic_cast <TComboBox     *>(P);
            TCustomEdit     *PTEdit       = dynamic_cast <TCustomEdit   *>(P);

            AnsiString sValue="NULL",sCaption=" ";
            if (PScrollBar != NULL)
                sValue=AnsiString(PScrollBar->Position);
            else if (PTrackBar != NULL)
                sValue=AnsiString(PTrackBar->Position);
            else if (PCheckBox != NULL)
            {
                sCaption=PCheckBox->Caption.Trim();
                sValue=(PCheckBox->Checked ? "1":"0");
            }
            else if (PRadioButton != NULL)
            {
                sCaption=PRadioButton->Caption.Trim();
                sValue=(PRadioButton->Checked ? "1":"0");
            }
            else if (PTComboBox != NULL)
                sValue=AnsiString (PTComboBox->Text);
            else if (PTEdit != NULL)
                sValue=  PTEdit->Text;


            if (sValue!="NULL")
            {
               if (bInit)
                   mapLogValue[fName+ "_" + P->Name]=sValue;
               else
                   Compare_Diff(fName+ "_" + P->Name,sValue,sCaption);
            }

         }
    }
}
//----------------------------------------------------------------------------------
bool TMyLog::Do_Log(TObject *PCtrl,AnsiString  tmpUser,AnsiString tmpFolder)
{
    bool bRetrun=false;
    if (tmpFolder!="")
        sFolder=tmpFolder;

    sUser=tmpUser;
    loglist.clear();
    TForm       *FrmPtr     = dynamic_cast <TForm       *>(PCtrl);

    if (FrmPtr!=NULL )
      fName= FrmPtr->Name;
    else
      Find_Parent(PCtrl);

    if (dynamic_cast <TWinControl *>(PCtrl) != NULL)
    {
        Control_Log_Value((TWinControl *)PCtrl,false);
        if (loglist.size()>0)
        {
            Save_Log();
            bRetrun=true;
        }
    }
    return bRetrun;
}
//---------------------------------------------------------------------------
void TMyLog::Compare_Diff(AnsiString Sfind,AnsiString Svalue,AnsiString sCaption)
{
    std::map<AnsiString,AnsiString >::iterator mapIter;
    mapIter=mapLogValue.find(Sfind);
    if(mapIter!=mapLogValue.end())
    {
        if (Svalue!=mapIter->second)
        {
            AnsiString tmps,s1;
            tmps=mapIter->second + " => " + Svalue ;
            // s1.sprintf("%-16s%s%-26s%s%-22s%s%s%-22s%s",sUser,Delimiter,Sfind,Delimiter,mapIter->second,Delimiter," => ", Svalue,Delimiter);
            s1.sprintf("%-16s%s%-26s%s%-22s%s%-22s%s%-32s",sUser,Delimiter,Sfind,Delimiter,tmps,Delimiter,AnsiString (Now()),Delimiter, sCaption);

            loglist.push_back(s1);
            //loglist.push_back(Space_Num(sUser,16)+Delimiter+ Sfind + Space_Num(Sfind,26)+Delimiter + tmps + Space_Num(tmps,22)+Delimiter);
            mapIter->second=Svalue;
        }
    }
    else
    {
        mapLogValue[Sfind]=Svalue;
    }
}
//---------------------------------------------------------------------------
AnsiString Space_Num(AnsiString S,int n)
{
   if (S.Length()>n)
       return "";

   AnsiString S1="                                                          ";
   return S1.SubString(1,n-S.Length()+1);

}
