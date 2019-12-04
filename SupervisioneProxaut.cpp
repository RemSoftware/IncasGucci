//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("PosVuoti.cpp", PosVuotiForm);
USEFORM("PosPresse.cpp", PosPresseForm);
USEFORM("Password.cpp", PasswordDlg);
USEFORM("plcCarico.cpp", FormPlcCarico);
USEFORM("scaffale.cpp", frScaffale); /* TFrame: File Type */
USEFORM("UDClist.cpp", FormUDCList);
USEFORM("tab_eventi.cpp", fTabEventi);
USEFORM("segnaliBaia.cpp", fSegnaliBaia);
USEFORM("stato_pressa.cpp", FormDatiPressa);
USEFORM("stato_pos.cpp", fCorsia);
USEFORM("MSG.cpp", MSGForm);
USEFORM("Chiusura.cpp", ChiusuraForm);
USEFORM("ChildWin.cpp", MDIChild);
USEFORM("datamodulecomandiagv.cpp", dmComandiAgv); /* TDataModule: File Type */
USEFORM("config.cpp", frConfig);
USEFORM("BaiaScarico.cpp", FormBaiaScarico);
USEFORM("AGVFrameL.cpp", frAgvL); /* TFrame: File Type */
USEFORM("baiacarico.cpp", FormBaiaCarico);
USEFORM("DB.cpp", dmDB); /* TDataModule: File Type */
USEFORM("Main.cpp", MainForm);
USEFORM("missioni_wms.cpp", fMissioniWms);
USEFORM("missioni.cpp", fMissioni);
USEFORM("dmgestionePLC.cpp", TDMgestionePLC); /* TDataModule: File Type */
USEFORM("dmgestione.cpp", DMGestioneEventi); /* TDataModule: File Type */
USEFORM("ins_UDC.cpp", FormInsUDC);
USEFORM("UDCDuplicati.cpp", FormUDCDuplicati);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	HANDLE mutex;
	try
	{
		UnicodeString MutexName = ExtractFileName(Application->ExeName).UpperCase();

		mutex = OpenMutex(MUTEX_ALL_ACCESS, false, MutexName.c_str());
		if (mutex == NULL) {
			mutex = CreateMutex(NULL, true, MutexName.c_str());
		} else {
			exit(EXIT_SUCCESS);
		}
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TdmDB), &dmDB);
         Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->CreateForm(__classid(TChiusuraForm), &ChiusuraForm);
         Application->CreateForm(__classid(TdmComandiAgv), &dmComandiAgv);
         Application->CreateForm(__classid(TDMGestioneEventi), &DMGestioneEventi);
         Application->CreateForm(__classid(TPasswordDlg), &PasswordDlg);
         Application->CreateForm(__classid(TMSGForm), &MSGForm);
         Application->CreateForm(__classid(TfTabEventi), &fTabEventi);
         Application->CreateForm(__classid(TfCorsia), &fCorsia);
         Application->CreateForm(__classid(TPosVuotiForm), &PosVuotiForm);
         Application->CreateForm(__classid(TTDMgestionePLC), &TDMgestionePLC);
         Application->CreateForm(__classid(TFormBaiaScarico), &FormBaiaScarico);
         Application->CreateForm(__classid(TFormUDCList), &FormUDCList);
         Application->CreateForm(__classid(TFormInsUDC), &FormInsUDC);
         Application->CreateForm(__classid(TFormPlcCarico), &FormPlcCarico);
         Application->CreateForm(__classid(TFormUDCDuplicati), &FormUDCDuplicati);
         Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	ReleaseMutex(mutex);
	return 0;
}
//---------------------------------------------------------------------------
