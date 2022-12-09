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
    
} // namespace AtomDestiny
