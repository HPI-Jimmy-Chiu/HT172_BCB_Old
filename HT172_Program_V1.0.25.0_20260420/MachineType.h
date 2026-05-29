//=============  Specific define  ==============================================
//#define AdlinkHSL
//#define CCD_USE
//#define NEW_CCD_API
//#define AUTO_CLEAN
//#define UseTempController
//#define SCAN_SIMULTE
//#define ONECYCLE_DEBUG
//=============  USER DEFINE ===================================================
//#define SOFT_SIMULATE

//#define DISABLE_SAFE_DOOR
//#define FORCE_BIN2                                        
//#define FORCE_RANDOM
#define USE_CompareCommandPos

//#define DISABLE_STORE                                                           //試機時未安裝Auto1-20的料架使用。
const int AUTOMATION_MAX_DATA=260;
//bool Enable_PLCSafety_IO        =false;    //Austin 20190531.01 增加PLC_IO模組及安全相關IO.

//注意事項----------------------------------------------------------------------
//  1. Memo跟List請用Clear(); 不要用Delete(i); 因為會吃記憶體
//  2. timeBeginPeriod(1); 只需要使用一次,可以讓cycle time更精準
//  3. 快速Compiler的東西會造成莫名其妙的錯誤!!
//  4. 使用除法和開根號時，要注意變數是不是0
//  5. 注意ItemIndex不可以是-1
//  6. 注意PageControl->ActivePage不要用,會有死機的風險;請愛用PageControl->ActivePageIndex
//------------------------------------------------------------------------------
//Function----------------------------------------------------------------------
// bP02_NeedleUpEarlyMode2
// EJZ2 頂針頂著讓吸嘴來吸

