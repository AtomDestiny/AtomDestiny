#include "AtomDestinyCore.h"

#include <vector>
#include <unordered_map>

// ReSharper disable once CppUnusedIncludeDirective
#include <AtomDestiny/Core/Hash.h>
#include <AtomDestiny/Unit/UnitLogicBase.h>

#include <Engine/Classes/GameFramework/Actor.h>

//
// We should use vector instead of hash because units would mostly iterate through this list.
// Search, addition and removing is not a super frequency operation.
//
using UnitList = std::vector<TWeakObjectPtr<AActor>>;

// Place core data here
struct AtomDestiny::Core::CoreData
{
    // represents active units at overall battle
    std::unordered_map<EGameSide, UnitList> activeUnits;
};

AtomDestiny::Core::Core():
    m_impl(std::make_unique<CoreData>())
{
    // UUnitLogicBase::unitCreated.Add(BindDynamic(this, &Core::OnUnitCreated));
}

AtomDestiny::Core::~Core() = default;

AtomDestiny::Core& AtomDestiny::Core::Instance()
{
    static Core core;
    return core;
}

void AtomDestiny::Core::AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    UnitList& unitList = m_impl->activeUnits[side];

    if (const auto iter = std::find(std::begin(unitList), std::end(unitList), actor.Get()); iter == std::end(unitList))
        unitList.push_back(std::move(actor));
}

void AtomDestiny::Core::RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    UnitList& unitList = m_impl->activeUnits[side];

    if (const auto iter = std::find(std::begin(unitList), std::end(unitList), actor.Get()); iter != std::end(unitList))
        unitList.erase(iter);
}

void AtomDestiny::Core::OnUnitCreated(const TWeakObjectPtr<AActor>& actor, EGameSide side, EUnitType)
{
    AddUnit(actor, side);
}

void AtomDestiny::Core::OnUnitDestroyed(const TWeakObjectPtr<AActor>& actor, EGameSide side, EUnitType)
{
    RemoveUnit(actor, side);
}
