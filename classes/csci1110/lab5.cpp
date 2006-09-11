#include <iostream>
#include <iomanip>

using namespace std;


int main () {

  int toDay = 1, dayCount = 1;
  int numberDays, startDay;
  bool firstRun = true;
 
  // Get input and check for validity
  cout << "Enter the day of the week the month starts on (Sun=1...Sat=7): ";
  cin >> startDay;
  if ( (startDay < 1) || (startDay > 7) ) {
    cout << "Invalid day of the week" << endl;
    return 1;
  }
  cout << "Enter the number of days in the month (28-31): ";
  cin >> numberDays;
  if ( (numberDays < 28) || (numberDays > 31) ) {
    cout << "Invalid number of days" << endl;
    return 1;
  }

  // Print header
  cout << " SUN MON TUE WED THU FRI SAT" << endl;
  cout << "----------------------------" << endl;

  // Loop until number of days in month is reached
  while ( toDay <= numberDays ) {
    // Check for padding needs
    if ( firstRun ) {
      while ( dayCount < startDay ) {
	cout << setw(4) << ' ';
        dayCount++;
      }
      firstRun = false;
    }
    // Print the numeric day of the week
    cout << setw(4) << toDay;
    // Add an end of line when the end of the week is reached
    if ( dayCount == 7 ) {
      cout << endl;
      dayCount = 1;
    } else {
      dayCount++;
    }
    toDay++;
  }
  cout << endl;

  return 0;

}
