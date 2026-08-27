#include "AtomDestinyGameInstance.h"

#include "AtomDestiny/Gameplay/AtomDestinySideSettings.h"
#include "AtomDestiny/Gameplay/AtomDestinyUnitSettings.h"
#include "AtomDestiny/Gameplay/SideStorage.h"
#include "AtomDestiny/Gameplay/UnitStorage.h"

#include <utility>

void UAtomDestinyGameInstance::Init()
{
    Super::Init();

    if (const UAtomDestinyUnitSettings* unitSettings = GetDefault<UAtomDestinyUnitSettings>();
        unitSettings != nullptr && unitSettings->GetUnits().Num() > 0)
    {
        TMap<EADUnitType, FUnitInfo> units = unitSettings->GetUnits();
        AtomDestiny::UnitStorage::Instance().Add(std::move(units));
    }

    if (const UAtomDestinySideSettings* sideSettings = GetDefault<UAtomDestinySideSettings>();
        sideSettings != nullptr && sideSettings->GetSides().Num() > 0)
    {
        TMap<EGameSide, FSideDefinition> sides = sideSettings->GetSides();
        AtomDestiny::SideStorage::Instance().Add(std::move(sides));
    }
}
