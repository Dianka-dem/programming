#include <stdio.h>

typedef struct {
    double re;
    double im;
} Complex;

Complex create(double real, double imag) {
    Complex num;
    num.re = real;
    num.im = imag;
    return num;
}

Complex minus(Complex a, Complex b) {
    Complex deduct;
    deduct.re = a.re - b.re;
    deduct.im = a.im - b.im;
    return deduct;
}

 Complex multiply(Complex a, Complex b) {
    Complex multiply;
    multiply.re = a.re * b.re - a.im * b.im;
    multiply.im = a.re * b.im + a.im * b.re;
    return multiply;
}

Complex square(Complex a) { 
    return multiply(a, a);
}

int main() {
   int N=3;
   Complex numbers [N];

   numbers[0]=create(2,3);
   numbers[1]=create(-1,4);
   numbers[2]=create(3,-2);

   for (int i=0; i<N; i++){
    printf("numbers[%d] = ", i);
    print_complex(numbers[i]);
    printf("\n");
   }
   
   Complex one = minus(numbers[1], numbers[2]);
   Complex two = multiply(numbers[0], one);
   Complex three = square(numbers[2]);
   Complex Q = minus(two,three);

   printf(" Q = a*(b-c) - c² = ");
   print_complex(two);
   printf(" - ");
   print_complex(three);
   printf(" = ");
   print_complex(Q);
   printf("\n");;
   return 0;
} 