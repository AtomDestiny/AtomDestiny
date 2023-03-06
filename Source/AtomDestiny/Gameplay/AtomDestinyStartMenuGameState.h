#pragma once

#include <mutex>

#include <AtomDestiny/AtomDestinyGameStateBase.h>
#include <AtomDestiny/Gameplay/UnitStorage.h>

#include "AtomDestinyStartMenuGameState.generated.h"

///
/// Use this class for start menu to add units to unit storage.
/// It initializes UnitStorage instance only once.
/// So check to use it only when game loads at start.
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
    TMap<EUnitType, FUnitInfo> m_units;
};
