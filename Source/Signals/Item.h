// (c) Smoke and Mirrors Games 2016

#pragma once

#include "ContainerNode.h"
#include "ActionClass.h"

class ASignalsBattleMode;
struct Combatant;

/**
 * Class representing a potentially useable / equippable game object.
 */
class Item
{
public:	
	/** Lifecycle.*/
	Item();
	~Item() = default;

	// Load from XML.
	void FromXml(FXmlNode * node);

	// Can the item be used in a battle?
	bool IsUseable() const;

	// Use the item in a battle.
	void Use(ASignalsBattleMode * const battle, Combatant * const user);

	// Can the item be equipped?
	bool IsEquippable() const;

	// Can the item be transformed into ergs?
	bool IsTransformable() const;

	// Get the item's transform value in ergs.
	int GetTransformValue() const;

	// Can the item be combined with others?
	bool IsCombinable() const;

	// How many ergs does it cost to combine the items?
	int GetCombinationCost(Item * const that) const;

	// Combines two items, returns the ID of the new one.
	int Combine(Item * const that) const;

	// Gets basic data about the item.
	int GetID() const;
	FString const & GetName() const;
	FString const & GetDescription() const;
	int GetIcon() const;

	// Gets the equip node.
	ContainerNode * GetEquipNode() const;

	// Load item data.
	static void Initialize();
	static void Terminate();

	// Get item by ID.
	static Item * FindByID(int id);

private:
	int _id;
	FString _name;
	FString _description;
	ContainerNode _root;
	ContainerNode * _useNode;
	ContainerNode * _equipNode;
	int _ergValue;
	bool _isCombinable;
	int _icon;
};

inline int Item::GetID() const
{
	return _id;
}

inline FString const & Item::GetName() const
{
	return _name;
}

inline FString const & Item::GetDescription() const
{
	return _description;
}

inline bool Item::IsUseable() const
{
	return(_useNode != nullptr);
}

inline bool Item::IsEquippable() const
{
	return(_equipNode != nullptr);
}

inline bool Item::IsTransformable() const
{
	return(_ergValue >= 0);
}

inline bool Item::IsCombinable() const
{
	return(_isCombinable);
}

inline int Item::GetTransformValue() const
{
	return _ergValue;
}

inline int Item::GetIcon() const
{
	return _icon;
}

inline ContainerNode * Item::GetEquipNode() const
{
	return _equipNode;
}