// ***********************************************************************
//
// tour.cpp --- defines the derived classes Tour which represents 
//              an element in a list of arrays integers that will be
//              used to represent a city tour in the traveling 
//              salesman problem.
//
// Classes included: Tour
//
// History --- initial coding 10/6/03 by Philip Sweany
//             updated by Philip Sweany 10/22/03
//
// No rights reserved.
//
// ***********************************************************************

#include "tour.h"

void *Tour::_pNext;
size_t Tour::_counter;
int distances[128][128];	// store distance between each pair
                                // in the 128-city tsp

// tsp is ONE permutation of the 128 cities.   
// It will be changed during the randomized 
// Tour generation.
int tsp[128] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
                10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
                20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
                70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
                80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
                100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
                110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
                120, 121, 122, 123, 124, 125, 126, 127}; 


// masks to be used in bit manipulation 
static unsigned mask[4] = {0x7F000000, 0x007F0000, 0x00007F00, 0x0000007F}; 
static unsigned umask[4] = {0x00FFFFFF, 0xFF00FFFF, 0xFFFF00FF, 0xFFFFFF00}; 

// -----------------------------------------------------------------------
// 
//   Tour --- the constructor
// 
// -----------------------------------------------------------------------
   Tour::Tour()
   {
       worthiness = 1000000;  // big value indicates worthiness not set
       _counter = 0;
       _pNext = 0;
   }

   void* Tour::operator new ( size_t bytes )
   {
      if (bytes > _counter)
      {
         _pNext = malloc(16 * bytes);
         _counter = 16 * bytes;
      }
      void* _cur = _pNext;
      _pNext = (void*)((size_t) _pNext + bytes);
      _counter -= bytes;
      return _cur;
   } 

// -----------------------------------------------------------------------
// 
//   Tour --- the constructor
// 
// -----------------------------------------------------------------------
   Tour::Tour(unsigned int bits[32])
   {
       int i;
       for( i = 0; i < 32; i++ )
           data[i] = bits[i];
       worthiness = 1000000;
       worthiness = evaluate();
   } 



// -----------------------------------------------------------------------
//
//   Print --- "print" the tour.
//
// -----------------------------------------------------------------------
   void Tour::print()
   {
        int i,j;
        int cities[128];   // to extract "cities" from the bits of data
        for( i = 0; i < 32; i++ ) 
            for( j = 0; j < 4; j++ )
                cities[4*i+j] = (data[i] & mask[j]) >> (8*(3-j));
         
        for( i = 0; i < 8; i++ ) 
        {
            for( j = 0; j < 16; j++ )
 		std::cout << cities[i*16+j] << " ";
            std::cout << std::endl;
        }

        std::cout << " is worth " << worthiness << std::endl << std::endl 
             << std::endl << std::endl;
   }


// -----------------------------------------------------------------------
//
//   Compute_cost --- compute the cost of the tour by summing the 
//                    costs between each pair of cities in the tour.
//                    The 128 8-bit city representations represent
//                    an ordering of the cities on the tour.
//
// -----------------------------------------------------------------------
static int compute_cost(unsigned int *bits)
{
    int i,j;
    int Acities[128];
    int cost;

    // first "extract" the cities by bit manipulations
    for( i = 0; i < 32; i++ ) 
        for( j = 0; j < 4; j++ )
        {
            Acities[4*i+j] = (bits[i] & mask[j]) >> (8*(3-j));
        }

    // sum the costs
    cost = 0;
    for( i = 1; i < 127; i++ )
        cost += distances[Acities[i]][Acities[i+1]];
    cost += distances[127][0];

    return cost;
}


// -----------------------------------------------------------------------
// 
//   Evaluate --- returns the worthiness of the tour.
// 
// -----------------------------------------------------------------------
   int Tour::evaluate()
   {
      if( worthiness >= 1000000 ) worthiness = 256000 - compute_cost(data); 
      return worthiness;
   } 

bool Tour::operator<  (Tour& a)
{
        if (evaluate() <  a.evaluate()) return 1;
        return 0;
}

