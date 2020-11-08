// Lab 1 - Multivariable optimization
// Made by: Aleksandra Podgurska and Szymon Siąkała
// Mk 2
// Group C
// Section 2

#include <iostream>
#include <cmath>

const double T = 0.618;
const double E = 0.00000000000000000001;

double function_value(double x, double y)
{
    return (exp(cos(x - 0.7)) + exp(cos(y + 0.1)));
}

double function_gradient_x(double x)
{
    return(sin(0.7 - x) * exp(cos(0.7 - x)));
}

double function_gradient_y(double y)
{
    return(-sin(0.1 + y) * exp(cos(0.1 + y)));
}

void interval_value(long double z[2], long double d[2], long double* interval, double function_interval[2])
{
    double interval1[2];
    double interval2[2];
    if (d[0] != 0)
    {
        interval1[0] = (function_interval[0] - z[0]) / d[0];
        interval1[1] = (function_interval[1] - z[0]) / d[0];
        if (interval1[0] > interval1[1])
        {
            double temp = interval1[0];
            interval1[0] = interval1[1];
            interval1[1] = temp;
        }
    }
    if (d[1] != 0)
    {
        interval2[0] = (function_interval[0] - z[1]) / d[1];
        interval2[1] = (function_interval[1] - z[1]) / d[1];
        if (interval2[0] > interval2[1])
        {
            double temp = interval2[0];
            interval2[0] = interval2[1];
            interval2[1] = temp;
        }
    }
    if (interval1[0] <= interval2[0])
    {
        interval[0] = interval2[0];
    }
    else
    {
        interval[0] = interval1[0];
    }
    if (interval1[1] <= interval2[1])
    {
        interval[1] = interval2[1];
    }
    else
    {
        interval[1] = interval1[1];
    }
}

double Golden_section_method(long double z[2], long double d[2], long double interval[2])
{
    double a = interval[0];
    double b = interval[1];
    double B1 = b - T * (b - a);
    double B2 = a + T * (b - a);
    int iterations = 1;
    while(!(b - a <= 0.000001))
    {
        if (function_value((z[0] + B1 * d[0]), (z[1] + B1 * d[1])) >= function_value((z[0] + B2 * d[0]), (z[1] + B2 * d[1])))
        {
            a = B1;
            B1 = B2;
            B2 = a + T * (b - a);
        }
        else if (function_value(z[0] + B1 * d[0], z[1] + B1 * d[1]) < function_value(z[0] + B2 * d[0], z[1] + B2 * d[1]))
        {
            b = B2;
            B2 = B1;
            B1 = b - T * (b - a);
        }
        iterations++;
    }
    std::cout << "Golden section iterations: " << iterations << std::endl;
    return ((b + a) / 2);
}

void Powell_method(double z0[2], double function_interval[2])
{
    std::cout << "Searching for minimum of function at initial point [" << z0[0] << ", " << z0[1] << "] in interval <";
    std::cout << function_interval[0] << ", " << function_interval[1] << "> using Powell method" << std::endl << std::endl;
    std::cout << "Powel iteration: 1" << std::endl;
    int iterations = 1;
    long double z[2] = { z0[0],z0[1] };
    long double d[2] = { 1,0 };
    long double interval[2] = { function_interval[0],function_interval[1] };
    interval_value(z, d, interval, function_interval);
    long double alpha = Golden_section_method(z, d, interval);
    z[0] = z[0] + alpha * d[0];
    z[1] = z[1] + alpha * d[1];
    d[0] = 0;
    d[1] = 1;
    interval_value(z, d, interval, function_interval);
    alpha = Golden_section_method(z, d, interval);
    long double z2[2] = { z[0] + alpha * d[0],z[1] + alpha * d[1] };
    while (!(((z2[0] - z[0]) <= E) && ((z2[1] - z[1]) <= E)))
    {
        iterations++;
        std::cout << "Powel iteration: " << iterations << std::endl;
        z[0] = z2[0];
        z[1] = z2[1];
        d[0] = z2[0] - z0[0];
        d[1] = z2[1] - z0[1];
        interval_value(z, d, interval, function_interval);
        alpha = Golden_section_method(z, d, interval);
        z2[0] = z[0] + alpha * d[0];
        z2[1] = z[1] + alpha * d[1];
    }
    std::cout << std::endl;
    std::cout << "Minimum found at point: [" << z2[0] << ", " << z2[1] << "]" << std::endl;
}

