#pragma once

#include <math.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Component.h"

namespace Helios
{
    // Segoe MDL2 icon list https://docs.microsoft.com/en-us/windows/uwp/design/style/segoe-ui-symbol-font
    unsigned short int battery_level_glyphs_horizontal [] = {
        0xE850, 0xE851, 0xE852, 0xE853, 0xE854, 0xE855, 0xE856, 0xE857, 0xE858, 0xE859, 0xE83F, // Normal
        0xE85A, 0xE85B, 0xE85C, 0xE85D, 0xE85E, 0xE85F, 0xE860, 0xE861, 0xE862, 0xE83E, 0xEA93, // Charging
        0xE996                                                                                  // Unknown
    };

    // Segoe MDL2 icon list https://docs.microsoft.com/en-us/windows/uwp/design/style/segoe-ui-symbol-font
    unsigned short int battery_level_glyphs_vertical [] = {
        0xF5F2, 0xF5F3, 0xF5F4, 0xF5F5, 0xF5F6, 0xF5F7, 0xF5F8, 0xF5F9, 0xF5FA, 0xF5FB, 0xF5FC, // Normal
        0xF5FD, 0xF5FE, 0xF5FF, 0xF600, 0xF601, 0xF602, 0xF603, 0xF604, 0xF605, 0xF606, 0xF607, // Charging
        0xF608                                                                                  // Unknown
    };

    class Battery : public Component
    {
    public:
        Battery(unsigned char unique_identifier, int x, int y) {
            this->uid = unique_identifier;
            this->transform.x = x;
            this->transform.y = y;

            // Size from https://github.com/JamesDearlove/Selene/blob/master/src/Selene/Components/BatteryDisplay.xaml
            this->transform.height = 30;
            this->transform.width = 60;

            // Set default status
            this->battery_status = 0;
            this->battery_orientation = true;
        }
        ~Battery() {}

        void Update() override { 
            if (SDL_GetTicks() > battery_timer + battery_timer_delay) 
                UpdateBatteryStatus(); 
        }
        void Render() override { 
            RenderGlyphs(GetBatteryGlyph(battery_percentage), this->transform.x, this->transform.y + 7);
            std::string percent_text = std::to_string(battery_percentage) + "%";
            RenderText(percent_text.c_str(), this->transform.x + 26, this->transform.y + 4);

            battery_time_remaining_prev = battery_time_remaining;
            battery_percentage_prev = battery_percentage;
            this->render_flag = false;
        }

        void UpdateBatteryStatus() {
            battery_status = ((int)SDL_GetPowerInfo(&battery_time_remaining, &battery_percentage) >= SDL_POWERSTATE_CHARGING) ? 1 : 0;
            battery_timer = SDL_GetTicks();

            if (battery_time_remaining != battery_time_remaining_prev || battery_percentage != battery_percentage_prev) this->render_flag = true;
        }

    private:
        unsigned char battery_status;   // 0 = Normal; 1 = Charging; 2 = Battery Saver
        bool battery_orientation;       // False = Horizontal; True = Vertical

        unsigned int battery_timer = 0, battery_timer_delay = 1000;
        int battery_time_remaining = 0, battery_percentage = 0;
        int battery_time_remaining_prev = 0, battery_percentage_prev = 0;

        // Quick method to calculate the battery level (0 - 10)
        inline unsigned char PercentageToLevel(double percent) { return (unsigned char)floor((percent + 4) / 10); }

        // Returns the right glyph for a given percentage
        unsigned short int GetBatteryGlyph(double percent) {
            unsigned char percent_level = PercentageToLevel(percent);
            unsigned short int* glyph_set = (battery_orientation) ? battery_level_glyphs_vertical : battery_level_glyphs_horizontal;

            // Use case test
            if (percent_level > 10) return glyph_set[22];
            if (battery_status > 1)  battery_status = 0;

            return glyph_set[percent_level + (11 * battery_status)];
        }
    };
}