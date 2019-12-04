//---------------------------------------------------------------------------

#ifndef baiacaricoH
#define baiacaricoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------
class TFormBaiaCarico : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TBitBtn *BitBtnClose;
	TBitBtn *BitBtnChange;
	TTimer *Timer1;
    TImage *Image1;
    TShape *Shape1;
    TGroupBox *GroupBox8;
    TLabel *Label1;
    TCheckBox *cbPrenotata;
    TCheckBox *cbEsclusa;
    TEdit *eAltPrel;
    TGroupBox *GroupBox3;
    TCheckBox *cbPrelPallet;
    TCheckBox *cbOkPrel;
    TBitBtn *BitBtn6;
    TGroupBox *GroupBox4;
    TCheckBox *cbRicIngresso;
    TBitBtn *BitBtn1;
    TSpeedButton *SpeedButton1;
    TCheckBox *cbDepPallet;
    TCheckBox *cbPrenVuoti;
    TCheckBox *cbOkPrenotaz;
    TCheckBox *cbDepositato;
    TCheckBox *cbPrelevato;
    TEdit *eAltDep;
    TLabel *Label2;
    TEdit *attesaAvvolgitore;
    TLabel *Label3;
    TLabel *oraAttivazione;
    TCheckBox *cbDueterziMag;
    TCheckBox *cbMagVuoto;
    TCheckBox *cbMancanzaVuoti;
    TCheckBox *cbAllarmeAttivo;
    TCheckBox *cbMontacarichiAttivo;
    TLabel *Label4;
    TLabel *Label5;
    TSpeedButton *PrioritaBtn;
    TEdit *PrioritaLed;
    TCheckBox *cbPrioritaMagazzino;
	void __fastcall BitBtnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtnChangeClick(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall PrioritaBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormBaiaCarico(TComponent* Owner);

	bool activate;
    void AggiornaCampiPlc(); 
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBaiaCarico *FormBaiaCarico;
//---------------------------------------------------------------------------
#endif
