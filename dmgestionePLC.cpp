//---------------------------------------------------------------------------


#pragma hdrstop
#include <vcl.h>
#include <IniFiles.hpp>
#include <System.WideStrUtils.hpp>

#include "dmgestionePLC.h"
#include "percorsiprototipo.h"
#include "socket.h"
#include "db.h"
#include "datamodulecomandiagv.h"
#include "plcphoenixthread.h"
#include "PLCThread.h"
#include "baiacarico.h"
#include "dmgestione.h"

extern CLIENTSOCKET *cs;
extern char N_carrelli;
extern DATILGV tab;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TTDMgestionePLC *TDMgestionePLC;
int lunghezza_campi[NCAMPI] = {20, 1, 1,1,60, 60, 60,60,10,25,15};
int indice_inizio_campi[NCAMPI] = {0, 20, 21,22, 30, 90, 150,210,270,280,305};
int tipologia_campi[NCAMPI] = {6, 1, 1,1, 6,6,6,6,6,6,6}; // 1 byte ,2 int/word,3 doppia word,4 float,5 double ,6 char,7 string con primi 2 byte per lunghezza max e lunghezza stringa,8 DDword 8 byte intero , long int
//elsenco campi :
/*
SSCC (4 byte)
Programma fasciatura (1 byte)
Layout Etichetta (1 byte)
Etichetta Codice Prodotto (60 char)
Etichetta EAN Prodotto (60 char)
Etichetta Descrizione (60 char)
Etichetta EAN Imballo (60 char)
codice articolo (10 char)
lotto (25 char)
etichetta EX Date (15 char)
*/
//---------------------------------------------------------------------------
__fastcall TTDMgestionePLC::TTDMgestionePLC(TComponent* Owner)
    : TDataModule(Owner)
{
	init = true;
    timerosso = 0;
}

