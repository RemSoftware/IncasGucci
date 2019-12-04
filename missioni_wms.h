//---------------------------------------------------------------------------

#ifndef missioni_wmsH
#define missioni_wmsH
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
class TfMissioniWms : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label3;
	TDateTimePicker *DaData;
	TCheckBox *FilterOnDateFrom;
	TCheckBox *FilterOnDateTo;
	TDateTimePicker *AData;
	TDateTimePicker *AOra;
	TDateTimePicker *DaOra;
	TCheckBox *NMisCheck;
	TCSpinEdit *NMis;
    TCheckBox *ckLinea;
	TCheckBox *CheckBox1;
	TDBGrid *DBGrid1;
	TDataSource *DataSource1;
	TADOQuery *Query1;
	TLabel *Label2;
	TLabel *Tot;
    TBitBtn *BtnDelete;
	TEdit *Destinazione1;
    TBitBtn *BtnAbort;
    TLabel *Label1;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label13;
    TEdit *eStato;
    TLabel *Label9;
    TCheckBox *ckStato;
    TBitBtn *InsertBtn;
    TLabel *Label11;
    TLabel *Label10;
	void __fastcall BtnDeleteClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall NMisCheckClick(TObject *Sender);
	void __fastcall BtnAbortClick(TObject *Sender);
	void __fastcall DBGrid1CellClick(TColumn *Column);
    void __fastcall InsertBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfMissioniWms(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfMissioniWms *fMissioniWms;
//---------------------------------------------------------------------------
#endif
