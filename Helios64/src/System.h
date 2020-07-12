#pragma once

#include <Windows.h>
#include <dwmapi.h>
#include <psapi.h>
#include <shellapi.h>
#include <memory>
#include <iostream>

namespace Helios {
	namespace System {
		struct ProcessCallbackData {
			HWND result_handle = nullptr;
			const char* search = "";
		};

		// Processes
		const char* get_window_title_by_process_name(const char* name);
		const HWND get_window_handle_by_process_name(const char* name);
		void get_name_from_pid(DWORD pid, char* process_name, int buffer);
		const char* get_name_of_window(HWND handle);

		// Appbar
		bool register_appbar(HWND handle, PAPPBARDATA* data);
		bool unregister_appbar(PAPPBARDATA* data);
		bool set_appbar_location(PAPPBARDATA* data, RECT* size);

		// Hide from Alt-Tab
		void hide_application_from_task_switcher(HWND handle);

		// Prevent Hiding on Application Peaking
		void disable_hiding_on_peaking(HWND handle);

		// Window Location
		void set_window_to_top(HWND handle);

		// Start Up
		void register_startup();
		void unregister_startup();

		// Callbacks
		BOOL CALLBACK window_callback(HWND handle, LPARAM lparam);
	};
}

