#include "AtomDestinyGameModeBase.h"

#include <vector>
#include <unordered_map>

// ReSharper disable once CppUnusedIncludeDirective
#include <AtomDestiny/Core/Hash.h>
#include <AtomDestiny/Logic/UnitLogicBase.h>

#include <Engine/Classes/GameFramework/Actor.h>

//
// We should use vector instead of hash because units would mostly iterate through this list.
// Search, addition and removing is not a super frequency operation.
//
using UnitList = std::vector<TWeakObjectPtr<AActor>>;

// Place core data here
struct AAtomDestinyGameModeBase::GameModeBasePrivateData
{
    // represents active units at overall battle
    std::unordered_map<EGameSide, UnitList> activeUnits;
};

AAtomDestinyGameModeBase::AAtomDestinyGameModeBase() :
    m_impl(new GameModeBasePrivateData{})
{
    // UUnitLogicBase::unitCreated.Add(BindDynamic(this, &Core::OnUnitCreated));
}

AAtomDestinyGameModeBase::~AAtomDestinyGameModeBase()
{
    delete m_impl;
}

void AAtomDestinyGameModeBase::AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    UnitList& unitList = m_impl->activeUnits[side];

    if (const auto iter = std::find(std::begin(unitList), std::end(unitList), actor); iter == std::end(unitList))
        unitList.push_back(std::move(actor));
}

void AAtomDestinyGameModeBase::RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    UnitList& unitList = m_impl->activeUnits[side];

    if (const auto iter = std::find(std::begin(unitList), std::end(unitList), actor); iter != std::end(unitList))
        unitList.erase(iter);
}

void AAtomDestinyGameModeBase::OnUnitCreated(AActor* actor, EGameSide side, EUnitType)
{
    AddUnit(TWeakObjectPtr<AActor>{ actor }, side);
}

void AAtomDestinyGameModeBase::OnUnitDestroyed(AActor* actor, EGameSide side, EUnitType)
{
    RemoveUnit(TWeakObjectPtr<AActor>{ actor }, side);
}
