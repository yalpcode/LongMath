#include <benchmark/benchmark.h>
#include <gflags/gflags.h>
#include <gtest/gtest.h>

#include <LongNum.hpp>
#include <iostream>

DEFINE_int32(precision, 100, "Number of digits of Pi to calculate and verify");

LongMath::LongNum CalculatePi(int precision) {
    LongMath::LongNum::setPrecision(precision + 1);

    LongMath::LongNum n0(1);
    LongMath::LongNum n(16);

    LongMath::LongNum a0(4);
    LongMath::LongNum b0(2);
    LongMath::LongNum c0(1);
    LongMath::LongNum d0(1);

    LongMath::LongNum a(1);
    LongMath::LongNum b(4);
    LongMath::LongNum c(5);
    LongMath::LongNum d(6);

    LongMath::LongNum pi;

    for (int k = 0; k <= precision; ++k) {
        pi = pi + n0 * (a0 / a - b0 / b - c0 / c - d0 / d);
        n0 = n0 / n;
        a += 8;
        b += 8;
        c += 8;
        d += 8;
    }

    return pi;
}

static void BM_CalculatePi(benchmark::State& state) {
    int precision = FLAGS_precision;

    for (auto _ : state) {
        LongMath::LongNum::setPrecision(precision + 1);

        LongMath::LongNum n0(1);
        LongMath::LongNum n(16);

        LongMath::LongNum a0(4);
        LongMath::LongNum b0(2);
        LongMath::LongNum c0(1);
        LongMath::LongNum d0(1);

        LongMath::LongNum a(1);
        LongMath::LongNum b(4);
        LongMath::LongNum c(5);
        LongMath::LongNum d(6);

        LongMath::LongNum pi;

        for (int k = 0; k <= precision; ++k) {
            pi = pi + n0 * (a0 / a - b0 / b - c0 / c - d0 / d);
            n0 = n0 / n;
            a += 8;
            b += 8;
            c += 8;
            d += 8;
        }

        std::string calculatedPi = pi.toString().substr(0, precision + 2);

        std::string correctPi =
            "3."
            "14159265358979323846264338327950288419716939937510582"
            "097494459230781640628620899862803482534211706798";
        correctPi = correctPi.substr(0, precision + 2);

        std::cout << "Calculated Pi: " << calculatedPi << ' '
                  << calculatedPi.length() << std::endl;

        EXPECT_EQ(calculatedPi, correctPi) << "Calculated Pi failed.";
    }
}

// BENCHMARK(BM_CalculatePi)->Iterations(1);

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    int precision = FLAGS_precision;

    auto start = std::chrono::high_resolution_clock::now();

    auto pi = CalculatePi(precision);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Время выполнения функции: " << duration.count() << " секунд"
              << std::endl;

    std::string calculatedPi = pi.toString().substr(0, precision + 2);

    std::string correctPi =
        "3."
        "14159265358979323846264338327950288419716939937510582"
        "097494459230781640628620899862803482534211706798";
    correctPi = correctPi.substr(0, precision + 2);

    std::cout << "Calculated Pi: " << calculatedPi << ' '
              << calculatedPi.length() << std::endl;

    EXPECT_EQ(calculatedPi, correctPi) << "Calculated Pi failed.";

    // ::benchmark::Initialize(&argc, argv);
    // if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
    //     return 1;
    // }
    // ::benchmark::RunSpecifiedBenchmarks();

    return 0;
}