//---------------------------------------------------------------------------
void __fastcall TTDMgestionePLC::DataModuleCreate(TObject *Sender)
{
    TimerEventi->Enabled = true;
    TimerPlc->Enabled = true;
   	TimerAllarmi->Enabled = true;

    //TimerPhoenix->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TTDMgestionePLC::TimerPlcTimer(TObject *Sender)
{
	int i, j, posagv, destagv, pal_dx_agv, pal_sx_agv, livellobatagv, misincorso, statoagv, statoattualeagv, in_marcia, muscite, mdascrivere;
	AnsiString stringa, stringa_allarme, stringa_allarme_dascrivere,codice_sscc;
	char code[24];

    // imposto datyi in memoria da Plc
	if (MainForm->count_inizio < 10)
		return;

	TimerPlc->Enabled = false;

   
	//------------------------------------------------------------
	// PLCINCAS PlcThread 0
    //-------------------------------------------------------------

	MainForm->PlcIncas.PlcConnesso = PLCThread[0]->connected;

	if ((PLCThread[0]->connected) || (MainForm->Simula))
	{
		if ((PLCThread[0]->connected) && (!MainForm->Simula))
		{
			MainForm->PlcIncas.StatoInput = PLCThread[0]->DB[MainForm->PlcIncas.dbInput][MainForm->PlcIncas.dwByteInput];

			MainForm->PlcIncas.uscite = PLCThread[0]->DB[MainForm->PlcIncas.dbuscite][MainForm->PlcIncas.dwuscite];
			if (MainForm->count_inizio < 20)
			{
				MainForm->PlcIncas.uscitedascrivere = MainForm->PlcIncas.uscite;
			}
		}
        else if (MainForm->Simula) 
        {
			MainForm->PlcIncas.uscite = MainForm->PlcIncas.uscitedascrivere;
		}

        //IN
		MainForm->PlcIncas.richiesta_impegno_incrocio = MainForm->PlcIncas.StatoInput & MainForm->bit[MainForm->PlcIncas.bit_richiesta_impegno_incrocio];

		//OUT
		MainForm->PlcIncas.consenso_impegno_incrocio = MainForm->PlcIncas.uscitedascrivere & MainForm->bit[MainForm->PlcIncas.bit_consenso_impegno_incrocio];
        MainForm->PlcIncas.impegno_inc_rosso = MainForm->PlcIncas.uscitedascrivere & MainForm->bit[MainForm->PlcIncas.bit_impegno_inc_rosso];
        MainForm->PlcIncas.impegno_inc_giallo = MainForm->PlcIncas.uscitedascrivere & MainForm->bit[MainForm->PlcIncas.bit_impegno_inc_giallo];
        MainForm->PlcIncas.impegno_inc_verde = MainForm->PlcIncas.uscitedascrivere & MainForm->bit[MainForm->PlcIncas.bit_impegno_inc_verde];
        

		// gestione watchdog
        if ((PLCThread[0]->connected) && (!MainForm->Simula))
		{
			PLCThread[0]->watchdog_plc = (PLCThread[0]->DB[PLCThread[0]->dbwatchdog_plc][PLCThread[0]->wordwatchdog_plc] & MainForm->bit[1]) ? 1 : 0;
            PLCThread[0]->watchdog_pc = (PLCThread[0]->DB[PLCThread[0]->dbwatchdog_pc][PLCThread[0]->wordwatchdog_pc] & MainForm->bit[4]) ? 1 : 0;

            if ((PLCThread[0]->watchdog_pc != PLCThread[0]->watchdog_plc) && (PLCThread[0]->connected))
            {
                if (PLCThread[0]->watchdog_plc) 
                    MainForm->PlcIncas.uscitedascrivere |= MainForm->bit[4];  //bit 2 scritto da P.
                else
                    MainForm->PlcIncas.uscitedascrivere &= MainForm->bitAnd[4];  //bit 2 scritto da P.

				if (PLCThread[0]->alarm_watchdog) 
                {
                    MainForm->MessageBar1->DelMsgNo(PLCThread[0]->num_alarm_watchdog);
					PLCThread[0]->alarm_watchdog = 0;
				}
				PLCThread[0]->count_alarm_watchdog = 0;
            }
			else 
            {
				PLCThread[0]->count_alarm_watchdog++;
				if (PLCThread[0]->count_alarm_watchdog >= 20) {
					if (!PLCThread[0]->alarm_watchdog) {
                        MainForm->MessageBar1->AddMsgNo(PLCThread[0]->num_alarm_watchdog);
                        dmDB->ActiveAlarm(MainForm->MessageBar1->Messages->Strings[PLCThread[0]->num_alarm_watchdog]);
						PLCThread[0]->alarm_watchdog = 1;
                    }
					PLCThread[0]->count_alarm_watchdog = 0;
                }
			}
			if (MainForm->PlcIncas.uscitedascrivere != MainForm->PlcIncas.uscite) 
            {
				PLCThread[0]->WriteByte(MainForm->PlcIncas.dbuscite, MainForm->PlcIncas.dwuscite, MainForm->PlcIncas.uscitedascrivere, MainForm->Simula);
                //Il messaggio vamesso SENZA WATCHDOG!!
                muscite = MainForm->PlcIncas.uscite &= MainForm->bitAnd[4];
                mdascrivere = MainForm->PlcIncas.uscitedascrivere &= MainForm->bitAnd[4];
                if (mdascrivere != muscite)
				    dmDB->LogMsg("Modificato uscite su PLC valore prec : " + IntToStr(MainForm->PlcIncas.uscite) + " , nuovo : " + IntToStr(MainForm->PlcIncas.uscitedascrivere));
			}
		}

        // dopo 30 secondi disattiva la forzatura
        if (MainForm->PlcIncas.forzatura_attiva) 
        {
            if (MainForm->PlcIncas.count_forzatura > 30) {
                MainForm->PlcIncas.forzatura_attiva = 0;
            }
            else
                MainForm->PlcIncas.count_forzatura++;
        }
        else
        {
            MainForm->PlcIncas.count_forzatura = 0;
        }
	}


    //------------------------------------------------------------
    // FASCIATORE PlcThread BOH 
    //-------------------------------------------------------------
    /*
	MainForm->PlcFascia.PlcConnesso = PLCThread[0]->connected;
        
	if ((PLCThread[0]->connected) || (MainForm->Simula))
	{
		if ((PLCThread[0]->connected) && (!MainForm->Simula))
		{
			MainForm->PlcFascia.StatoInput[0] = PLCThread[0]->DB[MainForm->PlcFascia.dbInput][MainForm->PlcFascia.dwByteInput] ;
			MainForm->PlcFascia.StatoInput[1] = PLCThread[0]->DB[MainForm->PlcFascia.dbInput][MainForm->PlcFascia.dwByteInput+1] ;
			MainForm->PlcFascia.StatoInput[2] = PLCThread[0]->DB[MainForm->PlcFascia.dbInput][MainForm->PlcFascia.dwByteInput+2] ; //

			MainForm->PlcFascia.uscite = PLCThread[0]->DB[MainForm->PlcFascia.dbuscite][MainForm->PlcFascia.dwuscite];
			if (MainForm->count_inizio < 20)
			{
				MainForm->PlcFascia.uscitedascrivere = MainForm->PlcFascia.uscite;
				//MainForm->PlcFascia.usciteetichettadascrivere = MainForm->PlcFascia.usciteetichetta;
			}
		}
        else if (MainForm->Simula) 
        {
			MainForm->PlcFascia.uscite = MainForm->PlcFascia.uscitedascrivere;
			//MainForm->PlcFascia.usciteetichetta = MainForm->PlcFascia.usciteetichettadascrivere;
		}

		MainForm->PlcFascia.ChiamataDeposito = MainForm->PlcFascia.StatoInput[MainForm->PlcFascia.addr_ChiamataDeposito] & MainForm->bit[MainForm->PlcFascia.bit_ChiamataDeposito];
		MainForm->PlcFascia.OKingresso = MainForm->PlcFascia.StatoInput[MainForm->PlcFascia.addr_OKingresso] & MainForm->bit[MainForm->PlcFascia.bit_OKingresso];
		MainForm->PlcFascia.FasciaNoALARM = MainForm->PlcFascia.StatoInput[MainForm->PlcFascia.addr_FasciaNoALARM] & MainForm->bit[MainForm->PlcFascia.bit_FasciaNoALARM];
		MainForm->PlcFascia.PalletAvvolto = MainForm->PlcFascia.StatoInput[MainForm->PlcFascia.addr_PalletAvvolto] & MainForm->bit[MainForm->PlcFascia.bit_PalletAvvolto];


        //se va via il segnale di pallet avvolto toglie il tempo di attesa
        if ((int(MainForm->PlcFascia.InizioAttesa) != 0) && (!MainForm->PlcFascia.PalletAvvolto)) 
        {
            MainForm->PlcFascia.InizioAttesa = 0;    
        }

		MainForm->PlcFascia.ok_missione_scarico_avvolgitore = 0 ;
        
		//temporizzo il segnali di fuori ingombro per avere deposito libero corretto perchè appena tolgo l'ingombro hjo ancora ok al deposito
		//if ((MainForm->PlcFascia.IngombroDepositoAgv )&&(!(MainForm->PlcFascia.uscitedascrivere & MainForm->bit[MainForm->PlcFascia.bit_IngombroDepositoAgv])))
		//	MainForm->PlcFascia.count_ingombro++ ;
		//else
		//	MainForm->PlcFascia.count_ingombro = 0 ;
			//segnali pos deposito
		//if ((!MainForm->PlcFascia.count_ingombro)||(MainForm->PlcFascia.count_ingombro > 3))
		//	MainForm->PlcFascia.IngombroDepositoAgv = MainForm->PlcFascia.uscitedascrivere & MainForm->bit[MainForm->PlcFascia.bit_IngombroDepositoAgv] ;

		MainForm->PlcFascia.RichiestaIngresso = MainForm->PlcFascia.uscitedascrivere & MainForm->bit[MainForm->PlcFascia.bit_RichiestaIngresso];
		MainForm->PlcFascia.pallet_depositato = MainForm->PlcFascia.uscitedascrivere & MainForm->bit[MainForm->PlcFascia.bit_pallet_depositato];
		MainForm->PlcFascia.pallet_prelevato = MainForm->PlcFascia.uscitedascrivere & MainForm->bit[MainForm->PlcFascia.bit_pallet_prelevato];

        //Reset del Pallet PRELEVATO
        if ((MainForm->PlcFascia.pallet_prelevato)) 
        {
            if (MainForm->PlcFascia.count_pallet_prelevato > 4) 
            {
                MainForm->PlcFascia.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcFascia.bit_pallet_prelevato];
                dmDB->LogMsg("Resetto segnale Pallet prelevato su Fasciatore 3 secondi dopo PalletAvvolto basso");
            }
            else
                MainForm->PlcFascia.count_pallet_prelevato++;
        }
        else
            MainForm->PlcFascia.count_pallet_prelevato = 0;

        
        //Reset del Pallet DEPOSITATO
        if ((MainForm->PlcFascia.pallet_depositato) && (!MainForm->PlcFascia.ChiamataDeposito)) 
        {
            if (MainForm->PlcFascia.count_pallet_depositato > 3) 
            {
                MainForm->PlcFascia.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcFascia.bit_pallet_depositato];
                dmDB->LogMsg("Resetto segnale Pallet depositato su Fasciatore 3 secondi dopo ChiamataDeposito basso");
            }
            else
                MainForm->PlcFascia.count_pallet_depositato++;
        }
        else
            MainForm->PlcFascia.count_pallet_depositato = 0;

        
		// gestione watchdog
		if ((PLCThread[0]->connected) && (!MainForm->Simula))
		{
            PLCThread[0]->watchdog_plc = (PLCThread[0]->DB[PLCThread[0]->dbwatchdog_plc][PLCThread[0]->wordwatchdog_plc] * 256) +
										   (PLCThread[0]->DB[PLCThread[0]->dbwatchdog_plc][PLCThread[0]->wordwatchdog_plc+1]);
            PLCThread[0]->watchdog_pc = (PLCThread[0]->DB[PLCThread[0]->dbwatchdog_pc][PLCThread[0]->wordwatchdog_pc] * 256) +
										  (PLCThread[0]->DB[PLCThread[0]->dbwatchdog_pc][PLCThread[0]->wordwatchdog_pc+1]);

            if ((PLCThread[0]->watchdog_pc != PLCThread[0]->watchdog_plc) && (PLCThread[0]->connected))
            {
				PLCThread[0]->WriteWord(PLCThread[0]->dbwatchdog_pc, PLCThread[0]->wordwatchdog_pc, PLCThread[0]->watchdog_plc+1, MainForm->Simula);   //LO INCREMENTA
				if (PLCThread[0]->alarm_watchdog) {
					MainForm->MessageBar1->DelMsgNo(PLCThread[0]->num_alarm_watchdog);
					PLCThread[0]->alarm_watchdog = 0;
                }
				PLCThread[0]->count_alarm_watchdog = 0;
            }
            else {
				PLCThread[0]->count_alarm_watchdog++;
                if (PLCThread[0]->count_alarm_watchdog >= 20) {
                    if (!PLCThread[0]->alarm_watchdog) {
                        MainForm->MessageBar1->AddMsgNo(PLCThread[0]->num_alarm_watchdog);
						PLCThread[0]->alarm_watchdog = 1;
                    }
					PLCThread[0]->count_alarm_watchdog = 0;
                }
			}

			if (MainForm->PlcFascia.uscitedascrivere != MainForm->PlcFascia.uscite) {
				PLCThread[0]->WriteByte(MainForm->PlcFascia.dbuscite, MainForm->PlcFascia.dwuscite, MainForm->PlcFascia.uscitedascrivere, MainForm->Simula);
				dmDB->LogMsg("Modificato uscite su deposito avvolgitore valore prec : " + IntToStr(MainForm->PlcFascia.uscite) + " , nuovo : " + IntToStr(MainForm->PlcFascia.uscitedascrivere));
			}
		}

        // dopo 30 secondi disattiva la forzatura
        if (MainForm->PlcFascia.forzatura_attiva) {
            if (MainForm->PlcFascia.count_forzatura > 30) {
                MainForm->PlcFascia.forzatura_attiva = 0;
            }
            else
                MainForm->PlcFascia.count_forzatura++;
        }
        else
            MainForm->PlcFascia.count_forzatura = 0;
	} */
    
    //----------------------------        
	TimerPlc->Enabled = true;

}

//---------------------------------------------------------------------------
/*
void TTDMgestionePLC::GetArrivalTime(int nLinea, int input, int bit)
{
	int i, j, news;
    AnsiString strsql;
    TADOQuery *ADOQuery;

    //nuovo stato
    news = MainForm->PlcRobot.StatoInput[input] & MainForm->bit[bit];
    
    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = dmDB->ADOConnection1;

    try
    {
        // salva o cancella dal DB l'ordine di arrivo
        if ((MainForm->PlcRobot.presenza_pallet_linea[nLinea] != news) && (news != 0) && (dmDB->ADOConnection1->Connected))   // fronte di salita
        {
            strsql.printf("UPDATE Robots SET timing = GetDate(), stato = 1 WHERE IDlinea = %d", nLinea);
            ADOQuery->SQL->Text = strsql;
            ADOQuery->ExecSQL();
        }
        else if ((MainForm->PlcRobot.presenza_pallet_linea[nLinea] != news) && (news == 0) && (dmDB->ADOConnection1->Connected))   // fronte di discesa
        {
            strsql.printf("UPDATE Robots SET timing = GetDate(), stato = 0 WHERE IDlinea = %d", nLinea);
            ADOQuery->SQL->Text = strsql;
            ADOQuery->ExecSQL();
        }
    }
    catch  (...)
    {
    }

    //cmq aggiorna
    MainForm->PlcRobot.presenza_pallet_linea[nLinea] = news;

    delete ADOQuery;
}
*/
//---------------------------------------------------------------------------
/*
void TTDMgestionePLC::InitStatoLinee()
{
	int i, j, news;
    AnsiString strsql;
    TADOQuery *ADOQuery;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = dmDB->ADOConnection1;

    for (j=1 ; j<=NLINEE_PREL ; j++)
    {
        MainForm->PlcRobot.presenza_pallet_linea[j] = 0;
    }

    try
    {
        strsql = "UPDATE Robots SET stato = 0";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
        ADOQuery->Close();
    }
    catch  (...)
    {
    }

    delete ADOQuery;
}

*/
//---------------------------------------------------------------------------
void __fastcall TTDMgestionePLC::TimerEventiTimer(TObject *Sender)
{
	int i, j, agv, k, anomalia_generatori;
	TMissione m;
	Evento ev;

	// gestisco le missioni degli agv e tutti gli eventi
	if (MainForm->count_inizio < 15)
        return;

	TimerEventi->Enabled = false;
	agv = 1;

    if ((!MainForm->alarmmissioni) && (!MainForm->AbilitaMissioni)) 
    {
		MainForm->MessageBar1->AddMsgNo(200);
        dmDB->ActiveAlarm(MainForm->MessageBar1->Messages->Strings[200]);
		MainForm->alarmmissioni = 1;
	}
	else if ((MainForm->alarmmissioni) && (MainForm->AbilitaMissioni))
	{
        MainForm->MessageBar1->DelMsgNo(200);
        MainForm->alarmmissioni = 0;
    }

    //ControlloIngressoFasciatore();
	ControlloSemaforo();
    ControlloIncrocioAGV1();
    
	TimerEventi->Enabled = true;
}

//---------------------------------------------------------------------------
// FASCIATORE
//

void TTDMgestionePLC::ControlloIngressoFasciatore()
{
    /*
    int i, j, res = 0, agv, baia, multidep, newdest, destprec, iningombro = 0;
    Evento ev;
    // gestione iingresso prelievo/deposito avvolgitore

    for (agv = 1; agv <= N_carrelli; agv++) 
    {
		if ((MainForm->DatiAgv[agv].richiestaconsenso) && (!MainForm->DatiAgv[agv].consensodato) && (MainForm->DatiAgv[agv].nodob == MainForm->PlcFascia.posdep)) 
        {
            try 
            {
				if ((MainForm->PlcFascia.ChiamataDeposito) || (MainForm->PlcFascia.PalletAvvolto))
                {
					if (!MainForm->PlcFascia.RichiestaIngresso) 
                    {
						MainForm->PlcFascia.uscitedascrivere |= MainForm->bit[MainForm->PlcFascia.bit_RichiestaIngresso];
						dmDB->LogMsg("Setto richiesta ingresso su fasciatore per Agv " + IntToStr(agv) + " in posizione " + IntToStr(MainForm->DatiAgv[agv].pos) + " con destinazione " + IntToStr(MainForm->DatiAgv[agv].dest));
					}
					if ((MainForm->PlcFascia.RichiestaIngresso) && (MainForm->PlcFascia.OKingresso)) {
						MainForm->DatiAgv[agv].consensodato = MainForm->DatiAgv[agv].nodob;
						MainForm->DatiAgv[agv].richiestaconsenso = 0;
						dmDB->LogMsg("Consenso ingresso per deposito su fasciatore inviato ad Agv " + IntToStr(agv) + " in pos " + IntToStr(MainForm->DatiAgv[agv].pos) + " ,dest " + IntToStr(MainForm->DatiAgv[agv].dest));
						sprintf(ev, "O,R,%d,%d,%d,%d", agv, MainForm->DatiAgv[agv].nodoa, MainForm->DatiAgv[agv].nodob, 1);
						cs->SendEv(ev);
						if (MainForm->DatiAgv[agv].alarmconsenso) 
                        {
							MainForm->MessageBar1->DelMsgNo(MainForm->DatiAgv[agv].num_alarm_consenso);
							MainForm->DatiAgv[agv].alarmconsenso = 0;
						}
					}
					else if ((!MainForm->DatiAgv[agv].alarmconsenso) && (MainForm->PlcFascia.RichiestaIngresso)) 
                    {
						MainForm->MessageBar1->Messages->Strings[MainForm->DatiAgv[agv].num_alarm_consenso] = "Agv in Attesa Ok all'ingresso su Fasciatore ";
						MainForm->MessageBar1->AddMsgNo(MainForm->DatiAgv[agv].num_alarm_consenso);
						MainForm->DatiAgv[agv].alarmconsenso = 1;
					}
				}
				else if (!MainForm->DatiAgv[agv].alarmconsenso) 
                {
					MainForm->MessageBar1->Messages->Strings[MainForm->DatiAgv[agv].num_alarm_consenso] = "Agv in Attesa su Fasciatore libero ";
					MainForm->MessageBar1->AddMsgNo(MainForm->DatiAgv[agv].num_alarm_consenso);
					MainForm->DatiAgv[agv].alarmconsenso = 1;
				}
			}
			catch (...) 
            {
				dmDB->LogMsg("Eccezione controllo str ingresso baia con stringa ricevuta : " + MainForm->DatiAgv[agv].strcons);
			}
		}
        //else if (MainForm->DatiAgv[agv].alarmconsenso) 
        //{
		//	MainForm->MessageBar1->DelMsgNo(MainForm->DatiAgv[agv].num_alarm_consenso);
		//	MainForm->DatiAgv[agv].alarmconsenso = 0;
		//}
		if ((MainForm->DatiAgv[agv].pos == MainForm->PlcFascia.posdep) ||
			(MainForm->DatiAgv[agv].consensodato == MainForm->PlcFascia.posdep))
            iningombro = agv;
	}
    
	if (!MainForm->PlcFascia.forzatura_attiva) 
    {
		if ((iningombro) && (!MainForm->PlcFascia.IngombroDepositoAgv))
        {
			//MainForm->PlcFascia.uscitedascrivere |= MainForm->bit[MainForm->PlcFascia.bit_IngombroDepositoAgv];
			//dmDB->LogMsg("Setto Agv in ingombro deposito su fasciatore per Agv " + IntToStr(agv) + " in posizione " + IntToStr(MainForm->DatiAgv[agv].pos) + " con destinazione " + IntToStr(MainForm->DatiAgv[agv].dest));
            MainForm->PlcFascia.IngombroDepositoAgv = 1;
		}
		else if  ((!iningombro) && (MainForm->PlcFascia.IngombroDepositoAgv)) 
        {
			//MainForm->PlcFascia.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcFascia.bit_IngombroDepositoAgv];
			//dmDB->LogMsg("Resetto Agv in ingombro deposito su fasciatore per con Agv 1 in pos " + IntToStr(MainForm->DatiAgv[1].pos) + " ,dest " + IntToStr(MainForm->DatiAgv[1].dest) );
            MainForm->PlcFascia.IngombroDepositoAgv = 0;
		}
        
		if (!MainForm->PlcFascia.IngombroDepositoAgv) 
        {
			// azzeramento richieste ingresso agv per deposito su fasciatore+
			// azzero quando non ho nessun agv che sta andando verso il deposito e ho almeno un segnale alto
			if ((MainForm->PlcFascia.RichiestaIngresso) && (!AgvInDestinazioneDepositoFasciatore())) 
            {
				MainForm->PlcFascia.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcFascia.bit_RichiestaIngresso];
				dmDB->LogMsg("Resetto Richiesta Ingresso su fasciatore per con Agv 1 in pos " + IntToStr(MainForm->DatiAgv[1].pos) + " ,dest " + IntToStr(MainForm->DatiAgv[1].dest) );
			}
		}
        else 
        {
			if ((!MainForm->DatiAgv[1].manuale)&&(!MainForm->DatiAgv[1].semiautomatico)&&(!MainForm->DatiAgv[1].alarm)
				&&(!MainForm->PlcFascia.OKingresso) && 
				(MainForm->DatiAgv[1].dest == MainForm->PlcFascia.posdep || MainForm->DatiAgv[1].pos == MainForm->PlcFascia.posdep)) 
            {
				if (!MainForm->DatiAgv[1].stand_by) 
                {
					dmDB->LogMsg("Blocco agv in ingresso fasciatore con ok ingresso basso");
					dmComandiAgv->StandByAgv(1, 1);
				}
			}
            else if ((!MainForm->DatiAgv[1].manuale)&&(!MainForm->DatiAgv[1].semiautomatico)
				    && (MainForm->PlcFascia.OKingresso)&&(MainForm->DatiAgv[1].stand_by)) 
            {
				dmDB->LogMsg("Sblocco agv in ingresso fasciatore con ok ingresso alto");
				dmComandiAgv->StandByAgv(1, 0);
			}
		}
	}
	if ((MainForm->DatiAgv[1].consensodato)&&(MainForm->DatiAgv[1].consensodato == MainForm->DatiAgv[1].pos)) 
    {
		dmDB->LogMsg("Resetto consenso dato ad agv in posizione "+IntToStr(MainForm->DatiAgv[1].pos));
		MainForm->DatiAgv[1].consensodato = 0 ;
	}
   */
}

//---------------------------------------------------------------------------

void __fastcall TTDMgestionePLC::TimerSemaforoTimer(TObject *Sender)
{
//    
}

//---------------------------------------------------------------------------

int TTDMgestionePLC::AgvInPosLuceSemaforoRossa()
{
    /*
	int i, res = 0;
	for (i = 1; i <= N_carrelli; i++) 
    {
		if (MainForm->PosizioniLuceRossaSemaforo.Pos(";" + IntToStr(MainForm->DatiAgv[i].pos) + ";")) 
        {
			res = i;
			break;
		}
	}
	return res;  */
	int res = 0;
    
    if (((MainForm->PlcIncas.impegno_inc_giallo) && (!MainForm->PlcIncas.impegno_inc_rosso) && (timerosso<=0)) ||
        ((MainForm->PlcIncas.impegno_inc_giallo) && (MainForm->PlcIncas.impegno_inc_rosso) && (timerosso<=0)) ||
         (MainForm->PosizioniLuceRossaSemaforo.Pos(";" + IntToStr(MainForm->DatiAgv[1].pos) + ";"))) 
    {
       res = 1;  
    }

    if ((MainForm->PlcIncas.impegno_inc_giallo) && (timerosso>0)) 
    {
        timerosso -= 1;   
    }
    return res;
}

//---------------------------------------------------------------------------

int TTDMgestionePLC::AgvInPosLuceSemaforoGialla()
{
	int i, res = 0;
	for (i = 1; i <= N_carrelli; i++) 
    {
		if (MainForm->PosizioniLuceGiallaSemaforo.Pos(";" + IntToStr(MainForm->DatiAgv[i].pos) + ";")) 
        {
			res = i;
			break;
		}
	}
	return res;
}

//---------------------------------------------------------------------------

void TTDMgestionePLC::ControlloSemaforo()
{
    int setta_luce_rossa, setta_luce_gialla;
    try 
    {  
        if (!MainForm->PlcIncas.forzatura_attiva)
        {
            //gestione ROSSO semaforo
            setta_luce_rossa = AgvInPosLuceSemaforoRossa();
            setta_luce_gialla = AgvInPosLuceSemaforoGialla();
            
            if ((!MainForm->PlcIncas.impegno_inc_rosso) && (setta_luce_rossa)) 
            {
                MainForm->PlcIncas.uscitedascrivere |= MainForm->bit[MainForm->PlcIncas.bit_impegno_inc_rosso];
                dmDB->LogMsg("Accendo Luce Rossa con Agv in pos "+IntToStr(MainForm->DatiAgv[1].pos));
            }
            else if ((MainForm->PlcIncas.impegno_inc_rosso) &&(!setta_luce_rossa)) 
            {
		    	MainForm->PlcIncas.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcIncas.bit_impegno_inc_rosso];
                dmDB->LogMsg("Spengo Luce Rossa con Agv in pos "+IntToStr(MainForm->DatiAgv[1].pos));
            }
            
            //gestione GIALLO semaforo
            if ((!MainForm->PlcIncas.impegno_inc_giallo) && (setta_luce_gialla)) 
            {
                MainForm->PlcIncas.uscitedascrivere |= MainForm->bit[MainForm->PlcIncas.bit_impegno_inc_giallo];
                dmDB->LogMsg("Accendo Luce Gialla con Agv in pos "+IntToStr(MainForm->DatiAgv[1].pos));
                timerosso = 2; //parte il timer per accendere il rosso che si accende 3 sec dopo il giallo
            }
            else if ((MainForm->PlcIncas.impegno_inc_giallo) &&(!setta_luce_gialla)) 
            {
		    	MainForm->PlcIncas.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcIncas.bit_impegno_inc_giallo];
                dmDB->LogMsg("Spengo Luce Gialla con Agv in pos "+IntToStr(MainForm->DatiAgv[1].pos));
            }
            
            //gestione VERDE semaforo
            if ((!MainForm->PlcIncas.impegno_inc_verde) && (!setta_luce_rossa) && (!setta_luce_gialla)) 
            {
                MainForm->PlcIncas.uscitedascrivere |= MainForm->bit[MainForm->PlcIncas.bit_impegno_inc_verde];
                dmDB->LogMsg("Accendo Luce Verde con Agv in pos "+IntToStr(MainForm->DatiAgv[1].pos));
            }
            else if ((MainForm->PlcIncas.impegno_inc_verde) && (setta_luce_rossa || setta_luce_gialla)) 
            {
		    	MainForm->PlcIncas.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcIncas.bit_impegno_inc_verde];
                dmDB->LogMsg("Spengo Luce Verde con Agv in pos "+IntToStr(MainForm->DatiAgv[1].pos));
            }
        }
    }
    catch (...) 
    {
    }
}

