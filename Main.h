// ----------------------------------------------------------------------------
#ifndef MainH
#define MainH
// ----------------------------------------------------------------------------
#include <System.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Messages.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include <Vcl.Imaging.GIFImg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include "ChildWin.h"
#include "MessageBar.h"
#include <time.h>
#include <Vcl.Imaging.jpeg.hpp>
#include "RotImg.hpp"
#include "MyShape.h"
#include "MyConnector.h"
// #include <System.ImageList.hpp>
#define NativeScreenHeight    800    //aggiornamento automatico definizione layout pagine
#define STRUTTURA_DATI_UDC_PLC	320


// ----------------------------------------------------------------------------

#define DEF_TYPE    	    0       //NIENTE
#define LINEACAR_TYPE	    1
#define LINEASCAR_TYPE	    2
#define AVVOLGITORE_TYPE    3


#define SCARICO1     	    110
#define CARICO3     	    30
#define FASCIATORE     	    1

#define NLINEE_PREL   	    9

#define POS_PREL_FALDE	    5
#define POS_PREL_VUOTI	    6

#define POS_DEP_FALDEDX	    70
#define POS_DEP_FALDESX	    80
#define POS_DEP_VUOTI	    95

//tipologia delle corsie
#define TYPE_UNDEFINED      0
#define TYPE_FASCIARE       1
#define TYPE_NONFASCIARE    2
#define TYPE_INCOMPLETI     3
#define TYPE_VUOTI          4
#define TYPE_SFOGLIATORE    5

#define TYPE_TUTTI          99

//tipologie STATI
#define DEPOSITATO_AVVOLG   101
#define DEPOSITATO_SEMILAV  102
#define DEPOSITATO_VUOTI    105
#define DEPOSITATO_FALDEDX  106
#define DEPOSITATO_FALDESX  107


//tipologie STATI
#define STATO_INSERITA      0
#define STATO_INCARICO      1
#define STATO_INIZIATA      2
#define STATO_PRELEVATA     3
#define STATO_DEPOSITATA    4
#define STATO_ABORTITA      5     //Abort AGV
#define STATO_ABORT_NOPREL  5     //Abort prima del prelievo
#define STATO_ABORT_PREL    6     //Abort dopo il prelievo
#define STATO_ERRATA        10    //Errore nei dati ricevuti
#define STATO_ABORTPC       55    //Abort da PC
#define STATO_CANCELLATA    66    //Cancellata da PC

// ----------------------------------------------------------------------------

typedef struct {
    int ID;
	TDateTime ordine;
    AnsiString lotto;
	AnsiString codArticolo;
    AnsiString etichetta_prodCode;
    AnsiString etichetta_prodEAN;
    AnsiString etichetta_descrizione;
	AnsiString etichetta_imballoEAN;
    AnsiString etichetta_expDate;
	AnsiString SSCC;
    int nScatole;
    int nColliPerScatola;
    int stato;
    int lineaPrelievo;
    int lineaDeposito;
    int priorita;
    int unloadState;
    TDateTime timeUnload;
    int toWrap;
    int wrapState;
    TDateTime timeWrap;
    int toPrint;
    int printContentFormat;
    int printedState;
    TDateTime timePrint;
    int storageState;
    TDateTime timeStorage;
} TLGV;

typedef struct {
	AnsiString lotto;
	AnsiString codArticolo;
    AnsiString etichetta_prodCode;
    AnsiString etichetta_prodEAN;
    AnsiString etichetta_descrizione;
	AnsiString etichetta_imballoEAN;
	AnsiString etichetta_expDate;
	AnsiString SSCC;
	int layoutetichetta;
	int prgfasciatura;
	int numscatole;
} TDatiSSCCPlc;


