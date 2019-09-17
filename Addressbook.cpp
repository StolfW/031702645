#include "Addressbook.h"

const wstring road = {
	L'路', L'道', L'街', L'巷', L'弄'
};

Addressbook::Addressbook(const wstring& str) : origin(str) {
	this->name.clear();
	this->phoneNumber.clear();
	level.resize(10);
	for (int i = 0; i < 10; i++) {
		this->missing[i] = 0;
	}
	origin.pop_back();
}

void Addressbook::init(const wstring& str) {
	this->origin = str;
	this->name.clear();
	this->phoneNumber.clear();
	level.resize(10);
	for (int i = 0; i < 10; i++) {
		this->missing[i] = 0;
	}
	origin.pop_back();
}

void Addressbook::normalization() {
	trimmed = origin;
	separateName(trimmed);
	separatePhoneNumber(trimmed);
	wstring tmp = trimmed.substr(0, 2);
	if (tmp == L"北京" || tmp == L"上海" || tmp == L"重庆" || tmp == L"天津") {
		trimmed.erase(0, 2);
		if (trimmed[0] == L'市') {
			trimmed = tmp + tmp + trimmed;
		}
		else {
			trimmed = tmp + tmp + L"市" + trimmed;
		}
	}
	int curLev = 0, curIdx = 0;
	wstring now;
	string curFile = "0a0";
	for (int i = 0; i < (int)trimmed.size(); i++) {
		if (curLev > 3) {
			curIdx = i;
			break;
		}
		now.push_back(trimmed[i]);
		vector<Element> fnd = search(curFile, now);
		if (fnd.size() == 1) {
			int k = i, cnt = (int)fnd[0].addr.size() - now.size();
			bool fail = 0;
			for (int j = 0; j < cnt; j++) {
				if (trimmed[i + j + 1] == fnd[0].addr[now.size() + j]) {
					k++;
				}
				else {
					if (cnt == 1 && (fnd[0].addr.back() == L'省' || fnd[0].addr.back() == L'市')) {
						continue;
					}
					else {
						fail = 1;
					}
				}
			}
			if (fail) {
				continue;
			}
			level[curLev++] = fnd[0];
			curFile.pop_back(); curFile.pop_back();
			curFile = fnd[0].id + "a0";
			i = k;
			now.clear();
		}
		else if (fnd.empty()) {
			i -= now.size();
			now.clear();
			missing[curLev++] = 1;
			curFile[curFile.size() - 1]++;
		}
	}
	int roadPos = 0, numPos = 0;
	bool hasRoad = 0, hasNum = 0;
	for (int i = trimmed.size() - 1; i >= curIdx && !hasRoad; i--) {
		for (int j = 0; j < (int)road.size() && !hasRoad; j++) {
			if (road[j] == trimmed[i]) {
				hasRoad = 1;
				roadPos = i;
			}
		}
	}
	for (int i = trimmed.size() - 1; i >= curIdx && !hasNum; i--) {
		if (trimmed[i] == L'号' && i && trimmed[i - 1] >= L'0' && trimmed[i - 1] <= L'9') {
			hasNum = 1;
			numPos = i;
		}
	}
	if (!hasRoad) missing[4] = 1, missing[5] = 1;
	else if (!hasNum) missing[5] = 1;
	if (hasRoad && hasNum) {
		level[4] = Element("", "", trimmed.substr(curIdx, roadPos - curIdx + 1));
		level[5] = Element("", "", trimmed.substr(roadPos + 1, numPos - roadPos));
		level[6] = Element("", "", trimmed.substr(numPos + 1, trimmed.size() - numPos - 1));
	}
	else if (hasRoad) {
		level[4] = Element("", "", trimmed.substr(curIdx, roadPos - curIdx + 1));
		level[5] = Element("", "", L"");
		level[6] = Element("", "", trimmed.substr(roadPos + 1, trimmed.size() - roadPos - 1));
	}
	else {
		level[4] = Element("", "", L"");
		level[5] = Element("", "", L"");
		level[6] = Element("", "", trimmed.substr(curIdx, trimmed.size() - curIdx));
	}
	for (int i = 2; i >= 0; i--) {
		if (missing[i] && level[i + 1].fid.size()) {
			string fid = level[i + 1].fid;
			while (fid.back() != '.') fid.pop_back();
			fid.pop_back();
			level[i] = searchId(fid + "a0", level[i + 1].fid);
		}
	}
	for (int i = 0; i < 3; i++) {
		if (level[i].addr == L"直辖市" || level[i].addr == L"省直辖县级行政区划" || level[i].addr == L"自治区直辖县级行政区划") {
			level[i].addr.clear();
		}
	}
}
Element Addressbook::searchId(const string& filename, const string& str) {
	io.open("Tables\\" + filename + ".in");
	io.read();
	return sundayId(io.toWString(0), str);
}

