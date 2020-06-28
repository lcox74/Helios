#include "Renderer.h"

namespace Helios {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int screen_width = 0;
	int screen_height = 0;

	int sdl_displacement_check = 0;
	int sdl_x = 0, sdl_y = 0;

	unsigned char last_render = 0;

	SDL_Color clear_colour = { 10, 10, 10 };
	
	std::vector<Component*> components;

	const int default_font_size = 16;
	TTF_Font* default_regular_font = nullptr;
	TTF_Font* default_light_font = nullptr;
	TTF_Font* default_semi_light_font = nullptr;
	TTF_Font* default_semi_bold_font = nullptr;
	TTF_Font* default_bold_font = nullptr;
	TTF_Font* default_glyph_font = nullptr;

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
		
		// Hide application from Alt-Tab (https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra)
		auto exStyle = GetWindowLongPtr(GetWindowHandler(), GWL_EXSTYLE);
		SetWindowLongPtr(GetWindowHandler(), GWL_EXSTYLE, exStyle | WS_EX_TOOLWINDOW);

		// Prevent application from hiding when desktop or other application peaking (https://docs.microsoft.com/en-us/windows/win32/api/dwmapi/nf-dwmapi-dwmsetwindowattribute)
		int desktop_window_manager_value = DWMNCRENDERINGPOLICY::DWMNCRP_ENABLED;
		DwmSetWindowAttribute(GetWindowHandler(), DWMWA_DISALLOW_PEEK, &desktop_window_manager_value, sizeof(int));
		DwmSetWindowAttribute(GetWindowHandler(), DWMWA_EXCLUDED_FROM_PEEK, &desktop_window_manager_value, sizeof(int));

