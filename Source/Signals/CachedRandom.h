#pragma once

#include "Random.h"
#include "Array.h"

class CachedRandom : public Random
{
public:
	CachedRandom(unsigned seed);

	float Next01() override;
	TArray<float> Peek(int count);

private:
	void fillCache( int count );

	TArray<float> _cache;
};

