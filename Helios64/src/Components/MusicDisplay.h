#pragma once

#include "Component.h"
#include "System.h"

namespace Helios {

    // Segoe MDL2 icon list https://docs.microsoft.com/en-us/windows/uwp/design/style/segoe-ui-symbol-font
    const unsigned short int media_glyphs[] = {
        0xE768, 0xE769, 0xE892, 0xE893
    };

    class MusicDisplay : public Component
    {
    public:
        MusicDisplay(int m) : Component(m) {
            // Size from https://github.com/JamesDearlove/Selene/blob/master/src/Selene/Components/MusicDisplay.xaml
            this->transform.height = 30;
            this->transform.width = 400;

            // Set properties
            this->current_song = Helios::System::get_window_handle_by_process_name("Spotify");
        }
        ~MusicDisplay() {}

        void SlowUpdate() override {
            const char* temp = Helios::System::get_name_of_window(current_song);
            if (this->current_song == nullptr) this->current_song = Helios::System::get_window_handle_by_process_name("Spotify");
            
            // Handle when spotify is closed
            if (temp == "") {
                this->saved_song_record = "";
                return;
            }
            
            // Handle when music is paused
            if (strcmp(temp, "Spotify") && strcmp(temp, "Spotify Premium")) {
                this->saved_song_record = temp;
                this->music_paused = false;
            }
            else if (temp != "") this->music_paused = true;
        }
        void Render() override {

            if (this->ComponentFocused()) {
                const unsigned short int media_action = (!this->music_paused) ? media_glyphs[1] : media_glyphs[0];
                Helios::Resources::RenderGlyph(this->monitor, media_action, this->transform.x + (int)(this->transform.w / 2.0f), this->transform.y + 7);
                Helios::Resources::RenderGlyph(this->monitor, media_glyphs[2], this->transform.x + (int)(this->transform.w / 2.0f) - 30, this->transform.y + 7);
                Helios::Resources::RenderGlyph(this->monitor, media_glyphs[3], this->transform.x + (int)(this->transform.w / 2.0f) + 30, this->transform.y + 7);
            }
            else {
                Helios::Resources::RenderText(this->monitor, this->saved_song_record, this->transform.x + (int)(this->transform.w/2.0f), this->transform.y + 4, 16, Helios::Resources::CENTRE);
            }
        }

    private:
        HWND current_song;
        bool music_paused = false;
        const char* saved_song_record = "";
    };
}