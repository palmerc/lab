#include <iostream>
#include "cname.h"

using namespace std;

int main()
{
   cName name;
  
   cout << "Name class program" << endl;
   while (true)
   {
      string first;
      string last;

      cout << "Enter a first and last name: ";
      cin >> first >> last;
      if (!cin) break;
         name.setName(first, last);

      cout << "First name: " << name.FirstName() << endl;
      cout << "Last name:  " << name.LastName() << endl;
      cout << "First name first: " << name.NameFNF() << endl;
      cout << "Last name first: " << name.NameLNF() << endl;

      cout << endl;
   }

   return 0;
}
