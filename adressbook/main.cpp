#include <bits/stdc++.h>
#include <locale>
#include <codecvt>
#include <Windows.h>
#define rep(i,a,b) for(int i=(a);i<(b);i++)
#define per(i,a,b) for(int i=(b)-1;i>=(a);i--)
#define ll long long
using namespace std;
int wei[65536];
int ans = 0, lend, lenc;
wstring c, d;
wofstream fout;
std::wstring StringToWString(const std::string& str) {
	size_t convertedChars = 0;
	setlocale(LC_ALL, "chs");
	const char* point_to_source = str.c_str();
	size_t new_size = str.size() + 1;
	wchar_t *point_to_destination = new wchar_t[new_size];
	wmemset(point_to_destination, 0, new_size);
	mbstowcs_s(&convertedChars, point_to_destination, new_size, point_to_source, new_size);
	std::wstring result = point_to_destination;
	delete[] point_to_destination;
	setlocale(LC_ALL, "C");
	return result;
}
std::string WStringToString(const std::wstring &wstr) {
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	if (nResult == 0) {
		return "";
	}
	return str;
}
string Utf8ToGbk(const char *src_str) {
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
void pei() {
	int w = 0;
	while (w + lend <= lenc) {
		int i = 0;
		bool f = false;
		while (i <= lend && !f) {
			if (c[i + w] != d[i])
				f = true;
			i++;
		}
		if (!f) {
			//cout << w << endl;
			wstring ws;
			for (int k = w + i; c[k] != 34; k--) ws = c[k] + ws;
			for (int k = w + i + 1; c[k] != 34; k++) ws.push_back(c[k]);
			fout << ws << endl;
			w++;
		}
		else {
			i = lend + 1;
			if (wei[c[i + w]] == -1)
				w = w + i + 1;
			else
				w = w + i - wei[c[w + i]];
		}
	}
	return;
}
char *buf;
const int maxlen = 2000000;
int main(int argv, char** argc) {
	if (argv < 3) return 0;
	ios::sync_with_stdio(false);
	wcout.imbue(locale("chs"));
	buf = new char[maxlen];

	ifstream fin;
	fin.open(argc[1]);
	ofstream fout;
	fout.open(argc[2]);
	string str((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	// 关闭打开的文件
	fout << str;
	fin.close();
	fout.close();
	/*ifstream fin("address.json");
	fin >> buf;
	c = StringToWString(Utf8ToGbk(buf));
	fin.close();
	fin.open("11.txt");
	fin >> buf;
	d = StringToWString(Utf8ToGbk(buf));
	fin.close();
	d = d.substr(1, d.size() - 1);
	lenc = c.size() - 1;
	lend = d.size() - 1;
	for (int i = 0; i < 65536; ++i)
		wei[i] = -1;
	for (int i = 0; i <= lend; ++i)
		wei[d[i]] = i;
	fout.imbue(locale("chs"));
	fout.open("111.txt");
	pei();
	fout.close();*/
	system("pause");
}
/*
福建省。北京市。福州市。安徽省。浙江省。河南省。河北省。列东街道。山东省。闽侯县。福州大学。
福州
*/