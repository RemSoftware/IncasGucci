//---------------------------------------------------------------------------

#ifndef PLCThreadH
#define PLCThreadH
//---------------------------------------------------------------------------
#define BCCWIN
#define LITTLE_ENDIAN
#include <System.Classes.hpp>
#include <vector>
#include <time.h>
#include "nodave.h"
#include "openSocket.h"
#include "PLCFunct.h"
//---------------------------------------------------------------------------

//#define	PLCPort    102
/*#define	PLCSlot    0  // ex 2    //0-1 plc s7 200-1200, 2 s7 300 , 3 s7 400 , 1 s7 1500
#define	PLCRack    0            */
#define	MPILocal   1   //ex 1
#define	MPIRemote  2   // ex 2

#define	WR_BYTE  1
#define	WR_WORD  2
#define	WR_DWORD 3
#define	SET_BIT  4
#define	RST_BIT  5
#define	WR_BLOCK 6
#define	N_BLOCKS_PLC 50  //massimi blocchi di lettura per singolo plc

#define	N_PLC    2
/*extern int blocklist[N_BLOCKS];
extern int blockadd[N_BLOCKS];
extern int blocklen[N_BLOCKS];    */

struct PLCWriteCom {
	int db;
	int add;
	int n;
	int cmd;
	char unsigned data[512];
};

class TPLCThread : public Classes::TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall TPLCThread(bool CreateSuspended,int plcid);
	void InizializzaPLC();
	void ConnettiPLC();
   	void DisconnettiPLC();
	void StopThread();
	void WriteData(int db, int add, int N, int cmd, unsigned char *data = NULL , bool simulazione = false );
	void WriteByte(int db, int add, unsigned char value,bool simulazione=false);
	void WriteWord(int db, int add, int value,bool simulazione=false);
	void WriteDWord(int db, int add, long int value,bool simulazione=false);
	void SetBit(int db, int add, int bit,bool simulazione=false);
	void ResetBit(int db, int add, int bit,bool simulazione=false);
	void WriteTimer(int db, int add, double value);
	void WriteBuffer(int db, int add, int N, unsigned char *buffer,bool simulazione=false) ;
    int ReadFileBlockList(int plcid) ;
	unsigned char* DataBlock(int db);
	bool initialized, connected;
	unsigned char DB[2100][3600];
   //	unsigned char TempDB[500][1200];
	daveInterface *di;
	daveConnection *dc;
	_daveOSserialType fds;
	bool Stopped;
	AnsiString PLCAddress;
	std::vector<PLCWriteCom> AsyncCom;
	int tentativi;
	int PLCError;
	int Step;
	clock_t t1, t2;
	int tempo_ciclo_int ;
	AnsiString CycleTime,ErrorString;
    int id_plc ;
    int PLCPort ;
    int dbwatchdog_pc,wordwatchdog_pc,typewatchdog;
    int dbwatchdog_plc,wordwatchdog_plc;
    int watchdog_plc,watchdog_pc ;
    int count_alarm_watchdog ;
    int alarm_watchdog,num_alarm_watchdog ;
    int N_BLOCKS_TOTAL ; // mi salvo per ogni plc il numero di blocchi da leggere
    //int N_BLOCKS;
    int blocklist[N_BLOCKS_PLC] ;
    int blockadd[N_BLOCKS_PLC] ;
    int blocklen[N_BLOCKS_PLC] ;
    int PLCRack,PLCSlot ;
};
//----------------------------------------------------------------------------
extern TPLCThread *PLCThread[N_PLC];
//----------------------------------------------------------------------------
#endif
 