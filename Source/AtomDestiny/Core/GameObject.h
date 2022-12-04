#pragma once

#include <Core/Public/Templates/SharedPointer.h>

#include <Engine/Classes/GameFramework/Actor.h>

namespace AtomDestiny
{
    using GameObject = TSharedPtr<AActor>;
    using GameObjectWeak = TWeakPtr<AActor>;
    
} // namespace AtomDestiny