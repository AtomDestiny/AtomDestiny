#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include <AtomDestiny/Weapon/ProjectilePoints.h>
#include <AtomDestiny/Weapon/WeaponParameters.h>

#include "Projectile.generated.h"

///
/// Defines which point use to calculate damage at Game state
///
UENUM(BlueprintType)
enum class EProjectileDamageOptions : uint8
{
    ProjectilePoint,
    ImpactPoint,
    TargetPoint
};

UINTERFACE(MinimalAPI, Blueprintable)
class UProjectile : public UInterface
{
    GENERATED_BODY()
};

///
/// Represents a projectile driven by IWeapon
///
class IProjectile
{
    GENERATED_BODY()

public:
    
    // Sets projectile points from weapon or another projectile
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetPoints(const FProjectilePoints& points) = 0;
    
    // Sets weapon parameters to projectile
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetParameters(const FWeaponParameters& parameters) = 0;
    
    // Returns projectile parameters
    UFUNCTION(Meta = (AllowOverride = true))
    virtual const FWeaponParameters& GetParameters() const = 0;
    
    // Returns projectile points
    UFUNCTION(Meta = (AllowOverride = true))
    virtual const FProjectilePoints& GetPoints() const = 0;
    
    // Initializes base data and launches a projectile
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void Launch() = 0;
};