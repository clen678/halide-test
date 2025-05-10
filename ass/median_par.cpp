#include "Halide.h"
#include "halide_image_io.h"
#include <chrono>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace Halide;
using namespace Halide::Tools;

void median_par() {
    Buffer<uint8_t> input = load_image("images/3.png");

    Var x, y, c;
    Func median_filter;

    // Collect all 25 pixels from the 5x5 neighborhood
    std::vector<Expr> neighborhood;
    for (int dy = -2; dy <= 2; ++dy) {
        for (int dx = -2; dx <= 2; ++dx) {
            Expr clamped_x = clamp(x + dx, 0, input.width() - 1);
            Expr clamped_y = clamp(y + dy, 0, input.height() - 1);
            neighborhood.push_back(cast<uint16_t>(input(clamped_x, clamped_y, c)));
        }
    }

    // Create a function to compute the median by sorting
    Func sorted;
    sorted(x, y, c) = neighborhood[0];
    for (size_t i = 1; i < neighborhood.size(); ++i) {
        sorted(x, y, c) = max(min(sorted(x, y, c), neighborhood[i]), min(sorted(x, y, c), neighborhood[i]));
    }

    // To properly compute median, use tuple sorting
    Func med;
    med(x, y, c) = undef<uint8_t>();
    med(x, y, c) = cast<uint8_t>(select(neighborhood.size() == 25,
        min(
            max(
                min(neighborhood[12], max(neighborhood[0], neighborhood[24])),
                max(neighborhood[6], neighborhood[18])
            ),
            max(neighborhood[3], neighborhood[21])
        ), 0));

    // Instead of tuple sorting above, let's do a simpler median: use a small sort routine manually
    Func median;
    median(x, y, c) = undef<uint8_t>();

    // Use a helper function to sort a vector of Exprs (done manually inside loop)
    Func compute_median;
    compute_median(x, y, c) = cast<uint8_t>(neighborhood[12]); // Default

    // Inline manual sort (bubble sort for simplicity, 25 elements)
    {
        std::vector<Expr> values = neighborhood;
        for (int i = 0; i < 25; ++i) {
            for (int j = i + 1; j < 25; ++j) {
                values[i] = select(values[i] > values[j], values[j], values[i]);
                values[j] = select(values[i] > values[j], values[i], values[j]);
            }
        }
        compute_median(x, y, c) = cast<uint8_t>(values[12]);
    }

    // Schedule
    Var xo, yo, xi, yi;
    compute_median
        .tile(x, y, xo, yo, xi, yi, 64, 64)
        .parallel(yo)
        .vectorize(xi, 4);

    Target target = get_target_from_environment();

    auto t1 = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = compute_median.realize({ input.width(), input.height(), input.channels() }, target);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Time (5x5 Median filter): " << std::chrono::duration<float, std::milli>(t2 - t1).count() << " ms\n";

    save_image(output, "median_par.png");
    std::cout << "Pipeline (5x5 Median) Success!\n";
}
