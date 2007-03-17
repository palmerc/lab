
#include <stdio.h>
#include <Trie.cc>
#include <Counts.h>
#include <XCount.h>


#if defined(__SUNPRO_CC)
# pragma pack(2)
#endif
class foo {
public:
#if defined(__INTEL_COMPILER) || defined(__GNUC__)
	int x __attribute__ ((packed));
#else
	int x;
#endif
	short y;
};
#if defined(__SUNPRO_CC)
# pragma pack()
#endif

class bar {
public:
	foo x;
	short y;
};



int main() 
{
	bar b;

	printf("sizeof(void *) = %lu, sizeof(long) = %lu\n",
		sizeof(void *), sizeof(long));
	printf("sizeof class foo = %lu, bar = %lu\n", sizeof(foo),
		sizeof(bar));

	printf("sizeof Trie<short,short> = %lu\n", sizeof(Trie<short,short>));
	printf("sizeof Trie<int,int> = %lu\n", sizeof(Trie<int,int>));
	printf("sizeof Trie<unsigned,Count> = %lu\n", sizeof(Trie<unsigned,Count>));
	printf("sizeof Trie<unsigned,XCount> = %lu\n", sizeof(Trie<unsigned,XCount>));
	printf("sizeof Trie<short,double> = %lu\n", sizeof(Trie<short,double>));

	b.x.x = 1;
	b.y = 2;
}
