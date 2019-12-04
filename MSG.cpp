//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MSG.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMSGForm *MSGForm;
//---------------------------------------------------------------------------
__fastcall TMSGForm::TMSGForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void ShowMSG(AnsiString msg) {
	MSGForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMSGForm::BitBtnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

