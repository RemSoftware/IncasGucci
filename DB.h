// ---------------------------------------------------------------------------

#ifndef DBH
#define DBH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <map>
#include <vector>

#include "main.h"

// ---------------------------------------------------------------------------
#define PALLET80x80 1
#define PALLET120x80 2
#define PALLET120x100 3
#define PALLET120x120 4
#define PALLETUNICO	 10   //postazione valida per tutti i tipi di pallet
#define LUNGHEZZA_TELEGRAMMA	 270	//lunghezza fissa tlelegramma
#define NCAMPI_TELEGRAMMA	29                //imposto 1 il campo handshake per scriverlo sul 26
// ---------------------------------------------------------------------------
typedef struct {
	int posprel;
    int posdep;
	int h_prel;
    int h_dep;
	int Agv;
	int tipo_mis;
	int tab_carico;
	int tab_scarico;
	int priorita;
	int tipo_pallet;
	int formato_pallet ;
    int progressivo_missione;
    int programma_fasciatura;
	AnsiString IdUdc;
	AnsiString Source;
	AnsiString Dest;
} TMissione;




// ---------------------------------------------------------------------------
typedef std::map<int, std::map<AnsiString, AnsiString> >TRecordList;
// ---------------------------------------------------------------------------
typedef std::map<AnsiString, std::map<AnsiString, AnsiString> >TIndexList;
typedef std::map<AnsiString, std::map<AnsiString, AnsiString> >::iterator IndexListIterator;

// ---------------------------------------------------------------------------
class TdmDB : public TDataModule {
__published: // IDE-managed Components

    TADOConnection *ADOConnection1;
    TTimer *TimerPuliziaStorici;
    TTimer *TimerConnect;
    TADOQuery *QueryMissioniSocket;
    TADOTable *AlarmTable;
    TADOTable *MissTable;
    TADOQuery *QueryNewAlarm;
    TADOQuery *QueryPos;
    TADOQuery *QueryMappa;
    TADOQuery *QueryCorsie;
	TADOQuery *QueryTipologie;
    TADOConnection *FMConnection;

