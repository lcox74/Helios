#pragma once

#include "Component.h"
#include "System.h"

namespace Helios {

    class MusicDisplay : public Component
    {
    public:
        MusicDisplay(int m) : Component(m) {
            // Size from https://github.com/JamesDearlove/Selene/blob/master/src/Selene/Components/MusicDisplay.xaml
            this->transform.height = 30;
            this->transform.width = 400;

            // Set default status
            this->t = -1;
            this->last_call = 0;
            this->current_call = 0;

            this->current_song = Helios::System::get_window_handle_by_process_name("Spotify");
        }
        ~MusicDisplay() {}

        void Update() override { }
        void Render() override {
            Helios::Resources::RenderText(this->monitor, Helios::System::get_name_of_window(current_song), this->transform.x, this->transform.y + 4, 16, Helios::Resources::CENTRE);
            this->render_flag = false;
        }

    private:
        float t;
        HWND current_song;
        Uint32 last_call, current_call;
    };
}