#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>
#include "plot.h"

using namespace std;

double f(double x, double a) { return a - sin(x); }

double noise(double h)
{
  std::default_random_engine gen;
  std::normal_distribution<double> normal(0.0, 1.0);

  double sigma = 0.3;

  return sigma * sqrt(h) * normal(gen);
}

void Euler_no_noise(vector<double> val, double x0, 
                    double t0, double h, double T, int window) {
  for (double a : val)
  {
    ostringstream ss;
    ss << a;
    string filename = "data_a_" + ss.str() + ".dat";
    ofstream file(filename);

    double t = t0;
    double x = x0;

    while (t <= T)
    {
      file << t << " " << x << endl;

      x = x + h * f(x, a);
      t = t + h;
    }

    file.close();
  }

  plot_graph(val,
           "data",
           "Euler method: dx/dt = a - sin(x)",
           0);
}

void Euler_noise(vector<double> val, double x0, 
                double t0, double h, double T, int window) {
  for (double a : val)
  {
    ostringstream ss;
    ss << a;
    string filename = "data_noise_a_" + ss.str() + ".dat";
    ofstream file(filename);

    double t = t0;
    double x = x0;

    while (t <= T)
    {
      file << t << " " << x << endl;

      x = x + h * f(x, a) + noise(h);
      t = t + h;
    }

    file.close();
  }

  plot_graph(val,
           "data_noise",
           "Euler method with noise",
           1);
}



int main()
{
  double t0 = 0.0;
  double x0 = 0.0;
  double h = 0.01;
  double T = 20.0;

  vector<double> a_values = {0.5, 1.5, 2.5};

  Euler_no_noise(a_values, x0, t0, h, T, 0);
  Euler_noise(a_values, x0, t0, h, T, 1);

  return 0;
}