//------------------------------------------------------------------------------
/*
V.1.07.40
Wafer_radius  1000 > 1100

V.1.07.39
Sam 20190620 : 增加頂針計數與報警

V.1.07.38

V.1.07.37


V.1.07.36
1. Add Wafer Pick up Error Auto Skip 並吹料
2. Add Strip Col Null IC

V.1.07.35
1. Wafer Ring Dircetion 增加開關


V.1.07.34
1. 增加功能開關 Buffer Kit PickUp 時是 Open/Close

V.1.07.33
1. Add BufferKit 汽缸

V.1.07.32
1. 整理 Online/Offline Real/Dummy SOFT_SIMULATE 模式

V.1.07.31
1. //Sam 20181001 : 其他角度也要檢查 Wafer Ring 方向

V.1.07.30
1. HT-1501A Add Rotation Funcatiom

V.1.07.22
1. Map Strip Frame Type5 使用 PitchY1 變數。

V.1.07.19
1. Fix cbA04_CheckLastRowCol 功能

V.1.07.18
1. 檢查 Wafer Pick Up 最後一排/行，若沒有檢查到 IC ，OffSet Pitch 後重新檢查第一排/行。

V.1.07.17
1. HT1501 增加 Bin Count 上限檢查。

V.1.07.16
1. Unload Tray 增加延遲，防止最後一顆沒放好。

V.1.07.15
1. 補 New Tray 時，起始位置 Bug Fix

V.1.07.14
1. 增加功能-入 Tray 方向：iTrayDirectionType==0 又下角為第一顆(新增並為預設)；iTrayDirectionType==1 左上角為第一顆(原本)
2. 檢查是否補充新 Tray 位置 -35000> -25000，解決 Auto2 排到最後幾排會誤報警 "00022"

V.1.07.13
1. BinBox Full 提示改為警報
2. 環型光 Alignment 未放下 Bug Fix

V.1.07.12
1. 增加 BinTray Warning Count
2. 移除停拍變速功能

V.1.07.11
1. 移除使用不到的變數與指標

V.1.07.10
1. 真空異常報警顯示 Fix
2. Tray 換行時，增加PnpZ 比必須在 Fly 高度
V.1.07.09
1. HT-1501 Clean out huang up Fix Bug

V.1.07.08
1. Fly 拍移動速度設定固定值

V.1.07.07
1. uHome EJ Z2 防撞機制加回去，通用所有機型。
2. Home 時 ServoAlarmOn 是等於 0 清除 Alarm 狀態。

V.1.07.06
1. 增加 Bin 連續異常報警 (未完成)20171028
2. HT-1501 UPH 優化 Bug 修正。
3. PnPX PnP1X 線馬回 HOME 會清除 Alarm Led，修正因為 ServoAlarmOn 是等於 0 造成 Alarm 狀態無法清除，需要重啟程式 Bug。

V.1.07.05
1. 因停拍優化使通訊差異 OneTouchGrabDownCCD 功能 Bug Fix
2. 修改與 Vision 通訊 Log 紀錄方式，Bug 修正讓硬碟不會爆掉，減少跑久因紀錄 Log 而變慢的問題。
3. 功能增加-到掃 Ring 的第一片位置，方便調整 Sensor 高度。
4. !!!!!!!!!!!!!!!!!!重大 Bug 修正 - Tray 位置參數錯誤 Offset 補償錯誤 Bug Fix，需要將 Auuto1 的 Teach Empty 與 Auot1 對調，Auto2 也是
5. HT-1501A PnPX Fly 高度不需要用到 OffSet
6. 功能增加-當對位時有發生 Wafer 缺料無法定位，自動 Null 掉 Block 左上角第一顆。

V.1.07.04
1. HT-1501A+ 增加 CatchY 再取放 Wafer 時要閃避馬達 (暫時不用了加了)
2. Ring 放回 Magazine 的 Z 軸 OffSet 獨立，方便調整。
3. 放 IC 入 Tray 吹氣延遲時間獨立，方便調整。
4. HT1501 增加 PnPX 移動前檢查保護
5. HT1501 優化停拍 Bug Fix
6. 增加功能-發生一點定位錯誤時，環形光可以上升，方便人員觀察。
7. 增加功能-發生一點定位錯誤時，可以自動 Null 掉第一顆 IC。

V.1.07.03
1. 增加 Voice Motor 單軸回 HOME 機制。
2. Magazine Map 顯示修正


V.1.07.02
1. Pnp1X 修正 BuffKit Retry 機制 Bug
2. HT-1501A PnPX 增加 Retry 機制- 之前是真空後建立吸 IC Retry，增加真空建立前也能 Retry。

V.1.07.00 Beta
1. HT-1501 PnPAction 停拍動作優化，增加等待 DOWN CCD 拍照結束功能，增加與 Vision 通訊，開啟等待 DOWN CCD 拍照結束功能需要 Vision 改版。

V.1.06.00 Beta
1. HT-1501A PnPAction3 停拍動作優化，增加等待 DOWN CCD 拍照結束功能，增加與 Vision 通訊，開啟等待 DOWN CCD 拍照結束功能需要 Vision 改版。
2. CycleTime 顯示修正 (僅修正HT-1501A)

V.1.05.00 Beta
1. PnPAction2 流程優化 (Break)
2. PnPAction2 Vacumm Delay time 方式變更為真空建立後再 Delay
3. PnPAction2 放 IC Vacumm 吹氣，增加報警機制和 Alarm Code
4. PnPAction3 流程優化 (Break)
5. PnPAction3 Vacumm Delay time 方式變更為真空建立後再 Delay
6. PnPAction3 放 IC Vacumm 吹氣，增加報警機制和 Alarm Code
7. PnPAction3 停拍動作優化，送 Trigger 訊號後，延遲設定時間後，就直接偷跑至 Auto1
8. PnPAction3 增加停拍時 Vision 分析時間紀錄。



V.1.04.03
1. Duumy Run 修復
2. 軟體模擬 Bug Fix

V.1.04.02
1. Duumy Run 修復

V.1.04.01
1. 掉 Ring 時的異常處理 Bug 修正。

V.1.04.00
1. 增加 Arm 吸嘴計數 Alarm
2. IO Monitot 增加 SnBuffer_HasIC
3. Category Count 紀錄修正
4. 增加 Tray Hook 偵測，若沒有亮可能是卡到 IC 導致 Hook On Sensor 沒有亮。
5. 批貨第一次跑的時候檢查 Category 資料是否需要清除
6. Arm1 放 IC 到 Buffer 增加偵測是否有 IC 防止疊料
*/


