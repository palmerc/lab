#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;

int maxSubsequence(vector<int> a) {
	int i, j, k, sum = 0, max = 0;
	for (i = 0; i < a.size() - 1; i++) {
		for (j = i; j < a.size() - 1; j++) {
			sum = 0;
			
			for (k = i; k <= j; k++)
				sum += a[k];
			if (sum > max)
				max = sum;
		}
	}
	return max;
}

int main () {
    clock_t start, finish;
    double time;
	string iFileName;
	int number;
	vector<int> a;
	
    cout << "Enter the name of the file: ";
	cin >> iFileName;
	
	ifstream inFile;
	ofstream outFile;
	inFile.open(iFileName.c_str(), ios::in);
	outFile.open("output.txt", ios::out);

	while (inFile >> number)
		a.push_back( (int)number );
	
    start = clock();
	int max = maxSubsequence( a );
    finish = clock();
    time = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "The maximum subsequence is " << max << endl;
    cout << "The time it took to execute " << time << endl;
	outFile << "The maximum subsequence is " << max << endl;
	outFile << "The time it took to execute " << time << endl;
    
	inFile.close();
	outFile.close();
    return 0;
}
