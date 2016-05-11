// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "Scheduler.h"

Scheduler::Scheduler()
{
}

Scheduler::~Scheduler()
{
}

void Scheduler::Add(int playerIndex, float delay)
{
	_queue.Enqueue(playerIndex, delay);
}

int Scheduler::Next()
{
	auto e = _queue.DequeueWithKey();
	_queue.AdjustPriorities(-e.priority);
	return e.item;
}

void Scheduler::Cancel(int playerIndex)
{
	_queue.Erase(playerIndex);
}
