/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/



/* ENV-BLK1.h  */
/*+-----------------------------------------------------------------------+
~T                            TYPEDEFS                                    !
  +-----------------------------------------------------------------------+*/
#ifndef BLK1_H
#define BLK1_H

typedef struct TypeBlkHdr 
{
         filetokentype BlkFile;
#ifdef STRUC_ALIGN_BY8
	 filetokentype BlkFile_64;
#endif /* STRUC_ALIGN_BY8 */
         idtype        DbId;
         idtype        CoreDbId;
         numtype       HeadFreeVbns;
         numtype       BlkDirChunk;
         numtype       HeadFreeBlks;
         numtype       HeadNullBlks;
         numtype       BlkTknChunk;
         numtype       BlkSizeChunk;
         farlongtype   EofMark;
} blkhdrtype;

/*+-----------------------------------------------------------------------+
~P                         END OF BLK.H                                   !
  +-----------------------------------------------------------------------+*/
#endif
