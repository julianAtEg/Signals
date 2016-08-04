#include "Signals.h"
#include "ActionDuration.h"
#include <map>

namespace ActionDuration
{
	static std::map<FString,EDuration> s_map =
	{
		{ TEXT("none"), EDuration::dtNone },
		{ TEXT("permanent"), EDuration::dtPermanent },
		{ TEXT("battle"), EDuration::dtBattle },
		{ TEXT("turns"), EDuration::dtTurns }
	};

	EDuration FromString(FString const & str)
	{
		auto lc = str.ToLower();
		return s_map.at(lc);
	}
}