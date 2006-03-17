
#ifndef __PDIR__
#define __PDIR__

#include <string>

using namespace std;

enum Direction 	{ 
	NORTH, 
	NORTHEAST, 
	EAST, 
	SOUTHEAST, 
	SOUTH, 
	SOUTHWEST, 
	WEST, 
	NORTHWEST 
};

/*
string printDirection(Direction d) {
	string dir;
	switch(d) {
		case NORTH:
			dir = "north";
			break;
		case NORTHEAST:
			dir = "northeast";
			break;
		case EAST:
			dir = "east";
			break;
		case SOUTHEAST:
			dir = "southeast";
			break;
		case SOUTH:
			dir = "south";
			break;
		case SOUTHWEST:
			dir = "southwest";
			break;
		case WEST:
			dir = "west";
			break;
		case NORTHWEST:
			dir = "northwest";
			break;
		default:
			dir = "bad direction!";
			;
	};
	return dir;
}
*/
#endif
