#include <stddef.h>
#include <values.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <except.h>
#include <conio.h>
#include <vector>
#include <deque>
#include <math.h>
#include <algorithm>
#include "percorsiprototipo.h"
#include "stlfunct.h"
#include "main.h"
#include "db.h"
#pragma link "MyShape"
// Dati globali dell'applicazione
extern DATILGV tab;
extern int SafetyDist;
extern int SideSafetyDist;
extern int LineSafetyDist;
extern char N_carrelli;
extern int SafetyDist;
extern int Svantaggio;
extern char File_name[50];

int *flag;
unsigned int *L;
int *v;
//----------------------------------------------------------------
/***************  METODI DI  NODI         ***********************/
//----------------------------------------------------------------




// Esegue la prenotazione del nodo
// Se già prenotato dallo stesso carrello ammette ulteriori prenotazioni
// e ritorna il numero di prenotazioni sul nodo
// Se inpossibile prenotare ritorna -1.
// Si occupa anche della prenotazione dei nodi vicini
char NODOLGV::Prenota(char agv, bool vicino, char n_pren) {
	set<int, less<int> >::iterator i;
	bool cant = false;

	// se vicino false stiamo prenotando il nodo, e di seguito prenotiamo
	// anche i nodi vicini
	if (!vicino) {
		if (((!occupato) || (id_agv == agv)) &&
			(!Occupato_Da_Vicino_Da_Altro(agv))) {
			// se la prenotazione da vicino ha esito negativo, cioè
			// vi è un carrello nel nodo vicino allora è necessario
			// annullare anche la prenotazione del nodo corrente
			// allo scopo di evitare scontri
			for (i = vicini.begin(); i != vicini.end(); ++i) {
				if (tab.mappa_nodi[*i].Prenota(agv, true, 1) < 0) {
					cant = true;
					break;
				}
			}
            if (cant) {
				do {
					Libera(agv, true, 1);
					i++;
				} while (i != vicini.begin());
				return -1;
			}
			id_agv = agv;
			occupato++;
			return occupato;
		}
	} else {
		// se vicino è true pronotiamo il nodo perchè vicino ad uno del percorso
		// assegnamo al nodo uno stato di prenotazione pari a quello del nodo del
		// percorso. Lo stato di prenotazione è associato al carrello:
		//    due carrelli possono prenotare come "vicino" lo stesso nodo
		if ((!occupato) || (id_agv == agv)) {
			occupato_da_vicino[agv] += n_pren;
			return n_pren;
		}
	}
	return -1;
}

// Esegue la liberazione del nodo
// Se prenotato più volte dallo stesso carrello ammette la liberazione
// di più prenotazione e ritorna il numero di prenotazioni rimaste
// Se inpossibile liberare ritorna -1
char NODOLGV::Libera(char agv, bool vicino, char n_pren) {
	set<int, less<int> >::iterator i;

	// se vicino è false liberiamo prima il nodo e poi i vicini
	if (!vicino) {
		if ((occupato) || (id_agv == agv)) {
			if (occupato > 0)
				occupato = occupato - 1;
			if (occupato == 0)
				id_agv = 0;
//			cout << "Agv " << (int)agv << ": Liberazione nodo " << id << " ok." << endl;
			for (i = vicini.begin(); i != vicini.end(); ++i)
				tab.mappa_nodi[*i].Libera(agv, true, 1); //occupato);
			return occupato;
		}
	} else {
//		cout << GetCurrentThreadId() << ": Liberazione nodo vicino " << id << " ok." << endl;
		if (occupato_da_vicino[agv] > 0)
			occupato_da_vicino[agv] -= n_pren;
		return n_pren;
	}
//	cout << "Agv " << (int)agv << ": Liberazione nodo " << id << " non ok." << endl;
	return -1;
}


