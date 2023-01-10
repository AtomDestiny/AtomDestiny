// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * ActorComponent for containing a HealthBar Widget
 */
UCLASS()
class ATOMDESTINY_API UHealthBarComponent : public UWidgetComponent
{
    GENERATED_BODY()

public:
    explicit UHealthBarComponent(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());
    virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
};
