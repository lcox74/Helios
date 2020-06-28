#include "ProcessHelper.h"

#include <iostream>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>

std::string current_search;
std::string search_result;

std::string Helios::Process::GetWindowNameByProcessName(std::string name)
{
    current_search = name;
    bool search = EnumWindows(EnumWindowCallback, NULL);
    if (search) return "";

    return search_result;
}

BOOL Helios::Process::EnumWindowCallback(HWND hWnd, LPARAM lparam)
{
    int length = GetWindowTextLength(hWnd);
    char* buffer = new char[length + 1];
    GetWindowTextA(hWnd, buffer, length + 1);
    std::string window_name(buffer);

    DWORD processID;
    DWORD actualProcId = GetWindowThreadProcessId(hWnd, &processID);

    if (ProcessIdToName(processID).find(current_search) != std::string::npos && IsWindowVisible(hWnd) && length != 0) {
        search_result = window_name;
        return FALSE;
    }
    return TRUE;
}

std::string Helios::Process::ProcessIdToName(DWORD pid)
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

    return std::string(process_name);
}
