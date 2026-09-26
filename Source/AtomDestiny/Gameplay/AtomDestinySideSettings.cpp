#include "AtomDestinySideSettings.h"

FName UAtomDestinySideSettings::GetCategoryName() const
{
    return FName(TEXT("Game"));
}

#if WITH_EDITOR
FText UAtomDestinySideSettings::GetSectionText() const
{
    return NSLOCTEXT("AtomDestiny", "ConflictSidesSection", "Conflict Sides");
}

FText UAtomDestinySideSettings::GetSectionDescription() const
{
    return NSLOCTEXT("AtomDestiny", "ConflictSidesSectionDesc",
        "Team colors, display names and base economy values for each conflict side.");
}
#endif
