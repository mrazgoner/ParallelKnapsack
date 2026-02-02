#pragma once

#include "DynamicKnapsackCalculator.h"
#include <functional>

class CudaKnapsackCalculator : public DynamicKnapsackCalculator
{
public:
    using DynamicKnapsackCalculator::DynamicKnapsackCalculator;
    ~CudaKnapsackCalculator() override;

protected:
    // override traversal to launch CUDA kernels per item
    void traverse(const std::function<void(int,int)>& worker) override;

    // lifecycle hooks
    void onPreCompute() override;
    void onPostCompute() override;

private:
    // device buffers
    double* d_profitMatrix = nullptr;
    int*    d_weight = nullptr;
    double* d_profit = nullptr;
};