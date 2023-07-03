#pragma once

#include "UnitInfo.h"

#include <AtomDestiny/Unit/Unit.h>
#include <AtomDestiny/Core/Concepts.h>

#include <Runtime/Core/Public/Containers/Map.h>
#include <RunTime/Core/Public/Containers/Array.h>

namespace AtomDestiny
{
    template <typename T>
    concept UnitStorageMap = Concepts::IsAnyOf<T, TMap<EUnitType, FUnitInfo>>;
    
    ///
    /// Stores pointers to all units and additional information.
    /// By default storage is empty, so user should create all references manually.
    /// For example, you can do that at start of the game one time only.
    ///
    class UnitStorage
    {
    public:
        static UnitStorage& Instance();

        template <typename Value>
        void Add(EUnitType type, Value&& value);

        template <typename Values>
        void Add(Values&& values) requires UnitStorageMap<Values>;

        bool Contains(const EUnitType type) const;
        TOptional<FUnitInfo> GetInfo(const EUnitType type) const;

        TArray<EUnitType> GetUnits() const; // copy specially, you should not use this method frequently
        
    private:
        TMap<EUnitType, FUnitInfo> m_storage;
    };

    //////////////////////////////////////////

    inline UnitStorage& UnitStorage::Instance()
    {
        static UnitStorage storage;
        return storage;
    }

    template <typename Value>
    void UnitStorage::Add(EUnitType type, Value&& value)
    {
        static_assert(std::is_same_v<std::remove_cvref_t<Value>, FUnitInfo>, "Value should be only FUnitInfo");
        m_storage.Add(type, std::forward<Value>(value));
    }

    template <typename Values>
    void UnitStorage::Add(Values&& values) requires UnitStorageMap<Values>
    {
        m_storage = std::forward<Values>(values);
    }
    
    inline bool UnitStorage::Contains(const EUnitType type) const
    {
        return m_storage.Contains(type);
    }

    inline TOptional<FUnitInfo> UnitStorage::GetInfo(const EUnitType type) const
    {
        if (const auto info = m_storage.Find(type); info != nullptr)
        {
            return TOptional{*info};
        }

        return NullOpt;
    }

    inline TArray<EUnitType> UnitStorage::GetUnits() const
    {
        TArray<EUnitType> units;
        units.Reserve(m_storage.Num());
        
        for ([[maybe_unused]] const auto& [unit, info] : m_storage)
        {
            units.Add(unit);
        }
        
        return units;
    }
    
} // namespace AtomDestiny
