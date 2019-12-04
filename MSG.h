//---------------------------------------------------------------------------

#ifndef MSGH
#define MSGH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TMSGForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TLabel *Label1;
	TBitBtn *BitBtnClose;
	void __fastcall BitBtnCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMSGForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMSGForm *MSGForm;
extern void ShowMSG(AnsiString msg);
//---------------------------------------------------------------------------
#endif
