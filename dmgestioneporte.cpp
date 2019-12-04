// ---------------------------------------------------------------------------

#pragma hdrstop

#include "dmgestioneporte.h"
#include "main.h"
// #include "dmphoenix.h"
#include "percorsiprototipo.h"
#include "socket.h"
#include "db.h"
#include "datamodulecomandiagv.h"
#include "PLCphoenixThread.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TDMGestione_Porte *DMGestione_Porte;
extern CLIENTSOCKET *cs;
extern DATILGV tab;

// ---------------------------------------------------------------------------
__fastcall TDMGestione_Porte::TDMGestione_Porte(TComponent* Owner)
	: TDataModule(Owner)
{
	sync_lamp = false;
	init = true;
	for (int i = 0; i < 10; i++)
		Postazioni_Libere_Antincendio[i] = 0;
	Postazioni_Libere_Antincendio[0] = 318;
	Postazioni_Libere_Antincendio[1] = 319;
	Postazioni_Libere_Antincendio[2] = 152;

}

// ---------------------------------------------------------------------------
void TDMGestione_Porte::LeggiStrutturaDatiPorte() {
	// aggiorno struttura porte
	AnsiString strsql, strlog = "";
	TADOQuery *ADOQuery;
	int res = 0, id = 0;
	try {

		if (!dmDB->ADOConnection1->Connected)
			return;
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = dmDB->ADOConnection1;
		strsql = "Select * from Porte ";
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = strsql;
		ADOQuery->Open();
		ADOQuery->First();
		while (!ADOQuery->Eof) {
			id = ADOQuery->FieldByName("IdPorta")->AsInteger;
			ClientData.DatiPorte[id].id = id;
			ClientData.DatiPorte[id].tipomodulo = ADOQuery->FieldByName("TipoModulo")->AsInteger;
			ClientData.DatiPorte[id].id_thread_plc = ADOQuery->FieldByName("ID_PLC")->AsInteger;
			ClientData.DatiPorte[id].Stato = ADOQuery->FieldByName("Stato")->AsInteger;
			ClientData.DatiPorte[id].DBInput = ADOQuery->FieldByName("DBInput")->AsInteger;
			ClientData.DatiPorte[id].DBOutput = ADOQuery->FieldByName("DBOutput")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_PortaAperta = ADOQuery->FieldByName("DW_Addr_PortaAperta")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_PortaChiusa = ADOQuery->FieldByName("DW_Addr_PortaChiusa")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_PortaTFuocoAperta = ADOQuery->FieldByName("DW_Addr_PortaTFuocoAperta")->AsInteger;
			ClientData.DatiPorte[id].Bit_PortaAperta = ADOQuery->FieldByName("Bit_PortaAperta")->AsInteger;
			ClientData.DatiPorte[id].Bit_PortaChiusa = ADOQuery->FieldByName("Bit_PortaChiusa")->AsInteger;
			ClientData.DatiPorte[id].Bit_PortaTFuocoAperta = ADOQuery->FieldByName("Bit_PortaTFuocoAperta")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_ApriPorta = ADOQuery->FieldByName("DW_Addr_ApriPorta")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_ChiudiPorta = ADOQuery->FieldByName("DW_Addr_ChiudiPorta")->AsInteger;
			ClientData.DatiPorte[id].Bit_ApriPorta = ADOQuery->FieldByName("Bit_ApriPorta")->AsInteger;
			ClientData.DatiPorte[id].Bit_ChiudiPorta = ADOQuery->FieldByName("Bit_ChiudiPorta")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_OkAccessoAgv = ADOQuery->FieldByName("DW_Addr_OkAccessoAgv")->AsInteger;
			ClientData.DatiPorte[id].Bit_OkAccessoAgv = ADOQuery->FieldByName("Bit_OkAccessoAgv")->AsInteger;
			ADOQuery->Next();
		}
		ADOQuery->Close();
		lettodatiporte = true;
	}
	catch (...) {
	}
	delete ADOQuery;

}

