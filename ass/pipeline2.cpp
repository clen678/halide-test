#include "Halide.h"
#include "halide_image_io.h"
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;

void halide_pipeline2() {
    Buffer<uint8_t> input = load_image("images/2.jpg");

    Var x, y, c;
    Func blur_x, blur_y;

    // Clamp x for 5-tap horizontal blur
    Expr xm2 = clamp(x - 2, 0, input.width() - 1);
    Expr xm1 = clamp(x - 1, 0, input.width() - 1);
    Expr xp1 = clamp(x + 1, 0, input.width() - 1);
    Expr xp2 = clamp(x + 2, 0, input.width() - 1);

    blur_x(x, y, c) = (
        cast<int>(input(xm2, y, c)) +
        cast<int>(input(xm1, y, c)) +
        cast<int>(input(x, y, c)) +
        cast<int>(input(xp1, y, c)) +
        cast<int>(input(xp2, y, c))
        ) / 5;

    // Clamp y for 5-tap vertical blur
    Expr ym2 = clamp(y - 2, 0, input.height() - 1);
    Expr ym1 = clamp(y - 1, 0, input.height() - 1);
    Expr yp1 = clamp(y + 1, 0, input.height() - 1);
    Expr yp2 = clamp(y + 2, 0, input.height() - 1);

    blur_y(x, y, c) = cast<uint8_t>((
        blur_x(x, ym2, c) +
        blur_x(x, ym1, c) +
        blur_x(x, y, c) +
        blur_x(x, yp1, c) +
        blur_x(x, yp2, c)
        ) / 5);

    Var xo, yo, xi, yi;

    blur_y
        .tile(x, y, xo, yo, xi, yi, 64, 64) // tile x and y by 64
        .parallel(yo)
        .vectorize(xi, 8); // or 16 depending on your CPU



    //Buffer<uint8_t> output = blur_y.realize({ input.width(), input.height(), input.channels() });

    // Time the realization
    auto t1 = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = blur_y.realize({ input.width(), input.height(), input.channels() });
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration<float, std::milli>(t2 - t1).count() << " ms\n";

    save_image(output, "blurred_5x5.png");
    printf("Pipeline (5x5 blur) Success!\n");
}
