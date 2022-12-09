#pragma once

#include <type_traits>

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
        
        check(actor != nullptr);

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
        check(actor != nullptr);
        
        static_assert(std::is_base_of_v<UInterface, UEInterface>, "Component parameter is not an UInterface");
        
        if (const auto interface = actor->FindComponentByInterface<UEInterface>(); interface != nullptr)
            return Cast<Interface>(interface);

        return nullptr;
    }
    
} // namespace AtomDestiny::Utils

// use name without I and U prefix
#define GET_AD_INTERFACE(name, actor) AtomDestiny::Utils::GetInterface<I##name, U##name>(actor);
