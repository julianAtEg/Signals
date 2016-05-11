#pragma once

#include "Array.h"


template<class T>
class PriorityQueue
{
public:
	struct Entry
	{
		T item;
		float priority;
	};

	PriorityQueue()
	{
	}

	~PriorityQueue()
	{
	}

	PriorityQueue( PriorityQueue<T> const & that )
	: _queue( that._queue )
	{
		
	}
	
	PriorityQueue & operator=( PriorityQueue<T> const & that )
	{
		if (this != &that)
		{
			_queue = that._queue;
		}
		
		return *this;
	}
	
	int Length() const
	{
		return _queue.Num();
	}

	void Enqueue( T item, float priority )
	{
		Entry e;
		e.item = item;
		e.priority = priority;
	
		int finalPos = 0;
		int high = Length();
		while( finalPos < high )
		{
			int middle = (finalPos + high)/2;
			if (priority < _queue[middle].priority)
			{
				high = middle;
			}
			else
			{
				finalPos = middle+1;
			}
		}
	
		_queue.InsertAt( finalPos, e );
	}

	void AdjustPriorities( float delta )
	{
		for( int i = 0; i < _queue.Size(); ++i )
		{
			auto & e = _queue[i];
			e.priority += delta;
		}
	}

	T Dequeue()
	{
		auto e = _queue[0];
		_queue.RemoveAt(0);
		return e.item;
	}

	Entry DequeueWithKey()
	{
		auto e = _queue[0];
		_queue.RemoveAt(0);
		return e;
	}

	void Erase( T value )
	{
		_queue.Remove(value);
	}

private:

	TArray<Entry> _queue;
};

