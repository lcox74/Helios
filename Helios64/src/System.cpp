#include "System.h"

const char* Helios::System::get_window_title_by_process_name(const char* name)
{
    // Set search
    ProcessCallbackData data;
    data.search = name;

    // Find target window
    bool search = EnumWindows(window_callback, (LPARAM)(&data));
    if (search) return "";

    // Get the window name
    return get_name_of_window(data.result_handle);
}

const HWND Helios::System::get_window_handle_by_process_name(const char* name)
{
    // Set search
    ProcessCallbackData data;
    data.search = name;

    // Find target window
    bool search = EnumWindows(window_callback, (LPARAM)(&data));
    if (search) return nullptr;

    // Get the window name
    return data.result_handle;
}

void Helios::System::get_name_from_pid(DWORD pid, char* process_name, int buffer)
{
    // Get a handle for the process to read
    HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    HMODULE module_handle;
    DWORD required_bytes_to_store_lphModule;

    // Check if process handle was created
    if (NULL != handle) {
        // Get the module handles, check if it failed
        if (EnumProcessModules(handle, &module_handle, sizeof(module_handle), &required_bytes_to_store_lphModule)) {
            // Get the process name
            GetModuleBaseNameA(handle, module_handle, process_name, buffer * sizeof(char));
        }
    }
    // Close handle to prevent leak
    CloseHandle(handle);
}

const char* Helios::System::get_name_of_window(HWND handle)
{
    if (handle == nullptr) return "";

    // Get window title length
    int length = GetWindowTextLength(handle);

    // Set buffer
    char* buffer = (char*)malloc((++length) * sizeof(char));

    // Get window title
    GetWindowTextA(handle, buffer, length + 1);

    return buffer;
}

BOOL Helios::System::window_callback(HWND handle, LPARAM lparam)
{
    // Cull processes that are definetly not the target
    int length = GetWindowTextLength(handle);
    if (IsWindowVisible(handle) && length != 0) {
        // Get the process ID
        DWORD pid;
        DWORD actual_proc_id = GetWindowThreadProcessId(handle, &pid);

        // Fetch process name
        char process_name[MAX_PATH];
        get_name_from_pid(pid, process_name, MAX_PATH);

        // Check if the process name is the target process
        if (strstr(process_name, ((ProcessCallbackData*)(lparam))->search)) {
            // Pass a reference back through the LPARAM and terminate the search
            // to save on compute cycles
            ((ProcessCallbackData*)(lparam))->result_handle = handle;
            return FALSE;
        }
    }

    // Continue search
    return TRUE;
}

bool Helios::System::register_appbar(HWND handle, PAPPBARDATA* data)
{
    // Setup new appbar data
    (*data) = new APPBARDATA();

    // Set basics of the appbar data
    (*data)->cbSize = sizeof(APPBARDATA);

    // Set window handle
    (*data)->hWnd = handle;
    if ((*data)->hWnd == nullptr) return false;

    // Set the windows message callback to an unreserved number
    (*data)->uCallbackMessage = (WM_USER + 0x01);

    // Register the app bar, return false if it failed
    if (!SHAppBarMessage(ABM_NEW, *data)) return false;

	return true;
}

bool Helios::System::unregister_appbar(PAPPBARDATA* data)
{
    // Check if appbar has actual data
    if (*data == nullptr) return false;

    // Un-Register the App Bar from the windows shell API
    SHAppBarMessage(ABM_REMOVE, *data);

    return true;
}

bool Helios::System::set_appbar_location(PAPPBARDATA *data, RECT* size)
{
    // Check if appbar has actual data
    if (*data == nullptr) return false;

    // Set location data
    (*data)->rc = *size;
    (*data)->uEdge = ABE_TOP;

    // Check if the size and position is valid
    SHAppBarMessage(ABM_QUERYPOS, *data);

    // Readjust the height and width if moved
    (*data)->rc.bottom = (*data)->rc.top + (size->bottom - size->top);
    (*data)->rc.right = (*data)->rc.left + (size->right - size->left);

    // Set the appbar rect bound with the system
    SHAppBarMessage(ABM_SETPOS, *data);

    // Be stubborn and put it appbar where I want it to
    if ((*size).top != (*data)->rc.top) {
        unregister_appbar(data);
        register_appbar((*data)->hWnd, data);
        set_appbar_location(data, size);
    }

    // Update the new size/position
    *size = (*data)->rc;

    // Set the winow
    MoveWindow((*data)->hWnd, (*data)->rc.left, (*data)->rc.top, (*data)->rc.right - (*data)->rc.left, (*data)->rc.bottom - (*data)->rc.top, true);

    return true;
}

void Helios::System::hide_application_from_task_switcher(HWND handle)
{
	// Hide application from Alt-Tab (https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra)
	LONG_PTR exStyle = GetWindowLongPtr(handle, GWL_EXSTYLE);
	SetWindowLongPtr(handle, GWL_EXSTYLE, exStyle | WS_EX_TOOLWINDOW);
}

void Helios::System::disable_hiding_on_peaking(HWND handle)
{
	// Prevent application from hiding when desktop or other application peaking (https://docs.microsoft.com/en-us/windows/win32/api/dwmapi/nf-dwmapi-dwmsetwindowattribute)
	int desktop_window_manager_value = DWMNCRENDERINGPOLICY::DWMNCRP_ENABLED;
	DwmSetWindowAttribute(handle, DWMWA_DISALLOW_PEEK, &desktop_window_manager_value, sizeof(int));
	DwmSetWindowAttribute(handle, DWMWA_EXCLUDED_FROM_PEEK, &desktop_window_manager_value, sizeof(int));
}

void Helios::System::set_window_to_top(HWND handle)
{
    // Prevent application from hiding when 'Show Desktop' button is pressed (https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos)
    SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void Helios::System::register_startup()
{
    // Get Application Path
    TCHAR app_location[MAX_PATH];
    HANDLE file_location_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
    GetModuleFileNameEx(file_location_handle, 0, app_location, MAX_PATH);

    // Open the auto-run register
    HKEY registry_key_handle;
    RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &registry_key_handle);

    // Set register value
    RegSetValueEx(registry_key_handle, L"HELIOSx64", 0, REG_SZ, (BYTE*)app_location, (DWORD)(wcslen(app_location) * sizeof(app_location[0])));

    // Clean
    RegCloseKey(registry_key_handle);
    CloseHandle(file_location_handle);
}

void Helios::System::unregister_startup()
{
    // Get Application Path
    TCHAR app_location[MAX_PATH];
    HANDLE file_location_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
    GetModuleFileNameEx(file_location_handle, 0, app_location, MAX_PATH);

    // Open the auto-run register
    HKEY registry_key_handle;
    RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &registry_key_handle);

    // Set register value
    RegDeleteValue(registry_key_handle, L"HELIOSx64");

    // Clean
    RegCloseKey(registry_key_handle);
    CloseHandle(file_location_handle);
}
