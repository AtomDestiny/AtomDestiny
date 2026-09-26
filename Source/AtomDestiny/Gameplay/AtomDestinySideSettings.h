#pragma once

#include "Engine/DeveloperSettings.h"

#include <AtomDestiny/Gameplay/SideDefinition.h>

#include "AtomDestinySideSettings.generated.h"

///
/// Startup catalog of conflict sides.
/// Project Settings -> Game -> Conflict Sides.
///
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Conflict Sides"))
class ATOMDESTINY_API UAtomDestinySideSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;

#if WITH_EDITOR
    virtual FText GetSectionText() const override;
    virtual FText GetSectionDescription() const override;
#endif

    const TMap<EGameSide, FSideDefinition>& GetSides() const { return m_sides; }

private:
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Sides", AllowPrivateAccess = "true"))
    TMap<EGameSide, FSideDefinition> m_sides;
};
