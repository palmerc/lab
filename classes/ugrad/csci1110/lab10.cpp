#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct DOB {
	int month, day, year;
};

struct Person {
	string first, last;
	int id;
	DOB dob;
};

void readrecord ( Person &individual, ifstream &inFile ) {
	char str[256];
	string dobText;

	inFile >> individual.first;
	inFile.ignore();
	inFile.getline(str, 256, ',');
	individual.last = str;
        inFile.getline(str, 256, ',');
	individual.id = atoi(str);
	inFile >> dobText;
	inFile.ignore();
	inFile.getline(str, 256, '-');
	individual.dob.month = atoi(str);
	inFile.getline(str, 256, '-');
	individual.dob.day = atoi(str);
	inFile >> individual.dob.year;
	inFile.ignore(256,'\n');
}

int main() {
	Person individual;
	string fileName, dobText, fname, lname;
	ifstream inFile;
	int ageCalculation;

        cout << "Enter the input file name: ";
	cin >> fileName;
	inFile.open(fileName.c_str());

	if ( !inFile ) {
		cout << "Can't open the input file." << endl;
		return 1;
	}
	
	readrecord( individual, inFile );

	while ( !inFile.eof() ) {
		if ( individual.dob.day == 1 && individual.dob.month == 1 )
		   ageCalculation = 2005 - individual.dob.year;
		else 
		   ageCalculation = 2005 - individual.dob.year - 1;

		cout << individual.last << ',' << individual.first << ',';
                cout << individual.id << ',' << individual.dob.day << '/';
                cout << individual.dob.month << '/' << individual.dob.year;
                cout << ",age=" << ageCalculation << endl;

		readrecord( individual, inFile );
	}

	inFile.close();
	return 0;
}
