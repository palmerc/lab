/*
 * LogicFunctionADT.h
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONADT_H_
#define LOGICFUNCTIONADT_H_

class LogicFunctionADT
{
public:
    virtual char calculate(char* inputs) const = 0;

    int getNumberInputs() const;
    const char* getName() const;

    void setNumberInputs(int n);
    void setName(const char* name);

private:
	int numberInputs;
	const char* name;
};

#endif /* LOGICFUNCTIONADT_H_ */
