#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "Event.generated.h"

// Unit parameters event
UENUM(BlueprintType)
enum class EEventType : uint8
{
    OnHealth,
    OnShield
};

UINTERFACE(MinimalAPI, Blueprintable)
class UEvent : public UInterface
{
    GENERATED_BODY()
};

///
/// Base interface for callbacks
///
class IEvent
{
    GENERATED_BODY()

public:
    
    // Invokes event with double value and event
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double Invoke(double value, EEventType eventType) = 0;
};
