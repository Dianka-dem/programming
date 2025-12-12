#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdio.h>

struct complex {
    double re;
    double im;
};

struct complex createComplex(double real, double imag);
struct complex minusComplex(struct complex a, struct complex b);
struct complex multiplyComplex(struct complex a, struct complex b);
struct complex squareComplex(struct complex a);

#endif