#pragma once

template<class T>
class TStack
{
public:
	TStack() = default;
	~TStack() = default;
	TStack(TStack<T> const & that) = default;
	TStack<T> & operator=(TStack<T> const & that) = default;

	bool IsEmpty() const
	{
		return(_items.Num() == 0);
	}

	void Clear()
	{
		_items.Empty();
	}

	void Push(T item)
	{
		_items.Add(item);
	}

	T Pop()
	{
		int index = _items.Num() - 1;
		T item = _items[index];
		_items.RemoveAt(index);
		return item;
	}

private:
	TArray<T> _items;
};
