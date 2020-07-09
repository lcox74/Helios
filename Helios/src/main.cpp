#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <memory>

#include <stdio.h>
#include <SDL2/SDL.h>

#include "Renderer.h"

#include "Component.h"
#include "Components/Launcher.h"
#include "Components/Battery.h"
#include "Components/Clock.h"
#include "Components/MusicDisplay.h"

//#define LAZY_MEMORY_CHECKER

#include "MemoryUtil.h"

#ifdef LAZY_MEMORY_CHECKER
void* operator new(size_t size) {
    s_allocations.allocated += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
    s_allocations.freed += size;
    free(memory);
}
void operator delete[](void* memory, size_t size) {
    s_allocations.freed += size;
    free(memory);
}
#endif

// Useful stuff for later
/*
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    int* Count = (int*)dwData;
    std::cout << lprcMonitor->left << ", " << lprcMonitor->top << ", " << lprcMonitor->right << ", " << lprcMonitor->bottom << std::endl;
    (*Count)++;
    return TRUE;
}

int MonitorCount()
{
    int Count = 0;
    if (EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&Count))
        return Count;
    return -1;//signals an error
}
*/

int main(int argc, char** argv) {

    //std::cout << MonitorCount() << " monitors \n";

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
    Helios::PushComponent(new Helios::MusicDisplay(1, (int)(Helios::GetMonitorWidth() / 2.0f) - 200, 0));
    Helios::PushComponent(new Helios::Battery(2, (int)(Helios::GetMonitorWidth()) - 230, 0));
    Helios::PushComponent(new Helios::Clock(3, (int)(Helios::GetMonitorWidth()) - 140, 0));

    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

    // Loop until user quits5
    bool running = true;
    SDL_Event event;
    while (running) {
#ifdef LAZY_MEMORY_CHECKER
        std::cout << "Frame: " << s_allocations.GetCurrentAllocated() << " bytes \n";
#endif
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