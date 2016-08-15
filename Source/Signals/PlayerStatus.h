#pragma once

struct Combatant;
class StatModifier;

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
	// Converts a string to a status.
	EPlayerStatus FromString(FString const & str);

	// Convets a status to a string.
	FString ToString(EPlayerStatus status);

	// Applies a status to a player. Returns an array of stat modifiers which
	// should be removed at some point by a task if the status is not sticky.
	TArray<StatModifier *> Apply(EPlayerStatus status, Combatant * player);

	// Determines if a status affects the turn schedule.
	bool AffectsSchedule(EPlayerStatus status);

	// Gets the inverse status of a status (ef 'fast' for 'slow') or NumStatusTypes
	// if none.
	EPlayerStatus GetAntiStatus(EPlayerStatus status);
}