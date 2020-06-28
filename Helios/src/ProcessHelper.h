#pragma once

#include <Windows.h>
#include <psapi.h>
#include <string>

namespace Helios {
	namespace Process {

		std::string GetWindowNameByProcessName(const char* name);
		BOOL CALLBACK EnumWindowCallback(HWND hWnd, LPARAM lparam);
		const char* ProcessIdToName(DWORD processId);
	}
}