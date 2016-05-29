#pragma once

#include "Signals.h"
#include "ActionMenuItem.generated.h"

USTRUCT(BlueprintType)
struct FActionMenuItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int IconIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "UI")
	int ID;

	// Is this an action node or a category node?
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "UI")
	bool IsLeaf;

	// Menu level. 0 is action bar, 1 is ring menu.
	int Level;

	// Parent (Category) ID. -1 = no parent.
	int ParentID;
};