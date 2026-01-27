#include "AntiDiagonalParallelKnapsackCalculator.h"
#include <algorithm>

#ifdef _OPENMP
#include <omp.h>
#endif

void AntiDiagonalParallelKnapsackCalculator::traverse(const std::function<void(int, int)>& worker)
{
    int anti_diagonals = m_rows + m_cols;

    #pragma omp parallel
    {
        for (int d = 2; d < anti_diagonals; ++d)
        {
            int start_row = std::max(1, d - m_cols + 1);
            int end_row = std::min(m_rows - 1, d - 1);
            if (start_row > end_row) continue;

            #pragma omp for schedule(dynamic, 8)
            for (int i = start_row; i <= end_row; ++i)
            {
                int j = d - i;
                worker(i, j);
            }
        }
    }
}