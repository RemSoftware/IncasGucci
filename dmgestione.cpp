 // ---------------------------------------------------------------------------

#pragma hdrstop

#include "main.h"
// #include "dmphoenix.h"
#include "percorsiprototipo.h"
#include "socket.h"
#include "db.h"
#include "datamodulecomandiagv.h"
#include "PLCThread.h"
#include "plcphoenixthread.h"
#include "baiacarico.h"
#include "dmgestione.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TDMGestioneEventi *DMGestioneEventi;
extern CLIENTSOCKET *cs;
extern char N_carrelli;
extern DATILGV tab;

// ---------------------------------------------------------------------------
__fastcall TDMGestioneEventi::TDMGestioneEventi(TComponent* Owner) 
    : TDataModule(Owner) 
{
    sync_lamp = false;
    init = true;
    missione_nuova_ricevuta = 0 ;
    count_missione_nuova_ricevuta = 0;
	for(int i = 0 ; i < 5 ; i++)  {
		PosIngombroVuoti[i] = 0 ;
		PosIngombroFaldeDx[i] = 0 ;
		PosIngombroFaldeSx[i] = 0 ;
	}
	PosIngombroVuoti[0] = 90;
	PosIngombroVuoti[1] = 91;
	PosIngombroVuoti[2] = 92;
	PosIngombroFaldeDx[0] = 70;
	PosIngombroFaldeDx[1] = 71;
	PosIngombroFaldeSx[0] = 80 ;
	PosIngombroFaldeSx[1] = 81 ;
}

// ---------------------------------------------------------------------------
void __fastcall TDMGestioneEventi::TimerEventiTimer(TObject *Sender) {
    int i, j, agv, k, anomalia_generatori;
    TMissione m;
    Evento ev;

    // gestisco le missioni degli agv e tutti gli eventi

    if (MainForm->count_inizio < 15)
        return;
    TimerEventi->Enabled = false;
    TimerEventi->Enabled = true;
}

// ---------------------------------------------------------------------------

void __fastcall TDMGestioneEventi::TimerPlcTimer(TObject * Sender) 
{
	int i, j, posagv, destagv, pal_dx_agv, pal_sx_agv, livellobatagv, misincorso, statoagv, statoattualeagv, in_marcia, nUdc, idcar;
	AnsiString stringa, stringa_allarme, stringa_allarme_dascrivere;
	char code[24];

    // imposto datyi in memoria da Plc
	if (MainForm->count_inizio < 10)
		return;
        
	TimerPlc->Enabled = false;

    /*
	// gestisco chiamata PRESENZA pallet nelle aree di carico
	for (i = 1; i <= NLINEE_PREL; i++) 
    {
        // Gestione della chiamata PALLET PRONTO ---------------------
        if ((MainForm->PlcRobot.presenza_pallet_linea[i]) && (!dmDB->TabPostazioni[MainForm->PlcRobot.pos_linea[i]]["IDUDC"].ToIntDef(0)) &&
            (!MainForm->PlcRobot.richiesta_ingresso_linea[i]) && (!MainForm->PlcRobot.pallet_prelevato_linea[i]))  //metto anche prelevato per ritardare un attimo l'inserimento linea nella tab
        {
			if (MainForm->PlcRobot.count_presenza_linea[i] > 2) 
            {
                // mette un UDC in posizione
                nUdc = dmDB->CreaIdUDC();
                idcar = dmDB->InsertCaricamento(i, nUdc, MainForm->PlcRobot.pos_linea[i]); // linea, udc, pos
                dmDB->UpdatePosCarico(MainForm->PlcRobot.pos_linea[i], nUdc, idcar);
                dmDB->CaricaTabelle();
                MainForm->aggiornamappa = 1;
            } 
            else  
                MainForm->PlcRobot.count_presenza_linea[i]++;
        }
        else
        {
            MainForm->PlcRobot.count_presenza_linea[i] = 0;
            MainForm->PlcRobot.count_assenza_linea[i] = 0;
        }
    }
    */
    TimerPlc->Enabled = true;
}

// ---------------------------------------------------------------------------

