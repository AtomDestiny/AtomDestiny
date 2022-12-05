#pragma once

#include <Core/Public/Templates/SharedPointer.h>

#include <Engine/Classes/GameFramework/Actor.h>

namespace AtomDestiny
{
    using GameObjectType = AActor;
    using GameObject = TSharedPtr<GameObjectType>;
    using GameObjectWeak = TWeakPtr<GameObjectType>;
    
} // namespace AtomDestiny