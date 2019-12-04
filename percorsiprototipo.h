#pragma hdrstop
#ifndef _CLASSI_HLGV
#define _CLASSI_HLGV

#include <iostream.h>
#include <vector>
#include <set>
#include <map>
#include <vcl.h>
#include <deque>
#include "MyShape.h"
#define pg              3.14159265359
#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a>b)?a:b)
#define rad(deg)        ((deg)*pg/180)
#define deg(rad)        ((rad)*180/pg)
using namespace std;


//----------------------------------------------------------------
//  Un ramo e' un tratto di percorso che connette due nodi.
//  Poiche' da ogni nodo si possono raggiungere diversi punti,
//  i rami sono memorizzati in liste.
//  Ogni nodo possiede una lista di rami. Ogni ramo fornisce
//  l'id del nodo che raggiunge.
//----------------------------------------------------------------
/*class RAMOLGV {
	public:
//		RAMO(int n = 0, int p = 0): dest(n), peso(p) {}
		void operator = (const RAMOLGV& rhs)
			{ dest = rhs.dest; peso = rhs.peso; }
		bool operator == (const RAMOLGV& rhs) const
			{ return (dest == rhs.dest); }
		bool operator != (const RAMOLGV& rhs) const
			{ return !(*this == rhs); }
		bool operator < (const RAMOLGV& rhs) const
			{ return (dest < rhs.dest); }
		int Peso(char agv);
	public:
		char tab[12];
        int bank;
        int mod;
        int direz;
        bool ok_forche;
        int ricdir;			// Identificativo del comando da eseguire da parte del carrello
		int dest;			// Identificativo del nodo raggiunto dal ramo
		int peso;			// Indice del tempo impiegato da un AGV per percorrere
		int lung;			// il tratto di percorso
        bool attesa_plc ;
        bool bit;
        AnsiString string_bit ;
        AnsiString string_plc ;
};

//----------------------------------------------------------------
// La 'lista_rami' è un oggetto che mantiene la lista dei rami di
// un nodo. (vedi definizione di nodo successiva)
//----------------------------------------------------------------
typedef map<int, RAMOLGV, less<int> > LISTA_RAMILGV;

*/

class RAMOLGV {
	public:
		RAMOLGV(){}
		void operator = (const RAMOLGV& rhs)
			{ dest = rhs.dest; peso = rhs.peso;strcpy(tab,rhs.tab); bank = rhs.bank;
              mod = rhs.mod; direz = rhs.direz;ok_forche = rhs.ok_forche; lung = rhs.lung;
              attesa_plc = rhs.attesa_plc; bit = rhs.bit;}
		bool operator == (const RAMOLGV& rhs) const
			{ return (dest == rhs.dest); }
		bool operator != (const RAMOLGV& rhs) const
			{ return !(*this == rhs); }
		bool operator < (const RAMOLGV& rhs) const
			{ return (dest < rhs.dest); }
		friend ostream & operator << (ostream &out, const RAMOLGV &w)
			{ return out << w.dest <<  "-" << "(" << w.peso << ")";}
		int Peso(char agv);
	public:
		char tab[12];
        int bank;
        int mod;
        int direz;
        bool ok_forche;
        int ricdir;			// Identificativo del comando da eseguire da parte del carrello
		int dest;			// Identificativo del nodo raggiunto dal ramo
		int peso;			// Indice del tempo impiegato da un AGV per percorrere
		int lung;			// il tratto di percorso
        bool attesa_plc ;
        bool bit;
        AnsiString string_bit ;
        AnsiString string_plc ;
};

//----------------------------------------------------------------
// La 'lista_rami' è un oggetto che mantiene la lista dei rami di
// un nodo. (vedi definizione di nodo successiva)
//----------------------------------------------------------------
typedef set<RAMOLGV, less<RAMOLGV> > LISTA_RAMILGV;


