#include "HeliosBar.h"

Helios::HeliosBar::HeliosBar(unsigned char m, int x, int y, int width)
{
	// Set size
	this->size = { x, y, x + width, y + 30 };
	this->monitor = m;

	// Setup window
	this->window = SDL_CreateWindow("Helios", x, y, width, 30, 
		SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR);
	if (!this->window) return;
	
	// Setup renderer
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_SOFTWARE);
	if (!this->renderer) return;
	Helios::Resources::SetRenderer(monitor, this->renderer);

	// Setup handle
	SetWindowHandle();
	if (!this->handle) return;

	// Set other Windows Items
	Helios::System::disable_hiding_on_peaking(this->handle);
	Helios::System::hide_application_from_task_switcher(this->handle);
	Helios::System::set_window_to_top(this->handle);

	// Set HeliosBar as a Windows Appbar
	Helios::System::register_appbar(this->handle, &this->appbar_data);
	Helios::System::set_appbar_location(&this->appbar_data, &this->size);

	// Initialise Component Layout
	this->layout = new ComponentLayout();
	
	this->AddComponentLeft(new Launcher(this->monitor));

	this->SetMiddleComponent(new MusicDisplay(this->monitor));

	this->AddComponentRight(new Clock(this->monitor));
	this->AddComponentRight(new Battery(this->monitor));
}

Helios::HeliosBar::~HeliosBar()
{
	this->Clean();
}

void Helios::HeliosBar::Clean()
{
	// Clean Components
	this->layout->left_components.clear();
	this->layout->right_components.clear();
	delete this->layout->center_component;
	this->layout = nullptr;

	Helios::System::unregister_appbar(&this->appbar_data);
	SDL_DestroyWindow(window);
}

Uint32 last_slow_update_call = 0, current_slow_update_call;
void Helios::HeliosBar::Update()
{
	current_slow_update_call = SDL_GetTicks();

	// Update Components
	for (auto const& c : this->layout->left_components) { 
		c->Update();
		if (current_slow_update_call < last_slow_update_call + 1000) c->SlowUpdate();
	}
	for (auto const& c : this->layout->right_components) { 
		c->Update();
		if (current_slow_update_call < last_slow_update_call + 1000) c->SlowUpdate();
	}
	if (this->layout->center_component) { 
		this->layout->center_component->Update();
		if (current_slow_update_call < last_slow_update_call + 1000) this->layout->center_component->SlowUpdate();
	}

	if (current_slow_update_call < last_slow_update_call + 1000) {
		last_slow_update_call = current_slow_update_call;
	}
}

void Helios::HeliosBar::Render()
{
	// Clear Window
	this->Clear();

	int left_handle = 0;
	int right_handle = this->size.right;

	// Render Components
	for (auto& c : this->layout->left_components) {
		c->SetXPosition(left_handle);
		c->Render();
		left_handle += c->GetWidth();
	}
	for (auto& c : this->layout->right_components) {
		right_handle -= c->GetWidth();
		c->SetXPosition(right_handle);
		c->Render();
	}
	if (this->layout->center_component) {
		this->layout->center_component->SetXPosition((int)(this->size.right / 2.0f - (int)(this->layout->center_component->GetWidth() / 2.0f)));
		this->layout->center_component->Render();
	}

	SDL_RenderPresent(this->renderer);
}

void Helios::HeliosBar::AddComponentLeft(Component* c)
{
	this->layout->left_components.push_back(c);
}

void Helios::HeliosBar::AddComponentRight(Component* c)
{
	this->layout->right_components.push_back(c);
}

void Helios::HeliosBar::SetMiddleComponent(Component* c)
{
	delete this->layout->center_component;
	this->layout->center_component = std::move(c);
}

void Helios::HeliosBar::SendEvent(const SDL_Event* e)
{
	// Send Events to Components
	for (auto const& c : this->layout->left_components) c->EventHandler(e);
	for (auto const& c : this->layout->right_components) c->EventHandler(e);
	if (this->layout->center_component) this->layout->center_component->EventHandler(e);
}

void Helios::HeliosBar::Clear()
{
	if (this->window == nullptr || this->renderer == nullptr) return;

	// Clears the renderer
	SDL_SetRenderDrawColor(this->renderer, this->clear_colour.r, this->clear_colour.g, this->clear_colour.b, 255);
	SDL_RenderClear(this->renderer);
}

void Helios::HeliosBar::SetWindowHandle()
{
	SDL_SysWMinfo info = SDL_SysWMinfo();

	// Calling the SDL Version to stop the bug with getting WM info
	SDL_VERSION(&info.version);

	// Getting the system info 
	if (SDL_GetWindowWMInfo(window, &info) == -1) return;

	this->handle = info.info.win.window;
}
