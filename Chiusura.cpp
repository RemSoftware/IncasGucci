//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Chiusura.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChiusuraForm *ChiusuraForm;
//---------------------------------------------------------------------------
__fastcall TChiusuraForm::TChiusuraForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TChiusuraForm::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
//	fMain->RestartTimer();
}
//---------------------------------------------------------------------------

void __fastcall TChiusuraForm::BitBtn5Click(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TChiusuraForm::BitBtn1Click(TObject *Sender)
{
	ModalResult = mrYes;
}
//---------------------------------------------------------------------------

void __fastcall TChiusuraForm::BitBtn2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TChiusuraForm::FormActivate(TObject *Sender)
{
	BitBtn1->Enabled = MainForm->TouchPanel;
}
//---------------------------------------------------------------------------

