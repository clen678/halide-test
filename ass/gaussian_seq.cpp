#include "Halide.h"
#include "halide_image_io.h"
#include <iostream>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;

void gaussian_seq() {
    // Load image into Halide Buffer
    Buffer<uint8_t> input = load_image("images/2.jpg");

    int width = input.width();
    int height = input.height();
    int channels = input.channels();

    // Output image
    Buffer<uint8_t> output(width, height, channels);

    // 5x5 Gaussian kernel with sigma ~1.0 (normalized to sum to 273)
    int kernel[5][5] = {
        { 1,  4,  7,  4, 1 },
        { 4, 16, 26, 16, 4 },
        { 7, 26, 41, 26, 7 },
        { 4, 16, 26, 16, 4 },
        { 1,  4,  7,  4, 1 }
    };
    const int kernel_sum = 273;

    // Time the blur
    auto t1 = std::chrono::high_resolution_clock::now();

    for (int y = 2; y < height - 2; ++y) {
        for (int x = 2; x < width - 2; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = 0;
                for (int ky = -2; ky <= 2; ++ky) {
                    for (int kx = -2; kx <= 2; ++kx) {
                        sum += kernel[ky + 2][kx + 2] * input(x + kx, y + ky, c);
                    }
                }
                output(x, y, c) = sum / kernel_sum;
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration<float, std::milli>(t2 - t1).count() << " ms\n";

    // Save output
    save_image(output, "gaussian_seq.png");
    std::cout << "Gaussian blur (sequential) completed and saved as gaussian_seq.png\n";
}
