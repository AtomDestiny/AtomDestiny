#pragma once

#include <Runtime/Engine/Classes/GameFramework/Pawn.h>

#include "UnitInfo.generated.h"

///
/// Should stores some unit info at menu.
/// Add additional fields here, to setup units at maps.
///
USTRUCT(BlueprintType)
struct FUnitInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit prefab"))
    TSubclassOf<APawn> prefab;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit color"))
    FColor color = FColor::Blue;
};
