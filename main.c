#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float f1(float x){
    return exp(-x) + 3;
}
float f2(float x){
    return 2*x - 2;
}
float f3(float x){
    float e = 0.000001;
    if (!(x < e && x > -e))
        return 1/x;
    return 1/0.000001;
}

float find_x0(float x1, float y1, float x2, float y2){
    return (x1*(y2 - y1) - y1*(x2 - x1))/(y2 - y1);
}
float root(float (*f)(float), float (*g)(float), float a, float b, float eps){
    float y_a = f(a) - g(a);                                                    // значение в точке а
    float y_b = f(b) - g(b);                                                    // значение в точке b
    float x = find_x0(a, y_a, b, y_b);                                          // находим точку пересечения хорды с осью Ох
    float y = f(x) - g(x);                                                      // находим значение функции (которая является разностью функций f и g) в этой точке

    float y_eps1 = f(x + eps) - g(x + eps);                                     // находим значения функции в eps-окрестности точки x
    float y_eps2 = f(x - eps) - g(x - eps);
    if (y * y_eps1 < 0 || y * y_eps2 < 0)                                       // если нашли корень с нужной степенью точности, выходим из функции
        return x;
    if (y * y_a >= 0)
        return root(f, g, x, b, eps);                                           // иначе рекурсивно запускаем функцию дальше, заменяя один из концов отрезка на х
    if (y * y_b >= 0)
        return root(f, g, a, x, eps);                                           // в зависимости от знака значения функции в точке х
    return 0.0;
}

float integral(float (*f)(float), float a, float b, float eps){
    if (a > b)                                                                  // необходимое условие, чтобы не проверять, в какой последовательности подавать аргументы
        return integral(f, b, a, eps);                                          
    int n = 10;                                                                 // стартовое значение n
    double delta = (b - a) / n;                                                 // стартовое значение диаметра разбиения отрезка
    double In = -1;
    double I2n = -2;                                                            // стартовые значения интеграллов (In и I2n), чтобы программа точно вошла в while
    while((3 * (fabs(In - I2n)) > eps)){                                        // используем метод "средних" прямоугольников, поэтому нам необходимо добавить константу
        In = I2n;
        double sum = 0;
        double xi0 = a;
        delta = delta / 2;                                                      // уменьшаем delta в 2 раза
        for (int i = 1; i < 2*n; i++){
            double xi = a + delta*i;
            double ksi = (xi + xi0) / 2;                                        // точка ksi находится посередине отрезка разбиения
            double y = f(ksi);
            sum += y;
            xi0 = xi;
        }
        I2n = delta*sum;                                                        // считаем интегральную сумму
        n = 2 * n;
    }
    return I2n;
}

int main(void)
{
    float eps1 = 0.00001;
    float a = 0.1;
    float b = 3.0;
    float x1 = root(f1, f2, a, b, eps1);
    float x2 = root(f2, f3, a, b, eps1);
    float x3 = root(f3, f1, a, b, eps1);

    printf("Intersection of functions:\n >> f1 f2: %.3f\n >> f2 f3: %.3f\n >> f1 f3: %.3f\n", x1, x2, x3);

    float eps2 = 0.00001;
    float s1 = integral(f1, x1, x3, eps2);
    float s2 = integral(f2, x1, x2, eps2);
    float s3 = integral(f3, x2, x3, eps2);

    printf("\nIntegrals:\n >> f1  [ %.3f ; %.3f ] : %.3f\n >> f2 [ %.3f ; %.3f ] : %.3f\n >> f3 [ %.3f ; %.3f ] : %.3f\n", x1, x3, s1, x1, x2, s2, x2, x1, s3);

    float s = s1 - s2 - s3;
    printf("\nAnswer: %.3f\n",s);
    return 0;
}

