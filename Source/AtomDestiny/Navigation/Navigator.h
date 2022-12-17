#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>
#include <Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h>
#include <Runtime/AIModule/Classes/AIController.h>

#include "Navigator.generated.h"

///
/// Base entity to use for navigation and AIControl
/// for Atom Destiny unit
///
UCLASS(ClassGroup = AI, BlueprintType, Blueprintable)
class ATOMDESTINY_API ANavigator : public AAIController
{
    GENERATED_BODY()

public:
    explicit ANavigator(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // Sets movement component to control speed and other values
    void SetMovementComponent(UFloatingPawnMovement* component);

    // move pawn to actor
    void Move(AActor* target);
    void Move(const FVector& point);
    void Stop();
    
    void SetSpeed(double speed);
    double GetSpeed() const;
    
    double GetStopDistance() const;

private:
    double m_stopDistance = 1.0f;
    
    TWeakObjectPtr<AActor> m_target;
    TWeakObjectPtr<UFloatingPawnMovement> m_pawnMovement;
};
