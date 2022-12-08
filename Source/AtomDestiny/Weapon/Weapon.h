#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

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
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetTarget(AActor* gameObject) = 0;

    // Weapon attack. Should be called after setTarget
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void Fire() = 0;

    ///
    /// Weapon states
    ///

    // Returns weapon firing state
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsFiring() const = 0;

    // Returns could weapon rotates or not
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsRotated() const = 0;

    // Returns AOE weapon status
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsAoe() const = 0;

    // Returns current weapon type
    UFUNCTION(Meta = (AllowOverride = true))
    virtual EWeaponType GetWeaponType() const = 0;

    // Returns weapon state of critical chance
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsCritical() const = 0;

    // Returns target search possibility
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsSeeTarget() const = 0;

    // Should we stop unit when attack? Generally should be used by Unit logic
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsShouldStop() const = 0;

    // Returns weapon use raycast status
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsRaycasted() const = 0;

    // Returns weapon friendly fire status
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsFriendlyFireAvailable() const = 0;

    // Returns some current parameters of weapon
    UFUNCTION(Meta = (AllowOverride = true))
    virtual FWeaponParameters GetParameters() const = 0;

    ///
    /// Weapon Base parameters
    ///

    // Returns started critical chance
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetBaseCriticalChance() const = 0;

    // Returns started critical rate
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetBaseCriticalRate() const = 0;

    // Returns base weapon damage
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetBaseDamage() const = 0;

    // Returns base attack range
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetBaseAttackRange() const = 0;

    /// Returns started base reload time
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetBaseReloadTime() const = 0;

    ///
    /// Weapon current parameters
    ///

    // Weapon max attack distance
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetAttackRange() const = 0;

    // Weapon min attack distance
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetMinAttackRange() const = 0;

    // Returns critical chance
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetCriticalChance() const = 0;

    // Returns critical rate
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetCriticalRate() const = 0;

    // Returns current weapon firing rate
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetFireRate() const = 0;

    // Returns current weapon damage
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetDamage() const = 0;

    // Returns reload time
    UFUNCTION(Meta = (AllowOverride = true))
    virtual float GetReloadTime() const = 0;
};
