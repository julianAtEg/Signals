#pragma once

enum EDuration
{
	dtNone,			// Undefined
	dtPermanent,	// Lasts forever
	dtBattle,		// Lasts for the duration of the battle
	dtTurns,		// Lasts for a number of turns.
};

namespace ActionDuration
{
	EDuration FromString(FString const & str);
}
