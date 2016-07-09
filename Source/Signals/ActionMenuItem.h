#pragma once

#include "ActionMenuItem.generated.h"

enum ItemType
{
	RunAction,
	UseInventoryItem,
};

USTRUCT(BlueprintType)
struct FActionMenuItem
{
	GENERATED_USTRUCT_BODY()

	// Item (terse) text.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Text;

	// Index of the icon to use in the menu (see SignalsBattleModeBP)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int IconIndex;

	// Verbose description of the item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Description;

	// Unique ID of the item.
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "UI")
	int ID;

	// Is this an action node or a category node?
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "UI")
	bool IsLeaf;

	// Can the user choose this option?
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "UI")
	bool IsSelectable;

	// Menu level. 0 is action bar, 1 is ring menu.
	int Level;

	// Parent (Category) ID. -1 = no parent.
	int ParentID;

	// What the item should do.
	ItemType ItemType;
};