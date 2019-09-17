#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <locale>
#include <codecvt>
using namespace std;

const int maxLen = 5000000;

class IO {
public:
	bool open(const char* filename);
	bool open(const string& filename);
	bool close();
	bool read(const int len = maxLen);
	void output(const string& filename, const wstring& str);
	wstring toWString(bool utf8);
private:
	char *buf;
	ifstream fin;
};