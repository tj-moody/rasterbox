#pragma once

#include "Color.hpp"

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

namespace rb {
    class Window {
        public:
            const unsigned int width, height;

            Window(unsigned int width,
                   unsigned int height,
                   const char* window_title);
            ~Window();

            void draw();
            void handleInput();
            void step();

            bool isOpen();
            void fill(rb::Color);

            void setPixel(unsigned int x, unsigned int y, Color color);
            void setPixel(glm::vec2 p, Color color);
            void setPixel(unsigned int n, Color color);
            void effectPass(void (*func)(rb::Window& window));
            void line(glm::vec2 p0, glm::vec2 p1, rb::Color color);
            void triangle(glm::vec2 p0,
                          glm::vec2 p1,
                          glm::vec2 p2,
                          rb::Color color);

        private:
            sf::Uint8* pixels;
            sf::RenderWindow window;
    };
} // namespace rb
