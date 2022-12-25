#include "UnitMovementComponent.h"

#include "Core/Logger.h"
#include "DrawDebugHelpers.h"

void UUnitMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    if (!Velocity.IsZero())
    {
        const FVector velocity = Velocity.GetUnsafeNormal();

        auto const& actorLoc = GetActorLocation();
        auto const& fwdVec = GetActorTransform().TransformVector({0,1,0});
        
        DrawDebugLine(GetWorld(), actorLoc, actorLoc + velocity * 1000, FColor::Red);
        DrawDebugLine(GetWorld(), actorLoc, actorLoc + fwdVec * 800, FColor::Blue);

        const FRotator rot = FRotationMatrix::MakeFromX(velocity).Rotator().Add(0,-90,0);
        
        const FRotator desiredRotation = FMath::RInterpTo(
            GetPawnOwner()->GetActorRotation(),
            rot,
            deltaTime,
            m_rotationSpeed
            );
        
        GetPawnOwner()->SetActorRotation(desiredRotation);
    }
    else
    {
        LOG_WARNING(TEXT("Velocity is 0"))
    }
}
