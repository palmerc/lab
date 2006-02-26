#include <stdio.h>

int factorial(int a) {
   int i, factorial = 1;

   if (a == 0) {
      return 1;
   }

   for (i=1; i <= a; i++)
      factorial *= i;

   return factorial;
}

int main() {
   int i, facto;
   double euler;

   for (i=0; i < 20; i++) {
      facto = factorial(i);
      euler += (double) 1/facto;
   }

   printf("Euler is %f\n\n", euler);

}
