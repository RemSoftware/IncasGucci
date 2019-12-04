// ---------------------------------------------------------------------------

#ifndef pos_UDCH
#define pos_UDCH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "db.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>

// ---------------------------------------------------------------------------
class TfrPosUDC : public TFrame
{
__published: // IDE-managed Components

    TPanel *Panel1;
    TImage *Image1;
    TPanel *Panel2;
    TMyShape *MyShape1;
    TShape *Shape1;
    TMyShape *MyShape2;
    TMyShape *MyShape3;
    void __fastcall MyShape1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private: // User declarations
        public : // User declarations

    __fastcall TfrPosUDC(TComponent* Owner);
void RiempiCampi(TRecordList TabPiani, int prenotata, int esclusa);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrPosUDC *frPosUDC;
// ---------------------------------------------------------------------------
#endif
