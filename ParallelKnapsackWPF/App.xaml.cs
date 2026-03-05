using System;
using System.Windows;
using Microsoft.Extensions.DependencyInjection;
using ParallelKnapsackWPF.Views;
using ParallelKnapsackWPF.ViewModels;
using ParallelKnapsackWPF.Services;

namespace ParallelKnapsackWPF
{
    public partial class App : Application
    {
        private IServiceProvider _serviceProvider;

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            var services = new ServiceCollection();
            ConfigureServices(services);
            _serviceProvider = services.BuildServiceProvider();

            var mainWindow = _serviceProvider.GetRequiredService<MainWindow>();
            mainWindow.Show();
        }

        private void ConfigureServices(IServiceCollection services)
        {
            // Services
            services.AddSingleton<IKnapsackInteropService, KnapsackInteropService>();

            // ViewModels
            services.AddSingleton<MainViewModel>();

            // Windows
            services.AddSingleton<MainWindow>();
        }

        protected override void OnExit(ExitEventArgs e)
        {
            if (_serviceProvider is IDisposable disposable)
            {
                disposable.Dispose();
            }
            base.OnExit(e);
        }
    }
}
