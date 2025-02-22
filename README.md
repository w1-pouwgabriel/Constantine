## Features

1. **Scene Setup**
    - Define and configure the 3D scene, including objects, materials, and lights.

2. **Ray Generation and Intersection Tests**
    - Implement ray generation from the camera and perform intersection tests with scene objects.

3. **Path Tracing Algorithm with Basic Illumination**
    - Develop the core path tracing algorithm to simulate light transport and compute basic illumination.

4. **Support for Multiple Bounces and Global Illumination**
    - Extend the path tracer to handle multiple light bounces and achieve realistic global illumination effects.

5. **Final Image Rendering with Optimizations**
    - Optimize the rendering process to produce the final image efficiently.

## Getting Started

### Prerequisites

- C++ Compiler
- CMake
- GLFW

### Installation

1. Build the project:
    ```sh
    mkdir build
    cd build
    cmake ..
    ```

### Usage

1. Run the path tracer:
    ```sh
    ./Constatine.exe
    ```

2. Configure the scene by editing the `scene.json` file.

### Contributing

Contributions are welcome! Please fork the repository and submit a pull request.

### License

This project is licensed under the MIT License.

## Contact

For any questions or feedback, please contact [email](mailto:gabriel.pouw@gmail.com).

## References

- [PBRT](http://www.pbrt.org/)
- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [BVH Building](https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/)

Thank you for using the Path Tracer project.
