#include<iostream>

using namespace std;

int main() {

  float multiplier, weight;
  string planet;

 cout << "Enter your weight and the name of a planet. (e.g. 150 earth): ";
 cin >> weight >> planet;

 if ( !(weight > 0) ) {
   cout << "Not a valid weight. Please reenter a weight greater than 0" << endl;
   return 1;
 }
  
 if ( planet == "mercury" || planet == "Mercury" )
    multiplier = 0.4155;
 else if ( planet == "venus" || planet == "Venus" )
    multiplier = 0.8975;
 else if ( planet == "earth" || planet == "Earth" )
    multiplier = 1.0;
 else if ( planet == "moon" || planet == "Moon" )
    multiplier = 0.166;
 else if ( planet == "mars" || planet == "Mars" )
    multiplier = 0.3507;
 else if ( planet == "jupiter" || planet == "Jupiter" )
    multiplier = 2.5374;
 else if ( planet == "saturn" || planet == "Saturn" )
    multiplier = 1.0677;
 else if ( planet == "uranus" || planet == "Uranus" )
    multiplier = 0.8947;
 else if ( planet == "neptune" || planet == "Neptune" )
    multiplier = 1.1794;
 else if ( planet == "pluto" || planet == "Pluto" )
    multiplier = 0.0899;
 else {
   cout << "Planet " << planet << " invalid. Please reenter in the form <weight planet_name>" << endl;
   return 1;
 }

 cout << "Your weight on " << planet << " is " << weight * multiplier << " pounds" << endl;
  
 return 0;
  
}
