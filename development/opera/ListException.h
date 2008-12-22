/*
 * ListException.h
 *
 *  Created on: Dec 20, 2008
 *      Author: palmerc
 */

#ifndef LISTEXCEPTION_H_
#define LISTEXCEPTION_H_

#include <stdexcept>
#include <string>

using namespace std;

class ListException: public logic_error
{
	public:
		ListException(const string & message = ""): logic_error(message.c_str())
		{
		}
};

#endif /* LISTEXCEPTION_H_ */
