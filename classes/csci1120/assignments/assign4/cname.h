#include <string>
class cName
{
 public:
   cName();                    //Constructor

   void setName(string newFirstName, string newLastName);
   string LastName() const;    // return Last Name
   string FirstName() const;   // return First Name
   string NameFNF() const;     // return Name in First Name First format
   string NameLNF() const;     // return Name in Last Name First format
     
 private:
   string firstName;
   string lastName;
};
