#include <iostream>

using namespace std;

// Function for coverting numbers to words
string converter(int numberIn) {
   switch ( numberIn ) {
      case 0:
         return "Zero";
         break;
      case 1:
         return "One";
         break;
      case 2:
         return "Two";
         break;
      case 3:
         return "Three";
         break;
      case 4:
         return "Four";
         break;
      case 5:
         return "Five";
         break;
      case 6:
         return "Six";
         break;
      case 7:
         return "Seven";
         break;
      case 8:
         return "Eight";
         break;
      case 9:
         return "Nine";
         break;
      case 10:
         return "Ten";
         break;
      case 11:
         return "Eleven";
         break;
      case 12:
         return "Twelve";
         break;
      case 13:
         return "Thirteen";
         break;
      case 14:
         return "Fourteen";
         break;
      case 15:
         return "Fifteen";
         break;
      case 16:
         return "Sixteen";
         break;
      case 17:
         return "Seventeen";
         break;
      case 18:
         return "Eighteen";
         break;
      case 19:
         return "Nineteen";
         break;
      case 20:
         return "Twenty";
         break;
      case 30:
         return "Thirty";
         break;
      case 40:
         return "Forty";
         break;
      case 50:
         return "Fifty";
         break;
   } 
}
   
int main() {
   int hour = 0, minutes = 0, remainder = 0;
   string hour_string, minutes_string, ampm_string;

   cout << "Enter time: ";
   cin >> hour >> minutes; 

   // Exit program if invalid input entered
   if ( hour < 0 || hour > 23 ) {
      cout << "Hours must be between 0 and 23" << endl;
      return 1;
   }
   if ( minutes < 0 || minutes > 59 ) {
      cout << "Minutes must be between 0 and 59" << endl;
      return 1;
   }

   // Check for Midnight and Noon special cases
   if ( hour == 0 && minutes == 0 ) {
      cout << "Midnight" << endl;
      return 0;
   }
   if ( hour == 12 && minutes == 0 ) {
      cout << "Noon" << endl;
      return 0;
   }

   // Convert hour to 12 hour clock and add AM/PM 
   if ( hour < 13 ) {
      hour_string = converter( hour );   
      ampm_string = "AM";
   } else {
      hour_string = converter( hour - 12 );   
      ampm_string = "PM";
   }
 
   // Convert minutes
   if ( minutes >= 0 && minutes <= 20 ) {
      minutes_string = converter( minutes );
   } else if ( minutes >= 21 && minutes <= 29 ) {
      remainder = minutes % 20;
      minutes_string = converter( minutes - remainder ) + " " + converter( remainder ); 
   } else if ( minutes == 30 ) {
      minutes_string = converter( minutes );
   } else if ( minutes >= 31 && minutes <= 39 ) {
      remainder = minutes % 30;
      minutes_string = converter( minutes - remainder ) + " " + converter( remainder ); 
   } else if ( minutes == 40 ) {
      minutes_string = converter( minutes );
   } else if ( minutes >= 41 && minutes <= 49 ) {
      remainder = minutes % 40;
      minutes_string = converter( minutes - remainder ) + " " + converter( remainder ); 
   } else if ( minutes == 50 ) {
      minutes_string = converter( minutes );
   } else if ( minutes >= 51 && minutes <= 59 ) {
      remainder = minutes % 50;
      minutes_string = converter( minutes - remainder ) + " " + converter( remainder ); 
   }
      
   cout << hour_string << " " << minutes_string << " " << ampm_string << endl;

   return 0;
}