//---------------------------------------------------------------------------

void __fastcall TTDMgestionePLC::TimerPhoenixTimer(TObject *Sender)
{
	int i, j, posagv, destagv, pal_dx_agv, pal_sx_agv, livellobatagv, misincorso, statoagv, statoattualeagv, in_marcia, val;
    AnsiString stringa, stringa_allarme, stringa_allarme_dascrivere;
    char code[24];
    
    // imposto dati in memoria da Plc
    if (MainForm->count_inizio < 15)
        return;
        
    TimerPhoenix->Enabled = false;
/*
    for (i = 0; i <= (MainForm->num_moduli_phoenix - 1); i++) 
    {
        if (init) 
        {
            if (PLCPhoenixThread[i]->connesso)
                dmDB->LogMsg("Modulo Plc Phoenix " + IntToStr(i) + " Connesso ");
            else
                dmDB->LogMsg("Plc Phoenix " + IntToStr(i) + " Non Connesso");
            id_lampeggio_sync = 1;
        }
        
        if ((MainForm->PlcRobot.PlcConnesso) && (!PLCPhoenixThread[i]->connesso))
            dmDB->LogMsg("Plc Phoenix " + IntToStr(i) + " disconnesso");
        else if ((!init) && (!MainForm->PlcRobot.PlcConnesso) && (PLCPhoenixThread[i]->connesso))
            dmDB->LogMsg("Plc Phoenix " + IntToStr(i) + " riconnesso");
            
        MainForm->PlcRobot.PlcConnesso = PLCPhoenixThread[i]->connesso;
        if ((!init) && (PLCPhoenixThread[i]->error_scrittura != MainForm->PlcRobot.ErroreScrittura)) {
            if (!PLCPhoenixThread[i]->error_scrittura)
                dmDB->LogMsg("Errore in scrittura su modulo Phoenix " + IntToStr(i) + " n° errore : " + IntToStr(PLCPhoenixThread[i]->error_scrittura));
            else
                dmDB->LogMsg("Scrittura tornata ok su modulo Phoenix " + IntToStr(i));
        }
        MainForm->PlcRobot.ErroreScrittura = PLCPhoenixThread[i]->error_scrittura;
        
        if ((PLCPhoenixThread[i]->connesso) || (MainForm->Simula)) 
        {
            if (!MainForm->PlcRobot.forzatura_input) 
            {
				MainForm->PlcRobot.input[1] = PLCPhoenixThread[i]->IngressiPhoenix[1];
				MainForm->PlcRobot.input[2] = PLCPhoenixThread[i]->IngressiPhoenix[3];
				MainForm->PlcRobot.input[3] = PLCPhoenixThread[i]->IngressiPhoenix[5];
				MainForm->PlcRobot.input[4] = PLCPhoenixThread[i]->IngressiPhoenix[7];
			}
            for (j = 1; j < 20; j++) {
                if (MainForm->count_inizio < 20)
                    PLCPhoenixThread[i]->UscitePhoenixDaScrivere[j] = PLCPhoenixThread[i]->UscitePhoenix[j];
                else 
                {
                    if (MainForm->Simula)
                    {
                        PLCPhoenixThread[i]->UscitePhoenix[j] = PLCPhoenixThread[i]->UscitePhoenixDaScrivere[j];
                    }
                    else if (PLCPhoenixThread[i]->UscitePhoenixDaScrivere[j] != PLCPhoenixThread[i]->UscitePhoenix[j]) 
                    {
                        // problema scrittura uscite non corrispondente con la lettura
                    }
                }
			}
			//Ingressi in mem
			MainForm->PlcRobot.presenza_pallet_linea[1] = MainForm->PlcRobot.input[1] & MainForm->bit[0];
			MainForm->PlcRobot.presenza_pallet_linea[2] = MainForm->PlcRobot.input[1] & MainForm->bit[1];
			MainForm->PlcRobot.presenza_pallet_linea[3] = MainForm->PlcRobot.input[1] & MainForm->bit[2];
			MainForm->PlcRobot.presenza_pallet_linea[4] = MainForm->PlcRobot.input[1] & MainForm->bit[3];
			MainForm->PlcRobot.presenza_pallet_linea[5] = MainForm->PlcRobot.input[1] & MainForm->bit[4];
			MainForm->PlcRobot.presenza_pallet_linea[6] = MainForm->PlcRobot.input[1] & MainForm->bit[5];
			MainForm->PlcRobot.consenso_vuoti = MainForm->PlcRobot.input[1] & MainForm->bit[6];
			MainForm->PlcRobot.consenso_falde[1] = MainForm->PlcRobot.input[1] & MainForm->bit[7];

			MainForm->PlcRobot.consenso_falde[2] = MainForm->PlcRobot.input[2] & MainForm->bit[0];
            MainForm->PlcRobot.gate_etichetta = MainForm->PlcRobot.input[2] & MainForm->bit[1];
            MainForm->PlcRobot.etichetta_verificata_non_ok = MainForm->PlcRobot.input[2] & MainForm->bit[2]; //è negato!!
            MainForm->PlcRobot.FMvision_ON = MainForm->PlcRobot.input[2] & MainForm->bit[3];
            MainForm->PlcRobot.FMvision_ALARM = MainForm->PlcRobot.input[2] & MainForm->bit[4];


			MainForm->PlcRobot.output[1] = PLCPhoenixThread[i]->UscitePhoenix[1];
			MainForm->PlcRobot.output[2] = PLCPhoenixThread[i]->UscitePhoenix[3];
			MainForm->PlcRobot.output[3] = PLCPhoenixThread[i]->UscitePhoenix[5];
			MainForm->PlcRobot.output[4] = PLCPhoenixThread[i]->UscitePhoenix[7];
			for( j=1 ; j<=6 ; j++)
				MainForm->PlcRobot.pallet_prelevato_linea[j] = PLCPhoenixThread[i]->UscitePhoenix[MainForm->PlcRobot.addr_pallet_prelevato_linea[j]] & MainForm->bit[MainForm->PlcRobot.bit_pallet_prelevato_linea[j]] ;
			MainForm->PlcRobot.richiesta_ingresso_vuoti = PLCPhoenixThread[i]->UscitePhoenix[MainForm->PlcRobot.addr_richiesta_ingresso_vuoti] & MainForm->bit[MainForm->PlcRobot.bit_richiesta_ingresso_vuoti] ;
			MainForm->PlcRobot.richiesta_ingresso_falde[1] = PLCPhoenixThread[i]->UscitePhoenix[MainForm->PlcRobot.addr_richiesta_ingresso_falde[1]] & MainForm->bit[MainForm->PlcRobot.bit_richiesta_ingresso_falde[1]] ;
			MainForm->PlcRobot.richiesta_ingresso_falde[2] = PLCPhoenixThread[i]->UscitePhoenix[MainForm->PlcRobot.addr_richiesta_ingresso_falde[2]] & MainForm->bit[MainForm->PlcRobot.bit_richiesta_ingresso_falde[2]] ;
			MainForm->PlcRobot.fuori_ingombro_vuoti = PLCPhoenixThread[i]->UscitePhoenix[MainForm->PlcRobot.addr_fuori_ingombro_vuoti] & MainForm->bit[MainForm->PlcRobot.bit_fuori_ingombro_vuoti] ;
			MainForm->PlcRobot.fuori_ingombro_falde[1] = PLCPhoenixThread[i]->UscitePhoenix[MainForm->PlcRobot.addr_fuori_ingombro_falde[1]] & MainForm->bit[MainForm->PlcRobot.bit_fuori_ingombro_falde[1]] ;
			MainForm->PlcRobot.fuori_ingombro_falde[2] = PLCPhoenixThread[i]->UscitePhoenix[MainForm->PlcRobot.addr_fuori_ingombro_falde[2]] & MainForm->bit[MainForm->PlcRobot.bit_fuori_ingombro_falde[2]] ;
			MainForm->PlcRobot.lgv_in_marcia = PLCPhoenixThread[i]->UscitePhoenix[5] & MainForm->bit[0] ;
			MainForm->PlcRobot.lgv_in_allarme = PLCPhoenixThread[i]->UscitePhoenix[5] & MainForm->bit[1] ;
			MainForm->PlcRobot.verifica_etichetta = PLCPhoenixThread[i]->UscitePhoenix[MainForm->PlcRobot.addr_verifica_etichetta] & MainForm->bit[MainForm->PlcRobot.bit_verifica_etichetta] ;

		}

        //interrompe la forzatura
        if ((MainForm->PlcRobot.forzatura_attiva)||(MainForm->PlcRobot.forzatura_input)) 
        {
			if (MainForm->PlcRobot.count_forzatura > 30) 
            {
                MainForm->PlcRobot.forzatura_attiva = 0;
                MainForm->PlcRobot.forzatura_input = 0 ;
            }
            else
                MainForm->PlcRobot.count_forzatura++;
        }
		else 
        {
			//gestione segnali lgv in allarme ed in marcia su FmVision
			if ((MainForm->DatiAgv[1].alarm)&&(!MainForm->PlcRobot.lgv_in_allarme)) 
            {
				PLCPhoenixThread[0]->ScriviPhoenix(1,5,true) ;
				dmDB->LogMsg("Segnalo agv in allarme su quadro FM Vision" );
			}else if ((!MainForm->DatiAgv[1].alarm)&&(MainForm->PlcRobot.lgv_in_allarme)) 
            {
				PLCPhoenixThread[0]->ScriviPhoenix(1,5,false) ;
				dmDB->LogMsg("Resetto segnale agv in allarme su quadro FM Vision" );
			}
			if ((MainForm->DatiAgv[1].pos > 0 )&&(!MainForm->DatiAgv[1].manuale )&&(!MainForm->DatiAgv[1].bitbattbassa )
				&&(!MainForm->DatiAgv[1].alarm )&&(!MainForm->DatiAgv[1].semiautomatico )&&(MainForm->AbilitaMissioni)) 
            {
				if (!MainForm->PlcRobot.lgv_in_marcia) 
                {
					PLCPhoenixThread[0]->ScriviPhoenix(0,5,true);
					dmDB->LogMsg("Segnalo agv in marcia su quadro FM Vision con missioni abilitate ed agv in automatico" );
				}
			}
            else if (MainForm->PlcRobot.lgv_in_marcia) 
            {
				PLCPhoenixThread[0]->ScriviPhoenix(0,5,false);
				dmDB->LogMsg("Resetto segnale agv in marcia su quadro FM Vision , Missioni Abilitate : "+IntToStr(MainForm->AbilitaMissioni));
			}

			for (j=1; j<=NLINEE_PREL; j++) 
            {
				if ((MainForm->PlcRobot.pallet_prelevato_linea[j]) && (!MainForm->PlcRobot.presenza_pallet_linea[j])) 
                {
					if (MainForm->PlcRobot.count_pallet_prelevato_linea[j] > 3) 
                    {
						PLCPhoenixThread[0]->ScriviPhoenix( MainForm->PlcRobot.bit_pallet_prelevato_linea[j],MainForm->PlcRobot.addr_pallet_prelevato_linea[j], false);
						dmDB->LogMsg("Resetto segnale pallet prelevato su linea "+IntToStr(j)+" 3 secondi dopo presenza pallet bassa");

					}
                    else
						MainForm->PlcRobot.count_pallet_prelevato_linea[j]++;
				}
                else
					MainForm->PlcRobot.count_pallet_prelevato_linea[j] = 0;
			}
			MainForm->PlcRobot.count_forzatura = 0;
		}
        //
    }

	init = false;
	sync_lamp = !sync_lamp;

	if (id_lampeggio_sync <= 4)
		id_lampeggio_sync++;
	else
		id_lampeggio_sync = 1;
*/        
	TimerPhoenix->Enabled = true;
}
//---------------------------------------------------------------------------

