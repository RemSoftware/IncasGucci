//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PosVuoti.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPosVuotiForm *PosVuotiForm;
//---------------------------------------------------------------------------
__fastcall TPosVuotiForm::TPosVuotiForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TPosVuotiForm::OnActivate(TObject *Sender)
{
    activate = true;
}
//---------------------------------------------------------------------------


void __fastcall TPosVuotiForm::BitBtn4Click(TObject *Sender)
{
    SpeedButton1->Down = true;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TPosVuotiForm::BitBtn1Click(TObject *Sender)
{
	MainForm->PlcFascia.uscitedascrivere = 0;
    
    //assegnazione in OR per non modificare l'eventuale stato attivo presente
    if (cbConsensoIncrocio->Checked)
       MainForm->PlcIncas.uscitedascrivere |= MainForm->bit[MainForm->PlcIncas.bit_consenso_impegno_incrocio];

    MainForm->aggiornamappa = 1;

    dmDB->LogMsg("Forzate uscitedascrivere  a " + IntToStr(MainForm->PlcIncas.uscitedascrivere));

   	MainForm->PlcFascia.forzatura_attiva = 1 ; //mi segno che sto forzando segnali
}
//---------------------------------------------------------------------------

void __fastcall TPosVuotiForm::BitBtn6Click(TObject *Sender)
{
    if (cbRichiestaIncrocio->Checked)    //ChiamataDeposito
        MainForm->PlcIncas.StatoInput |= MainForm->bit[ MainForm->PlcIncas.bit_richiesta_impegno_incrocio ];
    else
        MainForm->PlcIncas.StatoInput &= MainForm->bitAnd[ MainForm->PlcIncas.bit_richiesta_impegno_incrocio ];
    
        
    MainForm->aggiornamappa = 1;

    dmDB->LogMsg("Forzati ingressi a " + IntToStr(MainForm->PlcIncas.StatoInput));
}
//---------------------------------------------------------------------------

void __fastcall TPosVuotiForm::Timer1Timer(TObject *Sender)
{
	TCheckBox *Check;
	AnsiString stringa_hex, sCod;
	int i, indice_edit;
	TEdit *frLoad;
	// aggiorno
	Timer1->Enabled = false;

    
	if ((activate) || (SpeedButton1->Down) || ((!MainForm->pwdlevel) && (!MainForm->Simula))) {
		AggiornaCampiPlc();
	} 

	activate = false;
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void TPosVuotiForm::AggiornaCampiPlc() 
{
    cbRichiestaIncrocio->Checked = MainForm->PlcIncas.richiesta_impegno_incrocio;

    cbConsensoIncrocio->Checked = MainForm->PlcIncas.consenso_impegno_incrocio;
}
//---------------------------------------------------------------------------

