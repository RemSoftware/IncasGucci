// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "config.h"
#include "main.h"
#include "socket.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma resource "*.dfm"
TfrConfig *frConfig;
extern char N_carrelli;
extern CLIENTSOCKET *cs;

// ---------------------------------------------------------------------------
__fastcall TfrConfig::TfrConfig(TComponent* Owner)
    : TMDIChild(Owner)
{
    }

// ---------------------------------------------------------------------------
void __fastcall TfrConfig::FormActivate(TObject *Sender) {
	int i;
	AbilitaMissioni->Checked = MainForm->AbilitaMissioni;
	AbilitaLuci->Checked = MainForm->AbilitaTestLuciPulsantiere;
	LabeledEdit6->Text = MainForm->PosizioniLuceRossaSemaforo;
    //nPallet->Text = dmDB->TabParametri[103]["INTVALUE"].ToIntDef(0);
	//AbilitaLuci->Visible = MainForm->pwdlevel ;

/*	LabeledEdit6->Text = MainForm->PlcRobot.DatiRotAlarm[1].pos_rot_alarm;
	LabeledEdit7->Text = MainForm->PlcRobot.DatiRotAlarm[2].pos_rot_alarm;
	LabeledEdit8->Text = MainForm->PlcRobot.DatiRotAlarm[3].pos_rot_alarm;
	LabeledEdit9->Text = MainForm->PlcRobot.DatiRotAlarm[4].pos_rot_alarm;
	cbescludiPorta1->Checked = MainForm->PlcRobot.DatiPorte[1].escludi;
	cbEscludiPorta2->Checked = MainForm->PlcRobot.DatiPorte[2].escludi;
	cbEscludiPorta3->Checked = MainForm->PlcRobot.DatiPorte[3].escludi;

	lePortaAperta1->Text = MainForm->PlcRobot.DatiPorte[1].pos_porta_aperta;
	lePortaAperta2->Text = MainForm->PlcRobot.DatiPorte[2].pos_porta_aperta;
	lePortaAperta3->Text = MainForm->PlcRobot.DatiPorte[3].pos_porta_aperta;

	leAperturaPorta1->Text = MainForm->PlcRobot.DatiPorte[1].pos_apri_porta;
	leAperturaPorta2->Text = MainForm->PlcRobot.DatiPorte[2].pos_apri_porta;
	leAperturaPorta3->Text = MainForm->PlcRobot.DatiPorte[3].pos_apri_porta;

	leHPrel->Text="";
	leHDep->Text="";   */
	Timer1Timer(this);
	/* LabeledEdit12->Text  =  MainForm->DatiPlc[1].pos_porta_aperta[1] ;
	 LabeledEdit13->Text  =  MainForm->DatiPlc[1].pos_porta_aperta[2] ;
	 CheckBox1->Checked = MainForm->DatiPlc[1].escludiPorte & MainForm->bit[1] ;
	 CheckBox2->Checked = MainForm->DatiPlc[1].escludiPorte & MainForm->bit[2] ; */
}

