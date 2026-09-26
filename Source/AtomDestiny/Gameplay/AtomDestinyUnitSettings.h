#pragma once

#include "Engine/DeveloperSettings.h"
#include "AtomDestiny/Gameplay/UnitInfo.h"
#include "AtomDestiny/Unit/Unit.h"

#include "AtomDestinyUnitSettings.generated.h"

///
/// Startup catalog of units shown on training/setup cards.
/// Fill names (e.g. "Buggy", "Tank", "Mech"), prefabs and colors here:
/// Project Settings -> Game -> Unit Catalog.
///
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Unit Catalog"))
class ATOMDESTINY_API UAtomDestinyUnitSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;

#if WITH_EDITOR
    virtual FText GetSectionText() const override;
    virtual FText GetSectionDescription() const override;
#endif

    const TMap<EADUnitType, FUnitInfo>& GetUnits() const { return m_units; }

private:
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Units", AllowPrivateAccess = "true"))
    TMap<EADUnitType, FUnitInfo> m_units;
};
