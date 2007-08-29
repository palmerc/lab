#include "includes.h"

unsigned long Position::getx() {
	return x;
}

unsigned long Position::gety() {
	return y;
}

void Position::print() {
	cout << "x: " << x << endl << "y: " << y << endl;
}

bool Position::equals(const Position& p) {
	return (x == p.x && y == p.y);
}

Position::Position() {
	x = 0;
	y = 0;
}

Position::Position(const unsigned long xin, const unsigned long yin) {
	x = xin;
	y = yin;
}

Position::Position(const Position& p) {
	x = p.x;
	y = p.y;
}

void Position::move(const Direction d) {
	switch(d) {
		case NORTH:
			if(y > 0) {
				y--;
			} else {
				cerr << "Can't go north from here\n";
				print();
				return;
			}
			break;
		case NORTHEAST:
			if(y > 0 && x+1 < ULONG_MAX) {
				y--;
				x++;
			} else {
				cerr << "Can't go northeast from here\n";
				print();
				return;
			}
			break;
		case EAST:
			if(x+1 < ULONG_MAX) {
				x++;
			} else {
				cerr << "Can't go east from here\n";
				print();
				return;
			}
			break;
		case SOUTHEAST:
			if(y+1 < ULONG_MAX && x+1 < ULONG_MAX) {
				x++;
				y++;
			} else {
				cerr << "Can't go southeast from here\n";
				print();
				return;
			}
			break;
		case SOUTH:
			if(y+1 < ULONG_MAX) {
				y++;
			} else {
				cerr << "Can't go south from here\n";
				print();
				return;
			}
			break;
		case SOUTHWEST:
			if(x > 0 && y+1 < ULONG_MAX) {
				x--;
				y++;
			} else {
				cerr << "Can't go southwest from here\n";
				print();
				return;
			}
			break;
		case WEST:
			if(x > 0) {
				x--;
			} else {
				cerr << "Can't go west from here\n";
				print();
				return;
			}
			break;
		case NORTHWEST:
			if(x > 0 && y > 0) {
				x--;
				y--;
			} else {
				cerr << "Can't go northwest from here\n";
				print();
				return;
			}
			break;
		default:
			;
	};
}

