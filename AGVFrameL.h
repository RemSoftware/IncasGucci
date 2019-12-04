//---------------------------------------------------------------------------

#ifndef AGVFrameLH
#define AGVFrameLH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TfrAgvL : public TFrame
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *pAGV;
	TLabeledEdit *lePosizione;
	TLabeledEdit *leDestinazione;
	TLabeledEdit *leMissione;
	TImage *Image1;
	TLabeledEdit *leLivBatt;
	TPopupMenu *Forzature;
	TMenuItem *ForzaIdCoilsuAgv1;
	TGroupBox *GroupBox4;
	TCheckBox *cbAlarm;
	TCheckBox *cbCarica;
	TCheckBox *cbMan;
	TCheckBox *cbAuto;
	TCheckBox *cbBattBassa;
	TCheckBox *cbSemiAuto;
	TCheckBox *cbLoaded;
	TPanel *pInMis;
	TCheckBox *cbInMissione;
	TLabeledEdit *leCodicePallet;
	TLabeledEdit *leDest;
	TLabeledEdit *lePrel;
	TLabeledEdit *leIdArticolo;
	TLabeledEdit *leFasciatura;
private:	// User declarations
public:		// User declarations
	__fastcall TfrAgvL(TComponent* Owner);
	bool aggiornamento ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrAgvL *frAgvL;
//---------------------------------------------------------------------------
#endif
