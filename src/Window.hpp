#pragma once

#include "Color.hpp"

#include <SFML/Graphics.hpp>

namespace Rasterbox {
    class Window {
        public:
            const unsigned int width, height;

            Window(unsigned int width, unsigned int height, const char* window_title);
            ~Window();

            void step();
            void setPix(unsigned int x, unsigned int y, Color color);
            void setPix(unsigned int n, Color color);
            bool isOpen();
            void clear();
            void writePixels(Rasterbox::Color (*func)(unsigned int, unsigned int));

        private:
            sf::Uint8* pixels;
            sf::RenderWindow window;

            void draw();
    };
} // namespace Rasterbox
