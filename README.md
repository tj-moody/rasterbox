A toy software rasterizer, also for me to learn C++.

<img width="786" alt="Rendering of sample model" src="https://github.com/user-attachments/assets/9be552c6-34c9-492c-8302-fd5080c1ba9e">

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
