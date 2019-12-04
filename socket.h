#ifndef _SOCKET_H
#define _SOCKET_H

#pragma hdrstop
#include <scktcomp.hpp>
#include <extctrls.hpp>
#include <vector>
#include "eventi.h"

using namespace std;

#define PACCHETTO 100
#define MAX_REQ 5  /* massimo numero di richieste nella listen */

extern int MISSIONS_PORT;
extern int MISS_STATE_PORT;
extern int MARGRAF_REC_PORT;
extern int MARGRAF_SEND_PORT;
extern int AGV_REC_PORT;
extern int AGV_SEND_PORT;

class SRVSOCKET : public TServerSocket {
	public:
		SRVSOCKET(int port);
		__fastcall ~SRVSOCKET(void);
		int CreateSocket(int port);
		int ChangePort(int port);
		void SendAnswer(char *ev, TCustomWinSocket *Socket);
	protected:
		void __fastcall ServerSocketRead(TObject *Sender, TCustomWinSocket *Socket);
		virtual void Elabora(char *ev, TCustomWinSocket *Socket) {};
};

class CLIENTSOCKET : public TClientSocket  {
	public:
		CLIENTSOCKET(char *host = "", int port = 0);
		__fastcall ~CLIENTSOCKET(void);
		void SendEv(char *ev);
		TTimer *ritenta;
	protected:
		void __fastcall RitentaTimer(TObject *Sender);
		void __fastcall ClientSocketRead(TObject *Sender, TCustomWinSocket *Socket);
		void __fastcall ClientSocketError(TObject *Sender,
				TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
		void InviaTelegramma();
		vector<AnsiString> telegrammi;
		int Server_Port;
		char Server_Address[20];
};


class MISS_STATESOCKET : public SRVSOCKET {
	public:
		MISS_STATESOCKET();
	private:
		void Elabora(char *ev, TCustomWinSocket *Socket);
};


#endif

