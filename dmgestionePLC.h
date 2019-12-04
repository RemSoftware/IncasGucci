//---------------------------------------------------------------------------

#ifndef dmgestionePLCH
#define dmgestionePLCH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "main.h"
#define NCAMPI 11
//---------------------------------------------------------------------------
class TTDMgestionePLC : public TDataModule
{
__published:	// IDE-managed Components
    TTimer *TimerEventi;
    TTimer *TimerPlc;
    TTimer *TimerPhoenix;
    TTimer *TimerAllarmi;
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall TimerPlcTimer(TObject *Sender);
    void __fastcall TimerEventiTimer(TObject *Sender);
    void __fastcall TimerPhoenixTimer(TObject *Sender);
    void __fastcall TimerAllarmiTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TTDMgestionePLC(TComponent* Owner);
    void ControlloIngressoFasciatore(); 
    int AgvInDestinazionePrelievoFasciatore();
    int AgvInDestinazioneDepositoFasciatore();
    int AgvInDestinazioneDepositoVuoti(); 
    int AgvInDestinazioneDepositoMonta(); 
	void ScriviDatiUdcSuPlc(int indice_inizio_dati, TDatiSSCCPlc DatiDaScrivere);
    void ScriviSSCCSuPlcNavetta( AnsiString SSCC );
	void ReturnDatiSSCCDaPlc(int indice_inizio_dati, TDatiSSCCPlc &DatiScrittisuPlc);
	void ControlloSemaforo();
    void ControlloIncrocioAGV1();
    //void GetArrivalTime(int nLinea, int input, int bit);
    //void InitStatoLinee();
    int AgvInPosLuceSemaforoRossa();
    int PosInIngombroAGV1(int pos);
    
	bool init;
    int id_lampeggio_sync;
    bool sync_lamp;
};
//---------------------------------------------------------------------------
extern PACKAGE TTDMgestionePLC *TDMgestionePLC;
//---------------------------------------------------------------------------
#endif
