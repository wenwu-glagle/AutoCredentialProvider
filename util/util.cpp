#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <string>
#include <vector>

void __cdecl odprintf(const char* format, ...)
{
    char    buf[4096], * p = buf;
    va_list args;
    int     n;

    va_start(args, format);
    n = _vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
    va_end(args);

    p += (n < 0) ? sizeof buf - 3 : n;

    while (p > buf && isspace(p[-1]))
        *--p = '\0';

    *p++ = '\r';
    *p++ = '\n';
    *p = '\0';

    OutputDebugStringA(buf);
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string ws2s(const std::wstring& si){
	std::wstring ws(si);
	std::string s;
	s.assign(ws.begin(), ws.end()); 
	return s;
}

wchar_t* wcs2cs(std::wstring& s){
	std::vector<wchar_t> v(s.length() + 1);
	std::wcscpy(&v[0], s.c_str());
	wchar_t* w = &v[0];
	s = w;

	wchar_t* w2 = new wchar_t[wcslen(w) + 1];

	for(int i = 0; i < wcslen(w); i++){
		w2[i] = w[i];
	}
	w2[wcslen(w)] = L'\0';

	return w2;
}

char* s2cs(std::string& s){
	std::vector<char> v(s.length() + 1);
	std::strcpy(&v[0], s.c_str());
	char* w = &v[0];
	s = w;

	char* w2 = new char[strlen(w) + 1];

	for(int i = 0; i < strlen(w); i++){
		w2[i] = w[i];
	}
	w2[strlen(w)] = L'\0';

	return w2;
}

unsigned char* s2ucs(std::string& s){
	unsigned char* cs = (unsigned char*)s2cs(s);
	return cs;
}
