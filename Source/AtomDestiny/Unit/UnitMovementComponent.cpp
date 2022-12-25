#include "UnitMovementComponent.h"

void UUnitMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    if (!Velocity.IsZero())
    {
        const FVector velocity = Velocity.GetUnsafeNormal();
        const FQuat targetRotation = FQuat::FindBetween(velocity, GetPawnOwner()->GetActorLocation().ForwardVector);
        const FRotator desiredRotation = FMath::RInterpTo(GetPawnOwner()->GetActorRotation(), FRotator { targetRotation }, deltaTime, m_rotationSpeed);
        
        GetPawnOwner()->SetActorRotation(desiredRotation);
    }
}
