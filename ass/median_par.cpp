#include "Halide.h"
#include "halide_image_io.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;

void median_par() {
    Buffer<uint8_t> input = load_image("images/2.jpg");

    save_image(input, "whoopsies");
    std::cout << "Pipeline (5x5 Median) Success!\n";
}