//============= Customer Code define ===========================================
#define CC_HONPREC_QC             0


#define CC_Morningcore          766 //宸芯
#define CC_Paceis               767 //航芯源
#define CC_Sigmastar            768 //銳力
#define CC_BRAVETEK             769 //博發電子
#define CC_DJI_SZ               770 //大疆創新 深圳

#define CC_NXP_TJ               780 //恩智浦 天津
#define CC_XINYUN               781 //杭州芯云

#define CC_FOREHOPE_NINGBO      790 //甬矽 寧波
#define CC_SJ_Semiconductor     791 //盛合晶微
#define CC_SJ_Semiconductor_OS  792 //盛合晶微Open Short
#define CC_30JAVEE              793 //成都三零嘉微電子
#define CC_CAMBRICON            794 //上海寒武紀
#define CC_VATE                 795 //立衛科技
#define CC_ASIAOPTICAL          796 //亞洲光學
#define CC_HXYSEMI              797 //浙江航源芯
#define CC_LEADYO               798 //廣東利揚
#define CC_DENGLIN              799 //成都登臨科技

#define CC_BROADCOM_US          800 //Broadcom-US
#define CC_AVAGO_Korea          810 //安華高 韓國
#define CC_TSMC_TAINAN          820 //台積電 台南
#define CC_TSMC_HSINCHU         825 //台積電 新竹
#define CC_HTKJXA_CHINA         830 //西安 天水華天
#define CC_RF360                831 //RF360

#define CC_RIGGER_MICRO         834 //鄭州銳傑微
#define CC_XINITECH             835 //北京芯力
#define CC_GIGA_FORCE_Zhejiang  836 //季豐 浙江
#define CC_GIGA_FORCE_Shanghai  837 //季豐 上海
#define CC_CSAMQ                838 //csamq 長沙安牧泉
#define CC_GONGJIN_SHANGHAI     839 //上海共進

#define CC_SANDISK_CHINA        840 //SANDISK 上海
#define CC_GONGJIN_SUZHOU       841 //蘇州共進
#define CC_Mathilda             842 //馬舍科技
#define CC_DoosanTesna          843 //DOOSAN TESNA
#define CC_Renesas_M            844 //Renesas Malaysia.
#define CC_STK                  845 //STK 日本
#define CC_Renesas              846 //Renesas 日本
#define CC_THINE                847 //Thine 日本
#define CC_SINOICTECH           848 //上海華嶺
#define CC_ChipOn               849 //上海芯旺

#define CC_ChipMos_TAINAN       850 //南茂 台南
#define CC_ChipMos_ZHUBEI       851 //南茂 竹北
#define CC_ITS                  852 //創量科技
#define CC_NEXPERIA_Guangdong   853 //廣州 安世
#define CC_Atec_Semiconductor   854 //上海旻艾半導體.
#define CC_JSSI_Semiconductor   855 //江蘇芯德半導體.
#define CC_Microchip_FR         856 // Microchip 法國
#define CC_Indie_US             857 //Indie 美國.
#define CC_CETC                 858 //中國電子-第58研究所
#define CC_XDXCT                859 //象帝先

#define CC_MAXIM_THAILAND       860 //MAXIM 泰國
#define CC_Microchip_Thailand      861 //Microchip 泰國
#define CC_Microchip_Philippines   862 //Microchip 菲律賓
#define CC_Microchip_China         863 //Microchip 中國
#define CC_Microchip_US            864 //Microchip 美國
#define CC_HANA_MICRON          865
#define CC_ITestInc             866 //iTest,Inc. 美國
#define CC_EMemory              867 //EMemory 力旺電子
#define CC_CYUEAN               868 //CYUEAN 确安科技
#define CC_PANTHER              869 //PANTHER 鴻谷科技

