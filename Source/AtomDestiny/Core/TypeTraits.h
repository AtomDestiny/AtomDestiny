#pragma once

#include <type_traits>

#include <RunTime/Core/Public/Math/MathFwd.h>
#include <Engine/Classes/GameFramework/Actor.h>

namespace AtomDestiny::Traits
{
    template <typename T>
    constexpr static auto IsVector = std::is_same_v<std::remove_cvref_t<T>, FVector>;

    template <typename T>
    constexpr static auto IsActor = std::is_same_v<std::remove_cvref_t<std::remove_pointer<T>>, AActor>;

    template <typename T>
    constexpr static auto IsActorPointer = std::is_same_v<std::remove_cvref_t<T>, AActor*>;

} // namespace AtomDestiny::Traits
