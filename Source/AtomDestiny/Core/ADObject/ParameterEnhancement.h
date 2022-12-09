#pragma once

#include <Engine/Classes/GameFramework/Actor.h>

#include "ParameterEnhancement.generated.h"

UENUM(BlueprintType)
enum class EParameterModifier : uint8
{
    Plus,
    PlusCoefficient,
    Minus,
    MinusCoefficient
};

///
/// Stores all params for parameters enhancement.
/// And modifiers to increase/decrease some stats or params.
///
USTRUCT(BlueprintType)
struct FParameterEnhancement
{
    GENERATED_BODY()
    
    TWeakObjectPtr<AActor> enhancementObject;
    float value;
    EParameterModifier modifier;
};
