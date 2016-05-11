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

private:
	unsigned m_next;
};
