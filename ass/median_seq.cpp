#include "Halide.h"
#include "halide_image_io.h"
#include <algorithm>
#include <vector>
#include <chrono>
#include <iostream>

using namespace Halide::Tools;
using namespace Halide;

void median_seq() {
    Buffer<uint8_t> input = load_image("images/2.jpg");
    Buffer<uint8_t> output(input.width(), input.height(), input.channels());

    auto t1 = std::chrono::high_resolution_clock::now();

    // Loop through the image (excluding 2-pixel border)
    for (int c = 0; c < input.channels(); c++) {
        for (int y = 2; y < input.height() - 2; y++) {
            for (int x = 2; x < input.width() - 2; x++) {

                std::vector<uint8_t> window;

                // Collect 5x5 neighborhood
                for (int dy = -2; dy <= 2; dy++) {
                    for (int dx = -2; dx <= 2; dx++) {
                        window.push_back(input(x + dx, y + dy, c));
                    }
                }

                std::sort(window.begin(), window.end());
                output(x, y, c) = window[12]; // Median of 25 values
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Time (C++ 5x5 Median): "
        << std::chrono::duration<float, std::milli>(t2 - t1).count()
        << " ms\n";

    save_image(output, "median_seq.png");
    std::cout << "C++ 5x5 Median Filter Completed!\n";
}