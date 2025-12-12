struct complex {
    double re;
    double im;
};

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