void Conjugate_gradient_method(double z0[2], double function_interval[2])
{
    std::cout << "Searching for minimum of function at initial point [" << z0[0] << ", " << z0[1] << "] in interval <";
    std::cout  << function_interval[0] << ", " << function_interval[1] << "> using Conjugate gradient method" << std::endl << std::endl;
    std::cout << "Conjugate gradient iteration: 1" << std::endl;
    int iterations = 1;
    long double z[2] = { z0[0],z0[1] };
    long double d[2] = { -function_gradient_x(z0[0]),-function_gradient_y(z0[1]) };
    long double interval[2] = { function_interval[0],function_interval[1] };
    interval_value(z, d, interval, function_interval);
    long double alpha = Golden_section_method(z, d, interval);
    long double z2[2] = { z[0] + alpha * d[0],z[1] + alpha * d[1] };
    long double x, y, a, b, beta;
    while (!(((z2[0] - z[0]) <= E) && ((z2[1] - z[1]) <= E)))
    {
        x = function_gradient_x(z2[0]);
        y = function_gradient_y(z2[1]);
        a = function_gradient_x(z[0]);
        b = function_gradient_y(z[1]);
        beta = (x * (x - a) + y * (y - b)) / (pow(a, 2) + pow(b, 2));
        d[0] = -x + beta * d[0];
        d[1] = -y + beta * d[1];
        iterations++;
        std::cout << "Conjugate gradient iteration: " << iterations << std::endl;
        z[0] = z2[0];
        z[1] = z2[1];
        interval_value(z, d, interval, function_interval);
        alpha = Golden_section_method(z, d, interval);
        z2[0] = z[0] + alpha * d[0];
        z2[1] = z[1] + alpha * d[1];
        if (z2[0] > function_interval[1])
        {
            z2[0] = function_interval[1];
        }
        if (z2[0] < function_interval[0])
        {
            z2[0] = function_interval[0];
        }
        if (z2[1] > function_interval[1])
        {
            z2[1] = function_interval[1];
        }
        if (z2[1] < function_interval[0])
        {
            z2[1] = function_interval[0];
        }
    }
    std::cout << std::endl;
    std::cout << "Minimum found at point: [" << z2[0] << ", " << z2[1] << "]" << std::endl;
}

int main()
{
    double z0[2] = { 0,0 };
    double function_interval[2] = { -1,1 };
    Powell_method(z0, function_interval);
    std::cout << std::endl;
    Conjugate_gradient_method(z0, function_interval);
    std::cout << std::endl;
    std::cout << "###################################################################################################################" << std::endl;
    std::cout << std::endl;
    z0[0] = 0.5;
    z0[1] = 0;
    Powell_method(z0, function_interval);
    std::cout << std::endl;
    Conjugate_gradient_method(z0, function_interval);
    std::cout << std::endl;
    std::cout << "###################################################################################################################" << std::endl;
    std::cout << std::endl;
    z0[0] = -0.5;
    z0[1] = 0;
    Powell_method(z0, function_interval);
    std::cout << std::endl;
    Conjugate_gradient_method(z0, function_interval);
    std::cout << std::endl;
    std::cout << "###################################################################################################################" << std::endl;
    std::cout  << std::endl;
    z0[0] = -0.812678;
    z0[1] = 0.249999;
    Powell_method(z0, function_interval);
    std::cout << std::endl;
    Conjugate_gradient_method(z0, function_interval);
    std::cout << std::endl;
    std::cout << "###################################################################################################################" << std::endl;
    std::cout << std::endl;
    z0[0] = 0.1299;
    z0[1] = 0.589;
    Powell_method(z0, function_interval);
    std::cout << std::endl;
    Conjugate_gradient_method(z0, function_interval);
    std::cout << std::endl;
    std::cout << "###################################################################################################################" << std::endl;
    std::cout << std::endl;
    z0[0] = 0.8923;
    z0[1] = -0.5;
    Powell_method(z0, function_interval);
    std::cout << std::endl;
    Conjugate_gradient_method(z0, function_interval);
    std::cout << std::endl;
    std::cout << "###################################################################################################################" << std::endl;
    std::cout << std::endl;
    z0[0] = 0.1;
    z0[1] = -0.009;
    Powell_method(z0, function_interval);
    std::cout << std::endl;
    Conjugate_gradient_method(z0, function_interval);
    return 0;
}