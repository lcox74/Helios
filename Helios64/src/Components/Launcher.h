#pragma once

#include <SDL2/SDL.h>

#include "Component.h"

namespace Helios {
    class Launcher : public Component
    {
    public:
        Launcher(int m) : Component(m) {
            // Size from https://github.com/JamesDearlove/Selene/blob/master/src/Selene/Components/LauncherButton.xaml
            this->transform.height = 30;
            this->transform.width = 40;

            this->render_flag = true;
        }
        ~Launcher() {}

        void Update() override { }
        void Render() override {
            Helios::Resources::RenderGlyph(this->monitor, 0xE71D, this->transform.x + 10, this->transform.y + 7);
            this->render_flag = false;
        }

    };
}