﻿#pragma once

#include <AtomDestiny/Parameters/ParametersBase.h>

#include "UnitParameters.generated.h"

UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API UUnitParameters : public UParametersBase
{
    GENERATED_BODY()

public:
    explicit UUnitParameters(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // Adds damage to this Actor
    UFUNCTION(Meta = (AllowOverride = false))
    virtual void AddDamage(double damage, EWeaponType type, AActor* owner) override;
   
protected:
    virtual void BeginPlay() override;
    
    // Called every frame
    virtual void TickComponent(float deltaTime, ELevelTick levelTick, FActorComponentTickFunction* func) override;

private:
    void RenderHealthBar();
    void CheckHealthState();
};