/*
 void TDMGestione_Porte::LeggiStrutturaDatiPorteFumo() {
 // aggiorno struttura porte
 AnsiString strsql, strlog = "";
 TADOQuery *ADOQuery;
 int res = 0, id = 0;
 try {

 if (!dmDB->ADOConnection1->Connected)
 return;
 ADOQuery = new TADOQuery(NULL);
 ADOQuery->Connection = dmDB->ADOConnection1;
 strsql = "Select * from PorteFumo ";
 ADOQuery->SQL->Clear();
 ADOQuery->SQL->Text = strsql;
 ADOQuery->Open();
 ADOQuery->First();
 while (!ADOQuery->Eof) {
 id = ADOQuery->FieldByName("IdPorta")->AsInteger;
 ClientData.DatiPorteFumo[id].id = id ;
 ClientData.DatiPorteFumo[id].tipomodulo = ADOQuery->FieldByName("TipoModulo")->AsInteger;
 ClientData.DatiPorteFumo[id].id_thread_plc = ADOQuery->FieldByName("ID_PLC")->AsInteger;
 ClientData.DatiPorteFumo[id].DBInput = ADOQuery->FieldByName("DBInput")->AsInteger;
 ClientData.DatiPorteFumo[id].DBOutput = ADOQuery->FieldByName("DBOutput")->AsInteger;
 ClientData.DatiPorteFumo[id].DWInput = ADOQuery->FieldByName("DWInput")->AsInteger;
 ClientData.DatiPorteFumo[id].DWOutput = ADOQuery->FieldByName("DWOutput")->AsInteger;
 ClientData.DatiPorteFumo[id].Stato = ADOQuery->FieldByName("Stato")->AsInteger;
 ADOQuery->Next();
 }
 ADOQuery->Close();
 lettodatiportefumo = true;
 }
 catch (...) {
 }
 delete ADOQuery;

 } */

void __fastcall TDMGestione_Porte::TimerEventiTimer(TObject *Sender)
{
	int i, j, agv, k, anomalia_generatori;
	Evento ev;

	// gestisco le missioni degli agv e tutti gli eventi

	if (MainForm->count_inizio <= 15)
		return;
	TimerEventi->Enabled = false;
	agv = 1;
	if (dmDB->lettostatoagv) {
		if (!lettodatiporte)
			LeggiStrutturaDatiPorte();

		if (MainForm->count_inizio > 20) {
			ControllaPorteDaAprire();
			// GestioneRotAlarm() ;
			ControllaPorteAperte();
			ControllaResetSegnaleApriChiudiPorta(dmDB->bit[1]); // vedere se serve ,tolgo solo il segnake chiusura

		}
	}
	TimerEventi->Enabled = true;
}
// ---------------------------------------------------------------------------

void TDMGestione_Porte::LogStatoPorta(int porta) {
	AnsiString strlog;
	strlog = "Stato Porta " + IntToStr(porta)
		+ " , Porta Aperta : " + IntToStr(ClientData.DatiPorte[porta].PortaAperta)
		+ " , Porta Tagliafuoco : " + IntToStr(ClientData.DatiPorte[porta].PortaTFuoco)
		+ " , Porta Chiusa : " + IntToStr(ClientData.DatiPorte[porta].PortaChiusa)
		+ " , Apri Porta : " + IntToStr(ClientData.DatiPorte[porta].ApriPorta)
		+ " , Chiudi Porta : " + IntToStr(ClientData.DatiPorte[porta].ChiudiPorta);
	dmDB->LogMsg(strlog);
}

