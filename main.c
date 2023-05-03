#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double f1(double x);
double f2(double x);
double f3(double x);

int count = 0;

double find_x0(double x1, double y1, double x2, double y2){
    return (x1*(y2 - y1) - y1*(x2 - x1))/(y2 - y1);
}
double root(double (*f)(double), double (*g)(double), double a, double b, double eps){
    double y_a = f(a) - g(a);
    double y_b = f(b) - g(b);
    double x = find_x0(a, y_a, b, y_b);
    double y = f(x) - g(x);

    count++;

    double y_eps1 = f(x + eps) - g(x + eps);
    double y_eps2 = f(x - eps) - g(x - eps);
    if (y * y_eps1 <= 0 || y * y_eps2 <= 0)
        return x;
    if (y * y_a >= 0)
        return root(f, g, x, b, eps);
    if (y * y_b >= 0)
        return root(f, g, a, x, eps);
    return 0.0;
}

double integral(double (*f)(double), double a, double b, double eps){
    if (a > b)
        return integral(f, b, a, eps);
    int n = 10;
    double delta = (b - a) / n;
    double In = -1;
    double I2n = -2;
    while((3 * (fabs(In - I2n)) > eps)){
        In = I2n;
        double sum = 0;
        double xi0 = a;
        delta = delta / 2;
        for (int i = 1; i < 2*n; i++){
            double xi = a + delta*i;
            double ksii = (xi + xi0) / 2;
            double y = f(ksii);
            sum += y;
            xi0 = xi;
        }
        I2n = delta*sum;
        n = 2 * n;
    }
    return I2n;
}

int main(int argc, char** argv){

    double (*f[3]) (double) = {&f1, &f2, &f3};

    double eps1 = 0.00001;
    double a = 0.1;
    double b = 3.0;

    count = 0;
    double x1 = root(f1, f2, a, b, eps1);
    int c1 = count;
    count = 0;
    double x2 = root(f2, f3, a, b, eps1);
    int c2 = count;
    count = 0;
    double x3 = root(f3, f1, a, b, eps1);
    int c3 = count;

    double eps2 = 0.00001;
    double s1 = integral(f1, x1, x3, eps2);
    double s2 = integral(f2, x1, x2, eps2);
    double s3 = integral(f3, x2, x3, eps2);
    double s = s1 - s2 - s3;

    int i = 1;
    while (i < argc){
        if (strcmp(argv[i], "-help") == 0){
            printf(" valid command line keys:\n");
            printf(" >> -root: print all intersections\n >> -rootc: print number of iterations\n");
            printf(" >> -integral [ 1 - 3 ] [ a ] [ b ] : print the area under the graph\n    of one of the functions [ 1 - 3 ] on the segment [ a, b ]\n");
            printf(" >> -answer: print final answer of the task\n");
            printf(" functions:\n >> 1: e^(-x) + 3\n >> 2: 2x - 2\n >> 3: 1/x\n");
        }
        else if (strcmp(argv[i], "-root") == 0){
            printf(" roots:\n");
            printf(" >> f1 and f2: %f\n", x1);
            printf(" >> f2 and f3 : %f\n", x2);
            printf(" >> f3 and f1 : %f\n", x3);
        }
        else if (strcmp(argv[i], "-rootc") == 0){
            printf(" count:\n");
            printf(" >> f1 and f2: %d\n", c1);
            printf(" >> f2 and f3: %d\n", c2);
            printf(" >> f3 and f1: %d\n", c3);
        }
        else if (strcmp(argv[i], "-integral") == 0){
            int findex = argv[i + 1][0] - '0';
            float a1 = strtod(argv[i + 2], 0);
            float b1 = strtod(argv[i + 3], 0);
            i += 3;
            printf(" >> area of f%d on segment [ %lf, %lf ] : %.3lf\n", findex, a1, b1, integral(*(f[findex - 1]), a1, b1, eps2));
        }
        else if (strcmp(argv[i], "-answer") == 0){
            printf("\n answer:\n >> %.3lf\n", s);
        }
        
        i++;
    }

    return 0;
}