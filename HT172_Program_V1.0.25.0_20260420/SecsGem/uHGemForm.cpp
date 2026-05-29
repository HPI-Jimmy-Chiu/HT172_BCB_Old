#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

#include "uHGemForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfHGem *fHGem;
//---------------------------------------------------------------------------
__fastcall TfHGem::TfHGem(TComponent* Owner)
    : TForm(Owner)
{
    bReportSECS_GEM_Message=false;
    SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
}
//---------------------------------------------------------------------------
//S2F33 //pig 2013.04.30 SECS_GEM start
void TfHGem::SaveSECSDefineReportData(int RPTID,int VID,int VIDSum)
{
    AnsiString S,aRPTID,aVID;
    char str[256];

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_VID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    aRPTID="RPTID_"+AnsiString(RPTID);
    aVID="VID_"+AnsiString(VIDSum);
    SaveSECSDefineReportRPTData(RPTID);
    INIFile->WriteInteger(aRPTID,aVID,VID);
    SECS_GEM_VID.clear();
    SECS_GEM_VID.push_back(VID);
    SECS_GEM_RPTID[RPTID] = SECS_GEM_VID;

    delete INIFile;
}
//==============================================================================
void TfHGem::SaveSECSDefineReportRPTData(int RPTID)
{
    AnsiString S;
    char str[256];
    int iCount=0;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_RPTID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    while(1)
    {
        if( INIFile->ReadInteger("RPTID",AnsiString(iCount),0) == RPTID )
        {
            break;
        }
        if( INIFile->ReadInteger("RPTID",AnsiString(iCount),0)== 0 )
        {
            INIFile->WriteInteger( "RPTID",AnsiString(iCount),RPTID );
            break;
        }
        else
        {
            iCount++;
        }
    }

    delete INIFile;

}
//==============================================================================
void TfHGem::ReadSECSDefineReportData()
{
    AnsiString S,aRPTID,aVID;
    char str[256];
    int i,iCount=0;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_VID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    for( i=0;i<GEM_RPTIDCount;i++ )
    {
        aRPTID="RPTID_"+AnsiString( ReadSECSDefineReportRPTData(i) );
        if( ReadSECSDefineReportRPTData(i)!=0 )
        {
            SECS_GEM_VID.clear();
            while(1)
            {
                aVID="VID_"+AnsiString(iCount);
                if( INIFile->ReadInteger(aRPTID,aVID,0)!=0 )
                {
                    SECS_GEM_VID.push_back( INIFile->ReadInteger(aRPTID,aVID,0) );
                    SECS_GEM_RPTID[ReadSECSDefineReportRPTData(i)] = SECS_GEM_VID;
                    iCount++;
                }
                else
                {
                    iCount=0;
                    break;
                }
            }
        }
    }

    delete INIFile;
}
//==============================================================================
int TfHGem::ReadSECSDefineReportRPTData(int index)
{
    AnsiString S;
    char str[256];
    int ret;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_RPTID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    ret=INIFile->ReadInteger("RPTID",AnsiString(index),0);

    delete INIFile;

    return ret;
}
//==============================================================================
bool TfHGem::GetSECSDefineReportRPTIDRepeat(int RPTID)                                  //判斷是否有已定義的RPTID For S2F33
{
    map < int , vector < int > >::iterator pIter = SECS_GEM_RPTID.begin();

    while( pIter != SECS_GEM_RPTID.end() )
    {
        if( pIter->first == RPTID )
        {
            return true;
        }
        pIter++;
    }
    return false;

}
//==============================================================================
void TfHGem::SendSECSVIDForDefineReportRPTID(int RPTID)                                 //S6F19
{
    int iVSize,i;

    map < int , vector < int > >::iterator pIter = SECS_GEM_RPTID.begin();

    while( pIter != SECS_GEM_RPTID.end() )
    {
        if( pIter->first == RPTID )
        {
            vector < int > temp;
            temp = pIter->second;

            iVSize = temp.size();

            HGem->DataItemOut(iVSize,HType.LIST_TYPE,NULL);

            for( i = 0 ; i < iVSize ; i++ )
            {
                HGem->DataItemOutSVData( AnsiString(temp[i]) );
            }
            return;
        }
        pIter++;
    }
}
//==============================================================================
void TfHGem::InitialSECSDefineReport()         //Machine Define event report            //pig 2013.04.29 SECS_GEM
{
    int i;
    //RPTID

    //Machine Status
    SECS_GEM_VID.clear();
    SECS_GEM_VID.push_back( 111 );
    SECS_GEM_RPTID[1] = SECS_GEM_VID;

    //Machine Running Time ,Power Time
    SECS_GEM_VID.clear();
    SECS_GEM_VID.push_back( 105 );
    SECS_GEM_VID.push_back( 106 );
    SECS_GEM_RPTID[2] = SECS_GEM_VID;

    //Recipe
    SECS_GEM_VID.clear();
    SECS_GEM_VID.push_back( 112 );
    SECS_GEM_RPTID[3] = SECS_GEM_VID;

    //Loader Count, OutPass Count, OutPass Yield, OutFail Count, OutFail Yield
    SECS_GEM_VID.clear();
    SECS_GEM_VID.push_back( 120 );
    SECS_GEM_VID.push_back( 122 );
    SECS_GEM_VID.push_back( 128 );
    SECS_GEM_VID.push_back( 123 );
    SECS_GEM_VID.push_back( 129 );
    SECS_GEM_RPTID[4] = SECS_GEM_VID;

    // Recipe Constant
    SECS_GEM_VID.clear();
    SECS_GEM_VID.push_back( 0 );                                       //0:ALL
    SECS_GEM_RPTID[5] = SECS_GEM_VID;

    //Machine Action
    for(i=11;i<=27;i++)
    {
        SECS_GEM_VID.clear();
        SECS_GEM_VID.push_back( 0 );                                       //0:ALL
        SECS_GEM_RPTID[i] = SECS_GEM_VID;
    }

    //CEID

    SECS_GEM_RPTID_KEY.clear();
    SECS_GEM_RPTID_KEY.push_back( 1 );
    SECS_GEM_CEID[1] = SECS_GEM_RPTID_KEY;

    SECS_GEM_RPTID_KEY.clear();
    SECS_GEM_RPTID_KEY.push_back( 2 );
    SECS_GEM_CEID[2] = SECS_GEM_RPTID_KEY;

    SECS_GEM_RPTID_KEY.clear();
    SECS_GEM_RPTID_KEY.push_back( 3 );
    SECS_GEM_CEID[3] = SECS_GEM_RPTID_KEY;

    SECS_GEM_RPTID_KEY.clear();
    SECS_GEM_RPTID_KEY.push_back( 4 );
    SECS_GEM_CEID[4] = SECS_GEM_RPTID_KEY;

    SECS_GEM_RPTID_KEY.clear();
    SECS_GEM_RPTID_KEY.push_back( 5 );
    SECS_GEM_CEID[5] = SECS_GEM_RPTID_KEY;

    //Machine Action
    for(i=11;i<=27;i++)
    {
        SECS_GEM_RPTID_KEY.clear();
        SECS_GEM_RPTID_KEY.push_back( i );
        SECS_GEM_CEID[i] = SECS_GEM_RPTID_KEY;
    }


    //Enable
    for( i=1;i<=100;i++ )                                                    //Machine Define 50個
    {
        if(i<=50)
            SaveSECSCEIDEnableOrDisable(i,true);
        ReadSECSCEIDEnableOrDisable(i);
    }

}
//==============================================================================
void TfHGem::DeleteSECSDefineReportData( int RPTID, int iMode )                         //0:刪除單一個RPTID 1:刪除全部RPTID
{
    AnsiString S,aRPTID;
    char str[256];

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_VID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    aRPTID="RPTID_"+AnsiString(RPTID);

    DeleteSECSDefineReportRPTData(RPTID,iMode);
    if( iMode==0 )
    {
        INIFile->EraseSection(aRPTID);
    }
    else
    {
        DeleteFile(str);
    }

    delete INIFile;
}
//==============================================================================
void TfHGem::DeleteSECSDefineReportRPTData( int RPTID , int iMode )                     //0:刪除單一個RPTID 1:刪除全部RPTID
{
    AnsiString S;
    char str[256];
    int iCount=0,iSize;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_RPTID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    if( iMode==0 )
    {
        while(1)
        {
            if( INIFile->ReadInteger("RPTID",AnsiString(iCount),0)== RPTID )
            {
                INIFile->DeleteKey("RPTID",AnsiString(iCount));
                SECS_GEM_RPTID.erase(RPTID);
                break;
            }
            else
            {
                iCount++;
            }
        }
    }
    else
    {
        while(1)
        {
            if( INIFile->ReadInteger("RPTID",AnsiString(iCount),0) !=0 )
            {
                SECS_GEM_RPTID.erase( INIFile->ReadInteger("RPTID",AnsiString(iCount),0) );
                iSize = SECS_GEM_RPTID.size();
            }
            if( iSize <= GEM_DefineEventReport )          //目前機台定義的EVENT數量, 不可刪除
                break;
            iCount++;
        }
        DeleteFile(str);
    }

    delete INIFile;
}
//==============================================================================
void TfHGem::ShowRecipeName(AnsiString aMane)
{
    AnsiString S;

    S = aMane;
    do
    {
        S=S.Delete(1,S.Pos("\\"));
    }while ( S.Pos("\\")!=0 );

    fMain->cb_WorkFile->Text = S ;
}
//==============================================================================//
void TfHGem::SaveSECSDefineReportLinkData(int CEID,int RPTID,int RPTIDSum)              //S2F35
{
    AnsiString S,aCEID,aRPTID;
    char str[256];

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_RPTID_KEY.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    aCEID="CEID_"+AnsiString(CEID);
    aRPTID="RPTID_"+AnsiString(RPTIDSum);
    SaveSECSDefineReportCEIDLinkData(CEID);
    INIFile->WriteInteger(aCEID,aRPTID,RPTID);
    SECS_GEM_RPTID_KEY.clear();
    SECS_GEM_RPTID_KEY.push_back(RPTID);
    SECS_GEM_CEID[CEID] = SECS_GEM_RPTID_KEY;

    delete INIFile;
}
//==============================================================================
void TfHGem::SaveSECSDefineReportCEIDLinkData(int CEID)
{
    AnsiString S;
    char str[256];
    int iCount=0;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_CEID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    while(1)
    {
        if( INIFile->ReadInteger("CEID",AnsiString(iCount),0) == CEID )
        {
            break;
        }
        if( INIFile->ReadInteger("CEID",AnsiString(iCount),0)== 0 )
        {
            INIFile->WriteInteger( "CEID",AnsiString(iCount),CEID );
            SaveSECSCEIDEnableOrDisable(CEID,false);                            //Initial False
            break;
        }
        else
        {
            iCount++;
        }
    }

    delete INIFile;

}
//==============================================================================
void TfHGem::ReadSECSDefineReportLinkData()
{
    AnsiString S,aCEID,aRPTID;
    char str[256];
    int i,j,iCount=0;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_RPTID_KEY.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    for( i=0;i<GEM_RPTIDCount;i++ )                                             //Max Count 100
    {
        aCEID="CEID_"+AnsiString( ReadSECSDefineReportCEIDLinkData(i) );
        if( ReadSECSDefineReportCEIDLinkData(i)!=0 )
        {
            SECS_GEM_RPTID_KEY.clear();
            while(1)
            {
                aRPTID="RPTID_"+AnsiString(iCount);
                if( INIFile->ReadInteger(aCEID,aRPTID,0)!=0 )
                {
                    SECS_GEM_RPTID_KEY.push_back( INIFile->ReadInteger(aCEID,aRPTID,0) );
                    SECS_GEM_CEID[ReadSECSDefineReportCEIDLinkData(i)] = SECS_GEM_RPTID_KEY;
                    iCount++;
                }
                else
                {
                    iCount=0;
                    break;
                }
            }
        }
    }

    delete INIFile;
}
//==============================================================================
int TfHGem::ReadSECSDefineReportCEIDLinkData(int index)
{
    AnsiString S;
    char str[256];
    int ret;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_CEID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    ret=INIFile->ReadInteger("CEID",AnsiString(index),0);

    delete INIFile;

    return ret;
}
//==============================================================================
bool TfHGem::GetSECSDefineReportCEIDRepeat(int CEID)                                    //判斷是否有已定義的CEID For S2F35
{
    map < int , vector < int > >::iterator pIter = SECS_GEM_CEID.begin();

    while( pIter != SECS_GEM_CEID.end() )
    {
        if( pIter->first == CEID )
        {
            return true;
        }
        pIter++;
    }
    return false;

}
//==============================================================================
void TfHGem::DeleteSECSDefineReportLinkData( int CEID, int iMode )                      //0:刪除單一個CEID 1:刪除全部CEID
{
    AnsiString S,aCEID;
    char str[256];

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_RPTID_KEY.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    aCEID="CEID_"+AnsiString(CEID);

    DeleteSECSDefineReportCEIDLinkData(CEID,iMode);
    if( iMode==0 )
    {
        INIFile->EraseSection(aCEID);
    }
    else
    {
        DeleteFile(str);
    }

    delete INIFile;
}
//==============================================================================
void TfHGem::DeleteSECSDefineReportCEIDLinkData( int CEID , int iMode )                //0:刪除單一個CEID 1:刪除全部CEID
{
    AnsiString S;
    char str[256];
    int iCount=0,iSize;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_CEID.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    if( iMode==0 )
    {
        while(1)
        {
            if( INIFile->ReadInteger("CEID",AnsiString(iCount),0)== CEID )
            {
                INIFile->DeleteKey("CEID",AnsiString(iCount));
                SECS_GEM_CEID.erase(CEID);
                break;
            }
            else
            {
                iCount++;
            }
        }
    }
    else
    {
        while(1)
        {
            if( INIFile->ReadInteger("CEID",AnsiString(iCount),0) !=0 )
            {
                SECS_GEM_CEID.erase( INIFile->ReadInteger("CEID",AnsiString(iCount),0) );
                iSize = SECS_GEM_CEID.size();
            }
            if( iSize <= GEM_DefineEventReport )                                //目前機台定義的EVENT數量, 不可刪除
                break;
            iCount++;
        }
        DeleteFile(str);
    }

    delete INIFile;
}
//==============================================================================
void TfHGem::SECS_RPTIDForDefineReportCEIDEnable(int iCEID,bool bEnable)                //回傳已LINK CEID 的RPTID
{
    int iVSize,i;

    map < int , vector < int > >::iterator pIter = SECS_GEM_CEID.begin();

    while( pIter != SECS_GEM_CEID.end() )
    {
        if( pIter->first == iCEID )
        {
            vector < int > temp;
            temp = pIter->second;                //RPTID

            iVSize = temp.size();

            SaveSECSCEIDEnableOrDisable(iCEID,bEnable);

            for( i = 0 ; i < iVSize ; i++ )
            {
                SECS_VIDForDefineReportRPTIDEnable( iCEID,temp[i],bEnable );
            }
            return;
        }
        pIter++;
    }
}
//==============================================================================
void TfHGem::SECS_VIDForDefineReportRPTIDEnable(int iCEID,int iRPTID,bool bEnable)
{
    int iVSize2,i,iVID;

    map < int , vector < int > >::iterator pIter2 = SECS_GEM_RPTID.begin();

    while( pIter2 != SECS_GEM_RPTID.end() )
    {
        if( pIter2->first == iRPTID )
        {
            vector < int > temp2;
            temp2 = pIter2->second;                //RPTID

            iVSize2 = temp2.size();

            for( i = 0 ; i < iVSize2 ; i++ )
            {                     //[CEID][RPTID][VID]
                HSys.SECS.bEvenLink[iCEID][iRPTID][temp2[i]]=bEnable;
                HGem->GEM_bEvenLink[iCEID][iRPTID][temp2[i]]=bEnable;
            }
            return;
        }
        pIter2++;
    }
}
//==============================================================================
void TfHGem::SaveSECSCEIDEnableOrDisable(int iCEID,bool bEnable)
{
    AnsiString S;
    char str[256];

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_CEID_Enable.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    INIFile->WriteBool("CEID",AnsiString(iCEID),bEnable);

    delete INIFile;

}
//==============================================================================
bool TfHGem::ReadSECSCEIDEnableOrDisable(int iCEID)
{
    AnsiString S;
    char str[256];
    bool bEnable;

    S=HSys.CurrentDir+"\\SECS\\SYSTEM";
    sprintf(str,"%s\\secs_gem_CEID_Enable.ini",S.c_str());
    TIniFile *INIFile = new TIniFile(str);

    bEnable=INIFile->ReadBool("CEID",AnsiString(iCEID),false);

    SECS_RPTIDForDefineReportCEIDEnable(iCEID,bEnable);

    delete INIFile;

    return bEnable;
}
//==============================================================================
void TfHGem::DoSendSECSVIDForDefineReportCEID(int CEID)                                   //S6F19
{
    int iVSize,iVSize2,i,j,DATAID=1;
    AnsiString S;

    if( ReadSECSCEIDEnableOrDisable(CEID)==false )      return;

    map < int , vector < int > >::iterator pIter = SECS_GEM_CEID.begin();

    HGem->InitLocalHead(6,11,1);
    HGem->DataItemOut(3,HType.LIST_TYPE,NULL);
    HGem->DataItemOut(1,HType.UINT_4_TYPE,&DATAID);
    HGem->DataItemOut(1,HType.UINT_4_TYPE,&CEID);
    HGem->DataItemOut(1,HType.LIST_TYPE,NULL);

    while( pIter != SECS_GEM_CEID.end() )
    {
        if( pIter->first == CEID )
        {
            vector < int > temp;
            temp = pIter->second;

            iVSize = temp.size();

            HGem->DataItemOut(iVSize,HType.LIST_TYPE,NULL);

            for( i = 0 ; i < iVSize ; i++ )
            {
                map < int , vector < int > >::iterator pIter2 = SECS_GEM_RPTID.begin();

                HGem->DataItemOut(2,HType.LIST_TYPE,NULL);
                HGem->DataItemOut(1,HType.UINT_4_TYPE,&temp[i]);

                while( pIter2 != SECS_GEM_RPTID.end() )
                {
                    if( pIter2->first == temp[i] )
                    {
                        vector < int > temp2;
                        temp2 = pIter2->second;

                        iVSize2 = temp2.size();

                        HGem->DataItemOut(iVSize2,HType.LIST_TYPE,NULL);
                        if( CEID>=11 && CEID<=27 )                              //針對機台動作流程
                        {
                            SECS_MachineActionEventReportName(CEID);
                        }
                        else
                        {
                            for( j = 0 ; j < iVSize2 ; j++ )
                            {
                                HGem->DataItemOutSVData( AnsiString(temp2[j]) );
                            }
                        }
                    }
                    pIter2++;
                }

            }
        }
        pIter++;
    }
    HGem->SendLocalData();
}
//==============================================================================
void TfHGem::SECS_MachineActionEventReportName(int iCEID)                         //回傳機台動作訊息名稱
{

    AnsiString aNeme[]=
    {
        "START LOADER BOAT",
        "LOADER BOAT READY",
        "START INCCD SCAN",
        "INCCD SCAN FINISH",
        "START TRANSPORT TO PRESSURE",
        "PRESSURE READY",
        "START PICK AND PLACE",
        "PICK AND PLACE FINISH",
        "START PRESSURE",
        "PRESSURE FINISH",
        "START TRANSPORT TO UNLOADER",
        "UNLOADER READY",
        "START OUTCCD SCAN",
        "OUTCCD SCAN FINISH",
        "START TRANSPORT TO UNLOADER PASS",
        "START TRANSPORT TO UNLOADER FAIL",
        "FINISH WORK",
        "",
    };
    HGem->DataItemOut( strlen(aNeme[iCEID-11].c_str()) ,HType.ASCII_TYPE,aNeme[iCEID-11].c_str() );

}
//==============================================================================
//pig 2013.05.06 SECS_GEM end


