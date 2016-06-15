// (c) Smoke and Mirrors Games 2016

#pragma once

/**
 * Random number generator. 
 */
class Random
{
public:
	Random( unsigned seed );
	virtual ~Random();

	virtual float Next01();
	float Gaussian01();
	float HalfGaussian01();
	int NextInt(int min, int max);

private:
	unsigned m_next;
};

class WeightedSampler
{
public:
	WeightedSampler(Random * const rng, TArray<int> const & weights);

	void SetWeights(TArray<int> const & weights);
	int Next();
private:
	Random * const _rng;
	TArray<int> _lut;
};