// ---------------------------------------------------------------------------
void __fastcall TfrConfig::BitBtnChangeClick(TObject *Sender)
{
	int i, udc,mod_stato_demo = 0;
	if (MainForm->pwdlevel) {

/*
		if ((MainForm->AbilitaTestLuciPulsantiere)&&(!AbilitaLuci->Checked))
			MainForm->AbilitaTestLuciPulsantiereRimosso = 1 ;
		MainForm->AbilitaTestLuciPulsantiere = AbilitaLuci->Checked;   */
	}
	MainForm->PosizioniLuceRossaSemaforo = LabeledEdit6->Text;
    
	if (MainForm->AbilitaMissioni != AbilitaMissioni->Checked) {
		MainForm->AbilitaMissioni = AbilitaMissioni->Checked;
		dmDB->LogMsg("Abilita missioni Premuto, valore : " + BoolToStr(AbilitaMissioni->Checked, true));
	}
    
    //dmDB->AggiornaParametri(103, nPallet->Text.ToIntDef(0));    //numero di pallet oltre al quale viene fatta segnalazione

    dmDB->AggiornaTabParametri();
	dmDB->AggiornaStatoAgv(1);
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TfrConfig::BitBtn2Click(TObject *Sender)
{
	int i, baiaprel, baiadep, agv, idlista;
	UnicodeString strmessage;
	Evento ev;
	if (MainForm->pwdlevel) {
		if (!AbilitaMissioni->Checked) {
			if (Application->MessageBox(L"Resettare le missioni in corso sull' Agv , Ricordarsi di abortire anche da pannello Agv?", L"Conferma!!!", MB_YESNO) == IDYES) {
				sprintf(ev, "SVUOTA,%d", 0);
				cs->SendEv(ev);
				if (Application->MessageBox(L"Resettare tutte le prenotazioni in magazzino e delle presse / avvolgitore?", L"Conferma!!!", MB_YESNO) == IDYES) {
					try {
						dmDB->QueryPos->Close();
						dmDB->QueryPos->SQL->Clear();
						dmDB->QueryPos->SQL->Append("UPDATE Posizioni SET Prenotata = 0");
						dmDB->QueryPos->ExecSQL();
						dmDB->QueryPos->Close();
					}
					catch (...) {
							dmDB->LogMsg("Eccezione Reset Prenotazioni");
					}
				}
				MainForm->aggiornamappa = 1;
			}
			if (Application->MessageBox(L"Resettare le memorie di Agv in missione ????", L"Conferma!!!", MB_YESNO) == IDYES) {
				MainForm->DatiAgv[1].generata = 0 ;
			}
		}
		else
			ShowMessage("Per Resettare l'impianto bisogna deselezionare Abilita Generazioni Missioni ");
	}
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
}
// ---------------------------------------------------------------------------

void __fastcall TfrConfig::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled = false ;
	BitBtn2->Enabled = MainForm->pwdlevel;
	BitBtnChange->Enabled = true ;
	Timer1->Enabled = true ;

}
// ---------------------------------------------------------------------------

