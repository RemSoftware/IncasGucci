// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "selectAgvframe.h"
#include "SocketComunicazioneClient.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrSelectAgv *frSelectAgv;

// ---------------------------------------------------------------------------
__fastcall TfrSelectAgv::TfrSelectAgv(TComponent* Owner)
    : TFrame(Owner)
{
}
// ---------------------------------------------------------------------------

void TfrSelectAgv::RiempiCampi(int idparametro) {
    int i, id, val, j;

    TRecordList TabParametri;
    TfrSelectAgv *frSelAgv;
    TComboBox *Combo;
    TCheckBox *Check;
    TLabel *OggettoLabel;
    // colore
    if (idparametro % 2 == 0)
        mainPanel->Color = clBtnFace;
    else
        mainPanel->Color = clInactiveBorder;

    dmDB->CaricaTabella("Parametri order by IdParametro", TabParametri);
    // check agv
    val = TabParametri[idparametro]["INTVALUE"].ToIntDef(0);
    //assegna valori
    for (i = 1; i <= NAGV; i++) {
        Check = (TCheckBox*) FindComponent("cbAgv" + IntToStr(i));
        if (Check != NULL) {
            Check->Checked = val & dmDB->bit[i - 1];
        }
    }
    //disabilita gli AGV in eccesso
    for (i = NAGV + 1; i <= 7; i++) {
        Check = (TCheckBox*) FindComponent("cbAgv" + IntToStr(i));
        if (Check != NULL) {
            Check->Enabled = false;
            Check->Visible = false;
        }
    }
    //priorita
        cbPriority1->Text = TabParametri[idparametro + 8]["INTVALUE"].ToIntDef(0);
    // text
    lbTitolo->Caption = TabParametri[idparametro]["DESCRIZIONE"];



}

void TfrSelectAgv::SalvaCampi(int idparametro) {
    int i, id, val=0, j;
    TRecordList TabParametri;
    TfrSelectAgv *frSelAgv;
    TComboBox *Combo;
    TCheckBox *Check;
    TLabel *OggettoLabel;

    dmDB->CaricaTabella("Parametri order by IdParametro", TabParametri);

    for (i = 1; i <= NAGV; i++) {
        Check = (TCheckBox*) FindComponent("cbAgv" + IntToStr(i));
        if (Check != NULL)
            if (Check->Checked)
                val |= dmDB->bit[i - 1];
    }

    // faccio l'update del parametro
    dmDB->AggiornaParametri(idparametro + 1, val);
    // devo inviare un messaggio al server per ricaricare i parametri
    SocketDataModule->InviaAggiornamentoParametri(idparametro + 1);

    // faccio l'update del parametro
    dmDB->AggiornaParametri(idparametro + 11, cbPriority1->Text.SubString(1, 1).ToIntDef(0));
    // devo inviare un messaggio al server per ricaricare i parametri
    SocketDataModule->InviaAggiornamentoParametri(idparametro + 11);

}
