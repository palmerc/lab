/*
Copyright (C) 1989-1995 by Peter R. Homan, Norwalk, Ca.
All Rights Reserved.  No part of this program may be photocopied, reproduced,
or translated to another program language without the prior written consent of
the author, Peter R. Homan.

Permission has been expressly granted by Peter R.  Homan to use this program as
a SPEC benchmark.
*/


/* Utils   - BitVec.h */
#ifndef BITVEC_H
#define BITVERC_H

/*+-----------------------------------------------------------------------+*
~t                            TYPEDEFS
 *+-----------------------------------------------------------------------+*/
       typedef struct BitVec
       {
       indextype    AllocBits;
       indextype    ExtendBits;
       indextype    BitLength;
       indextype    HighestBitSet;
       bytetype    *BitField;

       }              BitVec;
       typedef BitVec        bitvectype;


/*+-----------------------------------------------------------------------+
~M                            METHODS                                     !
  +-----------------------------------------------------------------------+*/
/*     16.1.1                                                             */
       boolean BitVec_Create
                         (indextype     AllocBits, indextype     ExtendBits,
                          ft F,lt Z,zz *Status,    bitvectype  **BitStruc);

/*     16.1.2                                                             */
       boolean BitVec_PutBit
                         (bitvectype   *BitStruc,  indextype     BitNum,
                          bytetype      BitValue,  ft F,lt Z,zz *Status);

/*     16.1.3                                                             */
       boolean BitVec_GetBit
                         (bitvectype   *BitStruc,  indextype     BitNum,
                          ft F,lt Z,zz *Status,    bytetype     *BitValue);

/*+-----------------------------------------------------------------------+
~P                         END OF BITVEC.H                                !
  +-----------------------------------------------------------------------+*/
#endif
