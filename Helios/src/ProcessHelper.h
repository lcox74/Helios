#pragma once

#include <string>
#include <Windows.h>
#include <WinUser.h>

namespace Helios {
	namespace Process {

		std::string GetWindowNameByProcessName(const char* name);
		BOOL CALLBACK EnumWindowCallback(HWND hWnd, LPARAM lparam);
		const char* ProcessIdToName(DWORD processId);
	}
}