//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "tab_eventi.h"
#include "db.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfTabEventi *fTabEventi;
//---------------------------------------------------------------------------
__fastcall TfTabEventi::TfTabEventi(TComponent* Owner)
	: TMDIChild(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfTabEventi::FormActivate(TObject *Sender)
{
int i ;
	CheckBox2->Checked = false;
	CheckBox6->Checked = false;
	Destinazione1->Text ="";
	DBGrid1->ReadOnly = !MainForm->pwdlevel ;
	NMisCheckClick(this);

}
//---------------------------------------------------------------------------

void __fastcall TfTabEventi::NMisCheckClick(TObject *Sender)
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
	if (CheckBox2->Checked) {
		if (filtro)
			filter = filter + " AND ";
		filter =filter + "NomeBaia LIKE '%"+Destinazione1->Text+"%'";
		filtro = true;
	}
	if (CheckBox6->Checked) {
		if (filtro)
			filter = filter + " AND ";
		filter =filter + "StatoFotocellula ="+IntToStr(Edit2->Text.ToIntDef(0));
		filtro = true;
	}
	if (!filtro) {
		filter = "Select * from StatoBaie" ;
	}else {
		filter = "Select * from StatoBaie"+filter ;
	}
	filter = filter + " order by Id" ;
    Query1->SQL->Append(filter) ;
	Query1->Open();
	Tot->Caption = IntToStr(Query1->RecordCount);
	Query1->Last();
}
//---------------------------------------------------------------------------


