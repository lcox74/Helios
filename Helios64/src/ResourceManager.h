#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <vector>

namespace Helios {
	namespace Resources {

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

		TTF_Font* GetFont(int size = 16, int font_type = FONT_TYPE::NORMAL);
		TTF_Font* GetGlyph(int size = 16);
		
		void RenderText(int monitor, const char* text, int x, int y, int size = 16, int justification = TEXT_ALIGN::LEFT, int font_type = FONT_TYPE::NORMAL, SDL_Color colour = { 255, 255, 255 });
		void RenderGlyph(int monitor, unsigned short glyph, int x, int y, int size = 16, SDL_Color colour = { 255, 255, 255 });

		void SetRenderer(int monitor, SDL_Renderer* renderer);
		SDL_Renderer* GetRenderer(int monitor);
	
		void CleanResources();
	}
}