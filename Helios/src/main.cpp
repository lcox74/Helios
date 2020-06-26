#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <memory>

#include <stdio.h>
#include <SDL2/SDL.h>

#include "Renderer/Renderer.h"

#include "Component.h"
#include "Components/Launcher.h"
#include "Components/Battery.h"
#include "Components/Clock.h"

int main(int argc, char** argv) {

    // Check if instance is already running
    HANDLE mutex = CreateMutexEx(0, L"HELIOSx64", 0, 0);
    if (!mutex) return 0;

    if (!Helios::SetupRenderer()) {
        // Clean and release mutex
        Helios::Clean();
        ReleaseMutex(mutex);

        return 0;
    }

    Uint32 last_frame = 0, delta = 0;
    unsigned char delay = 50; // 20 FPS

    Helios::PushComponent(new Helios::Launcher(0, 0, 0));
    Helios::PushComponent(new Helios::Battery(1, (int)(Helios::GetMonitorWidth()) - 230, 0));
    Helios::PushComponent(new Helios::Clock(2, (int)(Helios::GetMonitorWidth()) - 140, 0));

    // Loop until user quits
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update Components
        Helios::UpdateComponents();

        // Render Components
        Helios::RenderComponents();

        // Frame Regulator
        delta = SDL_GetTicks() - last_frame;
        if (delta < delay) SDL_Delay(delay - delta);

        last_frame = SDL_GetTicks();
    }

    // Clean and release mutex
    Helios::Clean();
    ReleaseMutex(mutex);

    return 0;
}