int TTDMgestionePLC::AgvInDestinazionePrelievoFasciatore() {
    int i, res = 0;

/*    for (i = 1; i <= N_carrelli; i++) {
		if ((MainForm->DatiAgv[i].pos > 0) && ((MainForm->DatiAgv[i].consensodato == MainForm->DatiPlc.posprel[1]) || (MainForm->DatiAgv[i].consensodato == MainForm->DatiPlc.posprel[2]) || (MainForm->DatiAgv[i].consensodato == MainForm->DatiPlc.posprel[3]) ||
            (MainForm->DatiAgv[i].consensodato == MainForm->DatiPlc.posprel[4]) || ((MainForm->DatiAgv[i].richiestaconsenso) && ((MainForm->DatiAgv[i].nodob == MainForm->DatiPlc.posprel[1]) || (MainForm->DatiAgv[i].nodob == MainForm->DatiPlc.posprel[2]) || (MainForm->DatiAgv[i].nodob == MainForm->DatiPlc.posprel[3]) ||
            (MainForm->DatiAgv[i].nodob == MainForm->DatiPlc.posprel[4])))) && ((MainForm->DatiAgv[i].dest == MainForm->DatiPlc.posprel[1]) || (MainForm->DatiAgv[i].dest == MainForm->DatiPlc.posprel[2]) || (MainForm->DatiAgv[i].dest == MainForm->DatiPlc.posprel[3]) ||
            (MainForm->DatiAgv[i].dest == MainForm->DatiPlc.posprel[4]))) {
            res = i;
        }
	}    */
    return res;
}

