//---------------------------------------------------------------------------

#ifndef plcphoenixthreadH
#define plcphoenixthreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "ethwin32.h"
#include <vector>

#define N_PLC 17;   //l'indice 0 non lo uso, non funziona l'assegnazione con questa variabile, non so perchè, nella plcthread del lidnodave funziona
#define input_module 2;
#define output_module 2;
#define READ_BYTE_LENGTH 20;
#define WRITE_BYTE_LENGTH 20;


class Phoenix_Thread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall Phoenix_Thread(int id,bool CreateSuspended);
	void StopThread();
	int ConnettiPhoenix() ;
	int LetturaIngressi() ;
	int LetturaUscite() ;
	int DisconnettiPhoenix();
	int ScriviPhoenix(int Bit,int add,bool alza) ;
	int Stopped;
	UINT8 IngressiPhoenix[20] ;
	UINT8 UscitePhoenix[20];
	IBDDIHND dtiHnd;
	IBDDIRET ddiRet ;
	T_DDI_DTI_ACCESS Write ;
	T_DDI_DTI_ACCESS Read ;
	T_DDI_DTI_ACCESS ReadUscite ;
	T_DDI_MXI_ACCESS Message ;
    unsigned char bit[8],bitAnd[8] ;
	int device,connesso ;
	bool devoscrivere,stabilisci_connessione,leggi_output ;
};
//---------------------------------------------------------------------------
extern Phoenix_Thread *PLCPhoenixThread[1];

#endif
