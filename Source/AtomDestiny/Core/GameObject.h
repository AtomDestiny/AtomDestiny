#pragma once

#include <Engine/Classes/GameFramework/Actor.h>

namespace AtomDestiny
{
    using GameObjectType = AActor;
    using GameObject = TStrongObjectPtr<GameObjectType>;
    using GameObjectWeak = TWeakObjectPtr<GameObjectType>;
    
} // namespace AtomDestiny