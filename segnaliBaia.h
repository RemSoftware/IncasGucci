//---------------------------------------------------------------------------

#ifndef segnaliBaiaH
#define segnaliBaiaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Samples.Spin.hpp>
//---------------------------------------------------------------------------
class TfSegnaliBaia : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TTimer *Timer1;
	TBitBtn *BitBtnChange;
	TTabControl *TabControl1;
	TPanel *Panel26;
	TCheckBox *CheckBox1;
	TTimer *TimerTestPlc;
	TPanel *Panel4;
	TPanel *Panel5;
	TPanel *Panel6;
	TGroupBox *gbInput1;
	TPanel *pName1_0;
	TShape *sInputLed1_0;
	TPanel *pName1_1;
	TShape *sInputLed1_1;
	TPanel *pName1_2;
	TShape *sInputLed1_2;
	TPanel *pName1_3;
	TShape *sInputLed1_3;
	TPanel *pName1_4;
	TShape *sInputLed1_4;
	TPanel *pName1_5;
	TShape *sInputLed1_5;
	TPanel *pName1_6;
	TShape *sInputLed1_6;
	TPanel *pName1_7;
	TShape *sInputLed1_7;
	TGroupBox *gbInput2;
	TPanel *pName2_0;
	TShape *sInputLed2_0;
	TPanel *pName2_1;
	TShape *sInputLed2_1;
	TPanel *pName2_2;
	TShape *sInputLed2_2;
	TPanel *pName2_3;
	TShape *sInputLed2_3;
	TPanel *pName2_4;
	TShape *sInputLed2_4;
	TPanel *pName2_5;
	TShape *sInputLed2_5;
	TPanel *pName2_6;
	TShape *sInputLed2_6;
	TPanel *pName2_7;
	TShape *sInputLed2_7;
	TGroupBox *GroupBox3;
	TPanel *pName3_0;
	TShape *sInputLed3_0;
	TPanel *pName3_1;
	TShape *sInputLed3_1;
	TPanel *pName3_2;
	TShape *sInputLed3_2;
	TPanel *pName3_3;
	TShape *sInputLed3_3;
	TPanel *pName3_4;
	TShape *sInputLed3_4;
	TPanel *pName3_5;
	TShape *sInputLed3_5;
	TPanel *pName3_6;
	TShape *sInputLed3_6;
	TPanel *pName3_7;
	TShape *sInputLed3_7;
	TGroupBox *GroupBox4;
	TPanel *pName4_0;
	TShape *sInputLed4_0;
	TPanel *pName4_1;
	TShape *sInputLed4_1;
	TPanel *pName4_2;
	TShape *sInputLed4_2;
	TPanel *pName4_3;
	TShape *sInputLed4_3;
	TPanel *pName4_4;
	TShape *sInputLed4_4;
	TPanel *pName4_5;
	TShape *sInputLed4_5;
	TPanel *pName4_6;
	TShape *sInputLed4_6;
	TPanel *pName4_7;
	TShape *sInputLed4_7;
	TPanel *Panel7;
	TPanel *Panel8;
	TGroupBox *GroupBox1;
	TPanel *pNameOutput1_0;
	TShape *sOutputLed1_0;
	TPanel *pNameOutput1_1;
	TShape *sOutputLed1_1;
	TPanel *pNameOutput1_2;
	TShape *sOutputLed1_2;
	TPanel *pNameOutput1_3;
	TShape *sOutputLed1_3;
	TPanel *pNameOutput1_4;
	TShape *sOutputLed1_4;
	TPanel *pNameOutput1_5;
	TShape *sOutputLed1_5;
	TPanel *pNameOutput1_6;
	TShape *sOutputLed1_6;
	TPanel *pNameOutput1_7;
	TShape *sOutputLed1_7;
	TGroupBox *GroupBox2;
	TPanel *pNameOutput2_0;
	TShape *sOutputLed2_0;
	TPanel *pNameOutput2_1;
	TShape *sOutputLed2_1;
	TPanel *pNameOutput2_2;
	TShape *sOutputLed2_2;
	TPanel *pNameOutput2_3;
	TShape *sOutputLed2_3;
	TPanel *pNameOutput2_4;
	TShape *sOutputLed2_4;
	TPanel *pNameOutput2_5;
	TShape *sOutputLed2_5;
	TPanel *pNameOutput2_6;
	TShape *sOutputLed2_6;
	TPanel *pNameOutput2_7;
	TShape *sOutputLed2_7;
	TGroupBox *GroupBox5;
	TPanel *pNameOutput3_0;
	TShape *sOutputLed3_0;
	TPanel *pNameOutput3_1;
	TShape *sOutputLed3_1;
	TPanel *pNameOutput3_2;
	TShape *sOutputLed3_2;
	TPanel *pNameOutput3_3;
	TShape *sOutputLed3_3;
	TPanel *pNameOutput3_4;
	TShape *sOutputLed3_4;
	TPanel *pNameOutput3_5;
	TShape *sOutputLed3_5;
	TPanel *pNameOutput3_6;
	TShape *sOutputLed3_6;
	TPanel *pNameOutput3_7;
	TShape *sOutputLed3_7;
	TGroupBox *GroupBox6;
	TPanel *pNameOutput4_0;
	TShape *sOutputLed4_0;
	TPanel *pNameOutput4_1;
	TShape *sOutputLed4_1;
	TPanel *pNameOutput4_2;
	TShape *sOutputLed4_2;
	TPanel *pNameOutput4_3;
	TShape *sOutputLed4_3;
	TPanel *pNameOutput4_4;
	TShape *sOutputLed4_4;
	TPanel *pNameOutput4_5;
	TShape *sOutputLed4_5;
	TPanel *pNameOutput4_6;
	TShape *sOutputLed4_6;
	TPanel *pNameOutput4_7;
	TShape *sOutputLed4_7;
	TPanel *Panel9;
	void __fastcall sOutputLed1_0MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall sInputLed1_0MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall TabControl1Change(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall TimerTestPlcTimer(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TfSegnaliBaia(TComponent* Owner);
	void ColoraShape(int stato, TShape *t, TColor ColorON = clLime, TColor ColorOFF = clGray);
	void ComandaPresenza(int modulo,int nled,int stato,bool input);
	int baia,activate ,forzadati;
};
//---------------------------------------------------------------------------
extern PACKAGE TfSegnaliBaia *fSegnaliBaia;
//---------------------------------------------------------------------------
#endif
