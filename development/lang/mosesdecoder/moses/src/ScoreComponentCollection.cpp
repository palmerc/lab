// $Id: ScoreComponentCollection.cpp 1218 2007-02-16 18:08:37Z hieuhoang1972 $

#include "ScoreComponentCollection.h"
#include "StaticData.h"

ScoreComponentCollection::ScoreComponentCollection()
  : m_scores(StaticData::Instance().GetTotalScoreComponents(), 0.0f)
  , m_sim(&StaticData::Instance().GetScoreIndexManager())
{}

