#include "Halide.h"
#include <iostream>
#include <cstdlib> // For EXIT_SUCCESS and EXIT_FAILURE

// Forward declarations of your Halide pipeline functions.
// These functions are defined in your other .cpp files (e.g., pipeline1.cpp, pipeline2.cpp).
void halide_pipeline2(); // Example: Your first Halide pipeline
//void halide_pipeline2(); // If you have a second Halide pipeline

int main() {
    // 1. Program Start
    std::cout << "Starting Halide image processing program..." << std::endl;

    // 2. Call your Halide pipelines.  Wrap each call in a try-catch block
    //    to handle potential Halide errors gracefully.
    try {
        std::cout << "Running pipeline1..." << std::endl;
        halide_pipeline2();
        std::cout << "Pipeline1 completed successfully." << std::endl;
        //halide_pipeline2(); // Call your second pipeline if you have one
    }
    catch (const Halide::Error& e) {
        std::cerr << "Halide error occurred: " << e.what() << std::endl;
        std::cerr << "Program terminated due to Halide error." << std::endl;
        return EXIT_FAILURE; // Use EXIT_FAILURE for error exit
    }
    catch (const std::exception& e) {
        std::cerr << "Standard exception occurred: " << e.what() << std::endl;
        std::cerr << "Program terminated due to standard exception." << std::endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        std::cerr << "Program terminated." << std::endl;
        return EXIT_FAILURE;
    }

    // 3. Program End
    std::cout << "All pipelines completed successfully.  Exiting program." << std::endl;
    return EXIT_SUCCESS; // Use EXIT_SUCCESS for successful exit
}