//---------------------------------------------------------------------------
//pig 2012.10.03 SECS_GEM start
//int  GemCallBack()
//{
//    return fMain->MySFCode();
//}
int  TfHGem::MySFCode()
{
    unsigned char Command,HCACK,Type,CPACK;
    char CommandStr[256],GEM_RPTID[64],GEM_VID[100][64],MDLN[64],SOFTREV[64],GEM_CCODE[64],GEM_PPARM[64],WorkName[256];
    char str[256],t[64],t1[64];
    int ret,SVlen,i,sum,GEM_DATAID,RPTlen,VIDlen,j,k,m,GEM_CEID[100],iRPTID[100],CEIDlen,EClen,len,iVID,iGEM_RPTID,iCEID;
    float fPassYield,fFailYield;
    AnsiString S,S1,S2,GEMSVData,SN,SN2;
    bool bSV,bOk,bflag,GEM_CEED;
    GEM_OriginalWF=fMain->cb_WorkFile->Text;                                       //pig 2013.04.18
    if(HGem->Remote.MessageID_S==2 && HGem->Remote.MessageID_F==41 ) //  S10,F3 Terminal Display, Single (VTN)
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)      // 需要補充多重 Command
        {
            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);
            S=CommandStr;
            S=S.UpperCase();
            SVlen=0;
            if(S=="START")                                                      //HCACK 0=ok, 1=No Command, 2=無法立即執行, 3=至少一個參數是無效的
            {
//                if(SECS_MachineLock==false)
//                    fNote->BtnStartClick(this);
                if(SoftStart==true )
                    HCACK=0;
                else
                    HCACK=2;
            }
            else if(S=="STOP")
            {
                //fNote->BtnPauseClick(this);
                fMain->sbCleanOutClick(this);
                HCACK=0;
            }
            else if(S=="PAUSE")
            {
                fNote->BtnPauseClick(this);
//                SECS_MachineLock=true;
                HCACK=0;
            }
            else if(S=="CLEAR")
            {
                fMain->sbClearCountClick(this);
                HCACK=0;
            }
            else if(S=="RESUME")
            {
//                SECS_MachineLock=false;
                HCACK=0;
            }
            else if(S=="ABORT")
            {

            }
            else if(S=="ABORT")
            {

            }
            else if(S=="PP_SELECT")
            {
//
//                HGem->DataItemIn( 256   ,HType.ASCII_TYPE,WorkName);            //pig 2013.04.18
//                SN=WorkName;
//                SN=SN.UpperCase();
//
//                if( SN.Pos(".")!=0 )
//                {
//                    SN2=SN.Delete(SN.Length()-3,4);
//                }
//                else
//                {
//                    SN2=SN;
//                }
//                fMain->cb_WorkFile->Text=SN2;
//
//                fSetup->OpenWorkFile();                                                     // load setup data //
//                fTeach->OpenWorkFile();                                                     // load teach data //
//                fOffset->OpenWorkFile();
//                fInspectGull->LoadRecipe(SN+AnsiString(".ini"));                                                    // load offset data //
//                UpdateAllParameter();                                                       // ben add 20110803 update all //
//                SaveWorkData();
//                fOffset->sbOffsetUpdateClick(this);                                                            // save lastset.ini //
//                RecordProcess(fMain->ComboBox4->Text+" Change Work File");
//                if(strcmp(GEM_OriginalWF.c_str(),SN2.c_str())!=0)
//                {
//                    DoSendSECSVIDForDefineReportCEID(3);
//                }
//                SVlen=0;
//                HCACK=0;
            }
            else if(S=="PPDESELECT")
            {

            }
            else if(S=="PPSIGNALTOWER")
            {
                if(HGem->GetDataItemLenAndTypeAndDelete(SVlen,HType.LIST_TYPE)==1 )
                {
                    if(SVlen==0)
                    {
                        SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
                    }
                    else
                    {
                        HCACK=0;
                        SECS_GEM_PPSIGNALTOWER_CONTROL_flag=true;
                        HGem->InitLocalHead(2,42,0);    //S2F42
                        HGem->DataItemOut(2,HType.LIST_TYPE,NULL);
                        HGem->DataItemOut(1,HType.BINARY_TYPE,&HCACK);
                        HGem->DataItemOut(SVlen,HType.LIST_TYPE,NULL);
                        for(i=0;i<SVlen;i++)
                        {
                            if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1 )
                            {
                                HGem->DataItemIn( 256   ,HType.ASCII_TYPE,str);
                                HGem->DataItemIn( 1     ,HType.INT_4_TYPE,&ret);
                                S=str;
                                CPACK=0;
                                if(S=="RED")
                                    iSECS_GEM_PPSIGNALTOWER_CONTROL_RED=ret;
                                else if(S=="GREEN")
                                    iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN=ret;
                                else if(S=="YELLOW")
                                    iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW=ret;
                                else
                                    CPACK=1;
                                HGem->DataItemOut(2,HType.LIST_TYPE,NULL);
                                HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,&str);
                                HGem->DataItemOut(1,HType.BINARY_TYPE,&CPACK);
                            }
                            else
                            {
                                CPACK=3;
                                HGem->DataItemOut(2,HType.LIST_TYPE,NULL);
                                HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,&str);
                                HGem->DataItemOut(1,HType.BINARY_TYPE,&CPACK);
                                break;
                            }
                        }
                    }
                }
            }
            else if(S=="ONLINE_LOCAL")
            {
                GemBtnOnlineLocal->OnClick(this);
                HCACK=0;
            }
            else if(S=="ONLINE_REMOTE")
            {
                GemBtnOnlineRemote->OnClick(this);
                HCACK=0;
            }
            else if(S=="GO_LOCAL")
            {

            }
            else if(S=="GO_REMOTE")
            {

            }
            else
                HCACK=1;
            if(SVlen==0)
            {
//                HGem->InitLocalHead(2,42,0);    //S2F42
//                HGem->DataItemOut(2,HType.LIST_TYPE,NULL);
//                HGem->DataItemOut(1,HType.BINARY_TYPE,&HCACK);
//                HGem->DataItemOut(0,HType.LIST_TYPE,NULL);
            }
            HGem->SendLocalData();

            //HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
            /*
                0 = Acknowledge, command hasbeen performed
                1 = Command does not exist "
                2 = Cannot perform now
                3 = At least one parameter isinvalid "
                4 = Acknowledge, command willbe performed with completionsignaled later by an event
                5 = Rejected, Already inDesired Condition "
                6 = No such object exists
                7-63 Reserved "
            */

            return 1;
        }
    }
    else if(HGem->Remote.MessageID_S==100 && HGem->Remote.MessageID_F==5 ) //  S100,F5
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn( 1     ,HType.UINT_1_TYPE,&Command);
            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);
            if( Command==0)
                fNote->BtnPauseClick(this);
            else if( Command==1)
                fMain->sbClearCountClick(this);
            else if( Command==2)
                fNote->BtnStartClick(this);
        }
        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
        return 1;
    }
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==1 )
    {
        HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);
        S=CommandStr;
        S1=HSys.CurrentDir+"\\data\\";
        S2=S.SubString(1,16);
        if(CheckFileExist(S.c_str()))
        {
             HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,1);
        }
        else
        {
            if(S2==S1)
                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
            else
                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,3);
        }
        return 0;
    }
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==17 )
    {
        //_WorkFileChange(this);
    }
    else if(HGem->Remote.MessageID_S==6 && HGem->Remote.MessageID_F==19 )
    {
        if( HGem->DataItemIn( 1,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn( 1 ,HType.UINT_4_TYPE, &iGEM_RPTID );
            HGem->InitLocalHead(6,20,0);

            if( GetSECSDefineReportRPTIDRepeat(iGEM_RPTID) )
            {
                SendSECSVIDForDefineReportRPTID(iGEM_RPTID);
            }
            else
            {
                HGem->DataItemOut(0,HType.LIST_TYPE,NULL);
            }
            HGem->SendLocalData();

        }
    }
    else if(HGem->Remote.MessageID_S==2 && HGem->Remote.MessageID_F==33 )
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn( 1 ,HType.UINT_4_TYPE, &GEM_DATAID );               //DATAID
            if(HGem->GetDataItemLenAndTypeAndDelete(RPTlen,HType.LIST_TYPE)==1 )
            {
                if( RPTlen==0 )
                {
                    DeleteSECSDefineReportData(0,1);
                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
                    return 0;
                }
                for(i=0;i<RPTlen;i++)
                {
                    if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
                    {
                        HGem->DataItemIn( 1 ,HType.UINT_4_TYPE, &iGEM_RPTID );
                        iRPTID[i] = iGEM_RPTID;

                        if(HGem->GetDataItemLenAndTypeAndDelete(VIDlen,HType.LIST_TYPE)==1 )
                        {
                            bSV=false;
                            if( VIDlen==0 )       //Delete Define Report
                            {
                                DeleteSECSDefineReportData(iGEM_RPTID,0);
                                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
                                return 0;
                            }
                            else
                            {
                                if( GetSECSDefineReportRPTIDRepeat(iGEM_RPTID)==true )
                                {
                                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,3);
                                    return 3;
                                }
                            }
                            for(k=0;k<VIDlen;k++)
                            {
                                HGem->DataItemIn( 1 ,HType.UINT_4_TYPE, &iVID );
                                for( m=0;m<HGem->GetSVIDCount();m++ )
                                {
                                    S1=AnsiString(iVID);
                                    if( S1==HGem->GetSVID(m) )                  //pig 2013.04.18
                                    {
                                        bSV=true;
                                        sprintf(GEM_VID[k],"%s",HGem->GetSVID(m));
                                        SaveSECSDefineReportData(iGEM_RPTID,atoi(S1.c_str()),k);
                                        break;
                                    }
                                }
                                if(bSV==false)
                                {
                                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,4);
                                    return 4;
                                }
                            }
                        }
                    }
                }

                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);

                HGem->InitLocalHead(6,11,1);
                for(i=0;i<RPTlen;i++)
                {
                    HGem->DataItemOut(3,HType.LIST_TYPE,NULL);

                    HGem->DataItemOut(1,HType.UINT_4_TYPE,&GEM_DATAID);
                    HGem->DataItemOut(1,HType.UINT_4_TYPE,&iRPTID[i]);
                    HGem->DataItemOut(1,HType.LIST_TYPE,NULL);

                    HGem->DataItemOut(2,HType.LIST_TYPE,NULL);
                    HGem->DataItemOut(1,HType.UINT_4_TYPE,&iRPTID[i]);
                    HGem->DataItemOut(VIDlen,HType.LIST_TYPE,NULL);
                    for(j=0;j<VIDlen;j++)
                    {
                        HGem->DataItemOutSVData(GEM_VID[j]);
                    }
                    HGem->SendLocalData();
                }
                return 0;
            }
        }
        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,2);
    }
    else if(HGem->Remote.MessageID_S==2 && HGem->Remote.MessageID_F==35 )
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn( 1 ,HType.UINT_4_TYPE, &GEM_DATAID );
            if(HGem->GetDataItemLenAndTypeAndDelete(CEIDlen,HType.LIST_TYPE)==1 )
            {
                if( CEIDlen==0 )
                {
                    DeleteSECSDefineReportLinkData(0,1);
                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
                    return 0;
                }
                for(i=0;i<CEIDlen;i++)
                {
                    if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
                    {
                        HGem->DataItemIn( 1 ,HType.UINT_4_TYPE, &iCEID );         //CEID
                        GEM_CEID[i]=iCEID;

                        if(HGem->GetDataItemLenAndTypeAndDelete(RPTlen,HType.LIST_TYPE)==1 )
                        {
                            if( RPTlen==0 )                                     //Delete Define Report
                            {
                                DeleteSECSDefineReportLinkData(iCEID,0);
                                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
                                return 0;
                            }
                            else
                            {
                                if( GetSECSDefineReportCEIDRepeat(iCEID)==true )
                                {
                                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,3);
                                    return 3;
                                }
                            }
                            for(k=0;k<RPTlen;k++)
                            {
                                HGem->DataItemIn( 1 ,HType.UINT_4_TYPE, &iGEM_RPTID );
                                if( GetSECSDefineReportRPTIDRepeat(iGEM_RPTID)==false )
                                {
                                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,5);
                                    return 5;
                                }
                                SaveSECSDefineReportLinkData(iCEID,iGEM_RPTID,k);
                            }
                        }
                    }
                }
                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
                return 0;
            }
        }
        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,2);
    }
    else if(HGem->Remote.MessageID_S==2 && HGem->Remote.MessageID_F==37 )
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn(1,HType.BOOLEAN_TYPE,&GEM_CEED);

            if(HGem->GetDataItemLenAndTypeAndDelete(CEIDlen,HType.LIST_TYPE)==1 )
            {
                if( CEIDlen == 0 )
                {
                    for( j=0;j<100;j++ )
                    {
                        if( ReadSECSDefineReportCEIDLinkData(j)!=0 )
                        {
                            SECS_RPTIDForDefineReportCEIDEnable( ReadSECSDefineReportCEIDLinkData(j),GEM_CEED );
                        }
                    }
                }
                for(i=0;i<CEIDlen;i++)
                {
                    HGem->DataItemIn(1,HType.UINT_4_TYPE,&k);
                    GEM_CEID[i]= k;
                    if( GetSECSDefineReportCEIDRepeat(k)==false )
                    {
                        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,1);
                        return 1;
                    }

                    SECS_RPTIDForDefineReportCEIDEnable( GEM_CEID[i],GEM_CEED );
                }
                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
                return 0;
            }
        }
        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,2);
        return 2;
    }
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==23 )
    {
        if( HGem->DataItemIn( 4,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);
            S=CommandStr;
            if(CheckFileExist(S.c_str()))
            {
                HGem->DataItemIn( 256   ,HType.ASCII_TYPE,MDLN);
                HGem->DataItemIn( 256   ,HType.ASCII_TYPE,SOFTREV);
                S1=MDLN;
                S2=SOFTREV;
                if(S1!=GEM_MachineName)
                {
                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,1);
                    return 0;
                }
                if(S2!=GEM_MachineVer)
                {
                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,2);
                    return 0;
                }
                if( HGem->DataItemIn( GEM_Recipe_Constant,HType.LIST_TYPE,NULL)==1 )
                {
                    for(i=0;i<GEM_Recipe_Constant;i++)
                    {
                        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
                        {
                            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,GEM_CCODE);
                            HGem->DataItemIn( 1,HType.LIST_TYPE,NULL);
                            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,GEM_PPARM);
                            if( WriteRecipeData(CommandStr,GEM_CCODE,GEM_PPARM) ==1 )
                            {
                                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,3);
                                return 0;
                            }
                        }
                    }
                    HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
                    return 0;
                }
            }
        }
        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,5);
    }
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==26 )
    {
        if( HGem->DataItemIn( 4,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);                  //PPID
            S=CommandStr;
            S1=S.SubString(1,(S.Pos(".")-1));
            if(CheckFileExist(S.c_str()))
            {
                HGem->DataItemIn( 256   ,HType.ASCII_TYPE,MDLN);
                HGem->DataItemIn( 256   ,HType.ASCII_TYPE,SOFTREV);

                if( HGem->DataItemIn( GEM_Recipe_Constant,HType.LIST_TYPE,NULL)==1 )
                {
                    for(i=0;i<GEM_Recipe_Constant;i++)
                    {
                        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
                        {
                            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,GEM_CCODE);
                            HGem->DataItemIn( 1,HType.LIST_TYPE,NULL);
                            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,GEM_PPARM);
                            WriteRecipeData(CommandStr,GEM_CCODE,GEM_PPARM);
                        }
                    }
                    if(S1.AnsiCompare(fMain->cb_WorkFile->Text))
                    {
                        fSetup->OpenWorkFile();
                    }
                }
            }
        }
    }

    else if(HGem->Remote.MessageID_S==2 && HGem->Remote.MessageID_F==15 )
    {
        if(HGem->GetDataItemLenAndTypeAndDelete(EClen,HType.LIST_TYPE)==1 )
        {
            for(i=0;i<EClen;i++)
            {
                if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
                {
                    HGem->GetDataItemLenAndType(len,Type);
                    HGem->DataItemIn( len ,Type,&Command);
                    HGem->GetDataItemLenAndType(len,Type);
                    if(Type==HType.BOOLEAN_TYPE)
                    {
                        HGem->DataItemIn( len ,Type, &bflag );
//                        sum=bflag;
                    }
                    else
                    {
                        HGem->DataItemIn( len ,Type, &ret );
//                        sum=ret;
                    }
                    S=Command;
//                    fSpeed->SetHostNewValueforSECS(S,sum);
//                    SetHostNewValueforSECS(S,sum);
                }
            }
        }
    }

    return 0;
}
//---------------------------------------------------------------------------
//int ListTest[3]={150,151,156};
//int SubListTest[3]={100,160};

