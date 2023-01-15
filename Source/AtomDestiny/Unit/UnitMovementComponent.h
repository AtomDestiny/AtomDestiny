#pragma once

#include <Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h>

#include "UnitMovementComponent.generated.h"

///
/// Represents movement component for unit.
/// Should be used for all base Atom Destiny units, which controlled by AI and ANavigator.
/// 
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class ATOMDESTINY_API UUnitMovementComponent : public UFloatingPawnMovement
{
    GENERATED_BODY()
    
protected:
    virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rotation speed"))
    double m_rotationSpeed = 5.0f;
};
