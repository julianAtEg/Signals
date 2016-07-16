#pragma once

enum EPlayerStatus
{
	// Player selects random attacks / targets
	Deranged,

	// Player loses health every turn.
	Sick,

	// Player speed drops by 10 (Cancels Fast)
	Slow,

	// Player speed increases by 10 (cancels Slow)
	Fast,

	// Dexterity and evasion drops by 10 (cancels Sharp)
	Sloppy,

	// Dexterity and evasion increases by 10 (Cancel Sloppy)
	Sharp,

	// Strength and defence down by 5
	Weak,

	// Strength and defence up by 5
	Titan,
};