void __fastcall TfrConfig::BitBtn1Click(TObject *Sender)
{

	// per postazioni singole o singole file
    AnsiString strsql, nomecorsia, zona, poscliente, nomepiano, nomecorsia_table, nomestanza;
    int pos_opposta, pos_in_fila_opposta, tipologia, res, i, j, k, z, fila, verticale, CorsiaInizioDx, wshape, hshape, n_file_corsia, n_alveoli, pos_prec, next_pos, posopposta, pos;
    int numerofila, pos_in_fila, posinputplc, startfila, ord_corsia, ord_corsia_opposta, tipo_posizione, numfile, step, inizio, piani, dalbassoinalto,numpr;
    int h_prel[] = {80, 0, 0};
	int h_dep[] = {350, 0, 0};
    int abilitata = 0, k_pos, dasxadx;
    TADOQuery *ADOQuery;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return;

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append("delete from Posizioni");
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();

        // Posizioni Ingombro pallet---------------------------------------------------------------------------------------------------
        pos = 100;                   

        for (j = 0; j < 8; j++) 
        {
            for (i = 1; i <= 7; i++) 
            {
                poscliente = "Pos" + IntToStr(pos+i);

                if (i==1)
                    pos_prec = 0;
                else
                    pos_prec = pos+i-1;
                        
                if (i==7)
                    next_pos = 0; 
                else
                    next_pos = pos+i+1;

                strsql.printf("INSERT INTO Posizioni "
                              "(Pos,PosOpposta,CustomerPos,Prenotata,Disabilita,TipoPosizione,Height,Width," 
                              "Fila,PrevPosition,NextPosition,PosInFila,PosInputPLC,PosX,PosY,IdUdc,HPrel," 
                              "HDep,FormatoPallet,TabCarico,TabScarico)"
                              " VALUES (%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"

                            , pos+i                          //Pos
                            , 0                              //PosOpposta
                            , poscliente.c_str()             //CustomerPos
                            , 0                              //Prenotata
                            , 0                              //Disabilita
                            , 0                              //TipoPosizione     
                            , 15                             //Height             
                            , 15                             //Width              
                            , j+1                            //Fila
                            , pos_prec                       //PrevPosition
                            , next_pos                       //NextPosition
                            , i                              //PosInFila
                            , 0                              //PosInputPlc
                            , 0                              //Posx
                            , 0                              //Posy
                            , 0                              //IdUdc
                            , 0                              //Hprel
                            , 0                              //Hdep
                            , 0                              //Formato pallet
                            , 0                              //Tab Carico
                            , 0                              //Tab Scarico
                            );
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Text = strsql;
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();

            }
            //
            pos = pos + 100;
        }


        // Posizioni caricamento robot
        pos = 10;  
                         
        for (i = 1; i <= 9; i++) 
        {
            poscliente = "Pos" + IntToStr(pos);

            strsql.printf("INSERT INTO Posizioni "
                          "(Pos,PosOpposta,CustomerPos,Prenotata,Disabilita,TipoPosizione,Height,Width," 
                          "Fila,PrevPosition,NextPosition,PosInFila,PosInputPLC,PosX,PosY,IdUdc,HPrel," 
                          "HDep,FormatoPallet,TabCarico,TabScarico)"
                          " VALUES (%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"

                        , pos                            //Pos
                        , 0                              //PosOpposta
                        , poscliente.c_str()             //CustomerPos
                        , 0                              //Prenotata
                        , 0                              //Disabilita
                        , 0                              //TipoPosizione     
                        , 15                             //Height             
                        , 15                             //Width              
                        , 0                              //Fila
                        , 0                             //PrevPosition
                        , 0                             //NextPosition
                        , 0                               //PosInFila
                        , 0                              //PosInputPlc
                        , 0                              //Posx
                        , 0                              //Posy
                        , 0                              //IdUdc
                        , 0                              //Hprel
                        , 0                              //Hdep
                        , 0                              //Formato pallet
                        , 0                              //Tab Carico
                        , 0                              //Tab Scarico
                        );
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();

            pos = pos + 10;
        }

        //pos plc + vuoti
        for (i = 1; i <= 6; i++) 
        {
            poscliente = "Pos" + IntToStr(i);

            strsql.printf("INSERT INTO Posizioni "
                          "(Pos,PosOpposta,CustomerPos,Prenotata,Disabilita,TipoPosizione,Height,Width," 
                          "Fila,PrevPosition,NextPosition,PosInFila,PosInputPLC,PosX,PosY,IdUdc,HPrel," 
                          "HDep,FormatoPallet,TabCarico,TabScarico)"
                          " VALUES (%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"

                        , i                              //Pos
                        , 0                              //PosOpposta
                        , ""                             //CustomerPos
                        , 0                              //Prenotata
                        , 0                              //Disabilita
                        , 0                              //TipoPosizione     
                        , 15                             //Height             
                        , 15                             //Width              
                        , 0                              //Fila
                        , 0                             //PrevPosition
                        , 0                             //NextPosition
                        , 0                               //PosInFila
                        , 0                              //PosInputPlc
                        , 0                              //Posx
                        , 0                              //Posy
                        , 0                              //IdUdc
                        , 0                              //Hprel
                        , 0                              //Hdep
                        , 0                              //Formato pallet
                        , 0                              //Tab Carico
                        , 0                              //Tab Scarico
                        );
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();

        }
        
    }
    catch (...) {
        Application->MessageBox(L"Creazione posizioni ", L"Exception", MB_OK);
    }
    delete ADOQuery;

    Application->MessageBox(L"Creazione posizioni effettuata", L"Lavoro terminato", MB_OK);
}

//---------------------------------------------------------------------------

