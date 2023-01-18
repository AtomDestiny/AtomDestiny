#pragma once

#include <Engine/Classes/Components/PrimitiveComponent.h>

namespace AtomDestiny::Utils::Details
{
    static void SetSimulatePhysics(UActorComponent* const component, const bool enabled)
    {
        if (const auto primitiveComponent = Cast<UPrimitiveComponent>(component); primitiveComponent != nullptr)
        {
            primitiveComponent->SetSimulatePhysics(enabled);
        }
    }
    
} // namespace AtomDestiny::Utils::Details