//----------------------------------------------------------------
//  Il nodo e' una postazione di percorso. Ogni nodo ha un
//  ha in 'lst_rami' la lista dei rami che lo connettono ad
//  altri nodi.
//----------------------------------------------------------------
class NODOLGV  {
	public:

//NODO::NODO(int n, long x, long y, float a, char d, bool fork, bool ftc, char occ, char agv):

		NODOLGV(){}
		void operator = (const NODOLGV& rhs)
			{ id = rhs.id; posx = rhs.posx; posy = rhs.posy; ang = rhs.ang;
			  destinazione = rhs.destinazione; movefork=rhs.movefork;
              disFTC = rhs.disFTC; mut = rhs.mut; consenso = rhs.consenso; }
		bool operator == (const NODOLGV& rhs) const
			{ return (id == rhs.id); }
		bool operator != (const NODOLGV& rhs) const
			{ return !(*this == rhs); }
		bool operator < (const NODOLGV& rhs) const
			{ return (id < rhs.id); }
		friend ostream & operator << (ostream &out, const NODOLGV &w)
			{ return out << w.id << "(" << w.posx << "," << w.posy << "," << w.ang << ")"
			  << " {Occupato: " << (char)(w.id_agv + '0') << "}";}
		char Prenota(char agv, bool vicino, char n_pren);
		char Libera(char agv, bool vicino, char n_pren);
        char LiberaTutto(int agv) ;
		char Stato_occ(char agv);
		void Setvicini();
		char Occupato_Da();
		char Occupato_Da_Vicino(char agv);
		char Occupato_Da_Vicino_Da_Altro(char agv);
		bool Vicino(int id_test);
	public:
		int id;				// Numero identificativo della postazione
		long posx,			// Posizione X della postazione in mm
			posy;			// Posizione Y della postazione in mm
		float ang;			// Orientamento del carrello sul nodo
                bool movefork;
                bool disFTC;
                bool mut;
		bool destinazione;
        int preccalcolato ;
        int nodocreato ; 
        int consenso ;  //attesa di un consenso da terra (PLC)
        		// Indica se la postazione e' un punto di arrivo per
							// gli AGV oppure un punto di transito
		char	occupato;       	// Indica se la postazione e' occupata dall'AGV 'id_agv'
							// Lo stesso AGV può eseguire più prenotazioni
							// occupato contiene il numero totale di prenotazioni
		char	id_agv;       		// AGV che ha eseguito la prenotazione
		vector<char> occupato_da_vicino;
					// Vettore che contiene il numero di prenotazioni eseguite
					// da ciascun carrello per il passaggio in un nodo vicino
		LISTA_RAMILGV lst_rami;	// Lista dei rami che partono da questo nodo
		set<int, less<int> > vicini;	// Lista dei nodi vicini (distanza < x) vicini;
//		vector<int> vicini;	// Lista dei nodi vicini (distanza < x) vicini;
		vector<AnsiString> TabScarico;
        vector<AnsiString> TabCarico;
		int *precedenti;
};

typedef	map<int, NODOLGV, less<int> > TABELLA_NODILGV;

//----------------------------------------------------------------
//  La 'tabella_nodi' è la super-super-classe con tutte le classi come
//  proprietà
//----------------------------------------------------------------
class DATILGV {
	public:
		DATILGV() {};
		TABELLA_NODILGV mappa_nodi;
		void Load1(char *name);			// carica la struttura mappa_nodi con i dati provenienti da un file
		void Setvicini();				// compila la lista dei nodi vicini nei singoli nodi
		double Distanza(NODOLGV a, NODOLGV b);	// distanza (peso) tra due nodi
		int Dijkstra(int sorg, int *pred, char agv);	// calocola il vettore dei precedenti
		int Percorso(int sorg, int dest, deque<int> &res, char agv);	// determina il percorso ottimo
		long double DistanzaLaterale(NODOLGV a, NODOLGV b);    // distanza tra due nodi
        void InitDijkstra() ;
		void Empty();
        int initdijkstra ;
};

ostream& operator << (ostream& os, TABELLA_NODILGV& rhs);

#endif
