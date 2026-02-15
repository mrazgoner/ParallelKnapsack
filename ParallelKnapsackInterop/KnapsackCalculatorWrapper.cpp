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
            m_pWeights->push_back(static_cast<int>(weights[i]));
        }

        for (int i = 0; i < profits->Length; i++)
        {
            m_pProfits->push_back(static_cast<double>(profits[i]));
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
        array<double>^ result = gcnew array<double>(profitMatrix.size());

        for (size_t i = 0; i < profitMatrix.size(); i++)
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
        array<bool>^ result = gcnew array<bool>(resultVector.size());

        for (size_t i = 0; i < resultVector.size(); i++)
        {
            result[i] = resultVector[i];
        }

        return result;
    }
}
