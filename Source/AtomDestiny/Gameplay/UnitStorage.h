#pragma once

#include "UnitInfo.h"

#include <AtomDestiny/Unit/Unit.h>

#include <Runtime/Core/Public/Containers/Map.h>

namespace AtomDestiny
{
    ///
    /// Stores pointers to all units and additional information.
    /// By default storage is empty, so user should create all references manually.
    /// For example, you can do that at start of the game one time only.
    ///
    class UnitStorage
    {
    public:
        static UnitStorage& Instance()
        {
            static UnitStorage storage;
            return storage;
        }

        template<typename Value>
        void Add(EUnitType type, Value&& value)
        {
            static_assert(std::is_same_v<std::remove_cvref_t<Value>, FUnitInfo>, "Value should be only FUnitInfo");
            m_storage.Add(type, std::forward<Value>(value));
        }

        bool Contains(const EUnitType type) const
        {
            return m_storage.Contains(type);
        }

        TOptional<FUnitInfo> GetUnit(const EUnitType type) const
        {
            if (const auto info = m_storage.Find(type); info != nullptr)
            {
                return TOptional{*info};
            }

            return NullOpt;
        }
        
    private:
        TMap<EUnitType, FUnitInfo> m_storage;
    };
    
} // namespace AtomDestiny
