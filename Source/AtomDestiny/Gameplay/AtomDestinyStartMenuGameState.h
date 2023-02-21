#pragma once

#include <atomic>

#include <AtomDestiny/AtomDestinyGameStateBase.h>
#include <AtomDestiny/Gameplay/UnitStorage.h>

#include "AtomDestinyStartMenuGameState.generated.h"

///
/// Use this class for start menu to add units to unit storage.
/// It initializes UnitStorage instance only once.
/// So check to use it only when game loads at start.
///
UCLASS(Blueprintable)
class AAtomDestinyStartMenuGameState : public AAtomDestinyGameStateBase
{
    GENERATED_BODY()

protected:
    
private:
    inline static std::atomic<bool> Initialized {false};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Units", AllowPrivateAccess = "true"))
    TMap<EUnitType, FUnitInfo> m_units;
};
