using CommunityToolkit.Mvvm.ComponentModel;

namespace ParallelKnapsackWPF.Models
{
    /// <summary>
    /// Represents a single item in the knapsack problem
    /// </summary>
    public class KnapsackItem : ObservableObject
    {
        private int _id;
        private string _name;
        private int _weight;
        private double _profit;
        private bool _isSelected;

        public int Id
        {
            get => _id;
            set => SetProperty(ref _id, value);
        }

        public string Name
        {
            get => _name;
            set => SetProperty(ref _name, value);
        }

        public int Weight
        {
            get => _weight;
            set => SetProperty(ref _weight, value);
        }

        public double Profit
        {
            get => _profit;
            set => SetProperty(ref _profit, value);
        }

        public bool IsSelected
        {
            get => _isSelected;
            set => SetProperty(ref _isSelected, value);
        }

        public double Efficiency => Weight > 0 ? Profit / Weight : 0;
    }
}
