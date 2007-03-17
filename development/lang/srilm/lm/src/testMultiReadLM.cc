/*
 * testNgram --
 *	Rudimentary test for Ngram LM class
 */

#ifndef lint
static char Copyright[] = "Copyright (c) 2005, SRI International.  All Rights Reserved.";
static char RcsId[] = "@(#)$Header: /home/srilm/devel/lm/src/RCS/testNgram.cc,v 1.1 2005/07/17 22:54:13 stolcke Exp $";
#endif

#include <stdio.h>

#include "Ngram.h"

int
main (int argc, char *argv[])
{
    if (argc < 2) {
	cerr << "missing filename arg\n";
	exit(1);
    } else {
	File file(argv[1], "r");

	for (int i = 1; i <= 3; i ++) {
	    Vocab vocab;
	    Ngram ng(vocab, 3);

	    cerr << "reading LM no. " << i << endl;
	    if (!ng.read(file)) {
		cerr << "error in LM no. " << i << endl;
		exit(1);
	    }

	    File file(stderr);
	    ng.write(file);
	}
    }

    exit(0);
}
