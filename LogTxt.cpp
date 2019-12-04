//---------------------------------------------------------------------------
#pragma hdrstop

#include <vcl.h>
#include "stdio.h"
#include <System.IOUtils.hpp>
#include <System.SyncObjs.hpp>
#include "io.h"
#include "time.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
FILE *logfile;
AnsiString ultimo_log = "";
TCriticalSection *Lock = new TCriticalSection();
//---------------------------------------------------------------------------

int	ApriLog() {
	time_t t;
	struct tm *lt;
	AnsiString filename;
	char timestr[100];

	t = time(NULL);
	lt = localtime(&t);
	if (!DirectoryExists("Log")) {
		CreateDir("Log");
	}
	strftime(timestr, 100, "Log\\%Y%m%d.log", lt);
	filename = timestr;
	if ((logfile = fopen(filename.c_str(), "a+")) == NULL) {
		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------

void ChiudiLog()
{
	fflush(logfile);
	fclose(logfile);
}
//---------------------------------------------------------------------------

void LogTxt(char *s, ...)
{
	time_t t;
	struct tm *lt;
	char timestr[100];
	va_list argptr;
	AnsiString logstr;

	Lock->Acquire();
	try {
		va_start(argptr, s);
		logstr.vprintf(s, argptr);
		va_end(argptr);
		if (logstr != ultimo_log) {
			if (ApriLog() == 0) {
				t = time(NULL);
				lt = localtime(&t);
				strftime(timestr, 100, "%H:%M:%S", lt);
				fprintf(logfile, "[%s] %s\n", timestr, logstr.c_str());
				ChiudiLog();
				ultimo_log = "";
			}
        }
	} catch (...) {}
	Lock->Release();
}
//---------------------------------------------------------------------------

void CancellaLogVecchi(int giorni = 14)
{
	int i, res = 1;
	TStringDynArray files;
	time_t t;
	char timestr[100];
	struct tm *lt;
	AnsiString FileName, Limit;
	double secondi;

	secondi = giorni * 24 * 60 * 60;
	t = time(NULL) - secondi;
	lt = localtime(&t);
	strftime(timestr, 100, "%Y%m%d.log", lt);
	Limit = timestr;

	if (!DirectoryExists("Log")) {
		return;
	}
	files = TDirectory::GetFiles("Log", "*.*", TSearchOption::soTopDirectoryOnly);
	for (int i = 0; i < files.Length; i++) {
		FileName = ExtractFileName(files[i]);
		if (FileName <  Limit) {
        	DeleteFile(files[i]);
		}
	}
}
//---------------------------------------------------------------------------

