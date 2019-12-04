//---------------------------------------------------------------------------

#ifndef stato_pressaH
#define stato_pressaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormDatiPressa : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TBitBtn *BitBtnClose;
	TBitBtn *BitBtnChange;
	TTimer *Timer1;
    TGroupBox *GroupBox3;
    TCheckBox *cmChiamaVuoti1;
    TCheckBox *cmChiamaVuoti2;
    TCheckBox *cmChiamaVuoti4;
    TCheckBox *cmChiamaVuoti3;
    TGroupBox *GroupBox4;
    TCheckBox *cbChiamataVuotiConf1;
    TCheckBox *cbChiamataVuotiConf2;
    TCheckBox *cbChiamataVuotiConf4;
    TCheckBox *cbChiamataVuotiConf3;
    TGroupBox *GroupBox1;
    TCheckBox *cbChiamaPienoConf;
    TCheckBox *cbChiamaPieno;
    TLabel *Label1;
    TEdit *eLampeggio;
    TLabel *lbl2;
    TEdit *eContLamp;
	TGroupBox *GroupBox6;
	TCheckBox *cbInMissione;
	TCheckBox *cbEscludi;
	TGroupBox *GroupBox8;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *eAltPrel;
	TEdit *eAltDep;
	TLabel *Label4;
	TEdit *eDataOra;
	TLabel *Label5;
	TLabel *Label8;
	TEdit *ePos1;
	TEdit *ePos2;
	TLabel *Label6;
	TLabel *Label9;
	TEdit *ePos3;
	TEdit *ePos4;
	TPanel *Panel11;
	TLabel *Label7;
	TComboBox *cbTipoPal;
	void __fastcall BitBtnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall BitBtnChangeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormDatiPressa(TComponent* Owner);
	bool activate ;
    int id_pulsantiera ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDatiPressa *FormDatiPressa;
//---------------------------------------------------------------------------
#endif
