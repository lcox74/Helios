#pragma once

#include <math.h>
#include <string>
#include <chrono>
#include <ctime>   

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "../Component.h"

namespace Helios {
    class Clock : public Component
    {
    public:
        Clock(unsigned char unique_identifier, int x, int y) {
            this->uid = unique_identifier;
            this->transform.x = x;
            this->transform.y = y;

            // Size from https://github.com/JamesDearlove/Selene/blob/master/src/Selene/Components/TimeDisplay.xaml
            this->transform.height = 30;
            this->transform.width = 140;

            current_time = previous_time = "";
        }
        ~Clock() {}

        void Update() override { 
            __time64_t long_time;
            struct tm timeinfo;
            _time64(&long_time);

            localtime_s(&timeinfo, &long_time);

            char current[12];
            strftime(current, 80, "%I:%M:%S %p", &timeinfo);
            current_time = current;
            
            if (current_time != previous_time) this->render_flag = true;
        }
        void Render() override {
            RenderGlyphs(0xE121, this->transform.x, this->transform.y + 7, 16);
            RenderText(current_time.c_str(), this->transform.x + 26, this->transform.y + 4);

            previous_time = current_time;
            this->render_flag = false;
        }

    private:
        std::string current_time, previous_time;
    };
}