#include "eventi.h"
#include "socket.h"
#include "percorsiprototipo.h"
#include "main.h"
#include "db.h"

#define	TIMEOUT		30000
#define	RETRY_TIME	2000

extern DATILGV tab;
extern char N_carrelli;

// **************** SRVSOCKET ******************

SRVSOCKET::SRVSOCKET(int port):TServerSocket(NULL) {
	OnClientRead = ServerSocketRead;
	CreateSocket(port);
}

__fastcall SRVSOCKET::~SRVSOCKET(void) {
	Close();
	while (Active) {}
}

int SRVSOCKET::CreateSocket(int port) {
	ServerType = stNonBlocking;
	Port = port;
	Open();
	return port;
}

int SRVSOCKET::ChangePort(int port) {
	Close();
	CreateSocket(port);
	return 0;
}

void __fastcall SRVSOCKET::ServerSocketRead(TObject *Sender,
	  TCustomWinSocket *Socket)
{
	Evento ev;
	char *fine;

	strcpy(ev, Socket->ReceiveText().c_str());
	fine = strstr(ev, "\r");
	while (!strncmp(ev, "AGV=", 4) && (fine != NULL)) {
		*fine = '\0';
		Elabora(ev+4, Socket);
		strcpy(ev, fine + 1);
		fine = strstr(ev, "\r");
	}
}

void SRVSOCKET::SendAnswer(char *ev, TCustomWinSocket *Socket) {
	AnsiString e;

	e.printf("AGV=%s\r", ev);
	Socket->SendText(e);
}

// **************** MISSSOCKET ******************

// **************** CLIENTSOCKET ******************

CLIENTSOCKET::CLIENTSOCKET(char host[20], int port):TClientSocket(NULL) {
	telegrammi.clear();
	ritenta = new TTimer(NULL);
	ritenta->Enabled = false;
	ritenta->Interval = RETRY_TIME;
	ritenta->OnTimer = RitentaTimer;
	OnRead = ClientSocketRead;
	OnError = ClientSocketError;
	strcpy(Server_Address, host);
	if (port == 0)
		Server_Port = AGV_SEND_PORT;
	else
		Server_Port = port;
	ClientType = ctNonBlocking;
	Address = Server_Address;
	Port = Server_Port;
	Open();
}

__fastcall CLIENTSOCKET::~CLIENTSOCKET(void) {
	Close();
	delete ritenta;
}

void CLIENTSOCKET::SendEv(char *ev) {
	AnsiString e;

	e.printf("AGV=%s\r", ev);
	telegrammi.push_back(e);
	InviaTelegramma();
}

void CLIENTSOCKET::InviaTelegramma() {
	if (ritenta->Enabled)
		return;
	if (!Socket->Connected) {
		Address = Server_Address;
		Port = Server_Port;
		Open();
		ritenta->Interval = RETRY_TIME;
	} else if (!telegrammi.empty()) {
		Socket->SendText(telegrammi[0]);
		ritenta->Interval = TIMEOUT;
	}
	ritenta->Enabled = true;
}

void __fastcall CLIENTSOCKET::ClientSocketRead(TObject *Sender,
	  TCustomWinSocket *Socket)
{
	Evento ev;
	char *fine,stato;
    int idmiss;
    int causale = 0;
	ritenta->Enabled = false;
	strcpy(ev, Socket->ReceiveText().c_str());
	fine = strstr(ev, "\r");
	while (!strncmp(ev, "AGV=S,", 6) && (fine != NULL)) {
		*fine = '\0';
        sscanf(ev+6, "%d,%c", &idmiss, &stato);
		if (stato == 'A')
			dmDB->AbortMission(idmiss);
//		Log(ev+4);
		strcpy(ev, fine + 1);
		fine = strstr(ev, "\r");
	}
	while (!strncmp(ev, "AGV=OK,M,", 9) && (fine != NULL)) {
		*fine = '\0';
        sscanf(ev+9, "%d", &idmiss);
        //missione corretta ricevuta da AgvX
        dmDB->MissioneElaborataSuAgvX(idmiss,0);
		//MainForm->MissioneCorrettaSuAgvX(idmiss);
//		Log(ev+4);
		strcpy(ev, fine + 1);
		fine = strstr(ev, "\r");
	}
	while (!strncmp(ev, "AGV=ERR,M,", 10) && (fine != NULL)) {
		*fine = '\0';
        sscanf(ev+10, "%d", &idmiss, &causale);
        //missione NON corretta ricevuta da AgvX
        dmDB->MissioneElaborataSuAgvX(idmiss,causale);
		//MainForm->MissioneNonCorrettaSuAgvX(idmiss);
//		Log(ev+4);
		strcpy(ev, fine + 1);
		fine = strstr(ev, "\r");
	}
	if (!telegrammi.empty())
		telegrammi.erase(telegrammi.begin());
	if (!telegrammi.empty())
		InviaTelegramma();
}
//---------------------------------------------------------------------------

void __fastcall CLIENTSOCKET::RitentaTimer(TObject *Sender)
{
	char s[100];

	ritenta->Enabled = false;
	if (ritenta->Interval == TIMEOUT) {
		sprintf(s, "Timeout risposta socket: %s", telegrammi[0].c_str());
//		Log(s);
	}
	InviaTelegramma();
}

void __fastcall CLIENTSOCKET::ClientSocketError(TObject *Sender,
	  TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;
}
//---------------------------------------------------------------------------
MISS_STATESOCKET::MISS_STATESOCKET():SRVSOCKET(MISS_STATE_PORT) {}

void MISS_STATESOCKET::Elabora(char *ev, TCustomWinSocket *Socket) {
	char s[100];

	sprintf(s, "OK,%s", ev);
	SendAnswer(s, Socket);
}

