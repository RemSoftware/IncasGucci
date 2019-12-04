// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "udclist.h"
#include "plcCarico.h"
#include "PLCThread.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPlcCarico *FormPlcCarico;

void TFormPlcCarico::VisualizzaDatiPosizione() 
{
	TIndexList TabUDC;
	int i;
    dmDB->CaricaTabella("Posizioni where Pos = " + IntToStr(pos) + " AND Piano = 1", m_TabPosizioni1);
    dmDB->CaricaTabella("Posizioni where Pos = " + IntToStr(pos) + " AND Piano = 2", m_TabPosizioni2);
    
	//dmDBClient->FullTabellaK("Select *, ARTICOLI.DESCRIZIONE as [descart], ArticoliUDC.Codice as [CARTUDC] from UDC INNER JOIN TabTipoUDC ON UDC.CodTipoUDC = TabTipoUDC.IDTipoUDC LEFT OUTER JOIN ArticoliUDC ON UDC.IDArtUDC = ArticoliUDC.IDArtUDC LEFT OUTER JOIN Articoli ON ArticoliUDC.IDArticolo = Articoli.IDArticolo ORDER BY UDC.IDUDC", "IDUDC", TabUDC);

	/*for (i = 0; i < m_TabPosizioni.size(); i++) {
		Label22->Caption = m_TabPosizioni[i]["POS"];
		Label24->Caption = m_TabPosizioni[i]["CUSTOMERPOS"];
		eHPrel->Text = m_TabPosizioni[i]["HPREL"];
		ckDisabilitaPos->Checked = m_TabPosizioni[i]["DISABILITA"].ToIntDef(0);
		cPrenotataPos->Checked = m_TabPosizioni[i]["PRENOTATA"].ToIntDef(0);
		eUDC->Text = m_TabPosizioni[i]["IDUDC"].ToIntDef(0);
	}*/

    Label22->Caption = StrToInt(pos); //dmDB->TabPostazioni[pos]["POS"];
    Label24->Caption = m_TabPosizioni1[0]["CUSTOMERPOS"];
    Label3->Caption = m_TabPosizioni2[0]["CUSTOMERPOS"];
/*
    if (m_nType == DEPVUOTI_TYPE)
        eHPrel->Text = dmDB->TabPostazioni[pos]["HDEP"];
    else
    {
        eHPrel->Text = dmDB->TabPostazioni[pos]["HPREL"];
        attesaPrelievo->Text = dmDB->TabParametri[104]["INTVALUE"].ToIntDef(0);    //attesa -secondi- per il recupero del pallet dai robots
    }
*/
    ckDisabilitaPos->Checked = dmDB->TabPostazioni[pos]["DISABILITA"].ToIntDef(0);
    cPrenotataPos->Checked = dmDB->TabPostazioni[pos]["PRENOTATA"].ToIntDef(0);
    //eUDC->Text = dmDB->TabPostazioni[pos]["IDUDC"].ToIntDef(0);
    
    if (m_nType == LINEASCAR_TYPE)
    {
        eH1->Text = m_TabPosizioni1[0]["HDEP"];
        eH2->Text = m_TabPosizioni2[0]["HDEP"];
    }
    else
    {
        eH1->Text = m_TabPosizioni1[0]["HPREL"];
        eH2->Text = m_TabPosizioni2[0]["HPREL"];
    }
}


