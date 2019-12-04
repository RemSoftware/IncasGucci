// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UDClist.h"
#include "db.h"
#include "main.h"
#include "ins_UDC.h"
//#include "UDCform.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormUDCList *FormUDCList;

// ---------------------------------------------------------------------------
__fastcall TFormUDCList::TFormUDCList(TComponent* Owner)
    : TForm(Owner)
{
    blocca_insert = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormUDCList::BitBtn4Click(TObject *Sender)
{
    udcsel = 0;
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormUDCList::FormClose(TObject *Sender,
    TCloseAction &Action)
{
    ADOQuery1->Close();
    Edit1->Text = "";

	Edit3->Text = "";
    //dmDB->aggiorna_tab_posizioni_locale = 1;
    blocca_insert = 0;

}

// ---------------------------------------------------------------------------
void __fastcall TFormUDCList::DBGrid1DrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column,
    TGridDrawState State)
{
    int color;
    AnsiString code;
    if ((Column->FieldName == "Colore") &&
        (Column->Field->AsInteger > 0)) {
        code = IntToStr(Column->Field->AsInteger);
        color = Column->Field->AsInteger;
        DBGrid1->Canvas->Brush->Color = color;
        DBGrid1->Canvas->Font->Color = color;
        DBGrid1->Canvas->TextRect(Rect, Rect.left + 2, Rect.top + 2, code);
    }

}

// ---------------------------------------------------------------------------
void __fastcall TFormUDCList::FormActivate(TObject *Sender)
{
    Edit1->Text = "";

	Edit3->Text = "";
	// BitBtnEdit->Visible = false;
	// BitBtnAdd->Visible = false;

    Edit1Change(this);

}

// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::Edit1Change(TObject *Sender)
{
    Word Year, Month, Day;
    AnsiString filtro, str;

    filtro = "SELECT * from UDC where (1=1) "     ;

    try {
        if (Edit1->Text != "") {         ;
			filtro = filtro + " and CodUDC LIKE '%" + Edit1->Text + "%'";
        }
    }
    catch (...) {}
    try {
        if ((Edit3->Text != "") && (Edit3->Text.ToIntDef(0) > 0)) {
            filtro = filtro + " and IDUDC =" + Edit3->Text;
        }
    }
    catch (...) {}
    try {
        if (Edit2->Text != "") {      
			//filtro = filtro + " and DescTipoUDC LIKE '%" + Edit2->Text + "%'";
        } 
    }
    catch (...) {}


    /* if (!filter)
     filtro = "Select * from Articles  "; */
    filtro = filtro + " order by IDUDC";
    ADOQuery1->Close();
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add(filtro);
    ADOQuery1->Open();
    ADOQuery1->Last();
    Tot->Caption = IntToStr(ADOQuery1->RecordCount);
    BitBtn1->Enabled = !blocca_insert;
}

// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::BitBtn1Click(TObject *Sender) {
    int posold;
    AnsiString messaggio;
    udcsel = ADOQuery1->FieldByName("IdUDC")->AsInteger;
    posold = 0;//dmDBClient->CercaUDCinPosizione(udcsel);
    if (posold > 0) {
        messaggio = "Sei sicuro di voler muovere l'UDC " + String(udcsel) + " dalla pos " + String(posold) + " alla pos " + String(pos) + "?";
        if (Application->MessageBox(L"Sei sicuro di voler muovere l'UDC? ", L"Conferma!!!", MB_YESNO) == IDYES) {
            //dmDB->EliminaUDCdaPosizione(udcsel);
            //dmDB->InsertUDCInPosizione(udcsel, pos);
        }
    }
    else
        {}//dmDB->InsertUDCInPosizione(udcsel, pos);
    Close();

}
// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::BitBtn2Click(TObject *Sender)
{
    //FormCreaUDC->ShowModal();
    //Edit1Change(this);
}
// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::BitBtnEditClick(TObject *Sender)
{
    // Se sto editando l'udc dalle posizioni: Doppio click INSERISCE in POS
    if (BitBtn1->Enabled) 
    {
        BitBtn1Click(NULL);
    }
    // Se sto editando l'udc dalle anagrafiche: Doppio click EDITA
    else
    {
        if ((ADOQuery1->RecNo) && (ADOQuery1->FieldByName("IDUDC")->AsInteger > 0)) 
        {
            FormInsUDC->insert = false;
            FormInsUDC->edIDUDC->Text = ADOQuery1->FieldByName("IDUDC")->AsString;
            FormInsUDC->cbTipoUDC->Text = "";//dmExtraFunction->RiempiTipoUDC(FormInsUDC->cbTipoUDC, dmExtraFunction->PadS(ADOQuery1->FieldByName("Codtipoudc")->AsString, 2, "0"));
            //FormInsUDC->cbStatus->Text = dmExtraFunction->RiempiTipoStatus(FormInsUDC->cbStatus, dmExtraFunction->PadS(ADOQuery1->FieldByName("stato")->AsString, 2, "0"));
            //FormInsUDC->m_nIDArticolo = ADOQuery1->FieldByName("IDarticolo")->AsInteger;
            //FormInsUDC->edDescrArticolo->Text = ADOQuery1->FieldByName("DescArticolo")->AsString;
            
            if (FormInsUDC->m_nIDArticolo == 0) 
            {
                FormInsUDC->edDescrArticolo->Text = "";
            }

            FormInsUDC->ShowModal();
            Edit1Change(this);
        }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::BitBtnAddClick(TObject *Sender) {
	FormInsUDC->insert = true;
	FormInsUDC->edIDUDC->Text = ""; //dmDBClient->ReturnNewIDUDC();
    FormInsUDC->cbTipoUDC->Text = "";//dmExtraFunction->RiempiTipoUDC(FormInsUDC->cbTipoUDC, "00");
    //FormInsUDC->cbStatus->Text = dmExtraFunction->RiempiTipoUDC(FormInsUDC->cbStatus, "00"); ;

	FormInsUDC->ShowModal();
    Edit1Change(this);                                             
}
// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::ADOQuery1AfterScroll(TDataSet *DataSet)
{
    // per correggere il bug della grid che seleziona una riga sola e dopo non rimette la scrollbar
    DBGrid1->Width = DBGrid1->Width + 1;
    DBGrid1->Width = DBGrid1->Width - 1;
}
// ---------------------------------------------------------------------------
