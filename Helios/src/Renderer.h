#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_syswm.h>

#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>
#include <math.h>

#include <Windows.h>
#include <WinUser.h>
#include <Psapi.h>
#include <dwmapi.h>

#include "AppBar.h"
#include "Component.h"

namespace Helios {

	enum FONT_TYPE : int {
		NORMAL = 0,
		LIGHT,
		SEMI_LIGHT,
		SEMI_BOLD,
		BOLD
	};

	enum TEXT_ALIGN : int {
		LEFT = 0,
		CENTRE,
		RIGHT
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
	HWND GetWindowHandler();

	// Use at some point
	void SetRunAtStartUp(bool state);

	int GetMonitorWidth();
	int GetMonitorHeight();

	TTF_Font* FontSelectManager(bool glyph, int size = 16, int font_type = FONT_TYPE::NORMAL);

	// Render Text
	void RenderText(const char* text, int x, int y, TEXT_ALIGN justification = TEXT_ALIGN::LEFT, int size = 16, int font_type = FONT_TYPE::NORMAL, SDL_Color color = { 255, 255, 255 });

	// Render Glyphs
	void RenderGlyphs(uint16_t glyph, int x, int y, int size = 16, SDL_Color color = { 255, 255, 255 });

	// Render Clear
	void Clear();
	void SetTransparentColour(SDL_Color key);

	void Clean();

	
}