//---------------------------------------------------------------------------


#pragma hdrstop

#include "datamodulecomandiagv.h"
#include "main.h"
#include "percorsiprototipo.h"
#include "socket.h"
#include "db.h"
#include "socket.h"
#include "dmgestione.h"
#include <algorithm>
extern CLIENTSOCKET *cs ;
extern CLIENTSOCKET *sock ;
extern char N_carrelli ;
extern DATILGV tab;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TdmComandiAgv *dmComandiAgv;
//---------------------------------------------------------------------------
__fastcall TdmComandiAgv::TdmComandiAgv(TComponent* Owner)
	: TDataModule(Owner)
{
int i ;
	for(i= 0;i< 50 ;i++)
		ListaPosFuoriIngombroAgv[i] = 0 ;
	ListaPosFuoriIngombroAgv[1] = 100;
	ListaPosFuoriIngombroAgv[2] = 200;
	ListaPosFuoriIngombroAgv[3] = 25;
	ListaPosFuoriIngombroAgv[4] = 32;
	ListaPosFuoriIngombroAgv[5] = 304;
	ListaPosFuoriIngombroAgv[6] = 309;
	ListaPosFuoriIngombroAgv[7] = 1010;
	ListaPosFuoriIngombroAgv[8] = 1011;
	ListaPosFuoriIngombroAgv[9] = 77;
	ListaPosFuoriIngombroAgv[10] = 91;
	ListaPosFuoriIngombroAgv[11] = 62;
	ListaPosFuoriIngombroAgv[12] = 67;
	ListaPosFuoriIngombroAgv[13] = 92;
	ListaPosFuoriIngombroAgv[14] = 54;
	ListaPosFuoriIngombroAgv[15] = 1051;
	ListaPosFuoriIngombroAgv[16] = 129;
	ListaPosFuoriIngombroAgv[17] = 1121;
	ListaPosFuoriIngombroAgv[18] = 1123;
	ListaPosFuoriIngombroAgv[19] = 141;
	ListaPosFuoriIngombroAgv[20] = 145;
	ListaPosFuoriIngombroAgv[21] = 1132;
	ListaPosFuoriIngombroAgv[22] = 1131;
	nposfuoriingombro = 22 ;
}
//---------------------------------------------------------------------------
void TdmComandiAgv::Charge(int agv){
	Evento ev ;
	try {
		sprintf(ev, "%c,%d,%c",'C',agv,'C');
		sock->SendEv(ev);
		MainForm->DatiAgv[agv].incarica = 1 ;
		dmDB->LogMsg("Alzata uscita carica batterie su agv "+IntToStr(agv));
	}catch(...){
		dmDB->LogMsg("Eccezione sul comando charge agv "+IntToStr(agv));
	}
//    DatiAgv[1].incarica = true ;

}
void TdmComandiAgv::NoCharge(int agv){
	Evento ev ;
	try {
		sprintf(ev, "%c,%d,%c",'C',agv,'N');
		sock->SendEv(ev);
		MainForm->DatiAgv[agv].incarica = 0 ;
		dmDB->LogMsg("Abbassata uscita carica batterie su agv");
	}catch(...){
		dmDB->LogMsg("Eccezione sul comando nocharge agv "+IntToStr(agv));
	}
}

void TdmComandiAgv::AggiornaPrenotazioni() {
int j,agv,peso,nodoocc ;
deque<int> ris ;
deque<int>::iterator d_i ;
ris.clear();
TABELLA_NODILGV::iterator i;
	for (i = tab.mappa_nodi.begin(); i != tab.mappa_nodi.end(); ++i) {
		//scorro l'array dei nodi e resetto tutte le prenotazioni cos' le riaggiorno in base ai comando di ogni agv
		i->second.occupato = 0 ;
		i->second.id_agv = 0 ;
		/*
		for(j=1;j<=N_carrelli;j++)
			i->second.occupato_da_vicino[j] = 0 ;   */
		for(j=1;j<=N_carrelli;j++)
			i->second.LiberaTutto(j) ;
	}
	for(agv=1;agv<=N_carrelli;agv++) {
		if (tab.mappa_nodi.find(MainForm->DatiAgv[agv].pos) != tab.mappa_nodi.end()) {
			tab.mappa_nodi[MainForm->DatiAgv[agv].pos].Prenota(agv,false, 1) ; //prenoto la posizione
			if (((MainForm->DatiAgv[agv].generata)||(MainForm->DatiAgv[agv].semiautomatico))&&(MainForm->DatiAgv[agv].dest)&&(MainForm->DatiAgv[agv].dest!=MainForm->DatiAgv[agv].pos)) {
				peso = tab.Percorso( MainForm->DatiAgv[agv].pos,MainForm->DatiAgv[agv].dest,ris,1) ;
				if (peso > 0) {
					for(d_i= ris.begin(); d_i!= ris.end() ; d_i++) {
						//scorro il percorso e segno il nodo come prenotato da questo agv
						nodoocc = tab.mappa_nodi[*d_i].Prenota(agv,false, 1) ;
						if (nodoocc < 0) {
							//sono arrivato all'altro agv, vedere se devo continuare la prenotazione oppure no
							break ;
						}
					}
                }
			}
		}
    }
}

