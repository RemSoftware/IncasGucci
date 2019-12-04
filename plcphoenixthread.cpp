//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "plcphoenixthread.h"
#include "main.h"
#include "db.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Phoenix_Thread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
Phoenix_Thread *PLCPhoenixThread[1];
char OPEN_DTI[20] = "IBETH01N1_D";
//char OPEN_MXI[20] = "IBETH01N1_M";

__fastcall Phoenix_Thread::Phoenix_Thread(int id,bool CreateSuspended)
	: TThread(CreateSuspended)
{
	device = id ;
	NameThreadForDebugging(((AnsiString)"PLCPhoenixThread" + IntToStr(id)), ThreadID);
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
    bitAnd[2] = 0xFB ;
    bitAnd[3] = 0xF7;
    bitAnd[4] = 0xEF;
    bitAnd[5] = 0xDF;
    bitAnd[6] = 0xBF;
	bitAnd[7] = 0x7F;
	Stopped = 0;
	connesso = 0;
	devoscrivere = false ;
	leggi_output = true ;
	stabilisci_connessione = true ;
}
//---------------------------------------------------------------------------
void __fastcall Phoenix_Thread::Execute()
{
	//---- Place thread code here ----
	Sleep(5000);
	while (!Stopped) {
		try {
			if (!connesso) { // aspetto un po dopo l'avvio per connetteere i thread
				if (stabilisci_connessione) {
					connesso = ConnettiPhoenix();
					stabilisci_connessione = false ;
					Sleep(200);
				}else {
					DisconnettiPhoenix();
					Sleep(5000);
					stabilisci_connessione = true ;
				}
			}
			if (connesso) {
				// Lettura dati da PLC

				if (!devoscrivere) {
					connesso = LetturaIngressi();
					Sleep(100);
                }
				if (devoscrivere) {
					ddiRet = DDI_DTI_WriteData (dtiHnd, &Write);
					if (ddiRet != ERR_OK) {
						//connesso = 0 ;
						error_scrittura = error_scrittura + 1 ;
						if (error_scrittura > 10 ) {
							connesso = 0 ;
							error_scrittura = 0;
						}
					}else {
						devoscrivere = false ;
						error_scrittura = 0 ;
						//leggi_output = true ;
					}

				}else {
					//if (leggi_output) {
						connesso = LetturaUscite();
						leggi_output = false ;
					//}
				}
			   /*	if ((connesso)&&(!MainForm->PlcPhoenix[device].forzatura_input)) {
					MainForm->PlcPhoenix[device].input[1] = IngressiPhoenix[1] ;
					MainForm->PlcPhoenix[device].input[2] = IngressiPhoenix[3] ;
				} */
				/*MainForm->PlcPhoenix[device].output[1] = UscitePhoenix[1] ;
				MainForm->PlcPhoenix[device].output[2] = UscitePhoenix[3] ; */


			}
			//MainForm->PlcPhoenix[device].PlcConnesso = connesso ;
			Sleep(50);
		} catch (...) {}
	}
	DisconnettiPhoenix();
}
//---------------------------------------------------------------------------
int Phoenix_Thread::LetturaIngressi()
{
    // LETTURA DATI PLC PHOENIX CONTACT , leggo tutti gli ingressi
	Read.length = READ_BYTE_LENGTH ;  // lunghezza byte da leggere
    Read.address = 0 ;  // numero indirizzo da leggere
	Read.dataCons = DTI_DATA_BYTE ; // tipo lettura ;
	Read.data = IngressiPhoenix ;
	ddiRet = DDI_DTI_ReadData (dtiHnd, &Read);
	if (ddiRet != ERR_OK) {
		return 0;
	}else {
		return 1 ; // disconnessione OK
	}
}
int Phoenix_Thread::LetturaUscite()
{
	ReadUscite.length = WRITE_BYTE_LENGTH;  // lunghezza byte da leggere
	ReadUscite.address = 0 ;  // numero indirizzo da leggere
	ReadUscite.dataCons = DTI_DATA_BYTE|IB_TO_REMOTE_DTA|IB_NODE_1; // tipo lettura uscite;
	ReadUscite.data = UscitePhoenix ;
	ddiRet = DDI_DTI_ReadData (dtiHnd, &ReadUscite);
	if (ddiRet != ERR_OK) {
		return 0;
	}else {
		if (leggi_output)
			memcpy(UscitePhoenixDaScrivere,UscitePhoenix,sizeof(UscitePhoenixDaScrivere)) ;
		return 1 ; // disconnessione OK
	}
}

