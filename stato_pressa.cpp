// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "stato_pressa.h"
#include "db.h"
#include "main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDatiPressa *FormDatiPressa;

// ---------------------------------------------------------------------------
__fastcall TFormDatiPressa::TFormDatiPressa(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TFormDatiPressa::BitBtnCloseClick(TObject *Sender) {
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDatiPressa::FormActivate(TObject *Sender) {
	activate = true;
/*	Panel1->Caption = "Dati Pressa " + MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].NomePressa;
	eAltPrel->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].h_prel ;
	eAltDep->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].h_dep ;   */
	Timer1Timer(this);
}

// ---------------------------------------------------------------------------


void __fastcall TFormDatiPressa::Timer1Timer(TObject *Sender) {
	Timer1->Enabled = false;
	// BitBtnChange->Enabled = (!MainForm->DatiPlc[1].generata && cbCodice->Text != "");
	// BitBtn7->Enabled = (cbCodice->Text != "");
	if ((!MainForm->Simula) || (activate)) {
/*		cmChiamaVuoti1->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataVuoti[1];
		cmChiamaVuoti2->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataVuoti[2];
		cmChiamaVuoti3->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataVuoti[3];
		cmChiamaVuoti4->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataVuoti[4];

		cbChiamataVuotiConf1->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataVuotiConfermata[1];
		cbChiamataVuotiConf2->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataVuotiConfermata[2];
		cbChiamataVuotiConf3->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataVuotiConfermata[3];
		cbChiamataVuotiConf4->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataVuotiConfermata[4];

		cbChiamaPienoConf->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataPienoConfermata;
		cbChiamaPieno->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].ChiamataPieno;
		eLampeggio->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].lampeggio;
		eContLamp->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].countLampeggio;

		ePos1->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].pos[1];
		ePos2->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].pos[2];
		ePos3->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].pos[3];
		ePos4->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].pos[4];

		eDataOra->Text = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].DataOraP;   */
		// ????????????????
	}

	if ((!MainForm->pwdlevel) || (activate)) {
/*		cbEscludi->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].escludi;
		cbTipoPal->Text = cbTipoPal->Items->Strings[MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].pallet_presente];
		cbInMissione->Checked = MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].generata ;  */
	}

	activate = false;
	// pInMis->Enabled = MainForm->pwdlevel;
	GroupBox1->Enabled = (MainForm->pwdlevel || MainForm->Simula);
	Timer1->Enabled = true;
}
// ---------------------------------------------------------------------------


void __fastcall TFormDatiPressa::BitBtnChangeClick(TObject *Sender) {
	UnicodeString str ;
	int modifica_db = 0;
/*	if ((eAltPrel->Text.ToIntDef(0) > 0) && (eAltPrel->Text.ToIntDef(0) != MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].h_prel)) {
		MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].h_prel = eAltPrel->Text.ToIntDef(0);
		modifica_db = 1;
	}
	if ((eAltDep->Text.ToIntDef(0) > 0) && (eAltDep->Text.ToIntDef(0) != MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].h_dep)) {
		MainForm->PlcRobot.DatiPulsantiera[id_pulsantiera].h_dep = eAltDep->Text.ToIntDef(0);
		modifica_db = 1;
	}
	if (modifica_db)
		dmDB->AggiornaPulsanti(id_pulsantiera);     */
	Close();
}
// ---------------------------------------------------------------------------

