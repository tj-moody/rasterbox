#include "Window.hpp"

#include <cstdint>
#include <format>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#define BACKGROUND_COLOR sf::Color(36, 37, 38, 255)

#define FOV_Y        (glm::radians(90.f))
#define ASPECT_RATIO 1.f
#define Z_NEAR       0.1f
#define Z_FAR        1000.f

rb::Window::Window(unsigned int width,
                   unsigned int height,
                   std::string&& window_title)
    : width(width),
      height(height),
      window(sf::RenderWindow(sf::VideoMode({ width, height }), window_title)),
      window_title(window_title),
      // each pixel takes 4 Uint8's (RGBA)
      pixels(new uint8_t[width * height * 4]),
      sprite(texture) {
    this->depth_buffer.resize(width * height, -INFINITY);
    this->clear_depth_buffer();
    // this->window.setFramerateLimit(120);
    bool succeeded
        = texture.resize(sf::Vector2u(this->width, this->height), false);
    if (!succeeded) {
        // TODO:
    }
}

void rb::Window::write_pixels() {
    // PERF: possibly attach `texture` and `sprite` as
    // private fields to only allocate once, maybe use a different data flow
    // than sf::Uint8* -> sf::Texture -> sf::Sprite -> window.draw()
    texture.update(this->pixels);
    sprite = sf::Sprite(texture);
    this->window.draw(sprite);
}


void rb::Window::step() {
    this->set_title();
    this->handle_input();
    this->write_pixels();
    this->window.display();
    this->clear_depth_buffer();
    this->frame_num++;
}

void rb::Window::set_title() {
    float frame_time       = this->get_frame_time();
    unsigned int framerate = 1 / frame_time;
    float frame_time_ms    = 1000 * frame_time;

    this->window.setTitle(
        sf::String(std::format("{} - FPS: {}, {:.2f} ms elapsed.",
                               this->window_title,
                               framerate,
                               frame_time_ms)));
}

void rb::Window::set_limit_framerate(unsigned int limit) {
    this->window.setFramerateLimit(limit);
}

void rb::Window::handle_input() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* keyPressed
                   = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                window.close();
            }
        }
    }
}

