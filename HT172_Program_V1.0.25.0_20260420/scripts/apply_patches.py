# -*- coding: utf-8 -*-
"""
Apply code patches to Big5-encoded files safely.
All replacements use raw Big5 bytes to preserve encoding.
JimmyChiu 20260416
"""
import sys

def patch_file(filepath, replacements):
    """Read file as raw bytes, apply replacements, write back."""
    with open(filepath, 'rb') as f:
        data = f.read()

    for old, new, desc in replacements:
        old_bytes = old.encode('big5') if isinstance(old, str) else old
        new_bytes = new.encode('big5') if isinstance(new, str) else new
        count = data.count(old_bytes)
        if count == 0:
            print(f"  [SKIP] '{desc}' - pattern not found in {filepath}")
            continue
        if count > 1:
            print(f"  [WARN] '{desc}' - pattern found {count} times, replacing first only")
        data = data.replace(old_bytes, new_bytes, 1)
        print(f"  [OK] {desc}")

    with open(filepath, 'wb') as f:
        f.write(data)
    print(f"  Saved: {filepath}\n")


# ============================================================
# Patch 1: Barcode.cpp - WaitPhoto 3s -> 10s
# ============================================================
print("=== Patching Barcode.cpp ===")
patch_file(
    r"d:\HT172\HT172_Program_V1.0.25.0_20260416\Barcode.cpp",
    [
        (
            "WaitPhotoIdle.SetSecAndOn(3);",
            "WaitPhotoIdle.SetSecAndOn(10);",
            "WaitPhoto 3s -> 10s"
        ),
    ]
)

# ============================================================
# Patch 2: Barcode.cpp - Clear socket buffer in FSM_PhotoInit
# ============================================================
print("=== Patching Barcode.cpp (socket buffer clear) ===")
patch_file(
    r"d:\HT172\HT172_Program_V1.0.25.0_20260416\Barcode.cpp",
    [
        (
            'FSM_RESULT TMyCCDIDPanel::FSM_PhotoInit()\r\n'
            '{\r\n'
            '    sClipID="";\r\n'
            '    bDoWaitPhoto=false;\r\n'
            '    return FSM_DONE;\r\n'
            '}',
            'FSM_RESULT TMyCCDIDPanel::FSM_PhotoInit()\r\n'
            '{\r\n'
            '    // Clear socket buffer to avoid stale data from previous scan\r\n'
            '    if(cSocketClipID->Active && cSocketClipID->Socket->Connected)\r\n'
            '    {\r\n'
            '        try\r\n'
            '        {\r\n'
            '            while(cSocketClipID->Socket->ReceiveLength() > 0)\r\n'
            '                cSocketClipID->Socket->ReceiveText();\r\n'
            '        }\r\n'
            '        catch(...){}\r\n'
            '    }\r\n'
            '    sClipID="";\r\n'
            '    bDoWaitPhoto=false;\r\n'
            '    return FSM_DONE;\r\n'
            '}',
            "Add socket buffer clear in FSM_PhotoInit"
        ),
    ]
)

# ============================================================
# Patch 3: aLoader.cpp - Task 570 ERROR handling + Task 575
# ============================================================
print("=== Patching aLoader.cpp (Task 570 + 575) ===")

# The old Task 570 ERROR block (Big5 encoded)
# Original comment: //Sam 20250905 : 新增ClipID編碼檢查
# Big5 for 新增: \xb3\xf8\xbc\xba  ... we need exact bytes
# Let's use a unique ASCII anchor instead

old_570 = (
    '                if(sEmpty1ID.Pos("ERROR")>0)\r\n'
    '                {\r\n'
    '                    asStr.sprintf("Error barcode ClipID=%s", sEmpty1ID);\r\n'
    '                    ShowMyMessage(asStr);\r\n'
    '                    iRetry=0;\r\n'
    '                    Task=500;\r\n'
    '                    break;\r\n'
    '                }\r\n'
)

new_570_error = (
    '                //JimmyChiu 20260416 : CCD abnormal response check (ERROR / non-numeric / unexpected string)\r\n'
    '                if(sEmpty1ID.Pos("ERROR")>0 ||\r\n'
    '                   sEmpty1ID.Pos("FAIL")>0  ||\r\n'
    '                   sEmpty1ID.Pos("NG")>0    ||\r\n'
    '                   sEmpty1ID.Pos("TIMEOUT")>0)\r\n'
    '                {\r\n'
    '                    asStr.sprintf("CCD abnormal response ClipID=%s", sEmpty1ID);\r\n'
    '                    ret=ShowSystemError("80013", K_RETRY|K_SKIP, 0, asStr);\r\n'
    '                    if(ret==K_RETRY)\r\n'
    '                    {\r\n'
    '                        iRetry=0;\r\n'
    '                        Task=500;\r\n'
    '                    }\r\n'
    '                    else  //K_SKIP\r\n'
    '                    {\r\n'
    '                        if(HSys.FuncT.iT09_TrayIDReadPos==1)\r\n'
    '                            MotTrack_Work[eTrack1]->ClearTray();\r\n'
    '                        else\r\n'
    '                            MotEmpty1_Car->ClearTray();\r\n'
    '                        Task=575;\r\n'
    '                    }\r\n'
    '                    break;\r\n'
    '                }\r\n'
)

patch_file(
    r"d:\HT172\HT172_Program_V1.0.25.0_20260416\HT172_Module\aLoader.cpp",
    [
        (old_570, new_570_error, "Task 570 ERROR -> ShowSystemError with K_RETRY|K_SKIP"),
    ]
)

