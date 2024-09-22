A toy software rasterizer, also for me to learn c++.

# Dependencies
- A C++ Compiler with C++17 or later
- SFML (any recent version should work)
- GLM

# Building
```sh
g++ -std=c++20 -O3 -g -Wall -Wpedantic src/Window.cpp src/Color.cpp src/Effects.cpp src/Mesh.cpp src/Texture.cpp -lsfml-graphics -lsfml-window -lsfml-system -Xclang -fopenmp -lomp -o bin/rasterbox src/main.cpp
```

# Resources
- https://www.scratchapixel.com/index.html
- https://www.learnopengl.com
- https://github.com/ssloy/tinyrenderer
- Real-Time Rendering by T. Akenine-Möller, E. Haines, N. Hoffman, A. Pesce, M. Iwanicki, S. Hillaire, 4th ed.
