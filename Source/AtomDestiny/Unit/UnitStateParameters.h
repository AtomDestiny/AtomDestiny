#pragma once

#include <AtomDestiny/Unit/Shield.h>
#include <AtomDestiny/Unit/Unit.h>
#include <AtomDestiny/ObjectState/ObjectState.h>
#include <AtomDestiny/Weapon/Weapon.h>

#include "UnitStateParameters.generated.h"

///
/// Represents unit state storage class for all possible parameters.
/// It should be needed rarely.
///
USTRUCT(BlueprintType)
struct FUnitStateParameters
{
    GENERATED_BODY()

    double velocity = 0;
    double weaponCount = 0;
    double currentHealth = 0;
    double maxHealth = 0;
    double currentShieldValue = 0;
    double maxShieldValue = 0;
    double defence = 0;
    EShieldType shieldType;
    EDefenceType defenceType;
    EUnitType unitType;
    bool hasShield = false;
    const TArray<TScriptInterface<IWeapon>>& weaponList;
    double maxDamage = 0;
    double minDamage = 0;
    double maxRange = 0;
    double minRange = 0;
    bool isAnyWeaponCritical = false;
    bool isAnyWeaponAoe = false;
    uint32_t minerals = 0;
};
