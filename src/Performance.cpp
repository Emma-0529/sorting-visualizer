
#include "Performance.h"

#include <algorithm>
#include <chrono>
#include <utility>
#include <vector>

// Bubble Sort
static void benchmarkBubble(
    std::vector<int>& data,
    PerformanceResult& result
) {
    for (std::size_t end = data.size(); end > 1; --end) {
        bool swapped = false;

        for (std::size_t j = 1; j < end; ++j) {
            ++result.comparisons;

            if (data[j - 1] > data[j]) {
                std::swap(data[j - 1], data[j]);
                ++result.swaps;
                result.movements += 2;
                swapped = true;
            }
        }

        if (!swapped) break;
    }
}

// Merge Sort
static void mergeRange(
    std::vector<int>& data,
    std::vector<int>& temp,
    std::size_t left,
    std::size_t mid,
    std::size_t right,
    PerformanceResult& result
) {
    std::size_t i = left;
    std::size_t j = mid;
    std::size_t k = left;

    while (i < mid && j < right) {
        ++result.comparisons;

        if (data[i] <= data[j])
            temp[k++] = data[i++];
        else
            temp[k++] = data[j++];

        ++result.movements;
    }

    while (i < mid) {
        temp[k++] = data[i++];
        ++result.movements;
    }

    while (j < right) {
        temp[k++] = data[j++];
        ++result.movements;
    }

    for (std::size_t p = left; p < right; ++p) {
        data[p] = temp[p];
        ++result.movements;
    }
}

static void benchmarkMergeRecursive(
    std::vector<int>& data,
    std::vector<int>& temp,
    std::size_t left,
    std::size_t right,
    PerformanceResult& result
) {
    if (right - left <= 1) return;

    std::size_t mid = left + (right - left) / 2;

    benchmarkMergeRecursive(data, temp, left, mid, result);
    benchmarkMergeRecursive(data, temp, mid, right, result);

    mergeRange(data, temp, left, mid, right, result);
}

static void benchmarkMerge(
    std::vector<int>& data,
    PerformanceResult& result
) {
    std::vector<int> temp(data.size());

    benchmarkMergeRecursive(
        data, temp, 0, data.size(), result
    );
}

// Quick Sort (Lomuto partition)
static std::size_t benchmarkPartition(
    std::vector<int>& data,
    std::size_t low,
    std::size_t high,
    PerformanceResult& result
) {
    int pivot = data[high];
    std::size_t i = low;

    for (std::size_t j = low; j < high; ++j) {
        ++result.comparisons;

        if (data[j] <= pivot) {
            if (i != j) {
                std::swap(data[i], data[j]);
                ++result.swaps;
                result.movements += 2;
            }
            ++i;
        }
    }

    if (i != high) {
        std::swap(data[i], data[high]);
        ++result.swaps;
        result.movements += 2;
    }

    return i;
}

static void benchmarkQuickRecursive(
    std::vector<int>& data,
    std::size_t low,
    std::size_t high,
    PerformanceResult& result
) {
    if (low >= high) return;

    std::size_t pivot =
        benchmarkPartition(data, low, high, result);

    if (pivot > low)
        benchmarkQuickRecursive(data, low, pivot - 1, result);

    if (pivot < high)
        benchmarkQuickRecursive(data, pivot + 1, high, result);
}

static void benchmarkQuick(
    std::vector<int>& data,
    PerformanceResult& result
) {
    if (data.size() > 1) {
        benchmarkQuickRecursive(
            data, 0, data.size() - 1, result
        );
    }
}

// Benchmark all algorithms
BenchmarkReport benchmarkAllAlgorithms(
    const std::vector<int>& originalData
) {
    BenchmarkReport report;

    auto runBenchmark = [&](const std::string& name, auto sortFunction) {
        std::vector<int> data = originalData;

        PerformanceResult result;
        result.algorithmName = name;

        auto start = std::chrono::steady_clock::now();

        sortFunction(data, result);

        auto end = std::chrono::steady_clock::now();

        result.executionTimeMs =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        result.correctlySorted =
            std::is_sorted(data.begin(), data.end());

        report.results.push_back(result);
    };

    runBenchmark("Bubble Sort", benchmarkBubble);
    runBenchmark("Merge Sort", benchmarkMerge);
    runBenchmark("Quick Sort", benchmarkQuick);

    double fastestTime = 0.0;

    for (const auto& result : report.results) {
        if (result.correctlySorted &&
            (report.fastestAlgorithm.empty() ||
             result.executionTimeMs < fastestTime)) {

            fastestTime = result.executionTimeMs;
            report.fastestAlgorithm = result.algorithmName;
        }
    }

    if (report.fastestAlgorithm.empty())
        report.fastestAlgorithm = "No valid result";

    return report;
}
