// $Id: LanguageModelFactory.h 1227 2007-02-21 19:48:53Z hieuhoang1972 $

#ifndef _LANGUAGE_MODEL_FACTORY_H_
#define _LANGUAGE_MODEL_FACTORY_H_

#include <string>
#include <vector>
#include "TypeDef.h"

class LanguageModel;
class ScoreIndexManager;

namespace LanguageModelFactory {

	/**
	 * creates a language model that will use the appropriate
   * language model toolkit as its underlying implementation
	 */
	 LanguageModel* CreateLanguageModel(LMImplementation lmImplementation
																		, const std::vector<FactorType> &factorTypes     
																		, size_t nGramOrder
																		, const std::string &languageModelFile
																		, float weight
																		, ScoreIndexManager &scoreIndexManager);
	 
};

#endif
