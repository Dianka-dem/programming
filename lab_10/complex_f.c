#include "complex.h"

struct complex createComplex(double real, double imag) {
    struct complex num;
    num.re = real;
    num.im = imag;
    return num;
}

struct complex minusComplex(struct complex a, struct complex b) {
    struct complex deduct;
    deduct.re = a.re - b.re;
    deduct.im = a.im - b.im;
    return deduct;
}

struct complex multiplyComplex(struct complex a, struct complex b) {
    struct complex multiply;
    multiply.re = a.re * b.re - a.im * b.im;
    multiply.im = a.re * b.im + a.im * b.re;
    return multiply;
}

struct complex squareComplex(struct complex a) { 
    return multiplyComplex(a, a);
}
