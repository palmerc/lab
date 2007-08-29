#ifndef __PERSON__
#define __PERSON__

#include "includes.h"

class Person {

	private:
		Position 	location;
		unsigned long	health;
		string 		name;
	
	public:
				Person(Position& p, const std::string& n);
		void 		move(const Direction d);
		Position 	getPosition();
		bool		isAlive();

};

#endif
