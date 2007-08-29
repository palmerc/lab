// file cTime.cpp
// Implementation file for class cTime which 
// represents time data

// ---------------------------------------------- headers

#include "ctime.h"

#include <iostream>
using namespace std;

// ------------------------------------------- constuctors

cTime::cTime(int initialHour, int initialMinute, int initialSecond) :
       theSeconds((initialHour >= 0 && initialHour < 24 
           && initialMinute >= 0 && initialMinute < 60
	   && initialSecond >= 0 && initialSecond < 60) 
           ? initialHour * 60 * 60 + initialMinute * 60 + initialSecond: 0 )
{
}

// -------------------------------------- member functions

// ----------------------------------------------- SetTime

// Set time to newHour and newMinute and newSecond
void cTime::SetTime(int newHour, int newMinute, int newSecond)
{
  if (newHour >= 0 && 
      newHour < 24 &&
      newMinute >= 0 &&
      newMinute < 60 &&
      newSecond >= 0 &&
      newSecond < 60)
      theSeconds = newHour * 60 * 60 + newMinute * 60 + newSecond;
}

// Set time to newHour and newMinute
void cTime::SetTime(int newHour, int newMinute)
{
  if (newHour >= 0 && 
      newHour < 24 &&
      newMinute >= 0 &&
      newMinute < 60)
      theSeconds = newHour * 60 * 60 + newMinute * 60;
}

// set time as seconds since midnight
void cTime::SetTime(int newSecond)
{
  if (newSecond >= 0 &&
     newSecond < 24*60*60)
     theSeconds = newSecond;
} 

// ----------------------------------------------- SetHour
// set the hour to newHour, minute is unchanged
void cTime::SetHour(int newHour)
{
  int oldHour;
  
  if (newHour >= 0 && newHour < 24)
  {
    oldHour = theSeconds / 60 / 60;
    theSeconds += 60 * 60 * (newHour - oldHour);
  }
}

// -------------------------------------------- SetMinutes
// set the minute to newMinute, hour is unchanged
void cTime::SetMinute(int newMinute)
{
  int oldMinute;
  
  if (newMinute >= 0 &&
      newMinute < 60)
  {
    oldMinute = theSeconds / 60;
    theSeconds += 60 * (newMinute - oldMinute);
  }
}

// -------------------------------------------- SetSeconds
// set the second to newSecond, hour is unchanged
void cTime::SetSecond(int newSecond)
{
  int oldSecond;
  
  if (newSecond >= 0 &&
      newSecond < 60)
  {
    oldSecond = theSeconds % 60;
    theSeconds += newSecond - oldSecond;
  }
}
        
// -------------------------------------------- AddSeconds
// advance the time by adding seconds to the time
void cTime::AddSeconds(int seconds)
{
  theSeconds = (seconds + theSeconds) % (60*60*24);  
}

// -------------------------------------------------- Hour
// return the hour
int cTime::Hour() const
{
  int hour;
   
  hour = theSeconds / (60 * 60);
  return hour;
}

// ------------------------------------------------ Minute
// return the minute
int cTime::Minute() const
{
  int minutes = theSeconds / 60 % 60;
  return minutes;
}

// ------------------------------------------------ Second
// return the second
int cTime::Second() const
{
  int seconds = theSeconds % 60;
  return seconds;
}

// -------------------------------------------------- Time

// return the time in hour, minute and seconds
void cTime::Time(int& hour, int& minutes, int& seconds) const
{
  hour = Hour();
  minutes = Minute();
  seconds = Second();
}

// return the time as seconds since midnight
int cTime::Time() const
{
  return theSeconds;
}

// ----------------------------------------------- Display
// display(cout) the time
// if format is TWELVE_HOUR output as HH:MM am or pm
// else output as HH:MM using 24 hour format

void cTime::Display(int format) const
{
  bool am;
  int theHour = Hour();
  int theMinute = Minute();
  int theSecond = Second();
  
  // display the hour
  if (format == TWELVE_HOUR)
  {
    am = theHour < 12;
    
    if (theHour == 0 || theHour == 12)
      cout << 12;
    else if (am)
      cout << theHour;
    else
      cout << theHour - 12;
  }
  else
  {
    if (theHour < 10)
      cout << "0";
    cout << theHour;
  }
  
  // display the minute
  cout << ":";
  if (theMinute < 10)
    cout << "0";
  cout << theMinute;
  
  // display the second
  cout << ":";
  if (theSecond < 10)
    cout << "0";
  cout << theSecond;

  if (format == TWELVE_HOUR)
    cout << (am ? "am" : "pm");
}

// end of file cTime.cpp

