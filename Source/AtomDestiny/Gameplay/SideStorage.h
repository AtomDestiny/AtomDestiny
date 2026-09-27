#pragma once

#include <AtomDestiny/Gameplay/SideDefinition.h>
#include <AtomDestiny/Core/Concepts.h>

#include <Runtime/Core/Public/Containers/Map.h>

namespace AtomDestiny
{
    template <typename T>
    concept SideStorageMap = Concepts::IsAnyOf<T, TMap<EGameSide, FSideDefinition>>;

    ///
    /// Runtime lookup for conflict side definitions loaded at game startup.
    ///
    class SideStorage
    {
    public:
        static SideStorage& Instance();

        template <typename Value>
        void Add(EGameSide side, Value&& value);

        template <typename Values>
        void Add(Values&& values) requires SideStorageMap<Values>;

        bool Contains(EGameSide side) const;
        TOptional<FSideDefinition> GetDefinition(EGameSide side) const;
        FText GetDisplayName(EGameSide side) const;
        FLinearColor GetTeamColor(EGameSide side) const;

    private:
        TMap<EGameSide, FSideDefinition> m_storage;
    };

    inline SideStorage& SideStorage::Instance()
    {
        static SideStorage storage;
        return storage;
    }

    template <typename Value>
    void SideStorage::Add(EGameSide side, Value&& value)
    {
        static_assert(std::is_same_v<std::remove_cvref_t<Value>, FSideDefinition>, "Value should be only FSideDefinition");
        m_storage.Add(side, std::forward<Value>(value));
    }

    template <typename Values>
    void SideStorage::Add(Values&& values) requires SideStorageMap<Values>
    {
        m_storage = std::forward<Values>(values);
    }

    inline bool SideStorage::Contains(const EGameSide side) const
    {
        return m_storage.Contains(side);
    }

    inline TOptional<FSideDefinition> SideStorage::GetDefinition(const EGameSide side) const
    {
        if (const FSideDefinition* definition = m_storage.Find(side))
        {
            return TOptional{*definition};
        }

        return NullOpt;
    }

    inline FText SideStorage::GetDisplayName(const EGameSide side) const
    {
        if (const TOptional<FSideDefinition> definition = GetDefinition(side); definition.IsSet() && !definition->displayName.IsEmpty())
        {
            return definition->displayName;
        }

        return FText::FromString(GameSide::ToString(side));
    }

    inline FLinearColor SideStorage::GetTeamColor(const EGameSide side) const
    {
        if (const TOptional<FSideDefinition> definition = GetDefinition(side))
        {
            return definition->teamColor;
        }

        return FLinearColor::White;
    }

} // namespace AtomDestiny
