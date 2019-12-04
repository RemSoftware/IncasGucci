// ---------------------------------------------------------------------------

#ifndef configCBFrameH
#define configCBFrameH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>

// ---------------------------------------------------------------------------
class TfrCB : public TFrame
{
__published: // IDE-managed Components

    TGroupBox *GroupBox1;
    TCheckBox *CheckBox1;
    TComboBox *ComboBox1;

private: // User declarations
        public : // User declarations

    __fastcall TfrCB(TComponent* Owner);

    bool aggiornamento;
    int idagv;

    void RiempiCampi(int idparametro);
    int SalvaCampi(int idparametro, int vip);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrCB *frCB;
// ---------------------------------------------------------------------------
#endif
