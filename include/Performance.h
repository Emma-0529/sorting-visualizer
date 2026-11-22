
#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <string>
#include <vector>

struct PerformanceResult {
    std::string algorithmName;
    double executionTimeMs = 0.0;
    unsigned long long comparisons = 0;
    unsigned long long swaps = 0;
    unsigned long long movements = 0;
    bool correctlySorted = false;
};

struct BenchmarkReport {
    std::vector<PerformanceResult> results;
    std::string fastestAlgorithm;
};

BenchmarkReport benchmarkAllAlgorithms(
    const std::vector<int>& originalData
);

#endif
