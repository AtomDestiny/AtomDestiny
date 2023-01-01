#pragma once

#include <AtomDestiny/ObjectState/ObjectStateBase.h>

#include "UnitParameters.generated.h"

UCLASS(ClassGroup=(AtomDestiny), Blueprintable)
class ATOMDESTINY_API UUnitParameters final : public UObjectStateBase
{
    GENERATED_BODY()

public:
    explicit UUnitParameters(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // Adds damage to this Actor
    UFUNCTION(Meta = (AllowOverride = false))
    virtual void AddDamage(double damage, EWeaponType type, AActor* owner) override;
   
protected:
    
    // Called every frame
    virtual void TickComponent(float deltaTime, ELevelTick levelTick, FActorComponentTickFunction* func) override;

private:
    void RenderBar();
    void CheckHealthState();
};
