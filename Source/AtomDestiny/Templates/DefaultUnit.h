﻿#pragma once

class UUnitParameters;
class UUnitState;
class UUnitLogic;
class USceneComponent;
class UHealthBarComponent;
class UUnitMovementComponent;
class UUnitScrapDestroy;

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

    virtual void BeginPlay() override;
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Box collider"))
    TObjectPtr<USceneComponent> m_boxComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Ground point"))
    TObjectPtr<USceneComponent> m_groundPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Health bar"))
    TObjectPtr<UHealthBarComponent> m_healthBar;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit state"))
    TObjectPtr<UUnitState> m_unitState;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit logic"))
    TObjectPtr<UUnitLogic> m_unitLogic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit parameters"))
    TObjectPtr<UUnitParameters> m_unitParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit movement"))
    TObjectPtr<UUnitMovementComponent> m_unitMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit destroy behaviour"))
    TObjectPtr<UUnitScrapDestroy> m_unitDestroy;
};