bool Tour::operator<= (Tour& a)
{
        if (evaluate() <= a.evaluate()) return 1;
        return 0;
}

bool Tour::operator>  (Tour& a)
{
        if (evaluate() >  a.evaluate()) return 1;
        return 0;
}

bool Tour::operator>= (Tour& a)
{
        if (evaluate() >= a.evaluate()) return 1;
        return 0;
}

bool Tour::operator== (Tour& a)
{
        if (evaluate() == a.evaluate()) return 1;
        return 0;
}



// -----------------------------------------------------------------------
// 
//   Crossover --- Perform a crossover operation on this Tour.
//                 Crossover combines bits from two tours, this and t,
//                 to produce a new "offspring."   Basically we "randomly"
//                 choose a location, say L, to cross over. Then we want 
//                 the first L cities of "this" to be replaced with the
//                 first L cities of t.  The rest of the child's bits
//                 come from "this."  The only problem with that is that 
//                 for a "legal" tour we need to have each of the 128 
//                 "cities" represented exactly once in the "data" of 
//                 "this".  To do that requires some "fixing" up after 
//                 the initial crossover.           
//           
// -----------------------------------------------------------------------
void Tour::crossover(Tour& t, int crosspt)
{
    unsigned thisDual[128];
    unsigned tDual[128];
    int i,j;

    for( i = 0; i < 32; i++ )
        for(j = 0; j < 4; j++ )
        {
	    thisDual[(data[i]&mask[j])>>(8*(3-j))] = i * 4 + j;
	    tDual[(t.data[i]&mask[j])>>(8*(3-j))] = i * 4 + j;
        }

    int Iword = crosspt / 4;

    int Acities[128];
    int Bcities[128];
    for( i = 0; i < 32; i++ ) 
        for( j = 0; j < 4; j++ )
        {
            Acities[4*i+j] = (data[i] & mask[j]) >> (8*(3-j));
            Bcities[4*i+j] = (t.data[i] & mask[j]) >> (8*(3-j));
        }

    int k;
    for( k = 0; k < Iword; k++ )
       data[k] = t.data[k];	// the "new" bits.

    crosspt = (crosspt / 4) * 4;

    // "fix" duplicated values 
    for ( i = 0, j = 0; i < crosspt; i++ )
    {
        int tLoc;
        int thisLoc = thisDual[Bcities[i]];
        if( thisLoc > crosspt )  // we have to switch
        {
	    while( (tLoc =  tDual[Acities[j++]]) < crosspt )
		; // empty while loop

	    // tLoc should now point to the next city from
            // t that is duplicated in data.

            if ( j > crosspt ) 
                assert(0);

            // swap the two cites at data[thisLoc] and t.data[tLoc]
            unsigned u1, u2;
            u1 = (data[thisLoc/4] & mask[thisLoc % 4]) >> (8*(3-(thisLoc%4)));
            u2 = (t.data[tLoc/4] & mask[tLoc % 4]) >> (8*(3-(tLoc%4)));
            data[thisLoc/4] &= umask[thisLoc % 4];
            data[thisLoc/4] |= (u2 << (8 * (3-(thisLoc%4))));
        } 
    }
    if( ! is_valid() ) assert(0);
    mutate();
    worthiness = 1000000;
}
 
// -----------------------------------------------------------------------
// 
//   Mutate --- Randomly swap 0, 1 or 2 sets of cities.
// 
// -----------------------------------------------------------------------
void Tour::mutate()
{
    // look for mutation
    int city1, city2;
    unsigned thisDual[128];
    int i,j;

    for( i = 0; i < 32; i++ )
        for(j = 0; j < 4; j++ )
	    thisDual[(data[i]&mask[j])>>(8*(3-j))] = i * 4 + j;
    int trand = rand() % 1000;
    if( trand < 100 ) 
    {
        city1 = rand() % 128;
        city2 = rand() % 128;
        if( city1 != city2 ) 
        {
            set_city(city1, thisDual[city2]);
            set_city(city2, thisDual[city1]);
        }
        if( ! is_valid() ) 
            assert(0);
    }
    if( trand < 990 ) 
    {
        city1 = rand() % 128;
        city2 = rand() % 128;
        if( city1 != city2 ) 
        {
            set_city(city1, thisDual[city2]);
            set_city(city2, thisDual[city1]);
        }
    }
    if( ! is_valid() ) 
        assert(0);
}
 
