//---------------------------------------------------------------------------

#pragma hdrstop

#include "Ping.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool Ping(AnsiString ipaddress) {
	bool res;
	TIdIcmpClient *IdIcmpClient;

	IdIcmpClient = new TIdIcmpClient(NULL);
	IdIcmpClient->Host = ipaddress;
	IdIcmpClient->ReceiveTimeout = 50;
	IdIcmpClient->Ping();
	res = (IdIcmpClient->ReplyStatus->BytesReceived > 0);
	delete IdIcmpClient;
	return res;
}
//---------------------------------------------------------------------------
