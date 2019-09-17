#include "func.h"

const string tableFileName = "Tables\\";
wofstream wfout[10][10], Fout;

//string to_utf8(const wchar_t* buffer, int len) {
//	int nChars = ::WideCharToMultiByte(
//		CP_UTF8,
//		0,
//		buffer,
//		len,
//		NULL,
//		0,
//		NULL,
//		NULL);
//	if (nChars == 0)return"";
//
//	string newbuffer;
//	newbuffer.resize(nChars);
//	::WideCharToMultiByte(
//		CP_UTF8,
//		0,
//		buffer,
//		len,
//		const_cast<char*>(newbuffer.c_str()),
//		nChars,
//		NULL,
//		NULL);
//
//	return newbuffer;
//}
//
//string to_utf8(const wstring& str) {
//	return to_utf8(str.c_str(), (int)str.size());
//}

string intToString(int x) {
	if (x == 0) return "0";
	string res;
	while (x) {
		res.push_back(x % 10 + '0');
		x /= 10;
	}
	reverse(res.begin(), res.end());
	return res;
}
wstring stringToWString(const string& str) {
	int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
	wstring w_str(wide);
	delete[] wide;
	return w_str;
}
string wstringToString(const wstring& str) {
	unsigned len = str.size() * 4;
	size_t cnt = 0;
	setlocale(LC_CTYPE, "");
	char *p = new char[len];
	wcstombs_s(&cnt, p, len, str.c_str(), len);
	string str1(p);
	delete[] p;
	return str1;
}

int searchFiles(const wstring& wstr, int curIdx, int curLev, int curId, string preId) {
	for (int i = curIdx; i < (int)wstr.size(); i++) {
		if (wstr[i] == '[') {
			string nowId;
			if (curLev) nowId = preId + "." + intToString(curId - 1);
			else nowId = "0";
			for (int a = 0; a <= 3 - curLev; a++) {
				if (wfout[curLev][a]) wfout[curLev][a].close();
				wfout[curLev][a].open(tableFileName + nowId + "a" + intToString(a) + ".in");
				wfout[curLev][a].imbue(locale("chs"));
				wfout[curLev][a] << L';';
			}
			i = searchFiles(wstr, i + 1, curLev + 1, 0, nowId);
		}
		else if (wstr[i] == ']') {
			return i;
		}
		else {
			if (wstr[i] > 255) {
				int len = 0;
				wstring addr;
				while (wstr[i + len] > 255) {
					addr.push_back(wstr[i + len++]);
				}
				wstring nowId = stringToWString(preId) + L"." + stringToWString(intToString(curId));
				for (int a = curLev - 1; a >= 0; a--) {
					wfout[a][curLev - a - 1] << stringToWString(preId) << addr << nowId << L';';
				}
				curId++;
				i += len;
			}
		}
	}
	return -1;
}

void trimJson() {
	//wcout.imbue(locale("chs"));
	IO io;
	io.open("a.json");
	io.read();
	searchFiles(io.toWString(1), 0, 0, 0, "");
}