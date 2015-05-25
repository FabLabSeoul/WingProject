
#include "stdafx.h"
#include "stringfunc.h"


void common::replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if(from.empty())
		return;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

// "  skdfjskdjf  "
// "$$$skdfjskdjf$$$"
// "skdfjskdjf"
string& common::trim(string &str)
{
	for (int i=0; i < (int)str.length(); ++i)
	{
		if ((str[ i] == '\n') || (str[ i] == '\t') || (str[ i] == ' '))
			str[ i] = '$';
		else
			break;
	}

	for (int i=str.length()-1; i >= 0; --i)
	{
		if ((str[ i] == '\n') || (str[ i] == '\t') || (str[ i] == ' '))
			str[ i] = '$';
		else
			break;
	}

	replaceAll(str, "$", "");
	return str;
}


//------------------------------------------------------------------------
// �����ڵ带 ��Ƽ����Ʈ ���ڷ� ��ȯ
//------------------------------------------------------------------------
std::string common::wstr2str(const std::wstring &wstr)
{
	const int slength = (int)wstr.length() + 1;
	const int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, 0, 0, NULL, FALSE);
	char* buf = new char[len];
	::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, buf, len, NULL, FALSE);
	std::string r(buf);
	delete[] buf;
	return r;
}


//------------------------------------------------------------------------
// ��Ƽ����Ʈ ���ڸ� �����ڵ�� ��ȯ
//------------------------------------------------------------------------
std::wstring common::str2wstr(const std::string &str)
{
	int len;
	int slength = (int)str.length() + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


//------------------------------------------------------------------------
// ��Ʈ������
//------------------------------------------------------------------------
std::string common::format(const char* fmt, ...)
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );
	return textString;
}


//------------------------------------------------------------------------
// ��Ʈ������ wstring ��
//------------------------------------------------------------------------
std::wstring common::formatw(const char* fmt, ...)
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );
	return str2wstr(textString);
}


// �Ѿ�� ���� str �� �ҹ��ڷ� �ٲ㼭 �����Ѵ�.
string& common::lowerCase(string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

// �Ѿ�� ���� str �� �빮�ڷ� �ٲ㼭 �����Ѵ�.
string& common::upperCase(string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}


wstring& common::lowerCasew(wstring &str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

wstring& common::upperCasew(wstring &str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}


// ���� ���ڿ� str���� ������ delimeter �� �����ؼ� out �� �����ؼ� �����Ѵ�.
// delimeter �� ������� �ʴ´�.
void common::tokenizer(const string &str, const string &delimeter, const string &ignoreStr, OUT vector<string> &out)
{
	string tmp = str;
	int offset = 0;
	int first = 0;

	while (!tmp.empty())
	{
		const int pos = tmp.find(delimeter, offset);
		if (string::npos == pos)
		{
			out.push_back(tmp.substr(first));
			break;
		}
		else
		{
			const string tok = tmp.substr(offset, pos-offset);
			offset += tok.length() + delimeter.length();
			if (tok != ignoreStr)
			{
				out.push_back(tmp.substr(first, pos-first));
				first = offset;
			}

		}
	}
}


// ���� ���ڿ� str���� ������ delimeter �� �����ؼ� out �� �����ؼ� �����Ѵ�.
// delimeter �� ������� �ʴ´�.
void common::wtokenizer(const wstring &str, const wstring &delimeter, const wstring &ignoreStr, OUT vector<wstring> &out)
{
	wstring tmp = str;
	int offset = 0;
	int first = 0;

	while (!tmp.empty())
	{
		const int pos = tmp.find(delimeter, offset);
		if (wstring::npos == pos)
		{
			out.push_back(tmp.substr(first));
			break;
		}
		else
		{
			const wstring tok = tmp.substr(offset, pos - offset);
			offset += tok.length() + delimeter.length();
			if (tok != ignoreStr)
			{
				out.push_back(tmp.substr(first, pos - first));
				first = offset;
			}

		}
	}
}

