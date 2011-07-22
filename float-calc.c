#include <sysexits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <gmp.h>


/**
 * Compute the nth Fibonacci number with arbitrary-precision
 * floating-point arithmetic, using the formula fib(n) ≈ phi ^ n / sqrt(5)
 * (which is exact if you round the rhs to the nearest whole number).
 */
void fib_float(mpf_t *result, long n)
{
    mpf_t sqrt5, phi;
    mp_bitcnt_t bitcnt;

    /* We need about n lg(phi) bits of precision */
    bitcnt = n * 7 / 10;

    /* Initialise sqrt5 to the square root of 5 */
    mpf_init2(sqrt5, bitcnt);
    mpf_sqrt_ui(sqrt5, 5);

    /* Initialise phi to the Golden Ratio */
    mpf_init2(phi, bitcnt);
    mpf_set(phi, sqrt5);
    mpf_add_ui(phi, phi, 1);
    mpf_div_2exp(phi, phi, 1);

    /* Compute phi ^ n / sqrt5 */
    mpf_init2(*result, bitcnt);
    mpf_pow_ui(*result, phi, n);
    mpf_div(*result, *result, sqrt5);

    /* Dispose of the temporary variables */
    mpf_clear(sqrt5);
    mpf_clear(phi);
}

/**
 * Compute the nth Fibonacci number using integer arithmetic.
 *
 * This is not as fast as GMP’s built-in mpz_fib_ui routine,
 * rather it is a direct reimplementation of the fib_fast algorithm
 * from http://bosker.wordpress.com/2011/04/29/the-worst-algorithm-in-the-world/
 *
 * 
 */
void fib_int(mpz_t *result, long n)
{
    mpz_t a, b, c;
    long bit, mask;

    /* Set 'bit' to the most-significant 1-bit of 'n' */
    bit = 1; mask = 1;
    while (n > mask) {
        bit = bit << 1;
        mask = (mask << 1) | 1;
    }

    mpz_init_set_ui(a, 1);
    mpz_init_set_ui(b, 0);
    mpz_init_set_ui(c, 1);

    while (bit > 0) {
        if ( (n & bit) != 0 ) {
            /*  a, b = (a + c) * b, (b * b) + (c * c)  */
            mpz_add(a, a, c);
            mpz_mul(a, a, b);

            mpz_mul(b, b, b);
            mpz_addmul(b, c, c);
        }
        else {
            /*  a, b = (a * a) + (b * b), b * (c + a)  */
            mpz_add(c, c, a); /* Temporarily set c := c + a */

            mpz_mul(a, a, a);
            mpz_addmul(a, b, b);

            mpz_mul(b, b, c);
        }

        mpz_add(c, a, b);
        bit >>= 1;
    }

    mpz_init_set(*result, b);
    mpz_clear(a); mpz_clear(b); mpz_clear(c);
}

bool compute_and_compare(long n, clock_t *int_ticks, clock_t *float_ticks)
{
    mpz_t int_result;
    mpf_t float_result;
    clock_t t1, t2, t3;
    char *float_str, *int_str;
    int float_strlen, int_strlen;
    bool results_match;

    t1 = clock();

    /* Compute fib(n) using the integer method,
       and write the result to a string. */
    fib_int(&int_result, n);
    int_strlen = gmp_asprintf(&int_str, "%Zd", int_result);
    mpz_clear(int_result);

    t2 = clock();

    /* Compute fib(n) using the floating point method,
       and write the result to a string. */
    fib_float(&float_result, n);
    float_strlen = gmp_asprintf(&float_str, "%.0Ff", float_result);
    mpf_clear(float_result);

    t3 = clock();
    
    /* Sanity check: did the two methods give the same answer? */
    results_match = (
        (float_strlen == int_strlen) &&
        (strncmp(float_str, int_str, int_strlen) == 0)
    );

    /* Free the strings, which we no longer need. */
    free(float_str);
    free(int_str);

    /* Set the timer result variables */
    if (int_ticks)   *int_ticks   = t2 - t1;
    if (float_ticks) *float_ticks = t3 - t2;

    /* Return a flag indicating whether the results matched. */
    return results_match;
}

/**
 * Compute fib(n) by the two methods, and print how long each of them took.
 *
 * The return value is intended to be used as an exit status: it’s 0 if the
 * two methods gave the same result, and EX_SOFTWARE otherwise.
 */
int compute_both_ways(char *executable_name, long n)
{
    clock_t int_ticks, float_ticks;
    bool results_match;

    printf("Computing fib(%ld) in two different ways.\n", n);
    results_match = compute_and_compare(n, &int_ticks, &float_ticks);

    if (!results_match) {
        fprintf(stderr, "%s: different methods gave different results for fib(%ld)\n\n",
            executable_name, n);
        return EX_SOFTWARE;
    }

    printf("Integer computation took %lu ticks\nFloat computation took %lu ticks\n(at a rate of %d ticks per second)\n\n", int_ticks, float_ticks, CLOCKS_PER_SEC);

    return 0;
}

/**
 * Compute a range of Fibonacci numbers by both methods,
 * and print the table of results to stdout in tab-separated form.
 */
int graph(char *executable_name)
{
	long n;

	printf("n\tint\tfloat\n");

	for(n = 1000; n <= 1000000; n += 1000) {
		clock_t int_ticks, float_ticks;
		bool results_match;

	    results_match = compute_and_compare(n, &int_ticks, &float_ticks);

	    if (!results_match) {
	        fprintf(stderr, "%s: different methods gave different results for fib(%ld)\n\n",
	            executable_name, n);
	        return EX_SOFTWARE;
	    }

	    printf("%ld\t%lu\t%lu\n", n, int_ticks, float_ticks);
	}

	return 0;
}

/**
 * Compute the nth Fibonacci number using both the floating-point
 * method and the integer algorithm, and print the relative timings.
 *
 * Also check both methods give the same result, just to be sure.
 *
 * (As expected, this shows that the integer method is much faster.)
 */
int main(int argc, char **argv)
{
    long n;
    char *end_ptr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s (<n> | graph)\n", argv[0]);
        return EX_USAGE;
    }

    if (0 == strcmp("graph", argv[1])) {
        return graph(argv[0]);
    }
    else {
        n = strtol(argv[1], &end_ptr, 10);
        if (*(argv[1]) == 0 || *end_ptr != 0) {
            fprintf(stderr, "Usage: %s (<n> | graph)\n", argv[0]);
            fprintf(stderr, "Input was not a number\n");
            return EX_USAGE;
        }

        return compute_both_ways(argv[0], n);
    }
}
