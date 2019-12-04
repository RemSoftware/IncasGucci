//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "missioni_wms.h"
#include "db.h"
#include "main.h"
#include "socket.h"
#include "dmgestione.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfMissioniWms *fMissioniWms;
extern CLIENTSOCKET *cs ;
//---------------------------------------------------------------------------
__fastcall TfMissioniWms::TfMissioniWms(TComponent* Owner)
	: TMDIChild(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfMissioniWms::FormActivate(TObject *Sender)
{
    int i;
	DaData->Date = Date();
    AData->Date = Date();
    ckStato->Checked = true;
	FilterOnDateFrom->Checked = false;
    FilterOnDateTo->Checked = false;
    CheckBox1->Checked = false;
	ckLinea->Checked = false;
	BtnAbort->Enabled = false;
	BtnDelete->Visible = false;
	Destinazione1->Text ="";
	DBGrid1->ReadOnly = !MainForm->pwdlevel;
	NMisCheckClick(this);

}
//---------------------------------------------------------------------------

void __fastcall TfMissioniWms::NMisCheckClick(TObject *Sender)
{
    Word Year, Month,Day ;
	AnsiString str, filter;
    bool filtro = false ;
	int secondi_totali ,secondi_missione;
	float result,result_mis ;
	Query1->Close();
    Query1->SQL->Clear();

	filter = "SELECT * FROM MissioniWms WHERE (1=1) ";

   // controlli campo a video
	if (NMisCheck->Checked) 
		filter = filter + " AND Progressivo =" + NMis->Value;
    
	if (ckLinea->Checked) 
		filter =filter + " AND PosizionePrelievo = "+IntToStr(Destinazione1->Text.ToIntDef(0));

	if (ckStato->Checked) 
		filter =filter + " AND StatoMissione < " + IntToStr(STATO_DEPOSITATA);

	if (FilterOnDateFrom->Checked) 
    {
		DecodeDate(DaData->Date, Year, Month,Day);
		str = DateToStr(DaData->Date) +" "+ TimeToStr(DaOra->Time);
		filter += " AND DataOra >= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
	}
    
	if ((CheckBox1->Checked)&& eStato->Text !="") 
		filter = filter + " AND StatoMissione = " + IntToStr(eStato->Text.ToIntDef(0)); 

	if (FilterOnDateTo->Checked) 
    {
        DecodeDate(AData->Date, Year, Month,Day);
		str = DateToStr(AData->Date) +" "+ TimeToStr(AOra->Time);
		filter +=  " AND DataOra <= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
	}

	filter = filter + " ORDER BY Progressivo ASC";
    Query1->SQL->Append(filter);
	Query1->Open();
	Tot->Caption = IntToStr(Query1->RecordCount);
	Query1->Last();
}
//---------------------------------------------------------------------------
void __fastcall TfMissioniWms::BtnAbortClick(TObject *Sender)
{
	int progr = Query1->FieldByName("Progressivo")->AsInteger;
	AnsiString posCliente = Query1->FieldByName("PosizionePrelievo")->AsString;
    int nPos = dmDB->TabCustomerPos[posCliente]["POS"].ToIntDef(0);

    DMGestioneEventi->TimerMissioni->Enabled = false;
    
	if ((Application->MessageBox(L"Sei sicuro di voler abortire la missione selezionata?", L"Conferma!!!", MB_YESNO) == IDYES)) 
    {
		Query1->Close();
		Query1->SQL->Clear();
		Query1->SQL->Append( "UPDATE MissioniWms SET stato = " + IntToStr(STATO_ABORTPC) + " WHERE ID = " + IntToStr(progr));
		Query1->ExecSQL();
		Query1->Close();

        dmDB->UpdatePosCarico(nPos, 0, 0);   //mette UDC = 0  e ID caricamento = 0
        dmDB->CaricaTabelle();
        MainForm->aggiornamappa = 1;
        
		dmDB->LogMsg("Abort missione da Pagina caricamento (stato 66) ID = "+ IntToStr(progr));

		FormActivate(Sender);
	}

    DMGestioneEventi->TimerMissioni->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfMissioniWms::BtnDeleteClick(TObject *Sender)
{
	int progr = Query1->FieldByName("Progressivo")->AsInteger;
    
	if ((progr)&&(Application->MessageBox(L"Sicuro di voler cancellare la riga selezionata?", L"Conferma!!!", MB_YESNO) == IDYES)) 
    {
		Query1->Close();
		Query1->SQL->Clear();
		Query1->SQL->Append( "UPDATE MissioniWms SET stato = " + IntToStr(STATO_CANCELLATA) + " WHERE ID = " + IntToStr(progr));
		Query1->ExecSQL();
		Query1->Close();
		FormActivate(Sender);
	}

}
//---------------------------------------------------------------------------

void __fastcall TfMissioniWms::DBGrid1CellClick(TColumn *Column)
{
	BtnAbort->Enabled = Query1->RecNo && (Query1->FieldByName("Stato")->AsInteger == STATO_INSERITA || Query1->FieldByName("Stato")->AsInteger == STATO_INCARICO);
	BtnDelete->Enabled = MainForm->pwdlevel && Query1->RecNo && (Query1->FieldByName("Stato")->AsInteger > STATO_DEPOSITATA);
}
//---------------------------------------------------------------------------



void __fastcall TfMissioniWms::InsertBtnClick(TObject *Sender)
{
//    
}
//---------------------------------------------------------------------------

