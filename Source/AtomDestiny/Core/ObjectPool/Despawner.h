﻿#pragma once

#include <Components/ActorComponent.h>

#include "Despawner.generated.h"

///
/// Actor despawner by timer.
/// Actor would be des-pawned to Actor's Pool
///
UCLASS(Blueprintable)
class ATOMDESTINY_API UDespawner final : public UActorComponent
{
    GENERATED_BODY()

public:
    void Despawn(double time);

private:
    FTimerHandle m_timerHandle;
};
