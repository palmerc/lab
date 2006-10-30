#include <stdio.h>
#include <stdlib.h>

long factorial(int a) {
   int i, factorial = 1;

   if (a == 0) {
      return 1;
   }

   for (i=1; i <= a; i++)
      factorial *= i;

   return factorial;
}

int main() {
   int i;
   long facto;
   double euler;

   for (i=0; i < 50; i++) {
      facto = factorial(i);
      euler += (double) 1/facto;
      printf("Euler is %.60f\n\n", euler);
   }
   return 0;
}