int TTDMgestionePLC::AgvInDestinazioneDepositoFasciatore() 
{
    int i, res = 0;

    for (i = 1; i <= N_carrelli; i++) 
    {
        if ((MainForm->DatiAgv[i].pos > 0) && 
           ((MainForm->DatiAgv[i].consensodato == MainForm->PlcFascia.posdep) ||
			((MainForm->DatiAgv[i].richiestaconsenso) && ((MainForm->DatiAgv[i].nodob == MainForm->PlcFascia.posdep))))
			&& ((MainForm->DatiAgv[i].dest == MainForm->PlcFascia.posdep) )) 
        {
            res = i;
        }
    }
	return res;
}



int TTDMgestionePLC::AgvInDestinazioneDepositoVuoti() 
{
    int i, res = 0;

    for (i = 1; i <= N_carrelli; i++) 
    {
        if ((MainForm->DatiAgv[i].pos > 0) && 
           ((MainForm->DatiAgv[i].consensodato == MainForm->PlcRobot.posvuoti) ||
			((MainForm->DatiAgv[i].richiestaconsenso) && ((MainForm->DatiAgv[i].nodob == MainForm->PlcRobot.posvuoti))))
			&& ((MainForm->DatiAgv[i].dest == MainForm->PlcRobot.posvuoti) )) 
        {
            res = i;
        }
    }
	return res;
}