void __fastcall TDMGestioneEventi::TimerPhoenixTimer(TObject * Sender) 
{
	int i, j, posagv, destagv, pal_dx_agv, pal_sx_agv, livellobatagv, misincorso, statoagv, statoattualeagv, in_marcia, val, nUdc, idcar;
    AnsiString stringa, stringa_allarme, stringa_allarme_dascrivere;
    char code[24];
    
    // imposto dati in memoria da Plc
    if (MainForm->count_inizio < 15)
        return;
    TimerPhoenix->Enabled = false;
	TimerPhoenix->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TDMGestioneEventi::TimerGestioneLuciTimer(TObject *Sender) {
    // gestione luci Plc Phoenix
    int i, agv, j, statobaia, id_var_agv, stato_agv, idregprel, idregdep, idregagv, idregprelincorso, idregdepincorso, idregagvincorso;
    AnsiString str_message;
    if (MainForm->count_inizio < 10)
        return;
    TimerGestioneLuci->Enabled = false;
    TimerGestioneLuci->Enabled = true;
}

// ---------------------------------------------------------------------------

int TDMGestioneEventi::AgvInUscitaPrelievoFasciatore() {
    int i, j, res = 0;

/*    for (i = 1; i <= N_carrelli; i++) {
        for (j = 1; j <= 4; j++) {
            if ((MainForm->DatiAgv[i].pos == MainForm->DatiPlc.posprel[j]) || (MainForm->DatiAgv[i].pos == (MainForm->DatiPlc.posprel[j] + 200))) {
                res = i;
                break;
            }
        }
	}  */
    return res;
}

int TDMGestioneEventi::AgvInPosCb(int agv) {
    int i, j, res = 0;

    /* for ( i = 1 ; i<= N_carrelli ; i++) {
     if ((agv == i)||(agv == 0)) {
     for ( j = 1 ; j<= MainForm->num_pos_cb ; j++) {
     if (MainForm->DatiAgv[i].pos == MainForm->poscb[j] ) {
     res =  MainForm->poscb[j] ;
     break ;
     }
     }
     }
     } */
    return res;
}
int TDMGestioneEventi::AgvInIngombroVuoti(int agv) {
	int i, j, res = 0;

	if (MainForm->DatiAgv[agv].pos > 0) {
		for(i = 0 ; i < 5 ; i++)  {
			if (PosIngombroVuoti[i]) {
				if ((MainForm->DatiAgv[agv].pos == PosIngombroVuoti[i])||
					(MainForm->DatiAgv[agv].consensodato == PosIngombroVuoti[i])) {
					res = agv ;
					break ;
				}
			}
		}
	}
	return res;
}
int TDMGestioneEventi::AgvInIngombroFaldeDx(int agv) {
	int i, j, res = 0;

	if (MainForm->DatiAgv[agv].pos > 0) {
		for(i = 0 ; i < 5 ; i++)  {
			if (PosIngombroFaldeDx[i]) {
				if ((MainForm->DatiAgv[agv].pos == PosIngombroFaldeDx[i])||
					(MainForm->DatiAgv[agv].consensodato == PosIngombroFaldeDx[i])) {
					res = agv ;
					break ;
				}
			}
		}
	}
	return res;
}
int TDMGestioneEventi::AgvInIngombroFaldeSx(int agv) {
	int i, j, res = 0;

	if (MainForm->DatiAgv[agv].pos > 0) {
		for(i = 0 ; i < 5 ; i++)  {
			if (PosIngombroFaldeSx[i]) {
				if ((MainForm->DatiAgv[agv].pos == PosIngombroFaldeSx[i])||
					(MainForm->DatiAgv[agv].consensodato == PosIngombroFaldeSx[i])) {
					res = agv ;
					break ;
				}
			}
		}
	}
	return res;
}

int TDMGestioneEventi::AgvInDestCb(int agv) {
    int i, j, res = 0;

    /* for ( i = 1 ; i<= N_carrelli ; i++) {
     if ((agv == i)||(agv == 0)) {
     for ( j = 1 ; j<= MainForm->num_pos_cb ; j++) {
     if ((MainForm->DatiAgv[i].generata)&&(MainForm->DatiAgv[i].dest == MainForm->poscb[j] )) {
     res =  MainForm->poscb[j] ;
     break ;
     }
     }
     }
     } */
    return res;
}

// ---------------------------------------------------------------------------

void __fastcall TDMGestioneEventi::DataModuleCreate(TObject * Sender) {
    TimerGestioneLuci->Enabled = true;
    TimerEventi->Enabled = true;
    TimerPlc->Enabled = true;
    attiva_schermata_codice = false;
}
// ---------------------------------------------------------------------------

void __fastcall TDMGestioneEventi::TimerMissioniTimer(TObject * Sender) 
{
	int agv, i, posdep, posprel, pianodep, j, tipo_pal_udc, corsia_dest, mis_scarto_generata,scarto = 0;
    int priorita, dafasciare, line, ret, nLinea, nID, opposta, stato, prioritamag, tipoPallet;
	TADOQuery *ADOQuery;
    TDateTime dtInizio;
	TMissione m;
    TRecordList TabListaMissioniPerAgv;
	AnsiString strsql,stringa, corsia_dest_ricevuta, msgstr, OraMis, namePrel, nameDest, sSSCC;
    AnsiString custPosPrel, custPosDep, sIDudc;
    TLGV lgv;

    // timer per generazione nuove missoni
    if (MainForm->count_inizio < 20)
        return;

    if (!dmDB->ADOConnection1->Connected)
        return;

    TimerMissioni->Enabled = false;
    
	if (MainForm->AbilitaMissioni) 
    {
		try 
        {
			if ((MainForm->DatiAgv[1].pos > 0) && (!MainForm->DatiAgv[1].manuale ) && (!MainForm->DatiAgv[1].bitbattbassa)
		    && (!MainForm->DatiAgv[1].alarm) && (!MainForm->DatiAgv[1].semiautomatico ) && (!MainForm->DatiAgv[1].generata ))
            {
                line = 0;
                   
                //-------------------------------------------------------------------
                // GESTIONE MISSIONI provenienti da WMS
                //-------------------------------------------------------------------
                if ((!MainForm->DatiAgv[1].generata ) && (MainForm->AbilitaMissioni))
                {
                    posprel = GetMissioniWMS(nID, posdep, sIDudc, dtInizio, custPosPrel, custPosDep, tipoPallet);
                    if ((posprel > 0) && (posdep > 0))
                    {
                        line = 1;
                        m.posprel = posprel;
                        m.posdep = posdep;
                        m.tipo_mis = 0;
                        m.Agv = 1;
                        m.priorita = 0;  // la priorità la stabiliamo dopo
                        m.formato_pallet = 0;
                        m.Source =  custPosPrel;
                        m.Dest = custPosDep;
                        m.IdUdc = sIDudc;   //
                        m.progressivo_missione = nID;
                        m.tipo_pallet = tipoPallet;
                        dmDB->GeneraMissione(m);
                            
                        dmDB->UpdateStatoCaricamento(nID, STATO_INCARICO);
                        ResettaAllAllarmiLinea();
                    }
                    //resetta eventuali allarmi di chiamata  (?va bene)
                    else if (posprel <= 0)
                    {
                        ResettaAllAllarmiLinea();
                    }
                }

                //-----------------------------------------------------------------------------
                // GESTIONE MISSIONI recupero UDC ABORTITO mentre era in missione verso...boh?   
                //
                // 
                /*
                if ((!MainForm->DatiAgv[1].generata ) && (MainForm->DatiAgv[1].load)) 
                {
                    line = 15;
                    posprel = MainForm->DatiAgv[1].pos;    //solo deposito
                    namePrel = "";
                    
                    if (MainForm->DatiAgv[1].IdUdc != 0)  //è un pallet pieno
                    {
                        stato = dmDB->GetStatoUdc(MainForm->DatiAgv[1].IdUdc);

                        if (stato >= 0) 
                        {
                            posdep = dmDB->RicercaDepositoTipologiaUDC(stato, opposta);

                            if (posdep > 0) 
                            {
                                line = 16;
                                nameDest = dmDB->CustomerPosDaPosizione(posdep);
                                m.posprel = posprel;
                                m.posdep = posdep;
                                m.tipo_mis = 2;    //solo deposito
                                m.Agv = 1;
                                m.priorita = 0;
                                m.tipo_pallet = stato;
                                m.formato_pallet = 0;
                                m.Source = namePrel;
                                m.Dest = nameDest;
                                m.IdUdc = MainForm->DatiAgv[1].IdUdc; 
                                m.progressivo_missione = 0;                          // 
                            
                                m.programma_fasciatura = dafasciare;
                                dmDB->GeneraMissione(m);
                                
                                //Cancella gli eventuali allarmi
                                if (MainForm->gAllarmi.allarme_depositomag_avvolgitore)
                                {
                                    MainForm->gAllarmi.allarme_depositomag_avvolgitore = 0;
                                    MainForm->MessageBar1->DelMsgNo(219);    
                                }
                            }
                            else if (!MainForm->gAllarmi.allarme_depositomag_avvolgitore) 
                            {
                                MainForm->gAllarmi.allarme_depositomag_avvolgitore = 1;
                                MainForm->MessageBar1->AddMsgNo(219);    
                            }
                        }
                    }
                    else     // E' un pallet VUOTO
                    {
                        posdep = dmDB->RicercaDepositoTipologiaUDC(TYPE_VUOTI, opposta);

                        if (posdep > 0) 
                        {
                            line = 17;
                            nameDest = dmDB->CustomerPosDaPosizione(posdep);
                            m.posprel = posprel;
                            m.posdep = opposta;   //si appoggiano diversi da come si prelevano- la pos è salvata nella OPPOSTA
                            m.tipo_mis = 2;       //solo deposito
                            m.Agv = 1;
                            m.priorita = 0;
                            m.tipo_pallet = stato;
                            m.formato_pallet = 0;
                            m.Source = namePrel;
                            m.Dest = nameDest;
                            m.IdUdc = MainForm->DatiAgv[1].IdUdc; 
                            m.progressivo_missione = 0;                          // 
                            
                            m.programma_fasciatura = dafasciare;
                            dmDB->GeneraMissione(m);
                        }
                    }
                }  */
			}
		}
		catch (...) 
        {
			dmDB->LogMsg("Eccezione nel generatore di missioni - line " + IntToStr(line));
		}
	}
    
	TimerMissioni->Enabled = true;
}

// ---------------------------------------------------------------------------
//
int TDMGestioneEventi::GetMissioniWMS(int &nID, int &nPosDep, AnsiString &sIDudc, TDateTime &dtInizio, AnsiString &custPosPrel, AnsiString &custPosDep, int &tipoPallet)
{
    int ret = 0;
    TADOQuery *ADOQuery;
    AnsiString strsql;
    int nPosPrel = 0;
    int pp, pd;
    bool bMod = false;
    nID = 0;
    sIDudc = "";
    nPosDep = 0;
    custPosPrel = "";
    custPosDep = "";
    tipoPallet = 0;

    if  (!dmDB->ADOConnection1->Connected)
        return ret;        

    try
    {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
            
        strsql.printf("SELECT * FROM MissioniWms WHERE StatoMissione = %d ORDER BY Priorita ASC, DataOra ASC", STATO_INSERITA);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
        {
            ADOQuery->First();
            while (!ADOQuery->Eof) 
			{
                pp = dmDB->TabCustomerPos[ADOQuery->FieldByName("PosizionePrelievo")->AsString]["POS"].ToIntDef(0); //dmDB->PosizioneDaCustomerPos(ADOQuery->FieldByName("PosizionePrelievo")->AsString);
                pd = dmDB->TabCustomerPos[ADOQuery->FieldByName("PosizioneDeposito")->AsString]["POS"].ToIntDef(0); //dmDB->PosizioneDaCustomerPos(ADOQuery->FieldByName("PosizioneDeposito")->AsString);

                if ((pp > 100) || (pd < 100)) // verifica se c'è errore nei dati ricevuti  (posizioni di tipo sbagliato)
                {
                    dmDB->UpdateStatoCaricamento(ADOQuery->FieldByName("Progressivo")->AsInteger, STATO_ERRATA);
                    break;
                }

                if ((pp > 0) && (pd > 0) && (!dmDB->TabPostazioni[pp]["DISABILITA"].ToIntDef(0)) && (!dmDB->TabPostazioni[pd]["DISABILITA"].ToIntDef(0)) )   //se non sono disabilitate le 2 POS
                {
                    nID = ADOQuery->FieldByName("Progressivo")->AsInteger;
                    nPosPrel = dmDB->TabCustomerPos[ADOQuery->FieldByName("PosizionePrelievo")->AsString]["POS"].ToIntDef(0); //dmDB->PosizioneDaCustomerPos(ADOQuery->FieldByName("PosizionePrelievo")->AsString);
                    nPosDep = dmDB->TabCustomerPos[ADOQuery->FieldByName("PosizioneDeposito")->AsString]["POS"].ToIntDef(0); //dmDB->PosizioneDaCustomerPos(ADOQuery->FieldByName("PosizioneDeposito")->AsString);
                    sIDudc = ADOQuery->FieldByName("IDUDC")->AsString;
                    dtInizio = ADOQuery->FieldByName("DataOra")->AsDateTime;
                    custPosPrel = ADOQuery->FieldByName("PosizionePrelievo")->AsString;
                    custPosDep = ADOQuery->FieldByName("PosizioneDeposito")->AsString;
                    tipoPallet = ADOQuery->FieldByName("TipoUdc")->AsInteger;
                
                    dmDB->UpdatePosCarico(custPosPrel, sIDudc, nID);   //
                    break;
                }
                ADOQuery->Next();
            }
        }
        ADOQuery->Close();
    }
    catch  (...)
    {
    }
    delete ADOQuery;
	return nPosPrel;
}


// ---------------------------------------------------------------------------

bool TDMGestioneEventi::SuperataAttesaRobot(TDateTime dtInizio) 
{
    AnsiString str;
/*    
    int val = dmDB->TabParametri[104]["INTVALUE"].ToIntDef(0);   //attesa per i robots sec
    int ore = val / 3600;
    int min = (val % 3600) / 60;
    int sec = (val % 3600) % 60;
    str.printf("%02d:%02d:%02d", ore, min, sec);
    TDateTime dt = dtInizio + StrToTime(str);
    if (Now() < dt)
        return false;
    else */
        return true;
}

// ---------------------------------------------------------------------------

int TDMGestioneEventi::GetDestinazioneFromRobot(int posprel, int linea, TDateTime dtInizio) 
{
    int ret = 0;
    int opposta;
    TADOQuery *ADOQy;
    AnsiString strsql;
/*
    if (!dmDB->ADOConnection1->Connected) {
        return 0;
    }
    
    // è un pallet incompleto
    if (MainForm->PlcRobot.pallet_incompleto[linea]) 
    {
        ret = dmDB->RicercaDepositoTipologiaUDC(TYPE_INCOMPLETI, opposta);
        if (ret <= 0) 
			SettaAllarmeLineaPerMagazzino(posprel);
        return ret;
    }
    
    // Con l'avvolgitore escluso da PARAMETRO, le missioni vanno in magazzino  ma anche se non c'è la chiamata ed è passato il tempo max
    if ((!MainForm->PlcFascia.ChiamataDeposito && SuperataAttesaRobot(dtInizio)) || 
        (MainForm->PlcFascia.escludi) || (MainForm->EscludiAvvolgitore)) // || (!MainForm->PlcFascia.FasciaNoALARM)) 
    
    {
        ret = dmDB->RicercaDepositoTipologiaUDC(TYPE_FASCIARE, opposta);
        if (ret <= 0) 
			SettaAllarmeLineaPerMagazzino(posprel);
		//else
		//	ResettaAllarme(posprel);
	}
    else 
    {
        //La richiesta ingresso fa anche da INGOMBRO
        if ((((MainForm->PlcFascia.ChiamataDeposito) && (!MainForm->PlcFascia.RichiestaIngresso)) )
            && (!MainForm->PlcFascia.escludi))
        {
			ret = MainForm->PlcFascia.posdep;
			//ResettaAllarme(posprel);
		}
        // setta l'allarme se è escluso                          //La richiesta ingresso fa anche da INGOMBRO
        else if ((MainForm->PlcFascia.ChiamataDeposito) && (!MainForm->PlcFascia.RichiestaIngresso) && (MainForm->PlcFascia.escludi))
        {                    
            if (!MainForm->gAllarmi.allarme_posizioneDEP_esclusa) 
            {
                MainForm->gAllarmi.allarme_posizioneDEP_esclusa = 1;
                MainForm->MessageBar1->AddMsgNo(220);    
            }
        }
    }
*/
	return ret;
}

// ---------------------------------------------------------------------------

bool TDMGestioneEventi::SuperataAttesaAvvolgitore() 
{
    AnsiString str;
/*    
    int val = dmDB->TabParametri[100]["INTVALUE"].ToIntDef(0);   //attesa per il montacarichi sec
    int ore = val / 3600;
    int min = (val % 3600) / 60;
    int sec = (val % 3600) % 60;
    str.printf("%02d:%02d:%02d", ore, min, sec);
    TDateTime dt = MainForm->PlcFascia.InizioAttesa + StrToTime(str);
    if (Now() < dt)
        return false;
    else  */
        return true;
}

// ---------------------------------------------------------------------------

bool TDMGestioneEventi::SuperataAttesaMontacarichi() 
{
/*
    AnsiString str;

    int val = dmDB->TabParametri[101]["INTVALUE"].ToIntDef(0);   //attesa per l'avvolgitore che chiama min
    int ore = val / 3600;
    int min = (val % 3600) / 60;
    int sec = (val % 3600) % 60;
    str.printf("%02d:%02d:%02d", ore, min, sec);
    TDateTime dt = MainForm->PlcMonta.InizioAttesa + StrToTime(str);
    if (Now() < dt)
        return false;
    else  */
        return true;
}

// ---------------------------------------------------------------------------

int TDMGestioneEventi::GetDestinazioneFromAvvolgitore(int posprel) 
{
    int ret = 0;
    int opposta;
    TADOQuery *ADOQy;
    AnsiString strsql;

/*
    if (!dmDB->ADOConnection1->Connected) {
        return 0;
    }
    // Con l'avvolgitore escluso da PARAMETRO o da PLC, le missioni vanno in magazzino - anche se ha aspettato troppo
    if (((!MainForm->PlcMonta.ChiamataDeposito) && SuperataAttesaAvvolgitore()) || MainForm->PlcMonta.escludi || (!MainForm->PlcMonta.MontacarichiAttivo)) 
    {
        ret = dmDB->RicercaDepositoTipologiaUDC(TYPE_NONFASCIARE, opposta);
        if (ret <= 0) 
        {
            MainForm->gAllarmi.allarme_depositomag_avvolgitore = 1;
			MainForm->MessageBar1->AddMsgNo(219);    
        }
    }
    else 
    {
        if ((MainForm->PlcMonta.ChiamataDeposito) && (!MainForm->PlcMonta.RichiestaIngresso) && (!MainForm->PlcMonta.OKPrenotaVuoti) 
            && (!MainForm->PlcMonta.escludi))
        {
            ret = MainForm->PlcMonta.posprel;
        }
        // setta l'allarme se è escluso
        else if ((MainForm->PlcMonta.ChiamataDeposito) && (!MainForm->PlcMonta.RichiestaIngresso) && (MainForm->PlcMonta.escludi))
        {                    
            if (!MainForm->gAllarmi.allarme_montacarichi_esclusa) {
                MainForm->gAllarmi.allarme_montacarichi_esclusa = 1;
                MainForm->MessageBar1->AddMsgNo(222);    
            }
        }
    }
    */
	return ret;
}


void TDMGestioneEventi::SettaAllarmeLineaPerMagazzino(int pos)
{
/*
    if (pos == POS_C1 && !MainForm->gAllarmi.allarme_depositomag_linea1) {
        MainForm->gAllarmi.allarme_depositomag_linea1 = 1;
        MainForm->MessageBar1->AddMsgNo(210);    
    }
    else if (pos == POS_C2 && !MainForm->gAllarmi.allarme_depositomag_linea2) {
        MainForm->gAllarmi.allarme_depositomag_linea2 = 1;
        MainForm->MessageBar1->AddMsgNo(211);    
    }
    else if (pos == POS_C3 && !MainForm->gAllarmi.allarme_depositomag_linea3) {
        MainForm->gAllarmi.allarme_depositomag_linea3 = 1;
        MainForm->MessageBar1->AddMsgNo(212);    
    }
    else if (pos == POS_C4 && !MainForm->gAllarmi.allarme_depositomag_linea4) {
        MainForm->gAllarmi.allarme_depositomag_linea4 = 1;
        MainForm->MessageBar1->AddMsgNo(213);    
    }
    else if (pos == POS_C5 && !MainForm->gAllarmi.allarme_depositomag_linea5) {
        MainForm->gAllarmi.allarme_depositomag_linea5 = 1;
        MainForm->MessageBar1->AddMsgNo(214);    
    }
    else if (pos == POS_C6 && !MainForm->gAllarmi.allarme_depositomag_linea6) {
        MainForm->gAllarmi.allarme_depositomag_linea6 = 1;
        MainForm->MessageBar1->AddMsgNo(215);    
    }
    else if (pos == POS_C7 && !MainForm->gAllarmi.allarme_depositomag_linea7) {
        MainForm->gAllarmi.allarme_depositomag_linea7 = 1;
        MainForm->MessageBar1->AddMsgNo(216);    
    }
    else if (pos == POS_C8 && !MainForm->gAllarmi.allarme_depositomag_linea8) {
        MainForm->gAllarmi.allarme_depositomag_linea8 = 1;
        MainForm->MessageBar1->AddMsgNo(217);    
    }
    else if (pos == POS_C9 && !MainForm->gAllarmi.allarme_depositomag_linea9) {
        MainForm->gAllarmi.allarme_depositomag_linea9 = 1;
        MainForm->MessageBar1->AddMsgNo(218);    
    }
    */
}

void TDMGestioneEventi::ResettaAllarme(int pos)
{
/*
    if (pos == POS_C1 && MainForm->gAllarmi.allarme_depositomag_linea1) {
        MainForm->gAllarmi.allarme_depositomag_linea1 = 0;
        MainForm->MessageBar1->DelMsgNo(210);    
    }
    else if (pos == POS_C2 && MainForm->gAllarmi.allarme_depositomag_linea2) {
        MainForm->gAllarmi.allarme_depositomag_linea2 = 0;
        MainForm->MessageBar1->DelMsgNo(211);    
    }
    else if (pos == POS_C3 && MainForm->gAllarmi.allarme_depositomag_linea3) {
        MainForm->gAllarmi.allarme_depositomag_linea3 = 0;
        MainForm->MessageBar1->DelMsgNo(212);    
    }
    else if (pos == POS_C4 && MainForm->gAllarmi.allarme_depositomag_linea4) {
        MainForm->gAllarmi.allarme_depositomag_linea4 = 0;
        MainForm->MessageBar1->DelMsgNo(213);    
    }
    else if (pos == POS_C5 && MainForm->gAllarmi.allarme_depositomag_linea5) {
        MainForm->gAllarmi.allarme_depositomag_linea5 = 0;
        MainForm->MessageBar1->DelMsgNo(214);    
    }
    else if (pos == POS_C6 && MainForm->gAllarmi.allarme_depositomag_linea6) {
        MainForm->gAllarmi.allarme_depositomag_linea6 = 0;
        MainForm->MessageBar1->DelMsgNo(215);    
    }
	else if (pos == POS_C7 && MainForm->gAllarmi.allarme_depositomag_linea7) {
		MainForm->gAllarmi.allarme_depositomag_linea7 = 0;
		MainForm->MessageBar1->DelMsgNo(216);    
    }
	else if (pos == POS_C8 && MainForm->gAllarmi.allarme_depositomag_linea8) {
		MainForm->gAllarmi.allarme_depositomag_linea8 = 0;
		MainForm->MessageBar1->DelMsgNo(217);    
    }
	else if (pos == POS_C9 && MainForm->gAllarmi.allarme_depositomag_linea9) {
		MainForm->gAllarmi.allarme_depositomag_linea9 = 0;
        MainForm->MessageBar1->DelMsgNo(218);    
    }
    */
}

void TDMGestioneEventi::ResettaAllAllarmiLinea()
{
/*
    if (MainForm->gAllarmi.allarme_depositomag_linea1) 
    {
        MainForm->gAllarmi.allarme_depositomag_linea1 = 0;
        MainForm->MessageBar1->DelMsgNo(210);    
    }
    if (MainForm->gAllarmi.allarme_depositomag_linea2) 
    {
        MainForm->gAllarmi.allarme_depositomag_linea2 = 0;
        MainForm->MessageBar1->DelMsgNo(211);    
    }
    if (MainForm->gAllarmi.allarme_depositomag_linea3) 
    {
        MainForm->gAllarmi.allarme_depositomag_linea3 = 0;
        MainForm->MessageBar1->DelMsgNo(212);    
    }
    if (MainForm->gAllarmi.allarme_depositomag_linea4) 
    {
        MainForm->gAllarmi.allarme_depositomag_linea4 = 0;
        MainForm->MessageBar1->DelMsgNo(213);    
    }
    if (MainForm->gAllarmi.allarme_depositomag_linea5) 
    {
        MainForm->gAllarmi.allarme_depositomag_linea5 = 0;
        MainForm->MessageBar1->DelMsgNo(214);    
    }
    if (MainForm->gAllarmi.allarme_depositomag_linea6)
    {
        MainForm->gAllarmi.allarme_depositomag_linea6 = 0;
        MainForm->MessageBar1->DelMsgNo(215);
    }
	if (MainForm->gAllarmi.allarme_depositomag_linea7) 
    {
		MainForm->gAllarmi.allarme_depositomag_linea7 = 0;
		MainForm->MessageBar1->DelMsgNo(216);    
    }
	if (MainForm->gAllarmi.allarme_depositomag_linea8) 
    {
		MainForm->gAllarmi.allarme_depositomag_linea8 = 0;
		MainForm->MessageBar1->DelMsgNo(217);    
    }
	if (MainForm->gAllarmi.allarme_depositomag_linea9) 
    {
		MainForm->gAllarmi.allarme_depositomag_linea9 = 0;
        MainForm->MessageBar1->DelMsgNo(218);    
    }
	if (MainForm->gAllarmi.allarme_posizioneDEP_esclusa)
    {
        MainForm->gAllarmi.allarme_posizioneDEP_esclusa = 0;
        MainForm->MessageBar1->DelMsgNo(220);    
    }
    */
}


int TDMGestioneEventi::RitornaLineaPrelievoDaPos(int pos)
{

    int ret = 0;
/*
    for (int j=1; j<=NLINEE_PREL; j++)
    {
        if (MainForm->PlcRobot.pos_linea[j] == pos)
        {
            ret = j;
            break;    
        }
    }
*/
	return ret;
}


// ---------------------------------------------------------------------------
void TDMGestioneEventi::PalletPrelevatoDepositatoSuPressa(int pos, int prelevato) {
    int i, j, k;

/*    if ((pos >110)&&(pos < 115)) {
        //prelievo da avvolgitore
        //resetto tutte le lampadine
        for (i = 1; i <= 4; i++) {
			if (MainForm->DatiPhoenix.PulsantieraAvvolgitore.ChiamataConfermata[i]) {
                PLCPhoenixThread[0]->ScriviPhoenix( MainForm->DatiPhoenix.PulsantieraAvvolgitore.bit_chiamata_confermata[i],MainForm->DatiPhoenix.PulsantieraAvvolgitore.addr_chiamata_confermata[i], false);
                dmDB->LogMsg("Resetto luce Tipo Pallet su Avvolgitore , Tipo Pallet" + IntToStr(i));
            }
        }
    }else if ((pos < 10) || (pos > 800)) {
        // gestione pallet prelevato/depositato su pos scarti e pos D25 a terra
        for (i = 1; i <= 7; i++) {
            if ((pos == MainForm->DatiPhoenix.MPScarti[i].posprel) || (pos == MainForm->DatiPhoenix.MPScarti[i].posdep)) {
                if (prelevato) {
                    MainForm->DatiPhoenix.MPScarti[i].pallet_presente = 0;
                    PLCPhoenixThread[0]->ScriviPhoenix(MainForm->DatiPhoenix.MPScarti[i].bitOut_matprime, MainForm->DatiPhoenix.MPScarti[i].addrOut_matprime, false);
                    dmDB->LogMsg("Resetto Segnali PosMaterie Prime " + MainForm->DatiPhoenix.MPScarti[i].NomeBaia + " , dopo pallet prelevato da pos " + IntToStr(pos));
                }
                else {
                    MainForm->DatiPhoenix.MPScarti[i].pallet_presente = 1;
                    dmDB->LogMsg("Assegno pallet presente a PosMaterie Prime " + MainForm->DatiPhoenix.MPScarti[i].NomeBaia + " , dopo pallet depositato in pos " + IntToStr(pos));
                }
                dmDB->AggiornaPulsanti(16 + i);
                break;
            }
        }
    }
    else {
        for (i = 1; i <= MainForm->num_stazioni; i++) {
            for (j = 1; j <= 4; j++) {
                if (pos == MainForm->DatiPhoenix.DatiPulsantiera[i].pos[j]) {
                    if (prelevato) {
                        MainForm->DatiPhoenix.DatiPulsantiera[i].pallet_presente = 0;
                        PLCPhoenixThread[0]->ScriviPhoenix(MainForm->DatiPhoenix.DatiPulsantiera[i].bit_chiamata_vuoti_confermata[1], MainForm->DatiPhoenix.DatiPulsantiera[i].addr_chiamata_vuoti_confermata[1], false);
                        PLCPhoenixThread[0]->ScriviPhoenix(MainForm->DatiPhoenix.DatiPulsantiera[i].bit_chiamata_vuoti_confermata[2], MainForm->DatiPhoenix.DatiPulsantiera[i].addr_chiamata_vuoti_confermata[2], false);
                        PLCPhoenixThread[0]->ScriviPhoenix(MainForm->DatiPhoenix.DatiPulsantiera[i].bit_chiamata_vuoti_confermata[3], MainForm->DatiPhoenix.DatiPulsantiera[i].addr_chiamata_vuoti_confermata[3], false);
                        PLCPhoenixThread[0]->ScriviPhoenix(MainForm->DatiPhoenix.DatiPulsantiera[i].bit_chiamata_vuoti_confermata[4], MainForm->DatiPhoenix.DatiPulsantiera[i].addr_chiamata_vuoti_confermata[4], false);
                        PLCPhoenixThread[0]->ScriviPhoenix(MainForm->DatiPhoenix.DatiPulsantiera[i].bit_chiamata_pieno_confermata, MainForm->DatiPhoenix.DatiPulsantiera[i].addr_chiamata_pieno_confermata, false);
                        dmDB->LogMsg("Resetto Segnali Pulsantiera " + IntToStr(i) + " , dopo pallet prelevato da pos " + IntToStr(pos));
                    }
                    else {
                        for (k = 1; k <= 4; k++) {
                            if (MainForm->DatiPhoenix.DatiPulsantiera[i].ChiamataVuotiConfermata[k]) {
                                MainForm->DatiPhoenix.DatiPulsantiera[i].pallet_presente = k;
                                break;
                            }
                        }
                        if (!MainForm->DatiPhoenix.DatiPulsantiera[i].pallet_presente) {
                            dmDB->LogMsg("Associo tipo pal sconosciuto su Pressa " + MainForm->DatiPhoenix.DatiPulsantiera[i].NomePressa + " perchè nessun pulsante attivo");
                            MainForm->DatiPhoenix.DatiPulsantiera[i].pallet_presente = 5;
                        }
                    }
                    dmDB->AggiornaPulsanti(i);
                    break;
                }
            }
        }
	}
	*/
}


