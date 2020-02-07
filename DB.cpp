// ---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "DB.h"
#include "LogTxt.h"
#include "inifiles.hpp"
#include "time.h"
#include "main.h"
#include "socket.h"
#include "percorsiprototipo.h"
#include "dmgestione.h"
#include "datamodulecomandiagv.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmDB *dmDB;
AnsiString logstr;
extern DATILGV tab;
extern CLIENTSOCKET *cs;
extern char N_carrelli ;

// ---------------------------------------------------------------------------
Variant ReadField(TADOQuery *q, AnsiString f) {
    Variant res = "";

    if (!(q->IsEmpty() || q->Eof || q->FieldByName(f)->IsNull)) {
        res = q->FieldByName(f)->Value;
    }
    return res;
}

// ---------------------------------------------------------------------------
int ReadInt(TADOQuery *q, AnsiString f) {
    int res = 0;

    if (!(q->IsEmpty() || q->Eof || q->FieldByName(f)->IsNull)) {
        res = q->FieldByName(f)->Value;
    }
    return res;
}
// ---------------------------------------------------------------------------

AnsiString ReadString(TADOQuery *q, AnsiString f) {
    AnsiString res = "";

    if (!(q->IsEmpty() || q->Eof || q->FieldByName(f)->IsNull)) {
        res = q->FieldByName(f)->AsString;
    }
    return res;
}

// ---------------------------------------------------------------------------
float ReadFloat(TADOQuery *q, AnsiString f) {
    float res = 0.0;

    if (!(q->IsEmpty() || q->Eof || q->FieldByName(f)->IsNull)) {
        res = q->FieldByName(f)->AsFloat;
    }
    return res;
}

// ---------------------------------------------------------------------------
AnsiString TdmDB::RitornaOrarioDaSecondi(int secondi) {
    int ore = 0;
    int minuti = 0;
    int sec;
    char res[10];
    AnsiString result = "00:00:00";
    div_t x;
    try {
        if (secondi >= 3600) {
            x = div(secondi, 3600);
            ore = x.quot;
            secondi = x.rem;
        }
        else
            ore = 0;
        if (secondi >= 60) {
            x = div(secondi, 60);
            minuti = x.quot;
            secondi = x.rem;
        }
        else
            minuti = 0;
        sprintf(res, "%02d:%02d:%02d", ore, minuti, secondi);
        result = res;
    }
    catch (...) {
        ADOConnection1->Close();
        LogMsg("Eccezione calcolo RitornaOrarioDaSecondi");
    }
    return result;
}

AnsiString TdmDB::AggiungiRimuoviSecondiAOrario(AnsiString ora, int secondi, bool rimuovi) {
    int ore = 0;
    int minuti = 0;
    int sec;
    char res[10];
    AnsiString result = "00:00:00";
    div_t x;
    try {
        if (secondi >= 3600) {
            x = div(secondi, 3600);
            ore = x.quot;
            secondi = x.rem;
        }
        else {
            ore = 0;
        }
        if (secondi >= 60) {
            x = div(secondi, 60);
            minuti = x.quot;
            secondi = x.rem;
        }
        else {
            minuti = 0;
        }
        sprintf(res, "%02d:%02d:%02d", ore, minuti, secondi);
        result = res;
        if (rimuovi) {
            result = TimeToStr(StrToTime(ora) - StrToTime(result));
        } // ritorno l'ora in cui si esegue
        else {
            result = TimeToStr(StrToTime(ora) + StrToTime(result));
        } // ritorno l'ora in cui si esegue
    }
    catch (...) {
        // ADOConnection1->Close();
        LogMsg("Eccezione calcolo tempo");
    }
    return result;
}

AnsiString TdmDB::AggiungiRimuoviSecondiADataOra(AnsiString ora, int secondi, bool rimuovi) {
    int ore = 0;
    int minuti = 0;
    int sec;
    char res[10];
    AnsiString result = "00:00:00";
    div_t x;
    try {
        if (secondi >= 3600) {
            x = div(secondi, 3600);
            ore = x.quot;
            secondi = x.rem;
        }
        else {
            ore = 0;
        }
        if (secondi >= 60) {
            x = div(secondi, 60);
            minuti = x.quot;
            secondi = x.rem;
        }
        else {
            minuti = 0;
        }
        sprintf(res, "%02d:%02d:%02d", ore, minuti, secondi);
        result = res;
        if (rimuovi) {
            result = DateTimeToStr(StrToDateTime(ora) - StrToTime(result));
        } // ritorno l'ora in cui si esegue
        else {
            result = DateTimeToStr(StrToDateTime(ora) + StrToTime(result));
        } // ritorno l'ora in cui si esegue
    }
    catch (...) {
        // ADOConnection1->Close();
        LogMsg("Eccezione calcolo tempo");
    }
    return result;
}

int TdmDB::AggiornaCampoTabella(AnsiString Tabella, AnsiString CampoChiave, int ValoreChiave, AnsiString CampoModifica, AnsiString ValoreModifica, bool isstring) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    AnsiString Val;

    if (!ADOConnection1->Connected) {
        return 0;
    }
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (isstring) {
            Val = "'" + ValoreModifica + "'";
        }
        else {
            Val = ValoreModifica;
        }
        strsql.printf("UPDATE %s SET %s = %s WHERE %s = %d", Tabella, CampoModifica, Val, CampoChiave, ValoreChiave);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}
// ---------------------------------------------------------------------------