// Se un carrello prenota più di una volta un nodo gli appare come non
// prenotato. Se è già prenotato da un altro carrelo gli appare come prenotato.
char NODOLGV::Stato_occ(char agv) {
	char i, n;
	// conto le prenotazioni fatte sui nodi vicini dagli altri carrelli
	n = 0;
	for (i=1;i<=N_carrelli;i++) {
		if (i!= agv)
			n += Occupato_Da_Vicino(i);
	}
	// le pronotazioni fatte per il carrelo che vuole prenotare non vengono
	// considerate ...
	if (agv == id_agv)
		return 0;
	else
		return (n + occupato);
}

/* Inizializza elenco nodi vicini */
void NODOLGV::Setvicini() {
	TABELLA_NODILGV::iterator j;
	long d, sd, a1, a2;

	for (j = tab.mappa_nodi.begin(); j != tab.mappa_nodi.end(); ++j) {
		if (id != j->second.id) {
			d = tab.Distanza(*this, j->second);
			if (d <= LineSafetyDist) {
				a1 = ang;
				a2 = j->second.ang;
				if ((abs((a1 - a2)) < 1) ||
					(abs(abs(a1 - a2) - 180) < 1)) {
					sd = tab.DistanzaLaterale(*this, j->second);
					if (((!destinazione) &&
						 ((d < LineSafetyDist) && (sd < SideSafetyDist))) ||
						((destinazione) &&
						 ((d < SafetyDist) && (sd < SideSafetyDist)))) {
						// memorizziamo l'id dei nodi vicini in una lista
						if (find(vicini.begin(), vicini.end(), j->second.id) == vicini.end())
							vicini.insert(j->second.id);
                            //modifica 23/08/08 inserisco il vicino anche all'altro nodo
						if (find(tab.mappa_nodi[j->second.id].vicini.begin(), tab.mappa_nodi[j->second.id].vicini.end(), id) == tab.mappa_nodi[j->second.id].vicini.end())
							tab.mappa_nodi[j->second.id].vicini.insert(id);
					}
				} else {
					if (d < SafetyDist) {
						// memorizziamo l'id dei nodi vicini in una lista
						if (find(vicini.begin(), vicini.end(), j->second.id) == vicini.end())
							vicini.insert(j->second.id);
						if (find(tab.mappa_nodi[j->second.id].vicini.begin(), tab.mappa_nodi[j->second.id].vicini.end(), id) == tab.mappa_nodi[j->second.id].vicini.end())
							tab.mappa_nodi[j->second.id].vicini.insert(id);
					}
				}
			}
		}
	}
}
// Ritorna l'id del carrello che occupa il nodo
// Se non è occupato ritorna 0
char NODOLGV::Occupato_Da() {
	return id_agv;
}

char NODOLGV::Occupato_Da_Vicino(char agv) {
	return occupato_da_vicino[agv];
}

char NODOLGV::Occupato_Da_Vicino_Da_Altro(char agv) {
	int i;

	for (i=1; i<=N_carrelli; i++) {
		if ((i!=agv) && occupato_da_vicino[i])
			return true;
	}
	return false;
}
bool NODOLGV::Vicino(int id_test) {
	set<int, less<int> >::iterator j;

	if (id_test == id)
		return true;
	return (vicini.find(id_test) != vicini.end());
}

int RAMOLGV::Peso(char agv) {
//	int stato_occ;

//	stato_occ = tab.mappa_nodi[dest].Stato_occ(agv);
	return peso ;
}
char NODOLGV::LiberaTutto(int agv) {
	int n;
	set<int, less<int> >::iterator i;
    AnsiString stringa ;
	occupato = 0;
//	cout << "Liberazione nodo " << id << " ok." << endl;
    if ((tab.mappa_nodi.find(id) == tab.mappa_nodi.end())||(agv <=0) ||(agv > N_carrelli)) {
        stringa ="Nodo  "+IntToStr(id)+" non presente nel crs!!! oppure agv non corretto ("+IntToStr(agv)+")" ;
		dmDB->LogMsg(stringa.c_str());
	}else {
        occupato_da_vicino[agv] = 0;
        for (i = vicini.begin(); i != vicini.end(); ++i) {
            if (tab.mappa_nodi.find(*i) != tab.mappa_nodi.end())
                tab.mappa_nodi[*i].occupato_da_vicino[agv] = 0;
            else {
                stringa ="Nodo Vicino "+IntToStr(*i)+" non presente nel crs!!!" ;
				dmDB->LogMsg(stringa.c_str());
            }
        }
    }
	id_agv = 0;
	return 0;
}


