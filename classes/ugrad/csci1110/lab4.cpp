#include <iostream>
#include <fstream>

using namespace std;

int main() {

   ifstream inFile;
   
   int inputInt;
   int evenCount = 0, oddCount = 0, totalCount = 0;
   int lowestNum, highestNum;

   inFile.open("lab4.dat");
   
   inFile >> inputInt;
   
   while ( inFile ) {
     cout << inputInt << endl;

     if ( (inputInt % 2) == 0 )
       evenCount++;
     else
       oddCount++;

     if ( lowestNum > inputInt )
       lowestNum = inputInt;

     if ( highestNum < inputInt )
       highestNum = inputInt;
     totalCount++;
     
     inFile >> inputInt;     
   }

   cout << "The number of values is " << totalCount << endl;
   cout << "The number of even values is " << evenCount << endl;
   cout << "The number of odd values is " << oddCount << endl;
   cout << "The highest number is " << highestNum << endl;
   cout << "The lowest number is " << lowestNum << endl;
}
