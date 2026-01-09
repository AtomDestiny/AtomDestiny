#pragma once

#include <type_traits>

#include <GameFramework/Actor.h>

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
    [[maybe_unused]] Component* AddNewComponentToActor(const TWeakObjectPtr<AActor>& object)
    {
        return AddNewComponentToActor<Component>(object.Get());
    }

    // Searches component by Name
    [[nodiscard]] inline UActorComponent* FindComponentByName(const AActor* actor, const char* name)
    {
        for (UActorComponent* const component : actor->GetComponents())
        {
            if (component->GetName() == name)
            {
                return component;
            }
        }

        return nullptr;
    }

    template <typename Component>
    TWeakObjectPtr<Component> GetComponentFromReference(const FComponentReference& componentReference, AActor* actor)
    {
        UActorComponent* component = componentReference.GetComponent(actor);

        if (component == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Can not get component from FComponentReference"));
            return {};
        }

        return MakeWeakObjectPtr(CastChecked<Component>(component));
    }

    template <typename Component>
    TArray<Component*> GetComponents(const AActor* actor)
    {
        TArray<Component*> components;
        actor->GetComponents<Component>(components);

        return components;
    }

    template<typename Interface>
    TScriptInterface<Interface> CreateInterface(UObject* object, Interface* interface)
    {
        TScriptInterface<Interface> script;
        script.SetObject(object);
        script.SetInterface(interface);

        return script;
    }

    template<typename Interface, typename UEInterface>
    TScriptInterface<Interface> CreateInterface(UEInterface* object)
    {
        return CreateInterface(object, Cast<Interface>(object));
    }

    template<typename Interface>
    [[nodiscard]] TScriptInterface<Interface> GetInterface(AActor* actor)
    {
        if (actor == nullptr)
        {
            return nullptr;
        }

        if (const auto interface = actor->FindComponentByInterface<Interface>(); interface != nullptr)
            return CreateInterface<Interface>(actor, interface);

        return nullptr;
    }

    template<typename Interface>
    [[nodiscard]] TScriptInterface<Interface> GetInterface(const TWeakObjectPtr<AActor>& actor)
    {
        return GetInterface<Interface>(actor.Get());
    }

    template<typename Interface>
    [[nodiscard]] TArray<TScriptInterface<Interface>> GetInterfaces(AActor* actor)
    {
        if (actor == nullptr)
        {
            return {};
        }

        TArray<TScriptInterface<Interface>> interfaces;

        for (UActorComponent* component : actor->GetComponents())
        {
            if (component && Cast<Interface>(component) != nullptr)
            {
                interfaces.Add(CreateInterface<Interface>(component));
            }
        }

        return interfaces;
    }

    template<typename Interface>
    [[nodiscard]] TArray<TScriptInterface<Interface>> GetInterfaces(const TWeakObjectPtr<AActor>& actor)
    {
        return GetInterfaces<Interface>(actor.Get());
    }

} // namespace AtomDestiny::Utils
