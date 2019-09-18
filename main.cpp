#include "Addressbook.h"
#include "func.h"
#include "IO.h"

string infile = "1.txt", outfile = "2.txt";

int main(int argv, char** argc) {
	if (argv < 3) return 0;
	infile = argc[1], outfile = argc[2];
	IO io;
	io.open(infile);
	io.output(outfile, L"[");
	bool dot = 0;
	while (io.read(1000)) {
		if (dot) io.output(outfile, L",");
		dot = 1;
		Addressbook addressbook(io.toWString(1));
		addressbook.normalization();
		addressbook.Export(outfile);
	}
	io.output(outfile, L"]");
}
