#include "ParallelKnapsackCalculator.h"


void ParallelKnapsackCalculator::traverse(const std::function<void(int, int)>& worker)
{
	#pragma omp parallel
	{
		for (int item = 1; item <= m_n; ++item)
		{
			#pragma omp for schedule(static)
			for (int capacity = 1; capacity <= m_maxCapacity; ++capacity)
			{
				worker(item, capacity);
			}
		}
	}
}