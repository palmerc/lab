// main.cpp

#include "infixcalculator.h"
#include <iostream>
using namespace std;

int main()
{
   cStackAT<string> S;
   string  PE, token, stackTop;
   double  operand1, operand2;
   bool    success;

   cout << "Enter a infix expression: " << endl << endl;

   while (true)
   {
	cin >> token;
        if (!cin)
          break;
	if (token[0] == '(' || token[0] == ')' || token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')
	{	
	switch(token[0])
	{
	
		case('('):
			S.Push(token, success);
			break;
		case(')'):
			S.GetStackTop(token, success);

			while ( token[0] != '(')
          		{
				PE = PE + ' ' +  token;
		        	S.Pop(token, success);
			}	
			break;
		case('+'):
		case('-'):
		case('*'):
		case('/'):
			S.GetStackTop(stackTop, success);
			if (S.Empty())
				S.Push(token, success);
			else if (stackTop[0] != '(' && stackTop[0] != ')')
			{
				if ((token[0] == '+' || token[0] == '-') && (stackTop[0] == '*' || stackTop[0] == '/'))
				{
					S.Pop(stackTop, success);
					PE = PE + ' ' + stackTop;
				}
			}
			break;
	};
	}
        else
          PE = PE + ' ' + token[0];
   }

  cout << PE;

   int length = PE.length();
   int ctr = 0;
   cStackAT <double> R;
   while (length > ctr)
   {
      if (PE[ctr] == '+' || PE[ctr] == '-' ||
         PE[ctr] == '*' || PE[ctr] == '/')
      {
        R.Pop(operand2, success);
       R.Pop(operand1, success);
     switch (token[ctr])
        {
         case '+':
           R.Push(operand1+operand2, success);
	   ctr++;
           break;
         case '-':
           R.Push(operand1-operand2, success);
           ctr++;
	   break;
        case '*':
           R.Push(operand1*operand2, success);
           ctr++;
	   break;
        case '/':
           R.Push(operand1/operand2, success);
           ctr++;
	   break;
       };
     }
     else
      {
	R.Push(atof(token.c_str()), success);
	ctr++;
      }
   }
   R.Pop(operand1, success);
   cout << "Result: " << operand1 << endl;
}

 

