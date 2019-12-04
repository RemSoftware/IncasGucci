// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ins_UDC.h"
#include "db.h"
// #include "anagrafica_articoliUDC.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormInsUDC *FormInsUDC;

// ---------------------------------------------------------------------------
__fastcall TFormInsUDC::TFormInsUDC(TComponent* Owner)
	: TForm(Owner)
{
    m_nIDArticolo = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsUDC::BitBtn2Click(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TFormInsUDC::FormActivate(TObject *Sender)
{
	if (!insert) 
    {
        cbTipoUDC->Enabled = false;
    }
    else
    {
        cbTipoUDC->Enabled = true;
        m_nIDArticolo = 0;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsUDC::BitBtn1Click(TObject *Sender)
{
	AnsiString stringa;
	int ok, idudc;

	if (insert) 
    {
        if (m_nIDArticolo == 0 || cbTipoUDC->Text == "") 
        {
            Application->MessageBox(L"I dati dell'UDC non sono completi!", L"ATTENZIONE", MB_OK);
            return;    
        }
	    idudc = 0;//dmDBClient->ReturnNewIDUDC();

		stringa = " Insert Into UDC(IDUDC, CodTipoUDC, IDarticolo, CodUDC, Lotto)";
		stringa = stringa + " VALUES(" + IntToStr(idudc) + ", " +
			StrToInt(cbTipoUDC->Text.SubString(1, 2)) + ", " +
			m_nIDArticolo + "')";
	}
	else 
    {
		stringa = " Update UDC set IDUDC = " + edIDUDC->Text +
			", CodTipoUDC = " + StrToInt(cbTipoUDC->Text.SubString(1, 2)) +
			", CodUDC = '" + Trim(edDescrArticolo->Text.UpperCase()) + "'";
		stringa = stringa + " where IDUDC = " + edIDUDC->Text;
	}

	try {
		Query->Close();
		Query->SQL->Clear();
		Query->SQL->Append(stringa);
		Query->ExecSQL();
		Query->Close();
		dmDB->LogMsg(stringa);
		Close();
	}
	catch (...) {
		dmDB->LogMsg("Eccezione insert/update UDC");
		Query->Close();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsUDC::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	insert = false;
	// FormAnagraficaArticoliUDC->FormActivate(this);

}
// ---------------------------------------------------------------------------


void __fastcall TFormInsUDC::btInsArticoloClick(TObject *Sender)
{
/*
     FormAnagraficaArticoli->insert_produzione = true;
     FormAnagraficaArticoli->IdArticolo = 0;
     FormAnagraficaArticoli->ShowModal();
     if (FormAnagraficaArticoli->m_bSelected)
     {
        edCodUDC->Text = FormAnagraficaArticoli->ProductName;
        edDescrArticolo->Text = FormAnagraficaArticoli->Descrizione;
        m_nIDArticolo = FormAnagraficaArticoli->IdArticolo;
     }*/
}
//---------------------------------------------------------------------------

