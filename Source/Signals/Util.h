#pragma once

#include "Util.generated.h"

UCLASS()
class UUtil : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Util")
	static int GetNumberOfSetBits(int value);

	UFUNCTION(BlueprintCallable, Category = "Util")
	static int GetNextSetBitIndex(int start, int value);
};