// ----------------------------------------------------------------------------
class TMainForm : public TForm {
__published:
    TPanel *Panel1;
    TImage *Image3;
    TTimer *TimerPwd;
    TTimer *TimerOra;
    TImageList *ImageList1;
    TImageList *ImageList2;
    TPanel *Panel2;
    TBitBtn *bStatoAGV;
    TBitBtn *bMissioni;
    TBitBtn *bSetup;
    TBitBtn *bStorici;
    TBitBtn *bPwd;
    TBitBtn *bEsci;
    TImage *Image4;
    TImage *Image5;
    TMessageBar *MessageBar1;
    TImage *Image2;
    TScrollBox *ScrollBox1;
    TImage *Image1;
    TPopupMenu *PopupMenuPos;
    TRotateImage *img1;
    TRotateImage *img2;
    TPopupMenu *PopupMenu1;
    TPanel *Panel4;
    TStatusBar *MainStatusBar;
    TPanel *Panel3;
    TSpeedButton *ZoomOutSpeedButton;
    TSpeedButton *ExtendSpeedButton;
    TSpeedButton *ZoomInSpeedButton;
    TPanel *QuotaY;
    TPanel *QuotaX;
    TTimer *TimerSinottico;
    TBitBtn *sSinottico;
    TPanel *PanImage;
    TMyShape *MyShape1;
    TLabel *Label1;
    TMyShape *MyShape2;
    TLabel *Label2;
    TMyShape *MyShape3;
    TLabel *Label3;
    TMyShape *MyShape4;
    TLabel *Label4;
    TGroupBox *gbDBName;
    TLabel *lbDB;
    TLabel *lbAS400;
    TLabel *lbLastUpd;
	TRotateImage *img3;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn5;
    TMyShape *AbAvvolgitore;
    TBitBtn *bAnagrafiche;
    TImageList *ImageListMenu;
    TPopupMenu *PopupMenu;
    TPanel *pPassword;
    TSpeedButton *SpSimulazione;
    TBitBtn *ResetBtn;
    TMyShape *PrenotaVuoti;
    TBitBtn *BitBtn4;
    TMyShape *Priorita1;
    TMyShape *Priorita2;
    TPanel *PanelSemaforo;
    TShape *Verde;
    TShape *Rosso;
    TPanel *PanelIncrocio;
    TShape *VerdeIncrocio;
    TShape *RossoIncrocio;
    TShape *Giallo;

