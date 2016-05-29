#pragma once

class FXmlNode;

struct Ability
{
	// The name of the skill.
	FString Name;

	// Measures luck vs skill. At 0% skill, it's all luck (skewed distribution). At 100%
	// it's almost all skill (distribution peaked about level).
	int Skill;

	// Maximum level the skill can attain for this character.
	int MaxSkill;

	// Current ability points.
	int AP;

	// Load state from an XML ability node. If isDynamic is true, loads dynamic
	// state as well as static, otherwise sets dynamic parameters to initial values.
	void FromXml(FXmlNode * const node, bool isDynamic);
};

