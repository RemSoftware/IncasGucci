//---------------------------------------------------------------------------

#ifndef ChiusuraH
#define ChiusuraH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TChiusuraForm : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn5;
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BitBtn5Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TChiusuraForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TChiusuraForm *ChiusuraForm;
//---------------------------------------------------------------------------
#endif
