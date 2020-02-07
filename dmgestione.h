// ---------------------------------------------------------------------------

#ifndef dmgestioneH
#define dmgestioneH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TDMGestioneEventi : public TDataModule
{
__published: // IDE-managed Components

    TTimer *TimerEventi;
    TTimer *TimerGestioneLuci;
    TTimer *TimerMissioni;
    TTimer *TimerPlc;
    TTimer *TimerPhoenix;
    TTimer *TimerMissioniDB;

    void __fastcall TimerEventiTimer(TObject *Sender);
    void __fastcall TimerGestioneLuciTimer(TObject *Sender);
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall TimerMissioniTimer(TObject *Sender);
    void __fastcall TimerPlcTimer(TObject *Sender);
    void __fastcall TimerPhoenixTimer(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TDMGestioneEventi(TComponent* Owner);
    void ControllaMPScarti();
    int AgvInPosCb(int pos);
    int AgvInDestCb(int agv);
    void PalletPrelevatoDepositatoSuPressa(int pos, int prelevato);
    int AgvInUscitaPrelievoFasciatore();
    int GetMissioniWMS(int &nID, int &nPosDep, AnsiString &sIDudc, TDateTime &dtInizio, AnsiString &custPosPrel, AnsiString &custPosDep, int &tipoPallet); 
    int GetDestinazioneFromRobot(int posprel, int linea, TDateTime dtInizio); 
	int RitornaLineaPrelievoDaPos(int pos) ;
	int AgvInIngombroVuoti(int agv) ;
	int AgvInIngombroFaldeDx(int agv) ;
	int AgvInIngombroFaldeSx(int agv) ;
    void SettaAllarmeLineaPerMagazzino(int pos);
    void ResettaAllarme(int pos);
    void ResettaAllAllarmiLinea();
    int GetDestinazioneFromAvvolgitore(int posprel); 
    bool SuperataAttesaAvvolgitore();
    bool SuperataAttesaMontacarichi();
    bool SuperataAttesaRobot(TDateTime dtInizio); 


	int PosIngombroVuoti[5],PosIngombroFaldeDx[5],PosIngombroFaldeSx[5];
    int id_lampeggio_sync,count_missione_nuova_ricevuta,missione_nuova_ricevuta;
    bool sync_lamp, init, attiva_schermata_codice;
};

// ---------------------------------------------------------------------------
extern PACKAGE TDMGestioneEventi *DMGestioneEventi;
// ---------------------------------------------------------------------------
#endif