// -----------------------------------------------------------------------
// 
//   Set_city --- Set the data field so that city[dup] = miss.
// 
// -----------------------------------------------------------------------
void Tour::set_city(int miss, int dup)
{
    unsigned thisDual[128];
    int i,j,index;

    for( i = 0; i < 32; i++ )
        for(j = 0; j < 4; j++ )
	    thisDual[(data[i]&mask[j])>>(8*(3-j))] = i * 4 + j;
    index = thisDual[dup];

    data[index/4] &= umask[index % 4];
    data[index/4] |= (miss << (8 * (3-(index%4))));
}
 
// -----------------------------------------------------------------------
// 
//   Is_valid --- Check for a valid tour.
// 
// -----------------------------------------------------------------------
bool Tour::is_valid()
{
    int i,j;
    int Acities[128];
    int duplicates[128];
    int missing[128];
    int dupIndex;
    int missIndex;
    int found[128];
    bool result = true;

    for( i = 0; i < 128; i++ )
    { 
        found[i] = 0;
        duplicates[i] = -1;
        missing[i] = -1;
    }

    for( i = 0; i < 32; i++ ) 
        for( j = 0; j < 4; j++ )
        {
            Acities[4*i+j] = (data[i] & mask[j]) >> (8*(3-j));
            found[Acities[4*i+j]]++; 
        }
    dupIndex = 0;
    missIndex = 0;
    for( i = 0; i < 128; i++ )
    {
        if( found[i] == 0 ) missing[missIndex++] = i;
        while( found[i] > 1 ) 
        {
            duplicates[dupIndex++] = i; 
            found[i]--;
        }
    }

    if( dupIndex != missIndex ) return false;
    for( i = 0; i < missIndex; i++ )
        set_city(missing[i], duplicates[i]);

    if( missIndex > 0 ) return is_valid();   // recursive call to check
					     // on the new, fixed tour

    for( i = 0; i < 128; i++ )
    {
        if( found[i] == 0 )
        {
 	    std::cout << "We found no city " << i << std::endl;
	    result = false;
        }
        if( found[i] > 1 ) 
        {
	    std::cout << "City " << i << " is listed " 
                 <<  found[i] << " times" <<std::endl;
	    result = false;
        }
    } 
    return result;
}


Tour& build_a_random_bit_string()
{
    int i,j;
    Tour *t;
    unsigned int bits[32];
    for( i = 0; i < 32; i++ )
        bits[i] = 0;

    for( i = 0; i < 64; i++ )
    {
        int index1 = rand() % 128;
        int index2 = rand() % 128;
        int temp = tsp[index1];
        tsp[index1] = tsp[index2];
        tsp[index2] = temp;
    }
    for( i = 0; i < 32; i++ )
        for( j = 0; j < 4; j++ )
            bits[i] |= tsp[i*4+j] << (8*(3-j));
    
    t = new Tour(bits);
    return *t; 
}

void initialize_distances()
{
    int i,j;
    for( i = 1; i < 128; i++ )
    {
        for( j = 0; j < i; j++ )
        {
	    distances[i][j] = distances[j][i] = rand() % 1000;
            if( (rand() % 100) <  6 ) 
	        distances[i][j] = distances[j][i] = rand() % 100;
            if( (rand() % 100) >  91 ) 
	        distances[i][j] = distances[j][i] = rand() % 5000;
        }
    }
    for( i = 0; i < 128; i++ )
        distances[i][i] = 0;
}

Tour& combine(Tour& t1, Tour& t2)
{
    Tour *X = new Tour(t1);
    X->crossover( t2,(rand() % 70) + 15 );  // two point crossover
    X->crossover( t2,(rand() % 70) + 15 );

    X->evaluate();
    return *X;  
}

