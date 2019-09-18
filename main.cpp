#include "Addressbook.h"
#include "func.h"
#include "IO.h"

string infile = "1.txt", outfile = "2.txt";
wstring OUTPUT;

int main(int argv, char** argc) {
	if (argv < 3) return 0;
	infile = argc[1], outfile = argc[2];
	IO io;
	io.open(infile);
	OUTPUT.push_back(L'[');
	bool dot = 0;
	while (io.read(1000)) {
		if (dot) OUTPUT.push_back(L',');
		dot = 1;
		Addressbook addressbook(io.toWString(1));
		addressbook.normalization();
		addressbook.Export(OUTPUT);
	}
	OUTPUT.push_back(L']');
	io.output(outfile, OUTPUT);
}
