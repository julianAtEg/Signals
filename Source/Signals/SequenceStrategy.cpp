#include "Signals.h"
#include "SequenceStrategy.h"

//-----------------------------------------------------------------------------

static Strategy * ctor() { return new SequenceStrategy(); }
static const FString s_type(TEXT("sequence"));
static Strategy::Ctor s_ctor = Strategy::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

class SequenceNode
{
public:
	SequenceNode() = default;
	~SequenceNode() = default;
	virtual void FromXml(FXmlNode * node) = 0;
	virtual bool Check(TArray<WeightedAction> const & options) const = 0;
	virtual SelectionResult Run(SequenceStrategy * strategy, ASignalsBattleMode * battle, Combatant * npc) = 0;
};

class ActionNode : public SequenceNode
{
public:
	void FromXml(FXmlNode * node) override
	{
		// TODO
	}

	bool Check(TArray<WeightedAction> const & options) const override
	{
		// TODO
		return true;

	}

	SelectionResult Run(SequenceStrategy * strategy, ASignalsBattleMode * battle, Combatant * npc) override
	{
		// TODO
		return SelectionResult::Failed;
	}
};

class ChooseNode : public SequenceNode
{
public:
	void FromXml(FXmlNode * node) override
	{
		// TODO
	}

	bool Check(TArray<WeightedAction> const & options) const override
	{
		// TODO
		return true;
	}

	SelectionResult Run(SequenceStrategy * strategy, ASignalsBattleMode * battle, Combatant * npc) override
	{
		// TODO
		return SelectionResult::Failed;
	}

private:
	TArray<WeightedAction> _choices;
};


//-----------------------------------------------------------------------------

class SequenceActionSelector : public ActionSelectionPolicy
{
	SelectionResult Run(BasicStrategy * strategy, TArray<WeightedAction> const & options, ASignalsBattleMode * battle, Combatant * npc) override
	{
		SequenceStrategy * sequence = static_cast<SequenceStrategy *>(strategy);
		for( int count = 0; count < sequence->GetLength(); ++count )
		{
			int index = sequence->Advance();
			auto node = sequence->GetNode(index);
			if (node->Check(options))
			{
				auto rc = node->Run(sequence, battle, npc);
				if ((rc & SelectionResult::Succeeded) != 0)
				{
					return rc;
				}
			}
		}

		return SelectionResult::Failed;
	}
};

static SequenceActionSelector s_sas;

//-----------------------------------------------------------------------------

SequenceStrategy::SequenceStrategy()
: BasicStrategy()
, _currentIndex(-1)
, _sequence()
{

}

SequenceStrategy::~SequenceStrategy()
{
	for (auto node : _sequence)
	{
		delete node;
	}
}

void SequenceStrategy::FromXml(FXmlNode * node)
{
	BasicStrategy::FromXml(node);

	// TODO:

	setActionSelector(&s_sas);
}

int SequenceStrategy::Advance()
{
	++_currentIndex;
	if (_currentIndex >= _sequence.Num())
	{
		_currentIndex = 0;
	}

	return _currentIndex;
}