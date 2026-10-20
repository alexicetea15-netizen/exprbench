#include "evaluator.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> makePostfix(std::size_t operations) {
    std::vector<std::string> postfix;
    postfix.reserve(2 * operations + 1);

    postfix.push_back("1");

    for (std::size_t i = 0; i < operations; ++i) {
        postfix.push_back("1");
        postfix.push_back("+");
    }

    return postfix;
}

int main() {
    const std::vector<std::size_t> sizes = {
        10, 100, 1000, 10000
    };

    const int runs = 5;

    std::ofstream csv("results/benchmark.csv");

    if (!csv) {
        std::cerr << "Error: cannot open results/benchmark.csv\n";
        return 1;
    }

    csv << "operations,run,repetitions,total_cpu_seconds,"
           "avg_cpu_microseconds,checksum\n";

    csv << std::fixed << std::setprecision(9);

    for (std::size_t operations : sizes) {
        const std::vector<std::string> postfix =
            makePostfix(operations);

        const std::size_t repetitions =
            std::max<std::size_t>(10, 100000 / operations);

        const double expected =
            static_cast<double>(operations + 1);

        const double warmup = evaluatePostfix(postfix);

        if (warmup != expected) {
            std::cerr << "Error: incorrect benchmark input\n";
            return 1;
        }

        for (int run = 1; run <= runs; ++run) {
            double checksum = 0.0;

            std::clock_t start = std::clock();

            if (start == static_cast<std::clock_t>(-1)) {
                std::cerr << "Error: clock is unavailable\n";
                return 1;
            }

            for (std::size_t i = 0; i < repetitions; ++i) {
                checksum += evaluatePostfix(postfix);
            }

            std::clock_t finish = std::clock();

            if (finish == static_cast<std::clock_t>(-1)) {
                std::cerr << "Error: clock measurement failed\n";
                return 1;
            }

            const double elapsedSeconds =
                static_cast<double>(finish - start) /
                CLOCKS_PER_SEC;

            const double averageMicroseconds =
                elapsedSeconds * 1000000.0 / repetitions;

            csv << operations << ','
                << run << ','
                << repetitions << ','
                << elapsedSeconds << ','
                << averageMicroseconds << ','
                << checksum << '\n';

            std::cout
                << "Operations: " << operations
                << ", Run: " << run
                << ", Average CPU time: "
                << averageMicroseconds << " us\n";
        }
    }

    csv.close();

    if (!csv) {
        std::cerr << "Error: failed to write benchmark results\n";
        return 1;
    }

    std::cout << "\nResults saved to results/benchmark.csv\n";

    return 0;
}
