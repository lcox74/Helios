#include "Renderer.h"


namespace Helios {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int screen_width = 0;
	int screen_height = 0;

	int sdl_displacement_check = 0;
	int sdl_x = 0, sdl_y = 0;

	SDL_Color clear_colour = { 10, 10, 10 };
	
	std::vector<Component*> components;

	bool SetupRenderer() {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
		if (TTF_Init() == -1) return false;

		screen_width = GetSystemMetrics(SM_CXSCREEN);
		screen_height = GetSystemMetrics(SM_CYSCREEN);

		window = SDL_CreateWindow("Helios", SDL_WINDOWPOS_CENTERED, 0, screen_width, 30, SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR);
		if (!window) return false;

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (!renderer) return false;

		// Set the window to an app bar
		SetApplicationBar();

		// Clear the renderer
		Clear();

		// Draw Renderer
		SDL_RenderPresent(renderer);

		return true;
	}

	void SetApplicationBar() {
		if (window == nullptr || renderer == nullptr) SetupRenderer();
		if (!AppBar::RegisterAppBar(window, true)) return;

		AppBar::AppBarSetPos(window, AppBar::AB_EDGE::TOP);
	}

	void UnSetApplicationBar() { 
		if (window == nullptr || renderer == nullptr) SetupRenderer(); 
		AppBar::RegisterAppBar(window, false); 
	}

	void RefreshApplicationBar() {
		if (window == nullptr || renderer == nullptr) SetupRenderer();
		UnSetApplicationBar();
		SetApplicationBar();
	}

	void EnableDisplacementCheck() { sdl_displacement_check = 0; }

	void CheckAppBarDisplacement()
	{
		if (window == nullptr || renderer == nullptr) return;

		if (sdl_displacement_check < 10) {
			// Check if the bar is displaced
			SDL_GetWindowPosition(window, &sdl_x, &sdl_y);
			if (sdl_x != 0 || sdl_y != 0) {
				RefreshApplicationBar();

				// Find a way to make this not call every clear, used to reset the window for the app bar
				SDL_SetWindowPosition(window, 0, 0);
			}

			sdl_displacement_check++;
		}
	}

	void PushComponent(Component* component) { components.push_back(component); }
	void PopComponent() { components.pop_back(); components.shrink_to_fit(); }
	void UpdateComponents() {
		if (window == nullptr || renderer == nullptr) return;
		for (auto c : components) c->Update();
	}
	void RenderComponents() {
		if (window == nullptr || renderer == nullptr) return;

		bool re_render = false;
		for (auto c : components) {
			if (c->GetRenderFlag()) {
				re_render = true;
				break;
			}
		}

		if (!re_render) return;

		Clear();
		for (auto c : components) c->Render();
		SDL_RenderPresent(renderer);
	}

	SDL_Window* GetWindow() { return window; }
	int GetMonitorWidth() { return screen_width; }
	int GetMonitorHeight() { return screen_height; }

	void RenderText(const char* text, int x, int y, int size, int font_type, SDL_Color color) {
		if (window == nullptr || renderer == nullptr) return;

		TTF_Font* text_font;
		switch (font_type) {
		case FONT_TYPE::LIGHT:		text_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuil.ttf", size); break;
		case FONT_TYPE::SEMI_LIGHT: text_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuisl.ttf", size); break;
		case FONT_TYPE::SEMI_BOLD:	text_font = TTF_OpenFont("C:\\Windows\\Fonts\\seguisb.ttf", size); break;
		case FONT_TYPE::BOLD:		text_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuib.ttf", size); break;
		default: text_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeui.ttf", size);
		}

		SDL_Surface* text_surface = TTF_RenderText_Blended(text_font, text, color);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

		int text_width = 0, text_height = 0;
		SDL_QueryTexture(text_texture, NULL, NULL, &text_width, &text_height);

		SDL_Rect text_rect{ x, y, text_width, text_height };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

		TTF_CloseFont(text_font);			text_font = nullptr;
		SDL_FreeSurface(text_surface);		text_surface = nullptr;
		SDL_DestroyTexture(text_texture);	text_texture = nullptr;
	}

	void RenderGlyphs(uint16_t glyph, int x, int y, int size, SDL_Color color) {
		TTF_Font* glyph_font = TTF_OpenFont("C:\\Windows\\Fonts\\segmdl2.ttf", size);
		SDL_Surface* glyph_surface = TTF_RenderGlyph_Blended(glyph_font, glyph, color);
		SDL_Texture* glyph_texture = SDL_CreateTextureFromSurface(renderer, glyph_surface);

		int glyph_width = 0, glyph_height = 0;
		SDL_QueryTexture(glyph_texture, NULL, NULL, &glyph_width, &glyph_height);

		SDL_Rect glyph_rect{ x, y, glyph_width, glyph_height };
		SDL_RenderCopy(renderer, glyph_texture, NULL, &glyph_rect);

		TTF_CloseFont(glyph_font);			glyph_font = nullptr;
		SDL_FreeSurface(glyph_surface);		glyph_surface = nullptr;
		SDL_DestroyTexture(glyph_texture);	glyph_texture = nullptr;
	}

	void Clear() {
		if (window == nullptr || renderer == nullptr) return;

		CheckAppBarDisplacement();

		// Clears the renderer
		SDL_SetRenderDrawColor(renderer, clear_colour.r, clear_colour.g, clear_colour.b, 255);
		SDL_RenderClear(renderer);
	}

	void SetTransparentColour(SDL_Color key) {
		if (window == nullptr || renderer == nullptr) return;

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

		components.clear();
		components.shrink_to_fit();

		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}