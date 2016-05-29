#pragma once

#include "Action.h"

class PlayAudioNode : public ActionNode
{
public:
	PlayAudioNode();

	void FromXml(FXmlNode const * node) override;
	void Execute(UWorld * world, Combatant * source, TArray<Combatant *> const &) override;

private:
	FString _audioFile;
};