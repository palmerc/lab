#include <stdio.h>

#define SIZE 20
int adjacency[SIZE][SIZE];
int best_cost;
int *BestList;
main()
{
	void print_tour();
	void permute_list();
	int i,j,weight,num;
	int *NodeList;
	char c;
	int cost;
	int thisCost;

	for( i = 0; i < SIZE; i++ )
		for( j = 0; j < SIZE; j++ )
			adjacency[i][j] = -1;			

	/*  Read input and build adjacency matrix  */

	while( scanf("%c",&c) != EOF )
        {
                if( c == 'c' )
                {
                        scanf("%d",&num);
                }
                else if( c == 'a' )
                {
                        scanf("%d%d%d",&i,&j,&weight);
			adjacency[i][j] = weight;			
			adjacency[j][i] = weight;			
                }
        }
	
	NodeList = (int *) malloc(num * sizeof(int));
	BestList = (int *) malloc(num * sizeof(int));
	for( i = 0; i < num; i++ )
	{
		NodeList[i] = i + 1;
		BestList[i] = i + 1;
	}
	best_cost = compute_cost(BestList,num);
	permute_list(NodeList,num,num);
	print_tour(BestList,num,best_cost);
}

void permute_list(A,num,n)
	int A[],num,n;
{
	int i,j,temp;
	int cost;
	int start = n - num + 1;

		

	if( num == 1 )
	{
		cost = compute_cost(A,n);
		if( cost < best_cost )
		{
			int k;
			best_cost = cost;
			for(k = 0; k < n; k++)
				BestList[k] = A[k];			
		}
		return;
	}

	for(i = start; i < n; i++)
	{
		permute_list(A,num-1,n);

		/* rotate the list  */
		temp = A[n-1];
		for( j = n-1; j > start; j-- ) 
		{
			A[j] = A[j-1];
		}
		A[start] = temp;
	}
}

int compute_cost(List,num)
	int List[],num;
{
	int cost = 0;
	int i;

	for(i = 0; i < num-1; i++)
		cost += adjacency[List[i]][List[i+1]];
	cost +=  adjacency[List[i]][List[0]];

	return cost;
}

void print_tour(List,num,cost)
	int List[],num,cost;
{
	int i;
	printf("The tour ");
	for( i = 0; i < num; i++ )
		printf("%d ", List[i]);

	printf(" costs %d\n",cost);
}
