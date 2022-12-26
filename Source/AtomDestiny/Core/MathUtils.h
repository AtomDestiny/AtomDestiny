#pragma once

#include <Runtime/Core/Public/Math/MathFwd.h>

namespace AtomDestiny
{
    namespace Vector
    {
        // Calculates an angle between two vectors (Positions)
        static double Angle(const FVector& lhs, const FVector& rhs)
        {
            const double sqrtLenght = FMath::Sqrt(lhs.SquaredLength() * rhs.SquaredLength());
        
            if (sqrtLenght < 1E-15)
                return 0;

            const auto dot = FVector::DotProduct(lhs, rhs);
            const double clampRes = FMath::Clamp(dot / sqrtLenght, -1, 1);
            
            return FMath::Acos(clampRes) * 57.29578;
        }
        
    } // namespace Vector

    struct RotationRes
    {
        double angle = 0;   // angle between current location and target location
        FQuat rotation;     // rotation that should be applied to owner
    };

    struct RotatorRes
    {
        double angle = 0;
        FRotator rotator;
    };

    // calculates Rotation res to target
    static RotationRes RotationToTarget(const FVector& currentLocation, const FVector& targetLocation, float deltaTime, float rotationSpeedDegrees)
    {
        FVector targetVector = currentLocation - targetLocation;
        targetVector.Y = 0;

        RotationRes result;
        result.angle = Vector::Angle(targetVector, currentLocation.ForwardVector);

        const FVector lookVector = FMath::VInterpNormalRotationTo(currentLocation.ForwardVector, targetVector, deltaTime, rotationSpeedDegrees);
        result.rotation = FQuat::FindBetween(lookVector, FVector::UpVector);

        return result;
    }

    static RotationRes RotationToTarget(const USceneComponent* component, const AActor* actor, float deltaTime, float rotationSpeedDegrees)
    {
        check(component != nullptr);
        check(actor != nullptr);

        return RotationToTarget(component->GetComponentLocation(), actor->GetActorLocation(), deltaTime, rotationSpeedDegrees);
    }

    static RotationRes RotationToTarget(const AActor* current, const AActor* target, float deltaTime, float rotationSpeedDegrees)
    {
        check(current != nullptr);
        check(target != nullptr);

        return RotationToTarget(current->GetActorLocation(), target->GetActorLocation(), deltaTime, rotationSpeedDegrees);
    }

    // calculates rotation to root from current position
    static FQuat RotationToRoot(const FVector& current, const FVector& root, float deltaTime, float rotationSpeedDegrees)
    {
        const FVector lookVector = FMath::VInterpNormalRotationTo(current.ForwardVector, root.ForwardVector, deltaTime, rotationSpeedDegrees);
        return FQuat::FindBetween(lookVector, FVector::UpVector);
    }

    static FQuat RotationToRoot(const USceneComponent* component, const AActor* actor, float deltaTime, float rotationSpeedDegrees)
    {
        return RotationToRoot(component->GetComponentLocation(), actor->GetActorLocation(), deltaTime, rotationSpeedDegrees);
    }

    static RotatorRes LerpRotation(const FVector& currentLocation, const FVector& targetLocation,
        const FVector& currentForward, const FRotator& currentRotation, float deltaTime, float rotationSpeedDegrees)
    {
        FVector targetDirection = targetLocation - currentLocation;
        
        const FRotator rotation = FRotationMatrix::MakeFromX(targetDirection).Rotator();
        const FRotator desiredRotation = FMath::RInterpTo(currentRotation,rotation, deltaTime, rotationSpeedDegrees);

        return RotatorRes {
            .angle = Vector::Angle(targetDirection, currentForward),
            .rotator = desiredRotation,
        };
    }

    static RotatorRes LerpRotation(const USceneComponent* component, const AActor* actor, float deltaTime, float rotationSpeedDegrees)
    {
        check(component != nullptr);
        check(actor != nullptr);

        return LerpRotation(component->GetComponentLocation(), actor->GetActorLocation(),
            component->GetForwardVector(), component->GetComponentRotation(), deltaTime, rotationSpeedDegrees);
    }
    
} // namespace AtomDestiny
