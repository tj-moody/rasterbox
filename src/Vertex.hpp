#pragma once

#include "Color.hpp"

#include <glm/glm.hpp>

namespace rb {
    class Vertex {
        public:
            Vertex(rb::Color color, glm::vec3 position);
            Vertex();

            rb::Color color;
            glm::vec3 position;
    };
}
