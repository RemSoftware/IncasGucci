//---------------------------------------------------------------------------

#ifndef BaiaScaricoH
#define BaiaScaricoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
//#include <VCLTee.TeCanvas.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormBaiaScarico : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel3;
    TImage *Image1;
    TShape *Shape2;
    TGroupBox *GroupBox3;
    TCheckBox *cbScaDep;
    TCheckBox *cbOkDep;
    TGroupBox *GroupBox4;
    TCheckBox *cbRicIngresso;
    TGroupBox *GroupBox8;
    TLabel *Label1;
    TCheckBox *cbPrenotata;
    TCheckBox *cbEsclusa;
    TEdit *eAltDep;
    TPanel *Panel2;
    TBitBtn *BitBtnClose;
    TBitBtn *BitBtnChange;
    TTimer *Timer1;
    TSpeedButton *SpeedButton1;
    TBitBtn *BitBtn6;
    TBitBtn *BitBtn1;
    TSpeedButton *EscludiBtn;
    TLabel *Label5;
    TEdit *EscludiLed;
    TCheckBox *cbFasciato;
    TCheckBox *cbDepositato;
    TCheckBox *cbPrelevato;
    TEdit *eAltPrel;
    TLabel *Label2;
    TLabel *oraAttivazione;
    TLabel *Label4;
    void __fastcall BitBtnCloseClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtnChangeClick(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall EscludiBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormBaiaScarico(TComponent* Owner);
    void AggiornaCampiPlc(); 
    void EmptyTexts();

	bool activate ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBaiaScarico *FormBaiaScarico;
//---------------------------------------------------------------------------
#endif
