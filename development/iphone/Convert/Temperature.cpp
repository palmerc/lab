/*
 *  Temperature.cpp
 *  Convert
 *
 *  Created by Cameron Palmer on 15/08/2008.
 *  Copyright 2008 University of North Texas. All rights reserved.
 *
 */

#include "Temperature.h"

Temperature::Temperature() {}
Temperature::~Temperature() {}

double Temperature::getCelsiusTemp(void) {
	return celsiusTemp;
}
double Temperature::getFahrenheitTemp(void) {
	return fahrenheitTemp;
}
double Temperature::getKelvinTemp(void) {
	return celsiusTemp + 273;
}
	
void Temperature::setCelsiusTemp(double celsius) {
	celsiusTemp = celsius;
	fahrenheitTemp = (9.0/5.0 * celsius) + 32;
}
void Temperature::setFahrenheitTemp(double fahrenheit) {
	fahrenheitTemp = fahrenheit;
	celsiusTemp = (5.0/9.0) * (fahrenheit - 32);
}
void Temperature::setKelvinTemp(double kelvin) {
	setCelsiusTemp(kelvin - 273);
}