int TTDMgestionePLC::AgvInDestinazioneDepositoMonta() 
{
    int i, res = 0;
/*
    for (i = 1; i <= N_carrelli; i++) 
    {
        if ((MainForm->DatiAgv[i].pos > 0) && 
           ((MainForm->DatiAgv[i].consensodato == MainForm->PlcMonta.posprel) ||
			((MainForm->DatiAgv[i].richiestaconsenso) && ((MainForm->DatiAgv[i].nodob == MainForm->PlcMonta.posprel))))
			&& ((MainForm->DatiAgv[i].dest == MainForm->PlcMonta.posprel) )) 
        {
            res = i;
        }
    }
    */
	return res;
}

//---------------------------------------------------------------------------
// Gestisce l'OK all'ingombro AGV1
//---------------------------------------------------------------------------

int TTDMgestionePLC::PosInIngombroAGV1(int pos)
{
	int res = 0;
    AnsiString sNodiIngombro = dmDB->TabParametri[105]["STRINGVALUE"];       //sono salvati su DB da un operatore Proxaut
    
    if (sNodiIngombro.Pos(";" + IntToStr(pos) + ";")) 
    {
        res = pos;
    }
	return res;
}          


void TTDMgestionePLC::ControlloIncrocioAGV1()
{
    int i, res = 0, agv;
    Evento ev;
	int agv_ingombro = 0;
    
    for (agv = 1; agv <= N_carrelli; agv++) 
    { 
        if ((MainForm->DatiAgv[agv].richiestaconsenso) && (!MainForm->DatiAgv[agv].consensodato) &&
            (MainForm->DatiAgv[agv].strcons == "ingombro")) 
            //(!MainForm->PlcIncas.consenso_impegno_incrocio) && (!MainForm->PlcIncas.richiesta_impegno_incrocio))                      
        {
            try 
            {
                if (!MainForm->PlcIncas.consenso_impegno_incrocio)   //vedere a chi dare priorità in caso di arrivo in contemporanea,priorita Lgv
			    //if ((!MainForm->PlcIncas.consenso_impegno_incrocio)&&(!MainForm->PlcIncas.richiesta_impegno_incrocio))   //priorità carrellino
                {
                    MainForm->DatiAgv[agv].consensodato = MainForm->DatiAgv[agv].nodob;
                    MainForm->DatiAgv[agv].richiestaconsenso = 0;
                    dmDB->LogMsg("Consenso ingresso nell'area di interferenza AGV1 inviato ad Agv " + IntToStr(agv) + " in pos " + IntToStr(MainForm->DatiAgv[agv].pos) + " ,dest " + IntToStr(MainForm->DatiAgv[agv].dest));
                    sprintf(ev, "O,R,%d,%d,%d,%d", agv, MainForm->DatiAgv[agv].nodoa, MainForm->DatiAgv[agv].nodob, 1);
                    cs->SendEv(ev);
                    dmDB->AggiornaStatoAgv(agv);
                }
            }
            catch (...) {
                dmDB->LogMsg("Eccezione controllo str ingresso zona interferenza ramo nodoa-nodob : " + IntToStr(MainForm->DatiAgv[agv].nodoa) + "-" + IntToStr(MainForm->DatiAgv[agv].nodob));
            }
        }

        if ((PosInIngombroAGV1( MainForm->DatiAgv[agv].pos )) || 
            (PosInIngombroAGV1( MainForm->DatiAgv[agv].consensodato ))) 
        {
            // se ho dato il consenso e sono una posizione prima oppure sono in pos prelievo/deposito setto ingombro
            agv_ingombro = agv;
        }
    
        if ((MainForm->DatiAgv[agv].consensodato) && (MainForm->DatiAgv[agv].consensodato == MainForm->DatiAgv[agv].pos)) 
        {
            dmDB->LogMsg("Resetto consensodato con Agv " + IntToStr(agv) + " in posizione " + IntToStr(MainForm->DatiAgv[agv].pos));
            MainForm->DatiAgv[agv].consensodato = 0;
        }
    
    }
    
    // controllo consenso inviato ma richiesta bassa
	if (!MainForm->PlcIncas.forzatura_attiva) 
    {
        if ((!agv_ingombro) && (MainForm->PlcIncas.richiesta_impegno_incrocio) && (!MainForm->PlcIncas.consenso_impegno_incrocio)) 
        {
            dmDB->LogMsg("Setto OK ad incrocio a AGV1 con il mio Agv in pos " + IntToStr(MainForm->DatiAgv[1].pos) + " , dest " + IntToStr(MainForm->DatiAgv[1].dest) + " , consenso " + IntToStr(MainForm->DatiAgv[1].consensodato));
            MainForm->PlcIncas.uscitedascrivere |= MainForm->bit[MainForm->PlcIncas.bit_consenso_impegno_incrocio];
        }
        else if ((!MainForm->PlcIncas.richiesta_impegno_incrocio) && (MainForm->PlcIncas.consenso_impegno_incrocio)) 
        {
            dmDB->LogMsg("REsetto OK ad incrocio a AGV1 con il mio Agv in pos " + IntToStr(MainForm->DatiAgv[1].pos) + " , dest " + IntToStr(MainForm->DatiAgv[1].dest));
            MainForm->PlcIncas.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcIncas.bit_consenso_impegno_incrocio];
        }
    }
}





