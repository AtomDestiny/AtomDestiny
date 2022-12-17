#include "AtomDestinyGameModeBase.h"

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
struct AAtomDestinyGameModeBase::GameModeBasePrivateData
{
    // represents active units at overall battle
    std::unordered_map<EGameSide, SharedUnitList> activeUnits;

    // represents 
    std::unordered_map<EGameSide, std::vector<SharedUnitList>> enemies;
};

AAtomDestinyGameModeBase::AAtomDestinyGameModeBase() :
    m_impl(new GameModeBasePrivateData{})
{
    InitializeSides();
    InitializeEnemies();
    
    UUnitLogicBase::unitCreated.AddDynamic(this, &AAtomDestinyGameModeBase::OnUnitCreated);
    UUnitLogicBase::unitDestroyed.AddDynamic(this, &AAtomDestinyGameModeBase::OnUnitDestroyed);
}

AAtomDestinyGameModeBase::~AAtomDestinyGameModeBase()
{
    delete m_impl;
}

void AAtomDestinyGameModeBase::AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    UnitList& unitList = *(m_impl->activeUnits[side]);

    if (const auto iter = std::find(std::begin(unitList), std::end(unitList), actor); iter == std::end(unitList))
        unitList.push_back(std::move(actor));
}

void AAtomDestinyGameModeBase::RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    UnitList& unitList = *(m_impl->activeUnits[side]);

    if (const auto iter = std::find(std::begin(unitList), std::end(unitList), actor); iter != std::end(unitList))
        unitList.erase(iter);
}

void AAtomDestinyGameModeBase::OnUnitCreated(AActor* actor, EGameSide side, EUnitType)
{
    AddUnit(MakeWeakObjectPtr(actor), side);
}

void AAtomDestinyGameModeBase::OnUnitDestroyed(AActor* actor, EGameSide side, EUnitType)
{
    RemoveUnit(MakeWeakObjectPtr(actor), side);
}

void AAtomDestinyGameModeBase::InitializeSides()
{
    constexpr auto lastSide = static_cast<uint8>(EGameSide::None);

    for (uint8 side = 0; side < lastSide; ++side)
    {
        if (const EGameSide currentSide = static_cast<EGameSide>(side); !m_impl->activeUnits.contains(currentSide))
            m_impl->activeUnits.insert(std::make_pair(currentSide, std::make_shared<UnitList>()));
    }
}

void AAtomDestinyGameModeBase::InitializeEnemies()
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
