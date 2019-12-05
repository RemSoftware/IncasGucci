// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "password.h"
#include "chiusura.h"
#include "percorsiprototipo.h"
#include "supervisionesocket.h"
#include "DB.h"
#include "msg.h"
#include "config.h"
#include "allarmi.h"
#include "missioni.h"
#include "agv.h"
#include "PLCThread.h"
#include "plcphoenixthread.h"
#include "datamodulecomandiagv.h"
//#include "segnaliBaia.h"
//#include "baiacarico.h"
#include "baiascarico.h"
//#include "stato_pressa.h"
#include "missioni_wms.h"
#include "tab_eventi.h"
#include "stato_pos.h"
#include "PosVuoti.h"
#include "PosPresse.h"
#include "UDClist.h"
#include "plcCarico.h"

#include <inifiles.hpp>
#include <stdio.h>
char File_name[50];
char N_carrelli = 1;           //okkio che viene caricato dal file PARAMS.TXT
int MISSIONS_PORT = 20000;
/* porta su cui AgvX è server per ricezione missioni */
int MISS_STATE_PORT = 20001;
/* porta su cui AgvX invia stato missioni a AgvOrd */
int AGV_REC_PORT = 20002;
/* porta su cui AgvX è server per ricevere eventi Agv */
int AGV_SEND_PORT = 20003; /* porta su cui AgvX invia comandi Agv a AgvComm */
char HOST_COM_ADDRESS[20] = "127.0.0.1";
char HOST_ORD_ADDRESS[20] = "127.0.0.1";
char HOST_AGVX_ADDRESS[20] = "127.0.0.1";

DATILGV tab;

CLIENTSOCKET *sock;
CLIENTSOCKET *cs;
SUPERVISIONEMISS_STATESOCKET *ss;
TPaintBox *PaintBox1;
TMissione MisManuale;
// ---------------------------------------------------------------------------
#pragma link "MessageBar"
#pragma link "RotImg"
#pragma link "MyShape"
#pragma resource "*.dfm"
TMainForm *MainForm;

// ---------------------------------------------------------------------------
void TMainForm::LeggiParametri() {
    FILE *f;
    char s[80];

    int n;

    if ((f = fopen("params.txt", "r")) == NULL) {

		strcpy(File_name, "IncasGucci.crs");

        return;

    }

    rewind(f);

    while (!feof(f)) {
        if (fscanf(f, "%s", s) != 1)

            break;

        if (!strcmp(s, "File_name")) {

            fscanf(f, "%s", File_name);

        }
		else if (!strcmp(s, "N_carrelli")) {

            fscanf(f, "%d", &N_carrelli);
        }
        else if (!strcmp(s, "MISSIONS_PORT")) {

            fscanf(f, "%d", &MISSIONS_PORT);

        }
        else if (!strcmp(s, "MISS_STATE_PORT")) {

            fscanf(f, "%d", &MISS_STATE_PORT);

        }
        else if (!strcmp(s, "AGV_REC_PORT")) {

            fscanf(f, "%d", &AGV_REC_PORT);

        }
        else if (!strcmp(s, "AGV_SEND_PORT")) {

            fscanf(f, "%d", &AGV_SEND_PORT);

        }
        else if (!strcmp(s, "FORK_QUOTE_LOAD")) {

            fscanf(f, "%d", &quota_dep_terra);

        }
        else if (!strcmp(s, "HOST_COM_ADDRESS")) {

            fscanf(f, "%s", HOST_COM_ADDRESS);

        }
        else if (!strcmp(s, "HOST_ORD_ADDRESS")) {

            fscanf(f, "%s", HOST_ORD_ADDRESS);

        }
        else if (!strcmp(s, "HOST_AGVX_ADDRESS")) {

            fscanf(f, "%s", HOST_AGVX_ADDRESS);

        }

    }

    fclose(f);
}

void TMainForm::DisegnaCarrello(int car) {
    double x, y;
    TRotateImage *imgCarr;
    AnsiString filename;

    imgCarr = (TRotateImage*) FindComponent("img" + IntToStr(car));
    if (DatiAgv[car].load) {
		filename = "GucciAgv" + IntToStr(car) + "l";
    }
    else
		filename = "GucciAgv" + IntToStr(car);
    if ((DatiAgv[car].alarm) || (DatiAgv[car].manuale))
        filename += "a";
    filename += ".bmp";
    if (FileExists(filename)) {
        imgCarr->Picture->LoadFromFile(filename);
	}
    if (DatiAgv[car].pos != 0) {
        if (tab.mappa_nodi.find(DatiAgv[car].pos) != tab.mappa_nodi.end()) {
            x = Xo + ((Image1->Width * tab.mappa_nodi[DatiAgv[car].pos].posx) / W) + Image1->Left;
            y = Yo - ((Image1->Height * tab.mappa_nodi[DatiAgv[car].pos].posy) / H) + Image1->Top;

			if ((imgCarr->Width != (110 * zoom)) || (imgCarr->Height != (96 * zoom)) || (imgCarr->Angle != tab.mappa_nodi[DatiAgv[car].pos].ang) || (imgCarr->Top != (y - imgCarr->Height / 2)) || (imgCarr->Left != (x - imgCarr->Width / 2)) || (!imgCarr->Visible)) {
                imgCarr->Angle = tab.mappa_nodi[DatiAgv[car].pos].ang;
				imgCarr->Width = 70 * zoom;
				imgCarr->Height = 68 * zoom;
                imgCarr->Top = y - imgCarr->Height / 2;
                imgCarr->Left = x - imgCarr->Width / 2;
                imgCarr->Visible = true;
            }
        }
        else
            imgCarr->Visible = false;
    }
    else {
        imgCarr->Visible = false;
    }
}

void TMainForm::VisualizzaPercorsoAgv(int nagv) {
    TMyConnector *ToPickAgv, *ToDropAgv;
    TRotateImage *imgCarr;
    TMyShape *Pos;
    int agv;

    return;

    for (agv = 1; agv <= N_carrelli; agv++) {
        if ((agv == nagv) || (nagv == 0)) {
            ToPickAgv = (TMyConnector*) ScrollBox1->FindComponent("ToPickAgv" + IntToStr(agv));
            ToDropAgv = (TMyConnector*) ScrollBox1->FindComponent("ToDropAgv" + IntToStr(agv));
            imgCarr = (TRotateImage*) FindComponent("img" + IntToStr(agv));
            if (ToPickAgv == NULL) { // capita che quando chiudo il programma rientri qui come first, da capire come mai
                ToPickAgv1 = new TMyConnector(ScrollBox1);
                ToPickAgv1->Parent = ScrollBox1;

                ToPickAgv1->Name = "ToPickAgv1";

                ToPickAgv1->Visible = false;

                ToPickAgv1->Width = 1;

                ToPickAgv1->Height = 1;

                ToPickAgv1->Pen->Color = clLime;

                ToPickAgv1->Pen->Width = 3;

                ToPickAgv2 = new TMyConnector(ScrollBox1);

                ToPickAgv2->Parent = ScrollBox1;

                ToPickAgv2->Name = "ToPickAgv2";

                ToPickAgv2->Visible = false;

                ToPickAgv2->Width = 1;

                ToPickAgv2->Height = 1;

                ToPickAgv2->Pen->Color = clLime;

                ToPickAgv2->Pen->Width = 3;

                ToDropAgv1 = new TMyConnector(ScrollBox1);
                ToDropAgv1->Parent = ScrollBox1;

                ToDropAgv1->Name = "ToDropAgv1";

                ToDropAgv1->Visible = false;

                ToDropAgv1->Width = 1;

                ToDropAgv1->Height = 1;

                ToDropAgv1->Pen->Color = clYellow;

                ToDropAgv1->Pen->Width = 3;

                ToDropAgv2 = new TMyConnector(ScrollBox1);
                ToDropAgv2->Parent = ScrollBox1;

                ToDropAgv2->Name = "ToDropAgv2";

                ToDropAgv2->Visible = false;

                ToDropAgv2->Width = 1;

                ToDropAgv2->Height = 1;

                ToDropAgv2->Pen->Color = clYellow;

                ToDropAgv2->Pen->Width = 3;

                ToDropPosPrel = new TMyConnector(ScrollBox1);
                ToDropPosPrel->Parent = ScrollBox1;

                ToDropPosPrel->Name = "ToDropPosPrel";

                ToDropPosPrel->Visible = false;

                ToDropPosPrel->Width = 1;

                ToDropPosPrel->Height = 1;

                ToDropPosPrel->Pen->Color = clFuchsia;

                ToDropPosPrel->Pen->Width = 3;

            }
            if (ToPickAgv != NULL) {
                if ((imgCarr == NULL) || (!imgCarr->Visible) || (MainForm->DatiAgv[agv].load) || ((MainForm->DatiAgv[agv].prel == MainForm->DatiAgv[agv].pos) && (MainForm->DatiAgv[agv].prel)) || ((MainForm->DatiAgv[agv].dest == MainForm->DatiAgv[agv].pos) && (MainForm->DatiAgv[agv].dest)) ||
                    (!MainForm->DatiAgv[agv].prel && !MainForm->DatiAgv[agv].dest)) {
                    ToPickAgv->Tag = 0;
                    ToPickAgv->Visible = false;
                }
                else {
                    if ((ToPickAgv->Tag != MainForm->DatiAgv[agv].prel) && (MainForm->DatiAgv[agv].prel)) {
                        Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(MainForm->DatiAgv[agv].prel));
                        if (Pos == NULL)
                            Pos = (TMyShape*) FindComponent("Pos" + IntToStr(MainForm->DatiAgv[agv].prel));
                        if (Pos != NULL) {
                            ToPickAgv->Control1 = imgCarr;
                            ToPickAgv->Control2 = Pos;
                            ToPickAgv->Tag = MainForm->DatiAgv[agv].prel;
                            ToPickAgv->Visible = true;
                        }
                        else {
                            ToPickAgv->Tag = 0;
                            ToPickAgv->Visible = false;
                        }
                    }
                    else if ((ToPickAgv->Tag != MainForm->DatiAgv[agv].dest) && (MainForm->DatiAgv[agv].dest)) {
                        Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(MainForm->DatiAgv[agv].dest));
                        if (Pos == NULL)
                            Pos = (TMyShape*) FindComponent("Pos" + IntToStr(MainForm->DatiAgv[agv].dest));
                        if (Pos != NULL) {
                            ToPickAgv->Control1 = imgCarr;
                            ToPickAgv->Control2 = Pos;
                            ToPickAgv->Tag = MainForm->DatiAgv[agv].dest;
                            ToPickAgv->Visible = true;
                        }
                        else {
                            ToPickAgv->Tag = 0;
                            ToPickAgv->Visible = false;
                        }
                    }
                }
            }
            if (ToDropAgv != NULL) {
                if ((MainForm->DatiAgv[agv].dep) && (MainForm->DatiAgv[agv].load) && (MainForm->DatiAgv[agv].dep != MainForm->DatiAgv[agv].prel)) {
                    if (ToDropAgv->Tag != MainForm->DatiAgv[agv].dep) {
                        Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(MainForm->DatiAgv[agv].dep));
                        if (Pos == NULL)
                            Pos = (TMyShape*) FindComponent("Pos" + IntToStr(MainForm->DatiAgv[agv].dep));
                        if (Pos != NULL) {
                            ToDropAgv->Control1 = imgCarr;
                            ToDropAgv->Control2 = Pos;
                            ToDropAgv->Tag = MainForm->DatiAgv[agv].dep;
                            ToDropAgv->Visible = true;
                        }
                        else {
                            ToDropAgv->Tag = 0;
                            ToDropAgv->Visible = false;
                        }
                    }
                }
                else {
                    ToDropAgv->Tag = 0;
                    ToDropAgv->Visible = false;
                }
            }
        }
    }

}

__fastcall TMainForm::TMainForm(TComponent *Owner) : TForm(Owner) 
{
}

