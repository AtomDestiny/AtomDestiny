#pragma once

#include <Runtime/Core/Public/Math/MathFwd.h>

namespace AtomDestiny::Utils
{
    // Calculates an angle between two vectors (Positions)
    static double AngleBetweenVectors(const FVector& lhs, const FVector& rhs)
    {
        const double num = FMath::Sqrt(lhs.SquaredLength() * rhs.SquaredLength());
        
        if (num < 1E-15)
            return 0;

        const auto dot = FVector::DotProduct(lhs, rhs);
        const double clampRes = FMath::Clamp(dot / num, -1, 1);
        return FMath::Acos(clampRes) * 57.29578;
    }
    
} // namespace AtomDestiny
