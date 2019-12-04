//---------------------------------------------------------------------------

#ifndef allarmiH
#define allarmiH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include "cspin.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TfAllarmi : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label1;
	TLabel *Label3;
	TDateTimePicker *DaData;
	TCSpinEdit *Pos;
	TCheckBox *FilterOnPos;
	TCheckBox *FilterOnDateFrom;
	TCheckBox *FilterOnDateTo;
	TDateTimePicker *AData;
	TDateTimePicker *AOra;
	TDateTimePicker *DaOra;
	TEdit *Allarme;
	TCheckBox *CheckBox2;
	TCSpinEdit *CSpinEdit2;
	TRadioGroup *RadioGroup1;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox3;
	TEdit *Edit1;
	TCheckBox *CheckBox4;
	TCSpinEdit *CSpinEdit1;
	TDBGrid *DBGrid1;
	TDataSource *DataSource1;
	TADOQuery *Query1;
	TBitBtn *BitBtn1;
	TLabel *Tot;
	TLabel *Label2;
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall DBGrid1TitleClick(TColumn *Column);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall DBGrid1DblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfAllarmi(TComponent* Owner);
    struct order {
		AnsiString NomeCampo ;
        bool descendig ;
	};
	std::vector<order> Ordinamento ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfAllarmi *fAllarmi;
//---------------------------------------------------------------------------
#endif
