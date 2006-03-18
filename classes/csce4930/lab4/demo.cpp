#include "includes.h"

Direction bubbleSort(unsigned long North, unsigned long East, unsigned long South, unsigned long West) {
    // Code that bubble sorts the directions and chooses the easiest path.
    unsigned long directions[4];
    unsigned long temp;
    int i, j;
    
    directions[0] = North;
    directions[1] = East;
    directions[2] = South;
    directions[3] = West;
    cout << North << ' ' << East << ' ' << South << ' ' << West << endl;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < (3 - i); j++) {
            if (directions[j] > directions[j+1]) {
                temp = directions[j];
                directions[j] = directions[j+1];
                directions[j+1] = temp;
            }
        }
    }
    cout << directions[0] << ' ' << directions[1] << ' ' << directions[2] << ' ' << directions[3] << endl;
    
    for (i = 0; i < 4; i++) {
        if ( directions[i] == 0 )
            continue;
        cout << directions[i] << endl;
        if ( (North == directions[i]) && (directions[i] != 0) )
            return NORTH;
        else if ( (East == directions[i]) && (directions[i] != 0) )
            return EAST;
        else if ( (South == directions[i]) && (directions[i] != 0) )
            return SOUTH;
        else if ( (West == directions[i]) && (directions[i] != 0) )
            return WEST;
    }
}

Direction directPath(Position& me, Position& theGoal) {
    unsigned long cur_x = me.getx();
    unsigned long cur_y = me.gety();
    unsigned long goal_x = theGoal.getx();
    unsigned long goal_y = theGoal.gety();
    
    // The easiest solution is move toward the goal each time without concern about
    // distance. Of course this assumes that there are no zeros on the playing field.
    if (goal_x > cur_x)
        return EAST;
    if (goal_x < cur_x)
        return WEST;
    if (goal_y > cur_y)
        return SOUTH;
    if (goal_y < cur_y)
        return NORTH;
}

// This is the function the students must fill out.
// They are given their location and the location of the goal.
// The default behavior is to return SOUTH.  This is nonsense.

Direction decide(Position& me, Position& theGoal, unsigned long North, unsigned long East, unsigned long South, unsigned long West) {
    unsigned long cur_x = me.getx();
    unsigned long cur_y = me.gety();
    unsigned long goal_x = theGoal.getx();
    unsigned long goal_y = theGoal.gety();
    
    // Code that bubble sorts the directions and chooses the easiest path.
    unsigned long directions[4];
    unsigned long temp, dir_x, dir_y;
    int i, j;
    
    directions[0] = North;
    directions[1] = East;
    directions[2] = South;
    directions[3] = West;
    //cout << North << ' ' << East << ' ' << South << ' ' << West << endl;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < (3 - i); j++) {
            if (directions[j] > directions[j+1]) {
                temp = directions[j];
                directions[j] = directions[j+1];
                directions[j+1] = temp;
            }
        }
    }
        
    me.print();
    theGoal.print();
    // Which is the first direction that is non-zero?
    i = 0;
    j = 0;
    while ( j == 0 ) {
        if ( directions[i] == 0 )
            i++;
        else
            j = 1;
    }
    int first = i;
    
    // Which directions are toward the goal?
    if (goal_x > cur_x)
        dir_x = East;
    else if (goal_x < cur_x)
        dir_x = West;
    else
        dir_x = 0;
    cout << "dir_x: " << dir_x << endl;
    if (goal_y > cur_y)
        dir_y = South;
    else if (goal_y < cur_y)
        dir_y = North;
    else
        dir_y = 0;
    cout << "dir_y: " << dir_y << endl;
    
    if ( (dir_x <= dir_y) && (dir_x != 0) ) {
        if ( East == dir_x ) {
            cout << "Go EAST" << endl;
            return EAST;
        } else if ( West == dir_x ) {
            cout << "Go WEST" << endl;
            return WEST;
        }
    } else if ( (dir_y < dir_x) && (dir_y != 0) ) {
        if ( North == dir_y ) {
            cout << "Go NORTH" << endl;
            return NORTH;
        } else if ( South == dir_y ) {
            cout << "Go SOUTH" << endl;
            return SOUTH;
        }
    } else if ( (dir_x == 0 && dir_y > 0) || (dir_y == 0 && dir_x > 0) ) {
        if (dir_x > 0) {
            if ( East == dir_x ) {
                cout << "Go EAST" << endl;
                return EAST;
            } else if ( West == dir_x ) {
                cout << "Go WEST" << endl;
                return WEST;
            } else {
                cout << "Go Non-X" << endl;
            }
        }
        if (dir_y > 0) {
            if ( North == dir_y ) {
                cout << "Go NORTH" << endl;
                return NORTH;
            } else if ( South == dir_y ) {
                cout << "Go SOUTH" << endl;
                return SOUTH;
            } else {
                cout << "Go Non-Y" << endl;
            }
        }
    } else { // Refer to lowest non-zero in bubble sort
        if ( North == directions[first] )
            return NORTH;
        else if ( East == directions[first] )
            return EAST;
        else if ( South == directions[first] )
            return SOUTH;
        else if ( West == directions[first] )
            return WEST;
        else
            cout << "Oops" << endl;
    }
}

