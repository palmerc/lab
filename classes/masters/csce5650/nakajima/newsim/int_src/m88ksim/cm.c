/* @(#) File name: cm.c   Ver: 3.2   Cntl date: 3/6/89 11:59:13 */
static char copyright1[] = "Copyright (c) Motorola, Inc. 1986";

#include "functions.h"
#include <string.h>
#include <stdlib.h>

extern int	cmdcount;
extern char	*cmdptrs[];
extern int	usecmmu;


int cm(void)
{

	struct cmmu	*cmmu_ptr;
	unsigned int	*reg, value;
	char		choice, *errptr;

	if(usecmmu)
	{
		if(cmdcount == 4)
		{
			str_toupper(cmdptrs[1]);

			if(strcmp ("I", cmdptrs[1]) == 0)
			{
				cmmu_ptr = &Icmmu;
				choice = 'I';
			}
			else if(strcmp ("D", cmdptrs[1]) == 0)
			{
				cmmu_ptr = &Dcmmu;
				choice = 'D';
			}
			else
			{
				PPrintf("Internal error 1 in file cm.c\n");
				return -1;
			}

			str_toupper(cmdptrs[2]);

			if (strcmp ("SCR", cmdptrs[2]) == 0){
				reg = &(cmmu_ptr->control.SCMR);
			}else if (strcmp ("SCTR", cmdptrs[2]) == 0){
				reg = &(cmmu_ptr->control.SCTR);
			}else if (strcmp ("SAPR", cmdptrs[2]) == 0){
				reg = &(cmmu_ptr->control.SAPR);
			}else if (strcmp ("UAPR", cmdptrs[2]) == 0){
				reg = &(cmmu_ptr->control.UAPR);
			}else{
				PPrintf("Internal error 2 in file cm.c\n");
				return -1;
			}

			if(getexpr(cmdptrs[3], &errptr, &value) != -1)
				*reg = value;
			else
				PPrintf("Undefined symbol: %s\n", cmdptrs[2]);
		}
		else
		{
			PPrintf("Usage: cm I|D reg_name reg_value\n");
		}
	}
	else
	{
		PPrintf("Cache simulation not enabled.\n");
	}
	return 0;
}
