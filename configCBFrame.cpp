// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "configCBFrame.h"
#include "main.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrCB *frCB;

// ---------------------------------------------------------------------------
__fastcall TfrCB::TfrCB(TComponent* Owner)
    : TFrame(Owner)
{
}
// ---------------------------------------------------------------------------

void TfrCB::RiempiCampi(int idparametro) {
    int val;

    TIndexList TabParametri;

    dmDB->CaricaTabellaK("Parametri order by IdParametro", "IdParametro", TabParametri);

    // colore
    if (idparametro % 2 == 0)
        GroupBox1->Color = clBtnFace;
    else
        GroupBox1->Color = clInactiveBorder;

    // assegna valori

    val = TabParametri[idparametro + 30]["INTVALUE"].ToIntDef(0);
    ComboBox1->Text = val;

    val = TabParametri[30]["INTVALUE"].ToIntDef(0);
    CheckBox1->Checked = val & dmDB->bit[idparametro - 1];

}

int TfrCB::SalvaCampi(int idparametro, int vip) {
    int ret = 0, val;
    // cosi' fa cagare ma il passaggio del vettore non funziona

    val = ComboBox1->Text.ToIntDef(0);
    // faccio l'update del parametro
    dmDB->AggiornaParametri(idparametro + 30, val);
    // devo inviare un messaggio al server per ricaricare i parametri
   // SocketDataModule->InviaAggiornamentoParametri(30 + idparametro);

    // totale check box STATICCB
    if (CheckBox1->Checked)
        vip |= dmDB->bit[idparametro - 1];


    ret = vip;

    return ret;

}
