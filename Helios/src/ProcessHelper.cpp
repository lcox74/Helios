#include "ProcessHelper.h"
#include <iostream>

// Future Note:
// Make a struct that has the search name and
// desired window handle to no rely on global
// variables.
const char* current_search;

const char* Helios::Process::GetWindowNameByProcessName(const char* name)
{
    // Set the process to search
    
    current_search = name;

    // Get the window handle for the target process
    HWND desired_hWnd = nullptr;
    bool search = EnumWindows(EnumWindowCallback, (LPARAM)(&desired_hWnd));
    if (search) return "";

    // Get the window title from the window handle
    int length = GetWindowTextLength(desired_hWnd);
    char* buffer = (char*)malloc((++length) * sizeof(char));
    GetWindowTextA(desired_hWnd, buffer, length + 1);
    
    // Return the window name
    return buffer;
}

BOOL Helios::Process::EnumWindowCallback(HWND hWnd, LPARAM lparam)
{
    // Cull processes that are definetly not the target
    int length = GetWindowTextLength(hWnd);
    if (IsWindowVisible(hWnd) && length != 0) {
        // Get the process ID
        DWORD pid;
        DWORD actual_proc_id = GetWindowThreadProcessId(hWnd, &pid);

        // Check if the process name is the target process
        if (strstr(ProcessIdToName(pid), current_search)) {
            // Pass a reference back through the LPARAM and terminate the search
            // to save on compute cycles
            *((HWND*)(lparam)) = hWnd;
            return FALSE;
        }
    }
    
    // Continue search
    return TRUE;
}

const char* Helios::Process::ProcessIdToName(DWORD pid)
{
    // Get a handle for the process to read
    CHAR process_name[MAX_PATH] = "";
    HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    HMODULE module_handle;
    DWORD required_bytes_to_store_lphModule;

    // Check if process handle was created
    if (NULL != handle) {
        // Get the module handles, check if it failed
        if (EnumProcessModules(handle, &module_handle, sizeof(module_handle), &required_bytes_to_store_lphModule)) {
            // Get the process name
            GetModuleBaseNameA(handle, module_handle, process_name, sizeof(process_name) / sizeof(CHAR));
        }
    }
    // Close handle to prevent leak
    CloseHandle(handle);

    return process_name;
}
