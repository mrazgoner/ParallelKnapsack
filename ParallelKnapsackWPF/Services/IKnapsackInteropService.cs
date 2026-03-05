using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using ParallelKnapsackWPF.Models;

namespace ParallelKnapsackWPF.Services
{
    /// <summary>
    /// Service interface for knapsack computation via interop layer
    /// </summary>
    public interface IKnapsackInteropService
    {
        Task<KnapsackResult> ComputeAsync(
            IEnumerable<KnapsackItem> items,
            int capacity,
            AlgorithmType algorithm,
            CancellationToken cancellationToken = default);
    }
}
