#pragma once 

static class ParallelKnapsackCalculator
{
public:
	static void fill_profit_matrix(double*& profitMatrix, int*& weight, double*& profit, bool*& resultVector, int maxCapacity, int n);
private:
	static void fill_profit_matrix_kernel(double*& profitMatrix, int*& weight, double*& profit, int maxCapacity, int n, int item, int capacity);
	static void calculate_result_vector(double*& profitMatrix, int*& weight, bool*& resultVector, int maxCapacity, int n);
	static void initMatrix(double*& profitMatrix, int maxCapacity, int n);
};