int TdmComandiAgv::PercorsoMissioneLibero(int agv,int prel,int dep) {
	double x,y,x1,y1;

    int possuc,Peso,Pesoaltro,altroagv;
	deque<int> ris ;
	deque<int> risaltro ;
    // inserisco all'interno dell'agv il percorso che deve fare
    possuc = 0 ;
    deque<int>::iterator i ;
    deque<int>::iterator i_altro ;
    ris.clear() ;
    risaltro.clear() ;
    if (agv == 1)
        altroagv = 2 ;
    else
        altroagv = 1 ;
    if (MainForm->DatiAgv[altroagv].pos == 0)
        return 0 ;
    if ((MainForm->DatiAgv[altroagv].generata)||(MainForm->DatiAgv[altroagv].semiautomatico)||(MainForm->DatiAgv[altroagv].bitbattbassa)) {
        if ((MainForm->DatiAgv[altroagv].dest)&&(MainForm->DatiAgv[altroagv].pos)&&(MainForm->DatiAgv[altroagv].pos != MainForm->DatiAgv[altroagv].dest)) {
            Pesoaltro = tab.Percorso( MainForm->DatiAgv[altroagv].pos,MainForm->DatiAgv[altroagv].dest,risaltro,1); // sto andando al pelievo o al deposito
        }else if ((MainForm->DatiAgv[altroagv].prel)&&(MainForm->DatiAgv[altroagv].dep)&&(MainForm->DatiAgv[altroagv].prel != MainForm->DatiAgv[altroagv].dep)
            &&(MainForm->DatiAgv[altroagv].pos == MainForm->DatiAgv[altroagv].prel)) {//sono in posizione prelievo e ho una missione normale, aggiungo percorso verso dep
            Pesoaltro = tab.Percorso( MainForm->DatiAgv[altroagv].pos,MainForm->DatiAgv[altroagv].dep,risaltro,1);
        }
    }
    //PaintBox1->Canvas->Brush->Style = bsDiagCross ;
    if(( MainForm->DatiAgv[agv].pos)&&(prel)&&(dep)) {
        Peso = tab.Percorso( MainForm->DatiAgv[agv].pos,prel,ris,1);
        if (Peso >= 0) {
            if (Peso > 0) {
                for(i = ris.begin(); i != ris.end(); i++ ) {
                    if ((MainForm->DatiAgv[altroagv].pos == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].pos))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].pos].Vicino(*i))) {
                        possuc = *i ;
                        break ;
                    }else if (( risaltro.size() >0)&&(Pesoaltro > 0)) {
                        for(i_altro = risaltro.begin(); i_altro != risaltro.end(); i_altro++ ) {
                            if ((*i_altro == *i)||(tab.mappa_nodi[*i].Vicino(*i_altro))||(tab.mappa_nodi[*i_altro].Vicino(*i))) {
                                possuc = *i ;
                                break ;
                            }
                        }
                    }
                }
            }
            ris.clear();
            Peso = tab.Percorso( prel,dep,ris,1);
            if (Peso > 0) {
                for(i = ris.begin(); i != ris.end(); i++ ) {
                    if ((MainForm->DatiAgv[altroagv].pos == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].pos))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].pos].Vicino(*i))) {
                        possuc = *i ;
                        break ;
                    }else if (( risaltro.size() >0)&&(Pesoaltro > 0)){
                        for(i_altro = risaltro.begin(); i_altro != risaltro.end(); i_altro++ ) {
                            if ((*i_altro == *i)||(tab.mappa_nodi[*i].Vicino(*i_altro))||(tab.mappa_nodi[*i_altro].Vicino(*i))) {
								possuc = *i ;
								break ;
                            }
						}
                    }
                }
            } else if (Peso == -1)
                possuc = -1 ;
        } else if (Peso == -1)
            possuc = -1 ;
    } else
        possuc = -1 ;
    return possuc ;
}

