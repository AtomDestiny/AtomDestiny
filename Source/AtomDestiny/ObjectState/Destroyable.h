#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "Destroyable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDestroyable : public UInterface
{
    GENERATED_BODY()
};

///
/// Base interface for ADObject destroy
///
class IDestroyable
{
    GENERATED_BODY()

public:
    
    // Destroys object
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void Destroy() = 0;
};