//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TTDMgestionePLC::ScriviDatiUdcSuPlc(int indice_inizio_dati, TDatiSSCCPlc DatiDaScrivere)
{
AnsiString stringa_hex, stringa;
int i;
unsigned char dati_udc[STRUTTURA_DATI_UDC_PLC];
unsigned char dbword[4] ;
unsigned char byte ;
/*
0 SSCC (4 byte)
1 Programma fasciatura (1 byte)
2 Layout Etichetta (1 byte)
3 Etichetta Codice Prodotto (60 char)
4 Etichetta EAN Prodotto (60 char)
5 Etichetta Descrizione (60 char)
6 Etichetta EAN Imballo (60 char)
7 codice articolo (10 char)
8 lotto (25 char)
9 etichetta EX Date (15 char)
*/
	for(i=0;i<STRUTTURA_DATI_UDC_PLC;i++) //modificare la struttura di default telaio vuoto
		dati_udc[i] = 0;

    if (DatiDaScrivere.SSCC != "" && DatiDaScrivere.SSCC != "0")  //se non c'è l'SSCC lascia TUTTI 0 - per il controllo del plc
    {
		memcpy( dati_udc+indice_inizio_campi[0],DatiDaScrivere.SSCC.c_str(),DatiDaScrivere.SSCC.Length()) ; //20 char sscc
		//così non mette spazi dopo l'SSCC
	   /*	if (DatiDaScrivere.SSCC.Length() <20)
			DatiDaScrivere.SSCC += AnsiString::StringOfChar(' ',20-DatiDaScrivere.SSCC.Length());
		memcpy( dati_udc+indice_inizio_campi[0],DatiDaScrivere.SSCC.c_str(),lunghezza_campi[0]); //1 byte prg fasciatura */
    }

	dati_udc[indice_inizio_campi[1]] = DatiDaScrivere.prgfasciatura;
	dati_udc[indice_inizio_campi[2]] = DatiDaScrivere.layoutetichetta;
	dati_udc[indice_inizio_campi[3]] = DatiDaScrivere.numscatole;

    stringa = DatiDaScrivere.etichetta_prodCode.Trim();
	memcpy( dati_udc+indice_inizio_campi[4],stringa.c_str(), stringa.Length()); //lunghezza_campi[4]) ; //
    
    stringa = DatiDaScrivere.etichetta_prodEAN.Trim();
	memcpy( dati_udc+indice_inizio_campi[5],stringa.c_str(), stringa.Length()); //lunghezza_campi[5]) ; //
    
	memcpy( dati_udc+indice_inizio_campi[6],DatiDaScrivere.etichetta_descrizione.c_str(),lunghezza_campi[6]) ; //   questo NON trimmato
    
    stringa = DatiDaScrivere.etichetta_imballoEAN.Trim();
	memcpy( dati_udc+indice_inizio_campi[7],stringa.c_str(), stringa.Length()); //lunghezza_campi[7]) ; //
    
    stringa = DatiDaScrivere.codArticolo.Trim();
	memcpy( dati_udc+indice_inizio_campi[8],stringa.c_str(), stringa.Length()); //lunghezza_campi[8]) ; //
    
    stringa = DatiDaScrivere.lotto.Trim();
	memcpy( dati_udc+indice_inizio_campi[9],stringa.c_str(), stringa.Length()); //lunghezza_campi[9]) ; //
    
	memcpy( dati_udc+indice_inizio_campi[10],DatiDaScrivere.etichetta_expDate.c_str(),lunghezza_campi[10]) ; //
    
	PLCThread[0]->WriteBuffer(MainForm->PlcFascia.dbuscite, indice_inizio_dati,STRUTTURA_DATI_UDC_PLC,dati_udc,MainForm->Simula);

}

