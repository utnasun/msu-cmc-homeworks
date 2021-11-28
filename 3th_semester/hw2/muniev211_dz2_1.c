#include <stdio.h>

int main() {
    float eps;
    double x, t0, t1;
    printf("Enter eps = ");
    scanf("%f", & eps);
    printf("Enter x = ");
    scanf("%lf", & x);
    while (getchar() != EOF) {
        t0 = 0;
        t1 = 1;
        do {
            t0 = t1;
            t1 = (t0 + x / t0) / 2;
        } while (((t0 - t1) >= eps) || ((t0 - t1) <= -eps));
        printf("Root from x:\n");
        printf("%.10g\n", t1);
        printf("%f\n", t1);
        printf("%g\n", t1);
        printf("%e\n", t1);
        printf("Enter x = ");
        scanf("%lf", & x);
    }
    return 0;
}
