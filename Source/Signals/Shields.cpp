#include "Signals.h"
#include "Shields.h"
#include "AttackClass.h"
#include "Combatant.h"
#include "SignalsBattleMode.h"
#include "ResourceManager.h"

static const FString s_fxNames[EAttackClass::NumAttackClasses] =
{
	TEXT("P_barrier05"),	// Physical
	TEXT("P_shield04"),		// Fire
	TEXT("P_barrier05"),	// Ice
	TEXT("P_barrier05"),	// Electrical
	TEXT("P_barrier05"),	// Light
	TEXT("P_barrier05"),	// Plasma
	TEXT("P_barrier05"),	// Sound
	TEXT("P_barrier05"),	// Poison
	TEXT("P_barrier05"),	// Bacterial
	TEXT("P_barrier05"),	// Viral
};

namespace Shields
{
	void ShowShield(ASignalsBattleMode * battle, Combatant * player, EAttackClass type)
	{
		auto index = int(type);
		check(index >= 0 && index < (int)EAttackClass::NumAttackClasses);
		auto fx = battle->GetResourceManager()->LoadParticleSystem(s_fxNames[index]);
		FName socketName(TEXT("TargetSocket"));
		auto position = player->Avatar->GetMesh()->GetSocketTransform(socketName);
		UGameplayStatics::SpawnEmitterAtLocation(battle->GetWorld(), fx, position, true);
	}
}