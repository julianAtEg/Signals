#include "Signals.h"
#include "Combat.h"
#include "Random.h"
#include "XmlParser.h"

// Luck parameters.
static float s_luckLoCoefficients[2] =
{
	0.5f, 0.9f
};

static float s_luckHiCoefficients[2] =
{
	1.1f, 1.5f
};

static int s_luckSize = 10;

// Hit / miss parameters.
static float s_hit1v1 = 80.0f;		// Percent chance of a player of agility 1 hitting a player of evasion 1
static float s_hit100v1 = 99.0f;	// Percent chance of a player of agility 100 hitting a player of evasion 1
static float s_hit1v100 = 20.0f;	// Percent chance of a player of agility 1 hitting a player of evasion 100
static float s_hit100v100 = 94.0f;	// Percent chance of a player of agility 100 hitting a player of evasion 100

// Boost parameters.
static float s_boostMax1 = 0.5f;
static float s_boostMax100 = 1.5f;
static float s_boostTime1 = 10.0f;
static float s_boostTime100 = 3.5f;

namespace Combat
{
	void Load(FString const & fileName)
	{
		UE_LOG(SignalsLog, Log, TEXT("Loading combat data"));

		auto contentDir = FPaths::GameContentDir();
		auto filePath = FPaths::Combine(*contentDir, TEXT("Data"), *fileName);
		FXmlFile dataXml;
		if (!dataXml.LoadFile(filePath, EConstructMethod::Type::ConstructFromFile))
		{
			UE_LOG(SignalsLog, Error, TEXT("Error loading %s"), *fileName);
			return;
		}

		auto root = dataXml.GetRootNode();

		// Load luck data.
		auto luckNode = root->FindChildNode(TEXT("luck"));
		auto lo1 = luckNode->GetAttribute(TEXT("lo1"));
		s_luckLoCoefficients[0] = FCString::Atof(*lo1);
		auto lo2 = luckNode->GetAttribute(TEXT("lo2"));
		s_luckLoCoefficients[1] = FCString::Atof(*lo2);
		auto hi1 = luckNode->GetAttribute(TEXT("hi1"));
		s_luckHiCoefficients[0] = FCString::Atof(*hi1);
		auto hi2 = luckNode->GetAttribute(TEXT("hi2"));
		s_luckHiCoefficients[1] = FCString::Atof(*hi2);
		auto size = luckNode->GetAttribute(TEXT("size"));
		s_luckSize = FCString::Atoi(*size);

		// Load hit / miss data.
		auto hitNode = root->FindChildNode(TEXT("hit"));

		auto h1v1str = hitNode->GetAttribute(TEXT("hit1v1"));
		s_hit1v1 = FCString::Atof(*h1v1str);

		auto h1v100str = hitNode->GetAttribute(TEXT("hit1v100"));
		s_hit1v100 = FCString::Atof(*h1v100str);

		auto h100v1str = hitNode->GetAttribute(TEXT("hit100v1"));
		s_hit100v1 = FCString::Atof(*h100v1str);
		
		auto h100v100str = hitNode->GetAttribute(TEXT("hit100v100"));
		s_hit100v100 = FCString::Atof(*h100v100str);

		// Load boost data.
		auto boostNode = root->FindChildNode(TEXT("boost"));
		auto max1str = boostNode->GetAttribute(TEXT("max1"));
		s_boostMax1 = FCString::Atof(*max1str);
		auto max100str = boostNode->GetAttribute(TEXT("max100"));
		s_boostMax100 = FCString::Atof(*max100str);
		auto time1str = boostNode->GetAttribute(TEXT("time1"));
		s_boostTime1 = FCString::Atof(*time1str);
		auto time100str = boostNode->GetAttribute(TEXT("time100"));
		s_boostTime100 = FCString::Atof(*time100str);
	}

	float ComputeLuck(Random * const random, int skill)
	{
		check(skill >= 0 && skill <= 100);

		// At high luck levels (low skill) we're more likely to hit lower than higher.
		// At 50% luck we're equally likely to score higher or lower than the amount.
		// At low luck levels (high skill) we are more likely to score higher than lower.
		float luck = (100.0f - skill) / 100.0f;
		auto slo = s_luckLoCoefficients[0] * luck + s_luckLoCoefficients[1] * (1.0f - luck);
		auto shi = s_luckHiCoefficients[0] * luck + s_luckHiCoefficients[1] * (1.0f - luck);
		int lo = FMath::Max((int)(s_luckSize*slo), 0);
		int hi = FMath::Max((int)(s_luckSize*shi), 0);
		int range = (hi - lo);
		check(range > 0);

		// Build an up / down ramp starting at lo, peaking at LUCK_SIZE, then falling off to 0 at hi
		//
		//    /|`
		//	 / |  `
		//	/-------`
		//
		// This will be skewed to the left for low luck, and to the right for high luck.
		TArray<int> weights;
		int weight = 1;
		int delta = FMath::Max(s_luckSize / (s_luckSize - lo), 1);
		int i;
		for (i = lo; i <= s_luckSize; ++i)
		{
			weights.Add(weight);
			weight += delta;
		}

		delta = FMath::Max(s_luckSize/ (hi - s_luckSize), 1);
		for (; i <= hi; ++i)
		{
			weight -= delta;
			weights.Add(FMath::Max(weight, 1));
		}

		// Sample from the distribution we created, and compute the scale factor.
		WeightedSampler sampler(random, weights);
		return double(sampler.Next() + lo) / s_luckSize;
	}

	bool GetHitOrMiss(Random * const random, int attackAgility, int defenceEvasion)
	{
		check(attackAgility > 0 && attackAgility <= 100);
		check(defenceEvasion > 0 && defenceEvasion <= 100);

		// 2D linear interpolation.
		auto s = (defenceEvasion - 1) / 99.0f;
		auto top = (1.0f - s)*s_hit1v1 + s*s_hit1v100;
		auto bot = (1.0f - s)*s_hit100v1 + s*s_hit100v100;
		auto t = (attackAgility - 1) / 99.0f;
		int percent = (int)((1.0 - t)*top + t*bot);
		int roll = random->NextInt(0, 100);
		return(roll < percent);
	}

	int GetBoostTime(int level)
	{
		auto s = float(level - 1) / 99.0f;
		return((1.0f - s)*s_boostTime1 + s*s_boostTime100);
	}

	float ComputeBoostFactor(float boostTimeFrac, int level)
	{
		// Work out the maxiumum boost for the level you're at.
		auto s = float(level-1)/ 99.0f;
		auto maxBoost = s_boostMax1*(1.0f - s) + s*s_boostMax100;

		return(1.0f + boostTimeFrac * maxBoost);
	}
}