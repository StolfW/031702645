#include <locale>
#include <codecvt>
#include <iostream>
#include "Addressbook.h"
#include "func.h"
#include "IO.h"
using namespace std;

int main(int argv, char** argc) {
	if (argv < 3) return 0;
	IO io;
	//io.open("1.txt");
	io.open(argc[1]);
	wofstream wfout;
	//wfout.open("2.txt", ios::app);
	wfout.open(argc[2], ios::app);
	wfout.imbue(locale(wfout.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, little_endian>));
	wfout << L"[";
	wfout.close();
	bool dot = 0;
	while (io.read(1000)) {
		if (dot) {
			//wfout.open("2.txt", ios::app);
			wfout.open(argc[2], ios::app);
			wfout.imbue(locale(wfout.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, little_endian>));
			wfout << L",";
			wfout.close();
		}
		dot = 1;
		Addressbook addressbook(io.toWString(1));
		addressbook.normalization();
		//addressbook.Export("2.txt");
		addressbook.Export(argc[2]);
	}
	//wfout.open("2.txt", ios::app);
	wfout.open(argc[2], ios::app);
	wfout.imbue(locale(wfout.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, little_endian>));
	wfout << L"]";
	wfout.close();
}
