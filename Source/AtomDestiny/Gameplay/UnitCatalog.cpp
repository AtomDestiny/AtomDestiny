#include "UnitCatalog.h"

#include "AtomDestinyUnitSettings.h"
#include "UnitStorage.h"

namespace AtomDestiny
{
    void EnsureUnitCatalogLoaded()
    {
        UnitStorage& storage = UnitStorage::Instance();

        const UAtomDestinyUnitSettings* unitSettings = GetDefault<UAtomDestinyUnitSettings>();
        if (unitSettings == nullptr)
        {
            return;
        }

        const TMap<EADUnitType, FUnitInfo>& catalog = unitSettings->GetUnits();
        if (catalog.Num() == 0)
        {
            return;
        }

        if (storage.GetUnits().Num() != catalog.Num())
        {
            storage.Add(TMap<EADUnitType, FUnitInfo>(catalog));
        }
    }
}
