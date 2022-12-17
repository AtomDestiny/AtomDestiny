#include "AtomDestinyGameStateBase.h"

#include <vector>
#include <unordered_map>

// ReSharper disable once CppUnusedIncludeDirective
#include <ranges>
#include <AtomDestiny/Core/Hash.h>
#include <AtomDestiny/Logic/UnitLogicBase.h>

#include <Engine/Classes/GameFramework/Actor.h>

//
// We should use vector instead of hash because units would mostly iterate through this list.
// Search, addition and removing is not a super frequency operation.
//
using UnitList = std::vector<TWeakObjectPtr<AActor>>;
using SharedUnitList = std::shared_ptr<UnitList>;

// Place core data here
struct AAtomDestinyGameStateBase::GameStateBasePrivateData
{
    // represents active units at overall battle
    std::unordered_map<EGameSide, SharedUnitList> activeUnits;

    // represents 
    std::unordered_map<EGameSide, std::vector<SharedUnitList>> enemies;
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
    UnitList& unitList = *(m_impl->activeUnits[side]);

    if (const auto iter = std::find(std::begin(unitList), std::end(unitList), actor); iter == std::end(unitList))
        unitList.push_back(std::move(actor));
}

void AAtomDestinyGameStateBase::RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    UnitList& unitList = *(m_impl->activeUnits[side]);

    if (const auto iter = std::find(std::begin(unitList), std::end(unitList), actor); iter != std::end(unitList))
        unitList.erase(iter);
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
            m_impl->activeUnits.insert(std::make_pair(currentSide, std::make_shared<UnitList>()));
    }
}

void AAtomDestinyGameStateBase::InitializeEnemies()
{
    for (const EGameSide side : m_impl->activeUnits | std::views::keys)
    {
        m_impl->enemies.insert(std::make_pair(side, std::vector<SharedUnitList>{}));

        for (const EGameSide s : m_impl->activeUnits | std::views::keys)
        {
            if (side != s && side != EGameSide::None)
                m_impl->enemies[side].push_back(m_impl->activeUnits[s]);
        }
    }
}