// ---------------------------------------------------------------------------
__fastcall TFormPlcCarico::TFormPlcCarico(TComponent* Owner)
	: TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlcCarico::FormActivate(TObject *Sender)
{
	//
	//change_udc = false;
	activate = true;
    GroupBox2->Visible = true;

    if (m_nType == LINEASCAR_TYPE)
    {
        Label26->Caption = "H.Dep";
        Label1->Caption = "H.Dep";
    }
    else
    {
        if (pos == FASCIATORE)
            GroupBox2->Visible = false;
            
        Label26->Caption = "H.Prel";
        Label1->Caption = "H.Prel";
    }

	//SpeedButton1->Down = !ClientData.Simula;

	VisualizzaDatiPosizione();
	//m_nOldPalletID = StructPlc.PrelievoINC.IdUdcLetto;

	// input
	plc_scarico_griglie = 0;
	cambiocheck = 0;
	TCheckBox *OggettoCheck;
	AnsiString TempString;
	AnsiString Funzione = "PLCSIGNAL";
    //StructPlc = ClientData.PlcIncappucciatore[0];

	Timer1Timer(this);
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::Timer1Timer(TObject *Sender)
{
	TCheckBox *Check;
	AnsiString stringa_hex, sCod;
	int i, indice_edit;
	TEdit *frLoad;
	// aggiorno
	Timer1->Enabled = false;
	GroupBox2->Enabled = true;//MainForm->Simula;
	GroupBox3->Enabled = true;//MainForm->pwdlevel;
	GroupBox2->Enabled = true;//MainForm->pwdlevel;
	GroupBox8->Enabled = true;//MainForm->pwdlevel;

    
	if ((activate) || (SpeedButton1->Down) || ((!MainForm->pwdlevel) && (!MainForm->Simula))) {
		AggiornaCampiPlc();
	} 

	activate = false;
	Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------

void TFormPlcCarico::AggiornaCampiPlc() 
{
/*
	int nRob;
	TCheckBox *cb;
	AnsiString stringa_hex;

    if (MainForm->PlcRobot.PlcConnesso)
        Shape4->Brush->Color = clLime;
    else
        Shape4->Brush->Color = clRed;  
        
    //lWatchDogPlc->Caption = IntToStr( PLCThread[2]->watchdog_plc );
    //lWatchDogPc->Caption = IntToStr( PLCThread[2]->watchdog_pc );
    //lCycle->Caption = PLCThread[2]->CycleTime;
    //lErrors->Caption = PLCThread[2]->ErrorString;
        
    //postazione vuoti
    if (m_nType == LINEASCAR_TYPE)
    {
        ckInput1->Checked = MainForm->PlcRobot.richiesta_vuoti;
        ckInput2->Checked = MainForm->PlcRobot.OK_ingresso_vuoti;

        ckOutput1->Checked = MainForm->PlcRobot.richiesta_ingresso_vuoti;
        ckOutput3->Checked = MainForm->PlcRobot.vuoto_depositato;
    }
    //Postazioni robots
    else
    {
        nRob =  pos / 10;
        ckInput1->Checked = MainForm->PlcRobot.presenza_pallet_linea[nRob];
        ckInput2->Checked = MainForm->PlcRobot.OK_ingresso_linea[nRob];
        ckInput3->Checked = MainForm->PlcRobot.pallet_incompleto[nRob];

        ckOutput1->Checked = MainForm->PlcRobot.richiesta_ingresso_linea[nRob];
        ckOutput3->Checked = MainForm->PlcRobot.pallet_prelevato_linea[nRob];
        //eUDC->Text = IntToStr(StructPlc.PrelievoINC.IdUdcLetto);  //
    }
    */
}

void __fastcall TFormPlcCarico::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::BitBtn5Click(TObject *Sender)
{
    int nH2, nH1, nPos;
    String sMsg;
    
	if ((MainForm->pwdlevel))
    {
        sMsg = "Confermi la Modifica dei dati della Posizione ?";
		if (Application->MessageBox(sMsg.c_str(), L"Conferma!!!", MB_YESNO) == IDYES)
        {
            nH1 = eH1->Text.ToIntDef(0);
            nH2 = eH2->Text.ToIntDef(0);
            
            if (pos == FASCIATORE)  // Solo il 1 piano
                dmDB->AggiornaHDepHPrelPerPos(pos, nH1, nH2, true);
            else
                dmDB->AggiornaHDepHPrelPerPos(pos, nH1, nH2, false);
                
            dmDB->UpdateSoloStato(pos, cPrenotataPos->Checked, ckDisabilitaPos->Checked);
            
            dmDB->CaricaTabelle();
            dmDB->AggiornaTabParametri();
            MainForm->aggiornamappa = 1;
		}
    }
	else 
    {
        ShowMessage(L"L'operatore NON ha i permessi per fare questa modifica!");
	}  
    
	Close();    
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::BitBtn4Click(TObject *Sender) {
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::BitBtn6Click(TObject *Sender)
{
/*
	// forzo input in simulazione
	int nRob, udc;
	TCheckBox *cb;

	if (MainForm->Simula) 
    {
        //postazione vuoti
        if (m_nType == LINEASCAR_TYPE)
        {
            if (ckInput1->Checked)    //Richiesta vuoti
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_richiesta_vuoti] |= MainForm->bit[MainForm->PlcRobot.bit_richiesta_vuoti];    
            else
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_richiesta_vuoti] &= MainForm->bitAnd[MainForm->PlcRobot.bit_richiesta_vuoti];
        
            if (ckInput2->Checked)    //Consenso ingresso vuoti
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_OK_ingresso_vuoti] |= MainForm->bit[MainForm->PlcRobot.bit_OK_ingresso_vuoti];    
            else
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_OK_ingresso_vuoti] &= MainForm->bitAnd[MainForm->PlcRobot.bit_OK_ingresso_vuoti];
        
            dmDB->LogMsg("Forzati ingressi zona vuoti a " + IntToStr(MainForm->PlcRobot.StatoInput[0]) + " - " + IntToStr(MainForm->PlcRobot.StatoInput[1]) + " - " + 
                                                            IntToStr(MainForm->PlcRobot.StatoInput[2]) + " - " + IntToStr(MainForm->PlcRobot.StatoInput[3]));
        }
        //Postazioni robots
        else
        {
            nRob =  pos / 10;
            
            if (ckInput1->Checked)    //Richiesta prelievo pallet
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_presenza_pallet[nRob]] |= MainForm->bit[MainForm->PlcRobot.bit_presenza_pallet[nRob]];    
            else
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_presenza_pallet[nRob]] &= MainForm->bitAnd[MainForm->PlcRobot.bit_presenza_pallet[nRob]];

            if (ckInput2->Checked)    //OK Ingresso AGV
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_OK_ingresso_linea[nRob]] |= MainForm->bit[MainForm->PlcRobot.bit_OK_ingresso_linea[nRob]];    
            else
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_OK_ingresso_linea[nRob]] &= MainForm->bitAnd[MainForm->PlcRobot.bit_OK_ingresso_linea[nRob]];

            if (ckInput3->Checked)    //Pallet Incompleto
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_pallet_incompleto[nRob]] |= MainForm->bit[MainForm->PlcRobot.bit_pallet_incompleto[nRob]];    
            else
                MainForm->PlcRobot.StatoInput[MainForm->PlcRobot.addr_pallet_incompleto[nRob]] &= MainForm->bitAnd[MainForm->PlcRobot.bit_pallet_incompleto[nRob]];

            dmDB->LogMsg("Forzati ingressi zone Robots a " + IntToStr(MainForm->PlcRobot.StatoInput[0]) + " - " + IntToStr(MainForm->PlcRobot.StatoInput[1]) + " - " + 
                                                             IntToStr(MainForm->PlcRobot.StatoInput[2]) + " - " + IntToStr(MainForm->PlcRobot.StatoInput[3]));
        }
        MainForm->aggiornamappa = 1;
	} 
    */
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::BitBtn1Click(TObject *Sender)
{
	// forzo output
	int i;
	TCheckBox *cb;

	/*if (MainForm->pwdlevel) {
		output = 0;

		for (i = 0; i < 2; i++) {
			cb = (TCheckBox*) FindComponent("ckOutput" + IntToStr(i + 1));
			if ((cb != NULL) && (cb->Checked))
				output |= dmDB->bit[i];
		}
		SocketDataModule->InviaUscitePlc(pos, 0, output);    // Il byte (0) non è considerato perché lo prende il server dalle sue strutture (c'è solo 1 PLC)
	}                                                        // lo uso per trasmettere l'UDC negli input
     */
}

// ---------------------------------------------------------------------------
//non c'è il BOTTONE
void __fastcall TFormPlcCarico::SpeedButton2Click(TObject *Sender)
{
/*	FormUDCList->blocca_insert = false;
	FormUDCList->pos = pos;
	FormUDCList->ShowModal();
	FormActivate(this);  */
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::cbTipoUDCChange(TObject *Sender)
{
	if (!activate)
		change_udc = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::eUDCChange(TObject *Sender)
{
	if (!activate)
		change_udc = true;

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::cbStatusChange(TObject *Sender)
{
	if (!activate)
		change_udc = true;

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::ckDisabilitaPosClick(TObject *Sender)
{
	if (!activate)
		cambiocheck = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::cPrenotataPosClick(TObject *Sender)
{
	if (!activate)
		cambiocheck = true;

}
// ---------------------------------------------------------------------------
// NON c'é IL BOTTONE

void __fastcall TFormPlcCarico::SpeedButton3Click(TObject *Sender)
{
 /*	TUDC UdcMod;
	if ((eUDC->Text.ToIntDef(0)) && (MainForm->pwdlevel)) {
		dmDB->LeggiStrutturaUdc(UdcMod);
		dmDB->LeggiStrutturaUdc(UdcMod);
		if (MainForm->pwdlevel) {
			if (Application->MessageBox(L"Are you sure to delete this Item?", L"Confirm!!!", MB_YESNO) == IDYES) {
				eUDC->Text = "0";
				UdcMod.stato = 0;
				UdcMod.CodTipoUDC = 0;
				dmDB->InsertUpdateUDC(UdcMod);
				dmDB->UpdatePosNoStato(pos, eUDC->Text.ToIntDef(0), eHPrel->Text.ToIntDef(5), eHDep->Text.ToIntDef(65));

			}
		}

		FormActivate(this);
	}   */
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcCarico::CancellaUDCbtnClick(TObject *Sender)
{

	if ((eUDC->Text.ToIntDef(0)) && (MainForm->pwdlevel)) 
    {
        if (Application->MessageBox(L"Sei sicuro di voler cancellare l'UDC?", L"Conferma!!!", MB_YESNO) == IDYES) 
        {
            eUDC->Text = "0";
            dmDB->SvuotaPosizione(Label24->Caption);
            dmDB->CaricaTabelle();
        }
	    FormActivate(this);
	}
}
//---------------------------------------------------------------------------