void TfHGem::GemInitialDefault()
{
    GemSBSetup->OnClick=HGem->GemSBSetupClick;
    HGem->CurrentDirectory=HSys.CurrentDir;
    HGem->SetDisplayPtr(GemCommMemo);
//    HGem->MoveCheckCallBack=GemCallBack;
    HGem->SetTerminalWindows(GemTerminalWindow);
//    HGem->AcceptHostOnLineRequest=GemCheckBoxAcceptHostOnlineRequest;
//    HGem->GemTimeLabeTime=GemTimeString;
//    HGem->ckShowBinaryData=GemCheckBoxShowBinary;
//    HGem->GemCheckBoxShowHeadInformation=GemCheckBoxShowHeadInformation;
//    HGem->SFCodeResponseList=GemSendWaitSFResoponse;

    HGem->SECSConnectionState=SECSConnectionState;                              //SECS Connection State

    HGem->GEMCommunicatingState=GEMCommunicatingState;                          //Communicating State start
    HGem->BtnEnableComm=BtnEnableComm;
    BtnEnableComm->OnClick=HGem->BtnEnableCommClick;
    HGem->BtnDisableComm=BtnDisableComm;
    BtnDisableComm->OnClick=HGem->BtnDisableCommClick;
    HGem->EnableOrDisablePtr=GemEnableOrDisable;                                //Communicating State end

    HGem->GemPanelControlState=GemPanelControlState;                            //Control State <SV:4> start
    HGem->GemBtnOnlineRequest=GemBtnOnlineRequest;
    GemBtnOnlineRequest->OnClick=HGem->GemBtnOnlineRequestClick;
    HGem->GemBtnOfflineRequest=GemBtnOfflineRequest;
    GemBtnOfflineRequest->OnClick=HGem->GemBtnOfflineRequestClick;
    HGem->GemBtnOnlineRemote=GemBtnOnlineRemote;
    GemBtnOnlineRemote->OnClick=HGem->GemBtnOnlineRemoteClick;
    HGem->GemBtnOnlineLocal=GemBtnOnlineLocal;
    GemBtnOnlineLocal->OnClick=HGem->GemBtnOnlineLocalClick;                    //Control State <SV:4> end

    HGem->GemBtnSendTerminalMessage=GemBtnSendTerminalMessage;                  //
    GemBtnSendTerminalMessage->OnClick=HGem->GemBtnSendTerminalMessageClick;
    HGem->GemTerminalSendEdit=GemTerminalSendEdit;
    GemTerminalSendEdit->OnKeyDown=HGem->GemTerminalSendEditKeyDown;
    HGem->SetEstablishCommunicationsTryCount(5);

    HGem->GemRemoteReceipeList=GemRemoteReceipeList;                            //
    HGem->GemListRemoteFileName=GemListRemoteFileName;
    GemListRemoteFileName->OnClick=HGem->GemListRemoteFileNameClick;
    HGem->GemDownLoadRemoteFileName=GemDownLoadRemoteFileName;
    GemDownLoadRemoteFileName->OnClick=HGem->GemDownLoadRemoteFileNameClick;
    HGem->GemDeleteRemoteFileName=GemDeleteRemoteFileName;
    GemDeleteRemoteFileName->OnClick=HGem->GemDeleteRemoteFileNameClick;
    HGem->GemSelectAllRemoteFile=GemSelectAllRemoteFile;
    GemSelectAllRemoteFile->OnClick=HGem->GemSelectAllRemoteFileClick;
    HGem->GemDisSelectAllRemoteFile=GemDisSelectAllRemoteFile;
    GemDisSelectAllRemoteFile->OnClick=HGem->GemDisSelectAllRemoteFileClick;

    HGem->GemLocalFileLixtBox=GemLocalFileLixtBox;                              //
    HGem->GemRefreshLocalFile=GemRefreshLocalFile;
    GemRefreshLocalFile->OnClick=HGem->GemRefreshLocalFileClick;
    HGem->GemUpLoadLocalFileName=GemUpLoadLocalFileName;
    GemUpLoadLocalFileName->OnClick=HGem->GemUpLoadLocalFileNameClick;
    HGem->GemSelectAllLocalFile=GemSelectAllLocalFile;
    GemSelectAllLocalFile->OnClick=HGem->GemSelectAllLocalFileClick;
    HGem->GemDisSelectAllLocalFile=GemDisSelectAllLocalFile;
    GemDisSelectAllLocalFile->OnClick=HGem->GemDisSelectAllLocalFileClick;

    HGem->GemProcessProgramLoadInquire=GemProcessProgramLoadInquire;            //pig 2013.01.22 SECS_GEM
    GemProcessProgramLoadInquire->OnClick=HGem->GemProcessProgramLoadInquireClick;
    HGem->GemFormattedProcessProgramSend=GemFormattedProcessProgramSend;                      //S7F23
    GemFormattedProcessProgramSend->OnClick=HGem->GemFormattedProcessProgramSendClick;
    HGem->GemFormattedProcessProgramRequest=GemFormattedProcessProgramRequest;                //S7F25
    GemFormattedProcessProgramRequest->OnClick=HGem->GemFormattedProcessProgramRequestClick;

}
//---------------------------------------------------------------------------
void TfHGem::GemAlarmCodeList()
{
    TStringList *ALCD,*ALID,*ALTX;
    int i;

    ALCD=new(TStringList);
    ALID=new(TStringList);
    ALTX=new(TStringList);

//    ALCD->LoadFromFile( "D:\\ACTU900\\doc\\AlarmType.txt");
//    ALID->LoadFromFile( "D:\\ACTU900\\doc\\AlarmCode.txt");
//    ALTX->LoadFromFile( "D:\\ACTU900\\doc\\AlarmText.txt");
//
//    if( ALCD->Count != ALID->Count || ALCD->Count != ALTX->Count )
//    {
//        // error
//        return;
//    }
    // 1 = Alarm
    // 2 = Failure
    // 3 = Warning
    // 4 = Message
    // 5-63 Reserved
    HGem->ClearAlarmCodeList();
    for( i=0; i<ALID->Count;i++ )
    {
        HGem->AddAlarmCodeList(1,ALID->Strings[i],ALTX->Strings[i]);
    }
    //ALCD->LoadFromFile( "D:\\newtesthander\\doc\\MessageCodeListType.txt");
    //ALID->LoadFromFile( "D:\\newtesthander\\doc\\MessageCodeListCodet.txt");
    //ALTX->LoadFromFile( "D:\\newtesthander\\doc\\MessageCodeListText.txt");
    /*
    if( ALCD->Count != ALID->Count || ALCD->Count != ALTX->Count )
    {
        // error
        return;
    }
    // 1 = Alarm
    // 2 = Failure
    // 3 = Warning
    // 4 = Message
    // 5-63 Reserved
    for( i=0; i<ALID->Count;i++ )
    {
        HGem->AddAlarmCodeList(1,ALID->Strings[i],ALTX->Strings[i]);
    }
    */
    delete ALCD;
    delete ALID;
    delete ALTX;

    //HGem->StoreAlarmList(int ALCD,int ALID,AnsiString ALTXT);
}
//---------------------------------------------------------------------------
void TfHGem::GemInitial()
{
//    int ret, i;

    GemInitialDefault();
    HGem->SetTimeFormat(1); // 0:12-bytes, 1:16-bytes, 2:14-bytes, 3:ISO8601 format
    HGem->OpenCommuncation("127.0.0.1",5100);        // 本機
    //HGem->OpenCommuncation("192.168.120.103",5100);  // Joye
    //HGem->OpenCommuncation("10.85.5.150",5000);     // scc
//    HGem->OpenCommuncation("10.6.48.115",5009);
    //HGem->OpenCommuncation(5100);     // scc
    GemAlarmCodeList();
    /*ret=*/HGem->SetReceipeDirectoryAndGlobalName("D:\\HT-3009\\data\\","*.ini",3);       //  0 : 只傳副檔名為 bld
                                                                                       //  1 : 主檔名相同地所有檔案都要傳
                                                                                       //  2 : 路徑下的目錄名,及下面的檔案
                                                                                       //  3 : 路徑下的目錄下的檔案 (多層目錄)
    HGem->SetMachineTypeAndSoftwarseVer(fMain->LabHandlerName->Caption, sVersion);

    //-----------  SV ----------------------
    HGem->SetSVDataPointer(100,HType.ASCII_TYPE         ,"MachineType"          ,""      ,fData->labModel                   );   //
    HGem->SetSVDataPointer(101,HType.ASCII_TYPE         ,"MachineNo"            ,""      ,fData->labSerialNo                           );
    HGem->SetSVDataPointer(102,HType.ASCII_TYPE         ,"MachineID"            ,""      ,fData->labMachineId                           );
    HGem->SetSVDataPointer(103,HType.ASCII_TYPE         ,"SoftwareVersion"      ,""      ,fData->labVersion                          );        //
    HGem->SetSVDataPointer(104,HType.ASCII_TYPE         ,"Power ON Time"        ,"s"     ,fData->labPowerOnTime               );
    HGem->SetSVDataPointer(105,HType.ASCII_TYPE         ,"RunningTime"          ,"s"     ,fData->labRunningTime                );
    HGem->SetSVDataPointer(106,HType.ASCII_TYPE         ,"ProductTime"          ,"s"     ,fData->labProductTime                    );
    HGem->SetSVDataPointer(107,HType.ASCII_TYPE         ,"PauseTime"            ,"s"     ,fData->labPauseTime                    );
    HGem->SetSVDataPointer(108,HType.ASCII_TYPE         ,"JamTime"              ,"s"     ,fData->labJamTime                    );
    HGem->SetSVDataPointer(109,HType.ASCII_TYPE         ,"HomeTime"             ,"s"     ,fData->labHomeTime                    );
//

//                                                                                            &Sen[SnAirIsEnough].State
    HGem->SetSVDataPointer(119,HType.ASCII_TYPE         ,"MachineStatus"        ,""     ,fMain->palMainStatus                                );
    HGem->SetSVDataPointer(120,HType.ASCII_TYPE         ,"SetupFileName"        ,""      ,fMain->cb_WorkFile               );        //
    HGem->SetSVDataPointer(121,HType.ASCII_TYPE         ,"UserLevel"            ,""      ,fMain->ComboBox4                        );      //
    HGem->SetSVDataPointer(122,HType.INT_4_TYPE         ,"MachineRunMode"       ,""      ,&HSys.LastSet.iRealDummy               );
    HGem->SetSVDataPointer(123,HType.INT_4_TYPE         ,"MachineStartMode"     ,""     ,&HSys.LastSet.iStartMode                            );
//    HGem->SetSVDataPointer(124,HType.BOOLEAN_TYPE         ,"BarCodeReaderStatus"  ,""      ,fSetup->cb2DCodeReader   ->Checked              );
//    HGem->SetSVDataPointer(125,HType.BOOLEAN_TYPE         ,"FloatingCheckStatus"  ,""      ,fSetup->cbWACCD_Floating ->Checked         );
//    HGem->SetSVDataPointer(126,HType.BOOLEAN_TYPE         ,"PiggyBackCheckStatus" ,""      ,fSetup->cbWACCD_Piggyback->Checked  );
//    HGem->SetSVDataPointer(127,HType.BOOLEAN_TYPE         ,"PressureAir"          ,""     ,    );        HSys.Sen.SnSafeLock
//    HGem->SetSVDataPointer(128,HType.INT_4_TYPE         ,"VacuumAir1Status"     ,""    ,pan_MainContactPosition                );
//    HGem->SetSVDataPointer(129,HType.INT_4_TYPE         ,"VacuumAir2Status"     ,""      ,&HSys.LastSet.SendCT[0]                );
//    HGem->SetSVDataPointer(130,HType.INT_4_TYPE         ,"VacuumAir13Status"    ,""      ,pan_TotalCount                         );
//    HGem->SetSVDataPointer(131,HType.INT_4_TYPE         ,"VacuumAir4Status"     ,""      ,&HSys.LastSet.BinCT[0][0]              );
//    HGem->SetSVDataPointer(132,HType.INT_4_TYPE         ,"SafeLock"             ,""      ,&HSys.LastSet.BinCT[0][1]              );
//    HGem->SetSVDataPointer(133,HType.INT_4_TYPE         ,"MotorPower"           ,""      ,&HSys.LastSet.SendCT[0]                );
//    HGem->SetSVDataPointer(134,HType.INT_4_TYPE       ,"EMG1Status"           ,""      ,&HSys.Sen.SnVacuum1.Tag                        );
//    HGem->SetSVDataPointer(135,HType.BOOLEAN_TYPE       ,"EMG2Status"           ,""      ,&HSys.LastSet.BinCT[0][0]              );
//    HGem->SetSVDataPointer(136,HType.BOOLEAN_TYPE       ,"EMG3Status"           ,""      ,&HSys.LastSet.BinCT[0][1]              );
//    HGem->SetSVDataPointer(137,HType.BOOLEAN_TYPE       ,"EMG4Status"           ,""      ,&HSys.LastSet.BinCT[0][2]              );
//
//    HGem->SetSVDataPointer(140,HType.INT_4_TYPE         ,"UPH"                  ,""      ,&HSys.LastSet.BinCT[0][3]              );
//
//    HGem->SetSVDataPointer(147,HType.FT_4_TYPE          ,"LoaderCount"          ,""    ,pan_ContactForce                       );
//    HGem->SetSVDataPointer(148,HType.FT_4_TYPE          ,"TotalCount"           ,""    ,pan_ForcePerSite                       );
//
//    HGem->SetSVDataPointer(156,HType.INT_4_TYPE         ,"Ionizer01"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(157,HType.INT_4_TYPE         ,"Ionizer02"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(158,HType.INT_4_TYPE         ,"Ionizer03"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(159,HType.INT_4_TYPE         ,"Ionizer04"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(160,HType.INT_4_TYPE         ,"Ionizer05"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(161,HType.INT_4_TYPE         ,"Ionizer06"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(162,HType.INT_4_TYPE         ,"Ionizer07"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(163,HType.INT_4_TYPE         ,"Ionizer08"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(164,HType.INT_4_TYPE         ,"Ionizer09"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(165,HType.INT_4_TYPE         ,"Ionizer10"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(166,HType.INT_4_TYPE         ,"Ionizer11"            ,""     ,&HSys.SECS.fOutPassYield               );
//    HGem->SetSVDataPointer(167,HType.INT_4_TYPE         ,"Ionizer12"            ,""     ,&HSys.SECS.fOutPassYield               );
//
//
//
//    HGem->SetSVDataPointer(173,HType.INT_4_TYPE         ,"SafeDoor01"           ,"", &Sen[SnSafeDoor1].State);
//    HGem->SetSVDataPointer(174,HType.INT_4_TYPE         ,"SafeDoor02"           ,"", &Sen[SnSafeDoor2].State);
//    HGem->SetSVDataPointer(175,HType.INT_4_TYPE         ,"SafeDoor03"           ,"", &Sen[SnSafeDoor3].State);
//    HGem->SetSVDataPointer(176,HType.INT_4_TYPE         ,"SafeDoor04"           ,"", &Sen[SnSafeDoor4].State);
//    HGem->SetSVDataPointer(177,HType.INT_4_TYPE         ,"SafeDoor05"           ,"", &Sen[SnSafeDoor5].State);
//    HGem->SetSVDataPointer(178,HType.INT_4_TYPE         ,"SafeDoor06"           ,"", &Sen[SnSafeDoor6].State);
//    HGem->SetSVDataPointer(179,HType.INT_4_TYPE         ,"SafeDoor07"           ,"", &Sen[SnSafeDoor7].State);
//    HGem->SetSVDataPointer(180,HType.INT_4_TYPE         ,"SafeDoor08"           ,"", &Sen[SnSafeDoor8].State);
//    HGem->SetSVDataPointer(181,HType.INT_4_TYPE         ,"SafeDoor09"           ,"", &Sen[SnSafeDoor9].State);
//    HGem->SetSVDataPointer(182,HType.INT_4_TYPE         ,"SafeDoor10"           ,"", &Sen[SnSafeDoor10].State);
//
//
//
//    //--------------------------------------
//    //-----------  EC ----------------------
//    HGem->SetECDataPointer(20,HType.BOOLEAN_TYPE,"PPSIGNALTOWER_flag","",&SECS_GEM_PPSIGNALTOWER_CONTROL_flag,true,false,false));
//    HGem->SetECDataPointer(21,HType.INT_4_TYPE,"PPSIGNALTOWER_RED"   ,"",&iSECS_GEM_PPSIGNALTOWER_CONTROL_RED,2,0,0));
//    HGem->SetECDataPointer(22,HType.INT_4_TYPE,"PPSIGNALTOWER_GREEN" ,"",&iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN,2,0,0));
//    HGem->SetECDataPointer(23,HType.INT_4_TYPE,"PPSIGNALTOWER_YELLOW","",&iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW,2,0,0));
//
//    HGem->SetECDataPointer(30,HType.INT_4_TYPE,"SIGNALTOWER_Green_Running" ,"",&LastSet.MessageLight[0][0],2,0,0));
//    HGem->SetECDataPointer(31,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Running","",&LastSet.MessageLight[0][1],2,0,0));
//    HGem->SetECDataPointer(32,HType.INT_4_TYPE,"SIGNALTOWER_Red_Running"   ,"",&LastSet.MessageLight[0][2],2,0,0));
//
//    HGem->SetECDataPointer(33,HType.INT_4_TYPE,"SIGNALTOWER_Green_ErrorJam" ,"",&LastSet.MessageLight[1][0],2,0,0));
//    HGem->SetECDataPointer(34,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_ErrorJam","",&LastSet.MessageLight[1][1],2,0,0));
//    HGem->SetECDataPointer(35,HType.INT_4_TYPE,"SIGNALTOWER_Red_ErrorJam"   ,"",&LastSet.MessageLight[1][2],2,0,0));
//
//    HGem->SetECDataPointer(36,HType.INT_4_TYPE,"SIGNALTOWER_Green_Pause" ,"",&LastSet.MessageLight[2][0],2,0,0));
//    HGem->SetECDataPointer(37,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Pause","",&LastSet.MessageLight[2][1],2,0,0));
//    HGem->SetECDataPointer(38,HType.INT_4_TYPE,"SIGNALTOWER_Red_Pause"   ,"",&LastSet.MessageLight[2][2],2,0,0));
//
//    HGem->SetECDataPointer(39,HType.INT_4_TYPE,"SIGNALTOWER_Green_Message" ,"",&LastSet.MessageLight[3][0],2,0,0));
//    HGem->SetECDataPointer(40,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Message","",&LastSet.MessageLight[3][1],2,0,0));
//    HGem->SetECDataPointer(41,HType.INT_4_TYPE,"SIGNALTOWER_Red_Message"   ,"",&LastSet.MessageLight[3][2],2,0,0));
//
//    HGem->SetECDataPointer(42,HType.INT_4_TYPE,"SIGNALTOWER_Green_Heating" ,"",&LastSet.MessageLight[4][0],2,0,0));
//    HGem->SetECDataPointer(43,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Heating","",&LastSet.MessageLight[4][1],2,0,0));
//    HGem->SetECDataPointer(44,HType.INT_4_TYPE,"SIGNALTOWER_Red_Heating"   ,"",&LastSet.MessageLight[4][2],2,0,0));
//
//    HGem->SetECDataPointer(45,HType.INT_4_TYPE,"SIGNALTOWER_Green_Homing" ,"",&LastSet.MessageLight[5][0],2,0,0));
//    HGem->SetECDataPointer(46,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Homing","",&LastSet.MessageLight[5][1],2,0,0));
//    HGem->SetECDataPointer(47,HType.INT_4_TYPE,"SIGNALTOWER_Red_Homing"   ,"",&LastSet.MessageLight[5][2],2,0,0));


//    static int Max=100,Min=1,Default=50;
//    HGem->SetECDataPointer(101,HType.INT_4_TYPE,"CarryArmSpeed"    ,"%"    ,&iSECS_GEM_Speed[0]    ,&Min,&Max,&Default);
//    HGem->SetECDataPointer(102,HType.INT_4_TYPE,"InCCDSpeed"       ,"%"    ,&iSECS_GEM_Speed[1]    ,&Min,&Max,&Default);
//    HGem->SetECDataPointer(103,HType.INT_4_TYPE,"PressureArmSpeed" ,"%"    ,&iSECS_GEM_Speed[2]    ,&Min,&Max,&Default);
//    HGem->SetECDataPointer(104,HType.INT_4_TYPE,"OutCCDSpeed"      ,"%"    ,&iSECS_GEM_Speed[3]    ,&Min,&Max,&Default);
//    HGem->SetECDataPointer(105,HType.INT_4_TYPE,"MagazineSpeed"    ,"%"    ,&iSECS_GEM_Speed[4]    ,&Min,&Max,&Default);
//    HGem->SetECDataPointer(106,HType.INT_4_TYPE,"FeederZSpeed"     ,"%"    ,&iSECS_GEM_Speed[5]    ,&Min,&Max,&Default);
//    HGem->SetECDataPointer(107,HType.INT_4_TYPE,"UnLoaderYSpeed"   ,"%"    ,&iSECS_GEM_Speed[6]    ,&Min,&Max,&Default);
//
//    static int Max4=1000,Min4=10,Default4=10;
//    HGem->SetECDataPointer(108,HType.INT_4_TYPE,"CarryArmVacuumWaitTime"          ,"s"   ,&iCarryArmVacuumWaitTime        ,&Min4,&Max4,&Default4);
//    HGem->SetECDataPointer(109,HType.INT_4_TYPE,"PressureAreaVacuumWaitTime"  ,"s"   ,&iPressureAreaVacuumWaitTime    ,&Min4,&Max4,&Default4);
//    HGem->SetECDataPointer(110,HType.INT_4_TYPE,"CarryArmDestroyWaitTime"     ,"s"   ,&iCarryArmDestroyWaitTime       ,&Min4,&Max4,&Default4);
//    HGem->SetECDataPointer(111,HType.INT_4_TYPE,"PressureAreaDestroyWaitTime" ,"s"   ,&iPressureAreaDestroyWaitTime   ,&Min4,&Max4,&Default4);
//
//    static int Max1=60,Min1=0,Default1=10;
//    HGem->SetECDataPointer(112,HType.INT_4_TYPE,"PressureTime"    ,"s"    ,&iPressureTime    ,&Min1,&Max1,&Default1);
//
//    static int Max2=10000,Min2=0,Default2=100;
//    HGem->SetECDataPointer(113,HType.INT_4_TYPE,"PressureHeight"           ,"mm"    ,&iPressureHeight               ,&Min2,&Max2,&Default2);
//    HGem->SetECDataPointer(114,HType.INT_4_TYPE,"DieHeight"                ,"mm"    ,&iDieHeight                    ,&Min2,&Max2,&Default2);
//    HGem->SetECDataPointer(115,HType.INT_4_TYPE,"BoatZThickness"           ,"mm"    ,&iBoatFormZThickness           ,&Min2,&Max2,&Default2);
//    HGem->SetECDataPointer(116,HType.INT_4_TYPE,"PressureContactOffset"    ,"mm"    ,&iPressureContactOffset        ,&Min2,&Max2,&Default2);
//    HGem->SetECDataPointer(117,HType.INT_4_TYPE,"HeatSpreaderXWidth"       ,"mm"    ,&iHeatSpreaderSizeXWidth       ,&Min2,&Max2,&Default2);
//    HGem->SetECDataPointer(118,HType.INT_4_TYPE,"HeatSpreaderYLength"      ,"mm"    ,&iHeatSpreaderSizeYLength      ,&Min2,&Max2,&Default2);
//    HGem->SetECDataPointer(119,HType.INT_4_TYPE,"HeatSpreaderZThickness"   ,"mm"    ,&iHeatSpreaderSizeZThickness   ,&Min2,&Max2,&Default2);
//    HGem->SetECDataPointer(120,HType.INT_4_TYPE,"HeatSpreaderPitch"        ,"mm"    ,&iHeatSpreaderPitch            ,&Min2,&Max2,&Default2);
//
//    static int Max3=4,Min3=0,Default3=0;
//    HGem->SetECDataPointer(121,HType.INT_4_TYPE,"HeatSpreaderType"        ,""    ,&iHeatSpreaderType             ,&Min3,&Max3,&Default3);
//
//    static bool Max5=1,Min5=0,Default5=0;
//    HGem->SetECDataPointer(122,HType.BOOLEAN_TYPE,"OnLineMode"                ,""    ,&HSys.LastSet.bIsOnLineForGull             ,&Min5,&Max5,&Default5);
//    HGem->SetECDataPointer(123,HType.BOOLEAN_TYPE,"UseOutMagazine1Box"        ,""    ,&HSys.LastSet.bOnlyUseUnLoaderPassBox1     ,&Min5,&Max5,&Default5);
//    HGem->SetECDataPointer(124,HType.INT_4_TYPE,"MachineLight"               ,""    ,&HSys.SECS.MachineLight                    ,&Min5,&Max5,&Default5);
//
//    HGem->SetECDataPointer(20,HType.BOOLEAN_TYPE,"PPSIGNALTOWER_flag",""        ,&SECS_GEM_PPSIGNALTOWER_CONTROL_flag     ,&Min5,&Max5,&Default5);
//
//    static int Max6=2,Min6=0,Default6=0;
//    HGem->SetECDataPointer(21,HType.INT_4_TYPE,"PPSIGNALTOWER_RED",""           ,&iSECS_GEM_PPSIGNALTOWER_CONTROL_RED     ,&Min6,&Max6,&Default6);
//    HGem->SetECDataPointer(22,HType.INT_4_TYPE,"PPSIGNALTOWER_GREEN",""         ,&iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN   ,&Min6,&Max6,&Default6);
//    HGem->SetECDataPointer(23,HType.INT_4_TYPE,"PPSIGNALTOWER_YELLOW",""        ,&iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW  ,&Min6,&Max6,&Default6);

    HGem->Timer1->Enabled=true;
    InitialSECSDefineReport();                                                  //pig 2013.04.25 SECS_GEM
    ReadSECSDefineReportData();                                                 //pig 2013.04.25 SECS_GEM
    ReadSECSDefineReportLinkData();                                             //pig 2013.05.03 SECS_GEM
}
//==============================================================================
void TfHGem::UpDataSECSMachineStatus()                                                  //pig 2013.01.18 SECS_GEM
{
//    HSys.SECS.MachineLight=!HSys.Sw.SwLight.OutValue;
//    //安全門
//    HSys.SECS.SafeDoor[0]=HSys.Sen.SnFrontLeftSafeDoor.Status();
//    HSys.SECS.SafeDoor[1]=HSys.Sen.SnFrontMiddleLSafeDoor.Status();
//    HSys.SECS.SafeDoor[2]=HSys.Sen.SnFrontMiddleRSafeDoor.Status();
//    HSys.SECS.SafeDoor[3]=HSys.Sen.SnFrontRightSafeDoor.Status();
//    HSys.SECS.SafeDoor[4]=HSys.Sen.SnRearLeftSafeDoor.Status();
//    HSys.SECS.SafeDoor[5]=HSys.Sen.SnRearMiddleLSafeDoor.Status();
//    HSys.SECS.SafeDoor[6]=HSys.Sen.SnRearMiddleRSafeDoor.Status();
//    HSys.SECS.SafeDoor[7]=HSys.Sen.SnRearRightSafeDoor.Status();
//    HSys.SECS.SafeDoor[8]=HSys.Sen.SnHeatSpreaderSafeDoor.Status();
//
//    //EMG
//    HSys.SECS.EMG[0]=HSys.Sen.SnFrontLeftEMG.Status();
//    HSys.SECS.EMG[1]=HSys.Sen.SnFrontRightEMG.Status();
//    HSys.SECS.EMG[2]=HSys.Sen.SnRearLeftEMG.Status();
//    HSys.SECS.EMG[3]=HSys.Sen.SnRearRightEMG.Status();
//
//    //離子風扇
//    HSys.SECS.MachineIonFan[0]=HSys.Sen.SnIonFan1Alarm.Status();
//    HSys.SECS.MachineIonFanLevel[0]=HSys.Sen.SnIonFanLevel1Alarm.Status();
//    HSys.SECS.MachineIonFan[1]=HSys.Sen.SnIonFan2Alarm.Status();
//    HSys.SECS.MachineIonFanLevel[1]=HSys.Sen.SnIonFanLevel2Alarm.Status();
//    HSys.SECS.MachineIonFan[2]=HSys.Sen.SnIonFan3Alarm.Status();
//    HSys.SECS.MachineIonFanLevel[2]=HSys.Sen.SnIonFanLevel3Alarm.Status();
//    HSys.SECS.MachineIonFan[3]=HSys.Sen.SnIonFan4Alarm.Status();
//    HSys.SECS.MachineIonFanLevel[3]=HSys.Sen.SnIonFanLevel4Alarm.Status();
//
//    //
//    HSys.SECS.LOCK=HSys.Sen.SnSafeLock.Status();
//    HSys.SECS.MachineAir=HSys.Sen.SnAirIsEnough.Status();
//    HSys.SECS.MachineMotorPower=HSys.Sen.SnMotorPower.Status();
}
//==============================================================================
void __fastcall TfHGem::SetHostNewValueforSECS(AnsiString ID,int NewValue)     //pig 2013.01.15 SECS_GEM
{
//    if(ID=="122")
//    {
//        if(NewValue==1)
//            cb_OnLine->Checked=true;
//        else
//            cb_OnLine->Checked=false;
//        cb_OnLineClick(this);
//    }
//    else if(ID=="123")
//    {
//        if(NewValue==1)
//            cb_OnlyUseUnLoaderPassBox1->Checked=true;
//        else
//            cb_OnlyUseUnLoaderPassBox1->Checked=false;
//        cb_OnlyUseUnLoaderPassBox1Click(this);
//    }
//    else if(ID=="124")
//    {
//        if(NewValue==1)
//            HSys.Sw.SwLight.OutValue=true;
//        else
//            HSys.Sw.SwLight.OutValue=false;
//        LightClick(this);
//    }
//    else if(ID=="125")
    if(ID=="125")
    {
        if(NewValue==1)
            HSys.LastSet.iRealDummy=false;
        else
            HSys.LastSet.iRealDummy=true;
        fMain->pnRealDummyClick(this);
    }
    else if(ID=="126")
    {
        if(NewValue==1)
            HSys.LastSet.iStartMode=false;
        else
            HSys.LastSet.iStartMode=true;
        fMain->pnStartModeClick(this);
    }
    else
    {
        return;
    }
}
//==============================================================================
int TfHGem::WriteRecipeData(char *Path,char *Name,char *Data)                           //pig 2013.01.18 SECS_GEM
{
    AnsiString Value,S;
    bool bErr=false;

    TIniFile *INIFile = new TIniFile(Path);

    Value=Data;
    S=Name;
    if(S=="CarryArmVacuumWaitTime")
        INIFile->WriteString("Track & Vacuum","ed_CarryArmVacuumWaitTime",Value);
    else if(S=="PressureAreaVacuumWaitTime")
        INIFile->WriteString("Track & Vacuum","ed_PressureAreaVacuumWaitTime",Value);
    else if(S=="CarryArmDestroyWaitTime")
        INIFile->WriteString("Track & Vacuum","ed_CarryArmDestroyWaitTime",Value);
    else if(S=="PressureAreaDestroyWaitTime")
        INIFile->WriteString("Track & Vacuum","ed_PressureAreaDestroyWaitTime",Value);

    else if(S=="PressureTime")
        INIFile->WriteString("Cntact","ed_PressureTime",Value);
    else if(S=="PressureHeight")
        INIFile->WriteString("Cntact","ed_PressureHeight",Value);
    else if(S=="DieHeight")
        INIFile->WriteString("Cntact","ed_DieHeight",Value);
    else if(S=="BoatZThickness")
        INIFile->WriteString("Cntact","ed_BoatZThickness",Value);
    else if(S=="PressureContactOffset")
        INIFile->WriteString("Cntact","ed_PressureContactOffset",Value);

    else if(S=="HeatSpreaderSizeXWidth")
        INIFile->WriteString("Heat Spreader Form","ed_HeatSpreaderSizeXWidth",Value);
    else if(S=="HeatSpreaderSizeYLength")
        INIFile->WriteString("Heat Spreader Form","ed_HeatSpreaderSizeYLength",Value);
    else if(S=="HeatSpreaderSizeZThickness")
        INIFile->WriteString("Heat Spreader Form","ed_HeatSpreaderSizeZThickness",Value);
    else if(S=="HeatSpreaderPitch")
        INIFile->WriteString("Heat Spreader Form","ed_HeatSpreaderPitch",Value);
    else if(S=="HeatSpreaderType")
        INIFile->WriteString("Heat Spreader Form","rg_HeatSpreaderType",Value);
    else
        bErr=true;
    delete INIFile;
    if(bErr==true)
        return 1;
    return 0;
}





