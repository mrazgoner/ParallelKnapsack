#include <cuda_runtime.h>
#include "device_launch_parameters.h"
#include <iostream>

#define CUDA_CHECK(call) do { cudaError_t e = (call); if (e != cudaSuccess) { \
    std::cerr << "CUDA error " << cudaGetErrorString(e) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    std::terminate(); }} while(0)

// Kernel: compute one row (item) of DP. Each thread processes one capacity index.
__global__ void knapsack_item_kernel(double* profitMatrix, const int* weight, const double* profit,
    int cols, int item, int maxCapacity)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int capacity = tid + 1; // capacities 1..maxCapacity mapped to threads 0..maxCapacity-1
    if (capacity > maxCapacity) return;

    int stride = cols;
    int w = weight[item - 1];

    double prev = profitMatrix[(item - 1) * stride + capacity];
    if (w > capacity) {
        profitMatrix[item * stride + capacity] = prev;
    }
    else {
        double add = profitMatrix[(item - 1) * stride + (capacity - w)] + profit[item - 1];
        profitMatrix[item * stride + capacity] = (prev > add) ? prev : add;
    }
}

// Host wrapper function that launches the kernel
void launch_knapsack_kernel(double* d_profitMatrix, const int* d_weight, const double* d_profit,
    int cols, int item, int maxCapacity)
{
    const int threadsPerBlock = 256;
    int blocks = (maxCapacity + threadsPerBlock - 1) / threadsPerBlock;
    knapsack_item_kernel << <blocks, threadsPerBlock >> > (d_profitMatrix, d_weight, d_profit, cols, item, maxCapacity);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());
}