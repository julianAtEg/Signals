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

private:
	PriorityQueue<int> _queue;

};
