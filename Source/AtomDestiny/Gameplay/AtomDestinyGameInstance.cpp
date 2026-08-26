#include "AtomDestinyGameInstance.h"

#include "AtomDestiny/Gameplay/AtomDestinyUnitSettings.h"
#include "AtomDestiny/Gameplay/UnitStorage.h"

#include <utility>

void UAtomDestinyGameInstance::Init()
{
    Super::Init();

    const UAtomDestinyUnitSettings* settings = GetDefault<UAtomDestinyUnitSettings>();
    if (settings == nullptr || settings->GetUnits().Num() == 0)
    {
        return;
    }

    TMap<EADUnitType, FUnitInfo> units = settings->GetUnits();
    AtomDestiny::UnitStorage::Instance().Add(std::move(units));
}
