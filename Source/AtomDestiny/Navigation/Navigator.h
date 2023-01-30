#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>
#include <Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h>

#include <Runtime/AIModule/Classes/AIController.h>

#include <AtomDestiny/Core/Concepts.h>

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

    void SetStopDistance(double distance);
    double GetStopDistance() const;

    double GetRemainingDistance() const;

private:
    template <AtomDestiny::Concepts::NavigatorMovable T>
    bool CheckMoveRequest(const T& target) const;

    template <AtomDestiny::Concepts::NavigatorMovable T>
    void MoveImpl(const T& target);

    template <AtomDestiny::Concepts::NavigatorMovable T>
    EPathFollowingRequestResult::Type MoveAction(const T& target);

    template <AtomDestiny::Concepts::NavigatorMovable T>
    void SetTarget(const T& target);

    // Updates target navigation on every move request
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Every call update target", AllowPrivateAccess = "true"))
    bool m_updateNavigationOnEveryCall = false;
    
    double m_stopDistance = 1.0;
    
    TWeakObjectPtr<AActor> m_target;
    TWeakObjectPtr<UFloatingPawnMovement> m_pawnMovement;

    FVector m_targetPoint;
};
