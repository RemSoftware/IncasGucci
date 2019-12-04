// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "stato_pos.h"
#include "main.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "MyShape"
#pragma link "scaffale"
#pragma resource "*.dfm"
TfCorsia *fCorsia;
int ccont = 0;

// ---------------------------------------------------------------------------
__fastcall TfCorsia::TfCorsia(TComponent* Owner) : TMDIChild(Owner) 
{
     dmDB->RiempiTipologiaCorsia(cbTipo, 0);
}

// ---------------------------------------------------------------------------
void TfCorsia::VisualizzaScaffale() 
{
	TfrScaffale *frScaffale;
	AnsiString strsql, zona, str, scaffaleText;
	// in base al numero di agv abilitati aggiorno le posizioni di attesa per ogni agv
	TRecordList TabPosizioni;
	TRecordList TabCorsie;
	TIndexList TabUdc;
	TMyShape *Pal;
	int i, presenze[5], prenotata, esclusa, corsia_abilitata = 0,npiani = 1;
	zona = "";//dmDB->ReturnZonaDaPos(MainForm->pos_scaffale);
	/*if (!dmDB->CorsiaPrimaPosInAlto(MainForm->corsia_scaffale))
		str = "Posizioni WHERE Fila = " + IntToStr(MainForm->corsia_scaffale) + " ORDER BY Pos Desc";
	else  */
	str = "Posizioni WHERE Fila = " + IntToStr(MainForm->corsia_scaffale) + " ORDER BY Pos";
	// parto sempre da sx con la pos chiamata 1, poi nel crs può essere che sia l'ultima, dipende come deposito in corsia
	dmDB->CaricaTabella(str, TabPosizioni);
	dmDB->CaricaTabella("Corsie where Fila = " + IntToStr(MainForm->corsia_scaffale), TabCorsie);
	// dmDB->CaricaTabellaK("Udc ","IDARTICOLO", TabUdc);

	for (i = 0; i < 8; i++) {
		frScaffale = (TfrScaffale*)(FindComponent("frScaffale" + IntToStr(i + 1)));
		if (i < TabCorsie.size()) {
			Label6->Caption = TabCorsie[i]["FILA"];
			Label8->Caption = TabCorsie[i]["NOMECORSIA"];
			Label11->Caption = TabCorsie[i]["ZONA"];
			ckAbilita->Checked = TabCorsie[i]["ABILITATA"].ToIntDef(0);
			corsia_abilitata = TabCorsie[i]["ABILITATA"].ToIntDef(0);
			ckPrenotata->Checked = TabCorsie[i]["CORSIAPRENOTATA"].ToIntDef(0);
			ckPiena->Checked = TabCorsie[i]["PIENA"].ToIntDef(0);
			ckVuota->Checked = TabCorsie[i]["VUOTA"].ToIntDef(0);
			ckUsoPrel->Checked = TabCorsie[i]["INUSOPRELIEVO"].ToIntDef(0);
			ckUsoDep->Checked = TabCorsie[i]["INUSODEPOSITO"].ToIntDef(0);
			cbTipo->ItemIndex = TabCorsie[i]["TIPOLOGIA"].ToIntDef(0); //cbTipo->Text = dmDB->RiempiTipologiaCorsia(cbTipo, TabCorsie[i]["TIPOLOGIA"].ToIntDef(0));
			eHDep->Text = TabPosizioni[i]["HDEP"];
			eHPrel->Text = TabPosizioni[i]["HPREL"];
            cbPriorita->Text = TabCorsie[i]["PRIORITA"];
            frScaffale->Visible = true ;
		}
        else
            frScaffale->Visible = false ;
            
		if (frScaffale != NULL) {
			if (i < TabPosizioni.size()) {
				// colore pallet x dimensione
				frScaffale->Panel2->Caption = TabPosizioni[i]["CUSTOMERPOS"];
				prenotata = TabPosizioni[i]["PRENOTATA"].ToIntDef(0);
				if (!corsia_abilitata)
					esclusa = MainForm->bit[0] | MainForm->bit[1] | MainForm->bit[2] ;
				else
					esclusa = TabPosizioni[i]["DISABILITA"].ToIntDef(0);
				presenze[1] = TabPosizioni[i]["IDUDC"].ToIntDef(0) || TabPosizioni[i]["PRESENZAPALLET"].ToIntDef(0);

				if (presenze[1]) {
					frScaffale->MyShape1->Brush->Color = clAqua;
					frScaffale->MyShape1->Text = presenze[1] ;
				}
				else {
					frScaffale->MyShape1->Brush->Style = bsClear;
					frScaffale->MyShape1->Text = "";
				}
				if (prenotata & MainForm->bit[0]) {
					frScaffale->MyShape1->Pen->Color = clBlue;
					frScaffale->MyShape1->Pen->Width = 3;
				}
				else if (esclusa & MainForm->bit[0]) {
					frScaffale->MyShape1->Pen->Color = clRed;
					frScaffale->MyShape1->Pen->Width = 3;
				}
				else {
					frScaffale->MyShape1->Pen->Color = clBlack;
					frScaffale->MyShape1->Pen->Width = 1;
				}
				frScaffale->MyShape1->Text = presenze[1];

				if (MainForm->pos_scaffale == TabPosizioni[i]["POS"].ToIntDef(0)) 
                {
					// Gestione text piani scaffali

					Label2->Caption = TabPosizioni[i]["POS"];
					Label4->Caption = TabPosizioni[i]["CUSTOMERPOS"];
					statodisabilita = TabPosizioni[i]["DISABILITA"].ToIntDef(0);
					statoprenotata = TabPosizioni[i]["PRENOTATA"].ToIntDef(0);
					leUdc->Text =  TabPosizioni[i]["IDUDC"];
					ckDisabilitaPos->Checked = statodisabilita & MainForm->bit[MainForm->piano_scaffale - 1];
					cPrenotataPos->Checked = statoprenotata & MainForm->bit[MainForm->piano_scaffale - 1];
					cPresenzaPos->Checked = TabPosizioni[i]["PRESENZAPALLET"].ToIntDef(0) ;
			        leTipoPallet->Text = TabPosizioni[i]["TIPOPALLET"];
					Pal = (TMyShape*) frScaffale->FindComponent("MyShape" + IntToStr(MainForm->piano_scaffale));
					if (Pal != NULL) {
						frScaffale->Shape1->Top = Pal->Top - 9;
						frScaffale->Shape1->Left = Pal->Left - 5;
						frScaffale->Shape1->Visible = true;
					}
					else
						frScaffale->Shape1->Visible = false;
				}
				else
					frScaffale->Shape1->Visible = false;
				frScaffale->MyShape1->Tag = TabPosizioni[i]["POS"].ToIntDef(0);
				frScaffale->MyShape1->Hint = "1";
				frScaffale->Visible = true;
			}
			else {
				// scaffale non presente
				frScaffale->Visible = false;
			}
		}
	}
    if (Label8->Caption == "Vuoti") 
    {
        cbTipo->ItemIndex = -1;
        cbTipo->Enabled = false;
        cbTipo->Text = "VUOTI";
    }
    else
        cbTipo->Enabled = true;
    
}

