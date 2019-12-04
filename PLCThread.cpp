//---------------------------------------------------------------------------

#include <vcl.h>
#include <IniFiles.hpp>
#include <System.WideStrUtils.hpp>
#pragma hdrstop

#include "PLCThread.h"
//#include "DB.h"

//#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TPLCThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
TPLCThread *PLCThread[N_PLC];
/*int blocklist[N_BLOCKS] = {30, 105, 120, 145, 146, 147, 148, 149, 160, 161, 161, 162, 162, 220, 250};
int blockadd[N_BLOCKS] 	= { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 150,   0, 150,   0,   0};
int blocklen[N_BLOCKS]  = { 8,  12, 110,  14, 122, 122,  34, 200,  48, 150, 150, 150, 150,  16,  66};    */
//---------------------------------------------------------------------------


__fastcall TPLCThread::TPLCThread(bool CreateSuspended,int plcid)
	: TThread(CreateSuspended)
{
	TIniFile *ini;
    int i ;
//	NameThreadForDebugging("PLCThread", ThreadID);
	Stopped = false;
	ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	PLCAddress = ini->ReadString("PLC"+IntToStr(plcid), "PLCAddress", "127.0.0.1");
	PLCPort = StrToInt(ini->ReadString("PLC"+IntToStr(plcid), "PLCPort", "102"));
	PLCRack = StrToInt(ini->ReadString("PLC"+IntToStr(plcid), "PLCRack", "0"));
	PLCSlot = StrToInt(ini->ReadString("PLC"+IntToStr(plcid), "PLCSlot", "0")); //0-1 plc s7 200-1200, 2 s7 300 , 3 s7 400 , 1 s7 1500
	delete ini;
	initialized = false;
	connected = false;
	tentativi = 0;
    id_plc = plcid ;
	Step = 0;

    for ( i = 0;i< N_BLOCKS_PLC;i++) {
        blocklist[i] = 0;
        blockadd[i] = 0;
        blocklen[i] = 0 ;
    }
    ReadFileBlockList(plcid) ;
	memset(DB, 0, sizeof(DB));
}
//---------------------------------------------------------------------------
int TPLCThread::ReadFileBlockList(int plcid) {
	//leggo il file delle db da mettere in lettura
    //il nome del file lo ricavo dall'idplc
    FILE *f ;
    int i,n,blist,badd,blen ;
    AnsiString namefile ;
    namefile = "plc"+IntToStr(plcid)+".txt" ;
	f = fopen(namefile.c_str(),"r");
    N_BLOCKS_TOTAL = N_BLOCKS_PLC ;
	if (f == NULL) {
//		exit(-1);
        return 0 ;
	}else{
		fseek(f, 0, SEEK_SET);
		// leggo i valori di altezza e larghezza dell'impianto e il punto d'origine
        i = 0 ;
		while (!feof(f)) {
			n = fscanf(f , "%d,%d,%d" ,&blist,&badd,&blen );
            if (n == 3) {
                blocklist[i] = blist;
                blockadd[i] = badd;
                blocklen[i] = blen ;
                i++;
            }
		}
        N_BLOCKS_TOTAL = i ;
	}
	fclose(f);
	return 1;
}

unsigned char* TPLCThread::DataBlock(int db) {
	unsigned char *res = NULL;

	res = DB[db];
	return res;
}
//---------------------------------------------------------------------------

