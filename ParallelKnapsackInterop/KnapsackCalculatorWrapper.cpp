#include "KnapsackCalculatorWrapper.h"

namespace ParallelKnapsackInterop
{
    KnapsackCalculatorWrapper::KnapsackCalculatorWrapper(
        array<int>^ weights,
        array<double>^ profits,
        int maxCapacity,
        AlgorithmType algorithm)
        : m_pCalculator(nullptr)
        , m_pWeights(nullptr)
        , m_pProfits(nullptr)
    {
        m_pWeights = new std::vector<int>();
        m_pProfits = new std::vector<double>();

        for (int i = 0; i < weights->Length; i++)
        {
            int w = weights[i];
            m_pWeights->push_back(w);
        }

        for (int i = 0; i < profits->Length; i++)
        {
            double p = profits[i];
            m_pProfits->push_back(p);
        }

        switch (algorithm)
        {
        case AlgorithmType::Sequential:
            m_pCalculator = new DynamicKnapsackCalculator(*m_pWeights, *m_pProfits, maxCapacity);
            break;
        case AlgorithmType::Parallel:
            m_pCalculator = new ParallelKnapsackCalculator(*m_pWeights, *m_pProfits, maxCapacity);
            break;
        case AlgorithmType::AntiDiagonalParallel:
            m_pCalculator = new AntiDiagonalParallelKnapsackCalculator(*m_pWeights, *m_pProfits, maxCapacity);
            break;
        case AlgorithmType::Cuda:
            m_pCalculator = new CudaKnapsackCalculator(*m_pWeights, *m_pProfits, maxCapacity);
            break;
        default:
            throw gcnew ArgumentException("Unknown algorithm type");
        }
    }

    KnapsackCalculatorWrapper::~KnapsackCalculatorWrapper()
    {
        this->!KnapsackCalculatorWrapper();
    }

    KnapsackCalculatorWrapper::!KnapsackCalculatorWrapper()
    {
        if (m_pCalculator != nullptr)
        {
            delete m_pCalculator;
            m_pCalculator = nullptr;
        }

        if (m_pWeights != nullptr)
        {
            delete m_pWeights;
            m_pWeights = nullptr;
        }

        if (m_pProfits != nullptr)
        {
            delete m_pProfits;
            m_pProfits = nullptr;
        }
    }

    void KnapsackCalculatorWrapper::Compute()
    {
        if (m_pCalculator != nullptr)
        {
            m_pCalculator->compute();
        }
    }

    int KnapsackCalculatorWrapper::GetRows()
    {
        return m_pCalculator != nullptr ? m_pCalculator->rows() : 0;
    }

    int KnapsackCalculatorWrapper::GetCols()
    {
        return m_pCalculator != nullptr ? m_pCalculator->cols() : 0;
    }

    int KnapsackCalculatorWrapper::GetItemCount()
    {
        return m_pCalculator != nullptr ? m_pCalculator->itemCount() : 0;
    }

    int KnapsackCalculatorWrapper::GetMaxCapacity()
    {
        return m_pCalculator != nullptr ? m_pCalculator->maxCapacity() : 0;
    }

    double KnapsackCalculatorWrapper::GetMaxValue()
    {
        return m_pCalculator != nullptr ? m_pCalculator->maxValue() : 0.0;
    }

    array<double>^ KnapsackCalculatorWrapper::GetProfitMatrix()
    {
        if (m_pCalculator == nullptr)
            return nullptr;

        const std::vector<double>& profitMatrix = m_pCalculator->profitMatrix();
        array<double>^ result = gcnew array<double>(static_cast<int>(profitMatrix.size()));

        for (int i = 0; i < static_cast<int>(profitMatrix.size()); i++)
        {
            result[i] = profitMatrix[i];
        }

        return result;
    }

    array<bool>^ KnapsackCalculatorWrapper::GetResultVector()
    {
        if (m_pCalculator == nullptr)
            return nullptr;

        const std::vector<bool>& resultVector = m_pCalculator->resultVector();
        array<bool>^ result = gcnew array<bool>(static_cast<int>(resultVector.size()));

        for (int i = 0; i < static_cast<int>(resultVector.size()); i++)
        {
            result[i] = resultVector[i];
        }

        return result;
    }
}
