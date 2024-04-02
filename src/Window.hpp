#pragma once

#include "Color.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

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

            void write_pixels();
            void handle_input();
            void step();

            auto is_open() const -> bool;
            void fill(const rb::Color&);

            void set_depth(unsigned int n, float depth);
            void set_depth(unsigned int x, unsigned int y, float depth);

            auto get_depth(unsigned int x, unsigned int y) -> float;

            void set_pixel(unsigned int x, unsigned int y, const Color& color);
            void set_pixel(glm::vec2 p, const Color& color);
            void set_pixel(unsigned int n, const Color& color);

            void effect_pass(void (*func)(rb::Window& window));
            void draw_line(const glm::vec2& p0,
                          const glm::vec2& p1,
                          const rb::Color& color);
            void rasterize_triangle(const glm::vec3& t0,
                                   const glm::vec3& t1,
                                   const glm::vec3& t2,
                                   const glm::vec2& uv0,
                                   const glm::vec2& uv1,
                                   const glm::vec2& uv2,
                                   float color,
                                   const rb::Texture& uv_texture);
            auto get_time() const -> float;
            void render_mesh(const rb::Mesh& mesh);

            void write_depth_buffer();
            void set_limit_framerate(unsigned int);

            std::vector<float> depth_buffer;

        private:
            sf::RenderWindow window;
            std::string window_title;
            sf::Uint8* pixels;

            void clear_depth_buffer();
            auto get_framerate() const -> int;
    };
} // namespace rb