Element Addressbook::sundayId(const wstring& Text, const string& Pattern) {
	bit = new int[100000]();
	for (int i = 0; i < (int)Pattern.size(); ++i)
		bit[Pattern[i]] = i + 1;
	int w = 0;
	while (w + Pattern.size() <= Text.size()) {
		int i = 0;
		bool f = false;
		while (i < (int)Pattern.size() && !f) {
			if (Text[i + w] != Pattern[i])
				f = true;
			i++;
		}
		if (!f) {
			wstring ws, wfid;
			string fid;
			int pos = w - 1;
			while (Text[pos] > 255) {
				ws = Text[pos] + ws;
				pos--;
			}
			while (Text[pos] != ';') {
				wfid.push_back(Text[pos]);
				pos--;
			}
			fid = wstringToString(wfid);
			reverse(fid.begin(), fid.end());
			return (Element(fid, "", ws));
		}
		else {
			i = Pattern.size();
			if (bit[Text[i + w]] == 0)
				w = w + i + 1;
			else
				w = w + i - bit[Text[w + i]] + 1;
		}
	}
	return Element();
}

vector<Element> Addressbook::search(const string& filename, const wstring& str) {
	io.open("Tables\\" + filename + ".in");
	io.read();
	return sunday(io.toWString(0), str);
}

bool Addressbook::separateName(wstring& str) {
	int i = 0;
	while (str[i] != '!') i++; i++;
	while (str[i] > 255) name.push_back(str[i++]);
	str.erase(0, i + 1);
	return !name.empty();
}

bool Addressbook::separatePhoneNumber(wstring& str) {
	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] >= L'0' && str[i] <= L'9') {
			int len = 1;
			while (i + len - 1 < (int)str.size() && str[i + len - 1] >= L'0' && str[i + len - 1] <= L'9') len++; len--;
			if (len == 11) {
				phoneNumber = str.substr(i, len);
				str.erase(i, len);
				break;
			}
			i += len - 1;
		}
	}
	return ((int)phoneNumber.size() == 11);
}

vector<Element> Addressbook::sunday(const wstring& Text, const wstring& Pattern) {
	vector<Element> fnd;
	bit = new int[100000]();
	for (int i = 0; i < (int)Pattern.size(); ++i)
		bit[Pattern[i]] = i + 1;
	int w = 0;
	while (w + Pattern.size() <= Text.size()) {
		int i = 0;
		bool f = false;
		while (i < (int)Pattern.size() && !f) {
			if (Text[i + w] != Pattern[i])
				f = true;
			i++;
		}
		if (!f) {
			wstring ws;
			string fid, id;
			int pos = w;
			while (Text[pos] != ';') {
				if (Text[pos] > 255) ws = Text[pos] + ws;
				else fid.push_back((char)Text[pos]);
				pos--;
			}
			pos = w + 1;
			while (Text[pos] != ';') {
				if (Text[pos] > 255) ws.push_back(Text[pos]);
				else id.push_back((char)Text[pos]);
				pos++;
			}
			reverse(fid.begin(), fid.end());
			fnd.push_back(Element(fid, id, ws));
			if (fnd.size() > 1) return fnd;
			w++;
		}
		else {
			i = Pattern.size();
			if (bit[Text[i + w]] == 0)
				w = w + i + 1;
			else
				w = w + i - bit[Text[w + i]] + 1;
		}
	}
	return fnd;
}

void Addressbook::Export(const string& filename) {
	wofstream wfout(filename, ios::app);
	wfout.imbue(locale(wfout.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, little_endian>));
	int type = 0;
	for (int i = 0; i < (int)origin.size(); i++) {
		if (origin[i] >= L'0' && origin[i] <= L'9') {
			type = origin[i] - L'0';
			break;
		}
	}
	wfout << L"{\"姓名\":\"" << name << L"\",\"手机\":\"" << phoneNumber << L"\",\"地址\":[\"";
	if (type == 1) {
		for (int i = 0; i < 4; i++) {
			if (missing[i]) {
				wfout << L"\",\"";
			}
			else {
				wfout << level[i].addr << L"\",\"";
			}
		}
		wfout << level[4].addr + level[5].addr + level[6].addr << L"\"]}";
	}
	else if (type == 2) {
		for (int i = 0; i < 6; i++) {
			if (i < 4 && missing[i]) {
				wfout << L"\",\"";
			}
			else {
				wfout << level[i].addr << L"\",\"";
			}
		}
		wfout << level[6].addr << L"\"]}";
	}
	else {
		for (int i = 0; i < 6; i++) {
			wfout << level[i].addr << L"\",\"";
		}
		wfout << level[6].addr << L"\"]}";
	}
	wfout.close();
}