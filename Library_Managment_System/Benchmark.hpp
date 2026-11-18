#pragma once

#include "Catalog.hpp"
#include <chrono>
#include <vector>
#include <string>

// ============================================================================
// Benchmark: High-Precision Performance Stopwatch for Search Algorithms
// Measures exact CPU microsecond timings comparing O(log N) vs O(N)
// ============================================================================

struct BenchmarkResult {
    std::string algorithmName;
    int iterations{ 0 };
    long long totalDurationMicroseconds{ 0 };
    double averageDurationMicroseconds{ 0.0 };
    long long totalOperationsChecked{ 0 };
};

class Benchmark {
public:
    static BenchmarkResult runBinarySearchBenchmark(Catalog& catalog, int iterations = 1000);
    static BenchmarkResult runLinearSearchBenchmark(Catalog& catalog, int iterations = 1000);
    static void printComparisonReport(const BenchmarkResult& binRes, const BenchmarkResult& linRes);
};
