#include "Window.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iosfwd>

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
    this->depthBuffer.resize(width * height, -INFINITY);
    for (auto& pixel_depth : this->depthBuffer) { pixel_depth = -INFINITY; }
}

sf::Texture texture;
sf::Sprite sprite;
void rb::Window::draw() {
    this->window.setTitle(this->window_title
                          + " - FPS: " + std::to_string(this->getFrameRate()));

    // PERF: possibly attach `texture` and `sprite` as
    // private fields to only allocate once, maybe use a different data flow
    // than sf::Uint8* -> sf::Texture -> sf::Sprite -> window.draw()
    texture.create(this->width, this->height);
    texture.update(this->pixels);
    sprite = sf::Sprite(std::move(texture));
    this->window.draw(std::move(sprite));
    this->window.display();
}

void rb::Window::step() {
    this->frame_num++;
    this->handleInput();
    this->draw();
    this->clearDepthBuffer();
}

void rb::Window::handleInput() {
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) { this->window.close(); }
    }
}

void rb::Window::setPixel(unsigned int x,
                          unsigned int y,
                          const rb::Color& color) {
    if (x >= this->width || x < 0 || y >= this->height || y < 0) { return; }
    unsigned int index = (y * this->width * 4) + (x * 4);

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

void rb::Window::setDepth(unsigned int n, float depth) {
    this->depthBuffer[n] = std::move(depth);
}
void rb::Window::setDepth(unsigned int x, unsigned int y, float depth) {
    if (x >= this->width || x < 0 || y >= this->height || y < 0) { return; }
    this->depthBuffer[this->width * y + x] = std::move(depth);
}

auto rb::Window::getDepth(unsigned int x, unsigned int y) -> float {
    if (x >= this->width || x < 0 || y >= this->height || y < 0) {
        return -INFINITY;
    }
    return this->depthBuffer[this->width * y + x];
}

void rb::Window::setPixel(glm::vec2 p, const rb::Color& color) {
    if (p.x >= this->width || p.x < 0 || p.y >= this->height || p.y < 0) {
        return;
    }
    unsigned int index = (p.y * this->width * 4) + (p.x * 4);

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

void rb::Window::setPixel(unsigned int n, const rb::Color& color) {
    if (n < 0 || n > this->width * this->height) { return; }
    unsigned int index = n * 4;

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

bool rb::Window::isOpen() const { return this->window.isOpen(); }

sf::Clock elapsed;
auto rb::Window::getFrameRate() const -> int {
    double elapsed_time = elapsed.getElapsedTime().asSeconds();
    elapsed.restart();

    return (int) (1. / elapsed_time);
}

void rb::Window::fill(const rb::Color& color) {
    for (int n = 0; n < this->width * this->height; n++) {
        this->setPixel(n, color);
    }
}

void rb::Window::effectPass(void (*effect)(rb::Window& window)) {
    effect(*this);
}

auto absolute(int x) -> int {
    if (x < 0) { return -x; }
    return x;
}

void rb::Window::drawLine(const glm::vec2& p0,
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
            this->setPixel(y, x, color);
        } else {
            this->setPixel(x, y, color);
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


// t0, t1, t2 all have (x, y) in screen space, but z in world space.
void rb::Window::drawTriangle(const glm::vec3& v0,
                              const glm::vec3& v1,
                              const glm::vec3& v2,
                              const rb::Color& color) {
    if (v0.y == v1.y && v0.y == v2.y) { return; }

    const rb::Color edge_color(0, color.r, color.r);

    // cull back-facing triangles
    const float ABC = edge_function(v0, v1, v2);
    if (ABC < 1) { return; }

    const int bounding_box_left   = std::min(v0.x, std::min(v1.x, v2.x));
    const int bounding_box_right  = std::max(v0.x, std::max(v1.x, v2.x));
    const int bounding_box_bottom = std::min(v0.y, std::min(v1.y, v2.y));
    const int bounding_box_top    = std::max(v0.y, std::max(v1.y, v2.y));

    rb::Color _c = color;
    // _c = rb::Color(color.r * 893574 % 255);
    for (int x = bounding_box_left; x <= bounding_box_right; x++) {
        for (int y = bounding_box_bottom; y <= bounding_box_top; y++) {
            const glm::vec3 p(x, y, 0);

            const float ABP = edge_function(v0, v1, p);
            const float BCP = edge_function(v1, v2, p);
            const float CAP = edge_function(v2, v0, p);

            if (ABP < 0 || BCP < 0 || CAP < 0) { continue; }

            const float weight0 = ABP / ABC;
            const float weight1 = BCP / ABC;
            // const float weight2 = CAP / ABC;
            const float weight2 = 1 - weight0 - weight1;

            const float depth
                = weight0 * v0.z + weight1 * v1.z + weight2 * v2.z;
            // depth scaled to 0..=255

            if (this->getDepth(x, y) >= depth) { continue; }

            this->setPixel(x, y, _c);
            this->setDepth(x, y, 255 * (1 + depth) / 8);
        }
    }

    // this->line(v0, v1, edge_color);
    // this->line(v1, v2, edge_color);
    // this->line(v2, v0, edge_color);
}

// Currently leaves z in world space
auto model_to_screen(const rb::Window& window, const glm::vec3& model_pos)
    -> glm::vec3 {
    float x = window.width * (model_pos.x + 1) / 2;
    float y = window.height * (1 - (model_pos.y + 1) / 2);

    x /= 4;
    y /= 4;
    x += 300;
    y += 300;

    return glm::vec3(x, y, -model_pos.z);
}

void rb::Window::renderMesh(const rb::Mesh& mesh) {
    // glm::vec3 light_dir(1, -1, 1);
    glm::vec3 light_dir(0, 0, 1);
    light_dir = glm::normalize(light_dir);
    for (int i = 0; i < mesh.indices.size() - 3; i += 3) {
        unsigned int i1 = mesh.indices[i];
        unsigned int i2 = mesh.indices[i + 1];
        unsigned int i3 = mesh.indices[i + 2];

        glm::vec3 pos1 = mesh.vertices[i1].position;
        glm::vec3 pos2 = mesh.vertices[i2].position;
        glm::vec3 pos3 = mesh.vertices[i3].position;


        glm::vec3 normal = glm::normalize(glm::cross(pos3 - pos1, pos2 - pos1));

        float light = 255 * std::abs(glm::dot(normal, light_dir));

        glm::vec3 p1 = model_to_screen(*this, std::move(pos1));
        glm::vec3 p2 = model_to_screen(*this, std::move(pos2));
        glm::vec3 p3 = model_to_screen(*this, std::move(pos3));

        this->drawTriangle(p1, p2, p3, rb::Color(light));
    };
}

void rb::Window::clearDepthBuffer() {
    for (auto& pixel_depth : this->depthBuffer) { pixel_depth = -INFINITY; }
};

void rb::Window::drawDepthBuffer() {
    for (int n = 0; n < this->width * this->height; n++) {
        this->setPixel(n, rb::Color(this->depthBuffer[n]));
    }
}
