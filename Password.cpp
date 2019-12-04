//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Password.h"
#include "Main.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TPasswordDlg *PasswordDlg;
//---------------------------------------------------------------------
__fastcall TPasswordDlg::TPasswordDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------

void __fastcall TPasswordDlg::FormActivate(TObject *Sender)
{
	Password->SetFocus();	
}
//---------------------------------------------------------------------------


