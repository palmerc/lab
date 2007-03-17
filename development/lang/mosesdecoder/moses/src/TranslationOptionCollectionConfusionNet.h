// $Id: TranslationOptionCollectionConfusionNet.h 1218 2007-02-16 18:08:37Z hieuhoang1972 $
#pragma once

#include "TranslationOptionCollection.h"

class ConfusionNet;

class TranslationOptionCollectionConfusionNet : public TranslationOptionCollection {
 public:
	TranslationOptionCollectionConfusionNet(const ConfusionNet &source, size_t maxNoTransOptPerCoverage);

	void ProcessUnknownWord(		size_t sourcePos);

};
