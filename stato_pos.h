//---------------------------------------------------------------------------

#ifndef stato_posH
#define stato_posH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "MyShape.h"
#include <Vcl.Graphics.hpp>
#include "scaffale.h"
//---------------------------------------------------------------------------
class TfCorsia : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TfrScaffale *frScaffale1;
	TfrScaffale *frScaffale2;
	TfrScaffale *frScaffale3;
	TfrScaffale *frScaffale4;
	TfrScaffale *frScaffale5;
	TPanel *Panel6;
	TPanel *Panel7;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TPanel *Panel8;
	TBitBtn *BitBtn2;
	TPanel *Panel5;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TPanel *Panel9;
	TPanel *Panel10;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn3;
	TGroupBox *GroupBox1;
	TCheckBox *ckUsoPrel;
	TCheckBox *ckUsoDep;
	TCheckBox *ckAbilita;
	TCheckBox *ckPrenotata;
	TCheckBox *ckPiena;
	TCheckBox *ckVuota;
	TGroupBox *GroupBox2;
	TLabel *Label11;
	TLabel *Label12;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	TBitBtn *BitBtn6;
	TPanel *Panel3;
	TPanel *Panel4;
	TTimer *Timer1;
	TGroupBox *GroupBox3;
	TCheckBox *ckDisabilitaPos;
	TCheckBox *cPrenotataPos;
	TGroupBox *GroupBox4;
	TLabeledEdit *leUdc;
	TLabeledEdit *leLotto;
	TLabeledEdit *leAltezzaPallet;
	TLabeledEdit *leCodice;
	TLabeledEdit *leData;
    TLabeledEdit *eHDep;
    TLabeledEdit *leTipoPallet;
	TPanel *Panel11;
    TComboBox *cbTipo;
	TLabel *Label9;
    TfrScaffale *frScaffale6;
    TfrScaffale *frScaffale7;
    TfrScaffale *frScaffale8;
	TCheckBox *cPresenzaPos;
    TLabeledEdit *eHPrel;
    TComboBox *cbPriorita;
    TLabel *Label10;
	void __fastcall frScaffale1MyShape4MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn5Click(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BitBtn6Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfCorsia(TComponent* Owner);
	void VisualizzaScaffale() ;
	int forche[3],statodisabilita,statoprenotata ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfCorsia *fCorsia;
//---------------------------------------------------------------------------
#endif
