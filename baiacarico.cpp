// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "baiacarico.h"
#include "db.h"
#include "main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBaiaCarico *FormBaiaCarico;

// ---------------------------------------------------------------------------
__fastcall TFormBaiaCarico::TFormBaiaCarico(TComponent* Owner)
    : TForm(Owner)
{
    }

// ---------------------------------------------------------------------------
void __fastcall TFormBaiaCarico::BitBtnCloseClick(TObject *Sender)
{
    SpeedButton1->Down = true;
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormBaiaCarico::FormActivate(TObject *Sender)
{
    activate = true;
    
    PrioritaBtn->Down = MainForm->PrioritaMagazzino;
    if (PrioritaBtn->Down)
    {
        PrioritaLed->Color = clLime;   
        PrioritaBtn->Caption = "TOGLI PRIORITA'";
    }
    
    eAltPrel->Text = dmDB->TabPostazioni[MainForm->PlcMonta.posprel]["HPREL"];
    eAltDep->Text = dmDB->TabPostazioni[MainForm->PlcMonta.posprel]["HDEP"];

    attesaAvvolgitore->Text = dmDB->TabParametri[101]["INTVALUE"].ToIntDef(0);    //attesa -secondi- per l'avvolgitore prima di recuperare dal magazzino

    Timer1Timer(this);
}

// ---------------------------------------------------------------------------

void __fastcall TFormBaiaCarico::Timer1Timer(TObject *Sender) 
{
    Timer1->Enabled = false;

    GroupBox3->Enabled = (MainForm->pwdlevel || MainForm->Simula);
    GroupBox4->Enabled = (MainForm->pwdlevel || MainForm->Simula);
    GroupBox8->Enabled = (MainForm->pwdlevel || MainForm->Simula);
    BitBtn6->Visible = (MainForm->Simula);
    BitBtn1->Visible = (MainForm->pwdlevel || MainForm->Simula);
    BitBtnChange->Enabled = (MainForm->pwdlevel || MainForm->Simula);
    
	if ((activate) || (SpeedButton1->Down) || ((!MainForm->pwdlevel) && (!MainForm->Simula))) 
    {
		AggiornaCampiPlc();
    }
    else if ((MainForm->pwdlevel) && (!MainForm->Simula))
    {
    
    }

    activate = false;
    Timer1->Enabled = true;
}
// ---------------------------------------------------------------------------


void TFormBaiaCarico::AggiornaCampiPlc() 
{
    TDatiSSCCPlc DatiScrittisuPlc;
	TRecordList TabPosizioni;

	dmDB->CaricaTabella("posizioni where Pos = " + IntToStr(MainForm->PlcMonta.posprel), TabPosizioni);

    cbPrenotata->Checked = TabPosizioni[0]["PRENOTATA"].ToIntDef(0);
    cbEsclusa->Checked = TabPosizioni[0]["DISABILITA"].ToIntDef(0);
    
    cbDepPallet->Checked = MainForm->PlcMonta.ChiamataDeposito;
    cbOkPrel->Checked = MainForm->PlcMonta.OKingresso;
    cbPrenVuoti->Checked = MainForm->PlcMonta.PrenotaVuoti;
    cbPrelPallet->Checked = MainForm->PlcMonta.PrelievoVuoti;
    cbMontacarichiAttivo->Checked = MainForm->PlcMonta.MontacarichiAttivo;
    cbPrioritaMagazzino->Checked = MainForm->PlcMonta.PrioritaMagazzinoPLC;

	cbRicIngresso->Checked = MainForm->PlcMonta.RichiestaIngresso;
	//cbLgvIngombro->Checked = MainForm->PlcMonta.IngombroPrelievoAgv;
	cbOkPrenotaz->Checked = MainForm->PlcMonta.OKPrenotaVuoti;
	cbDepositato->Checked = MainForm->PlcMonta.pallet_depositato;
	cbPrelevato->Checked = MainForm->PlcMonta.vuoto_prelevato;
    cbDueterziMag->Checked = MainForm->PlcMonta.DueTerziMag;
    cbMagVuoto->Checked = MainForm->PlcMonta.MagVuoto;
    cbMancanzaVuoti->Checked = MainForm->PlcMonta.MancanzaVuoti;
    cbAllarmeAttivo->Checked = MainForm->PlcMonta.AllarmeAttivo;

	Shape1->Visible = (MainForm->PlcMonta.PrelievoVuoti || (!MainForm->PlcMonta.ChiamataDeposito));

    oraAttivazione->Caption = TimeToStr(MainForm->PlcMonta.InizioAttesa);
}
// ---------------------------------------------------------------------------

void __fastcall TFormBaiaCarico::BitBtnChangeClick(TObject *Sender)
{
    AnsiString stringa;
    int statodisabilita, statoprenotata, altezzaPrel, altezzaDep;
	int res;

    if (Application->MessageBox(L"Sei sicuro di voler effettuare le modifiche sulla posizione?", L"Conferma!!!", MB_YESNO) != IDYES)
        return;
    
    statodisabilita = (cbEsclusa->Checked)  ? 1 : 0;
    statoprenotata = (cbPrenotata->Checked)  ? 1 : 0;
    altezzaPrel = eAltPrel->Text.ToIntDef(0);
    altezzaDep = eAltDep->Text.ToIntDef(0);
    
    dmDB->QueryPos->Close();
	dmDB->QueryPos->SQL->Clear();
	stringa.printf("UPDATE Posizioni SET Disabilita = %d , Prenotata = %d, HPrel = %d, HDep = %d  WHERE pos = %d", statodisabilita, statoprenotata, altezzaPrel, altezzaDep, MainForm->PlcMonta.posprel);
	dmDB->QueryPos->SQL->Append(stringa);
    res = dmDB->QueryPos->ExecSQL();
    dmDB->QueryPos->Close();
    
    dmDB->AggiornaParametri(101, attesaAvvolgitore->Text.ToIntDef(0));    //attesa -secondi- per l'avvolgitore prima di recuperare dal magazzino

    dmDB->AggiornaTabParametri();
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
    MainForm->aggiornamappa = 1;
}
//---------------------------------------------------------------------------
//ingressi
//
void __fastcall TFormBaiaCarico::BitBtn6Click(TObject *Sender)
{
    //-----
    if (cbDepPallet->Checked)    //ChiamataDeposito
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_ChiamataDeposito ] |= MainForm->bit[ MainForm->PlcMonta.bit_ChiamataDeposito ];
    else
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_ChiamataDeposito ] &= MainForm->bitAnd[ MainForm->PlcMonta.bit_ChiamataDeposito ];

    if (cbOkPrel->Checked)       //consenso ingresso
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_OKingresso ] |= MainForm->bit[ MainForm->PlcMonta.bit_OKingresso ];
    else
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_OKingresso ] &= MainForm->bitAnd[ MainForm->PlcMonta.bit_OKingresso ];

    if (cbPrenVuoti->Checked)       //Prenotazione vuoti
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_PrenotaVuoti ] |= MainForm->bit[ MainForm->PlcMonta.bit_PrenotaVuoti ];
    else
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_PrenotaVuoti ] &= MainForm->bitAnd[ MainForm->PlcMonta.bit_PrenotaVuoti ];

    if (cbPrelPallet->Checked)       //Prelievo vuoti
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_PrelievoVuoti ] |= MainForm->bit[ MainForm->PlcMonta.bit_PrelievoVuoti ];
    else
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_PrelievoVuoti ] &= MainForm->bitAnd[ MainForm->PlcMonta.bit_PrelievoVuoti ];

    if (cbMontacarichiAttivo->Checked)       //Montacarichi che sta lavorando
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_MontacarichiAttivo ] |= MainForm->bit[ MainForm->PlcMonta.bit_MontacarichiAttivo ];
    else
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_MontacarichiAttivo ] &= MainForm->bitAnd[ MainForm->PlcMonta.bit_MontacarichiAttivo ];

    if (cbPrioritaMagazzino->Checked)       //Priorità alle missioni Magazzino-Montacarichi
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_PrioritaMagazzinoPLC ] |= MainForm->bit[ MainForm->PlcMonta.bit_PrioritaMagazzinoPLC ];
    else
        MainForm->PlcMonta.StatoInput[ MainForm->PlcMonta.addr_PrioritaMagazzinoPLC ] &= MainForm->bitAnd[ MainForm->PlcMonta.bit_PrioritaMagazzinoPLC ];

}
//---------------------------------------------------------------------------
//uscite
//
void __fastcall TFormBaiaCarico::BitBtn1Click(TObject *Sender)
{
	MainForm->PlcMonta.uscitedascrivere = 0;
    
    //assegnazione in OR per non modificare l'eventuale stato attivo presente
    if (cbRicIngresso->Checked)
       MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_RichiestaIngresso];
                                                       
    //if (cbLgvIngombro->Checked)
    //   MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_IngombroPrelievoAgv];

    if (cbOkPrenotaz->Checked)
       MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_OKPrenotaVuoti];

    if (cbDepositato->Checked)
       MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_pallet_depositato];

    if (cbPrelevato->Checked)
       MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_vuoto_prelevato];

    if (cbDueterziMag->Checked)
       MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_DueTerziMag];

    if (cbMagVuoto->Checked)
       MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_MagVuoto];

    if (cbMancanzaVuoti->Checked)
       MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_MancanzaVuoti];

    if (cbAllarmeAttivo->Checked)
       MainForm->PlcMonta.uscitedascrivere |= MainForm->bit[MainForm->PlcMonta.bit_AllarmeAttivo];

   	MainForm->PlcMonta.forzatura_attiva = 1 ; //mi segno che sto forzando segnali
}
//---------------------------------------------------------------------------


void __fastcall TFormBaiaCarico::PrioritaBtnClick(TObject *Sender)
{
	if (PrioritaBtn->Down)
    { 
        PrioritaLed->Color = clLime;    
        PrioritaBtn->Caption = "TOGLI PRIORITA'"; 
    }
    else
    {
        PrioritaLed->Color = clBtnFace;    
        PrioritaBtn->Caption = "DAI PRIORITA'"; 
    }

    MainForm->PrioritaMagazzino = PrioritaBtn->Down;
	dmDB->AggiornaStatoAgv(1);

    MainForm->aggiornamappa = 1;
}
//---------------------------------------------------------------------------

