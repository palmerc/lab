//list.h, a header file for functions in list.cc
//CSCI 1120
//Will Briggs

#define NOT_FOUND -1
#define MAX_ITEMS 50

//Seems strange to define this in list.h; what do lists and directions
//have to do with each other (except that you can have a list of directions,
//as here)?  When we learn templates, we'll be able to separate these
//relatively independent concepts.

enum Direction {NORTH, SOUTH, EAST, WEST};

typedef Direction Item;

//These operations maintain an ordered list

void insert (Item List[], int& howmany, Item thing);
     //inserts thing into List, if there's room; else prints error message

int  search (Item List[], int  howmany, Item thing); 
     //returns index of thing, if found; else returns NOT_FOUND

void print  (Item List[], int  howmany);

void append (Item List[], int& howmany, Item thing);
     //appends to end of list, if room; else prints error message

void remove_last (Item List[], int& howmany);
     //removes last item; if none, prints error message
