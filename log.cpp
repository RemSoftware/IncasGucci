// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "log.h"
#include "db.h"
#include "main.h"
#include "MSG.h"
#include "ExtraFunction.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfLog *fLog;

// ---------------------------------------------------------------------------
__fastcall TfLog::TfLog(TComponent* Owner)
    : TMDIChild(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfLog::CheckBox1Click(TObject *Sender)
{
    //
    Word Year, Month, Day;
    AnsiString str = "";
    Query1->Close();
    Query1->SQL->Clear();
    AnsiString filtro;
    filtro = "Select * from Log where (1=1) ";

    if (Edit1->Text != "") {
        filtro = filtro + " and Descrizione LIKE '%" + Edit1->Text + "%'";
    }

    if (Allarme->Text != "") {
        filtro = filtro + " and Sorgente LIKE '%" + Allarme->Text + "%'";
    }

    if (Edit2->Text != "") {
        filtro = filtro + " and PosizioniAgv LIKE '%" + Edit2->Text + "%'";
    }

    if (Edit3->Text != "") {
        filtro = filtro + " and UDCAgv LIKE '%" + Edit3->Text + "%'";
    }

    if (Edit4->Text != "") {
        filtro = filtro + " and DestAgv LIKE '%" + Edit4->Text + "%'";
    }
    if (FilterOnDateFrom->Checked) {
        DecodeDate(DaData->Date, Year, Month, Day);
        str = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
        filtro = filtro + " and dataora >= '" + str + " " + TimeToStr(DaOra->Time) + "'"; // filtro = filtro + "DataOra >= convert(datetime,'" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "', 103)";
    }
    if (FilterOnDateTo->Checked) {
        DecodeDate(AData->Date, Year, Month, Day);
        str = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
        filtro = filtro + " and dataora <= '" + str + " " + TimeToStr(AOra->Time) + "'";
    }

    filtro = filtro + " order by DataOra";
    Query1->SQL->Add(filtro);
    Query1->Open();
    Query1->Last();
    Tot->Caption = IntToStr(Query1->RecordCount);
    Ordinamento.clear();
    order Ordin;
    Ordin.NomeCampo = "DataOra";
    Ordin.descendig = false;
    Ordinamento.push_back(Ordin);
    for (int i = 0; i < DBGrid1->Columns->Count; i++) {
        if (DBGrid1->Columns->Items[i]->FieldName == "DataOra")
            DBGrid1->Columns->Items[i]->Title->Color = clMenu;
        else
            DBGrid1->Columns->Items[i]->Title->Color = clBtnFace;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfLog::FormActivate(TObject *Sender)
{
    BitBtn1->Visible = dmDB->pwdlevel;
    CheckBox1Click(this);

}

// ---------------------------------------------------------------------------
void __fastcall TfLog::DBGrid1TitleClick(TColumn *Column)
{
    order Ricerca;
    std::vector<order>::iterator i;
    Query1->Close();
    Query1->SQL->Strings[0] = Query1->SQL->Strings[0].SetLength(Query1->SQL->Strings[0].Pos("order by") - 1);
    Ricerca.NomeCampo = Column->FieldName;
    for (i = Ordinamento.begin(); i != Ordinamento.end(); i++) {
        if (i->NomeCampo == Ricerca.NomeCampo)
            break;
    }
    if (i != Ordinamento.end()) {
        Ricerca.descendig = !i->descendig;
        Ordinamento.erase(i);
        Ordinamento.insert(Ordinamento.begin(), Ricerca); // metto davanti così l'ordinamento principale è quello
        if (Ricerca.descendig)
            Column->Title->Color = clGrayText;
        else
            Column->Title->Color = clMenu;
    }
    else {
        Column->Title->Color = clMenu;
        Ricerca.descendig = false;
        Ordinamento.insert(Ordinamento.begin(), Ricerca); // metto davanti così l'ordinamento principale è quello
    }
    Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + "order by ";
    for (i = Ordinamento.begin(); i != Ordinamento.end(); i++) {
        if (i != Ordinamento.begin())
            Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + " , ";
        Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + i->NomeCampo;
        if (i->descendig)
            Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + " desc";
    }
    Query1->Open();

}

// ---------------------------------------------------------------------------
void __fastcall TfLog::BitBtn1Click(TObject *Sender)
{
    if (Application->MessageBox(L"Are you sure to remove all record?", L"Confirm!!!", MB_YESNO) == IDYES) {
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Append("delete from Log ");
        Query1->ExecSQL();
        Query1->Close();
        dmDB->LogMsg("delete from Log");
        FormActivate(Sender);
    }

}
// ---------------------------------------------------------------------------

void __fastcall TfLog::DBGrid1DblClick(TObject *Sender)
{
    if ((Query1->Active) && (Query1->RecNo) && (Query1->FieldByName("Descrizione")->AsString != "")) {
        MSGForm->Label1->Caption = Query1->FieldByName("Descrizione")->AsString;
        MSGForm->Show();
    }

}
// ---------------------------------------------------------------------------

void __fastcall TfLog::FormCreate(TObject *Sender)
{
    AnsiString oradel;

    DaData->Date = Date();
    AData->Date = Date();
    Allarme->Text = "";

}
// ---------------------------------------------------------------------------

void __fastcall TfLog::BitBtn2Click(TObject *Sender)
{
    int res;
    if (Application->MessageBox(L"Do you want to historicise table?", L"Please confirm", MB_YESNO) == IDYES) {
        res = dmExtraFunction->StoricizzaTabelle("Log", "History_Log", "dataora", "MONTH", 1);
        if (res > 0)
            Application->MessageBox(L"Historicised", L"Ok", MB_OK);
        else
            Application->MessageBox(L"Historicizing problems", L"Read log", MB_OK);
        FormActivate(Sender);
    }
}
// ---------------------------------------------------------------------------
