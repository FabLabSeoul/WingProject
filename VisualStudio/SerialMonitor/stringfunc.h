#pragma once


namespace common
{
	void replaceAll(string& str, const string& from, const string& to);
	string& trim(string &str);

	string& lowerCase(string &str);
	string& upperCase(string &str);
	wstring& lowerCasew(wstring &str);
	wstring& upperCasew(wstring &str);

	string wstr2str(const wstring &wstr);
	wstring str2wstr(const string &str);

	string format(const char* fmt, ...);
	wstring formatw(const char* fmt, ...);

	void tokenizer(const string &str, const string &delimeter, const string &ignoreStr, OUT vector<string> &out);
	void wtokenizer(const wstring &str, const wstring &delimeter, const wstring &ignoreStr, OUT vector<wstring> &out);

}
