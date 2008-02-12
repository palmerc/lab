/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Utils   - BitVec.c */
#define  BITVEC_C
#include <math.h>
#include "defines.h"
#include "typedefs.h"
#include "bitvec.h"

/* ------------------------------------------------------------------------
                         BIT VEC PROCEDURES
   ------------------------------------------------------------------------*/
/*     16.1.1                                                             */
boolean BitVec_Create    (sizetype      AllocBits, indextype     ExtendBits,  
                          ft F,lt Z,zz *Status,    bitvectype  **BitStruc)
{
       farlongtype   ByteCount     = ((AllocBits | 7)+1) / 8;
static farlongtype   SizeOfBitVec  = (farlongtype )sizeof (BitVec);   

 if (DeBug)
 if (sprintf  (Msg, "\n    BitVec_Create:: AllocBits=%3u; ExtendBits=%4u;\n",
               AllocBits, ExtendBits))
     TraceMsg (0,   Msg);

 if (( *BitStruc            = (bitvectype *)VoidExtendCore (&SizeOfBitVec))
                           != NULL)
 if (((*BitStruc)->BitField = (bytetype   *)VoidExtendCore (&ByteCount))
                           != NULL)
 {
   (*BitStruc)->AllocBits      = AllocBits;
   (*BitStruc)->ExtendBits     = ExtendBits;
   (*BitStruc)->BitLength      = AllocBits;
   (*BitStruc)->HighestBitSet  = 0;

 } else
   *Status    = BitVec_CoreNoMore;


return(STAT);
}


/* ------------------------------------------------------------------------*/
/*     16.1.2                                                             */
boolean BitVec_PutBit    (bitvectype   *BitStruc,  indextype     BitNum,
                          bytetype      BitValue,  ft F,lt Z,zz *Status)
{
indextype        ByteNum   = BitNum >> 3;

/* bit value must be 0 or 1                                                */

 BitValue   = BitValue ? 1 : 0;

 if (BitNum > BitStruc->BitLength)
 {
    sprintf  (Msg, 
              "\n BitVec_PutBit:: BitNum=%2d > BitLength=%2d; BitValue= %3u\n",
               BitNum, BitStruc->BitLength, BitValue);
    TraceMsg (0,   Msg);
    *Status = BitVec_OutOfRange;

 } else  {
   if (BitValue) /* Set   */
       BitStruc->BitField[ ByteNum ] |= BitValue << (BitNum & 7);
    else          /* Reset */
       BitStruc->BitField[ ByteNum ] &= ~(1 << (BitNum & 7));
 }
 if (BitStruc->HighestBitSet  < BitNum)
     BitStruc->HighestBitSet  = BitNum;

 if (DeBug)
 if (sprintf  (Msg, "\n    BitVec_PutBit:: BitNum[%4d]=%2d;\n",
               BitNum, BitValue))
     TraceMsg (0,   Msg);

return(STAT);
}


/* ------------------------------------------------------------------------*/
/*     16.1.3                                                             */
boolean BitVec_GetBit    (bitvectype   *BitStruc,  indextype     BitNum,
                          ft F,lt Z,zz *Status,    bytetype     *BitValue)
{
indextype        ByteNum   = BitNum >> 3;

*BitValue    =  0;

 if (BitNum  > BitStruc->BitLength)
 {
    sprintf  (Msg, 
    "\n BitVec_GetBit:: BitNum=%2d > BitCount=%2d; BitValue= %3u\n",
               BitNum, BitStruc->BitLength, *BitValue);
    TraceMsg (0,   Msg);
   *Status   =  BitVec_OutOfRange;

 } else
   *BitValue =  (BitStruc->BitField[ ByteNum ] & 1 << (BitNum & 7)) ? 1: 0;

 if (DeBug)
 if (sprintf  (Msg, "\n    BitVec_GetBit:: BitNum=%2d; BitValue= %3u\n",
               BitNum, *BitValue))
     TraceMsg (0,   Msg);

return(STAT);
}


/*+-----------------------------------------------------------------------+
~P                         END OF BITVEC.C                                !
  +-----------------------------------------------------------------------+*/
