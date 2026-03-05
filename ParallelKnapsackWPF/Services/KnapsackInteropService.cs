using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using ParallelKnapsackInterop;
using ParallelKnapsackWPF.Models;

namespace ParallelKnapsackWPF.Services
{
    /// <summary>
    /// Service implementation wrapping the native C++ interop layer
    /// </summary>
    public class KnapsackInteropService : IKnapsackInteropService
    {
        public async Task<KnapsackResult> ComputeAsync(
            IEnumerable<KnapsackItem> items,
            int capacity,
            AlgorithmType algorithm,
            CancellationToken cancellationToken = default)
        {
            return await Task.Run(() =>
            {
                var itemsList = items.ToList();
                var weights = itemsList.Select(i => i.Weight).ToArray();
                var profits = itemsList.Select(i => i.Profit).ToArray();

                using (var calc = new KnapsackCalculatorWrapper(
                    weights,
                    profits,
                    capacity,
                    MapAlgorithmType(algorithm)))
                {
                    var sw = Stopwatch.StartNew();
                    calc.Compute();
                    sw.Stop();

                    var resultVector = calc.GetResultVector();
                    var selectedItems = itemsList
                        .Where((item, i) => resultVector[i])
                        .Select(item => new KnapsackItem
                        {
                            Id = item.Id,
                            Name = item.Name,
                            Weight = item.Weight,
                            Profit = item.Profit,
                            IsSelected = true
                        })
                        .ToList();

                    return new KnapsackResult
                    {
                        MaxValue = calc.GetMaxValue(),
                        SelectedItems = selectedItems,
                        TotalWeight = selectedItems.Sum(i => i.Weight),
                        ExecutionTime = sw.Elapsed,
                        Algorithm = algorithm,
                        ComputedAt = DateTime.Now,
                        ItemCount = itemsList.Count,
                        Capacity = capacity
                    };
                }
            }, cancellationToken);
        }

        private KnapsackCalculatorWrapper.AlgorithmType MapAlgorithmType(AlgorithmType type)
        {
            return type switch
            {
                AlgorithmType.Sequential => KnapsackCalculatorWrapper.AlgorithmType.Sequential,
                AlgorithmType.Parallel => KnapsackCalculatorWrapper.AlgorithmType.Parallel,
                AlgorithmType.AntiDiagonal => KnapsackCalculatorWrapper.AlgorithmType.AntiDiagonalParallel,
                AlgorithmType.Cuda => KnapsackCalculatorWrapper.AlgorithmType.Cuda,
                _ => throw new ArgumentException($"Unknown algorithm type: {type}")
            };
        }
    }
}
