#pragma once

#include "DynamicKnapsackCalculator.h"
#include "ParallelKnapsackCalculator.h"
#include "AntiDiagonalParallelKnapsackCalculator.h"
#include "../CudaKnapsack/CudaKnapsackCalculator.h"
#include <vector>

#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;

namespace ParallelKnapsackInterop
{
    /// <summary>
    /// Managed wrapper for DynamicKnapsackCalculator
    /// </summary>
    public ref class KnapsackCalculatorWrapper
    {
    public:
        /// <summary>
        /// Algorithm type enum
        /// </summary>
        enum class AlgorithmType
        {
            Sequential,
            Parallel,
            AntiDiagonalParallel,
            Cuda
        };

        /// <summary>
        /// Constructor
        /// </summary>
        KnapsackCalculatorWrapper(array<int>^ weights, array<double>^ profits, int maxCapacity, AlgorithmType algorithm);

        /// <summary>
        /// Destructor
        /// </summary>
        ~KnapsackCalculatorWrapper();

        /// <summary>
        /// Finalizer
        /// </summary>
        !KnapsackCalculatorWrapper();

        /// <summary>
        /// Compute the knapsack solution
        /// </summary>
        void Compute();

        /// <summary>
        /// Get number of rows in profit matrix
        /// </summary>
        int GetRows();

        /// <summary>
        /// Get number of columns in profit matrix
        /// </summary>
        int GetCols();

        /// <summary>
        /// Get number of items
        /// </summary>
        int GetItemCount();

        /// <summary>
        /// Get maximum capacity
        /// </summary>
        int GetMaxCapacity();

        /// <summary>
        /// Get maximum value achieved
        /// </summary>
        double GetMaxValue();

        /// <summary>
        /// Get profit matrix as managed array
        /// </summary>
        array<double>^ GetProfitMatrix();

        /// <summary>
        /// Get result vector (which items are included)
        /// </summary>
        array<bool>^ GetResultVector();

    private:
        DynamicKnapsackCalculator* m_pCalculator;
        std::vector<int>* m_pWeights;
        std::vector<double>* m_pProfits;
    };
}