//----------------------------------------------------------------
/**********************  METODI DI DATI *************************/
//----------------------------------------------------------------

/* Carica intera struttura percorsi da file formato IML */
void DATILGV::Load1(char *name) {
	int i, p, n, d,t,m,b,dir,num,ntab;
	long x, y;
	float r, a;
		int fo,ft,mut,fork,dist,plc,cons,carat,bitplc,curs;
        char Att_plc[50],Bit_plc[50] ;
	FILE *f;
	char s[80],tabella[12];
	RAMOLGV rm;
	NODOLGV nd;
    TMyShape *shapenodo ;
	// Apro file f percorsi generati da editor percorsi IML
	f = fopen(name,"r");
	if (f == NULL) {
	   //	cout << "Could not find file '" << name << "'. Bye ...";
		//exit(-1);
		return ;
	}
    initdijkstra = 0 ;
	// Leggo tutte le linee dei nodi identificate dalla string 'Ep'
	fseek(f, 0, SEEK_SET);
	while (!feof(f)) {
		fscanf(f, "%s", s);
		if (!strcmp(s,"Ep")) {
			// Identificata la string 'Ep' leggo i dati seguenti ossia
			// l' ID del nodo, le coordinate, se destinazione o meno
			carat=fscanf(f, "%d%ld%ld%f%f%s%d%d%d%d", &n, &x, &y, &a, &r, s, &fo, &ft, &mut,&cons);
			// Inserisco nodo in tabella
			nd.id = n ;
			nd.posx = x ;
			nd.posy = y ;
			nd.ang = a ;
			nd.movefork = fo ;
			nd.disFTC = ft ;
			nd.mut = mut ;
			if (!strcmp(s,"Y"))
				nd.destinazione = 1 ;
			else
				nd.destinazione = 0 ;
            if ((carat < 10 ) ||(cons > 1))
            	nd.consenso = 0 ;
            else
            	nd.consenso = cons ;
            nd.TabCarico.clear();
            nd.TabScarico.clear();
            nd.vicini.clear();
            nd.preccalcolato = 0 ;
			mappa_nodi[n] = nd ;
			mappa_nodi[n].occupato = 0;
			mappa_nodi[n].id_agv = 0;
            mappa_nodi[n].nodocreato = 0 ;
			for (i = 0; i <= N_carrelli; i++) {
				mappa_nodi[n].occupato_da_vicino.push_back(0);
			}
            //inizializzo vettore tabelle carico/scarico con possibilita' di inserimento 5 tabelle
		}
        else
		    if (!strcmp(s,"Nv")) {
			   fscanf(f, "%d", &x);
               //mappa_nodi[n].vicini.push_back(x);
               mappa_nodi[n].vicini.insert(x);
            }
        else
		    if (!strcmp(s,"Ld")) {
			   fscanf(f, "%s",&s);
			   mappa_nodi[n].TabCarico.push_back(s);
            }
        else
		    if (!strcmp(s,"Ul")) {
			   fscanf(f, "%s",&s);
			   //mappa_nodi[n].TabScarico[ntab] = s;
			   mappa_nodi[n].TabScarico.push_back(s);
			}
	}
	// Dopo aver creato tutti i nodi leggo le linee dei rami identificate da 'Vb'
	fseek(f, 0, SEEK_SET);
	while (!feof(f)) {
		// Mi posiziono sulla linea Ep
		fscanf(f, "%s", s);
		if (!strcmp(s,"Ep")) {
			fscanf(f, "%ld", &n);
			s[0] = 0;
			// Leggo tutte le linee Vb successive all'Ep individuato
			// fino al successivo
			while ((!feof(f)) && (strcmp(s,"Ep"))) {
				fscanf(f, "%s", s);
				if (!strcmp(s,"Vb")) {
					// Identificata la string 'Vb' leggo i dati seguenti ossia
					// l' ID del nodo destinazione e il peso del ramo
// vecchio					carat = fscanf(f, "%d%d%d%d%d%d%d%d%d%d", &i, &d, &b, &t, &m, &p, &dir, &fork,&dist,&plc);
					carat = fscanf(f, "%d%d%s%d%d%d%d%d", &i, &d, tabella, &p, &dir, &fork, &dist, &plc);
					// Inserisco il ramo nella tabella
					rm.dest = d ;
					rm.peso = p ;
					rm.lung = dist ;
					strcpy(rm.tab,tabella);
                    rm.bank= 1 ;
					rm.mod = 0;
					rm.direz = dir;
                    rm.ok_forche = fork;
                    rm.ricdir = 1 ;
                    rm.attesa_plc = plc;
                    if (plc == 1) {
                    	curs = ftell(f);
						fscanf(f, "%s%d", &Att_plc,&bitplc);
                        if ((!strcmp(Att_plc,"Ep"))||(!strcmp(Att_plc,"Vb"))||
                            (!strcmp(Att_plc,"Nv"))||(!strcmp(Att_plc,"Ld"))||
                            (!strcmp(Att_plc,"Ul"))) {
                    		rm.attesa_plc = 0;
                    		rm.string_plc = "" ;
                            fseek(f, curs, SEEK_SET);
                        }else
                    		rm.string_plc = Att_plc ;
                        if (bitplc == 1) {
                    		curs = ftell(f);
                            fscanf(f, "%s", &Bit_plc);
                            if ((!strcmp(Bit_plc,"Ep"))||(!strcmp(Bit_plc,"Vb"))||
                            	(!strcmp(Bit_plc,"Nv"))||(!strcmp(Bit_plc,"Ld"))||
                                (!strcmp(Bit_plc,"Ul"))) {
                                rm.bit = 0;
                                rm.string_bit = "" ;
								fseek(f, curs, SEEK_SET);
                            }else{
                                rm.bit = bitplc;
                                rm.string_bit = Bit_plc ;
                            }
                        }else{
                    		rm.bit = 0;
                            rm.string_bit = "" ;
                        }
                    }else {
                    	rm.attesa_plc = 0;
                    	rm.string_plc = "" ;
						fscanf(f, "%d",&bitplc);
                        if (bitplc == 1) {
                            curs = ftell(f);
                            fscanf(f, "%s", &Bit_plc);
                            if ((!strcmp(Bit_plc,"Ep"))||(!strcmp(Bit_plc,"Vb"))||
                            	(!strcmp(Bit_plc,"Nv"))||(!strcmp(Bit_plc,"Ld"))||
                                (!strcmp(Bit_plc,"Ul"))) {
                                //non c'è la stringa relativa al bit
                                rm.bit = 0;
                                rm.string_bit = "" ;
								fseek(f, curs, SEEK_SET);
                            }else{
                                rm.bit = bitplc;
                                rm.string_bit = Bit_plc ;
                            }
                        }else{
                    		rm.bit = 0;
                            rm.string_bit = "" ;
                        }
                    }
					mappa_nodi[n].lst_rami.insert(rm);
				} else if (!strcmp(s,"Ep")) {
					fseek(f, -2, SEEK_CUR);
				}
			}
		}
	}
	fclose(f);
}

