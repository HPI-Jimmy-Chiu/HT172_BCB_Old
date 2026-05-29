//---------------------------------------------------------------------------
#ifndef setupH
#define setupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ALed.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <MPlayer.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <FileCtrl.hpp>
#include <Graphics.hpp>
#include <CheckLst.hpp>
#include "butPa1.h"
#include "cprod.h"
#include "HTray.h"
#include <Menus.hpp>
#include "htray.h"
#include <vector>
//------------------------------------------------------------------------------
class cBinAreaGUIConvert;
class cBinAreaSettingGUI;
class cBinAreaGUIData;
//------------------------------------------------------------------------------
class PACKAGE TMyBinPanel : public TComponent
{
    public:
        __fastcall TMyBinPanel(TComponent* Owner, int index, TPanel *parent, AnsiString Name);
        __fastcall ~TMyBinPanel();
        TPanel      *Panel;
        TTMyTray    *mtBinSelect;
        TEdit       *edBinSetting;
        TEdit       *edLink;
        int     BackTray    [TEST_MAX_BIN+1][eTrayCount];
        bool    bLink       [eTrayCount];
        int     iLink       [eTrayCount];
        int     iTotalBin;
        void    __fastcall InitmtBinSelectData();
};
extern vector<TMyBinPanel*> MyBinPanel;
//---------------------------------------------------------------------------
class cBinAreaSettingGUI
{
    private:
        void __fastcall sgBinAreaDrawCell(TObject *Sender, int ACol, int ARow,
            const TRect &Rect, TGridDrawState State);
        void __fastcall sgBinAreaMouseDown(TObject *Sender,
            TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall sgBinAreaMouseUp(TObject *Sender,
            TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall sgBinAreaMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        int iHoverCol;
        int iHoverRow;
        int bBinMouseDown;
        int iBinSelectedStart,iBinSelectedEnd;
        int iAreaSelectedStart,iAreaSelectedEnd;
        cBinAreaGUIData &refBinAreaGUIData;
        void InitialSGBinSetting(TStringGrid* sg);
        void InitialSGBinSettingTitle(TStringGrid* sg,int ibinS,int ibinR);
        TStringGrid* sgBinArea;
    public:     // User declarations
        cBinAreaSettingGUI(cBinAreaGUIData &a): refBinAreaGUIData(a)
        {
            Clear();
        }
        ~cBinAreaSettingGUI();
        void Clear()
        {
            iBinStart=0;
            iBinRange=0;
            bBinMouseDown=false;
            iBinSelectedStart=iBinSelectedEnd=iAreaSelectedStart=iAreaSelectedEnd=0;
        }
        void SetSGBinSetting(TStringGrid* sg,int ibinS,int ibinR);
        int iBinStart;
        int iBinRange;
        bool IsValidGrid(TStringGrid* sg);
        AnsiString GetCellValue(TStringGrid* sg,int row, int col);
        void SetCellValue(TStringGrid* sg,int row, int col, AnsiString value);
};
//---------------------------------------------------------------------------
class cBinAreaGUIData
{
    private:
        bool bBackTray[TEST_MAX_BIN+1][eTrayCount];
    public:     // User declarations
        cBinAreaGUIData()
        {
            Clear();
        }
        ~cBinAreaGUIData(){}
        void Clear()
        {
            ZeroMemory(bBackTray, sizeof(bBackTray));
        }
        bool GetBackTray(int bin, int tray) const;
        void SetBackTray(int bin, int tray, bool value);
};
//---------------------------------------------------------------------------
class cBinAreaGUIConvert
{
    private:
        vector<cBinAreaSettingGUI*> vBinAreaGUI;
    public:     // User declarations
        cBinAreaGUIConvert()
        {
            Clear();
        }
        ~cBinAreaGUIConvert(){}
        void Clear()
        {
        }
        void BackTray_to_cBinAutoTrans();
        void cBinAutoTrans_to_BackTray();
        void AddTStringGrid(TStringGrid *sg, int iStart, int iRange);
        cBinAreaGUIData cGUIData;
        void GetStartRange(int iorder, int imaxbinnum, int ibinrange, int &iStart, int &iRange);
};
//---------------------------------------------------------------------------
class TfSetup : public TForm
{
__published:    // IDE-managed Components
    TSaveDialog *SaveDialog;
    TPanel *Panel5;
    TSpeedButton *sbBuild;
        TSpeedButton *sbTrayForm;
    TSpeedButton *sbExit;
    TSpeedButton *sbSaveAs;
    TSpeedButton *sbUpdate;
        TSpeedButton *SpeedButton14;
        TSpeedButton *SpeedButton15;
        TOpenDialog *OpenPickMap;
        TOpenDialog *OpenTestMap;
    TSpeedButton *spbBinSetting;
    TRadioGroup *rgRunMode;
    TOpenDialog *OpenDialog1;
    TSpeedButton *spbNeedleAndPnp;
    TPageControl *pcSetup;
    TTabSheet *TabWorkFileManegement;
    TPanel *Panel10;
    TSpeedButton *spbDeleteSetupFile;
    TLabel *Label11;
    TLabel *Label14;
    TBevel *Bevel1;
    TSpeedButton *spbImport;
    TSpeedButton *spbExport;
    TCheckListBox *clWorkFile;
    TDriveComboBox *DriveComboBox1;
    TDirectoryListBox *DirectoryListBox1;
    TTabSheet *TabSetup;
    TPanel *Panel20;
    TLabel *Label45;
    TPanel *Panel27;
    TImage *Image3;
    TLabel *Label20;
    TLabel *Label39;
    TLabel *Label41;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label44;
    TLabel *labTrayForm;
    TEdit *edXStart;
    TEdit *edXPitch;
    TEdit *edYStart;
    TEdit *edYPitch;
    TEdit *edXDivision;
    TEdit *edYDivision;
    TTMyTray *TMyTray1;
    TPanel *Panel28;
    TLabel *labLoaderWaitTime;
    TImage *Image4;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label13;
    TLabel *Label15;
    TLabel *Label10;
    TLabel *Label3;
    TEdit *edLoadTrayArrival;
    TEdit *edLoadLock;
    TEdit *edLoadMiddle;
    TEdit *edLoadDown;
    TPanel *Panel29;
    TLabel *Label96;
    TImage *Image2;
    TLabel *Label46;
    TLabel *Label47;
    TLabel *Label73;
    TLabel *Label74;
    TEdit *edUnloadUp;
    TEdit *edUnloadArrival;
    TTabSheet *tsBinSetting;
    TLabel *labBin;
    TTabSheet *TabSheet2;
    TPanel *Panel14;
    TLabel *Label21;
    TPanel *Panel15;
    TLabel *Label52;
    TLabel *Label55;
    TPanel *Panel16;
    TLabel *Label53;
    TEdit *edRetryCount;
    TTabSheet *TabSheet1;
    TFileListBox *flWorkFileList;
    TFileListBox *flTemp;
    TFileListBox *flImport;
    TGroupBox *GroupBox1;
    TLabel *labLDSpeed;
    TEdit *edLDSpeed;
    TLabel *Label5;
    TEdit *edLD2ndSpeed;
    TLabel *Label12;
    TEdit *edEmpty1Speed;
    TLabel *Label16;
    TEdit *edEmpty12ndSpeed;
    TLabel *Label17;
    TEdit *edEmpty2Speed;
    TLabel *Label18;
    TEdit *edEmpty22ndSpeed;
    TLabel *Label19;
    TEdit *edTrack1Speed;
    TLabel *Label22;
    TEdit *edTrack12ndSpeed;
    TLabel *Label23;
    TEdit *edTrack2Speed;
    TLabel *Label24;
    TEdit *edTrack22ndSpeed;
    TLabel *Label25;
    TEdit *edTrack3Speed;
    TLabel *Label26;
    TEdit *edTrack32ndSpeed;
    TLabel *Label27;
    TEdit *edTrack4Speed;
    TLabel *Label28;
    TEdit *edTrack42ndSpeed;
    TLabel *Label29;
    TLabel *Label30;
    TEdit *edTransferSpeed;
    TEdit *edTransfer2ndSpeed;
    TLabel *Label31;
    TEdit *edAutoPickDelay;
    TLabel *Label32;
    TLabel *Label33;
    TEdit *edAutoPlaceDelay;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TEdit *Edit3;
    TLabel *Label37;
    TEdit *edMagPickDelay;
    TLabel *Label38;
    TLabel *Label40;
    TEdit *edMagPlaceDelay;
    TLabel *Label50;
    TLabel *lblAutoPreZDown;
    TEdit *edAutoPreZDown;
    TCheckBox *cbAutoPreZDown;
    TCheckBox *cbAutoPreSuckOff;
    TLabel *lblAutoPreSuckOff;
    TEdit *edAutoPreSuckOff;
    TEdit *edAutoPreSuckOn;
    TLabel *lblAutoPreSuckOn;
    TCheckBox *cbAutoPreSuckOn;
    TLabel *lblMagPreSuckOn;
    TLabel *lblMagPreSuckOff;
    TLabel *lblMagPreZDown;
    TCheckBox *cbMagPreSuckOn;
    TCheckBox *cbMagPreSuckOff;
    TEdit *edMagPreSuckOn;
    TEdit *edMagPreSuckOff;
    TCheckBox *cbMagPreZDown;
    TEdit *edMagPreZDown;
    TPageControl *pgcBinSet;
    TTabSheet *tsAutoBinSet;
    TPanel *Panel2;
    TLabel *Label1;
    TLabel *Label2;
    TPanel *Panel3;
    TLabel *Label67;
    TRadioGroup *rgUseSuck;
    TTMyTray *mtSortArm1OnOff;
    TTMyTray *mtSortArm2OnOff;
    TLabel *lblDestroyCheckTime;
    TEdit *edtDestroyCheckTime;
    TScrollBox *ScrollBox1;
    TPanel *pnlAutoBin;
    TStringGrid *sgBinSet;
    TPanel *Panel1;
    TButton *btnClearBin;
    TButton *btnDoAutoBinSet;
    TRadioGroup *rgSoter;
    TGroupBox *grpSettinBin;
    TButton *btnSelectAllBin;
    TButton *btnUnSelectAllBin;
    TButton *btnSelect1To20;
    TButton *btnSelect21To41;
    TTabSheet *tsCCD;
    TSpeedButton *sbCCD;
    TFileListBox *flLoadTrayMap;
    TButton *btnSelect1To50;
    TButton *btnSelect51To98;
    TButton *btnSelect99To150;
    TTabSheet *tsBinSetting2;
    TPageControl *PageControl1;
    TTabSheet *tsBin1_16;
    TStringGrid *sgBinOrder1;
    TLabel *Label48;
    TEdit *edAutoZUpDelay;
    void __fastcall sbBuildClick(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall sbExitClick(TObject *Sender);
    void __fastcall sbSaveAsClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall spbExportClick(TObject *Sender);
    void __fastcall spbDeleteSetupFileClick(TObject *Sender);
    void __fastcall spbImportClick(TObject *Sender);
        void __fastcall SpPickCPClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall edReleaseOffsetMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edXStartMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnClearBinClick(TObject *Sender);
    void __fastcall btnDoAutoBinSetClick(TObject *Sender);
    void __fastcall btnSelectAllBinClick(TObject *Sender);
    void __fastcall btnUnSelectAllBinClick(TObject *Sender);
    void __fastcall btnSelect1To20Click(TObject *Sender);
    void __fastcall btnSelect21To41Click(TObject *Sender);
    void __fastcall rgSoterClick(TObject *Sender);
    void __fastcall rgUseSuckClick(TObject *Sender);
    void __fastcall mtSortArm2OnOffMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtSortArm1OnOffMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnSelect1To50Click(TObject *Sender);
    void __fastcall btnSelect51To98Click(TObject *Sender);
    void __fastcall btnSelect99To150Click(TObject *Sender);
private:    // User declarations
    bool bMouseDown;
    bool bBinMouseDown;
    void __fastcall BinChange();
    int iStartX, iEndX;
    int iStartY, iEndY;
    int iCurrX, iCurrY;
    int iOldX, iOldY;
//    void SetBinByArea(TStringList *sList,int iBin,int iArea);
    void SetBinByArea(int iBin,int iArea);
    int iBinStart,iBinEnd;
    int iAreaStart,iAreaEnd;
public:     // User declarations
    bool fShow;
    __fastcall TfSetup(TComponent* Owner);
    void __fastcall SaveWorkFile(AnsiString S);
    void __fastcall OpenWorkFile();
    void __fastcall UpdateWorkFileCheckListBox();

    void __fastcall SetEditOnMouse(TWinControl *PCtrl);
    void ClearBinSet(int tag, bool bSetToFile=false);
    void AutoBinSet();
    void AutoBinSet(const std::vector<int> &vbins);
    void AutoBinSet(const std::vector<int> &vbins,int iTrayOrder[eTrayCount]);
    bool ParseCommaSeparatedString(const AnsiString &sBinSelectedList, std::vector<int> &result);
    void AutoBinSetAutoAreaOnly(AnsiString sBinSelectedList);                   //Ian : 自動設定bin tray在Auto區only
    void DisPlayBinTrayMap();
    TStringList *sListBin;
    TStringList *sListTray;
    TCheckBox *cbEnableBin[TEST_MAX_BIN];	                                            //Sam 20250718 : 要到 Bin150
    AnsiString sBinSetting;
    std::vector<int> VecBins;
    std::vector<int> GetTrayOrderList();
    AnsiString GetSetUpFileName();
    AnsiString GetSetUpBinDataFileName();
    //bin setting v2
    cBinAreaGUIConvert cBinArea;
    void InitialBinAreaSG();
};
//---------------------------------------------------------------------------
extern PACKAGE TfSetup *fSetup;
//---------------------------------------------------------------------------
#endif
