//---------------------------------------------------------------------------

#ifndef PosPresseH
#define PosPresseH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>

typedef struct {
    int IDUDC;
    int IDarticolo;
    int IdGestionale;
	int CodTipoUDC;
	int tara;
	int tara_pezzi;
    int stato;
    int pesoiniziale;
    int pesoattuale;
    int IdOut;
    char Lotto[50];
} TUDC;

// ---------------------------------------------------------------------------
// Struttura utilizzata per leggere/scrivere records dei pallet in ingresso
// (non di comunicazione tra CLIENT e SERVER)

typedef struct {
    int ID;
    int stato;
    AnsiString pressa;
    AnsiString pos;
	AnsiString tipo;
	int impilazione;
	AnsiString username;
	int quarantena;
    int anomalia;
	AnsiString articolo;
	AnsiString descrizione;
	AnsiString lotto;
    int pezzi;
    int vassoi;
    int ntot;
    TDateTime lavorazione;
	AnsiString sscc;
	AnsiString gtin;
} TPALLET;


//---------------------------------------------------------------------------
class TPosPresseForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *PanelTitle;
    TPanel *Panel5;
    TLabel *Label20;
    TLabel *LabelPos;
    TLabel *Label23;
    TLabel *LabelPosCli;
    TGroupBox *GroupBox5;
    TGroupBox *GroupBox3;
    TCheckBox *ckDisabilitaPos;
    TCheckBox *cPrenotataPos;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TEdit *edID;
    TLabel *Label2;
    TShape *Led;
    TLabel *LedLabel;
    TTimer *Timer1;
    TLabeledEdit *eHPrel;
    TBitBtn *BitBtn1;
    TGroupBox *GroupBox2;
    TLabel *Label1;
    TCheckBox *ckInput1;
    TCheckBox *ckInput2;
    TCheckBox *ckInput3;
    TEdit *eUDC;
    TBitBtn *BitBtn6;
    TEdit *test;
    TGroupBox *GroupBox1;
    TCheckBox *ckOutput2;
    TCheckBox *ckOutput1;
    TBitBtn *BitBtn2;
    TCheckBox *ckOutput3;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall ckDisabilitaPosClick(TObject *Sender);
    void __fastcall cPrenotataPosClick(TObject *Sender);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall LedMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    
private:	// User declarations

public:		// User declarations
    void VisualizzaDatiPosizione(); 
    void EmptyText(); 

    int pos, cambiocheck;
    int m_nType;
    AnsiString m_sLastType;
    TPALLET m_pallet;

    TRecordList m_TabPosizioni;

    __fastcall TPosPresseForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPosPresseForm *PosPresseForm;
//---------------------------------------------------------------------------
#endif
