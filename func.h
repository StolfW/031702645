#pragma once
#include "IO.h"

string intToString(int x);
wstring stringToWString(const string& str);
string wstringToString(const wstring& str);
int searchFiles(const wstring& wstr, int curIdx, int curLev, int curId, string preId);
void trimJson();
//string to_utf8(const wchar_t* buffer, int len);
//string to_utf8(const wstring& str);
