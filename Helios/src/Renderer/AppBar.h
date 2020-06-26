#pragma once

#include <Windows.h>
#include <shellapi.h>

#include <SDL2/SDL_syswm.h>

namespace Helios {
	namespace AppBar {
		enum AB_EDGE : int {
			LEFT = 0,
			TOP,
			RIGHT,
			BOTTOM
		};

		bool RegisterAppBar(SDL_Window* window, bool register_state);
		void PASCAL AppBarSetPos(SDL_Window* window, UINT window_edge, int desired_size = 30);
		HWND GetWindowHandle(SDL_Window* window);
	}
}

