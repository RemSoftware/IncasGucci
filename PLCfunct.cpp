//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PLCfunct.h"

//---------------------------------------------------------------------------

int DBD(char *buffer) {
	return (((unsigned char)buffer[0] << 24) |
			((unsigned char)buffer[1] << 16) |
			((unsigned char)buffer[2] << 8) |
			(unsigned char)buffer[3]);
}

int DBW(char *buffer) {
	return (((unsigned char)buffer[0] << 8) |
			(unsigned char)buffer[1]);
}

void FormatDBD(int n, char *buffer) {
	buffer[0] = (n & BYTE3) >> 24;
	buffer[1] = (n & BYTE2) >> 16;
	buffer[2] = (n & BYTE1) >> 8;
	buffer[3] = n & BYTE0;
}
void FormatDDBD(unsigned long long n, char *buffer) {
	buffer[0] = (n & LONGBYTE7) >> 56;
	buffer[1] = (n & LONGBYTE6) >> 48;
	buffer[2] = (n & LONGBYTE5) >> 40;
	buffer[3] = (n & LONGBYTE4) >> 32;
	buffer[4] = (n & LONGBYTE3) >> 24;
	buffer[5] = (n & LONGBYTE2) >> 16;
	buffer[6] = (n & LONGBYTE1) >> 8;
	buffer[7] = n & LONGBYTE0;
}
unsigned long long ReturnLongDaBuffer(char *buffer) {
unsigned long long res = 0 ;
	res = LONGBYTE7 ;
	res = ~LONGBYTE7 ;
	res = (buffer[7] & ~LONGBYTE0);
	res += ((buffer[6] & ~LONGBYTE1) << 8);
	res += ((buffer[5] & ~LONGBYTE2) << 16);
	res += ((buffer[4] & ~LONGBYTE3) << 24);
	res += ((buffer[3] & ~LONGBYTE4) << 32);
	res += ((buffer[2] & ~LONGBYTE5) << 40);
	res += ((buffer[1] & ~LONGBYTE6) << 48);
	res += (buffer[0] & ~LONGBYTE7) << 56;
	return res ;
}

void FormatDBW(int n, char *buffer) {
	buffer[0] = (n & BYTE1) >> 8;
	buffer[1] = n & BYTE0;
}

char GetBit(unsigned char value, unsigned char bit) {
	return (value & (1 << bit));
}

double GetTimer(char *buffer) {
	double factor, res = 0;
	int v[3], base;

	base = (buffer[0] & 0x30) >> 4;
	switch(base) {
	case 0:
		// 10ms
		factor = 0.01;
		break;
	case 1:
		// 100ms
		factor = 0.1;
		break;
	case 2:
		// 1s
		factor = 1.0;
		break;
	case 3:
		// 10s
		factor = 10.0;
	}
	v[0] = (buffer[0] & 0x0F);
	v[1] = (buffer[1] & 0xF0) >> 4;
	v[2] = (buffer[1] & 0x0F);
	res = (v[0] * 100 + v[1] * 10 + v[2]) * factor;
	return res;
}

void FormatTimer(double t, char *buffer) {
	int v[3], base, rate, n, n2;
	AnsiString s;

	n2 = t * 10.0 * 10.0 + 0.1;
	s = IntToStr(n2);
	s = s.SubString(1, 3);
	while (s.Length() < 3) {
		s = "0" + s;
	}
	v[0] = s[1] - '0';
	v[1] = s[2] - '0';
	v[2] = s[3] - '0';
	n = s.ToIntDef(0);
	rate = n2 / n;
	if (rate == 1) {
		base = 0; // 10ms
	} else if (rate == 10) {
		base = 1; // 100ms
	} else if (rate == 100) {
		base = 2; // 1s
	} else if (rate == 1000) {
		base = 3; // 10s
	}
	buffer[0] = (base << 4) + v[0];
	buffer[1] = (v[1] << 4) + v[2];
}