/* Calcolo della distanza tra due nodi */
double DATILGV::Distanza(NODOLGV a, NODOLGV b) {
	long double n1, n2;
	n1 = a.posx - b.posx;
	n2 = a.posy - b.posy;
	return sqrtl((n1*n1)+(n2*n2));
}


/* Inizializza elenco nodi vicini */
void DATILGV::Setvicini() {
	TABELLA_NODILGV::iterator i;
	set<int, less<int> >::iterator k;
	FILE* f;
	AnsiString filename;
	char s[200];
	int t1, t2, r, n, q, nv, j;
	bool calcola = true;

    if (N_carrelli <= 1)// se ho solo un agv non importa calcolare i nodi vicini
        return ;
	filename = File_name;
	filename.Delete(filename.Pos("."), filename.Length());
	filename += ".nv";
	t1 = FileAge(File_name);
	if (FileExists(filename)) {
		try {
			f = fopen(filename.c_str(), "r");
			fscanf(f, "%d", &t2);
			if (f && (t1 == t2)) {
                while (!feof(f)) {
                    r = fscanf(f, "%d%d", &n, &q);
                    if (r == 2) {
                        for( j = 0; j < q; j++) {
                            r = fscanf(f, "%d", &nv);
                            if (r == 1) {
                                //modifica 23/08/08 inserisco i nodi vicini se esiste il nodo e metto come nodo vicino il viceversa
                                if  ((mappa_nodi.find(n) != mappa_nodi.end())&&
                                    (mappa_nodi.find(nv) != mappa_nodi.end())) {
                                    if (find(mappa_nodi[n].vicini.begin(),mappa_nodi[n].vicini.end(),nv) == mappa_nodi[n].vicini.end()) {
                                        mappa_nodi[n].vicini.insert(nv);
                                    }
                                    if (find(mappa_nodi[nv].vicini.begin(),mappa_nodi[nv].vicini.end(),n) == mappa_nodi[nv].vicini.end()) {   //aggiunta, mod 23/08/08 contollava se n era un nodo vicino di n e inseriva tutti lo stesso
                                        mappa_nodi[nv].vicini.insert(n);
                                    }
                                }
                            }
                        }
                    }
                }
                fclose(f);
                calcola = false;
            }
		} catch (...) {}
	}
	if (calcola) {
		f = fopen(filename.c_str(), "w+");
		fprintf(f, "%d ", t1);
		for (i = mappa_nodi.begin(); i != mappa_nodi.end(); ++i) {
			i->second.Setvicini();
			if (!i->second.vicini.empty()) {
				n = i->second.id;
				q = i->second.vicini.size();
				fprintf(f, "%d %d ", n, q);
				for (k = i->second.vicini.begin();
					 k != i->second.vicini.end();
					 ++k) {
					fprintf(f, "%d ", *k);
				}
				fprintf(f, "\n");
			}
		}
		fclose(f);
        Setvicini() ;   //richiamo la set vicini perchè così metto i nodi vicini di un nodo anche all'altro, altrimenti quando compilo i percorsi non lo fa
	}
}