    void __fastcall TimerPuliziaStoriciTimer(TObject *Sender);
    void __fastcall TimerConnectTimer(TObject *Sender);
    void __fastcall ADOConnection1AfterConnect(TObject *Sender);
    void __fastcall ADOConnection1ConnectComplete(TADOConnection *Connection, Error * const Error, TEventStatus &EventStatus);
    void __fastcall DataModuleDestroy(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TdmDB(TComponent* Owner);
    void PuliziaStorici();
    void ConnettiDB();
	void DisconnettiDB();
    void ConnettiFmDB(); 
    void DisconnettiFmDB(); 
    
    int verificaConnessioneDB();
    AnsiString AggiungiRimuoviSecondiAOrario(AnsiString ora, int secondi, bool rimuovi);
    AnsiString AggiungiRimuoviSecondiADataOra(AnsiString ora, int secondi, bool rimuovi);
    void CaricaTabella(AnsiString TableName, TRecordList &RecList);
    void CaricaTabellaK(AnsiString TableName, AnsiString KeyField, TIndexList &RecList);
    int AggiornaCampoTabella(AnsiString Tabella, AnsiString CampoChiave, int ValoreChiave, AnsiString CampoModifica, AnsiString ValoreModifica, bool isstring);
    int LeggiCampoTabella(AnsiString Tabella, AnsiString CampoDaLeggere, AnsiString ClauslaWhere, AnsiString &Valore);
    void Segnalazione(int n, AnsiString msg, int all, int popup = 0, int popupstation = 0);
    void LeggiSegnalazioneAttiva(AnsiString &msg, int &all);
    void AcquisisciSegnalazioni();
    void AcquisisciSegnalazioneAttiva(int nmsg);
    void AcquisisciSegnalazioneAttiva();
    void Log(AnsiString Sorgente, AnsiString Tipo, AnsiString Evento);
    void LogCom(AnsiString Telegramma);
    void Log_MISS(int agv, int posizione, int destinazione, AnsiString comando, AnsiString posbloccate);
    int AdattatoreRichiesto(AnsiString TipoMacchina, int ID_Macchina, int ID_TipoPallet);
    void LogMsg(AnsiString Descrizione);
    void LeggiParametri(int baia = 0);
    void LeggiParametriGrafici();
    void LeggiPulsanti(); // add
    void AggiornaPulsanti(int puls); // add
    void LeggiStatoAgv();
    void AggiornaStatoAgv(int agv);
    void GestioneAllarmi(int agv);
    //int AggiornaParametri(int baia);
    int AggiornaParametri(int id_parametro, int val, AnsiString valstr = "");

    int EsisteGiaMissioneVersoSella(AnsiString sella, AnsiString coil);
    int CreaIdMissione();
    int GeneraMissione(TMissione m);
    int GeneraMissioneDemo(int pos_prel, int h_prel, int pos_dep, int h_dep, int tipo_mis, int agv);
    int AggiornaMissione(int id, int stato, int agv);
    int AggiornaDestinazioneMissione(int id, int pos_dep);
    void InserisciCorsiePostazioniDatabase();
	void ModificaCorsiePostazioniDatabase();
    int NumFilePerCorsia(int corsia, AnsiString nomecorsia);
    int NumAlveoliPerCorsia(int corsia, AnsiString nomecorsia);
    int PosInizioCorsia(int corsia, int fila, int &posopposta);
    int ReturnFilaDaPos(int pos);
    int ReturnPrimaPosDaFila(int fila);
    void SvuotaCorsia(int corsia);
    void ImpostaCodiceArticoloCorsia(int corsia, AnsiString Codice);
    void SvuotaPosizione(AnsiString nomepos);
    void PrenotaCorsia(int corsia, int prenota);
    void PrenotaPos(int pos, int piano, int prenota, int corsiadaprenotare);
    void SettaCorsiaPiena(int corsia, int setta);
    AnsiString ReturnCodiceDaIdArticolo(int IdArticolo, AnsiString &Lotto, int &h_pallet);
    int CreaIdArticolo();
    void ModificaCodiceArticoloUdc(int IdArticolo, AnsiString Codice);
    int RicercaDeposito(int tipopallet, TMissione &m, AnsiString corsiaimpostata = "");
	void ArticoloPrelevatoDepositato(int pos, AnsiString idArticolo, int presenza);
    void SettaCorsiaInUso(int corsia, int prelievo, int setta);
    void AbilitaCorsia(int corsia, int setta);
    int UdcPresenteInArchivio(int udc);
    void CreaModificaUdc(int udc, AnsiString Codice, AnsiString Lotto, int h_pal);
    void AbortMission(int idmiss);
    int CodicePresenteInArchivio(AnsiString CodiceArticolo);
    AnsiString ReturnZonaDaPos(int pos,int &ordinamento);
    AnsiString ReturnTimeDaDateTime(AnsiString DataOra);
    AnsiString ReturnTimeMission(int idmis);
    int RitornaTipoPos(int pos);
    AnsiString RiempiTipologiaCorsia(TComboBox * tcbCombo, int actual); 
    void AggiornaTabParametri();

    int ReturnFilaDaNomeCorsia(AnsiString NomeCorsia);
    int ReturnNextFilaDaPos(int pos, AnsiString zona);
    int ReturnPrevFilaDaPos(int pos, AnsiString zona);
    int AggiornaCampoUdc(int udc, int valore);
    AnsiString ReturnTipoPalletStr(int tipopallet);
    int RitornaProgrammaFasciaturadaUdc(int udc);
    int InviaRichiestaStampaEtichettaUdc(int udc);
    void CaricaTabelle();
    int FilaPosizione(int pos);
    AnsiString NomeCorsiaDaPosizione(int pos);
    AnsiString CustomerPosDaPosizione(int pos);
    int CorsiaPrimaPosInAlto(int fila);
    int QuotaForcheCorsiaPerPiano(int fila, int piano);
    int RitornaUdcRimosso();
    int UpdateUdcRimosso(int stato_record, int codice_udc);
    int TrovaUdcInPosizione(AnsiString CodiceUdc, int &corsia, int &piano);
    int RitornaTipoPalletdaUdc(int udc);
    AnsiString CustomerPosDaPosizioneEPiano(int pos, int piano);
    int UpdateDestinazioneUdc(int codice_udc, AnsiString Locazione);
    AnsiString RitornaDestinazioneDaUdc(int udc);
	AnsiString RitornaOrarioDaSecondi(int secondi) ;

    int ScriviSQLCmd(AnsiString SQLcomd, AnsiString FunzioneChiamante);
	int ReturnIdPortaDaStringaConsensoPorta(AnsiString str_consenso) ;
	int StatoPorta(int porta) ;
	AnsiString PosizioniRichiestaAperturaPorta(int porta) ;
	AnsiString PosizioniPortaAperta(int porta)  ;
	AnsiString StrConsensoPorta(int porta) ;
	void AggiornaTabPorte() ;
	void AggiornaStatoPorteStruttura() ;
	AnsiString DaBufferAStringa(unsigned char *data, int lunghezza_str) ;
	int RitornaPosInputPlc(int pos) ;
	int ReturnPosDaPosCliente(AnsiString Poscliente) ;
	int RitornaAltezzaForchePosizione(int pos ,int &hdep) ;
	void AggiornaMemoriePlc() ;
	void RicercaNuovaMissioneDaGenerare() ;
	void MissioneElaborataSuAgvX(int idmiss,int causale = 0) ;
	int CompilaDatiPosizione(AnsiString nomepos,int prelievo,int formato_pallet,int &h_forche,int &tab_car_scar) ;
	int AggiornaMissioneWms(int progressivo, int stato, int agv) ;
	int IdTipoVassoio(int val) ;
	int IdTipoLarghezzaUdc(int val) ;
	AnsiString ReturnNomePosDaPosEFormato(int pos,int formato_pallet) ;
	int ReturnIdMissioneDaProgressivo(int progressivo) ;
	void PrenotaNomePosizione(AnsiString nomepos,int prenota) ;
	int ReturnFilaDaPosCliente(AnsiString Poscliente) ;
	int GetLGVrecord(AnsiString SSCC, TLGV &lgv);
    int GetUDCdapos(int nPos);
    int GetLastLGVrecordFromCAR(AnsiString sName, TLGV &lgv);
	int DaBufferAByte(unsigned char *data);
	int DaBufferAWord(unsigned char *data);
	int DaBufferADoppiaWord(unsigned char *data);
	int GetSSCCrecordStructPlc(AnsiString SSCC, TDatiSSCCPlc &Struct) ;
    void UpdatePresenzaVuotiFalde(int type, int act); 
    int RicercaDepositoTipologiaUDC(int tipologia, int &opposta); 
    int GetAbilitataIfCorsia(int corsia); 
    void  UpdateCorsielist();
    void FullTabellaK(AnsiString TableName, AnsiString KeyField, TIndexList &RecList); 
    int RicercaPrelievoUdcinCorsia(int tipologia, int tipologia2, int &priorita, int &dafasciare); 
    AnsiString GetTipologiaCorsiaString(int corsia); 
    int GetTipologiaCorsia(int fila);
    int RitornaIdUdcPosizione(int pos); 
    int RitornaPos1000x1200(int pos);
    int ReturnPrioritaMagDaPos(int pos); 
    void UpdateUnloadState(AnsiString SSCC, int state); 
    void UpdateWrapState(AnsiString SSCC); 
    void UpdatePrintedState(AnsiString SSCC,int printed_state);
    void UpdateStorageState(AnsiString SSCC); 
	int ContaPalletInCorsia(int fila);
	void SettaCorsiaVuota(int corsia, int setta);
    int CreaIdUDC();
    int UpdateSoloStato(int pos, int prenota, int escludi);
    int UpdatePosCarico(AnsiString PosCli, AnsiString tudc, int idcar);
    int InsertCaricamento(int linea, int udc, int pos); 
    int UpdateStatoCaricamento(int ID, int stato);
    AnsiString GetDataCaricamento(int nLinea);
    int RitornaPresenza(int pos);
    void ModificaStatoUdc(int IDUDC, int Stato); 
    int GetStatoUdc(int IDUDC); 
    int AggiornaHDepHPrelPerPos(int pos, int h1, int h2, bool solo1);
    int GetPalletMagazzino(int tipo); 
    int GetVuotiInMagazzino(); 
    int PosizioneDaCustomerPos(AnsiString customerpos);
    int AggiornaESvuotaPosDeposito();
    void PrenotaPosIntera(int pos,int prenota); 
    AnsiString AlmenoUnUdcInPos(int pos); 


    TIndexList m_TabCorsie, TabParametri;
	TDateTime t;
	TIndexList TabPostazioni,TabPorte, TabCustomerPos;
	TIndexList TabCorsie;
	TRecordList TabTipoVassoi,TabTipoLarghezzaUdc ;
    int escludi_bertolotti, peso_min, diametro_min, escludi_missioni, log_comunicazioni, lettostatoagv, lettoparametri;
    int tempo_prelievo, tempo_deposito, ultimo_id_articolo;
    bool aggiornamentolista;
    AnsiString ultima_data_priorita,Lingua;
};

// ---------------------------------------------------------------------------
extern PACKAGE TdmDB *dmDB;
extern AnsiString logstr;
// ---------------------------------------------------------------------------
#endif
