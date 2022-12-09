#pragma once

#include <vector>
#include <utility>
#include <unordered_map>

#include <Engine/Classes/Components/SceneComponent.h>

#include <AtomDestiny/Core/Macros.h>
#include <AtomDestiny/Core/ADObject/Parameterizable.h>

#include "ADObject.generated.h"

///
/// Represents Parameterizable Atom Destiny object.
/// Minimal entity for any Atom Destiny object.
/// (Units, Buildings, Abilities).
///
UCLASS(Abstract, Blueprintable)
class ATOMDESTINY_API UADObject : public USceneComponent, public IParameterizable
{
    GENERATED_BODY()

    using GameObjectWeak = TWeakObjectPtr<AActor>;
    using GameObjectPairParameterList = std::pair<std::vector<GameObjectWeak>, std::vector<FParameterEnhancement>>;

    ///
    /// Type for any Atom Destiny parameters
    ///
    using ObjectEnhancementParameters = std::unordered_map<EObjectParameters, GameObjectPairParameterList>;

public:
    using USceneComponent::USceneComponent;

    UFUNCTION(Meta = (AllowOverride = true))
    virtual void AddParameter(EObjectParameters parameter, const FParameterEnhancement& enhancement) override;
    
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void RemoveParameter(EObjectParameters parameter, AActor* enhancementObject) override;
    
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void ZeroParameter(EObjectParameters parameter, const FParameterZeroPack& pack) override;

    /// Interprets ParameterEnhancement values and returns resulting value
    static float InterpretParameterModifier(float baseValue, const FParameterEnhancement& enhancement);

protected:
    
    // Recalculates parameter for implementor
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void RecalculateParameter(EObjectParameters parameter) PURE_VIRTUAL_METHOD

    // Set parameter to zero
    UFUNCTION(Meta = (AllowOverride = true))
    virtual void ZeroizeParameter(EObjectParameters parameter) PURE_VIRTUAL_METHOD

    // Returns current parameters key types
    std::vector<EObjectParameters> GetParameterTypes() const;
    std::vector<FParameterEnhancement>& GetParameterEnhancementList(EObjectParameters parameter);

    // Returns status of parameter
    bool GetParameterAvailable(EObjectParameters parameter);
    std::vector<TWeakObjectPtr<AActor>>& GetParameterZeroList(EObjectParameters parameter);

    // Adds parameter to object parameters
    void AddNewParameter(EObjectParameters parameter);
    void AddNewParameters(const std::vector<EObjectParameters>& parameters);

    // Adds enhancement value to parameter
    void AddToParameter(EObjectParameters parameter, const FParameterEnhancement& enhancement);

    // Removes enhancement value from parameter
    void RemoveFromParameter(EObjectParameters parameter, const TWeakObjectPtr<AActor>& enhanceObject);

private:
    
    // Removes all parameters additional values which are null
    void RemoveNullParameters(EObjectParameters parameter);

    // Recalculates any parameter
    void Recalculate(EObjectParameters parameter);

    ObjectEnhancementParameters m_enhancementParameters;
};
