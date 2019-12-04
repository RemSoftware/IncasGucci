//---------------------------------------------------------------------------

#ifndef PosVuotiH
#define PosVuotiH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "db.h"
#include "Main.h"
//---------------------------------------------------------------------------
class TPosVuotiForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel4;
    TBitBtn *BitBtn2;
    TSpeedButton *SpeedButton1;
    TGroupBox *GroupBox3;
    TCheckBox *cbRichiestaIncrocio;
    TBitBtn *BitBtn6;
    TGroupBox *GroupBox4;
    TCheckBox *cbConsensoIncrocio;
    TBitBtn *BitBtn1;
    TTimer *Timer1;
	void __fastcall OnActivate(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
          



private:	// User declarations
public:		// User declarations
	__fastcall TPosVuotiForm(TComponent* Owner);
	int pos, h_pos_prel, h_pos_dep, num_in_pos;
    int m_nType;
	TRecordList TabPosizioni;
	bool activate;

    void AggiornaCampiPlc(); 
};
//---------------------------------------------------------------------------
extern PACKAGE TPosVuotiForm *PosVuotiForm;
//---------------------------------------------------------------------------
#endif
