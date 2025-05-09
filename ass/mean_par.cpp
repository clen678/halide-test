#include "Halide.h"
#include "halide_image_io.h"
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;

void mean_par() {
    Buffer<uint8_t> input = load_image("images/2.jpg");

    Var x, y, c, k_x, k_y;
    Func blur_x, blur_y;

    RDom rx(-2, 5), ry(-2, 5); // Define reduction domains for the 5 taps

	// Define the expression for the 5x5 mean blur
    Expr clamped_x = clamp(x + rx, 0, input.width() - 1);
    blur_x(x, y, c) = sum(cast<uint16_t>(input(clamped_x, y, c))) / 5;
    Expr clamped_y = clamp(y + ry, 0, input.height() - 1);
    blur_y(x, y, c) = cast<uint8_t>(sum(blur_x(x, clamped_y, c))) / 5;

    Var xo, yo, xi, yi;
    blur_y
        .tile(x, y, xo, yo, xi, yi, 64, 64)
        .parallel(yo)// just xo works too
        .parallel(xo)
        .vectorize(xi, 4);


	Target target(get_target_from_environment());

    auto t1 = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = blur_y.realize({ input.width(), input.height(), input.channels() }, target);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Time (with sum): " << std::chrono::duration<float, std::milli>(t2 - t1).count() << " ms\n";

    save_image(output, "blurred_5x5_sum.png");
    printf("Pipeline (5x5 blur with sum) Success!\n");
}
