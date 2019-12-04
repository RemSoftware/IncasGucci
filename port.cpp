// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "port.h"
#include "SocketComunicazioneClient.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPortStatus *FormPortStatus;

// ---------------------------------------------------------------------------
__fastcall TFormPortStatus::TFormPortStatus(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TFormPortStatus::Timer1Timer(TObject *Sender)
{
    // aggiorno status
    int i;
    Timer1->Enabled = false;
    if (!forzatura_attiva) {
        if ((activate) || (SpeedButton1->Down) || ((!dmDB->pwdlevel) && (!ClientData.Simula))) {
            CheckBox1->Checked = ClientData.DatiPorte[idport].ApriPorta;

			ckInput1->Checked = ClientData.DatiPorte[idport].PortaAperta;
			ckInput2->Checked = ClientData.DatiPorte[idport].PortaTFuoco;



        }
        count_forzatura_attiva = 0;
    }
    else {
        if (count_forzatura_attiva > 10) {
            count_forzatura_attiva = 0;
            forzatura_attiva = false;
        }
        else {
            count_forzatura_attiva++;
        }
    }
                Edit3->Enabled = dmDB->pwdlevel;
            Edit4->Enabled = dmDB->pwdlevel;
            Edit5->Enabled = dmDB->pwdlevel;
    activate = false;
    Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPortStatus::BitBtn1Click(TObject *Sender)
{
    AnsiString stringa;
    dmDB->QueryPos->Close();
    dmDB->QueryPos->SQL->Clear();
    stringa = "Update Porte Set Descrizione = '" + Edit2->Text + "'";
    stringa = stringa + " , PosApriPorta = '" + Edit1->Text + "'";
    if (dmDB->pwdlevel) {
        stringa = stringa + " , PosPortaAperta = '" + Edit3->Text + "'";
        stringa = stringa + " , StrConsenso = '" + Edit4->Text + "'";
        stringa = stringa + " , PosTagliaFuoco = '" + Edit5->Text + "'";
    }
    stringa = stringa + " where IdPorta = " + IntToStr(idport);
    dmDB->QueryPos->SQL->Append(stringa);
    dmDB->QueryPos->ExecSQL();
    dmDB->QueryPos->Close();
    dmDB->aggiorna_tab_porte_locale = 1;
    dmDB->LogMsg("Aggiorna porte " + stringa);
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPortStatus::FormActivate(TObject *Sender)
{
    activate = true;

    Label4->Caption = IntToStr(idport);
    Label10->Caption = dmDB->TabPorte[IntToStr(idport)]["NOMEPORTA"];
    Edit1->Enabled = dmDB->pwdlevel;
    Edit3->Enabled = dmDB->pwdlevel;
    Edit4->Enabled = dmDB->pwdlevel;
    GroupBox1->Enabled = (ClientData.Simula || dmDB->pwdlevel);
    GroupBox2->Enabled = ClientData.Simula;
    Edit1->Text = dmDB->PosizioniRichiestaAperturaPorta(idport);
    Edit2->Text = dmDB->TabPorte[IntToStr(idport)]["DESCRIZIONE"];
    Edit3->Text = dmDB->PosizioniPortaAperta(idport);
    Edit4->Text = dmDB->StrConsensoPorta(idport);
    Edit5->Text = dmDB->StrConsensoPortaTFuoco(idport);
    RadioGroup2->ItemIndex = dmDB->StatoPorta(idport);
    RadioGroup2->Enabled = dmDB->pwdlevel;
    SpeedButton1->Down = !ClientData.Simula;
    forzatura_attiva = false;
    Timer1Timer(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPortStatus::FormClose(TObject *Sender,
    TCloseAction &Action)
{
    Timer1->Enabled = false;

}

// ---------------------------------------------------------------------------
void __fastcall TFormPortStatus::FormDeactivate(TObject *Sender)
{
    Close();
}

// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::BitBtn3Click(TObject *Sender)
{
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::RadioGroup2Click(TObject *Sender)
{
    AnsiString stringa, str;
    if (!activate) {
        SocketDataModule->InviaStatoPorta(idport, RadioGroup2->ItemIndex);
        dmDB->LogMsg("Modificata Modalità Porta " + IntToStr(idport) + " in : " + IntToStr(RadioGroup2->ItemIndex));
    }

}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::ckInput1Click(TObject *Sender)
{
    int i, val;
    TCheckBox *Check;
    if ((!activate) && (ClientData.Simula)) {
        val = 0;
        for (i = 0; i < 4; i++) {
            Check = (TCheckBox*) FindComponent("ckInput" + IntToStr(i + 1));
            if ((Check != NULL) && (Check->Checked))
                val |= dmDB->bit[i];
        }
        ClientData.DatiPorte[idport].forzature_attive = 1;
        SocketDataModule->InviaSimulazioneInputPorta(idport, 0, val);

    }

}
// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::BitBtn2Click(TObject *Sender)
{
	int i, val;
    TCheckBox *Check;
	if ((!activate) && (dmDB->pwdlevel)) {
		val = 0;
        for (i = 0; i < 4; i++) {
            Check = (TCheckBox*) FindComponent("CheckBox" + IntToStr(i + 1));
            if ((Check != NULL) && (Check->Checked))
                val |= dmDB->bit[i];
        }
        ClientData.DatiPorte[idport].forzature_attive = 1;
        SocketDataModule->InviaUscitePorta(idport, 0, val);
    }

}
// ---------------------------------------------------------------------------

