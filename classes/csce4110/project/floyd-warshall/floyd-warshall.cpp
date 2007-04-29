#include <limits>
#include <iostream>

int MAX_INT = 1000; // I need a better choice here

int W[5][5] = { {0, 3, 8, MAX_INT, -4},
	{MAX_INT, 0, MAX_INT, 1, 7},
	{MAX_INT, 4, 0, MAX_INT, MAX_INT},
	{2, MAX_INT, -5, 0, MAX_INT},
	{MAX_INT, MAX_INT, MAX_INT, 6, 0} };
int rows = 5;

int min(int x, int y)
{
	return ((x) < (y) ? (x) : (y));
}

void FloydWarshall()
{
	for (int k = 0; k < rows; ++k)
    {
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < rows; ++j)
			{
				W[i][j] = min(W[i][j], W[i][k] + W[k][j]);
			}
		}
    }
}

int main()
{
	FloydWarshall();
	
	for (int i=0; i < rows; ++i)
    {
	   for (int j=0; j < rows; ++j)
	   {
	      std::cout << W[i][j] << " ";
       }
       std::cout << std::endl;
    }
	return 0;
}
