//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//#include "VoloPalletINForm.h"
#include "DB.h"
//#include "ExtraFunction.h"
//#include "DBClient.h"
#include "main.h"
#include "PosPresse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPosPresseForm *PosPresseForm;
//---------------------------------------------------------------------------
__fastcall TPosPresseForm::TPosPresseForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TPosPresseForm::EmptyText() 
{
    edID->Text = "";
}

void TPosPresseForm::VisualizzaDatiPosizione() 
{
    TUDC UdcMod;
    TLGV lgv;
    int ret;
    AnsiString sName, sPos;

    EmptyText();

    dmDB->CaricaTabella("Posizioni where Pos = " + IntToStr(pos), m_TabPosizioni);
    LabelPos->Caption = m_TabPosizioni[0]["POS"];
    LabelPosCli->Caption = m_TabPosizioni[0]["CUSTOMERPOS"];

    switch (m_nType) 
    {
        case LINEACAR_TYPE:
        default:
            PanelTitle->Caption = "Dati Posizione di Carico";
            sPos = m_TabPosizioni[0]["CUSTOMERPOS"];          //Prende solo il carattere 2 (che nella tabella di scambio corrisponde alla linea)
            sName = sPos[2];  
            Led->Visible = true;
            LedLabel->Visible = true;   
           	Timer1->Enabled = true;
            break; 
        //case DEPVUOTI_TYPE:
        //    PanelTitle->Caption = "Dati Posizione Vuoti";
        //    break; 
    
    }
    ret = dmDB->GetLastLGVrecordFromCAR(sName, lgv);
    if (ret) 
    {
        edID->Text = lgv.ID;
    }   
    
    ckDisabilitaPos->Checked = m_TabPosizioni[0]["DISABILITA"].ToIntDef(0);
    cPrenotataPos->Checked = m_TabPosizioni[0]["PRENOTATA"].ToIntDef(0);
}

//---------------------------------------------------------------------------


void __fastcall TPosPresseForm::FormActivate(TObject *Sender)
{
    cambiocheck = 0;
    VisualizzaDatiPosizione();    
}
//---------------------------------------------------------------------------


void __fastcall TPosPresseForm::BitBtn4Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TPosPresseForm::ckDisabilitaPosClick(TObject *Sender)
{
    cambiocheck = 1;    
}
//---------------------------------------------------------------------------

void __fastcall TPosPresseForm::cPrenotataPosClick(TObject *Sender)
{
    cambiocheck = 1;    
}
//---------------------------------------------------------------------------

void __fastcall TPosPresseForm::BitBtn5Click(TObject *Sender)
{
    int pos, vuoto, res;
	int statodisabilita,statoprenotata;
    AnsiString stringa;
    
    pos = StrToInt(LabelPos->Caption);
    if (cambiocheck) 
    {
        if ((MainForm->pwdlevel))
        {
            statodisabilita = ckDisabilitaPos->Checked;
            statoprenotata = cPrenotataPos->Checked;
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            stringa.printf("UPDATE Posizioni set DISABILITA = %d , PRENOTATA = %d where pos = %d ", statodisabilita, statoprenotata, pos);
            dmDB->QueryPos->SQL->Append(stringa);
            res = dmDB->QueryPos->ExecSQL();
            dmDB->QueryPos->Close();

            dmDB->LogMsg("Modificati stati su posizione carico - " + stringa);

            dmDB->CaricaTabelle();
            dmDB->AggiornaMemoriePlc();
		    MainForm->aggiornamappa = 1;
        }
        else
        {
            Application->MessageBox(L"Questi dati possono essere cambiati solo da operatore abilitato!", L"ATTENZIONE", MB_OK);
            return;
        }
    }

    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TPosPresseForm::LedMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    int comando = 0, ncar = 0;
    AnsiString sName, sPos;
    
	Timer1->Enabled = false;
    if (MainForm->Simula) 
    {
        sPos = m_TabPosizioni[0]["CUSTOMERPOS"];          //Prende solo il carattere 2 (che nella tabella di scambio corrisponde alla linea)
        sName = sPos[2]; 
        ncar = sName.ToIntDef(0);
/*        
		//posso forzare il segnale di chiamata
        if (Led->Brush->Color == clGray)
            comando = 1 ;
        if (comando)
            MainForm->PlcRobot.input[1] |= MainForm->bit[ncar-1];
        else
            MainForm->PlcRobot.input[1] &= MainForm->bitAnd[ncar-1];
        MainForm->PlcRobot.forzatura_input = 1 ;
*/
        MainForm->aggiornamappa = 1;
    }
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TPosPresseForm::Timer1Timer(TObject *Sender)
{
	int ncar, stato;
    AnsiString sName, sPos;
    
	Timer1->Enabled = false;
	if (m_nType == LINEACAR_TYPE) 
    {
    /*
        sPos = m_TabPosizioni[0]["CUSTOMERPOS"];          //Prende solo il carattere 2 (che nella tabella di scambio corrisponde alla linea)
        sName = sPos[2]; 
        ncar = sName.ToIntDef(0);
        stato = MainForm->PlcRobot.input[1] & MainForm->bit[ncar-1];

        if (stato) 
            Led->Brush->Color = clLime;
        else
            Led->Brush->Color = clGray;
    */
	}
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TPosPresseForm::BitBtn1Click(TObject *Sender)
{
//    
}
//---------------------------------------------------------------------------

