// ---------------------------------------------------------------------------

#ifndef plcCaricoH
#define plcCaricoH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include "db.h"

// ---------------------------------------------------------------------------
class TFormPlcCarico : public TForm
{
__published: // IDE-managed Components

    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel4;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TSpeedButton *SpeedButton1;
    TTimer *Timer1;
    TGroupBox *GroupBox3;
    TCheckBox *ckDisabilitaPos;
    TCheckBox *cPrenotataPos;
    TPanel *Panel5;
    TLabel *Label20;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TGroupBox *GroupBox8;
    TLabel *Label26;
    TEdit *eH1;
    TGroupBox *GroupBox2;
    TLabel *Label1;
    TEdit *eH2;
    TLabel *Label2;
    TEdit *eUDC;
    TSpeedButton *CancellaUDCbtn;
    TLabel *Label3;

    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall cbTipoUDCChange(TObject *Sender);
    void __fastcall eUDCChange(TObject *Sender);
    void __fastcall cbStatusChange(TObject *Sender);
    void __fastcall ckDisabilitaPosClick(TObject *Sender);
    void __fastcall cPrenotataPosClick(TObject *Sender);
    void __fastcall SpeedButton3Click(TObject *Sender);
    void __fastcall CancellaUDCbtnClick(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TFormPlcCarico(TComponent* Owner);
    void VisualizzaDatiPosizione();
    void AggiornaCampiPlc(); 

    int pos, idplc, pos_plc, pos_struttura_plc, activate, input, output, plc_scarico_griglie, change_udc, cambiocheck;
    //DatiPlcIncappucciatore StructPlc; //StructPlcVisualizzata;
    int posizionecarico;
	int m_nOldPalletID;
    TRecordList m_TabPosizioni1;
    TRecordList m_TabPosizioni2;
    int m_nType;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormPlcCarico *FormPlcCarico;
// ---------------------------------------------------------------------------
#endif
