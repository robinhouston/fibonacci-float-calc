# Fibonacci float calc

Some simple programs to investigate what happens when you use Binet’s formula to compute Fibonacci numbers.

They were written in conjunction with [my blog post on the same subject](http://bosker.wordpress.com/2011/07/27/computing-fibonacci-numbers-using-binet’s-formula/).

Arbitrary-precision arithmetic is provided by the wonderful [GMP library](http://gmplib.org/).

## fixed.c

Naively compute Fibonacci numbers using Binet’s algorithm. Uses the C "double" type for the computation, so starts to give wrong answers somewhere in the 70s. You can select between three different algorithms for computing powers, which give differently-wrong answers. The best seems to be <code>--squaring</code>, which gives correct answers for <i>n</i> up to and including 75.

This technique is essentially useless, since it is slower and less reliable than simply looking up in a static array of the first 80 Fibonacci numbers.

## float-calc.c

This program includes a variety of algorithms:

 * `int` – the fast integer algorithm from [The worst algorithm in the world?](https://bosker.wordpress.com/2011/04/29/the-worst-algorithm-in-the-world), implemented using GMP integers.
 * `float` – the arbitrary-precision floating-point algorithm from [my blog post](http://bosker.wordpress.com/2011/07/27/computing-fibonacci-numbers-using-binet’s-formula/).
 * `builtin` – GMP’s super-optimised built-in Fibonacci routine.
 * `lucas` – another integer algorithm, this time using Lucas number identities.

There is also a 'graph' mode that will run the `int` and `float` algorithms for all the multiples of 1000 up to a million, and print the results as a table suitable for importing into a spreadsheet. This was used to produce the graphs in the blog post.

## lucas.c

Another integer algorithm using Lucas-number identities. This is a more highly optimised algorithm than the one in `float-calc.c`.
