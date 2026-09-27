#pragma once

#include <mutex>

#include <AtomDestiny/AtomDestinyGameStateBase.h>
#include <AtomDestiny/Gameplay/UnitStorage.h>

#include "AtomDestinyStartMenuGameState.generated.h"

///
/// Optional start-menu override for the unit catalog.
/// Prefer Project Settings -> Game -> Unit Catalog for names/prefabs.
/// If this map is filled, it replaces UnitStorage once when the menu loads.
///
UCLASS(Blueprintable)
class ATOMDESTINY_API AAtomDestinyStartMenuGameState : public AAtomDestinyGameStateBase
{
    GENERATED_BODY()

protected:
    virtual void PreInitializeComponents() override;
    
private:
    inline static std::once_flag Initialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit prefabs", AllowPrivateAccess = "true"))
    TMap<EADUnitType, FUnitInfo> m_units;
};
