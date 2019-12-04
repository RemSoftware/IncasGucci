// ---------------------------------------------------------------------------

#ifndef UDClistH
#define UDClistH
// ---------------------------------------------------------------------------
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
#include <Vcl.ComCtrls.hpp>

// ---------------------------------------------------------------------------
class TFormUDCList : public TForm
{
__published: // IDE-managed Components

    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TDBGrid *DBGrid1;
    TADOQuery *ADOQuery1;
    TDataSource *DataSource1;
    TPanel *Panel5;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn4;
    TGroupBox *GroupBox1;
    TEdit *Edit1;
    TLabel *Label1;
    TLabel *Label3;
    TEdit *Edit3;
    TLabel *Label6;
    TLabel *Tot;
    TBitBtn *BitBtnEdit;
    TBitBtn *BitBtnAdd;
    TEdit *Edit2;
    TLabel *Label2;

    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall DBGrid1DrawColumnCell(TObject *Sender, const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Edit1Change(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtnEditClick(TObject *Sender);
    void __fastcall BitBtnAddClick(TObject *Sender);
    void __fastcall ADOQuery1AfterScroll(TDataSet *DataSet)  ;

private: // User declarations
        public : // User declarations
    __fastcall TFormUDCList(TComponent* Owner);

    int pos, blocca_insert, udcsel;
    bool activate;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormUDCList *FormUDCList;
// ---------------------------------------------------------------------------
#endif
