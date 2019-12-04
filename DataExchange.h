// ---------------------------------------------------------------------------

#ifndef DataExchangeH
#define DataExchangeH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>

// ---------------------------------------------------------------------------
class TDataForm : public TForm
{
__published: // IDE-managed Components

    TPanel *Panel2;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn4;
    TBitBtn *btRefresh;
    TPanel *Panel1;
    TLabel *lb09;
    TEdit *ed09;
    TMemo *Memo1;
    TEdit *ed01;
    TLabel *lb01;
    TEdit *ed02;
    TLabel *lb03;
    TCheckBox *cb08;
    TCheckBox *cb10;
    TComboBox *ComboBox1;
    TComboBox *ComboBox2;
    TADOQuery *QueryDataE;
    TADOQuery *QueryCampi;
    TLabel *lb06;
    TLabel *Label1;

    void __fastcall FormActivate(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);

private: // User declarations
        public : // User declarations

    AnsiString NomeObj(AnsiString Funzione, int ID);
    int TipoObj(AnsiString Funzione, int ID);
    AnsiString TestoObj(AnsiString Funzione, int ID);
    int StartObj(AnsiString Funzione, int ID);
    int LenObj(AnsiString Funzione, int ID);
    AnsiString DefaultObj(AnsiString Funzione, int ID);
    bool EnableObj(AnsiString Funzione, int ID);
    int ColoreObj(AnsiString Funzione, int ID);
    AnsiString Traduci(AnsiString Funzione, int ID)  ;

    void SetLabel(AnsiString Funzione,AnsiString ID);
    void SetCheck(AnsiString Funzione,AnsiString ID);
    void SetEnable(AnsiString Funzione,AnsiString tipo, AnsiString ID);
    void SetColor(AnsiString Funzione,AnsiString ID);
    void SetDefault(AnsiString Funzione,AnsiString ID);
    bool SetLingua(AnsiString Funzione,AnsiString ID)  ;
    void RiempiStruttura(AnsiString Funzione);
    void CompilaForm(TObject * Sender, AnsiString Funzione)    ;

    __fastcall TDataForm(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TDataForm *DataForm;
// ---------------------------------------------------------------------------
#endif
