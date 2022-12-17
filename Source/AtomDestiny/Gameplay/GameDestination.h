#pragma once

#include "GameDestination.generated.h"

USTRUCT()
struct FGameDestination
{
    GENERATED_BODY(BlueprintType)

    TWeakObjectPtr<AActor> playerDestination;
    TWeakObjectPtr<AActor> enemyDestination;
    TWeakObjectPtr<AActor> playerRespawn;
    TWeakObjectPtr<AActor> enemyRespawn;
    TWeakObjectPtr<AActor> neutralDestination;
};
