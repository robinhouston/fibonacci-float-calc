# Fibonacci float calc

Two simple programs to investigate what happens when you use floating-point arithmetic and Binet’s formula to compute Fibonacci numbers.

## float-calc.c

The usual behaviour is to take a number 'n' on the command-line, to compute the n’th Fibonacci number using two different methods, and to report how long each took. There is also a 'graph' mode that will run the computation for all the multiples of 1000 up to a million, and print the results as a table suitable for importing into a spreadsheet. And you can ask it to use just one of the algorithms and print the answer by running <code>./float-calc 100 int</code> or <code>./float-calc 100 float</code>, et cetera.

Arbitrary-precision arithmetic is provided by the wonderful [GMP library](http://gmplib.org/).


## fixed.c

Uses the C "double" type for the computation, so starts to give wrong answers somewhere in the 70s. You can select between three different algorithms for computing powers, which give differently-wrong answers. The best seems to be <code>--squaring</code>, which gives correct answers for <i>n</i> up to and including 75.
