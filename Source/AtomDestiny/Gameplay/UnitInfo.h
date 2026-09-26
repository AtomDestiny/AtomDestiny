#pragma once

#include <Runtime/Engine/Classes/GameFramework/Pawn.h>
#include <Runtime/Engine/Classes/Engine/Texture2D.h>

#include "UnitInfo.generated.h"

///
/// Catalog data for a unit type. Filled in Project Settings (Unit Catalog)
/// or on the start menu GameState, then used by unit cards and spawners.
///
USTRUCT(BlueprintType)
struct FUnitInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit name"))
    FText name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit icon"))
    TSoftObjectPtr<UTexture2D> icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Unit prefab"))
    TSubclassOf<APawn> prefab;

    /** Optional UI/card accent color. Does not affect battlefield team tint. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Card accent color"))
    FColor color = FColor::Blue;
};
