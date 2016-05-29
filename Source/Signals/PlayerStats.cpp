// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "PlayerStats.h"

UPlayerStats::UPlayerStats(FObjectInitializer const & init)
: Super(init)
, HitPoints( 0 )
, MaxHitPoints( 0 )
, Speed( 0 )
, Strength( 0 )
, Defence( 0 )
{

}

void UPlayerStats::Load(FString const & filePath)
{
	UE_LOG(SignalsLog, Log, TEXT("Loading player stats from %s"),*filePath);

	FXmlFile dataXml;
	if (!dataXml.LoadFile(filePath, EConstructMethod::Type::ConstructFromFile))
	{
		UE_LOG(SignalsLog, Error, TEXT("Error loading %s"), *filePath);
		return;
	}

	auto rootNode = dataXml.GetRootNode();
	fromXml(rootNode);
}

void UPlayerStats::fromXml(FXmlNode * const root)
{
	auto hpStr = root->GetAttribute(TEXT("hp"));
	HitPoints = FCString::Atoi(*hpStr);

	auto maxHpStr = root->GetAttribute(TEXT("maxHP"));
	MaxHitPoints = FCString::Atoi(*maxHpStr);
	
	auto speedStr = root->GetAttribute(TEXT("speed"));
	Speed = FCString::Atoi(*speedStr);

	auto strengthStr = root->GetAttribute(TEXT("strength"));
	Strength = FCString::Atoi(*strengthStr);

	auto defenceStr = root->GetAttribute(TEXT("defence"));
	Defence = FCString::Atoi(*defenceStr);
}