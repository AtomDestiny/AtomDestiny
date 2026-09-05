#pragma once

#include <AtomDestiny/AtomDestiny.h>

#include <Runtime/Engine/Classes/Engine/Texture2D.h>

#include "SideDefinition.generated.h"

///
/// Static profile of a conflict side (Project Settings -> Conflict Sides).
///
USTRUCT(BlueprintType)
struct FSideDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Display name"))
    FText displayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Team color"))
    FLinearColor teamColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Icon"))
    TSoftObjectPtr<UTexture2D> icon;

    /** Base cash income per tick (applied by game mode when economy is enabled). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Base cash income"))
    float baseCashIncome = 0.f;

    /** Base energy flow per tick (applied by game mode when economy is enabled). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Base energy flow"))
    float baseEnergyFlow = 0.f;
};

///
/// Per-match runtime resources for a side (stored on GameState).
///
USTRUCT(BlueprintType)
struct FSideRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Cash"))
    float cash = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Energy"))
    float energy = 0.f;
};
