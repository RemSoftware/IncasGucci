//---------------------------------------------------------------------------

#ifndef missioniH
#define missioniH
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
class TfMissioni : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label3;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label4;
	TLabel *Label8;
	TDateTimePicker *DaData;
	TCheckBox *FilterOnDateFrom;
	TCheckBox *FilterOnDateTo;
	TDateTimePicker *AData;
	TDateTimePicker *AOra;
	TDateTimePicker *DaOra;
	TCheckBox *PrelCheck;
	TCSpinEdit *Prel;
	TCheckBox *DepCheck;
	TCSpinEdit *Dep;
	TCheckBox *NMisCheck;
	TCSpinEdit *NMis;
	TCSpinEdit *CSpinEdit1;
	TCSpinEdit *CSpinEdit2;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox4;
	TCSpinEdit *CSpinEdit3;
	TCheckBox *CheckBox1;
	TComboBox *ComboBox1;
	TDBGrid *DBGrid1;
	TDataSource *DataSource1;
	TADOQuery *Query1;
	TLabel *Label2;
	TLabel *Tot;
	TBitBtn *BitBtn1;
	TCheckBox *CheckBox3;
	TComboBox *ComboBox2;
	TEdit *Destinazione1;
	TCheckBox *CheckBox5;
	TEdit *Edit1;
    TLabel *Label1;
    TLabel *lbTotConcluse;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label15;
	TLabel *Label16;
	TADOQuery *ADOQuery1;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall NMisCheckClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfMissioni(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfMissioni *fMissioni;
//---------------------------------------------------------------------------
#endif
