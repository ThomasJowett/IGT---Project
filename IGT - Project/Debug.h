#pragma once

#include <Windows.h>
#include <cstdarg>

namespace Debug
{
#define DBG_OUTPUT(...) Debug::Output(__FILE__, __LINE__, __VA_ARGS__)
#define OUTPUT(...) Debug::Output(__VA_ARGS__)

	static void Output(WCHAR* pFormat, ...)
	{
		WCHAR buffer[1024] = { 0 };
		va_list args;
		va_start(args, pFormat);
		wvsprintf(buffer, pFormat, args);
		va_end(args);

		OutputDebugString(buffer);
	}

	static void Output(const char* file, const int line, const char* pFormat, ...)
	{
		WCHAR* format;

		int nChars = MultiByteToWideChar(CP_ACP, 0, pFormat, -1, NULL, 0);
		format = new WCHAR[nChars];
		MultiByteToWideChar(CP_ACP, 0, pFormat, -1, (LPWSTR)format, nChars);
		
		WCHAR buffer[1024] = { 0 };
		int stringLength = wsprintf(buffer, L"%hs(%d): ", file, line);

		va_list args;
		va_start(args, format);
		wvsprintf(buffer + stringLength, format, args);
		va_end(args);

		OutputDebugString(buffer);

		delete[] format;
	}

	static void OutputWithLine(const char* file, const int line, WCHAR *pFormat, ...)
	{
		WCHAR buffer[1024] = { 0 };
		int stringLength = wsprintf(buffer, L"%hs(%d): ", file, line);

		va_list args;
		va_start(args, pFormat);
		wvsprintf(buffer + stringLength, pFormat, args);
		va_end(args);

		OutputDebugString(buffer);
	}
};