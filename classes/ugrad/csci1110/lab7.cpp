#include <iostream>
#include <iomanip>

using namespace std;

const float PINE = 0.89;
const float FIR = 1.09;
const float CEDAR = 2.26;
const float MAPLE = 4.50;
const float OAK = 3.10;
const float TWELVEINCHES = 12.0;

int main () {
   char wood_type;
   string xwood_type;
   int pieces, height, width, length;
   float wood_cost = 0, subtotal = 0, bored_feet = 0;
   bool looper = true;

   while ( looper ) {  
      cout << "Enter item: ";
      cin >> wood_type; 
      // Loop until you receive a T for total
      if ( wood_type == 'T' ) {
         looper = false;
         break;
      } else {
         cin >> pieces >> width >> height >> length;
         bored_feet = pieces * ((width * height * length) / TWELVEINCHES); 
         // Check for type of wood and multiply board feet by price
         switch ( wood_type ) {
            case 'P':
               wood_cost = PINE * bored_feet;
               xwood_type = " Pine";
               break;
            case 'F':
               wood_cost = FIR * bored_feet;
               xwood_type = " Fir";
               break;
            case 'C':
               wood_cost = CEDAR * bored_feet;
               xwood_type = " Cedar";
               break;
            case 'M':
               wood_cost = MAPLE * bored_feet;
               xwood_type = " Maple";
               break;
            case 'O':
               wood_cost = OAK * bored_feet;
               xwood_type = " Oak";
               break;
         }
         cout << pieces << " ";
         cout << width << "X" << height << "X" << length << xwood_type;
         // Fix the decimal position to two places
         cout << setiosflags(ios::fixed) << setprecision(2) << ", cost: $" << wood_cost << endl; 
         subtotal += wood_cost;
      }
   }
   cout << "Total cost: $" << subtotal << endl; 

   return 0;
}
