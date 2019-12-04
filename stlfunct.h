#pragma hdrstop
#ifndef _STLFUNCT_H
#define _STLFUNCT_H

#include <vector>
#include <list>

using namespace std;

//
// Visualizzazione dati in vector<int>
//
ostream& operator << (ostream& os, vector<int>& rhs);
ostream& operator << (ostream& os, vector<char>& rhs);

//
// Visualizzazione dati in deque<int>
//
ostream& operator << (ostream& os, deque<int>& rhs);

//
// Visualizzazione dati in deque<COMMAND>
//
//ostream& operator << (ostream& os, deque<COMMAND>& rhs);

/*void ShowData(vector<int>& rhs);
void ShowData(vector<char>& rhs);
void ShowData(deque<int>& rhs);
void ShowData(deque<COMMAND>& rhs);
*/
#endif

