#include "Signals.h"
#include "WeightedAction.h"
#include "Action.h"

WeightedAction::WeightedAction()
: ActionName(TEXT(""))
, Weight(0)
, _action(nullptr)
{
}

Action * WeightedAction::GetAction() const
{
	if (_action == nullptr)
	{
		_action = Action::FindAction(ActionName);
	}

	return _action;
}