#ifndef __POSITION__
#define __POSITION__

#include "includes.h"

class Position {
	
	private:
		unsigned long x;
		unsigned long y;

	public:
				Position();
				Position(const unsigned long x, const unsigned long y);
				Position(const Position& p);
		bool		equals(const Position& p);
		void		move(const Direction d);
		void		print();
		unsigned long 	getx();
		unsigned long	gety(); 
};

#endif
