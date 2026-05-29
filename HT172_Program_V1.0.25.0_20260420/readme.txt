[1]
所有 IO , Motor 透過  HSys來運作
HSys 是 class SYSTEM_MODULAR
SYSTEM_MODULAR defined at database.h

而 SYSTEM_MODULAR 內有
   CYLINDER_MODULAR        Cyn;
   SENSOR_MODULAR          Sen;
   SWITCH_MODULAR          Sw;
   SUCKER_MODULAR          Suck;

    子類別用來定義 IO,Motor 變數

HSys 建構子 將會自動載入 相關之 資料表
HSys 會自計算 Motor,Sensor,Switch 等等數量不需人工定義


[2] 畫面儲存利用
    FormSysTools->OpenFormData(S);  // S 是檔名
    FormSysTools->SaveFormData(TabVacuumPurge,"Vacuum_Purge");        // arg1 是欲存檔之 VCL container
    FormSysTools->SaveFormData(TabContact,"Cntact");
    FormSysTools->CloseFormData();
    畫面載入利用
    FormSysTools->OpenFormData(S);  // S 是檔名
    FormSysTools->LoadFormData(TabVacuumPurge,"Vacuum_Purge");        // arg1 是欲存檔之 VCL container
    FormSysTools->LoadFormData(TabContact,"Cntact");
    FormSysTools->CloseFormData();

[3] 中英文只要
    HSys.LastSet.iLanguageCountry=1  or
    HSys.LastSet.iLanguageCountry=0

    systools.cpp 之 Timer 會自動切換 ,及所有元件之 enable,visible 等等狀態

[4] 設定所有元件之 enable,visible 等等狀態
    則於 systools.cpp 中

    void __fastcall TFormSysTools::SpeedButton2Click(TObject *Sender)
    {
        if(bSetUserAccessLevel==false)
        {
            InitialSetLevelPopMenuList();   // 開始設定
            bSetUserAccessLevel=true;
            SpeedButton2->Caption="Stop set user access level";
        }
        else
        {
            RestoreSetLevelPopMenu();       // 還原設定
            bSetUserAccessLevel=false;
            SpeedButton2->Caption="Begin set user access level";
        }

    }