void TDMGestione_Porte::ControllaPorteDaAprire() {

	int i, res = 0, agv, baia, porta;
	Evento ev;
	for (agv = 1; agv <= NAGV; agv++) { // la porta tagliafuoco non la controllo
		if ((ClientData.DatiAgv[agv].richiestaconsenso) && (!ClientData.DatiAgv[agv].consensodato)) {
			try {
				porta = dmDB->ReturnIdPortaDaStringaConsensoPorta(MainForm->strcons[agv]);
				if (porta) {
					res = AgvInPosAperturaPorta(porta, agv);
					if ((!ClientData.DatiPorte[porta].ApriPorta) && (ClientData.DatiPorte[porta].Stato == 1)) { // porta in automatico
						// co nla memoria di apertura non la richiudo ma faccio passare l'agv
						if (res) {
							// apro la porta
							ClientData.DatiPorte[porta].RichiestaAttiva++;
							ClientData.DatiAgv[agv].primo_ciclo_consenso = 0;
							dmDB->LogMsg("Apro Porta " + IntToStr(porta) + " per passaggio agv " + IntToStr(agv));
							ApriChiudiPorta(porta, APRIPORTA);

						}
					}
					else if (((res) || (ClientData.DatiPorte[porta].Stato >= 2)) && (!ClientData.DatiPorte[porta].ChiudiPorta) &&
						((ClientData.DatiPorte[porta].Stato == 3) ||
						((ClientData.DatiPorte[porta].PortaAperta) &&
						((ClientData.DatiPorte[porta].ApriPorta) || (ClientData.DatiPorte[porta].Stato == 2))) ||
						((ClientData.DatiPorte[porta].PortaTFuoco) &&
						((ClientData.DatiPorte[porta].Stato == 2)))))
					{ // porta esclusa oppure aperta in manuale o con richiesta
						// porta esclusa oppure aperta,invio consenso
						if (ClientData.DatiAgv[agv].primo_ciclo_consenso) // gestione inversione agv su porta già aperta
								ClientData.DatiPorte[porta].RichiestaAttiva++;
						ClientData.DatiAgv[agv].consensodato = ClientData.DatiAgv[agv].nodob;
						ClientData.DatiAgv[agv].richiestaconsenso = 0;
						dmDB->LogMsg("Consenso attraversamento porta inviato ad Agv " + IntToStr(agv) + " in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " ,dest " + IntToStr(ClientData.DatiAgv[agv].dest));
						LogStatoPorta(porta);
						sprintf(ev, "O,R,%d,%d,%d,%d", agv, ClientData.DatiAgv[agv].nodoa, ClientData.DatiAgv[agv].nodob, 1);
						cs->SendEv(ev);
						dmDB->AggiornaStatoAgv(agv);
						if (ClientData.DatiAgv[agv].allarme_consenso_porta) {
							MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso_porta);
							ClientData.DatiAgv[agv].allarme_consenso_porta = 0;
							dmDB->AggiornaAllarmiAttivi();
						}
					}
					else if ((!ClientData.DatiAgv[agv].allarme_consenso_porta) && (ClientData.DatiAgv[agv].pos == ClientData.DatiAgv[agv].nodoa)) {
						MainForm->MessageBar1->Messages->Strings[ClientData.DatiAgv[agv].num_allarme_consenso_porta] = "Agv in Attesa apertura porta " + IntToStr(porta);
						MainForm->MessageBar1->AddMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso_porta);
						ClientData.DatiAgv[agv].allarme_consenso_porta = 1;
						dmDB->AggiornaAllarmiAttivi();
					}
					ClientData.DatiAgv[agv].primo_ciclo_consenso = 0;

				}
			}
			catch (...) {
				dmDB->LogMsg("Eccezione controllo str apertura porte con stringa ricevuta : " + MainForm->strcons[agv]);
			}
		}
	}
}

int TDMGestione_Porte::AggiungiRimuoviRichiestaConsensoPorta(AnsiString strconsenso, int aggiungi) {

	int j, res = 0;
	AnsiString str_porta;

	j = dmDB->ReturnIdPortaDaStringaConsensoPorta(strconsenso);
	if ((j > 0) && (j <= NUM_PORTE)) { // la porta tagliafuoco non la controllo
		if ((!aggiungi) && (ClientData.DatiPorte[j].RichiestaAttiva > 0))
			ClientData.DatiPorte[j].RichiestaAttiva--;
		else
			ClientData.DatiPorte[j].RichiestaAttiva = aggiungi;
		res = 1;
		dmDB->LogMsg("AggiungiRimuoviRichiestaConsensoPorta str " + strconsenso + " valore attuale : " + IntToStr(ClientData.DatiPorte[j].RichiestaAttiva));
	}
	return res;
}

