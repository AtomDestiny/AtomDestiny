#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include <AtomDestiny/Core/ADObject/ObjectParameters.h>
#include <AtomDestiny/Core/ADObject/ParameterEnhancement.h>
#include <AtomDestiny/Core/ADObject/ParameterZeroPack.h>

#include "Parameterizable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UParameterizable : public UInterface
{
    GENERATED_BODY()
};

///
/// Interface implementation can change Atom destiny object parameters
///
class IParameterizable
{    
    GENERATED_BODY()

public:
    
    // Adds additional value to parameter
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void AddParameter(EObjectParameters parameter, const FParameterEnhancement& enhancement) = 0;
        
    // Removes additional value from parameter
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void RemoveParameter(EObjectParameters parameter, AActor* enhancementObject) = 0;
        
    // Sets parameter to zero or non zero
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void ZeroParameter(EObjectParameters parameter, const FParameterZeroPack& pack) = 0;
};
