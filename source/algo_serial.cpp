#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>
#include <chrono>
#include "plot.h"

using namespace std;

const double a = 0.5;
const double sigma = 0.6;

double f(double x, double a) { return a - sin(x); }

double noise(double h)
{
  static std::default_random_engine gen(random_device{}());
  static std::normal_distribution<double> normal(0.0, 1.0);

  return sigma * sqrt(h) * normal(gen);
}

void Euler_no_noise(vector<double> val, double x0, double t0, double h,
                    double T)
{
  for (double a : val)
  {
    ostringstream ss;
    ss << a;
    string filename = "data/data_a_" + ss.str() + ".dat";
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
    string filename = "data/data_noise_a_" + ss.str() + ".dat";
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
      x = x + h * f(x, a) + noise(h);
      t += h;

      if (x > x_barrier)
      {
        tau.push_back(t);
        break;
      }
    }
  }
  return tau;
}

void crossings_graph(const vector<double> &tau,
                     double Tmax,
                     int bins,
                     int Ntraj)
{
  double dt = Tmax / bins;

  vector<int> hist(bins, 0);
  vector<int> cumulative(bins, 0);

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

  ofstream file("data/particles_left.dat");

  for (int i = 0; i < bins; i++)
  {
    double t = i * dt;

    double left = 1.0 - cumulative[i] / (double)Ntraj;

    file << t << " " << left << endl;
  }

  file.close();

  plot_probability_graph("data/particles_left.dat",
                         "Кол-во частиц внутри ямы",
                         "Время",
                         "Кол-во частиц",
                         3,
                         a, Tmax, sigma, Ntraj);
}

void test(double h, double t0, double x0, double T, int bins, int Ntraj, double Tmax, int iteration_amount) {
    chrono::duration<double> total_time;

    for(int i = 0; i < iteration_amount; i++) {
        cout << "Starting " << "i = " << i << " simulation for Ntraj = " << Ntraj << "..." << endl;

        auto start = chrono::high_resolution_clock::now();

        vector<double> tau = probability_density(a, h, Tmax, Ntraj);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;

        cout << "Time taken for probability_density: " << diff.count() << " seconds" << endl;

        //crossings_graph(tau, Tmax, bins, Ntraj);

        total_time += diff;
    }

    std::cout << endl;
    std::cout << "Total time taken: " << total_time.count() << endl;
    std::cout << "Mean time: " << total_time.count() / iteration_amount << endl;
}

int main()
{
  double h = 0.01;
  double t0 = 0.0;
  double x0 = 0.0;
  double T = 20.0;

  const int bins = 200;
  const int Ntraj = 10000;
  const double Tmax = 1000;

  test(h, t0, x0, T, bins, Ntraj, Tmax, 10);

  // vector<double> a_values = {0.5, 1.5, 2.5};

  // Euler_no_noise(a_values, x0, t0, h, T);
  // Euler_noise(a_values, x0, t0, h, T);
  // cout << "Starting simulation for Ntraj = " << Ntraj << "..." << endl;
  // auto start = chrono::high_resolution_clock::now();
  // vector<double> tau = probability_density(a, h, Tmax, Ntraj);
  // auto end = chrono::high_resolution_clock::now();
  // chrono::duration<double> diff = end - start;
  // cout << "Time taken for probability_density: " << diff.count() << " seconds" << endl;
  // crossings_graph(tau, Tmax, bins, Ntraj);

  return 0;
}