int TDMGestione_Porte::ControllaPorteAperte() {

	int i, res = 0, agv_in_pos_apertura = 0, consenso_dato_agv_su_porta = 0;

	for (i = 1; i <= NUM_PORTE; i++) {
		if (((ClientData.DatiPorte[i].PlcConnesso) || (ClientData.Simula)) &&
			(!ClientData.DatiPorte[i].forzature_attive)) {
			if ((ClientData.DatiPorte[i].PortaAperta) || (ClientData.DatiPorte[i].ApriPorta) ||
				(ClientData.DatiPorte[i].PortaTFuoco)) {
				res = AgvInPosPorta(i);
				agv_in_pos_apertura = AgvInPosAperturaPorta(i);
				consenso_dato_agv_su_porta = ConsensoInviatoAdAgvInPosAperturaPorta(i);
				if (dmDB->StatoPorta(i) != 1) {
					// porta non in automatico
					if ((ClientData.DatiPorte[i].ApriPorta) || (ClientData.DatiPorte[i].ChiudiPorta) || (ClientData.DatiPorte[i].ChiudiPorta)) {
						// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPhoenix[0].bit_apri_porta[i],ClientData.DatiPhoenix[0].addr_apri_porta[i],false) ;
						ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
						dmDB->LogMsg("Abbasso segnale porta " + IntToStr(i) + " perchè porta non in automatico");
					}
					if (ClientData.DatiPorte[i].ChiudiPorta) {
						// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPhoenix[0].bit_chiudi_porta[i],ClientData.DatiPhoenix[0].addr_chiudi_porta[i],false) ;
						// dmDB->LogMsg("Abbasso segnale chiusura porta "+IntToStr(i)+" perchè porta non in automatico");
					}
				}
				else if ((!res) && (!consenso_dato_agv_su_porta) && (!ClientData.DatiPorte[i].count_comando) &&
					((!agv_in_pos_apertura) || (!ClientData.DatiPorte[i].RichiestaAttiva))) {
					// chiudo la porta
					if (!ClientData.DatiPorte[i].memoria_apertura_operatore) {
						ClientData.DatiPorte[i].RichiestaAttiva = 0;
						if (ClientData.DatiPorte[i].ApriPorta) // || (!ClientData.DatiPorte[i].ChiudiPorta))
								dmDB->LogMsg("Chiudo Porta " + IntToStr(i) + " perchè nessun agv in zona");
						// sostituire con   ApriChiudiPorta(i, ABBASSASEGNALIPORTA); ???
						ApriChiudiPorta(i, CHIUDIPORTA);
					}
					else {
						// tolgo il segnale di apertura se cè la memoria dell'apertura operatore
						if (ClientData.DatiPorte[i].ApriPorta)
							dmDB->LogMsg("Abbasso Segnali Porta " + IntToStr(i) + " perchè agv transitato e memoria operatore alta");
						ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
					}
					// dmDB->AggiornaParametri(i);
				}
				else if ((res) && (consenso_dato_agv_su_porta) && (!ClientData.DatiAgv[consenso_dato_agv_su_porta].manuale)) {
					// chiudo la porta
					if (((!ClientData.DatiPorte[i].PortaAperta) && (ClientData.DatiPorte[i].ApriPorta)) ||
						(ClientData.DatiPorte[i].ChiudiPorta)) {
						if (!ClientData.DatiAgv[consenso_dato_agv_su_porta].stand_by) {
							dmComandiAgv->StandByAgv(consenso_dato_agv_su_porta, 1);
							dmDB->LogMsg("Blocco agv " + IntToStr(consenso_dato_agv_su_porta) + " perchè segnale porta aperta mancante su porta " + IntToStr(i));
							LogStatoPorta(i);
						}
					}
					else if ((ClientData.DatiAgv[consenso_dato_agv_su_porta].stand_by) && (!ClientData.DatiPorte[i].ChiudiPorta)) {
						if ((ClientData.DatiPorte[i].PortaAperta) && (ClientData.DatiPorte[i].ApriPorta)) {
							if (!ClientData.DatiAgv[consenso_dato_agv_su_porta].stand_by_antincendio) {
								dmComandiAgv->StandByAgv(consenso_dato_agv_su_porta, 0);
								dmDB->LogMsg("Riavvio agv " + IntToStr(consenso_dato_agv_su_porta) + " perchè segnale porta aperta ritornato su porta " + IntToStr(i));
								LogStatoPorta(i);
							}
						}
					}
				}
			}
		}
	}
	return res;
}

int TDMGestione_Porte::AgvInPostazioni_Libere_Antincendio(int agv) {
	int i, res = 0;
	AnsiString pos_porta_aperta;
	for (i = 0; i < 10; i++) {
		if (ClientData.DatiAgv[agv].pos == Postazioni_Libere_Antincendio[i]) {
			res = i;
			break;
		}
	}
	return res;
}

int TDMGestione_Porte::AgvInPosPorta(int porta) {
	int i, res = 0;
	AnsiString pos_porta_aperta;
	if ((porta > 0) && (porta <= NUM_PORTE)) {
		pos_porta_aperta = dmDB->PosizioniPortaAperta(porta);
		for (i = 1; i <= NAGV; i++) {
			if (pos_porta_aperta.Pos(";" + IntToStr(ClientData.DatiAgv[i].pos) + ";")) {
				res = i;
				break;
			}
		}
	}
	return res;
}

