#include "UnitMovementComponent.h"

#include "DrawDebugHelpers.h"

void UUnitMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    if (!Velocity.IsZero())
    {
        const FVector velocity = Velocity.GetUnsafeNormal();
        const FVector actorLocation = GetActorLocation();
        const FVector forwardVector = GetOwner()->GetActorForwardVector();

        if (m_drawDebugNavigation)
        {
            DrawDebugLine(GetWorld(), actorLocation, actorLocation + velocity * 1000, FColor::Red);
            DrawDebugLine(GetWorld(), actorLocation, actorLocation + forwardVector * 800, FColor::Blue);
        }

        const FRotator rotation = FRotationMatrix::MakeFromX(velocity).Rotator();
        const FRotator desiredRotation = FMath::RInterpTo(GetPawnOwner()->GetActorRotation(),rotation, deltaTime, m_rotationSpeed);
        
        GetPawnOwner()->SetActorRotation(desiredRotation);
    }
}
