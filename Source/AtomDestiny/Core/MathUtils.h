﻿#pragma once

#include <Runtime/Core/Public/Math/MathFwd.h>

namespace AtomDestiny
{
    namespace Vector
    {
        // Calculates an angle between two vectors (Positions)
        static double Angle(const FVector& lhs, const FVector& rhs)
        {
            const double sqrtLength = FMath::Sqrt(lhs.SquaredLength() * rhs.SquaredLength());
        
            if (sqrtLength < 1E-15)
                return 0;

            const auto dot = FVector::DotProduct(lhs, rhs);
            const double clampRes = FMath::Clamp(dot / sqrtLength, -1, 1);
            
            return FMath::Acos(clampRes) * 57.29578;
        }

        static FVector RotateTowards(const FVector& from, const FVector& to, const double maxRadiansDelta)
        {
            FVector fromDirection = from;
            FVector toDirection = to;
            
            fromDirection.Normalize();
            toDirection.Normalize();
            
            const double angleRadians = FGenericPlatformMath::Acos(FVector::DotProduct(fromDirection,toDirection));
            const double resultAngleRadians = FGenericPlatformMath::Min(angleRadians, maxRadiansDelta);

            const FVector axis = FVector::CrossProduct(fromDirection, toDirection);
            const FQuat rotationIncrement { axis, resultAngleRadians }; // Quaternion.AngleAxis

            return rotationIncrement * from;
        }
        
    } // namespace Vector
    
    struct RotatorRes
    {
        double angle = 0;
        FRotator rotator;
    };

    static FRotator LerpRotationRoot(const USceneComponent* component, const AActor* owner, float deltaTime, float rotationSpeedDegrees)
    {
        const FRotator rotation = FRotationMatrix::MakeFromX(owner->GetActorForwardVector()).Rotator();
        return FMath::RInterpTo(component->GetComponentRotation(),rotation, deltaTime, rotationSpeedDegrees);
    }
    
    static RotatorRes LerpRotation(const FVector& currentLocation, const FVector& targetLocation,
        const FVector& currentForward, const FRotator& currentRotation, float deltaTime, float rotationSpeedDegrees)
    {
        const FVector targetDirection = targetLocation - currentLocation;
        
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

    static RotatorRes LerpRotation(const AActor* current, const AActor* target, float deltaTime, float rotationSpeedDegrees)
    {
        check(current != nullptr);
        check(target != nullptr);

        return LerpRotation(current->GetActorLocation(), target->GetActorLocation(),
            current->GetActorForwardVector(), current->GetActorRotation(), deltaTime, rotationSpeedDegrees);
    }
    
} // namespace AtomDestiny
