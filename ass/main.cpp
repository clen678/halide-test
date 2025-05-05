#include "Halide.h"
#include <iostream>
#include <cstdlib> // For EXIT_SUCCESS and EXIT_FAILURE

// Forward declarations of your Halide pipeline functions.
// These functions are defined in your other .cpp files (e.g., pipeline1.cpp, pipeline2.cpp).
void halide_pipeline1(); // First Halide pipeline
void mean_par(); // Second Halide pipeline
void mean_seq();
void gaussian_seq();
void gaussian_par();

int main() {
    // 1. Program Start
    std::cout << "Starting Halide image processing program..." << std::endl;

    // Ask the user which pipeline they want to run
    int pipelineChoice;
    std::cout << "Enter 1,2,3,4,etc to run pipelines ";
    std::cin >> pipelineChoice;

    // 2. Call the chosen Halide pipeline based on user input
    try {
        if (pipelineChoice == 1) {
            std::cout << "Running Pipeline 1..." << std::endl;
            halide_pipeline1(); // Call the first pipeline
            std::cout << "Pipeline 1 completed successfully." << std::endl;
        }
        else if (pipelineChoice == 2) {
            std::cout << "Running Pipeline 2..." << std::endl;
            mean_par(); // Call the second pipeline
            std::cout << "Pipeline 2 completed successfully." << std::endl;
        }
        else if (pipelineChoice == 3) {
            std::cout << "Running Pipeline 3..." << std::endl;
            mean_seq(); // Call the second pipeline
            std::cout << "Pipeline 3 completed successfully." << std::endl;
        }
        else if (pipelineChoice == 4) {
            std::cout << "Running Pipeline 4..." << std::endl;
            gaussian_par(); // Call the second pipeline
            std::cout << "Pipeline 4 completed successfully." << std::endl;
        }
        else if (pipelineChoice == 5) {
            std::cout << "Running Pipeline 5..." << std::endl;
            gaussian_seq(); // Call the second pipeline
            std::cout << "Pipeline 5 completed successfully." << std::endl;
        }
        else {
            std::cerr << "Invalid choice! Please enter 1 or 2." << std::endl;
            return EXIT_FAILURE;
        }
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
    std::cout << "All pipelines completed successfully. Exiting program." << std::endl;
    return EXIT_SUCCESS; // Use EXIT_SUCCESS for successful exit
}
