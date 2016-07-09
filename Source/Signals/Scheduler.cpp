// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "Scheduler.h"

Scheduler::Scheduler()
{
}

Scheduler::~Scheduler()
{
}

Scheduler::Scheduler(Scheduler const & that)
: _queue(that._queue)
{

}

Scheduler & Scheduler::operator=(Scheduler const & that)
{
	if (this != &that)
	{
		_queue = that._queue;
	}

	return *this;
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

void Scheduler::Clear()
{
	_queue.Clear();
}

int Scheduler::Peek(int n) const
{
	return _queue.Peek(n);
}