//---------------------------------------------------------------------------

#ifndef scaffaleH
#define scaffaleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------
class TfrScaffale : public TFrame
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TImage *Image1;
	TPanel *Panel2;
	TMyShape *MyShape1;
	TShape *Shape1;
private:	// User declarations
public:		// User declarations
	__fastcall TfrScaffale(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrScaffale *frScaffale;
//---------------------------------------------------------------------------
#endif
