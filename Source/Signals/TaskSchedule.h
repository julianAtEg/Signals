#pragma once

class TaskBase
{
	friend class TaskScheduleBase;
public:
	virtual ~TaskBase() = default;

	int GetInterval() const;
	int GetNextRunFrame() const;
	int GetNumRunsLeft() const;
	bool HasExpired() const;
	bool IsMandatory() const;

protected:
	TaskBase(int interval = 1, int iterations = -1, bool mandatory=true);

private:
	void schedule(int thisFrame);
	void iterate();

	int _interval;
	int _nextRunFrame;
	int _iterations;
	bool _mandatory;
};

inline int TaskBase::GetInterval() const { return _interval; }
inline int TaskBase::GetNextRunFrame() const { return _nextRunFrame; }
inline int TaskBase::GetNumRunsLeft() const { return _iterations; }
inline bool TaskBase::HasExpired() const { return _iterations == 0; }
inline bool TaskBase::IsMandatory() const { return _mandatory; }
inline void TaskBase::iterate() { if (_iterations > 0) { --_iterations; } }
inline void TaskBase::schedule( int thisFrame ) { _nextRunFrame = thisFrame + _interval; }

//-----------------------------------------------------------------------------

template<class T>
class Task : public TaskBase
{
public:
	typedef T OperandType;
	
	Task( int interval=1, int iterations=-1, bool mandatory=true )
	: TaskBase( interval, iterations, mandatory )
	{
		
	}
	
	virtual ~Task() = default;
	
	virtual bool Run( T * object ) = 0;
};

//-----------------------------------------------------------------------------

class TaskScheduleBase
{
public:
	TaskScheduleBase();
	virtual ~TaskScheduleBase() = default;

	// Advances to the next frame.
	void NextFrame();

	// Removes all tasks from the schedule.
	void Clear();

protected:
	TaskBase * dequeue();
	void schedule(TaskBase * task);
	TaskBase * popTask();

private:
	int _currentFrame;
	TArray<TaskBase* > _tasks;
};

//-----------------------------------------------------------------------------

template<class T>
class TaskSchedule : public TaskScheduleBase
{
public:
	typedef typename T::OperandType OperandType;

	// Schedule a task to be run.
	void Schedule(T * task)
	{
		schedule(task);
	}
	
	// Runs all tasks that are currently pending on the current turn.
	void RunPendingTasks( OperandType * object )
	{
		NextFrame();
		T * task;
		while( (task = static_cast<T *>(dequeue())) != nullptr )
		{
			task->Run(object);
			if (task->HasExpired())
			{
				delete task;
			}
		}
	}

	// Cleans up the schedule, running all mandatory tasks and freeing memory.
	void Finish(OperandType * object)
	{
		T * task;
		while ((task = static_cast<T *>(popTask())) != nullptr)
		{
			if (task->IsMandatory())
			{
				task->Run(object);
			}
			delete task;
		}
	}
};

