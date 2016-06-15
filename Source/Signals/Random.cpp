// (c) Smoke and Mirrors Games 2016
#include "Signals.h"
#include "Random.h"

// Park-Miller data.
#define M 2147483647	/* 2^31 - 1 (A large prime number) */
#define A 16807			/* Prime root of M, passes statistical tests and produces a full cycle */
#define Q 127773		/* M / A (To avoid overflow on A * seed) */
#define R 2836			/* M % A (To avoid overflow on A * seed) */

//-----------------------------------------------------------------------------

static unsigned lcgParkMiller(unsigned seed);
static float boxMuller(Random * const rng, float m, float s);

template<class T>
inline T Abs(T x)
{
	return(x >= T(0) ? x : -x);
}

//-----------------------------------------------------------------------------

Random::Random( unsigned seed )
{
	checkf(seed > 0, TEXT("Random seed cannot be 0 or less"));

	m_next = lcgParkMiller(seed);
}

Random::~Random()
{
}

float Random::Next01()
{
	unsigned value = m_next;
	m_next = lcgParkMiller(value);
	return float(value) / float(0xFFFFFFFFU);
}

float Random::Gaussian01()
{
	return boxMuller(this, 0.0f, 1.0f);
}

float Random::HalfGaussian01()
{
	return Abs( boxMuller(this, 0.0f, 1.0f) );
}

int Random::NextInt(int min, int max)
{
	float range = float(max-min);
	float selected = float(min) + Next01()*range;
	return int(selected);
}

//-----------------------------------------------------------------------------

WeightedSampler::WeightedSampler(Random * const rng, TArray<int> const & weights)
: _rng( rng )
{
	SetWeights(weights);
}

void WeightedSampler::SetWeights(TArray<int> const & weights)
{
	_lut.Empty();
	for (int i = 0; i < weights.Num(); ++i)
	{
		for (int j = 0; j < weights[i]; ++j)
		{
			_lut.Add(i);
		}
	}
}

int WeightedSampler::Next()
{
	int index = _rng->NextInt(0, _lut.Num() - 1);
	return _lut[index];
}

//-----------------------------------------------------------------------------

static unsigned lcgParkMiller(unsigned seed)
{
	unsigned hi = seed / Q;
	unsigned lo = seed % Q;
	int test = A * lo - R * hi;
	if (test <= 0)
	{
		test += M;
	}
		
	return test;
}

static float boxMuller( Random * const rng, float m, float s )
{				        
	float x1, x2, w, y1;
	static float y2;
	static int useLast = 0;

	if (useLast)
	{
		y1 = y2;
		useLast = 0;
	}
	else
	{
		do
		{
			x1 = 2.0 * rng->Next01() - 1.0;
			x2 = 2.0 * rng->Next01() - 1.0;
			w = x1 * x1 + x2 * x2;
		} 
		while (w >= 1.0);

		w = sqrt((-2.0 * log(w)) / w);
		y1 = x1 * w;
		y2 = x2 * w;
		useLast = 1;
	}

	return(m + y1 * s);
}


