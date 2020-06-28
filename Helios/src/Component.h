#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Renderer.h"

namespace Helios
{
	struct Transform {
		union {
			struct {
				unsigned short int x, w;
				unsigned char y, h;
			};
			struct {
				unsigned short int x, width;
				unsigned char y, height;
			};
		};

		// Get left most side of the transform
		unsigned short int GetLeftBound() { return x; }
		// Get right most side of the transform
		unsigned short int GetRightBound() { return x + w; }
		// Get top most side of the transform
		unsigned short int GetTopBound() { return y; }
		// Get bottom most side of the transform
		unsigned short int GetBottomBound() { return y + h; }
	};

	class Component {
	public:
		// Can be called multiple times a frame
		virtual void Update() = 0;
		// Called onces a frame to render the component
		virtual void Render() = 0;

		// Trigger on mouse click
		virtual void OnClick() { }

		// Trigger on mouse enter component
		virtual void OnEnter() { }
		// Trigger on mouse hover over component
		virtual void OnHover() { }
		// Trigger when mouse leaves component
		virtual void OnLeave() { }

		// Trigger when component is created
		virtual void OnAwake() { }
		// Trigger once component is initialised and on first update
		virtual void OnStart() { }
		// Trigger when application is closing
		virtual void OnClose() { }

		// Returns the components transform (position and size)
		Transform GetTransform() { return this->transform; }

		// Checks if a given position is inside the component
		bool GetInBounds(int x, int y) {
			return	(x >= this->transform.x) && (x <= this->transform.x + this->transform.w) &&
				(y >= this->transform.y) && (y <= this->transform.y + this->transform.h);
		}

		// Get the components ID (This should be unique; MAX 256 Components)
		unsigned char GetId() { return this->uid; }
		bool GetRenderFlag() { return this->render_flag; }

	protected:
		unsigned char uid; // Unique identifier for the component
		Transform transform; // Components position and size
		bool render_flag = true;
	};
}

#endif