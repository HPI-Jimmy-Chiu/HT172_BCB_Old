//---------------------------------------------------------------------------
#include "IncludeAllHeader.h"       //Dell 將.h統一,可加速build
#pragma hdrstop

USEFORM("main.cpp", fMain);
USEFORM("database.cpp", DataModule1);
USEFORM("mymessbox.cpp", MyMessageBox);
USEFORM("note.cpp", fNote);
USEFORM("uhome.cpp", fHome);
USEFORM("iosetview.cpp", fiosetview);
USEFORM("setup.cpp", fSetup);
USEFORM("data.cpp", fData);
USEFORM("maintenance.cpp", fMaintenance);
USEFORM("Password.cpp", fPassword);
USEFORM("uteach.cpp", fTeach);
USEFORM("uMotorTest.cpp", fMotorTest);
USEFORM("uOffset.cpp", fOffset);
USEFORM("uspeed.cpp", fSpeed);
USEFORM("LevelSetup.cpp", fLevelSetup);
USEFORM("systools.cpp", FormSysTools);
USEFORM("myQwertyKeyBoard.cpp", fQwertyKey);
USEFORM("SecsGem\uHGemEquipment.cpp", HGem);
USEFORM("SecsGem\UsecegemMainFrom.cpp", FSECS);
USEFORM("uTrayEditForm.cpp", TrayEditForm);
USEFORM("MyBinDisp.cpp", DataModule3); /* TDataModule: File Type */
USEFORM("Barcode.cpp", frm2DID);
USEFORM("ComPort.cpp", fComPort);
USEFORM("uPadInterface.cpp", fPadInterface);
USEFORM("TrayStepMotor.cpp", dmTrayMotor); /* TDataModule: File Type */
USEFORM("aseTest.cpp", ASESendMessage);
USEFORM("HT172_Module\DecodeXML.cpp", fXML);
USEFORM("FTPClient.cpp", fFTPClient);
USEFORM("Automation\automation.cpp", fAutomation);
//---------------------------------------------------------------------------
#include <winbase.h>
#include "cmydef.h"
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpstrTemp, int iLength)
{
    ::CreateMutex(NULL,FALSE,"HT-172");
    if(::GetLastError())
        return 0;
    try
    {
         Application->Initialize();
         Application->Title = "HT-172";
         Application->CreateForm(__classid(TfMain), &fMain);
         Application->CreateForm(__classid(TDataModule1), &DataModule1);
         Application->CreateForm(__classid(TMyMessageBox), &MyMessageBox);
         Application->CreateForm(__classid(TfNote), &fNote);
         Application->CreateForm(__classid(TfHome), &fHome);
         Application->CreateForm(__classid(Tfiosetview), &fiosetview);
         Application->CreateForm(__classid(TfSetup), &fSetup);
         Application->CreateForm(__classid(TfData), &fData);
         Application->CreateForm(__classid(TfMaintenance), &fMaintenance);
         Application->CreateForm(__classid(TfPassword), &fPassword);
         Application->CreateForm(__classid(TfTeach), &fTeach);
         Application->CreateForm(__classid(TfMotorTest), &fMotorTest);
         Application->CreateForm(__classid(TfOffset), &fOffset);
         Application->CreateForm(__classid(TfSpeed), &fSpeed);
         Application->CreateForm(__classid(TfLevelSetup), &fLevelSetup);
         Application->CreateForm(__classid(TFormSysTools), &FormSysTools);
         Application->CreateForm(__classid(TfQwertyKey), &fQwertyKey);
         Application->CreateForm(__classid(THGem), &HGem);
         Application->CreateForm(__classid(TFSECS), &FSECS);
         Application->CreateForm(__classid(TTrayEditForm), &TrayEditForm);
         Application->CreateForm(__classid(TDataModule3), &DataModule3);
         Application->CreateForm(__classid(Tfrm2DID), &frm2DID);
         Application->CreateForm(__classid(TfComPort), &fComPort);
         Application->CreateForm(__classid(TfPadInterface), &fPadInterface);
         Application->CreateForm(__classid(TdmTrayMotor), &dmTrayMotor);
         Application->CreateForm(__classid(TASESendMessage), &ASESendMessage);
         Application->CreateForm(__classid(TfXML), &fXML);
         Application->CreateForm(__classid(TfFTPClient), &fFTPClient);
         Application->CreateForm(__classid(TfAutomation), &fAutomation);
         Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

