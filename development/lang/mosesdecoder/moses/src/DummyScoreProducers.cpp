// $Id: DummyScoreProducers.cpp 1227 2007-02-21 19:48:53Z hieuhoang1972 $

#include "StaticData.h"
#include "DummyScoreProducers.h"
#include "WordsRange.h"

DistortionScoreProducer::DistortionScoreProducer(ScoreIndexManager &scoreIndexManager)
{
	scoreIndexManager.AddScoreProducer(this);
}

size_t DistortionScoreProducer::GetNumScoreComponents() const
{
	return 1;
}

const std::string DistortionScoreProducer::GetScoreProducerDescription(int idx) const
{
	return "distortion score";
}

float DistortionScoreProducer::CalculateDistortionScore(const WordsRange &prev, const WordsRange &curr) const
{
  if (prev.GetNumWordsCovered() == 0)
  { // 1st hypothesis with translated phrase. NOT the seed hypo.
    return - (float) curr.GetStartPos();
  }
  else
  { // add distortion score of current translated phrase to
    // distortions scores of all previous partial translations
    return - (float) curr.CalcDistortion(prev);
	}
}

WordPenaltyProducer::WordPenaltyProducer(ScoreIndexManager &scoreIndexManager)
{
	scoreIndexManager.AddScoreProducer(this);
}

size_t WordPenaltyProducer::GetNumScoreComponents() const
{
	return 1;
}

const std::string WordPenaltyProducer::GetScoreProducerDescription(int idx) const
{
	return "word penalty";
}

UnknownWordPenaltyProducer::UnknownWordPenaltyProducer(ScoreIndexManager &scoreIndexManager)
{
	scoreIndexManager.AddScoreProducer(this);
}

size_t UnknownWordPenaltyProducer::GetNumScoreComponents() const
{
	return 1;
}

const std::string UnknownWordPenaltyProducer::GetScoreProducerDescription(int idx) const
{
	return "unknown word penalty";
}

