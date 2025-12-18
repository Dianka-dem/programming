#include "complex.h"

int main() {
   int N=3;
   Complex* numbers = (Complex*)malloc(N * sizeof(Complex));

   if (numbers == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
   }

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

   free(numbers);
    numbers = NULL;

   return 0;
} 