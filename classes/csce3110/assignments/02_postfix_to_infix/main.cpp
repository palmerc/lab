#include<iostream>
#include<string>
#include "List.h"
using namespace std;
int main()
{
   List L;
   string input, temp, c;
   
   cout << "Enter a postfix expression " << endl;
   cin >> input;
   
   for (int i=0; i < input.size(); i++) {
      c = input[i];
      switch (c[0])
      {
         case('+'):
         case('-'):
         case('*'):
         case('/'):
            temp = L.pop() + input[i] + L.pop();
            L.push(temp);
            break;
         default:
            L.push(c);
      };
   }
   while (!L.isEmpty())
      cout << L.pop();
   cout << endl;
   return 0;
}
