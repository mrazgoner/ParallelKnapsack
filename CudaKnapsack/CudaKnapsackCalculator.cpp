#include "CudaKnapsackCalculator.h"
#include <cuda_runtime.h>
#include <cassert>
#include <iostream>

#define CUDA_CHECK(call) do { cudaError_t e = (call); if (e != cudaSuccess) { \
    std::cerr << "CUDA error " << cudaGetErrorString(e) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    std::terminate(); }} while(0)

// Forward declare the CUDA wrapper (defined in .cu file)
extern void launch_knapsack_kernel(double* d_profitMatrix, const int* d_weight, const double* d_profit,
    int cols, int item, int maxCapacity);

CudaKnapsackCalculator::~CudaKnapsackCalculator()
{
    if (d_profitMatrix) CUDA_CHECK(cudaFree(d_profitMatrix));
    if (d_weight) CUDA_CHECK(cudaFree(d_weight));
    if (d_profit) CUDA_CHECK(cudaFree(d_profit));
}

void CudaKnapsackCalculator::onPreCompute()
{
    size_t elems = static_cast<size_t>(m_rows) * static_cast<size_t>(m_cols);
    CUDA_CHECK(cudaMalloc(&d_profitMatrix, elems * sizeof(double)));
    CUDA_CHECK(cudaMalloc(&d_weight, static_cast<size_t>(m_n) * sizeof(int)));
    CUDA_CHECK(cudaMalloc(&d_profit, static_cast<size_t>(m_n) * sizeof(double)));

    CUDA_CHECK(cudaMemcpy(d_weight, m_weight.data(), static_cast<size_t>(m_n) * sizeof(int), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_profit, m_profit.data(), static_cast<size_t>(m_n) * sizeof(double), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemset(d_profitMatrix, 0, elems * sizeof(double)));
}

void CudaKnapsackCalculator::traverse(const std::function<void(int, int)>& /*worker*/)
{
    for (int item = 1; item <= m_n; ++item) {
        launch_knapsack_kernel(d_profitMatrix, d_weight, d_profit, m_cols, item, m_maxCapacity);
    }
}

void CudaKnapsackCalculator::onPostCompute()
{
    size_t elems = static_cast<size_t>(m_rows) * static_cast<size_t>(m_cols);
    CUDA_CHECK(cudaMemcpy(m_profitMatrix.data(), d_profitMatrix, elems * sizeof(double), cudaMemcpyDeviceToHost));

    CUDA_CHECK(cudaFree(d_profitMatrix)); d_profitMatrix = nullptr;
    CUDA_CHECK(cudaFree(d_weight)); d_weight = nullptr;
    CUDA_CHECK(cudaFree(d_profit)); d_profit = nullptr;
}