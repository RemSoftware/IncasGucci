//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "SupervisioneSocket.h"
#include "percorsiprototipo.h"
#include "db.h"
#include "dmgestione.h"
#include "datamodulecomandiagv.h"
#include "dmgestioneporte.h"
#include "dmgestioneplc.h"
#include "PLCThread.h"

extern CLIENTSOCKET *cs ;
//extern CLIENTSOCKET *sock ;
extern DATILGV tab;
//---------------------------------------------------------------------------
SUPERVISIONEMISS_STATESOCKET::SUPERVISIONEMISS_STATESOCKET():MISS_STATESOCKET() {}

void SUPERVISIONEMISS_STATESOCKET::Elabora(char *ev, TCustomWinSocket *Socket) {
	char s[100], str[100], answer[100], stmis,op;
    Evento ev2 ;
	int n, pos,pianodep,corsiadep,linea_prel, idmiss,idmislista, i,id,allarme,sorg,lista,destinazione,poscodice,cambiodest,idalenia,rulliera,posagv,idvuoti;
		AnsiString Msg,str2,strcons, prevstat;
		TObject *Sender ;
//	Variant locvalues[2];
	AnsiString Codice,NoPrel,CodiceCliente,orapal,stringa,HuTyp,Source,Dest,CP,HuIdent;
	TDatiSSCCPlc DatiDaScrivere ;
	int dest = 0,word,idmacchina,val,baia_prel=0,baia_dep=0,tipomis,porta,posdep,prel,posold,nagv,loaded,nodoa,nodob,corsia,bitingombro,livellobatt,resetingombro,idmisprotec,bypass ;
	bool hogenerato,iningombro,abilitata,update = false ;
    AnsiString IdUdc;
	int pianoprel,stato_manuale,stato_allarme,progressivo_wms,programma_fasciatura,tipopos;
   //	if ((dmDB->ADOConnection1->Connected)&&(MainForm->lettostatoagv)&&(MainForm->lettoparametriplc)&&(MainForm->count_inizio > 3)) {
	if ((dmDB->ADOConnection1->Connected)&&(dmDB->lettostatoagv)&&(MainForm->count_inizio > 5)) {
        sprintf(answer, "OK,%s", ev);
        SendAnswer(answer, Socket);
//        MainForm->AggiornaStatoAgv() ;
//    }
//	if ((dmDB->ADOConnection1->Connected)&&(dmDB->lettostatoagv)) {
		switch (ev[0]) {
        case EV_BIT:
            break;
        case EV_ATTESA:
            if (ev[2] == 'N') {
                //se sto per prelevare segnalo agv in ingombro
                sscanf(ev+4, "%d,%d,%c", &n,&nodoa,&op);
                nagv = n ;
                //MainForm->DatiAgv[n].caricoincorso = 1 ;
			  /*  if (MainForm->Plc[1].ChiamataDeposito) {
                    sprintf(ev, "O,N,%d,%d,%c,%d",n,nodoa,op,1);
                    cs->SendEv(ev);
                    MainForm->LogMsg("Inviato consenso al deposito all'agv "+IntToStr(n)+ " con scarico libero");
				}*/
            }else {
				sscanf(ev+4, "%d,%d,%d,%s,%d", &n,&nodoa, &nodob,str,posagv);
				nagv = n ;
				MainForm->DatiAgv[nagv].nodoa = nodoa ;
				MainForm->DatiAgv[nagv].nodob = nodob ;
				MainForm->DatiAgv[nagv].richiestaconsenso = 1 ;
				MainForm->DatiAgv[nagv].consensodato = 0 ;
				MainForm->DatiAgv[nagv].primo_ciclo_consenso = 1 ;
				strcons = str ;
				if (strcons.Pos(","))
					strcons= strcons.SubString(1,strcons.Pos(",")-1) ;
				MainForm->DatiAgv[nagv].strcons = strcons ;
               /* if (strcons.Pos("mod_dest") > 0) {
                    MainForm->DatiAgv[nagv].richiestaconsenso = 0 ;
                    if (!MainForm->MissTable->Active)
                        MainForm->MissTable->Open();
                    if (MainForm->MissTable->Locate ("Id",MainForm->DatiAgv[nagv].idmis,Opts)) {
                        posdep = MainForm->MissTable->FieldByName("PosDep")->AsInteger;
                        if (posdep == MainForm->Plc[1].posprelnuova) {                                //solo per la modalità senza codici
                            //guardo che k'altro agv non sia nella destinazione
                            if ((MainForm->DatiAgv[nagv].bitbattbassa)||
                                ((MainForm->Plc[1].TotalePalletPresenti >= MainForm->Plc[1].numpalletminimo)&&(MainForm->Plc[1].TotalePalletPresentiNuova < MainForm->Plc[1].numpalletminimo))
                                || (!MainForm->Plc[1].TotalePalletPresentiNuova )
                                ||(MainForm->CheckBox2->Checked)
                                ||(MainForm->Plc[1].TotalePalletPresenti > MainForm->Plc[1].TotalePalletPresentiNuova)
                                ||(MainForm->Plc[1].TotalePalletPresentiNuova < MainForm->Plc[1].numpalletminimo)
                                ||((MainForm->Plc[1].TotalePalletPresenti >= MainForm->Plc[1].numpalletminimo)&&(MainForm->Plc[1].TotalePalletPresentiNuova >= MainForm->Plc[1].numpalletminimo)
                                    &&((MainForm->Plc[1].OraChiamataNuova == "")||
                                    ((MainForm->Plc[1].OraChiamata != "")&&(MainForm->Plc[1].OraChiamataNuova != "")&&(StrToDateTime(MainForm->Plc[1].OraChiamata) < StrToDateTime(MainForm->Plc[1].OraChiamataNuova)))))) {
                                MainForm->LogMsg("Modificata missione ad Agv "+IntToStr(nagv)+ " con destinazione vecchia "+IntToStr(posdep)+" , destinazione nuova a rulliera vecchia pos 1, agv in pos "+IntToStr(MainForm->DatiAgv[nagv].pos));
                                MainForm->MissTable->Edit();
                                MainForm->MissTable->FieldByName("PosDep")->AsInteger = MainForm->Plc[1].posprel;
                                MainForm->DatiAgv[nagv].dest = MainForm->Plc[1].posprel ;
                                MainForm->DatiAgv[nagv].dep = MainForm->Plc[1].posprel ;
                                MainForm->MissTable->Post();
                                sprintf(ev, "%c,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d", EV_NUOVAMISSIONE, MainForm->DatiAgv[nagv].idmis, posdep,0, MainForm->Plc[1].posprel,0, 0, 4,0,0,nagv);
                                cs->SendEv(ev);
                            }else {
                                MainForm->LogMsg("Consenso dato ad andare alla rulliera nuova con Agv "+IntToStr(nagv)+ " in pos "+IntToStr(MainForm->DatiAgv[nagv].pos)+" ,dest "+IntToStr(MainForm->DatiAgv[nagv].dest)+" ,load "+IntToStr(MainForm->DatiAgv[nagv].load)+" ,prel "+IntToStr(MainForm->DatiAgv[nagv].prel)+" ,dep "+IntToStr(MainForm->DatiAgv[nagv].dep));
                                sprintf(ev, "O,R,%d,%d,%d,%d",nagv,nodoa,nodob,1);
                                cs->SendEv(ev);
                            }
                        }else {
                            MainForm->LogMsg("Consenso dato ad andare alla rulliera nuova con Agv "+IntToStr(nagv)+ " in pos "+IntToStr(MainForm->DatiAgv[nagv].pos)+" ,dest "+IntToStr(MainForm->DatiAgv[nagv].dest)+" ,load "+IntToStr(MainForm->DatiAgv[nagv].load)+" ,prel "+IntToStr(MainForm->DatiAgv[nagv].prel)+" ,dep "+IntToStr(MainForm->DatiAgv[nagv].dep));
                            sprintf(ev, "O,R,%d,%d,%d,%d",nagv,nodoa,nodob,1);
                            cs->SendEv(ev);
                        }
                    }else {
                        MainForm->LogMsg("Consenso dato per mod_dest con Agv "+IntToStr(nagv)+ " in pos "+IntToStr(MainForm->DatiAgv[nagv].pos)+" ,dest "+IntToStr(MainForm->DatiAgv[nagv].dest)+" ,load "+IntToStr(MainForm->DatiAgv[nagv].load)+" ,prel "+IntToStr(MainForm->DatiAgv[nagv].prel)+" ,dep "+IntToStr(MainForm->DatiAgv[nagv].dep));
                        sprintf(ev, "O,R,%d,%d,%d,%d",nagv,nodoa,nodob,1);
                        cs->SendEv(ev);
                    }
                    MainForm->MissTable->Close();
                        //passo altezze e tabelle car scar a 0 così restano quelle della missione , inviare idmis uguale e posprel=posdep = dest missione lgv
                        //invio la missione che toglie la richiesta e fa partire l'agv aggirando la navetta
                }
                if (strcons.Pos("deposita") > 0) {
                    if (MainForm->Plc[1].ChiamataDeposito) {
                        MainForm->LogMsg("Consenso dato per andare al deposito con Agv "+IntToStr(nagv)+ " in pos "+IntToStr(MainForm->DatiAgv[nagv].pos)+" ,dest "+IntToStr(MainForm->DatiAgv[nagv].dest)+" ,load "+IntToStr(MainForm->DatiAgv[nagv].load)+" ,prel "+IntToStr(MainForm->DatiAgv[nagv].prel)+" ,dep "+IntToStr(MainForm->DatiAgv[nagv].dep));
                        sprintf(ev, "O,R,%d,%d,%d,%d",nagv,nodoa,nodob,1);
                        cs->SendEv(ev);
                        if (MainForm->DatiAgv[nagv].alarmconsenso) {
                            MainForm->MessageBar1->DelMsgNo(195) ;
                            MainForm->DatiAgv[nagv].alarmconsenso = 0 ;
                        }
                    }else if ((!MainForm->DatiAgv[nagv].alarmconsenso)&&(MainForm->DatiAgv[nagv].pos == nodoa)) {
                        MainForm->MessageBar1->Messages->Strings[195] = "Agv Waiting Free Discharge ";
                        MainForm->MessageBar1->AddMsgNo(195) ;
                        MainForm->DatiAgv[nagv].alarmconsenso = 1 ;
                    }
				}*/
			}
		break ;
		case 'L':
			//cambia lo stato del loaded
			sscanf(ev+2, "%d,%d", &nagv, &loaded);
		  /*	if (MainForm->DatiAgv[nagv].load != loaded ) {
				MainForm->LogMsg("Ricevuto nuovo stato del carico a bordo dopo ripristino agv : Load Prec : "+IntToStr(MainForm->DatiAgv[nagv].load)+" , Load Nuovo : "+IntToStr(loaded));
				MainForm->DatiAgv[nagv].load = loaded ;
			}  */

		break ;
		case '£':
			//richiesta consenso ramo completata
			sscanf(ev+2, "%d,%d,%d,%s", &n,&nodoa, &nodob,str);
			nagv = n ;
			MainForm->DatiAgv[nagv].nodoa = nodoa ;
			MainForm->DatiAgv[nagv].nodob = nodob ;
			strcons = str ;
			if (strcons.Pos(","))
				strcons= strcons.SubString(1,strcons.Pos(",")-1) ;
			if ((MainForm->DatiAgv[nagv].consensodato)&&(nodob == MainForm->PlcFascia.posdep))
				MainForm->DatiAgv[nagv].consensodato = 0 ;
			MainForm->DatiAgv[nagv].strcons = "";
		break ;
		case EV_STATOAGV:
            sscanf(ev+2, "%d,%d,%d,%d,%d,%d,%8s,%s", &n, &pos,&dest, &idmiss, &livellobatt,&bypass,s, str);
			nagv = n ;
			stato_manuale = MainForm->DatiAgv[nagv].manuale;
			stato_allarme = MainForm->DatiAgv[nagv].alarm;
			if ((MainForm->DatiAgv[nagv].pos != 0)&&(pos == 0 )) {
                //abortire tutte le missioni relative a quell'agv
				dmDB->LogMsg("Agv "+IntToStr(n)+ " disabilitato in pos : "+ IntToStr(MainForm->DatiAgv[nagv].pos));
				//DataModuleSap->InviaStatoAllarmeAgv(nagv,true,true) ; // agv ok
			}else if ((pos != 0)&&(MainForm->DatiAgv[nagv].pos == 0 )) {
				dmDB->LogMsg("Agv "+IntToStr(n)+ " riabilitato in pos : "+ IntToStr(pos));
				//DataModuleSap->InviaStatoAllarmeAgv(nagv,false,true) ; // agv ok
			}
            if (MainForm->DatiAgv[nagv].pos != pos ){
                MainForm->DatiAgv[nagv].posprec = MainForm->DatiAgv[nagv].pos;
				MainForm->DatiAgv[nagv].pos = pos ;
				update = true ;
				//dmComandiAgv->AggiornaPrenotazioni();
			}
            MainForm->DatiAgv[nagv].idmis = idmiss ;
            MainForm->DatiAgv[nagv].LivelloBatteria = livellobatt ;
            MainForm->DatiAgv[nagv].stand_by = bypass & MainForm->bit[0] ;
			MainForm->DatiAgv[nagv].bumper_bypass = bypass & MainForm->bit[1] ;

			MainForm->DatiAgv[nagv].registro5 = bypass ;
            if ((s[0] == '1')&&(!MainForm->DatiAgv[nagv].bitbattbassa)) {
                MainForm->DatiAgv[nagv].bitbattbassa = true ;
                dmDB->LogMsg("Ricevuto segnale da Agv di Batteria Bassa, capacita : "+IntToStr(livellobatt)+", pos : "+IntToStr(pos));
			}
            else if (s[0] == '0') {
                if (MainForm->DatiAgv[nagv].bitbattbassa)
                    dmDB->LogMsg("Ricevuto reset segnale da Agv di Batteria Bassa, capacita : "+IntToStr(livellobatt)+", pos : "+IntToStr(pos));
                MainForm->DatiAgv[nagv].bitbattbassa = false ;
			}
            if (idmiss != 0) {
				stringa = "Select * from Missioni where id = "+IntToStr(idmiss);
				dmDB->QueryMissioniSocket->Close();
                dmDB->QueryMissioniSocket->SQL->Clear();
                dmDB->QueryMissioniSocket->SQL->Append( stringa) ;
                dmDB->QueryMissioniSocket->Open();
                if (dmDB->QueryMissioniSocket->RecordCount) {
					MainForm->DatiAgv[nagv].tipomis = dmDB->QueryMissioniSocket->FieldByName("TipoMissione")->AsInteger;
                    if ((MainForm->DatiAgv[nagv].tipomis < 2)&&(!MainForm->DatiAgv[nagv].load))
                        MainForm->DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
                    else
						MainForm->DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    MainForm->DatiAgv[nagv].prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
					MainForm->DatiAgv[nagv].dep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
					MainForm->DatiAgv[nagv].NomePosPrel = dmDB->QueryMissioniSocket->FieldByName("PosPrelCliente")->AsString;
					MainForm->DatiAgv[nagv].NomePosDep = dmDB->QueryMissioniSocket->FieldByName("PosDepCliente")->AsString;
				}
                dmDB->QueryMissioniSocket->Close();
            }else {
                MainForm->DatiAgv[nagv].dest = dest;
                MainForm->DatiAgv[nagv].prel = 0;
				MainForm->DatiAgv[nagv].dep = 0;
				MainForm->DatiAgv[nagv].NomePosPrel = "";
				MainForm->DatiAgv[nagv].NomePosDep = "";
			}
            if (pos != 0 ) {
                if (s[7] == '1') {
                    if (!MainForm->DatiAgv[nagv].manuale) {
                       // taucs->SendEv(ev);
						update = true ;
						dmDB->LogMsg("Agv numero "+IntToStr(n)+ " in manuale in pos : "+ IntToStr(pos));
                    }
                    //in manuale resetto i motori se sono in pos 1 o 2
                    MainForm->DatiAgv[nagv].manuale = true ;
					MainForm->DatiAgv[nagv].stand_by = 0 ;
					MainForm->DatiAgv[nagv].richiestaconsenso = 0 ;
					//MainForm->DatiAgv[nagv].consensodato = 0 ;
					MainForm->DatiAgv[nagv].incarica = 0 ;
					MainForm->DatiAgv[nagv].annullamissione = 0 ;
					if (MainForm->DatiAgv[nagv].alarmconsenso) {
						MainForm->DatiAgv[nagv].alarmconsenso = 0 ;
						MainForm->MessageBar1->DelMsgNo(MainForm->DatiAgv[nagv].num_alarm_consenso);   //alarm 502---506);
					}
                    //se ho una richiesta alta e non sono nella posizione e neanche nel pos fuori ingombro lo resetto
                }else {
                    if (MainForm->DatiAgv[nagv].manuale) {
                       // taucs->SendEv(ev);
                        update = true ;
						dmDB->LogMsg("Agv numero "+IntToStr(n)+ " in automatico in pos : "+ IntToStr(pos)+" load : " + s[1] + " ; idmiss : " + IntToStr(idmiss));
    //                    MainForm->InviaVelocita(MainForm->DatiAgv[nagv].vel_mis) ;
                    }
                    MainForm->DatiAgv[nagv].manuale = false ;
                }
            }
            if (s[6] == '1')
                MainForm->DatiAgv[nagv].semiautomatico = 1 ;
            else
                MainForm->DatiAgv[nagv].semiautomatico = 0 ;

			if (s[2] == '0')
                MainForm->DatiAgv[nagv].incarica = false ;
			else
			{
				MainForm->DatiAgv[nagv].incarica = true ;
				MainForm->DatiAgv[nagv].consensodato = 0;
            }
            if (strcmp(MainForm->DatiAgv[nagv].allarme,str) != 0 ) {
                strcpy(MainForm->DatiAgv[nagv].allarme,str);
				dmDB->GestioneAllarmi(nagv);
                update = true ;
            }
            if (s[1] == '1') {
                //MainForm->DatiAgv[nagv].load = true ;
                if (!MainForm->DatiAgv[nagv].load) {
                    MainForm->DatiAgv[nagv].load = true ;
					dmDB->LogMsg("Ricevuto Agv con carico a borsdo , Agv numero "+IntToStr(n)+ " in pos : "+ IntToStr(pos)+" ; idmiss : " + IntToStr(idmiss));
                }
            }else if (MainForm->DatiAgv[nagv].load) {
                MainForm->DatiAgv[nagv].load = false ;
				dmDB->LogMsg("Ricevuto Agv senza il carico a borsdo , Agv numero "+IntToStr(n)+ " in pos : "+ IntToStr(pos)+" ; idmiss : " + IntToStr(idmiss));
                update = true;
            }
			if (update) {
            /// due volte in errore qui
				MainForm->DisegnaCarrello(nagv) ;
				MainForm->VisualizzaPercorsoAgv(nagv);
			}
            
            //DISCRIMINARE on/off se il WMS può inviare nuove missioni
            prevstat = MainForm->DatiAgv[nagv].stato;
            if ((MainForm->DatiAgv[nagv].bitbattbassa) || (strcmp(MainForm->DatiAgv[nagv].allarme, "NONE")) || (MainForm->DatiAgv[nagv].manuale))
                MainForm->DatiAgv[nagv].stato = 1;
            else
                MainForm->DatiAgv[nagv].stato = 0;
            if (prevstat != MainForm->DatiAgv[nagv].stato) 
                update = true;
                
			if (update)
				dmDB->AggiornaStatoAgv(nagv);

			/*
			if (((stato_manuale)&&(!MainForm->DatiAgv[nagv].manuale)&&(!MainForm->DatiAgv[nagv].alarm))||
				((stato_allarme)&&(!MainForm->DatiAgv[nagv].manuale)&&(!MainForm->DatiAgv[nagv].alarm)))
				DataModuleSap->InviaStatoAllarmeAgv(nagv,false) ; // agv ok
			else if (((!stato_manuale)&&(MainForm->DatiAgv[nagv].manuale))||
				((!stato_allarme)&&(MainForm->DatiAgv[nagv].alarm)))
				DataModuleSap->InviaStatoAllarmeAgv(nagv,true) ; // agv ok
			 */
			break;
        case EV_STATOMISSIONE :
            //sscanf(ev+2, "%d,%c,%d,%d,%d",&idmiss  , s , &pos, &id,&loaded);
			sscanf(ev+2, "%d,%c,%d,%d,%d",&idmiss  , s , &pos, &id,&loaded);
            nagv = id ;
            if (MainForm->DatiAgv[nagv].pos != pos ){
                MainForm->DatiAgv[nagv].posprec = MainForm->DatiAgv[nagv].pos;
                MainForm->DatiAgv[nagv].pos = pos ;
            }
            MainForm->DatiAgv[nagv].idmis = idmiss ;
			MainForm->DatiAgv[nagv].load = loaded ;
			stringa = "Select * from Missioni where id = "+IntToStr(idmiss);
            dmDB->QueryMissioniSocket->Close();
            dmDB->QueryMissioniSocket->SQL->Clear();
            dmDB->QueryMissioniSocket->SQL->Append( stringa) ;
			dmDB->QueryMissioniSocket->Open();
			if (dmDB->QueryMissioniSocket->RecordCount) {
				progressivo_wms = dmDB->QueryMissioniSocket->FieldByName("ProgressivoMissione")->AsInteger;
				MainForm->DatiAgv[nagv].NomePosPrel = dmDB->QueryMissioniSocket->FieldByName("PosPrelCliente")->AsString;
				MainForm->DatiAgv[nagv].NomePosDep = dmDB->QueryMissioniSocket->FieldByName("PosDepCliente")->AsString;
			}
			switch (s[0]) {
            case 'I':   //Missione Iniziata
				if (dmDB->QueryMissioniSocket->RecordCount) 
                {
					if (dmDB->QueryMissioniSocket->FieldByName("TipoMissione")->AsInteger < 2)
                        MainForm->DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
                    else
						MainForm->DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
					MainForm->DatiAgv[nagv].tipomis = dmDB->QueryMissioniSocket->FieldByName("TipoMissione")->AsInteger;
					MainForm->DatiAgv[nagv].prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
					MainForm->DatiAgv[nagv].dep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    progressivo_wms = dmDB->QueryMissioniSocket->FieldByName("ProgressivoMissione")->AsInteger;

				}
				dmDB->QueryMissioniSocket->Close();
				dmDB->AggiornaMissione(idmiss,1,nagv) ;
				dmDB->LogMsg("Missione n° "+ IntToStr(idmiss)+" iniziata da agv numero " + IntToStr(id));

				if (progressivo_wms) 
					dmDB->UpdateStatoCaricamento(progressivo_wms, STATO_INIZIATA);
                
                break;
            case 'P':   //Prelievo effettuato
				if (dmDB->QueryMissioniSocket->RecordCount) 
                {
					tipomis = dmDB->QueryMissioniSocket->FieldByName("TipoMissione")->AsInteger;
					prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
					posdep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    tipopos = dmDB->RitornaTipoPos(prel);
  					MainForm->DatiAgv[nagv].IdUdc = dmDB->QueryMissioniSocket->FieldByName("IdUdc")->AsString;
					MainForm->DatiAgv[nagv].programma_fasciatura = dmDB->QueryMissioniSocket->FieldByName("ProgrammaFasciatura")->AsInteger;
   					MainForm->DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
					Source = dmDB->QueryMissioniSocket->FieldByName("PosPrelCliente")->AsString;
					Dest = dmDB->QueryMissioniSocket->FieldByName("PosDepCliente")->AsString;
                    progressivo_wms = dmDB->QueryMissioniSocket->FieldByName("ProgressivoMissione")->AsInteger;
                }
				dmDB->QueryMissioniSocket->Close();
				dmDB->LogMsg("Prelievo missione "+ IntToStr(idmiss)+" eseguito in posizione : "+ IntToStr(pos)+" da agv numero " + IntToStr(id));
				dmDB->AggiornaMissione(idmiss,2,nagv);

				if (progressivo_wms) 
					dmDB->UpdateStatoCaricamento(progressivo_wms, STATO_PRELEVATA);

				/*linea_prel = DMGestioneEventi->RitornaLineaPrelievoDaPos(prel);
				if (linea_prel)  //ROBOTS
                {
                    MainForm->PlcRobot.uscitedascrivere[MainForm->PlcRobot.addr_pallet_prelevato_linea[linea_prel]] |= MainForm->bit[MainForm->PlcRobot.bit_pallet_prelevato_linea[linea_prel]];
                    dmDB->LogMsg("SETTO segnale pallet prelevato su linea "+IntToStr(linea_prel));
				} 
                else if (prel == MainForm->PlcFascia.posdep)   //FASCIATORE
                {
                    //PLCThread[0]->SetBit(MainForm->PlcFascia.dbuscite, MainForm->PlcFascia.addr_pallet_prelevato, MainForm->PlcFascia.bit_pallet_prelevato, true);
                    MainForm->PlcFascia.uscitedascrivere |= MainForm->bit[MainForm->PlcFascia.bit_pallet_prelevato];
                    dmDB->LogMsg("SETTO segnale pallet prelevato su Fasciatore ");
                }
                else if (prel == MainForm->PlcMonta.posprel)  //MONTACARICHI
                {
                    MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_vuoto_prelevato];
                    dmDB->LogMsg("SETTO segnale vuoto prelevato su Montacarichi ");
                }  */
                
				MainForm->DatiAgv[nagv].consensodato = 0;
				dmDB->ArticoloPrelevatoDepositato(prel,"",0);
				if (dmDB->FilaPosizione(prel) > 0 ) {
					if (dmDB->ContaPalletInCorsia(dmDB->FilaPosizione(prel))) 
                    {
						dmDB->SettaCorsiaInUso(dmDB->FilaPosizione(prel),1, true);
						dmDB->SettaCorsiaPiena(dmDB->FilaPosizione(prel), 0);
					}
                    else
						dmDB->SvuotaCorsia (dmDB->FilaPosizione(prel));
				}
				//dmDB->PrenotaNomePosizione(Source,0);
                dmDB->PrenotaPosIntera(prel,0);
//                MainForm->DisegnaCarrello(id);
                break;
            case 'C':   //Missione Completata
                posdep = 0 ;
                NoPrel ="";
				if (dmDB->QueryMissioniSocket->RecordCount) {
//                    posprel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger ;
					posdep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger ;
					//pianodep = dmDB->QueryMissioniSocket->FieldByName("PianoDep")->AsInteger ;
					tipomis = dmDB->QueryMissioniSocket->FieldByName("TipoMissione")->AsInteger;
					IdUdc = dmDB->QueryMissioniSocket->FieldByName("IdUdc")->AsString;
					Source = dmDB->QueryMissioniSocket->FieldByName("PosPrelCliente")->AsString;
					Dest = dmDB->QueryMissioniSocket->FieldByName("PosDepCliente")->AsString;
					programma_fasciatura = dmDB->QueryMissioniSocket->FieldByName("ProgrammaFasciatura")->AsInteger;
                    progressivo_wms = dmDB->QueryMissioniSocket->FieldByName("ProgressivoMissione")->AsInteger;
                    tipopos = dmDB->RitornaTipoPos(posdep);
                }
				dmDB->LogMsg("Missione "+ IntToStr(idmiss)+" completata, posizione : "+ IntToStr(pos)+" da Agv n° " + IntToStr(id));
                dmDB->QueryMissioniSocket->Close();
				dmDB->AggiornaMissione(idmiss,3,nagv) ;

                // Aggiorna lo stato nella tabella di carico 
				if (progressivo_wms)
					dmDB->UpdateStatoCaricamento(progressivo_wms, STATO_DEPOSITATA);

                /*if (posdep == MainForm->PlcFascia.posdep)   //FASCIATORE
                {
                    //PLCThread[0]->SetBit(MainForm->PlcFascia.dbuscite, MainForm->PlcFascia.addr_pallet_depositato, MainForm->PlcFascia.bit_pallet_depositato, true);
                    MainForm->PlcFascia.uscitedascrivere |= MainForm->bit[MainForm->PlcFascia.bit_pallet_depositato];
                    dmDB->LogMsg("SETTO segnale pallet depositato su Fasciatore ");
                } 
                else if (posdep == MainForm->PlcMonta.posprel)  //MONTACARICHI
                {
                    MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_pallet_depositato];
                    dmDB->LogMsg("SETTO segnale pallet depositato su Montacarichi ");
                } 
                else if (posdep == MainForm->PlcRobot.posvuoti)  //Posizione VUOTI
                {
			    	MainForm->PlcRobot.uscitedascrivere[MainForm->PlcRobot.addr_vuoto_depositato] |= MainForm->bit[MainForm->PlcRobot.bit_vuoto_depositato];
                    dmDB->LogMsg("SETTO segnale pallet depositato su Posizione VUOTI ");
                }  */
                
				if ((tipomis  == 0)||(tipomis  == 2)) 
                {
					//dmDB->PrenotaNomePosizione(Dest,0);
                    dmDB->PrenotaPosIntera(posdep,0);
					if (posdep == MainForm->PlcFascia.posdep) 
                    {
						//dmDB->GetSSCCrecordStructPlc(IdUdc, DatiDaScrivere);
						if (IdUdc == "" || IdUdc == NULL)
							DatiDaScrivere.prgfasciatura = programma_fasciatura;
                        else
                        {
                            //dmDB->UpdateUnloadState(IdUdc, DEPOSITATO_AVVOLG);  //depositato su rulliera
                        }
                        
						//TDMgestionePLC->ScriviDatiUdcSuPlc(MainForm->PlcFascia.inizio_indice_dati_sscc_pos_deposito,DatiDaScrivere);
					}
                    if (tipopos == DEF_TYPE && IdUdc != "" && IdUdc != NULL)    //se è una POS di magazzino e ho SSCC
                    {
						dmDB->UpdateStorageState(IdUdc);                 // aggiorno stato depositato in magazzino
                    }
					//dmDB->PrenotaPos(posdep,pianodep , 0, 0) ;
					dmDB->ArticoloPrelevatoDepositato(posdep, MainForm->DatiAgv[nagv].IdUdc,1) ;
					if (dmDB->FilaPosizione(posdep) > 0 ) {
						dmDB->SettaCorsiaInUso(dmDB->FilaPosizione(posdep),0, true) ;
					}
				}
				/*
				if (MainForm->DatiAgv[nagv].IdUdc != "") {
					CP = MainForm->DatiAgv[nagv].StrCPAgv ;
					DataModuleSap->InviaStatoMissioneGenericoASap(HuIdent,"",Dest,CP,HuTyp,"") ;//al deposito metto Source vuoto
				}*/
				/*if ((posdep < 4) &&((tipomis  == 0)||(tipomis  == 2)))
					MainForm->stato_pos_demo[posdep] = 1 ;  */
				MainForm->DatiAgv[nagv].idmis = 0 ;
				MainForm->DatiAgv[nagv].prel = 0 ;
				MainForm->DatiAgv[nagv].dep = 0 ;
				MainForm->DatiAgv[nagv].generata = 0 ;
                MainForm->DatiAgv[nagv].tipomis = -1 ;
				MainForm->DatiAgv[nagv].consensodato = 0 ;
				MainForm->DatiAgv[nagv].annullamissione = 0 ;
				MainForm->DatiAgv[nagv].richiestaconsenso = 0 ;
				MainForm->DatiAgv[nagv].IdUdc = "";
				MainForm->DatiAgv[nagv].tipopallet = 0 ;
				MainForm->DatiAgv[nagv].formato_pallet = 0 ;
				MainForm->DatiAgv[nagv].programma_fasciatura = 0 ;
                MainForm->counterAzzeraUdc = 10;

/*                MainForm->InvioLinea(nagv,0) ;
				MainForm->DisegnaCarrello(id);  */
              break;
            case 'A':   //Missione Abortita
//                MainForm->AbortMission(idmiss);
                dmDB->QueryMissioniSocket->Open();
                if (dmDB->QueryMissioniSocket->RecordCount) {
					prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
					NoPrel = dmDB->QueryMissioniSocket->FieldByName("prelevato")->AsString;
					posdep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger ;
					tipomis = dmDB->QueryMissioniSocket->FieldByName("TipoMissione")->AsInteger;
					IdUdc = dmDB->QueryMissioniSocket->FieldByName("IdUdc")->AsString;
					Source = dmDB->QueryMissioniSocket->FieldByName("PosPrelCliente")->AsString;
					Dest = dmDB->QueryMissioniSocket->FieldByName("PosDepCliente")->AsString;
                    progressivo_wms = dmDB->QueryMissioniSocket->FieldByName("ProgressivoMissione")->AsInteger;
					//baia_dep = dmDB->QueryMissioniSocket->FieldByName("PosDepCliente")->AsInteger;
				}
				dmDB->LogMsg("Missione n° "+ IntToStr(idmiss)+" abortita su agv n° "+ IntToStr(id) + " carico : " + IntToStr(loaded));
				dmDB->QueryMissioniSocket->Close();
				//guardare se ho un'anomalia sull'abort
				//guardo se devo uscire dall'ingombro
				//linea_prel = DMGestioneEventi->RitornaLineaPrelievoDaPos(prel);

				if (NoPrel != "")           // è stato prelevato? SI
                {
					if (progressivo_wms)
                    {
						dmDB->LogMsg("Aggiorno stato missione ad ABORT su linea WMS "+IntToStr(progressivo_wms));
                        dmDB->UpdateStatoCaricamento(progressivo_wms, STATO_ABORT_PREL);
                        dmDB->UpdatePosCarico(Source, "", 0);   //mette UDC = 0  e ID caricamento = 0
                    }
				}
                else if (IdUdc != "" && IdUdc != NULL)    //NO
                {
                    if (progressivo_wms)
                    {
						dmDB->LogMsg("Aggiorno stato missione ad ABORT su linea WMS "+IntToStr(progressivo_wms));
                        dmDB->UpdateStatoCaricamento(progressivo_wms, STATO_ABORT_NOPREL);
                        dmDB->UpdatePosCarico(Source, "", 0);   //mette UDC = 0  e ID caricamento = 0
					}
				}

				if (MainForm->DatiAgv[nagv].annullamissione)
					dmDB->AggiornaMissione(idmiss,5,nagv) ;
				else
					dmDB->AggiornaMissione(idmiss,4,nagv) ;
			   /*	if (NoPrel == "") {
					if (prel == MainForm->DatiPlc[1].pos) {
						MainForm->DatiPlc[1].CodicePallet = "" ;
						MainForm->DatiPlc[1].generata = 0 ;
						MainForm->DatiPlc[1].pos_dep_missione = 0 ;
					}else {
					 //rimuovere prenotazione magazzino
					}
				}   */
				if ((NoPrel == "")&&(tipomis  < 2))
                {
                    dmDB->PrenotaPosIntera(prel,0);
					//dmDB->PrenotaNomePosizione(Source,0);
                }

				if ((tipomis  == 0)||(tipomis  == 2)) 
                {
					//dmDB->PrenotaNomePosizione(Dest,0);
                    dmDB->PrenotaPosIntera(posdep,0);
				}
				/*
				CP = MainForm->DatiAgv[nagv].StrCPAgv ;
				if (NoPrel == "")
					DataModuleSap->InviaStatoMissioneGenericoASap(HuIdent,Source,"",CP,HuTyp,MainForm->ErrorMisAbortitaAgvScaricoNoPrelievo) ;//al prel metto dest vuoto
				else {
					if (MainForm->DatiAgv[nagv].load)
						DataModuleSap->InviaStatoMissioneGenericoASap(HuIdent,"",Dest,CP,HuTyp,MainForm->ErrorMisAbortitaAgvCarico) ;//al prel metto dest vuoto
					else
						DataModuleSap->InviaStatoMissioneGenericoASap(HuIdent,"",Dest,CP,HuTyp,MainForm->ErrorMisAbortitaAgvScaricoDopoPrelievo) ;//al prel metto dest vuoto
				}
				*/
				MainForm->DatiAgv[nagv].generata = 0 ;
				MainForm->DatiAgv[nagv].idmis = 0 ;
				MainForm->DatiAgv[nagv].prel = 0 ;
				MainForm->DatiAgv[nagv].dep = 0 ;
				MainForm->DatiAgv[nagv].tipomis = -1 ;
				MainForm->DatiAgv[nagv].annullamissione = 0 ;
				MainForm->DatiAgv[nagv].richiestaconsenso = 0 ;
				MainForm->DatiAgv[nagv].consensodato = 0 ;
                if (!MainForm->DatiAgv[nagv].load) 
                {
				    MainForm->DatiAgv[nagv].IdUdc = "";
                }
				MainForm->DatiAgv[nagv].tipopallet = 0 ;
				MainForm->DatiAgv[nagv].formato_pallet = 0 ;
				MainForm->DatiAgv[nagv].programma_fasciatura = 0 ;
				// lo tolgo in abort mission se era mis di pesata MainForm->DatiAgv[nagv].missionedipesata = 0 ;
/*                MainForm->InvioLinea(nagv,0) ;
				MainForm->DisegnaCarrello(id);*/
            }
			dmDB->AggiornaStatoAgv(nagv) ;

			//dmComandiAgv->AggiornaPrenotazioni();
            dmDB->CaricaTabelle();
            dmDB->AggiornaMemoriePlc();
			MainForm->aggiornamappa  = 1 ;
			MainForm->DisegnaCarrello(id);
			MainForm->VisualizzaPercorsoAgv(id) ;
		}
    }
//	if ((dmDB->ADOConnection1->Connected)&&(dmDB->lettostatoagv)&&(MainForm->count_inizio > 5)) {
  //      sprintf(answer, "OK,%s", ev);
 //       SendAnswer(answer, Socket);
//        MainForm->AggiornaStatoAgv() ;
 //   }
}



#pragma package(smart_init)

