#include <benchmark/benchmark.h>

#include <LongNum.hpp>
#include <iostream>

// Бенчмарк для сложения
static void BM_Addition(benchmark::State& state) {
    LongMath::LongNum a(state.range(0));
    LongMath::LongNum b(state.range(1));
    for (auto _ : state) {
        benchmark::DoNotOptimize(a + b);
    }
}
BENCHMARK(BM_Addition)->Args({1, 2});  // Пример входных данных

// Бенчмарк для умножения
static void BM_Multiplication(benchmark::State& state) {
    LongMath::LongNum a(state.range(0));
    LongMath::LongNum b(state.range(1));
    for (auto _ : state) {
        benchmark::DoNotOptimize(a * b);
    }
}
BENCHMARK(BM_Multiplication)->Args({1, 2});  // Пример входных данных

// Бенчмарк для извлечения квадратного корня
static void BM_SquareRoot(benchmark::State& state) {
    LongMath::LongNum a(state.range(0));
    for (auto _ : state) {
        benchmark::DoNotOptimize(a.sqrt());
    }
}
BENCHMARK(BM_SquareRoot)->Args({2});  // Пример входных данных

// Бенчмарк для деления
static void BM_Division(benchmark::State& state) {
    LongMath::LongNum a(state.range(0));
    LongMath::LongNum b(state.range(1));
    for (auto _ : state) {
        benchmark::DoNotOptimize(a / b);
    }
}
BENCHMARK(BM_Division)->Args({1, 2});  // Пример входных данных

// Точка входа для бенчмарков
BENCHMARK_MAIN();