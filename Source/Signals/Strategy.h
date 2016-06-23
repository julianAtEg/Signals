#pragma once

class FXmlNode;
class ASignalsBattleMode;
struct Combatant;

/**
 * Core AI class for NPC decision-making in battle.
 */
class Strategy
{
public:
	typedef Strategy * (*Ctor)();

	virtual ~Strategy();
	virtual bool Run(ASignalsBattleMode * battle, Combatant * npc) = 0;
	virtual void FromXml(FXmlNode * node) = 0;

	static Strategy * Create(FString const & name);
	static Ctor RegisterCtor(FString const & type, Ctor ctor);

protected:
	Strategy();

private:
	
};
