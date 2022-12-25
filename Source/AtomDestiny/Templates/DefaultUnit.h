﻿#pragma once

class UUnitParameters;
class UUnitState;
class UUnitLogic;
class USceneComponent;
class UUnitMovementComponent;

#include <Runtime/Engine/Classes/GameFramework/Pawn.h>

#include "DefaultUnit.generated.h"

///
/// Represents default unit with basic components.
/// It consists of Box collider, Unit parameters (heath and defence),
/// Unit state (massive API to unit info and control), Ground point, Movement component and
/// Unit logic (AI control component).
///
UCLASS(BlueprintType, Blueprintable, meta=(ShortTooltip="Default unit with basic needed components to participate at Atom Destiny game"))
class ADefaultUnit : public APawn
{
    GENERATED_BODY()
    
public:
    explicit ADefaultUnit(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Box collider"))
    TObjectPtr<USceneComponent> m_boxComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Ground point"))
    TObjectPtr<USceneComponent> m_groundPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit state"))
    TObjectPtr<UUnitState> m_unitState;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit logic"))
    TObjectPtr<UUnitLogic> m_unitLogic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit paremeters"))
    TObjectPtr<UUnitParameters> m_unitParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit movement"))
    TObjectPtr<UUnitMovementComponent> m_unitMovement;
};
