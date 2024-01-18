#pragma once

#include "UnitInfo.h"

#include <AtomDestiny/Unit/Unit.h>
#include <AtomDestiny/Core/Concepts.h>

#include <Runtime/Core/Public/Containers/Map.h>
#include <RunTime/Core/Public/Containers/Array.h>

namespace AtomDestiny
{
    template <typename T>
    concept UnitStorageMap = Concepts::IsAnyOf<T, TMap<EADUnitType, FUnitInfo>>;
    
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
        void Add(EADUnitType type, Value&& value);

        template <typename Values>
        void Add(Values&& values) requires UnitStorageMap<Values>;

        bool Contains(const EADUnitType type) const;
        TOptional<FUnitInfo> GetInfo(const EADUnitType type) const;

        TArray<EADUnitType> GetUnits() const; // copy specially, you should not use this method frequently
        
    private:
        TMap<EADUnitType, FUnitInfo> m_storage;
    };

    //////////////////////////////////////////

    inline UnitStorage& UnitStorage::Instance()
    {
        static UnitStorage storage;
        return storage;
    }

    template <typename Value>
    void UnitStorage::Add(EADUnitType type, Value&& value)
    {
        static_assert(std::is_same_v<std::remove_cvref_t<Value>, FUnitInfo>, "Value should be only FUnitInfo");
        m_storage.Add(type, std::forward<Value>(value));
    }

    template <typename Values>
    void UnitStorage::Add(Values&& values) requires UnitStorageMap<Values>
    {
        m_storage = std::forward<Values>(values);
    }
    
    inline bool UnitStorage::Contains(const EADUnitType type) const
    {
        return m_storage.Contains(type);
    }

    inline TOptional<FUnitInfo> UnitStorage::GetInfo(const EADUnitType type) const
    {
        if (const auto info = m_storage.Find(type); info != nullptr)
        {
            return TOptional{*info};
        }

        return NullOpt;
    }

    inline TArray<EADUnitType> UnitStorage::GetUnits() const
    {
        TArray<EADUnitType> units;
        units.Reserve(m_storage.Num());
        
        for ([[maybe_unused]] const auto& [unit, info] : m_storage)
        {
            units.Add(unit);
        }
        
        return units;
    }
    
} // namespace AtomDestiny
