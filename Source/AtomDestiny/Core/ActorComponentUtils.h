#pragma once

#include <assert.h>
#include <type_traits>

#include "AtomDestinyCore.h"

#include <Engine/Classes/GameFramework/Actor.h>

namespace AtomDestiny::Utils
{
    template<typename Component>
    using IsActorComponent = std::is_base_of<UActorComponent, Component>;

    ///
    /// Adds new component to actor as new instance
    /// 
    template<typename Component>
    [[maybe_unused]] Component* AddNewComponentToActor(AActor* actor)
    {
        static_assert(IsActorComponent<Component>::value, "Component parameter is not an UActorComponent");
        
        assert(actor != nullptr);

        const auto newComponent = NewObject<Component>(actor);
        newComponent->RegisterComponent();
        
        actor->AddInstanceComponent(newComponent);

        return newComponent;
    }

    template<typename Component>
    [[maybe_unused]] Component* AddNewComponentToActor(const TStrongObjectPtr<AActor>& object)
    {
        return AddNewComponentToActor<Component>(object.Get());
    }
    
    template<typename Interface, typename UEInterface>
    [[nodiscard]] Interface* GetInterface(AActor* actor)
    {
        static_assert(std::is_base_of_v<UInterface, UEInterface>, "Component parameter is not an UInterface");

        const auto interface = actor->FindComponentByInterface<UEInterface>();

        if (interface != nullptr)
            return Cast<Interface>(interface);

        return nullptr;
    }
    
} // namespace AtomDestiny::Utils