void __fastcall TPLCThread::Execute()
{
	bool allarmi[512];
	int i, j, db, add, n, nall,k;
	AnsiString s;
	unsigned char *buffer;
	time_t t;

	Sleep(5000);
	while (!Stopped) {
		try {
			if (!initialized) {
				InizializzaPLC();
			}
			else if (!connected) {
				ConnettiPLC();
			}
			else if (AsyncCom.empty()) {
				// Lettura dati da PLC
				if (Step < N_BLOCKS_TOTAL) {
					db = blocklist[Step];
					add = blockadd[Step];
					n = blocklen[Step];
                   /* for (k = add ; k<= (add+n) ; k++)
                        TempDB[db][k] = DB[db][k] ;  */
                    if (n > 0) {
                        buffer = DB[blocklist[Step]] + add;
                        PLCError = daveReadBytes(dc, daveDB, db, add, n, buffer);
                      /*  for (k = add ; k<= (add+n) ; k++) {
                            if (TempDB[db][k] != DB[db][k] ) {
                                //invio aggiornamento ai client valore plc cambiato
                            }
                        }  */
                    }else {
                        Step = -1;
                        PLCError = 0 ; // se ho più di un plc imposto NBLOCK per le letture massime e se ho nbyte a 0 da leggere rinizio dal primo step di quel plc
                    }
				}
				if (PLCError == 0) {
					tentativi = 0;
					Step++;
					if ((Step >= N_BLOCKS_TOTAL)||(Step == 0)) {
						Step = 0;
						t2 = clock();
						t = t2 - t1;
						t1 = t2;
						CycleTime.printf("%d", t);
						tempo_ciclo_int = CycleTime.ToIntDef(-1) ;
						CycleTime.printf("%d ms", t);
					}
                    ErrorString = "ok" ;
				}
				else {
					tentativi++;
					if (tentativi > 30) { //va in base al tempo di sleep sotto
						DisconnettiPLC();
                        tentativi = 0 ;
                    }
                    ErrorString = daveStrerror(PLCError) ;
					Step++;
					if ((Step >= N_BLOCKS_TOTAL)||(Step == 0)) {
						Step = 0;
						t2 = clock();
						t = t2 - t1;
						t1 = t2;
						CycleTime.printf("%d", t);
						tempo_ciclo_int = CycleTime.ToIntDef(-1) ;
						CycleTime.printf("%d ms", t);
					}
				}
			}
			else {
				// Esegui scrittura PLC

				switch (AsyncCom[0].cmd) {
				case WR_BYTE:
					PLCError = daveWriteBytes(dc, daveDB, AsyncCom[0].db, AsyncCom[0].add, AsyncCom[0].n, AsyncCom[0].data);
					break;
				case WR_WORD:
					PLCError = daveWriteBytes(dc, daveDB, AsyncCom[0].db, AsyncCom[0].add, AsyncCom[0].n, AsyncCom[0].data);
					break;
				case WR_DWORD:
					PLCError = daveWriteBytes(dc, daveDB, AsyncCom[0].db, AsyncCom[0].add, AsyncCom[0].n, AsyncCom[0].data);
					break;
				case SET_BIT:
					PLCError = daveSetBit(dc, daveDB, AsyncCom[0].db, AsyncCom[0].add, AsyncCom[0].n);
					break;
				case RST_BIT:
					PLCError = daveClrBit(dc, daveDB, AsyncCom[0].db, AsyncCom[0].add, AsyncCom[0].n);
					break;
				case WR_BLOCK:
					PLCError = daveWriteBytes(dc, daveDB, AsyncCom[0].db, AsyncCom[0].add, AsyncCom[0].n, AsyncCom[0].data);
					break;
				}

				if (PLCError == 0) {
					tentativi = 0;
					AsyncCom.erase(AsyncCom.begin());
				}
				else {
					tentativi++;
					ErrorString = daveStrerror(PLCError) ;
					if (tentativi > 10)
						DisconnettiPLC();
				}
			}
			Sleep(50);
		} catch (...) {}
	}
	DisconnettiPLC();
	Suspend();
}
//---------------------------------------------------------------------------

void TPLCThread::StopThread() {
	Stopped = true;
}
//---------------------------------------------------------------------------

void TPLCThread::InizializzaPLC() {
	int n;

	fds.rfd = openSocket(PLCPort, PLCAddress.c_str());
	fds.wfd = fds.rfd;
	if (fds.rfd > 0) {
		di = daveNewInterface(fds, "IF1", MPILocal, daveProtoISOTCP, daveSpeed187k); // diretto sulla cp (300-400)
		/*
	public static readonly int daveSpeed9k = 0;
    public static readonly int daveSpeed19k =   1;
	public static readonly int daveSpeed187k =   2;
	public static readonly int daveSpeed500k =  3;
    public static readonly int daveSpeed1500k =  4;
	public static readonly int daveSpeed45k =    5;
	public static readonly int daveSpeed93k =   6;
	}*/
		//di = daveNewInterface(fds, "IF1", MPILocal, daveProtoISOTCP243, daveSpeed187k); // cp 243
		//di = daveNewInterface(fds, "IF1", MPILocal, daveProtoPPI_IBH, daveSpeed187k); //modulo aggiuntivo ibh link s7++
		//daveSetTimeout(di,500);//5000000);
		//slot : 0-1 plc s7 200-1200, 2 s7 300 , 3 s7 400 , 1 s7 1500
		daveSetTimeout(di,1000);//5000000);
		dc = daveNewConnection(di, MPIRemote, PLCRack, PLCSlot);
		PLCError = daveConnectPLC(dc);
		ErrorString = daveStrerror(PLCError) ;
	} else
		PLCError = -1;
	if (!PLCError) {
		initialized = true;
		connected = true;
	}
	t1 = clock();
	CycleTime = "0ms";
}
//---------------------------------------------------------------------------

