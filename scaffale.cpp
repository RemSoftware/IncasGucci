//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "scaffale.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyShape"
#pragma resource "*.dfm"
TfrScaffale *frScaffale;
//---------------------------------------------------------------------------
__fastcall TfrScaffale::TfrScaffale(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