//
// Fills in the given variables with the terrain difficulty of the 
// locations around the given position.
//
void getSurroundings(Position p, unsigned int &n, unsigned int &e, unsigned int &s, unsigned int &w); 

vector< vector<unsigned long> > map;

int main(int argc, char** argv) {

	vector<unsigned long> row;
	unsigned long var;
	unsigned long x_temp = 0;
	unsigned long x_dimension = 0;
	unsigned long y_dimension = 0;
	cout << "Map Contents:\n";
	ifstream file;
	file.open(MAP_FILE);
	while(file >> var) {
		x_temp++;
		row.push_back(var);
		cout << "\t" << var;
		if(file.peek() == '\n') {
			if(x_temp > x_dimension) {
				x_dimension = x_temp;
			}
			x_temp = 0;
			y_dimension++;
			map.push_back(row);
			row.clear();
			cout << endl;
		}
	}
	file.close();
	row.clear();
	cout << "Map dimensions: " << x_dimension << "x" << y_dimension << endl;

	/*
	 * Initialize some variables for the main loop.
	 */
	unsigned long movecount = 0;
	unsigned int north, south, east, west;
	string name = "me!";
	Position start(STARTX, STARTY);
	Position end(ENDX, ENDY);
	Person me(start, name); 
	row.clear();

	/*
	 * Announce current position.
	 */
	Position current = me.getPosition();
	cout << "I am here: " << endl;
	current.print();
	cout << "The end is there: " << endl;
	end.print();
	cout << endl << "Moves: \n";

	/*
	 * Jump into the loop.
	 * Basically, keep going as long as we're alive and not there yet.
	 */
	while(!current.equals(end) && me.isAlive()) {
		string::size_type x = (string::size_type)current.getx();
		string::size_type y = (string::size_type)current.gety();

		if(y >= map.size() || y < 0) {
			cerr << "You failed!\nPlayer attempted to move out of bounds.\nX: " << x << "\nY: " << y << endl;
			return -1;
		}

		row = map.at(y);

		if(x >= row.size() || x < 0) {
			cerr << "You failed!\nPlayer attempted to move out of bounds.\nX: " << x << "\nY: " << y << endl;
			return -1;
		}

		movecount += row.at(x);
		cout << "\tRunning Total: " << movecount << endl;

		getSurroundings(current, north, east, south, west);
		Direction d = decide(current, end, north, east, south, west);
		me.move(d);
		current = me.getPosition();
	}
	cout << "Success! You reached the goal in " << movecount << " moves!" << endl;

	return 0;
}


void getSurroundings(Position p, unsigned int &n, unsigned int &e, unsigned int &s, unsigned int &w) {

	vector<unsigned long> row1;
	vector<unsigned long> row2;
	vector<unsigned long> row3;
	
	int x = p.getx();
	int y = p.gety();

	// Erase incoming values.
	n = 0;
	e = 0;
	s = 0;
	w = 0;

	// Make sure we don't access a point off the grid
	if(x < 0 || y < 0) {
		n = 0;
		s = 0;
		e = 0;
		w = 0;
	} else {

		//	  n
		//  	w * e
		// 	  s	
		if(y - 1 >= 0 && y < map.size()) {
			row1 = map.at(y - 1);
			if(x >= 0 && x < row1.size()) {
				n = row1.at(x);
			} else {
				n = 0;
			}
		}

		if(y >= 0 && y < map.size()) {
			row2 = map.at(y);
			if(x + 1 < row2.size()) {
				e = row2.at(x + 1);
			} else {
				e = 0;
			}
			if(x - 1 > 0) {
				w = row2.at(x - 1);
			} else {
				w = 0;
			}

		}

		if(y + 1 >= 0 && (y + 1) < map.size()) {
			row3 = map.at(y + 1);
			if(x >= 0 && x < row3.size()) {
				s = row3.at(x);
			} else {
				s = 0;
			}
		}
		if(n <=0 || n > 100) {
			n = 0;
		}
		if(s <=0 || s > 100) {
			s = 0;
		}
		if(e <=0 || e > 100) {
			e = 0;
		}
		if(w <=0 || w > 100) {
			w = 0;
		}

		// This is some really ugly code...
	}
}
