#pragma once

#include <GameFramework/Actor.h>

#include "ParameterZeroPack.generated.h"

///
/// Stores data to understand how to zero parameters
///
USTRUCT(BlueprintType)
struct FParameterZeroPack
{
    GENERATED_BODY()
    
    TWeakObjectPtr<AActor> zeroObject;
    bool status;
};
