#include "AimRocket.h"

#include "AtomDestiny/Unit/UnitState.h"

#include "AtomDestiny/Core/MathUtils.h"
#include "AtomDestiny/Core/ObjectPool/ActorPool.h"

AAimRocket::AAimRocket(const FObjectInitializer& objectInitializer):
    ARocketBase(objectInitializer)
{
}

void AAimRocket::Tick(float deltaSeconds)
{
    Super::Tick(deltaSeconds);
    
    if (m_locked)
    {
        const FVector actorPosition = GetActorLocation();
        const FVector targetVector = m_parameters.target.IsValid() ? m_parameters.target->GetActorLocation() - actorPosition : m_points.endPosition - actorPosition;
        const FVector forward = AtomDestiny::Vector::RotateTowards(GetActorForwardVector(), targetVector, deltaSeconds * m_angularSpeed);
        
        SetActorRotation(forward.Rotation());   // set transform forward
    }

    SetActorLocation(GetActorLocation() + GetActorForwardVector() * m_speed * deltaSeconds);
    
    m_trace->SetActorLocation(m_rocketEngine->GetComponentLocation());
    m_trace->SetActorRotation(FQuat::FindBetweenVectors(m_rocketEngine->GetForwardVector(), FVector::UpVector));
}

void AAimRocket::Launch()
{
    check(m_parameters.target != nullptr);
    check(m_trace != nullptr);
    check(m_rocketEngine != nullptr);
    
    m_points.endPosition = m_parameters.target->FindComponentByClass<UUnitState>()->GetGroundPoint<FVector>();

    const FVector enginePoint = m_rocketEngine->GetComponentLocation();
    const FQuat lookRotation = FQuat::MakeFromRotationVector(m_rocketEngine->GetForwardVector());
    
    m_trace = AtomDestiny::ObjectPool::Instance().Spawn(m_tracePrefab, enginePoint, lookRotation);

    SetOwner(nullptr);
    
    RunCoroutine(LockOn());
}
