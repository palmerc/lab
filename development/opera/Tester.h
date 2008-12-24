/*
 * Tester.h
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#ifndef TESTER_H_
#define TESTER_H_

const char *or2_table [] =
{
"txt",
"xtt",
"fff",
0
};

const char *or3_table [] =
{
"txxt",
"xtxt",
"xxtt",
"ffff",
0
};

const char *and2_table [] =
{
"ttt",
"fxf",
"xff",
0
};

const char *and3_table [] =
{
"tttt",
"fxxf",
"xfxf",
"xxff",
0
};

const char *xor2_table  [] =
{
"tft",
"ftt",
"fff",
"ttf",
0
};

const char *xor3_table  [] =
{
"fftt",
"ftft",
"tfft",
"tttt",
"ffff",
"fttf",
"ttff",
"tftf",
0
};

const char *impl_table [] =
{
"xtt",
"fxt",
"tff",
0
};

const char *not_table [] =
{
"tf",
"ft",
0
};

const char *incl_table [] =
{
"txxt",
0
};

#endif /* TESTER_H_ */
