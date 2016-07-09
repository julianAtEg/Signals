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
	Item();
	~Item() = default;

	void FromXml(FXmlNode * node);

	bool IsUseable() const;
	void Use(ASignalsBattleMode * const battle, Combatant * const user);

	bool IsEquippable() const;
	void Equip(ASignalsBattleMode * const battle, Combatant * const user);
	void Unequip(ASignalsBattleMode * const battle, Combatant * const user);

	bool IsTransformable() const;
	int GetTransformValue() const;

	bool IsCombinable() const;
	int Combine(TArray<Item *> const & others);

	int GetID() const;
	FString const & GetName() const;
	FString const & GetDescription() const;
	int GetIcon() const;

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
	ContainerNode * _unequipNode;
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