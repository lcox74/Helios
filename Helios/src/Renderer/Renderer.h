#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_syswm.h>

#include <assert.h>
#include <cstdint>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <memory>

#include "AppBar.h"
#include "../Component.h"

namespace Helios {

	enum FONT_TYPE : int {
		NORMAL = 0,
		LIGHT,
		SEMI_LIGHT,
		SEMI_BOLD,
		BOLD
	};

	bool SetupRenderer();
	
	// Set the window as an App Bar
	void SetApplicationBar();
	void UnSetApplicationBar();
	void RefreshApplicationBar();

	// Error Checking
	void EnableDisplacementCheck();
	void CheckAppBarDisplacement();

	void PushComponent(Component* component);
	void PopComponent();

	void UpdateComponents();
	void RenderComponents();

	SDL_Window* GetWindow();
	int GetMonitorWidth();
	int GetMonitorHeight();

	// Render Text
	void RenderText(const char* text, int x, int y, int size = 12, int font_type = FONT_TYPE::NORMAL, SDL_Color color = { 255, 255, 255 });

	// Render Glyphs
	void RenderGlyphs(uint16_t glyph, int x, int y, int size = 12, SDL_Color color = { 255, 255, 255 });

	// Render Clear
	void Clear();
	void SetTransparentColour(SDL_Color key);

	void Clean();

	
}