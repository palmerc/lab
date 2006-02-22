// It wants the numbers that ARE NOT prime and don't have a prime within + or - 10 digits.
// examples:
// 3328-4100 returns 4  (eg. 3480, 3750, 3978, 4038)
// 10-1000 returns 0
// 19240-19710 returns 53
// 23659-24065 returns 20
// 97001-97691 returns 89

#include <iostream>

using namespace std;

class Primes {
   public:
      Primes();
      int farFromPrimes(int A, int B);
      bool isPrime(int A);
};

Primes::Primes() {
}

bool Primes::isPrime(int A) {
   for ( int i=2; i < A; i++ ) {
      if ( A % i == 0 ) {
         return false;
      }
   }
   return true;      
}

int Primes::farFromPrimes(int A, int B) {
   int farfrom = 0;
   int previous = 0;
   int earlier = 0;
      
   for ( int i=A; i <= B; i++ ) {
      if ( isPrime(i) ) {
         if ( (i - previous) >= 10 && (previous - earlier) >= 10 ) {
            cout << previous << endl;
            farfrom++;
         }
         earlier = previous;
         previous = i;
         
      }
   }
   return farfrom;
}

int main() {
   int A, B, C;
   Primes p;
   
   cout << "Enter a starting number: ";
   cin >> A;
   cout << "Enter an ending number: ";
   cin >> B;
   
   C = p.farFromPrimes(A, B);
   
   cout << "Found " << C << " far primes." << endl;
}