#define CC_ARDENTEC             870 //欣詮                                      //20160805 Mylin Modify Show Only Ambient or High for ARDENTEC
#define CC_FULCAP               871 //詮容                                      //2016.06.30 ,Brian
#define CC_AOSL                 872 //萬有半導體
#define CC_Nuvoton_Israel       873 //新唐 以色列
#define CC_GT                   874 //寰邦科技
#define CC_Novatek              875 //聯詠科技
#define CC_Sunplus              876 //凌陽科技
#define CC_Amlogic              877 //晶晨半導體
#define CC_Higon                878 //成都海光
#define CC_Kingston             879 //金士頓

#define CC_Spreadtrum           880 //展訊 上海
#define CC_Amazon               881 //Amazon
#define CC_Murata               882 //Murata 村田製作所
#define CC_Goertek              883 //歌爾微電子
#define CC_FMSH                 884 //上海復旦微電子
#define CC_HDSC                 885 //華大半導體
#define CC_SANECHIPS            886 //中興微電子 深圳
#define CC_HABANA               887 //HABANA
#define CC_GIS                  888 //業泓科技
#define CC_CENTER               889 //中芯國際

#define CC_UMC                  890 //聯電
#define CC_ATEC                 891 //艾科
#define CC_WINSTEK              892 //台星科
#define CC_SANAN                893 //三安光電
#define CC_UTAC_TW              894 //聯測科技 台灣
#define CC_BARUN                895 //百齡
#define CC_SIGURD_SUZHOU        896 //矽格 蘇州
#define CC_SILTERRA_CHINKIANG   897 //矽佳 鎮江
#define CC_AMD_SUZHOU           898 //AMD  蘇州
#define CC_YTEC                 899 //久元

#define CC_ISE_US               901 //ISE 美國
#define CC_HYGEIA_SUZHOU        902 //海京 蘇州
#define CC_JSI_HAOXING          903 //紹興 長電
#define CC_JINGJIAWEI_CHANGSHA  904 //長沙景嘉微
#define CC_ISE_SH               905 //上海月芯半導體科技

#define CC_INTEL_IL             906 //INTEL 以色列
#define CC_IBM_CANADA           907 // IBM 加拿大
#define CC_PGC                  908 // 巨有科技
#define CC_ITESTSEMI            909 // 安測半導體

#define CC_SPIL_SHINCHU         910 //矽品 新竹.
#define CC_SPIL_TAICHUNG_LOGIC  911 //矽品 台中.
#define CC_SPIL_CHINA_SUZHOU    912 //矽品 蘇州.
#define CC_HUAWEI               913 //華為.
#define CC_ANST                 914 //蘇州安盛
#define CC_VTEST                915 //無錫偉測半導體
#define CC_TFME_CHINA           916 //通富微電
#define CC_JCET_5               917 //江蘇長電 五廠
#define CC_BOJIAN               918 //蘇州 博劍
#define CC_VTEST_Shanghai       919 //上海偉測半導體

#define CC_KYEC_CHEN            920 //京元 記憶體.銅鑼廠
#define CC_KYEC_LEE             921 //京元 邏輯.
#define CC_KYEC_JCTHIU          922 //京元 科園
#define CC_DL_TEK               923 //東琳.
#define CC_KYEC_XILINX          924 //京元 Xilinx
#define CC_KYEC_STM             925 //京元 STM
#define CC_Advantest_GE         926 //Advantest 德國