// Metodi che implementano l'algoritmo di Dijkstra
int DATILGV::Dijkstra(int sorg, int *pred, char agv) {
	// pred[j] è l'elemento precedente a j nel cammino ottimo
	// a partire da sorg
	// flag[j] struttura utilizzata dall'alg.
	// L[j] costo minimo per raggiungere J dalla sorgente

	int j, k, h, min, p, peso, N;
	N = (*(mappa_nodi.rbegin())).second.id + 1;

	TABELLA_NODILGV::iterator i;
	LISTA_RAMILGV::iterator r;
	vector<int> flag(N);
	vector<int> L(N);

	// inizializzazione
	for(j=1; j<N; j++) {
		flag[j] = 0;
		pred[j] = -1;
		L[j] = MAXINT;
	}
/*    if (!initdijkstra) {
        for (r = mappa_nodi[sorg].lst_rami.begin(); r != mappa_nodi[sorg].lst_rami.end(); ++r) {
            j = (*r).dest;
            p = (*r).Peso(agv);
            pred[j] = sorg;
            L[j] = p;
        }
	}else{  */
        for (r = mappa_nodi[sorg].lst_rami.begin(); r != mappa_nodi[sorg].lst_rami.end(); r++) {
            j = r->dest;
            p = r->peso;
            pred[j] = sorg;
            L[j] = p;
        }
 //   }
	flag[sorg] = 1;
	L[sorg] = 0;
	h = sorg;
	for(k=1; k < mappa_nodi.size() - 1; k++) {
		min=MAXINT;
		for(j=1; j<N; j++) {
			if ((flag[j]==0) && (L[j]<min)) {
				min=L[j];
				h=j;
			}
		}
		flag[h]=1;
		for (r = mappa_nodi[h].lst_rami.begin(); r != mappa_nodi[h].lst_rami.end(); ++r) {
			j = r->dest;
            p = r->peso;
        	peso = (L[h] + p);
			if ( (flag[j] == 0) && (peso < L[j]) ) {
				L[j] = L[h] + p;
				pred[j] = h;
			}
		}
/*
old, uso il calcolo dell'agvx con i percorsi salvati su file altrimenti ad ogni calcolo perdeva tempo e si impallava il supervisore
j = (*r).dest;
			p = (*r).Peso(agv);
			if (L[h] == MAXINT)
				peso = MAXINT;
			else
				peso = (L[h] + p);
			if ( (flag[j] == 0) && (peso < L[j]) ) {
				L[j] = L[h] + p;
				pred[j] = h;
			}
		}    */
	}
	return 0;
}

