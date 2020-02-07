//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "missioni.h"
#include "db.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfMissioni *fMissioni;
//---------------------------------------------------------------------------
__fastcall TfMissioni::TfMissioni(TComponent* Owner)
	: TMDIChild(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfMissioni::BitBtn1Click(TObject *Sender)
{
	if (Application->MessageBox(L"Sicuro di voler cancellare lo storico?", L"Conferma!!!", MB_YESNO) == IDYES) {
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Append( "delete from Missioni ") ;
        Query1->ExecSQL();
        Query1->Close();
        FormActivate(Sender);
	}

}
//---------------------------------------------------------------------------
void __fastcall TfMissioni::FormActivate(TObject *Sender)
{
int i ;
	DaData->Date = Date();
    AData->Date = Date();
	FilterOnDateFrom->Checked = false;
    FilterOnDateTo->Checked = false;
    CheckBox1->Checked = false;
	CheckBox2->Checked = false;
	CheckBox3->Checked = false;
	CheckBox4->Checked = false;
	BitBtn1->Enabled = MainForm->pwdlevel ;
    Destinazione1->Text ="";
	NMisCheckClick(this);

}
//---------------------------------------------------------------------------

void __fastcall TfMissioni::NMisCheckClick(TObject *Sender)
{
    Word Year, Month,Day ;
//    AnsiString str = "",filter = "Select * from Missioni where ";
	AnsiString str = "",filter = " where ",filtrotempo,filtronpallet,filtronpalletvuoti;
    bool filtro = false ;
	int secondi_totali ,secondi_missione;
	float result,result_mis ;
	Query1->Close();
    Query1->SQL->Clear();
                   // controlli campo a video
    NMis->Text = NMis->Text.ToIntDef(0); // evito l'errore di campo non numerico
    NMis->SelStart = NMis->Text.Length(); // mi posiziono sull'ultimo carattere. Si puo' evitare
	if (NMisCheck->Checked) {
		filter =filter + "Id =" + NMis->Value;
		filtro = true;
	}
	if (PrelCheck->Checked) {
		if (filtro)
			filter = filter + " AND ";
		filter =filter + "PosPrel >=" + Prel->Value ;
		filter =filter +" AND PosPrel <=" + Dep->Value;
		filtro = true;
	}
	if (DepCheck->Checked) {
		if (filtro)
			filter = filter + " AND ";
		filter =filter + "PosDep >=" + CSpinEdit1->Value ;
		filter =filter + " AND PosDep <=" + CSpinEdit2->Value;
		filtro = true;
	}
	if (FilterOnDateFrom->Checked) {
		DecodeDate(DaData->Date, Year, Month,Day);
		//str = IntToStr(Day)+"."+IntToStr(Month)+"."+IntToStr(Year); //il separatore per le query sulle tabelle di BDE è il . e non la barra /
		str = DateToStr(DaData->Date) +" "+ TimeToStr(DaOra->Time) ;
		if (filtro)
			filter = filter + " AND ";
		//filter = filter + "generata >= '" + str +" "+ TimeToStr(DaOra->Time) + "'";
		filter += "generata >= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
		filtro = true;
	}
	if ((CheckBox2->Checked)&&(Destinazione1->Text!="")) {
		if (filtro)
			filter = filter + " AND ";
		filter = filter +"PosPrelCliente LIKE'%" + Destinazione1->Text+"%' OR PosDepCliente LIKE'%" + Destinazione1->Text+"%'";
		filtro = true;
	}
	if ((CheckBox1->Checked)&&(ComboBox1->Items->IndexOf(ComboBox1->Text)>=0)) {
		if (filtro)
			filter = filter + " AND ";
		filter = filter + "Esito = '" + ComboBox1->Text+"'";
		filtro = true;
	}
	if (CheckBox3->Checked) {
		if (filtro)
			filter = filter + " AND ";
		filter =filter+ "TipoMissione =" + ComboBox2->Items->IndexOf(ComboBox2->Text);
		filtro = true;
	}
	if (CheckBox4->Checked) {
		if (filtro)
			filter = filter + " AND ";
		filter =filter+ "Agv =" + CSpinEdit3->Value;
		filtro = true;
	}
	if ((CheckBox5->Checked)&&(Edit1->Text.ToIntDef(0))) {
		if (filtro)
			filter = filter + " AND ";
		filter = filter +"IdUdc LIKE %" + Edit1->Text+"%";
		filtro = true;
	}
	if (FilterOnDateTo->Checked) {
        DecodeDate(AData->Date, Year, Month,Day);
		//str = IntToStr(Day)+"."+IntToStr(Month)+"."+IntToStr(Year); //il separatore per le query sulle tabelle di BDE è il . e non la barra /
		str = DateToStr(AData->Date) +" "+ TimeToStr(AOra->Time) ;
		if (filtro)
			filter = filter + " AND ";
		//filter =filter + "generata <= '" + str +" "+ TimeToStr(AOra->Time) + "'";
		filter += "generata <= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
		filtro = true;
	}
	if (!filtro) {
		filter = "Select * from Missioni" ;
		filtrotempo = "SELECT COUNT(*) AS N, SUM(CAST(Fine - Iniziata AS float) * 24 * 60 * 60) AS T FROM Missioni";
		filtrotempo = filtrotempo + " where NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) AND Esito ='COMPLETATA'";
		filtronpallet = "SELECT COUNT(*) AS N FROM Missioni where ( TipoMissione = 0 or TipoMissione = 2 ) AND NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) and Esito ='COMPLETATA' AND (PosPrel > 110 AND PosPrel < 115 )";
		filtronpalletvuoti = "SELECT COUNT(*) AS N FROM Missioni where ( TipoMissione = 0 or TipoMissione = 2 ) AND NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) and Esito ='COMPLETATA' AND (PosPrel > 800 AND PosPrel < 805 )";
	}else {
		filtrotempo = "SELECT COUNT(*) AS N, SUM(CAST(Fine - Iniziata AS float) * 24 * 60 * 60) AS T FROM Missioni"+ filter+" AND NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) and Esito ='COMPLETATA'";
		filtronpallet = "SELECT COUNT(*) AS N FROM Missioni "+filter+" AND ( TipoMissione = 0 or TipoMissione = 2 ) AND NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) and Esito ='COMPLETATA' AND (PosPrel > 110 AND PosPrel < 115 )";
		filtronpalletvuoti = "SELECT COUNT(*) AS N FROM Missioni "+filter+" AND ( TipoMissione = 0 or TipoMissione = 2 ) AND NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) and Esito ='COMPLETATA' AND (PosPrel > 800 AND PosPrel < 805 )";
		filter = "Select * from Missioni"+filter ;
	}
	filter = filter + " order by generata" ;
    Query1->SQL->Append(filter) ;
	Query1->Open();
	Tot->Caption = IntToStr(Query1->RecordCount);
	Query1->Last();
	try {
		ADOQuery1->Close();
		ADOQuery1->SQL->Clear();
		ADOQuery1->SQL->Append(filtrotempo) ;
		ADOQuery1->Open();
		if (ADOQuery1->FieldByName("T")->AsString != "") {
			result = ADOQuery1->FieldByName("T")->AsFloat ;
			result_mis = result / Query1->RecordCount ;
			secondi_totali = ADOQuery1->FieldByName("T")->AsInteger ;
			secondi_missione = secondi_totali / Query1->RecordCount ;
			//str.sprintf("%.2f",result) ;
			lbTotConcluse->Caption = IntToStr(Query1->RecordCount);
			str.sprintf("%.2f",result_mis) ;
			Label14->Caption = str +" sec.";
			Label16->Caption = dmDB->RitornaOrarioDaSecondi(secondi_missione);
		}
		ADOQuery1->Close();
	}catch(...){
		dmDB->LogMsg("Eccezzione su calcolo tempistiche missioni");
	}
	try {
		ADOQuery1->Close();
		ADOQuery1->SQL->Clear();
		ADOQuery1->SQL->Append(filtronpallet) ;
		ADOQuery1->Open();
		if (ADOQuery1->FieldByName("N")->AsString != "") {
			Label12->Caption = ADOQuery1->FieldByName("N")->AsString;
		}
		ADOQuery1->Close();
		ADOQuery1->SQL->Clear();
		ADOQuery1->SQL->Append(filtronpalletvuoti) ;
		ADOQuery1->Open();
		if (ADOQuery1->FieldByName("N")->AsString != "") {
			Label15->Caption = ADOQuery1->FieldByName("N")->AsString;
		}
		ADOQuery1->Close();
	}catch(...){
		dmDB->LogMsg("Eccezzione su calcolo tempistiche missioni 2");
	}

}
//---------------------------------------------------------------------------