#define CC_ASE_KaohSiung_K12    929 //日月光 高雄K12.
#define CC_ASE_SG               930 //日月光 新加坡.
#define CC_ASE_JP               931 //日月光 日本.
#define CC_ASE_Korea            932 //日月光 韓國.
#define CC_ASE_CL               933 //日月光 中壢.
#define CC_ASE_SH               934 //日月光 上海.
#define CC_ASE_N                935 //日月光 NXP.
#define CC_ASE_KaohSiung        936 //日月光 高雄.
#define CC_ASE_M                937 //日月光 Malaysia.
#define CC_ASE_KaohSiung_K3     938 //日月光 高雄K3
#define CC_ASE_KaohSiung_K11    939 //日月光 高雄K11 AOSH-1000.

#define CC_UTAC                 940 //Only for Open/Short Handler
#define CC_SIGURD_HUKOU         941 //矽格 湖口
#define CC_RFMD_BEIJING         942 //RFMD 北京.
#define CC_SCC                  943 //SC 上海.
#define CC_SCS                  944 //SC 新加坡.
#define CC_SIGURD_ChungXing     945 //矽格 中興
#define CC_SIGURD_PeiXing       946 //矽格 北興
#define CC_SCK                  947 //星科金朋 - 韓國
#define CC_RFMD_USA             948 //RFMD 美國
#define CC_ASE_KS               949 //日月光 崑山.

#define CC_APTOS                950 //群豐.
#define CC_WINBOND              951 //華邦.
#define CC_G_Link               952 //G Link.
#define CC_AMKOR                953 //艾克爾.
#define CC_GIGA                 954 //GIGA.
#define CC_LINGSEN              955 //菱生.
#define CC_Greatek              956 //超豐.
#define CC_PTI                  957 //力成
#define CC_THAILIN              958 //泰林
#define CC_JCET                 959 //江蘇長電

#define CC_OSE                  960 //華泰.
#define CC_MTI                  961 //聚成.
#define CC_NUVOTON              962 //新唐
#define CC_UPRTEK               963 //群耀.
#define CC_Eutrend              964 //聿勤                                      //pig 2012.02.13 EutrendFTP
#define CC_TICP                 965 //高雄典範.                                 //pig 2012.01.06 典範
#define CC_THEIL                966 //同欣電子
#define CC_TERAPOWER            967 //晶兆成
#define CC_RICHTEK              968 //立錡                                      //Alick 20161206 add 新增 客戶碼
#define CC_TSI                  969 //誠遠科技

#define CC_GIGAS                970 //全智
#define CC_AMKOR_Korea          971 //安靠韓國.
#define CC_AMKOR_China          972 //安靠上海.
#define CC_AMKOR_Japan          973 //安靠日本.
#define CC_AMKOR_Philippines    974 //安靠菲律賓

#define CC_AnalogDevice_Phil    976 //Analog菲律賓

#define CC_INTEL_M              979 // INTEL Malaysia
#define CC_CARSEM_M             980 // Carsem Malaysia
#define CC_UNISEM_M             981 // Unisem Malaysia
#define CC_AMD_M                982 // AMD Malaysia                             //pig 2016.07.21 ModifyAMD
#define CC_INARI_M              983 // INARI Malaysia                           // 2016.10.12 , Joye , ADD Customer Code
#define CC_ONSEMI_M             984 // INARI Malaysia                           // 2016.10.12 , Joye , ADD Customer Code

#define CC_MAXIM                985
#define CC_MARVELL              986
#define CC_ATMEL                987
#define CC_USI                  988 //環鴻科技
#define CC_DYNACARD             989 //新東亞微電子

#define CC_CYPRESS              990
#define CC_GERADTECH_CHINA      991 // 智瑞達
#define CC_GM_TEST              992
#define CC_I_TECH               993
#define CC_WIN_PAC              994
#define CC_SILICON_LABS_SG      995
#define CC_SILICON_LABS_SZ      996
#define CC_Altera_USA           997
#define CC_ETRENDTECH           998 //逸昌科技                                  //KaiHuang 20201214 Add
#define CC_QUALCOMM             999 //高通


//====================================================

#define MAX_IONFAN 8       //2013-08-28    Dell
