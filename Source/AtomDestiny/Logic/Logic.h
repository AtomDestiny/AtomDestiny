#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include <AtomDestiny/Unit/Unit.h>
#include <AtomDestiny/Weapon/Weapon.h>
#include <AtomDestiny/AtomDestiny.h>

#include "Logic.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class ULogic : public UInterface
{
    GENERATED_BODY()
};

///
/// Unit base logic interface
///
class ILogic
{
    GENERATED_BODY()

public:
    
    // Returns current unit side
    UFUNCTION(Meta = (AllowOverride = true))
    virtual EGameSide GetSide() const = 0;

    // Returns current unit type
    UFUNCTION(Meta = (AllowOverride = true))
    virtual EUnitType GetUnitType() const = 0;

    // Returns current unit size
    UFUNCTION(Meta = (AllowOverride = true))
    virtual EUnitSize GetSize() const = 0;

    // Returns current unit cost
    UFUNCTION(Meta = (AllowOverride = true))
    virtual int32 GetCost() const = 0;

    // Returns logic binded weapon collections
    UFUNCTION(Meta = (AllowOverride = true))
    virtual const TArray<TScriptInterface<IWeapon>>& GetAllWeapon() const = 0;

    // Returns max base unit velocity
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetVelocity() const = 0;

    // Sets side to unit
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetSide(EGameSide side) = 0;

    // Sets destination to other Actor/Point
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetDestination(AActor* destination) = 0;

    // Overloaded setting destination to logic as point
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetDestinationByPoint(FVector destination) = 0;

    // Recalculates base unit parameters
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void UpdateParameters() = 0;
};