void DATILGV::InitDijkstra() {
	TABELLA_NODILGV::iterator i;
	int n, t1, t2, bytes = sizeof(int) * (mappa_nodi.rbegin()->second.id + 1);
	FILE* f;
	AnsiString filename;
	bool calcola = true;

	filename = File_name;
	filename.Delete(filename.Pos("."), filename.Length());
	filename += ".dij";
	t1 = FileAge(File_name);
	if (FileExists(filename)) {
		try {
			f = fopen(filename.c_str(), "rb");
			fread(&t2, sizeof(int), 1, f);
			if (f ) {
				i = mappa_nodi.begin();
				while (i != mappa_nodi.end()) {
//					fread(&n, sizeof(int), 1, f);
					i->second.precedenti = (int *)malloc(bytes);
					//i->second.precedentiinit = (int *)malloc(bytes);
					fread(i->second.precedenti, sizeof(int), (mappa_nodi.rbegin()->second.id + 1), f);
                    //i->second.precedentiinit = i->second.precedenti ;
                    i->second.preccalcolato = 1 ;
					i++;
				}
				fclose(f);
				calcola = false;
			}
		} catch (...) {}
	}
}
int DATILGV::Percorso(int sorg, int dest, deque<int> &result, char agv) {
	int i, j, peso, N, bytes ;
	RAMOLGV rm ;
    FILE *f;
    if ((sorg == 0) || (dest == 0))
		return -1;

    if ((mappa_nodi.find(sorg) == mappa_nodi.end())||(mappa_nodi.find(dest) == mappa_nodi.end())) {
		dmDB->LogMsg("Calcolo percorso su posizioni non esistenti, Sorg : "+IntToStr(sorg)+ " , dest : "+IntToStr(dest)) ;
        return -1 ;
    }
	if (sorg == dest)
		return 0;

   //	N = (*(mappa_nodi.rbegin())).second.id + 1;
	//vector<int> v(N);
   // int *v =(int *)malloc(bytes);;
	//  Calcolo vettore dei precedenti con algoritmo di Dijkstra
	AnsiString filename;
/*    if(!initdijkstra ) {
	    TABELLA_NODILGV::iterator k;
        filename = File_name;
        filename.Delete(filename.Pos("."), filename.Length());
        filename += ".dij";
        if (FileExists(filename)) {
            try {
                f = fopen(filename.c_str(), "rb");
                if (f != NULL) {
                    k = mappa_nodi.begin();
                    while (k != mappa_nodi.end()) {
    //					fread(&n, sizeof(int), 1, f);
                        k->second.precedenti = (int *)malloc(bytes);
                        //i->second.precedentiinit = (int *)malloc(bytes);
                        fread(k->second.precedenti, sizeof(int), (mappa_nodi.rbegin()->second.id + 1), f);
                        //i->second.precedentiinit = i->second.precedenti ;
                        k++;
                    }
                    initdijkstra = 1 ;
                    fclose(f);
                }
            } catch (...) {}
        }
    }
    if(!initdijkstra)
    */
    if (mappa_nodi[sorg].preccalcolato != 1){
        bytes = sizeof(int) * (mappa_nodi.rbegin()->second.id + 1);
        mappa_nodi[sorg].precedenti = (int *)malloc(bytes);
        Dijkstra(sorg,  mappa_nodi[sorg].precedenti, agv); // ricalcolo percorso in base alla posizione degli altri agv
        mappa_nodi[sorg].preccalcolato = 1 ;
    }/*else {
        mappa_nodi[sorg].precedenti = (int *)malloc(bytes);
        Dijkstra(sorg,  mappa_nodi[sorg].precedenti, agv); // ricalcolo percorso in base alla posizione degli altri agv
	} */
    //Dijkstra(sorg, v, agv);
	// Scorro a ritroso il vettore dei precedenti dalla destinazione
	// all'origine, memorizzando i nodi del percorso trovato
	i = dest;
	peso = 0;
    try {
        while (i != sorg) {
            //j = v[i];
            j = mappa_nodi[sorg].precedenti[i];


            if (j<=0)
                return -1;
            result.push_front(i);
            rm.dest = i ;
            peso += mappa_nodi[j].lst_rami.find(rm)->peso;
            i = j;
        }
    }catch(...){
        dmDB->LogMsg("Eccezione su calcolo percorso : Sorg : "+IntToStr(sorg)+ " , dest : "+IntToStr(dest));
    }
/*	while (i != sorg) {
		//j = v[i];
        j = mappa_nodi[sorg].precedenti[i] ;

		if (j<=0)
			return -1;
		result.push_front(i);
		peso += mappa_nodi[j].lst_rami[i].peso;
		i = j;
	}     */
//	result.insert(result.begin(), sorg);
	return peso;
}

