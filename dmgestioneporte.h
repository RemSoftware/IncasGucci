// ---------------------------------------------------------------------------

#ifndef dmgestioneporteH
#define dmgestioneporteH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ExtCtrls.hpp>

#define ABBASSASEGNALIPORTA		0
#define APRIPORTA		1
#define APRIPORTAMETA	2
#define CHIUDIPORTA		3

// ---------------------------------------------------------------------------
class TDMGestione_Porte : public TDataModule
{
__published: // IDE-managed Components

    TTimer *TimerEventi;
    TTimer *TimerPlcPorte;

    void __fastcall TimerEventiTimer(TObject *Sender);
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall TimerPlcPorteTimer(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TDMGestione_Porte(TComponent* Owner);
    int ControllaPorteAperte();
    int AgvInPosPorta(int porta);
    int AgvInPosAperturaPorta(int porta, int controlla_agv = 0);
    int ConsensoInviatoAdAgvInPosAperturaPorta(int porta, int controlla_agv = 0);
    int ApriChiudiPorta(int porta, int apri);
    void ControllaPorteDaAprire();
    int AggiungiRimuoviRichiestaConsensoPorta(AnsiString strconsenso, int aggiungi);
    int GestioneRotAlarm();
    int AgvInPosRotAlarm(int rotalarm);
    int SettaRotAlarm(int rotalarm, bool setta);
    void ControllaResetSegnaleApriChiudiPorta(int tiporeset);
    void GestioneSegnaliLavatrice();
    void LeggiStrutturaDatiPorte();


    void LogStatoPorta(int porta);
    void ModificaByteStrutturaPorte(int input, int idporta, int byte, int value);
    int AgvInPostazioni_Libere_Antincendio(int agv);

    bool sync_lamp, init;


    int Postazioni_Libere_Antincendio[10];
};

// ---------------------------------------------------------------------------
extern PACKAGE TDMGestione_Porte *DMGestione_Porte;
// ---------------------------------------------------------------------------
#endif