//---------------------------------------------------------------------------
void TTDMgestionePLC::ReturnDatiSSCCDaPlc(int indice_inizio_dati, TDatiSSCCPlc &DatiScrittisuPlc)
{
/*
0 SSCC (20 byte)
1 Programma fasciatura (1 byte)
2 Layout Etichetta (1 byte)
3 Etichetta Codice Prodotto (60 char)
4 Etichetta EAN Prodotto (60 char)
5 Etichetta Descrizione (60 char)
6 Etichetta EAN Imballo (60 char)
7 codice articolo (10 char)
8 lotto (25 char)
9 etichetta EX Date (15 char)
*/
unsigned char dati_udc[STRUTTURA_DATI_UDC_PLC];
int i, j ;
	j = 0 ;
	for(i = indice_inizio_dati; i < indice_inizio_dati+STRUTTURA_DATI_UDC_PLC; i++) {
		dati_udc[j] = PLCThread[0]->DB[MainForm->PlcFascia.dbuscite][i];
		j++ ;
	}
	DatiScrittisuPlc.SSCC = dmDB->DaBufferAStringa(dati_udc+indice_inizio_campi[0],lunghezza_campi[0]);
	DatiScrittisuPlc.prgfasciatura = dmDB->DaBufferAByte(dati_udc+indice_inizio_campi[1]);
	DatiScrittisuPlc.layoutetichetta = dmDB->DaBufferAByte(dati_udc+indice_inizio_campi[2]);
	DatiScrittisuPlc.numscatole = dmDB->DaBufferAByte(dati_udc+indice_inizio_campi[3]);
	DatiScrittisuPlc.etichetta_prodCode = dmDB->DaBufferAStringa(dati_udc+indice_inizio_campi[4],lunghezza_campi[4]);
	DatiScrittisuPlc.etichetta_prodEAN = dmDB->DaBufferAStringa(dati_udc+indice_inizio_campi[5],lunghezza_campi[5]);
	DatiScrittisuPlc.etichetta_descrizione = dmDB->DaBufferAStringa(dati_udc+indice_inizio_campi[6],lunghezza_campi[6]);
	DatiScrittisuPlc.etichetta_imballoEAN = dmDB->DaBufferAStringa(dati_udc+indice_inizio_campi[7],lunghezza_campi[7]);
	DatiScrittisuPlc.codArticolo = dmDB->DaBufferAStringa(dati_udc+indice_inizio_campi[8],lunghezza_campi[8]);
	DatiScrittisuPlc.lotto = dmDB->DaBufferAStringa(dati_udc+indice_inizio_campi[9],lunghezza_campi[9]);
	DatiScrittisuPlc.etichetta_expDate = dmDB->DaBufferAStringa(dati_udc+indice_inizio_campi[10],lunghezza_campi[10]);

}

//----------------------------------------------------------------------------------
void TTDMgestionePLC::ScriviSSCCSuPlcNavetta( AnsiString SSCC )
{
AnsiString stringa_hex, SSCCdascrivere;
int i;
unsigned char dati_udc[20];
/*
0 SSCC (20 byte)
*/
/*
	for(i=0;i<20;i++) 
		dati_udc[i] = 0;
        
    SSCCdascrivere = SSCC;
	if (SSCCdascrivere.Length() <20)
		SSCCdascrivere += AnsiString::StringOfChar(' ', 20-SSCCdascrivere.Length());
        
	memcpy( dati_udc, SSCCdascrivere.c_str(), 20); 

	PLCThread[0]->WriteBuffer(MainForm->PlcMonta.dbuscite, MainForm->PlcMonta.byte_inizio_SSCCscritto, 20, dati_udc, MainForm->Simula);
*/
}

//---------------------------------------------------------------------------
// Controlla la capienza a 2/3 del magazzino, il magazzino vuoto e ...boh?
void __fastcall TTDMgestionePLC::TimerAllarmiTimer(TObject *Sender)
{
	TimerAllarmi->Enabled = false;
    int my;

    try 
    {  

        /*
        if ((MainForm->PlcMonta.PlcConnesso) || (MainForm->Simula))
        {
            // Controllo Riempimento MAGAZZINO------------------------------------------
            int nPallet = dmDB->GetPalletMagazzino(TYPE_NONFASCIARE);
            if (nPallet > 0) 
            {
                if (nPallet >= dmDB->TabParametri[103]["INTVALUE"].ToIntDef(0))   //magazzino in esaurimento spazio
                {
                    if (!MainForm->PlcMonta.DueTerziMag) 
                    {
                        //Setto il segnale al montacarichi
                        MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_DueTerziMag];
                        dmDB->LogMsg("Setto MAGAZZINO in riempimento al PLC Montacarichi ");
                    }
                }
                else
                {
                    if (MainForm->PlcMonta.DueTerziMag) 
                    {
                        //Resetto il segnale al montacarichi
                        MainForm->PlcMonta.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcMonta.bit_DueTerziMag];
                        dmDB->LogMsg("Resetto MAGAZZINO in riempimento al PLC Montacarichi ");
                    }
                }
            }

            // Controllo  MAGAZZINO vuoto   ------------------------------------------
            nPallet = dmDB->GetPalletMagazzino(TYPE_TUTTI);
            if (nPallet >= 0) 
            {
                if (nPallet > 0)   //NON è vuoto
                {
                    if (MainForm->PlcMonta.MagVuoto) 
                    {
                        //Resetto il segnale al montacarichi
                        MainForm->PlcMonta.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcMonta.bit_MagVuoto];
                        dmDB->LogMsg("Resetto MAGAZZINO VUOTO al PLC Montacarichi ");
                    }
                }
                else      //vuoto
                {
                    if (!MainForm->PlcMonta.MagVuoto) 
                    {
                        //Setto il segnale al montacarichi
                        MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_MagVuoto];
                        dmDB->LogMsg("Setto MAGAZZINO VUOTO al PLC Montacarichi ");
                    }
                }
            }
            
            // Controllo mancanza VUOTI------------------------------------------
            nPallet = dmDB->GetVuotiInMagazzino();
            if (nPallet >= 0) 
            {
                if (nPallet > 0)   //Ci sono dei vuoti
                {
                    if (MainForm->PlcMonta.MancanzaVuoti) 
                    {
                        //Resetto il segnale al montacarichi
                        MainForm->PlcMonta.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcMonta.bit_MancanzaVuoti];
                        dmDB->LogMsg("Resetto MANCANZA vuoti al PLC Montacarichi ");
                    }
                }
                else      //non ci sono più VUOTI
                {
                    if (!MainForm->PlcMonta.MancanzaVuoti) 
                    {
                        //Setto il segnale al montacarichi
                        MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_MancanzaVuoti];
                        dmDB->LogMsg("Setto MANCANZA vuoti al PLC Montacarichi ");
                    }
                }
            }
            
            //Controllo se c'è un allarme attivo-------------------------------------
            if (MainForm->MessageBar1->ActMsg->Count)   // c'è un allarme attivo nella barra di stato
            {
                if (!MainForm->PlcMonta.AllarmeAttivo) 
                {
                    //Setto il segnale al montacarichi
                    MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_AllarmeAttivo];
                    dmDB->LogMsg("Setto Allarme ATTIVO nell'impianto al PLC Montacarichi ");
                }
            }
            else
            {
                if (MainForm->PlcMonta.AllarmeAttivo) 
                {
                    //Resetto il segnale al montacarichi
                    MainForm->PlcMonta.uscitedascrivere &= MainForm->bitAnd[MainForm->PlcMonta.bit_AllarmeAttivo];
                    dmDB->LogMsg("Resetto Allarme ATTIVO nell'impianto al PLC Montacarichi ");
                }
            }
        }
        */
    }
    catch (...) {
    }
    
	TimerAllarmi->Enabled = true;
}
//---------------------------------------------------------------------------

