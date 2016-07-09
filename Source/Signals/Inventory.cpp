#include "Signals.h"
#include "Inventory.h"
#include "Item.h"

void Inventory::FromXml(FXmlNode * node)
{
	for (auto itemNode : node->GetChildrenNodes())
	{
		auto idStr = itemNode->GetAttribute(TEXT("id"));
		int itemID = FCString::Atoi(*idStr);
		auto countStr = itemNode->GetAttribute(TEXT("count"));
		int count = FCString::Atoi(*countStr);
		_itemTable.Add(itemID, count);
	}
}

void Inventory::Clear()
{
	_itemTable.Empty();
}

void Inventory::AddItem(int itemID)
{
	UE_LOG(SignalsLog, Log, TEXT("Inventory: Add Item %d"), itemID);

	check(itemID >= 0);

	// Any existing entries for this item type?
	auto pcount = _itemTable.Find(itemID);
	if (pcount == nullptr)
	{
		// Nope, first of its kind.
		_itemTable.Add(itemID, 1);
	}
	else
	{
		// Yup, one more them.
		_itemTable[itemID] = (*pcount + 1);
	}
}

void Inventory::RemoveItem(int itemID)
{
	UE_LOG(SignalsLog, Log, TEXT("Inventory: Remove Item %d"), itemID);

	check(itemID >= 0);

	auto pcount = _itemTable.Find(itemID);
	if (pcount != nullptr)
	{
		int count = *pcount-1;
		check(count >= 0);
		if (count == 0)
		{
			_itemTable.Remove(itemID);
		}
		else
		{
			_itemTable[itemID] = count;
		}
	}
}

TArray<FInventoryEntry> Inventory::GetItems() const
{
	TArray<FInventoryEntry> result;

	for (auto & kv : _itemTable)
	{
		auto item = Item::FindByID(kv.Key);
		check(item != nullptr);

		FInventoryEntry entry;
		entry.ItemID = kv.Key;
		entry.Count = kv.Value;
		entry.Name = item->GetName();
		entry.Description = item->GetDescription();
		entry.IconIndex = item->GetIcon();
		result.Emplace(entry);
	}

	return result;
}

TArray<FInventoryEntry> Inventory::GetItems(EItemProperty filter) const
{
	TArray<FInventoryEntry> result;

	for (auto & kv : _itemTable)
	{
		auto item = Item::FindByID(kv.Key);
		check(item != nullptr);

		if (((filter == EItemProperty::CanUseProperty) && item->IsUseable()) ||
			((filter == EItemProperty::CanTransformProperty) && item->IsTransformable()) ||
			((filter == EItemProperty::CanEquipProperty) && item->IsEquippable()))
		{
			FInventoryEntry entry;
			entry.ItemID = kv.Key;
			entry.Count = kv.Value;
			entry.Name = item->GetName();
			entry.Description = item->GetDescription();
			entry.IconIndex = item->GetIcon();
			result.Add(entry);
		}
	}

	return result;
}
