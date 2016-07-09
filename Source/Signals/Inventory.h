#pragma once

#include "Inventory.generated.h"

class Item;

UENUM(BlueprintType)
enum EItemProperty
{
	CanUseProperty,
	CanEquipProperty,
	CanTransformProperty,
};

USTRUCT(BlueprintType)
struct FInventoryEntry
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int IconIndex;
};

/**
 * Holds the items owned by a player.
 */
class Inventory
{
public:
	Inventory() = default;
	~Inventory() = default;

	// Load inventory data.
	void FromXml(FXmlNode * node);

	// Get the items in the inventory.
	TArray<FInventoryEntry> GetItems() const;
	TArray<FInventoryEntry> GetItems( EItemProperty filter ) const;

	// Adds an item to the inventory.
	void AddItem(int itemID);

	// Removes an item from the inventory.
	void RemoveItem(int itemID);

	// Removes all items.
	void Clear();

private:
	// Map of item ID to count.
	TMap<int, int> _itemTable;
};
