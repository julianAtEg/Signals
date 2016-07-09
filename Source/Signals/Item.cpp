// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "Item.h"
#include "SignalsBattleMode.h"
#include "ActionRunner.h"

//-----------------------------------------------------------------------------

static TMap<int, Item *> s_itemTable;

//-----------------------------------------------------------------------------

/*static*/
void Item::Initialize()
{
	UE_LOG(SignalsLog, Log, TEXT("Loading item data"));

	check(s_itemTable.Num() == 0); // Only call once!

	auto contentFolder = FPaths::GameContentDir();
	auto itemsPath = FPaths::Combine(*contentFolder, TEXT("Data/items.xml"));
	FXmlFile itemsXml;
	if (!itemsXml.LoadFile(itemsPath, EConstructMethod::Type::ConstructFromFile))
	{
		UE_LOG(SignalsLog, Error, TEXT("Error loading items.xml"));
	}

	auto root = itemsXml.GetRootNode();
	auto & children = root->GetChildrenNodes();
	for (auto itemNode : children)
	{
		auto item = new Item();
		item->FromXml(itemNode);
		s_itemTable.Add(item->GetID(), item);
	}

}

/*static*/
void Item::Terminate()
{
	for (auto & kv : s_itemTable)
	{
		delete kv.Value;
	}
	s_itemTable.Empty();
}

/*static*/
Item * Item::FindByID(int id)
{
	auto pitem = s_itemTable.Find(id);
	if (pitem != nullptr)
	{
		return *pitem;
	}
	else
	{
		return nullptr;
	}
}

Item::Item()
: _id(-1)
, _name(TEXT(""))
, _description(TEXT(""))
, _root()
, _useNode(nullptr)
, _equipNode(nullptr)
, _unequipNode(nullptr)
, _ergValue(-1)
, _isCombinable(false)
, _icon(0)
{

}

void Item::FromXml(FXmlNode * node)
{
	auto idStr = node->GetAttribute(TEXT("id"));
	_id = FCString::Atoi(*idStr);
	_name = node->GetAttribute(TEXT("name"));
	_description = node->GetAttribute(TEXT("description"));
	auto ergStr = node->GetAttribute(TEXT("ergValue"));
	_ergValue = FCString::Atoi(*ergStr);
	auto combStr = node->GetAttribute(TEXT("combinable"));
	combStr.ToLowerInline();
	_isCombinable = (combStr == TEXT("true"));
	auto iconStr = node->GetAttribute(TEXT("icon"));
	_icon = FCString::Atoi(*iconStr);

	// Read children.
	_root.FromXml(node);

	// Lashup of actions.
	_useNode = (ContainerNode *)_root.FindChildOfType(TEXT("use"));
	_equipNode = (ContainerNode *)_root.FindChildOfType(TEXT("equip"));
	_unequipNode = (ContainerNode *)_root.FindChildOfType(TEXT("unequip"));
}

void Item::Use(ASignalsBattleMode * const battle, Combatant * const user)
{
	check(IsUseable());

	ActionRunner runner;
	runner.RunAtomic(battle, _useNode);
}

void Item::Equip(ASignalsBattleMode * const battle, Combatant * const user)
{
	check(IsEquippable());
	check(_unequipNode != nullptr);

	ActionRunner runner;
	runner.RunAtomic(battle, _equipNode);
}

void Item::Unequip(ASignalsBattleMode * const battle, Combatant * const user)
{
	check(IsEquippable());
	check(_unequipNode != nullptr);

	ActionRunner runner;
	runner.RunAtomic(battle, _unequipNode);
}

int Item::Combine(TArray<Item *> const & others)
{
	// TODO: combination table.
	return -1;
}