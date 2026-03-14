using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using ParallelKnapsackWPF.Models;
using ParallelKnapsackWPF.Services;

namespace ParallelKnapsackWPF.ViewModels
{
    /// <summary>
    /// Main ViewModel managing calculator operations
    /// </summary>
    public class MainViewModel : ObservableObject
    {
        private readonly IKnapsackInteropService _interopService;

        private AlgorithmType _selectedAlgorithm = AlgorithmType.Sequential;
        private int _capacity = 100;
        private bool _isBusy;
        private string _statusMessage = "Ready";
        private KnapsackResult _result;

        public MainViewModel(IKnapsackInteropService interopService)
        {
            _interopService = interopService;
            Items = new ObservableCollection<KnapsackItem>();

            AddItemCommand = new RelayCommand(AddItem);
            RemoveItemCommand = new RelayCommand<KnapsackItem>(
                item => RemoveItem(item),
                item => item != null);
            ComputeCommand = new AsyncRelayCommand(
                ComputeAsync,
                () => Items.Count > 0 && Capacity > 0 && !IsBusy);
            ClearCommand = new RelayCommand(Clear);

            // Add some sample items
            AddSampleItems();
        }

        public ObservableCollection<KnapsackItem> Items { get; }

        public AlgorithmType SelectedAlgorithm
        {
            get => _selectedAlgorithm;
            set => SetProperty(ref _selectedAlgorithm, value);
        }

        public int Capacity
        {
            get => _capacity;
            set
            {
                if (SetProperty(ref _capacity, value))
                    CommandManager.InvalidateRequerySuggested();
            }
        }

        public bool IsBusy
        {
            get => _isBusy;
            set
            {
                if (SetProperty(ref _isBusy, value))
                    CommandManager.InvalidateRequerySuggested();
            }
        }

        public string StatusMessage
        {
            get => _statusMessage;
            set => SetProperty(ref _statusMessage, value);
        }

        public KnapsackResult Result
        {
            get => _result;
            set => SetProperty(ref _result, value);
        }

        public ICommand AddItemCommand { get; }
        public ICommand RemoveItemCommand { get; }
        public ICommand ComputeCommand { get; }
        public ICommand ClearCommand { get; }

        private void AddItem()
        {
            var id = Items.Count > 0 ? Items.Max(i => i.Id) + 1 : 1;
            Items.Add(new KnapsackItem
            {
                Id = id,
                Name = $"Item {id}",
                Weight = 1,
                Profit = 1
            });
            CommandManager.InvalidateRequerySuggested();
        }

        private void RemoveItem(KnapsackItem item)
        {
            Items.Remove(item);
            CommandManager.InvalidateRequerySuggested();
        }

        private async Task ComputeAsync()
        {
            try
            {
                IsBusy = true;
                StatusMessage = "Computing...";
                Result = null;

                Result = await _interopService.ComputeAsync(
                    Items,
                    Capacity,
                    SelectedAlgorithm);

                StatusMessage = $"Completed in {Result.ExecutionTime.TotalMilliseconds:F2}ms using {Result.Algorithm}";
            }
            catch (Exception ex)
            {
                StatusMessage = $"Error: {ex.Message}";
                MessageBox.Show(
                    ex.Message,
                    "Computation Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);
            }
            finally
            {
                IsBusy = false;
            }
        }

        private void Clear()
        {
            Items.Clear();
            Result = null;
            StatusMessage = "Ready";
            CommandManager.InvalidateRequerySuggested();
        }

        private void AddSampleItems()
        {
            Items.Add(new KnapsackItem { Id = 1, Name = "Item 1", Weight = 2, Profit = 3 });
            Items.Add(new KnapsackItem { Id = 2, Name = "Item 2", Weight = 3, Profit = 4 });
            Items.Add(new KnapsackItem { Id = 3, Name = "Item 3", Weight = 4, Profit = 5 });
            Items.Add(new KnapsackItem { Id = 4, Name = "Item 4", Weight = 5, Profit = 6 });
        }
    }
}