		// Prevent application from hiding when 'Show Desktop' button is pressed (https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos)
		SetWindowPos(GetWindowHandler(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

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

	void EnableDisplacementCheck() { 
		sdl_displacement_check = 0; 
	}

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

	void PushComponent(Component* component) { 
		components.push_back(component);
	}
	void PopComponent() {
		components.pop_back(); 
		components.shrink_to_fit();
	}
	void UpdateComponents() {
		if (window == nullptr || renderer == nullptr) return;
		for (auto c : components) c->Update();
	}
	void RenderComponents() {
		if (window == nullptr || renderer == nullptr) return;
		last_render++;

		bool re_render = false;
		for (auto c : components) {
			if (c->GetRenderFlag()) {
				re_render = true;
				break;
			}
		}

		if (!re_render && last_render < 250) return;
		last_render = 0;

		Clear();
		for (auto c : components) c->Render();
		SDL_RenderPresent(renderer);
	}

	SDL_Window* GetWindow() { 
		return window; 
	}
	HWND GetWindowHandler()	{
		SDL_SysWMinfo info = SDL_SysWMinfo();

		// Calling the SDL Version to stop the bug with getting WM info
		SDL_VERSION(&info.version);

		// Getting the system info 
		if (SDL_GetWindowWMInfo(window, &info) == -1) return nullptr;

		return info.info.win.window;
	}

	void SetRunAtStartUp(bool state)
	{
		// Get Application Path
		TCHAR app_location[MAX_PATH];
		HANDLE file_location_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
		GetModuleFileNameEx(file_location_handle, 0, app_location, MAX_PATH);
		
		// Open the auto-run register
		HKEY registry_key_handle;
		RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &registry_key_handle);

		// Set register value
		if (state)
			RegSetValueEx(registry_key_handle, L"HELIOSx64", 0, REG_SZ, (BYTE*)app_location, (DWORD)(wcslen(app_location) * sizeof(app_location[0])));
		else
			RegDeleteValue(registry_key_handle, L"HELIOSx64");
		
		// Clean
		RegCloseKey(registry_key_handle);
		CloseHandle(file_location_handle);
	}

	int GetMonitorWidth() {
		return screen_width; 
	}
	int GetMonitorHeight() { 
		return screen_height; 
	}

	TTF_Font* FontSelectManager(bool glyph, int size, int font_type)
	{
		if (glyph) {
			if (size == default_font_size) {
				if (default_glyph_font == nullptr) default_glyph_font = TTF_OpenFont("C:\\Windows\\Fonts\\segmdl2.ttf", default_font_size);
				return default_glyph_font; 
			}
			return TTF_OpenFont("C:\\Windows\\Fonts\\segmdl2.ttf", size);
		}

		if (size == default_font_size) {
			switch (font_type) {
			case FONT_TYPE::LIGHT:	
				if (default_light_font == nullptr) default_light_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuil.ttf", default_font_size);
				return default_light_font;
			case FONT_TYPE::SEMI_LIGHT:
				if (default_semi_light_font == nullptr) default_semi_light_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuisl.ttf", default_font_size);
				return default_semi_light_font;
			case FONT_TYPE::SEMI_BOLD:
				if (default_semi_bold_font == nullptr) default_semi_bold_font = TTF_OpenFont("C:\\Windows\\Fonts\\seguisb.ttf", default_font_size);
				return default_semi_bold_font;
			case FONT_TYPE::BOLD:
				if (default_bold_font == nullptr) default_bold_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuib.ttf", default_font_size);
				return default_bold_font;
			default:	
				if (default_regular_font == nullptr) default_regular_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeui.ttf", default_font_size);
				return default_regular_font;
			}
		}

		switch (font_type) {
		case FONT_TYPE::LIGHT:		return TTF_OpenFont("C:\\Windows\\Fonts\\segoeuil.ttf", size); break;
		case FONT_TYPE::SEMI_LIGHT: return TTF_OpenFont("C:\\Windows\\Fonts\\segoeuisl.ttf", size); break;
		case FONT_TYPE::SEMI_BOLD:	return TTF_OpenFont("C:\\Windows\\Fonts\\seguisb.ttf", size); break;
		case FONT_TYPE::BOLD:		return TTF_OpenFont("C:\\Windows\\Fonts\\segoeuib.ttf", size); break;
		default:					return TTF_OpenFont("C:\\Windows\\Fonts\\segoeui.ttf", size);
		}
	}

	void RenderText(const char* text, int x, int y, TEXT_ALIGN justification, int size, int font_type, SDL_Color color) {
		if (window == nullptr || renderer == nullptr) return;

		TTF_Font* text_font = FontSelectManager(false, size, font_type);

		SDL_Surface* text_surface = TTF_RenderText_Blended(text_font, text, color);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

		int text_width = 0, text_height = 0;
		SDL_QueryTexture(text_texture, NULL, NULL, &text_width, &text_height);

		int altered_justification = x;
		if (justification == TEXT_ALIGN::CENTRE) altered_justification -= (int)(text_width / 2.0f);
		else if (justification == TEXT_ALIGN::RIGHT) altered_justification -= text_width;

		SDL_Rect text_rect{ altered_justification, y, text_width, text_height };
		SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

		if (size != default_font_size) {
			TTF_CloseFont(text_font);
			text_font = nullptr;
		}
		SDL_FreeSurface(text_surface);		text_surface = nullptr;
		SDL_DestroyTexture(text_texture);	text_texture = nullptr;
	}

	void RenderGlyphs(uint16_t glyph, int x, int y, int size, SDL_Color color) {
		TTF_Font* glyph_font = FontSelectManager(true, size);

		SDL_Surface* glyph_surface = TTF_RenderGlyph_Blended(glyph_font, glyph, color);
		SDL_Texture* glyph_texture = SDL_CreateTextureFromSurface(renderer, glyph_surface);

		int glyph_width = 0, glyph_height = 0;
		SDL_QueryTexture(glyph_texture, NULL, NULL, &glyph_width, &glyph_height);

		SDL_Rect glyph_rect{ x, y, glyph_width, glyph_height };
		SDL_RenderCopy(renderer, glyph_texture, NULL, &glyph_rect);

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

		TTF_CloseFont(default_regular_font);		default_regular_font = nullptr;
		TTF_CloseFont(default_light_font);			default_light_font = nullptr;
		TTF_CloseFont(default_semi_light_font);		default_semi_light_font = nullptr;
		TTF_CloseFont(default_semi_bold_font);		default_semi_bold_font = nullptr;
		TTF_CloseFont(default_bold_font);			default_bold_font = nullptr;
		TTF_CloseFont(default_glyph_font);			default_glyph_font = nullptr;

		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}