using System;
using System.Collections.Generic;

namespace ParallelKnapsackWPF.Models
{
    /// <summary>
    /// Result of a knapsack computation
    /// </summary>
    public class KnapsackResult
    {
        public double MaxValue { get; set; }
        public List<KnapsackItem> SelectedItems { get; set; }
        public int TotalWeight { get; set; }
        public TimeSpan ExecutionTime { get; set; }
        public AlgorithmType Algorithm { get; set; }
        public double[,] DpMatrix { get; set; }
        public DateTime ComputedAt { get; set; }
        public int ItemCount { get; set; }
        public int Capacity { get; set; }
    }
}
