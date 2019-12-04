//---------------------------------------------------------------------------

#ifndef UDCDuplicatiH
#define UDCDuplicatiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormUDCDuplicati : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel4;
    TDBGrid *DBGrid1;
    TADOQuery *ADOQuery1;
    TDataSource *DataSource1;
    TPanel *Panel5;
    TPanel *Panel6;
    TBitBtn *BitBtn8;
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormUDCDuplicati(TComponent* Owner);
	int insert_produzione;
	AnsiString TipoProdotto ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormUDCDuplicati *FormUDCDuplicati;
//---------------------------------------------------------------------------
#endif