void __fastcall TfCorsia::frScaffale1MyShape4MouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMyShape *Pal;
	Pal = (TMyShape*) Sender;
	if ((Pal != NULL) && (Pal->Tag)) {
		MainForm->pos_scaffale = Pal->Tag;
		MainForm->piano_scaffale = Pal->Hint.ToIntDef(1);
		VisualizzaScaffale();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::FormActivate(TObject * Sender) {
	VisualizzaScaffale();
	Timer1Timer(this);
	BitBtn4->Caption = "Applica Modifiche";
    BitBtn4->Enabled=true; //MainForm->pwdlevel;
	BitBtn2->Enabled=MainForm->pwdlevel;
    BitBtn6->Enabled=true; //MainForm->pwdlevel;
    //leUdc->ReadOnly = MainForm->pwdlevel;
    ckDisabilitaPos->Enabled = MainForm->pwdlevel;
    cPrenotataPos->Enabled = MainForm->pwdlevel;
	// gestione limiti corsie
	int daCorsia, aCorsia;

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn1Click(TObject * Sender) {
	int retval;
	/* coma fa a funzionare???
	 if (MainForm->corsia_scaffale < MainForm->num_corsie) {
	 MainForm->corsia_scaffale++;
	 }
	 else
	 MainForm->corsia_scaffale =  1;
	 MainForm->pos_scaffale = dmDB->ReturnPrimaPosDaFila(MainForm->corsia_scaffale);
	 MainForm->piano_scaffale = 1;
	 VisualizzaScaffale();
	 */
	retval = dmDB->ReturnNextFilaDaPos(MainForm->corsia_scaffale, Label11->Caption);
	if (retval != MainForm->corsia_scaffale) {
		MainForm->corsia_scaffale = retval;
		MainForm->pos_scaffale = dmDB->ReturnPrimaPosDaFila(MainForm->corsia_scaffale);
		MainForm->piano_scaffale = 1;
		VisualizzaScaffale();
	}

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn5Click(TObject * Sender) {

	int retval;
	/* coma fa a funzionare???
	 if (MainForm->corsia_scaffale > 1) {
	 MainForm->corsia_scaffale--;
	 }
	 else
	 MainForm->corsia_scaffale = MainForm->num_corsie;
	 MainForm->pos_scaffale = dmDB->ReturnPrimaPosDaFila(MainForm->corsia_scaffale);
	 MainForm->piano_scaffale = 1;
	 VisualizzaScaffale();
	 */
	retval = dmDB->ReturnPrevFilaDaPos(MainForm->corsia_scaffale, Label11->Caption);
	if (retval != MainForm->corsia_scaffale) {
		MainForm->corsia_scaffale = retval;
		MainForm->pos_scaffale = dmDB->ReturnPrimaPosDaFila(MainForm->corsia_scaffale);
		MainForm->piano_scaffale = 1;
		VisualizzaScaffale();
	}

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn3Click(TObject * Sender) {
	if (Application->MessageBox(L"Sicuro di voler svuotare l'intera Corsia ?", L"Conferma!!!", MB_YESNO) == IDYES) {
		dmDB->SvuotaCorsia(MainForm->corsia_scaffale);
		VisualizzaScaffale();
		MainForm->aggiornamappa = 1;
	}
}

// ---------------------------------------------------------------------------

void __fastcall TfCorsia::Timer1Timer(TObject * Sender) {
	Timer1->Enabled = false;
	GroupBox1->Enabled = MainForm->pwdlevel;
	BitBtn3->Enabled = MainForm->pwdlevel;
	BitBtn2->Enabled = (leUdc->Text != "");
    BitBtn4->Enabled = true;  //MainForm->pwdlevel;
    BitBtn6->Enabled = true; //MainForm->pwdlevel;
    //leUdc->ReadOnly = MainForm->pwdlevel;
    ckDisabilitaPos->Enabled = MainForm->pwdlevel;
    cPrenotataPos->Enabled = MainForm->pwdlevel;
	Timer1->Enabled = true;
	if ((ccont == 10) || (ccont == 0)) {
		ccont = 0;
		BitBtn4->Caption = "Applica Modifiche";
	}
	else
		ccont++;

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn4Click(TObject * Sender) {
	AnsiString stringa;
	int res, tipologia;
	try 
    {
	 	if (dmDB->ADOConnection1->Connected) 
        {
			MainForm->aggiornamappa = 1;
			dmDB->QueryPos->Close();
			dmDB->QueryPos->SQL->Clear();
            
            if (Label8->Caption == "Vuoti") 
                tipologia = 4;
            else
                tipologia = cbTipo->ItemIndex;
                
			stringa.printf("UPDATE Corsie set Abilitata = %d , CorsiaPrenotata = %d , Piena = %d , Vuota = %d , Tipologia = %d "
				            ", InUsoPrelievo = %d , InUsoDeposito = %d, Priorita = %d  WHERE Fila = %d",
				    ckAbilita->Checked, ckPrenotata->Checked, ckPiena->Checked, ckVuota->Checked, tipologia, ckUsoPrel->Checked, ckUsoDep->Checked, 
                    cbPriorita->Text.ToIntDef(0), MainForm->corsia_scaffale);
			dmDB->QueryPos->SQL->Append(stringa);

			res = dmDB->QueryPos->ExecSQL();
			dmDB->QueryPos->Close();
            
			BitBtn4->Caption = "Record Aggiornato";
			ccont = 1;
            
			if (res) 
            {
				dmDB->LogMsg(stringa);
				VisualizzaScaffale();
				// Close();
			} 
		} 
	}
	catch (...) {
	}
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn2Click(TObject * Sender) 
{
	if (Application->MessageBox(L"Sicuro di voler svuotare la posizione ?", L"Conferma!!!", MB_YESNO) == IDYES) 
    {
		dmDB->SvuotaPosizione(Label4->Caption);
        dmDB->SettaCorsiaPiena(MainForm->corsia_scaffale, 0);
		VisualizzaScaffale();
		MainForm->aggiornamappa = 1;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn6Click(TObject * Sender) {
	AnsiString stringa,udc;
	int res;
	try {
		if (dmDB->ADOConnection1->Connected) {
			MainForm->aggiornamappa = 1;
			if (ckDisabilitaPos->Checked)
				statodisabilita |= MainForm->bit[MainForm->piano_scaffale - 1];
			else
				statodisabilita &= MainForm->bitAnd[MainForm->piano_scaffale - 1];
			if (cPrenotataPos->Checked)
				statoprenotata |= MainForm->bit[MainForm->piano_scaffale - 1];
			else
				statoprenotata &= MainForm->bitAnd[MainForm->piano_scaffale - 1];

			dmDB->QueryPos->Close();
			dmDB->QueryPos->SQL->Clear();
			udc = leUdc->Text ;
			stringa.printf("UPDATE Posizioni set DISABILITA = %d , PRENOTATA = %d ,PresenzaPallet = %d , IDUDC = %s  where pos = %d ", statodisabilita, statoprenotata,cPresenzaPos->Checked,
				 udc.c_str(), MainForm->pos_scaffale);
			dmDB->QueryPos->SQL->Append(stringa);
			res = dmDB->QueryPos->ExecSQL();
			dmDB->QueryPos->Close();
			if (res) {
				dmDB->LogMsg(stringa);
				// Close();
				if (cPresenzaPos->Checked)
					dmDB->SettaCorsiaVuota(Label6->Caption.ToIntDef(0),0);
				VisualizzaScaffale();
			}
		}
	}
	catch (...) {
	}
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
}
// ---------------------------------------------------------------------------