void TdmDB::CaricaTabella(AnsiString TableName, TRecordList &RecList) {
    AnsiString strsql, campo;
    TADOQuery *ADOQuery;
    Variant k;
    int j, i = 0;
    AnsiString val;

    if (!ADOConnection1->Connected) {
        return;
    }
    RecList.clear();
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT * FROM %s", TableName.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        while (!ADOQuery->Eof) {
            for (j = 0; j < ADOQuery->FieldCount; j++) {
                campo = ADOQuery->FieldList->Fields[j]->FieldName.UpperCase();
                val = ADOQuery->FieldList->Fields[j]->AsString;
                RecList[i][campo] = val;
            }
            ADOQuery->Next();
            i++;
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::CaricaTabellaK(AnsiString TableName, AnsiString KeyField, TIndexList &RecList) {
    AnsiString strsql, campo, chiave, valore;
    TADOQuery *ADOQuery;
    Variant k;
    int j, i = 0;

    if (!ADOConnection1->Connected) {
        return;
    }
    RecList.clear();
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT * FROM %s", TableName.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        while (!ADOQuery->Eof) {
            for (j = 0; j < ADOQuery->FieldCount; j++) {
                chiave = ADOQuery->FieldByName(KeyField)->AsString;
                campo = ADOQuery->FieldList->Fields[j]->FieldName.UpperCase();
                valore = ADOQuery->FieldList->Fields[j]->AsString;
                RecList[chiave][campo] = valore;
            }
            ADOQuery->Next();
            i++;
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

__fastcall TdmDB::TdmDB(TComponent* Owner) : TDataModule(Owner) {
    TIniFile * ini;
    AnsiString ConnectionString, ConnectionString2;

    t = 0;
    ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    ADOConnection1->Close();
    ConnectionString = ini->ReadString("Database", "ConnectionString", "");
    if (ConnectionString != "") {
        ADOConnection1->ConnectionString = ConnectionString;
    }
    FMConnection->Connected = false;
	ConnectionString = ini->ReadString("DatabaseFM", "ConnectionString", "");
    if (ConnectionString != "") {
        FMConnection->ConnectionString = ConnectionString;
    }
	delete ini;
	lettostatoagv = 0;
    lettoparametri = 0;
    TimerConnectTimer(this);
    // TimerConnect->Enabled = true;
    TimerPuliziaStorici->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TdmDB::TimerPuliziaStoriciTimer(TObject *Sender) {
    TDateTime adesso;
    WORD adessoY, adessoM, adessoD, Y, M, D;

    TimerPuliziaStorici->Enabled = false;
    try {
        adesso = Now();
        DecodeDate(adesso, adessoY, adessoM, adessoD);
        DecodeDate(t, Y, M, D);
        if ((adessoD != D) || (adessoM != M) || (adessoY != Y)) {
            // Cambio giorno
            PuliziaStorici();
            t = adesso;
        }
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    TimerPuliziaStorici->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TdmDB::TimerConnectTimer(TObject *Sender) {
	int i;
    TimerConnect->Enabled = false;

    if (ADOConnection1->Connected) 
    {
        // DisconnettiDB();
        if (!lettostatoagv) 
        {
			LeggiStatoAgv();
			CaricaTabelle();
			AggiornaTabPorte();
			AggiornaMemoriePlc();
            AggiornaTabParametri();
		}
    }
    else 
    {
        ConnettiDB();
    }

    if (FMConnection->Connected) 
    {
        //TODO: something
    }
    else 
    {
		//ConnettiFmDB();
    }

    
	TimerConnect->Enabled = true;
}

// ---------------------------------------------------------------------------

void TdmDB::ConnettiDB() {
    int m, p;

    try {
        ADOConnection1->Close();
        ADOConnection1->Open();
        LogTxt("Database Locale connected");
        // LeggiParametri(); // leggo parametri all'avvio
    }
    catch (...) {
        ADOConnection1->Connected = false;
    }
}

// ---------------------------------------------------------------------------


void TdmDB::DisconnettiDB() {
    ADOConnection1->Close();
    ADOConnection1->Connected = false;
    LogTxt("Database Locale disconnected");
}

// ---------------------------------------------------------------------------

void TdmDB::ConnettiFmDB()
{
    try {
        FMConnection->Close();
        FMConnection->Open();
        LogTxt("Database FmVision connected");
    }
    catch (...) 
    {
        FMConnection->Connected = false;
    }
}


void TdmDB::DisconnettiFmDB() 
{
    FMConnection->Close();
    FMConnection->Connected = false;
    LogTxt("Database FmVision disconnected");
}


// ---------------------------------------------------------------------------
int TdmDB::verificaConnessioneDB() {
    AnsiString strsql, articolo, pos;
    TADOQuery *ADOQuery;
    int res = 0;

    if (!ADOConnection1->Connected) {
        return 0;
    }
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
		strsql = "SELECT * FROM INFORMATION_SCHEMA.TABLES";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Close();
        res = 1;
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}
// ---------------------------------------------------------------------------

int TdmDB::LeggiCampoTabella(AnsiString Tabella, AnsiString CampoDaLeggere, AnsiString ClauslaWhere, AnsiString &Valore) {
    AnsiString strsql;

    TADOQuery *ADOQuery;

    int res = 0;

    if (!ADOConnection1->Connected) {

        return res;
    }
    Valore = ""; // Inizializzazione valore di ritorno

    // Creazione stringa Query

    strsql.printf("SELECT %s FROM %s %s", CampoDaLeggere.c_str(), Tabella.c_str(), ClauslaWhere.c_str());

    try {

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        // Verifica risultati
        if (!ADOQuery->Eof) {
            Valore = ADOQuery->FieldByName(CampoDaLeggere)->AsString;
        }
    }
    catch (Exception &E) {
        // return E
        ADOConnection1->Close();
        res = -1;
    }

    delete ADOQuery;

    return res;

}

// ---------------------------------------------------------------------------

void TdmDB::AcquisisciSegnalazioneAttiva(int nmsg) {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("UPDATE Segnalazioni SET Acquisito = GetDate() WHERE (N = '%d') AND Acquisito IS NULL", nmsg);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::AcquisisciSegnalazioneAttiva() {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 * FROM Segnalazioni WHERE Acquisito IS NULL ORDER BY DataOra");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (!ADOQuery->Eof) {
            ADOQuery->Edit();
            ADOQuery->FieldByName("Acquisito")->AsDateTime = Now();
            ADOQuery->Post();
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::LeggiSegnalazioneAttiva(AnsiString &msg, int &all) {

    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        msg = "";
        all = 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 * FROM Segnalazioni WHERE Acquisito IS NULL ORDER BY DataOra");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (!ADOQuery->Eof) {
            msg = ReadString(ADOQuery, "Messaggio");
            all = ReadInt(ADOQuery, "Allarme");
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::AcquisisciSegnalazioni() {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("UPDATE Segnalazioni SET Acquisito = GetDate() WHERE Acquisito IS NULL");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::Segnalazione(int nall, AnsiString msg, int all, int popup, int popupstation) {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        // strsql.printf("SELECT TOP (1) * FROM Segnalazioni WHERE (Messaggio = '%s') AND ((Acquisito IS NULL) OR (GETDATE() - Acquisito < 1.0 / 24.0 / 20.0))", msg);
        strsql.printf("SELECT TOP (1) * FROM Segnalazioni WHERE (Messaggio = '%s') AND (Acquisito IS NULL)", msg);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (!ADOQuery->Eof) {
            // Messaggio già inviato
            ADOQuery->Close();
            delete ADOQuery;
            return;
        }
        ADOQuery->Close();
        strsql.printf("INSERT INTO Segnalazioni (N, Messaggio, Allarme, Popup, StationPopup) VALUES (%d, '%s', %d, %d, %d)", nall, msg.c_str(), all, popup, popupstation);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::Log(AnsiString Sorgente, AnsiString Tipo, AnsiString Evento) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;

    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        if (Evento.Length() > 200) {
            Evento.SetLength(200);
        }
        while (Evento.Pos("'")) {
            Evento[Evento.Pos("'")] = '*';
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP (1) * FROM Storico ORDER BY DataOra DESC");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (!ADOQuery->Eof) {
            // Messaggio già loggato
            ev = ReadString(ADOQuery, "Evento");
            if (ev == Evento) {
                ADOQuery->Close();
                delete ADOQuery;
                return;
            }
        }
        ADOQuery->Close();
        strsql.printf("INSERT INTO Storico (Sorgente, Tipo, Evento) VALUES ('%s', '%s', '%s')", Sorgente, Tipo, Evento.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}

void TdmDB::LogMsg(AnsiString Descrizione) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev, oradel, Statostr;
    Word Year, Month, Day;
    int nagv;
    DecodeDate((Now() - 5), Year, Month, Day);
    oradel = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
    // oradel = DateToStr(Now() - 30) ;
    if (!ADOConnection1->Connected) {
        return;
    }
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        ADOQuery->Close();
        /* strsql.printf("delete from Allarmi where LogOp = 1 AND DataOra < '%s'",+oradel.c_str());
         ADOQuery->SQL->Text = strsql;
		 ADOQuery->ExecSQL();
         ADOQuery->Close(); */
        if (Descrizione.Length() > 400) {
			Descrizione.SetLength(400);
        }
        while (Descrizione.Pos("'")) {
            Descrizione[Descrizione.Pos("'")] = '*';
        }
		for (nagv = 1; nagv <= N_carrelli; nagv++) {
            Statostr += "Agv " + IntToStr(nagv) + " in pos " + IntToStr(MainForm->DatiAgv[nagv].pos) + " dest " + IntToStr(MainForm->DatiAgv[nagv].dest) + " ; ";
        }
        strsql.printf("Insert Into Allarmi ( DataOra , Descrizione , Missione ,Posizione,PosPrec ,Agv ,Nallarme ,Destinazione,Ripristinato,LogOp,StatoAgv) VALUES (current_timestamp,'%s',0,0,0,0,0,0,0,1,'%s')", Descrizione.c_str(),
            Statostr.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::LogCom(AnsiString Telegramma) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;

    try {
        if ((!ADOConnection1->Connected) || (!log_comunicazioni)) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("INSERT INTO LOG_COMUNICAZIONE " "(Origine, Destinazione, Tipo, Telegramma) " "VALUES ('%s','%s','%s','%s')", Telegramma.SubString(7, 2), Telegramma.SubString(9, 2), Telegramma.SubString(12, 2),
            Telegramma.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::Log_MISS(int agv, int posizione, int destinazione, AnsiString comando, AnsiString posbloccate) {
    logstr.sprintf("Comando ad AGV%d: posizione=%s, destinazione=%d, comando=%d, posbloccate=%s", agv, posizione, destinazione, comando.c_str(), posbloccate.c_str());
    Log("SERVER", "MISS", logstr);
}

void TdmDB::PuliziaStorici() {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("DELETE FROM Allarmi WHERE (LogOp = 0 AND (DataOra < (current_timestamp - 30))) OR (LogOp = 1 AND (DataOra < (current_timestamp - 3)))");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
        strsql.printf("DELETE FROM Missioni WHERE (Generata < (current_timestamp - 30))");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TdmDB::ADOConnection1AfterConnect(TObject *Sender) {
    if (ADOConnection1->Connected) {
        /* Log("SERVER", "EVENT", "Database locale connected");
         CaricaTabellaK("Postazioni WHERE SELLA IS NOT NULL AND Nodo > 0", "Sella", TabSelle);
         CaricaTabellaK("Postazioni", "Nodo", TabNodi);
         CaricaTabellaK("Postazioni", "ID_Postazione", TabPostazioni); */
    }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TdmDB::LeggiParametri(int baia) {
    AnsiString strsql, strlog = "";
    TADOQuery *ADOQuery;
    int res = 0, id;
    try {

        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "Select * from Plc ";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        while (!ADOQuery->Eof) {
            if ((baia == 0) || (baia == ADOQuery->FieldByName("Id")->AsInteger)) {
                id = ADOQuery->FieldByName("Id")->AsInteger;
                /* MainForm->DatiBaie[id].pos_apri_porta[1] = ADOQuery->FieldByName("PosPorta1")->AsString ;
                 MainForm->DatiBaie[id].pos_apri_porta[2] = ADOQuery->FieldByName("PosPorta2")->AsString ;
                 MainForm->DatiBaie[id].pos_apri_porta[3] = ADOQuery->FieldByName("PosPorta3")->AsString ;
                 MainForm->DatiBaie[id].pos_apri_porta[4] = ADOQuery->FieldByName("PosPorta4")->AsString ;
                 MainForm->DatiBaie[id].pos_rot_allarm[1] = ADOQuery->FieldByName("PosRot1")->AsString ;
                 MainForm->DatiBaie[id].pos_rot_allarm[2] = ADOQuery->FieldByName("PosRot2")->AsString ;
                 MainForm->DatiBaie[id].Destinazione = ADOQuery->FieldByName("Destinazione")->AsInteger ;
                 MainForm->DatiBaie[id].escludiPorte = ADOQuery->FieldByName("DisabilitaPorte")->AsInteger ;
                 MainForm->DatiBaie[id].agv_associato = ADOQuery->FieldByName("AgvAssociato")->AsInteger ;
                 MainForm->DatiBaie[id].generataPrel = ADOQuery->FieldByName("Generata")->AsInteger & MainForm->bit[0];
                 MainForm->DatiBaie[id].generataDep = ADOQuery->FieldByName("Generata")->AsInteger & MainForm->bit[1];
                 MainForm->DatiBaie[id].tipobaia = ADOQuery->FieldByName("TipoBaia")->AsInteger ;
                 MainForm->DatiBaie[id].ChiamataPrel = ADOQuery->FieldByName("StatoChiamate")->AsInteger & MainForm->bit[0];
                 MainForm->DatiBaie[id].ChiamataDep = ADOQuery->FieldByName("StatoChiamate")->AsInteger & MainForm->bit[1];
                 MainForm->DatiBaie[id].EscludiPrel = ADOQuery->FieldByName("Escludi")->AsInteger & MainForm->bit[0];
                 MainForm->DatiBaie[id].EscludiDep[1] = ADOQuery->FieldByName("Escludi")->AsInteger & MainForm->bit[1];
                 MainForm->DatiBaie[id].EscludiDep[2] = ADOQuery->FieldByName("Escludi")->AsInteger & MainForm->bit[2];
                 MainForm->DatiBaie[id].EscludiDep[3] = ADOQuery->FieldByName("Escludi")->AsInteger & MainForm->bit[3];
                 MainForm->PlcPhoenix[id].richiesta_porta_attiva[1] = ADOQuery->FieldByName("RichiestaAttiva")->AsInteger & MainForm->bit[0];
                 MainForm->PlcPhoenix[id].richiesta_porta_attiva[2] = ADOQuery->FieldByName("RichiestaAttiva")->AsInteger & MainForm->bit[1];
                 MainForm->PlcPhoenix[id].richiesta_porta_attiva[3] = ADOQuery->FieldByName("RichiestaAttiva")->AsInteger & MainForm->bit[2];
                 MainForm->PlcPhoenix[id].richiesta_porta_attiva[4] = ADOQuery->FieldByName("RichiestaAttiva")->AsInteger & MainForm->bit[3];
                 MainForm->PlcPhoenix[id].strApriPorta[1] = "porta1baia"+IntToStr(id);
                 MainForm->PlcPhoenix[id].strApriPorta[2] = "porta2baia"+IntToStr(id);
                 MainForm->PlcPhoenix[id].strApriPorta[3] = "porta3baia"+IntToStr(id);
                 MainForm->PlcPhoenix[id].strApriPorta[4] = "porta4baia"+IntToStr(id);
                 MainForm->PlcPhoenix[id].tempo_attesa = ADOQuery->FieldByName("TempoFtc")->AsInteger ; */
            }
            ADOQuery->Next();
        }
        ADOQuery->Close();
        lettoparametri = true;
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
}

/*
int TdmDB::AggiornaParametri(int baia) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0, richiesta = 0, i, generata = 0, escludi = 0, chiamate = 0;
    AnsiString Val;

    if ((!ADOConnection1->Connected) || (!lettoparametri)) {
        return 0;
    }
    try {
         for(i=0 ; i<=3;i++) {
         if (MainForm->PlcPhoenix[baia].richiesta_porta_attiva[i+1])
         richiesta |= MainForm->bit[i] ;
         }
         if (MainForm->DatiBaie[baia].ChiamataPrel)
         chiamate |= MainForm->bit[0] ;
         if (MainForm->DatiBaie[baia].ChiamataDep)
         chiamate |= MainForm->bit[1] ;
         if (MainForm->DatiBaie[baia].generataPrel)
         generata |= MainForm->bit[0] ;
         if (MainForm->DatiBaie[baia].generataDep)
         generata |= MainForm->bit[1] ;
         if (MainForm->DatiBaie[baia].EscludiPrel)
         escludi |= MainForm->bit[0] ;
         if (MainForm->DatiBaie[baia].EscludiDep[1])
         escludi |= MainForm->bit[1] ;
         if (MainForm->DatiBaie[baia].EscludiDep[2])
         escludi |= MainForm->bit[2] ;
         if (MainForm->DatiBaie[baia].EscludiDep[3])
         escludi |= MainForm->bit[3] ;
         ADOQuery = new TADOQuery(NULL);
         ADOQuery->Connection = ADOConnection1;
         strsql.printf("UPDATE Plc SET PosPorta1 = '%s',PosPorta2 = '%s',"
         "PosPorta3 = '%s',PosPorta4 = '%s',"
         "PosRot1 = '%s',PosRot2 = '%s',DisabilitaPorte =%d,RichiestaAttiva =%d,"
         "Destinazione =%d , AgvAssociato =%d , Generata =%d, Escludi =%d , StatoChiamate =%d, TempoFtc =%d WHERE id = %d",
         MainForm->DatiBaie[baia].pos_apri_porta[1].c_str(),
         MainForm->DatiBaie[baia].pos_apri_porta[2].c_str(),
         MainForm->DatiBaie[baia].pos_apri_porta[3].c_str(),
         MainForm->DatiBaie[baia].pos_apri_porta[4].c_str(),
         MainForm->DatiBaie[baia].pos_rot_allarm[1].c_str(),
         MainForm->DatiBaie[baia].pos_rot_allarm[2].c_str(),
         MainForm->DatiBaie[baia].escludiPorte,
         richiesta,
         MainForm->DatiBaie[baia].Destinazione,
         MainForm->DatiBaie[baia].agv_associato,
         generata,
         escludi,
         chiamate,
         MainForm->PlcPhoenix[baia].tempo_attesa,
         baia
         );
         ADOQuery->SQL->Text = strsql;
         res = ADOQuery->ExecSQL(); 
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}
*/

int TdmDB::AggiornaParametri(int id_parametro, int val, AnsiString valstr) 
{
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0, richiesta = 0, i, generata = 0, escludi = 0, chiamate = 0;
    AnsiString Val;

    if (!ADOConnection1->Connected)
        return 0;
    try 
    {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("UPDATE Parametri SET IntValue = %d ,StringValue = '%s' WHERE IdParametro = %d", val, valstr.c_str(), id_parametro);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}


void TdmDB::LeggiParametriGrafici() {
    AnsiString stringa, ora, carica, allarmi, str1;
    int pos, id, escludi, stato, statoplc;
    TADOQuery *Query2;
    char st[20];

    if (!ADOConnection1->Connected) {
        return;
    }
    try {
        Query2 = new TADOQuery(NULL);
        Query2->Connection = ADOConnection1;
        stringa = "Select * From OggettiGrafici ";
        str1 = AnsiString::StringOfChar('0', 99);
        try {
            Query2->Close();
            Query2->SQL->Clear();
            Query2->SQL->Append(stringa);
            Query2->Open();
            if (Query2->RecordCount > 0) {
                Query2->First();
                while (!Query2->Eof) {
                    id = Query2->FieldByName("id")->AsInteger;
/*					if (id <= 3) { // da 1 -9 pulsantiera, 10=PLC
						MainForm->DatiPlc.DatiSemafori[id].posx = Query2->FieldByName("PosX")->AsInteger;
						MainForm->DatiPlc.DatiSemafori[id].posy = Query2->FieldByName("PosY")->AsInteger;
					}else if (id <= 13){
						MainForm->DatiPlc.x_presenza_pallet[id-4] = Query2->FieldByName("PosX")->AsInteger;
						MainForm->DatiPlc.y_presenza_pallet[id-4] = Query2->FieldByName("PosY")->AsInteger;
						MainForm->DatiPlc.w_presenza_pallet[id-4] = Query2->FieldByName("Width")->AsInteger;
						MainForm->DatiPlc.h_presenza_pallet[id-4] = Query2->FieldByName("Height")->AsInteger;
					}    */
					Query2->Next();
				}
			}
			Query2->Close();
		}
		catch (...) {
			// ADOConnection1->Close();
			LogMsg("Eccezione su leggi parametri grafici");
			Query2->Close();
		}
	}
	catch (...) {
		// ADOConnection1->Close();
	}
	delete Query2;
}

void TdmDB::LeggiPulsanti() {
    AnsiString stringa, ora, carica, allarmi, str1;
    int pos, id, escludi, stato, statoplc;
    TADOQuery *Query2;
    char st[20];


}

void TdmDB::AggiornaPulsanti(int puls) {
    AnsiString stringa;
    int startcont = puls;
    TADOQuery *Query2;
    int res = 0;

}

void TdmDB::LeggiStatoAgv() {
	AnsiString stringa, ora, carica, allarmi, str1,testudc;
    int pos, nagv, escludi, stato, statoplc;
    TADOQuery *Query2;
    char st[20];

    if (!ADOConnection1->Connected) {
        return;
    }
    try {
        Query2 = new TADOQuery(NULL);
        Query2->Connection = ADOConnection1;
        nagv = 1;
        stringa = "Select * From StatoAgv ";
        str1 = AnsiString::StringOfChar('0', 99);
        try {
            Query2->Close();
            Query2->SQL->Clear();
            Query2->SQL->Append(stringa);
            Query2->Open();
            if (Query2->RecordCount > 0) {
                Query2->First();
                while (!Query2->Eof) {
                    nagv = Query2->FieldByName("id")->AsInteger;
					if (nagv <= N_carrelli) {
						MainForm->DatiAgv[nagv].pos = Query2->FieldByName("pos")->AsInteger;
						MainForm->DatiAgv[nagv].posprec = Query2->FieldByName("posprec")->AsInteger;
						MainForm->DatiAgv[nagv].load = Query2->FieldByName("loaded")->AsInteger;
						MainForm->DatiAgv[nagv].generata = Query2->FieldByName("Generata")->AsInteger;
						MainForm->DatiAgv[nagv].tipomis = Query2->FieldByName("Tipo_Missione")->AsInteger;
						MainForm->DatiAgv[nagv].idmis = Query2->FieldByName("idmis")->AsInteger;
						MainForm->DatiAgv[nagv].LivelloBatteria = Query2->FieldByName("livellobatt")->AsInteger;
						MainForm->DatiAgv[nagv].dest = Query2->FieldByName("dest")->AsInteger;
						MainForm->DatiAgv[nagv].prel = Query2->FieldByName("prelievo")->AsInteger;
						MainForm->DatiAgv[nagv].dep = Query2->FieldByName("deposito")->AsInteger;
						MainForm->DatiAgv[nagv].IdUdc = Query2->FieldByName("idudc")->AsString;
						MainForm->DatiAgv[nagv].IdUdcTest = Query2->FieldByName("idudc")->AsString;
					   //	MainForm->DatiAgv[nagv].tipopallet = Query2->FieldByName("TipoPallet")->AsInteger;
					   //	MainForm->DatiAgv[nagv].formato_pallet = Query2->FieldByName("FormatoPallet")->AsInteger;
						MainForm->DatiAgv[nagv].programma_fasciatura = Query2->FieldByName("ProgrammaFasciatura")->AsInteger;
						MainForm->DatiAgv[nagv].consensodato = Query2->FieldByName("ConsensoDato")->AsInteger;
						MainForm->DatiAgv[nagv].bitbattbassa = Query2->FieldByName("BattBassa")->AsInteger;
						if (nagv == 1) {


							MainForm->AbilitaMissioni = Query2->FieldByName("Abilitazioni")->AsInteger & MainForm->bit[0];
							MainForm->EscludiAvvolgitore = Query2->FieldByName("Abilitazioni")->AsInteger & MainForm->bit[1];
							MainForm->DisabilitaTelecamera = Query2->FieldByName("Abilitazioni")->AsInteger & MainForm->bit[2];
							MainForm->PrioritaMagazzino = Query2->FieldByName("Abilitazioni")->AsInteger & MainForm->bit[3];
                            MainForm->PosizioniLuceRossaSemaforo = Query2->FieldByName("PosizioniLuceRossaSemaforo")->AsString;
                            MainForm->PosizioniLuceGiallaSemaforo = Query2->FieldByName("PosizioniLuceGiallaSemaforo")->AsString;
							//MainForm->DatiPlc.h_prel = Query2->FieldByName("HPrelRulliera")->AsInteger ;
							//MainForm->poscb[1] = Query2->FieldByName("PosCb1")->AsInteger ;
							//MainForm->static_battery = Query2->FieldByName("StaticCb")->AsInteger ;
						}
						stato = Query2->FieldByName("stato")->AsInteger;
						MainForm->DatiAgv[nagv].registro5 = Query2->FieldByName("registro5")->AsInteger;
						MainForm->DatiAgv[1].stand_by = MainForm->DatiAgv[nagv].registro5 & MainForm->bit[0] ;
						allarmi = TrimRight(Query2->FieldByName("Allarmi")->AsString);
						MainForm->DatiAgv[nagv].manuale = stato & MainForm->bit[0];
						MainForm->DatiAgv[nagv].alarm = stato & MainForm->bit[1];
						MainForm->DatiAgv[nagv].semiautomatico = stato & MainForm->bit[2];
						strcpy(MainForm->DatiAgv[nagv].allarme, allarmi.c_str());
						strcpy(MainForm->DatiAgv[nagv].allarmiPrec, str1.c_str());
						// EdReturnCb->Text = IntToStr( temporitornocb);
						// DisegnaCarrello(nagv);
					}
					Query2->Next();
				}
			}
			Query2->Close();
			lettostatoagv = 1;
			MainForm->aggiornaagv = 1;
			LogMsg("Avvio Supervisione");
		}
        catch (...) {
			// ADOConnection1->Close();
            LogMsg("Eccezione su leggi stato agv");
            Query2->Close();
        }
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete Query2;
}

void TdmDB::AggiornaStatoAgv(int agv) {
    AnsiString stringa, allarmi, idpal;
    int pos, idmis, livello, i, j, escludi, stato, statoplc = 0, abilitazioni = 0;
    TADOQuery *Query2;
    int res = 0;

    if ((!ADOConnection1->Connected) || (!lettostatoagv)) {
        return;
    }
    try {
        Query2 = new TADOQuery(NULL);
        Query2->Connection = ADOConnection1;
        //abilitazione delle missioni 
        if (MainForm->AbilitaMissioni) {
            abilitazioni |= MainForm->bit[0];
        }
        //avvolgitore escluso dalla ciclica
        if (MainForm->EscludiAvvolgitore) {
            abilitazioni |= MainForm->bit[1];
        }
        //telecamera escluso dalla ciclica  -non servirebbe-
        if (MainForm->DisabilitaTelecamera) {
            abilitazioni |= MainForm->bit[2];
        }
        //dai priorità alle missioni magazzino-rulliera
        if (MainForm->PrioritaMagazzino) {
            abilitazioni |= MainForm->bit[3];
        }
		// abilitazioni |= MainForm->DatiPlc[1].escludiPorte ;
        for (i = 1; i <= N_carrelli; i++) {
            if ((i == agv) || (agv == 0)) {
                pos = MainForm->DatiAgv[i].pos;
                // idmis = MainForm->DatiAgv[i].idmis ;
                livello = MainForm->DatiAgv[i].LivelloBatteria;
                allarmi = MainForm->DatiAgv[i].allarmi;
                stato = MainForm->DatiAgv[i].stato;
                
                if (MainForm->DatiAgv[i].manuale) {
                    stato |= MainForm->bit[0];
                }
                if (MainForm->DatiAgv[i].alarm) {
                    stato |= MainForm->bit[1];
                }
                if (MainForm->DatiAgv[i].semiautomatico) {
                    stato |= MainForm->bit[2];
                }
                if (allarmi.Length() > 100) {
                    allarmi.SetLength(100);
                }
                // ora = DataOraSql() ;
                stringa = "UPDATE StatoAgv set pos = " + IntToStr(pos);
                stringa = stringa + " , posprec = " + IntToStr(MainForm->DatiAgv[i].posprec) + " , prelievo = " + IntToStr(MainForm->DatiAgv[i].prel);
                stringa = stringa + " , deposito = " + IntToStr(MainForm->DatiAgv[i].dep);
                stringa = stringa + " , generata = " + IntToStr(MainForm->DatiAgv[i].generata);
				stringa = stringa + " , Tipo_Missione = " + IntToStr(MainForm->DatiAgv[i].tipomis);
                stringa = stringa + " , stato = " + IntToStr(MainForm->DatiAgv[i].stato);
                stringa = stringa + " , dest = " + IntToStr(MainForm->DatiAgv[i].dest);
                stringa = stringa + " , loaded = " + IntToStr(MainForm->DatiAgv[i].load) + " , livellobatt = " + IntToStr(livello);
				stringa = stringa + " , idmis = " + IntToStr(MainForm->DatiAgv[i].idmis);
				stringa = stringa + " , idudc = '" + MainForm->DatiAgv[i].IdUdc + "'";
				//stringa = stringa + " , idudc = " + MainForm->DatiAgv[i].IdUdcTest;
				//stringa = stringa + " , Tipopallet = " + IntToStr(MainForm->DatiAgv[i].tipopallet);
				//stringa = stringa + " , Formatopallet = " + IntToStr(MainForm->DatiAgv[i].formato_pallet);
				stringa = stringa + " , ProgrammaFasciatura = " + IntToStr(MainForm->DatiAgv[i].programma_fasciatura);
                stringa = stringa + " , registro5 = " + IntToStr(MainForm->DatiAgv[i].registro5);
                stringa = stringa + " , ConsensoDato = " + IntToStr(MainForm->DatiAgv[i].consensodato);
                stringa = stringa + " , BattBassa = " + IntToStr(MainForm->DatiAgv[i].bitbattbassa);
                // hprel, offsetdeposito ????
                if (i == 1) {
                    statoplc = 0;

					stringa = stringa + " , Abilitazioni = " + IntToStr(abilitazioni);
					stringa = stringa + " , PosizioniLuceRossaSemaforo = '"+MainForm->PosizioniLuceRossaSemaforo+"'";
					stringa = stringa + " , PosizioniLuceGiallaSemaforo = '"+MainForm->PosizioniLuceGiallaSemaforo+"'";
                }
                stringa = stringa + " , Allarmi = '" + allarmi + "' where id = " + IntToStr(i);

                try {
                    Query2->Close();
                    Query2->SQL->Clear();
                    Query2->SQL->Append(stringa);
                    // MainForm->LogMsg(stringa);
                    Query2->ExecSQL();
					Query2->Close();
                }
                catch (...) {
                    // ADOConnection1->Close();
                    LogMsg("Eccezione su Aggiorno stato agv");
                    Query2->Close();
                }
            }
        }
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete Query2;

}

void TdmDB::GestioneAllarmi(int agv) {
    int nagv;

    AnsiString str1, messalarm, StringaMail, data, ora, stringa, Statostr;
    // Variant locvalues[4];
    TLocateOptions Opts;
    AnsiString oradel;
    Word Year, Month, Day;
    TADOQuery *QueryAllarmi, *QueryUpdate;
    str1 = AnsiString::StringOfChar('0', 99);
    int i, n;

    DecodeDate((Now() - 30), Year, Month, Day);
    oradel = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
    // oradel = DateToStr(Now() - 30) ;
    if (ADOConnection1->Connected) {
        try {
            QueryAllarmi = new TADOQuery(NULL);
            QueryAllarmi->Connection = ADOConnection1;
            QueryUpdate = new TADOQuery(NULL);
            QueryUpdate->Connection = ADOConnection1;
            /* QueryAllarmi->Close();
             QueryAllarmi->SQL->Clear();
             stringa.printf("delete from Allarmi where LogOp = 0 AND DataOra < '%s'",+oradel.c_str());
             QueryAllarmi->SQL->Append(stringa);
             QueryAllarmi->ExecSQL(); */
            QueryAllarmi->Close();
            for (nagv = 1; nagv <= N_carrelli; nagv++) {
                Statostr += "Agv " + IntToStr(nagv) + " in pos " + IntToStr(MainForm->DatiAgv[nagv].pos) + " dest " + IntToStr(MainForm->DatiAgv[nagv].dest) + " ; ";
            }

			for (nagv = 1; nagv <= N_carrelli; nagv++) {
                if ((nagv == agv) || (agv == 0)) {
                    if (strcmp(MainForm->DatiAgv[nagv].allarme, "NONE")) { // se ho allarme tara devo entrare qua lo stesso
                        strcpy(MainForm->DatiAgv[nagv].allarmi, str1.c_str());
                        MainForm->DatiAgv[nagv].allarmi[99] = '0';
                        if (strcmp(MainForm->DatiAgv[nagv].allarme, "NONE")) { // se non ho allarmi lascio la stringa tutta a 0
                            for (i = 0; i < strlen(MainForm->DatiAgv[nagv].allarme); i += 2) {
                                sscanf(MainForm->DatiAgv[nagv].allarme + i, "%2X", &n);
                                if ((n > 0) && (n < 100)) {
                                    MainForm->DatiAgv[nagv].allarmi[n] = '1';
                                }
                            }
                        }
                        for (n = 0; n < 100; n++) {
                            if ((MainForm->DatiAgv[nagv].allarmiPrec[n] == '0') && (MainForm->DatiAgv[nagv].allarmiPrec[n] != MainForm->DatiAgv[nagv].allarmi[n])) {
                                if (!MainForm->DatiAgv[nagv].manuale) { // discrimino cb difettoso e allarmi manuali
                                    if (n < 10) {
                                        messalarm = IntToStr(nagv) + "0" + IntToStr(n);
                                    }
                                    else {
                                        messalarm = IntToStr(nagv) + IntToStr(n);
                                    }
                                    // controllo che l'allare precedente sia diverso da questo
                                    if (MainForm->DatiAgv[nagv].idmis > 0) {
                                        stringa = "Select * From Allarmi where Nallarme = '" + IntToStr(n) + "' AND Posizione =" + IntToStr(MainForm->DatiAgv[nagv].pos);
                                        stringa += " AND Missione =" + IntToStr(MainForm->DatiAgv[nagv].idmis);
                                    }
                                    else {
                                        DecodeDate((Now()), Year, Month, Day);
                                        data = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year);
                                        // il separatore per le query sulle tabelle di BDE è il . e non la barra /
                                        ora = TimeToStr(Time() - StrToTime("00:05:00"));
                                        stringa = "Select * From Allarmi where Nallarme = '" + IntToStr(n) + "' AND Posizione =" + IntToStr(MainForm->DatiAgv[nagv].pos);
                                        stringa += " AND Destinazione =" + IntToStr(MainForm->DatiAgv[nagv].dest) + " AND DataOra > '" + data + " " + ora + "'";
                                    }
                                    MainForm->MessageBar1->Messages->Strings[StrToInt(messalarm)] = "Agv " + IntToStr(nagv) + " in Allarme : " + MainForm->MessageBar1->Messages->Strings[n];
                                    if (n != 23) { // non salvo lo stand by
                                        try {
                                            QueryAllarmi->Close();
                                            QueryAllarmi->SQL->Clear();
                                            QueryAllarmi->SQL->Append(stringa);
                                            QueryAllarmi->Open();
                                            if (!QueryAllarmi->RecordCount) {
                                                QueryAllarmi->Close();
                                                QueryAllarmi->SQL->Clear();
                                                stringa = "Insert Into Allarmi ( DataOra , Descrizione , Posizione,PosPrec ,Destinazione,Missione,Agv,NAllarme,Ripristinato,StatoAgv,LogOp )";
                                                stringa = stringa + " VALUES (current_timestamp,'" + MainForm->MessageBar1->Messages->Strings[StrToInt(messalarm)] + "'," +
                                                    IntToStr(MainForm->DatiAgv[nagv].pos) + "," +
                                                    IntToStr(MainForm->DatiAgv[nagv].posprec) + "," + IntToStr(MainForm->DatiAgv[nagv].dest) + "," +
                                                    IntToStr(MainForm->DatiAgv[nagv].idmis) + ",'" + IntToStr(nagv) + "','" + IntToStr(n) + "',0,'" + Statostr + "',0)";
                                                QueryAllarmi->SQL->Append(stringa);
                                                QueryAllarmi->ExecSQL();
                                            }
                                            QueryAllarmi->Close();
                                        }
                                        catch (...) {
                                            // ADOConnection1->Close();
                                            LogMsg("Exception Insert Alarm");
                                            QueryAllarmi->Close();
                                        }
                                    }

                                    MainForm->DatiAgv[nagv].allarmiPrec[n] = MainForm->DatiAgv[nagv].allarmi[n];
                                    if (n != 41) { // non imposto allarme su cb non funzionante
                                        MainForm->DatiAgv[nagv].alarm = true;
                                    }
                                    MainForm->MessageBar1->AddMsgNo(StrToInt(messalarm));
                                }
                            }
                            else
                                if ((MainForm->DatiAgv[nagv].allarmiPrec[n] == '1') && (MainForm->DatiAgv[nagv].allarmiPrec[n] != MainForm->DatiAgv[nagv].allarmi[n])) {
                                if (n < 10) {
                                    messalarm = IntToStr(nagv) + "0" + IntToStr(n);
                                }
                                else {
                                    messalarm = IntToStr(nagv) + IntToStr(n);
                                }
                                MainForm->DatiAgv[nagv].allarmiPrec[n] = MainForm->DatiAgv[nagv].allarmi[n];
                                MainForm->MessageBar1->DelMsgNo(StrToInt(messalarm));
                                QueryAllarmi->Close();
                                QueryAllarmi->SQL->Clear();
                                try {
                                    // UPDATE    Alarms   SET Time = CONVERT(varchar, GETDATE() - DateTime, 108)  //108 è il parametro per hh:mm:ss
									stringa = "Select * From Allarmi where NAllarme = '" + IntToStr(n) + "' AND Agv = '" + IntToStr(nagv) + "' AND Tempo IS NULL ";
                                    QueryAllarmi->SQL->Append(stringa);
                                    QueryAllarmi->Open();
                                    if (QueryAllarmi->RecordCount) {
                                        while (!QueryAllarmi->Eof) {
                                            // stringa = stringa +" VALUES (GetDate(),'"+MainForm->MessageBar1->Messages->Strings[StrToInt(messalarm)]+"',"+IntToStr(MainForm->DatiAgv[nagv].pos)+","+IntToStr(MainForm->DatiAgv[nagv].posprec)+","+IntToStr(MainForm->DatiAgv[nagv].dest)+","+IntToStr(MainForm->DatiAgv[nagv].idmis)+",'"+IntToStr(nagv)+"','"+IntToStr(n)+"',0)";
                                            ora = ReturnTimeDaDateTime(DateTimeToStr(QueryAllarmi->FieldByName("DataOra")->AsDateTime));
                                            stringa = "Update Allarmi Set DataOraRipristino = current_timestamp , Tempo = '" + ora + "' , Ripristinato = ";
                                            if (MainForm->DatiAgv[nagv].manuale) {
                                                stringa = stringa + " 1 ";
                                            }
                                            else
                                                if (MainForm->DatiAgv[nagv].bumper_bypass) {
                                                stringa = stringa + " 3 ";
                                            }
                                            else {
                                                stringa = stringa + " 2 ";
                                            }
                                            stringa = stringa + " where NAllarme = '" + IntToStr(n) + "' AND Agv = '" + IntToStr(nagv) + "' AND Tempo IS NULL ";
                                            QueryUpdate->Close();
                                            QueryUpdate->SQL->Clear();
                                            QueryUpdate->SQL->Append(stringa);
                                            QueryUpdate->ExecSQL();
                                            QueryUpdate->Close();
                                            QueryAllarmi->Next();
                                        }
                                    }
                                    // stringa = stringa +" where NAllarme = '"+IntToStr(i)+"' AND Agv = '"+IntToStr(nagv)+"' AND Tempo IS NULL ";
                                    // QueryAllarmi->SQL->Append(stringa) ;
                                    // QueryAllarmi->ExecSQL();
                                    QueryAllarmi->Close();
                                }
                                catch (...) {
                                    // ADOConnection1->Close();
                                    LogMsg("Error Save Restarted Alarm 1 ");
                                    QueryAllarmi->Close();
                                }
                            }
                        }
                    }
                    else {
                        if (strcmp(MainForm->DatiAgv[nagv].allarmiPrec, str1.c_str())) {
                            for (i = 0; i < 100; i++) {
                                if (MainForm->DatiAgv[nagv].allarmiPrec[i] == '1') {
                                    if (i < 10) {
                                        messalarm = IntToStr(nagv) + "0" + IntToStr(i);
                                    }
                                    else {
                                        messalarm = IntToStr(nagv) + IntToStr(i);
                                    }
                                    MainForm->MessageBar1->DelMsgNo(StrToInt(messalarm));
                                    QueryAllarmi->Close();
                                    QueryAllarmi->SQL->Clear();
                                    try {
                                        stringa = "Select * From Allarmi where NAllarme = '" + IntToStr(i) + "' AND Agv = '" + IntToStr(nagv) + "' AND Tempo IS NULL ";
                                        QueryAllarmi->SQL->Append(stringa);
                                        QueryAllarmi->Open();
                                        if (QueryAllarmi->RecordCount) {
                                            while (!QueryAllarmi->Eof) {
                                                // stringa = stringa +" VALUES (GetDate(),'"+MainForm->MessageBar1->Messages->Strings[StrToInt(messalarm)]+"',"+IntToStr(MainForm->DatiAgv[nagv].pos)+","+IntToStr(MainForm->DatiAgv[nagv].posprec)+","+IntToStr(MainForm->DatiAgv[nagv].dest)+","+IntToStr(MainForm->DatiAgv[nagv].idmis)+",'"+IntToStr(nagv)+"','"+IntToStr(n)+"',0)";
                                                ora = ReturnTimeDaDateTime(DateTimeToStr(QueryAllarmi->FieldByName("DataOra")->AsDateTime));
                                                stringa = "Update Allarmi Set DataOraRipristino = current_timestamp , Tempo = '" + ora + "' , Ripristinato = ";
                                                if (MainForm->DatiAgv[nagv].manuale) {
                                                    stringa = stringa + " 1 ";
                                                }
                                                else
                                                    if (MainForm->DatiAgv[nagv].bumper_bypass) {
                                                    stringa = stringa + " 3 ";
                                                }
                                                else {
                                                    stringa = stringa + " 2 ";
                                                }
                                                stringa = stringa + " where NAllarme = '" + IntToStr(i) + "' AND Agv = '" + IntToStr(nagv) + "' AND Tempo IS NULL ";
                                                QueryUpdate->Close();
                                                QueryUpdate->SQL->Clear();
                                                QueryUpdate->SQL->Append(stringa);
                                                QueryUpdate->ExecSQL();
                                                QueryUpdate->Close();
                                                QueryAllarmi->Next();
                                            }
                                        }
                                        // stringa = stringa +" where NAllarme = '"+IntToStr(i)+"' AND Agv = '"+IntToStr(nagv)+"' AND Tempo IS NULL ";
                                        // QueryAllarmi->SQL->Append(stringa) ;
                                        // QueryAllarmi->ExecSQL();
                                        QueryAllarmi->Close();
                                    }
                                    catch (...) {
                                        // ADOConnection1->Close();
                                        LogMsg("Error Save Restarted Alarm 2 ");
                                        QueryAllarmi->Close();
                                    }
                                }
                            }
                            /* if ((MainForm->DatiAgv[nagv].alarm)&&(program == 1))
                             LogMsg("Agv "+IntToStr(nagv) + " non in allarme "); */
                        }
                        strcpy(MainForm->DatiAgv[nagv].allarmi, str1.c_str());
                        strcpy(MainForm->DatiAgv[nagv].allarmiPrec, str1.c_str());
                        MainForm->DatiAgv[nagv].allarmi[99] = '0';
                        MainForm->DatiAgv[nagv].allarmiPrec[99] = '0';
                        MainForm->DatiAgv[nagv].alarm = false;

                    }
                }
            }
        }
        catch (...) {
            // ADOConnection1->Close();
        }
        delete QueryAllarmi;
        delete QueryUpdate;
    }
    // InvioMail(ListaAllarmi->Text);
}

int TdmDB::CreaIdMissione() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idmiss = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT * FROM Missioni order by Generata");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        idmiss = ADOQuery->FieldByName("Id")->AsInteger;
        do {
            if (idmiss == 32678) {
                idmiss = 1;
            }
            else {
                idmiss++;
            }
        }
        while (ADOQuery->Locate("Id", idmiss, Opts));
        ADOQuery->Close();
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return idmiss;
}

int TdmDB::GeneraMissione(TMissione m) 
{
	int res = 0, idmiss, tabcarico = 0, tabscarico = 0, Priorita = 1,no_h_forche,larghezza_vassoio = 1,larghezza_udc = 1;
    TADOQuery *ADOQuery;
	AnsiString strsql, nomebaiaprel ="",nomebaiadep="";
    Evento ev;

    if (!ADOConnection1->Connected) 
        return 0;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = ADOConnection1;
    try 
    {
		idmiss = CreaIdMissione();
		if ((m.Source == "")&&(m.tipo_mis != 2))
			m.Source = ReturnNomePosDaPosEFormato(m.posprel, m.formato_pallet) ;
		else if (m.tipo_mis == 2)
			m.Source = "PosizioneAgv";
		if (m.Dest == "")
			m.Dest = ReturnNomePosDaPosEFormato(m.posdep, m.formato_pallet) ;
		// if ((m.Agv)&&((MainForm->DatiAgv[m.Agv].incarica)||(DMGestioneEventi->AgvInPosCb(m.Agv))))
		// dmComandiAgv->NoCharge(m.Agv);
		if (m.tipo_mis == 2) {
			m.h_prel = 400;
			m.tab_carico = 0;
		}else
			CompilaDatiPosizione(m.Source,1,m.formato_pallet,m.h_prel,m.tab_carico);
		CompilaDatiPosizione(m.Dest,0,m.formato_pallet,m.h_dep,m.tab_scarico);
        // Se carico un pallet da fasciare devo dire all'agv di andare piano (è stato deciso tab carico 1)
        if (m.tipo_pallet == TYPE_FASCIARE) 
        {
            m.tab_carico = 1;    
        }
        Priorita = m.priorita;
		if ((idmiss > 0) && (tab.mappa_nodi.find(m.posprel) != tab.mappa_nodi.end() || m.tipo_mis == 2) && (tab.mappa_nodi.find(m.posdep) != tab.mappa_nodi.end())) {
			dmDB->LogMsg("Missione per TipoPallet = "+IntToStr(m.tipo_pallet)+" , formato pallet : "+IntToStr(m.formato_pallet));
			strsql.printf
				("INSERT INTO Missioni ( ID,POSPREL ,POSDEP, ESITO ,GENERATA,TIPOMISSIONE,PRIORITA,AGV,HPREL,"
				"HDEP,PosPrelCliente,PosDepCliente,IdUdc,TraslPick,TraslDrop,TabCarico,TabScarico,ProgressivoMissione, ProgrammaFasciatura)"
				 " VALUES (%d,%d,%d,'%s',%s,%d,%d,%d,%d"
				 ",%d,'%s','%s','%s',%d,%d,%d,%d,%d,%d)"
				 ,idmiss, m.posprel, m.posdep,"GENERATA", "GetDate()", m.tipo_mis, Priorita, m.Agv, m.h_prel
				 ,m.h_dep,  m.Source.c_str(), m.Dest.c_str(), m.IdUdc, 0,0, m.tab_carico, m.tab_scarico
                 ,m.progressivo_missione, m.programma_fasciatura);
            ADOQuery->SQL->Text = strsql;
			res = ADOQuery->ExecSQL();
			sprintf(ev, "%c,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,0,0,%d,%d", EV_NUOVAMISSIONE, idmiss, m.posprel, m.h_prel, m.posdep, m.h_dep,Priorita, m.tipo_mis, m.tab_carico, m.tab_scarico, m.Agv,0,0);       //passo come variabile trasl_pick/drp il tipo udc
            cs->SendEv(ev);
            if (res == 1) {
                if (m.Agv) {
                    MainForm->DatiAgv[m.Agv].idmis = idmiss;
                    MainForm->DatiAgv[m.Agv].generata = 1;
                    if (m.tipo_mis < 2) {
                        MainForm->DatiAgv[m.Agv].dest = m.posprel;
                    }
                    else {
                        MainForm->DatiAgv[m.Agv].dest = m.posdep;
                    }
                    MainForm->DatiAgv[m.Agv].prel = m.posprel;
                    MainForm->DatiAgv[m.Agv].dep = m.posdep;
					MainForm->DatiAgv[m.Agv].tipomis = m.tipo_mis;
					AggiornaStatoAgv(m.Agv);
                }
                if (m.tipo_mis < 2) 
                {
                    dmDB->PrenotaPosIntera(m.posprel,1);
					//dmDB->PrenotaNomePosizione(m.Source,1);
				}
                if (m.tipo_mis < 4) 
                {
                    dmDB->PrenotaPosIntera(m.posdep,1);
					//dmDB->PrenotaNomePosizione(m.Dest,1);
                }
                /* if (MainForm->DatiPlc[1].pos == m.posprel) {
                 MainForm->DatiPlc[1].generata = 1 ;
                 MainForm->DatiPlc[1].m.posdep_missione = m.posdep ;
                 } */
                AggiornaStatoAgv(1);
                CaricaTabelle();
                AggiornaMemoriePlc();
                MainForm->aggiornamappa = 1;
                // dmComandiAgv->AggiornaPrenotazioni();
            }
        }
        else {
            strsql = "Id Missione non trovato o m.posprel " + IntToStr(m.posprel) + " o m.posdep " + IntToStr(m.posdep) + " non presenti nel crs";
        }
        LogMsg(strsql);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDB::GeneraMissioneDemo(int pos_prel, int h_prel, int pos_dep, int h_dep, int tipo_mis, int agv) {
    int res = 0, idmiss, tabcarico = 0, tabscarico = 0, Priorita = 1;
    TADOQuery *ADOQuery;
    AnsiString strsql, nomebaiaprel, nomebaiadep;
    Evento ev;
    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        idmiss = CreaIdMissione();
        nomebaiaprel = "POS" + IntToStr(pos_prel);
        nomebaiadep = "POS" + IntToStr(pos_dep);
        if ((idmiss > 0) && (tab.mappa_nodi.find(pos_prel) != tab.mappa_nodi.end()) && (tab.mappa_nodi.find(pos_dep) != tab.mappa_nodi.end())) {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            strsql.printf
				("INSERT INTO Missioni ( ID,POSPREL ,POSDEP, ESITO ,GENERATA," "TIPOMISSIONE,IdUdc,PRIORITA,AGV,HPREL,HDEP,HCORSIA,CORSIADEP,PIANOPREL,PIANODEP,POSPRELCLIENTE,POSDEPCLIENTE,CodiceUdc,Lotto)"
                " VALUES (%d,%d,%d,'%s',%s,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s')", idmiss, pos_prel,
                pos_dep, "GENERATA", "GetDate()", tipo_mis, 0, Priorita, agv, h_prel, h_dep, 0, 0, 1, 1, nomebaiaprel.c_str(), nomebaiadep.c_str(), "0", "0");
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            sprintf(ev, "%c,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,0,0,0,%d", EV_NUOVAMISSIONE, idmiss, pos_prel, h_prel, pos_dep, h_dep, Priorita, tipo_mis, tabcarico, tabscarico, agv, 0);
            cs->SendEv(ev);
            if (res == 1) {
                if (agv) {
                    MainForm->DatiAgv[agv].idmis = idmiss;
                    MainForm->DatiAgv[agv].generata = 1;
                    if (tipo_mis < 2) {
                        MainForm->DatiAgv[agv].dest = pos_prel;
                    }
                    else {
                        MainForm->DatiAgv[agv].dest = pos_dep;
                    }
                    MainForm->DatiAgv[agv].prel = pos_prel;
                    MainForm->DatiAgv[agv].dep = pos_dep;
                    MainForm->DatiAgv[agv].tipomis = tipo_mis;
                }
                AggiornaStatoAgv(agv);
                MainForm->aggiornamappa = 1;
                // dmComandiAgv->AggiornaPrenotazioni();
            }
        }
        else {
            strsql = "Id Missione non trovato o pos_prel " + IntToStr(pos_prel) + " o pos_dep " + IntToStr(pos_dep) + " non presenti nel crs";
        }
        LogMsg(strsql);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDB::AggiornaMissione(int id, int stato, int agv) {
	AnsiString strsql, time;
	TADOQuery *ADOQuery;
	int res = 0;

	try {
		if (stato > 2) {
			time = ReturnTimeMission(id);
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;
		strsql = "UPDATE Missioni SET Esito = ";
		switch (stato) {
		case 1:
			strsql = strsql + "'INIZIATA' , Iniziata = current_timestamp, Agv = " + IntToStr(agv) + " , BatteriaIniziale = " + IntToStr(MainForm->DatiAgv[agv].LivelloBatteria);
			break;
		case 2:
			strsql = strsql + "'PRELEVATO' , Prelevato = current_timestamp";
			break;
		case 3:
			strsql = strsql + "'COMPLETATA' , Fine = current_timestamp, BatteriaFinale = " + IntToStr(MainForm->DatiAgv[agv].LivelloBatteria) + " , Tempo = '" + time + "'";
			break;
		case 4:
            strsql = strsql + "'ABORTITA' , Fine = current_timestamp, BatteriaFinale = " + IntToStr(MainForm->DatiAgv[agv].LivelloBatteria) + " , Tempo = '" + time + "'";
            break;
        case 5:
			strsql = strsql + "'ANNULLATA' , Fine = current_timestamp, BatteriaFinale = " + IntToStr(MainForm->DatiAgv[agv].LivelloBatteria) + " , Tempo = '" + time + "'";
            break;
        }
        strsql = strsql + " where id = " + IntToStr(id);
        ADOQuery->SQL->Text = strsql;
		res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
        // ADOConnection1->Close();
    }
	delete ADOQuery;
    return res;
}
int TdmDB::AggiornaMissioneWms(int progressivo, int stato, int agv) {
	AnsiString strsql, time;
	TADOQuery *ADOQuery;
	int res = 0;

	try {
		/*if (stato > 2) {
			time = ReturnTimeMission(id);
		} */
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = FMConnection;
		strsql = "UPDATE LGV SET Stato = "+IntToStr(stato);
	 /*	switch (stato) {
		case 2:
			strsql = strsql + " , DataOraInizioMissione = GetDate() , Agv = "+IntToStr(agv); //iniziata
			break;
		case 3:
			strsql = strsql + " , DataOraPrelievoMissione = GetDate()";   //prelevato
			break;
		case 4:
			strsql = strsql + " , DataOraFineMissione = GetDate()";   //completata
			break;
		case 5:
			strsql = strsql + " , DataOraFineMissione = GetDate()";  //abortita
			break;
		case 6:
			strsql = strsql + " , DataOraFineMissione = GetDate()";  //abortita
			break;
		}  */
		strsql = strsql + " where ID = " + IntToStr(progressivo);
        ADOQuery->SQL->Text = strsql;
		res = ADOQuery->ExecSQL();
        dmDB->LogMsg("Aggiorno Missione WMS : "+ strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
        // ADOConnection1->Close();
    }
	delete ADOQuery;
    return res;
}

int TdmDB::AggiornaDestinazioneMissione(int id, int pos_dep) {
	AnsiString strsql, time;
	TADOQuery *ADOQuery;
	int res = 0;

	try {
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;
		strsql = "UPDATE Missioni SET posdep = " + IntToStr(pos_dep);
        strsql += " where id = " + IntToStr(id);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnTimeMission(int idmis) {
    AnsiString stringa, time = "00:00:00";
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from missioni where Id = " + IntToStr(idmis);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            if (ADOQuery->FieldByName("Iniziata")->AsString != "") {
                time = TimeToStr(Now() - ADOQuery->FieldByName("Iniziata")->AsDateTime);
            }
            else {
                time = "00:00:00";
            }
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return time;
}

AnsiString TdmDB::ReturnTimeDaDateTime(AnsiString DataOra) {
    AnsiString stringa, time = "00:00:00";
    AnsiString strsql;
    int res = 0;

    try {
        time = TimeToStr(Now() - StrToDateTime(DataOra));
    }
    catch (...) {
        // ADOConnection1->Close();
        time = "00:00:00";
    }
    return time;
}

void TdmDB::ModificaCorsiePostazioniDatabase() {
	AnsiString strsql, nomecorsia, zona, poscliente, poscliente1, poscliente2, poscliente3, poscliente4;
    int res, i, j, k, fila, p, posiniziocorsia, posinizioopposta, h_prel, h_prel2, verticale, wshape, hshape, n_file_corsia, n_alveoli, pos_prec, next_pos, posopposta, pos;
    int prima_pos_in_alto, h_dep1, h_dep2, h_dep3, n_file_corsia_terra, n_alveoli_terra, piani_max, tipo_pallet, num_fila_magazzino;
    TADOQuery *ADOQuery;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        h_dep1 = 200;
        h_dep2 = 2600;
        h_dep3 = 3400;
        h_prel = 95;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append("delete from Corsie where fila>=150 ");
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append("delete from Posizioni where pos>=1500 ");
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();

        zona = "B";
        n_file_corsia = 8;
        n_file_corsia_terra = 3;
        n_alveoli_terra = 4;
        num_fila_magazzino = 1;
        verticale = 1;
        wshape = 16;
        hshape = 16;

        for (j = 1; j <= (n_file_corsia + n_file_corsia_terra); j++) {
            if (j == 1) {
                fila = 155;
                piani_max = 2;
                posiniziocorsia = 1550;
                n_alveoli = 8;
                tipo_pallet = PALLET120x100; // tipo pallet universale
                prima_pos_in_alto = 1;
                h_dep1 = 300;

            }
            else if (j == 9) {
                fila = 150;
                piani_max = 1;
                posiniziocorsia = 1500;
                n_alveoli = 3;
                prima_pos_in_alto = 0;
                nomecorsia = "T03SA00000";
                tipo_pallet = 10; // tipo pallet universale
                h_dep1 = 450;
            }

            if (j < 9) {
                nomecorsia = "M05SB0" + IntToStr(j) + "100";
            }

            strsql.printf
				("INSERT INTO Corsie " "(Fila,NomeCorsia,PianiMax,CodiceArticolo,IdArticolo,TipoPallet,Abilitata,InUsoPrelievo,CorsiaPrenotata,Piena,Vuota,Zona,InUsoDeposito,Priorita,Hdep1,Hdep2,Hdep3,NumPos,PrimaPosInAlto) "
                "VALUES (%d,'%s',%d,'%s',%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d)", fila,
                nomecorsia.c_str(), piani_max, "", 0, tipo_pallet // tipo pallet
                , 0, 0, 0 // corsia prenotata
                , 0, 0, zona.c_str(), 0, 0, h_dep1 // hdep1
                , h_dep2, h_dep3, n_alveoli, prima_pos_in_alto);
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();
            // posiniziocorsia = ;//PosInizioCorsia(i, j, posinizioopposta);

            for (k = 1; k <= n_alveoli; k++) {
                pos = posiniziocorsia + k;
                if (k == 1) {
                    pos_prec = 0;
                }
                else {
                    pos_prec = pos - 1;
                }
                if (k == n_alveoli) {
                    next_pos = 0;
                }
                else {
                    next_pos = pos + 1;
                }

                if (piani_max == 2) {
                    poscliente = nomecorsia.SubString(1, nomecorsia.Length() - 3);
                    poscliente1 = poscliente + "10" + IntToStr(k);
                    poscliente2 = poscliente + "20" + IntToStr(k);
                    poscliente3 = poscliente + "30" + IntToStr(k);
                    poscliente = poscliente1;
                }
                else {
                    // poscliente = "T03SA0" + IntToStr(j - 15) + "00" + IntToStr(k); //
                    poscliente = "T03SA00000";
                    poscliente1 = poscliente;
                    poscliente2 = poscliente;
                    poscliente3 = poscliente;
                }

                strsql.printf
                    ("INSERT INTO Posizioni " "(Pos,CustomerPos,Prenotata,Disabilita,TipoPosizione" ",Vertical,Height,Width,NomeCorsia,Fila,PrevPosition,NextPosition,PosInFila,Zona"
                    ",IdUdc1,IdUdc2,IdUdc3,CustomerPos1,CustomerPos2,CustomerPos3,HPrel,HDep) " "VALUES (%d,'%s',%d,%d,%d,%d,%d,%d,'%s'"
                    ",%d,%d,%d,%d,'%s',%d,%d,%d,'%s','%s','%s',%d,%d)", pos, poscliente.c_str(), 0, 0, tipo_pallet, verticale, hshape, wshape, nomecorsia.c_str(), fila, pos_prec, next_pos, k, zona.c_str(), 0, 0, 0,
                    poscliente1.c_str(),
                    poscliente2.c_str(), poscliente3.c_str(), h_prel, MainForm->quota_dep_terra);
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Text = strsql;
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();
            }
            fila++;
            num_fila_magazzino++;
            posiniziocorsia = posiniziocorsia + 10;
        }
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
}

void TdmDB::InserisciCorsiePostazioniDatabase() {
	AnsiString strsql, nomecorsia, zona, poscliente,pos_str;
	int res,ordinamento, i, j, k, fila, p, posiniziocorsia, posinizioopposta, verticale, wshape, hshape, n_file_corsia, n_alveoli, pos_prec, next_pos, posopposta, pos;
	int prima_pos_in_alto,h_prel[7], h_dep[7], n_file_corsia_terra, n_alveoli_terra, piani_max, tipo_pallet, num_fila_magazzino;
	int offs,primaposfila,fila_molt,n,n_campate,pos_inizio_campata,pos_attuale ,trasl_prel[7], trasl_dep[7],trasl_prelsx[7], trasl_depsx[7],trasl_preldx[7], trasl_depdx[7],tab_prel[7], tab_dep[7],tab_prelsx[7], tab_depsx[7],tab_preldx[7], tab_depdx[7];
	bool latosx ;
	TADOQuery *ADOQuery;
    try {
        if (!ADOConnection1->Connected) {
            return;
		}
		offs = 500;

		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append("delete from Posizioni ");
        res = ADOQuery->ExecSQL();
		ADOQuery->Close();
		fila = 1;
		// setta i parametri delle 2 corsie (1=A, 2=B)
		for (i = 1; i <= 8; i++) {   // 8 corsie da 18 postazioni per 4 piani
			if (i==1) {
				n_campate = 12 ;
				k = 1 ;
			}else {
				k = 1 ; //inizio dalla prima posizione ogni campata campata
				if (i <= 3)
					n_campate = 7 ;
				else if (i <= 4)
					n_campate = 18 ;//ex 7 , sono 16 ma 2081-2091 non esistono, li salto nel ciclo
				else if (i == 5)
					n_campate = 18 ;
				else if (i <= 7)
					n_campate = 15 ;
				else if (i == 8)
					n_campate = 20 ;
			}
			if (i <= 2)
				fila_molt = 1 ;
			else if (i <= 4)
				fila_molt = 2 ;
			else if (i <= 6)
				fila_molt = 3 ;
			else if (i <= 8)
				fila_molt = 4 ;
			if ((i== 1)||(i==3)||(i==5)||(i==7))
				latosx = true ;
			else
				latosx = false ;
			if (i ==1)
				pos_attuale = 15 ; //partenza inizio conteggio posizione, la corsia 1 parte dalle prime due campate che inserisco sotto
			else
				pos_attuale = 1 ;
			for (k ; k <= n_campate; k++) {   // 20 campate max
				if ((i == 4) &&(k == 8))  // salto pos 2081-2091
					k = 10 ;
				pos_inizio_campata = (k *10)  ;
				for (n = 1; n <= 7; n++) {   // 7 posizioni a campata
					pos = (1000 * fila_molt) + pos_inizio_campata + n ;
					pos_str = IntToStr(pos_attuale);
					pos_str = AnsiString::StringOfChar('0', 3 - pos_str.Length())+pos_str;
					poscliente = "P00"+IntToStr(i)+"00"+pos_str+"00"; //P0010000101 P - Corsia 001 - Costante 00 - Pos 001 - Piano 01
					tipo_pallet = 0 ;
					verticale = 0 ;
					hshape = 14 ;
					wshape = 40 ;
					if ((n == 1)||((n == 2)&&(i == 1) && (k >= 11))) {
						primaposfila = pos ;
						pos_prec = 0;
					}else
						pos_prec = pos-1;
					if ((n== 7)||((n == 6)&&(i == 1) && (k >= 11)))
						next_pos = 0 ;
					else
						next_pos = pos+1;
					zona = "00"+IntToStr(i); //da 1 a 8 ;
					ordinamento = 1 ; // da destra a sinistra
					strsql.printf
						("INSERT INTO Posizioni " "(Pos,CustomerPos,Prenotata,Disabilita,TipoPosizione,"
						"Height,Width,Fila,PrevPosition,NextPosition,PosInFila,Zona"
						",LatoSx,PosInputPlc,PosDisegnoMappa) " "VALUES (%d,'%s',%d,%d,%d"
						",%d,%d,%d,%d,%d,%d,'%s'"
						",%d,0,%d)",
						pos, poscliente.c_str(), 0, 0, tipo_pallet,
						hshape, wshape, fila, pos_prec, next_pos, n, zona.c_str(),
						latosx,primaposfila);
					ADOQuery->SQL->Clear();
					ADOQuery->SQL->Text = strsql;
					res = ADOQuery->ExecSQL();
					ADOQuery->Close();
					pos_attuale++ ;
				}
				fila++;
			}
			//inserisco pos 102-104-1006-112-114-116 e tutte le posizioni singole 2101-2102-2103...
		}
	}
	catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
}

int TdmDB::NumFilePerCorsia(int corsia, AnsiString nomecorsia) {
	int res = 0;

	if ((corsia == 1) || (nomecorsia == "A")) {
		res = 7;
	}
else
		if ((corsia == 2) || (nomecorsia == "B")) {
		res = 8;
	}
	/* else if ((corsia == 3)||(nomecorsia == "C"))
	 res = 8 ; */
else
        if ((corsia == 3) || (nomecorsia == "D")) {
        res = 9;
    }
else
        if ((corsia == 4) || (nomecorsia == "E")) {
        res = 9;
    }
else
        if ((corsia == 5) || (nomecorsia == "F")) {
        res = 15;
    }
else
        if ((corsia == 6) || (nomecorsia == "G")) {
        res = 9;
    }
else
        if ((corsia == 7) || (nomecorsia == "H")) {
        res = 3;
    }
    return res;
}

int TdmDB::NumAlveoliPerCorsia(int corsia, AnsiString nomecorsia) {
    int res = 0;

    if ((corsia == 1) || (nomecorsia == "A")) {
        res = 10;
    }
else
        if ((corsia == 2) || (nomecorsia == "B")) {
        res = 21;
    }
    /* else if ((corsia == 3)||(nomecorsia == "C"))
     res = 10 ; */
else
        if ((corsia == 3) || (nomecorsia == "D")) {
        res = 10;
    }
else
        if ((corsia == 4) || (nomecorsia == "E")) {
        res = 14;
    }
else
        if ((corsia == 5) || (nomecorsia == "F")) {
        res = 14;
    }
else
        if ((corsia == 6) || (nomecorsia == "G")) {
        res = 12;
    }
else
        if ((corsia == 7) || (nomecorsia == "H")) {
        res = 12;
    }
    return res;
}

int TdmDB::PosInizioCorsia(int corsia, int fila, int &posopposta) {
    int res = 0;
    posopposta = 0;
    if (corsia == 1) {
        res = 2460 + (40 * (fila - 1));
        posopposta = res - 20;
    }
else
        if (corsia == 2) {
        res = 2720 + (60 * (fila - 1));
        posopposta = res + 30; // vedere se per scaffale B e C ci saranno le doppie pos ; res + 30 ;
        /* }else if (corsia == 3){
         res = 2750 + (60 * (fila-1))  ;
         posopposta = 0 ;// vedere se per scaffale B e C ci saranno le doppie pos ; res - 30 ; */
    }
else
        if (corsia == 3) {
        res = 3220 + (40 * (fila - 1));
        posopposta = res + 20;
    }
else
        if (corsia == 4) {
        res = 2080 + (40 * (fila - 1));
        posopposta = res + 20;
    }
else
        if (corsia == 5) {
        res = 1480 + (40 * (fila - 1));
        posopposta = res + 20;
    }
else
        if (corsia == 6) {
        res = 1140 + (40 * (fila - 1));
        posopposta = res - 20;
    }
else
        if (corsia == 7) {
        res = 1020 + (40 * (fila - 1));
        posopposta = res - 20;
    }
    return res;
}

int TdmDB::ReturnFilaDaNomeCorsia(AnsiString NomeCorsia) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        // nel caso in cui si volesse azzerare la posizione
        // stringa =stringa.SubString(0, stringa.Length()-3)+"000";
        stringa = "Select * from Corsie where NomeCorsia ='" + NomeCorsia.Trim() + "'";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("fila")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnNextFilaDaPos(int pos, AnsiString zona) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from Corsie where fila >= " + IntToStr(pos) + " and zona= '" + zona + "' order by fila";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            // se trova record in avanti avanza, senno' resta dove sei
            if (!ADOQuery->Eof) {
                ADOQuery->Next();
            }
            res = ADOQuery->FieldByName("fila")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnPrevFilaDaPos(int pos, AnsiString zona) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from Corsie where fila <= " + IntToStr(pos) + " and zona= '" + zona + "' order by fila DESC";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            // se trova record in avanti avanza, senno' resta dove sei
            if (!ADOQuery->Eof) {
                ADOQuery->Next();
            }
            res = ADOQuery->FieldByName("fila")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnFilaDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from Posizioni where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("fila")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnPrimaPosDaFila(int fila) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from Posizioni where Fila = " + IntToStr(fila) + " order by Pos";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("pos")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnZonaDaPos(int pos,int &ordinamento) {
    AnsiString stringa, res = "";
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
		ordinamento = 1 ;
        stringa = "Select * from Posizioni where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
			res = ADOQuery->FieldByName("Zona")->AsString;
			ordinamento = ADOQuery->FieldByName("Ordinamento")->AsInteger;
		}
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

void TdmDB::SvuotaCorsia(int corsia) 
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    
    if (!dmDB->ADOConnection1->Connected) 
        return;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = ADOConnection1;
    try 
    {
        stringa = "UPDATE Corsie SET Piena = 0 , InUsoPrelievo = 0 , InUsoDeposito = 0 ,"; // vedere se azzerare il codice o no CodiceArticolo ='', " ;
        stringa += "Vuota = 1"; // , Alveoli_Massimi = "+IntToStr(alveolitot);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Svuotata Corsia " + IntToStr(corsia));
        stringa = "UPDATE Posizioni SET IdUdc = 0, PresenzaPallet = 0 ";
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
}

void TdmDB::ImpostaCodiceArticoloCorsia(int corsia, AnsiString Codice) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET CodiceArticolo = '" + Codice + "' , InUsoDeposito = 1 ";
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Modificato Codice su corsia  " + IntToStr(corsia) + " , Codice " + Codice);
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}

void TdmDB::SvuotaPosizione(AnsiString nomepos) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res, posocc = -1;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
		stringa = "UPDATE Posizioni SET IDUDC = 0, PresenzaPallet = 0 ";
		stringa += " where CustomerPos = '"+nomepos+"'";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
		dmDB->LogMsg("Svuotata Posizione " + nomepos);

        // SETTAGGIO FLAG IN USO DEPOSITO DA CONTROLLARE ED ATTIVARE
        // guardo se fila e' vuota per settare inusodeposito

		delete ADOQuery;
		// MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}

void TdmDB::PrenotaCorsia(int corsia, int prenota) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET CorsiaPrenotata = " + IntToStr(prenota);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Prenotata Corsia " + IntToStr(corsia) + " , prenota : " + IntToStr(prenota));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}

void TdmDB::PrenotaPos(int pos, int piano, int prenota, int corsiadaprenotare) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res, pren, i, j;
    try {
		if (!ADOConnection1->Connected) {
			return;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;
		stringa = "UPDATE Piani SET Prenotata = " + IntToStr(prenota);
		stringa += " where Pos = " + IntToStr(pos)+" and piano = "+IntToStr(piano);
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = stringa;
		res = ADOQuery->ExecSQL();
		ADOQuery->Close();
		dmDB->LogMsg("Prenotata Pos " + IntToStr(pos) + " ,piano " + IntToStr(piano) + " prenota : " + IntToStr(prenota));
		delete ADOQuery;
		   /*	if (corsiadaprenotare) {
				PrenotaCorsia(corsiadaprenotare, prenota);
			}*/
		// MainForm->LogMsg(stringa);
	}
	catch (...) {
		// ADOConnection1->Close();
	}

}

void TdmDB::PrenotaNomePosizione(AnsiString nomepos,int prenota) {
	AnsiString stringa;
    TADOQuery *ADOQuery;
    int res, pren, i, j;
    try {
		if (!ADOConnection1->Connected) {
			return;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;
		stringa = "UPDATE Posizioni SET Prenotata = " + IntToStr(prenota);
		stringa += " where CustomerPos = '" +nomepos+"'" ;
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = stringa;
		res = ADOQuery->ExecSQL();
		ADOQuery->Close();
		dmDB->LogMsg("Prenotata Pos " + nomepos + " prenota : " + IntToStr(prenota));
		delete ADOQuery;
		   /*	if (corsiadaprenotare) {
				PrenotaCorsia(corsiadaprenotare, prenota);
			}*/
		// MainForm->LogMsg(stringa);
	}
	catch (...) {
		// ADOConnection1->Close();
	}

}

void TdmDB::PrenotaPosIntera(int pos,int prenota) 
{
	AnsiString stringa;
    TADOQuery *ADOQuery;
    int res, pren, i, j;

    if (!ADOConnection1->Connected) 
    {
        return;
	}
    try 
    {
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;
		stringa = "UPDATE Posizioni SET Prenotata = " + IntToStr(prenota);
		stringa += " WHERE Pos = " + IntToStr(pos);
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = stringa;
		res = ADOQuery->ExecSQL();
		ADOQuery->Close();
		dmDB->LogMsg("Prenotate tutte le Pos " + IntToStr(pos) + " prenota : " + IntToStr(prenota));
		delete ADOQuery;
	}
	catch (...) 
    {
	}

}


int TdmDB::CreaIdArticolo() {
	TADOQuery *ADOQuery;
	AnsiString strsql, ev;
	TLocateOptions Opts;
	Opts.Clear();
    int idsrticolo = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT * FROM Udc order by IdArticolo");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        idsrticolo = ADOQuery->FieldByName("IdArticolo")->AsInteger;
        if (ultimo_id_articolo > idsrticolo) {
            idsrticolo = ultimo_id_articolo;
        }
        else {
            do {
                if (idsrticolo > 4000000000) {
                    idsrticolo = 1;
                }
                else {
                    idsrticolo++;
                }
            }
            while (ADOQuery->Locate("IdArticolo", idsrticolo, Opts));
        }
        ADOQuery->Close();
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    ultimo_id_articolo = idsrticolo; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return idsrticolo;
}

void TdmDB::ModificaCodiceArticoloUdc(int IdArticolo, AnsiString Codice) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Udc SET CodiceArticolo = '" + Codice + "'";
		stringa += " where Udc = " + IntToStr(IdArticolo);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Modificato Codice su IdArticolo  " + IntToStr(IdArticolo) + " , Codice " + Codice);
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}

AnsiString TdmDB::ReturnTipoPalletStr(int tipopallet) {
    AnsiString tipostr = "";
    if (tipopallet == 1) {
        tipostr = "80x80";
    }
else
        if (tipopallet == 2) {
        tipostr = "120x80";
    }
else
        if (tipopallet == 3) {
        tipostr = "120x100";
    }
else
        if (tipopallet == 4) {
        tipostr = "120x120";
    }
}

AnsiString TdmDB::ReturnCodiceDaIdArticolo(int IdArticolo, AnsiString & Lotto, int &h_pallet) {
    AnsiString stringa, res = "";
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        Lotto = "";
        h_pallet = MainForm->altezza_max_pallet;
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from Udc where IdArticolo = " + IntToStr(IdArticolo);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("CodiceArticolo")->AsString;
            Lotto = ADOQuery->FieldByName("Lotto")->AsString;
            h_pallet = ADOQuery->FieldByName("AltezzaPallet")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

void TdmDB::SettaCorsiaPiena(int corsia, int setta) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET Piena = " + IntToStr(setta);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato Corsia Piena su Fila " + IntToStr(corsia) + " , piena : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}

void TdmDB::AbilitaCorsia(int corsia, int setta) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET Abilitata = " + IntToStr(setta);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato Abilitata su Fila " + IntToStr(corsia) + " , valore : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}
void TdmDB::SettaCorsiaVuota(int corsia, int setta) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET Vuota = " + IntToStr(setta);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
		dmDB->LogMsg("Settato Vuota su Fila " + IntToStr(corsia) + " , valore : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}

void TdmDB::SettaCorsiaInUso(int corsia, int prelievo, int setta) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (prelievo) {
			stringa = "UPDATE Corsie SET InUsoPrelievo = " + IntToStr(setta);
			if (setta)
				stringa = stringa + " , InUsoDeposito = 0" ;
		} // controllare se la corsia risulta vuota
        else {
			stringa = "UPDATE Corsie SET InUsoDeposito = " + IntToStr(setta) + " , Vuota = 0";
			if (setta)
				stringa = stringa + " , InUsoPrelievo = 0" ;
		}
        stringa = stringa + " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        if (prelievo) {
            dmDB->LogMsg("Settato Corsia in uso per prelievo su Fila " + IntToStr(corsia) + " , valore : " + IntToStr(setta));
        }
        else {
            dmDB->LogMsg("Settato Corsia in uso per deposito su Fila " + IntToStr(corsia) + " , valore : " + IntToStr(setta));
        }
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}

int TdmDB::RicercaDeposito(int tipopallet, TMissione & m, AnsiString corsiaimpostata) {

    AnsiString stringa, stringapos, Vassoio;
    int pos = 0, pianicorsia, fila, filapos, vas, daalveolo, aalveolo, h_pallet_sotto, lascialiberoa, lottopos, corsianuovolotto;
    bool trovata = false, cambiofila;
    QueryCorsie->Close();
    QueryCorsie->SQL->Clear();
/*    m.posdep = 0;
    m.pianodep = 0;
    m.h_dep = 0;
    m.PosDepositoCliente = "";
    m.CorsiaDepositoCliente = "";
    if (corsiaimpostata == "") {
        stringa = "select * from Corsie where Abilitata = 1 AND TipoPallet = " + IntToStr(tipopallet);
        stringa = stringa + " order by InUsoDeposito DESC,Priorita,Fila ";
    }
    else {
        stringa = "select * from Corsie where Abilitata = 1 AND NomeCorsia = '" + corsiaimpostata + "' AND ( TipoPallet = " + IntToStr(tipopallet) + " Or TipoPallet = " + IntToStr(PALLETUNICO) + ")";
        stringa = stringa + " order by InUsoDeposito DESC,Priorita,Fila ";
    }
    QueryCorsie->SQL->Append(stringa);
    QueryCorsie->Open();
    if (QueryCorsie->RecordCount) {
        QueryCorsie->First();
        fila = 0;
        pos = 0;
        cambiofila = false;
        while ((!QueryCorsie->Eof) && (!trovata)) {
            m.CorsiaDepositoCliente = QueryCorsie->FieldByName("NomeCorsia")->AsString;
            pianicorsia = QueryCorsie->FieldByName("PianiMax")->AsInteger;
            QueryPos->Close();
            QueryPos->SQL->Clear();
            filapos = QueryCorsie->FieldByName("FILA")->AsInteger;
            cambiofila = false;
            stringapos = "select * from Posizioni where Fila = " + IntToStr(filapos);
            stringapos = stringapos + " order by Pos ";
            QueryPos->SQL->Append(stringapos);
            QueryPos->Open();
            if (QueryPos->RecordCount) {
                QueryPos->First();
                while ((!QueryPos->Eof) && (!trovata) && (!cambiofila)) {
					if ((!QueryPos->FieldByName("IdUdc1")->AsInteger) &&
						((pianicorsia < 2) || (!QueryPos->FieldByName("IdUdc2")->AsInteger))
						&& ((pianicorsia < 3) || (!QueryPos->FieldByName("IdUdc3")->AsInteger)) &&
                        (!QueryPos->FieldByName("Prenotata")->AsInteger)) {
                        // posizione vuota e disponibile
                        if (!(QueryPos->FieldByName("Disabilita")->AsInteger & MainForm->bit[0])) {
                            pos = QueryPos->FieldByName("Pos")->AsInteger;
                            filapos = QueryPos->FieldByName("Fila")->AsInteger;
                            m.PosDepositoCliente = QueryPos->FieldByName("CustomerPos1")->AsString;
                            m.posdep = QueryPos->FieldByName("Pos")->AsInteger;
                            m.pianodep = 1;
                            if (pianicorsia < 2) {
                                m.h_dep = MainForm->quota_dep_terra;
                            } // QuotaForcheCorsiaPerPiano(filapos,m.pianodep); // piano terra vado ad altezza QueryPos->FieldByName("AltezzaPrelievo1")->AsInteger ;
                            else {
                                m.h_dep = QueryCorsie->FieldByName("HDep1")->AsInteger;
                            } // QuotaForcheCorsiaPerPiano(filapos,m.pianodep); // piano terra vado ad altezza QueryPos->FieldByName("AltezzaPrelievo1")->AsInteger ;
                            m.h_corsia = m.h_dep; // nelle prime 3 pos passo la quota di deposito = alla quota in corsia
                            m.corsia_dep = QueryPos->FieldByName("Fila")->AsInteger;
                        }
                        else
                            if ((pianicorsia > 1) && (!(QueryPos->FieldByName("Disabilita")->AsInteger & MainForm->bit[1]))) {
                            pos = QueryPos->FieldByName("Pos")->AsInteger;
                            filapos = QueryPos->FieldByName("Fila")->AsInteger;
                            m.PosDepositoCliente = QueryPos->FieldByName("CustomerPos3")->AsString;
                            m.posdep = QueryPos->FieldByName("Pos")->AsInteger;
                            m.pianodep = 2;
                            m.h_dep = QueryCorsie->FieldByName("HDep2")->AsInteger;
                            // QuotaForcheCorsiaPerPiano(filapos,m.pianodep); // piano terra vado ad altezza QueryPos->FieldByName("AltezzaPrelievo1")->AsInteger ;
                            m.h_corsia = m.h_dep; // nelle prime 3 pos passo la quota di deposito = alla quota in corsia
                            m.corsia_dep = QueryPos->FieldByName("Fila")->AsInteger;
                        }
                        else
                            if ((pianicorsia > 2) && (!(QueryPos->FieldByName("Disabilita")->AsInteger & MainForm->bit[2]))) {
                            pos = QueryPos->FieldByName("Pos")->AsInteger;
                            filapos = QueryPos->FieldByName("Fila")->AsInteger;
                            m.PosDepositoCliente = QueryPos->FieldByName("CustomerPos3")->AsString;
                            m.posdep = QueryPos->FieldByName("Pos")->AsInteger;
                            m.pianodep = 3;
                            m.h_dep = QueryCorsie->FieldByName("HDep3")->AsInteger;
                            // QuotaForcheCorsiaPerPiano(filapos,m.pianodep); // piano terra vado ad altezza QueryPos->FieldByName("AltezzaPrelievo1")->AsInteger ;
                            m.h_corsia = m.h_dep; // nelle prime 3 pos passo la quota di deposito = alla quota in corsia
                            m.corsia_dep = QueryPos->FieldByName("Fila")->AsInteger;
                        }
					} else if ((pianicorsia >= 2) && (!QueryPos->FieldByName("Prenotata")->AsInteger) && (!QueryPos->FieldByName("IdUdc2")->AsInteger)) {
                        if (!(QueryPos->FieldByName("Disabilita")->AsInteger & MainForm->bit[1])) {
                            pos = QueryPos->FieldByName("Pos")->AsInteger;
                            filapos = QueryPos->FieldByName("Fila")->AsInteger;
                            m.PosDepositoCliente = QueryPos->FieldByName("CustomerPos3")->AsString;
                            m.posdep = QueryPos->FieldByName("Pos")->AsInteger;
                            m.pianodep = 2;
                            m.h_dep = QueryCorsie->FieldByName("HDep2")->AsInteger;
                            // QuotaForcheCorsiaPerPiano(filapos,m.pianodep); // piano terra vado ad altezza QueryPos->FieldByName("AltezzaPrelievo1")->AsInteger ;
                            m.h_corsia = m.h_dep; // nelle prime 3 pos passo la quota di deposito = alla quota in corsia
							m.corsia_dep = QueryPos->FieldByName("Fila")->AsInteger;
							trovata = true ;
                        }
					} else if ((pianicorsia >= 3) && (!QueryPos->FieldByName("Prenotata")->AsInteger) && (!QueryPos->FieldByName("IdUdc3")->AsInteger)) {
                        if (!(QueryPos->FieldByName("Disabilita")->AsInteger & MainForm->bit[2])) {
                            pos = QueryPos->FieldByName("Pos")->AsInteger;
                            filapos = QueryPos->FieldByName("Fila")->AsInteger;
                            m.PosDepositoCliente = QueryPos->FieldByName("CustomerPos3")->AsString;
                            m.posdep = QueryPos->FieldByName("Pos")->AsInteger;
                            m.pianodep = 3;
                            m.h_dep = QueryCorsie->FieldByName("HDep3")->AsInteger;
                            // QuotaForcheCorsiaPerPiano(filapos,m.pianodep); // piano terra vado ad altezza QueryPos->FieldByName("AltezzaPrelievo1")->AsInteger ;
                            m.h_corsia = m.h_dep; // nelle prime 3 pos passo la quota di deposito = alla quota in corsia
							m.corsia_dep = QueryPos->FieldByName("Fila")->AsInteger;
							trovata = true ;
                        }
                    } else if (QueryPos->FieldByName("Prenotata")->AsInteger) {
                        pos = -1;
                        trovata = true;
                        m.posdep = -1;
                        m.pianodep = 0;
                        m.h_dep = 0;
                        m.corsia_dep = 0;
                        // fila prenotata, aspetto
                    } else if (pos) {
                        trovata = true;
                    }
                    else
                        if (!pos) {
                        cambiofila = true;
                    }

                    QueryPos->Next();
                }
                if ((QueryPos->Eof) && (!trovata) && (pos)) { // corsia vuota
                    trovata = true;
                }
                if ((QueryPos->Eof) && (!trovata) && (!pos)) { // corsia piena
                    SettaCorsiaPiena(filapos, 1);
                }

            }
            QueryCorsie->Next(); // se cambio la fila ricontrollo la prima posizione se è libera
        }
    }
    QueryPos->Close();
	QueryCorsie->Close();   */
    return pos;
}

void TdmDB::FullTabellaK(AnsiString TableName, AnsiString KeyField, TIndexList &RecList) 
{
    AnsiString strsql, campo, chiave, valore;
    TADOQuery *ADOQuery;
    Variant k;
    int j, i = 0;

    if (!dmDB->ADOConnection1->Connected)
        return;
    RecList.clear();
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("%s", TableName.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        while (!ADOQuery->Eof) {
            for (j = 0; j < ADOQuery->FieldCount; j++) {
                chiave = ADOQuery->FieldByName(KeyField)->AsString;
                campo = ADOQuery->FieldList->Fields[j]->FieldName.UpperCase();
                valore = ADOQuery->FieldList->Fields[j]->AsString;
                RecList[chiave][campo] = valore;
            }
            ADOQuery->Next();
            i++;
        }
        ADOQuery->Close();
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
    }
    delete ADOQuery;
}

void  TdmDB::UpdateCorsielist()
{
    AnsiString stringa;
    
    stringa = "Select * from Corsie";
    FullTabellaK(stringa, "Fila", m_TabCorsie);
}


int TdmDB::GetAbilitataIfCorsia(int corsia) 
{
    AnsiString stringa;
    int  ret = 1;
    //stringa = "select * from Corsie where Fila = " + IntToStr(corsia);

    //QueryCorsie->SQL->Append(stringa);
    //QueryCorsie->Open();
    
    //if (QueryCorsie->RecordCount) 
    //{
    if (corsia != 0) 
        ret =  m_TabCorsie[corsia]["ABILITATA"].ToIntDef(0); //QueryCorsie->FieldByName("Abilitata")->AsInteger;   
        
    //}
    //QueryCorsie->Close();
    return ret;
}


int TdmDB::GetTipologiaCorsia(int fila) 
{
    return m_TabCorsie[fila]["TIPOLOGIA"].ToIntDef(0);
}



AnsiString TdmDB::GetTipologiaCorsiaString(int corsia) 
{
    AnsiString stringa = "";
    if (corsia != 0) 
    {
        switch (m_TabCorsie[corsia]["TIPOLOGIA"].ToIntDef(0)) 
        {
            
        case TYPE_UNDEFINED:
            stringa = "INDEFINITA";
            break;
        case TYPE_FASCIARE:
            stringa = "Prodotti da FASCIARE";
            break;
        case TYPE_NONFASCIARE:
            stringa = "Prodotti da NON FASCIARE";
            break;
        case TYPE_INCOMPLETI:
            stringa = "Prodotti INCOMPLETI";
            break;
        default:
            stringa = "";
        }
    }
    return stringa;
}


int TdmDB::RicercaDepositoTipologiaUDC(int tipologia, int &opposta) 
{
    AnsiString stringa, stringapos, Vassoio;
    int pos = 0, pianicorsia, ordinamento_dec = 0, fila, filapos, prenotazioni;
    bool trovata = false, cambiofila;
    
    QueryCorsie->Close();
    QueryCorsie->SQL->Clear();
    /* TAB tipologia Udc
     TipoCorsia	                        
     1	Semilavorati                	    
     2	Appoggio                    	    
     3	Prodotti esterni da Fasciare    	
     4	Rientro da navetta              	
     5	Etichette non riuscite          	
     6	Prodotti esterni da NON Fasciare   	
     */

    stringa = "SELECT * FROM Corsie WHERE Fila > 0 AND Abilitata = 1 AND Piena = 0 AND Tipologia = " + IntToStr(tipologia);
    stringa = stringa + " ORDER BY InUsoDeposito DESC, InUsoPrelievo, Priorita, Fila ";

    QueryCorsie->SQL->Append(stringa);
    QueryCorsie->Open();
    
    if (QueryCorsie->RecordCount) 
    {
        QueryCorsie->First();
        fila = 0;
        pos = 0;
        opposta = 0;
        
        while ((!QueryCorsie->Eof) && (!trovata)) 
        {
            QueryPos->Close();
            QueryPos->SQL->Clear();
            cambiofila = false;
            filapos = QueryCorsie->FieldByName("FILA")->AsInteger;
            ordinamento_dec = QueryCorsie->FieldByName("OrdinamentoCorsia")->AsInteger;
            stringapos = "select * from Posizioni where Fila = " + IntToStr(filapos);
            if (ordinamento_dec)
                stringapos = stringapos + " order by Pos DESC ";
            else
                stringapos = stringapos + " order by Pos ";
            QueryPos->SQL->Append(stringapos);
            QueryPos->Open();
            if (QueryPos->RecordCount) {
                QueryPos->First();
                while ((!QueryPos->Eof) && (!trovata) && (!cambiofila)) {
                    if ((!QueryPos->FieldByName("IdUDC")->AsInteger) && (!QueryPos->FieldByName("Prenotata")->AsInteger) && 
                        (!QueryPos->FieldByName("PresenzaPallet")->AsInteger)) {
                        // posizione vuota e disponibile
                        if (!QueryPos->FieldByName("Disabilita")->AsInteger) {
                            pos = QueryPos->FieldByName("Pos")->AsInteger;
                            opposta = QueryPos->FieldByName("PosOpposta")->AsInteger;
                            filapos = QueryPos->FieldByName("Fila")->AsInteger;
                        }
                    }
                    else {
                        if (QueryPos->FieldByName("Prenotata")->AsInteger) {
                            pos = 0;
                            opposta = 0;
                            cambiofila = true;
                       }
                        else if (pos) {
                            trovata = true;
                        }
                        else {
                            // prima posizione impegnata
                            pos = 0;
                            opposta = 0;
                            cambiofila = true;
                        }
                    }
                    QueryPos->Next();
                }
                if ((QueryPos->Eof) && (!trovata) && (pos)) // corsia vuota
                        trovata = true;
                if ((QueryPos->Eof) && (!trovata) && (!pos)) // corsia piena
                        SettaCorsiaPiena(filapos, 1);

            }
            QueryCorsie->Next(); // se cambio la fila ricontrollo la prima posizione se è libera
			if ((!pos) && (QueryCorsie->Eof))
				return -1;
        }
    }
    QueryPos->Close();
    QueryCorsie->Close();
    return pos;
}

//--------------------------------------------------------------------------
// Vengono specificate fino a due tipologie di corsia da ricercare
// (serve per le corsie da ettichettare ASSIEME a quelle da non etichettare)
int TdmDB::RicercaPrelievoUdcinCorsia(int tipologia, int tipologia2, int &priorita, int &dafasciare) 
{
    AnsiString stringa, stringapos, Vassoio;
    int pos = 0, pianicorsia, ordinamento_dec = 0, fila, filapos, prenotazioni;
    bool trovata = false, cambiofila;
    QueryCorsie->Close();
    QueryCorsie->SQL->Clear();
    /* TAB tipologia Udc
     TipoCorsia	                        
     1	Semilavorati                	    
     2	Appoggio                    	    
     3	Prodotti esterni da Fasciare    	
     4	Rientro da navetta              	
     5	Etichette non riuscite          	
     6	Prodotti esterni da NON Fasciare   	
     */

    dafasciare = 0;
    stringa = "SELECT * FROM Corsie WHERE Fila > 0 AND Abilitata = 1 AND (Tipologia = " + IntToStr(tipologia);
    if (tipologia2 >= 0) 
        stringa = stringa + " OR Tipologia = " + IntToStr(tipologia2);
    stringa = stringa + ") ORDER BY InUsoPrelievo DESC, InUsoDeposito, Priorita, Fila ";

    QueryCorsie->SQL->Append(stringa);
    QueryCorsie->Open();
    
    if (QueryCorsie->RecordCount) 
    {
        QueryCorsie->First();
        fila = 0;
        pos = 0;
        
        while ((!QueryCorsie->Eof) && (!trovata)) 
        {
            prenotazioni = 0;
            QueryPos->Close();
            QueryPos->SQL->Clear();
            cambiofila = false;
            filapos = QueryCorsie->FieldByName("FILA")->AsInteger;
            ordinamento_dec = QueryCorsie->FieldByName("OrdinamentoCorsia")->AsInteger;
            stringapos = " select * from Posizioni where Fila = " + IntToStr(filapos);
            if (ordinamento_dec)
                stringapos = stringapos + " order by Pos DESC ";
            else
                stringapos = stringapos + " order by Pos ";
                
            QueryPos->SQL->Append(stringapos);
            QueryPos->Open();
            if (QueryPos->RecordCount) 
            {
                QueryPos->First();
				while ((!QueryPos->Eof) && (!trovata) && (!cambiofila)) 
                {
                    if (((QueryPos->FieldByName("IdUDC")->AsInteger) || (QueryPos->FieldByName("PresenzaPallet")->AsInteger)) && 
                        (!QueryPos->FieldByName("Prenotata")->AsInteger)) 
                    {
						// posizione piena e disponibile
						if (!QueryPos->FieldByName("Disabilita")->AsInteger) {
							pos = QueryPos->FieldByName("Pos")->AsInteger;
							filapos = QueryPos->FieldByName("Fila")->AsInteger;
                            priorita = QueryCorsie->FieldByName("Priorita")->AsInteger; 
                            dafasciare = (QueryCorsie->FieldByName("Tipologia")->AsInteger == TYPE_FASCIARE) ? 1 : 0;
                            trovata = true;
                        }
                    }
                    else {
                        if (QueryPos->FieldByName("Prenotata")->AsInteger) 
                        {
                            pos = 0;
                            cambiofila = true;
                        }
                        else {
                            // prima posizione vuota
                            pos = 0;
                        }
                    }
                    QueryPos->Next();
                }
            }
            QueryCorsie->Next(); // se cambio la fila ricontrollo la prima posizione se è libera
        }
    }
    QueryPos->Close();
    QueryCorsie->Close();
    return pos;
}



void TdmDB::ArticoloPrelevatoDepositato(int pos, AnsiString idArticolo , int presenza) 
{

    AnsiString stringa;
    AnsiString Locazione;
    TADOQuery *ADOQuery;
    int res;
    
    if (!ADOConnection1->Connected) {
        return;
    }
    try 
    {
        ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;
		//stringa = "Update Piani set IdUdc = '" + idArticolo + "' , LarghezzaUdc = "+IntToStr(tipoudc)+", LarghezzaVassoio = "+IntToStr(tipovassoio)+" where NomePos = '" + nomepos+"'";
		stringa = "UPDATE Posizioni SET IdUdc = '" + idArticolo + "',PresenzaPallet = "+IntToStr(presenza)+"  WHERE Pos = " + IntToStr(pos);
		ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa + " res query : " + IntToStr(res));
        delete ADOQuery;
 /*

		if ((idArticolo > 0) && (pos > 400)) {
			Locazione = CustomerPosDaPosizioneEPiano(pos, piano);
			UpdateDestinazioneUdc(idArticolo, Locazione);
		}
 */
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
}

int TdmDB::UdcPresenteInArchivio(int udc) {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;
	try {
		if (!ADOConnection1->Connected) {
			return 0;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from Udc where IdArticolo = " + IntToStr(udc);
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
        res = ADOQuery->RecordCount;
		ADOQuery->Close();
        // MainForm->LogMsg(stringa);
	}
    catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
    return res;
}
int TdmDB::ContaPalletInCorsia(int fila) {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;
	try {
		if (!ADOConnection1->Connected) {
			return 0;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "SELECT * from posizioni where fila  = " + IntToStr(fila)+" and presenzapallet > 0";
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
        res = ADOQuery->RecordCount;
		ADOQuery->Close();
        // MainForm->LogMsg(stringa);
	}
    catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
    return res;
}

int TdmDB::CodicePresenteInArchivio(AnsiString CodiceArticolo) {
	AnsiString stringa;
	AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from Udc where CodiceArticolo = '" + CodiceArticolo + "'";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        res = ADOQuery->RecordCount;
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

void TdmDB::CreaModificaUdc(int udc, AnsiString Codice, AnsiString Lotto, int h_pal) {
    int res = 0, idmiss, tabcarico = 0, tabscarico = 0, Priorita = 1;
    TADOQuery *ADOQuery;
    AnsiString strsql, nomebaiaprel;
    Evento ev;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        // if ((m.Agv)&&((MainForm->DatiAgv[m.Agv].incarica)||(DMGestioneEventi->AgvInPosCb(m.Agv))))
        // dmComandiAgv->NoCharge(m.Agv);
        if (!UdcPresenteInArchivio(udc)) {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            strsql.printf("INSERT INTO UDC ( IDARTICOLO,CODICEARTICOLO ,LOTTO, DATAORA ,ALTEZZAPALLET)" " VALUES (%d,'%s','%s',%s,%d)", udc, Codice.c_str(), Lotto.c_str(), "GetDate()", h_pal);
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();
            ultimo_id_articolo = 0;
        }
        else {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            strsql.printf("Update UDC set CODICEARTICOLO = '%s' ,LOTTO = '%s' ,ALTEZZAPALLET = %d" " where IdArticolo = %d", Codice.c_str(), Lotto.c_str(), h_pal, udc);
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();
        }
        LogMsg(strsql + " , res : " + IntToStr(res));
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return;
}

void TdmDB::AbortMission(int idmiss) {
    TADOQuery *ADOQuery;
	int prel, posdep,pianoprel = 0, pianodep = 0, tipomis, corsiadep, nagv,progressivo_wms = 0, linea_prel;
	AnsiString NoPrel = "", stringa,Source,Dest;
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from Missioni where id = " + IntToStr(idmiss);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            // posprel = ADOQuery->FieldByName("posprel")->AsInteger ;
            prel = ADOQuery->FieldByName("posprel")->AsInteger;
            NoPrel = ADOQuery->FieldByName("prelevato")->AsString;
            posdep = ADOQuery->FieldByName("posdep")->AsInteger;
			tipomis = ADOQuery->FieldByName("tipomissione")->AsInteger;
            nagv = ADOQuery->FieldByName("Agv")->AsInteger;
			progressivo_wms = ADOQuery->FieldByName("ProgressivoMissione")->AsInteger;
			Source = ADOQuery->FieldByName("PosPrelCliente")->AsString;
			Dest = ADOQuery->FieldByName("PosDepCliente")->AsString;
        }
        dmDB->LogMsg("Missione n° " + IntToStr(idmiss) + " abortita da lista agvx");
        ADOQuery->Close();
        // guardare se ho un'anomalia sull'abort
		// guardo se devo uscire dall'ingombro
		dmDB->AggiornaMissione(idmiss, 4, nagv);
        
		if (progressivo_wms)
        {
            dmDB->UpdateStatoCaricamento(progressivo_wms, STATO_ABORTITA);
            dmDB->UpdatePosCarico(Source, "", 0);   //mette UDC = 0  e ID caricamento = 0
            //linea_prel = DMGestioneEventi->RitornaLineaPrelievoDaPos(prel);
            dmDB->LogMsg("Aggiorno stato missione ad ABORT su linea WMS "+IntToStr(progressivo_wms));
        }
            
		if ((NoPrel == "")&&(tipomis  < 2)) 
        {
            dmDB->PrenotaPosIntera(prel,0);
			//dmDB->PrenotaNomePosizione(Source,0);
		   //	DataModuleSap->InviaStatoMissioneGenericoASap(HuIdent,Source,"",CP,HuTyp,MainForm->ErrorMisAbortitaAgvScaricoNoPrelievo) ;//al prel metto dest vuoto
		}

        dmDB->PrenotaPosIntera(posdep,0);
		//dmDB->PrenotaNomePosizione(Dest,0);

        CaricaTabelle();
        AggiornaMemoriePlc();
        MainForm->aggiornamappa = 1;
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return;
}

void __fastcall TdmDB::ADOConnection1ConnectComplete(TADOConnection * Connection, Error* const Error, TEventStatus & EventStatus) {
    // imposto dataora nel formato giorno/mese/anno perchè ogni versione sql in base al paese è diversa e nei filtri dopo fa casino
    try {
        QueryPos->Close();
        QueryPos->SQL->Clear();
        QueryPos->SQL->Append("set DATEFORMAT dmy"); // setto formato database giorno/mese/anno
        QueryPos->ExecSQL();
    }
    catch (...) {
        // ADOConnection1->Close();
    }

}

// ---------------------------------------------------------------------------
void __fastcall TdmDB::DataModuleDestroy(TObject * Sender) {
    DisconnettiDB();

}

// ---------------------------------------------------------------------------
int TdmDB::AggiornaCampoUdc(int udc, int valore) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    AnsiString Val;
/*
	if (!ADOConnectionSMInformatica->Connected) {
        return 0;
    }
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnectionSMInformatica;
        strsql.printf("UPDATE PMFUdc SET IDCampo = %d WHERE IdUdc = %d", valore, udc);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
    }
    catch (...) {
        // ADOConnectionSMInformatica->Close();
        ScriviSQLCmd(strsql, "AggiornaCampoUdc");
    }
    delete ADOQuery;
	return res;
	*/
}



void TdmDB::CaricaTabelle() 
{
	CaricaTabellaK("Posizioni order by pos", "Pos", TabPostazioni);
	CaricaTabellaK("Posizioni order by CustomerPos", "CustomerPos", TabCustomerPos);
}
int TdmDB::IdTipoVassoio(int val) {
	int i,valda,vala,res = 0 ;
	for (i = 0; i < TabTipoVassoi.size(); i++) {
		valda = TabTipoVassoi[i]["VALOREDA"].ToIntDef(0);
		vala = TabTipoVassoi[i]["VALOREA"].ToIntDef(0);
		if ((val >= valda)&&(val <= vala)) {
			res = TabTipoVassoi[i]["RISULTATO"].ToIntDef(0);
			break ;
		}
	}

	return res ;
}
int TdmDB::IdTipoLarghezzaUdc(int val) {
	int i,valda,vala,res = 0 ;
	for (i = 0; i < TabTipoLarghezzaUdc.size(); i++) {
		valda = TabTipoLarghezzaUdc[i]["VALOREDA"].ToIntDef(0);
		vala = TabTipoLarghezzaUdc[i]["VALOREA"].ToIntDef(0);
		if ((val >= valda)&&(val <= vala)) {
			res = TabTipoLarghezzaUdc[i]["RISULTATO"].ToIntDef(0);
			break ;
		}
	}

	return res ;
}

int TdmDB::FilaPosizione(int pos) {
	return TabPostazioni[IntToStr(pos)]["FILA"].ToIntDef(0);
}

AnsiString TdmDB::NomeCorsiaDaPosizione(int pos) {
    return TabPostazioni[IntToStr(pos)]["NOMECORSIA"];
}

AnsiString TdmDB::CustomerPosDaPosizione(int pos) {
    return TabPostazioni[IntToStr(pos)]["CUSTOMERPOS"];
}

int TdmDB::PosizioneDaCustomerPos(AnsiString customerpos) 
{
    IndexListIterator i;
    int res = 0; // ricerco se la posizone passta è una posizione 1000x1200 rispetto quella utilizzata

    for (i = TabPostazioni.begin(); i != TabPostazioni.end(); i++) 
    {
        if (i->second["CUSTOMERPOS"] == customerpos) 
        {
            res = i->second["POS"].ToIntDef(0);
            break;
        }
    }

    return res;
}

AnsiString TdmDB::AlmenoUnUdcInPos(int pos) 
{
    IndexListIterator i;
    AnsiString res = ""; // ricerco se c'è almeno un UDC su uno dei 2 piani

    for (i = TabCustomerPos.begin(); i != TabCustomerPos.end(); i++) 
    {
        if (i->second["POS"].ToIntDef(0) == pos) 
        {
            if (i->second["IDUDC"] != "" && i->second["IDUDC"] != NULL) 
            {
                res = i->second["IDUDC"];                
            }
        }
    }

    return res;
}

AnsiString TdmDB::CustomerPosDaPosizioneEPiano(int pos, int piano) {
    return TabPostazioni[IntToStr(pos)]["CUSTOMERPOS" + IntToStr(piano)];
}

int TdmDB::RitornaTipoPos(int pos) {
    return TabPostazioni[IntToStr(pos)]["TIPOPOSIZIONE"].ToIntDef(0);
}

int TdmDB::RitornaPresenza(int pos) {
    return TabPostazioni[IntToStr(pos)]["PRESENZAPALLET"].ToIntDef(0);
}

int TdmDB::RitornaPos1000x1200(int pos) {
    return TabPostazioni[IntToStr(pos)]["POS1000X1200"].ToIntDef(0);
}

int TdmDB::CorsiaPrimaPosInAlto(int fila) {
    return TabCorsie[IntToStr(fila)]["PRIMAPOSINALTO"].ToIntDef(0);
}

int TdmDB::QuotaForcheCorsiaPerPiano(int fila, int piano) {
    return TabCorsie[IntToStr(fila)]["HDEP" + IntToStr(piano)].ToIntDef(0);
}

void TdmDB::AggiornaTabParametri() {
    CaricaTabellaK("Parametri order by IdParametro", "IdParametro", TabParametri);
}

// ---------------------------------------------------------------------------


int TdmDB::TrovaUdcInPosizione(AnsiString CodiceUdc, int &corsia, int &piano) {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;
	try {
		corsia = 0;
		if (!ADOConnection1->Connected) {
			return 0;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from Posizioni where IdUdc LIKE '%" + CodiceUdc + "%'";
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount) {
			res = ADOQuery->FieldByName("Pos")->AsInteger;
            corsia = ADOQuery->FieldByName("Fila")->AsInteger;
			piano = ADOQuery->FieldByName("Piano")->AsInteger;
		}
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ScriviSQLCmd(AnsiString SQLCmd, AnsiString FunzChiamante) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    AnsiString Val;
    // return 0;
    if (!ADOConnection1->Connected) {
        return 0;
    }
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        SQLCmd = StringReplace(SQLCmd, "'", "°", TReplaceFlags() << rfReplaceAll);
        strsql.printf("INSERT INTO SQLCmd (SQLString, FunzioneChiamante, Generata, DataOra) VALUES ('%s','%s', %d, current_timestamp)", SQLCmd, FunzChiamante, 0, DateTimeToStr(Now()));
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnIdPortaDaStringaConsensoPorta(AnsiString str_consenso) {
	IndexListIterator i;
    int res = 0;
    for (i = TabPorte.begin(); i != TabPorte.end(); i++) {
        if (i->second["STRCONSENSO"] == str_consenso) {
            res = i->second["IDPORTA"].ToIntDef(0);
            break;
        }
    }
    return res;
}

int TdmDB::StatoPorta(int porta) {
    return TabPorte[IntToStr(porta)]["STATO"].ToIntDef(0);
}

AnsiString TdmDB::PosizioniRichiestaAperturaPorta(int porta) {
	return TabPorte[IntToStr(porta)]["POSAPRIPORTA"];
}

AnsiString TdmDB::PosizioniPortaAperta(int porta) {
	return TabPorte[IntToStr(porta)]["POSPORTAAPERTA"];
}
AnsiString TdmDB::StrConsensoPorta(int porta) {
    return TabPorte[IntToStr(porta)]["STRCONSENSO"];
}
void TdmDB::AggiornaTabPorte() {
    CaricaTabellaK("Porte order by IdPorta", "IdPorta", TabPorte);
	AggiornaStatoPorteStruttura();
}
void TdmDB::AggiornaStatoPorteStruttura() {
	IndexListIterator i;
	int res = 0;
  /*	for (i = TabPorte.begin(); i != TabPorte.end(); i++) {
		DMGestione_Porte->DatiPorte[i->second["IDPORTA"].ToIntDef(1)].Stato = i->second["STATO"].ToIntDef(0);
	}  */

}

AnsiString TdmDB::DaBufferAStringa(unsigned char *data, int lunghezza_str)
{
	// ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
	char car;
	int j;
	AnsiString stringa = "",str_res="";
	for(j=0; j < lunghezza_str; j++) {
		stringa = "" ;
		stringa.printf("%c",data[j]);
		if ((stringa != "")&&(data[j] != 0))
			str_res = str_res + stringa ;
	}
	return str_res;
}
int TdmDB::DaBufferAByte(unsigned char *data)
{
	// ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
	char car;
	int res;
	res = data[0] ;
	return res;
}
int TdmDB::DaBufferAWord(unsigned char *data)
{
	// ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
	char car;
	int res;
	res = (data[0] * 256 )+ data[1] ;
	return res;
}
int TdmDB::DaBufferADoppiaWord(unsigned char *data)
{
	// ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
	char car;
	int res;
	res = (data[0] * 256 * 256 * 256  )+(data[1] * 256 * 256  )+(data[2] * 256  )+ data[3] ;
	return res;
}

int TdmDB::RitornaPosInputPlc(int pos) {
    return TabPostazioni[IntToStr(pos)]["POSINPUTPLC"].ToIntDef(0);
}
int TdmDB::ReturnPosDaPosCliente(AnsiString Poscliente) {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;
	try {
		if (!ADOConnection1->Connected) {
			return 0;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from Posizioni where CustomerPos ='"+Poscliente+"'";
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount > 0) {
			res = ADOQuery->FieldByName("pos")->AsInteger;
			//piano = ADOQuery->FieldByName("piano")->AsInteger;
		}
		ADOQuery->Close();
		// MainForm->LogMsg(stringa);
	}
	catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
	return res;
}
int TdmDB::ReturnFilaDaPosCliente(AnsiString Poscliente) {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;
	try {
		if (!ADOConnection1->Connected) {
			return 0;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from Piani where CustomerPos ='"+Poscliente+"'";
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount > 0) {
			res = ADOQuery->FieldByName("Fila")->AsInteger;
		}
		ADOQuery->Close();
		// MainForm->LogMsg(stringa);
	}
	catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
	return res;
}
AnsiString TdmDB::ReturnNomePosDaPosEFormato(int pos,int formato_pallet) {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	AnsiString res = "0";
	try {
		if (!ADOConnection1->Connected) {
			return res;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from Posizioni where Pos ="+IntToStr(pos)+" and FormatoPallet = "+IntToStr(formato_pallet);
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount > 0) {
			res = ADOQuery->FieldByName("CustomerPos")->AsString;
		}
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
	}
    catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
	return res;
}

int TdmDB::RitornaAltezzaForchePosizione(int pos , int &hdep) {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;
	hdep = 0 ;
	try {
		if (!ADOConnection1->Connected) {
			return 0;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from Posizioni where pos = "+IntToStr(pos);
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount > 0) {
			res = ADOQuery->FieldByName("hprel")->AsInteger;
			hdep = ADOQuery->FieldByName("hdep")->AsInteger;
		}
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
	}
    catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
	return res;
}

int TdmDB::CompilaDatiPosizione(AnsiString nomepos,int prelievo,int formato_pallet,int &h_forche,int &tab_car_scar) {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;
	tab_car_scar = 0 ;
	h_forche = 0 ;
	try {
		if (!ADOConnection1->Connected) {
			return 0;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from Posizioni where CustomerPos = '"+nomepos+"' and FormatoPallet = "+IntToStr(formato_pallet);
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount > 0) {
			res = 1 ;
			if (prelievo) {
				h_forche = ADOQuery->FieldByName("hprel")->AsInteger;
				tab_car_scar = ADOQuery->FieldByName("TabCarico")->AsInteger;
				//traslazione = ADOQuery->FieldByName("TraslPrel")->AsInteger;
			}else {
				h_forche = ADOQuery->FieldByName("HDep")->AsInteger;
				tab_car_scar = ADOQuery->FieldByName("TabScarico")->AsInteger;
				//traslazione = ADOQuery->FieldByName("TraslDrop")->AsInteger;
			}

		}
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
	}
    catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
	return res;
}

void TdmDB::AggiornaMemoriePlc() 
{
 	MainForm->PlcFascia.generata = TabPostazioni[MainForm->PlcFascia.posdep]["PRENOTATA"].ToIntDef(0);
	MainForm->PlcFascia.escludi = TabPostazioni[MainForm->PlcFascia.posdep]["DISABILITA"].ToIntDef(0);

 	//MainForm->PlcMonta.generata = TabPostazioni[MainForm->PlcMonta.posprel]["PRENOTATA"].ToIntDef(0);
	//MainForm->PlcMonta.escludi = TabPostazioni[MainForm->PlcMonta.posprel]["DISABILITA"].ToIntDef(0);

 	MainForm->PlcRobot.vuoti_generati = TabPostazioni[MainForm->PlcRobot.posvuoti]["PRENOTATA"].ToIntDef(0);
	MainForm->PlcRobot.vuoti_esclusi = TabPostazioni[MainForm->PlcRobot.posvuoti]["DISABILITA"].ToIntDef(0);
}


void TdmDB::RicercaNuovaMissioneDaGenerare() {
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	TMissione Mis ;
	int res = 0;
/*	try {
		if (!ADOConnection1->Connected) {
			return ;
		}
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

		stringa = "Select * from MissioniWms where StatoMissione = 0 order by Progressivo";
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount > 0) {
			ADOQuery->First();
			while (!ADOQuery->Eof) {
				Mis.Source = ADOQuery->FieldByName("PosizionePrelievo")->AsString ;
				Mis.Dest = ADOQuery->FieldByName("PosizioneDeposito")->AsString ;
				Mis.IdUdc = ADOQuery->FieldByName("IdUdc")->AsInteger ;
				Mis.Agv = 1 ;
				Mis.tipo_pallet = ADOQuery->FieldByName("TipoPallet")->AsInteger ;
				Mis.formato_pallet = ADOQuery->FieldByName("FormatoPallet")->AsInteger ;
				Mis.progressivo_missione = ADOQuery->FieldByName("Progressivo")->AsInteger ;
				Mis.priorita = ADOQuery->FieldByName("Priorita")->AsInteger ;
				Mis.tipo_mis = ADOQuery->FieldByName("TipoMissione")->AsInteger ;
				Mis.posprel = ReturnPosDaPosCliente(Mis.Source) ;
				Mis.posdep = ReturnPosDaPosCliente(Mis.Dest) ;
				if (Mis.tipo_mis == 2) {
					Mis.posprel = MainForm->DatiAgv[1].pos ;
				}
				if ((Mis.posprel > 0 )&&(Mis.posdep > 0)) {
					if (dmDB->GeneraMissione(Mis))
						dmDB->AggiornaMissioneWms(Mis.progressivo_missione,1,0) ;
					else {
						dmDB->LogMsg("Missione non creata da Missione WMS , Progressivo : "+IntToStr(Mis.progressivo_missione));
						dmDB->AggiornaMissioneWms(Mis.progressivo_missione,8,0) ;
					}
				}

				ADOQuery->Next();
			}

		}
        ADOQuery->Close();
		// MainForm->LogMsg(stringa);
	}
    catch (...) {
		// ADOConnection1->Close();
	}
	delete ADOQuery;
    */
}

void TdmDB::MissioneElaborataSuAgvX(int idmiss,int causale)
{
AnsiString str_missione = "";
AnsiString strsql;
TADOQuery *ADOQuery;
int res = 0;
	/* causali errore missione ricevuta :
		0 missione corretta ricevuta da Agvx
		1 missione assegnata non modificabile generico
		2 missione asssegnata non modificabile per deadlock
		3 missione asssegnata non modificabile per newtask
		4 missione asssegnata non modificabile perchè stato carico cambiato su agv
		5 missione assegnata non modificabile per eccezione
		6 id missione già presente
		7 posizione prelievo o deposito non esistenti
		8 posprel = posdep con tipo missione di movimento
		9 tab carico o scarico non presenti su posprel o posdep
		10 idmissione 0 non accettato
		11 missione di solo prelievo per agv già carico non accettata
		12 missione di solo deposito per agv scarico non accettata
	*/
	try {
		if (!ADOConnection1->Connected) {
			return ;
		}
		switch(causale) {
			case 0 :
				str_missione = "Missione Ok" ;
				break ;
			case 1 :
				str_missione = "ERR : 1 Missione assegnata non modificabile generico";
				break ;
			case 2 :
				str_missione = "ERR : 2 missione asssegnata non modificabile per deadlock ";
				break ;
			case 3 :
				str_missione = "ERR : 3 missione asssegnata non modificabile per newtask ";
				break ;
			case 4 :
				str_missione = "ERR : 4 missione asssegnata non modificabile perchè stato carico cambiato su agv";
				break ;
			case 5 :
				str_missione = "ERR : 5 missione assegnata non modificabile per eccezione ";
				break ;
			case 6 :
				str_missione = "ERR : 6 id missione già presente ";
				break ;
			case 7 :
				str_missione = "ERR : 7 posizione prelievo o deposito non esistenti ";
				break ;
			case 8 :
				str_missione = "ERR : 8 posprel = posdep con tipo missione di movimento ";
				break ;
			case 9 :
				str_missione = "ERR : 9 tab carico o scarico non presenti su posprel o posdep ";
				break ;
			case 10 :
				str_missione = "ERR : 10 idmissione 0 non accettato ";
				break ;
			case 11 :
				str_missione = "ERR : 11 missione di solo prelievo per agv già carico non accettata ";
				break ;
			case 12 :
				str_missione = "ERR : 12 missione di solo deposito per agv scarico non accettata";
				break ;
		}
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
		strsql = "UPDATE Missioni SET Risposta_Agvx = '"+str_missione+"'";
		strsql = strsql + " where id = " + IntToStr(idmiss);
		ADOQuery->SQL->Text = strsql;
		res = ADOQuery->ExecSQL();
		if (!causale)
			LogMsg("Missione  " + IntToStr(idmiss) + " elaborata da Agv X correttamente");
		else
			LogMsg("Missione  " + IntToStr(idmiss) + " non elaborata da Agv X correttamente, msg "+str_missione);
		//vedere cosa fare
	}catch(...){
	}
}

int TdmDB::ReturnIdMissioneDaProgressivo(int progressivo) {
    AnsiString stringa, time = "00:00:00";
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from missioni where ProgressivoMissione = " + IntToStr(progressivo);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
          	res = ADOQuery->FieldByName("Id")->AsInteger ;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}
// ---------------------------------------------------------------------------

AnsiString TdmDB::RiempiTipologiaCorsia(TComboBox * tcbCombo, int actual) 
{
    // riempio codice articolo combo box
    AnsiString stringa, sval, ret = "";
    int nVal;
    TComboBox *cbCombo;
    TADOQuery *ADOQuery;
    
    cbCombo = tcbCombo;
    cbCombo->Items->Clear();

    if (!ADOConnection1->Connected)
        return "";

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = ADOConnection1;
    
    stringa = "SELECT * FROM TabTipologiaCorsia ORDER BY CodTipo ";

    try 
    {
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        
        if (ADOQuery->RecordCount) 
        {
            ADOQuery->First();
            while (!ADOQuery->Eof) 
            {
                nVal = ADOQuery->FieldByName("CodTipo")->AsInteger;
                sval = ADOQuery->FieldByName("Descrizione")->AsString;

                cbCombo->Items->Append(sval);
                if (nVal == actual)
                    ret = sval;
                ADOQuery->Next();
            }
        }
        ADOQuery->Close();
    }
    catch (...) 
    {
        dmDB->LogMsg("Eccezione su RiempiTipologia Corsia");
    }
    return ret;
}


//-------------------------------------------------------------------------------------------------------------------------------------
// Recupera i dati in ingresso nella tabella PalletIN  -con ID-
//-------------------------------------------------------------------------------------------------------------------------------------
int TdmDB::GetLGVrecord(AnsiString SSCC, TLGV &lgv)
{
    AnsiString sStr;
    TADOQuery *ADOQuery;
    int nRet = 0;

    if (!FMConnection->Connected)
        return 0;

    if (SSCC == "") 
        return 0;

    ADOQuery = new TADOQuery(NULL);
    try
    {
        ADOQuery->Connection = FMConnection;
		sStr = "SELECT * FROM LGV WHERE SSCC = " + SSCC;
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = sStr;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
        {
            lgv.ID = ADOQuery->FieldByName("ID")->AsInteger;
            lgv.ordine = ADOQuery->FieldByName("ordine")->AsInteger;
            lgv.lotto = ADOQuery->FieldByName("lotto")->AsString;
            lgv.codArticolo = ADOQuery->FieldByName("codArticolo")->AsString;
            lgv.etichetta_prodCode = ADOQuery->FieldByName("etichetta_prodCode")->AsString;
            lgv.etichetta_prodEAN = ADOQuery->FieldByName("etichetta_prodEAN")->AsString;
            lgv.etichetta_descrizione = ADOQuery->FieldByName("etichetta_descrizione")->AsString;
			lgv.etichetta_imballoEAN = ADOQuery->FieldByName("etichetta_imballoEAN")->AsString;
			lgv.etichetta_expDate = ADOQuery->FieldByName("etichetta_expDate")->AsString;
			lgv.SSCC = ADOQuery->FieldByName("SSCC")->AsString;
            lgv.nScatole = ADOQuery->FieldByName("nScatole")->AsInteger;
            lgv.nColliPerScatola = ADOQuery->FieldByName("nColliPerScatola")->AsInteger;
            lgv.stato = ADOQuery->FieldByName("stato")->AsInteger;
            lgv.lineaPrelievo = ADOQuery->FieldByName("lineaPrelievo")->AsInteger;
            lgv.lineaDeposito = ADOQuery->FieldByName("lineaDeposito")->AsInteger;
            lgv.priorita = ADOQuery->FieldByName("priorita")->AsInteger;
            lgv.unloadState = ADOQuery->FieldByName("unloadState")->AsInteger;
            lgv.timeUnload = ADOQuery->FieldByName("timeUnload")->AsDateTime;
            lgv.toWrap = ADOQuery->FieldByName("toWrap")->AsInteger;
            lgv.wrapState = ADOQuery->FieldByName("wrapState")->AsInteger;
            lgv.timeWrap = ADOQuery->FieldByName("timeWrap")->AsDateTime;
            lgv.toPrint = ADOQuery->FieldByName("toPrint")->AsInteger;
            lgv.printContentFormat = ADOQuery->FieldByName("printContentFormat")->AsInteger;
            lgv.printedState = ADOQuery->FieldByName("printedState")->AsInteger;
            lgv.timePrint = ADOQuery->FieldByName("timePrint")->AsDateTime;
            lgv.storageState = ADOQuery->FieldByName("storageState")->AsInteger;
            lgv.timeStorage = ADOQuery->FieldByName("timeStorage")->AsDateTime;

            nRet =  1;
        }
        else
            nRet =  0;
    }
    catch (...)
    {
		LogMsg("Eccezione GetLGVrecord, SSCC = " + SSCC);
        nRet =  0;
    }
    delete ADOQuery;
    return nRet;

}
int TdmDB::GetSSCCrecordStructPlc(AnsiString SSCC, TDatiSSCCPlc &Struct)
{
    AnsiString sStr;
    TADOQuery *ADOQuery;
    int nRet = 0;

    if (!FMConnection->Connected)
        return 0;

    ADOQuery = new TADOQuery(NULL);
    try
    {
        ADOQuery->Connection = FMConnection;
		sStr = "SELECT * FROM LGV WHERE SSCC = " + SSCC;
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = sStr;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
        {
			Struct.lotto = ADOQuery->FieldByName("lotto")->AsString;
			Struct.codArticolo = ADOQuery->FieldByName("codArticolo")->AsString;
			Struct.etichetta_prodCode = ADOQuery->FieldByName("etichetta_prodCode")->AsString;
			Struct.etichetta_prodEAN = ADOQuery->FieldByName("etichetta_prodEAN")->AsString;
			Struct.etichetta_descrizione = ADOQuery->FieldByName("etichetta_descrizione")->AsString;
			Struct.etichetta_imballoEAN = ADOQuery->FieldByName("etichetta_imballoEAN")->AsString;
			Struct.etichetta_expDate = ADOQuery->FieldByName("etichetta_expDate")->AsString;
			Struct.SSCC = ADOQuery->FieldByName("SSCC")->AsString;
			Struct.prgfasciatura = ADOQuery->FieldByName("toWrap")->AsInteger;
			Struct.layoutetichetta = ADOQuery->FieldByName("toPrint")->AsInteger;
            Struct.numscatole = ADOQuery->FieldByName("nScatole")->AsInteger;
			nRet =  1;
		}
		else
		{
			Struct.lotto = "";
			Struct.codArticolo = "";
			Struct.etichetta_prodCode = "";
			Struct.etichetta_prodEAN = "";
			Struct.etichetta_descrizione = "";
			Struct.etichetta_imballoEAN = "";
			Struct.etichetta_expDate = "";
			Struct.SSCC = "";
			Struct.prgfasciatura = 0;
			Struct.layoutetichetta = 0;
            Struct.numscatole = 0;
			nRet =  0;
		}
	}
	catch (...)
    {
		LogMsg("Eccezione GetSSCCrecordStructPlc, SSCC = " + SSCC);
        nRet =  0;
    }
    delete ADOQuery;
    return nRet;

}

//-------------------------------------------------------------------------------------------------------------------------------------
int TdmDB::GetUDCdapos(int nPos)
{
    AnsiString sStr;
    TADOQuery *ADOQuery;
    int nRet = 0;

    if (!ADOConnection1->Connected)
        return 0;

    ADOQuery = new TADOQuery(NULL);
    try {
        ADOQuery->Connection = ADOConnection1;
        sStr = "SELECT PresenzaPallet FROM Posizioni WHERE Pos = " + IntToStr(nPos);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = sStr;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
        {
            nRet = ADOQuery->FieldByName("PresenzaPallet")->AsInteger;
        }
        else
            nRet =  0;
    }
    catch (...)
    {
        LogMsg("Eccezione GetUDCdapos, Pos = " + IntToStr(nPos));
        nRet =  0;
    }
    delete ADOQuery;
    return nRet;

}

//-------------------------------------------------------------------------------------------------------------------------------------
// Recupera L'ultimo recor in pos carico passata 
//-------------------------------------------------------------------------------------------------------------------------------------
int TdmDB::GetLastLGVrecordFromCAR(AnsiString sName, TLGV &lgv)
{
    AnsiString sStr, sCar;
    TADOQuery *ADOQuery;
    int nRet = 0;
    int car;

    if (!FMConnection->Connected)
        return 0;

    ADOQuery = new TADOQuery(NULL);
    try 
    {
        ADOQuery->Connection = FMConnection;
        sStr = "SELECT * FROM LGV WHERE stato < 1 AND lineaPrelievo = " + sName + " ORDER BY priorita ASC, timestamp ASC";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = sStr;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
        {
            lgv.ID = ADOQuery->FieldByName("ID")->AsInteger;
            lgv.ordine = ADOQuery->FieldByName("ordine")->AsInteger;
            lgv.lotto = ADOQuery->FieldByName("lotto")->AsString;
            lgv.codArticolo = ADOQuery->FieldByName("codArticolo")->AsString;
            lgv.etichetta_prodCode = ADOQuery->FieldByName("etichetta_prodCode")->AsString;
            lgv.etichetta_prodEAN = ADOQuery->FieldByName("etichetta_prodEAN")->AsString;
            lgv.etichetta_descrizione = ADOQuery->FieldByName("etichetta_descrizione")->AsString;
			lgv.etichetta_imballoEAN = ADOQuery->FieldByName("etichetta_imballoEAN")->AsString;
			lgv.etichetta_expDate = ADOQuery->FieldByName("etichetta_expDate")->AsString;
            lgv.SSCC = ADOQuery->FieldByName("SSCC")->AsString;
            lgv.nScatole = ADOQuery->FieldByName("nScatole")->AsInteger;
            lgv.nColliPerScatola = ADOQuery->FieldByName("nColliPerScatola")->AsInteger;
            lgv.stato = ADOQuery->FieldByName("stato")->AsInteger;
            lgv.lineaPrelievo = ADOQuery->FieldByName("lineaPrelievo")->AsInteger;
            lgv.lineaDeposito = ADOQuery->FieldByName("lineaDeposito")->AsInteger;
            lgv.priorita = ADOQuery->FieldByName("priorita")->AsInteger;
            lgv.unloadState = ADOQuery->FieldByName("unloadState")->AsInteger;
            lgv.timeUnload = ADOQuery->FieldByName("timeUnload")->AsDateTime;
            lgv.toWrap = ADOQuery->FieldByName("toWrap")->AsInteger;
            lgv.wrapState = ADOQuery->FieldByName("wrapState")->AsInteger;
            lgv.timeWrap = ADOQuery->FieldByName("timeWrap")->AsDateTime;
            lgv.toPrint = ADOQuery->FieldByName("toPrint")->AsInteger;
            lgv.printContentFormat = ADOQuery->FieldByName("printContentFormat")->AsInteger;
            lgv.printedState = ADOQuery->FieldByName("printedState")->AsInteger;
            lgv.timePrint = ADOQuery->FieldByName("timePrint")->AsDateTime;
            lgv.storageState = ADOQuery->FieldByName("storageState")->AsInteger;
            lgv.timeStorage = ADOQuery->FieldByName("timeStorage")->AsDateTime;

            nRet =  1;
        }
        else
            nRet =  0;
    }
    catch (...)
    {
        LogMsg("Eccezione GetLastLGVrecordFromCAR, Pos = " + sName);
        nRet =  0;
    }
    delete ADOQuery;
    return nRet;

}

// ---------------------------------------------------------------------------

void TdmDB::UpdatePresenzaVuotiFalde(int type, int act) 
{
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int pos;
/*
    if (!ADOConnection1->Connected) 
        return;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = ADOConnection1;

    try 
    {
        if (type == PRELVUOTI_TYPE)
            pos = POS_PREL_VUOTI;
        else 
            pos = POS_PREL_FALDE;
            
        if (act == 0) 
			strsql.printf("UPDATE Posizioni SET IdUdc = 0 , PresenzaPallet = 1 WHERE pos = %d", pos);
        else
            strsql.printf("UPDATE Posizioni SET IdUdc = 0 , PresenzaPallet = 0 WHERE pos = %d", pos);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) 
    {
    }
    delete ADOQuery;    */
}

// ---------------------------------------------------------------------------

int TdmDB::RitornaIdUdcPosizione(int pos) 
{
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;

    if (!ADOConnection1->Connected) 
        return 0;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = ADOConnection1;

	try {
		stringa = "SELECT * FROM Posizioni WHERE pos = "+IntToStr(pos);
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount > 0) 
        {
			res = ADOQuery->FieldByName("IdUdc")->AsInteger;
		}
        ADOQuery->Close();
	}
    catch (...) 
    {
        LogMsg("Eccezione RitornaIdUdcPosizione, Pos = " + IntToStr(pos));
        res =  0;
	}
	delete ADOQuery;
	return res;
}


// ---------------------------------------------------------------------------

int TdmDB::ReturnPrioritaMagDaPos(int pos) 
{
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;

    if (!ADOConnection1->Connected) 
        return 0;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = ADOConnection1;
	try 
    {
		stringa = "SELECT * FROM Posizioni INNER JOIN Corsie ON Posizioni.Fila = Corsie.Fila WHERE Posizioni.Pos = "+IntToStr(pos);
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->Open();
		if (ADOQuery->RecordCount > 0) 
        {
			res = ADOQuery->FieldByName("Priorita")->AsInteger;
		}
		ADOQuery->Close();
	}
	catch (...) {
		LogMsg("Eccezione in ReturnPrioritaMagDaPos - " + stringa);
	}
	delete ADOQuery;
	return res;
}


// ---------------------------------------------------------------------------
// Aggiorna STATO e ORA (mettendo NOW()) dello SCARICO

void TdmDB::UpdateUnloadState(AnsiString SSCC, int state) 
{
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;

    if (!FMConnection->Connected) 
        return;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = FMConnection;
	try 
    {
		stringa = "UPDATE LGV SET unloadState = " + IntToStr(state) + ", timeUnload = GetDate() WHERE SSCC = " + SSCC;
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->ExecSQL();
		ADOQuery->Close();
        LogMsg("Aggiorno stato/ora SCARICO, SSCC = " + SSCC);
	}
	catch (...) {
		LogMsg("Eccezione in UpdateUnloadState - " + stringa);
	}
	delete ADOQuery;
}

// ---------------------------------------------------------------------------
// Aggiorna STATO e ORA (mettendo NOW()) della FASCIATURA

void TdmDB::UpdateWrapState(AnsiString SSCC) 
{
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;

    if (!FMConnection->Connected) 
        return;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = FMConnection;
	try 
    {
		stringa = "UPDATE LGV SET wrapState = 1, timeWrap = GetDate() WHERE wrapState = 0 AND SSCC = " + SSCC;
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->ExecSQL();
		ADOQuery->Close();
        LogMsg("Aggiorno stato/ora FASCIATURA, SSCC = " + SSCC);
	}
	catch (...) {
		LogMsg("Eccezione in UpdateWrapState - " + stringa);
	}
	delete ADOQuery;
}

// ---------------------------------------------------------------------------
// Aggiorna STATO e ORA (mettendo NOW()) della STAMPA ETICHETTA

void TdmDB::UpdatePrintedState(AnsiString SSCC,int printed_state)
{
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;

    if (!FMConnection->Connected) 
        return;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = FMConnection;
	try 
    {
		stringa = "UPDATE LGV SET printedState = "+IntToStr(printed_state)+" , timePrint = GetDate() WHERE SSCC = " + SSCC;
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->ExecSQL();
		ADOQuery->Close();
		LogMsg("Aggiorno stato/ora STAMPA ETICHETTA, SSCC = " + SSCC+" , printedState = "+IntToStr(printed_state));
	}
	catch (...) {
		LogMsg("Eccezione in UpdatePrintedState - " + stringa);
	}
	delete ADOQuery;
}

// ---------------------------------------------------------------------------
// Aggiorna STATO e ORA (mettendo NOW()) dello scaricamento a MAGAZZINO

void TdmDB::UpdateStorageState(AnsiString SSCC) 
{
	AnsiString stringa;
	AnsiString strsql;
	TADOQuery *ADOQuery;
	int res = 0;

    if (!FMConnection->Connected) 
        return;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = FMConnection;
	try 
    {
		stringa = "UPDATE LGV SET storageState = 1, timeStorage = GetDate() WHERE SSCC = " + SSCC;
		ADOQuery->Close();
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Append(stringa);
		ADOQuery->ExecSQL();
		ADOQuery->Close();
        LogMsg("Aggiorno stato/ora scaricamento in MAGAZZINO, SSCC = " + SSCC);
	}
	catch (...) {
		LogMsg("Eccezione in UpdateStorageState - " + stringa);
	}
	delete ADOQuery;
}

// ---------------------------------------------------------------------------

int TdmDB::CreaIdUDC() 
{
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idsrticolo = 0;
    int nGiro;
    int res = 0;
    try 
    {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 IDUDC FROM UDC order by IdUDC DESC");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        idsrticolo = ADOQuery->FieldByName("IDUDC")->AsInteger;
        //do 
        //{
            if (idsrticolo > 4000000000) 
            {
                idsrticolo = 1;
            }
            else
            {
                idsrticolo++;
            }
        //}
        //while (ADOQuery->Locate("IDUDC", idsrticolo, Opts));

        ADOQuery->Close();
        ADOQuery->SQL->Clear();

        strsql.printf("INSERT INTO UDC (IDUDC, CodUDC, Stato, CodTipoUDC) VALUES (%d,' ', 0, 0)", idsrticolo);
        ADOQuery->SQL->Text = strsql;

        if (ADOQuery->ExecSQL()) 
            res = idsrticolo;
    }
    catch (...) 
    {
		LogMsg("Eccezione su: " + strsql);
    }
    delete ADOQuery;
    return res;
}

// ---------------------------------------------------------------------------

int TdmDB::UpdateSoloStato(int pos, int prenota, int escludi) 
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;

    if (!ADOConnection1->Connected)
        return 0;

    ADOQuery = new TADOQuery(NULL);
    try {
        ADOQuery->Connection = ADOConnection1;

        stringa.printf("UPDATE Posizioni SET " " Prenotata = %d, " " Disabilita = %d " "  WHERE Pos = %d " ,
            prenota, escludi, pos, pos);

        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
		LogMsg(stringa);
    }
    catch (...) {
		LogMsg("Eccezione su: " + stringa);
    }
    delete ADOQuery;
    return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Aggiorna la presenza UDC nella pos carico - nella "presenzaPallet mette l'ID della tabella di caricamento per comodità
//
int TdmDB::UpdatePosCarico(AnsiString PosCli, AnsiString tudc, int idcar)
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;

    if (!ADOConnection1->Connected)
        return 0;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = ADOConnection1;

    try
    {
        stringa.printf("UPDATE Posizioni SET IDUDC = '%s', PresenzaPallet = %d  WHERE CustomerPos = '%s'", tudc, idcar, PosCli);

        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
		ADOQuery->ExecSQL();
		dmDB->LogMsg(stringa);
		res = 1;
    }
    catch (...)
    {
        LogMsg("Eccezione UpdatePosCarico, CustomerPos = " + PosCli);
    }
    delete ADOQuery;
    return res;
}
// ---------------------------------------------------------------------------
// Inserisce un record nel caricamento

int TdmDB::InsertCaricamento(int linea, int udc, int pos) 
{
	AnsiString strsql, strg;
	TADOQuery *ADOQuery;
    TLocateOptions Opts;
    Opts.Clear();
    int lastID = 0;

	try 
    {
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = ADOConnection1;

        strsql.printf("SELECT TOP 1 ID FROM MissioniCaricamento order by ID DESC");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        lastID = ADOQuery->FieldByName("ID")->AsInteger;
        do 
        {
            if (lastID > 4000000000) 
            {
                lastID = 1;
            }
            else
                lastID++;
        }
        while (ADOQuery->Locate("ID", lastID, Opts));
        ADOQuery->Close();

		strsql.printf("INSERT INTO MissioniCaricamento (ID, Stato, DataOra, Linea, IDUDC, pos) VALUES (%d, 0, GetDate(), %d, %d, %d ) ", lastID, linea, udc, pos);
        ADOQuery->SQL->Text = strsql;
		ADOQuery->ExecSQL();
        dmDB->LogMsg("Inserisco richiesta di caricamento : "+ strsql + " , result : " + IntToStr(lastID));
        
    }
    catch (...) 
    {
        LogMsg("Eccezione InsertCaricamento, pos = " + IntToStr(pos));
    }
	delete ADOQuery;
    return lastID;
}


///////////////////////////////////////////////////////////////
//  Aggiorna lo stato della tabella di Caricamento
//
int TdmDB::UpdateStatoCaricamento(int ID, int stato)
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;

    if (!ADOConnection1->Connected)
        return 0;

    ADOQuery = new TADOQuery(NULL);
    ADOQuery->Connection = ADOConnection1;

    try
    {
        if (stato == STATO_INIZIATA) 
            stringa.printf("UPDATE MissioniWms SET StatoMissione = %d, DataOraInizioMissione=GetDate() WHERE Progressivo = %d", stato, ID);
            
        else if (stato == STATO_PRELEVATA)
            stringa.printf("UPDATE MissioniWms SET StatoMissione = %d, DataOraPrelievoMissione=GetDate() WHERE Progressivo = %d", stato, ID);

        else if (stato == STATO_DEPOSITATA)
            stringa.printf("UPDATE MissioniWms SET StatoMissione = %d, DataOraFineMissione=GetDate() WHERE Progressivo = %d", stato, ID);

        else 
            stringa.printf("UPDATE MissioniWms SET StatoMissione = %d  WHERE Progressivo = %d", stato, ID);

        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Aggiorna stato tabella MissioniWms con ID " + IntToStr(ID) + " = " + IntToStr(stato));
    }
    catch (...)
    {
        LogMsg("Eccezione UpdateStatoCaricamento, ID = " + IntToStr(ID) + ", stato = " + IntToStr(stato));
    }
    delete ADOQuery;
    return res;
}

// ---------------------------------------------------------------------------
// Inserisce un record nel caricamento

AnsiString TdmDB::GetDataCaricamento(int nLinea) 
{
	AnsiString strsql;
	TADOQuery *ADOQuery;
	AnsiString res = "";

	ADOQuery = new TADOQuery(NULL);
	try 
    {
		ADOQuery->Connection = ADOConnection1;
		strsql.printf("SELECT * FROM MissioniCaricamento WHERE Linea = %d AND Stato = 0", nLinea);   //solo quelle appena inserite
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) 
        {
            res = TimeToStr(ADOQuery->FieldByName("DataOra")->AsDateTime);
        }
        else
        {
            res = TimeToStr(0);
        }
        ADOQuery->Close();
    }
    catch (...) 
    {
        LogMsg("Eccezione GetDataCaricamento, Linea = " + IntToStr(nLinea));
    }
	delete ADOQuery;
    return res;
}

// ---------------------------------------------------------------------------

void TdmDB::ModificaStatoUdc(int IDUDC, int Stato) 
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    
    if (!ADOConnection1->Connected) 
        return;
        
    ADOQuery = new TADOQuery(NULL);
    try 
    {
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE UDC SET Stato = " + IntToStr(Stato) + " WHERE IDUDC = " + IntToStr(IDUDC);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Modificato Stato = " + IntToStr(Stato) + " su UDC  " + IntToStr(IDUDC));
    }
    catch (...) 
    {
    }

    delete ADOQuery;
}

// ---------------------------------------------------------------------------

int TdmDB::GetStatoUdc(int IDUDC) 
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = -1;
    
    if (!ADOConnection1->Connected) 
        return res;
        
	ADOQuery = new TADOQuery(NULL);
	try 
    {
		ADOQuery->Connection = ADOConnection1;
		stringa.printf("SELECT * FROM UDC WHERE IDUDC = %d", IDUDC);
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) 
        {
            res = ADOQuery->FieldByName("Stato")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) 
    {
    }
    delete ADOQuery;
    return res;
}

// ---------------------------------------------------------------------------
int TdmDB::AggiornaHDepHPrelPerPos(int pos, int h1, int h2, bool solo1)
{
	// Posizioni Vuoti
	AnsiString stringa1, stringa2;
	TADOQuery *ADOQuery;
	int res = 0;

	if (!ADOConnection1->Connected)
		return 0;

	ADOQuery = new TADOQuery(NULL);
	ADOQuery->Connection = ADOConnection1;

	try
	{
        if (pos < 100)  //Posizione di carico
        {
    		stringa1.printf("UPDATE Posizioni SET HPrel = %d  WHERE pos = %d AND Piano = 1", h1, pos);
    		stringa2.printf("UPDATE Posizioni SET HPrel = %d  WHERE pos = %d AND Piano = 2", h2, pos);
        }
        else    //Posizione di SCARICO
        {
    		stringa1.printf("UPDATE Posizioni SET HDep = %d  WHERE pos = %d AND Piano = 1", h1, pos);
    		stringa2.printf("UPDATE Posizioni SET HDep = %d  WHERE pos = %d AND Piano = 2", h2, pos);
        }

		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = stringa1;
		res = ADOQuery->ExecSQL();
		dmDB->LogMsg(stringa1);

        if (!solo1) // se non è specificato il "solo1" fa anche il 2ndo piano
        {
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Text = stringa2;
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();
		    dmDB->LogMsg(stringa2);
        }        
	}
	catch (...)
	{
        LogMsg("Eccezione AggiornaHDepHPrelPerPos, query = " + stringa1 + "-" + stringa2);
	}
	delete ADOQuery;
	return 0;
}

// ---------------------------------------------------------------------------

int TdmDB::GetPalletMagazzino(int tipo) 
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = -1;
    
    if (!ADOConnection1->Connected) 
        return res;
        
	ADOQuery = new TADOQuery(NULL);
	try 
    {
		ADOQuery->Connection = ADOConnection1;
		stringa = "SELECT * FROM Posizioni INNER JOIN Corsie ON Posizioni.Fila = Corsie.Fila  ";

        if (tipo == TYPE_TUTTI) 
            stringa += "WHERE Posizioni.Pos > 100 AND Posizioni.pos < 200 AND Posizioni.PresenzaPallet = 1";
        else
            stringa += "WHERE Posizioni.Pos > 100 AND Posizioni.pos < 200 AND Posizioni.PresenzaPallet = 1 AND Corsie.Tipologia = " + IntToStr(tipo);
            
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        res = ADOQuery->RecordCount;
        ADOQuery->Close();
    }
    catch (...) 
    {
    }
    delete ADOQuery;
    return res;
}

// ---------------------------------------------------------------------------

int TdmDB::GetVuotiInMagazzino() 
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = -1;
    
    if (!ADOConnection1->Connected) 
        return res;
        
	ADOQuery = new TADOQuery(NULL);
	try 
    {
		ADOQuery->Connection = ADOConnection1;
		stringa = "SELECT * FROM Posizioni INNER JOIN Corsie ON Posizioni.Fila = Corsie.Fila  ";
        stringa += "WHERE Posizioni.Pos > 200 AND Posizioni.PresenzaPallet = 1 AND Corsie.Tipologia = 4";
            
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        res = ADOQuery->RecordCount;
        ADOQuery->Close();
    }
    catch (...) 
    {
    }
    delete ADOQuery;
    return res;
}

// ---------------------------------------------------------------------------
int TdmDB::AggiornaESvuotaPosDeposito()
{
	// Posizioni Vuoti
	AnsiString stringa;
	TADOQuery *ADOQuery;
	int res = 0;

	if (!ADOConnection1->Connected)
		return 0;

	ADOQuery = new TADOQuery(NULL);
	ADOQuery->Connection = ADOConnection1;

	try
	{
		stringa.printf("UPDATE Posizioni SET IdUdc = 0, PresenzaPallet = 0  WHERE TipoPosizione = %d", LINEASCAR_TYPE);

		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = stringa;
		res = ADOQuery->ExecSQL();
		ADOQuery->Close();
		dmDB->LogMsg(stringa);
	}
	catch (...)
	{
        LogMsg("Eccezione AggiornaESvuotaPosDeposito, query = " + stringa);
	}
	delete ADOQuery;
	return 0;
}

// ---------------------------------------------------------------------------


