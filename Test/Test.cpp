// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

#ifdef _OPENMP
#include <omp.h>
#endif

int main()
{
    const int nItems = 8;
    const int capacity = 10;
    const int m_rows = nItems + 1;
    const int m_cols = capacity + 1;

    // Serial traversal order (just for reference)
    std::vector<int> order(static_cast<size_t>(m_rows) * static_cast<size_t>(m_cols), 0);

    int count = 0;
    int anti_diagonals = m_rows + m_cols;
    for (int d = 2; d < anti_diagonals; ++d)
    {
        int start_row = std::max(1, d - m_cols + 1);
        int end_row = std::min(m_rows - 1, d - 1);
        if (start_row > end_row) continue;
        for (int i = start_row; i <= end_row; ++i)
        {
            int j = d - i;
            if (j < 1 || j >= m_cols) continue;
            ++count;
            order[static_cast<size_t>(i) * m_cols + j] = count;
        }
    }

    const char* outName = "traversal_test.tsv";
    std::ofstream out(outName);
    if (!out) { std::cerr << "Failed to open " << outName << '\n'; return 1; }

    out << "row\\col";
    for (int c = 0; c < m_cols; ++c) out << '\t' << c;
    out << '\n';
    for (int r = 0; r < m_rows; ++r)
    {
        out << r;
        for (int c = 0; c < m_cols; ++c) out << '\t' << order[static_cast<size_t>(r) * m_cols + c];
        out << '\n';
    }
    out << '\n';
    out.close();
    std::cout << "Wrote serial traversal block (" << m_rows << "x" << m_cols << "), count=" << count << '\n';

    // --- Parallel diagnostic and thread map ---
    std::vector<int> threadMap(static_cast<size_t>(m_rows) * static_cast<size_t>(m_cols), 0);
    // Map of anti-diagonal number for each visited cell (serial and parallel)
    std::vector<int> serialDiagMap(static_cast<size_t>(m_rows) * static_cast<size_t>(m_cols), 0);
    std::vector<int> parallelDiagMap(static_cast<size_t>(m_rows) * static_cast<size_t>(m_cols), 0);

    // Fill serialDiagMap during the same anti-diagonal traversal
    for (int d = 2; d < anti_diagonals; ++d)
    {
        int start_row = std::max(1, d - m_cols + 1);
        int end_row = std::min(m_rows - 1, d - 1);
        if (start_row > end_row) continue;
        for (int i = start_row; i <= end_row; ++i)
        {
            int j = d - i;
            if (j < 1 || j >= m_cols) continue;
            serialDiagMap[static_cast<size_t>(i) * m_cols + j] = d;
        }
    }

#ifdef _OPENMP
    // Print OpenMP team size for visibility
    #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "OpenMP active: max_threads=" << omp_get_max_threads()
                      << " team_size=" << omp_get_num_threads() << '\n';
        }

        for (int d = 2; d < anti_diagonals; ++d)
        {
            int start_row = std::max(1, d - m_cols + 1);
            int end_row = std::min(m_rows - 1, d - 1);
            if (start_row > end_row) continue;

            // dynamic scheduling for small/imbalanced diagonals
            #pragma omp for schedule(dynamic, 8)
            for (int i = start_row; i <= end_row; ++i)
            {
                int j = d - i;
                if (j < 1 || j >= m_cols) continue;
                int tid = omp_get_thread_num();
                threadMap[static_cast<size_t>(i) * m_cols + j] = tid + 1; // 1-based thread number
                parallelDiagMap[static_cast<size_t>(i) * m_cols + j] = d; // record diagonal number (sync number)
            }
            // implicit barrier: synchronization between diagonals
        }
    }
#else
    // Fallback single-threaded parallel map (all 1)
    for (int d = 2; d < anti_diagonals; ++d)
    {
        int start_row = std::max(1, d - m_cols + 1);
        int end_row = std::min(m_rows - 1, d - 1);
        if (start_row > end_row) continue;
        for (int i = start_row; i <= end_row; ++i)
        {
            int j = d - i;
            if (j < 1 || j >= m_cols) continue;
            threadMap[static_cast<size_t>(i) * m_cols + j] = 1;
            parallelDiagMap[static_cast<size_t>(i) * m_cols + j] = d;
        }
    }
#endif

    // Append parallel thread map
    std::ofstream outAppend(outName, std::ios::app);
    if (!outAppend) { std::cerr << "Failed to append " << outName << '\n'; return 1; }

    outAppend << "Parallel traversal (thread numbers, 0=boundary)\n";
    outAppend << "row\\col";
    for (int c = 0; c < m_cols; ++c) outAppend << '\t' << c;
    outAppend << '\n';
    for (int r = 0; r < m_rows; ++r)
    {
        outAppend << r;
        for (int c = 0; c < m_cols; ++c) outAppend << '\t' << threadMap[static_cast<size_t>(r) * m_cols + c];
        outAppend << '\n';
    }
    outAppend << '\n';

    // Append serial anti-diagonal numbers (sync id that would occur)
    outAppend << "Serial anti-diagonal number per cell (0 = boundary)\n";
    outAppend << "row\\col";
    for (int c = 0; c < m_cols; ++c) outAppend << '\t' << c;
    outAppend << '\n';
    for (int r = 0; r < m_rows; ++r)
    {
        outAppend << r;
        for (int c = 0; c < m_cols; ++c) outAppend << '\t' << serialDiagMap[static_cast<size_t>(r) * m_cols + c];
        outAppend << '\n';
    }
    outAppend << '\n';

    // Append parallel anti-diagonal numbers (the actual sync id assigned during parallel run)
    outAppend << "Parallel anti-diagonal number per cell (0 = boundary)\n";
    outAppend << "row\\col";
    for (int c = 0; c < m_cols; ++c) outAppend << '\t' << c;
    outAppend << '\n';
    for (int r = 0; r < m_rows; ++r)
    {
        outAppend << r;
        for (int c = 0; c < m_cols; ++c) outAppend << '\t' << parallelDiagMap[static_cast<size_t>(r) * m_cols + c];
        outAppend << '\n';
    }
    outAppend << '\n';

    outAppend.close();

    std::cout << "Appended thread map and diagonal (sync) maps to " << outName << ".\n";
    return 0;
}