    void __fastcall bEsciClick(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall TimerOraTimer(TObject *Sender);
    void __fastcall TimerPwdTimer(TObject *Sender);
    void __fastcall bPwdClick(TObject *Sender);
    void __fastcall MainStatusBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
    void __fastcall bStoriciClick(TObject *Sender);
    void __fastcall Image4Click(TObject *Sender);
    void __fastcall bStatoAGVClick(TObject *Sender);
    void __fastcall bSetupClick(TObject *Sender);
    void __fastcall bMissioniClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sSinotticoClick(TObject *Sender);
    void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall ShapePosMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall ShapePosMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ShapePulsantieraMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ExcludePosition1Click(TObject *Sender);
    void __fastcall RemoveExclusion1Click(TObject *Sender);
    void __fastcall ZoomInSpeedButtonClick(TObject *Sender);
    void __fastcall ExtendSpeedButtonClick(TObject *Sender);
    void __fastcall ZoomOutSpeedButtonClick(TObject *Sender);
    void __fastcall img2MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled);
    void __fastcall MoveMis1Click(TObject *Sender);
    void __fastcall DropMis1Click(TObject *Sender);
    void __fastcall MoveAgv1Click(TObject *Sender);
    void __fastcall DropAgv1Click(TObject *Sender);
    void __fastcall DropPal1Click(TObject *Sender);
    void __fastcall PrelPal1Click(TObject *Sender);
    void __fastcall StandBy1Click(TObject *Sender);
    void __fastcall Pos1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall TimerSinotticoTimer(TObject *Sender);
    void __fastcall Svuotacorsia1Click(TObject *Sender);
    void __fastcall DisabilitaCorsia1Click(TObject *Sender);
    void __fastcall Abilitacorsia1Click(TObject *Sender);
    void __fastcall sCaricoClick(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BtnUDCOutClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall bAnagraficheMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall UDCClick(TObject * Sender);
    void __fastcall SpSimulazioneClick(TObject *Sender);
    void __fastcall ResetBtnClick(TObject *Sender);
    void __fastcall PanelIncrocioMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall CaricaPalletDaPiano1(TObject * Sender); 
    void __fastcall CaricaPalletDaPiano2(TObject * Sender); 

private:
    template<class T>
    void __fastcall CreateMDIChild(T *form, String Name, String Caption);

public:
    virtual __fastcall TMainForm(TComponent *Owner);
    void __fastcall AfterConstruction(void);
    void SetVisible();
    void CambiaLingua(int lang, bool warning = true);
    void SavePassword();
    void LoadPassword();
    void UpdControls();
    void LeggiParametri();
    void ConvertiStringaDisplay(AnsiString stringa, int indice_inizio_registro, int indice_display);
    void AggiornaMappa();
    void EscludiPosSelezionate(int pos, int escludi);
    void DisegnaCarrello(int car);
    int RicercaImgLayout(int posx, int posy);
    void VisualizzaPercorsoAgv(int nagv = 0);
    int GetPrenotataFromPos(int pos);

    struct Agv {
        int pos;
        int idmis;
        int dest;
        int posprec;
        int LivelloBatteria;
        int load;
        int manuale;
        int alarm;
        int incarica;
        int bitbattbassa;
        int semiautomatico;
        char allarme[100];
        char allarmi[100];
        char allarmiPrec[100];
        int registro5;
        int stand_by;
        int bumper_bypass;
        int errorecb;
        int generata;
        int tipomis;
        int numpallet;
        int alarmconsenso;
        int nodoa, nodob, richiestaconsenso;
        int annullamissione;
        int prel, dep, consensodato;
        int num_alarm_consenso;
		int primo_ciclo_consenso;
		int tipopallet;
		int formato_pallet ;
		int programma_fasciatura ;
		int IdUdc;
        int stato;
		AnsiString IdUdcTest;
		AnsiString DataOraFineCarica, imgfile, imgfile_agv;
        AnsiString DataOraAttesa, strcons,NomePosPrel,NomePosDep;
    };


	struct Fasciatore 
    {
		int posdep;
		int h_dep;

		int PlcConnesso;
		AnsiString IdUdc,IdUdcdascrivere;
		int generata; 
		int escludi; 

		int dbInput;
		int dwByteInput;
		int StatoInput[3];

		int dbuscite;
		int dwuscite;
		int uscite;
		int uscitedascrivere;
		//int usciteetichetta;
		//int usciteetichettadascrivere;

        //IN
		int ChiamataDeposito;
        int addr_ChiamataDeposito;
        int bit_ChiamataDeposito;
        
		int OKingresso;
        int addr_OKingresso;
        int bit_OKingresso;

		int PalletAvvolto;
        int addr_PalletAvvolto;
        int bit_PalletAvvolto;
        
		int FasciaNoALARM;
        int addr_FasciaNoALARM;
        int bit_FasciaNoALARM;
        
		int RullieraAUTO;
		int RullieraMAN;
		int LgvEscluso;
		int AvvolgitorevEscluso;

		int presenze_pallet[5];
		int ok_missione_scarico_avvolgitore;

        //OUT
		int IngombroDepositoAgv;               //USATO COME FLAG!! non come uscita
		//int bit_IngombroDepositoAgv;
		int RichiestaIngresso;
		int bit_RichiestaIngresso;

        int pallet_depositato;
        int addr_pallet_depositato;
        int bit_pallet_depositato;
        int count_pallet_depositato;

        int pallet_prelevato;
        int addr_pallet_prelevato;
        int bit_pallet_prelevato;
        int count_pallet_prelevato;

		int forzatura_attiva;
		int count_forzatura;
		//int count_ingombro;
        TDateTime InizioAttesa;
    };

/*    
	struct Montacarichi 
    {
		int posprel;
		int h_prel;
        
		int PlcConnesso;
		int generata;
		int escludi;

        int dbInput;
        int dwByteInput;
        int StatoInput[3]; // byte 2 e byte 3 - byte 8 e byte 9 della word di chiamate

		int dbuscite;
		int dwuscite;
		int uscite;
		int uscitedascrivere;

        //IN
		int ChiamataDeposito;
        int addr_ChiamataDeposito;
        int bit_ChiamataDeposito;

		int OKingresso;
        int addr_OKingresso;
        int bit_OKingresso;

		int PrenotaVuoti;
        int addr_PrenotaVuoti;
        int bit_PrenotaVuoti;

		int PrelievoVuoti;
        int addr_PrelievoVuoti;
        int bit_PrelievoVuoti;
        
		int MontacarichiAttivo;
        int addr_MontacarichiAttivo;
        int bit_MontacarichiAttivo;
        
		int PrioritaMagazzinoPLC;
        int addr_PrioritaMagazzinoPLC;
        int bit_PrioritaMagazzinoPLC;
        
		int IngombroPrelievoAgv;              //USATO COME FLAG!! non come uscita
		//int bit_IngombroPrelievoAgv;
        
        //OUT
		int RichiestaIngresso;
		int OKPrenotaVuoti;
		int bit_RichiestaIngresso;
		int bit_OKPrenotaVuoti;

        int pallet_depositato;
        int addr_pallet_depositato;
        int bit_pallet_depositato;
        int count_pallet_depositato;

        int vuoto_prelevato;
        int addr_vuoto_prelevato;
        int bit_vuoto_prelevato;
        int count_vuoto_prelevato;

		int AllarmeAttivo;
		int bit_AllarmeAttivo;

		int MancanzaVuoti;
		int bit_MancanzaVuoti;

		int DueTerziMag;
		int bit_DueTerziMag;
        
		int MagVuoto;
		int bit_MagVuoto;
        
		int forzatura_attiva;
		int count_forzatura;

        TDateTime InizioAttesa;
    };
*/    

	struct Incas 
    {
		int forzatura_attiva;
		int count_forzatura;
        
		int PlcConnesso;
        
        int dbInput;
        int dwByteInput;
        int StatoInput; // 
        
		int dbuscite;
		int dwuscite;
		int uscite;     // 
		int uscitedascrivere;
        
        int pos;


        //IN
		int richiesta_impegno_incrocio;
		int bit_richiesta_impegno_incrocio;
        
        //OUT
		int consenso_impegno_incrocio;         //per la gestione incrocio con AGV1
		int bit_consenso_impegno_incrocio;

		int impegno_inc_rosso;             //per la gestione semaforo  nel corridoio
		int bit_impegno_inc_rosso;       

		int impegno_inc_giallo;             
		int bit_impegno_inc_giallo;       

		int impegno_inc_verde;             
		int bit_impegno_inc_verde;       

		int lgv_in_marcia;
		int lgv_in_allarme;

	};



	struct Robots 
    {
        int posvuoti;
		int vuoti_generati;
		int vuoti_esclusi;
        
		int forzatura_attiva;
		int count_forzatura;
        
		int PlcConnesso;
		//int input[24];
		//int output[24];
		//int forzatura_input;
        
        int dbInput;
        int dwByteInput;
        int StatoInput[5]; // byte 1 to 5 
        
		int dbuscite;
		int dwuscite;
		int uscite[5];     // byte 1 to 5 
		int uscitedascrivere[5];
        
        int pos_linea[NLINEE_PREL + 1];

		int IngombroPrelievoAgv[NLINEE_PREL + 1];               //USATO COME FLAG!! non come uscita
		int IngombroDepositoAgv;                               //USATO COME FLAG!! non come uscita

        //IN
		int presenza_pallet_linea[NLINEE_PREL + 1];
		int addr_presenza_pallet[NLINEE_PREL + 1];
		int bit_presenza_pallet[NLINEE_PREL + 1];
		int count_presenza_linea[NLINEE_PREL + 1];
		int count_assenza_linea[NLINEE_PREL + 1];

		int OK_ingresso_linea[NLINEE_PREL + 1];
		int addr_OK_ingresso_linea[NLINEE_PREL + 1];
		int bit_OK_ingresso_linea[NLINEE_PREL + 1];

		int pallet_incompleto[NLINEE_PREL + 1];
		int addr_pallet_incompleto[NLINEE_PREL + 1];
		int bit_pallet_incompleto[NLINEE_PREL + 1];

		int richiesta_vuoti;
		int addr_richiesta_vuoti;
		int bit_richiesta_vuoti;
		int OK_ingresso_vuoti;
		int addr_OK_ingresso_vuoti;
		int bit_OK_ingresso_vuoti;
        
        //OUT
		int richiesta_ingresso_linea[NLINEE_PREL + 1];
		int addr_richiesta_ingresso_linea[NLINEE_PREL + 1];
		int bit_richiesta_ingresso_linea[NLINEE_PREL + 1];
        
		int pallet_prelevato_linea[NLINEE_PREL + 1];
		int addr_pallet_prelevato_linea[NLINEE_PREL + 1];
		int bit_pallet_prelevato_linea[NLINEE_PREL + 1];
		int count_pallet_prelevato_linea[NLINEE_PREL + 1];

		int richiesta_ingresso_vuoti;
		int addr_richiesta_ingresso_vuoti;
		int bit_richiesta_ingresso_vuoti;

        int vuoto_depositato;
        int addr_vuoto_depositato;
        int bit_vuoto_depositato;
        int count_vuoto_depositato;

		//int fuori_ingombro_vuoti;
		//int addr_fuori_ingombro_vuoti;
		//int bit_fuori_ingombro_vuoti;

		int lgv_in_marcia;
		int lgv_in_allarme;

	};

    
	struct Allarmi 
    {
        int allarme_depositomag_linea1;
        int allarme_depositomag_linea2;
        int allarme_depositomag_linea3;
        int allarme_depositomag_linea4;
        int allarme_depositomag_linea5;
        int allarme_depositomag_linea6;
        int allarme_depositomag_linea7;
        int allarme_depositomag_linea8;
        int allarme_depositomag_linea9;
        
        int allarme_depositomag_avvolgitore;

        int allarme_posizioneDEP_esclusa;     //avvolgitore
        int allarme_posizionePREL_esclusa;    //avvolgitore
        int allarme_montacarichi_esclusa;     //montacarichi
		int allarme_vuoti;
		int allarme_vuoti_mancanti;
    };


	Agv DatiAgv[2];
    
	Fasciatore PlcFascia;
    //Montacarichi PlcMonta;
	Robots PlcRobot;
    Incas PlcIncas;
    
    Allarmi gAllarmi;
    int pwdlevel, all;
    unsigned char bit[8], bitAnd[8];
    AnsiString PWD, RobotAddress, PosizioniLuceRossaSemaforo, PosizioniLuceGiallaSemaforo;
    int RobotPort, baiaformSegnali, TouchPanel, stationnumber, onlyelectrode, EnableManuals, countwatchdog, actual, prev, num_stazioni;
    bool Simula, formbaiaattivo, forzature_attive;
    AnsiString msg, timestr, NomeClient, format_data_ora_datatbase, CorsiaDiDefault, CorsiaDiScarto;
    int tempo_carica, init_mappa, tempo_carica_batt_bassa, AbilitaMissioni, percentuale_batt_bassa, percentuale_batt_ok, poscb, EscludiAvvolgitore, DisabilitaTelecamera;
    int PrioritaMagazzino;
    int zoom, count_inizio, alarmmissioni, chiusura_applicazione, num_pallet_minimi, tempo_pallet, count_cb, count_forzature;
    double Xo, Yo, FormH, FormW, H, W;
    int num_rot_alarm, num_porte, altezza_max_pallet, diff_pick_drop, pos_in_fila_forche_corsia;
    int pos_scaffale, piano_scaffale, corsia_scaffale, num_corsie, altezza_corsia_piano1_2, altezza_corsia_piano3_4;
    // variabili per zoom e selezione
    int agvdaspostare, Contatore, x1, x2, y1, y2, Contatoresel, x1sel, x2sel, y1sel, y2sel, xmouse, ymouse, countmousemove, countwheel;
    int widthextend, heightextend, ultimapospassata, ridisegnapalsel, filaincellaselezionatamin, filaincellaselezionatamax;
    int AbilitaDemo, num_moduli_phoenix, aggiornamappa, aggiornaagv, h_corsia[5], quota_dep_terra, offset_dep, pos_mappa_no_db[4], num_pos_no_database;
    int h_prel_demo[4], h_dep_demo[4], stato_pos_demo[4], AbilitaTestLuciPulsantiere, AbilitaTestLuciPulsantiereRimosso;
	bool ret, mousedown, retsel, mousedownsel,avanza_pos2;
    int counterAzzeraUdc;
	TTime ScrollMouse;
    TMyConnector *ToPickAgv1, *ToPickAgv2, *ToDropAgv1, *ToDropAgv2, *ToDropPosPrel;

};

// ----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);
// ----------------------------------------------------------------------------
#endif
