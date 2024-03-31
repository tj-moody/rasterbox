#pragma once

#include "Mesh.hpp"

namespace rb {
    namespace Assets {
        auto load_obj(const char* filename) -> std::optional<rb::Mesh>;
    }
}
