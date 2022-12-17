#pragma once

#include "GameDestination.generated.h"

USTRUCT(BlueprintType)
struct FGameDestination
{
    GENERATED_BODY()

    TWeakObjectPtr<AActor> playerDestination;
    TWeakObjectPtr<AActor> enemyDestination;
    TWeakObjectPtr<AActor> playerRespawn;
    TWeakObjectPtr<AActor> enemyRespawn;
    TWeakObjectPtr<AActor> neutralDestination;
};
