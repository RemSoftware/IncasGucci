//---------------------------------------------------------------------------

#ifndef ins_UDCH
#define ins_UDCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormInsUDC : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TADOQuery *Query;
    TEdit *edIDUDC;
    TLabel *Label3;
    TLabel *Label1;
    TComboBox *cbTipoUDC;
    TEdit *edDescrArticolo;
    TLabel *Label2;
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btInsArticoloClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormInsUDC(TComponent* Owner);
	bool insert ;
	int idtabella;
    int m_nIDArticolo;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormInsUDC *FormInsUDC;
//---------------------------------------------------------------------------
#endif
