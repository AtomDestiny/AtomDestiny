#pragma once

#include "GameDestination.generated.h"

USTRUCT(BlueprintType)
struct FGameDestination
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Player destination"))
    TWeakObjectPtr<AActor> playerDestination;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enemy destination"))
    TWeakObjectPtr<AActor> enemyDestination;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Player respawn"))
    TWeakObjectPtr<AActor> playerRespawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enemy respawn"))
    TWeakObjectPtr<AActor> enemyRespawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Neutral destination"))
    TWeakObjectPtr<AActor> neutralDestination;
};
