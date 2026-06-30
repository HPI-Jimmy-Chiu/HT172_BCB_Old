# HT172 — Claude Code 專案指引

本檔由 Claude Code 自動載入。最重要的兩條規則放在最前面。

---

## 🔴 規則 1:Big5/CP950 原始碼 — 絕不可用 Edit / Write / MultiEdit 工具

HT172 的 `.cpp` / `.h` / `.dfm` 全部是 **Big5 (CP950) / CRLF** 編碼,內含繁體中文註解。

**已實測證實(2026-06-30,hex 級驗證):**
- 用 harness 的 **Edit** 或 **Write** 工具碰這些檔 → 中文每個 byte 被換成 `EF BF BD`(U+FFFD),**永久損毀且不可復原**(77 → 98/99 bytes)。
- 原因:harness 的 Read/Edit/Write 把檔當 UTF-8 處理,Big5 bytes 解碼即變 U+FFFD,寫回時定型。
- 唯一安全做法 = **PowerShell CP950 round-trip**(見下;實測只改到目標 ASCII byte,中文 0 損)。

### 安全編輯法(只有這一種)
```powershell
$enc  = [System.Text.Encoding]::GetEncoding(950)        # Big5 / CP950
$text = $enc.GetString([System.IO.File]::ReadAllBytes($p))
$text = $text.Replace('舊ASCII片段', '新ASCII片段')      # 只改 ASCII;新增註解一律英文
[System.IO.File]::WriteAllBytes($p, $enc.GetBytes($text))
```
- 把以上寫成 `.ps1` 檔再執行 —— 內嵌的長 byte 操作會被本機防毒/AMSI 誤擋。
- 若已損毀:`git checkout -- <path>` 從 HEAD 還原;U+FFFD 無從修復。

### 自動防護(已安裝)
`.claude/hooks/guard-big5.ps1` 是 PreToolUse hook(matcher `Write|Edit|MultiEdit`):
目標檔若不是合法 UTF-8(即 Big5 舊檔)會被**擋下並提示**改用 PowerShell CP950;ASCII / UTF-8 檔正常放行。
任何錯誤路徑一律 fail-open(放行),不會卡住正常編輯。
> hook 在 Claude Code 啟動時載入;新裝後可能要重開 session 才生效。本檔的人為紀律才是第一道防線。

---

## 🟢 規則 2:Codegraph — 程式碼導航用,但別把它的輸出貼回檔案

`HT172_Program_V1.0.25.0_20260420/` 已建 codegraph 索引(215 檔 / 10,634 nodes / 26,643 edges;含 C++ 類別/方法/enum 與 .dfm 表單)。

**用途:** 取代 grep + Read 迴圈。改動沒有測試保護的舊碼前,先看 blast radius / callers。
- 探索:MCP `codegraph_explore`,或 CLI `codegraph explore "..."`
- 呼叫者 / 影響面:`codegraph callers <symbol>` / `codegraph impact <symbol>`
- 改完同步:`codegraph sync "d:\HT172\HT172_Program_V1.0.25.0_20260420"`
  (MCP 連線時會自動 watch 同步,無獨立 daemon 指令可下)
- 全量重建:`codegraph index "<path>"`;狀態:`codegraph status "<path>"`

**⚠️ Big5 注意:** codegraph 以 UTF-8 讀檔,所以它回傳的原始碼中**中文註解是亂碼(U+FFFD)**。
- 只用它「找符號 / 看 call-path / 看 blast radius」。
- 要看真正位元組(尤其含中文)請用 Read,或 PowerShell CP950。
- **絕不可**把 codegraph 輸出的程式碼複製貼回 `.cpp` —— 會把 Big5 毀掉(同規則 1)。
- 索引唯讀,只寫 `.codegraph/`(~19MB,已 gitignore,本機用),不會改到原始碼。
