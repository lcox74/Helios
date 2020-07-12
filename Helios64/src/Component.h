#pragma once

#include <WinUser.h>

#include <SDL2/SDL.h>

#include "ResourceManager.h"

namespace Helios {

	struct Transform {
		union {
			struct {
				int x, w;
				int y, h;
			};
			struct {
				int x, width;
				int y, height;
			};
		};
	};

	class Component {
	public:
		Component(int m) {
			this->monitor = m;
			this->transform.x = 0;
			this->transform.y = 0;
		}
		~Component() { }

		// Can be called multiple times a frame
		virtual void Update() = 0;
		// Called onces a frame to render the component
		virtual void Render() = 0;

		// Trigger on mouse click
		virtual void OnClick(int x, int y, int button) { }

		// Trigger on mouse enter component
		virtual void OnEnter(int x, int y) { }
		// Trigger on mouse hover over component
		virtual void OnHover(int x, int y) { }
		// Trigger when mouse leaves component
		virtual void OnLeave(int x, int y) { }

		void EventHandler(const SDL_Event* e) {
			POINT p;
			GetCursorPos(&p);

			const bool in_bounds = this->GetInBounds(e->motion.x, e->motion.y) | this->GetInBounds(p.x, p.y);

			switch (e->type) {
			case SDL_MOUSEBUTTONDOWN: // Handle Click Events
				if (this->GetInBounds(e->button.x, e->button.y))
					this->OnClick(e->button.x, e->button.y, e->button.button);
				break;
			case SDL_MOUSEMOTION: // Handle Mouse Movement Events
				if (in_bounds && !this->mouse_in_component) {
					this->OnEnter(e->motion.x, e->motion.y);
					this->mouse_in_component = true;
				}
				else if (in_bounds && this->mouse_in_component) {
					this->OnHover(e->motion.x, e->motion.y);
				}
				else if (!in_bounds && this->mouse_in_component) {
					this->OnLeave(e->motion.x, e->motion.y);
					this->mouse_in_component = false;
				}
				break;
			case SDL_WINDOWEVENT:
				if (e->window.event == SDL_WINDOWEVENT_LEAVE) {
					if (!in_bounds && this->mouse_in_component) {
						this->OnLeave(p.x, p.y);
						this->mouse_in_component = false;
					}
				}
				else if (e->window.event == SDL_WINDOWEVENT_ENTER) { 
					if (in_bounds && !this->mouse_in_component) {
						this->OnEnter(p.x, p.y);
						this->mouse_in_component = true;
					}
				}
				break;
				
			}
		}

		// Returns the components transform (position and size)
		Transform GetTransform() const { return this->transform; }
		int GetWidth() const { return this->transform.w; }

		// Checks if a given position is inside the component
		bool GetInBounds(int x, int y) const {
			return	(x >= this->transform.x) && (x <= this->transform.x + this->transform.w) &&
					(y >= this->transform.y) && (y <= this->transform.y + this->transform.h);
		}

		void SetXPosition(int x) { this->transform.x = x; }
		const bool ComponentFocused() const { return this->mouse_in_component; }

	protected:
		Transform transform;

		int monitor;

		bool render_flag = true;
	private:
		bool mouse_in_component = false;
	};

}
