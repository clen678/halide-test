#include "Halide.h"
#include "halide_image_io.h"
#include <iostream>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;

void gaussian_par() {
    Buffer<uint8_t> input = load_image("images/2.jpg");

    Var x, y, c;
    Func blur_x, blur_y, g;

    // Define Gaussian weights (1, 4, 6, 4, 1)
    g(x) = select(
        x == 0, 1,
        x == 1, 4,
        x == 2, 6,
        x == 3, 4,
        1);

    // Horizontal and vertical reduction domains
    RDom rx(-2, 5), ry(-2, 5);

    // Define the expression for the 5x5 Gaussian blur
    Expr clamped_x = clamp(x + rx, 0, input.width() - 1);
    blur_x(x, y, c) = sum(cast<uint16_t>(input(clamped_x, y, c)) * g(rx + 2));

    Expr clamped_y = clamp(y + ry, 0, input.height() - 1);
    blur_y(x, y, c) = cast<uint8_t>(sum(blur_x(x, clamped_y, c) * g(ry + 2)) / 256);

    // Schedule
    Var xo, yo, xi, yi;
    blur_y
        .tile(x, y, xo, yo, xi, yi, 64, 64)
        .parallel(yo)
        .vectorize(xi, 4);

    Target target = get_target_from_environment();

    auto t1 = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = blur_y.realize({ input.width(), input.height(), input.channels() }, target);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Time (5x5 Gaussian separable): " << std::chrono::duration<float, std::milli>(t2 - t1).count() << " ms\n";

    save_image(output, "gaussian_par.png");
    std::cout << "Pipeline (5x5 separable Gaussian) Success!\n";
}
