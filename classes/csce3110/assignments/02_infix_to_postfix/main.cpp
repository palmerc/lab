/*
Cameron L Palmer
Problem 6 - Infix to Postfix Calculator

Instructions:
To compile: g++ -g -o i2p main.cpp
To run: ./i2p
*/

// main.cpp

#include<iostream>
#include<string>
#include "List.h"
using namespace std;
int main()
{
   List L;
   string input, output;
   char temp;
   
   cout << "Enter a infix expression " << endl;
   cin >> input;
   
   for (int i=0; i < input.size(); i++) {
      switch (input[i])
      {
         case('('):
            L.push(input[i]);
            break;
         case(')'):
            while (L.showTail() != '(')
               cout << L.pop();
            L.pop();
            break;
         case('+'):
         case('-'):
            if (L.showTail() == '*' || L.showTail() == '/')
            {
               temp = L.pop();
               while (!L.isEmpty() && 
                     (L.showTail() != '(') &&
                     (L.showTail() != ')') &&
                     (temp != '+' || temp != '-'))
               {
                  cout << temp;
                  temp = L.pop();
               }
               cout << temp;
            }
            L.push(input[i]);
            break;
         case('*'):
         case('/'):
            if (L.showTail() == '^')
               {
               temp = L.pop();
               while (!L.isEmpty() && 
                     (L.showTail() != '(') &&
                     (L.showTail() != ')') &&
                     (temp != '*' || temp != '/'))
               {
                  cout << temp;
                  temp = L.pop();
               }
               cout << temp;
            }
            L.push(input[i]);
            break;
         case('^'):
            L.push(input[i]);
            break;
         default:
            cout << input[i];
      };
   }
   while (!L.isEmpty())
      cout << L.pop();
   cout << endl;
   return 0;
}
