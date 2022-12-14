#pragma once

#include <AtomDestiny/Parameters/ShieldParametersBase.h>

#include "UnitShieldParameters.generated.h"

///
/// Represents base units parameters with health and shield.
///
UCLASS(ClassGroup=(AtomDestiny), Blueprintable)
class UUnitShieldParameters final : public UShieldParametersBase
{
    GENERATED_BODY()
    
protected:
    virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
    virtual void AddDamage(double damage, EWeaponType type, AActor* owner) override;

    void RenderShieldBar();
};
