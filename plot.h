#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

using namespace std;

void plot_graph(const vector<double> &val,
                const string &prefix,
                const string &title,
                int window)
{
  string script = "plot_" + prefix + ".gnu";
  ofstream plot(script);

  plot << "set term qt " << window << "\n";
  plot << "set title '" << title << "'\n";
  plot << "set xlabel 't'\n";
  plot << "set ylabel 'x(t)'\n";
  plot << "plot ";

  for (int i = 0; i < val.size(); i++)
  {
    plot << "'" << prefix << "_a_" << val[i]
         << ".dat' with lines title 'a=" << val[i] << "'";

    if (i != val.size() - 1)
      plot << ", ";
  }

  plot << "\npause -1\n";
  plot.close();

  string cmd = "gnuplot -persist " + script + " &";
  system(cmd.c_str());
}

void plot_probability_graph(const string& file,
                            const string& title,
                            const string& xlabel,
                            const string& ylabel,
                            int window,
                            double a,
                            double Tmax,
                            double sigma,
                            int Ntraj)
{
    ofstream plot("plot_temp.gnu");

    plot << "set term qt " << window << " size 1200,800\n";

    plot << "set title '" << title << "'\n";
    plot << "set xlabel '" << xlabel << "'\n";
    plot << "set ylabel '" << ylabel << "'\n";

    plot << "set xrange [-5:*]\n";
    plot << "set yrange [-0.2:1.2]\n";

    plot << "set grid\n";

    plot << "set label 1 "
         << "'a=" << a
         << ", T=" << Tmax
         << ", sigma= "<< sigma
         << "' at graph 0.02,0.95\n";

    plot << "plot '" << file << "' with lines lw 2 title 'data'\n";

    plot << "pause -1\n";

    plot.close();

    system("gnuplot -persist plot_temp.gnu &");
}