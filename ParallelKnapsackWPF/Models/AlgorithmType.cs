namespace ParallelKnapsackWPF.Models
{
    /// <summary>
    /// Algorithm type enumeration matching the interop layer
    /// </summary>
    public enum AlgorithmType
    {
        Sequential,
        Parallel,
        AntiDiagonal,
        Cuda
    }
}
