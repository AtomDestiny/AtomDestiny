#include "AtomDestinyGameStateBase.h"

#include <unordered_map>
#include <ranges>

// ReSharper disable once CppUnusedIncludeDirective
#include <AtomDestiny/Core/Hash.h>
#include <AtomDestiny/Logic/UnitLogicBase.h>

#include <Engine/Classes/GameFramework/Actor.h>

// Place core data here
struct AAtomDestinyGameStateBase::GameStateBasePrivateData
{
    // represents active units at overall battle
    std::unordered_map<EGameSide, FSharedGameStateUnitList> activeUnits;

    // represents 
    std::unordered_map<EGameSide, FSharedEnemiesList> enemies;
};

AAtomDestinyGameStateBase::AAtomDestinyGameStateBase() :
    m_impl(new GameStateBasePrivateData{})
{
    InitializeSides();
    InitializeEnemies();
    
    UUnitLogicBase::unitCreated.AddDynamic(this, &AAtomDestinyGameStateBase::OnUnitCreated);
    UUnitLogicBase::unitDestroyed.AddDynamic(this, &AAtomDestinyGameStateBase::OnUnitDestroyed);
}

AAtomDestinyGameStateBase::~AAtomDestinyGameStateBase()
{
    delete m_impl;
}

void AAtomDestinyGameStateBase::AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    FGameStateUnitList& unitList = *(m_impl->activeUnits[side]);

    if (const auto index = unitList.Find(actor); index == INDEX_NONE)
        unitList.Add(std::move(actor));
}

void AAtomDestinyGameStateBase::RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    FGameStateUnitList& unitList = *(m_impl->activeUnits[side]);

    if (const auto index = unitList.Find(actor); index == INDEX_NONE)
        unitList.RemoveAt(index);
}

TWeakObjectPtr<AActor> AAtomDestinyGameStateBase::GetDestination(EGameSide side) const
{
    switch (side)
    {
    case EGameSide::Rebels:
        return m_destination.playerDestination;

    case EGameSide::Federation:
        return m_destination.enemyDestination;

    case EGameSide::Neutral:
        return m_destination.neutralDestination;

    default:
        return nullptr;
    }
}

FSharedEnemiesList AAtomDestinyGameStateBase::GetEnemies(EGameSide side) const
{
    return m_impl->enemies[side];
}

void AAtomDestinyGameStateBase::OnUnitCreated(AActor* actor, EGameSide side, EUnitType)
{
    AddUnit(MakeWeakObjectPtr(actor), side);
}

void AAtomDestinyGameStateBase::OnUnitDestroyed(AActor* actor, EGameSide side, EUnitType)
{
    RemoveUnit(MakeWeakObjectPtr(actor), side);
}

void AAtomDestinyGameStateBase::InitializeSides()
{
    constexpr auto lastSide = static_cast<uint8>(EGameSide::None);

    for (uint8 side = 0; side < lastSide; ++side)
    {
        if (const EGameSide currentSide = static_cast<EGameSide>(side); !m_impl->activeUnits.contains(currentSide))
            m_impl->activeUnits.insert(std::make_pair(currentSide, MakeShared<FGameStateUnitList>()));
    }
}

void AAtomDestinyGameStateBase::InitializeEnemies()
{
    for (const EGameSide side : m_impl->activeUnits | std::views::keys)
    {
        m_impl->enemies.insert(std::make_pair(side, FSharedEnemiesList{}));

        for (const EGameSide s : m_impl->activeUnits | std::views::keys)
        {
            if (side != s && side != EGameSide::None)
                m_impl->enemies[side]->Add(m_impl->activeUnits[s]);
        }
    }
}
