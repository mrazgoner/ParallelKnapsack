// Benchmark.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <random>
#include <iomanip>
#include "DynamicKnapsackCalculator.h"
#include "ParallelKnapsackCalculator.h"

using namespace std;
using clk = chrono::high_resolution_clock;
using ms = chrono::duration<double, milli>;

static void print_stats(const vector<double>& samples_ms, const string& name)
{
    if (samples_ms.empty()) return;
    vector<double> s = samples_ms;
    sort(s.begin(), s.end());
    double sum = accumulate(s.begin(), s.end(), 0.0);
    double mean = sum / s.size();
    double median = s[s.size() / 2];
    double minimum = s.front();
    double maximum = s.back();

    cout << "Benchmark: " << name << '\n';
    cout << "  runs:   " << s.size() << '\n';
    cout << "  min:    " << minimum << " ms\n";
    cout << "  median: " << median << " ms\n";
    cout << "  mean:   " << mean << " ms\n";
    cout << "  max:    " << maximum << " ms\n\n";
}

static void write_output_tsv(const DynamicKnapsackCalculator& calc,
                             const std::vector<int>& weight,
                             const std::vector<double>& profit,
                             const std::string& filename = "output.tsv")
{
    const size_t rows = static_cast<size_t>(calc.rows());
    const size_t cols = static_cast<size_t>(calc.cols());

    // Defensive: avoid producing huge TSVs for large benchmarks.
    if (rows * cols > 10000) {
        cerr << "Skipping TSV write for large matrix (" << rows << "x" << cols << ") -> " << filename << '\n';
        return;
    }

    std::ofstream out(filename);
    if (!out) {
        cerr << "write_output_tsv: failed to open " << filename << '\n';
        return;
    }

    int n = static_cast<int>(weight.size());
    out << "items:";
    for (int i = 0; i < n; ++i) out << '\t' << "#" << i;
    out << '\n';

    out << "weights:";
    for (int i = 0; i < n; ++i) out << '\t' << weight[i];
    out << '\n';

    out << "value:";
    for (int i = 0; i < n; ++i) out << '\t' << profit[i];
    out << "\n\n";

    out << "Calculation Matrix:\n";
    const auto& mat = calc.profitMatrix();
    for (size_t r = 0; r < rows; ++r)
    {
        out << '\t';
        for (size_t c = 0; c < cols; ++c)
        {
            out << mat[r * cols + c] << '\t';
        }
        out << '\n';
    }
    out << '\n';

    out << "optimal set: " << calc.maxValue() << '\n';
    out << '\n' << "item:";
    const auto& res = calc.resultVector();
    for (int i = 0; i < calc.itemCount(); ++i)
        out << '\t' << (res[i] ? "Yes" : "No");
    out << '\n';

    out.close();
}

template <typename Factory>
vector<double> benchmark_factory(Factory makeCalculator, int iterations, int warmups = 1)
{
    vector<double> samples;
    samples.reserve(iterations);

    // warmups
    for (int w = 0; w < warmups; ++w) {
        auto c = makeCalculator();
        c->compute();
    }

    for (int i = 0; i < iterations; ++i)
    {
        auto c = makeCalculator(); // construct fresh to avoid cached effects
        auto t0 = clk::now();
        c->compute();
        auto t1 = clk::now();
        ms d = t1 - t0;
        samples.push_back(d.count());
    }
    return samples;
}

