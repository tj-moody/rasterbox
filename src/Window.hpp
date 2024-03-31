#pragma once

#include "Color.hpp"
#include "Mesh.hpp"

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace rb {
    class Window {
        public:
            const unsigned int width, height;
            unsigned int frame_num = 0;

            Window(unsigned int width,
                   unsigned int height,
                   std::string&& window_title);

            void writePixels();
            void handleInput();
            void step();

            auto isOpen() const -> bool;
            void fill(const rb::Color&);

            void setDepth(unsigned int n, float depth);
            void setDepth(unsigned int x, unsigned int y, float depth);

            auto getDepth(unsigned int x, unsigned int y) -> float;

            void setPixel(unsigned int x, unsigned int y, const Color& color);
            void setPixel(glm::vec2 p, const Color& color);
            void setPixel(unsigned int n, const Color& color);

            void effectPass(void (*func)(rb::Window& window));
            void drawLine(const glm::vec2& p0,
                          const glm::vec2& p1,
                          const rb::Color& color);
            void rasterizeTriangle(const glm::vec3& t0,
                                   const glm::vec3& t1,
                                   const glm::vec3& t2,
                                   const rb::Color& color);
            auto getTime() const -> float;
            void renderMesh(const rb::Mesh& mesh);

            void writeDepthBuffer();

            std::vector<float> depthBuffer;

        private:
            sf::RenderWindow window;
            std::string window_title;
            sf::Uint8* pixels;

            void clearDepthBuffer();
            auto getFrameRate() const -> int;
    };
} // namespace rb
