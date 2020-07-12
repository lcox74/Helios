#pragma once

#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_syswm.h>

#include "System.h"
#include "Component.h"

#include "Components/Launcher.h"
#include "Components/Battery.h"
#include "Components/Clock.h"
#include "Components/MusicDisplay.h"

namespace Helios {
	struct ComponentLayout {
		std::vector<Component*> left_components;
		Component* center_component = nullptr;
		std::vector<Component*> right_components;
	};

	class HeliosBar {
	public:
		HeliosBar(unsigned char m, int x, int y, int width);

		~HeliosBar();
		void Clean();

		SDL_Renderer* GetRenderer() const { return this->renderer; }

		void Update();
		void Render();

		void AddComponentLeft(Component* c);
		void AddComponentRight(Component* c);
		void SetMiddleComponent(Component* c);

		void SendEvent(const SDL_Event* e);

		void Clear();

		unsigned char GetMonitor() const { return this->monitor; }
		RECT GetSize() const { return this->size; }

	private:
		// Window properties
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		HWND handle = nullptr;

		// Appbar Data
		PAPPBARDATA appbar_data = nullptr;

		// Components
		ComponentLayout* layout = nullptr;

		// Window size
		RECT size;

		// Clear Colour
		const SDL_Color clear_colour = { 10, 10, 10 };

		// Monitor assuming less than 256 avaliable monitors
		unsigned char monitor = 0;

		void SetWindowHandle();
	};
}