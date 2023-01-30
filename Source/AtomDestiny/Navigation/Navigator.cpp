#include "Navigator.h"

#include <Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h>

#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Core/TypeTraits.h>

using namespace AtomDestiny::Concepts;

namespace
{

template <NavigatorMovable T>
static void PrintMoveFailed(const AActor* owner, const T& target)
{
    if (owner == nullptr)
    {
        LOG_WARNING(TEXT("Owner is nullptr, can not print Navigator messages"));
        return;
    }

    const auto name = owner->GetActorNameOrLabel();

    if constexpr (AtomDestiny::Traits::IsVector<T>)
    {
        LOG_WARNING(TEXT("Unit %s, failed to move to point %s"), *name, *target.ToString());
    }
    else
    {
        LOG_WARNING(TEXT("Unit %s, failed to move to target %s"), *name, *target->GetActorNameOrLabel());
    }
}

} // namespace

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
    MoveImpl(target);
}

void ANavigator::Move(const FVector& point)
{
    MoveImpl(point);
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
    
    if (m_target.IsValid())
    {
        return (m_target->GetActorLocation() - owner->GetActorLocation()).Length();
    }

    return (m_targetPoint - owner->GetActorLocation()).Length();
}

template <NavigatorMovable T>
bool ANavigator::CheckMoveRequest(const T& target) const
{
    if constexpr (AtomDestiny::Traits::IsVector<T>)
    {
        return !(target == m_targetPoint && !m_updateNavigationOnEveryCall);
    }
    else
    {
        return !(target == m_target.Get() && !m_updateNavigationOnEveryCall);
    }
}

template <NavigatorMovable T>
void ANavigator::MoveImpl(const T& target)
{
    if (!CheckMoveRequest(target))
    {
        return;
    }

    const EPathFollowingRequestResult::Type result = MoveAction(target);

    if (result == EPathFollowingRequestResult::Type::Failed)
    {
 #ifdef PRINT_NAVIGATOR_MESSAGES
        PrintMoveFailed(GetOwner(), target);
 #endif // PRINT_NAVIGATOR_MESSAGES
        return;
    }

    SetTarget(target);
}

template <NavigatorMovable T>
EPathFollowingRequestResult::Type ANavigator::MoveAction(const T& target)
{
    if constexpr (AtomDestiny::Traits::IsVector<T>)
    {
        return MoveToLocation(target, static_cast<float>(m_stopDistance));
    }
    else
    {
        return MoveToActor(target, static_cast<float>(m_stopDistance));
    }
}

template <NavigatorMovable T>
void ANavigator::SetTarget(const T& target)
{
    if constexpr (AtomDestiny::Traits::IsVector<T>)
    {
        m_target = nullptr;
        m_targetPoint = target;
    }
    else
    {
        m_target = MakeWeakObjectPtr(target);
        m_targetPoint = FVector{};
    }
}
