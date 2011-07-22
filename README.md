A simple program to investigate the performance of using floating-point arithmetic and Binet’s formula to compute Fibonacci numbers.

Arbitrary-precision arithmetic is provided by the wonderful [GMP library](http://gmplib.org/).

The usual behaviour is to take a number 'n' on the command-line, to compute the n’th Fibonacci number using two different methods, and to report how long each took. There is also a 'graph' mode that will run the computation for all the multiples of 1000 up to a million, and print the results as a table suitable for importing into a spreadsheet.

