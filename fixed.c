/* Attempt to compute Fibonacci numbers using ordinary fixed-precision
 * floating-point arithmetic. Of course this will give correct answers
 * only for small values of n.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>


/* Fibonacci algorithms */

double fib_pow(long n)
{
    double phi = (sqrt(5) + 1) / 2.0;
    return round(pow(phi, n) / sqrt(5));
}

typedef double (*power_f_t)(double, long);

double fib_whevs(long n, power_f_t pow_f)
{
    double phi = (sqrt(5) + 1) / 2.0;
    return round(pow_f(phi, n) / sqrt(5));
}


/* Power algorithms */

/**
 * Compute an integer power of a double using repeated
 * squaring.
 */
double pow_squaring(double d, long n)
{
    long bit, mask;
    double result = 1.0;

    /* Set 'bit' to the most-significant 1-bit of 'n' */
    bit = 1; mask = 1;
    while (n > mask) {
        bit = bit << 1;
        mask = (mask << 1) | 1;
    }

    while (bit > 0) {
        result *= result;
        if ( (n & bit) != 0 ) {
            result *= d;
        }
        bit >>= 1;
    }

    return result;
}

/**
 * Compute an integer power of a double using straightforward
 * repeated multiplication. For our purposes, this works less
 * well than the pow_squaring algorithm.
 */
double pow_slow(double d, long n)
{
    double result = 1.0;
    while (n-- > 0) result *= d;
    return result;
}


/* Main program */

int main(int argc, char **argv)
{
    long n;
    char *end_ptr;

    if (argc >= 2) {
        int i = argc - 1;

        n = strtol(argv[i], &end_ptr, 10);
        if (*(argv[i]) == 0 || *end_ptr != 0) {
            fprintf(stderr, "Usage: %s [--pow | --squaring | --slow] <n>\n", argv[0]);
            fprintf(stderr, "Input was not a number: %s\n", argv[i]);
            return EX_USAGE;
        }

        if (argc == 2 || (argc == 3 && 0 == strcmp("--pow", argv[1]))) {
            printf("fib(%ld) = %.0f\n", n, fib_pow(n));
            return 0;
        }
        else if (argc == 3 && 0 == strcmp("--squaring", argv[1])) {
            printf("fib(%ld) = %.0f\n", n, fib_whevs(n, pow_squaring));
            return 0;
        }
        else if (argc == 3 && 0 == strcmp("--slow", argv[1])) {
            printf("fib(%ld) = %.0f\n", n, fib_whevs(n, pow_slow));
            return 0;
        }
    }

    fprintf(stderr, "Usage: %s [--pow | --squaring | --slow] <n>\n", argv[0]);
    return EX_USAGE;
}
