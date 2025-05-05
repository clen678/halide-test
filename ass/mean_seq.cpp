#include "Halide.h"
#include "halide_image_io.h"  // For Halide image loading and saving
#include <iostream>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;

void mean_seq() {
    // Load image into Halide Buffer (this is similar to using stb_image)
    Buffer<uint8_t> input = load_image("images/2.jpg");

    // Get image dimensions
    int width = input.width();
    int height = input.height();
    int channels = input.channels();

    // Create a buffer to store the output image
    Buffer<uint8_t> output(width, height, channels);

    // Process the image with a normal C++ mean blur (5x5)
    auto t1 = std::chrono::high_resolution_clock::now();

    for (int y = 2; y < height - 2; ++y) { // Skip border pixels
        for (int x = 2; x < width - 2; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = 0;
                // 5x5 kernel (mean blur)
                for (int ky = -2; ky <= 2; ++ky) {
                    for (int kx = -2; kx <= 2; ++kx) {
                        sum += input(x + kx, y + ky, c);
                    }
                }
                // Set the average to the output buffer
                output(x, y, c) = sum / 25;
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration<float, std::milli>(t2 - t1).count() << " ms\n";

    // Save the output image using Halide's save_image function
    save_image(output, "mean_seq.png");
}