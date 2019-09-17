#include "IO.h"

bool IO::open(const char* filename) {
	if (fin) fin.close();
	fin.open(filename);
	if (!fin.good()) {
		if (fin) fin.close();
		return false;
	}
	return true;
}

bool IO::open(const string& filename) {
	if (fin) fin.close();
	fin.open(filename);
	if (!fin.good()) {
		if (fin) fin.close();
		return false;
	}
	return true;
}

bool IO::close() {
	if (fin) {
		fin.close();
		return true;
	}
	return false;
}

bool IO::read(const int len) {
	if (!fin) return false;
	buf = new char[len];
	if (fin >> buf) {
		return true;
	}
	fin.close();
	return false;
}

wstring IO::toWString(bool utf8) {
	if (!utf8) {
		setlocale(LC_ALL, "chs");
		const char* _Source = buf;
		size_t _Dsize = strlen(buf) + 1, cnt = 0;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs_s(&cnt, _Dest, _Dsize, _Source, _Dsize);
		wstring result = _Dest;
		delete[]_Dest;
		setlocale(LC_ALL, "C");
		delete[] buf;
		return result;
	}
	int len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, buf, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string str(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	size_t cnt = 0;
	setlocale(LC_ALL, "chs");
	const char* pts = str.c_str();
	size_t ns = str.size() + 1;
	wchar_t *ptd = new wchar_t[ns];
	wmemset(ptd, 0, ns);
	mbstowcs_s(&cnt, ptd, ns, pts, ns);
	wstring res = ptd;
	delete[] ptd;
	setlocale(LC_ALL, "C");
	//res.erase(0, 1);
	delete[] buf;
	return res;
}

void IO::output(const string& filename, const wstring& str) {
	wofstream wfout(filename, ios::app);
	wfout.imbue(locale(wfout.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, little_endian>));
	wfout << str;
	wfout.close();
}