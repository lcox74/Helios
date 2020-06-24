#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_syswm.h>

#include <assert.h>
#include <cstdint>
#include <Windows.h>

#include <iostream>

namespace Helios {

	void Initialise();

	// Render Text
	void RenderText(const char* text, int x, int y, int size = 12, SDL_Color color = { 255, 255, 255 });

	// Render Glyphs
	void RenderGlyphs(uint16_t glyph, int x, int y, int size = 12, SDL_Color color = { 255, 255, 255 });

	// Render Clear
	void Clear();
	void SetTransparentColour(SDL_Color key);

	void Clean();

	
}