int TdmComandiAgv::PercorsoOccupatoDaAltro(int agv,int destinazione){
int peso,peso2,incrocio2,ok,nodofuoriingombro,nessuningombro;
deque<int> ris,ris2 ;
deque<int>::iterator d_i,d_i2,d_incrocio ;
int posxagv,posxaltroagv,destxagv,destxaltroagv,nagv;
ris.clear();
ris2.clear();
    ok = 0 ;
	if ((tab.mappa_nodi.find(MainForm->DatiAgv[agv].pos) != tab.mappa_nodi.end())&&
        (tab.mappa_nodi.find(destinazione) != tab.mappa_nodi.end())) {
		peso = tab.Percorso( MainForm->DatiAgv[agv].pos,destinazione,ris,1) ;
        if (peso == 0) {
            //sono in posizione, ritorno ok
            return 0 ;
		}else if (peso > 0) {
            incrocio2 = 0 ;
			for(d_i= ris.begin(); d_i!= ris.end() ; d_i++) {
				//scorro il percorso e segno il nodo come prenotato da questo agv
				if (((tab.mappa_nodi[*d_i].occupato) && (tab.mappa_nodi[*d_i].id_agv != agv)) ||
					(tab.mappa_nodi[*d_i].Occupato_Da_Vicino_Da_Altro(agv))) {
					//percorso impegnato dall'altro, guardo se arrivo alla posizione dell'altro
	// tolto se sta arrivando non va bene                if (( *d_i == MainForm->DatiAgv[altroagv].pos) ||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].pos].Vicino(*d_i))||(tab.mappa_nodi[*d_i].Vicino(MainForm->DatiAgv[altroagv].pos))) {
						//incrocio l'altro. Se l'altro sta andando via e la destinazione non mi incrocia posso partire
					nagv = tab.mappa_nodi[*d_i].id_agv ;
					if ((MainForm->DatiAgv[nagv].generata) &&(MainForm->DatiAgv[nagv].dest)&&
						(tab.mappa_nodi.find(destinazione) != tab.mappa_nodi.end())&&
						(tab.mappa_nodi.find(MainForm->DatiAgv[nagv].dest) != tab.mappa_nodi.end())
						&&(MainForm->DatiAgv[nagv].dest != MainForm->DatiAgv[nagv].pos)) {
						return nagv ;

					}else {
						//vado in incrocio con l'altro, controllo se questo agv si può spostare fuori ingombro rispetto la destinazione dell'altro agv e nel caso ritorno che l'altro può fare la missione
						if ((!MainForm->DatiAgv[nagv].generata) &&(!MainForm->DatiAgv[nagv].manuale)&&(!MainForm->DatiAgv[nagv].semiautomatico)&&(!MainForm->DatiAgv[nagv].alarm)&&(MainForm->DatiAgv[nagv].pos > 0)) {  //solo  se sono vuoto oppure ho dei pallet vuoti lo sposto, altrimenti
							//mi incrocio con un agv, devo provare a mandarlo fuori ingombro visto che non ha missioni

						}else
							return nagv ;
					}


					if (incrocio2) {
						//l'agv che incrocio sta scappando quindi posso assegnare la missione
						break ;
					}
				}
			}
		}else
			return -1 ;
	}
	if (incrocio2)
		ok = agv ; //ritorno l'agv che sto guardando
	return ok ;
	//DisegnaMappaNodi() ;
}
int TdmComandiAgv::AgvNelPercorsoAltri(int agv,int dest) {
	double x,y,x1,y1;

	int possuc,Peso,altroagv;
	deque<int> ris ;

	possuc = 0 ;
	deque<int>::iterator i ;
    ris.clear() ;
    //PaintBox1->Canvas->Brush->Style = bsDiagCross ;
	for(altroagv = 1 ; altroagv <= N_carrelli ; altroagv++) {
		if ((altroagv != agv)&&(!possuc)) {
			if (dest == 0)
				dest = MainForm->DatiAgv[altroagv].dest ;
			if (dest > 0) {
				Peso = tab.Percorso( MainForm->DatiAgv[altroagv].pos,dest,ris,1);
				if (Peso > 0) {
					for(i = ris.begin(); i != ris.end(); i++ ) {
						if ((MainForm->DatiAgv[agv].pos == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[agv].pos))||(tab.mappa_nodi[MainForm->DatiAgv[agv].pos].Vicino(*i))) {
							possuc = altroagv ; //ritorno l'agv che passa dove so o con questo
							break ;
						}
					}
				}
			}
		}
	}
	return possuc ;
}
int TdmComandiAgv::AgvInDestinazioneAltro(int agv,int altroagv) {
	double x,y,x1,y1;

    int possuc,Peso;
	deque<int> ris ;
    possuc = 0 ;
    deque<int>::iterator i ;
    ris.clear() ;
    //PaintBox1->Canvas->Brush->Style = bsDiagCross ;
    if ((MainForm->DatiAgv[agv].prel > 0) &&(MainForm->DatiAgv[agv].dep > 0 )) {
        Peso = tab.Percorso( MainForm->DatiAgv[agv].prel,MainForm->DatiAgv[agv].dep,ris,1);
        if (Peso > 0) {
            for(i = ris.begin(); i != ris.end(); i++ ) {
                if ((MainForm->DatiAgv[altroagv].pos == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].pos))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].pos].Vicino(*i))) {
                    possuc = *i ;
                    break ;
                }
                if ((MainForm->DatiAgv[altroagv].pos != MainForm->DatiAgv[altroagv].dest) &&(MainForm->DatiAgv[altroagv].dest > 0)&&((MainForm->DatiAgv[altroagv].dest == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].dest))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].dest].Vicino(*i)))) {
                    possuc = *i ;
                    break ;
                }
                if ((MainForm->DatiAgv[altroagv].dep > 0)&&((MainForm->DatiAgv[altroagv].dep == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].dep))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].dep].Vicino(*i)))) {
                    possuc = *i ;
                    break ;
                }
            }
        }
    }else {
       // non controllo
    }
    return possuc ;
}
int TdmComandiAgv::PercorsoMissioneOccupatoDaAltri(int agv,int prel,int dep ) {
	double x,y,x1,y1;

	int possuc,Peso,posattuale,altroagv;
	deque<int> ris ;

    bool primoincrocio ;
    possuc = 0 ;
    deque<int>::iterator i ;
	ris.clear() ;
    //PaintBox1->Canvas->Brush->Style = bsDiagCross ;
    primoincrocio = false ;
    if ((prel >0 ) &&(dep > 0)) {
        Peso = tab.Percorso( prel,dep,ris,1);
        //se l'altro ha già prelevato potrei controllare se ad arrivare a destinazione libero il percorso di questa missione
        //fermo il ciclo se l'agv devo ancora acaricare altrimenti controllo se da quando trova l'altro agv continuando lo libera, se lo libera fino alla fine significa che sta andando in una posizione fuori dall'ingombro dell'altro e che l'altro può ansare
        if (Peso > 0) {
			for(altroagv = 1 ; altroagv <= N_carrelli ; altroagv++) {
				if (altroagv != agv) {
					for(i = ris.begin(); i != ris.end(); i++ ) {
						if ((MainForm->DatiAgv[altroagv].pos == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].pos))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].pos].Vicino(*i))) {
							if ((!MainForm->DatiAgv[altroagv].generata)||(MainForm->DatiAgv[altroagv].manuale)) {
							//se arrivo in una posizone dove è presente l'altro controllo che non stia facendo una missione di movimento per spostarsi( caso di missioni di prelievo fuori con agv che è dentro e fa prima a portar fuori un pallet che andare a prelevare fuori ma seglie quella perche avrebbe il persorco libero
							// se sta facendo una missione di movimento e la destinazione risulta nel percorso di questo lo controlla con gli if dopo
								//se è in missione posso controllare se dalla sua posizione alla destinazione non incrocia questo agv
								if (AgvNelPercorsoAltri(altroagv,MainForm->DatiAgv[altroagv].dest)) {
									possuc =altroagv ;
									break ;
								}
							}
						}
						if ((MainForm->DatiAgv[altroagv].pos != MainForm->DatiAgv[altroagv].dest) &&(MainForm->DatiAgv[altroagv].dest > 0)&&((MainForm->DatiAgv[altroagv].dest == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].dest))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].dest].Vicino(*i)))) {
							possuc = altroagv ;
							break ;
						}
						if ((MainForm->DatiAgv[altroagv].dep > 0)&&((MainForm->DatiAgv[altroagv].dep == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].dep))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].dep].Vicino(*i)))) {
							possuc = altroagv ;
							break ;
						}
					}
				}
			}
		}
	}
	return possuc ;
}
int TdmComandiAgv::TempoMissioneAgvDaEseguire(int agv) {
	double x,y,x1,y1;

	int possuc,Peso = -1,Peso2 = -1,loadtime = 15,unloadtime = 15;
	deque<int> ris ;

	possuc = 0 ;
	deque<int>::iterator i ;
	ris.clear() ;
	//PaintBox1->Canvas->Brush->Style = bsDiagCross ;
    if ((MainForm->DatiAgv[agv].generata)&&(MainForm->DatiAgv[agv].pos)&&(MainForm->DatiAgv[agv].prel)&&(!MainForm->DatiAgv[agv].load)) {
        Peso = tab.Percorso( MainForm->DatiAgv[agv].pos,MainForm->DatiAgv[agv].prel,ris,1);
        if (Peso >= 0)
            Peso +=loadtime ;
    }else
        Peso = 0 ;
    if (Peso >= 0) {
        if ((MainForm->DatiAgv[agv].load)&&(MainForm->DatiAgv[agv].dest > 0))
            Peso2 = tab.Percorso( MainForm->DatiAgv[agv].pos,MainForm->DatiAgv[agv].dest,ris,1);
        else if ((!MainForm->DatiAgv[agv].load)&&(MainForm->DatiAgv[agv].prel > 0)&&(MainForm->DatiAgv[agv].dep > 0))
            Peso2 = tab.Percorso( MainForm->DatiAgv[agv].prel,MainForm->DatiAgv[agv].dep,ris,1);
        if (Peso2 >= 0)
            Peso2 +=unloadtime ;
        if (Peso2 >= 0)
            Peso += Peso2 ;
        else
            Peso = -1 ;
    }
    return Peso ;
}
int TdmComandiAgv::TempoMissione(int agv,int prel,int dest) {
	double x,y,x1,y1;

    int possuc,Peso,Peso2;
	deque<int> ris ;

    possuc = 0 ;
    deque<int>::iterator i ;
    ris.clear() ;
    //PaintBox1->Canvas->Brush->Style = bsDiagCross ;
    if ((MainForm->DatiAgv[agv].pos)&&(prel)) {
        Peso = tab.Percorso( MainForm->DatiAgv[agv].pos,prel,ris,1);
        if ((Peso)&(prel != dest)) {
            Peso2 = tab.Percorso( prel,dest,ris,1);
            if (Peso2)
                Peso += Peso2 ;
            else
                Peso = -1 ;
        }
    }else
        Peso = -1 ;
    return Peso ;
}
int TdmComandiAgv::RicercaAgvVicinoPrelievo(int prel) {
	double x,y,x1,y1;

	int pospartenza,Peso = 100000,agv,agv_trovato = 0,PesoAgv = 0,pesoaltra;
	deque<int> ris ;

	deque<int>::iterator i ;
	ris.clear() ;
	pesoaltra = -1 ;
	//PaintBox1->Canvas->Brush->Style = bsDiagCross ;
	//if ((!MainForm->DatiAgv[1].batteriabassa)&&(!MainForm->DatiAgv[1].manuale)) {
	for (agv = 1 ; agv <= N_carrelli ; agv++) {
		if (((MainForm->DatiAgv[agv].generata)&&(MainForm->DatiAgv[agv].dep)&&(tab.mappa_nodi[prel].Vicino(MainForm->DatiAgv[agv].dep)))||
			((MainForm->DatiAgv[agv].pos != MainForm->DatiAgv[agv].dest)&&(MainForm->DatiAgv[agv].dest)&&(tab.mappa_nodi[prel].Vicino(MainForm->DatiAgv[agv].dest)))) //tolto controllo posizione altrimenti in corsia mi trova come più vicina quella del lato forche opposta
			PesoAgv = 0 ; //ho questo agv che sta andando nella crsia del prelievo o verso quella corsia, assegno a lui
		else if (MainForm->DatiAgv[agv].pos > 0) {
			if ((MainForm->DatiAgv[agv].generata)&&(MainForm->DatiAgv[agv].dep)) {
				pesoaltra = TempoMissioneAgvDaEseguire(agv) ;
				pospartenza = MainForm->DatiAgv[agv].dep ;
			}else if ((MainForm->DatiAgv[agv].pos != MainForm->DatiAgv[agv].dest)&&(MainForm->DatiAgv[agv].dest)) {
				pesoaltra = tab.Percorso( MainForm->DatiAgv[agv].pos,MainForm->DatiAgv[agv].dest,ris,1);
				pospartenza = MainForm->DatiAgv[agv].dest ;
			}else
				pospartenza = MainForm->DatiAgv[agv].pos ;
			PesoAgv = tab.Percorso( pospartenza,prel,ris,1);
			if (pesoaltra > 0)
				Peso += pesoaltra ;
			if ((PesoAgv >= 0)&&(PesoAgv < Peso)) {
				Peso = PesoAgv;
				agv_trovato = agv ;
            }
		}
	}
	return Peso ;
}