void rb::Window::set_pixel(unsigned int x,
                           unsigned int y,
                           const rb::Color& color) {
    if (x >= this->width || x < 0 || y >= this->height || y < 0) { return; }
    const unsigned int index = (y * this->width * 4) + (x * 4);

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

void rb::Window::set_depth(unsigned int n, float depth) {
    this->depth_buffer[n] = depth;
}
void rb::Window::set_depth(unsigned int x, unsigned int y, float depth) {
    if (x >= this->width || x < 0 || y >= this->height || y < 0) { return; }
    this->depth_buffer[this->width * y + x] = depth;
}

auto rb::Window::get_depth(unsigned int x, unsigned int y) -> float {
    if (x >= this->width || x < 0 || y >= this->height || y < 0) {
        return -INFINITY;
    }
    return this->depth_buffer[this->width * y + x];
}

void rb::Window::set_pixel(glm::uvec2 p, const rb::Color& color) {
    if (p.x >= this->width || p.x < 0 || p.y >= this->height || p.y < 0) {
        return;
    }
    const unsigned int index = (p.y * this->width * 4) + (p.x * 4);

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

void rb::Window::set_pixel(unsigned int n, const rb::Color& color) {
    if (n < 0 || n > this->width * this->height) { return; }
    const unsigned int index = n * 4;

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

auto rb::Window::is_open() const -> bool { return this->window.isOpen(); }

sf::Clock elapsed;
auto rb::Window::get_frame_time() -> float {
    const float elapsed_time = elapsed.getElapsedTime().asSeconds();
    elapsed.restart();

    return elapsed_time;
}

void rb::Window::fill(const rb::Color& color) {
    for (unsigned int n = 0; n < this->width * this->height; n++) {
        this->set_pixel(n, color);
    }
}

void rb::Window::effect_pass(void (*effect)(rb::Window& window)) {
    effect(*this);
}

void rb::Window::draw_line(const glm::vec2& p0,
                           const glm::vec2& p1,
                           const rb::Color& color) {
    int x0 = p0.x;
    int y0 = p0.y;
    int x1 = p1.x;
    int y1 = p1.y;

    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int derror2 = std::abs(dy) * 2;
    int error2  = 0;

    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            this->set_pixel(y, x, color);
        } else {
            this->set_pixel(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

auto edge_function(const glm::vec3& p1,
                   const glm::vec3& p2,
                   float x,
                   float y) -> float {
    return (p2.x - p1.x) * (y - p1.y) - (p2.y - p1.y) * (x - p1.x);
}

#define CAMERA_POS 2
// Currently leaves z in world space
auto clip_to_screen(const unsigned int& width,
                    const unsigned int& height,
                    const glm::vec3& pos) -> glm::vec3 {
    const float screen_x = width * (pos.x + 1) / 2;
    const float screen_y = height * (1 - (pos.y + 1) / 2);

    // x /= 4;
    // y /= 4;
    // x += 300;
    // y += 300;

    return glm::vec3(screen_x, screen_y, pos.z);
}

void rb::Window::fragment_shader(const unsigned int& x,
                                 const unsigned int& y,
                                 const float& depth,
                                 rb::Color&& surface_color,
                                 glm::vec3&& normal,
                                 const glm::vec3& light_dir) {
    const float surface_light = 255 * std::abs(glm::dot(light_dir, normal));
    surface_color.scale(surface_light);

    this->set_pixel(x, y, surface_color);
    this->set_depth(x, y, depth);
}

void rb::Window::vertex_shader(glm::vec3&& v0,
                               glm::vec3&& v1,
                               glm::vec3&& v2,
                               const glm::vec2& uv0,
                               const glm::vec2& uv1,
                               const glm::vec2& uv2,
                               const glm::vec3& normal0,
                               const glm::vec3& normal1,
                               const glm::vec3& normal2,
                               const rb::Texture& uv_texture,
                               const glm::vec3& light_dir) {
    // Degenerate triangles
    if (v0.y == v1.y && v0.y == v2.y) { return; }

    // Triangles outside of view frustum
    if ((v0.x < -1.f && v1.x < -1.f && v2.x < -1.f)
        || (v0.x > +1.f && v1.x > +1.f && v2.x > +1.f)
        // || (v0.z < -1.f && v1.z < -1.f && v2.z < -1.f)
        // || (v0.z > +1.f && v1.z > +1.f && v2.z > +1.f)
        || (v0.y < -1.f && v1.y < -1.f && v2.y < -1.f)
        || (v0.y > +1.f && v1.y > +1.f && v2.y > +1.f)) {
        return;
    }

    v0 = clip_to_screen(width, height, v0);
    v1 = clip_to_screen(width, height, v1);
    v2 = clip_to_screen(width, height, v2);

    const float area = edge_function(v0, v1, v2.x, v2.y);
    if (area < 1) { return; }

    const unsigned int bb_neg_x = std::min(v0.x, std::min(v1.x, v2.x));
    const unsigned int bb_pos_x = std::max(v0.x, std::max(v1.x, v2.x));
    const unsigned int bb_neg_y = std::min(v0.y, std::min(v1.y, v2.y));
    const unsigned int bb_pos_y = std::max(v0.y, std::max(v1.y, v2.y));

    for (unsigned int x = bb_neg_x; x <= bb_pos_x; x++) {
        for (unsigned int y = bb_neg_y; y <= bb_pos_y; y++) {
            // TODO: Move more of this to frag shader
            const float v0_area = edge_function(v1, v2, x, y);
            const float v1_area = edge_function(v2, v0, x, y);
            const float v2_area = edge_function(v0, v1, x, y);

            if (v2_area < 0 || v0_area < 0 || v1_area < 0) { continue; }

            const float weight0 = v0_area / area;
            const float weight1 = v1_area / area;
            // const float weight2 = val_0_1 / area;
            const float weight2 = 1 - weight0 - weight1;

            const float depth
                = weight0 * v0.z + weight1 * v1.z + weight2 * v2.z;

            const float current_depth = this->get_depth(x, y);
            if (current_depth > depth && current_depth != -INFINITY) {
                continue;
            }

            const glm::vec2 uv(
                weight0 * uv0.x + weight1 * uv1.x + weight2 * uv2.x,
                weight0 * uv0.y + weight1 * uv1.y + weight2 * uv2.y);

            rb::Color color = uv_texture.get(uv);

            // clang-format off
            glm::vec3 normal(
                weight0 * normal0.x + weight1 * normal1.x + weight2 * normal2.x,
                weight0 * normal0.y + weight1 * normal1.y + weight2 * normal2.y,
                weight0 * normal0.z + weight1 * normal1.z + weight2 * normal2.z
            );
            // clang-format on

            this->fragment_shader(x,
                                  y,
                                  depth,
                                  std::move(color),
                                  std::move(normal),
                                  light_dir);
        }
    }
}

// TODO: Will represent camera, currently unimplemented
constexpr glm::mat4 view_matrix = glm::mat4(1.0f);

glm::mat4 perspective = glm::perspective(FOV_Y, ASPECT_RATIO, Z_NEAR, Z_FAR);

void rb::Window::render_mesh(const rb::Mesh& mesh) {
    // glm::vec3 light_dir(1, -1, 1);
    glm::vec3 light_dir = glm::normalize(glm::vec3(0, 0, 1));

    // clang-format off
    // #pragma omp parallel for
    // clang-format on
    for (unsigned int i = 0; i < mesh.vertex_indices.size() - 3; i += 3) {
        const unsigned int v_i0 = mesh.vertex_indices[i];
        const unsigned int v_i1 = mesh.vertex_indices[i + 1];
        const unsigned int v_i2 = mesh.vertex_indices[i + 2];

        glm::vec3 v0 = mesh.vertices[v_i0];
        glm::vec3 v1 = mesh.vertices[v_i1];
        glm::vec3 v2 = mesh.vertices[v_i2];

        const glm::mat4 model
            = mesh.model_scale * mesh.model_translation * mesh.model_rotation;

        v0 = perspective * view_matrix * model * glm::vec4(v0, 1);
        v1 = perspective * view_matrix * model * glm::vec4(v1, 1);
        v2 = perspective * view_matrix * model * glm::vec4(v2, 1);

        const unsigned int uv_i0 = mesh.uv_indices[i];
        const unsigned int uv_i1 = mesh.uv_indices[i + 1];
        const unsigned int uv_i2 = mesh.uv_indices[i + 2];

        const glm::vec2& uv0 = mesh.uv_coordinates[uv_i0];
        const glm::vec2& uv1 = mesh.uv_coordinates[uv_i1];
        const glm::vec2& uv2 = mesh.uv_coordinates[uv_i2];

        const unsigned int n_i0 = mesh.normal_indices[i];
        const unsigned int n_i1 = mesh.normal_indices[i + 1];
        const unsigned int n_i2 = mesh.normal_indices[i + 2];

        glm::vec3 normal0
            = mesh.model_rotation * glm::vec4(mesh.normals[n_i0], 0);
        glm::vec3 normal1
            = mesh.model_rotation * glm::vec4(mesh.normals[n_i1], 0);
        glm::vec3 normal2
            = mesh.model_rotation * glm::vec4(mesh.normals[n_i2], 0);


        this->vertex_shader(std::move(v0),
                            std::move(v1),
                            std::move(v2),
                            uv0,
                            uv1,
                            uv2,
                            std::move(normal0),
                            std::move(normal1),
                            std::move(normal2),
                            mesh.uv_texture,
                            light_dir

        );
        // this->draw_depth_buffer();
        // this->write_pixels();
        // this->window.display();
    };
}

void rb::Window::clear_depth_buffer() {
    for (auto& pixel_depth : this->depth_buffer) { pixel_depth = -INFINITY; }
};


void rb::Window::draw_depth_buffer() {
    for (unsigned int n = 0; n < this->width * this->height; n++) {
        const float depth = this->depth_buffer[n];
        this->set_pixel(n, rb::Color(255 * depth));
    }
}
