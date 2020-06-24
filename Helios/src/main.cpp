#include <iostream>
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>
#include <SDL2/SDL.h>

#include "Renderer/Renderer.h"

int main(int argc, char** argv) {
    Helios::Initialise();

    Uint32 last_frame = 0;
    unsigned char delay = 50;

    // Loop until user quits
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update


        // Render Frame
        if (SDL_GetTicks() > last_frame + delay) {
            Helios::Clear();
            last_frame = SDL_GetTicks();
        }
        
    }

    return 0;
}