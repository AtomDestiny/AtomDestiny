#pragma once

#include <GameFramework/Actor.h>

#include "WeaponParameters.generated.h"

///
/// All weapons types at game.
/// You should not add new types.
///
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Ballistic,
    Energy,
    Explosive,
    Elemental,
};

// Data struct for weapon parameters
USTRUCT(BlueprintType)
struct FWeaponParameters
{
    GENERATED_BODY()
    
    double damage = 0;
    double criticalChance = 0;
    double criticalRate = 0;
    double explosionRadius = 0;
    EWeaponType weaponType;
    TWeakObjectPtr<AActor> owner;
    TWeakObjectPtr<AActor> target;
};
