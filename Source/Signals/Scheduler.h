// (c) Smoke and Mirrors Games 2016

#pragma once

#include "PriorityQueue.h"

/**
 * Class responsible for scheduling character turns in the game.
 */
class SIGNALS_API Scheduler
{
public:
	Scheduler();
	~Scheduler();
	Scheduler(Scheduler const & that);
	Scheduler & operator=(Scheduler const & that);

	// Adds the indexed player to the schedule at the given speed.
	void Add(int playerIndex, float delay);

	// Pulls the next player index from the queue.
	int Next();

	// Removes the player index entirely from the schedule.
	void Cancel(int playerIndex);

	// Removes all entries.
	void Clear();

	// Number of elements in the schedule.
	int Size() const;

	// What's in the schedule at the given index?
	int Peek(int index) const;

private:
	PriorityQueue<int> _queue;

};

inline int Scheduler::Size() const
{
	return _queue.Length();
}