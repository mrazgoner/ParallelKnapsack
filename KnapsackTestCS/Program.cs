using System;
using ParallelKnapsackInterop;

namespace KnapsackTestCS
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=== Parallel Knapsack Calculator - C# Test ===\n");

            // Sample knapsack problem
            int[] weights = { 2, 3, 4, 5 };
            double[] profits = { 3.0, 4.0, 5.0, 6.0 };
            int capacity = 5;

            Console.WriteLine("Problem Parameters:");
            Console.WriteLine($"Weights: [{string.Join(", ", weights)}]");
            Console.WriteLine($"Profits: [{string.Join(", ", profits)}]");
            Console.WriteLine($"Capacity: {capacity}\n");

            // Test Sequential Algorithm
            TestAlgorithm("Sequential", weights, profits, capacity,
                KnapsackCalculatorWrapper.AlgorithmType.Sequential);

            // Test Parallel Algorithm
            TestAlgorithm("Parallel", weights, profits, capacity,
                KnapsackCalculatorWrapper.AlgorithmType.Parallel);

            // Test Anti-Diagonal Parallel Algorithm
            TestAlgorithm("Anti-Diagonal Parallel", weights, profits, capacity,
                KnapsackCalculatorWrapper.AlgorithmType.AntiDiagonalParallel);

            Console.WriteLine("\nPress any key to exit...");
            Console.ReadKey();
        }

        static void TestAlgorithm(string name, int[] weights, double[] profits, int capacity,
            KnapsackCalculatorWrapper.AlgorithmType algorithm)
        {
            Console.WriteLine($"--- {name} Algorithm ---");

            using (var calculator = new KnapsackCalculatorWrapper(weights, profits, capacity, algorithm))
            {
                // Measure execution time
                var stopwatch = System.Diagnostics.Stopwatch.StartNew();
                calculator.Compute();
                stopwatch.Stop();

                // Display results
                Console.WriteLine($"Max Value: {calculator.GetMaxValue()}");
                Console.WriteLine($"Items Included: [{string.Join(", ", calculator.GetResultVector())}]");
                Console.WriteLine($"Matrix Size: {calculator.GetRows()} x {calculator.GetCols()}");
                Console.WriteLine($"Execution Time: {stopwatch.ElapsedMilliseconds} ms\n");
            }
        }
    }
}
