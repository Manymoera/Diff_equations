#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;

// Константы из вашей задачи
const double a_val = 0.5;
const double sigma_val = 0.6;

// Параметр B из формулы (6) и (25)
// Для Langevin dx = f*dt + sigma*dW, D = sigma^2 / 2. 
// В тексте B = 1/D для соответствия коэф. диффузии.
const double B = 2.0 / (sigma_val * sigma_val); 

// Функция потенциала u(x) = -B * integral(f(x) dx)
double u_func(double x) {
    return -B * (a_val * x + cos(x));
}

// Подынтегральные функции
double exp_u(double x) { return exp(u_func(x)); }
double exp_minus_u(double x) { return exp(-u_func(x)); }

// Численное интегрирование методом трапеций
double integrate(double (*func)(double), double start, double end, int steps = 10000) {
    double step = (end - start) / steps;
    double area = 0.0;
    for (int i = 0; i < steps; i++) {
        double x1 = start + i * step;
        double x2 = start + (i + 1) * step;
        area += (func(x1) + func(x2)) / 2.0 * step;
    }
    return area;
}

double calculate_theoretical_tau() {
    double c = asin(a_val) - 1.0;          // Левая граница
    double d = M_PI - asin(a_val);         // Правая граница (барьер)
    double x0 = c;                         // Начальная точка
    double L = 20.0;                       // "Бесконечность" для интеграла

    int n = 2000;
    double dv = (d - x0) / n;
    
    // 1. Первый член формулы (25): интеграл от x0 до d [ e^u(v) * интеграл от c до v (e^-u(y) dy) ]
    double term1 = 0.0;
    for (int i = 0; i < n; i++) {
        double v = x0 + i * dv;
        double inner_integral = integrate(exp_minus_u, c, v, 500);
        term1 += exp_u(v) * inner_integral * dv;
    }

    // 2. Второй член формулы (25): интеграл от d до inf [e^u(v)] * интеграл от c до d [e^-u(y)]
    double integral_d_inf = integrate(exp_u, d, L, 5000);
    double integral_c_d = integrate(exp_minus_u, c, d, 2000);
    double term2 = integral_d_inf * integral_c_d;

    return B * (term1 + term2);
}

int main() {
    double tau_theory = calculate_theoretical_tau();
    
    cout << fixed << setprecision(6);
    cout << "--- Теоретический расчет (Формула 25) ---" << endl;
    cout << "Параметр a: " << a_val << endl;
    cout << "Параметр sigma: " << sigma_val << endl;
    cout << "Параметр B: " << B << endl;
    cout << "Теоретическое среднее время жизни (tau): " << tau_theory << endl;
    
    return 0;
}