int TdmComandiAgv::PercorsoMissione(int agv,int prel,int dest) {
	double x,y,x1,y1;

	int possuc,Peso,Peso2;
	deque<int> ris ;
    // inserisco all'interno dell'agv il percorso che deve fare
    possuc = 0 ;
    deque<int>::iterator i ;
    ris.clear() ;
    Peso = -1 ;
    //PaintBox1->Canvas->Brush->Style = bsDiagCross ;
    if ((MainForm->DatiAgv[agv].pos)&&(prel) &&(dest)) {
        Peso = tab.Percorso( MainForm->DatiAgv[agv].pos,prel,ris,1);
        if (Peso > 0) {
            Peso2 = tab.Percorso( prel,dest,ris,1);
            if (Peso2)
                Peso += Peso2 ;
            else
                Peso = -1 ;
        }
    }
    return Peso ;
}
int TdmComandiAgv::PercorsoPrelievoLibero(int agv,int prel) {
	double x,y,x1,y1;

	int possuc,Peso,altroagv;
	deque<int> ris ;
    // inserisco all'interno dell'agv il percorso che deve fare
    possuc = 0 ;
    deque<int>::iterator i ;
    ris.clear() ;
	//PaintBox1->Canvas->Brush->Style = bsDiagCross ;
    if ((MainForm->DatiAgv[agv].pos)&&(prel)) {
		Peso = tab.Percorso( MainForm->DatiAgv[agv].pos,prel,ris,1);
		if (Peso == 0)
			return agv ;
		if (Peso > 0) {
			for(altroagv = 1 ; altroagv <= N_carrelli ; altroagv++) {
				if (altroagv != agv) {
					for(i = ris.begin(); i != ris.end(); i++ ) {
						if ((MainForm->DatiAgv[altroagv].pos == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].pos))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].pos].Vicino(*i))) {
							possuc = altroagv ;
							break ;
						}
						//guardo anche se sto andando in quela corsia con l'altro
						if ((MainForm->DatiAgv[altroagv].pos != MainForm->DatiAgv[altroagv].dest)&&(MainForm->DatiAgv[altroagv].dest)&&
							((MainForm->DatiAgv[altroagv].dest == *i)||(tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[altroagv].dest))||(tab.mappa_nodi[MainForm->DatiAgv[altroagv].dest].Vicino(*i)))) {
			//                if ((*i != MainForm->DatiAgv[agv].pos)&& (!tab.mappa_nodi[*i].Vicino(MainForm->DatiAgv[agv].pos))&& (!tab.mappa_nodi[MainForm->DatiAgv[agv].pos].Vicino(*i))){  //se la posizione è uguale sono davanti all'altro agv
								possuc = altroagv ;
								break ;
			//                }
						}
					}
				}
			}
		} else if (Peso == -1)
			possuc = -1 ;
    }else
        possuc = -1 ;
    return possuc ;
}
int TdmComandiAgv::MandaAgvFuoriIngombro(int agv,int posprel,int posdep) {
	int i,altroagv = 0 ,peso,pesomis,postrovata;
	deque<int> ris ;
	bool nook ;
	peso = 100000;
    pesomis = 0 ;
	postrovata = 0;

	//controllo se l'agv che devo spostare può taggiungere dei nodi impostati senza interferire con la missione dell'altro
    //ritorno il nodo più vicino
	for(i = 1 ; i<=nposfuoriingombro;i++) {
		if (PercorsoMissioneLibero(agv,MainForm->DatiAgv[agv].pos,ListaPosFuoriIngombroAgv[i])==0) {
			nook = false ;
			for(altroagv =1  ;altroagv <= N_carrelli; altroagv++) {
				if ((altroagv != agv)&&(NodoPresenteNelPercorso(altroagv,posprel,posdep,ListaPosFuoriIngombroAgv[i]))) {
					nook = true ;
					break ;
				}
			}
			if (!nook) {
				pesomis = tab.Percorso( MainForm->DatiAgv[agv].pos,ListaPosFuoriIngombroAgv[i],ris,1) ;
				if ((pesomis > 0 ) && (pesomis < peso)) {
					//controllo se è impostata una modalità diversa dalla prima se il nodo trovato è nella stessa zona dell'agv che farà la missione
					//se è nella stessa zona gli agigungo un 100 al peso in modo da spostare fuyori ingombro in altra zona l'agv
					peso = pesomis ;
					postrovata = ListaPosFuoriIngombroAgv[i] ;
				}
			}
		}
    }
    return postrovata ;
}

