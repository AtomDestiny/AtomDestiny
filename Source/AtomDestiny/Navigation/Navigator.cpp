#include "Navigator.h"

#include "Navigation/CrowdFollowingComponent.h"

ANavigator::ANavigator(const FObjectInitializer& objectInitializer)
    : AAIController(objectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void ANavigator::Move(TWeakObjectPtr<AActor> target)
{
    m_target = std::move(target);
    AAIController::MoveToActor(m_target.Get());
}

void ANavigator::Stop()
{
    AAIController::StopMovement();
}
