#pragma once

#include "BasicStrategy.h"

class SequenceNode;

class SequenceStrategy : public BasicStrategy
{
public:
	SequenceStrategy();
	~SequenceStrategy();

	void FromXml(FXmlNode * node) override;
	int Advance();
	SequenceNode * GetNode(int index) const;
	int GetLength() const;

private:
	int _currentIndex;
	TArray<SequenceNode *> _sequence;
};

inline SequenceNode * SequenceStrategy::GetNode(int index) const
{
	return _sequence[index];
}

inline int SequenceStrategy::GetLength() const
{
	return _sequence.Num();
}