int TdmComandiAgv::NodoPresenteNelPercorso(int agv,int prel,int dep,int nodo) {
	double x,y,x1,y1;

    int possuc,Peso,altroagv;
	deque<int> ris ;
    // inserisco all'interno dell'agv il percorso che deve fare
    possuc = 0 ;
    deque<int>::iterator i ;
    ris.clear() ;
	//PaintBox1->Canvas->Brush->Style = bsDiagCross ;
	if(( MainForm->DatiAgv[agv].pos)&&(prel)&&(dep)) {
		Peso = tab.Percorso( MainForm->DatiAgv[agv].pos,prel,ris,1);
		if (Peso >= 0) {
			if (Peso > 0) {
				for(i = ris.begin(); i != ris.end(); i++ ) {
					if ((nodo == *i)||(tab.mappa_nodi[*i].Vicino(nodo))||(tab.mappa_nodi[nodo].Vicino(*i))) {
						possuc = *i ;
						break ;
					}
				}
			}
			Peso = tab.Percorso( prel,dep,ris,1);
			if (Peso > 0) {
				for(i = ris.begin(); i != ris.end(); i++ ) {
					if ((nodo == *i)||(tab.mappa_nodi[*i].Vicino(nodo))||(tab.mappa_nodi[nodo].Vicino(*i))) {
						possuc = *i ;
						break ;
					}
				}
			} else if (Peso == -1)
                possuc = -1 ;
		} else if (Peso == -1)
			possuc = -1 ;
	} else
		possuc = -1 ;
	return possuc ;
}
int TdmComandiAgv::NodoPresenteNelPercorsoAltri(int agv,int nodo) {

	int possuc,Peso,PesoDep,altroagv,prel = 0,dep = 0,debug;
	deque<int> ris ;
    // inserisco all'interno dell'agv il percorso che deve fare
    possuc = 0 ;
    deque<int>::iterator i ;
	ris.clear() ;
	//PaintBox1->Canvas->Brush->Style = bsDiagCross ;
	for (altroagv = 1 ; altroagv <= N_carrelli ; altroagv++) {
		if (altroagv != agv) {
			if (MainForm->DatiAgv[altroagv].pos == nodo)
				return altroagv ;
			if ((MainForm->DatiAgv[altroagv].pos) && (!MainForm->DatiAgv[altroagv].load)&& (MainForm->DatiAgv[altroagv].prel || MainForm->DatiAgv[altroagv].dest)) {
				if ((MainForm->DatiAgv[altroagv].pos != MainForm->DatiAgv[altroagv].prel)&&(MainForm->DatiAgv[altroagv].prel)&&(MainForm->DatiAgv[altroagv].tipomis < 4))
					prel = MainForm->DatiAgv[altroagv].prel ;
				else if ((MainForm->DatiAgv[altroagv].pos != MainForm->DatiAgv[altroagv].dest)&&(MainForm->DatiAgv[altroagv].dest))
					prel = MainForm->DatiAgv[altroagv].dest ;
				if (prel)
					Peso = tab.Percorso( MainForm->DatiAgv[altroagv].pos,prel,ris,1);
				if (Peso > 0) {
					for(i = ris.begin(); i != ris.end(); i++ ) {
						if ((nodo == *i)||(tab.mappa_nodi[*i].Vicino(nodo))||(tab.mappa_nodi[nodo].Vicino(*i))) {
							possuc = altroagv ;
							break ;
						}
					}
				}

			}
			if ((MainForm->DatiAgv[altroagv].pos) && (MainForm->DatiAgv[altroagv].load  || MainForm->DatiAgv[altroagv].dep)) {
				if ((!MainForm->DatiAgv[altroagv].load) &&(MainForm->DatiAgv[altroagv].prel)&&(MainForm->DatiAgv[altroagv].pos != MainForm->DatiAgv[altroagv].prel))
					prel = MainForm->DatiAgv[altroagv].prel ;
				else
					prel = MainForm->DatiAgv[altroagv].pos ;
				if ((!MainForm->DatiAgv[altroagv].dep) &&(MainForm->DatiAgv[altroagv].dest))
					dep = MainForm->DatiAgv[altroagv].dest ;
				else
					dep = MainForm->DatiAgv[altroagv].dep ;
				Peso = 0 ;
				ris.clear() ;
				if (prel && dep)
					Peso = tab.Percorso(prel,dep,ris,1);
				if (Peso > 0) {
					for(i = ris.begin(); i != ris.end(); i++ ) {
						debug = *i ;
						if ((nodo == *i)||(tab.mappa_nodi[*i].Vicino(nodo))||(tab.mappa_nodi[nodo].Vicino(*i))) {
							possuc = *i ;
							break ;
						}
					}
				}
			}
		}
	}
	return possuc ;
}