// Visible, human-sized example: compute both sequential and parallel,
// print results to console and write TSVs for inspection.
static void runVisibleExample()
{
    cout << "\n=== Visible example (serial vs parallel) ===\n";

    vector<int> weight = { 10, 4, 7, 4, 6, 8, 5, 2 };
    vector<double> gain = { 5.0, 7.0, 12.0, 4.0, 5.0, 7.0, 9.0, 2.0 };
    const int capacity = 10;

    DynamicKnapsackCalculator seqCalc(weight, gain, capacity);
    ParallelKnapsackCalculator parCalc(weight, gain, capacity);

    seqCalc.compute();
    parCalc.compute();

    // print a compact matrix preview to console (rows x cols)
    const auto& seqMat = seqCalc.profitMatrix();
    const auto& parMat = parCalc.profitMatrix();
    const int rows = seqCalc.rows();
    const int cols = seqCalc.cols();

    cout << "Sequential matrix (" << rows << "x" << cols << "):\n";
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
            cout << seqMat[r * cols + c] << '\t';
        cout << '\n';
    }
    cout << '\n';

    cout << "Parallel matrix (" << rows << "x" << cols << "):\n";
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
            cout << parMat[r * cols + c] << '\t';
        cout << '\n';
    }
    cout << '\n';

    cout << "Sequential maxValue: " << seqCalc.maxValue() << '\n';
    cout << "Parallel   maxValue: " << parCalc.maxValue() << '\n';

    cout << "Sequential result vector: ";
    for (auto b : seqCalc.resultVector()) cout << (b ? "1" : "0") << ' ';
    cout << '\n';

    cout << "Parallel result vector:   ";
    for (auto b : parCalc.resultVector()) cout << (b ? "1" : "0") << ' ';
    cout << '\n';

    // write TSV outputs (safe: function skips large matrices)
    write_output_tsv(seqCalc, weight, gain, "output_sequential.tsv");
    write_output_tsv(parCalc, weight, gain, "output_parallel.tsv");
    cout << "Wrote output_sequential.tsv and output_parallel.tsv (if small enough)\n\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
    // Keep a small visible example for inspection
    runVisibleExample();

    // Larger problem where parallelism has a chance to pay off:
    const int nItems = 5000;      // requested larger input
    const int capacity = 5000;    // requested larger input
    const int iterations = 3;     // keep low for large runs
    const int warmups = 1;

    cout << "Problem: items=" << nItems << " capacity=" << capacity
         << " iterations=" << iterations << " warmups=" << warmups << "\n";

    // Random generator seeded by high-resolution clock
    std::mt19937_64 rng(static_cast<uint64_t>(chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> weightDist(1, std::max(1, capacity / 2));
    std::uniform_real_distribution<double> profitDist(1.0, 100.0);

    vector<int> weight;
    vector<double> profit;
    weight.reserve(nItems);
    profit.reserve(nItems);

    for (int i = 0; i < nItems; ++i) {
        int w = weightDist(rng);
        if (w <= 0) w = 1;
        weight.push_back(w);
        profit.push_back(profitDist(rng));
    }

    // Factories capture the generated random data
    auto seqFactory = [&]() -> unique_ptr<DynamicKnapsackCalculator> {
        return make_unique<DynamicKnapsackCalculator>(weight, profit, capacity);
    };

    auto parFactory = [&]() -> unique_ptr<DynamicKnapsackCalculator> {
        return make_unique<ParallelKnapsackCalculator>(weight, profit, capacity);
    };

    // Run one verification (no TSV for large problem)
    {
        auto seqCalc = seqFactory();
        auto parCalc = parFactory();

        cout << "Running single verification run...\n";
        seqCalc->compute();
        parCalc->compute();

        double sVal = seqCalc->maxValue();
        double pVal = parCalc->maxValue();

        cout << "  Sequential maxValue: " << sVal << "\n";
        cout << "  Parallel   maxValue: " << pVal << "\n";

        const double eps = 1e-6;
        if (fabs(sVal - pVal) > eps) {
            cerr << "ERROR: results differ (seq=" << sVal << ", par=" << pVal << ")\n";
            return 1;
        } else {
            cout << "Verification passed: results match within eps=" << eps << "\n";
        }

        // Skip TSV for large matrices (write_output_tsv enforces)
        write_output_tsv(*seqCalc, weight, profit, "output_sequential_random.tsv");
        write_output_tsv(*parCalc, weight, profit, "output_parallel_random.tsv");
    }

    cout << "Running benchmark (" << iterations << " runs, " << warmups << " warmups)...\n\n";

    auto seqSamples = benchmark_factory(seqFactory, iterations, warmups);
    auto parSamples = benchmark_factory(parFactory, iterations, warmups);

    print_stats(seqSamples, "Sequential (DynamicKnapsackCalculator)");
    print_stats(parSamples, "Parallel (ParallelKnapsackCalculator - anti-diagonal)");

    double seq_mean = accumulate(seqSamples.begin(), seqSamples.end(), 0.0) / seqSamples.size();
    double par_mean = accumulate(parSamples.begin(), parSamples.end(), 0.0) / parSamples.size();
    cout << "Summary: mean speedup = " << seq_mean / par_mean << "x (seq / par)\n";

    return 0;
}

