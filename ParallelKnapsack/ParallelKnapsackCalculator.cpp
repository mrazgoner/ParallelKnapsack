#include "ParallelKnapsackCalculator.h"

void ParallelKnapsackCalculator::fill_profit_matrix(double*& profitMatrix, int*& weight, double*& profit, bool*& resultVector, int maxCapacity, int n)
{
	initMatrix(profitMatrix, maxCapacity, n);

	for (int item = 1; item <= n; item++)
	{
		for (int capacity = 1; capacity <= maxCapacity; capacity++)
		{
			fill_profit_matrix_kernel(profitMatrix, weight, profit, maxCapacity, n, item, capacity);
		}
	}

	calculate_result_vector(profitMatrix, weight, resultVector, maxCapacity, n);
}


void ParallelKnapsackCalculator::fill_profit_matrix_kernel(double*& profitMatrix, int*& weight, double*& profit, int maxCapacity, int n, int item, int capacity)
{
	int add_weight = weight[item - 1];
	if (add_weight > capacity)
	{
		profitMatrix[(maxCapacity + 1) * item + capacity] = profitMatrix[(maxCapacity + 1) * (item - 1) + capacity];
	}
	else
	{
		double prev_item_profit = profitMatrix[(maxCapacity + 1) * (item - 1) + capacity];
		double add_profit = profitMatrix[(maxCapacity + 1) * (item - 1) + capacity - weight[item - 1]] + profit[item - 1];

		if (prev_item_profit > add_profit)
		{
			profitMatrix[(maxCapacity + 1) * item + capacity] = prev_item_profit;
		}
		else
		{
			profitMatrix[(maxCapacity + 1) * item + capacity] = add_profit;
		}
	}
}

void ParallelKnapsackCalculator::calculate_result_vector(double*& profitMatrix, int*& weight, bool*& resultVector, int maxCapacity, int n)
{
	int i = n;
	int j = maxCapacity;
	double maxValue = profitMatrix[(maxCapacity + 1) * i + j];

	for (int i = 0; i < n; i++)
	{
		resultVector[i] = false;
	}

	for (; i > 0; i--)
	{
		if (profitMatrix[(maxCapacity + 1) * i + j] != profitMatrix[(maxCapacity + 1) * (i - 1) + j])
		{
			resultVector[i - 1] = true;
			j -= weight[i - 1];
		}
	}
}

void ParallelKnapsackCalculator::initMatrix(double*& profitMatrix, int maxCapacity, int n)
{
	profitMatrix = new double[(n + 1) * (maxCapacity + 1)];

	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= maxCapacity; j++)
		{
			profitMatrix[i * maxCapacity + j] = 0;
		}
	}
}