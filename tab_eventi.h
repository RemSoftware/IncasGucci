//---------------------------------------------------------------------------

#ifndef tab_eventiH
#define tab_eventiH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include "cspin.h"
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TfTabEventi : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label3;
	TCheckBox *NMisCheck;
	TCSpinEdit *NMis;
	TCheckBox *CheckBox2;
	TDBGrid *DBGrid1;
	TDataSource *DataSource1;
	TADOQuery *Query1;
	TLabel *Label2;
	TLabel *Tot;
	TEdit *Destinazione1;
	TCheckBox *CheckBox6;
	TEdit *Edit2;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall NMisCheckClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfTabEventi(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfTabEventi *fTabEventi;
//---------------------------------------------------------------------------
#endif
