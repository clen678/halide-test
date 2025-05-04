#include "Halide.h"
#include "halide_image_io.h"
using namespace Halide::Tools;

void halide_pipeline1() {
    // Halide code here...
    Halide::Buffer<uint8_t> input = load_image("images/rgb.png");

    Halide::Func brighter;

    Halide::Var x, y, c;

    Halide::Expr value = input(x, y, c);

    value = Halide::cast<float>(value) * 1.5f;

    value = Halide::min(value, 255.0f);

    value = Halide::cast<uint8_t>(value);

    brighter(x, y, c) = value;

    Halide::Buffer<uint8_t> output = brighter.realize({ input.width(), input.height(), input.channels() });

    save_image(output, "brighter.png");

    printf("Pipeline1 Success!\n");
}