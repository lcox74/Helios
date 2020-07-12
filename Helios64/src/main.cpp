#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "HeliosBar.h"

bool application_running = true;
HANDLE mutex;

int main(int argc, char** argv) {

	// Check if instance is already running
	mutex = CreateMutexEx(0, L"HELIOSx64", 0, 0);
	if (!mutex) return 0;

	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
	if (TTF_Init() == -1) return 1;

	int screen_width = GetSystemMetrics(SM_CXSCREEN);

	Helios::HeliosBar* bar = new Helios::HeliosBar(0, 0, 0, screen_width);

	Uint32 last_frame = 0, delta = 0;
	unsigned char delay = 50; // 20 FPS

	SDL_Event event;
	while (application_running) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) break;
			else bar->SendEvent(&event);
		}

		bar->Update();
		bar->Render();

		// Frame Regulator
		delta = SDL_GetTicks() - last_frame;
		if (delta < delay) SDL_Delay(delay - delta);

		last_frame = SDL_GetTicks();
	}

	ReleaseMutex(mutex);
	delete bar;

	return 0;
}