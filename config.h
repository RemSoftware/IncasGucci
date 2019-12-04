//---------------------------------------------------------------------------

#ifndef configH
#define configH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "DB.h"
//---------------------------------------------------------------------------
class TfrConfig : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel4;
	TPanel *Panel3;
	TBitBtn *BitBtnChange;
	TPanel *Panel5;
	TCheckBox *AbilitaMissioni;
	TBitBtn *BitBtn2;
	TTimer *Timer1;
	TPanel *Panel8;
	TPanel *Panel10;
	TLabeledEdit *leHPrel;
	TPanel *Panel14;
	TPanel *Panel15;
	TLabeledEdit *LabeledEdit6;
    TLabeledEdit *leHDep;
	TPanel *Panel6;
    TCheckBox *cbEscludiPorta3;
	TPanel *Panel7;
    TLabeledEdit *lePortaAperta2;
    TCheckBox *cbEscludiPorta2;
    TLabeledEdit *leAperturaPorta2;
	TPanel *Panel9;
    TLabeledEdit *lePortaAperta1;
    TCheckBox *cbescludiPorta1;
    TLabeledEdit *leAperturaPorta1;
    TLabeledEdit *lePortaAperta3;
    TLabeledEdit *leAperturaPorta3;
	TCheckBox *AbilitaLuci;
    TBitBtn *BitBtn1;
    TLabeledEdit *nPallet;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall BitBtnChangeClick(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrConfig(TComponent* Owner);
	TRecordList TabAttesa;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrConfig *frConfig;
//---------------------------------------------------------------------------
#endif
