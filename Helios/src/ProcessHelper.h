#pragma once

#include <Windows.h>
#include <psapi.h>
#include <string>

namespace Helios {
	namespace Process {
		const char* GetWindowNameByProcessName(const char* name);
		BOOL CALLBACK EnumWindowCallback(HWND hWnd, LPARAM lparam);
		void ProcessIdToName(DWORD processId, char* process_name, int buffer = MAX_PATH);
	}
}