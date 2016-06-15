#include "Signals.h"
#include "AttackClass.h"

namespace
{
	FString s_names[] =
	{
		TEXT("physical"),
		TEXT("fire"),
		TEXT("ice"),
		TEXT("electro"),
		TEXT("light"),
		TEXT("plasma"),
		TEXT("sound"),
		TEXT("poison"),
		TEXT("bacterial"),
		TEXT("viral")
	};
}

namespace AttackClass
{
	EAttackClass FromString(FString const & str)
	{
		checkf(sizeof(s_names) / sizeof(s_names[0]) == (int)EAttackClass::NumClasses, 
			   TEXT("Mismatch between enum EAttackClass and string reps"));

		auto lcName = str.ToLower();
		for (int i = 0; i < EAttackClass::NumClasses; ++i)
		{
			if (lcName == s_names[i])
			{
				return (EAttackClass)i;
			}
		}

		checkf(false, TEXT("Invalid class name '%s'"), *str);

		return EAttackClass::NumClasses;
	}

	FString ToString(EAttackClass attackClass)
	{
		checkf(sizeof(s_names) / sizeof(s_names[0]) == (int)EAttackClass::NumClasses,
			TEXT("Mismatch between enum EAttackClass and string reps"));

		return s_names[attackClass];
	}
}