/* @(#) File name: targstdio.h   Ver: 3.1   Cntl date: 1/20/89 14:39:11 */
/*	@(#)stdio.h	6.1		*/

typedef struct {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	char	_flag;
	char	_file;
} TARGETFILE;