void TPLCThread::ConnettiPLC() {
	PLCError =daveConnectPLC(dc);
	if (PLCError == 0)
		connected = true;
}

void TPLCThread::DisconnettiPLC() {
	if (connected) {
		daveDisconnectPLC(dc);
		daveDisconnectAdapter(di);
	}
	initialized = false;
	connected = false;
	tentativi = 0;
}
//---------------------------------------------------------------------------

void TPLCThread::WriteData(int db, int add, int N, int cmd, unsigned char *data,bool simulazione)
{
	PLCWriteCom c;

	c.db = db;
	c.add = add;
	c.n = N;
	c.cmd = cmd;
	if (simulazione) {
		// scrivo in memoria i valori già quando richiamo la funzione, non eseguo la scrittura
	}else {
		if (data != NULL) {
			memcpy(c.data, data, 512);
		} else {
			c.data[0] = 0;
		}
		AsyncCom.push_back(c);
	}
}
//---------------------------------------------------------------------------

void TPLCThread::WriteBuffer(int db, int add, int N, unsigned char *buffer,bool simulazione)
{
	unsigned char *data;
	data = DataBlock(db);
	if (data != NULL) {
		memcpy(data + add, buffer, N);
	}
	WriteData(db, add, N, WR_BLOCK, buffer,simulazione);
	PLCWriteCom c;
}
void TPLCThread::WriteDWord(int db, int add, long int value,bool simulazione)
{
	unsigned char buffer[10], *data;

	FormatDBD(value, buffer);
	data = DataBlock(db);
	if (data != NULL) {
		memcpy(data + add, buffer, 4);
	}
	WriteData(db, add, 4, WR_DWORD, buffer,simulazione);
	PLCWriteCom c;
}
//---------------------------------------------------------------------------

void TPLCThread::WriteByte(int db, int add, unsigned char value,bool simulazione)
{
	unsigned char buffer[10], *data;

	buffer[0] = value;
	data = DataBlock(db);
	if (data != NULL) {
		memcpy(data + add, buffer, 1);
	}
	WriteData(db, add, 1, WR_BYTE, buffer,simulazione);
	PLCWriteCom c;
}
//---------------------------------------------------------------------------

void TPLCThread::WriteWord(int db, int add, int value,bool simulazione)
{
	unsigned char buffer[10], *data;

	FormatDBW(value, buffer);
	data = DataBlock(db);
	if (data != NULL) {
		memcpy(data + add, buffer, 2);
	}
	WriteData(db, add, 2, WR_WORD, buffer,simulazione);
	PLCWriteCom c;
}
//---------------------------------------------------------------------------

void TPLCThread::SetBit(int db, int add, int bit,bool simulazione)
{
	unsigned char *data;

	data = DataBlock(db);
	if (data != NULL) {
		data[add] |= (1 << bit);
	}
	WriteData(db, add, bit, SET_BIT,NULL,simulazione);
	PLCWriteCom c;
}
//---------------------------------------------------------------------------

void TPLCThread::ResetBit(int db, int add, int bit,bool simulazione)
{
	unsigned char *data;

	data = DataBlock(db);
	if (data != NULL) {
		data[add] &= ~(1 << bit);
	}
	WriteData(db, add, bit, RST_BIT,NULL,simulazione);
	PLCWriteCom c;
}
//---------------------------------------------------------------------------

void TPLCThread::WriteTimer(int db, int add, double value)
{
	unsigned char buffer[10], *data;

	FormatTimer(value, buffer);
	data = DataBlock(db);
	if (data != NULL) {
		memcpy(data + add, buffer, 2);
	}
	WriteData(db, add, 2, WR_WORD, buffer);
	PLCWriteCom c;
}
//---------------------------------------------------------------------------



