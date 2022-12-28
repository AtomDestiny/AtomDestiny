#include "Navigator.h"

#include <Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h>
#include <AtomDestiny/Core/Logger.h>

ANavigator::ANavigator(const FObjectInitializer& objectInitializer)
    : AAIController(objectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("CrowdPathFollowingComponent")))
{
    bStartAILogicOnPossess = true;
    
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
    
    if (!CheckMoveRequest(target))
        return;
    
    const EPathFollowingRequestResult::Type result = MoveToActor(target, static_cast<float>(m_stopDistance));

    if (result == EPathFollowingRequestResult::Type::Failed)
    {
        if (m_logMovingResults)
            LOG_WARNING(TEXT("Failed to move to target %s"), *target->GetActorNameOrLabel());
        
        return;
    }

    m_target = MakeWeakObjectPtr(target);
    m_targetPoint = FVector{};
}

void ANavigator::Move(const FVector& point)
{
    if (!CheckMoveRequest(point))
        return;
    
    const EPathFollowingRequestResult::Type result = MoveToLocation(point, static_cast<float>(m_stopDistance));

    if (result == EPathFollowingRequestResult::Type::Failed)
    {
        if (m_logMovingResults)
            LOG_WARNING(TEXT("Failed to move to point %s"), *point.ToString());
        
        return;
    }

    m_target = nullptr;
    m_targetPoint = point;
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

bool ANavigator::CheckMoveRequest(const AActor* target) const
{
    if (target == m_target.Get() && !m_updateNavigationOnEveryCall)
    {
        CheckLogPrint(target->GetActorNameOrLabel());
        return false;
    }
    
    return true;
}

bool ANavigator::CheckMoveRequest(const FVector& point) const
{
    if (point == m_targetPoint && !m_updateNavigationOnEveryCall)
    {
        CheckLogPrint(point.ToString());
        return false;
    }
    
    return true;
}

void ANavigator::CheckLogPrint(const FString& message) const
{
    if (m_logMovingResults)
    {
        LOG_WARNING(TEXT("Already moving to target %s"), *message);
    }
}