# Now patch the CheckClipIDCode block to add K_SKIP
print("=== Patching aLoader.cpp (CheckClipIDCode K_SKIP) ===")

old_clipid_check = (
    '                    ShowSystemError("80012", K_RETRY, 0, asStr);\r\n'
    '                    iRetry=0;\r\n'
    '                    Task=500;\r\n'
    '                    break;\r\n'
)

new_clipid_check = (
    '                    ret=ShowSystemError("80012", K_RETRY|K_SKIP, 0, asStr);     //JimmyChiu 20260416 : add K_SKIP\r\n'
    '                    if(ret==K_RETRY)\r\n'
    '                    {\r\n'
    '                        iRetry=0;\r\n'
    '                        Task=500;\r\n'
    '                    }\r\n'
    '                    else  //K_SKIP\r\n'
    '                    {\r\n'
    '                        if(HSys.FuncT.iT09_TrayIDReadPos==1)\r\n'
    '                            MotTrack_Work[eTrack1]->ClearTray();\r\n'
    '                        else\r\n'
    '                            MotEmpty1_Car->ClearTray();\r\n'
    '                        Task=575;\r\n'
    '                    }\r\n'
    '                    break;\r\n'
)

patch_file(
    r"d:\HT172\HT172_Program_V1.0.25.0_20260416\HT172_Module\aLoader.cpp",
    [
        (old_clipid_check, new_clipid_check, "CheckClipIDCode add K_SKIP"),
    ]
)

# Add Task 575 before case 1000
print("=== Patching aLoader.cpp (add Task 575) ===")

old_case1000 = '        case 1000:\r\n'
# Find the right case 1000 - it's after the Empty1 timeout retry block
# Use more context to ensure unique match
old_before_1000 = (
    '                    Task=560;\r\n'
    '                }\r\n'
    '            }\r\n'
    '            break;\r\n'
    '        case 1000:\r\n'
    '            if(SwEmptyCW->Status()==false)\r\n'
    '                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(25);\r\n'
)

new_before_1000 = (
    '                    Task=560;\r\n'
    '                }\r\n'
    '            }\r\n'
    '            break;\r\n'
    '        case 575:   //JimmyChiu 20260416 : After skip, confirm tray removed from CCD position\r\n'
    '            if(HSys.FuncT.iT09_TrayIDReadPos==1)\r\n'
    '            {\r\n'
    '                if(SnEmptyHasTray[eTrack1]->IsOn())\r\n'
    '                    ShowMyMessage("Please remove the tray at Empty1 CCD Position");\r\n'
    '                else\r\n'
    '                    Task=1;\r\n'
    '            }\r\n'
    '            else\r\n'
    '            {\r\n'
    '                if(Sn_CCDPos[iEmpty1]->IsOn())\r\n'
    '                    ShowMyMessage("Please remove the tray at Empty1 CCD Position");\r\n'
    '                else\r\n'
    '                    Task=1;\r\n'
    '            }\r\n'
    '            break;\r\n'
    '        case 1000:\r\n'
    '            if(SwEmptyCW->Status()==false)\r\n'
    '                tEmpty1ToTrackDelay[iNowEmptyTrack].SetSecAndOn(25);\r\n'
)

patch_file(
    r"d:\HT172\HT172_Program_V1.0.25.0_20260416\HT172_Module\aLoader.cpp",
    [
        (old_before_1000, new_before_1000, "Add Task 575 sensor removal check"),
    ]
)

# ============================================================
# Patch 4: systools.cpp - Add alarm code 80013
# ============================================================
print("=== Patching systools.cpp (add alarm 80013) ===")

# Use ASCII-only anchors around the insertion point
old_systools = (
    '                                           "[1] Check empty tray ID.",\r\n'
    '                                           "[1] \\xbacT\\xbf{empty tray ID\\xaa\\xba\\xbf\\xf8\\xbaa")'  # not reliable
)

# Better approach: match the ASCII part between 80012 and 80021
# Read file and find exact bytes
with open(r"d:\HT172\HT172_Program_V1.0.25.0_20260416\systools.cpp", 'rb') as f:
    sdata = f.read()

# Find 80012 block end and 80021 block start
marker1 = b'"80012"'
marker2 = b'"80021"'
idx1 = sdata.index(marker1)
idx2 = sdata.index(marker2)

# Find the blank lines between them
between = sdata[idx1:idx2]
# Find the last ");" before 80021
end_of_80012 = between.rfind(b'");')
insert_point = idx1 + end_of_80012 + 4  # after ");\r\n"

# Check what's at that point
check = sdata[insert_point:insert_point+20]
print(f"  Insert point bytes: {check}")

new_alarm = (
    b'\r\n'
    b'    InsertAlarmCodeToDB(eOther, "80013",   "Message Error -- CCD returned abnormal response." ,          //JimmyChiu 20260416 : CCD abnormal response\r\n'
    b'                                           "Message Error -- CCD returned abnormal response!",\r\n'
    b'                                           "palWorkEmpty1ID",\r\n'
    b'                                           "[1] Check CCD connection and barcode quality.",\r\n'
    b'                                           "[1] Check CCD connection and barcode quality.");\r\n'
)

sdata = sdata[:insert_point] + new_alarm + sdata[insert_point:]

with open(r"d:\HT172\HT172_Program_V1.0.25.0_20260416\systools.cpp", 'wb') as f:
    f.write(sdata)
print("  [OK] Added alarm code 80013")
print("  Saved: systools.cpp\n")

print("=== All patches applied successfully ===")
