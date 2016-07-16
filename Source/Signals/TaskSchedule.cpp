#include "Signals.h"
#include "TaskSchedule.h"

static bool less(TaskBase & a, TaskBase & b)
{
	return( a.GetNextRunFrame() < b.GetNextRunFrame() );
}

TaskBase::TaskBase(int interval, int iterations, bool mandatory)
: _interval(interval)
, _iterations(iterations)
, _mandatory(mandatory)
{
	check(interval > 0);
	check(iterations != 0);
}

TaskScheduleBase::TaskScheduleBase()
: _currentFrame(-1)
{
	_tasks.Heapify(less);
}

void TaskScheduleBase::NextFrame()
{
	++_currentFrame;
}

void TaskScheduleBase::Clear()
{
	for (auto task : _tasks)
	{
		delete task;
	}
	_tasks.Empty();
	_currentFrame = 0;
}

void TaskScheduleBase::schedule(TaskBase * task)
{
	task->schedule(_currentFrame);
	_tasks.HeapPush(task, less);
}

TaskBase * TaskScheduleBase::popTask()
{
	if (_tasks.Num() == 0)
	{
		return nullptr;
	}

	TaskBase * task;
	_tasks.HeapPop(task, less);
	return task;
}

TaskBase * TaskScheduleBase::dequeue()
{
	if (_tasks.Num() == 0)
	{
		return nullptr;
	}

	auto task = _tasks.HeapTop();
	if (task->GetNextRunFrame() > _currentFrame)
	{
		return nullptr;
	}

	_tasks.HeapPopDiscard(less);
	bool reschedule = false;
	if (task->GetNumRunsLeft() < 0)
	{
		// Infinite runs
		reschedule = true;
	}
	else if (task->GetNumRunsLeft() > 0)
	{
		task->iterate();
		if (task->GetNumRunsLeft() > 0)
		{
			reschedule = true;
		}
	}

	if (reschedule)
	{
		schedule(task);
	}
	else if (_tasks.Num() == 0)
	{
		_currentFrame = 0;
	}

	return task;
}

