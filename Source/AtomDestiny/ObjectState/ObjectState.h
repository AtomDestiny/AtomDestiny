#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include <AtomDestiny/Weapon/WeaponParameters.h>
#include <AtomDestiny/ObjectState/BalanceParameters.h>

#include <AtomDestiny/Event/Event.h>

#include "ObjectState.generated.h"

///
/// Defence type of ADObject.
/// Conceptually it should be referenced to Units, but can be used on Buildings.
///
UENUM(BlueprintType)
enum class EDefenceType : uint8
{
    Light,
    Medium,
    Heavy
};

UINTERFACE(MinimalAPI, Blueprintable)
class UObjectState : public UInterface
{
    GENERATED_BODY()
};

///
/// Object base parameters interface.
/// Represents health and other params.
/// Realize to have a destroyable object with parameters.
/// Can be used together with IShield.
///
class IObjectState
{
    GENERATED_BODY()

public:
    
    // Adds damage to object
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void AddDamage(double damage, EWeaponType type, AActor* owner) = 0;
    
    // For healers only
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void RegenerateHealth(double health) = 0;
    
    // Returns state of max health
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsHealthMax() const = 0;
    
    // Returns current health
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetHealth() const = 0;
    
    // Returns max unit value
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetMaxHealth() const = 0;
    
    // Returns started max object health
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseMaxHealth() const = 0;
    
    // Returns defence type
    UFUNCTION(Meta = (AllowOverride = true))
    virtual EDefenceType GetDefenceType() const = 0;
    
    // Returns current defence
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetDefence() const = 0;
    
    // Returns base defence
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseDefence() const = 0;
    
    // Returns additional defence params
    UFUNCTION(Meta = (AllowOverride = true))
    virtual const FBalanceParameters& GetDefenceAdditionalParameters() const = 0;
    
    // Add event to unit parameters
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void AddEvent(TScriptInterface<IEvent> e) = 0;
    
    // Returns reference to unit event
    UFUNCTION(Meta = (AllowOverride = true))
    virtual TScriptInterface<IEvent> GetEvent() const = 0;
};
