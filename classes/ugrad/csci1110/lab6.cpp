#include <iostream>

using namespace std;

const int TILES_PER_BOX = 20;

// Calculate the number of tiles required for the room
int roomfunc( int tilesize ) {
   int numtiles = 0, wfeet = 0, winches = 0, lfeet = 0, linches = 0;
   int wtotal, ltotal, wtiles, ltiles;

   cout << "Enter room width (feet and inches, seperated by a space): ";
   cin >> wfeet >> winches;
   // Convert the input to inches
   wtotal = (wfeet * 12) + winches;
   // Divide width total inches by tilesize
   wtiles = wtotal / tilesize;
   // Check if you need one more tile to finish the job
   if ( (wtotal % tilesize) > 0 )
      wtiles++;

   cout << "Enter room length (feet and inches, seperated by a space): ";
   cin >> lfeet >> linches;
   // Convert the input to inches
   ltotal = (lfeet * 12) + linches;
   // Divide total inches by tilesize
   ltiles = ltotal / tilesize;
   // Check if you need one more tile to finish the job
   if ( (ltotal % tilesize) > 0 )
      ltiles++;

   numtiles = (ltiles * wtiles);

   cout << "Room requires " << numtiles << " tiles." << endl;

   return numtiles;
}

int main () {
   int loopcount = 0, 
       roomcount = 0, 
       tilesize = 0, 
       totaltiles = 0, 
       numboxes = 0,
       extratiles = 0;

   cout << "Enter the number of rooms: ";
   cin >> roomcount;
   cout << "Enter the size of tile in inches: ";
   cin >> tilesize;
   // Loop once for each room
   while ( loopcount < roomcount ) {
      totaltiles += roomfunc( tilesize );
      loopcount++;
   }

   cout << "Total tiles required is " << totaltiles << endl;
   numboxes =  totaltiles / TILES_PER_BOX;
   if ( totaltiles % TILES_PER_BOX > 0 )
      numboxes++;
   cout << "Number of boxes needed is " << numboxes << endl;
   extratiles = (numboxes * TILES_PER_BOX) - totaltiles;
   cout << "There will be " << extratiles << " extra tiles." << endl;

   return 0;
}
