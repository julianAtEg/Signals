#include "Signals.h"
#include "ActionClass.h"

namespace
{
	FString s_names[] =
	{
		TEXT("offensive"),
		TEXT("defensive"),
		TEXT("restorative"),
		TEXT("neutral"),
	};
}

namespace ActionClass
{
	EActionClass FromString(FString const & str)
	{
		checkf(sizeof(s_names) / sizeof(s_names[0]) == (int)EActionClass::NumActionClasses,
			TEXT("Mismatch between enum EActionClass and string reps"));

		auto lcName = str.ToLower();
		for (int i = 0; i < EActionClass::NumActionClasses; ++i)
		{
			if (lcName == s_names[i])
			{
				return (EActionClass)i;
			}
		}

		checkf(false, TEXT("Invalid class name '%s'"), *str);

		return EActionClass::NumActionClasses;
	}

	FString ToString(EActionClass actionClass)
	{
		checkf(sizeof(s_names) / sizeof(s_names[0]) == (int)EActionClass::NumActionClasses,
			TEXT("Mismatch between enum EActionClass and string reps"));

		return s_names[actionClass];
	}
}