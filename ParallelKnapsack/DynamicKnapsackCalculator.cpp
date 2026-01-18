#include "DynamicKnapsackCalculator.h"
#include <algorithm>
#include <cassert>

DynamicKnapsackCalculator::DynamicKnapsackCalculator(const std::vector<int>& weight,
                                                       const std::vector<double>& profit,
                                                       int maxCapacity)
    : m_weight(weight)
    , m_profit(profit)
    , m_maxCapacity(maxCapacity)
    , m_n(static_cast<int>(weight.size()))
    , m_rows(m_n + 1)
    , m_cols(m_maxCapacity + 1)
    , m_maxValue(0.0)
{
    m_profitMatrix.assign(static_cast<size_t>(m_rows) * static_cast<size_t>(m_cols), 0.0);
    m_resultVector.assign(static_cast<size_t>(m_n), false);
}

void DynamicKnapsackCalculator::compute()
{
    initMatrix();
    onPreCompute();

    traverse([this](int item, int capacity) {
        this->processCell(item, capacity);
        });

    onPostCompute();
    calculate_result_vector();
}

int DynamicKnapsackCalculator::rows() const noexcept { return m_rows; }
int DynamicKnapsackCalculator::cols() const noexcept { return m_cols; }
int DynamicKnapsackCalculator::itemCount() const noexcept { return m_n; }
int DynamicKnapsackCalculator::maxCapacity() const noexcept { return m_maxCapacity; }

const std::vector<double>& DynamicKnapsackCalculator::profitMatrix() const noexcept
{
    return m_profitMatrix;
}

const std::vector<bool>& DynamicKnapsackCalculator::resultVector() const noexcept
{
    return m_resultVector;
}

double DynamicKnapsackCalculator::maxValue() const noexcept
{
    return m_maxValue;
}

void DynamicKnapsackCalculator::initMatrix()
{
    std::fill(m_profitMatrix.begin(), m_profitMatrix.end(), 0.0);
}

void DynamicKnapsackCalculator::processCell(int item, int capacity)
{
    fill_profit_matrix_kernel_default(item, capacity);
}

void DynamicKnapsackCalculator::traverse(const std::function<void(int, int)>& worker)
{
    for (int item = 1; item <= m_n; ++item)
    {
        for (int capacity = 1; capacity <= m_maxCapacity; ++capacity)
        {
            worker(item, capacity);
        }
    }
}

void DynamicKnapsackCalculator::onPreCompute() {}
void DynamicKnapsackCalculator::onPostCompute() {}

void DynamicKnapsackCalculator::fill_profit_matrix_kernel_default(int item, int capacity)
{
    int add_weight = m_weight[item - 1];
    const int stride = m_cols;

    if (add_weight > capacity)
    {
        m_profitMatrix[stride * item + capacity] = m_profitMatrix[stride * (item - 1) + capacity];
    }
    else
    {
        double prev_item_profit = m_profitMatrix[stride * (item - 1) + capacity];
        double add_profit = m_profitMatrix[stride * (item - 1) + (capacity - add_weight)] + m_profit[item - 1];

        if (prev_item_profit > add_profit)
        {
            m_profitMatrix[stride * item + capacity] = prev_item_profit;
        }
        else
        {
            m_profitMatrix[stride * item + capacity] = add_profit;
        }
    }
}

void DynamicKnapsackCalculator::calculate_result_vector()
{
    const int stride = m_cols;
    int i = m_n;
    int j = m_maxCapacity;
    m_maxValue = m_profitMatrix[stride * i + j];

    std::fill(m_resultVector.begin(), m_resultVector.end(), false);

    for (; i > 0; --i)
    {
        if (m_profitMatrix[stride * i + j] != m_profitMatrix[stride * (i - 1) + j])
        {
            m_resultVector[i - 1] = true;
            j -= m_weight[i - 1];
            if (j < 0) j = 0;
        }
    }
}
