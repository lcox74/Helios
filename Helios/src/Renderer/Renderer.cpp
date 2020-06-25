#include "Renderer.h"


namespace Helios {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int screen_width = 0;
	int screen_height = 0;

	SDL_Color clear_colour = { 10, 10, 10 };

	void Initialise() {
		assert(SDL_Init(SDL_INIT_VIDEO) == 0);
		assert(TTF_Init() != -1);

		screen_width = GetSystemMetrics(SM_CXSCREEN);
		screen_height = GetSystemMetrics(SM_CYSCREEN);

		window = SDL_CreateWindow("Helios", SDL_WINDOWPOS_CENTERED, 0, screen_width, 30, SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR);
		assert(window);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		assert(renderer);

		// Set the window to an app bar
		RefreshApplicationBar();

		// Clear the renderer
		Clear();

		// Draw Renderer
		SDL_RenderPresent(renderer);
	}

	void SetApplicationBar() {
		assert(AppBar::RegisterAppBar(window, true));
		AppBar::AppBarSetPos(window, AppBar::AB_EDGE::TOP);
	}

	void UnSetApplicationBar() { AppBar::RegisterAppBar(window, false); }

	void RefreshApplicationBar() {
		UnSetApplicationBar();
		SetApplicationBar();
	}

	SDL_Window* GetWindow() { return window; }

	void RenderText(const char* text, int x, int y, int size, SDL_Color color) {
		TTF_Font* text_font = TTF_OpenFont("res/fonts/segoeui.ttf", size);
		SDL_Surface* text_surface = TTF_RenderText_Shaded(text_font, text, color, clear_colour);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

		int text_width = 0, text_height = 0;
		SDL_QueryTexture(text_texture, NULL, NULL, &text_width, &text_height);

		SDL_Rect text_rect{ x, y, text_width, text_height };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

		SDL_FreeSurface(text_surface);
		SDL_DestroyTexture(text_texture);

		SDL_RenderPresent(renderer);
	}

	void RenderGlyphs(uint16_t glyph, int x, int y, int size, SDL_Color color) {
		TTF_Font* glyph_font = TTF_OpenFont("res/fonts/SegMDL2.ttf", size);
		SDL_Surface* glyph_surface = TTF_RenderGlyph_Shaded(glyph_font, glyph, color, clear_colour);
		SDL_Texture* glyph_texture = SDL_CreateTextureFromSurface(renderer, glyph_surface);

		int glyph_width = 0, glyph_height = 0;
		SDL_QueryTexture(glyph_texture, NULL, NULL, &glyph_width, &glyph_height);

		SDL_Rect glyph_rect{ x, y, glyph_width, glyph_height };
		SDL_RenderCopy(renderer, glyph_texture, NULL, &glyph_rect);

		SDL_FreeSurface(glyph_surface);
		SDL_DestroyTexture(glyph_texture);

		SDL_RenderPresent(renderer);
	}

	void Clear() {
		// Find a way to make this not call every clear, used to reset the window for the app bar
		SDL_SetWindowPosition(Helios::GetWindow(), 0, 0);

		// Clears the renderer
		SDL_SetRenderDrawColor(renderer, clear_colour.r, clear_colour.g, clear_colour.b, 255);
		SDL_RenderClear(renderer);
	}

	void SetTransparentColour(SDL_Color key) {
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);
		HWND hWnd = wmInfo.info.win.window;

		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, RGB(key.r, key.g, key.b), 0, LWA_COLORKEY);

		clear_colour = key;
	}

	void Clean() {
		UnSetApplicationBar();
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}