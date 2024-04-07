#include "Window.hpp"

#define BACKGROUND_COLOR sf::Color(36, 37, 38, 255)

rb::Window::Window(unsigned int width,
                   unsigned int height,
                   std::string&& window_title)
    : width(width),
      height(height),
      window(sf::RenderWindow(sf::VideoMode(width, height), window_title)),
      window_title(window_title),
      // each pixel takes 4 Uint8's (RGBA)
      pixels(new sf::Uint8[width * height * 4]) {
    this->depth_buffer.resize(width * height, -INFINITY);
    this->clear_depth_buffer();
    // this->window.setFramerateLimit(120);
}

sf::Texture texture;
sf::Sprite sprite;
void rb::Window::write_pixels() {
    // PERF: possibly attach `texture` and `sprite` as
    // private fields to only allocate once, maybe use a different data flow
    // than sf::Uint8* -> sf::Texture -> sf::Sprite -> window.draw()
    texture.create(this->width, this->height);
    texture.update(this->pixels);
    sprite = sf::Sprite(std::move(texture));
    this->window.draw(sprite);
}

void rb::Window::step() {
    this->handle_input();
    this->window.setTitle(this->window_title
                          + " - FPS: " + std::to_string(this->get_framerate()));
    this->write_pixels();
    this->window.display();
    this->clear_depth_buffer();
    this->frame_num++;
}

void rb::Window::set_limit_framerate(unsigned int limit) {
    this->window.setFramerateLimit(limit);
}

sf::Event event;
void rb::Window::handle_input() {
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) { this->window.close(); }
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
auto rb::Window::get_framerate() -> unsigned int {
    const float elapsed_time = elapsed.getElapsedTime().asSeconds();
    elapsed.restart();

    return (1. / elapsed_time);
}

void rb::Window::fill(const rb::Color& color) {
    for (unsigned int n = 0; n < this->width * this->height; n++) {
        this->set_pixel(n, color);
    }
}

void rb::Window::effect_pass(void (*effect)(rb::Window& window)) {
    effect(*this);
}

auto absolute(int x) -> int {
    if (x < 0) { return -x; }
    return x;
}

