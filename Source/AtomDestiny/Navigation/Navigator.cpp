#include "Navigator.h"

#include <Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h>

ANavigator::ANavigator(const FObjectInitializer& objectInitializer)
    : AAIController(objectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void ANavigator::SetMovementComponent(UFloatingPawnMovement* component)
{
    check(component != nullptr)
    m_pawnMovement = TWeakObjectPtr<UFloatingPawnMovement>{ component };
}

void ANavigator::Move(AActor* target)
{
    check(target != nullptr)
    
    m_target = MakeWeakObjectPtr(target);
    m_targetPoint = FVector{};
    
    MoveToActor(m_target.Get());
}

void ANavigator::Move(const FVector& point)
{
    m_target = nullptr;
    m_targetPoint = point;
    
    MoveToLocation(point);
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

void ANavigator::SetStopDistance(double distance)
{
    m_stopDistance = distance;
}

double ANavigator::GetStopDistance() const
{
    return m_stopDistance;
}

double ANavigator::GetRemainingDistance() const
{
    const AActor* owner = m_pawnMovement->GetOwner();
    check(owner);
    
    if (m_target != nullptr)
        return (m_target->GetActorLocation() - owner->GetActorLocation()).Length();

    return (m_targetPoint - owner->GetActorLocation()).Length();
}
