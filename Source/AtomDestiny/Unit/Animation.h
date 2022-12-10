#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "Animation.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAnimation : public UInterface
{
    GENERATED_BODY()
};

///
/// Base interface for unit animation
///
class IAnimation
{
    GENERATED_BODY()
    
public:
    
    // Stop animation
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void Idle() = 0;
    
    // Move animation
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void Walk() = 0;
    
    // Attack animation
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void Attack() = 0;
};
