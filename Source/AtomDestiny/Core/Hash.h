#pragma once

#include "HashException.h"

#include <Core/Public/Templates/SharedPointer.h>

//
// Possibility to work std::unordered_map with TSharedPtr<T>, TWeakPtr and similar
// TStrongObjectPtr, TWeakObjectPtr entities
//

template <typename T>
struct std::hash<TSharedPtr<T>>
{
    [[nodiscard]] size_t operator()(const TSharedPtr<T>& ptr) const noexcept
    {
        return hash<typename TSharedPtr<T>::ElementType*>()(ptr.Get());
    }
};

template <typename T>
struct std::hash<TWeakPtr<T>>
{
    [[nodiscard]] size_t operator()(const TWeakPtr<T>& ptr) const noexcept
    {
        if (!ptr.IsValid())
            throw AtomDestiny::HashException("Bad weak ptr at std::hash");
        
        const TSharedPtr<T> pinnedPtr = ptr.Pin();
        return hash<typename TWeakPtr<T>::ElementType*>()(pinnedPtr.Get());
    }
};

template <typename T>
struct std::hash<TStrongObjectPtr<T>>
{
    [[nodiscard]] size_t operator()(const TStrongObjectPtr<T>& ptr) const noexcept
    {
        return hash<T*>()(ptr.Get());
    }
};

template <typename T>
struct std::hash<TWeakObjectPtr<T>>
{
    [[nodiscard]] size_t operator()(const TWeakObjectPtr<T>& ptr) const noexcept
    {
        if (!ptr.IsValid())
            throw AtomDestiny::HashException("Bad weak object ptr at std::hash");
        
        return hash<T*>()(ptr.Get());
    }
};
