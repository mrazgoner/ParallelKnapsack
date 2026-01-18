
#pragma once

#include <vector>
#include <functional>

class DynamicKnapsackCalculator
{
public:
    DynamicKnapsackCalculator(const std::vector<int>& weight, const std::vector<double>& profit, int maxCapacity);
    virtual ~DynamicKnapsackCalculator() = default;
    void compute();

    int rows() const noexcept;
    int cols() const noexcept;
    int itemCount() const noexcept;
    int maxCapacity() const noexcept;
    double maxValue() const noexcept;

    const std::vector<double>& profitMatrix() const noexcept;
    const std::vector<bool>& resultVector() const noexcept;

    virtual void traverse(const std::function<void(int, int)>& worker);

protected:
    virtual void processCell(int item, int capacity);
    virtual void onPreCompute();
    virtual void onPostCompute();

private:
    void initMatrix();
    void fill_profit_matrix_kernel_default(int item, int capacity);
    void calculate_result_vector();

protected:
    std::vector<double> m_profitMatrix;
    std::vector<int>    m_weight;
    std::vector<double> m_profit;
    std::vector<bool>   m_resultVector;

    int m_maxCapacity;
    int m_n;
    int m_rows;
    int m_cols;
    double m_maxValue;
};