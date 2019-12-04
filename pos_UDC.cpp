// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "pos_UDC.h"
#include "stato_pos.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyShape"
#pragma resource "*.dfm"
TfrPosUDC *frPosUDC;

// ---------------------------------------------------------------------------
__fastcall TfrPosUDC::TfrPosUDC(TComponent* Owner)
    : TFrame(Owner)
{
}
// ---------------------------------------------------------------------------

void TfrPosUDC::RiempiCampi(TRecordList TabPiani, int prenotata, int esclusa) {
    TMyShape *MyShape;
    int idudc, npiani, Height;
    // if (TabPiani.size() > 3)
    // npiani = 3;
    // else
    npiani = TabPiani.size();

    if (TabPiani.size() > 0) {
        Height = 240 / npiani;
        if (Height>80) Height=80;
        for (int k = 1; k <= npiani; k++) {
            MyShape = (TMyShape*)(FindComponent("MyShape" + IntToStr(k)));
            idudc = TabPiani[k - 1]["IDUDC"].ToIntDef(0);
            if (MyShape == NULL) {
                MyShape = new TMyShape(this);
                 MyShape->Parent = this->Panel1;
                MyShape->Shape = mstRectangle;
                MyShape->Width = 43;
                MyShape->Left = 7;
                MyShape->Visible = true;
                MyShape->Name = "MyShape" + IntToStr(k);
            }
            MyShape->Tag = TabPiani[k - 1]["POS"].ToIntDef(0);
            MyShape->Height = Height;
            MyShape->Top = 270 - (k * MyShape->Height);
            //  MyShape->OnMouseUp = MyShape1MouseUp;

            if (idudc > 0) {
                MyShape->Hint = TabPiani[k - 1]["IDUDC"].ToIntDef(0);
                MyShape->Visible = true;
                MyShape->Brush->Color = clWebOrange; // dmDBClient->ReturnaColoreDaUDC(TabPosizioni[i]["IDUDC"].ToIntDef(0));
                MyShape->Text = MyShape->Hint;

            }
            else {
                MyShape->Brush->Style = bsClear;
                MyShape->Text = "...";
                MyShape->Hint = "...";
                MyShape->Pen->Color = clHotLight;
                MyShape->Pen->Width = 1;
            }
            if (prenotata) {
                MyShape->Pen->Color = clBlue;
                MyShape->Pen->Width = 3;
            }
            else if (esclusa) {
                MyShape->Pen->Color = clRed;
                MyShape->Pen->Width = 3;
            }
            else {
                MyShape->Pen->Color = clAqua;
                MyShape->Pen->Width = 1;
            }
            if (TabPiani[k - 1]["DISABILITATA"].ToIntDef(0)) {
                MyShape->Visible = true;
                MyShape->Brush->Style = bsHorizontal;
                MyShape->Brush->Color = clBlack; // dmDBClient->ReturnaColoreDaUDC(TabPosizioni[i]["IDUDC"].ToIntDef(0));
                MyShape->Text = "Disab.";
            }

        }
        // ANNERISCI I PIANI MANCANTI
        for (int k = npiani + 1; k <= 3; k++) {
            MyShape = (TMyShape*)(FindComponent("MyShape" + IntToStr(k)));
            if (MyShape == NULL) {
                MyShape = new TMyShape(this);
                // MyShape->Parent = this->Panel1;
                MyShape->Name = "MyShape" + IntToStr(k + 1);
            }
            MyShape->Visible = true;
            MyShape->Brush->Color = clBlack;

        }
    }

}
void __fastcall TfrPosUDC::MyShape1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  // 	TMenuItem *Item = (TMenuItem*) Sender;
  	fCorsia->frScaffale1MyShape4MouseUp(Sender,  Button,  Shift,  X,  Y);
}
//---------------------------------------------------------------------------

