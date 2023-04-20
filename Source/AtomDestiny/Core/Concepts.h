#pragma once

#include <concepts>

#include <RunTime/Core/Public/Math/MathFwd.h>
#include <Engine/Classes/GameFramework/Actor.h>

namespace AtomDestiny::Concepts
{
    namespace Details
    {
        template <typename T>
        using Type = std::remove_cvref_t<T>;
        
    } // namespace Details

    template<typename T, typename ... U>
    concept IsAnyOf = (std::same_as<Details::Type<T>, U> || ...);

    template <typename T>
    concept IsVector = std::is_same_v<Details::Type<T>, FVector>;

    template <typename T>
    concept IsActor = std::is_same_v<Details::Type<std::remove_pointer<T>>, AActor>;

    template <typename T>
    concept IsActorPointer = std::is_same_v<Details::Type<T>, AActor*>;

    template <typename T>
    concept NavigatorMovable = IsAnyOf<T, FVector, AActor*>;

    template <typename T>
    concept HasNoneValue = std::is_enum_v<T> && requires (T t)
    {
        { T::None };
    };

} // namespace AtomDestiny::Concepts
