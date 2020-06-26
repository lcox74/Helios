#pragma once

#include <SDL2/SDL.h>

#include "../Component.h"

namespace Helios {
    class Launcher : public Component
    {
    public:
        Launcher(unsigned char unique_identifier, int x, int y) {
            this->uid = unique_identifier;
            this->transform.x = x;
            this->transform.y = y;

            // Size from https://github.com/JamesDearlove/Selene/blob/master/src/Selene/Components/LauncherButton.xaml
            this->transform.height = 30;
            this->transform.width = 40;

            this->render_flag = true;
        }
        ~Launcher() {}

        void Update() override { }
        void Render() override {
            RenderGlyphs(0xE71D, this->transform.x + 10, this->transform.y + 7, 16);
            this->render_flag = false;
        }

    };
}