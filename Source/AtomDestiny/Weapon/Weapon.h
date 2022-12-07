#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include <AtomDestiny/Core/Macros.h>

#include "WeaponParameters.h"
#include "Weapon.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UWeapon : public UInterface
{
    GENERATED_BODY()
};

///
/// Base interface for all weapon controllers
///
class IWeapon
{
    GENERATED_BODY()

public:
    
    ///
    /// Weapon controls
    ///

    // Sets current target to weapon. Should be called before fire
    virtual void SetTarget(const GameObjectWeak& gameObject) PURE_VIRTUAL_METHOD

    // Weapon attack. Should be called after setTarget
    virtual void Fire() PURE_VIRTUAL_METHOD;

    ///
    /// Weapon states
    ///

    // Returns weapon firing state
    virtual bool IsFiring() const PURE_VIRTUAL_METHOD;

    // Returns could weapon rotates or not
    virtual bool IsRotated() const PURE_VIRTUAL_METHOD;

    // Returns AOE weapon status
    virtual bool IsAoe() const PURE_VIRTUAL_METHOD;

    // Returns current weapon type
    virtual AtomDestiny::Weapon::WeaponType GetWeaponType() const PURE_VIRTUAL_METHOD;

    // Returns weapon state of critical chance
    virtual bool IsCritical() const PURE_VIRTUAL_METHOD;

    // Returns target search possibility
    virtual bool IsSeeTarget() const PURE_VIRTUAL_METHOD;

    // Should we stop unit when attack? Generally should be used by Unit logic
    virtual bool IsShouldStop() const PURE_VIRTUAL_METHOD;

    // Returns weapon use raycast status
    virtual bool IsRaycasted() const PURE_VIRTUAL_METHOD;

    // Returns weapon friendly fire status
    virtual bool IsFriendlyFireAvailable() const PURE_VIRTUAL_METHOD;

    // Returns some current parameters of weapon
    virtual AtomDestiny::Weapon::WeaponParameters GetParameters() const PURE_VIRTUAL_METHOD;

    ///
    /// Weapon Base parameters
    ///

    // Returns started critical chance
    virtual float GetBaseCriticalChance() const PURE_VIRTUAL_METHOD;

    // Returns started critical rate
    virtual float GetBaseCriticalRate() const PURE_VIRTUAL_METHOD;

    // Returns base weapon damage
    virtual float GetBaseDamage() const PURE_VIRTUAL_METHOD;

    // Returns base attack range
    virtual float GetBaseAttackRange() const PURE_VIRTUAL_METHOD;

    /// Returns started base reload time
    virtual float GetBaseReloadTime() const PURE_VIRTUAL_METHOD;

    ///
    /// Weapon current parameters
    ///

    // Weapon max attack distance
    virtual float GetAttackRange() const PURE_VIRTUAL_METHOD;

    // Weapon min attack distance
    virtual float GetMinAttackRange() const PURE_VIRTUAL_METHOD;

    // Returns critical chance
    virtual float GetCriticalChance() const PURE_VIRTUAL_METHOD;

    // Returns critical rate
    virtual float GetCriticalRate() const PURE_VIRTUAL_METHOD;

    // Returns current weapon firing rate
    virtual float GetFireRate() const PURE_VIRTUAL_METHOD;

    // Returns current weapon damage
    virtual float GetDamage() const PURE_VIRTUAL_METHOD;

    // Returns reload time
    virtual float GetReloadTime() const PURE_VIRTUAL_METHOD;
};