void TdmComandiAgv::AnnullaMissione(int agv)
{
	//
	Evento ev ;
    AnsiString com;
    try {
//        sprintf(ev, "%c,%d,%c,%d,%d",'C',agv,'Z',20,0);    //alzo uscita cb 20h
            com = "ann_miss";
            sprintf(ev, "%c,%d,%c,%s,%c,%d",'C',agv,'>',com.c_str(),'B',1);    //alzo uscita cb 20h
            sock->SendEv(ev);
			dmDB->LogMsg("Inviato Annulla Missione all agv "+IntToStr(agv) +" in posizione "+IntToStr(MainForm->DatiAgv[agv].pos)+" destinazione "+IntToStr(MainForm->DatiAgv[agv].dest));
			MainForm->DatiAgv[agv].annullamissione = 1 ;
			//DisegnaCarrello(agv) ;
    }catch(...){
		dmDB->LogMsg("Eccezione sul comando invio Annulla Missione agv");
    }
}
void TdmComandiAgv::AnnullaRitornoCaricaBatterie(int agv_nuova_missione)
{
	int agv,agv_fermi_disponibili;
	//PaintBox1->Canvas->Brush->Style = bsDiagCross ;
	//if ((!MainForm->DatiAgv[1].batteriabassa)&&(!MainForm->DatiAgv[1].manuale)) {
	agv_fermi_disponibili = 0 ;
	for (agv = 1 ; agv <= N_carrelli ; agv++) {
		if ((!MainForm->DatiAgv[agv].generata) &&(!MainForm->DatiAgv[agv].annullamissione) &&
			(!MainForm->DatiAgv[agv].manuale)&&(!MainForm->DatiAgv[agv].semiautomatico)&&
			(MainForm->DatiAgv[agv].pos > 0)&&(!DMGestioneEventi->AgvInPosCb(agv)))
			agv_fermi_disponibili++ ;
			// mi segno quanti agv ho siponibili e non in carica per la missione ricevuta perchè rischio di
			//annullare un ritorno al cb e assegnare la missione all'altro e creare un deadlock
	}
	for (agv = 1 ; agv <= N_carrelli ; agv++) {
		if (((agv == agv_nuova_missione)||((agv_nuova_missione == 0)&&(!agv_fermi_disponibili)))&&
			(MainForm->DatiAgv[agv].generata) &&(!MainForm->DatiAgv[agv].annullamissione) &&(!MainForm->DatiAgv[agv].manuale)&&(!MainForm->DatiAgv[agv].semiautomatico)
			&&(!MainForm->DatiAgv[agv].load)&&(DMGestioneEventi->AgvInDestCb(agv))) {
			AnnullaMissione(agv) ;
		}
	}
}
void TdmComandiAgv::StandByAgv(int agv,int value){
	Evento ev ;
    AnsiString var  ;
    try {
//        sprintf(ev, "%c,%d,%c,%d,%d",'C',agv,'Z',20,0);    //alzo uscita cb 20h
        var = "st_by_pc";
        sprintf(ev, "%c,%d,%c,%s,%c,%d",'C',agv,'>',var.c_str(),'B',value);    //alzo uscita cb 20h
        sock->SendEv(ev);
        if (value)
			dmDB->LogMsg("Fermato Agv "+IntToStr(agv)+" con Stand By in pos " +IntToStr(MainForm->DatiAgv[agv].pos));
        else
			dmDB->LogMsg("Ripristinato Agv "+IntToStr(agv)+" da Stand By in pos " +IntToStr(MainForm->DatiAgv[agv].pos));
		MainForm->DatiAgv[agv].stand_by = value ;
    }catch(...){
		dmDB->LogMsg("Eccezione sul comando invio invio stand_by agv");
    }
}
void TdmComandiAgv::InviaHCorsia(int agv,int hcorsia){
	Evento ev ;
    AnsiString var  ;
    try {
//        sprintf(ev, "%c,%d,%c,%d,%d",'C',agv,'Z',20,0);    //alzo uscita cb 20h
		var = "alt_depo";
		sprintf(ev, "%c,%d,%c,%s,%c,%d",'C',agv,'>',var.c_str(),'I',hcorsia);    //alzo uscita cb 20h
		sock->SendEv(ev);
		dmDB->LogMsg("Forzata variabile alt_depo su agv "+IntToStr(agv)+" in posizione "+IntToStr(MainForm->DatiAgv[agv].pos)+" , valore : "+IntToStr(hcorsia));
	}catch(...){
		dmDB->LogMsg("Eccezione sul comando InviaHCorsia");
    }
}

