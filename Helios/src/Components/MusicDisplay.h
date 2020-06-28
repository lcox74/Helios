#pragma once

#include "Component.h"
#include "ProcessHelper.h"

namespace Helios {

    class MusicDisplay : public Component
    {
    public:
        MusicDisplay(unsigned char unique_identifier, int x, int y) {
            this->uid = unique_identifier;
            this->transform.x = x;
            this->transform.y = y;

            // Size from https://github.com/JamesDearlove/Selene/blob/master/src/Selene/Components/MusicDisplay.xaml
            this->transform.height = 30;
            this->transform.width = 400;

            // Set default status
            this->t = -1;
            this->last_call = 0;
        }
        ~MusicDisplay() {}

        void Update() override { 
            if (SDL_GetTicks() > last_call + 1000) {
                std::string now_song = GetSpotifyTrackInfo();
                if (now_song != current_song) {
                    current_song = now_song;
                    this->render_flag = true;
                }

                last_call = SDL_GetTicks();
            }
            
        }
        void Render() override {
            //RenderScrollingText("Hello There! I'm writing a big thing", this->transform.x, this->transform.y, 100, t);
            //t += 0.016f;
            RenderText(current_song.c_str(), this->transform.x + ((int)(this->transform.width / 2.0f)), this->transform.y + 4, TEXT_ALIGN::CENTRE);
            this->render_flag = false;
        }

        std::string GetSpotifyTrackInfo() { return Process::GetWindowNameByProcessName("Spotify"); }

    private:
        float t;
        std::string current_song;
        Uint32 last_call;
    };
}