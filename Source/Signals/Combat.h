#pragma once

class Random;

namespace Combat
{
	// Loads combat data from a file.
	void LoadData(FString const & fileName);

	// Pseudo-random hit / miss determination based on agility values.
	// Returns true if the result is "hit".
	bool GetHitOrMiss( Random * const random, int attackAgility, int defenceEvasion );

	// Computes the semi-random luck factor given a skill level.
	float ComputeLuck( Random * const random, int skill );

	// Gets the boost time in ms for the supplied level.
	int GetBoostTime(int level);

	// Computes the amount of boost given the fraction left and the player level.
	float ComputeBoostFactor( float boostTimeFrac, int level );
}