void Phoenix_Thread::StopThread() {
	TInterlocked::Exchange(Stopped, 1);
	/* Wait for this thread to finish executing */
	this->WaitFor();
	dmDB->LogMsg("Thread terminato per device "+IntToStr(device));
}
int Phoenix_Thread::ConnettiPhoenix() {
    IBDDIRET ret ;

    //connetto al plc phoenix passato per parametro (se ho più di un phoenix da leggere)
	if (device< 10)
		sprintf(OPEN_DTI,"%s%d%s","IBETH0",device,"N1_D");
	else
		sprintf(OPEN_DTI,"%s%d%s","IBETH",device,"N1_D");
	ret = DDI_DevOpenNode(OPEN_DTI,DDI_RW,&dtiHnd);
	if (ret != ERR_OK) {
		//dmDB->LogMsg("Comunicazione su Phoenix non connessa.");
		return 0;
    }else {
		//dmDB->LogMsg("Connesso con Phoenix id "+IntToStr(device)+" Handle : "+dtiHnd);
		//dmDB->LogMsg("Comunicazione su Phoenix connessa.");
		return 1 ; // connessione OK
	}
}
int Phoenix_Thread::DisconnettiPhoenix() {
	IBDDIRET ret ;

    //disconnetto al plc phoenix passato per parametro (se ho più di un phoenix da leggere)
	//dmDB->LogMsg("Disconnessione con Phoenix id "+IntToStr(device));
	ret = DDI_DevCloseNode(dtiHnd);
	if (ret != ERR_OK) {
		return 0;
	}else {
		return 1 ; // disconnessione OK
	}
}
int Phoenix_Thread::ScriviPhoenix(int Bit,int add,bool alza) {
// procedura per scrivere le uscite del phoenix, tengo in memoria lo stato delle uscite
// con add 0 alzo le uscite 5.1 - 6.1 - 5.4. - 6.4 - 7.1 - 8.1 - 7.4 - 8.4

		Write.length = WRITE_BYTE_LENGTH ;  // lunghezza byte da leggere
		Write.address = 0x00 ;  // numero indirizzo da leggere
		Write.dataCons = DTI_DATA_BYTE ; // tipo scrittura;
		if (alza)  // alzo l'uscita o l'abbasso
			UscitePhoenixDaScrivere[add] = UscitePhoenixDaScrivere[add] | bit[Bit] ;
		else
			UscitePhoenixDaScrivere[add] = ((UscitePhoenixDaScrivere[add]) & bitAnd[Bit] );
		if (connesso) {
			devoscrivere = true ; // forse conviene scrivere nel thread
			Write.data = UscitePhoenixDaScrivere ;
			//ddiRet = DDI_DTI_WriteData (dtiHnd, &Write);
		}/*else if (MainForm->Simula) {
			MainForm->PlcRobot[device].output[1] = UscitePhoenix[1] ;
			MainForm->PlcRobot[device].output[2] = UscitePhoenix[3] ;
			MainForm->PlcRobot[device].output[3] = UscitePhoenix[5] ;
			MainForm->PlcRobot[device].output[4] = UscitePhoenix[7] ;
		} */
		if (ddiRet != ERR_OK) {
			return 0;
		}else {
			return 1 ; //  OK
		}
}

