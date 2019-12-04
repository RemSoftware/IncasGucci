// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DB.h"
#include "ExtraFunction.h"
#include "DataExchange.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDataForm *DataForm;

// ---------------------------------------------------------------------------
__fastcall TDataForm::TDataForm(TComponent* Owner)
    : TForm(Owner)
{
}
// ---------------------------------------------------------------------------

AnsiString TDataForm::NomeObj(AnsiString Funzione, int ID) {
    // ritorna il nome
    AnsiString ret = "";
    TADOQuery *QueryDataE;
    AnsiString stringa;
    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select nome From StrutturaDati where funzione=UPPER(TRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = Trim(QueryDataE->FieldByName("Nome")->AsString);
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");

    }
    QueryDataE->Close();
    return ret;
}

int TDataForm::TipoObj(AnsiString Funzione, int ID) {
    /* ritorna il tipo
     0-int
     2-float
     3-text
     4-bool
     5-combo
     */
    int ret = -1;
    TADOQuery *QueryDataE;
    AnsiString stringa;
    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select tipo  From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = QueryDataE->FieldByName("tipo")->AsInteger;
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");
    }
    QueryDataE->Close();
    return ret;
}

AnsiString TDataForm::TestoObj(AnsiString Funzione, int ID) {
    // ritorna il testo caption
    AnsiString ret = "";
    TADOQuery *QueryDataE;
    AnsiString stringa;

    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select testo From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = Trim(QueryDataE->FieldByName("testo")->AsString);
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");
    }
    QueryDataE->Close();
    return ret;
}

AnsiString TDataForm::Traduci(AnsiString Funzione, int ID) {
    // ritorna il testo caption
    AnsiString ret = "";
    TADOQuery *QueryDataE;
    AnsiString stringa;
    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select * From translations where funzione=UPPER(LTRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = Trim(QueryDataE->FieldByName(dmDB->Lingua)->AsString);
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");
    }
    QueryDataE->Close();
    return ret;
}

int TDataForm::StartObj(AnsiString Funzione, int ID) {
    // ritorna lo start
    int ret = -1;
    TADOQuery *QueryDataE;
    AnsiString stringa;

    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select StartString From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = QueryDataE->FieldByName("StartString")->AsInteger;
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");
    }
    QueryDataE->Close();
    return ret;
}

int TDataForm::LenObj(AnsiString Funzione, int ID) {
    // ritorna la lunghezza
    int ret = -1;
    TADOQuery *QueryDataE;
    AnsiString stringa;

    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select LenString From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = QueryDataE->FieldByName("LenString")->AsInteger;
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");
    }
    QueryDataE->Close();
    return ret;
}

AnsiString TDataForm::DefaultObj(AnsiString Funzione, int ID) {
    // ritorna il valore di default
    AnsiString ret = "";
    TADOQuery *QueryDataE;
    AnsiString stringa;

    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select defaultValue From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = Trim(QueryDataE->FieldByName("defaultValue")->AsString);
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");

    }
    QueryDataE->Close();
    return ret;
}

bool TDataForm::EnableObj(AnsiString Funzione, int ID) {
    // ritorna l'enable
    bool ret = false;
    TADOQuery *QueryDataE;
    AnsiString stringa;
    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select enable From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = QueryDataE->FieldByName("enable")->AsBoolean;
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");

    }
    QueryDataE->Close();
    return ret;

}

int TDataForm::ColoreObj(AnsiString Funzione, int ID) {
    // ritorna il colore
    int ret = -1;
    TADOQuery *QueryDataE;
    AnsiString stringa;

    if (!dmDB->ADOConnection1->Connected)
        return ret;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select colore  From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "')) and ID =" + String(ID);
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        if (QueryDataE->RecordCount > 0) {
            ret = QueryDataE->FieldByName("colore")->AsInteger;
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");

    }
    QueryDataE->Close();
    return ret;
}

void TDataForm::SetLabel(AnsiString Funzione, AnsiString ID) {
    // setta la label del campo id passato
    TLabel *OggettoLabel;
    AnsiString TempString;
    OggettoLabel = (TLabel*) FindComponent("lb" + ID);
    if (OggettoLabel != NULL) {
        TempString = TestoObj(Funzione, ID.ToIntDef(0));
        if (TempString != "")
            OggettoLabel->Caption = TempString;
    }

}

bool TDataForm::SetLingua(AnsiString Funzione, AnsiString ID) {
    // traduci
    bool ret = false;
    TLabel *OggettoLabel;
    AnsiString TempString;
    OggettoLabel = (TLabel*) FindComponent("lb" + ID);
    if (OggettoLabel != NULL) {
        TempString = Traduci(Funzione, ID.ToIntDef(0));
        if (TempString != "") {
            OggettoLabel->Caption = TempString;
            ret = true;
        }
    }

}