// ---------------------------------------------------------------------------
template<class T>
void __fastcall TMainForm::CreateMDIChild(T *f, String Name, String Caption) {
    bool exists = false; // mod
    int i;

    ScrollBox1->Visible = false;
    SetVisible();
    SendMessage(ClientHandle, WM_SETREDRAW, FALSE, 0);
    for (i = MDIChildCount - 1; i >= 0; i--) {
        if (MDIChildren[i]->Name != Name) {
            ((TMDIChild*)MDIChildren[i])->SetInvisible();
            MDIChildren[i]->Close();
        }
    }
    for (i = 0; i < MDIChildCount; i++) {
        if (MDIChildren[i]->Name == Name) {
            MDIChildren[i]->Caption = Caption;
            ((T*)MDIChildren[i])->PanelTitle->Caption = Caption;
            ShowWindow(MDIChildren[i]->Handle, SW_MAXIMIZE);
            ShowWindow(MDIChildren[i]->Handle, SW_MAXIMIZE);
            ((TMDIChild*)MDIChildren[i])->Mostra();
            exists = true;
            break;
        }
    }
    if (!exists) {
        Application->CreateForm(__classid(T), &f);
        f->Name = Name;
        f->Caption = Caption;
        f->PanelTitle->Caption = Caption;
        ShowWindow(f->Handle, SW_MAXIMIZE);
        f->Mostra();
        if (Screen->Height < NativeScreenHeight)
            f->ScaleBy(Screen->Height - Image4->Height - Panel2->Height - MainStatusBar->Height, NativeScreenHeight - Image4->Height - Panel2->Height - MainStatusBar->Height);
        f->WindowState = wsMinimized;
        f->WindowState = wsMaximized;
    }
    SendMessage(ClientHandle, WM_SETREDRAW, TRUE, 0);
    RedrawWindow(ClientHandle, NULL, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_NOINTERNALPAINT);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bEsciClick(TObject *Sender) {
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose) {
    int res;

    res = ChiusuraForm->ShowModal();
    CanClose = ((res == IDYES) || (res == IDOK));
    chiusura_applicazione = CanClose;
    if (CanClose) {
        ShowCursor(true);
    }
    if (res == IDYES) {
        HANDLE hToken;
        TOKEN_PRIVILEGES tkp;

        // Get a token for this process.
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
            // Get the LUID for the shutdown privilege.
            LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

            tkp.PrivilegeCount = 1; // one privilege to set
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            // Get the shutdown privilege for this process.
            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

            ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
        }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender) 
{
    int lang, i, j;
    TIniFile *Ini;
    TCursor Save_Cursor = Screen->Cursor;
    FILE *f;
    char s[100];
    double fXo, fYo;
    double fH, fW, fFH, fFW;
    Screen->Cursor = crHourGlass; // Show hourglass cursor
    ShowWindow(Handle, SW_HIDE);
    if (DebugHook) 
    {
        pwdlevel = 2;
    }
    else 
    {
        pwdlevel = 0;
    }
    PWD = "proxaut";
    CorsiaDiDefault = "T01SA00000";
    CorsiaDiScarto = "T03SA00000";
    /* Ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
     NomeClient = Ini->ReadString("Impostazioni", "NomeClient", "Client001");
     delete Ini; */
    LoadPassword();
    UpdControls();
    Visible = false;
    Screen->Cursor = Save_Cursor;
    bit[0] = 1;
    bit[1] = 2;
    bit[2] = 4;
    bit[3] = 8;
    bit[4] = 16;
    bit[5] = 32;
    bit[6] = 64;
    bit[7] = 128;
    bitAnd[0] = 0xFE;
    bitAnd[1] = 0xFD;
    bitAnd[2] = 0xFB;
    bitAnd[3] = 0xF7;
    bitAnd[4] = 0xEF;
    bitAnd[5] = 0xDF;
    bitAnd[6] = 0xBF;
    bitAnd[7] = 0x7F;
    num_stazioni = 9;
    baiaformSegnali = 0;
    LeggiParametri();
	tab.Load1("IncasGucci.crs");
	f = fopen("IncasGucci.crs", "r");
    if (f == NULL) {

        // exit(-1);

    }
    else {

        fseek(f, 0, SEEK_SET);
        // leggo i valori di altezza e larghezza dell'impianto e il punto d'origine
        while (!feof(f)) {

            fscanf(f, "%s", s);

            if (!strcmp(s, "H")) {

                fscanf(f, "%lf", &fH);

                H = fH;

            }
            else if (!strcmp(s, "W")) {

                fscanf(f, "%lf", &fW);

                W = fW;

            }
            else if (!strcmp(s, "Xo")) {

                fscanf(f, "%lf", &fXo);

                Xo = fXo;

            }
            else if (!strcmp(s, "Yo")) {

                fscanf(f, "%lf", &fYo);

                Yo = fYo;

            }
            else if (!strcmp(s, "FH")) {

                fscanf(f, "%lf", &fFH);

                FormH = fFH;

            }
            else if (!strcmp(s, "FW")) {

                fscanf(f, "%lf", &fFW);

                FormW = fFW;

            }

        }

        /* Xo = (Xo * Image1->Width) / FormW;    //Ricalcolo l'origine in base alla dimensione della finestra
         Yo = (Yo * Image1->Height) / FormH;
         FormH = Image1->Height;
         FormW = Image1->Width; */
    }
    fclose(f);
    poscb = 1;

	PLCThread[0] = new TPLCThread(true, 0);
    PLCThread[0]->Resume();

    // PLC Incas
	PLCThread[0]->dbwatchdog_pc = 400;      // se presente abilito funzione watchdog
	PLCThread[0]->wordwatchdog_pc = 0;
	PLCThread[0]->dbwatchdog_plc = 401;     // 
	PLCThread[0]->wordwatchdog_plc = 0;
    PLCThread[0]->typewatchdog = 1;         // tipo 1 lo associo al pc che copia il watchdog del plc
	PLCThread[0]->num_alarm_watchdog = 203;



	//PLCPhoenixThread[0] = new Phoenix_Thread(1,true);
	//PLCPhoenixThread[0]->Resume();

	DatiAgv[1].num_alarm_consenso = 196;
	corsia_scaffale = 0;
    pos_scaffale = 501;
    piano_scaffale = 1;

    cs = new CLIENTSOCKET(HOST_AGVX_ADDRESS, MISSIONS_PORT);
    sock = new CLIENTSOCKET(HOST_COM_ADDRESS);
    ss = new SUPERVISIONEMISS_STATESOCKET;
    count_inizio = 0;
    init_mappa = 0;
    chiusura_applicazione = 0;
    num_rot_alarm = 4;
    num_porte = 3;
    MisManuale.posprel = 0;
    MisManuale.Source = "";
    MisManuale.tipo_pallet = 0;

	//---------------------------------------
	// ZONA ROBOTS
/*    
	PlcRobot.posvuoti = 95;
	PlcRobot.dbInput = 107;
    PlcRobot.dwByteInput = 1;
    PlcRobot.dbuscite = 106;
    PlcRobot.dwuscite = 1;

    //posizioni delle linee caricamento
	for (i = 1; i <= NLINEE_PREL; i++) 
    {
	    PlcRobot.pos_linea[i] = i*10;
    }

    //IN- presenze sulle linee caricamento
	PlcRobot.addr_presenza_pallet[1] = 0;
	PlcRobot.bit_presenza_pallet[1] = 0;
	PlcRobot.addr_presenza_pallet[2] = 0;
	PlcRobot.bit_presenza_pallet[2] = 4;
	PlcRobot.addr_presenza_pallet[3] = 1;
	PlcRobot.bit_presenza_pallet[3] = 0;
	PlcRobot.addr_presenza_pallet[4] = 1;
	PlcRobot.bit_presenza_pallet[4] = 4;
	PlcRobot.addr_presenza_pallet[5] = 2;
	PlcRobot.bit_presenza_pallet[5] = 0;
	PlcRobot.addr_presenza_pallet[6] = 2;
	PlcRobot.bit_presenza_pallet[6] = 4;
	PlcRobot.addr_presenza_pallet[7] = 3;
	PlcRobot.bit_presenza_pallet[7] = 0;
	PlcRobot.addr_presenza_pallet[8] = 3;
	PlcRobot.bit_presenza_pallet[8] = 4;
	PlcRobot.addr_presenza_pallet[9] = 4;
	PlcRobot.bit_presenza_pallet[9] = 0;
    
	PlcRobot.addr_OK_ingresso_linea[1] = 0;
	PlcRobot.bit_OK_ingresso_linea[1] = 2;
	PlcRobot.addr_OK_ingresso_linea[2] = 0;
	PlcRobot.bit_OK_ingresso_linea[2] = 6;
	PlcRobot.addr_OK_ingresso_linea[3] = 1;
	PlcRobot.bit_OK_ingresso_linea[3] = 2;
	PlcRobot.addr_OK_ingresso_linea[4] = 1;
	PlcRobot.bit_OK_ingresso_linea[4] = 6;
	PlcRobot.addr_OK_ingresso_linea[5] = 2;
	PlcRobot.bit_OK_ingresso_linea[5] = 2;
	PlcRobot.addr_OK_ingresso_linea[6] = 2;
	PlcRobot.bit_OK_ingresso_linea[6] = 6;
	PlcRobot.addr_OK_ingresso_linea[7] = 3;
	PlcRobot.bit_OK_ingresso_linea[7] = 2;
	PlcRobot.addr_OK_ingresso_linea[8] = 3;
	PlcRobot.bit_OK_ingresso_linea[8] = 6;
	PlcRobot.addr_OK_ingresso_linea[9] = 4;
	PlcRobot.bit_OK_ingresso_linea[9] = 2;

	PlcRobot.addr_pallet_incompleto[1] = 0;
	PlcRobot.bit_pallet_incompleto[1] = 3;
	PlcRobot.addr_pallet_incompleto[2] = 0;
	PlcRobot.bit_pallet_incompleto[2] = 7;
	PlcRobot.addr_pallet_incompleto[3] = 1;
	PlcRobot.bit_pallet_incompleto[3] = 3;
	PlcRobot.addr_pallet_incompleto[4] = 1;
	PlcRobot.bit_pallet_incompleto[4] = 7;
	PlcRobot.addr_pallet_incompleto[5] = 2;
	PlcRobot.bit_pallet_incompleto[5] = 3;
	PlcRobot.addr_pallet_incompleto[6] = 2;
	PlcRobot.bit_pallet_incompleto[6] = 7;
	PlcRobot.addr_pallet_incompleto[7] = 3;
	PlcRobot.bit_pallet_incompleto[7] = 3;
	PlcRobot.addr_pallet_incompleto[8] = 3;
	PlcRobot.bit_pallet_incompleto[8] = 7;
	PlcRobot.addr_pallet_incompleto[9] = 4;
	PlcRobot.bit_pallet_incompleto[9] = 3;

	PlcRobot.addr_richiesta_vuoti = 4;
	PlcRobot.bit_richiesta_vuoti = 5;
    
	PlcRobot.addr_OK_ingresso_vuoti = 4;
	PlcRobot.bit_OK_ingresso_vuoti = 6;


    //OUT - richiesta ingresso nelle linee caricamento
	PlcRobot.addr_richiesta_ingresso_linea[1] = 0;
	PlcRobot.bit_richiesta_ingresso_linea[1] = 0;
	PlcRobot.addr_richiesta_ingresso_linea[2] = 0;
	PlcRobot.bit_richiesta_ingresso_linea[2] = 4;
	PlcRobot.addr_richiesta_ingresso_linea[3] = 1;
	PlcRobot.bit_richiesta_ingresso_linea[3] = 0;
	PlcRobot.addr_richiesta_ingresso_linea[4] = 1;
	PlcRobot.bit_richiesta_ingresso_linea[4] = 4;
	PlcRobot.addr_richiesta_ingresso_linea[5] = 2;
	PlcRobot.bit_richiesta_ingresso_linea[5] = 0;
	PlcRobot.addr_richiesta_ingresso_linea[6] = 2;
	PlcRobot.bit_richiesta_ingresso_linea[6] = 4;
	PlcRobot.addr_richiesta_ingresso_linea[7] = 3;
	PlcRobot.bit_richiesta_ingresso_linea[7] = 0;
	PlcRobot.addr_richiesta_ingresso_linea[8] = 3;
	PlcRobot.bit_richiesta_ingresso_linea[8] = 4;
	PlcRobot.addr_richiesta_ingresso_linea[9] = 4;
	PlcRobot.bit_richiesta_ingresso_linea[9] = 0;

    //pallet prelevato dalle linee caricamento
	PlcRobot.addr_pallet_prelevato_linea[1] = 0;
	PlcRobot.bit_pallet_prelevato_linea[1] = 2;
	PlcRobot.addr_pallet_prelevato_linea[2] = 0;
	PlcRobot.bit_pallet_prelevato_linea[2] = 6;
	PlcRobot.addr_pallet_prelevato_linea[3] = 1;
	PlcRobot.bit_pallet_prelevato_linea[3] = 2;
	PlcRobot.addr_pallet_prelevato_linea[4] = 1;
	PlcRobot.bit_pallet_prelevato_linea[4] = 6;
	PlcRobot.addr_pallet_prelevato_linea[5] = 2;
	PlcRobot.bit_pallet_prelevato_linea[5] = 2;
	PlcRobot.addr_pallet_prelevato_linea[6] = 2;
	PlcRobot.bit_pallet_prelevato_linea[6] = 6;
	PlcRobot.addr_pallet_prelevato_linea[7] = 3;
	PlcRobot.bit_pallet_prelevato_linea[7] = 2;
	PlcRobot.addr_pallet_prelevato_linea[8] = 3;
	PlcRobot.bit_pallet_prelevato_linea[8] = 6;
	PlcRobot.addr_pallet_prelevato_linea[9] = 4;
	PlcRobot.bit_pallet_prelevato_linea[9] = 2;

	PlcRobot.addr_richiesta_ingresso_vuoti = 4;
	PlcRobot.bit_richiesta_ingresso_vuoti = 4;

	PlcRobot.addr_vuoto_depositato = 4;
	PlcRobot.bit_vuoto_depositato = 5;
*/

	//---------------------------------------
	// FASCIATORE
    /*
	PlcFascia.posdep = 1 ;

	PlcFascia.dbInput = 107;
	PlcFascia.dwByteInput = 0;
	PlcFascia.dbuscite = 106;
	PlcFascia.dwuscite = 1;     // le mie partono dalla 1

    //IN
	PlcFascia.addr_ChiamataDeposito = 2;
	PlcFascia.bit_ChiamataDeposito = 3;
	
	PlcFascia.addr_PalletAvvolto = 2;
	PlcFascia.bit_PalletAvvolto = 2;

	PlcFascia.addr_OKingresso = 2;
	PlcFascia.bit_OKingresso = 4;
    
    //OUT
	PlcFascia.bit_RichiestaIngresso = 0;
	//PlcFascia.bit_IngombroDepositoAgv = 1;

	PlcFascia.addr_pallet_depositato = 0;
	PlcFascia.bit_pallet_depositato = 1;

	PlcFascia.addr_pallet_prelevato = 0;
	PlcFascia.bit_pallet_prelevato = 2;

    PlcFascia.InizioAttesa = 0;
    */
    
	//---------------------------------------
	// INCAS

	PlcIncas.dbInput = 401;
	PlcIncas.dwByteInput = 0;
	PlcIncas.dbuscite = 400;
	PlcIncas.dwuscite = 0;     // 

    //IN
	//PlcIncas.addr_ChiamataDeposito = 2;
	PlcIncas.bit_richiesta_impegno_incrocio = 0;
	
    //OUT
	PlcIncas.bit_consenso_impegno_incrocio = 0;
    PlcIncas.bit_impegno_inc_rosso = 1;
    PlcIncas.bit_impegno_inc_giallo = 2;
    PlcIncas.bit_impegno_inc_verde = 3;

	//---------------------------------------
    
	pos_mappa_no_db[1] = 0;
	pos_mappa_no_db[2] = 0;
    pos_mappa_no_db[3] = 0;
    num_pos_no_database = 0;
	num_moduli_phoenix = 1;

    AbAvvolgitore->Visible = false;
    PrenotaVuoti->Visible = false;
    Priorita1->Visible = false;
    Priorita2->Visible = false;
    PanelIncrocio->Visible = false;
    PanelSemaforo->Visible = false;

    counterAzzeraUdc = 10;
    
	avanza_pos2 = true ;
	TimerOraTimer(Sender);
    TimerSinottico->Enabled = true;
    lbLastUpd->Caption = "Last start: " + timestr;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::AfterConstruction(void) {
    FFormState = FFormState >> fsVisible;
    TForm::AfterConstruction();
}
// ---------------------------------------------------------------------------

void TMainForm::SetVisible() {
    FFormState << fsVisible;
}
// ---------------------------------------------------------------------------

void TMainForm::SavePassword() {
    FILE *f;
    char s[100] = "";
    int i;

    if (f = fopen("pwd.txt", "w")) {
        for (i = 0; i < PWD.Length(); i++) {
            s[i] = PWD.c_str()[i] + 1 - (i % 3);
        }
        fprintf(f, "%s\n", s);
        fclose(f);
    }
}
// ---------------------------------------------------------------------------

void TMainForm::LoadPassword() {
    FILE *f;
    char s[100] = "";
    int i;

    if (f = fopen("pwd.txt", "r")) {
        fscanf(f, "%s", s);
        PWD = "";
        for (i = 0; i < strlen(s); i++) {
            PWD += (char)(s[i] - 1 + (i % 3));
        }
        fclose(f);
    }
}
// ---------------------------------------------------------------------------

void TMainForm::UpdControls() {
    // bSetup->Enabled = (fMain->pwdlevel > 1);
    MainStatusBar->Refresh();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TimerPwdTimer(TObject * Sender) {
    TimerPwd->Enabled = false;
    pwdlevel = 0;
    UpdControls();
    /* if ((!FindWindowA("TAppBuilder", NULL)) && TouchPanel) {
     LockKeyboard();
     ShowCursor(false);
     } */
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TimerOraTimer(TObject * Sender) {
    TimerOra->Enabled = false;
    format_data_ora_datatbase = "dd/mm/yyyy hh':'mm':'ss";
    timestr = FormatDateTime("dddd d mm yyyy h':'mm':'ss", Now());
    // timestr = dmDB->AggiungiRimuoviSecondiADataOra( DateTimeToStr(Now()),5000,true);
    // dmDB->LeggiSegnalazioneAttiva(msg, all);
    MainStatusBar->Refresh();
    if (MainForm->count_inizio <= 20) {
        if (count_inizio == 1) {
            Image1->Align = alNone;
            Image1->Height = ScrollBox1->Height - 10;
            Image1->Width = ScrollBox1->Width - PanImage->Width - 10;
        }
        else if (count_inizio == 2) {
            zoom = 1;
            Xo = (Xo * Image1->Width) / FormW;
            // Ricalcolo l'origine in base alla dimensione della finestra
            Yo = (Yo * Image1->Height) / FormH;
            FormH = Image1->Height;
            FormW = Image1->Width;
            widthextend = Image1->Width;
            heightextend = Image1->Height;
        }
        else if ((!init_mappa) && (dmDB->lettostatoagv) && (count_inizio > 5)) {
			AggiornaMappa();
			img1->BringToFront();
			Image1->Visible = true;
			ExtendSpeedButtonClick(this);
        }
        if ((count_inizio > 5) && (!Panel2->Enabled) && (dmDB->lettostatoagv))
            Panel2->Enabled = true;

        count_inizio++;
    }
    else {
        // visualizzo stato chiamate e rotalamr
    }
    if ((!MDIChildCount) && (!MainForm->ScrollBox1->Visible)) {
        MainForm->ScrollBox1->Visible = true;
    }
    if (MainForm->ScrollBox1->Visible) {
        if (aggiornamappa)
            AggiornaMappa();
        if (aggiornaagv) {
            DisegnaCarrello(1);
			VisualizzaPercorsoAgv(0);

        }
        aggiornamappa = 0;
        aggiornaagv = 0;
    }
    TimerOra->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bPwdClick(TObject * Sender) {
    int res;
    AnsiString NewPwd;

    PasswordDlg->BitBtn3->Visible = false;
    PasswordDlg->Label1->Caption = "Digita la password";
    PasswordDlg->Password->Text = "";
    res = PasswordDlg->ShowModal();
    if (res == IDOK) {
        if (PasswordDlg->Password->Text == "simula") {
            Simula = true;
            ShowCursor(true);
        }
        else if (PasswordDlg->Password->Text == "proxaut") {
            pwdlevel = 2;
            ShowCursor(true);
        }
        else if (PasswordDlg->Password->Text == PWD)
            pwdlevel = 1;
        if (pwdlevel > 0) {
            // UnlockKeyboard();
            TimerPwd->Enabled = true;
        }
        else {
            ShowMessage("Password errata");
        }
    }
    else if (res == IDYES) {
        if ((PasswordDlg->Password->Text == "proxautman") || (PasswordDlg->Password->Text == PWD)) {
            PasswordDlg->BitBtn3->Visible = false;
            PasswordDlg->Label1->Caption = "Digita la nuova password";
            PasswordDlg->Password->Text = "";
            res = PasswordDlg->ShowModal();
            if (res == IDOK) {
                NewPwd = PasswordDlg->Password->Text;
                PasswordDlg->Label1->Caption = "Conferma la nuova password";
                PasswordDlg->Password->Text = "";
                res = PasswordDlg->ShowModal();
                if (res == IDOK) {
                    if (NewPwd == PasswordDlg->Password->Text) {
                        PWD = NewPwd;
                        SavePassword();
                    }
                    else
                        ShowMessage("Errore nella conferma password");
                }
            }
        }
        else {
            ShowMessage("Password errata");
        }
    }
    else {
        TimerPwdTimer(NULL);
        Simula = false;
    }
    UpdControls();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MainStatusBarDrawPanel(TStatusBar * StatusBar, TStatusPanel * Panel, const System::Types::TRect & Rect) {
    static int n = 0;
    static int n2 = 0;
    static int prev = 0;
    int index_plc_display;
    TCanvas *pCanvas = StatusBar->Canvas;
    int t, l;

    switch (Panel->Index) {
    case 0:
        StatusBar->Panels->Items[0]->Text = timestr;
        pCanvas->Brush->Color = clBtnFace;
        pCanvas->Font->Color = clBlack;
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;
    case 1:
        switch (pwdlevel) {
        case 1:
            n = 1;
            break;
        case 2:
            n = 2;
            break;
        default:
            n = 0;
        }
        l = (Rect.Width() - ImageList1->Width) / 2;
        t = (Rect.Height() - ImageList1->Height) / 2;
        ImageList1->Draw(StatusBar->Canvas, Rect.left + l, Rect.top + t, n);
        break;
    case 2:
        if (AbilitaMissioni) {
            n2 = 1 - n2;
            actual = prev;
            countwatchdog = 0;
        }
        l = (Rect.Width() - ImageList2->Width) / 2;
        t = (Rect.Height() - ImageList2->Height) / 2;
        ImageList2->Draw(StatusBar->Canvas, Rect.left + l, Rect.top + t, n2);
        break;
    /*case 3:
		if (PLCThread[0]->connected) 
        {
            pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
        }
        else 
        {
            if (Simula) 
            {
                pCanvas->Brush->Color = clAqua;
                pCanvas->Font->Color = clBlue;
            }
            else 
            {
                pCanvas->Brush->Color = clRed;
                pCanvas->Font->Color = clWhite;
            }
        }
        
		if (PLCThread[0]->PLCError == 0)
			Panel->Text = "PLC Filmatura WD: Plc " + IntToStr(PLCThread[0]->watchdog_plc) + " ; Pc " + IntToStr(PLCThread[0]->watchdog_pc);
		else
			Panel->Text = "PLC Filmatura " + PLCThread[0]->ErrorString;
            
		pCanvas->FillRect(Rect);
		l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;  */
        
    case 4:
		if (PLCThread[0]->connected) 
        {
            pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
        }
        else
        {
            if (Simula) 
            {
                pCanvas->Brush->Color = clAqua;
                pCanvas->Font->Color = clBlue;
            }
            else 
            {
                pCanvas->Brush->Color = clRed;
                pCanvas->Font->Color = clWhite;
            }
        }
        
		if (PLCThread[0]->PLCError == 0)
	    	Panel->Text = "PLC Incas WD: Plc " + IntToStr(PLCThread[0]->watchdog_plc) + " ; Pc " + IntToStr(PLCThread[0]->watchdog_pc);
		else
			Panel->Text = "PLC Incas " + PLCThread[0]->ErrorString;
            
		pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;
    /*    
	case 5:
		if (PLCThread[2]->connected) 
        {
            pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
        }
        else
        {
            if (Simula) 
            {
                pCanvas->Brush->Color = clAqua;
                pCanvas->Font->Color = clBlue;
            }
            else 
            {
                pCanvas->Brush->Color = clRed;
                pCanvas->Font->Color = clWhite;
            }
        }
        
		if (PLCThread[2]->PLCError == 0)
	    	Panel->Text = "PLC Area Robot WD: Plc " + IntToStr(PLCThread[2]->watchdog_plc) + " ; Pc " + IntToStr(PLCThread[2]->watchdog_pc);
		else
			Panel->Text = "PLC Area Robot " + PLCThread[2]->ErrorString;
            
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;
    */    
	case 5:
        if (dmDB->ADOConnection1->Connected) { // per oranon esiste, utilizzo il mio database
            lbDB->Caption = "Local: " + dmDB->ADOConnection1->ConnectionString.SubString(AnsiPos("Source", dmDB->ADOConnection1->ConnectionString) + 7, 15);
            lbDB->Color = clWindowFrame;
            // lbDB->Caption=  dmDB->ADOConnection1->ConnectionString.SubString(AnsiPos("Source",dmDB->ADOConnection1->ConnectionString)+7,AnsiPos(";",dmDB->ADOConnection1->ConnectionString.SubString(AnsiPos("Source",dmDB->ADOConnection1->ConnectionString)-AnsiPos("Source",dmDB->ADOConnection1->ConnectionString)))) ;
            pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
        }
        else {
			lbDB->Caption = "DB ERROR";
			lbDB->Color = clRed;
            pCanvas->Brush->Color = clRed;
            pCanvas->Font->Color = clWhite;
        }
        pCanvas->FillRect(Rect);
		l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;
   /*	case 7:
		if (dmDB->FMConnection->Connected) { // per oranon esiste, utilizzo il mio database
			lbAS400->Caption = "FMVision: " + dmDB->FMConnection->ConnectionString.SubString(AnsiPos("Source", dmDB->ADOConnection1->ConnectionString) + 7, 15);
			lbAS400->Color = clWindowFrame;
			pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
        }
        else {
			lbAS400->Caption = "FMVision: DB ERROR ";
			lbAS400->Color = clRed;
			pCanvas->Brush->Color = clRed;
			pCanvas->Font->Color = clWhite;
        }
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;   */
	case 6:
        if (AbilitaMissioni) {
            pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
            Panel->Text = "Generazione Missioni Abilitato";
        }
        else {
            pCanvas->Brush->Color = clRed;
            pCanvas->Font->Color = clWhite;
            Panel->Text = "Generazione Missioni Disabilitato";
        }
        pCanvas->FillRect(Rect);
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + 2, Rect.top + t, Panel->Text);
        break;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Image4Click(TObject * Sender) {
    if (BorderStyle == bsNone) {
        BorderStyle = bsSizeable;
    }
    else {
        BorderStyle = bsNone;
        ShowWindow(Handle, SW_MAXIMIZE);
        ShowWindow(Handle, SW_MAXIMIZE);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bStoriciClick(TObject * Sender) {

    CreateMDIChild(fAllarmi, "fAllarmi", "Storico Allarmi");
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bStatoAGVClick(TObject * Sender) {
    CreateMDIChild(fAGV, "fAGV", "Stato AGV");
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bSetupClick(TObject * Sender) {
    CreateMDIChild(frConfig, "frConfig", "Setup");
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void TMainForm::ConvertiStringaDisplay(AnsiString stringa, int indice_inizio_registro, int indice_display) {
    // converto la stringa all'interno del registro
    int i, indice_str, word, testbyte;
    char codice[50], carattere;
    AnsiString byte_low, byte_hight;
    indice_str = 0;
    strcpy(codice, stringa.c_str());
    for (i = 0; i < 20; i++) {
        if (indice_str < stringa.Length()) {
            byte_hight.sprintf("%d", codice[indice_str]);
        }
        else
            byte_hight.sprintf("%d", '\0');
        indice_str++;
        if (indice_str <= stringa.Length()) {
            byte_low.sprintf("%d", codice[indice_str]);
        }
        else
            byte_low.sprintf("%d", '\0');
        indice_str++;
        word = (byte_hight.ToIntDef(0) * 256) + byte_low.ToIntDef(0);
        // MainForm->DatiDisplay[indice_display].RegistriVariabili[indice_inizio_registro+i] = word ;

    }
}

void __fastcall TMainForm::bMissioniClick(TObject * Sender) {
    CreateMDIChild(fMissioni, "fMissioni", "Storico Missioni");

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject * Sender, TCloseAction & Action) {
    int i;
    dmDB->LogMsg("Disconnessione Supervisore");
    dmDB->AggiornaStatoAgv(0);
    delete ss;
    delete cs;
    delete sock;
    dmDB->LogMsg("Supervisore Chiuso");
}

// ---------------------------------------------------------------------------

int TMainForm::GetPrenotataFromPos(int pos) 
{
    IndexListIterator i;
    int res = 0; // ricerco se una delle due possibili pos è prenotata

    for (i = dmDB->TabCustomerPos.begin(); i != dmDB->TabCustomerPos.end(); i++) 
    {
        if (i->second["POS"].ToIntDef(0) == pos) 
        {
            if (i->second["PRENOTATA"].ToIntDef(0))
            {   
                res = i->second["PRENOTATA"].ToIntDef(0);
                break;
            }
        }
    }

    return res;
}

// ---------------------------------------------------------------------------

void TMainForm::AggiornaMappa() 
{
    AnsiString stringa, stringa2, str;
    double x, y, posx, posy, xselezione1, xselezione2, yselezione1, yselezione2;
    int imgw, imgh, countrecord, changestep, nobarra, escludi = 0, prenotata = 0, cellaTop, cellaLeft, i;
    int pos, piano, idimg, pianimax, tipo_pallet;
    int numpallet = 0;
    TMyShape *PosCella;
    /* TLabel *Lab ;
     TTime TempoDisegnaMappa ; */
    // ridisegnapalsel = 0 ;
    // stringa = "Select * From Posizioni inner join Corsie on Posizioni.Fila = Corsie.Fila order by Corsie.Fila,Pos,Piano";
    // stringa = "Select * From Corsie inner join Posizioni on Corsie.Fila = Posizioni.Fila inner join Udc on ( Posizioni.IdArticolo1 = Udc.IdArticolo OR  Posizioni.IdArticolo2 = Udc.IdArticolo OR Posizioni.IdArticolo3 = Udc.IdArticolo OR  Posizioni.IdArticolo4 = Udc.IdArticolo ) order by Posizioni.Fila,Pos";

    //Aggiorna la tabella in memoria che verrà usata per il disegno (per non accedere sempre al db)
    dmDB->UpdateCorsielist();
    //return ;
	stringa = "Select * from Posizioni order by Pos";
    if (dmDB->ADOConnection1->Connected) 
    {
        /* if (program == 1)
         TempoDisegnaMappa = Time(); */
        /* if ((y1sel) &&(y2sel)&&(y1sel != y2sel)&&(x1sel) &&(x2sel)&&(x1sel!=x2sel)) {
         //no barra
         nobarra = 1 ;
         }else */
        nobarra = 0;
        if ((!nobarra) && (PaintBox1 != NULL)) {
            delete PaintBox1;
            PaintBox1 = NULL;
        }
        else if ((Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
            PaintBox1->Canvas->Pen->Mode = pmNot;
            PaintBox1->Canvas->Pen->Color = clWhite;
            PaintBox1->Canvas->Brush->Style = bsClear;
            PaintBox1->Canvas->Rectangle(x1sel, y1sel, x2sel, y2sel);
        }
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();

            countrecord = 1;
            if (dmDB->QueryPos->RecordCount > 0) 
            {
                dmDB->QueryPos->First();
                while (!dmDB->QueryPos->Eof) 
                {
                    idimg = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
					if (dmDB->QueryPos->FieldByName("Disabilita")->AsInteger || !dmDB->GetAbilitataIfCorsia(dmDB->QueryPos->FieldByName("Fila")->AsInteger))
                        escludi = 1;
                        
					//if (dmDB->QueryPos->FieldByName("Prenotata")->AsInteger)
                    //    prenotata = 1;
                    if (GetPrenotataFromPos(dmDB->QueryPos->FieldByName("Pos")->AsInteger)) 
                        prenotata = 1;

					tipo_pallet = dmDB->QueryPos->FieldByName("TipoPallet")->AsInteger;
					numpallet = dmDB->QueryPos->FieldByName("IdUdc")->AsInteger;
					PosCella = (TMyShape*) ScrollBox1->FindComponent("Pos" + dmDB->QueryPos->FieldByName("Pos")->AsString);
                    if (PosCella == NULL) {
                        // Lab->SendToBack();
                        PosCella = new TMyShape(ScrollBox1);
                        PosCella->Parent = ScrollBox1;
						PosCella->Name = "Pos" + dmDB->QueryPos->FieldByName("Pos")->AsString;
						PosCella->Hint = "Pos " + dmDB->QueryPos->FieldByName("CustomerPos")->AsString + " - " + dmDB->GetTipologiaCorsiaString(dmDB->QueryPos->FieldByName("Fila")->AsInteger);
                        PosCella->Shape = mstRectangle;
                        // PosCella->Text = IntToStr(idimg) ;
                        PosCella->Tag = idimg;
                        PosCella->Font->Name = "Arial Narrow";
                        PosCella->Font->Size = 8;
                        PosCella->ShowHint = true;
                        PosCella->OnMouseMove = ShapePosMouseMove;
						PosCella->OnMouseUp = ShapePosMouseUp;
						PosCella->Brush->Style = bsClear ;
                        // MappaPosizioni[idimg] = PosCella ;

                    }
                    else {
                        PosCella->Hint = "Pos " + dmDB->QueryPos->FieldByName("CustomerPos")->AsString; 

                    }
                    if (PosCella != NULL) {
                        imgw = dmDB->QueryPos->FieldByName("Width")->AsInteger;
                        imgh = dmDB->QueryPos->FieldByName("Height")->AsInteger;
                        /*if (zoom <= 1) 
                        {
                            imgw = 16;
                            imgh = 16;
                        }
                        else if (zoom > 1) 
                        {
                            imgw = 14;
                            imgh = 14;
                        } */
                        if ((imgw * zoom) != PosCella->Width) {
                            PosCella->Width = imgw * zoom;
                            PosCella->Height = imgh * zoom;
                        }
                        if (zoom == 1)
                            PosCella->Font->Size = 6;
                        else if (zoom == 2)
                            PosCella->Font->Size = 8;
                        else if (zoom > 2)
                            PosCella->Font->Size = 10;
                        pos = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                        if (tab.mappa_nodi.find(pos) != tab.mappa_nodi.end()) {
                            x = Xo + ((Image1->Width * tab.mappa_nodi[pos].posx) / W) + Image1->Left;
                            y = Yo - ((Image1->Height * tab.mappa_nodi[pos].posy) / H) + Image1->Top;

                            cellaTop = y - PosCella->Height / 2;
                            cellaLeft = x - PosCella->Width / 2;
                            if (tab.mappa_nodi[pos].ang == 90)
                                cellaTop = cellaTop + 10;
                            if ((cellaTop != PosCella->Top) || (cellaLeft != PosCella->Left)) {
                                PosCella->Top = cellaTop;
                                PosCella->Left = cellaLeft;
                            }
                        }
                        else
							PosCella->Visible = false;

						if (!dmDB->QueryPos->FieldByName("PosInputPlc")->AsInteger) {
							if (!numpallet)
								PosCella->Brush->Style = bsClear;
							else if (numpallet  &&
									 dmDB->QueryPos->FieldByName("Pos")->AsString != POS_PREL_FALDE &&
									 dmDB->QueryPos->FieldByName("Pos")->AsString != POS_PREL_VUOTI)
								PosCella->Brush->Color = clAqua;
                        }
						if (dmDB->QueryPos->FieldByName("TipoPosizione")->AsInteger == DEF_TYPE )
						{
							 if (dmDB->QueryPos->FieldByName("PresenzaPallet")->AsInteger &&
								 !dmDB->QueryPos->FieldByName("IdUdc")->AsInteger)
                             {
                                 PosCella->Brush->Color = clAqua;
                             }
                             //con tabella FM
							 else if (dmDB->QueryPos->FieldByName("IdUdc")->AsInteger)
								PosCella->Brush->Color = clAqua;
                             // VUOTI
                             else
								PosCella->Brush->Style = bsClear;
						}

                        if (idimg == PlcFascia.posdep) //Avvolgitore
                        {
                            AbAvvolgitore->Visible = true;
                            AbAvvolgitore->Top = PosCella->Top - 25;
                            AbAvvolgitore->Left = PosCella->Left + 10;
                        }
                        
                        if (idimg == CARICO3) //SEMAFORO SU : Posizione carico 3 - vicino a dove c'è l'incrocio
                        {
                            //PanelIncrocio->Visible = true;
                            //PanelIncrocio->Top = PosCella->Top + 10;
                            //PanelIncrocio->Left = PosCella->Left + 40;
                            
                            //PanelSemaforo->Visible = true;
                            //PanelSemaforo->Top = PosCella->Top;
                            //PanelSemaforo->Left = PosCella->Left + 30;
                        } 
                        
                        if (idimg == SCARICO1) //SEMAFORO SU : In mezzo al percorso tra CAR e SCAR
                        {
                            PanelSemaforo->Visible = true;
                            PanelSemaforo->Top = PosCella->Top;
                            PanelSemaforo->Left = PosCella->Left + 410;
                        }
                        
						if (!dmDB->QueryPos->FieldByName("Pos")->AsInteger == POS_PREL_FALDE &&
							!dmDB->QueryPos->FieldByName("Pos")->AsInteger == POS_PREL_VUOTI &&
                            !dmDB->QueryPos->FieldByName("PosInputPlc")->AsInteger)
						{
						    PosCella->Text = IntToStr(numpallet);
                        }
                        if (prenotata) {
                            PosCella->Pen->Color = clBlue;
                            PosCella->Pen->Width = 3;
                        }
                        else if (escludi) {
                            PosCella->Pen->Color = clRed;
                            PosCella->Pen->Width = 3;
                        }
                        else if (tipo_pallet == 1) {
                            PosCella->Pen->Color = clGreen;
                            PosCella->Pen->Width = 3;
                        }
                        else if (tipo_pallet == 2) {
                            PosCella->Pen->Color = clPurple;
                            PosCella->Pen->Width = 3;
                        }
                        else if (tipo_pallet == 3) {
                            PosCella->Pen->Color = clLime;
                            PosCella->Pen->Width = 3;
                        }
                        else if (tipo_pallet == 4) {
                            PosCella->Pen->Color = clSilver;
                            PosCella->Pen->Width = 3;
                        }
                        else if (tipo_pallet == 10) {
                            PosCella->Pen->Color = 0x0007AFF8;
                            PosCella->Pen->Width = 3;
                        }
                        else {
                            PosCella->Pen->Color = clWhite;
                            PosCella->Pen->Width = 1;
                        }

                        if (x1sel > x2sel) {
                            xselezione1 = x2sel + Image1->Left;
                            xselezione2 = x1sel + Image1->Left;
                        }
                        else {
                            xselezione1 = x1sel + Image1->Left;
                            xselezione2 = x2sel + Image1->Left;
                        }
                        if (y1sel > y2sel) {
                            yselezione1 = y2sel + Image1->Top;
                            yselezione2 = y1sel + Image1->Top;
                        }
                        else {
                            yselezione1 = y1sel + Image1->Top;
                            yselezione2 = y2sel + Image1->Top;
                        }
                        if ((PosCella->Top >= yselezione1) && (PosCella->Top <= yselezione2) && (PosCella->Left >= xselezione1) && (PosCella->Left <= xselezione2)) {
                            // if ((y >= yselezione1) &&(y <= yselezione2)&&(x >= xselezione1) &&(x <= xselezione2)) {
                            // sono dentro la selezione
                            if (PosCella->Pen->Color != clSilver) {
                                PosCella->Pen->Color = clSilver;
                                PosCella->Pen->Width = 3;
                                ridisegnapalsel = 1;
                            }
                        }
                        else if (PosCella->Pen->Color == clSilver) {
                            PosCella->Pen->Color = clBlack;
                            PosCella->Pen->Width = 1;
                            ridisegnapalsel = 1;
                        }

                    }
                    numpallet = 0;
                    escludi = 0;
                    prenotata = 0;
                    dmDB->QueryPos->Next();
                }
            }
            
            if (EscludiAvvolgitore)
                AbAvvolgitore->Brush->Color = clRed;
            else
                AbAvvolgitore->Brush->Style = bsClear;
            
            if (PrioritaMagazzino)
            {
                Priorita1->Brush->Color = clLime;
                Priorita2->Brush->Color = clLime;
            }
            else
            {
                Priorita1->Brush->Style = bsClear;
                Priorita2->Brush->Style = bsClear;
            }
            
            dmDB->QueryPos->Close();
            init_mappa = 1;
            if (PaintBox1 == NULL) {
                PaintBox1 = new TPaintBox(ScrollBox1);
                PaintBox1->Parent = ScrollBox1;
                PaintBox1->OnMouseDown = Image1->OnMouseDown;
                PaintBox1->OnMouseMove = Image1->OnMouseMove;
                PaintBox1->OnDragOver = Image1->OnDragOver;
                PaintBox1->SendToBack();
                PaintBox1->Align = alClient;
            }
            x1sel = 0;
            x2sel = 0;
            y1sel = 0;
            y2sel = 0;
            // ScrollBox1->Show();

        }
        catch (...) {
        }
    }
}

void __fastcall TMainForm::ShapePosMouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TMyShape *Pos;
    // apro il popup
    int tipopos, i, popupok = 0, idplc, presenza=0;
    PopupMenuPos->Items->Clear();
    Pos = (TMyShape*) Sender;
	tipopos = dmDB->RitornaTipoPos(Pos->Tag);
    presenza = dmDB->RitornaPresenza(Pos->Tag);
    
	if (Pos != NULL) {
        if (Button == mbRight) {
            if ((mousedown) && (x1 != x2) && (y1 != y2) && (Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
                if (Pos != NULL) {
                    X = Pos->Left + X;
                    Y = Pos->Top + Y;
                    Image1MouseUp(Sender, Button, Shift, X, Y);
                }
            }
            else {
                if (agvdaspostare) {
                    TMenuItem *NewItem1 = new TMenuItem(this);
                    // create the separator
                    NewItem1->Caption = "Muovere Lgv " + IntToStr(agvdaspostare) + " in Pos " + IntToStr(Pos->Tag) + " ???";
                    NewItem1->Tag = Pos->Tag;
                    NewItem1->OnClick = MoveAgv1Click;
                    PopupMenuPos->Items->Add(NewItem1);
                    if (DatiAgv[agvdaspostare].load) {
                        TMenuItem *NewItem2 = new TMenuItem(this);
                        // create the separator
                        NewItem2->Caption = "Muovere Lgv " + IntToStr(agvdaspostare) + " in Pos " + IntToStr(Pos->Tag) + " per depositare il pallet ???";
                        NewItem2->Tag = Pos->Tag;
                        NewItem2->OnClick = DropAgv1Click;
                        PopupMenuPos->Items->Add(NewItem2);
                    }
                    popupok = 1;
                }
                else if ((MisManuale.posprel) && (!presenza)) 
                {
                   /* TMenuItem *NewItem1 = new TMenuItem(this);
					// create the separator
					NewItem1->Caption = "Creare Missione di Deposito Pallet in Posizione " + IntToStr(Pos->Tag) + " ???";
                    NewItem1->Tag = Pos->Tag;
                    NewItem1->OnClick = DropPal1Click;
                    PopupMenuPos->Items->Add(NewItem1);     */

                }
                else if (!MisManuale.posprel) {
                    /* vedere in che postazioni abilitarlo
                     TMenuItem *NewItem1 = new TMenuItem(this);
                     // create the separator
                     NewItem1->Caption = "Creare Missione di prelievo in posizione "+dmDB->NomeCorsiaDaPosizione(Pos->Tag)+" ???"  ;
                     NewItem1->Tag = Pos->Tag;
                     NewItem1->OnClick = PrelPal1Click;
                     PopupMenuPos->Items->Add(NewItem1); */
                }
                TMenuItem *NewItem2 = new TMenuItem(this);
                // create the separator
                NewItem2->Caption = "Disabilita Posizioni";
                NewItem2->Tag = Pos->Tag;
                NewItem2->OnClick = ExcludePosition1Click;
                PopupMenuPos->Items->Add(NewItem2);
                TMenuItem *NewItem3 = new TMenuItem(this);
                // create the separator
                NewItem3->Caption = "Abilita Posizioni";
                NewItem3->Tag = Pos->Tag;
                NewItem3->OnClick = RemoveExclusion1Click;
                PopupMenuPos->Items->Add(NewItem3);

                PopupMenuPos->Items->InsertNewLineAfter(NewItem3);
                
                if (tipopos == LINEACAR_TYPE)
                {
                    TMenuItem *NewItem7 = new TMenuItem(this);
                    // create the separator
                    NewItem7->Caption = "Carica Pallet da Piano 1";
                    NewItem7->Tag = Pos->Tag;
                    NewItem7->OnClick = CaricaPalletDaPiano1;
                    PopupMenuPos->Items->Add(NewItem7);
                    
                    TMenuItem *NewItem8 = new TMenuItem(this);
                    // create the separator
                    NewItem8->Caption = "Carica Pallet da Piano 2";
                    NewItem8->Tag = Pos->Tag;
                    NewItem8->OnClick = CaricaPalletDaPiano2;
                    PopupMenuPos->Items->Add(NewItem8);
                    
                }
                if ((tipopos == LINEASCAR_TYPE) && (MisManuale.posprel) && (!presenza))
                {
                    TMenuItem *NewItem8 = new TMenuItem(this);
					// create the separator
					NewItem8->Caption = "Creare Missione di Deposito Pallet in Posizione " + IntToStr(Pos->Tag) + " ???";
                    NewItem8->Tag = Pos->Tag;
                    NewItem8->OnClick = DropPal1Click;
                    PopupMenuPos->Items->Add(NewItem8); 
                }
				popupok = 1;

                if (popupok) {
                    PopupMenuPos->Tag = Pos->Tag;
                    PopupMenuPos->Popup(Left + X + Pos->Left, Top + Y + Pos->Top + Panel2->Height + Image5->Height);
                }
            }

		}
        else if (Button == mbLeft) 
        {
            if (Pos->Tag) 
            {
                // posizioni di magazzino
                if (tipopos == DEF_TYPE)
                {
                    pos_scaffale = Pos->Tag;
	                corsia_scaffale = dmDB->ReturnFilaDaPos(Pos->Tag);
				    CreateMDIChild(fCorsia, "fCorsia", "Stato Corsia");
                }
                //scarico
                else if (tipopos == LINEASCAR_TYPE)
                {
					FormPlcCarico->m_nType = tipopos;
                    FormPlcCarico->pos = Pos->Tag;
                    FormPlcCarico->ShowModal();
                }
                //deposito filmatore
                else if (tipopos == AVVOLGITORE_TYPE)
                {
                    FormBaiaScarico->ShowModal();
                }
                //robot
                else if (tipopos == LINEACAR_TYPE)
                {
					FormPlcCarico->m_nType = tipopos;
                    FormPlcCarico->pos = Pos->Tag;
                    FormPlcCarico->ShowModal();
                }
			}
            agvdaspostare = 0;
            // spostaframe = 0 ;
			// ridisegnapalsel = 0;
        }
    }
}

void __fastcall TMainForm::ShapePulsantieraMouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TMyShape *Pos;
    // apro il popup
    int tipopos, i, popupok = 0, idplc;
    PopupMenuPos->Items->Clear();
    Pos = (TMyShape*) Sender;
    if (Pos != NULL) {
        if (Button == mbRight) {
            if ((mousedown) && (x1 != x2) && (y1 != y2) && (Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
                if (Pos != NULL) {
                    X = Pos->Left + X;
                    Y = Pos->Top + Y;
                    Image1MouseUp(Sender, Button, Shift, X, Y);
                }
            }
            else {
                if (agvdaspostare) {
                    TMenuItem *NewItem1 = new TMenuItem(this);
                    // create the separator
					NewItem1->Caption = "Muovere Lgv " + IntToStr(agvdaspostare) + " in pos " + IntToStr(Pos->Tag) +" ???";
					NewItem1->Tag = Pos->Tag;
                    NewItem1->OnClick = MoveAgv1Click;
                    PopupMenuPos->Items->Add(NewItem1);
                    if (DatiAgv[agvdaspostare].load) {
						TMenuItem *NewItem2 = new TMenuItem(this);
						// create the separator
						NewItem2->Caption = "Muovere Lgv " + IntToStr(agvdaspostare) + " in pos " + IntToStr(Pos->Tag) + " per depositare il Pallet ???";
                        NewItem2->Tag = Pos->Tag;
                        NewItem2->OnClick = DropAgv1Click;
                        PopupMenuPos->Items->Add(NewItem2);
                    }
                    popupok = 1;
                }
				else if (MisManuale.posprel) {
                    TMenuItem *NewItem1 = new TMenuItem(this);
                    // create the separator
					NewItem1->Caption = "Creare Missione di Deposito su pos  " + IntToStr(Pos->Tag) + " ???";
                    NewItem1->Tag = Pos->Tag;
                    NewItem1->OnClick = DropPal1Click;
                    PopupMenuPos->Items->Add(NewItem1);

                }
                else if (!MisManuale.posprel) {
                    TMenuItem *NewItem1 = new TMenuItem(this);
                    // create the separator
					NewItem1->Caption = "Creare Missione di prelievo in posizione " + IntToStr(Pos->Tag) + " ???";
                    NewItem1->Tag = Pos->Tag;
                    NewItem1->OnClick = PrelPal1Click;
                    PopupMenuPos->Items->Add(NewItem1);

                }
                TMenuItem *NewItem2 = new TMenuItem(this);
                // create the separator
				NewItem2->Caption = "Disabilita Posizione";
                NewItem2->Tag = Pos->Tag;
                NewItem2->OnClick = ExcludePosition1Click;
                PopupMenuPos->Items->Add(NewItem2);
                TMenuItem *NewItem3 = new TMenuItem(this);
                // create the separator
				NewItem3->Caption = "Abilita Posizione";
                NewItem3->Tag = Pos->Tag;
                NewItem3->OnClick = RemoveExclusion1Click;
                PopupMenuPos->Items->Add(NewItem3);
                popupok = 1;

                if (popupok) {
                    PopupMenuPos->Tag = Pos->Tag;
                    PopupMenuPos->Popup(Left + X + Pos->Left, Top + Y + Pos->Top + Panel2->Height + Image5->Height);
                }
            }

        }
        else if (Button == mbLeft) {
            if (Pos->Tag) {
                //FormDatiPressa->id_pulsantiera = Pos->Tag;
                //FormDatiPressa->Show();
            }
            agvdaspostare = 0;
            // spostaframe = 0 ;
            // ridisegnapalsel = 0;
        }
    }
}

void __fastcall TMainForm::ShapePosMouseMove(TObject * Sender, TShiftState Shift, int X, int Y) {
    //
    TMyShape *Shape;
    Shape = (TMyShape*) Sender;
    if ((Shape != NULL) && (Shape->Tag)) {
        if (Shape->Tag != ultimapospassata) {
            // ultimapospassata = Shape->Tag ;
            // AggiornaDatiPos(ultimapospassata) ;
        }
    }
}

void __fastcall TMainForm::sSinotticoClick(TObject * Sender) 
{
    int i;
    for (i = MDIChildCount - 1; i >= 0; i--) {
        if (MDIChildren[i]->Name != Name) {
            ((TMDIChild*)MDIChildren[i])->SetInvisible();
            MDIChildren[i]->Close();
        }
    }
    AggiornaMappa();
    // CreateMDIChild(fCorsia, "fCorsia", "Stato Corsia");
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Image1MouseDown(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    if (Button == mbMiddle) {
        xmouse = 0;
        ymouse = 0;
        countmousemove = 0;
        Screen->Cursor = crHandPoint;
        Screen->Tag = 0;
    }
    else if (Button == mbLeft) {
        retsel = false;
        x1sel = X;
        y1sel = Y;
        x2sel = x1sel;
        y2sel = y1sel;
        mousedownsel = true;
        Contatoresel = 0;
    }
    else if ((Button == mbRight) && (zoom < 6)) {
        ret = false;
        x1 = X;
        y1 = Y;
        x2 = x1;
        y2 = y1;
        mousedown = true;
        Contatore = 0;
    }
    else if ((Button == mbRight) && (zoom >= 6)) {
        ExtendSpeedButtonClick(this);
    }

}
// ---------------------------------------------------------------------------


void __fastcall TMainForm::Image1MouseMove(TObject * Sender, TShiftState Shift, int X, int Y) {
    TShiftState app, appleft, appright;
    TImage *CellaImg;
    AnsiString str;
    TLabel *Lab;
    app << ssMiddle;
    TImage *Img;
    TRotateImage *MyImg;
    TMyShape *MyPos;
    if ((Shift == app) && (Screen->Cursor == crHandPoint)) {
        if (String(Sender->ClassName()) == "TMyShape") {
            MyPos = (TMyShape*) Sender;
            if (MyPos != NULL) {
                X = MyPos->Left + X;
                Y = MyPos->Top + Y;
            }
            // cerco l'immagine
        }
        if (xmouse == 0) {
            xmouse = X;
            ymouse = Y;
            return;
        }
        if ((countmousemove >= 1) || (ymouse == 0)) {
            // Image1->Hide();
            if (xmouse < X)
                ScrollBox1->HorzScrollBar->Position -= abs(X - xmouse);
            else if (xmouse > X)
                ScrollBox1->HorzScrollBar->Position += abs(X - xmouse);
            if (ymouse < Y)
                ScrollBox1->VertScrollBar->Position -= abs(Y - ymouse);
            else if (ymouse > Y)
                ScrollBox1->VertScrollBar->Position += abs(Y - ymouse);
            countmousemove = 0;
            // Image1->Show();
        }
        else
            countmousemove++;
    }
    else {
        if (String(Sender->ClassName()) == "TMyShape") {
            MyPos = (TMyShape*) Sender;
            if (MyPos != NULL) {
                X = MyPos->Left + X;
                Y = MyPos->Top + Y;
            }
            // cerco l'immagine
        }
        else if (String(Sender->ClassName()) == "TRotateImage") {
            MyImg = (TRotateImage*) Sender;
            if (MyImg != NULL) {
                X = MyImg->Left + X;
                Y = MyImg->Top + Y;
            }
            // cerco l'immagine
        }
        appleft << ssLeft;
        if (Shift == appleft) {
            if ((Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
                PaintBox1->Canvas->Pen->Mode = pmNot;
                PaintBox1->Canvas->Pen->Color = clWhite;
                PaintBox1->Canvas->Brush->Style = bsClear;
                PaintBox1->Canvas->Rectangle(x1sel, y1sel, x2sel, y2sel);
            }
            if (PaintBox1 != NULL) {
                PaintBox1->Canvas->Pen->Style = psDot;
                PaintBox1->Canvas->Pen->Color = clBlack;
                PaintBox1->Canvas->Brush->Style = bsClear;
                PaintBox1->Canvas->Rectangle(x1sel, y1sel, X, Y);
                x2sel = X;
                y2sel = Y;
                ret = true;
                Contatore++;
            }
        }
        appright << ssRight;
        if ((Shift == appright) && (zoom < 2)) {
            if ((Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
                PaintBox1->Canvas->Pen->Mode = pmNot;
                PaintBox1->Canvas->Pen->Color = clWhite;
                PaintBox1->Canvas->Brush->Style = bsClear;
                PaintBox1->Canvas->Rectangle(x1, y1, x2, y2);
            }
            if (PaintBox1 != NULL) {
                PaintBox1->Canvas->Pen->Style = psDot;
                PaintBox1->Canvas->Pen->Color = clBlack;
                PaintBox1->Canvas->Brush->Style = bsClear;
                PaintBox1->Canvas->Rectangle(x1, y1, X, Y);
                x2 = X;
                y2 = Y;
                ret = true;
                Contatore++;
            }
        }

    }
    QuotaX->Caption = " X : " + IntToStr(X);
    QuotaY->Caption = " Y : " + IntToStr(Y);
    ultimapospassata = 0;

}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ExcludePosition1Click(TObject * Sender) {
    int escludi;
    escludi = bit[0] | bit[1] | bit[2];
    EscludiPosSelezionate(PopupMenuPos->Tag, escludi);
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::RemoveExclusion1Click(TObject * Sender) {
    EscludiPosSelezionate(PopupMenuPos->Tag, 0);
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
}

void __fastcall TMainForm::ZoomInSpeedButtonClick(TObject * Sender) {
    TShiftState Shift;
    bool Hand;
    TPoint MP;
    MP.x = 700; // centro il mouse
    MP.y = 450;
    countwheel = 3;
    FormMouseWheel(this, Shift, 130, MP, Hand);

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ExtendSpeedButtonClick(TObject * Sender) {
    TShiftState Shift;
    bool Hand;
    TTime tempo;
    TPoint MP;
    tempo = Time() - StrToTime("00:00:01");
    if (tempo > ScrollMouse) {
        MP.x = 700; // centro il mouse
        MP.y = 450;
        countwheel = 3;
        Image1->Width = widthextend * 2;
        Image1->Height = heightextend * 2;
        zoom = 2;
        FormMouseWheel(this, Shift, -130, MP, Hand);
    }
    else
        MainForm->AggiornaMappa();

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ZoomOutSpeedButtonClick(TObject * Sender) {
    TShiftState Shift;
    bool Hand;
    TPoint MP;
    MP.x = 700; // centro il mouse
    MP.y = 450;
    countwheel = 3;
    FormMouseWheel(this, Shift, -130, MP, Hand);

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::img2MouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TRotateImage *Img;
    int pos_sel,tipopos = 0;
    Img = (TRotateImage*) Sender;
    PopupMenu1->Items->Clear();
    if ((Button == mbRight) && (Img != NULL)) {
        /* if ((!DatiAgv[Img->Tag].manuale)&&(DatiAgv[Img->Tag].idmis)&&(DatiAgv[Img->Tag].pos > 0)) {
         //abilitare quando è pronta sull'agv
         TMenuItem *NewItem = new TMenuItem(this); // create the separator
         NewItem->Caption = "Abort Mission on Agv n° "+IntToStr(Img->Tag)+" !!!";
         NewItem->Tag = Img->Tag ; //mi salvo l'id dell'agv
         NewItem->OnClick = Aborted1Click;
         PopupMenu1->Items->Add(NewItem);
         } else */
        if ((!DatiAgv[Img->Tag].manuale) && (!DatiAgv[Img->Tag].alarm) && (DatiAgv[Img->Tag].pos > 0) && (!DatiAgv[Img->Tag].load) && (!DatiAgv[Img->Tag].idmis)) {
            // abilitare quando è pronta sull'agv
            TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
            NewItem1->Caption = "Crea Missione di movimento per AGV n° " + IntToStr(Img->Tag) + " !";
            NewItem1->Tag = Img->Tag; // mi salvo l'id dell'agv
            NewItem1->OnClick = MoveMis1Click;
            PopupMenu1->Items->Add(NewItem1);
        }
        else if ((!DatiAgv[Img->Tag].manuale) && (pwdlevel) && (!DatiAgv[Img->Tag].alarm) && (DatiAgv[Img->Tag].pos > 0) && (DatiAgv[Img->Tag].load) && (!DatiAgv[Img->Tag].idmis)) {
            // abilitare quando è pronta sull'agv
            TMenuItem *NewItem2 = new TMenuItem(this); // create the separator
            NewItem2->Caption = "Crea Missione di deposito per AGV n° " + IntToStr(Img->Tag) + " !";
            NewItem2->Tag = Img->Tag; // mi salvo l'id dell'agv
            NewItem2->OnClick = DropMis1Click;
            PopupMenu1->Items->Add(NewItem2);
        }
        if ((!DatiAgv[Img->Tag].manuale) && ((!DatiAgv[Img->Tag].alarm) || (DatiAgv[Img->Tag].stand_by)) && (DatiAgv[Img->Tag].pos > 0)) {
            // abilitare quando è pronta sull'agv
            TMenuItem *NewItem3 = new TMenuItem(this); // create the separator
            if (!DatiAgv[Img->Tag].stand_by)
                NewItem3->Caption = "STANDBY AGV n° " + IntToStr(Img->Tag) + " !";
            else
                NewItem3->Caption = "Restart AGV n° " + IntToStr(Img->Tag) + " !";
            NewItem3->Tag = Img->Tag; // mi salvo l'id dell'agv
            NewItem3->OnClick = StandBy1Click;
            PopupMenu1->Items->Add(NewItem3);
		}

	}
	else if (Img != NULL) {
		agvdaspostare = 0;
		pos_sel = RicercaImgLayout(Img->Left + X, Img->Top + Y);
		if (pos_sel)
		{
			tipopos = dmDB->RitornaTipoPos(pos_sel);
			// posizioni di magazzino
			if (tipopos == DEF_TYPE)
			{
				pos_scaffale = pos_sel;
				corsia_scaffale = dmDB->ReturnFilaDaPos(pos_sel);
				CreateMDIChild(fCorsia, "fCorsia", "Stato Corsia");
			}
			//deposito/prelievo rulliera
			else if (tipopos == AVVOLGITORE_TYPE)
			{
				FormBaiaScarico->ShowModal();
			}
			//scarico
			else if (tipopos == LINEASCAR_TYPE)
			{
                FormPlcCarico->m_nType = tipopos;
                FormPlcCarico->pos = pos_sel;
                FormPlcCarico->ShowModal();
			}
			//carico
			else if (tipopos == LINEACAR_TYPE)
			{
				//PosPresseForm->pos = pos_sel;
				//PosPresseForm->m_nType = tipopos;
				//PosPresseForm->ShowModal();
                FormPlcCarico->m_nType = tipopos;
                FormPlcCarico->pos = pos_sel;
                FormPlcCarico->ShowModal();
			}
		}
	}

}

// ---------------------------------------------------------------------------
void TMainForm::EscludiPosSelezionate(int pos, int escludi) {
    // escludo pos selezionate
    AnsiString stringa;
    TMyShape *Pal;
    bool sel = false, primo = true;
	stringa = "Update Posizioni Set Disabilita = " + IntToStr(escludi) + " where Pos = " + IntToStr(pos);
	// Image1->Visible = false ;
	dmDB->QueryMappa->Close();
	dmDB->QueryMappa->SQL->Clear();
	dmDB->QueryMappa->SQL->Append("Select * from Posizioni where Pos <> " + IntToStr(pos));
	dmDB->QueryMappa->Open();
	if (dmDB->QueryMappa->RecordCount) {
		dmDB->QueryMappa->First();
		while (!dmDB->QueryMappa->Eof) {
			Pal = (TMyShape*) ScrollBox1->FindComponent("Pos" + dmDB->QueryMappa->FieldByName("Pos")->AsString);
			if (Pal != NULL) {
				if (Pal->Pen->Color == clSilver) {
					stringa = stringa + " OR ";
					stringa = stringa + "Pos = " + dmDB->QueryMappa->FieldByName("Pos")->AsString;
					if (primo)
						primo = false;
					sel = true;
				}
			}
			dmDB->QueryMappa->Next();
		}
	}
	// Image1->Visible = true ;
	dmDB->QueryPos->Close();
	dmDB->QueryPos->SQL->Clear();
	dmDB->QueryPos->SQL->Append(stringa);
	dmDB->QueryPos->ExecSQL();
	dmDB->LogMsg(stringa);
	dmDB->QueryMappa->Close();
	ridisegnapalsel = 0;
	/* if (MainForm->program != 1)
	 MainForm->AggiornaMappaServer(5); */
	AggiornaMappa();

}

void __fastcall TMainForm::FormMouseWheel(TObject * Sender, TShiftState Shift, int WheelDelta, TPoint & MousePos, bool &Handled) {
	// zoom
    int propy, propx, i;
    TTime tempo;
    TImage *Img;
    bool disegna;
    disegna = false;
    tempo = Time() - StrToTime("00:00:01");
    if (tempo > ScrollMouse) {
        if (WheelDelta >= 120) {
            if (countwheel > 2) {
                if (Image1->Width < 3000) {
                    // Image1->Width *= 2;
                    // Image1->Height *= 2;
                    // Image1->Align = alClient ;
                    // propy = (MousePos.y * (ScrollBox1->Height * 2))/ScrollBox1->VertScrollBar->Range ;
                    // propx = (MousePos.x * (ScrollBox1->Width * 2))/ScrollBox1->HorzScrollBar->Range ;
                    MousePos.y = MousePos.y - (Height - ClientHeight) - Panel2->Top - Image5->Top;
                    propy = MousePos.y;
                    propx = MousePos.x;
                    Image1->Width *= 2;
                    Image1->Height *= 2;
                    // PanScroll->Top = 0 - ScrollBox1->VertScrollBar->Position;
                    // PanScroll->Left = 0 - ScrollBox1->HorzScrollBar->Position;
                    // Image1->Top = 0 ;
                    // Image1->Left = 0 ;
                    ScrollBox1->VertScrollBar->Range = Image1->Height;
                    ScrollBox1->HorzScrollBar->Range = Image1->Width;
                    ScrollBox1->HorzScrollBar->Position *= 2;
                    ScrollBox1->VertScrollBar->Position *= 2;
                    zoom *= 2;
                    disegna = true;
                    // MousePos.x = MousePos.x  *2  ;
                    ScrollBox1->HorzScrollBar->Position += propx;
                    // ScrollBox1->HorzScrollBar->Position+MousePos.x;
                    ScrollBox1->VertScrollBar->Position += propy;
                    // ScrollBox1->VertScrollBar->Position+MousePos.y;
                }
                ScrollMouse = Time();
                countwheel = 0;
            }
            else
                countwheel++;
        }
        else if ((WheelDelta <= -120) && (Image1->Width > 1400)) {
            if (countwheel > 2) {
                // Image1->Width = Image1->Width / 2;
                // Image1->Height = Image1->Height / 2;
                // Image1->Align = alClient ;
                MousePos.y = MousePos.y - (Height - ClientHeight) - Panel2->Top - Image5->Top;
                if (((Image1->Width / 2) < widthextend) || ((Image1->Height / 2) < heightextend)) {
                    Image1->Width = widthextend;
                    Image1->Height = heightextend;
                }
                else {
                    Image1->Width /= 2;
                    Image1->Height /= 2;
                }
                // PanScroll->Top = 0 - ScrollBox1->VertScrollBar->Position;
                // PanScroll->Left = 0 - ScrollBox1->HorzScrollBar->Position;
                ScrollBox1->VertScrollBar->Range = Image1->Height;
                ScrollBox1->HorzScrollBar->Range = Image1->Width;
                ScrollBox1->HorzScrollBar->Position /= 2;
                ScrollBox1->VertScrollBar->Position /= 2;
                Image1->Top = 0;
                Image1->Left = 0;
                if (zoom >= 2)
                    zoom /= 2;
                disegna = true;
                Image1->Align = alNone;
                ScrollMouse = Time();
                countwheel = 0;
            }
            else
                countwheel++;
        }
        if (disegna) {
            Xo = (Xo * Image1->Width) / FormW;
            // Ricalcolo l'origine in base alla dimensione della finestra
            Yo = (Yo * Image1->Height) / FormH;
            FormH = Image1->Height;
            FormW = Image1->Width;
            for (i = 1; i <= N_carrelli; i++)
                DisegnaCarrello(i);
            AggiornaMappa();
        }
        // CreaShapePos();
    }
    else
        countwheel = 0;

}

int TMainForm::RicercaImgLayout(int posx, int posy) {
    AnsiString stringa, stringa2, str;
    double x, y;
    int pos_return = 0;
    TMyShape *Pos;
    stringa = "Select * From Posizioni order by Pos ";

    if (dmDB->ADOConnection1->Connected) {
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while ((!dmDB->QueryPos->Eof) && (pos_return == 0)) {
                    Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + dmDB->QueryPos->FieldByName("Pos")->AsString);
                    if (Pos) {
                        if (((posy >= Pos->Top) && (posy <= (Pos->Top + Pos->Height))) && ((posx >= Pos->Left) && (posx <= (Pos->Left + Pos->Width)))) { // trovata cella
                            pos_return = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                        }
                    }
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
        }
        catch (...) {
        }
    }
    return pos_return;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::MoveMis1Click(TObject * Sender) {
    //
    int agv;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        ShowMessage("Seleziona la destinazione per Lgv " + IntToStr(Item->Tag) + "!!! (Pulsante destro del Mouse)");
        agvdaspostare = Item->Tag;
    }

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::DropMis1Click(TObject * Sender) {
    //
    int agv;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        ShowMessage("Seleziona la posizione di deposito per Lgv " + IntToStr(Item->Tag) + "!!! (Pulsante destro del Mouse)");
        agvdaspostare = Item->Tag;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MoveAgv1Click(TObject * Sender) {
    //
    AnsiString str, code;
    int corsia_dep;
    TMenuItem *Item = (TMenuItem*) Sender;
    MisManuale.tipo_mis = 4;
    if ((Item != NULL) && (agvdaspostare)) 
    {
        MisManuale.posprel = DatiAgv[agvdaspostare].pos;
        MisManuale.posdep  = Item->Tag;
		if (MisManuale.posdep) 
        {
			dmDB->GeneraMissione(MisManuale);
		}
	}
	agvdaspostare = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::DropAgv1Click(TObject * Sender) {
    //
    AnsiString str, code, corsia_dep;
    int posdep;
    TMissione m;
	TMenuItem *Item = (TMenuItem*) Sender;
    if ((Item != NULL) && (agvdaspostare) && (DatiAgv[agvdaspostare].load)) {
        corsia_dep = dmDB->NomeCorsiaDaPosizione(Item->Tag);
        posdep = dmDB->RicercaDeposito(DatiAgv[agvdaspostare].tipopallet, m, corsia_dep);
        if (posdep) {
            dmDB->GeneraMissione(MisManuale);
        }
        else
            ShowMessage("Corsia selezionata non disponibile o tipo di pallet a bordo Agv non coerente con la corsia selezionata");

    }
    agvdaspostare = 0;
}

void __fastcall TMainForm::DropPal1Click(TObject * Sender) 
{
    //
    AnsiString str, code, corsia_dep;
    int posdep, tipopos;
    TMenuItem *Item = (TMenuItem*) Sender;

    if (Item == NULL)
        return;
        
    if (!Item->Tag)
        return;
    
	if (!AbilitaMissioni) 
    {
        ShowMessage("ATTENZIONE!!! Le missioni sono disabilitate!!!");
        return;
    }
    
	if (DatiAgv[1].generata) 
    {
        ShowMessage("ATTENZIONE!!! C'è una missione in corso! Attendere la fine della missione.");
        return;
    }
    
	tipopos = dmDB->RitornaTipoPos(Item->Tag);
	if ((MisManuale.posprel) && (tipopos == LINEASCAR_TYPE)) 
    {
        MisManuale.posdep = Item->Tag;
        MisManuale.Agv = 1;
    	dmDB->GeneraMissione(MisManuale);
        MisManuale.posprel = 0;
        MisManuale.Source = "";
    }
}

void __fastcall TMainForm::PrelPal1Click(TObject * Sender) 
{
    AnsiString str, tipo_pal;
    int corsia_dep, posdep, tipo_pal_udc, id;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) 
    {
        ShowMessage("Seleziona la posizione di deposito per Lgv!!!\n (Pulsante destro del Mouse)");
        MisManuale.posprel = Item->Tag;
        MisManuale.Source = "";
	}
}

void __fastcall TMainForm::CaricaPalletDaPiano1(TObject * Sender) 
{
    AnsiString custpos = "";
    int posdep, prot;
    TMenuItem *Item = (TMenuItem*) Sender;

    IndexListIterator i;
    
    if (Item != NULL) 
    {
        for (i = dmDB->TabCustomerPos.begin(); i != dmDB->TabCustomerPos.end(); i++) 
        {
            prot = i->second["POS"].ToIntDef(0);
            prot = i->second["CUSTOMERPOS"].Pos("_1");
            if ((i->second["POS"].ToIntDef(0) == Item->Tag) && (i->second["CUSTOMERPOS"].Pos("_1"))) 
            {
                custpos = i->second["CUSTOMERPOS"];
                break;
            }
        }
    
        ShowMessage("Seleziona la posizione di deposito per Lgv!!!\n (Pulsante destro del Mouse)");
        MisManuale.posprel = Item->Tag;
        MisManuale.Source = custpos;
	}
}


void __fastcall TMainForm::CaricaPalletDaPiano2(TObject * Sender) 
{
    AnsiString custpos = "";
    int posdep;
    TMenuItem *Item = (TMenuItem*) Sender;

    IndexListIterator i;
    
    if (Item != NULL) 
    {
        for (i = dmDB->TabCustomerPos.begin(); i != dmDB->TabCustomerPos.end(); i++) 
        {
            if ((i->second["POS"].ToIntDef(0) == Item->Tag) && (i->second["CUSTOMERPOS"].Pos("_2"))) 
            {
                custpos = i->second["CUSTOMERPOS"];
                break;
            }
        }
    
        ShowMessage("Seleziona la posizione di deposito per Lgv!!!\n (Pulsante destro del Mouse)");
        MisManuale.posprel = Item->Tag;
        MisManuale.Source = custpos;
	}
}


void __fastcall TMainForm::StandBy1Click(TObject * Sender) {
    //
	TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        dmComandiAgv->StandByAgv(Item->Tag, !DatiAgv[Item->Tag].stand_by);
        /* else
         SocketDataModuleClient->Invia("MSGTYPE=STANDBY|AGV="+IntToStr(Item->Tag)+"|VAL="+IntToStr(!DatiAgv[Item->Tag].stand_by)+"|");
         */
    }

}

void __fastcall TMainForm::Pos1MouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
/*
    TMyShape *Pos;
    // apro il popup
    int popupok = 0;
    PopupMenuPos->Items->Clear();
    Pos = (TMyShape*) Sender;
    if (Pos != NULL) {
        if (Button == mbRight) {
            if (Pos->Tag == 1) {
                // postazione deposito avvolgitore
                if ((MisManuale.posprel) && (MisManuale.tipo_pallet)) {
                    TMenuItem *NewItem1 = new TMenuItem(this);
                    // create the separator
                    NewItem1->Caption = "Creare Missione di deposito pallet su avvolgitore??";
                    NewItem1->Tag = Pos->Tag + 10;
                    NewItem1->OnClick = DropPal1Click;
                    PopupMenuPos->Items->Add(NewItem1);

                }
                TMenuItem *NewItem2 = new TMenuItem(this);
                // create the separator
                NewItem2->Caption = "Disabilita Deposito Avvolgitore";
                NewItem2->Tag = Pos->Tag + 10;
                NewItem2->OnClick = ExcludePosition1Click;
                PopupMenuPos->Items->Add(NewItem2);
                TMenuItem *NewItem3 = new TMenuItem(this);
                // create the separator
                NewItem3->Caption = "Abilita Deposito Avvolgitore";
                NewItem3->Tag = Pos->Tag + 10;
                NewItem3->OnClick = RemoveExclusion1Click;
                PopupMenuPos->Items->Add(NewItem3);
                popupok = 1;

                if (popupok) {
                    PopupMenuPos->Tag = Pos->Tag + 10;
                    PopupMenuPos->Popup(Left + X + Pos->Left, Top + Y + Pos->Top + Panel2->Height + Image5->Height);
                }

            }
            else if (Pos->Tag == 2) {
                // postazione prelievo avvolgitore
                if (!MisManuale.posprel) {
                    TMenuItem *NewItem1 = new TMenuItem(this);
                    // create the separator
                    NewItem1->Caption = "Creare Missione di prelievo pallet da avvolgitore??";
                    NewItem1->Tag = Pos->Tag + 10;
                    NewItem1->OnClick = PrelPal1Click;
                    PopupMenuPos->Items->Add(NewItem1);

                }
                TMenuItem *NewItem2 = new TMenuItem(this);
                // create the separator
                NewItem2->Caption = "Disabilita Prelievo Avvolgitore";
                NewItem2->Tag = Pos->Tag + 10;
                NewItem2->OnClick = ExcludePosition1Click;
                PopupMenuPos->Items->Add(NewItem2);
                TMenuItem *NewItem3 = new TMenuItem(this);
                // create the separator
                NewItem3->Caption = "Abilita Prelievo Avvolgitore";
                NewItem3->Tag = Pos->Tag + 10;
                NewItem3->OnClick = RemoveExclusion1Click;
                PopupMenuPos->Items->Add(NewItem3);
                popupok = 1;

                if (popupok) {
                    PopupMenuPos->Tag = Pos->Tag + 10;
                    PopupMenuPos->Popup(Left + X + Pos->Left, Top + Y + Pos->Top + Panel2->Height + Image5->Height);
                }

            }
		}
		else if (Pos->Tag < 3) {
			// visualizzo pagina per impostazione codice e memoria generata
			FormBaiaCarico->Show();
			// FormBaiaCarico->cbCodice->Text = MainForm->DatiPlc[1].CodicePallet ;
        }
    }
*/}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TimerSinotticoTimer(TObject * Sender) 
{
    TMyShape *Oggetto;
    TShape *OggettoShape;
    TPanel *PanSemaforo;
    int i, imgw, imgh, cellaTop, cellaLeft, agv, altroagv;
    double x, y;
    // visualizzo eventi sinottico
    TimerSinottico->Enabled = false;
	if (dmDB->lettostatoagv) 
    {
        // Fasciatore
		/*Oggetto = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(MainForm->PlcFascia.posdep));
		if (Oggetto != NULL) 
        {
			if (!MainForm->PlcFascia.ChiamataDeposito)
				Oggetto->Brush->Color = clYellow;
			else
				Oggetto->Brush->Style = bsClear;
		}  */

        //Semafori
        Rosso->Visible = PlcIncas.impegno_inc_rosso;
        Giallo->Visible = PlcIncas.impegno_inc_giallo;
        Verde->Visible = PlcIncas.impegno_inc_verde;

        //RossoIncrocio->Visible = !PlcIncas.consenso_impegno_incrocio;
        //VerdeIncrocio->Visible = PlcIncas.consenso_impegno_incrocio;

        
		/*Oggetto = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(MainForm->PlcMonta.posprel));
		if (Oggetto != NULL) 
        {
			if ((MainForm->PlcMonta.PrelievoVuoti) || (!MainForm->PlcMonta.ChiamataDeposito && MainForm->PlcMonta.MontacarichiAttivo))
				Oggetto->Brush->Color = clYellow;
			else
				Oggetto->Brush->Style = bsClear;
			if (avanza_pos2) {
				Oggetto->BringToFront();
				avanza_pos2 = false ;
			}
		} 
        
        if (MainForm->PlcMonta.PrenotaVuoti)
            PrenotaVuoti->Brush->Color = clFuchsia;
        else
            PrenotaVuoti->Brush->Style = bsClear;
         */
         
		/*if (zoom == 1) {
			if (MainForm->PlcFascia.presenze_pallet[2])
				PosAvv->Brush->Color = clYellow;
			else
				PosAvv->Brush->Style = bsClear;
			PosAvv->Visible = true ;
		}else {
			PosAvv->Visible = false ;
		}     */
	}
	//BitBtn3->Visible = (pwdlevel && Simula);

    if (counterAzzeraUdc) 
    {
        counterAzzeraUdc -= 1;
        if (counterAzzeraUdc == 0) 
        {
            dmDB->AggiornaESvuotaPosDeposito();   
            aggiornamappa = 1; 
        } 
    }
    
    if ((DebugHook)) 
    {
        if (Simula)
            SpSimulazione->Caption = "RES ";
        else
            SpSimulazione->Caption = "SIM ";
        SpSimulazione->Visible = true;
    }
    else
        SpSimulazione->Visible = false;
    
	TimerSinottico->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Svuotacorsia1Click(TObject * Sender) {
	//
	int corsia;
	if (Application->MessageBox(L" Sicuro di voler svuotare l 'intera Corsia ?", L"Conferma!!!", MB_YESNO) == IDYES) {
        corsia = dmDB->ReturnFilaDaPos(PopupMenuPos->Tag);
        dmDB->SvuotaCorsia(corsia);
        AggiornaMappa();
    }

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::DisabilitaCorsia1Click(TObject * Sender) {
    //
    int corsia = dmDB->ReturnFilaDaPos(PopupMenuPos->Tag);
    dmDB->AbilitaCorsia(corsia, 0);
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
    AggiornaMappa();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Abilitacorsia1Click(TObject * Sender) {
    //
    int corsia = dmDB->ReturnFilaDaPos(PopupMenuPos->Tag);
    dmDB->AbilitaCorsia(corsia, 1);
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
    AggiornaMappa();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::sCaricoClick(TObject * Sender)
{
 //   FormBaiaCarico->Show();

}
// ---------------------------------------------------------------------------


void __fastcall TMainForm::BitBtn2Click(TObject * Sender) 
{
    //
    //CreateMDIChild(fSegnaliBaia, "fSegnaliBaia", "Segnali Phoenix");
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn4Click(TObject * Sender) 
{
    CreateMDIChild(fMissioniWms, "fMissioniWms", "Lista Missioni");
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::BtnUDCOutClick(TObject * Sender) {
    CreateMDIChild(fTabEventi, "fTabEventi", "Tabella Stato Baie");
}
// ---------------------------------------------------------------------------
// ABILITA

void __fastcall TMainForm::BitBtn1Click(TObject *Sender)
{
	//if (MainForm->pwdlevel) {
		AbilitaMissioni = 1;
		dmDB->LogMsg("Abilita missioni Premuto");
	    dmDB->AggiornaStatoAgv(1);
    //}
}
//---------------------------------------------------------------------------
//DISABILITA
void __fastcall TMainForm::BitBtn5Click(TObject *Sender)
{
	//if (MainForm->pwdlevel) {
		AbilitaMissioni = 0;
		dmDB->LogMsg("Disabilita missioni Premuto");
	    dmDB->AggiornaStatoAgv(1);
    //}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
    bool Hand;
    TPoint MP;
    float RapportoOriz, RapportoVert, zoomprec;
    if (Screen->Cursor == crHandPoint)
        Screen->Cursor = crDefault;

    if ((((mousedown) && (x1 != x2) && (y1 != y2) && (Button == mbRight)) || ((mousedownsel) && (x1sel != x2sel) && (y1sel != y2sel) && (Button == mbLeft))) && (PaintBox1 != NULL)) {
        // disegno zoom in base a rettangolo
        if (Button == mbRight) {
            // col destro zoom
            if ((zoom <= 1) || (zoom >= 6))
                zoomprec = zoom - 1;
            else
                zoomprec = zoom;
            zoom = 2;
            Image1->Width = widthextend * zoom;
            Image1->Height = heightextend * zoom;
            Image1->Top = 0;
            Image1->Left = 0;
            ScrollBox1->HorzScrollBar->Range = Image1->Width;
            ScrollBox1->VertScrollBar->Range = Image1->Height;
            if (x1 > x2)
                x1 = x2; // + (abs(x1-x2)/2);
            else
                x1 = x1; // + (abs(x2-x1)/2);
            if (y1 > y2)
                y1 = y2; // +(abs(y1-y2)/2);
            else
                y1 = y1; // +(abs(y2-y1)/2);
            ScrollBox1->HorzScrollBar->Position = x1 * (zoom - zoomprec);
            ScrollBox1->VertScrollBar->Position = y1 * (zoom - zoomprec);
            Xo = (Xo * Image1->Width) / FormW;
            // Ricalcolo l'origine in base alla dimensione della finestra
            Yo = (Yo * Image1->Height) / FormH;
            FormH = Image1->Height;
            FormW = Image1->Width;
            AggiornaMappa();
            aggiornaagv = 1;
        }
        else if (Button == mbLeft) {
            // col sinistro seleziono
            AggiornaMappa();
        }
    }
    else if (ridisegnapalsel) {
        ridisegnapalsel = 0;
        AggiornaMappa();
    }
    agvdaspostare = 0;
    mousedown = false;
    mousedownsel = false;

    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bAnagraficheMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	PopupMenu->Items->Clear();

	TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
	NewItem1->Caption = "UDC ";
	NewItem1->Tag = 1;
	ImageListMenu->Draw(NewItem1->Bitmap->Canvas, X, Y, 1); // non va
	NewItem1->OnClick = UDCClick;
	PopupMenu->Items->Add(NewItem1);

	PopupMenu->Popup(X + bAnagrafiche->Left, Y + bAnagrafiche->Height);
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::UDCClick(TObject * Sender) {
	FormUDCList->blocca_insert = true;
	FormUDCList->Show();
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::SpSimulazioneClick(TObject *Sender)
{
    if (Simula)
        Simula = false; 
    else
        Simula = true; 
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ResetBtnClick(TObject *Sender)
{
	int i, baiaprel, baiadep, agv, idlista;
	UnicodeString strmessage;
	Evento ev;
	if (pwdlevel) 
    {
		if (!AbilitaMissioni) {
			if (Application->MessageBox(L"Resettare le missioni in corso sull' AGV?\n (Ricordarsi di abortire anche da pannello Agv)", L"Conferma!!!", MB_YESNO) == IDYES) {
				sprintf(ev, "SVUOTA,%d", 0);
				cs->SendEv(ev);
				if (Application->MessageBox(L"Resettare tutte le prenotazioni in magazzino / avvolgitore / montacarichi?", L"Conferma!!!", MB_YESNO) == IDYES) {
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
				aggiornamappa = 1;
			}
			if (Application->MessageBox(L"Resettare le memorie di AGV in missione ????", L"Conferma!!!", MB_YESNO) == IDYES) 
            {
				DatiAgv[1].generata = 0 ;
			}
		}
		else
			ShowMessage("Per Resettare l'impianto le Missioni devono essere DISABILITATE!");
	}
    dmDB->CaricaTabelle();
    dmDB->AggiornaMemoriePlc();
    aggiornamappa = 1;
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::PanelIncrocioMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    PosVuotiForm->Show();
}
//---------------------------------------------------------------------------

