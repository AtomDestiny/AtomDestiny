#pragma once

#include "AtomDestiny/Core/GameObject.h"

#include "WeaponParameters.generated.h"

namespace AtomDestiny::Weapon
{
    ///
    /// All weapons types at game.
    /// You should not add new types.
    ///
    UENUM()
    enum class WeaponType : uint8
    {
        Ballistic,
        Energy,
        Explosive,
        Elemental,
    };

} // namespace AtomDestiny::Weapon

// Data struct for weapon parameters
USTRUCT()
struct FWeaponParameters
{
    GENERATED_BODY()
    
    float damage;
    float criticalChance;
    float criticalRate;
    AtomDestiny::Weapon::WeaponType weaponType;
    float explosionRadius;
    GameObjectWeak owner;
    GameObjectWeak target;
};

namespace AtomDestiny::Weapon
{
    using WeaponParameters = FWeaponParameters;

} // namespace AtomDestiny::Weapon