int TDMGestione_Porte::AgvInPosAperturaPorta(int porta, int controlla_agv) {
	int i, res = 0;
	AnsiString pos_porta_aperta, pos_apertura;
	if ((porta > 0) && (porta <= NUM_PORTE)) {
		pos_apertura = dmDB->PosizioniRichiestaAperturaPorta(porta);
		pos_porta_aperta = dmDB->PosizioniPortaAperta(porta);
		for (i = 1; i <= NAGV; i++) {
			if ((controlla_agv == 0) || (controlla_agv == i)) {
				if ((pos_apertura.Pos(";" + IntToStr(ClientData.DatiAgv[i].pos) + ";")) ||
					(pos_porta_aperta.Pos(";" + IntToStr(ClientData.DatiAgv[i].pos) + ";"))) {
					res = i;
					break;
				}
			}
		}
	}
	return res;
}

int TDMGestione_Porte::ConsensoInviatoAdAgvInPosAperturaPorta(int porta, int controlla_agv) {
	int i, res = 0;
	AnsiString pos_porta_aperta, pos_apertura;
	// ulteriore controll odi non avere un consenso dato su una posizone presente nella lista della porta che voglio chiudere
	if ((porta > 0) && (porta <= NUM_PORTE)) {
		pos_apertura = dmDB->PosizioniRichiestaAperturaPorta(porta);
		pos_porta_aperta = dmDB->PosizioniPortaAperta(porta);
		for (i = 1; i <= NAGV; i++) {
			if ((controlla_agv == 0) || (controlla_agv == i)) {
				if ((pos_apertura.Pos(";" + IntToStr(ClientData.DatiAgv[i].consensodato) + ";")) ||
					(pos_porta_aperta.Pos(";" + IntToStr(ClientData.DatiAgv[i].consensodato) + ";"))) {
					res = i;
					break;
				}
			}
		}
	}
	return res;
}

