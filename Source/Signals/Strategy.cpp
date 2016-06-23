#include "Signals.h"
#include "Strategy.h"

typedef TMap<FString, Strategy::Ctor> CtorMap;
static CtorMap & ctorMap();

//-----------------------------------------------------------------------------

/*static*/
Strategy::Ctor Strategy::RegisterCtor(FString const & type, Ctor ctor)
{
	UE_LOG(SignalsLog, Log, TEXT("Regstering strategy type '%s'"), *type);
	ctorMap().Add(type, ctor);
	return ctor;
}

/*static*/
Strategy * Strategy::Create(FString const & type)
{
	auto pctor = ctorMap().Find(type);
	checkf(pctor != nullptr, TEXT("Unknown strategy type '%s'"), *type);
	auto ctor = *pctor;
	return ctor();
}

Strategy::Strategy()
{	
}

Strategy::~Strategy()
{	
}

//-----------------------------------------------------------------------------

static CtorMap & ctorMap()
{
	static CtorMap map;
	return map;
}