#pragma once

#include "AtomDestiny/Core/AtomDestinyCore.h"

//
// Make work std::unordered_map with GameObject entity
//
template <>
struct std::hash<AtomDestiny::GameObject>
{
    [[nodiscard]] size_t operator()(const AtomDestiny::GameObject& object) const noexcept
    {
        return hash<AtomDestiny::GameObject::ElementType*>()(object.Get());
    }
};
