#include "Benchmark.hpp"
#include <iostream>
#include <iomanip>

BenchmarkResult Benchmark::runBinarySearchBenchmark(Catalog& catalog, int iterations) {
    catalog.sortByISBN();
    BenchmarkResult res;
    res.algorithmName = "Binary Search O(log N)";
    res.iterations = iterations;

    const auto& books = catalog.getBooks();
    if (books.empty()) return res;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        int targetIsbn = books[i % books.size()].isbn;
        SearchResult sr = catalog.searchByISBN(targetIsbn);
        res.totalOperationsChecked += sr.operationsCount;
    }
    auto end = std::chrono::high_resolution_clock::now();

    res.totalDurationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    res.averageDurationMicroseconds = static_cast<double>(res.totalDurationMicroseconds) / iterations;
    return res;
}

BenchmarkResult Benchmark::runLinearSearchBenchmark(Catalog& catalog, int iterations) {
    BenchmarkResult res;
    res.algorithmName = "Linear Search O(N)";
    res.iterations = iterations;

    const auto& books = catalog.getBooks();
    if (books.empty()) return res;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::string query = books[i % books.size()].title;
        SearchResult sr = catalog.searchByTitleOrAuthor(query);
        res.totalOperationsChecked += sr.operationsCount;
    }
    auto end = std::chrono::high_resolution_clock::now();

    res.totalDurationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    res.averageDurationMicroseconds = static_cast<double>(res.totalDurationMicroseconds) / iterations;
    return res;
}

void Benchmark::printComparisonReport(const BenchmarkResult& binRes, const BenchmarkResult& linRes) {
    std::cout << "\n===================================================================\n";
    std::cout << "          ALGORITHMIC PERFORMANCE BENCHMARK (1,000 RUNS)           \n";
    std::cout << "===================================================================\n";
    std::cout << std::left << std::setw(24) << "Algorithm"
              << std::setw(16) << "Total Time (us)"
              << std::setw(18) << "Avg Time (us/run)"
              << std::setw(16) << "Total Ops" << "\n";
    std::cout << "-------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(24) << binRes.algorithmName
              << std::setw(16) << binRes.totalDurationMicroseconds
              << std::setw(18) << binRes.averageDurationMicroseconds
              << std::setw(16) << binRes.totalOperationsChecked << "\n";
    std::cout << std::left << std::setw(24) << linRes.algorithmName
              << std::setw(16) << linRes.totalDurationMicroseconds
              << std::setw(18) << linRes.averageDurationMicroseconds
              << std::setw(16) << linRes.totalOperationsChecked << "\n";
    std::cout << "===================================================================\n\n";
}
