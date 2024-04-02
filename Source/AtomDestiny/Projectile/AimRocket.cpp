#include "AimRocket.h"

#include "AtomDestiny/Unit/UnitState.h"

#include "AtomDestiny/Core/ObjectPool/ActorPool.h"

AAimRocket::AAimRocket(const FObjectInitializer& objectInitializer):
    ARocketBase(objectInitializer)
{
}

void AAimRocket::Tick(float deltaSeconds)
{
    
}

void AAimRocket::Launch()
{
    check(m_parameters.target != nullptr);
    
    m_points.endPosition = m_parameters.target->FindComponentByClass<UUnitState>()->GetGroundPoint<FVector>();

    const FVector enginePoint = m_rocketEngine->GetComponentLocation();
    const FQuat lookRotation = FQuat::MakeFromRotationVector(m_rocketEngine->GetForwardVector());
    
    m_trace = AtomDestiny::ObjectPool::Instance().Spawn(m_tracePrefab, enginePoint, lookRotation);

    SetOwner(nullptr);
    
    RunCoroutine(LockOn());
}
