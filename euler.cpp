#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

// Правая часть уравнения
double f(double x, double a)
{
    return a - sin(x);
}

int main()
{
    double t0 = 0.0;
    double x0 = 0.0;      // начальное значение x
    double h = 0.01;     // шаг
    double T = 20.0;

    vector<double> a_values = { 0.5, 1.5, 2.5 };

    for (double a : a_values)
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

            // Метод Эйлера
            x = x + h * f(x, a);
            t = t + h;
        }

        file.close();
    }

    // Создание скрипта для gnuplot
    ofstream plot("plot.gnu");
    plot << "set title 'Euler method: dx/dt = a - sin(x)'\n";
    plot << "set xlabel 't'\n";
    plot << "set ylabel 'x(t)'\n";
    plot << "plot ";

    for (int i = 0; i < a_values.size(); i++)
    {
        plot << "'data_a_" << a_values[i] << ".dat' with lines title 'a="
            << a_values[i] << "'";
        if (i != a_values.size() - 1)
            plot << ", ";
    }

    plot << "\npause -1\n";
    plot.close();

    system("gnuplot plot.gnu");

    return 0;
}
