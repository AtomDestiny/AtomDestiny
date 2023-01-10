#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include <AtomDestiny/Parameters/BalanceParameters.h>

#include "Shield.generated.h"

///
/// Represents Shield type.
/// By default uses Elector type.
///
UENUM(BlueprintType)
enum class EShieldType : uint8
{
    Electrical,
    Plasmas
};

UINTERFACE(MinimalAPI, Blueprintable)
class UShield : public UInterface
{
    GENERATED_BODY()
};

///
/// Base interface for unit energy shield
///
class IShield
{
    GENERATED_BODY()
    
public:
    
    // Returns current shield value
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetUnitShield() const = 0;
    
    // Returns max shield value
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetMaxUnitShield() const = 0;
    
    // Returns start max shield value
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseMaxUnitShield() const = 0;
    
    // Returns shield type
    UFUNCTION(Meta = (AllowOverride = true))
    virtual EShieldType GetShieldType() const = 0;
    
    // Returns shield absorbation
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetShieldAbsorbation() const = 0;
    
    // Returns start shield absorbation
    UFUNCTION(Meta = (AllowOverride = true))
    virtual double GetBaseShieldAbsorbation() const = 0;
    
    // Returns shield additional parameters
    UFUNCTION(Meta = (AllowOverride = true))
    virtual const FBalanceParameters& GetShieldAdditionalParameters() const = 0;
};
