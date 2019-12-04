//---------------------------------------------------------------------------

#ifndef datamodulecomandiagvH
#define datamodulecomandiagvH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TdmComandiAgv : public TDataModule
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
	__fastcall TdmComandiAgv(TComponent* Owner);
	void Charge(int agv);
	void NoCharge(int agv);
	void AggiornaPrenotazioni() ;
	int PercorsoMissioneLibero(int agv,int prel,int dep) ;
	int PercorsoOccupatoDaAltro(int agv,int destinazione);
	int AgvNelPercorsoAltri(int agv,int dest) ;
	int AgvInDestinazioneAltro(int agv,int altroagv) ;
	int PercorsoMissioneOccupatoDaAltri(int agv,int prel,int dep);
	int TempoMissioneAgvDaEseguire(int agv) ;
	int TempoMissione(int agv,int prel,int dest) ;
	int RicercaAgvVicinoPrelievo(int prel);
	int PercorsoMissione(int agv,int prel,int dest) ;
	int PercorsoPrelievoLibero(int agv,int prel);
	int MandaAgvFuoriIngombro(int agv,int posprel,int posdep) ;
	int NodoPresenteNelPercorso(int agv,int prel,int dep,int nodo) ;
	void AnnullaMissione(int agv);
	void AnnullaRitornoCaricaBatterie(int agv_nuova_missione) ;
	void StandByAgv(int agv,int value);
	int NodoPresenteNelPercorsoAltri(int agv,int nodo) ;
	void InviaHCorsia(int agv,int hcorsia) ;
	int ListaPosFuoriIngombroAgv[50] ;
	int nposfuoriingombro ;
};
//---------------------------------------------------------------------------
extern PACKAGE TdmComandiAgv *dmComandiAgv;
//---------------------------------------------------------------------------
#endif
