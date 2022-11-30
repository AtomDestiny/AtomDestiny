#pragma once

#include <Core/Public/Templates/SharedPointer.h>

//
// Possibility to work std::unordered_map with TSharedPtr<T> entity
//
template <typename T>
struct std::hash<TSharedPtr<T>>
{
    [[nodiscard]] size_t operator()(const TSharedPtr<T>& ptr) const noexcept
    {
        return hash<typename TSharedPtr<T>::ElementType*>()(ptr.Get());
    }
};
