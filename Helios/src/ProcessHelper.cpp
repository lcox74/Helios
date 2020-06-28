#include "ProcessHelper.h"

const char* current_search;
std::string search_result;

std::string Helios::Process::GetWindowNameByProcessName(const char* name)
{
    current_search = name;
    bool search = EnumWindows(EnumWindowCallback, NULL);
    if (search) return "";

    return search_result;
}

BOOL Helios::Process::EnumWindowCallback(HWND hWnd, LPARAM lparam)
{
    int length = GetWindowTextLength(hWnd);

    if (IsWindowVisible(hWnd) && length != 0) {
        DWORD processID;
        DWORD actualProcId = GetWindowThreadProcessId(hWnd, &processID);

        if (strstr(ProcessIdToName(processID), current_search)) {
            char* buffer = new char[length + 1];
            GetWindowTextA(hWnd, buffer, length + 1);

            std::string window_name(buffer);
            delete[] buffer;

            search_result = window_name;
            return FALSE;
        }
    }
    
    return TRUE;
}

const char* Helios::Process::ProcessIdToName(DWORD pid)
{
    CHAR process_name[MAX_PATH] = "";
    HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    HMODULE hMod;
    DWORD cbNeeded;

    if (NULL != handle) {
        if (EnumProcessModules(handle, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseNameA(handle, hMod, process_name, sizeof(process_name) / sizeof(CHAR));
        }
    }
    CloseHandle(handle);

    return process_name;
}
