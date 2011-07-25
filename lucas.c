#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include <gmp.h>


static mpz_t temp;

void fib_luc(mpz_t *fib_result, mpz_t *luc_result, long n)
{
    if (n == 0) {
        mpz_set_ui(*fib_result, 0);
        mpz_set_ui(*luc_result, 2);
        return;
    }

    fib_luc(fib_result, luc_result, n / 2);
    if (n % 2 == 0) {
        mpz_mul(temp, *fib_result, *fib_result);
        mpz_mul(*fib_result, *fib_result, *luc_result);
        mpz_mul_ui(*luc_result, temp, 5);
        if ((n & 2) == 0)
            mpz_add_ui(*luc_result, *luc_result, 2);
        else
            mpz_sub_ui(*luc_result, *luc_result, 2);
    } else {
        mpz_mul(temp, *fib_result, *luc_result);

        mpz_add(*luc_result, *luc_result, *fib_result);
        mpz_divexact_ui(*luc_result, *luc_result, 2);
        mpz_mul(*luc_result, *luc_result, *fib_result);
        mpz_mul_ui(*luc_result, *luc_result, 5);
        if ((n & 2) == 0)
            mpz_add_ui(*luc_result, *luc_result, 1);
        else
            mpz_sub_ui(*luc_result, *luc_result, 1);

        mpz_set(*fib_result, *luc_result);
        mpz_submul_ui(*fib_result, temp, 2);
    }
}

void fib(mpz_t *result, long n)
{
    mpz_t luc;

    mpz_init(luc); mpz_init(temp);
    fib_luc(result, &luc, n/2);
    if (n % 2 != 0) {
        mpz_addmul_ui(luc, *result, 5);
        mpz_divexact_ui(luc, luc, 2);
        mpz_mul(*result, *result, luc);
        if ((n & 2) == 0)
            mpz_add_ui(*result, *result, 1);
        else
            mpz_sub_ui(*result, *result, 1);
    } else {
        mpz_mul(*result, *result, luc);
    }
    mpz_clear(luc); mpz_clear(temp);
}

int main(int argc, char **argv)
{
    long n;
    char *end_ptr;
    mpz_t fib_n;

    if (argc == 2) {
        n = strtol(argv[1], &end_ptr, 10);
        if (*(argv[1]) == 0 || *end_ptr != 0) {
            fprintf(stderr, "%s: Input is not a number: %s\n", argv[0], argv[1]);
            goto usage;
        }

        mpz_init(fib_n);
        fib(&fib_n, n);
        gmp_printf("fib(%ld) = %Zd\n", n, fib_n);
        mpz_clear(fib_n);
        return 0;
    }

    usage:
    fprintf(stderr, "Usage: %s <n>\n", argv[0]);
    return EX_USAGE;
}
