#include "Navigator.h"

#include "Navigation/CrowdFollowingComponent.h"

ANavigator::ANavigator(const FObjectInitializer& objectInitializer)
    : AAIController(objectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
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
