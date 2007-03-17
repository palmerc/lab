/*
 * NBest.h --
 *	N-best lists
 *
 * Copyright (c) 1995-2006, SRI International.  All Rights Reserved.
 *
 * @(#)$Header: /home/srilm/devel/lm/src/RCS/NBest.h,v 1.32 2006/09/20 13:39:28 stolcke Exp $
 *
 */

#ifndef _NBest_h_
#define _NBest_h_

#ifdef PRE_ISO_CXX
# include <iostream.h>
#else
# include <iostream>
using namespace std;
#endif

#include "Boolean.h"
#include "Prob.h"
#include "File.h"
#include "Map.h"
#include "Vocab.h"
#include "Array.h"
#include "LM.h"
#include "MemStats.h"
#include "Debug.h"

#undef valid		/* avoids conflict with class member on some systems */

/* 
 * Magic string headers identifying Decipher N-best lists
 */
const char nbest1Magic[] = "NBestList1.0";
const char nbest2Magic[] = "NBestList2.0";

typedef float NBestTimestamp;

/*
 * Optional detailed information associated with words in N-best lists
 */
class NBestWordInfo {
public:
    NBestWordInfo();
    ~NBestWordInfo();
    NBestWordInfo &operator= (const NBestWordInfo &other);

    void write(File &file);			// write info to file
    Boolean parse(const char *s);		// parse info from string
    void invalidate();				// invalidate info
    Boolean valid() const;			// check that info is valid
    void merge(const NBestWordInfo &other);	// combine two pieces of info

    VocabIndex word;
    NBestTimestamp start;
    NBestTimestamp duration;
    LogP acousticScore;
    LogP languageScore;
    char *phones;
    char *phoneDurs;
    /*
     * The following two are used optionally when used as input to 
     * WordMesh::wordAlign() to encode case where the word/transition
     * posteriors differ from the overall hyp posteriors.
     */
    Prob wordPosterior;				// word posterior probability
    Prob transPosterior;			// transition to next word p.p.

    /*
     * Utility functions
     */
    static unsigned length(const NBestWordInfo *words);
    static NBestWordInfo *copy(NBestWordInfo *to, const NBestWordInfo *from);
    static VocabIndex *copy(VocabIndex *to, const NBestWordInfo *from);
};

extern const char *phoneSeparator;	// used for phones & phoneDurs strings
extern const NBestTimestamp frameLength; // quantization unit of word timemarks

/*
 * Support for maps with (const NBestWordInfo *) as keys
 */

unsigned LHash_hashKey(const NBestWordInfo *key, unsigned maxBits);
const NBestWordInfo *Map_copyKey(const NBestWordInfo *key);
void Map_freeKey(const NBestWordInfo *key);
Boolean LHash_equalKey(const NBestWordInfo *key1, const NBestWordInfo *key2);
int SArray_compareKey(const NBestWordInfo *key1, const NBestWordInfo *key2);


/*
 * A hypothesis in an N-best list with associated info
 */
class NBestHyp {
public:
    NBestHyp();
    ~NBestHyp();
    NBestHyp &operator= (const NBestHyp &other);

    void rescore(LM &lm, double lmScale, double wtScale);
    void decipherFix(LM &lm, double lmScale, double wtScale);
    void reweight(double lmScale, double wtScale, double amScale = 1.0);

    Boolean parse(char *line, Vocab &vocab, unsigned decipherFormat = 0,
			LogP acousticOffset = 0.0,
			Boolean multiwords = false, Boolean backtrace = false);
    void write(File &file, Vocab &vocab, Boolean decipherFormat = true,
						    LogP acousticOffset = 0.0);

    VocabIndex *words;
    NBestWordInfo *wordInfo;
    LogP acousticScore;
    LogP languageScore;
    unsigned numWords;
    LogP totalScore;
    Prob posterior;
    unsigned numErrors;
    unsigned rank;
};

class NBestList: public Debug
{
public:
    NBestList(Vocab &vocab, unsigned maxSize = 0,
			Boolean multiwords = false, Boolean backtrace = false);
    ~NBestList() {};

    static unsigned initialSize;

    unsigned numHyps() { return _numHyps; };
    NBestHyp &getHyp(unsigned number) { return hypList[number]; };
    void sortHyps();

    void rescoreHyps(LM &lm, double lmScale, double wtScale);
    void decipherFix(LM &lm, double lmScale, double wtScale);
    void reweightHyps(double lmScale, double wtScale, double amScale = 1.0);
    void computePosteriors(double lmScale, double wtScale,
					double postScale, double amScale = 1.0);
    void removeNoise(LM &lm);

    unsigned wordError(const VocabIndex *words,
				unsigned &sub, unsigned &ins, unsigned &del);

    double minimizeWordError(VocabIndex *words, unsigned length,
				double &subs, double &inss, double &dels,
				unsigned maxRescore = 0, Prob postPrune = 0.0);

    void acousticNorm();
    void acousticDenorm();

    Boolean read(File &file);
    Boolean write(File &file, Boolean decipherFormat = true,
						unsigned numHyps = 0);
    void memStats(MemStats &stats);

    Vocab &vocab;
    LogP acousticOffset;

private:
    Array<NBestHyp> hypList;
    unsigned _numHyps;
    unsigned maxSize;
    Boolean multiwords;		// split multiwords
    Boolean backtrace;		// keep backtrace information (if available)
};

#endif /* _NBest_h_ */
