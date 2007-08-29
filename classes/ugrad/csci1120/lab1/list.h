//list.h, a header file for functions in list.cc
//CSCI 1120
//Will Briggs

#define NOT_FOUND -1
#define MAX_ITEMS 20
//#define INITIAL_VALUE 0x10FFFFFF
#define INITIAL_VALUE 0x0

typedef char Item;

//These operations maintain an ordered list

void insert (Item List[], int& howmany, Item thing);
     //inserts thing into List, if there's room; else prints error message

int  search (Item List[], int  howmany, Item thing); 
     //returns index of thing, if found; else returns NOT_FOUND

void print  (Item List[], int  howmany);
