#include "Signals.h"
#include "CachedRandom.h"

static const int CACHE_SIZE = 6;

CachedRandom::CachedRandom(unsigned seed)
: Random(seed)
{
}

float CachedRandom::Next01()
{
	if (_cache.Num() == 0)
	{
		fillCache(CACHE_SIZE);
	}

	float value = _cache[0];
	_cache.RemoveAt(0);
	return value;
}

TArray<float> CachedRandom::Peek(int count)
{
	if (_cache.Num() < count)
	{
		fillCache(count - _cache.Num());
	}

	return _cache;
}

void CachedRandom::fillCache(int count)
{
	for (int i = 0; i < count; ++i)
	{
		_cache.Add(Random::Next01());
	}
}