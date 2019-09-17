#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
using namespace std;

const int maxLen = 5000000;

class IO {
public:
	bool open(const char* filename);
	bool open(const string& filename);
	bool close();
	bool read(const int len = maxLen);
	wstring toWString(bool utf8);
private:
	char *buf;
	ifstream fin;
};