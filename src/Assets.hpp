#pragma once

#include "Mesh.hpp"

namespace rb {
    namespace Assets {
        std::optional<rb::Mesh> load_obj(const char* filename);
    }
}
