#include "Navigator.h"

#include <Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h>

ANavigator::ANavigator(const FObjectInitializer& objectInitializer)
    : AAIController(objectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void ANavigator::SetMovementComponent(UFloatingPawnMovement* component)
{
    check(component != nullptr)
    m_pawnMovement = TWeakObjectPtr<UFloatingPawnMovement>{ component };
}

void ANavigator::Move(AActor* target)
{
    check(target != nullptr)
    m_target = TWeakObjectPtr<AActor>{target};
    
    MoveToActor(m_target.Get());
}

void ANavigator::Stop()
{
    AAIController::StopMovement();
}

void ANavigator::SetSpeed(double speed)
{
    m_pawnMovement->MaxSpeed = static_cast<float>(speed);
}

double ANavigator::GetSpeed() const
{
    return m_pawnMovement->GetMaxSpeed();
}

double ANavigator::GetStopDistance() const
{
    return m_stopDistance;
}
