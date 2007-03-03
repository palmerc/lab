// ***********************************************************************
//
//   Tour.h --- defines the derived classes Tour which represents 
//              an element in a list of arrays integers that will be
//              used to represent a city tour in the traveling 
//              salesman problem.
//
// Classes included: Tour
//
// History --- initial coding 10/3/03 by Philip Sweany
//             "improved" coding 11/11/03 by Joe Student
//
// No rights reserved.
//
// ***********************************************************************

#ifndef TOURH
#define TOURH

#include <iostream>
#include <cstdlib>
#include <assert.h>
// ***********************************************************************
// 
//   Class Tour --- the element of a doubly linked list.
// 
// ***********************************************************************
class Tour 
{
    public :

// -----------------------------------------------------------------------
// 
//   Tour --- the constructor
// 
// -----------------------------------------------------------------------
   Tour(); 


// -----------------------------------------------------------------------
// 
//   Tour --- Alternate constructor
// 
// -----------------------------------------------------------------------
   Tour(unsigned int *); 


// -----------------------------------------------------------------------
//
//   Print --- "print" the link.
//
// -----------------------------------------------------------------------
   void print();


// -----------------------------------------------------------------------
// 
//   Evaluate --- returns the worthiness of the tour.
// 
// -----------------------------------------------------------------------
   int evaluate(); 


// -----------------------------------------------------------------------
// 
//   Crossover --- Perform a crossover operation on this Tour.
// 
// -----------------------------------------------------------------------
   void crossover(Tour& t, int index); 

	// student code, so no comments :-)
                bool operator<  (Tour& a);
                bool operator<= (Tour& a);
                bool operator>  (Tour& a);
                bool operator>= (Tour& a);
                bool operator== (Tour& a);

   private:

   bool is_valid();
   void set_city(int index, int citi);
   void mutate();

   int worthiness;    // the "value" of the tour
   unsigned data[32];  
   // *******************************************************************
   // 
   //  This representation of the Traveling Salesman problem that we're 
   //  planning to solve with the Genetic Algorithm is intuitively      
   //  obvious :-).  Our 32 unsigned ints provide 1024 bits which are   
   //  used to represent 128 8-bit fields.  Since any legal traveling
   //  salesman tour must include all 128 cities(nodes), and 128 edges
   //  (because we have to return to the starting place, our 1024 bits
   //  represent the order that the cities will be visited during a tour.
   //  So looking at the "first" 8 bits (well actually 7, but since I didn't
   //  want to try hacking 7-bit quantities, so I just ignore one bit),
   //  will represent the starting city.  The next 8 bits indicate the
   //  second city in the tour, ..., up to the 128th "position" in the
   //  string which denotes that last new city in the tour.  The cost of
   //  a tour then is just the 128 costs associated with city1-city2,
   //  city2-city3, ..., city127-city128 and city128 back to city1.
   // 
   // *******************************************************************

};

extern Tour& combine(Tour&, Tour&);
Tour& build_a_random_bit_string();
void initialize_distances();


#endif