void DATILGV::Empty() {
	TABELLA_NODILGV::iterator i;
	LISTA_RAMILGV lst;

	for (i = mappa_nodi.begin(); i != mappa_nodi.end(); ++i) {
		lst = (*i).second.lst_rami;
		lst.erase(lst.begin(),lst.end());
	}
	mappa_nodi.erase(mappa_nodi.begin(),mappa_nodi.end());
}

/* Visualizza grafo */
ostream& operator << (ostream& os, TABELLA_NODILGV& rhs) {
	TABELLA_NODILGV::iterator i;
	LISTA_RAMILGV::iterator j;
	LISTA_RAMILGV lst;

	os << "Numero nodi:" << rhs.size() << endl;
	for (i = rhs.begin(); i != rhs.end(); ++i) {
		os << (*i).second << " > ";
		lst = (*i).second.lst_rami;
		for (j = lst.begin(); j != lst.end(); ++j) {
			os << *j << ", ";
		}
		os << " " << (*i).second.occupato_da_vicino << endl;
	}
	return(os);
}

long double DATILGV::DistanzaLaterale(NODOLGV p1, NODOLGV p2) {
	long double a, b, c, d, e, f, r, v, result;

	a = p1.posx;
	b = p1.posy;
	c = p2.posx;
	d = p2.posy;
	e = c + 1000 * cos(rad(p2.ang));
	f = d + 1000 * sin(rad(p2.ang));
	r = e - c;
	v = d - f;
	try {
		result = abs(r*(b-d)+v*(a-c))/sqrtl(v*v+r*r);
	} catch(...) {
		result = -1;
	}
	return result;
}

