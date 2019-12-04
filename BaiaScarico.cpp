//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BaiaScarico.h"
#include "db.h"
#include "main.h"
#include "dmgestionePLC.h"
#include "plcfunct.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBaiaScarico *FormBaiaScarico;
extern TTDMgestionePLC *TDMgestionePLC;
//---------------------------------------------------------------------------
__fastcall TFormBaiaScarico::TFormBaiaScarico(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormBaiaScarico::BitBtnCloseClick(TObject *Sender)
{
    SpeedButton1->Down = true;
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormBaiaScarico::FormActivate(TObject *Sender)
{
    activate = true;
    
    EscludiBtn->Down = MainForm->EscludiAvvolgitore;
    if (EscludiBtn->Down)
    {
        EscludiLed->Color = clRed;   
        EscludiBtn->Caption = "ABILITA";
    }
    
    eAltDep->Text = dmDB->TabPostazioni[MainForm->PlcFascia.posdep]["HDEP"];
    eAltPrel->Text = dmDB->TabPostazioni[MainForm->PlcFascia.posdep]["HPREL"];

    EscludiBtn->Enabled = (MainForm->pwdlevel) ? true : false;

    Timer1Timer(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormBaiaScarico::Timer1Timer(TObject *Sender)
{
    Timer1->Enabled = false;


    GroupBox3->Enabled = (MainForm->pwdlevel || MainForm->Simula);
    GroupBox4->Enabled = (MainForm->pwdlevel || MainForm->Simula);
    GroupBox8->Enabled = (MainForm->pwdlevel || MainForm->Simula);
    BitBtn6->Enabled = (MainForm->Simula);
    BitBtn1->Enabled = (MainForm->pwdlevel || MainForm->Simula);
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
//---------------------------------------------------------------------------

void TFormBaiaScarico::AggiornaCampiPlc() 
{
    TDatiSSCCPlc DatiScrittisuPlc;
	TRecordList TabPosizioni;
    AnsiString sSSCC;

	dmDB->CaricaTabella("posizioni where Pos = " + IntToStr(MainForm->PlcFascia.posdep), TabPosizioni);

    cbPrenotata->Checked = TabPosizioni[0]["PRENOTATA"].ToIntDef(0);
    cbEsclusa->Checked = TabPosizioni[0]["DISABILITA"].ToIntDef(0);
    
    cbScaDep->Checked = MainForm->PlcFascia.ChiamataDeposito;
    cbOkDep->Checked = MainForm->PlcFascia.OKingresso;
    cbFasciato->Checked = MainForm->PlcFascia.PalletAvvolto;

    cbRicIngresso->Checked = MainForm->PlcFascia.RichiestaIngresso;
    cbPrelevato->Checked = MainForm->PlcFascia.pallet_prelevato;
    cbDepositato->Checked = MainForm->PlcFascia.pallet_depositato;
    //cbLgvIngombro->Checked = MainForm->PlcFascia.IngombroDepositoAgv;

    oraAttivazione->Caption = TimeToStr(MainForm->PlcFascia.InizioAttesa);
}
void __fastcall TFormBaiaScarico::BitBtnChangeClick(TObject *Sender)
{
    AnsiString stringa;
    int statodisabilita, statoprenotata, altezzaDep, altezzaPrel;
	int res;
    
    if (Application->MessageBox(L"Sei sicuro di voler effettuare le modifiche sulla posizione?", L"Conferma!!!", MB_YESNO) != IDYES)
        return;
    
    statodisabilita = (cbEsclusa->Checked)  ? 1 : 0;
    statoprenotata = (cbPrenotata->Checked)  ? 1 : 0;
    altezzaDep = eAltDep->Text.ToIntDef(0);
    altezzaPrel = eAltPrel->Text.ToIntDef(0);
    
    dmDB->QueryPos->Close();
    dmDB->QueryPos->SQL->Clear();
    stringa.printf("UPDATE Posizioni SET Disabilita = %d , Prenotata = %d, HDep = %d, HPrel = %d  WHERE pos = %d ", statodisabilita, statoprenotata, altezzaDep, altezzaPrel, MainForm->PlcFascia.posdep);
    dmDB->QueryPos->SQL->Append(stringa);
    res = dmDB->QueryPos->ExecSQL();
    dmDB->QueryPos->Close();

    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
    MainForm->aggiornamappa = 1;
}
//---------------------------------------------------------------------------
//ingressi
void __fastcall TFormBaiaScarico::BitBtn6Click(TObject *Sender)
{
    if (cbScaDep->Checked)    //ChiamataDeposito
        MainForm->PlcFascia.StatoInput[ MainForm->PlcFascia.addr_ChiamataDeposito ] |= MainForm->bit[ MainForm->PlcFascia.bit_ChiamataDeposito ];
    else
        MainForm->PlcFascia.StatoInput[ MainForm->PlcFascia.addr_ChiamataDeposito ] &= MainForm->bitAnd[ MainForm->PlcFascia.bit_ChiamataDeposito ];
    
    if (cbOkDep->Checked)     //OKingresso
        MainForm->PlcFascia.StatoInput[ MainForm->PlcFascia.addr_OKingresso ] |= MainForm->bit[ MainForm->PlcFascia.bit_OKingresso ];
    else
        MainForm->PlcFascia.StatoInput[ MainForm->PlcFascia.addr_OKingresso ] &= MainForm->bitAnd[ MainForm->PlcFascia.bit_OKingresso ];
        
    if (cbFasciato->Checked)    //PalletAvvolto
        MainForm->PlcFascia.StatoInput[ MainForm->PlcFascia.addr_PalletAvvolto ] |= MainForm->bit[ MainForm->PlcFascia.bit_PalletAvvolto ];    
    else
        MainForm->PlcFascia.StatoInput[ MainForm->PlcFascia.addr_PalletAvvolto ] &= MainForm->bitAnd[ MainForm->PlcFascia.bit_PalletAvvolto ];
        
    MainForm->aggiornamappa = 1;

    dmDB->LogMsg("Forzati ingressi in deposito a " + IntToStr(MainForm->PlcFascia.StatoInput[0]) + " - " + IntToStr(MainForm->PlcFascia.StatoInput[1]));
}
//---------------------------------------------------------------------------
//uscite
void __fastcall TFormBaiaScarico::BitBtn1Click(TObject *Sender)
{
	MainForm->PlcFascia.uscitedascrivere = 0;
    
    //assegnazione in OR per non modificare l'eventuale stato attivo presente
    if (cbRicIngresso->Checked)
       MainForm->PlcFascia.uscitedascrivere |= MainForm->bit[MainForm->PlcFascia.bit_RichiestaIngresso];
                                                       
    if (cbDepositato->Checked)
       MainForm->PlcFascia.uscitedascrivere |= MainForm->bit[MainForm->PlcFascia.bit_pallet_depositato];

    if (cbPrelevato->Checked)
       MainForm->PlcFascia.uscitedascrivere |= MainForm->bit[MainForm->PlcFascia.bit_pallet_prelevato];

    //if (cbLgvIngombro->Checked)
    //   MainForm->PlcFascia.uscitedascrivere |= MainForm->bit[MainForm->PlcFascia.bit_IngombroDepositoAgv];

    MainForm->aggiornamappa = 1;

    dmDB->LogMsg("Forzate uscitedascrivere in deposito a " + IntToStr(MainForm->PlcFascia.uscitedascrivere));

   	MainForm->PlcFascia.forzatura_attiva = 1 ; //mi segno che sto forzando segnali
}
//---------------------------------------------------------------------------

void __fastcall TFormBaiaScarico::EscludiBtnClick(TObject *Sender)
{
	if (EscludiBtn->Down)
    { 
        EscludiLed->Color = clRed;    
        EscludiBtn->Caption = "ABILITA"; 
    }
    else
    {
        EscludiLed->Color = clBtnFace;    
        EscludiBtn->Caption = "DISABILITA"; 
    }

    MainForm->EscludiAvvolgitore = EscludiBtn->Down;
	dmDB->AggiornaStatoAgv(1);

    MainForm->aggiornamappa = 1;
}
//---------------------------------------------------------------------------