void TDataForm::SetCheck(AnsiString Funzione, AnsiString ID) {
    // setta il check  del campo id passato
    TCheckBox *OggettoCheck;
    AnsiString TempString;
    OggettoCheck = (TCheckBox*) FindComponent("cb" + ID);
    if (OggettoCheck != NULL) {
        TempString = TestoObj(Funzione, ID.ToIntDef(0));
        if (TempString != "")
            OggettoCheck->Caption = TempString;
        OggettoCheck->Checked = StrToBoolDef(DefaultObj(Funzione, ID.ToIntDef(0)), false);

    }
}

void TDataForm::SetEnable(AnsiString Funzione, AnsiString tipo, AnsiString ID) {
    // setta enabled del campo id passato
    TControl *OggettoCheck;
    AnsiString TempString;
    OggettoCheck = (TControl*) FindComponent(tipo + ID);
    if (OggettoCheck != NULL) {
        OggettoCheck->Enabled = EnableObj(Funzione, ID.ToIntDef(0));
    }

}

void TDataForm::SetColor(AnsiString Funzione, AnsiString ID) {
    // setta io colore del campo id passato
    TLabel *OggettoLabel;
    AnsiString TempString;
    OggettoLabel = (TLabel*) FindComponent("lb" + ID);
    if (OggettoLabel != NULL) {
        OggettoLabel->Font->Color = ColoreObj(Funzione, ID.ToIntDef(0));
    }
}

void TDataForm::SetDefault(AnsiString Funzione, AnsiString ID) {
    // setta la label del campo id passato
    TEdit *OggettoEdit;
    AnsiString TempString;
    OggettoEdit = (TEdit*) FindComponent("ed" + ID);
    if (OggettoEdit != NULL) {
        if (TipoObj(Funzione, ID.ToIntDef(0)) <= 2)
            OggettoEdit->Alignment = taRightJustify;
        TempString = DefaultObj(Funzione, ID.ToIntDef(0));
        if (TempString != "")
            OggettoEdit->Text = TempString;
    }
}

void TDataForm::RiempiStruttura(AnsiString Funzione) {
    TADOQuery *QueryDataE;
/*    AnsiString stringa;
    int ccont = 0;
    if (!dmDB->ADOConnection1->Connected)
        return;
    try {
        QueryDataE = new TADOQuery(NULL);
        QueryDataE->Connection = dmDB->ADOConnection1;

        stringa = "Select *  From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "')) order by startstring";
        QueryDataE->Close();
        QueryDataE->SQL->Clear();
        QueryDataE->SQL->Append(stringa);
        QueryDataE->Open();
        while (!QueryCampi->Eof) {
            dmDB->Struttura[ccont].ID = QueryCampi->FieldByName("ID")->AsInteger;
            dmDB->Struttura[ccont].Nome = QueryCampi->FieldByName("Nome")->AsAnsiString;
            dmDB->Struttura[ccont].Tipo = QueryCampi->FieldByName("Tipo")->AsInteger;
            dmDB->Struttura[ccont].StartString = QueryCampi->FieldByName("StartString")->AsInteger;
            dmDB->Struttura[ccont].LenString = QueryCampi->FieldByName("LenString")->AsInteger;
            ccont++;
            QueryCampi->Next();
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");

    }
    QueryDataE->Close();
    return;        */

}

void TDataForm::CompilaForm(TObject * Sender, AnsiString Funzione) {
    TADOQuery *QueryCampi;

    AnsiString stringa, TempString, TempID;

    int tipocampo;

    if (!dmDB->ADOConnection1->Connected)
        return;
    try {
        QueryCampi = new TADOQuery(NULL);
        QueryCampi->Connection = dmDB->ADOConnection1;

        stringa = "Select * From StrutturaDati where funzione=UPPER(LTRIM('" + Funzione + "'))";
        QueryCampi->Close();
        QueryCampi->SQL->Clear();
        QueryCampi->SQL->Append(stringa);
        QueryCampi->Open();
        while (!QueryCampi->Eof) {
            TempID = dmExtraFunction->PadS(QueryCampi->FieldByName("ID")->AsString, 2, "0");
            tipocampo = TipoObj(Funzione, TempID.ToIntDef(0));
            switch (tipocampo) {
            case 0: // int
                if (!DataForm->SetLingua(Funzione, TempID))
                    DataForm->SetLabel(Funzione, TempID);
                DataForm->SetDefault(Funzione, TempID);
                DataForm->SetEnable(Funzione, "ed", TempID);
                DataForm->SetColor(Funzione, TempID);
            case 3: // text
                if (!SetLingua(Funzione, TempID))
                    SetLabel(Funzione, TempID);
                SetLingua(Funzione, TempID);
                SetDefault(Funzione, TempID);
                SetColor(Funzione, TempID);
                break;
            case 4: // bool
                SetCheck(Funzione, TempID);
                SetEnable(Funzione, "cb", TempID);
                break;
            }
            QueryCampi->Next();
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
        dmDB->LogMsg("Eccezione su leggi struttura");
    }
    QueryDataE->Close();

}

void __fastcall TDataForm::FormActivate(TObject *Sender) {
    CompilaForm(this, "dataex");

}

// ---------------------------------------------------------------------------
void __fastcall TDataForm::BitBtn4Click(TObject * Sender)
{
    Close();
}
// ---------------------------------------------------------------------------
