#include <benchmark/benchmark.h>

#include <LongNum.hpp>
#include <iostream>

static void BM_CalculatePi(benchmark::State& state) {
    for (auto _ : state) {
        LongMath::LongNum::setPrecision(100);
        LongMath::LongNum a(1);
        LongMath::LongNum b =
            LongMath::LongNum(1) / LongMath::LongNum(2).sqrt();
        LongMath::LongNum t = LongMath::LongNum(1) / LongMath::LongNum(4);
        LongMath::LongNum p = LongMath::LongNum(1);

        for (size_t i = 0; i < 20; ++i) {
            LongMath::LongNum a_next = (a + b) / 2;
            LongMath::LongNum b_next = (a * b).sqrt();
            LongMath::LongNum t_next = t - p * (a - a_next) * (a - a_next);
            LongMath::LongNum p_next = p * 2;

            a = a_next;
            b = b_next;
            t = t_next;
            p = p_next;
        }

        LongMath::LongNum pi = (a + b) * (a + b) / (t * 4);
        std::cout << "Calculated Pi: " << pi << std::endl;
        std::cout << "Correct    Pi: "
                  << "3."
                     "14159265358979323846264338327950288419716939937510582"
                     "09749445923078164062862089986280348253421170679821480"
                     "86513282306647093844609550582231725359408128481117450"
                     "28410270193852110555964462294895493038196"
                  << std::endl;
    }
}

BENCHMARK(BM_CalculatePi);

BENCHMARK_MAIN();