#include "cname.h"
#include <string>

   cName::cName()                    //Constructor
   {
   }

   void cName::setName(string newFirstName, string newLastName)
   {
      firstName = newFirstName;
      lastName = newLastName;
   }

   string cName::LastName() const    // return Last Name
   {
      return lastName;
   }

   string cName::FirstName() const   // return First Name
   {
      return firstName;
   }

   string cName::NameFNF() const     // return Name in First Name First format
   {
      string nameFNF = firstName + " " + lastName;
      return nameFNF; 
   }

   string cName::NameLNF() const     // return Name in Last Name First format
   {
      string nameLNF = lastName + ", " + firstName;
      return nameLNF;
   }
