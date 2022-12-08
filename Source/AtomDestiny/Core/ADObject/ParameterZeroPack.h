#pragma once

#include <Engine/Classes/GameFramework/Actor.h>

#include "ParameterZeroPack.generated.h"

///
/// Stores data to understand how to zero parameters
///
USTRUCT()
struct FParameterZeroPack
{
    GENERATED_BODY()
    
    TWeakObjectPtr<AActor> zeroObject;
    bool status;
};
