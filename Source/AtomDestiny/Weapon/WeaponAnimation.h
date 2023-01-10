#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "WeaponAnimation.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponAnimation : public UInterface
{
    GENERATED_BODY()
};

///
/// Base interface for weapon animation
///
class IWeaponAnimation
{
    GENERATED_BODY()
    
public:
    
    // Returns weapon ready status
    UFUNCTION(Meta = (AllowOverride = true))
    virtual bool IsReady() const = 0;

    // Plays animation
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void Animate() = 0;

    // Sets default state of animation
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void SetDefaultState() = 0;
};
