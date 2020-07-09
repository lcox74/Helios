#include "AppBar.h"

// Code implemented from https://docs.microsoft.com/en-us/windows/win32/shell/application-desktop-toolbars

namespace Helios {
    namespace AppBar {

        bool app_bar_registered = false;
        unsigned int app_bar_callback = 0;
        int height = 0, width = 0;

        RECT size{ 0, 0, 0, 30 };
        PAPPBARDATA data;

        // The application needs to be registered as an App Bar to be used like a seperate task bar
        bool RegisterAppBar(SDL_Window* window, bool register_state)
        {
            // Set the windows message callback to an unreserved number
            app_bar_callback = (WM_USER + 0x01);

            data = new APPBARDATA();

            // Set basics of the appbar data
            data->cbSize = sizeof(APPBARDATA);

            // Set window handle
            data->hWnd = GetWindowHandle(window);
            if (data->hWnd == nullptr) return false;

            // Register the App Bar
            if (register_state) {
                // Set the App Bar's winows message callback
                data->uCallbackMessage = app_bar_callback;

                // Register the app bar, return false if it failed
                if (!SHAppBarMessage(ABM_NEW, data)) return false;

                // Set stats
                app_bar_registered = true;
            }
            else {
                // Un-Register the App Bar from the windows shell API
                SHAppBarMessage(ABM_REMOVE, data);
                app_bar_registered = false;
            }

            return true;
        }

        // Set a registered App Bar to be fixed to a particular side of the screen
        void PASCAL AppBarSetPos(SDL_Window* window, int desired_size)
        {
            // Error Checking
            if (app_bar_registered == false) {
                if (!RegisterAppBar(window, true)) return;
            }

            data->rc = size;
            data->uEdge = ABE_TOP;
            
            // Set window handle
            data->hWnd = GetWindowHandle(window);
            if (data->hWnd == nullptr) return;

            // Set the window to the specific edge
            height = desired_size;
            data->rc.left = 0;
            data->rc.right = GetSystemMetrics(SM_CXSCREEN);

            // Check if the size and position is valid
            SHAppBarMessage(ABM_QUERYPOS, data);

            // Adjust window size to fit the appropriate edge
            data->rc.bottom = data->rc.top + height;

            // Check the window rect bound with the system
            SHAppBarMessage(ABM_SETPOS, data);

            // Set the winow
            MoveWindow(data->hWnd, data->rc.left, data->rc.top, data->rc.right - data->rc.left, data->rc.bottom - data->rc.top, true);
        }

        // Get the window handle from the SDL Window instance
        HWND GetWindowHandle(SDL_Window* window) {
            SDL_SysWMinfo info = SDL_SysWMinfo();

            // Calling the SDL Version to stop the bug with getting WM info
            SDL_VERSION(&info.version);

            // Getting the system info 
            if (SDL_GetWindowWMInfo(window, &info) == -1) return nullptr;
            
            return info.info.win.window;
        }
    }
}