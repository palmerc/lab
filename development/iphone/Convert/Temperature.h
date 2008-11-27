/*
 *  Temperature.h
 *  Convert
 *
 *  Created by Cameron Palmer on 15/08/2008.
 *  Copyright 2008 University of North Texas. All rights reserved.
 *
 */

class Temperature {

private:
	double celsiusTemp;
	double fahrenheitTemp;

public:
	Temperature();
	~Temperature();

	double getCelsiusTemp(void);
	double getFahrenheitTemp(void);
	double getKelvinTemp(void);

	void setCelsiusTemp(double);
	void setFahrenheitTemp(double);
	void setKelvinTemp(double);
};