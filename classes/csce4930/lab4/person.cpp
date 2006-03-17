
#include "includes.h"

bool Person::isAlive() {
	return (health > 0);
}

Person::Person(Position& p, const std::string& n) {
	location = p;
	name = n;
	health = 100;
}

void Person::move(const Direction d) {
	location.move(d);
}

Position Person::getPosition() {
	return location;
}
