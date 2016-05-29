#include "signals.h"
#include "Ability.h"
#include "XmlParser.h"

void Ability::FromXml(FXmlNode * const node, bool isDynamic)
{
	Name = node->GetAttribute(TEXT("name"));
	auto maxSkillStr = node->GetAttribute(TEXT("maxSkill"));
	MaxSkill = FCString::Atoi(*maxSkillStr);
	if (isDynamic)
	{
		auto apStr = node->GetAttribute(TEXT("ap"));
		AP = FCString::Atoi(*apStr);
		auto skillStr = node->GetAttribute(TEXT("skill"));
		Skill = FCString::Atoi(*skillStr);
	}
	else
	{
		AP = 0;
		Skill = 0;
	}
}
