//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "allarmi.h"
#include "db.h"
#include "main.h"
#include "MSG.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfAllarmi *fAllarmi;
//---------------------------------------------------------------------------
__fastcall TfAllarmi::TfAllarmi(TComponent* Owner)
	: TMDIChild(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfAllarmi::CheckBox1Click(TObject *Sender)
{
	//
    Word Year, Month,Day ;
    AnsiString str = "";
    Query1->Close();
	Query1->SQL->Clear() ;
    bool filter = false;
    AnsiString filtro;
    filtro = "Select * from Allarmi where ";
	if ((CheckBox1->Checked)&&(!CheckBox3->Checked)) {
		if (filter)
			filtro = filtro + " AND ";
		filtro = filtro + "LogOp = 1";
		filter = true;
	}else if ((!CheckBox1->Checked)&&(CheckBox3->Checked)) {
		if (filter)
			filtro = filtro + " AND ";
		filtro = filtro + "LogOp = 0";
		filter = true;
	}
	if (Allarme->Text != "") {
		if (filter)
			filtro = filtro + " AND ";
		filtro = filtro + "Nallarme = '" + Allarme->Text + "'";
		filter = true;
	}
	if (FilterOnPos->Checked) {
		if (filter)
			filtro = filtro + " AND ";
		filtro = filtro +"Posizione =" + Pos->Value;
		filter = true;
	}
	if (CheckBox2->Checked) {
		if (filter)
			filtro = filtro + " AND ";
		filtro = filtro +"Destinazione =" + CSpinEdit2->Value;
		filter = true;
	}
	if (CheckBox4->Checked) {
		if (filter)
			filtro = filtro + " AND ";
		filtro = filtro +"agv ='" + CSpinEdit1->Value;
		filtro = filtro +"'" ;
		filter = true;
	}
	if (RadioGroup1->ItemIndex > 0) {
		if (filter)
			filtro = filtro + " AND ";
		filtro = filtro + "ripristinato ="+ IntToStr(RadioGroup1->ItemIndex-1);
		filter = true;
	}
	if (Edit1->Text != "") {
		if (filter)
			filtro = filtro + " AND ";
		filtro = filtro + "Descrizione LIKE '%" + Edit1->Text + "%'";
		filter = true;
	}
	if (FilterOnDateFrom->Checked) {
        DecodeDate(DaData->Date, Year, Month,Day);
		//str = IntToStr(Day)+"."+IntToStr(Month)+"."+IntToStr(Year); //il separatore per le query sulle tabelle di BDE è il . e non la barra /
		str = DateToStr(DaData->Date) +" "+ TimeToStr(DaOra->Time) ;
		if (filter)
			filtro = filtro + " AND ";
		//filtro = filtro + "DataOra >= '" + str +" "+ TimeToStr(DaOra->Time) + "'";
		//convert(datetime, '04/04/2017 10:00:00', 103)
		filtro = filtro + "DataOra >= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
		//filtro = filtro + "DataOra >= convert(datetime,'" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "', 103)";
		filter = true;
	}
	if (FilterOnDateTo->Checked) {
        DecodeDate(AData->Date, Year, Month,Day);
		//str = IntToStr(Day)+"."+IntToStr(Month)+"."+IntToStr(Year); //il separatore per le query sulle tabelle di BDE è il . e non la barra /
		str = DateToStr(AData->Date) +" "+ TimeToStr(AOra->Time) ;
		if (filter)
			filtro = filtro + " AND ";
		//filtro = filtro + "DataOra <= '" + str +" "+ TimeToStr(AOra->Time) + "'";
		filtro = filtro + "DataOra <= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
		//filtro = filtro + "DataOra >= convert(datetime,'" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "', 103)";
		filter = true;
	}
    if (! filter)
        filtro = "Select * from Allarmi";

    filtro = filtro + " order by DataOra";
    Query1->SQL->Add(filtro);
	Query1->Open();
    Query1->Last();
	Tot->Caption = IntToStr(Query1->RecordCount);
    Ordinamento.clear();
    order Ordin ;
    Ordin.NomeCampo = "DataOra" ;
    Ordin.descendig = false ;
    Ordinamento.push_back(Ordin);
    for (int i = 0 ; i < DBGrid1->Columns->Count ; i++) {
        if (DBGrid1->Columns->Items[i]->FieldName == "DataOra")
            DBGrid1->Columns->Items[i]->Title->Color = clMenu;
        else
            DBGrid1->Columns->Items[i]->Title->Color = clBtnFace;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAllarmi::FormActivate(TObject *Sender)
{
	BitBtn1->Visible = MainForm->pwdlevel ;
	CheckBox1Click(this);

}
//---------------------------------------------------------------------------
void __fastcall TfAllarmi::DBGrid1TitleClick(TColumn *Column)
{
    order Ricerca;
	std::vector<order>::iterator i ;
    Query1->Close();
    Query1->SQL->Strings[0]= Query1->SQL->Strings[0].SetLength(Query1->SQL->Strings[0].Pos("order by")-1) ;
    Ricerca.NomeCampo = Column->FieldName ;
    for ( i = Ordinamento.begin(); i != Ordinamento.end() ; i++) {
        if (i->NomeCampo == Ricerca.NomeCampo)
            break;
    }
    if (i != Ordinamento.end()) {
        Ricerca.descendig = !i->descendig ;
        Ordinamento.erase(i);
        Ordinamento.insert(Ordinamento.begin(),Ricerca);//metto davanti così l'ordinamento principale è quello
        if (Ricerca.descendig)
            Column->Title->Color = clGrayText ;
        else
            Column->Title->Color = clMenu;
    }else {
        Column->Title->Color = clMenu;
        Ricerca.descendig = false ;
        Ordinamento.insert(Ordinamento.begin(),Ricerca);//metto davanti così l'ordinamento principale è quello
    }
    Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + "order by " ;
    for ( i = Ordinamento.begin(); i != Ordinamento.end() ; i++) {
        if (i != Ordinamento.begin())
            Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + " , " ;
        Query1->SQL->Strings[0] =Query1->SQL->Strings[0] + i->NomeCampo ;
        if (i->descendig)
            Query1->SQL->Strings[0] =Query1->SQL->Strings[0] + " desc";
    }
    Query1->Open();

}
//---------------------------------------------------------------------------
void __fastcall TfAllarmi::BitBtn1Click(TObject *Sender)
{
	if (Application->MessageBox(L"Sicuro di voler cancellare lo storico?", L"Conferma!!!", MB_YESNO) == IDYES) {
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Append( "delete from Allarmi ") ;
        Query1->ExecSQL();
        Query1->Close();
        FormActivate(Sender);
	}

}
//---------------------------------------------------------------------------

void __fastcall TfAllarmi::DBGrid1DblClick(TObject *Sender)
{
	if ((Query1->Active)&&(Query1->RecNo)&&(Query1->FieldByName("Descrizione")->AsString != "")) {
		MSGForm->Label1->Caption = Query1->FieldByName("Descrizione")->AsString ;
		MSGForm->Show();
	}

}
//---------------------------------------------------------------------------

void __fastcall TfAllarmi::FormCreate(TObject *Sender)
{
    AnsiString oradel;

	DaData->Date = Date();
    AData->Date = Date();
    FilterOnPos->Checked = false;
    FilterOnDateFrom->Checked = false;
    FilterOnDateTo->Checked = false;
    CheckBox1->Checked = false;
    CheckBox2->Checked = false;
    CheckBox3->Checked = true;
    RadioGroup1->ItemIndex = 0 ;
	Allarme->Text = "";

}
//---------------------------------------------------------------------------

