using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace WpfApp1
{
  /// <summary>
  /// Interaction logic for App.xaml
  /// </summary>
  public partial class App : Application
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            int port_count = Convert.ToInt32(e.Args[0]);
            for ( int i= 0;i< Math.Min(port_count, 9178); i++)
            {
                int port = 9179 - i;
                MainWindow window = new MainWindow(port);
                window.Show();
            }
           
        }
    }
}