void rb::Window::draw_line(const glm::vec2& p0,
                           const glm::vec2& p1,
                           const rb::Color& color) {
    int x0 = p0.x;
    int y0 = p0.y;
    int x1 = p1.x;
    int y1 = p1.y;

    bool steep = false;
    if (absolute(x0 - x1) < absolute(y0 - y1)) {
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
                   const glm::vec3& p3) -> float {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

// TODO: Separate into vertex/fragment stages

// t0, t1, t2 all have (x, y) in screen space, but z in world space.
void rb::Window::rasterize_triangle(const glm::vec3& v0,
                                    const glm::vec3& v1,
                                    const glm::vec3& v2,
                                    const glm::vec2& uv0,
                                    const glm::vec2& uv1,
                                    const glm::vec2& uv2,
                                    const glm::vec3& normal0,
                                    const glm::vec3& normal1,
                                    const glm::vec3& normal2,
                                    const rb::Texture& uv_texture,
                                    const glm::vec3& light_dir) {
    if (v0.y == v1.y && v0.y == v2.y) { return; }

    // cull back-facing triangles
    const float val_0_1_2 = edge_function(v0, v1, v2);
    if (val_0_1_2 < 1) { return; }

    const unsigned int bounding_box_left = std::min(v0.x, std::min(v1.x, v2.x));
    const unsigned int bounding_box_right
        = std::max(v0.x, std::max(v1.x, v2.x));
    const unsigned int bounding_box_bottom
        = std::min(v0.y, std::min(v1.y, v2.y));
    const unsigned int bounding_box_top = std::max(v0.y, std::max(v1.y, v2.y));

    for (unsigned int x = bounding_box_left; x <= bounding_box_right; x++) {
        for (unsigned int y = bounding_box_bottom; y <= bounding_box_top; y++) {
            const glm::vec3 p(x, y, 0);

            const float val_0_1 = edge_function(v0, v1, p);
            const float val_1_2 = edge_function(v1, v2, p);
            const float val_2_0 = edge_function(v2, v0, p);

            if (val_0_1 < 0 || val_1_2 < 0 || val_2_0 < 0) { continue; }

            const float weight0 = val_1_2 / val_0_1_2;
            const float weight1 = val_2_0 / val_0_1_2;
            // const float weight2 = val_0_1 / ABC;
            const float weight2 = 1 - weight0 - weight1;

            const float depth
                = weight0 * v0.z + weight1 * v1.z + weight2 * v2.z;


            const float current_depth = this->get_depth(x, y);
            if (current_depth <= depth && current_depth != -INFINITY) {
                continue;
            }

            const glm::vec2 uv(
                weight0 * uv0.x + weight1 * uv1.x + weight2 * uv2.x,
                weight0 * uv0.y + weight1 * uv1.y + weight2 * uv2.y);

            rb::Color color = uv_texture.get(uv);

            // clang-format off
            const glm::vec3 normal(
                weight0 * normal0.x + weight1 * normal1.x + weight2 * normal2.x,
                weight0 * normal0.y + weight1 * normal1.y + weight2 * normal2.y,
                weight0 * normal0.z + weight1 * normal1.z + weight2 * normal2.z
            );
            // clang-format on

            const float surface_light
                = 255 * std::abs(glm::dot(light_dir, normal));
            color.scale(surface_light);

            this->set_pixel(x, y, color);
            this->set_depth(x, y, depth);
        }
    }
}

// Currently leaves z in world space
auto model_to_screen(const rb::Window& window, const glm::vec3& model_pos)
    -> glm::vec3 {
    const float x = window.width * (model_pos.x + 1) / 2;
    const float y = window.height * (1 - (model_pos.y + 1) / 2);

    // x /= 4;
    // y /= 4;
    // x += 300;
    // y += 300;

    return glm::vec3(x, y, model_pos.z);
}

void rb::Window::render_mesh(const rb::Mesh& mesh) {
    // glm::vec3 light_dir(1, -1, 1);
    glm::vec3 light_dir(0, 0, 1);
    light_dir = glm::normalize(light_dir);
    for (unsigned int i = 0; i < mesh.vertex_indices.size() - 3; i += 3) {
        const unsigned int vertex_index0 = mesh.vertex_indices[i];
        const unsigned int vertex_index1 = mesh.vertex_indices[i + 1];
        const unsigned int vertex_index2 = mesh.vertex_indices[i + 2];

        const glm::vec3& world_pos0 = mesh.vertices[vertex_index0];
        const glm::vec3& world_pos1 = mesh.vertices[vertex_index1];
        const glm::vec3& world_pos2 = mesh.vertices[vertex_index2];

        const glm::vec3& vert0 = model_to_screen(*this, world_pos0);
        const glm::vec3& vert1 = model_to_screen(*this, world_pos1);
        const glm::vec3& vert2 = model_to_screen(*this, world_pos2);

        const unsigned int uv_index0 = mesh.uv_indices[i];
        const unsigned int uv_index1 = mesh.uv_indices[i + 1];
        const unsigned int uv_index2 = mesh.uv_indices[i + 2];

        const glm::vec2& uv_coord0 = mesh.uv_coordinates[uv_index0];
        const glm::vec2& uv_coord1 = mesh.uv_coordinates[uv_index1];
        const glm::vec2& uv_coord2 = mesh.uv_coordinates[uv_index2];

        const unsigned int normal_index0 = mesh.normal_indices[i];
        const unsigned int normal_index1 = mesh.normal_indices[i + 1];
        const unsigned int normal_index2 = mesh.normal_indices[i + 2];

        const glm::vec3& normal0 = mesh.normals[normal_index0];
        const glm::vec3& normal1 = mesh.normals[normal_index1];
        const glm::vec3& normal2 = mesh.normals[normal_index2];

        this->rasterize_triangle(vert0,
                                 vert1,
                                 vert2,
                                 uv_coord0,
                                 uv_coord1,
                                 uv_coord2,
                                 normal0,
                                 normal1,
                                 normal2,
                                 mesh.uv_texture,
                                 light_dir);
    };
}

void rb::Window::clear_depth_buffer() {
    for (auto& pixel_depth : this->depth_buffer) { pixel_depth = -INFINITY; }
};


void rb::Window::write_depth_buffer() {
    for (unsigned int n = 0; n < this->width * this->height; n++) {
        const float depth = this->depth_buffer[n];
        this->set_pixel(n, rb::Color(50 * (depth + 2)));
    }
}
