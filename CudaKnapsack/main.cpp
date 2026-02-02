#include <iostream>
#include <vector>
#include <memory>
#include "CudaKnapsackCalculator.h"

using namespace std;

int main()
{
    cout << "=== CUDA Knapsack Test ===\n\n";

    // Small test problem
    vector<int> weight = { 10, 4, 7, 4, 6, 8, 5, 2 };
    vector<double> gain = { 5.0, 7.0, 12.0, 4.0, 5.0, 7.0, 9.0, 2.0 };
    const int capacity = 10;

    cout << "Input:\n";
    cout << "  Items: " << weight.size() << "\n";
    cout << "  Capacity: " << capacity << "\n";
    cout << "  Weights: ";
    for (int w : weight) cout << w << " ";
    cout << "\n  Values: ";
    for (double v : gain) cout << v << " ";
    cout << "\n\n";

    // Create CUDA calculator and compute
    auto calc = make_unique<CudaKnapsackCalculator>(weight, gain, capacity);

    cout << "Computing on GPU...\n";
    calc->compute();
    cout << "Done.\n\n";

    // Print results
    const auto& mat = calc->profitMatrix();
    const int rows = calc->rows();
    const int cols = calc->cols();

    cout << "Profit matrix (" << rows << "x" << cols << "):\n";
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
            cout << mat[r * cols + c] << '\t';
        cout << '\n';
    }
    cout << '\n';

    cout << "Max value: " << calc->maxValue() << "\n";
    cout << "Selected items: ";
    for (size_t i = 0; i < calc->resultVector().size(); ++i)
        cout << (calc->resultVector()[i] ? "1" : "0") << " ";
    cout << "\n\n";

    cout << "Test completed successfully.\n";
    return 0;
}