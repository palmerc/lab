// file ctime.h
// Header file for class cTime which represents time data

#ifndef _CTIME_H_
#define _CTIME_H_

enum {TWELVE_HOUR = 12, TWENTYFOUR_HOUR = 24};

class cTime
{
  public:
    // constuctors
    cTime(int initialHour = 0, int initialMinute = 0, int initialSecond = 0);
    
    // member functions
    void SetTime(int newHour, int newMinute, int newSecond);
    // Set time to newHour, newMinute and newSecond

    void SetTime(int newHour, int newMinute);
    // Set time to newHour, newMinute and newSecond
    
    void SetTime(int newSeconds); 
    // set time as seconds since midnight
    
    void SetHour(int newHour);
    // set the hour to newHour, minute is unchanged
    
    void SetMinute(int newMinute);
    // set the minute to newMinute, hour is unchanged

    void SetSecond(int newSecond);
    // set the minute to newMinute, hour is unchanged
            
    void AddSeconds(int seconds);
    // advance the time by adding minutes to the time
    
    int Hour() const;
    // return the hour
    
    int Minute() const;
    // return the minute

    int Second() const;
    // return the second
    
    void Time(int& hour, int& minute, int& second) const;
    // return the time in hour and minute
    
    int Time() const;
    // return the time as minutes since midnight
    
    void Display(int format=TWELVE_HOUR) const;
    // display(cout) the time
    // if format is TWELVE_HOUR output as HH:MM am or pm
    // else output as HH:MM using 24 hour format
    
  private:
    long theSeconds;        // minutes since midnight
        
};      // end of class cTime

#endif // _CTIME_H_
