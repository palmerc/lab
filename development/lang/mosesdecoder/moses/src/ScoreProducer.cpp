// $Id: ScoreProducer.cpp 798 2006-08-30 17:00:41Z phkoehn $

#include <iostream>
#include <typeinfo>
#include "ScoreProducer.h"
#include "StaticData.h"
#include "ScoreIndexManager.h"

unsigned int ScoreProducer::s_globalScoreBookkeepingIdCounter(0);

ScoreProducer::~ScoreProducer() {}

ScoreProducer::ScoreProducer()
{
  m_scoreBookkeepingId = UNASSIGNED;
}
 
