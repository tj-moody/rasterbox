cmake_minimum_required(VERSION 3.31)
project(rasterbox)

set(CMAKE_CXX_STANDARD 17)

include_directories(/opt/homebrew/include)
include_directories(src)

link_directories(/opt/homebrew/lib)

add_executable(rasterbox
        src/Color.cpp
        src/Color.hpp
        src/Effects.cpp
        src/Effects.hpp
        src/main.cpp
        src/Mesh.cpp
        src/Mesh.hpp
        src/Texture.cpp
        src/Texture.hpp
        src/Window.cpp
        src/Window.hpp)
