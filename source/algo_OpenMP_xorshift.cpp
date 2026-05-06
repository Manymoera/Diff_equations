#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <iomanip>
#include "plot.h"
#include "xorshift.h"

using namespace std;

const double a = 0.5;
const double sigma = 1.0;

namespace tau
{
    const double B = 2.0 / (sigma * sigma);

    double u_func(double x) { return -B * (a * x + cos(x)); }
    double exp_u(double x) { return exp(u_func(x)); }
    double exp_minus_u(double x) { return exp(-u_func(x)); }

    double integrate(double (*func)(double), double start, double end, int steps = 1000)
    {
        if (abs(start - end) < 1e-9)
            return 0.0;
        double step = (end - start) / steps;
        double area = 0.0;
        for (int i = 0; i < steps; i++)
        {
            area += (func(start + i * step) + func(start + (i + 1) * step));
        }
        return area * step / 2.0;
    }

    double calculate_theoretical_tau()
    {
        double c = asin(a) - 1.0;
        double d = M_PI - asin(a);

        int n = 1000;
        double dv = (d - c) / n;
        double term1 = 0.0;

        auto get_val = [&](double v)
        {
            return exp_u(v) * integrate(exp_minus_u, c, v, 1000);
        };

        double prev_val = 0.0;
        for (int i = 1; i <= n; i++)
        {
            double v = c + i * dv;
            double current_val = get_val(v);
            term1 += (prev_val + current_val);
            prev_val = current_val;
        }

        return B * (term1 * dv / 2.0);
    }
}

double f(double x, double a) { return a - sin(x); }

double noise(double h)
{

    static thread_local Xorshift64 gen(std::random_device{}());

    double u1 = gen.next_double();
    double u2 = gen.next_double();

    double z = sqrt(-2.0 * log(u1 + 1e-15)) * cos(2.0 * M_PI * u2);

    return sigma * sqrt(h) * z;
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

vector<double> probability_density(double a, double h, double Tmax, int Ntraj)
{
    double x_start = asin(a) - 1;
    double x_barrier = M_PI - asin(a);

    vector<double> tau(Ntraj, -1.0);

    #pragma omp parallel for
    for (int i = 0; i < Ntraj; i++)
    {
        double x = x_start;
        double t = 0;
        while (t < Tmax)
        {
            x = x + h * f(x, a) + noise(h);
            if (x < x_start) x = x_start + (x_start - x);
            t += h;

            if (x > x_barrier)
            {
                tau[i] = t;
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
        // Учитываем только те частицы, которые покинули яму (t >= 0)
        if (t >= 0)
        {
            int k = t / dt;
            if (k < bins)
                hist[k]++;
        }
    }

    int sum = 0;

    for (int i = 0; i < bins; i++)
    {
        sum += hist[i];
        cumulative[i] = sum;
    }

    ofstream file("data/particles_left.dat");

    double auc = 0.0;
    double prev_left = 1.0; // В момент t=0 в яме 100% частиц (1.0)

    for (int i = 0; i < bins; i++)
    {
        double t = i * dt;

        double left = 1.0 - cumulative[i] / (double)Ntraj;

        file << t << " " << left << endl;

        // МЕТОД ТРАПЕЦИЙ: Площадь = ( (y1 + y2) / 2 ) * dx
        auc += (prev_left + left) / 2.0 * dt;
        prev_left = left;
    }

    double sum_t = 0;
    int escaped = 0;
    for (double t : tau)
    {
        if (t > 0)
        {
            sum_t += t;
            escaped++;
        }
    }

    file.close();

    cout << "Площадь под графиком: " << auc << endl;

    plot_probability_graph("data/particles_left.dat",
                           "Кол-во частиц внутри ямы",
                           "Время",
                           "Кол-во частиц",
                           3,
                           a, Tmax, sigma, Ntraj);
}

void test(double h, double t0, double x0, double T, int bins, int Ntraj, double Tmax, int iteration_amount)
{
    chrono::duration<double> total_time;

    for (int i = 0; i < iteration_amount; i++)
    {
        //cout << "Starting " << "i = " << i << " simulation for Ntraj = " << Ntraj << "..." << endl;

        auto start = chrono::high_resolution_clock::now();

        vector<double> tau = probability_density(a, h, Tmax, Ntraj);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;

        //cout << "Time taken for probability_density: " << diff.count() << " seconds" << endl;

        crossings_graph(tau, Tmax, bins, Ntraj);

        total_time += diff;
    }

    std::cout << endl;
    //std::cout << "Total time taken: " << total_time.count() << endl;
    //std::cout << "Mean time: " << total_time.count() / iteration_amount << endl;
}

int main()
{
    double h = 0.001;
    double t0 = 0.0;
    double x0 = 0.0;
    double T = 20.0;

    const int bins = 1000;
    const int Ntraj = 10000;
    const double Tmax = 1000;

    vector<double> a_values = {0.5, 1.5, 2.5};

    test(h, t0, x0, T, bins, Ntraj, Tmax, 1);

    double tau_theory = tau::calculate_theoretical_tau();

    cout << fixed << setprecision(3);
    cout << "Аналитический рассчет:" << endl;
    cout << "Параметр a: " << a << endl;
    cout << "Параметр sigma: " << sigma << endl;
    cout << "tau = " << tau_theory << endl;

    return 0;
}