int TDMGestione_Porte::ApriChiudiPorta(int porta, int apri) {
	int i, res = 0;

	if ((porta > 0) && (porta <= NUM_PORTE)) {
		// DataModulePhoenix->ScriviPhoenix(MainForm->PlcPhoenix[baia].bit_apri_chiudi_porta[porta],MainForm->PlcPhoenix[baia].add_apri_chiudi_porta[porta],true,baia) ;
		if (ClientData.DatiPorte[porta].tipomodulo == 2) {
			// modulo phoenix
			if (apri == 1) {
				// alzo aoertura e abbasso chiusura
				if (ClientData.DatiPorte[porta].Bit_ApriPorta >= 0)
					PLCPhoenixThread[ClientData.DatiPorte[porta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[porta].Bit_ApriPorta, ClientData.DatiPorte[porta].DW_Addr_ApriPorta, true);
				// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPhoenix[0].bit_chiudi_porta[porta],ClientData.DatiPhoenix[0].addr_chiudi_porta[porta],false) ;
			}
			else {
				// alzo chiusura e abbasso apertura
				if (ClientData.DatiPorte[porta].Bit_ApriPorta >= 0)
					PLCPhoenixThread[ClientData.DatiPorte[porta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[porta].Bit_ApriPorta, ClientData.DatiPorte[porta].DW_Addr_ApriPorta, false);

			}
		}
		/* else {
		 if (apri == 1) {
		 // apro porta totale
		 ClientData.DatiPorte[porta].uscitedascrivere = ClientData.DatiPorte[porta].uscitedascrivere | dmDB->bit[ClientData.DatiPorte[porta].Bit_ApriPorta];
		 ClientData.DatiPorte[porta].uscitedascrivere = (ClientData.DatiPorte[porta].uscitedascrivere & dmDB->bitAnd[ClientData.DatiPorte[porta].Bit_ChiudiPorta]); // resetto chiudi e apri meta
		 }

		 else if (apri == 2) {
		 // apro porta parziale
		 // ClientData.DatiPorte[porta].uscitedascrivere = ClientData.DatiPorte[porta].uscitedascrivere | dmDB->bit[ClientData.DatiPorte[porta].Bit_ApriPortaMeta];
		 ClientData.DatiPorte[porta].uscitedascrivere = (ClientData.DatiPorte[porta].uscitedascrivere & dmDB->bitAnd[ClientData.DatiPorte[porta].Bit_ChiudiPorta]) & dmDB->bitAnd[ClientData.DatiPorte[porta].Bit_ApriPorta]; // resetto chiudi e apri meta
		 }

		 else if (apri == 3) {
		 // chiudo porta
		 ClientData.DatiPorte[porta].uscitedascrivere = ClientData.DatiPorte[porta].uscitedascrivere | dmDB->bit[ClientData.DatiPorte[porta].Bit_ChiudiPorta];
		 // ClientData.DatiPorte[porta].uscitedascrivere = (ClientData.DatiPorte[porta].uscitedascrivere & dmDB->bitAnd[ClientData.DatiPorte[porta].Bit_ApriPortaMeta]) & dmDB->bitAnd[ClientData.DatiPorte[porta].Bit_ApriPorta]; // resetto chiudi e apri meta
		 }
		 else {
		 // resetto tutto
		 ClientData.DatiPorte[porta].uscitedascrivere = (ClientData.DatiPorte[porta].uscitedascrivere & dmDB->bitAnd[ClientData.DatiPorte[porta].Bit_ApriPorta]) & dmDB->bitAnd[ClientData.DatiPorte[porta].Bit_ChiudiPorta]; // resetto chiudi e apri meta
		 }
		 } */
	}
	return res;
}

void TDMGestione_Porte::ControllaResetSegnaleApriChiudiPorta(int tiporeset) {
	int i;
	// vedere se serve resettare il comando
	// se si resetta il consenso va dato solo col segnale apertura
	// tiporeset parametrizzo se devo togliereil segnale apri porta con porta aperta, e chiudi porta con porta non aperta

	for (i = 1; i <= NUM_PORTE; i++) {
		if (((ClientData.DatiPorte[i].PlcConnesso) || (ClientData.Simula)) &&
			(!ClientData.DatiPorte[i].forzature_attive)) {
			if ((ClientData.DatiPorte[i].PortaAperta) && (ClientData.DatiPorte[i].ApriPorta) && (tiporeset & dmDB->bit[0])) {
				// abbasso apri porta dopo segnale porta aperta
				// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPorte[i].bit_apri_porta[i],ClientData.DatiPorte[i].addr_apri_porta[i],false) ;
				ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
				dmDB->LogMsg("Abbasso segnale apertura porta " + IntToStr(i) + " con porta aperta e comando alto");
			}
			else if ((ClientData.DatiPorte[i].PortaChiusa) && (ClientData.DatiPorte[i].ChiudiPorta) && (tiporeset & dmDB->bit[1]) && (dmDB->StatoPorta(i) == 1)) {
				// abbasso apri porta dopo segnale porta chiusa
				ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
				dmDB->LogMsg("Abbasso segnale chiusura porta " + IntToStr(i) + " con porta chiusa ");
				// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPorte[i].bit_chiudi_porta[i],ClientData.DatiPorte[i].addr_chiudi_porta[i],false) ;
				// dmDB->LogMsg("Abbasso segnale chiusura porta "+IntToStr(i)+" con porta non più aperta e comando alto");
			}
		}
	}
}

int TDMGestione_Porte::GestioneRotAlarm() {

	int i, j, res = 0, value;

	/* for (i=1 ; i<= MainForm->num_rot_alarm ; i++)  {
	 if ((!MainForm->DatiPlc[1].forzatura_attiva)&&((MainForm->DatiPlc[1].PlcConnesso)||(ClientData.Simula))) {
	 res = AgvInPosRotAlarm(i) ;
	 if (((!res)&&(MainForm->DatiPlc[1].rot_alarm[i]))||
	 ((res)&&(!MainForm->DatiPlc[1].rot_alarm[i]))){
	 //accendo - spengo luce rotalarm
	 SettaRotAlarm(i,res);
	 }
	 }
	 } */
}

int TDMGestione_Porte::AgvInPosRotAlarm(int rotalarm) {
	int i, res = 0;

	/* for ( i = 1 ; i<= NAGV ; i++) {
	 //vedere se accendere solo se l'agv è in missione
	 if ((MainForm->DatiPlc[1].pos_rot_alarm[rotalarm].Pos (";"+IntToStr(ClientData.DatiAgv[i].pos)+";"))) {
	 res = i ;
	 break ;
	 }
	 } */
	return res;
}

int TDMGestione_Porte::SettaRotAlarm(int rotalarm, bool setta) {
	int i, res = 0;

	/* if ((rotalarm > 0)&&(rotalarm <= MainForm->num_rot_alarm)) {
	 if (setta) {
	 MainForm->DatiPlc[1].statorot_alarm_dascrivere |= dmDB->bit[MainForm->DatiPlc[1].bitrot_alarm[rotalarm]] ;
	 dmDB->LogMsg("Setto Luce RotAlarm "+IntToStr(rotalarm)+" , posRotAlarm : "+MainForm->DatiPlc[1].pos_rot_alarm[rotalarm]);
	 }else {
	 MainForm->DatiPlc[1].statorot_alarm_dascrivere &= MainForm->bitAnd[MainForm->DatiPlc[1].bitrot_alarm[rotalarm]] ;
	 dmDB->LogMsg("Reseetto Luce RotAlarm "+IntToStr(rotalarm)+" , posRotAlarm : "+MainForm->DatiPlc[1].pos_rot_alarm[rotalarm]);
	 }
	 } */
	return res;
}

void __fastcall TDMGestione_Porte::DataModuleCreate(TObject *Sender)
{
	TimerEventi->Enabled = true;
	TimerPlcPorte->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TDMGestione_Porte::TimerPlcPorteTimer(TObject *Sender)
{
	int agv, i, j;
	char code[24];
	// imposto datyi in memoria da Plc
	if ((MainForm->count_inizio < 15) || (MainForm->close_program))
		return;
	TimerPlcPorte->Enabled = false;

	for (i = 1; i <= NUM_PORTE; i++) {
		if ((lettodatiporte) && (ClientData.DatiPorte[i].id_thread_plc >= 0))
			CompilaStrutturaPlcPorte(ClientData.DatiPorte[i]);

	}
	if (MainForm->count_inizio > 20) {
		// if (ClientData.SegnaleAntincendio) {
		for (agv = 1; agv <= NAGV; agv++) {
			if ((!ClientData.DatiAgv[agv].stand_by_antincendio) && (!ClientData.DatiAgv[agv].manuale) && (ClientData.DatiAgv[agv].pos > 0)) {
				if (!AgvInPostazioni_Libere_Antincendio(agv)) {
					dmComandiAgv->StandByAgv(agv, 1);
					dmDB->LogMsg("Blocco agv " + IntToStr(agv) + " per segnale antincendio attivo ");
					ClientData.DatiAgv[agv].stand_by_antincendio = 1;
				}
			}
		}
		/*
		 if (!ClientData.AgvFermiPerAntincendio) {
		 dmDB->LogMsg("Setto navette ferme su plc per antincendio ");
		 PLCPhoenixThread[2]->WriteByte(ClientData.DatiPorte[1].DBOutput,2,dmDB->bit[0],ClientData.Simula) ;
		 }
		 */
		/* }else {
		 for(agv = 1 ; agv <= NAGV ; agv++) {
		 if ((ClientData.DatiAgv[agv].stand_by_antincendio)&&(ClientData.DatiAgv[agv].stand_by)
		 &&(!ClientData.DatiAgv[agv].manuale)&&(ClientData.DatiAgv[agv].pos > 0)) {
		 dmComandiAgv->StandByAgv(agv,0);
		 dmDB->LogMsg("Resetto stand by agv "+IntToStr(agv) +" per segnale antincendio basso ");
		 ClientData.DatiAgv[agv].stand_by_antincendio = 0 ;
		 }
		 }
		 if (ClientData.AgvFermiPerAntincendio) {
		 dmDB->LogMsg("Resetto navette ferme su plc per antincendio ");
		 PLCPhoenixThread[2]->WriteByte(ClientData.DatiPorte[1].DBOutput,2,0,ClientData.Simula) ;
		 }
		 } */
	}
	/* if ((ClientData.AgvFermiPerAntincendio)&&(!alarm_antincendio)) {
	 MainForm->MessageBar1->AddMsgNo(804);
	 alarm_antincendio = 1 ;
	 }else if ((!ClientData.AgvFermiPerAntincendio)&&(alarm_antincendio)) {
	 MainForm->MessageBar1->DelMsgNo(804);
	 alarm_antincendio = 0 ;
	 } */
	// aggiungere gestione segnali lavatrice
	init = false;
	TimerPlcPorte->Enabled = true;

}

// ---------------------------------------------------------------------------
void TDMGestione_Porte::CompilaStrutturaPlcPorte(TPorte &StructPlcPorte) {
	AnsiString stringa;
	int i, j, indice_byte;
	// compilo i dati della struttura plc passata

	StructPlcPorte.PlcConnesso = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->connesso;
	if ((PLCPhoenixThread[StructPlcPorte.id_thread_plc]->connesso) || (ClientData.Simula)) {
		// leggo stato segnale antincendio
		// ClientData.SegnaleAntincendio = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->DB[StructPlcPorte.DBInput][2] & dmDB->bit[0];
		// ingressi
		/* if ((PLCPhoenixThread[StructPlcPorte.id_thread_plc]->connesso) && (!ClientData.Simula))  {
		 PLCPhoenixThread[StructPlcPorte.id_thread_plc]->LetturaIngressi() ;
		 } */
		if ((StructPlcPorte.Bit_PortaAperta >= 0) && (StructPlcPorte.DW_Addr_PortaAperta >= 0)) {

			StructPlcPorte.PortaAperta = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->IngressiPhoenix[StructPlcPorte.DW_Addr_PortaAperta] & dmDB->bit[StructPlcPorte.Bit_PortaAperta];
		}
		// StructPlcPorte.PortaChiusa = StructPlcPorte.ByteInput & dmDB->bit[StructPlcPorte.Bit_PortaChiusa];
		if ((StructPlcPorte.Bit_PortaTFuocoAperta >= 0) && (StructPlcPorte.DW_Addr_PortaTFuocoAperta >= 0)) {
			StructPlcPorte.PortaTFuoco = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->IngressiPhoenix[StructPlcPorte.DW_Addr_PortaTFuocoAperta] & dmDB->bit[StructPlcPorte.Bit_PortaTFuocoAperta];
		}
		// StructPlcPorte.OkAccessoAgv = StructPlcPorte.ByteInput & dmDB->bit[StructPlcPorte.Bit_OkAccessoAgv];
		// leggo dati udc
		// lettura uscite
		/*
		 if ((PLCPhoenixThread[StructPlcPorte.id_thread_plc]->connesso) && (!ClientData.Simula)) {
		 PLCPhoenixThread[StructPlcPorte.id_thread_plc]->LetturaUscite() ;
		 } */
		// impostazioni variabili uscite
		if ((StructPlcPorte.Bit_ApriPorta >= 0) && (StructPlcPorte.DW_Addr_ApriPorta >= 0))
			StructPlcPorte.ApriPorta = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->UscitePhoenix[StructPlcPorte.DW_Addr_ApriPorta] & dmDB->bit[StructPlcPorte.Bit_ApriPorta];
		StructPlcPorte.memoria_apertura_operatore = 0;
	}
	// gestisco uscite :
	if (MainForm->count_inizio > 20) {

		// ClientData.AgvFermiPerAntincendio = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->DB[StructPlcPorte.DBOutput][2] & dmDB->bit[0];
		/*
		 if ((!StructPlcPorte.forzature_attive) && (StructPlcPorte.Stato == 1) && (!StructPlcPorte.memoria_apertura_operatore)) {
		 if ((StructPlcPorte.ApriPorta) && (!StructPlcPorte.PortaAperta)) {
		 StructPlcPorte.count_comando++;
		 }
		 else if ((StructPlcPorte.ChiudiPorta) && (!StructPlcPorte.PortaChiusa)) {
		 StructPlcPorte.count_comando++;
		 }
		 else
		 StructPlcPorte.count_comando = 0;
		 if (StructPlcPorte.count_comando > 60) {
		 dmDB->LogMsg("Resetto segnali porta " + IntToStr(StructPlcPorte.id) + " dopo 30 secondi di segnale alto e nessuna corrispondenza , stato uscite : " + IntToStr(StructPlcPorte.uscitedascrivere));
		 StructPlcPorte.uscitedascrivere = 0;
		 }
		 }
		 */
		if (StructPlcPorte.forzature_attive) {
			if (StructPlcPorte.count_forzature > 30) {
				StructPlcPorte.count_forzature = 0;
				StructPlcPorte.forzature_attive = 0;
			}
			else
				StructPlcPorte.count_forzature++;
		}
		else
			StructPlcPorte.count_forzature = 0;
	}

}

void TDMGestione_Porte::ModificaByteStrutturaPorte(int input, int idporta, int byte, int value)
{
	int i;
	if ((idporta) && (idporta < NUM_PORTE)) {
		if (input) {
			// ClientData.DatiPorte[idporta].ByteInput = value;
		}
		else {
			ClientData.DatiPorte[idporta].forzature_attive = true;
			if (value) {
				ApriChiudiPorta(idporta, APRIPORTA);
			}
			else
				ApriChiudiPorta(idporta, ABBASSASEGNALIPORTA);
		}
	}
}
