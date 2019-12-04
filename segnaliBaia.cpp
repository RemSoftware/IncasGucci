//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "segnaliBaia.h"
#include "plcphoenixthread.h"
//#include "dmphoenix.h"
#include "db.h"
#include "main.h"
//#include "SocketComunicazioneClient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma resource "*.dfm"
TfSegnaliBaia *fSegnaliBaia;
extern char N_carrelli ;
//---------------------------------------------------------------------------
__fastcall TfSegnaliBaia::TfSegnaliBaia(TComponent* Owner)
	: TMDIChild(Owner)
{
}
//---------------------------------------------------------------------------
void TfSegnaliBaia::ColoraShape(int stato, TShape *t, TColor ColorON, TColor ColorOFF)
{
	if (stato) {
		t->Brush->Color = ColorON;
	} else {
		t->Brush->Color = ColorOFF;
	}
}
//---------------------------------------------------------------------------

void TfSegnaliBaia::ComandaPresenza(int modulo,int nled,int stato,bool input) {
    TShape *Led ;
    bool led_trovato ;
	led_trovato = false ;
	if (input) {
		Led = (TShape *) FindComponent("sInputLed"+IntToStr(modulo)+"_"+IntToStr(nled));
		if (Led!= NULL)
			led_trovato = true ;
	}else {
		Led = (TShape *) FindComponent("sOutputLed"+IntToStr(modulo)+"_"+IntToStr(nled));
		if (Led!= NULL)
			led_trovato = true ;
	}

	if (led_trovato)
		ColoraShape(stato, Led);
}
void __fastcall TfSegnaliBaia::sOutputLed1_0MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    TShape *Led ;
    int comando = 0 ;
	// forzo uscita su plc phoenix
    
	if (MainForm->pwdlevel) {
		//posso forzare i segnali
		MainForm->PlcRobot.forzatura_attiva = 1 ; //mi segno che sto forzando segnali
		Led = (TShape *) Sender ;
		if (Led != NULL) {
			if (Led->Brush->Color == clGray)
				comando = 1 ;
		   /*	if ((Led->Tag < 4)&&(StrToInt(Led->Hint) == 3))
			 -	MainForm->PlcPhoenix[baia].impulso_apri_chiudi_porta[Led->Tag+1] = 3; //impulso standard perchè non so se apri o chiudi così dopo 10 sec lo resetto*/

			if ((MainForm->PlcRobot.PlcConnesso )||(MainForm->Simula)) {
				//DataModulePhoenix->ScriviPhoenix(Led->Tag,StrToInt(Led->Hint),comando,baia) ;
				PLCPhoenixThread[0]->ScriviPhoenix(Led->Tag,StrToInt(Led->Hint),comando) ;
				dmDB->LogMsg("Forzato Segnale su Baia "+IntToStr(baia)+" , Nome Segnale : "+Led->Name +" , valore : "+IntToStr(comando));
			}

		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfSegnaliBaia::Timer1Timer(TObject *Sender)
{
	int i,j ;
	Timer1->Enabled = false;
	activate = 1 ;
	for(i=1 ; i<=4 ; i++) {
		for(j=0 ; j<=7 ; j++) {
			ComandaPresenza(i,j,MainForm->PlcRobot.input[i] & MainForm->bit[j],true) ;
			ComandaPresenza(i,j,MainForm->PlcRobot.output[i] & MainForm->bit[j],false) ;
		}

	}
	activate = 0 ;
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------




void __fastcall TfSegnaliBaia::FormActivate(TObject *Sender)
{
	int i,j ;
	activate = 1 ;
	forzadati = 0 ;
	TPanel *Pan ;
    
	baia = 1 ;    //FISSO
    
	for(i = 1 ; i<=4 ; i++) {
		for(j = 0 ; j<= 7 ; j++) {
			Pan = (TPanel *) FindComponent("pName"+IntToStr(i)+"_"+IntToStr(j));
			if (Pan != Null) {
				Pan->Caption = MainForm->PlcRobot.InputName[i][j];
			}
			Pan = (TPanel *) FindComponent("pNameOutput"+IntToStr(i)+"_"+IntToStr(j));
			if (Pan != Null) {
				Pan->Caption = MainForm->PlcRobot.OutputName[i][j];
			}
		}
	}
	Timer1Timer(this);
}
//---------------------------------------------------------------------------

void __fastcall TfSegnaliBaia::sInputLed1_0MouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
TShape *Led ;
int comando = 0 ;
	// forzo uscita su plc phoenix
	if ((MainForm->pwdlevel)||(MainForm->Simula)) {
		//posso forzare i segnali
		Led = (TShape *) Sender ;
		if (Led != NULL) {
			if (Led->Brush->Color == clGray)
				comando = 1 ;
			if (comando)
				MainForm->PlcRobot.input[StrToInt(Led->Hint)] |= MainForm->bit[Led->Tag];
			else
				MainForm->PlcRobot.input[StrToInt(Led->Hint)] &= MainForm->bitAnd[Led->Tag];
			MainForm->PlcRobot.forzatura_input = 1 ;

		}
	}

}
//---------------------------------------------------------------------------



void __fastcall TfSegnaliBaia::TabControl1Change(TObject *Sender)
{
	//MainForm->baiaformSegnali = TabControl1->TabIndex + 1 ;
	FormActivate(this);
}
//---------------------------------------------------------------------------


void __fastcall TfSegnaliBaia::CheckBox1Click(TObject *Sender)
{
    /*
    int i,abilita_timer = 0  ;
	TimerTestPlc->Enabled = false ;
	for (i=1 ; i<= MainForm->num_moduli_phoenix ; i++) {
		if (baia != i)
			MainForm->PlcRobot[i].test_lampade = 0 ;
		else {
			MainForm->PlcRobot[i].test_lampade = CheckBox1->Checked ;
			abilita_timer = 1 ;
		}
	}
	TimerTestPlc->Enabled = abilita_timer ;
    */
}
//---------------------------------------------------------------------------


void __fastcall TfSegnaliBaia::TimerTestPlcTimer(TObject *Sender)
{
int i,k,j,add_uscita_alta,bit_uscita_alta,add;
AnsiString str_messaggio_display ;
	// modifico uscite su plc ,accendo una alla volta tutte le uscite in ordine a tempo
 /*	add_uscita_alta = 0;
	bit_uscita_alta = 8 ;
	TimerTestPlc->Enabled = false ;
	for (i=1 ; i<= MainForm->num_moduli_phoenix ; i++) {
		//scorro tutte le uscite e salvo la prima alta
		if (MainForm->PlcRobot[i].test_lampade) { // abilito test per ogni
			for (k = 1 ; k < 3 ; k++) {
				for (j = 0 ; j < 8 ; j++) {
					if (MainForm->PlcRobot[i].PlcConnesso) {
						if (MainForm->PlcRobot[i].output[k] & MainForm->bit[j]) {
							add_uscita_alta = k;
							bit_uscita_alta = j ;
							break ;
						}
					}
				}
			}
			//se non ho uscite alte parto dalla prima
			if ((!add_uscita_alta)&&(bit_uscita_alta == 8)) {
				add_uscita_alta = 1;
				bit_uscita_alta = 0 ;
			}else {
			//passo all'uscita successiva
				if ((bit_uscita_alta == 7)||((bit_uscita_alta == 3)&&(add_uscita_alta == 1))) {
					if (add_uscita_alta == 2)
						add_uscita_alta = 1 ;
					else
						add_uscita_alta = 2 ;
					bit_uscita_alta = 0 ;
				}else
					bit_uscita_alta++ ;
			}
			if (add_uscita_alta) {
				for (k = 1 ; k < 3 ; k++) {
					for (j = 0 ; j < 8 ; j++) {
						if (MainForm->PlcRobot[i].PlcConnesso) {
							if (MainForm->PlcRobot[i].output[k] & MainForm->bit[j]) {
								if ((add_uscita_alta != k) ||(bit_uscita_alta != j)) {
									if (k == 2)
										add = 3 ;
									else
										add = k ;
									//DataModulePhoenix->ScriviPhoenix(j,add,false,i) ;
                                    if (dmDB->ServerFlag)
									    PLCPhoenixThread[i]->ScriviPhoenix(j,add,false) ;
                                    else
                                        SocketDataModuleClient->ScriviPhoenixClient(i,add,j, false) ;
								}
							}else if ((!(MainForm->PlcRobot[i].output[k] & MainForm->bit[j])) &&
									(add_uscita_alta == k) &&(bit_uscita_alta == j)) {
								   //
								if (k == 2)
									add = 3 ;
								else
									add = k ;
								//DataModulePhoenix->ScriviPhoenix(j,add,true,i) ;
                                if (dmDB->ServerFlag)
								    PLCPhoenixThread[i]->ScriviPhoenix(j,add,true) ;
                                else
                                    SocketDataModuleClient->ScriviPhoenixClient(i,add,j, true) ;
							}
						}
					}
				}
			}
		}
	}          */
	// modifico i registri del display
/*	for (i=1 ; i<= num_stazioni ; i++) {
		str_messaggio_display = "Staz "+IntToStr(i)+" msg "+IntToStr(DatiDisplay[i].nmessaggio);
		DatiDisplay[i].nmessaggio++ ;
		ConvertiStringaDisplay(str_messaggio_display,14,i);
	}   */
	TimerTestPlc->Enabled = true ;


}
//---------------------------------------------------------------------------











