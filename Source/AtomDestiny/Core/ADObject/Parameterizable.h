#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include <AtomDestiny/Core/Macros.h>
#include <AtomDestiny/Core/ADObject/ObjectParameters.h>

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
    virtual void AddParameter(ObjectParameters parameter, const ParameterEnhancement& enhancement) PURE_VIRTUAL_METHOD;
        
    // Removes additional value from parameter
    virtual void RemoveParameter(ObjectParameters parameter, const GameObject& enhancementObject) PURE_VIRTUAL_METHOD;
        
    // Sets parameter to zero or non zero
    virtual void ZeroParameter(ObjectParameters parameter, const ParameterZeroPack& pack) PURE_VIRTUAL_METHOD;
};
