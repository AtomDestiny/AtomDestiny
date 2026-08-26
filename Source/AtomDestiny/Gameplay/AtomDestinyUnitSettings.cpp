#include "AtomDestinyUnitSettings.h"

FName UAtomDestinyUnitSettings::GetCategoryName() const
{
    return FName(TEXT("Game"));
}

#if WITH_EDITOR
FText UAtomDestinyUnitSettings::GetSectionText() const
{
    return NSLOCTEXT("AtomDestiny", "UnitCatalogSection", "Unit Catalog");
}

FText UAtomDestinyUnitSettings::GetSectionDescription() const
{
    return NSLOCTEXT("AtomDestiny", "UnitCatalogSectionDesc",
        "Display names, prefabs and colors for unit cards. Applied at game startup.");
}
#endif
