A toy software rasterizer, also for me to learn c++.

# Dependencies
- A C++ Compiler with C++17 or later
- SFML (any recent version should work)
- GLM

# Building
```sh
g++ -std=c++17 -O3 src/Window.cpp src/Color.cpp src/Effects.cpp src/Vertex.cpp src/Mesh.cpp src/Assets.cpp -lsfml-graphics -lsfml-window -lsfml-system -o bin/rasterbox src/main.cpp 
```
