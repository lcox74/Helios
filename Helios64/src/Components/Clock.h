#pragma once

#include <math.h>
#include <string>
#include <chrono>
#include <ctime>   

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Component.h"

namespace Helios {
    class Clock : public Component
    {
    public:
        Clock(int m) : Component(m) {
            // Size from https://github.com/JamesDearlove/Selene/blob/master/src/Selene/Components/TimeDisplay.xaml
            this->transform.height = 30;
            this->transform.width = 130;
        }
        ~Clock() {}

        void Update() override {
            __time64_t long_time;
            struct tm timeinfo;
            _time64(&long_time);

            localtime_s(&timeinfo, &long_time);

            char time_now[12];
            strftime(time_now, 80, "%I:%M:%S %p", &timeinfo);
            current_time = time_now;

            char date_now[12];
            strftime(date_now, 80, "%d %b %Y", &timeinfo);
            current_date = date_now;

        }

        void Render() override {
            if (!ComponentFocused()) {
                Helios::Resources::RenderGlyph(this->monitor, 0xE121, this->transform.x, this->transform.y + 7);
                Helios::Resources::RenderText(this->monitor, current_time.c_str(), this->transform.x + 26, this->transform.y + 4);
            }
            else {
                Helios::Resources::RenderGlyph(this->monitor, 0xE787, this->transform.x, this->transform.y + 8);
                Helios::Resources::RenderText(this->monitor, current_date.c_str(), this->transform.x + 28, this->transform.y + 4);
            }
        }

    private:
        std::string current_time, current_date;
    };
}