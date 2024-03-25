#pragma once

#include "Color.hpp"

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace rb {
    class Window {
        public:
            const unsigned int width, height;

            Window(unsigned int width,
                   unsigned int height,
                   std::string&& window_title);
            ~Window();

            void draw();
            void handleInput();
            void step();

            bool isOpen() const;
            void fill(const rb::Color&);

            void setPixel(unsigned int x, unsigned int y, Color color);
            void setPixel(glm::vec2 p, Color color);
            void setPixel(unsigned int n, Color color);
            void effectPass(void (*func)(rb::Window& window));
            void line(const glm::vec2& p0,
                      const glm::vec2& p1,
                      const rb::Color& color);
            void triangle(const glm::vec2& p0,
                          const glm::vec2& p1,
                          const glm::vec2& p2,
                          const rb::Color& color);
            float getTime() const;

        private:
            sf::RenderWindow window;
            std::string window_title;
            sf::Uint8* pixels;

            std::string getFrameRate() const;
    };
} // namespace rb
