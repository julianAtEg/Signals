#pragma once

struct Combatant;

enum EPlayerStatus
{
	// Player selects random attacks / targets for N turns.
	Deranged,

	// Player loses health every turn
	Sick,

	// Player speed, dexterity, evasion drops by N (Cancels Fast)
	Slow,

	// Player speed, dexterity, evasion increases by N (cancels Slow)
	Fast,

	// Dexterity and evasion drops by N (cancels Sharp)
	Sloppy,

	// Dexterity and evasion increases by N (Cancel Sloppy)
	Sharp,

	// Strength and defence down by N
	Weak,

	// Strength and defence up by N
	Titan,

	// Won't participate in cooperative actions.
	Selfish,

	// Player cannot act for N turns.
	Paralyzed,

	// New ones above here
	NumStatusTypes
};

namespace PlayerStatus
{
	EPlayerStatus FromString(FString const & str);
	FString ToString(EPlayerStatus status);
	void Apply(EPlayerStatus status, Combatant * player);
	void Remove(EPlayerStatus status, Combatant * player);
	bool AffectsSchedule(EPlayerStatus status);
}