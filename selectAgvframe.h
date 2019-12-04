//---------------------------------------------------------------------------

#ifndef selectAgvframeH
#define selectAgvframeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "DB.h"
//---------------------------------------------------------------------------
class TfrSelectAgv : public TFrame
{
__published:	// IDE-managed Components
    TPanel *mainPanel;
    TGroupBox *GroupBox1;
    TCheckBox *cbAgv2;
    TCheckBox *cbAgv7;
    TCheckBox *cbAgv6;
    TCheckBox *cbAgv5;
    TCheckBox *cbAgv4;
    TCheckBox *cbAgv3;
    TCheckBox *cbAgv1;
    TLabel *lbTitolo;
    TComboBox *cbPriority1;
    TLabel *Label7;
private:	// User declarations
public:		// User declarations
	__fastcall TfrSelectAgv(TComponent* Owner);
    void  RiempiCampi(int idparametro);
    void SalvaCampi(int idparametro)     ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrSelectAgv *frSelectAgv;
//---------------------------------------------------------------------------
#endif
