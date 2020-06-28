#pragma once

#include <string>
#include <Windows.h>
#include <WinUser.h>

namespace Helios {
	namespace Process {

		std::string GetWindowNameByProcessName(std::string name);
		BOOL CALLBACK EnumWindowCallback(HWND hWnd, LPARAM lparam);
		std::string ProcessIdToName(DWORD processId);
	}
}