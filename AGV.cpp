//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AGV.h"
#include "Main.h"
#include "UDCDuplicati.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CHILDWIN"
#pragma link "AGVFrameL"
#pragma resource "*.dfm"
TfAGV *fAGV;
extern char N_carrelli ;
//---------------------------------------------------------------------------
__fastcall TfAGV::TfAGV(TComponent* Owner)
	: TMDIChild(Owner)
{
	first = true ;
	TimerUpdTimer(NULL);
}
//---------------------------------------------------------------------------

void TfAGV::ColoraShape(int stato, TShape *t, TColor ColorON, TColor ColorOFF)
{
	if (stato) {
		t->Brush->Color = ColorON;
	} else {
		t->Brush->Color = ColorOFF;
	}
}
//---------------------------------------------------------------------------

AnsiString NavDesc(char n) {
	switch(n) {
	case 'A': return "'A' - Assente";
	case 'I': return "'I' - Idle";
	case 'R': return "'R' - Running";
	case 'S': return "'S' - Stopped";
	case 'W': return "'W' - Waiting";
	case 'B': return "'B' - Blocked";
	case 'D': return "'D' - Done";
	case 'E': return "'E' - Error";
	}
	return "?";
}
//---------------------------------------------------------------------------

AnsiString AutoDesc(char n) {
	switch(n) {
	case 'A': return "'A' - Automatico";
	case 'S': return "'S' - Semiautomatico";
	case 'M': return "'M' - Manuale";
	}
	return "?";
}
//---------------------------------------------------------------------------

AnsiString CaricoDesc(char n) {
	switch(n) {
	case 'E': return "'E' - Scarico";
	case 'R': return "'R' - Carico 1";
	case 'L': return "'L' - Carico 2";
	case 'F': return "'F' - Carico completo";
	}
	return "?";
}
//---------------------------------------------------------------------------

void __fastcall TfAGV::TimerUpdTimer(TObject *Sender)
{
	short i, n;
	int h_pallet ;
	char statoagv[1];
	AnsiString s,statoagvstr,filename,lotto;
	TfrAgvL *frAgvL;

	TimerUpd->Enabled = false;
	active = true ;
	for (i = 1; i <= N_carrelli; i++) {
		frAgvL = (TfrAgvL*)(FindComponent("frAgvL" + IntToStr(i)));
		frAgvL->aggiornamento = true ;
		frAgvL->lePosizione->Text = MainForm->DatiAgv[i].pos;
		frAgvL->leLivBatt->Text = MainForm->DatiAgv[i].LivelloBatteria;
		frAgvL->leDestinazione->Text = MainForm->DatiAgv[i].dest;
		frAgvL->leMissione->Text = MainForm->DatiAgv[i].idmis;
		frAgvL->cbAuto->Checked = (!MainForm->DatiAgv[i].manuale && !MainForm->DatiAgv[i].semiautomatico);
		frAgvL->cbMan->Checked = MainForm->DatiAgv[i].manuale ;
		frAgvL->cbAlarm->Checked = MainForm->DatiAgv[i].alarm ;
		frAgvL->cbSemiAuto->Checked = MainForm->DatiAgv[i].semiautomatico ;
		frAgvL->cbLoaded->Checked = MainForm->DatiAgv[i].load ;
		frAgvL->cbCarica->Checked = MainForm->DatiAgv[i].incarica ;
		frAgvL->cbBattBassa->Checked = MainForm->DatiAgv[i].bitbattbassa ;
		frAgvL->leIdArticolo->Text = MainForm->DatiAgv[i].IdUdc;
		frAgvL->lePrel->Text = MainForm->DatiAgv[i].prel;
		frAgvL->leDest->Text = MainForm->DatiAgv[i].dep;
		frAgvL->leFasciatura->Text = MainForm->DatiAgv[i].programma_fasciatura;
		//frAgvL->leFormato->Text = MainForm->DatiAgv[i].formato_pallet;

		if ((!first)&&(((MainForm->DatiAgv[i].load)&&(!frAgvL->cbLoaded->Checked))||
			((MainForm->DatiAgv[i].load)&&(!frAgvL->cbLoaded->Checked))))
			first = true ; // ricarico dati pallet

		if (first) {
			if (MainForm->DatiAgv[i].load)
				frAgvL->leCodicePallet->Text = dmDB->ReturnTipoPalletStr(MainForm->DatiAgv[i].tipopallet);
			else
				frAgvL->leCodicePallet->Text = "Nessun Pallet";
		}
		if ((first)||(!MainForm->pwdlevel))
			frAgvL->cbInMissione->Checked = MainForm->DatiAgv[i].generata ;
/*		switch(MainForm->DatiAgv[i].statomis) {
			case 1 :
					frAgvL->leStatoMis->Text = "GENERATA" ;
					break ;
			case 2 :
					frAgvL->leStatoMis->Text = "INIZIATA" ;
					break ;
			case 3 :
					frAgvL->leStatoMis->Text = "PRELEVATO" ;
					break ;
			default :
					frAgvL->leStatoMis->Text = "NO MISS" ;
					break ;
		}    */
		if (MainForm->DatiAgv[i].load)
			filename = "figurino_sicon_load.png";
		else
			filename = "figurino_sicon_unload.png";

		if (((MainForm->DatiAgv[i].imgfile_agv != filename)||(first))&&(FileExists(filename))) {
			frAgvL->Image1->Picture->LoadFromFile(filename);
			MainForm->DatiAgv[i].imgfile_agv = filename ;
		}
	   /*	if (TabAGV[i]["ABILITATO"].ToIntDef(0)) {
			frAgvL->Panel1->Color = clBtnFace ;
		} else {
			frAgvL->Panel1->Color = clRed ;
		}  */
		frAgvL->aggiornamento = false ;
		frAgvL->pInMis->Enabled = MainForm->pwdlevel ;
	}
	active = false ;
	first = false ;
	TimerUpd->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfAGV::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Shift.Contains(ssCtrl) && (Key == VK_DELETE))
		Key = 0; // ignore
}
//---------------------------------------------------------------------------

void __fastcall TfAGV::frAgvL1cbInMissioneClick(TObject *Sender)
{
/*TCheckBox *Check ;
	if ((MainForm->pwdlevel)&&(!active)) {
		Check = (TCheckBox *) Sender;
		if ((Check !=NULL)&&(Check->Tag)) {
			if (Application->MessageBox(L"Sicuro di voler modificare lo stato della memoria di missione creata sull'Agv?", L"Conferma!!!", MB_YESNO) == IDYES) {
				MainForm->DatiAgv[Check->Tag].generata = Check->Checked ;
				dmDB->LogMsg("Modificata memoria generata su Agv "+IntToStr(Check->Tag)+" a : "+BoolToStr(Check->Checked,true));
			}

        }
	}
	*/
}
//---------------------------------------------------------------------------

void __fastcall TfAGV::frAgvL1ForzaIdCoilsuAgv1Click(TObject *Sender)
{
	//
	AnsiString code ;
	if (MainForm->DatiAgv[1].load) {
		code = InputBox(L"Inserire Codice", L"Codice Udc", L"0");
		if (code != "0")
			MainForm->DatiAgv[1].IdUdc = StrToInt(code) ;
	}
}
//---------------------------------------------------------------------------


void __fastcall TfAGV::FormActivate(TObject *Sender)
{
	first = true ;
}
//---------------------------------------------------------------------------


void __fastcall TfAGV::CruscottoBtnClick(TObject *Sender)
{
    FormUDCDuplicati->Show();   
}
//---------------------------------------------------------------------------

