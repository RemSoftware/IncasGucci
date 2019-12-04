//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChildWin.h"
#include "main.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
//--------------------------------------------------------------------- 
__fastcall TMDIChild::TMDIChild(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------

void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	//SetInvisible();
	Action = caFree;
}
//---------------------------------------------------------------------

void __fastcall TMDIChild::BitBtnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::AfterConstruction(void)
{
	FFormState >> fsVisible;
	TForm::AfterConstruction();
}

void  TMDIChild::SetVisible()
{
	FFormState << fsVisible;
	Visible = true;
}
//---------------------------------------------------------------------------

void  TMDIChild::SetInvisible()
{
	FFormState >> fsVisible;
}
//---------------------------------------------------------------------------

void  TMDIChild::Mostra()
{
	SetVisible();
	Visible = true;
//	Invalidate();
//	InvalidateRect(Handle, NULL, TRUE);
//	RedrawWindow(Handle, NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
}
//---------------------------------------------------------------------------


