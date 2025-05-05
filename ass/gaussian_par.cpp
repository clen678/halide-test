#include "Halide.h"
#include "halide_image_io.h"
#include <iostream>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;

void gaussian_par() {
    Buffer<uint8_t> input = load_image("images/2.jpg");
    int width = input.width();
    int height = input.height();
    int channels = input.channels();

    Var x, y, c;
    Func input_func, blur_x, blur_y;

    // Define clamped input
    input_func(x, y, c) = cast<int>(BoundaryConditions::repeat_edge(input)(x, y, c));

    // 1D Gaussian kernel: [1 4 6 4 1]
    // Normalize factor: 16
    blur_x(x, y, c) = (
        input_func(x - 2, y, c) +
        input_func(x - 1, y, c) * 4 +
        input_func(x, y, c) * 6 +
        input_func(x + 1, y, c) * 4 +
        input_func(x + 2, y, c)
        );

    blur_y(x, y, c) = cast<uint8_t>((
        blur_x(x, y - 2, c) +
        blur_x(x, y - 1, c) * 4 +
        blur_x(x, y, c) * 6 +
        blur_x(x, y + 1, c) * 4 +
        blur_x(x, y + 2, c)
        ) / 256); // 16 * 16 normalization

    // Schedule: parallelize y, vectorize x
    Var yo, yi;
    blur_y
        .split(y, yo, yi, 32)
        .parallel(yo);
        //.vectorize(x, 8);

    // Timing
    Target target = get_target_from_environment();
    auto t1 = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = blur_y.realize({ width, height, channels }, target);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Halide Gaussian blur (parallel) time: "
        << std::chrono::duration<float, std::milli>(t2 - t1).count()
        << " ms\n";

    save_image(output, "gaussian_halide_parallel.png");
    std::cout << "Saved output as gaussian_halide_parallel.png\n";
}
