#pragma once
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include "IO.h"
#include "func.h"
using namespace std;

struct Element {
	Element() {}
	Element(const string& fid, const string& id, const wstring& addr) : fid(fid), id(id), addr(addr) {}
	string fid, id;
	wstring addr;
};

class Addressbook {
public:
	Addressbook(const wstring& str);
	void init(const wstring& str);
	void normalization();
	bool separateName(wstring& str);
	bool separatePhoneNumber(wstring& str);
	void Export(const string& filename);
	vector<Element> search(const string& filename, const wstring& str);
	Element searchId(const string& filename, const string& str);
	vector<Element> sunday(const wstring& Text, const wstring& Pattern);
	Element sundayId(const wstring& Text, const string& Pattern);

private:
	bool missing[10];
	vector<Element> level;
	wstring origin, trimmed;
	wstring name;
	wstring phoneNumber;
	IO io;
	int* bit;
};