#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include "plot.h"

using namespace std;

double f(double x, double a) { return a - sin(x); }

double noise(double h, double _sigma)
{
  static std::default_random_engine gen(random_device{}());
  static std::normal_distribution<double> normal(0.0, 1.0);

  return _sigma * sqrt(h) * normal(gen);
}

void Euler_no_noise(vector<double> val, double x0, double t0, double h,
                    double T)
{
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

  plot_graph(val, "data", "Метод Эйлера: dx/dt = a - sin(x)", 0);
}

void Euler_noise(vector<double> val, double x0, double t0, double h, double T)
{
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

      x = x + h * f(x, a) + noise(h, 0.3);
      t = t + h;
    }

    file.close();
  }

  plot_graph(val, "data_noise", "Метод Эйлера с шумом", 1);
}

vector<double> probability_density(double a,
                                   double h,
                                   double Tmax,
                                   int Ntraj)
{
  double x_start = asin(a) - 1;
  double x_barrier = M_PI - asin(a);

  vector<double> tau;

  for (int i = 0; i < Ntraj; i++)
  {
    double x = x_start;
    double t = 0;

    while (t < Tmax)
    {
      x = x + h * (a - sin(x)) + noise(h, 0.3);
      t += h;

      if (x > x_barrier)
      {
        tau.push_back(t);
        break;
      }
    }
  }

  cout << "Transitions found: " << tau.size() << endl;

  return tau;
}

void probability_graph(const vector<double> &tau,
                       double Tmax,
                       int bins)
{
  double dt = Tmax / bins;

  vector<int> hist(bins, 0);

  for (double t : tau)
  {
    int k = t / dt;
    if (k < bins)
      hist[k]++;
  }

  ofstream file("probability.dat");

  for (int i = 0; i < bins; i++)
  {
    double t = i * dt;

    double p = 0;
    if (tau.size() > 0)
      p = hist[i] / (double)(tau.size() * dt);

    file << t << " " << p << endl;
  }

  file.close();

  plot_probability_graph("probability.dat",
                         "Barrier crossing probability density",
                         "time",
                         "P(t)",
                         2,
                         dt);
}

void crossings_graph(const vector<double>& tau,
                     double Tmax,
                     int bins,
                     int Ntraj)
{
    double dt = Tmax / bins;

    vector<int> hist(bins,0);
    vector<int> cumulative(bins,0);

    for (double t : tau)
    {
        int k = t / dt;
        if (k < bins)
            hist[k]++;
    }

    int sum = 0;

    for (int i = 0; i < bins; i++)
    {
        sum += hist[i];
        cumulative[i] = sum;
    }

    ofstream file("particles_left.dat");

    for (int i = 0; i < bins; i++)
    {
        double t = i * dt;

        double left = 1.0 - cumulative[i] / (double)Ntraj;

        file << t << " " << left << endl;
    }

    file.close();

    plot_probability_graph("particles_left.dat",
                           "Кол-во частиц за барьером",
                           "Время",
                           "Кол-во частиц",
                           2,
                           dt);
}

int main()
{
  double h = 0.01;

  double t0 = 0.0;
  double x0 = 0.0;
  double T = 20.0;

  vector<double> a_values = {0.5, 1.5, 2.5};

  //Euler_no_noise(a_values, x0, t0, h, T);
  //Euler_noise(a_values, x0, t0, h, T);

  double a = 0.95;
  double Tmax = 100;
  int Ntraj = 10000;
  int bins = 200;

  vector<double> tau = probability_density(a, h, Tmax, Ntraj);

  //probability_graph(tau, Tmax, bins);

  crossings_graph(tau, Tmax, bins, Ntraj);

  return 0;
}
