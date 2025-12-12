#include "complex.h"

int main() {
    struct complex a = createComplex(2, 3);
    struct complex b = createComplex(-1, 4);
    struct complex c = createComplex(3, -2);

    struct complex paz = minusComplex(b, c); 
    struct complex dva = multiplyComplex(a, paz); 
    struct complex tri = squareComplex(c);    
    struct complex Q = minusComplex(dva, tri);   

    printf("\nQ = a*(b-c) - c^2 = ");
    printf("%.2f + %.2fi", Q.re, Q.im);
    return 0;
}